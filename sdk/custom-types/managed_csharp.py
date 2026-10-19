"""Optional reflection-free C# complete canonical records over admitted IR.

Called only by managed_profile's fixed trusted-source registry after its one
actual bounded.load_catalog. No catalog/source parsing, native metadata, ROS
resource, runtime import bridge, ambient plugin or permission is added here.
"""
import json
import re


SCALARS = {
    'bool': (1, False, 'bool'),
    'uint8': (1, False, 'byte'), 'int8': (1, True, 'sbyte'),
    'uint16': (2, False, 'ushort'), 'int16': (2, True, 'short'),
    'uint32': (4, False, 'uint'), 'int32': (4, True, 'int'),
    'uint64': (8, False, 'ulong'), 'int64': (8, True, 'long'),
    'float32': (4, False, 'float'), 'float64': (8, False, 'double'),
}
# The public value type prefix cannot equal a legal native field: actual ROS1
# and ROS2 field grammars both require a leading letter. Codec metadata/methods
# live in separate classes, so no extra native-field refusal or renaming occurs.
NAMESPACE = 'Uwvm.RosWasm.Generated'
OUTPUT = 'csharp/BoundedRecords.cs'
OBJECT_MEMBERS = {'Equals', 'ReferenceEquals', 'GetHashCode', 'GetType',
                  'ToString', 'MemberwiseClone', 'Finalize'}


def source_types(ordered):
    return {record.symbol: {'value': '_' + record.symbol,
                           'codec': record.symbol + 'Codec',
                           'namespace': NAMESPACE + '.BoundedRecords'}
            for record in ordered}


def element_type(record, member, records):
    return SCALARS[member.type][2] if member.type in SCALARS else 'string' if member.type == 'string' else '_' + records[(record.version, member.type)].symbol


def element_minimum(record, member, records):
    return SCALARS[member.type][0] if member.type in SCALARS else 4 if member.type == 'string' else records[(record.version, member.type)].minimum


RUNTIME = '''// Generated complete canonical records; no native ABI, CDR or ROS grant.
#nullable enable
using System;
using _Codecs = global::Uwvm.RosWasm.Codecs;
namespace Uwvm.RosWasm.Generated;

public static class BoundedRecords {
    private static byte[] OwnedWire(byte[] value, int minimum, int maximum) {
        if (value == null || value.Length < minimum || value.Length > maximum)
            throw new ArgumentException("complete bounded wire required");
        return (byte[])value.Clone();
    }
    private static void WriteText(_Codecs.Writer writer, string value, int maximum) {
        if (value == null || value.Length > maximum || value.IndexOf('\\0') >= 0)
            throw new ArgumentException("bounded custom string without NUL required");
        byte[] bytes = _Codecs.Utf8(value);
        if (bytes.Length > maximum) throw new ArgumentException("custom UTF8 byte bound");
        writer.Unsigned((ulong)bytes.Length, 4).Raw(bytes);
    }
    private static string ReadText(_Codecs.Reader reader, int maximum) {
        ulong count = reader.Unsigned(4);
        if (count > (ulong)maximum || count > (ulong)reader.Remaining)
            throw new FormatException("custom UTF8 extent");
        string value = _Codecs.Text(reader.Take((int)count));
        if (value.IndexOf('\\0') >= 0) throw new FormatException("NUL in custom string");
        return value;
    }
    private static int ReadCount(_Codecs.Reader reader, int maximum, int elementMinimum) {
        ulong count = reader.Unsigned(4);
        if (count > (ulong)maximum || (elementMinimum != 0 &&
                count > (ulong)reader.Remaining / (ulong)elementMinimum))
            throw new FormatException("sequence count/remaining extent");
        return (int)count;
    }
    private static bool ReadBool(_Codecs.Reader reader) {
        ulong value = reader.Unsigned(1);
        if (value > 1) throw new FormatException("canonical bool must be0/1");
        return value == 1;
    }
    private static void WriteFloat32(_Codecs.Writer writer, float value) {
        if (!float.IsFinite(value)) throw new ArgumentException("finite float32 required");
        // uint preserves exactly the actual32 bits before widening to ulong.
        writer.Unsigned(unchecked((uint)BitConverter.SingleToInt32Bits(value)), 4);
    }
    private static float ReadFloat32(_Codecs.Reader reader) {
        float value = BitConverter.Int32BitsToSingle(unchecked((int)reader.Unsigned(4)));
        if (!float.IsFinite(value)) throw new FormatException("nonfinite canonical float32");
        return value;
    }
'''


def semantics(bounded, record, indent, captured):
    rule = bounded.standard_semantics(record)
    def scalar(name):
        index = next(i for i, field in enumerate(record.members) if field.name == name)
        return 'part' + str(index) if captured else 'value.@' + name
    if rule in ('normalized-time', 'normalized-duration'):
        field = scalar('nanosec' if record.version == 2 else 'nsec')
        # Exact official subsecond slots are signed/unsigned32, both fit long.
        return [indent + 'if ((long)' + field + ' < 0 || (long)' + field + ' >= 1000000000L)',
                indent + '    throw new ArgumentException("normalized subsecond field required");']
    if rule == 'joint-state-optional-equal-length-arrays':
        sizes = [scalar(name) + '.Length' for name in ('name', 'position', 'velocity', 'effort')]
        bad = ' || '.join('(' + a + ' != 0 && ' + b + ' != 0 && ' + a + ' != ' + b + ')' for i, a in enumerate(sizes) for b in sizes[i + 1:])
        return [indent + 'if (' + bad + ')',
                indent + '    throw new ArgumentException("nonempty JointState arrays must have equal length");']
    return []


def emit_csharp(parent, bounded, records, ordered, interfaces):
    """Produce source only from the same actual complete resolved record objects."""
    names = {'BoundedRecords'}
    for record in ordered:
        if records.get((record.version, record.name)) is not record or not re.fullmatch(r'Ros[12][A-Za-z0-9_]+', record.symbol):
            raise parent.GenerationError('actual complete bounded record symbol required for C#')
        for symbol in ('_' + record.symbol, record.symbol + 'Codec'):
            if symbol in names:
                raise parent.GenerationError('C# generated type collision')
            names.add(symbol)
        for field in record.members:
            if not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*', field.name):
                raise parent.GenerationError('C# exact admitted native field grammar required')
            if field.type not in SCALARS and field.type != 'string' and (record.version, field.type) not in records:
                raise parent.GenerationError('new scalar needs reviewed C# emitter')
    lines = [RUNTIME]
    for record in ordered:
        name, value_type = record.symbol, '_' + record.symbol
        lines += ['    public sealed class ' + value_type + ' {']
        for field in record.members:
            typ = element_type(record, field, records)
            if field.array:
                initial = 'new ' + typ + '[' + str(field.count if field.array == 'array' else 0) + ']'
            elif field.native_default is not None:
                initial = bounded.scalar_default_expression(field.type,field.native_default,'csharp')
            elif field.type == 'string':
                initial = '""'
            elif field.type not in SCALARS:
                initial = 'new ' + typ + '()'
            else:
                initial = 'false' if field.type == 'bool' else '0'
            hiding = 'new ' if field.name in OBJECT_MEMBERS else ''
            lines += ['        public ' + hiding + typ + ('[]' if field.array else '') + ' @' + field.name + ' = ' + initial + ';']
        lines += ['        public ' + value_type + '() {']
        for field in record.members:
            if field.array == 'array' and (field.type == 'string' or field.type not in SCALARS):
                initial = '""' if field.type == 'string' else 'new ' + element_type(record, field, records) + '()'
                lines += ['            for (int index=0; index<' + str(field.count) + '; ++index) this.@' + field.name + '[index] = ' + initial + ';']
        lines += ['        }', '    }', '',
                  '    public sealed class ' + name + 'Codec : global::Uwvm.RosWasm.ICodec<' + value_type + '> {',
                  '        public static readonly ' + name + 'Codec Instance = new ' + name + 'Codec();',
                  '        private ' + name + 'Codec() {}',
                  '        public const int ROS_VERSION = ' + str(record.version) + ';',
                  '        public const string NATIVE_TYPE = ' + json.dumps(record.name) + ';',
                  '        public const string? NATIVE_IDENTITY = ' + ('null' if record.identity is None else json.dumps(record.identity)) + ';',
                  '        public const string CODEC_IDENTITY = ' + json.dumps(record.codec_identity) + ';',
                  '        public const int MIN_WIRE_BYTES = ' + str(record.minimum) + ', MAX_WIRE_BYTES = ' + str(record.maximum) + ';',
                  '        public byte[] Encode(' + value_type + ' value) {',
                  '            var writer = new _Codecs.Writer(MAX_WIRE_BYTES);',
                  '            Write' + name + '(writer, value);',
                  '            byte[] wire = writer.Finish();',
                  '            if (wire.Length < MIN_WIRE_BYTES || wire.Length > MAX_WIRE_BYTES)',
                  '                throw new ArgumentException("complete encoded extent");',
                  '            return wire;', '        }',
                  '        public ' + value_type + ' Decode(byte[] bytes) {',
                  '            var reader = new _Codecs.Reader(OwnedWire(bytes, MIN_WIRE_BYTES, MAX_WIRE_BYTES));',
                  '            ' + value_type + ' value = Read' + name + '(reader);',
                  '            reader.End();', '            return value;', '        }',
                  '    }', '',
                  '    private static void Write' + name + '(_Codecs.Writer writer, ' + value_type + ' value) {',
                  '        if (value == null) throw new ArgumentNullException(nameof(value));']
        for index, field in enumerate(record.members):
            typ, value = element_type(record, field, records), 'part' + str(index)
            lines += ['        ' + typ + ('[]' if field.array else '') + ' ' + value + ' = value.@' + field.name + ';']
            indent = '        '
            if field.array:
                lines += [indent + 'if (' + value + ' == null || ' + value + '.Length ' + ('!=' if field.array == 'array' else '>') + ' ' + str(field.count) + ')',
                          indent + '    throw new ArgumentException("fixed/sequence count bound");']
                if field.array == 'sequence':
                    lines += [indent + 'writer.Unsigned((ulong)' + value + '.Length, 4);']
                lines += [indent + 'for (int index=0; index<' + value + '.Length; ++index) {']
                indent += '    '
                lines += [indent + typ + ' element = ' + value + '[index];']
                value = 'element'
            if field.type == 'bool':
                call = 'writer.Unsigned(' + value + ' ? 1UL : 0UL, 1);'
            elif field.type == 'float32':
                call = 'WriteFloat32(writer, ' + value + ');'
            elif field.type == 'float64':
                call = 'writer.Float64(' + value + ');'
            elif field.type in SCALARS:
                width, signed, _ = SCALARS[field.type]
                # Narrow negative values mask to their actual width BEFORE the
                # ulong widening; Writer.Unsigned then retains exact wire bits.
                bit_type = {1:'byte', 2:'ushort', 4:'uint', 8:'ulong'}[width]
                argument = 'unchecked((' + bit_type + ')' + value + ')' if signed else value
                call = 'writer.Unsigned(' + argument + ', ' + str(width) + ');'
            elif field.type == 'string':
                call = 'WriteText(writer, ' + value + ', ' + str(field.string_bound) + ');'
            else:
                call = 'Write' + records[(record.version, field.type)].symbol + '(writer, ' + value + ');'
            lines += [indent + call]
            if field.array:
                lines += ['        }']
        lines += semantics(bounded, record, '        ', captured=True)
        lines += ['    }', '', '    private static ' + value_type + ' Read' + name + '(_Codecs.Reader reader) {',
                  '        var value = new ' + value_type + '();']
        for field in record.members:
            typ = element_type(record, field, records)
            if field.type == 'bool':
                expression = 'ReadBool(reader)'
            elif field.type == 'float32':
                expression = 'ReadFloat32(reader)'
            elif field.type == 'float64':
                expression = 'reader.Float64()'
            elif field.type in SCALARS:
                width, signed, _ = SCALARS[field.type]
                expression = 'unchecked((' + typ + ')reader.Unsigned(' + str(width) + '))'
            elif field.type == 'string':
                expression = 'ReadText(reader, ' + str(field.string_bound) + ')'
            else:
                expression = 'Read' + records[(record.version, field.type)].symbol + '(reader)'
            target = 'value.@' + field.name
            if field.array:
                count = str(field.count) if field.array == 'array' else 'ReadCount(reader, ' + str(field.count) + ', ' + str(element_minimum(record, field, records)) + ')'
                lines += ['        ' + target + ' = new ' + typ + '[' + count + '];',
                          '        for (int index=0; index<' + target + '.Length; ++index) ' + target + '[index] = ' + expression + ';']
            else:
                lines += ['        ' + target + ' = ' + expression + ';']
        lines += semantics(bounded, record, '        ', captured=False)
        lines += ['        return value;', '    }', '']
    lines += ['}']
    return {OUTPUT: '\n'.join(lines) + '\n'}
