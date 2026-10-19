#!/usr/bin/env python3
"""Capture complete installed ROS2 Message IDL with the selected real SDK parser.

This native administrative tool emits source facts, never a native hash, entity,
permission or acceptance report. Its parser modules are explicitly selected and
executed from captured source bytes, bypassing cached Python bytecode. The owner
must independently admit the complete physical SDK, dependencies and process.
"""
import argparse
import hashlib
import io
import json
import math
import os
from pathlib import Path
import re
import stat
import sys
import types

FORMAT = 'uwvm-ros2-installed-message-idl-source-v1'
LIMIT = 16 * 1024 * 1024


def need(value, reason):
    if not value:
        raise ValueError(reason)


def read(path, maximum=LIMIT):
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path,
         'physical absolute source file required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        before = os.fstat(fd)
        need(stat.S_ISREG(before.st_mode) and before.st_nlink == 1 and
             0 <= before.st_size <= maximum, 'bounded ordinary single-link source required')
        chunks = []; size = 0
        while True:
            chunk = os.read(fd, min(65536, maximum + 1 - size))
            if not chunk:
                break
            size += len(chunk); chunks.append(chunk)
            need(size <= before.st_size and size <= maximum, 'source grew during capture')
        key = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns, s.st_nlink)
        need(size == before.st_size and key(before) == key(os.fstat(fd)) == key(path.lstat()),
             'source changed during one-FD capture')
        return b''.join(chunks)
    finally:
        os.close(fd)


def pin(raw):
    return dict(bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())


def strict(raw):
    def pairs(rows):
        result = {}
        for key, value in rows:
            need(key not in result, 'duplicate JSON member')
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs,
        parse_constant=lambda _: need(False, 'nonfinite JSON token'))


def parser_sources(root, expected):
    """Load the fixed genuine parser package from exact captured .py bytes.

    Lark and standard-library dependencies still belong to the independently
    admitted native SDK. No request or Guest can name an import/plugin here.
    """
    need(not any(n == 'rosidl_parser' or n.startswith('rosidl_parser.') for n in sys.modules),
         'fresh parser process required; no ambient rosidl_parser module')
    package = root / 'rosidl_parser'
    captures = {name: read(package / name, 2 * 1024 * 1024)
                for name in ('__init__.py', 'definition.py', 'grammar.lark', 'parser.py')}
    for name in ('definition.py', 'parser.py'):
        need(pin(captures[name])['sha256'] == expected[name], 'explicit SDK parser SHA differs: ' + name)
    for name, raw in captures.items():
        if name == 'grammar.lark':
            continue
        module_name = 'rosidl_parser' if name == '__init__.py' else 'rosidl_parser.' + name[:-3]
        module = types.ModuleType(module_name); module.__file__ = str(package / name)
        module.__package__ = 'rosidl_parser'
        if name == '__init__.py':
            module.__path__ = [str(package)]
        if name == 'parser.py':
            # The genuine parser opens its grammar once while importing. Serve
            # precisely that already captured file, avoiding a second pathname
            # read/ABA window without rewriting any SDK parser source.
            def grammar_open(path, mode='r', encoding=None):
                need(Path(path) == package/'grammar.lark' and mode == 'r' and encoding == 'utf-8',
                     'unexpected SDK parser filesystem read')
                return io.StringIO(captures['grammar.lark'].decode('utf-8', errors='strict'))
            module.__dict__['open'] = grammar_open
        sys.modules[module_name] = module
        exec(compile(raw, module.__file__, 'exec', dont_inherit=True), module.__dict__)
    return sys.modules['rosidl_parser.definition'], sys.modules['rosidl_parser.parser'], captures


def scalar_value(kind, value):
    if kind == 'string':
        need(type(value) is str and '\0' not in value and len(value.encode('utf-8')) <= 32768,
             'bounded Unicode scalar text required')
        return value
    if kind == 'bool':
        need(type(value) is bool, 'actual Boolean annotation required')
        return str(value)
    if kind in ('float32', 'float64'):
        need(type(value) in (float, int) and math.isfinite(value), 'finite actual floating annotation required')
        return str(float(value))
    need(kind in ('int8', 'uint8', 'byte', 'char', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64') and
         type(value) is int, 'actual integer annotation required')
    return str(value)


def describe(definition, message, *, protocol=False):
    """Keep declaration presence independently of its native constructor value."""
    scalar = {'short':'int16', 'long':'int32', 'long long':'int64',
        'unsigned short':'uint16', 'unsigned long':'uint32', 'unsigned long long':'uint64',
        'float':'float32', 'double':'float64', 'boolean':'bool', 'octet':'byte', 'char':'char'}
    for width in (8, 16, 32, 64):
        scalar['int'+str(width)] = 'int'+str(width)
        scalar['uint'+str(width)] = 'uint'+str(width)

    def atom(value):
        if type(value) is definition.BasicType:
            need(value.typename in scalar, 'unsupported complete native scalar')
            return scalar[value.typename], 0
        if type(value) in (definition.UnboundedString, definition.BoundedString):
            return 'string', value.maximum_size if type(value) is definition.BoundedString else 0
        need(type(value) is definition.NamespacedType, 'unresolved/wide/unknown native field type')
        name = '/'.join(value.namespaced_name())
        category = '(?:msg|srv|action)' if protocol else 'msg'
        need(re.fullmatch(r'[a-z][a-z0-9_]{0,62}/'+category+r'/[A-Z][A-Za-z0-9_]*', name), 'exact Message dependency required')
        return name, 0

    def annotations(item):
        need(all(x.name in ('default', 'verbatim', 'unit') for x in item.annotations),
             'unhandled native semantic annotation')
        values = [x.value for x in item.annotations if x.name == 'default']
        need(len(values) <= 1, 'duplicate actual native default')
        if values and type(values[0]) is dict:
            need(set(values[0]) == {'value'}, 'exact native default annotation value required')
            values = [values[0]['value']]
        return values

    structure = message.structure
    need(not annotations(structure), 'structure default unsupported')
    fields = []; includes = set()
    retained_annotations = [dict(scope='structure', name=structure.namespaced_type.name,
        values=[dict(name=x.name, value=x.value) for x in structure.annotations])]
    for member in structure.members:
        shape = member.type; array = type(shape) is definition.Array
        sequence = type(shape) in (definition.BoundedSequence, definition.UnboundedSequence)
        base = shape.value_type if array or sequence else shape
        kind, text_bound = atom(base)
        defaults = annotations(member)
        retained_annotations.append(dict(scope='member', name=member.name,
            values=[dict(name=x.name, value=x.value) for x in member.annotations]))
        need(not defaults or not (array or sequence), 'complete array defaults need another supported domain')
        fields.append(dict(name=member.name, type=kind,
            array_size=shape.size if array else 0, is_array=array, string_bound=text_bound,
            is_sequence=sequence, sequence_bound=shape.maximum_size if type(shape) is definition.BoundedSequence else 0,
            native_default=scalar_value(kind, defaults[0]) if defaults else '', has_declared_default=bool(defaults)))
        if '/' in kind:
            includes.add(kind + '.idl')
    constants = []
    for constant in message.constants:
        need(not annotations(constant), 'constant default annotation unsupported')
        retained_annotations.append(dict(scope='constant', name=constant.name,
            values=[dict(name=x.name, value=x.value) for x in constant.annotations]))
        kind, bound = atom(constant.type)
        # rosidl_parser derives a string constant's BoundedString type from
        # len(value), even when its original IDL declaration is `const string`.
        # This is a literal size, not a serialized member capacity. Keep the
        # complete value and require the genuine parser's exact relationship.
        need('/' not in kind and
             (not bound or kind == 'string' and type(constant.value) is str and
              bound == len(constant.value)), 'native constant must have a scalar/text type')
        constants.append(dict(name=constant.name, type=kind, value=scalar_value(kind, constant.value)))
    return dict(type='/'.join(structure.namespaced_type.namespaced_name()), fields=fields,
                constants=constants, annotations=retained_annotations), includes


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('request', 'parser-root', 'out'):
        p.add_argument('--'+name, required=True, type=Path)
    p.add_argument('--parser-sha256', required=True); p.add_argument('--definition-sha256', required=True)
    args = p.parse_args(); sys.dont_write_bytecode = True
    request_raw = read(args.request, 2 * 1024 * 1024); request = strict(request_raw)
    need(type(request) is dict and set(request) == {'format', 'records'} and
         request['format'] == FORMAT and type(request['records']) is list and
         1 <= len(request['records']) <= 128, 'closed explicit installed Message request required')
    need(args.parser_root.is_absolute() and args.parser_root.resolve(strict=True) == args.parser_root,
         'physical selected parser root required')
    definition, parser, sources = parser_sources(args.parser_root,
        {'parser.py':args.parser_sha256, 'definition.py':args.definition_sha256})
    rows = []; captures = {}; names = set(); total = 0
    for selected in request['records']:
        need(type(selected) is dict and set(selected) == {'type', 'idl'} and
             type(selected['type']) is str and re.fullmatch(r'[a-z][a-z0-9_]{0,62}/msg/[A-Z][A-Za-z0-9_]*', selected['type']) and
             selected['type'] not in names, 'unique explicit actual Message/IDL association required')
        names.add(selected['type']); path = Path(selected['idl']); raw = read(path, 1024 * 1024)
        total += len(raw); need(total <= LIMIT, 'whole installed IDL closure exceeds16MiB')
        captures[str(path)] = raw; content = parser.parse_idl_string(raw.decode('utf-8', errors='strict'))
        need(all(type(x) in (definition.Message, definition.Include) for x in content.elements),
             'only complete Message IDL is admitted; Service/Action roots are independent')
        messages = content.get_elements_of_type(definition.Message)
        need(len(messages) == 1, 'one original Message per selected IDL required')
        row, dependencies = describe(definition, messages[0])
        need(row['type'] == selected['type'], 'installed IDL type differs from explicit selection')
        actual_includes = [x.locator for x in content.get_elements_of_type(definition.Include)]
        need(len(actual_includes) == len(set(actual_includes)) and set(actual_includes) == dependencies,
             'complete exact direct native IDL include set required')
        row['installed_idl'] = dict(path=str(path), **pin(raw)); rows.append(row)
    for row in rows:
        need(all('/' not in f['type'] or f['type'] in names for f in row['fields']),
             'missing complete installed native dependency')
    for name, raw in captures.items():
        need(read(Path(name)) == raw, 'installed source changed before final receipt')
    for name, raw in sources.items():
        need(read(args.parser_root/'rosidl_parser'/name) == raw, 'actual parser source changed')
    need(read(args.request) == request_raw, 'request changed before final receipt')
    result = dict(format=FORMAT, scope='Installed IDL source facts only; no Native identity/execution, Gateway binding, authority or acceptance.',
        request=pin(request_raw), parser_sources={name:pin(raw) for name, raw in sources.items()},
        exporter_source=pin(read(Path(__file__).resolve())), records=rows, default_authority=[])
    raw = (json.dumps(result, sort_keys=True, indent=2, ensure_ascii=True, allow_nan=False)+'\n').encode()
    need(len(raw) <= LIMIT and args.out.is_absolute() and args.out.parent.resolve(strict=True) == args.out.parent,
         'bounded fresh physical caller-owned output required')
    fd = os.open(args.out, os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC, 0o600)
    with os.fdopen(fd, 'wb') as stream:
        stream.write(raw)


if __name__ == '__main__':
    main()
