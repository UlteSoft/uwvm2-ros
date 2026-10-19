"""Optional complete Python/Java/C# record codecs over the existing bounded IR.

The original bounded generator and its six record outputs are never called or
changed here. This module does not discover native identities, emit a native
registry, create ROS resources or load a ROS library. Every language consumes
the same already resolved Field/Record objects and canonical codec identities.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import keyword
from pathlib import Path
import re
import sys


SCALARS = {
    'bool': (1, False, 'boolean'),
    'uint8': (1, False, 'int'), 'int8': (1, True, 'byte'),
    'uint16': (2, False, 'int'), 'int16': (2, True, 'short'),
    'uint32': (4, False, 'long'), 'int32': (4, True, 'int'),
    # Java long deliberately stores all uint64 bits, including bit63 and MAX.
    'uint64': (8, False, 'long'), 'int64': (8, True, 'long'),
    'float32': (4, False, 'float'), 'float64': (8, False, 'double'),
}
JAVA_RESERVED = set('abstract assert boolean break byte case catch char class const '
    'continue default do double else enum extends final finally float for goto if '
    'implements import instanceof int interface long native new package private '
    'protected public return short static strictfp super switch synchronized this '
    'throw throws transient try void volatile while true false null _'.split())
# JLS17 3.8/3.9 and8.3: contextual keywords are legal FieldDeclarator
# Identifiers here; every generated class TypeIdentifier has its RosN prefix.
MAX_OUTPUT_BYTES = 64 * 1024 * 1024


def validate_ir(parent, bounded, records, ordered, interfaces, languages):
    """Refuse an unsupported optional-language name/type; never rename a field.

    Syntax, native declarations, dependency closure, bounds and identities are
    authoritative decisions of bounded.load_catalog, not a second parser here.
    The defensive checks below prevent a future unsupported scalar/semantic
    rule from silently acquiring an incomplete optional codec.
    """
    if not isinstance(languages, (list, tuple)) or not languages or len(set(languages)) != len(languages):
        raise parent.GenerationError('select unique optional record languages explicitly')
    if any(language not in EMITTERS for language in languages):
        raise parent.GenerationError('optional emitter is not implemented: ' + repr(languages))
    if len(ordered) != len(records) or len({r.symbol for r in ordered}) != len(ordered):
        raise parent.GenerationError('complete resolved unique record order required')
    for record in ordered:
        if records.get((record.version, record.name)) is not record:
            raise parent.GenerationError('record order must refer to the actual resolved IR')
        if not re.fullmatch(r'UWVMROS1_[0-9a-f]{64}', record.codec_identity):
            raise parent.GenerationError('existing resolved canonical identity required')
        if not 0 <= record.minimum <= record.maximum <= bounded.MAX_WIRE:
            raise parent.GenerationError('resolved wire bound is invalid')
        if bounded.standard_semantics(record) not in ('', 'normalized-time',
                'normalized-duration', 'joint-state-optional-equal-length-arrays'):
            raise parent.GenerationError('optional complete-value semantic rule is not implemented')
        for member in record.members:
            # ROS1 permits upper-case native names. Do not overwrite generated
            # metadata or Java's codec object when such a real field is present.
            if ('python' in languages or 'java' in languages) and member.name in {
                    'ROS_VERSION', 'NATIVE_TYPE', 'NATIVE_IDENTITY',
                    'CODEC_IDENTITY', 'MIN_WIRE_BYTES', 'MAX_WIRE_BYTES'}:
                raise parent.GenerationError('native field collides with optional codec metadata: ' + member.name)
            if 'java' in languages and member.name == 'CODEC':
                raise parent.GenerationError('native field collides with Java codec object')
            if 'python' in languages and member.name in {'encode', 'decode'}:
                # Both are already reserved by today's base parser. Keep this
                # selected-emitter guard if a future admitted IR relaxes it.
                raise parent.GenerationError('native field collides with Python codec method: ' + member.name)
            if 'python' in languages and (keyword.iskeyword(member.name) or keyword.issoftkeyword(member.name)):
                raise parent.GenerationError('native field is a Python keyword: ' + member.name)
            if 'java' in languages and member.name in JAVA_RESERVED:
                raise parent.GenerationError('native field is a Java keyword: ' + member.name)
            if member.type not in SCALARS and member.type != 'string' and (record.version, member.type) not in records:
                raise parent.GenerationError('optional scalar/record is not implemented: ' + member.type)
            if member.type in parent.SCALARS and member.type not in SCALARS:
                raise parent.GenerationError('new authoritative scalar needs a reviewed optional emitter')
            if member.array not in ('', 'array', 'sequence') or (member.array and not 1 <= member.count <= bounded.MAX_NODES):
                raise parent.GenerationError('resolved array shape is invalid')
            if member.native_default is not None:
                if member.array:raise parent.GenerationError('array defaults require a complete optional emitter')
                try:
                    for language in languages:
                        bounded.scalar_default_expression(member.type,member.native_default,language)
                except ValueError as error:
                    raise parent.GenerationError('optional scalar default is not implemented: '+str(error)) from None
    # Service and Action component records are complete codecs, not an invented
    # RPC facade, native protocol family or new ownership implementation.
    if any(role not in ('message', 'request', 'response', 'goal', 'result', 'feedback')
           for interface in interfaces for role in interface.components):
        raise parent.GenerationError('unknown interface component role')


def element_minimum(record, member, records):
    if member.type in SCALARS:
        return SCALARS[member.type][0]
    if member.type == 'string':
        return 4
    return records[(record.version, member.type)].minimum


def python_type(record, member, records):
    # Native field grammar starts with a letter, so these private aliases
    # cannot be shadowed by class fields while resolving postponed annotations.
    base = ('_py_bool' if member.type == 'bool' else '_py_float' if member.type in ('float32', 'float64') else
            '_py_int' if member.type in SCALARS else '_py_str' if member.type == 'string' else
            '_record_' + records[(record.version, member.type)].symbol)
    return '_py_list[' + base + ']' if member.array else base


def python_default(bounded, record, member, records):
    scalar = (bounded.scalar_default_expression(member.type,member.native_default,'python') if member.native_default is not None else
              'False' if member.type == 'bool' else '0.0' if member.type in ('float32', 'float64') else
              '0' if member.type in SCALARS else "''" if member.type == 'string' else
              records[(record.version, member.type)].symbol + '()')
    if member.array == 'sequence':
        return '_field(default_factory=lambda: [])'
    if member.array == 'array':
        return '_field(default_factory=lambda: [' + scalar + ' for _ in range(' + str(member.count) + ')])'
    if member.type not in SCALARS and member.type != 'string':
        # A lambda resolves the type in module globals, not class locals where
        # a legal ROS1 field can have the same spelling as a dependency type.
        return '_field(default_factory=lambda: ' + records[(record.version, member.type)].symbol + '())'
    return scalar


PYTHON_RUNTIME = '''# Generated complete canonical records; source-only until actually tested.
from __future__ import annotations
from dataclasses import dataclass as _dataclass, field as _field
from builtins import (classmethod as _classmethod, bool as _py_bool,
    float as _py_float, int as _py_int, str as _py_str, list as _py_list)
from typing import ClassVar as _ClassVar
from roswasm.codecs import Writer as _Writer, Reader as _Reader

def _write_text(writer, value, maximum):
    if type(value) is not str or len(value) > maximum or '\\0' in value:
        raise ValueError('bounded custom UTF-8 string without NUL required')
    raw = value.encode('utf-8', 'strict')
    if len(raw) > maximum:
        raise ValueError('custom UTF-8 byte bound')
    writer.integer(len(raw), 4).raw(raw)

def _read_text(reader, maximum):
    count = reader.integer(4)
    if count > maximum:
        raise ValueError('custom UTF-8 byte bound')
    value = reader.take(count).decode('utf-8', 'strict')
    if '\\0' in value:
        raise ValueError('NUL in custom string')
    return value

def _read_count(reader, maximum, element_minimum):
    count = reader.integer(4)
    # Reader.data/position are the current SDK's actual owned-byte view/cursor.
    # The complete top-level wire was already bounded before Reader creation.
    if count > maximum or (element_minimum and count >
            (len(reader.data) - reader.position) // element_minimum):
        raise ValueError('sequence count/remaining extent')
    return count

def _read_bool(reader):
    value = reader.integer(1)
    if value > 1:
        raise ValueError('canonical bool must be0/1')
    return bool(value)

def _wire(value, minimum, maximum):
    if type(value) is not bytes or not minimum <= len(value) <= maximum:
        raise ValueError('complete bounded immutable wire required')
    return value
'''


def python_semantics(bounded, record, value, indent, captured=False):
    rule = bounded.standard_semantics(record)
    if rule.startswith('normalized-'):
        name = 'nanosec' if record.version == 2 else 'nsec'
        field = ('_part' + str(next(i for i, m in enumerate(record.members) if m.name == name))
                 if captured else value + '.' + name)
        return [indent + 'if not 0 <= ' + field + ' < 1000000000:',
                indent + "    raise ValueError('normalized subsecond field required')"]
    if rule == 'joint-state-optional-equal-length-arrays':
        sizes = [('_count' + str(next(i for i, m in enumerate(record.members) if m.name == name)))
                 if captured else 'len(' + value + '.' + name + ')'
                 for name in ('name', 'position', 'velocity', 'effort')]
        return [indent + '_sizes = [' + ', '.join(sizes) + ']',
                indent + 'if len({n for n in _sizes if n}) > 1:',
                indent + "    raise ValueError('nonempty JointState arrays must have equal length')"]
    return []


def emit_python(parent, bounded, records, ordered, interfaces):
    lines = [PYTHON_RUNTIME]
    for record in ordered:
        name = record.symbol
        lines += ['@_dataclass', 'class ' + name + ':',
                  '    ROS_VERSION: _ClassVar[_py_int] = ' + str(record.version),
                  '    NATIVE_TYPE: _ClassVar[_py_str] = ' + repr(record.name),
                  '    NATIVE_IDENTITY: _ClassVar[_py_str | None] = ' + repr(record.identity),
                  '    CODEC_IDENTITY: _ClassVar[_py_str] = ' + repr(record.codec_identity),
                  '    MIN_WIRE_BYTES: _ClassVar[_py_int] = ' + str(record.minimum),
                  '    MAX_WIRE_BYTES: _ClassVar[_py_int] = ' + str(record.maximum)]
        for member in record.members:
            lines += ['    ' + member.name + ': ' + python_type(record, member, records) + ' = ' + python_default(bounded, record, member, records)]
        lines += ['', '    def encode(self):', '        writer = _Writer(' + str(record.maximum) + ')',
                  '        _write_' + name + '(writer, self)', '        wire = writer.finish()',
                  '        return _wire(wire, ' + str(record.minimum) + ', ' + str(record.maximum) + ')', '',
                  '    @_classmethod', '    def decode(cls, wire):',
                  '        if cls is not ' + name + ':', "            raise TypeError('exact generated codec class required')",
                  '        reader = _Reader(_wire(wire, ' + str(record.minimum) + ', ' + str(record.maximum) + '))',
                  '        value = _read_' + name + '(reader)', '        reader.end()', '        return value', '',
                  '_record_' + name + ' = ' + name, '',
                  'def _write_' + name + '(writer, value):',
                  '    if type(value) is not ' + name + ':', "        raise TypeError('exact generated record required')"]
        for index, member in enumerate(record.members):
            value = '_part' + str(index)
            lines += ['    ' + value + ' = value.' + member.name]
            indent = '    '
            if member.array:
                lines += [indent + 'if type(' + value + ') not in (list, tuple):', indent + "    raise TypeError('array value required')",
                          indent + '_count' + str(index) + ' = len(' + value + ')',
                          indent + 'if _count' + str(index) + (' != ' if member.array == 'array' else ' > ') + str(member.count) + ':',
                          indent + "    raise ValueError('fixed/sequence count bound')"]
                if member.array == 'sequence':
                    lines += [indent + 'writer.integer(_count' + str(index) + ', 4)']
                # Fixed range keeps work bounded even if a caller mutates a list.
                lines += [indent + 'for _index in range(_count' + str(index) + '):']
                indent += '    '
                # Capture each actual element once, including bool's type and
                # integer conversion; never validate one list read then emit
                # a second read after application mutation.
                lines += [indent + '_element = ' + value + '[_index]']
                value = '_element'
            if member.type == 'bool':
                lines += [indent + 'if type(' + value + ') is not bool:', indent + "    raise TypeError('bool field required')",
                          indent + 'writer.integer(int(' + value + '), 1)']
            elif member.type in ('float32', 'float64'):
                lines += [indent + 'writer.' + ('f32' if member.type == 'float32' else 'f64') + '(' + value + ')']
            elif member.type in SCALARS:
                width, signed, _ = SCALARS[member.type]
                lines += [indent + 'writer.integer(' + value + ', ' + str(width) + ', ' + str(signed) + ')']
            elif member.type == 'string':
                lines += [indent + '_write_text(writer, ' + value + ', ' + str(member.string_bound) + ')']
            else:
                lines += [indent + '_write_' + records[(record.version, member.type)].symbol + '(writer, ' + value + ')']
        # Validate the exact scalars/counts just encoded, not a later reread of
        # mutable application attributes; every nested writer does the same.
        lines += python_semantics(bounded, record, 'value', '    ', captured=True)
        if not record.members:
            lines += ['    pass']
        lines += ['', 'def _read_' + name + '(reader):', '    value = ' + name + '()']
        for index, member in enumerate(record.members):
            if member.type == 'bool':
                expression = '_read_bool(reader)'
            elif member.type in ('float32', 'float64'):
                expression = 'reader.' + ('f32()' if member.type == 'float32' else 'f64()')
            elif member.type in SCALARS:
                width, signed, _ = SCALARS[member.type]
                expression = 'reader.integer(' + str(width) + ', ' + str(signed) + ')'
            elif member.type == 'string':
                expression = '_read_text(reader, ' + str(member.string_bound) + ')'
            else:
                expression = '_read_' + records[(record.version, member.type)].symbol + '(reader)'
            if member.array:
                count = str(member.count)
                if member.array == 'sequence':
                    count = '_read_count(reader, ' + count + ', ' + str(element_minimum(record, member, records)) + ')'
                expression = '[' + expression + ' for _ in range(' + count + ')]'
            lines += ['    value.' + member.name + ' = ' + expression]
        lines += python_semantics(bounded, record, 'value', '    ')
        lines += ['    return value', '']
    return {'bounded_records.py': '\n'.join(lines) + '\n'}


JAVA_RUNTIME = '''package org.uwvm.roswasm.generated;

import org.uwvm.roswasm.Codec;
import org.uwvm.roswasm.Codecs;

/** Generated complete canonical records; no CDR/native registration or ROS authority. */
public final class BoundedRecords {
    private BoundedRecords() {}
    private static void writeText(Codecs.Writer writer, String value, int maximum) {
        if (value == null || value.length() > maximum || value.indexOf('\\0') >= 0)
            throw new IllegalArgumentException("bounded custom string without NUL required");
        byte[] bytes = Codecs.utf8(value);
        if (bytes.length > maximum) throw new IllegalArgumentException("custom UTF-8 byte bound");
        writer.integer(bytes.length, 4, false).raw(bytes);
    }
    private static String readText(Codecs.Reader reader, int maximum) {
        long count = reader.integer(4, false);
        if (count > maximum || count > reader.remaining()) throw new IllegalArgumentException("custom UTF-8 extent");
        String value = Codecs.text(reader.take((int)count));
        if (value.indexOf('\\0') >= 0) throw new IllegalArgumentException("NUL in custom string");
        return value;
    }
    private static int readCount(Codecs.Reader reader, int maximum, int elementMinimum) {
        long count = reader.integer(4, false);
        if (count > maximum || (elementMinimum != 0 && count > reader.remaining()/elementMinimum))
            throw new IllegalArgumentException("sequence count/remaining extent");
        return (int)count;
    }
    private static boolean readBool(Codecs.Reader reader) {
        long value = reader.integer(1, false);
        if (value > 1) throw new IllegalArgumentException("canonical bool must be0/1");
        return value == 1;
    }
    private static byte[] ownedWire(byte[] value, int minimum, int maximum) {
        if (value == null || value.length < minimum || value.length > maximum)
            throw new IllegalArgumentException("complete bounded wire required");
        return value.clone();
    }
'''


def java_element(record, member, records):
    if record.profile == 'uwvm-ros-original-node-control-c-cpp-v1' and member.type == 'string' and member.string_encoding == 'raw_bytes':
        return 'byte[]'
    return SCALARS[member.type][2] if member.type in SCALARS else 'String' if member.type == 'string' else records[(record.version, member.type)].symbol


def java_semantics(bounded, record, value, indent, captured=False):
    rule = bounded.standard_semantics(record)
    if rule.startswith('normalized-'):
        name = 'nanosec' if record.version == 2 else 'nsec'
        field = ('part' + str(next(i for i, m in enumerate(record.members) if m.name == name))
                 if captured else value + '.' + name)
        return [indent + 'if (' + field + ' < 0 || ' + field + ' >= 1000000000L)',
                indent + '    throw new IllegalArgumentException("normalized subsecond field required");']
    if rule == 'joint-state-optional-equal-length-arrays':
        sizes = [('part' + str(next(i for i, m in enumerate(record.members) if m.name == n)) + '.length')
                 if captured else value + '.' + n + '.length'
                 for n in ('name', 'position', 'velocity', 'effort')]
        bad = ' || '.join('(' + a + ' != 0 && ' + b + ' != 0 && ' + a + ' != ' + b + ')' for i, a in enumerate(sizes) for b in sizes[i+1:])
        return [indent + 'if (' + bad + ')', indent + '    throw new IllegalArgumentException("nonempty JointState arrays must have equal length");']
    return []


JAVA_CONTROL_RUNTIME = """
    private static void writeRaw(Codecs.Writer writer, byte[] value, int maximum) {
        if (value == null || value.length > maximum) throw new IllegalArgumentException("bounded raw bytes");
        writer.integer(value.length, 4, false).raw(value);
    }
    private static byte[] readRaw(Codecs.Reader reader, int maximum) {
        long count = reader.integer(4, false);
        if (count > maximum || count > reader.remaining()) throw new IllegalArgumentException("bounded raw extent");
        return reader.take((int)count);
    }
"""

def emit_java(parent, bounded, records, ordered, interfaces):
    control = bool(ordered) and all(r.profile == bounded.CONTROL_PROFILE for r in ordered)
    if any(r.profile == bounded.CONTROL_PROFILE for r in ordered) and not control:
        raise parent.GenerationError('control Java records require one complete profile')
    lines = [JAVA_RUNTIME + (JAVA_CONTROL_RUNTIME if control else '')]
    for record in ordered:
        name = record.symbol
        lines += ['    public static final class ' + name + ' {',
                  '        public static final int ROS_VERSION = ' + str(record.version) + ';',
                  '        public static final String NATIVE_TYPE = ' + json.dumps(record.name) + ';',
                  '        public static final String NATIVE_IDENTITY = ' + ('null' if record.identity is None else json.dumps(record.identity)) + ';',
                  '        public static final String CODEC_IDENTITY = ' + json.dumps(record.codec_identity) + ';',
                  '        public static final int MIN_WIRE_BYTES = ' + str(record.minimum) + ', MAX_WIRE_BYTES = ' + str(record.maximum) + ';']
        if control:
            lines += ['        public static final long CARTESIAN_WIRE_BYTES = ' + ('0x%016xL' % record.cartesian_maximum) + ';',
                      '        public static final int AGGREGATE_WIRE_BYTES = ' + str(record.aggregate_wire_bytes) + ';']
            for constant in record.constants:
                if constant['name'] in {m.name for m in record.members} | {'ROS_VERSION','NATIVE_TYPE','NATIVE_IDENTITY','CODEC_IDENTITY','MIN_WIRE_BYTES','MAX_WIRE_BYTES','CARTESIAN_WIRE_BYTES','AGGREGATE_WIRE_BYTES','CODEC'}:
                    raise parent.GenerationError('control Java constant/metadata collision')
                lines += ['        public static final ' + SCALARS[constant['type']][2] + ' ' + constant['name'] + ' = ' + bounded.scalar_default_expression(constant['type'],constant['value'],'java') + ';']
        for member in record.members:
            typ = java_element(record, member, records)
            if member.array:
                expression = 'new ' + typ.removesuffix('[]') + '[' + str(member.count if member.array == 'array' else 0) + ']' + ('[]' if typ.endswith('[]') else '')
            elif member.native_default is not None:
                expression = bounded.scalar_default_expression(member.type,member.native_default,'java')
            elif member.type == 'string':
                expression = 'new byte[0]' if control and member.string_encoding == 'raw_bytes' else '""'
            elif member.type not in SCALARS:
                expression = 'new ' + typ + '()'
            else:
                expression = 'false' if member.type == 'bool' else '0'
            lines += ['        public ' + typ + ('[]' if member.array else '') + ' ' + member.name + ' = ' + expression + ';']
        lines += ['        public ' + name + '() {']
        for member in record.members:
            if member.array == 'array' and (member.type == 'string' or member.type not in SCALARS):
                expression = ('new byte[0]' if control and member.string_encoding == 'raw_bytes' else '""') if member.type == 'string' else 'new ' + java_element(record, member, records) + '()'
                lines += ['            for (int i=0; i<' + str(member.count) + '; ++i) this.' + member.name + '[i] = ' + expression + ';']
        lines += ['        }', '        public static final Codec<' + name + '> CODEC = new Codec<' + name + '>() {',
                  '            public byte[] encode(' + name + ' value) {',
                  '                Codecs.Writer writer = new Codecs.Writer(MAX_WIRE_BYTES);',
                  '                write' + name + '(writer, value);', '                byte[] wire = writer.finish();',
                  '                if (wire.length < MIN_WIRE_BYTES || wire.length > MAX_WIRE_BYTES)',
                  '                    throw new IllegalArgumentException("complete encoded extent");',
                  '                return wire;', '            }',
                  '            public ' + name + ' decode(byte[] bytes) {',
                  '                Codecs.Reader reader = new Codecs.Reader(ownedWire(bytes, MIN_WIRE_BYTES, MAX_WIRE_BYTES));',
                  '                ' + name + ' value = read' + name + '(reader);', '                reader.end();',
                  '                return value;', '            }', '        };', '    }', '',
                  '    private static void write' + name + '(Codecs.Writer writer, ' + name + ' value) {',
                  '        if (value == null) throw new IllegalArgumentException("nonnull complete record required");']
        if control: lines += ['        int startPosition = writer.position();']
        for index, member in enumerate(record.members):
            typ = java_element(record, member, records);value = 'part' + str(index)
            lines += ['        ' + typ + ('[]' if member.array else '') + ' ' + value + ' = value.' + member.name + ';']
            indent = '        '
            if member.array:
                lines += [indent + 'if (' + value + ' == null || ' + value + '.length ' + ('!=' if member.array == 'array' else '>') + ' ' + str(member.count) + ')',
                          indent + '    throw new IllegalArgumentException("fixed/sequence count bound");']
                if member.array == 'sequence':
                    lines += [indent + 'writer.integer(' + value + '.length, 4, false);']
                lines += [indent + 'for (int i=0; i<' + value + '.length; ++i) {'];indent += '    ';value += '[i]'
            if member.type == 'bool':
                call = 'writer.integer(' + value + ' ? 1 : 0, 1, false);'
            elif member.type in ('float32', 'float64'):
                call = 'writer.' + ('f32' if member.type == 'float32' else 'f64') + '(' + value + ');'
            elif member.type in SCALARS:
                width, signed, _ = SCALARS[member.type]
                call = 'writer.integer(' + value + ', ' + str(width) + ', ' + str(signed).lower() + ');'
            elif member.type == 'string':
                call = ('writeRaw' if control and member.string_encoding == 'raw_bytes' else 'writeText') + '(writer, ' + value + ', ' + str(member.string_bound) + ');'
            else:
                call = 'write' + records[(record.version, member.type)].symbol + '(writer, ' + value + ');'
            lines += [indent + call]
            if member.array:
                lines += ['        }']
        lines += java_semantics(bounded, record, 'value', '        ', captured=True)
        if control: lines += ['        if (writer.position()-startPosition < ' + str(record.minimum) + ' || writer.position()-startPosition > ' + str(record.maximum) + ') throw new IllegalArgumentException("nested complete extent");']
        lines += ['    }', '', '    private static ' + name + ' read' + name + '(Codecs.Reader reader) {']
        if control: lines += ['        int startRemaining = reader.remaining();']
        lines += ['        ' + name + ' value = new ' + name + '();']
        for index, member in enumerate(record.members):
            typ = java_element(record, member, records)
            if member.type == 'bool':
                expression = 'readBool(reader)'
            elif member.type in ('float32', 'float64'):
                expression = 'reader.' + ('f32()' if member.type == 'float32' else 'f64()')
            elif member.type in SCALARS:
                width, signed, _ = SCALARS[member.type]
                expression = ('(' + typ + ')' if typ != 'long' else '') + 'reader.integer(' + str(width) + ', ' + str(signed).lower() + ')'
            elif member.type == 'string':
                expression = ('readRaw' if control and member.string_encoding == 'raw_bytes' else 'readText') + '(reader, ' + str(member.string_bound) + ')'
            else:
                expression = 'read' + records[(record.version, member.type)].symbol + '(reader)'
            target = 'value.' + member.name
            if member.array:
                count = str(member.count) if member.array == 'array' else 'readCount(reader, ' + str(member.count) + ', ' + str(element_minimum(record, member, records)) + ')'
                lines += ['        ' + target + ' = new ' + typ.removesuffix('[]') + '[' + count + ']' + ('[]' if typ.endswith('[]') else '') + ';',
                          '        for (int i=0; i<' + target + '.length; ++i) ' + target + '[i] = ' + expression + ';']
            else:
                lines += ['        ' + target + ' = ' + expression + ';']
        lines += java_semantics(bounded, record, 'value', '        ')
        if control: lines += ['        if (startRemaining-reader.remaining() < ' + str(record.minimum) + ' || startRemaining-reader.remaining() > ' + str(record.maximum) + ') throw new IllegalArgumentException("nested complete extent");']
        lines += ['        return value;', '    }', '']
    lines += ['}']
    return {'org/uwvm/roswasm/generated/BoundedRecords.java': '\n'.join(lines) + '\n'}


def emit_csharp(parent, bounded, records, ordered, interfaces):
    # The fixed adjacent reviewed source is pinned by generate before loading;
    # no catalog or environment chooses a module. Retire this private module
    # after emission so repeated explicit source generations do not reuse it.
    name = '_uwvm_optional_managed_csharp'
    module = _trusted_module(name, Path(__file__).resolve().with_name('managed_csharp.py'))
    try:
        return module.emit_csharp(parent, bounded, records, ordered, interfaces)
    finally:
        if sys.modules.get(name) is module:
            del sys.modules[name]


# Extension points are trusted source, not a catalog-selected plugin mechanism.
# Further languages require separately reviewed source/API/output admission.
EMITTERS = {'python': emit_python, 'java': emit_java, 'csharp': emit_csharp}
OUTPUTS = {'python': {'bounded_records.py'},
           'java': {'org/uwvm/roswasm/generated/BoundedRecords.java'},
           'csharp': {'csharp/BoundedRecords.cs'}}


def generate(parent, bounded, catalog: Path, output: Path, *, languages,
             require_native_identity=False):
    """Emit optional sources to a fresh revision using one authoritative load.

    Preparation catalogs deliberately lack native identities. Requiring their
    existing strict syntax is optional; neither route qualifies those identities
    against an installed native SDK or enables native registration.
    """
    if type(require_native_identity) is not bool:
        raise parent.GenerationError('native identity syntax selection must be boolean')
    if not isinstance(languages, (tuple, list)) or not languages or not all(type(x) is str for x in languages):
        raise parent.GenerationError('select optional languages explicitly')
    languages = tuple(languages)
    catalog, output = Path(catalog), Path(output)
    sdk = Path(parent.__file__).resolve().parents[1]
    if Path(bounded.__file__).resolve() != sdk / 'custom-types/bounded_profile.py':
        raise parent.GenerationError('coherent authoritative bounded module required')
    if output.exists() or output.is_symlink() or output.resolve().is_relative_to(sdk):
        raise parent.GenerationError('fresh managed output outside the SDK source tree required')
    source_files = {'optional_generator': Path(__file__), 'schema_generator': Path(parent.__file__),
                    'bounded_generator': Path(bounded.__file__)}
    if 'python' in languages:
        source_files['python_codecs'] = sdk / 'python/roswasm/codecs.py'
    if 'java' in languages:
        source_files['java_codec_interface'] = sdk / 'java/src/main/java/org/uwvm/roswasm/Codec.java'
        source_files['java_codecs'] = sdk / 'java/src/main/java/org/uwvm/roswasm/Codecs.java'
    if 'csharp' in languages:
        source_files['csharp_emitter'] = Path(__file__).resolve().with_name('managed_csharp.py')
        source_files['csharp_codec_interface'] = sdk / 'csharp/RosWasm/Result.cs'
        source_files['csharp_codecs'] = sdk / 'csharp/RosWasm/Codecs.cs'
    sources = {}
    for role, path in source_files.items():
        raw = parent.bounded_read(path, parent.MAX_SOURCE_BYTES)
        sources[role] = {'path': str(path.resolve()), 'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}
    records, ordered, interfaces, pins = bounded.load_catalog(parent, catalog, require_native_identity)
    validate_ir(parent, bounded, records, ordered, interfaces, languages)
    products = {}
    for language in languages:
        emitted = EMITTERS[language](parent, bounded, records, ordered, interfaces)
        if set(emitted) != OUTPUTS[language] or any(type(body) is not str for body in emitted.values()) or products.keys() & emitted.keys():
            raise parent.GenerationError('optional emitter must produce its exact disjoint source roles')
        products.update(emitted)
    if sum(len(body.encode('utf-8')) for body in products.values()) > MAX_OUTPUT_BYTES:
        raise parent.GenerationError('optional generated source output exceeds64MiB')
    proof = {'format_version': 1, 'kind': 'optional-managed-bounded-record-sources-v1',
             'status': 'GENERATED_OPTIONAL_RECORD_SOURCE_ONLY', 'languages': list(languages),
             'language_inventory_is_extensible': True, 'source_inputs': pins,
             'generator_and_foundation_sources': sources,
             'schema_summary': bounded.summary(interfaces, pins),
             'records': [{'ros_version': r.version, 'type': r.name, 'symbol': r.symbol,
                          'native_identity': r.identity, 'adapter_codec_identity': r.codec_identity,
                          'minimum_wire_bytes': r.minimum, 'maximum_wire_bytes': r.maximum,
                          'expanded_nodes': r.nodes, 'depth': r.depth,
                          'native_defaults': {m.name: m.native_default for m in r.members if m.native_default is not None},
                          'complete_value_semantics': bounded.standard_semantics(r)} for r in ordered],
             'outputs': {name: {'bytes': len(body.encode('utf-8')), 'sha256': hashlib.sha256(body.encode('utf-8')).hexdigest()} for name, body in products.items()},
             'native_identity_syntax_required': require_native_identity,
             'native_qualification': 'NOT_PERFORMED', 'native_registry_emitted': False,
             'implicit_permissions': [], 'RPC_facades_emitted': False,
             'historical_six_record_emitters_called': False,
             'compiler_execution': 'NOT_RUN', 'native_execution': 'NOT_RUN'}
    if 'csharp' in languages:
        # Public C# value names avoid all admitted native-field spellings;
        # codec metadata lives separately, without changing native/canonical IR.
        proof['csharp_source_types'] = {r.symbol: {
            'value': '_' + r.symbol, 'codec': r.symbol + 'Codec',
            'namespace': 'Uwvm.RosWasm.Generated.BoundedRecords'} for r in ordered}
    # All trusted generator/SDK and original schema bytes must remain identical
    # before the first output write. The immutable IR is not reconstructed here.
    for source in sources.values():
        raw = parent.bounded_read(Path(source['path']), parent.MAX_SOURCE_BYTES)
        if len(raw) != source['bytes'] or hashlib.sha256(raw).hexdigest() != source['sha256']:
            raise parent.GenerationError('managed generator/foundation changed during emission')
    raw = parent.bounded_read(catalog, parent.MAX_CATALOG_BYTES)
    if len(raw) != pins['catalog']['bytes'] or hashlib.sha256(raw).hexdigest() != pins['catalog']['sha256']:
        raise parent.GenerationError('managed catalog changed during emission')
    root = catalog.resolve().parent
    for source in pins['definitions']:
        path = (root / source['file']).resolve()
        if not path.is_relative_to(root):
            raise parent.GenerationError('managed definition escaped source root')
        raw = parent.bounded_read(path, parent.MAX_SOURCE_BYTES)
        if len(raw) != source['bytes'] or hashlib.sha256(raw).hexdigest() != source['sha256']:
            raise parent.GenerationError('managed definition changed during emission')
    output.mkdir(parents=True, exist_ok=False)
    for name, body in products.items():
        destination = output / name
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(body, encoding='utf-8')
    (output / 'generation.managed.json').write_text(json.dumps(proof, indent=2, sort_keys=True) + '\n', encoding='utf-8')
    return proof


def _trusted_module(name, path):
    """CLI-only fixed SDK source loader; the catalog cannot select this path."""
    if name in sys.modules:
        raise RuntimeError('private managed generator module name already occupied')
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError('trusted source loader unavailable')
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
    parser.add_argument('--language', choices=tuple(EMITTERS), action='append', required=True)
    parser.add_argument('--require-native-identity', action='store_true', help='require existing identity syntax, never infer native qualification')
    args = parser.parse_args(argv)
    sdk = Path(__file__).resolve().parents[1]
    parent = _trusted_module('_uwvm_optional_managed_custom_types', sdk / 'tools/custom_types.py')
    bounded = _trusted_module('_uwvm_optional_managed_bounded_profile', sdk / 'custom-types/bounded_profile.py')
    generate(parent, bounded, args.catalog, args.out, languages=tuple(args.language),
             require_native_identity=args.require_native_identity)
    return 0


if __name__ == '__main__':
    sys.dont_write_bytecode = True
    raise SystemExit(main())
