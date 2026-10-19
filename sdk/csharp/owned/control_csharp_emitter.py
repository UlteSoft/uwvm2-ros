"""Disjoint control-only successor of the exact complete managed_csharp body.

No schema parser or handwritten field projection is introduced. The operator
supplies captured trusted base bytes; counted template substitutions retain its
complete scalar/default/array/record read/write branches. Not run in authoring.
"""
import hashlib
import json
import sys
import types

BASE_SHA256='16f4d958745b25154f55fb19a7b1ac0cce684f15271a38d6344256148deb1b81'

def successor(raw):
    if hashlib.sha256(raw).hexdigest()!=BASE_SHA256:
        raise ValueError('exact independently reviewed complete C# base emitter required')
    text=raw.decode('utf-8')
    changes=[]
    def once(old,new,count=1):
        nonlocal text
        if text.count(old)!=count:raise ValueError('C# base template anchor/count changed')
        text=text.replace(old,new);changes.append({'before':old,'after':new,'count':count})
    once("else 'string' if member.type == 'string' else", "else ('byte[]' if member.string_encoding == 'raw_bytes' else 'string') if member.type == 'string' else")
    once('global::Uwvm.RosWasm.Codecs;', 'global::Uwvm.RosWasm.Owned.ControlWire;')
    once("NAMESPACE = 'Uwvm.RosWasm.Generated'", "NAMESPACE = 'Uwvm.RosWasm.Generated.Control'")
    once('namespace Uwvm.RosWasm.Generated;', 'namespace Uwvm.RosWasm.Generated.Control;')
    once("initial = 'new ' + typ + '[' + str(field.count if field.array == 'array' else 0) + ']'",
         "initial = ('new byte[' + str(field.count if field.array == 'array' else 0) + '][]') if field.type == 'string' and field.string_encoding == 'raw_bytes' else 'new ' + typ + '[' + str(field.count if field.array == 'array' else 0) + ']'" )
    once("elif field.type == 'string':\n                initial = '\"\"'", "elif field.type == 'string':\n                initial = 'Array.Empty<byte>()' if field.string_encoding == 'raw_bytes' else '\"\"'")
    once("initial = '\"\"' if field.type == 'string' else 'new ' + element_type(record, field, records) + '()'",
         "initial = ('Array.Empty<byte>()' if field.string_encoding == 'raw_bytes' else '\"\"') if field.type == 'string' else 'new ' + element_type(record, field, records) + '()'")
    once("lines += ['        public ' + value_type + '() {']", """for constant in record.constants:
            lines += ['        public const ' + SCALARS[constant['type']][2] + ' @' + constant['name'] + ' = ' + bounded.scalar_default_expression(constant['type'],constant['value'],'csharp') + ';']
        lines += ['        public ' + value_type + '() {']""")
    once("'        public const int MIN_WIRE_BYTES = '", "'        public const ulong CARTESIAN_WIRE_BYTES = ' + str(record.cartesian_maximum) + 'UL;',\n                  '        public const int AGGREGATE_WIRE_BYTES = ' + str(record.aggregate_wire_bytes) + ';',\n                  '        public const int MIN_WIRE_BYTES = '")
    once("'        if (value == null) throw new ArgumentNullException(nameof(value));']", "'        if (value == null) throw new ArgumentNullException(nameof(value));',\n                  '        int recordStart = writer.Position;']")
    once("call = 'WriteText(writer, ' + value + ', ' + str(field.string_bound) + ');'", "call = ('WriteRaw' if field.string_encoding == 'raw_bytes' else 'WriteText') + '(writer, ' + value + ', ' + str(field.string_bound) + ');'")
    once("expression = 'ReadText(reader, ' + str(field.string_bound) + ')'", "expression = ('ReadRaw' if field.string_encoding == 'raw_bytes' else 'ReadText') + '(reader, ' + str(field.string_bound) + ')'")
    once("'        var value = new ' + value_type + '();']", "'        int recordStart = reader.Position;', '        var value = new ' + value_type + '();']")
    once("lines += semantics(bounded, record, '        ', captured=True)", "lines += semantics(bounded, record, '        ', captured=True)\n        lines += ['        CheckExtent(writer.Position-recordStart, ' + str(record.minimum) + ', ' + str(record.maximum) + ');']")
    once("lines += semantics(bounded, record, '        ', captured=False)", "lines += semantics(bounded, record, '        ', captured=False)\n        lines += ['        CheckExtent(reader.Position-recordStart, ' + str(record.minimum) + ', ' + str(record.maximum) + ');']")
    # Jagged raw-byte arrays require allocation in C#'s outer-before-inner order.
    once("lines += ['        ' + target + ' = new ' + typ + '[' + count + '];',", "lines += ['        ' + target + ' = ' + ('new byte[' + count + '][]' if field.type == 'string' and field.string_encoding == 'raw_bytes' else 'new ' + typ + '[' + count + ']') + ';',")
    once("    private static int ReadCount", """    private static void CheckExtent(int count, int minimum, int maximum) {
        if (count < minimum || count > maximum) throw new FormatException("nested complete record extent");
    }
    private static void WriteRaw(_Codecs.Writer writer, byte[] value, int maximum) {
        if (value == null || value.Length > maximum) throw new ArgumentException("bounded raw byte string");
        writer.Unsigned((ulong)value.Length,4).Raw(value);
    }
    private static byte[] ReadRaw(_Codecs.Reader reader, int maximum) {
        ulong n=reader.Unsigned(4);
        if(n>(ulong)maximum || n>(ulong)reader.Remaining)throw new FormatException("raw byte string extent");
        return reader.Take((int)n);
    }
    private static int ReadCount""")
    return text.encode('utf-8'),changes

def emit(parent,bounded,records,ordered,interfaces,base_raw):
    if len(ordered)!=32 or any(r.profile!=bounded.CONTROL_PROFILE for r in ordered):
        raise parent.GenerationError('complete original32 CONTROL IR required')
    if any(m.string_encoding not in ('utf8','raw_bytes') for r in ordered for m in r.members if m.type=='string'):
        raise parent.GenerationError('unimplemented byte/string domain')
    # Keep actual CONTROL semantics: bounded.standard_semantics currently returns
    # empty for this profile. Do not silently invent a different wire identity.
    if any(bounded.standard_semantics(r) for r in ordered):
        raise parent.GenerationError('new CONTROL semantic rule requires emitter review')
    raw,changes=successor(base_raw)
    name='_uwvm_owned_csharp_complete_emitter'
    if name in sys.modules:raise ValueError('private emitter namespace already occupied')
    module=types.ModuleType(name);module.__file__='captured-managed_csharp-control-successor.py';sys.modules[name]=module
    try:
        exec(compile(raw,module.__file__,'exec'),module.__dict__)
        products=module.emit_csharp(parent,bounded,records,ordered,interfaces)
    finally:sys.modules.pop(name,None)
    records_body=products.pop('csharp/BoundedRecords.cs')
    if products:raise ValueError('unexpected C# base product')
    facts=bounded.control_service_facts(interfaces)
    if len(facts)!=7:raise ValueError('all seven service roots required')
    roots=['// Generated from the same complete original32 IR. Names/IDs grant no authority.',
        'using Uwvm.RosWasm.Owned;','using R=Uwvm.RosWasm.Generated.Control.BoundedRecords;',
        'namespace Uwvm.RosWasm.Generated.Control;','public static class ControlRoots {']
    by_name={r.name:r for r in ordered}
    for fact in facts:
        q,s=by_name[fact['request']['type']],by_name[fact['response']['type']]
        roots += ['    public static ServiceDefinition<R._'+q.symbol+',R._'+s.symbol+'> '+fact['symbol']+'() => new ServiceDefinition<R._'+q.symbol+',R._'+s.symbol+'>('+json.dumps(fact['native_type'])+','+json.dumps(fact['codec_identity'])+',R.'+q.symbol+'Codec.Instance,R.'+s.symbol+'Codec.Instance,'+str(q.maximum)+','+str(s.maximum)+');']
    roots+=['}']
    return {'BoundedRecords.cs':records_body,'ControlRoots.cs':'\n'.join(roots)+'\n'}, {'base_sha256':BASE_SHA256,'successor_source_sha256':hashlib.sha256(raw).hexdigest(),'counted_template_changes':changes,'services':facts}
