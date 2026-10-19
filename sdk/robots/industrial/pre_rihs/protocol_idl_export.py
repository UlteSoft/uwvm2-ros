#!/usr/bin/env python3
"""Capture complete legacy ROS2 Message/Service/Action installed IDL facts.

This is an explicit native administration tool, not a Guest import. It executes
the reviewed SDK parser from captured source and retains actual legacy derived
protocol messages. It neither creates endpoints nor exports native identities.
The independent owner must admit all SDK/source/process/resource evidence.
"""
import argparse
import hashlib
from pathlib import Path
import re
import sys
import types

FORMAT = 'uwvm-ros2-installed-protocol-idl-source-v1'
TYPE = r'[a-z][a-z0-9_]{0,62}/(?:msg|srv|action)/[A-Z][A-Za-z0-9_]*'


def helper():
    path = Path(__file__).with_name('idl_export.py')
    # Bootstrap source admission is also the independent owner's responsibility.
    # The shared reader then reopens this exact ordinary file before returning.
    raw = path.read_bytes()
    module = types.ModuleType('_installed_protocol_idl_helpers')
    module.__file__ = str(path)
    exec(compile(raw, str(path), 'exec', dont_inherit=True), module.__dict__)
    module.need(module.read(path) == raw, 'IDL helper changed during source capture')
    return module, raw


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('request', 'parser-root', 'out'):
        p.add_argument('--'+name, required=True, type=Path)
    for name in ('parser', 'definition', 'helper'):
        p.add_argument('--'+name+'-sha256', required=True)
    args = p.parse_args(); sys.dont_write_bytecode = True
    h, helper_raw = helper(); need = h.need
    need(h.pin(helper_raw)['sha256'] == args.helper_sha256, 'explicit actual IDL helper SHA differs')
    request_raw = h.read(args.request, 2*1024*1024); request = h.strict(request_raw)
    need(type(request) is dict and set(request) == {'format', 'interfaces'} and
         request['format'] == FORMAT and type(request['interfaces']) is list and
         1 <= len(request['interfaces']) <= 128, 'closed explicit installed protocol request required')
    definition, parser, sources = h.parser_sources(args.parser_root,
        {'parser.py':args.parser_sha256, 'definition.py':args.definition_sha256})
    # ServiceEvent belongs to a different actual SDK protocol. The native C++
    # layer separately rejects every hash-capable support, including null hashes.
    need('event_message' not in definition.Service.__slots__, 'select the actual pre-ServiceEvent SDK parser')
    records = {}; interfaces = {}; roots = {}; captures = {}; total = 0

    def name(value):
        result = '/'.join(value.namespaced_name())
        need(re.fullmatch(TYPE, result), 'exact native protocol type required')
        return result

    def message(value, owner, source):
        row, _ = h.describe(definition, value, protocol=True)
        typ = row['type']; need(typ not in records, 'duplicate complete native Message declaration')
        row.update(owner=owner, installed_idl=source)
        records[typ] = row
        return typ

    def service(value, owner, source):
        typ = name(value.namespaced_type)
        need(typ not in interfaces and not hasattr(value, 'event_message'), 'exact legacy Service required')
        interfaces[typ] = dict(type=typ, kind='service', owner=owner, installed_idl=source,
            components=dict(request=message(value.request_message, owner, source),
                            response=message(value.response_message, owner, source)))
        return typ

    for selected in request['interfaces']:
        need(type(selected) is dict and set(selected) == {'type', 'idl'} and
             type(selected['type']) is str and re.fullmatch(TYPE, selected['type']) and
             selected['type'] not in roots, 'unique original protocol/IDL selection required')
        typ = selected['type']; path = Path(selected['idl']); raw = h.read(path, 1024*1024)
        total += len(raw); need(total <= h.LIMIT, 'complete installed protocol source exceeds16MiB')
        need(str(path) not in captures, 'one physical original IDL per selected root required')
        captures[str(path)] = raw; source = dict(path=str(path), **h.pin(raw))
        content = parser.parse_idl_string(raw.decode('utf-8', errors='strict'))
        values = [x for x in content.elements if type(x) is not definition.Include]
        need(len(values) == 1 and type(values[0]) in (definition.Message, definition.Service, definition.Action),
             'one complete actual Message/Service/Action per original IDL required')
        value = values[0]; includes = [x.locator for x in content.get_elements_of_type(definition.Include)]
        need(len(includes) == len(set(includes)), 'duplicate original native IDL include')
        if type(value) is definition.Message:
            actual = message(value, typ, source)
            interfaces[typ] = dict(type=typ, kind='message', owner=typ, installed_idl=source,
                                   components=dict(message=actual))
            declared = [actual]; implicit = []
        elif type(value) is definition.Service:
            actual = service(value, typ, source)
            declared = list(interfaces[typ]['components'].values()); implicit = []
        else:
            actual = name(value.namespaced_type)
            components = {role:message(getattr(value, role), typ, source) for role in ('goal', 'result', 'feedback')}
            declared = list(components.values())
            send = service(value.send_goal_service, typ, source)
            result = service(value.get_result_service, typ, source)
            feedback = message(value.feedback_message, typ, source)
            interfaces[typ] = dict(type=typ, kind='action', owner=typ, installed_idl=source,
                components=components, send_goal_service=send, get_result_service=result,
                feedback_message=feedback)
            implicit = [x.locator for x in value.implicit_includes]
        need(actual == typ and typ.split('/')[1] == {'message':'msg', 'service':'srv', 'action':'action'}[interfaces[typ]['kind']],
             'actual installed protocol root differs from selection')
        roots[typ] = dict(declared=declared, includes=includes, implicit_includes=implicit)

    for typ, row in records.items():
        for field in row['fields']:
            need('/' not in field['type'] or field['type'] in records, 'missing whole native Message dependency: '+typ)
    for typ, root in roots.items():
        dependencies = {records[f['type']]['owner']+'.idl' for record in root['declared']
                        for f in records[record]['fields'] if '/' in f['type'] and records[f['type']]['owner'] != typ}
        # The genuine legacy parser appends Action.implicit_includes to the
        # returned content even when they are absent from the original IDL text.
        # Retain and require that exact SDK-injected set, rather than discarding
        # those real protocol dependencies or inventing source include lines.
        need(set(root['includes']) == dependencies|set(root['implicit_includes']),
             'actual complete original IDL include set differs: '+typ)
        need(all(path.endswith('.idl') and path[:-4] in roots for path in root['implicit_includes']),
             'missing genuine SDK-injected Action dependency')
    for typ, row in interfaces.items():
        if row['kind'] == 'action':
            need('action_msgs/srv/CancelGoal' in interfaces and 'action_msgs/msg/GoalStatusArray' in interfaces,
                 'complete Action cancellation/status source is mandatory')
    for path, raw in captures.items():
        need(h.read(Path(path)) == raw, 'actual installed IDL changed')
    for path, raw in sources.items():
        need(h.read(args.parser_root/'rosidl_parser'/path) == raw, 'actual SDK parser changed')
    own = h.read(Path(__file__).resolve())
    need(h.read(args.request) == request_raw and h.read(Path(h.__file__)) == helper_raw, 'source/request changed')
    result = dict(format=FORMAT, scope='Complete actual legacy protocol IDL source facts; no native identity, execution, endpoint or authority.',
        request=h.pin(request_raw), parser_sources={k:h.pin(v) for k,v in sources.items()},
        exporter_sources=dict(protocol=h.pin(own), message=h.pin(helper_raw)),
        roots=roots, interfaces=list(interfaces.values()), records=list(records.values()), default_authority=[])
    import json
    output = (json.dumps(result, sort_keys=True, separators=(',', ':'), allow_nan=False)+'\n').encode()
    need(len(output) <= h.LIMIT and args.out.is_absolute() and args.out.parent.resolve(strict=True) == args.out.parent,
         'bounded fresh physical output required')
    import os
    fd = os.open(args.out, os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC, 0o600)
    with os.fdopen(fd, 'wb') as stream:
        stream.write(output)


if __name__ == '__main__':
    main()
