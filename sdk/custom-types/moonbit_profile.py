"""Explicit optional MoonBit complete-record emitter over the actual bounded IR.

This standalone producer never invokes or changes the original six emitters,
managed_profile.py or managed_csharp.py. No ROS library, native registry, Host
permission, compiler, shell or Guest is created here. SOURCE_ONLY_UNEXECUTED.
"""
from __future__ import annotations

import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys

OUTPUTS = {'moonbit/bounded_records.mbt', 'moonbit/moon.pkg'}
MAX_OUTPUT_BYTES = 64 * 1024 * 1024
SCALARS = {
    'bool': ('Bool', 1),
    'uint8': ('Int', 1), 'int8': ('Int', 1),
    'uint16': ('Int', 2), 'int16': ('Int', 2),
    'uint32': ('UInt', 4), 'int32': ('Int', 4),
    'uint64': ('UInt64', 8), 'int64': ('Int64', 8),
    'float32': ('Float', 4), 'float64': ('Double', 8)}
# Actual MoonBit lexical categories: lident fields, uident types. Conservative
# keyword/reserved refusals preserve original native names instead of renaming.
RESERVED = set(('as else extern fn if let const match using mut type struct enum extenum trait derive '
    'while break continue import return throw raise try catch pub priv proof_assert proof_let readonly '
    'true false test loop for in impl with guard async is suberror and letrec enumview noraise nocancel '
    'defer lexscan lexmatch where declare nobreak extend module move ref static super unsafe use await '
    'dyn abstract do final macro override typeof virtual yield local method alias assert package recur '
    'isnot define downcast inherit member namespace upcast void lazy include mixin protected sealed '
    'constructor atomic volatile anyframe anytype asm comptime errdefer export opaque orelse resume '
    'threadlocal unreachable dynclass dynobj dynrec var finally noasync assume').split())

# Source-language spelling only. These three exact original CONTROL fields are
# mandatory, complete native members named `type`, a real MoonBit keyword. Never
# rename their Native/IR/schema/wire identity or drop them. No generic keyword
# escape or automatic fallback is admitted; any resulting local collision fails.
CONTROL_MEMBER_SPELLINGS = {
    (2, 'rcl_interfaces/msg/ParameterDescriptor', 'type'): 'type_value',
    (2, 'rcl_interfaces/msg/ParameterValue', 'type'): 'type_value',
    (2, 'type_description_interfaces/msg/Field', 'type'): 'type_value',
}


def field_name(record, member):
    if record.profile == 'uwvm-ros-original-node-control-c-cpp-v1':
        return CONTROL_MEMBER_SPELLINGS.get((record.version, record.name, member.name), member.name)
    return member.name


def checked_ir(parent, bounded, records, ordered, interfaces):
    if len(ordered) != len(records) or len({record.symbol for record in ordered}) != len(ordered):
        raise parent.GenerationError('actual complete resolved record order required')
    if set(SCALARS) != set(parent.SCALARS):
        raise parent.GenerationError('new authoritative scalar requires explicit MoonBit support')
    public = {'BoundedWriter', 'BoundedReader'}
    for record in ordered:
        if records.get((record.version, record.name)) is not record:
            raise parent.GenerationError('MoonBit emitter requires actual resolved IR objects')
        if (not re.fullmatch(r'Ros[12][A-Za-z0-9_]+', record.symbol) or
                not re.fullmatch(r'UWVMROS1_[0-9a-f]{64}', record.codec_identity) or
                not 0 <= record.minimum <= record.maximum <= bounded.MAX_WIRE):
            raise parent.GenerationError('actual resolved symbol/identity/full bound required')
        names = (record.symbol, record.symbol + 'Codec')
        if not record.members:
            names += (record.symbol + 'Empty',)
        for name in names:
            if name in public:
                raise parent.GenerationError('MoonBit generated type/codec namespace collision')
            public.add(name)
        if bounded.standard_semantics(record) not in ('', 'normalized-time',
                'normalized-duration', 'joint-state-optional-equal-length-arrays'):
            raise parent.GenerationError('unknown complete-value semantic rule')
        if len({field_name(record, member) for member in record.members}) != len(record.members):
            raise parent.GenerationError('explicit MoonBit member spelling collides with an original field')
        for member in record.members:
            if (member.string_encoding not in ('utf8', 'raw_bytes') or
                    (member.string_encoding == 'raw_bytes' and
                     (record.profile != bounded.CONTROL_PROFILE or member.type != 'string'))):
                raise parent.GenerationError('raw byte strings require the actual explicit control IR')
            if (not re.fullmatch(r'[a-z][A-Za-z0-9_]*', field_name(record, member)) or field_name(record, member) in RESERVED):
                raise parent.GenerationError('native field is not an admitted MoonBit lident: ' + member.name)
            if member.type not in SCALARS and member.type != 'string' and (record.version, member.type) not in records:
                raise parent.GenerationError('unknown MoonBit scalar/nested complete record')
            if member.array not in ('', 'array', 'sequence') or (member.array and not 1 <= member.count <= bounded.MAX_NODES):
                raise parent.GenerationError('actual fixed/sequence shape required')
            if member.type == 'string' and not 1 <= member.string_bound <= bounded.MAX_WIRE - 4:
                raise parent.GenerationError('actual bounded UTF8 field required')
            if member.native_default is not None:
                if member.array or member.type not in SCALARS:
                    raise parent.GenerationError('array/string/nested native defaults unsupported')
                default_value(bounded, record, member, records)  # validate every selected default before writes
    if any(role not in ('message', 'request', 'response', 'goal', 'result', 'feedback')
           for interface in interfaces for role in interface.components):
        raise parent.GenerationError('unknown complete interface component role')


def element_type(record, member, records):
    if member.type == 'string' and member.string_encoding == 'raw_bytes':
        return 'Bytes'
    return (SCALARS[member.type][0] if member.type in SCALARS else 'String'
            if member.type == 'string' else records[(record.version, member.type)].symbol)


def element_minimum(record, member, records):
    return (SCALARS[member.type][1] if member.type in SCALARS else 4
            if member.type == 'string' else records[(record.version, member.type)].minimum)


def default_value(bounded, record, member, records):
    if member.native_default is not None:
        # Validate existing normalized IR through its real authoritative helper;
        # do not parse .msg or invent a MoonBit canonical/default identity domain.
        bounded.scalar_default_expression(member.type, member.native_default, 'python')
        text = member.native_default
        if member.type == 'bool':
            return text.lower()
        if member.type.startswith(('uint', 'int')):
            if text == '-9223372036854775808':
                return '(-9223372036854775807L - 1L)'
            if member.type == 'int32' and text == '-2147483648':
                return '(-2147483647 - 1)'
            return text + ('UL' if member.type == 'uint64' else 'L' if member.type == 'int64'
                           else 'U' if member.type == 'uint32' else '')
        if member.type in ('float32', 'float64'):
            width = 32 if member.type == 'float32' else 64
            bits = int.from_bytes(struct.pack('<f' if width == 32 else '<d', float(text)), 'little')
            return '(' + str(bits) + ('U).reinterpret_as_float()' if width == 32 else 'UL).reinterpret_as_double()')
        raise ValueError('unsupported normalized MoonBit scalar default')
    if member.type in ('float32', 'float64'):
        return '(0U).reinterpret_as_float()' if member.type == 'float32' else '0.0'
    if member.type == 'bool':
        return 'false'
    if member.type in SCALARS:
        return '0UL' if member.type == 'uint64' else '0L' if member.type == 'int64' else '0U' if member.type == 'uint32' else '0'
    if member.type == 'string':
        return 'b""' if member.string_encoding == 'raw_bytes' else '""'
    return records[(record.version, member.type)].symbol + '::new()'


def invalid_semantics(bounded, record, names, captured_counts=False):
    rule = bounded.standard_semantics(record)
    if rule.startswith('normalized-'):
        field = next(m for m in record.members if m.name == ('nanosec' if record.version == 2 else 'nsec'))
        value = names[field.name]
        if field.type == 'uint32':
            return value + ' >= 1000000000U'
        return value + ' < 0 || ' + value + ' >= 1000000000'
    if rule == 'joint-state-optional-equal-length-arrays':
        sizes = [names[name] if captured_counts else names[name] + '.length()'
                 for name in ('name', 'position', 'velocity', 'effort')]
        return ' || '.join('(' + a + ' != 0 && ' + b + ' != 0 && ' + a + ' != ' + b + ')'
                          for i, a in enumerate(sizes) for b in sizes[i+1:])
    return ''


def write_scalar(record, member, records, value, indent):
    typ = member.type
    if typ == 'bool':
        return [indent + 'writer.unsigned(if ' + value + ' { 1UL } else { 0UL }, 1)']
    if typ in ('uint8', 'uint16', 'int8', 'int16'):
        width = SCALARS[typ][1]
        low, high = (0, (1 << (width*8))-1) if typ.startswith('u') else (-(1 << (width*8-1)), (1 << (width*8-1))-1)
        return [indent + 'if ' + value + ' < ' + str(low) + ' || ' + value + ' > ' + str(high) + ' { writer.error = 22 }',
                indent + 'writer.unsigned(' + value + '.to_int64().reinterpret_as_uint64() & ' + str((1 << (width*8))-1) + 'UL, ' + str(width) + ')']
    if typ == 'int32':
        call = 'writer.unsigned(' + value + '.to_int64().reinterpret_as_uint64() & 4294967295UL, 4)'
    elif typ == 'uint32': call = 'writer.unsigned(' + value + '.to_uint64(), 4)'
    elif typ == 'int64': call = 'writer.unsigned(' + value + '.reinterpret_as_uint64(), 8)'
    elif typ == 'uint64': call = 'writer.unsigned(' + value + ', 8)'
    elif typ == 'float32': call = 'writer.float32(' + value + ')'
    elif typ == 'float64': call = 'writer.float64(' + value + ')'
    elif typ == 'string': call = ('writer.blob(' if member.string_encoding == 'raw_bytes' else 'writer.text(') + value + ', ' + str(member.string_bound) + ')'
    else: call = 'write_' + records[(record.version, typ)].symbol + '(writer, ' + value + ')'
    return [indent + call]


def read_scalar(record, member, records):
    if member.type == 'string' and member.string_encoding == 'raw_bytes':
        return 'reader.blob(' + str(member.string_bound) + ')'
    return {'bool':'reader.boolean()', 'uint8':'reader.unsigned(1).to_int()',
        'int8':'reader.signed8()', 'uint16':'reader.unsigned(2).to_int()', 'int16':'reader.signed16()',
        'uint32':'reader.unsigned(4).to_uint()', 'int32':'reader.unsigned(4).to_uint().reinterpret_as_int()',
        'uint64':'reader.unsigned(8)', 'int64':'reader.unsigned(8).reinterpret_as_int64()',
        'float32':'reader.float32()', 'float64':'reader.float64()',
        'string':'reader.text(' + str(member.string_bound) + ')'}.get(member.type,
            'read_' + (records[(record.version, member.type)].symbol if member.type not in SCALARS and member.type != 'string' else '') + '(reader)')


CONTROL_RUNTIME = '''
// Explicit control byte domain; preserve NUL and invalid UTF8 without conversion.
// Other native strings continue through strict text(). No CDR/layout projection.
fn BoundedWriter::blob(self : BoundedWriter, value : Bytes, maximum : Int) -> Unit {
  if self.error != 0 { return () }
  if value.length() > maximum { self.error = 90; return () }
  self.unsigned(value.length().to_uint64(), 4)
  for byte in value { self.byte(byte) }
}

fn BoundedReader::blob(self : BoundedReader, maximum : Int) -> Bytes {
  let count = self.unsigned(4)
  if self.error != 0 { return b"" }
  if count > maximum.to_uint64() || count > (self.bytes.length() - self.position).to_uint64() {
    self.error = 22
    return b""
  }
  let first = self.position
  self.position = first + count.to_int()
  Bytes::makei(count.to_int(), fn(index) { self.bytes[first + index] })
}
'''


def emit(parent, bounded, records, ordered, interfaces, runtime):
    checked_ir(parent, bounded, records, ordered, interfaces)
    if any(record.profile == bounded.CONTROL_PROFILE for record in ordered):
        runtime += CONTROL_RUNTIME
    lines = [runtime, '// Complete actual IR records follow. No native registration.\n']
    for record in ordered:
        name = record.symbol
        control = record.profile == bounded.CONTROL_PROFILE
        if record.members:
            lines += ['pub(all) struct ' + name + ' {']
            for member in record.members:
                typ = element_type(record, member, records)
                lines += ['  mut ' + field_name(record, member) + ' : ' + ('Array[' + typ + ']' if member.array else typ)]
            lines += ['}', 'pub fn ' + name + '::new() -> ' + name + ' {', '  {']
            for member in record.members:
                value = default_value(bounded, record, member, records)
                if member.array:
                    value = '[]' if member.array == 'sequence' else 'Array::makei(' + str(member.count) + ', fn(_) { ' + value + ' })'
                lines += ['    ' + field_name(record, member) + ': ' + value + ',']
            lines += ['  }', '}']
        else:
            # A complete empty service/action component has no invented wire
            # field: a typed singleton represents its sole empty value.
            lines += ['pub(all) enum ' + name + ' { ' + name + 'Empty }',
                      'pub fn ' + name + '::new() -> ' + name + ' { ' + name + 'Empty }']
        codec = name + 'Codec'
        lines += ['pub(all) struct ' + codec + ' {',
                  '  ros_version : Int', '  native_type : String', '  native_identity : String?',
                  '  codec_identity : String', '  min_wire_bytes : Int', '  max_wire_bytes : Int', '}',
                  'pub fn ' + codec + '::new() -> ' + codec + ' {',
                  '  { ros_version: ' + str(record.version) + ', native_type: ' + json.dumps(record.name) + ',',
                  '    native_identity: ' + ('None' if record.identity is None else 'Some(' + json.dumps(record.identity) + ')') + ',',
                  '    codec_identity: ' + json.dumps(record.codec_identity) + ',',
                  '    min_wire_bytes: ' + str(record.minimum) + ', max_wire_bytes: ' + str(record.maximum) + ' }', '}',
                  'pub fn ' + codec + '::encode(_self : ' + codec + ', value : ' + name + ') -> Result[Bytes, Int] {',
                  '  let writer : BoundedWriter = { bytes: [], maximum: ' + str(record.maximum) + ', error: 0 }',
                  '  write_' + name + '(writer, value)',
                  '  if writer.error != 0 { return Err(writer.error) }',
                  '  if writer.bytes.length() < ' + str(record.minimum) + ' { return Err(22) }',
                  '  Ok(Bytes::makei(writer.bytes.length(), fn(index) { writer.bytes[index] }))', '}',
                  'pub fn ' + codec + '::decode(_self : ' + codec + ', input : Bytes) -> Result[' + name + ', Int] {',
                  '  if input.length() < ' + str(record.minimum) + ' || input.length() > ' + str(record.maximum) + ' { return Err(22) }',
                  '  let reader : BoundedReader = { bytes: input.copy(), position: 0, error: 0 }',
                  '  let value = read_' + name + '(reader)',
                  ('  if reader.error != 0 { Err(reader.error) } else if reader.position != reader.bytes.length() { Err(22) } else { Ok(value) }' if control else
                   '  if reader.error != 0 || reader.position != reader.bytes.length() { Err(22) } else { Ok(value) }'), '}',
                  'fn write_' + name + '(writer : BoundedWriter, value : ' + name + ') -> Unit {']
        if control:
            lines += ['  let record_start = writer.bytes.length()']
        captures = {}
        for index, member in enumerate(record.members):
            value = 'part' + str(index)
            captures[member.name] = value
            lines += ['  let ' + value + ' = value.' + field_name(record, member)]
            indent = '  '
            if member.array:
                count = 'count' + str(index)
                captures[member.name] = count
                lines += [indent + 'let ' + count + ' = ' + value + '.length()',
                          indent + 'if ' + count + (' != ' if member.array == 'array' else ' > ') + str(member.count) + ' { writer.error = 22; return () }']
                if member.array == 'sequence':
                    lines += [indent + 'writer.unsigned(' + count + '.to_uint64(), 4)']
                lines += [indent + 'for index = 0; index < ' + count + '; index = index + 1 {',
                          indent + '  let element = ' + value + '[index]']
                value, indent = 'element', indent + '  '
            lines += write_scalar(record, member, records, value, indent)
            if member.array:
                lines += ['  }']
        invalid = invalid_semantics(bounded, record, captures, captured_counts=True)
        if invalid: lines += ['  if ' + invalid + ' { writer.error = 22 }']
        if control:
            lines += ['  if writer.bytes.length() - record_start > ' + str(record.maximum) + ' { writer.error = 90 }']
        lines += ['}', 'fn read_' + name + '(reader : BoundedReader) -> ' + name + ' {']
        if control:
            lines += ['  let record_start = reader.position']
        values = {}
        for index, member in enumerate(record.members):
            value = 'part' + str(index)
            values[member.name] = value
            expression = read_scalar(record, member, records)
            if member.array:
                count = str(member.count) if member.array == 'array' else 'reader.count(' + str(member.count) + ', ' + str(element_minimum(record, member, records)) + ')'
                lines += ['  let ' + value + ' : Array[' + element_type(record, member, records) + '] = []',
                          '  let count' + str(index) + ' = ' + count,
                          '  for index = 0; index < count' + str(index) + '; index = index + 1 {',
                          '    ' + value + '.push(' + expression + ')', '  }']
            else:
                lines += ['  let ' + value + ' = ' + expression]
        invalid = invalid_semantics(bounded, record, values)
        if invalid: lines += ['  if ' + invalid + ' { reader.error = 22 }']
        if control:
            lines += ['  if reader.position - record_start > ' + str(record.maximum) + ' { reader.error = 90 }']
        lines += ['  { ' + ', '.join(field_name(record, member) + ': ' + values[member.name] for member in record.members) + ' }' if record.members else '  ' + name + 'Empty', '}', '']
        if control:
            # Complete actual IR metadata, including original constant literals,
            # native aliases/defaults/bounds, aggregate/cartesian/source fields.
            # Encode UTF8 as an explicit byte literal, avoiding any source-language
            # Unicode/string escaping assumption. Metadata never grants rights.
            metadata = asdict(record)
            metadata['symbol'] = record.symbol
            metadata['moonbit_member_spellings'] = {member.name: field_name(record, member) for member in record.members}
            raw_metadata = json.dumps(metadata, sort_keys=True, separators=(',', ':'), ensure_ascii=False).encode('utf-8')
            literal = 'b"' + ''.join('\\x' + format(byte, '02x') for byte in raw_metadata) + '"'
            lines += ['pub fn ' + codec + '::schema_bytes(_self : ' + codec + ') -> Bytes { ' + literal + ' }',
                      'pub fn ' + codec + '::aggregate_wire_bytes(_self : ' + codec + ') -> UInt64 { ' + str(record.aggregate_wire_bytes) + 'UL }',
                      'pub fn ' + codec + '::cartesian_maximum(_self : ' + codec + ') -> UInt64 { ' + str(record.cartesian_maximum) + 'UL }']
            for constant in record.constants:
                if constant['type'] not in SCALARS or not re.fullmatch(r'[A-Z][A-Z0-9_]*', constant['name']):
                    raise parent.GenerationError('actual complete scalar constant required')
                field = bounded.Field(name=constant['name'], type=constant['type'], native_default=constant['value'])
                value = default_value(bounded, record, field, records)
                lines += ['// Original constant; no instance field or wire bytes.',
                          'pub fn ' + codec + '::constant_' + constant['name'] + '(_self : ' + codec + ') -> ' + SCALARS[constant['type']][0] + ' { ' + value + ' }']
    return {'moonbit/bounded_records.mbt':'\n'.join(lines) + '\n',
            'moonbit/moon.pkg':'import { "uwvm/ros/wire", "moonbitlang/core/encoding/utf8", "moonbitlang/core/float" }\n'}


def generate(parent, bounded, catalog, output, *, require_native_identity=False):
    if type(require_native_identity) is not bool:
        raise parent.GenerationError('explicit native identity syntax policy required')
    catalog, output = Path(catalog), Path(output)
    sdk = Path(parent.__file__).resolve().parents[1]
    if Path(bounded.__file__).resolve() != sdk / 'custom-types/bounded_profile.py':
        raise parent.GenerationError('actual coherent authoritative bounded module required')
    if output.exists() or output.is_symlink() or output.resolve().is_relative_to(sdk):
        raise parent.GenerationError('fresh output outside SDK source required')
    sources = {}
    for role, path in {'moonbit_generator':Path(__file__),
        'moonbit_runtime':Path(__file__).with_name('moonbit_runtime.mbt.in'),
        'schema_generator':Path(parent.__file__), 'bounded_generator':Path(bounded.__file__),
        'moonbit_wire':sdk/'moonbit/wire/wire.mbt', 'moonbit_wire_package':sdk/'moonbit/wire/moon.pkg'}.items():
        raw = parent.bounded_read(path, parent.MAX_SOURCE_BYTES)
        sources[role] = {'path':str(path.resolve()), 'bytes':len(raw), 'sha256':hashlib.sha256(raw).hexdigest()}
    runtime = parent.bounded_read(Path(sources['moonbit_runtime']['path']), parent.MAX_SOURCE_BYTES).decode('utf-8')
    records, ordered, interfaces, pins = bounded.load_catalog(parent, catalog, require_native_identity)
    products = emit(parent, bounded, records, ordered, interfaces, runtime)
    if set(products) != OUTPUTS or sum(len(body.encode()) for body in products.values()) > MAX_OUTPUT_BYTES:
        raise parent.GenerationError('closed MoonBit output set/64MiB source bound')
    proof = {'format_version':1, 'kind':'optional-moonbit-bounded-record-sources-v1',
        'status':'GENERATED_OPTIONAL_RECORD_SOURCE_ONLY', 'language':'moonbit',
        'source_inputs':pins, 'generator_and_foundation_sources':sources,
        'schema_summary':bounded.summary(interfaces, pins),
        'records':[{'ros_version':r.version,'type':r.name,'symbol':r.symbol,
            'native_identity':r.identity,'adapter_codec_identity':r.codec_identity,
            'minimum_wire_bytes':r.minimum,'maximum_wire_bytes':r.maximum,
            'expanded_nodes':r.nodes,'depth':r.depth,
            'complete_value_semantics':bounded.standard_semantics(r),
            'native_defaults':{m.name:m.native_default for m in r.members if m.native_default is not None},
            'moonbit_source_types':{'value':r.symbol,'codec':r.symbol+'Codec',
                'constructor':'new','metadata':'max_wire_bytes/min_wire_bytes/native_identity/codec_identity/ros_version/native_type'}} for r in ordered],
        'outputs':{name:{'bytes':len(body.encode()),'sha256':hashlib.sha256(body.encode()).hexdigest()} for name,body in products.items()},
        'native_identity_syntax_required':require_native_identity,
        'native_qualification':'NOT_PERFORMED','native_registry_emitted':False,
        'implicit_permissions':[],'RPC_facades_emitted':False,
        'historical_emitters_called':False,'compiler_execution':'NOT_RUN','native_execution':'NOT_RUN'}
    for source in sources.values():
        raw = parent.bounded_read(Path(source['path']), parent.MAX_SOURCE_BYTES)
        if {'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()} != {key:source[key] for key in ('bytes','sha256')}:
            raise parent.GenerationError('MoonBit generator/foundation changed before write')
    raw = parent.bounded_read(catalog, parent.MAX_CATALOG_BYTES)
    if {'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()} != pins['catalog']:
        raise parent.GenerationError('catalog changed before MoonBit output')
    for source in pins['definitions']:
        path = (catalog.resolve().parent / source['file']).resolve()
        if not path.is_relative_to(catalog.resolve().parent):
            raise parent.GenerationError('definition escaped original catalog root')
        raw = parent.bounded_read(path, parent.MAX_SOURCE_BYTES)
        if {'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()} != {key:source[key] for key in ('bytes','sha256')}:
            raise parent.GenerationError('definition changed before MoonBit output')
    output.mkdir(parents=True, exist_ok=False)
    for name, body in products.items():
        path = output/name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(body, encoding='utf-8')
    (output/'generation.moonbit.json').write_text(json.dumps(proof, indent=2, sort_keys=True)+'\n', encoding='utf-8')
    return proof


def fixed_module(name, path):
    if name in sys.modules:
        raise RuntimeError('private generator module already occupied')
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError('fixed source loader unavailable')
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    try:
        spec.loader.exec_module(module)
    except BaseException:
        sys.modules.pop(name, None)
        raise
    return module


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('catalog', type=Path)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--language', choices=('moonbit',), required=True)
    parser.add_argument('--require-native-identity', action='store_true')
    args = parser.parse_args(argv)
    sdk = Path(__file__).resolve().parents[1]
    parent = fixed_module('_uwvm_optional_moonbit_schema', sdk/'tools/custom_types.py')
    bounded = fixed_module('_uwvm_optional_moonbit_bounded', sdk/'custom-types/bounded_profile.py')
    generate(parent, bounded, args.catalog, args.out, require_native_identity=args.require_native_identity)
    return 0


if __name__ == '__main__':
    sys.dont_write_bytecode = True
    raise SystemExit(main())
