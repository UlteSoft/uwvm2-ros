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
                    'CODEC_IDENTITY', 'MIN_WIRE_BYTES', 'MAX_WIRE_BYTES',
                    'CARTESIAN_MAX_WIRE_BYTES', 'AGGREGATE_WIRE_BYTES', 'DECLARED_CONSTANTS'}:
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
            '_py_int' if member.type in SCALARS else ('_py_bytes' if member.string_encoding == 'raw_bytes' else '_py_str') if member.type == 'string' else
            '_record_' + records[(record.version, member.type)].symbol)
    return '_py_list[' + base + ']' if member.array else base


def python_default(bounded, record, member, records):
    scalar = (bounded.scalar_default_expression(member.type,member.native_default,'python') if member.native_default is not None else
              'False' if member.type == 'bool' else '0.0' if member.type in ('float32', 'float64') else
              '0' if member.type in SCALARS else ("b''" if member.string_encoding == "raw_bytes" else "''") if member.type == 'string' else
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
    float as _py_float, int as _py_int, str as _py_str, bytes as _py_bytes, list as _py_list)
from typing import ClassVar as _ClassVar
from roswasm.codecs import Writer as _Writer, Reader as _Reader

def _write_raw(writer, value, maximum):
    if type(value) is not bytes or len(value) > maximum:
        raise ValueError('bounded raw bytes required')
    writer.integer(len(value), 4).raw(value)

def _read_raw(reader, maximum):
    count = reader.integer(4)
    if count > maximum:
        raise ValueError('raw byte count bound')
    return reader.take(count)

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
                  '    MAX_WIRE_BYTES: _ClassVar[_py_int] = ' + str(record.maximum),
                  '    CARTESIAN_MAX_WIRE_BYTES: _ClassVar[_py_int] = ' + str(record.cartesian_maximum),
                  '    AGGREGATE_WIRE_BYTES: _ClassVar[_py_int] = ' + str(record.aggregate_wire_bytes),
                  '    DECLARED_CONSTANTS: _ClassVar = ' + repr(record.constants)]
        for constant in record.constants:
            lines += ['    ' + constant['name'] + ': _ClassVar = ' + bounded.scalar_default_expression(constant['type'], constant['value'], 'python')]
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
                  '    if type(value) is not ' + name + ':', "        raise TypeError('exact generated record required')", '    _start = len(writer.data)']
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
                lines += [indent + ('_write_raw' if member.string_encoding == 'raw_bytes' else '_write_text') + '(writer, ' + value + ', ' + str(member.string_bound) + ')']
            else:
                lines += [indent + '_write_' + records[(record.version, member.type)].symbol + '(writer, ' + value + ')']
        # Validate the exact scalars/counts just encoded, not a later reread of
        # mutable application attributes; every nested writer does the same.
        lines += python_semantics(bounded, record, 'value', '    ', captured=True)
        lines += ['    if not ' + str(record.minimum) + ' <= len(writer.data) - _start <= ' + str(record.maximum) + ':', "        raise ValueError('nested encoded record aggregate extent')"]
        if not record.members:
            lines += ['    pass']
        lines += ['', 'def _read_' + name + '(reader):', '    _start = reader.position', '    value = ' + name + '()']
        for index, member in enumerate(record.members):
            if member.type == 'bool':
                expression = '_read_bool(reader)'
            elif member.type in ('float32', 'float64'):
                expression = 'reader.' + ('f32()' if member.type == 'float32' else 'f64()')
            elif member.type in SCALARS:
                width, signed, _ = SCALARS[member.type]
                expression = 'reader.integer(' + str(width) + ', ' + str(signed) + ')'
            elif member.type == 'string':
                expression = ('_read_raw' if member.string_encoding == 'raw_bytes' else '_read_text') + '(reader, ' + str(member.string_bound) + ')'
            else:
                expression = '_read_' + records[(record.version, member.type)].symbol + '(reader)'
            if member.array:
                count = str(member.count)
                if member.array == 'sequence':
                    count = '_read_count(reader, ' + count + ', ' + str(element_minimum(record, member, records)) + ')'
                expression = '[' + expression + ' for _ in range(' + count + ')]'
            lines += ['    value.' + member.name + ' = ' + expression]
        lines += python_semantics(bounded, record, 'value', '    ')
        lines += ['    if not ' + str(record.minimum) + ' <= reader.position - _start <= ' + str(record.maximum) + ':', "        raise ValueError('nested complete record aggregate extent')", '    return value', '']
    return {'bounded_records.py': '\n'.join(lines) + '\n'}


# Only this disjoint explicit control emitter is supported by this operator.
EMITTERS = {'python': emit_python}
