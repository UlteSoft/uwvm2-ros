"""Independent complete industrial Message/Service/Action source domain.

This operator is source preparation, never a ROS loader, grant, identity oracle,
compiler, or robot driver. Original definitions are parsed without filtering.
The caller supplies the admitted, SHA-paired existing parent/bounded source and
all fixed template source roles. Each API owns isolated function globals; it
never changes an existing imported generator or its historical output domain.

The explicit domain adds source constants, scalar/string declared defaults,
ROS1 time/duration, ROS2 byte/char distinctions, and reversible language member
spellings. Every instance field remains in canonical order and native metadata.
Unknown array/nested defaults, IDL preprocessing and unsupported C/C++ keyword
members fail closed before source publication; no field is erased to accept it.
"""
from pathlib import Path
import ast
import copy
import hashlib
import json
import math
import os
import re
import stat
import sys
import types

PROFILE = 'uwvm-ros-vendor-complete-bounded-v1'
MAX_SOURCE = 16 * 1024 * 1024
MAX_WIRE = 32768
MAX_TYPES = 512
MAX_DEPTH = 16
MAX_NODES = 4096
MAX_TOTAL_NODES = 16384
LANGUAGES = ('c', 'cpp', 'rust', 'javascript', 'assemblyscript', 'tinygo')
C_KEYWORDS = set(('auto break case char const continue default do double else enum extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while _Alignas _Alignof _Atomic _Bool _Complex _Generic _Imaginary _Noreturn _Static_assert _Thread_local alignas alignof asm bool catch class concept consteval constexpr constinit const_cast co_await co_return co_yield decltype delete dynamic_cast explicit export false friend mutable namespace new noexcept nullptr operator private protected public reinterpret_cast requires static_assert static_cast template this thread_local throw true try typeid typename using virtual wchar_t and or not xor bitand bitor compl and_eq or_eq not_eq xor_eq').split())


def need(value, message):
    if not value:
        raise ValueError(message)


def strict_json(raw):
    def pairs(rows):
        out = {}
        for k, v in rows:
            need(k not in out, 'duplicate JSON member')
            out[k] = v
        return out
    def number(s):
        value = float(s)
        need(math.isfinite(value), 'nonfinite JSON number')
        return value
    return json.loads(raw, object_pairs_hook=pairs, parse_float=number,
                      parse_constant=lambda s: need(False, 'nonfinite JSON token'))


def canonical(value):
    return (json.dumps(value, indent=2, sort_keys=True, ensure_ascii=True,
                       allow_nan=False) + '\n').encode()


def read_source(path, maximum=MAX_SOURCE):
    """Ordinary one-FD bounded read; final path, extent and identity stay paired."""
    path = Path(path)
    need(path.is_absolute(), 'absolute source path required')
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        before = os.fstat(fd)
        need(stat.S_ISREG(before.st_mode) and before.st_nlink == 1 and
             0 <= before.st_size <= maximum, 'bounded single-link ordinary source required')
        chunks = []; count = 0
        while True:
            block = os.read(fd, min(65536, maximum + 1 - count))
            if not block:
                break
            count += len(block)
            need(count <= maximum and count <= before.st_size, 'source grew beyond admission')
            chunks.append(block)
        after = os.fstat(fd); named = os.stat(path, follow_symlinks=False)
        key = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mode, s.st_nlink,
                         s.st_mtime_ns, s.st_ctime_ns)
        need(key(before) == key(after) == key(named) and count == before.st_size,
             'source changed during read')
        return b''.join(chunks)
    finally:
        os.close(fd)


def _module(raw, path, name):
    """Execute only the caller's actual captured source; never Python bytecode."""
    module = types.ModuleType(name)
    module.__file__ = str(path)
    previous = sys.modules.get(name)
    need(previous is None, 'fresh private source module name required')
    sys.modules[name] = module
    try:
        exec(compile(raw, str(path), 'exec', dont_inherit=True), module.__dict__)
    finally:
        if sys.modules.get(name) is module:
            del sys.modules[name]
    return module


def _uncomment(line):
    quote = None; escaped = False
    for index, char in enumerate(line):
        if escaped:
            escaped = False
        elif char == '\\' and quote:
            escaped = True
        elif quote:
            if char == quote:
                quote = None
        elif char in ('"', "'"):
            quote = char
        elif char == '#':
            return line[:index].strip()
    need(quote is None, 'unterminated quoted source literal')
    return line.strip()


def _text_literal(literal):
    need(type(literal) is str and 0 < len(literal) <= 32768,
         'finite original string declaration required')
    # Exact pinned rosidl_adapter parse_primitive_value_string semantics:
    # remove matching outer quotes, then unescape only that quote. A Python
    # literal evaluator would incorrectly turn original backslash-n/x00 into
    # different semantic native bytes and a different TypeDescription default.
    if literal[:1] in ('"', "'"):
        quote = literal[0]
        need(len(literal) >= 2 and literal[-1] == quote, 'matching original outer quote required')
        value = literal[1:-1]
        need(re.search(r'(?<!\\)' + re.escape(quote),value) is None,
             'original string inner quote is not escaped')
        value = value.replace('\\' + quote,quote)
    else:
        value = literal
    try:
        raw = value.encode('utf-8', errors='strict')
    except UnicodeError:
        raise ValueError('original text is not Unicode scalar UTF-8') from None
    need(b'\0' not in raw, 'NUL text is outside this industrial text domain')
    return value


def _native_name(parent, version, name, kind, standard=False):
    parts = name.split('/') if type(name) is str else []
    need(type(version) is int and version in (1, 2) and
         len(parts) == (2 if version == 1 else 3) and
         re.fullmatch('[a-z][a-z0-9_]{0,62}', parts[0]) and
         re.fullmatch('[A-Za-z][A-Za-z0-9_]*' if version == 1 else '[A-Z][A-Za-z0-9_]*', parts[-1]) and len(name) <= 255,
         'exact original native interface name required')
    need(version == 1 or parts[1] == {'message':'msg','service':'srv','action':'action'}[kind],
         'native category differs from complete source suffix')
    return name


def _native_metadata_key(interface, interfaces):
    """Disambiguate real ROS1 Message/Action roots in our exporter JSON only.

    Runtime/native type names and complete payload identities stay original.
    Noncolliding domains preserve their historical exporter keys byte-for-byte.
    A key is derived from the full selected source domain, never guest input.
    """
    collision = sum(other.version == interface.version and other.name == interface.name
                    for other in interfaces) > 1
    return interface.kind + ':' + interface.name if collision else interface.name


def _reference(parent, version, package, name):
    if version == 1 and name in ('time', 'duration'):
        return name
    if name == 'Header':
        name = 'std_msgs/Header'
    bits = name.split('/')
    if len(bits) == 1:
        name = package + ('/msg/' if version == 2 else '/') + name
    elif len(bits) == 2 and version == 2:
        name = bits[0] + '/msg/' + bits[1]
    return _native_name(parent, version, name, 'message')


def _member(name):
    need(type(name) is str and re.fullmatch('[A-Za-z][A-Za-z0-9_]*', name) and
         len(name) <= 128 and name not in C_KEYWORDS,
         'member has no proved C/C++ source spelling in this finite industrial profile')
    return name


def _parse_fields(parent, text, version, package, bounds, used, prefix='',
                  standard=False, native_type='', control=False, constants=None):
    """Keep constants/defaults and exact fields; capacities are separate quotas."""
    result = []; names = set(); constant_names = set()
    for number, original in enumerate(text.splitlines(), 1):
        line = _uncomment(original)
        if not line:
            continue
        constant = re.fullmatch(r'([A-Za-z][A-Za-z0-9_]*)\s+([A-Z][A-Z0-9_]*)\s*=\s*(.+)', line)
        if constant:
            typ, name, literal = constant.groups()
            need(constants is not None and name not in constant_names and name not in names,
                 'duplicate complete source constant/member')
            need(typ in parent.SCALARS or typ in ('string','char','byte'),
                 'constant must have an actual scalar native type')
            wire = ('int8' if typ == 'byte' and version == 1 else 'uint8') if typ in ('byte','char') else typ
            value = _text_literal(literal) if typ == 'string' else _original_scalar_default(parent, wire, literal)
            constants.append({'name':name,'type':wire,'native_type':typ,
                              'literal':literal,'value':value})
            constant_names.add(name)
            continue
        field = re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:<=([0-9]+))?(?:\[([0-9]*|<=[0-9]+)\])?\s+([A-Za-z][A-Za-z0-9_]*)(?:\s+(.+))?', line)
        need(field is not None, 'unsupported full original field syntax at line ' + str(number))
        typ, string_native, array, name, literal = field.groups()
        _member(name)
        need(name not in names and name not in constant_names, 'duplicate original field')
        names.add(name)
        native_scalar = typ if typ in ('byte','char') else ''
        if native_scalar:
            typ = 'int8' if native_scalar == 'byte' and version == 1 else 'uint8'
        need(typ != 'wstring', 'wide strings need a separate complete canonical domain')
        supplied = bounds.get(prefix + name, {})
        if prefix + name in bounds:
            used.add(prefix + name)
        need(type(supplied) is dict and set(supplied) <= {'string','sequence'} and
             all(type(v) is int and 1 <= v <= MAX_WIRE for v in supplied.values()),
             'strict explicit canonical string/sequence quotas required')
        native_string = int(string_native) if string_native else 0
        need(not string_native or version == 2 and typ == 'string' and 1 <= native_string <= MAX_WIRE-4,
             'invalid actual native string capacity')
        string_bound = supplied.get('string', native_string)
        if typ == 'string':
            need(1 <= string_bound <= MAX_WIRE-4 and (not native_string or string_bound <= native_string),
                 'every original string needs an explicit in-range byte capacity')
        else:
            need('string' not in supplied, 'text capacity on nontext member')
        shape = ''; count = 0; native_sequence = 0
        if array is not None:
            if array.startswith('<='):
                need(version == 2, 'native bounded sequence is ROS2 only')
                native_sequence = int(array[2:]); shape = 'sequence'
                count = supplied.get('sequence', native_sequence)
            elif array:
                shape = 'array'; count = int(array)
            else:
                shape = 'sequence'; count = supplied.get('sequence', 0)
            need(1 <= count <= (MAX_WIRE if typ == 'uint8' else MAX_NODES) and
                 (not native_sequence or count <= native_sequence),
                 'finite complete sequence/fixed-array capacity required')
        need('sequence' not in supplied or shape == 'sequence', 'sequence quota on fixed/scalar member')
        default = None
        if literal is not None:
            need(version == 2, 'declared defaults are ROS2 source declarations')
            if array is not None:
                need(shape == 'array' and typ in ('float32','float64'), 'only fixed floating-array defaults are admitted')
                default = _fixed_float_default(parent,typ,literal,count)
            elif typ == 'string':
                default = _text_literal(literal)
                need(len(default.encode('utf-8')) <= string_bound, 'original default exceeds canonical quota')
            else:
                need(typ in parent.SCALARS, 'nested default is not a scalar declaration')
                default = _original_scalar_default(parent, typ, literal)
        resolved = typ if typ in parent.SCALARS or typ == 'string' else _reference(parent,version,package,typ)
        result.append(Field(name, resolved, shape, count, string_bound if typ == 'string' else 0,
                            native_string, native_sequence, default, native_scalar, 'utf8'))
    need(len(result) < MAX_NODES, 'too many complete fields')
    return result


def _semantic(record):
    if record.version == 1 and record.name in ('time','duration'):
        return 'normalized-duration' if record.name == 'duration' else 'normalized-time'
    if record.version == 2 and record.name in ('builtin_interfaces/msg/Time','builtin_interfaces/msg/Duration'):
        return 'normalized-time'
    if record.name in ('sensor_msgs/JointState','sensor_msgs/msg/JointState'):
        return 'joint-state-optional-equal-length-arrays'
    return ''


# The complete bounded resolver is retained as source, with only the explicit
# industrial format/parser/registration policy above substituted. No original
# parent/bounded module is patched on disk or shared across concurrent APIs.
def _load_impl(parent, path: Path, require_identity=True):
    """Resolve an acyclic complete schema and pin all exact input definitions."""
    raw = parent.bounded_read(path, parent.MAX_CATALOG_BYTES)
    catalog = parent.strict_json(raw)
    if not isinstance(catalog, dict) or set(catalog) != {'format', 'types'} or catalog['format'] != PROFILE:
        raise parent.GenerationError('bounded catalog requires exactly format and types')
    standard = True
    control = False
    specs = catalog['types']
    if not isinstance(specs, list) or not 1 <= len(specs) <= MAX_TYPES:
        raise parent.GenerationError('bounded catalog requires1..512 interfaces')
    root, records, interfaces, inputs, seen = path.resolve().parent, {}, [], [], set()
    for spec in specs:
        if not isinstance(spec, dict) or set(spec) - {'ros_version', 'type', 'file', 'native_identity', 'component_identities', 'bounds', 'registered', 'wire_limits'} or not {'ros_version', 'type', 'file'} <= set(spec):
            raise parent.GenerationError('unsupported/missing bounded catalog entry key')
        if not isinstance(spec['file'], str):
            raise parent.GenerationError('definition file must be a relative string')
        source = (root / spec['file']).resolve()
        if not source.is_relative_to(root):
            raise parent.GenerationError('definition escapes bounded catalog root')
        kind = {'.msg': 'message', '.srv': 'service', '.action': 'action'}.get(source.suffix)
        if kind is None:
            raise parent.GenerationError('bounded-v2 accepts official .msg/.srv/.action; unsupported IDL preprocessing must be generated into these complete definitions first')
        version, name = spec['ros_version'], spec['type']
        name = control_native_name(parent,version,name,kind) if control else native_name(parent, version, name, kind, standard)
        registered = spec.get('registered', True)
        if control and (type(registered) is not bool or (registered and name not in control_registered_names())):
            raise parent.GenerationError('control registration is limited to the exact default endpoint family')
        if type(registered) is not bool:
            raise parent.GenerationError('explicit Message dependency or registered interface required')
        if standard and (version, name) in standard_names() and registered and (version, name) not in standard_topic_names():
            raise parent.GenerationError('official standard dependency-only and existing builtin IDs cannot be registered as new Topics')
        # ROS1 permits Foo.msg beside Foo.action: their actual C++ headers are
        # Foo.h and FooAction.h. Keep all native names and kind-specific
        # descriptors; never replace the Message with the Action or its payload.
        key = (version, name, kind)
        if key in seen:
            raise parent.GenerationError('duplicate native interface in one category')
        same = {k for v,n,k in seen if (v,n) == (version,name)}
        if same and (version != 1 or same | {kind} != {'message','action'}):
            raise parent.GenerationError('native interface categories collide')
        seen.add(key)
        body = parent.bounded_read(source, parent.MAX_SOURCE_BYTES)
        try:
            text = body.decode('utf-8')
        except UnicodeError as error:
            raise parent.GenerationError('native definitions must be UTF-8') from error
        pin = {'file': spec['file'], 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()}
        if control and {'bytes':len(body),'sha256':pin['sha256']}!=control_sources()[name]:
            raise parent.GenerationError('control original full source differs: '+name)
        inputs.append(pin)
        sections = re.split(r'^\s*---\s*(?:#[^\n]*)?$', text, flags=re.MULTILINE)
        roles = {'message': ['message'], 'service': ['request', 'response'], 'action': ['goal', 'result', 'feedback']}[kind]
        if len(sections) != len(roles):
            raise parent.GenerationError('wrong number of complete .srv/.action components')
        wire_limits = spec.get('wire_limits', {})
        if not isinstance(wire_limits, dict) or ('wire_limits' in spec and not wire_limits) or set(wire_limits) - set(roles):
            raise parent.GenerationError('control wire_limits must name exact existing component roles')
        if any(type(value) is not int or not 1 <= value <= MAX_WIRE for value in wire_limits.values()):
            raise parent.GenerationError('control aggregate wire limit must be a strict positive integer <=32768')
        native_identity = identity(parent, version, spec.get('native_identity'), require_identity, name)
        components = spec.get('component_identities', {})
        if not isinstance(components, dict) or set(components) - set(roles) or (kind == 'message' and components):
            raise parent.GenerationError('component_identities must name only the exact Service/Action roles')
        bounds, used = spec.get('bounds', {}), set()
        if not isinstance(bounds, dict) or not all(isinstance(key, str) for key in bounds):
            raise parent.GenerationError('bounds must map complete declared field paths to explicit limits')
        interface = Interface(version, name, kind, native_identity, registered=registered, profile=catalog['format'])
        for role, section in zip(roles, sections):
            part_name = name if kind == 'message' else name + ('_' if version == 2 else '') + role.title()
            part_identity = native_identity if kind == 'message' else identity(parent, version, components.get(role), require_identity, name + ':' + role)
            constants=[]
            members = parse_fields(parent, section, version, name.split('/')[0], bounds, used, '' if kind == 'message' else role + '.', standard, part_name, control, constants)
            if not members and kind == 'message' and registered:
                raise parent.GenerationError('empty native Topics are outside this profile; empty Service/Action components are supported')
            record = Record(version, part_name, members, part_identity, pin, '' if kind == 'message' else role, name, kind)
            record.profile = catalog['format']
            record.constants = constants
            record.original_section = section
            record.aggregate_wire_bytes = wire_limits.get(role, 0)
            if standard:
                standard_shape(parent, record)
            key = (version, part_name)
            if key in records:
                raise parent.GenerationError('component/message native record name collision')
            records[key] = record;interface.components[role] = record
        if set(bounds) != used:
            raise parent.GenerationError('bounds contain an undeclared field path')
        interfaces.append(interface)
    if standard:
        # ROS1 primitive values have no MD5/RIHS or MSG dependency section.
        # Their actual C++ fields/types are verified in generated native traits.
        needed = {m.type for r in records.values() if r.version == 1 for m in r.members if m.type in ('time','duration')}
        for name in sorted(needed):
            scalar = 'uint32' if name == 'time' else 'int32'
            item = Record(1,name,[Field('sec',scalar),Field('nsec',scalar)],None,
                {'ros1_native_primitive':name,'native_fields':'actual ros::Time/Duration sec/nsec; compiled static traits required'},kind='native-primitive',profile=PROFILE)
            records[(1,name)] = item
    # C++/Rust/JS/Go/AS share the concatenated public type spelling.  Package
    # separators/capitalization and generated companion suffixes must not make
    # two distinct official types occupy one emitted namespace.  C's uppercase
    # macro namespace is checked independently.  Refusal preserves native names.
    public, c_names, c_macros = set(), set(), set()
    for record in records.values():
        symbols = {record.symbol, record.symbol + 'Codec', 'Decode' + record.symbol,
                   'read' + record.symbol, record.symbol + 'CodecIdentity',
                   record.symbol + 'MaxWireBytes'}
        c_symbols = {record.c_symbol} | {record.c_symbol + suffix for suffix in
                    ('_valid', '_wire_size', '_write_fields', '_read_fields', '_encode', '_decode')}
        for member in record.members:
            if member.type == 'string':
                c_symbols.add(record.c_symbol + '_' + member.name + '_text')
            if member.array == 'sequence':
                c_symbols.add(record.c_symbol + '_' + member.name + '_sequence')
        macros = {record.c_symbol.upper() + suffix for suffix in
                  ('_MAX_WIRE_BYTES', '_MIN_WIRE_BYTES', '_CODEC_IDENTITY')}
        if wire_extensions(record, records):
            c_symbols.update(record.c_symbol + suffix for suffix in ('_shape', '_add', '_measure', '_read_status'))
            macros.update(record.c_symbol.upper() + suffix for suffix in ('_CARTESIAN_MAX_WIRE_BYTES', '_AGGREGATE_WIRE_BYTES'))
        if public.intersection(symbols) or c_names.intersection(c_symbols) or c_macros.intersection(macros):
            raise parent.GenerationError('native records collide in generated language namespace')
        public.update(symbols);c_names.update(c_symbols);c_macros.update(macros)
    for interface in interfaces:
        if interface.kind != 'message':
            symbols = {interface.symbol + 'Client', interface.symbol + 'Server'}
            if public.intersection(symbols):
                raise parent.GenerationError('native interface collides with generated client/server namespace')
            public.update(symbols)
    ordered, active = [], set()
    def resolve(record, depth=1):
        key = (record.version, record.name)
        if depth > MAX_DEPTH or key in active:
            raise parent.GenerationError('recursive/excess-depth bounded records are unsupported')
        if record.codec_identity:
            if depth + record.depth - 1 > MAX_DEPTH:
                raise parent.GenerationError('cached bounded dependency exceeds depth16')
            return
        active.add(key);minimum,maximum,cartesian,nodes,member_ir = 0,0,0,1,[]
        record.has_native_defaults = False
        for member in record.members:
            if member.type in parent.SCALARS:
                low = high = full_high = parent.SCALARS[member.type][1];count = 1;child_ir = {'scalar': member.type};child_depth = 1
            elif member.type == 'string':
                low,high,count,child_depth = 4,4+member.string_bound,1,1
                full_high = high
                child_ir = ({'raw_bytes': member.string_bound, 'encoding': 'raw_bytes'} if member.string_encoding == 'raw_bytes' else {'utf8_bytes': member.string_bound, 'nul': 'forbidden'})
            else:
                child = records.get((record.version, member.type))
                if child is None or (child.kind != 'message' and not (standard and record.version == 1 and child.kind == 'native-primitive')):
                    raise parent.GenerationError('nested message dependency must be explicitly catalogued: ' + member.type)
                resolve(child, depth+1)
                low,high,count,child_depth = child.minimum,child.maximum,child.nodes,child.depth
                full_high = child.cartesian_maximum
                child_ir = {'message': member.type, 'canonical_identity': child.codec_identity}
                record.has_native_defaults |= child.has_native_defaults
            if member.native_scalar:
                child_ir['native_scalar'] = member.native_scalar
            if member.native_default is not None:
                record.has_native_defaults = True
                # Add this key only for declared defaults; every existing
                # no-default canonical identity domain stays byte-identical.
                child_ir['native_default'] = member.native_default
            if member.array:
                high *= member.count;full_high *= member.count
                # A uint8 array is one bounded byte storage span, not a graph
                # of separately allocated/nested objects. Every actual byte
                # still contributes to full wire size and is encoded/checked.
                # The32KiB wire cap bounds its work even when node cost is2.
                # Other scalar/message arrays retain Cartesian node charging.
                count = 2 if member.type == 'uint8' else count*member.count+1
                child_depth += 1
                low = low*member.count if member.array == 'array' else 4
                if member.array == 'sequence':
                    high += 4;full_high += 4
                child_ir = {member.array: member.count, 'element': child_ir}
            minimum += low;maximum += high;cartesian += full_high;nodes += count
            record.depth = max(record.depth, child_depth+1)
            if (maximum > MAX_WIRE and not record.aggregate_wire_bytes) or cartesian > (1 << 64) - 1 or nodes > MAX_NODES or record.depth > MAX_DEPTH:
                raise parent.GenerationError('bounded wire32KiB/uint64-cartesian/node4096/depth16 ceiling exceeded: '+record.name+' wire='+str(maximum)+' cartesian='+str(cartesian)+' nodes='+str(nodes)+' depth='+str(record.depth))
            member_ir.append({'name': member.name, 'wire': child_ir})
        if record.aggregate_wire_bytes and minimum > record.aggregate_wire_bytes:
            raise parent.GenerationError('aggregate wire limit is smaller than the complete record minimum')
        record.minimum,record.maximum,record.nodes = minimum,min(maximum,record.aggregate_wire_bytes) if record.aggregate_wire_bytes else maximum,nodes
        record.cartesian_maximum = cartesian
        ir = {'profile': record.profile,'ros_version':record.version,'type':record.name,'members':member_ir,
              'byte_order':'little-endian','booleans':'0-or-1','floats':'finite-ieee754'}
        if record.aggregate_wire_bytes:
            ir['aggregate_wire_bytes'] = record.aggregate_wire_bytes
        if record.constants:
            ir['declared_constants'] = record.constants
        if standard_semantics(record):
            ir['complete_value_semantics'] = standard_semantics(record)
        record.codec_identity = 'UWVMROS1_' + hashlib.sha256(parent.canonical_json(ir)).hexdigest()
        active.remove(key);ordered.append(record)
    for record in records.values():
        resolve(record)
    for record in ordered:
        if record.has_native_defaults:
            new_public = 'Default' + record.symbol
            new_c = record.c_symbol + '_default'
            if new_public in public or new_c in c_names:
                raise parent.GenerationError('declared-default initializer collides with a generated namespace')
            public.add(new_public);c_names.add(new_c)
    if sum(record.nodes for record in ordered) > MAX_TOTAL_NODES:
        raise parent.GenerationError('total expanded generation nodes exceed16384')
    for interface in interfaces:
        ir = {'profile':interface.profile,'ros_version':interface.version,'type':interface.name,'kind':interface.kind,
              'components':{role:record.codec_identity for role,record in interface.components.items()},
              'tickets':'runtime-owned-u64','result_kinds':[2,3,4] if interface.kind=='action' else []}
        interface.codec_identity = interface.components['message'].codec_identity if interface.kind=='message' else 'UWVMROS1_'+hashlib.sha256(parent.canonical_json(ir)).hexdigest()
    return records,ordered,interfaces,{'catalog':{'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()},'definitions':inputs}


def _c_text(value):
    return '"' + ''.join('\\%03o' % byte for byte in value.encode('utf-8')) + '"'


def _rust_text(value):
    chunks = []
    for char in value:
        code = ord(char)
        if char == '"': chunks.append('\\"')
        elif char == '\\': chunks.append('\\\\')
        elif code < 32 or code == 127: chunks.append('\\u{' + format(code,'x') + '}')
        else: chunks.append(char)
    return '"' + ''.join(chunks) + '"'


def _native_definition(record):
    if hasattr(record,'original_section'):
        return record.original_section + ('\n' if not record.original_section.endswith('\n') else '')
    return ''.join(m.type+' '+m.name+'\n' for m in record.members)


def _fixed_float_default(parent, type_, literal, count):
    """Normalize each finite original fixed-array element without evaluating code."""
    need(type(literal) is str and len(literal) <= 8192 and
         literal.startswith('[') and literal.endswith(']') and 1 <= count <= 128,
         'bounded complete fixed floating-array literal required')
    try:
        values = ast.literal_eval(literal)
    except (ValueError,SyntaxError,TypeError,MemoryError,RecursionError):
        raise parent.GenerationError('fixed floating-array default is not literal data') from None
    need(type(values) is list and len(values) == count and
         all(type(value) in (int,float) for value in values),
         'fixed floating-array default shape/type differs')
    return '[' + ', '.join(_original_scalar_default(parent,type_,repr(value)) for value in values) + ']'


def _array_default_values(type_, normalized):
    """Recheck immutable default IR before emitting target source."""
    need(type_ in ('float32','float64') and type(normalized) is str and
         len(normalized) <= 8192 and normalized.startswith('[') and normalized.endswith(']'),
         'admitted fixed floating-array default required')
    try:
        values = ast.literal_eval(normalized)
    except (ValueError,SyntaxError,TypeError,MemoryError,RecursionError):
        raise ValueError('fixed floating-array IR is not literal data') from None
    need(type(values) is list and 1 <= len(values) <= 128 and
         all(type(value) is float and math.isfinite(value) for value in values) and str(values) == normalized,
         'canonical finite floating-array IR required')
    return [repr(value) for value in values]


def _js_default(member, record, records):
    """An explicit fixed array is one value, never N copies of an array."""
    if member.array == 'array' and member.native_default is not None and member.type in ('float32','float64'):
        values = _array_default_values(member.type,member.native_default)
        need(len(values) == member.count, 'fixed array default count differs')
        return _default_expression(member.type,member.native_default,'javascript')
    return _original_js_default(member,record,records)


def _default_expression(type_, normalized, language):
    if type_ in ('float32','float64') and type(normalized) is str and normalized.startswith('['):
        values = _array_default_values(type_,normalized)
        elements = [_original_default_expression(type_,value,language) for value in values]
        if language == 'c':return '{' + ','.join(elements) + '}'
        if language == 'tinygo':return '[' + str(len(values)) + ']' + type_ + '{' + ','.join(elements) + '}'
        need(language in ('rust','javascript','assemblyscript'), 'implemented fixed-array target required')
        return '[' + ','.join(elements) + ']'
    if type_ != 'string':
        return _original_default_expression(type_, normalized, language)
    need(type(normalized) is str and '\0' not in normalized,
         'admitted native text default required')
    normalized.encode('utf-8', errors='strict')
    if language == 'c':
        return _c_text(normalized)
    if language == 'rust':
        return 'String::from(' + _rust_text(normalized) + ')'
    need(language in ('javascript','assemblyscript','tinygo'), 'implemented text-default target required')
    return json.dumps(normalized, ensure_ascii=False)


def _member_maps(ordered, bounded):
    maps = bounded.control_member_maps(ordered)
    for record in ordered:
        rows = maps[record.name]
        rows['c'] = {m.name:m.name for m in record.members}
        rows['cpp'] = dict(rows['c'])
        for language, mapping in rows.items():
            need(len(set(mapping.values())) == len(mapping), 'source-language member collision')
            forbidden = {'tinygo':{'WireSize','Encode'},
                         'assemblyscript':{'wireSize','writeBounded','readBounded','validValue',
                                           'maxWireBytes','aggregateWireBytes','cartesianWireBytes'},
                         'rust':{'wire_size','encode_into','read_fields','MAX_WIRE_BYTES',
                                 'CARTESIAN_WIRE_BYTES','AGGREGATE_WIRE_BYTES','CODEC_IDENTITY'}}
            need(not set(mapping.values()) & forbidden.get(language,set()), 'field collides with generated method')
        for constant in record.constants:
            need(constant['name'] not in {'MAX_WIRE_BYTES','CARTESIAN_WIRE_BYTES','AGGREGATE_WIRE_BYTES','CODEC_IDENTITY'},
                 'constant collides with generated wire fact')
    return maps


def _rename_as_support(text):
    """Rename helper identifier tokens without altering native symbols/literals.

    The previous substring replacement also changed StreamControlCodec into
    StreamVendorCodec. Quoted original constants and comments are source data;
    only complete unquoted helper tokens belong to this adapter's namespace.
    """
    mapping = {'ControlEncoded':'VendorEncoded','ControlDecoded':'VendorDecoded',
               'ControlCodec':'VendorCodec','ControlWriter':'VendorWriter'}
    output = []; at = 0
    while at < len(text):
        first = text[at]
        if first in ('"', "'", chr(96)):
            start = at; quote = first; at += 1
            while at < len(text):
                if text[at] == chr(92):
                    at += 2
                elif text[at] == quote:
                    at += 1; break
                else:
                    at += 1
            else:
                raise ValueError('unterminated generated AssemblyScript literal')
            output.append(text[start:at]); continue
        if text.startswith('//', at):
            end = text.find(chr(10), at)
            if end < 0: end = len(text)
            output.append(text[at:end]); at = end; continue
        if text.startswith('/*', at):
            end = text.find('*/', at+2)
            need(end >= 0, 'unterminated generated AssemblyScript comment')
            end += 2; output.append(text[at:end]); at = end; continue
        if first.isascii() and (first.isalpha() or first in ('_', '$')):
            start = at; at += 1
            while at < len(text) and text[at].isascii() and (text[at].isalnum() or text[at] in ('_', '$')):
                at += 1
            token = text[start:at]; output.append(mapping.get(token, token))
        else:
            output.append(first); at += 1
    return ''.join(output)


class API:
    """Source-paired API for Root's finite, default-denied preparation pipeline.

    The caller must bind this new mode explicitly in its reviewed vendor28
    preparer. Passing this catalog to the old vendor19/preparer is refused;
    this object never changes that inventory or catches its refusal.
    """
    def __init__(self, parent_raw, bounded_raw, *, parent_path, bounded_path,
                 parent_sha256, bounded_sha256, templates, adapter_raw, adapter_sha256):
        need(type(parent_raw) is bytes and type(bounded_raw) is bytes and
             len(parent_raw) <= MAX_SOURCE and len(bounded_raw) <= MAX_SOURCE and
             hashlib.sha256(parent_raw).hexdigest() == parent_sha256 and
             hashlib.sha256(bounded_raw).hexdigest() == bounded_sha256,
             'exact captured original generator pair required')
        need(Path(parent_path).is_absolute() and Path(bounded_path).is_absolute(),
             'actual absolute source observations required')
        import uuid
        suffix = uuid.uuid4().hex  # Private module identity, never a native/schema identity.
        self.parent = _module(parent_raw,parent_path,'_industrial_parent_' + suffix)
        self.bounded = _module(bounded_raw,bounded_path,'_industrial_bounded_' + suffix)
        need(all(hasattr(self.bounded,name) for name in
             ('Field','Record','Interface','emit_control_rust','emit_control_js','emit_control_go',
              'emit_control_as','emit_c','emit_cpp','emit_native','ros2_interface_schema')),
             'actual reviewed complete bounded implementation required')
        need(type(adapter_raw) is bytes and len(adapter_raw) <= MAX_SOURCE and
             hashlib.sha256(adapter_raw).hexdigest() == adapter_sha256 and
             read_source(Path(__file__)) == adapter_raw, 'actual captured industrial adapter source required')
        self.sources = {'industrial_adapter':{'bytes':len(adapter_raw),'sha256':adapter_sha256},'parent':{'bytes':len(parent_raw),'sha256':parent_sha256},
                        'bounded':{'bytes':len(bounded_raw),'sha256':bounded_sha256}}
        self.captured = {str(parent_path):parent_raw,str(bounded_path):bounded_raw,str(Path(__file__)):adapter_raw}
        need(type(templates) is dict and set(templates) == {
             'interface_templates/bounded_interfaces.' + e + '.in' for e in ('rs','mjs','go','ts')},
             'all actual four RPC template sources required')
        self.templates = dict(templates)
        for name, raw in self.templates.items():
            need(type(raw) is bytes and len(raw) <= MAX_SOURCE, 'bounded captured template source required')
            self.captured[str(Path(bounded_path).parent/name)] = raw
        # Every copied callable has one private global dictionary. The public
        # modules and their old mode functions are never mutated or rebound.
        env = dict(vars(self.bounded))
        for name, function in list(env.items()):
            if isinstance(function,types.FunctionType) and function.__module__ == self.bounded.__name__:
                copied = types.FunctionType(function.__code__, env, function.__name__,
                                            function.__defaults__, function.__closure__)
                copied.__kwdefaults__ = function.__kwdefaults__
                env[name] = copied
        # Explicit globals for every isolated source function below. In
        # particular _member needs C_KEYWORDS, which does not exist in the
        # captured old bounded module. Never copy all globals/classes over the
        # real bounded Field/Record/Interface and emitter implementations.
        env.update({'Path':Path,'ast':ast,'copy':copy,'hashlib':hashlib,
                    'json':json,'math':math,'os':os,'re':re,'stat':stat,
                    'sys':sys,'types':types,'PROFILE':PROFILE,
                    'MAX_SOURCE':MAX_SOURCE,'MAX_WIRE':MAX_WIRE,
                    'MAX_TYPES':MAX_TYPES,'MAX_DEPTH':MAX_DEPTH,
                    'MAX_NODES':MAX_NODES,'MAX_TOTAL_NODES':MAX_TOTAL_NODES,
                    'C_KEYWORDS':C_KEYWORDS,'LANGUAGES':LANGUAGES,
                    '_original_scalar_default':self.bounded.scalar_native_default,
                    '_original_default_expression':self.bounded.scalar_default_expression,
                    '_original_js_default':env['js_default']})
        for name, function in globals().copy().items():
            if isinstance(function,types.FunctionType) and function.__module__ == __name__:
                env[name] = types.FunctionType(function.__code__,env,function.__name__,
                                               function.__defaults__,function.__closure__)
        env.update({'parse_fields':env['_parse_fields'],'native_name':env['_native_name'],
                    'standard_reference':env['_reference'],'standard_semantics':env['_semantic'],
                    'scalar_default_expression':env['_default_expression'],
                    'js_default':env['_js_default'],
                    'native_definition':env['_native_definition']})
        self.env = env
        # The complete Source bytes are authoritative even if ambient files or
        # Python bytecode change. Every actually used pathname is reopened once
        # before publication and compared to its admitted bytes.
        self.original_reader = self.parent.bounded_read
        def fixed_read(path, maximum):
            key = str(Path(path))
            if key in self.captured:
                raw = self.captured[key]
                need(len(raw) <= maximum, 'captured source exceeds its selected bound')
                return raw
            return read_source(Path(path),maximum)
        self.parent.bounded_read = fixed_read

    def load_catalog(self, path, require_identity=True):
        return self.env['_load_impl'](self.parent,Path(path),require_identity)

    def member_maps(self, ordered):
        return _member_maps(ordered,self.bounded)

    def records(self, records, ordered):
        """Six genuine complete record emitters; no product is materialized here."""
        env = self.env; parent = self.parent
        self.member_maps(ordered)
        comment_records = {k:copy.copy(v) for k,v in records.items()}
        for r in comment_records.values():
            r.constants = [dict(k,literal=k['literal'].replace('*/','* /')) for k in r.constants]
        c = env['emit_c'](parent,comment_records,[comment_records[(r.version,r.name)] for r in ordered])
        # C inline text is an owned {size,data} value, not assignable char*.
        # Replace only the exact generated default assignment for each actual
        # string default; ordinary zero initialization and all codecs remain.
        for record in ordered:
            for member in record.members:
                if member.array == 'array' and member.native_default is not None:
                    values = env['_array_default_values'](member.type,member.native_default)
                    need(len(values) == member.count, 'complete fixed-array count differs')
                    old = '  next.' + member.name + '=' + env['_default_expression'](member.type,member.native_default,'c') + ';'
                    new = '\n'.join('  next.' + member.name + '[' + str(index) + ']=' +
                        env['_default_expression'](member.type,value,'c') + ';' for index,value in enumerate(values))
                    begin = c.index('static inline uint32_t ' + record.c_symbol + '_default(')
                    end = c.index('\n}',begin) + 2
                    body = c[begin:end]
                    need(body.count(old) == 1, 'exact owned C fixed-array initializer required')
                    c = c[:begin] + body.replace(old,new) + c[end:]
                if member.type == 'string' and member.native_default is not None:
                    literal = _c_text(member.native_default)
                    old = '  next.' + member.name + '=' + literal + ';'
                    raw = member.native_default.encode('utf-8')
                    new = ('  next.' + member.name + '.size=' + str(len(raw)) + 'u;'
                           'uwvm_bounded_copy(next.' + member.name + '.data,' + literal + ',' + str(len(raw)) + 'u);')
                    begin = c.index('static inline uint32_t ' + record.c_symbol + '_default(')
                    end = c.index('\n}',begin) + 2
                    body = c[begin:end]
                    need(body.count(old) == 1, 'exact one owned C text-default assignment required')
                    c = c[:begin] + body.replace(old,new) + c[end:]
        # Constants are class/module facts. Strip them only from private emission
        # clones, never from original records, canonical IR or native proof.
        # This lets every target use its real constant type including string.
        clones = {key:copy.copy(record) for key,record in records.items()}
        for record in clones.values():
            record.constants = []
        order = [clones[(r.version,r.name)] for r in ordered]
        rust = env['emit_control_rust'](parent,clones,order)
        go = env['emit_control_go'](parent,clones,order)
        as_ = env['emit_control_as'](parent,clones,order)
        js = env['emit_control_js'](parent,records,ordered)
        for record in ordered:
            for member in record.members:
                if member.array != 'array' or member.native_default is None:
                    continue
                values = env['_array_default_values'](member.type,member.native_default)
                field = self.bounded.control_as_member(member.name)
                old = ('for(let i=0;i<' + str(member.count) + ';++i)this.' + field + '.push(' +
                       env['_default_expression'](member.type,member.native_default,'assemblyscript') + ');')
                new = ''.join('this.' + field + '.push(' +
                       env['_default_expression'](member.type,value,'assemblyscript') + ');' for value in values)
                marker = 'export class ' + record.symbol + ' {'
                begin = as_.index(marker);end = as_.find('\nexport class ',begin + len(marker))
                if end < 0:end = len(as_)
                body = as_[begin:end]
                need(body.count(old) == 1, 'exact owned AssemblyScript fixed-array initializer required')
                as_ = as_[:begin] + body.replace(old,new) + as_[end:]
        versions = {r.version for r in ordered}
        need(len(versions) == 1, 'one explicit ROS backend per industrial source bundle')
        version = next(iter(versions))
        go = go.replace('//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1',
                        '//go:build tinygo && (wasm || tinygo.wasm) && ros' + str(version) + ' && !ros' + str(3-version))
        for r in ordered:
            rust_constants = []; go_constants = []; as_constants = []
            for k in r.constants:
                typ = k['type']; value = k['value']; name = k['name']
                if typ == 'string':
                    rust_type = "&'static str"; rust_value = _rust_text(value)
                    go_type = as_type = 'string'; go_value = as_value = json.dumps(value,ensure_ascii=False)
                else:
                    rust_type = parent.SCALARS[typ][3]; go_type = parent.SCALARS[typ][4]; as_type = parent.SCALARS[typ][6]
                    rust_value = env['scalar_default_expression'](typ,value,'rust')
                    go_value = env['scalar_default_expression'](typ,value,'tinygo')
                    as_value = env['scalar_default_expression'](typ,value,'assemblyscript')
                rust_constants.append('pub const '+name+':'+rust_type+'='+rust_value+';')
                # Go constant declarations cannot call math.Float*frombits. Preserve
                # each declared finite float as an exact hexadecimal constant;
                # its typed destination rounds exactly as the original width.
                if typ in ('float32','float64'):
                    import struct
                    numeric = float(value)
                    if typ == 'float32':
                        numeric = struct.unpack('<f',struct.pack('<f',numeric))[0]
                    go_value = numeric.hex()
                go_constants.append('const '+r.symbol+'_'+name+' '+go_type+'='+go_value)
                as_constants.append('static readonly '+name+':'+as_type+'='+as_value+';')
            if rust_constants:
                rust += '\nimpl '+r.symbol+' {'+'\n'.join(rust_constants)+'}\n'
                go += '\n'+'\n'.join(go_constants)+'\n'
                marker = 'export class '+r.symbol+' {'
                need(as_.count(marker) == 1, 'one exact AS record declaration required')
                as_ = as_.replace(marker,marker+'\n'+'\n'.join(as_constants),1)
        # Source names describe reuse, not a CONTROL identity or registry.
        as_ = _rename_as_support(as_)
        as_ = as_.replace('Complete original-control canonical records.','Complete industrial canonical records.')
        return {'bounded_records.h':c,'bounded_records.hpp':env['emit_cpp'](ordered),
                'bounded_records.rs':rust,'bounded_records.mjs':js,
                'bounded_records.go':go,'bounded_records.ts':as_}

    def declarations(self, records, interface):
        """Complete modern native graph; constants remain separate source facts."""
        if interface.kind == 'message':
            rows = []
            def visit(record):
                if any(name == record.name for name,fields in rows):
                    return
                rows.append((record.name, []))
                rows[-1][1].extend(self._fields(record))
                for m in record.members:
                    if m.type not in self.parent.SCALARS and m.type != 'string':
                        visit(records[(record.version,m.type)])
            visit(interface.components['message'])
            return rows
        rows = self.bounded.ros2_interface_schema(self.parent,records,interface)
        by_name = {r.name:r for r in records.values()}
        return [(name,self._fields(by_name[name]) if name in by_name else fields)
                for name,fields in rows]

    def _fields(self, record):
        fields = []
        for m in record.members:
            typ = 'byte' if record.version == 2 and m.native_scalar == 'byte' else m.type
            fields.append((m.name,typ,m.count if m.array == 'array' else 0,m.array == 'array',
                           m.native_string_bound,m.array == 'sequence',m.native_sequence_bound) +
                          (m.native_default if m.native_default is not None else '',m.native_default is not None))
        if not fields:
            fields = [('structure_needs_at_least_one_member','uint8',0,False,0,False,0,'',False)]
        return fields

    def constant_facts(self, ordered):
        return {r.name:r.constants for r in ordered if r.constants}

    def native_message_source_facts(self, ordered):
        """Complete Message declarations for independent installed-IDL pairing.

        This is source data, not a native identity. The separate actual SDK IDL
        parser must retain default presence, including an explicitly empty text
        default, and the compiled proof checks actual types/constructor values.
        """
        for record in ordered:
            need(record.version == 2 and record.kind == 'message',
                 'installed-source admission currently requires ROS2 Messages')
        return self.native_payload_source_facts(ordered)

    def native_payload_source_facts(self, ordered):
        """All original ROS2 payload declarations, including Service/Action parts.

        This does not supply any implicit protocol definition or native hash.
        The separately captured genuine SDK supplies those complete declarations.
        """
        result = {}
        for record in ordered:
            need(record.version == 2 and record.kind in ('message','service','action'),
                 'actual ROS2 Message payload source required')
            fields = [dict(zip(('name','type','array_size','is_array','string_bound',
                'is_sequence','sequence_bound','native_default','has_declared_default'), row))
                for row in self._fields(record)]
            constants = [dict(name=c['name'], type='byte' if c['native_type'] == 'byte' else c['type'],
                              value=c['value']) for c in record.constants]
            result[record.name] = dict(type=record.name, fields=fields, constants=constants)
        return result

    def installed_protocol_module(self):
        """Load only the fixed reviewed native protocol source verifier.

        Its original bytes are part of the proof domain. This trusted operator
        facility is never callable or selectable through a Wasm import.
        """
        path = Path(__file__).parent/'pre_rihs/protocol_source_proof.py'
        raw = read_source(path)
        module = types.ModuleType('_complete_installed_protocol_verifier')
        module.__file__ = str(path)
        exec(compile(raw,str(path),'exec',dont_inherit=True),module.__dict__)
        need(read_source(path) == raw, 'actual protocol source verifier changed')
        return module

    def verify_native_message_source_proof(self, records, ordered, pins, raw, *, require_outputs=True):
        """Admit explicit source proof DATA; this does not establish execution."""
        need(type(raw) is bytes and len(raw) <= MAX_SOURCE, 'bounded original installed-source proof required')
        proof = strict_json(raw)
        need(type(proof) is dict and set(proof) == {'format','scope','domain','source_proof_sha256','outputs','default_authority'} and
             proof['format'] == 'uwvm-ros2-installed-message-source-proof-v1' and proof['default_authority'] == [],
             'closed explicit installed-source proof required')
        domain = proof['domain']
        need(type(domain) is dict and set(domain) == {'profile','catalog_inputs','source_generators','proof_generator','records','installed_idl_facts'} and
             domain['profile'] == PROFILE and canonical(domain['catalog_inputs']) == canonical(pins) and
             canonical(domain['source_generators']) == canonical(self.sources) and
             canonical(domain['records']) == canonical(self.native_message_source_facts(ordered)),
             'proof is not paired to the original complete source/generator domain')
        maker = read_source(Path(__file__).parent/'pre_rihs/source_proof.py')
        need(canonical(domain['proof_generator']) == canonical({'bytes':len(maker),'sha256':hashlib.sha256(maker).hexdigest()}) and
             proof['source_proof_sha256'] == hashlib.sha256(canonical(domain)).hexdigest(),
             'actual installed-source maker or proof digest differs')
        facts = domain['installed_idl_facts']
        exporter = read_source(Path(__file__).parent/'pre_rihs/idl_export.py')
        need(type(facts) is dict and set(facts) == {'format','scope','request','parser_sources','exporter_source','records','default_authority'} and
             facts['format'] == 'uwvm-ros2-installed-message-idl-source-v1' and facts['default_authority'] == [] and
             canonical(facts['exporter_source']) == canonical({'bytes':len(exporter),'sha256':hashlib.sha256(exporter).hexdigest()}) and
             type(facts['records']) is list and len(facts['records']) == len(ordered),
             'complete actual installed-IDL/exporter association required')
        need(type(facts['parser_sources']) is dict and set(facts['parser_sources']) ==
             {'__init__.py','definition.py','grammar.lark','parser.py'}, 'complete actual SDK parser source set required')
        for pin in facts['parser_sources'].values():
            need(type(pin) is dict and set(pin) == {'bytes','sha256'} and type(pin['bytes']) is int and
                 0 <= pin['bytes'] <= 2*1024*1024 and type(pin['sha256']) is str and re.fullmatch('[0-9a-f]{64}',pin['sha256']),
                 'bounded actual SDK parser source role required')
        actual = {}
        for row in facts['records']:
            need(type(row) is dict and set(row) == {'type','fields','constants','installed_idl','annotations'} and
                 type(row['annotations']) is list and len(row['annotations']) <= 8193 and
                 row['type'] not in actual, 'unique complete installed Message required')
            actual[row['type']] = {k:row[k] for k in ('type','fields','constants')}
            pin = row['installed_idl']
            need(type(pin) is dict and set(pin) == {'path','bytes','sha256'} and
                 type(pin['path']) is str and Path(pin['path']).is_absolute() and
                 type(pin['bytes']) is int and 0 <= pin['bytes'] <= 1024*1024 and
                 type(pin['sha256']) is str and re.fullmatch('[0-9a-f]{64}',pin['sha256']),
                 'bounded original installed IDL role required')
        # Python numeric equality treats False==0 and 3==3.0. Source facts are
        # typed JSON declarations; a recomputed public digest cannot make those
        # representations interchangeable.
        need(canonical(actual) == canonical(domain['records']),
             'installed native IDL fields/constants/default presence differ')
        outputs = proof['outputs']
        if require_outputs:
            need(type(outputs) is dict and set(outputs) == {'native_source.hpp','native_source_identity.cc',
                 'native_source_provider.hpp','native_source_types.h'}, 'complete emitted proof source roles required')
            for pin in outputs.values():
                need(type(pin) is dict and set(pin) == {'bytes','sha256'} and type(pin['bytes']) is int and
                     0 < pin['bytes'] <= MAX_SOURCE and type(pin['sha256']) is str and
                     re.fullmatch('[0-9a-f]{64}',pin['sha256']), 'bounded emitted proof source fact required')
        else:
            need(type(outputs) is dict and not outputs, 'pre-emission proof validation requires no output claims')
        return proof

    def constant_proof(self, ordered, version):
        """Actual typed generated constants, not guessed offsets or RIHS values."""
        proof_namespace = 'uwvm_industrial_source_' + hashlib.sha256(canonical(self.constant_facts(ordered))).hexdigest()
        lines = ['/** Cold typed source-constant proof. No Node, socket or authority. */',
                 '#pragma once','#include <string_view>','#include <cstdint>',
                 'namespace ' + proof_namespace + ' {',
                 'inline bool validate() {']
        for r in ordered:
            if r.version != version:
                continue
            for c in r.constants:
                access = r.native + '::' + c['name']
                if c['type'] == 'string':
                    lines.append('if(std::string_view{' + access + '}!=std::string_view{' +
                                 _c_text(c['value']) + ',' + str(len(c['value'].encode('utf-8'))) + '})return false;')
                else:
                    value = self.env['scalar_default_expression'](c['type'],c['value'],'c')
                    lines.append('if(' + access + '!=' + value + ')return false;')
        lines += ['return true;}','}']
        return '\n'.join(lines) + '\n'

    def native(self, records, ordered, interfaces, version):
        """Real generated C++ conversions; complete native proof still required.

        This source does not weaken the old Gateway matcher. An old matcher
        which refuses constants/octet/native standard endpoints must reject;
        Root's independent explicit new-domain provider is a separate dependency.
        """
        env = self.env
        original = env['ros2_interface_schema']
        def schema(parent, all_records, interface):
            return self.declarations(all_records,interface)
        # This API is a serial source-generation object. Save/restore even if
        # complete native emission refuses. No public/global module is modified.
        env['ros2_interface_schema'] = schema
        try:
            text = env['emit_native'](self.parent,records,ordered,interfaces,version)
        finally:
            env['ros2_interface_schema'] = original
        # Preserve byte vs uint8 in every native field table, while canonical
        # byte storage and true native C++ scalar checks remain untouched.
        for root in ordered:
            if root.version != version or root.kind == 'native-primitive':
                continue
            marker = 'struct ' + root.symbol + 'Codec {'
            begin = text.index(marker)
            next_record = text.find('/** Complete official ',begin + len(marker))
            end = len(text) if next_record < 0 else next_record
            block = text[begin:end]
            closure = []
            def visit(item):
                if item in closure:
                    return
                closure.append(item)
                for m in item.members:
                    if m.type not in self.parent.SCALARS and m.type != 'string':
                        visit(records[(item.version,m.type)])
            visit(root)
            if version == 2:
                for index,item in enumerate(closure):
                    fields = []
                    for row in self._fields(item):
                        fields.append('{' + ','.join(json.dumps(x) if isinstance(x,str) else
                                      str(x).lower() if isinstance(x,bool) else str(x) for x in row) + '}')
                    line = 'inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_' + str(index) + '[]={' + ','.join(fields) + '};'
                    pattern = r'^inline static constexpr ::uwvm2::imported::ros::ipc::custom_native_field schema_fields_' + str(index) + r'\[\]=.*$'
                    block,count = re.subn(pattern,lambda _:line,block,flags=re.MULTILINE)
                    need(count == 1, 'one exact native complete field table required')
            text = text[:begin] + block + text[end:]
        # The complete-vendor trait is a new explicit trusted build seam. Root's
        # corresponding descriptor/trait implementation must be compiled with
        # this output; old Gateway headers intentionally cannot accept it.
        proof_namespace = 'uwvm_industrial_source_' + hashlib.sha256(canonical(self.constant_facts(ordered))).hexdigest()
        native_types = {r.native for r in ordered if r.version == version}
        native_types.update('::' + i.name.replace('/','::') + ('Action' if version == 1 and i.kind == 'action' else '')
                            for i in interfaces if i.version == version)
        trait = ['namespace uwvm2::imported::ros::ipc {']
        for actual in sorted(native_types):
            trait += ['template<> struct complete_vendor_native_traits<' + actual + '> {',
                      'static constexpr bool enabled=true;',
                      'static bool constants() noexcept {return ::' + proof_namespace + '::validate();}', '};']
        trait += ['}']
        insertion = '\n#include "native_constants.hpp"\n' + '\n'.join(trait) + '\n'
        at = text.index('namespace uwvm_custom_bounded_native {')
        text = text[:at] + insertion + text[at:]
        for record in ordered:
            if record.version != version:
                continue
            marker = 'struct ' + record.symbol + 'Codec {'
            need(text.count(marker) == 1, 'one real Native codec per complete record required')
            text = text.replace(marker, marker + '\nstatic constexpr bool complete_vendor_profile=true;',1)
        # The factory must call the cold proof before a Node/native endpoint.
        # Codecs also reject altered compile-time constants on each conversion.
        text = text.replace('static bool encoded_size(Native const& value,size_t& out) noexcept {size_t n=0;',
            'static bool encoded_size(Native const& value,size_t& out) noexcept {if(!::uwvm_industrial_source_constants::validate())return false;size_t n=0;')
        text = text.replace('static bool to_record(Native const& value,Record& record) noexcept {',
            'static bool to_record(Native const& value,Record& record) noexcept {if(!::uwvm_industrial_source_constants::validate())return false;')
        text = text.replace('static bool from_record(Record const& record,Native& value) {',
            'static bool from_record(Record const& record,Native& value) {if(!::uwvm_industrial_source_constants::validate())return false;')
        return text.replace('::uwvm_industrial_source_constants::validate()',
            '::uwvm_industrial_source_' + hashlib.sha256(canonical(self.constant_facts(ordered))).hexdigest() + '::validate()')

    def exporter(self, records, ordered, interfaces, *, version, helper_source):
        """Emit a genuine stopped native metadata source; never invoke it.

        Includes are derived from exact source category/name. Root must separately
        admit actual installed generated headers/full MD/link/SDK and original
        exporter ownership before releasing its original SIGSTOP and I gate.
        """
        need(type(helper_source) is bytes and len(helper_source) <= MAX_SOURCE,
             'actual captured native exporter source required')
        source = helper_source.decode('utf-8')
        begin = source.index('namespace\n{\n'); end = source.index('\nint main()\n')
        helper = source[begin:end]
        need(helper.count('namespace\n{\n') == 1, 'one exact native helper namespace required')
        lines = ['/** Industrial native metadata source; no Node, endpoint or grant. */',
                 '#include "tests/ros_custom_types/bounded-v2/native_protocol.h"',
                 '#include <array>','#include <string_view>','#include <stdexcept>',
                 '#include <csignal>','#include <cstdint>',
                 '#if UWVM_CUSTOM_NATIVE_ROS_VERSION != ' + str(version),
                 '#error "Select the exact source-reviewed ROS major"','#endif']
        heads = set()
        for i in interfaces:
            parts = i.name.split('/')
            head = (parts[0] + '/' + parts[-1] + ('Action' if i.kind == 'action' else '') + '.h'
                    if version == 1 else parts[0] + '/' + parts[1] + '/' + self.parent.ros2_header(parts[-1]) + '.hpp')
            heads.add(head)
        lines += ['#include <' + h + '>' for h in sorted(heads)]
        if version == 1:
            lines += ['#include <ros/message_traits.h>','#include <ros/service_traits.h>']
        else:
            lines += ['#include <rosidl_typesupport_cpp/message_type_support.hpp>',
                      '#include <rosidl_typesupport_cpp/service_type_support.hpp>',
                      '#include <rosidl_typesupport_cpp/action_type_support.hpp>']
        warm = []
        if version == 2:
            helper += '\nnamespace {template<class S> void industrial_warm(S const* s){if(!s||!s->get_type_hash_func||!s->get_type_description_func)throw std::runtime_error{"modern native metadata unavailable"};auto h=s->get_type_hash_func(s);auto d=s->get_type_description_func(s);if(!h||h->version!=1||!d)throw std::runtime_error{"native metadata warm failed"};}}\n'
            for number,i in enumerate(interfaces):
                native = '::' + i.name.replace('/','::')
                get = 'rosidl_typesupport_cpp::get_' + i.kind + '_type_support_handle<' + native + '>()'
                warm.append('industrial_warm(' + get + ');')
                for role in i.components:
                    component = native if role == 'message' else native + '::' + role.title()
                    warm.append('industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<' + component + '>());')
                if i.kind == 'action':
                    root = 'a' + str(number)
                    warm.append('auto ' + root + '=' + get + ';')
                    for member,kind,typ in [('goal_service_type_support','service','SendGoalService'),
                                             ('result_service_type_support','service','GetResultService'),
                                             ('cancel_service_type_support','service','CancelGoalService'),
                                             ('feedback_message_type_support','message','FeedbackMessage'),
                                             ('status_message_type_support','message','GoalStatusMessage')]:
                        actual = 'rosidl_typesupport_cpp::get_' + kind + '_type_support_handle<typename ' + native + '::Impl::' + typ + '>()'
                        warm.append('industrial_warm(' + actual + ');if(' + root + '->' + member + '!=' + actual + ')throw std::runtime_error{"actual five-link Action association differs"};')
        lines += ['#include "native_constants.hpp"',helper,'int main() {try {',
                  'if(!::uwvm_industrial_source_constants::validate())throw std::runtime_error{"native source constants differ"};',
                  'if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};',
                  'bounded_native_io::gate(\'I\');',
                  'bounded_native_io::print(output(),"{\\"format\\":\\"uwvm-ros-industrial-native-v1\\",\\"ros_version\\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\\"source_profile\\":\\"' + PROFILE + '\\",\\"constant_schema_sha256\\":\\"' + hashlib.sha256(canonical(self.constant_facts(ordered))).hexdigest() + '\\",\\"types\\":{");']
        stop_index = next(n for n,line in enumerate(lines) if line.startswith('if(::raise(SIGSTOP)'))
        lines[stop_index:stop_index] = warm
        for index,i in enumerate(interfaces):
            native = '::' + i.name.replace('/','::')
            if index:
                lines.append('bounded_native_io::print(output(),\',\');')
            lines += ['quoted(' + json.dumps(_native_metadata_key(i,interfaces)) + ');']
            if version == 1:
                trait = '::ros::service_traits' if i.kind == 'service' else '::ros::message_traits'
                root = native + ('Action' if i.kind == 'action' else '')
                lines += ['bounded_native_io::print(output(),":{\\"native_identity\\":");',
                          'quoted(' + trait + '::MD5Sum<' + root + '>::value());',
                          'bounded_native_io::print(output(),",\\"components\\":{");']
            else:
                kind = i.kind
                lines += ['bounded_native_io::print(output(),":{\\"official\\":");',
                          'metadata(rosidl_typesupport_cpp::get_' + kind + '_type_support_handle<' + native + '>());',
                          'bounded_native_io::print(output(),",\\"components\\":{");']
            roles = {'message':('message',),'service':('request','response'),'action':('goal','result','feedback')}[i.kind]
            for n,role in enumerate(roles):
                part = native if role == 'message' else (native + role.title() if version == 1 and i.kind == 'action' else native + '::' + role.title())
                lines.append('component<' + part + '>(' + json.dumps(role) + ',' + ('true' if n else 'false') + ');')
            lines.append('bounded_native_io::print(output(),"}}");')
        lines += ['bounded_native_io::print(output(),"}}\\n");return 0;',
                  '}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\\n");return 1;}}']
        return ('\n'.join(lines) + '\n').replace('::uwvm_industrial_source_constants::validate()',
            '::uwvm_industrial_source_' + hashlib.sha256(canonical(self.constant_facts(ordered))).hexdigest() + '::validate()')

    def _modern_graph(self, rows, root):
        scalar = {'int8':2,'uint8':3,'int16':4,'uint16':5,'int32':6,'uint32':7,
                  'int64':8,'uint64':9,'float32':10,'float64':11,'bool':15,'byte':16,'string':17}
        result = {}
        for name,fields in rows:
            values = []
            for f in fields:
                label,typ,fixed,is_array,text_bound,is_sequence,sequence_bound = f[:7]
                kind = scalar.get(typ,1)
                if typ == 'string' and text_bound:
                    kind = 21
                if is_array:
                    kind += 48
                elif is_sequence:
                    kind += 96 if sequence_bound else 144
                values.append({'name':label,'type_id':kind,
                               'capacity':fixed if is_array else sequence_bound if is_sequence else 0,
                               'string_capacity':text_bound,
                               'nested_type_name':'' if typ in scalar else typ,
                               'default_value':f[7] if len(f) >= 8 else ''})
            need(name not in result, 'duplicate native expected record')
            result[name] = {'type':name,'fields':values}
        need(root in result, 'complete original native root required')
        return {'root':result[root],'references':{k:v for k,v in result.items() if k != root}}

    def _verify_modern(self, actual, expected):
        need(type(actual) is dict and set(actual) == {'native_identity','root','references'},
             'complete modern native metadata required; no fallback')
        identity = self.bounded.identity(self.parent,2,actual['native_identity'],True,'native root')
        need(canonical(actual['root']) == canonical(expected['root']) and type(actual['references']) is list,
             'native original root fields/defaults/types differ')
        references = {}
        for row in actual['references']:
            need(type(row) is dict and type(row.get('type')) is str and row['type'] not in references,
                 'complete unique native reference graph required')
            references[row['type']] = row
        need(canonical(references) == canonical(expected['references']), 'native complete dependency/protocol graph differs')
        return identity

    def _verify_installed_message(self, actual, expected, proof, *, protocol=False):
        need(type(actual) is dict and set(actual) == {'native_identity','root','references',
             'identity_provenance','native_baked_hash_available','source_proof_sha256'} and
             actual['identity_provenance'] == ('PRE_RIHS_INSTALLED_IDL_AND_TYPED_NATIVE_PROTOCOL_DERIVED_RIHS01' if protocol else
                 'PRE_RIHS_INSTALLED_IDL_AND_TYPED_NATIVE_INTROSPECTION_DERIVED_RIHS01') and
             actual['native_baked_hash_available'] is False and
             actual['source_proof_sha256'] == proof['source_proof_sha256'],
             'explicit genuine pre-RIHS installed-source provenance required')
        return self._verify_modern({k:actual[k] for k in ('native_identity','root','references')}, expected)

    def _signature(self, text, package):
        """ROS1 complete definition comparison, including source constants."""
        result = []
        for original in text.splitlines():
            line = _uncomment(original)
            if not line:
                continue
            constant = re.fullmatch(r'([A-Za-z][A-Za-z0-9_]*)\s+([A-Z][A-Z0-9_]*)\s*=\s*(.+)',line)
            if constant:
                typ,name,literal = constant.groups()
                wire = 'int8' if typ == 'byte' else 'uint8' if typ == 'char' else typ
                need(wire in self.parent.SCALARS or wire == 'string', 'unknown ROS1 constant type')
                value = _text_literal(literal) if wire == 'string' else self.bounded.scalar_native_default(self.parent,wire,literal)
                result.append(('constant',typ,name,value))
                continue
            field = re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(\[([0-9]*)\])?\s+([A-Za-z][A-Za-z0-9_]*)',line)
            need(field is not None, 'unknown complete ROS1 native definition grammar')
            typ,array,count,name = field.groups()
            if typ == 'byte':
                typ = 'int8'
            elif typ == 'char':
                typ = 'uint8'
            if typ not in self.parent.SCALARS and typ != 'string':
                typ = _reference(self.parent,1,package,typ)
            result.append(('field',typ,name,'sequence' if array and not count else 'array' if array else '',int(count or 0)))
        return result

    def _verify_ros1(self, actual, record, records):
        need(type(actual) is dict and set(actual) == {'type','native_identity','definition'} and
             actual['type'] == record.name and type(actual['definition']) is str and
             len(actual['definition'].encode()) <= MAX_SOURCE,
             'complete original ROS1 component type/Definition required')
        identity = self.bounded.identity(self.parent,1,actual['native_identity'],True,record.name)
        sections = {}; current = record.name; lines = []
        for line in actual['definition'].splitlines():
            if line.startswith('MSG: '):
                need(current not in sections, 'duplicate actual ROS1 definition section')
                sections[current] = '\n'.join(lines);current = line[5:].strip();lines = []
            elif line and all(c == '=' for c in line):
                continue
            else:
                lines.append(line)
        need(current not in sections, 'duplicate final native definition section')
        sections[current] = '\n'.join(lines)
        expected = {}
        def visit(r):
            if r.kind == 'native-primitive' or r.name in expected:
                return
            expected[r.name] = self._signature(r.original_section,r.name.split('/')[0])
            for m in r.members:
                if m.type not in self.parent.SCALARS and m.type != 'string':
                    visit(records[(1,m.type)])
        visit(record)
        need(set(sections) == set(expected), 'missing/extra complete ROS1 native dependency')
        for name,body in sections.items():
            need(self._signature(body,name.split('/')[0]) == expected[name],
                 'original ROS1 fields/constants differ: ' + name)
        return identity

    def bind(self, catalog_path, native_raw, *, installed_source_proof=None):
        """Copy full independently exported identities after source comparison.

        A matching metadata document alone is never execution/SDK/permission
        evidence. Root retains the genuine stopped exporter, full source/MD/link,
        original SDK headers/libraries and complete Native value witnesses.
        """
        need(type(native_raw) is bytes and len(native_raw) <= MAX_SOURCE, 'bounded native metadata DATA required')
        records,ordered,interfaces,pins = self.load_catalog(catalog_path,False)
        actual = strict_json(native_raw)
        versions = {i.version for i in interfaces}
        need(len(versions) == 1, 'one actual backend required')
        version = next(iter(versions))
        proof = None
        protocol_graphs = None
        native_format = 'uwvm-ros-industrial-native-v1'
        keys = {'format','ros_version','source_profile','constant_schema_sha256','types'}
        if installed_source_proof is not None:
            need(version == 2 and type(installed_source_proof) is bytes and len(installed_source_proof) <= MAX_SOURCE,
                 'explicit bounded ROS2 installed source proof required')
            selected = strict_json(installed_source_proof)
            if type(selected) is dict and selected.get('format') == 'uwvm-ros2-installed-protocol-source-proof-v1':
                module = self.installed_protocol_module()
                proof = module.verify(self,records,ordered,interfaces,pins,installed_source_proof)
                _,_,protocol_graphs = module.snapshot(self,proof['domain']['installed_idl_facts'])
                native_format = module.NATIVE_FORMAT
            else:
                need(all(i.kind == 'message' for i in interfaces), 'Message proof cannot admit Service/Action roots')
                proof = self.verify_native_message_source_proof(records,ordered,pins,installed_source_proof)
                native_format = 'uwvm-ros-industrial-native-pre-rihs-source-v1'
            keys.add('source_proof_sha256')
            need(type(actual) is dict and actual.get('format') == native_format and
                 actual.get('source_proof_sha256') == proof['source_proof_sha256'],
                 'explicit source proof requires its original pre-RIHS exporter; no modern downgrade')
        else:
            # Modern TypeDescription omits the presence distinction for an
            # explicitly empty text default. Only the separately selected,
            # complete original-IDL path can attest that declaration.
            need(not any(m.native_default == '' for r in ordered for m in r.members),
                 'explicit empty text default requires independent original native IDL source proof')
        need(type(actual) is dict and set(actual) == keys and
             actual['format'] == native_format and
             type(actual['ros_version']) is int and actual['ros_version'] == version and
             actual['source_profile'] == PROFILE and actual['constant_schema_sha256'] ==
             hashlib.sha256(canonical(self.constant_facts(ordered))).hexdigest() and
             type(actual['types']) is dict and set(actual['types']) == {_native_metadata_key(i,interfaces) for i in interfaces},
             'actual exporter/source-domain/whole-inventory association differs')
        document = strict_json(read_source(Path(catalog_path)))
        by_name = {(row['type'], {'.msg':'message','.srv':'service','.action':'action'}[Path(row['file']).suffix]):row for row in document['types']}
        for i in interfaces:
            row = by_name[(i.name,i.kind)]
            need(row.get('native_identity') is None and not row.get('component_identities'),
                 'bind only an original unbound catalog; preserve earlier bindings')
            value = actual['types'][_native_metadata_key(i,interfaces)]
            need(type(value) is dict and set(value) == ({'official','components'} if version == 2 else {'native_identity','components'}) and
                 type(value['components']) is dict and set(value['components']) == set(i.components),
                 'whole native root/component role set required')
            components = {}
            for role,record in i.components.items():
                if version == 2:
                    # Use an actual Message interface view only to select this
                    # complete payload graph; no new name/identity is invented.
                    payload = copy.copy(i);payload.kind = 'message';payload.components = {'message':record}
                    expected = self._modern_graph(protocol_graphs[record.name] if protocol_graphs is not None else
                                                  self.declarations(records,payload),record.name)
                    components[role] = (self._verify_installed_message(value['components'][role],expected,proof,protocol=protocol_graphs is not None)
                                        if proof else self._verify_modern(value['components'][role],expected))
                else:
                    components[role] = self._verify_ros1(value['components'][role],record,records)
            if version == 2:
                root = self._modern_graph(protocol_graphs[i.name] if protocol_graphs is not None else self.declarations(records,i),i.name)
                row['native_identity'] = (self._verify_installed_message(value['official'],root,proof,protocol=protocol_graphs is not None)
                                          if proof else self._verify_modern(value['official'],root))
            else:
                row['native_identity'] = self.bounded.identity(self.parent,1,value['native_identity'],True,i.name)
            if i.kind == 'message':
                need(row['native_identity'] == components['message'], 'native Message root/payload identity differs')
            else:
                row['component_identities'] = components
        return document, {'profile':PROFILE,'status':'SOURCE_BOUND_NOT_NATIVE_OR_HARDWARE_QUALIFIED',
                          'default_authority':[],'source_inputs':pins,
                          'native_metadata':{'bytes':len(native_raw),'sha256':hashlib.sha256(native_raw).hexdigest()},
                          'constant_schema_sha256':actual['constant_schema_sha256']}

    def products(self, path, *, guest_only, helper_source):
        """Prepare complete Source products in memory. No compiler or ROS is run."""
        records,ordered,interfaces,pins = self.load_catalog(path,not guest_only)
        products = self.records(records,ordered)
        products.update({'bounded_interfaces.h':self.env['emit_c_interfaces'](interfaces),
                         'bounded_interfaces.hpp':self.env['emit_cpp_interfaces'](interfaces)})
        managed,template_pins = self.env['emit_managed_interfaces'](self.parent,interfaces)
        products.update(managed)
        version_set = {r.version for r in ordered}
        need(len(version_set) == 1, 'one explicit backend per source revision')
        version = next(iter(version_set))
        products['native_constants.hpp'] = self.constant_proof(ordered,version)
        products['native_identity.cc'] = self.exporter(records,ordered,interfaces,version=version,
                                                       helper_source=helper_source)
        if not guest_only:
            manifest = self.env['emit_manifest'](self.parent,records,ordered,[i for i in interfaces if i.registered])
            for record in ordered:
                marker = 'inline constexpr custom_topic_descriptor custom_record_' + record.symbol + '{'
                start = manifest.index(marker); end = manifest.index('};',start)
                declaration = manifest[start:end]
                # No use of official_standard or original_control. The explicit
                # new trusted domain is checked by the paired current headers.
                need(declaration.endswith(',' + str(record.minimum)), 'exact complete descriptor tail required')
                manifest = manifest[:start] + declaration + ',false,false,true' + manifest[end:]
            products['manifest.hpp'] = manifest
            products['native_ros' + str(version) + '.hpp'] = self.native(records,ordered,interfaces,version)
        proof = self.env['summary'](interfaces,pins)
        proof.update({'format':PROFILE,'status':'GENERATED_SOURCE_ONLY_NOT_COMPILED_OR_EXECUTED',
                      'source_generator_pair':self.sources,'source_inputs':pins,
                      'implemented_record_outputs':list(LANGUAGES),'default_authority':[],
                      'member_map':self.member_maps(ordered),'declared_constants':self.constant_facts(ordered),
                      'declared_empty_default_native_binding':'MODERN_METADATA_REFUSED; EXPLICIT_INSTALLED_IDL_MESSAGE_SOURCE_PROOF_REQUIRED',
                      'declared_defaults':{r.name:{m.name:m.native_default for m in r.members if m.native_default is not None}
                                           for r in ordered if any(m.native_default is not None for m in r.members)},
                      'native_registry_emitted':not guest_only,
                      'required_native_domain_provider':'paired complete_vendor descriptor/Native trait/source-metadata admission; requires actual Root implementation and full rebuild; old refusal is preserved',
                      'records':[{'ros_version':r.version,'type':r.name,'codec_identity':r.codec_identity,
                                  'minimum_wire_bytes':r.minimum,'maximum_wire_bytes':r.maximum,
                                  'cartesian_maximum_wire_bytes':r.cartesian_maximum,'aggregate_wire_bytes':r.aggregate_wire_bytes,
                                  'expanded_nodes':r.nodes,'native_identity':r.identity} for r in ordered],
                      'facade_template_inputs':template_pins,
                      'outputs':{n:{'bytes':len(s.encode()),'sha256':hashlib.sha256(s.encode()).hexdigest()} for n,s in products.items()}})
        need(set(products) and all(len(s.encode()) <= MAX_SOURCE for s in products.values()), 'finite complete emitted Source bound')
        # Pin the captured sources to their actual named sources again. An A/B/A
        # alternate input cannot become the compiled input or a claimed proof.
        for name,raw in self.captured.items():
            need(read_source(Path(name)) == raw, 'source changed since actual capture: ' + name)
        current = read_source(Path(path))
        need({'bytes':len(current),'sha256':hashlib.sha256(current).hexdigest()} == pins['catalog'],
             'original catalog changed after resolution')
        for row in pins['definitions']:
            raw = read_source(Path(path).resolve().parent/row['file'])
            need({'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()} ==
                 {'bytes':row['bytes'],'sha256':row['sha256']}, 'complete definition changed after resolution')
        return products,proof

    def generate(self, path, output, *, guest_only=True, helper_source):
        """Write a fresh finite source revision, retaining any refused partials."""
        path = Path(path); output = Path(output)
        need(output.is_absolute() and not output.exists() and not output.is_symlink() and
             output.parent.resolve(strict=True) == output.parent, 'fresh canonical caller-owned Source output required')
        products,proof = self.products(path,guest_only=guest_only,helper_source=helper_source)
        proof['native_exporter_helper_source'] = {'bytes':len(helper_source),'sha256':hashlib.sha256(helper_source).hexdigest()}
        proof['source_template_inputs'] = {n:{'bytes':len(b),'sha256':hashlib.sha256(b).hexdigest()}
                                           for n,b in self.templates.items()}
        output.mkdir(mode=0o700)
        def write(name, raw):
            need('/' not in name and '\\' not in name and name not in ('.','..') and len(raw) <= MAX_SOURCE,
                 'one bounded generated source leaf required')
            fd = os.open(output/name,os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC,0o600)
            try:
                with os.fdopen(fd,'wb') as stream:
                    fd = None
                    stream.write(raw)
            finally:
                if fd is not None:
                    os.close(fd)
        for name,source in products.items():
            write(name,source.encode('utf-8'))
        for name,raw in self.captured.items():
            need(read_source(Path(name)) == raw, 'original source changed before final source receipt')
        need(hashlib.sha256(read_source(path)).hexdigest() == proof['source_inputs']['catalog']['sha256'],
             'catalog changed before source receipt')
        for row in proof['source_inputs']['definitions']:
            need(hashlib.sha256(read_source(path.resolve().parent/row['file'])).hexdigest() == row['sha256'],
                 'original definition changed before source receipt')
        write('generation.vendor.json',canonical(proof))
        return proof
