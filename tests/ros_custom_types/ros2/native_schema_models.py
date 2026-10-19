#!/usr/bin/env python3
"""Independent synthetic models for complete ROS2 native schema admission.

All descriptions here are synthetic dictionaries. These checks do not execute
the C++ matcher, any native TypeSupport function, a compiler, DDS or a Guest.
They provide limited source/model evidence, never native interoperability PASS.
"""
from __future__ import annotations

import argparse
from copy import deepcopy
import hashlib
import json
from pathlib import Path

# Independent values from official Jazzy FieldType declarations. Fixed arrays
# occupy the primitive ID plus 48; sequences and strings have different IDs.
IDS = {'bool': 15, 'int8': 2, 'uint8': 3, 'int16': 4, 'uint16': 5,
       'int32': 6, 'uint32': 7, 'int64': 8, 'uint64': 9, 'float32': 10, 'float64': 11}


def native(record: dict) -> dict:
    return {'type': record['type'], 'fields': [
        {'name': field['name'], 'id': IDS.get(field['type'], 1) + (48 if field['array'] is not None else 0),
         'capacity': field['array'] or 0, 'string_capacity': 0,
         'nested': '' if field['type'] in IDS else field['type'], 'default': None}
        for field in record['fields']]}


def qualifies(schema: list[dict], description: dict, root: str) -> bool:
    """Model exact ordered fields and complete reachable, acyclic closure."""
    if not 1 <= len(schema) <= 128 or len(description['references']) != len(schema) - 1:
        return False
    types = [record['type'] for record in schema]
    if len(types) != len(set(types)) or root not in types:
        return False
    expected = {record['type']: record for record in schema}
    seen = set()

    def visit(name: str, stack: tuple[str, ...]) -> bool:
        if name in stack or len(stack) >= 16 or name not in expected:
            return False
        seen.add(name)
        fields = expected[name]['fields']
        if not 1 <= len(fields) <= 4096 or len({field['name'] for field in fields}) != len(fields):
            return False
        for field in fields:
            bound = field['array']
            if bound is not None and not 1 <= bound <= 4096:
                return False
            if field['type'] not in IDS and not visit(field['type'], stack + (name,)):
                return False
        return True

    if not visit(root, ()) or seen != set(expected):
        return False
    actual = [description['root'], *description['references']]
    names = [record['type'] for record in actual]
    if description['root']['type'] != root or len(names) != len(set(names)) or set(names) != set(expected):
        return False
    return all(record == native(expected[record['type']]) for record in actual)


def run() -> dict:
    root = 'uwvm_shape/msg/Sample'
    child = 'uwvm_shape/msg/Child'
    fields = lambda *items: [{'name': name, 'type': kind, 'array': count} for name, kind, count in items]
    schema = [{'type': root, 'fields': fields(('sequence', 'uint32', None), ('child', child, None), ('tag', 'uint8', 2))},
              {'type': child, 'fields': fields(('measurement', 'float64', None), ('active', 'bool', None))}]
    valid = {'root': native(schema[0]), 'references': [native(schema[1])]}
    checks = []

    def check(name: str, result: bool) -> None:
        checks.append({'name': name, 'pass': bool(result)})

    check('full root and fixed nested closure accepted', qualifies(schema, valid, root))
    mutations = [
        ('different field name', lambda d: d['root']['fields'][0].update(name='other')),
        ('narrower native scalar', lambda d: d['root']['fields'][0].update(id=5)),
        ('same-width signed scalar differs', lambda d: d['root']['fields'][0].update(id=6)),
        ('reordered native fields', lambda d: d['root']['fields'].reverse()),
        ('missing native field', lambda d: d['root']['fields'].pop()),
        ('extra native field', lambda d: d['root']['fields'].append(deepcopy(d['root']['fields'][0]))),
        ('wrong fixed array bound', lambda d: d['root']['fields'][2].update(capacity=3)),
        ('bounded sequence is not a fixed array', lambda d: d['root']['fields'][2].update(id=99)),
        ('unbounded sequence is not a fixed array', lambda d: d['root']['fields'][2].update(id=147)),
        ('scalar with array capacity refused', lambda d: d['root']['fields'][0].update(capacity=1)),
        ('nested name mismatch', lambda d: d['root']['fields'][1].update(nested='uwvm_shape/msg/Other')),
        ('primitive with nested name refused', lambda d: d['root']['fields'][0].update(nested=child)),
        ('string capacity refused', lambda d: d['root']['fields'][0].update(string_capacity=1)),
        ('native default refused', lambda d: d['root']['fields'][0].update(default='0')),
        ('nonnull empty default refused', lambda d: d['root']['fields'][0].update(default='')),
        ('wrong root name', lambda d: d['root'].update(type='uwvm_shape/msg/Other')),
        ('missing native dependency', lambda d: d['references'].clear()),
        ('extra native dependency', lambda d: d['references'].append(deepcopy(d['references'][0]))),
        ('wrong native dependency name', lambda d: d['references'][0].update(type='uwvm_shape/msg/Other')),
        ('different dependency field width', lambda d: d['references'][0]['fields'][0].update(id=10)),
        ('different dependency field order', lambda d: d['references'][0]['fields'].reverse()),
        ('octet is not uint8', lambda d: d['root']['fields'][2].update(id=64)),
    ]
    for name, mutate in mutations:
        changed = deepcopy(valid);mutate(changed)
        check(name, not qualifies(schema, changed, root))
    changed = deepcopy(schema);changed[0]['fields'][0]['type'] = 'uint16'
    check('genuine native identity beside truncated catalog refused', not qualifies(changed, valid, root))
    changed = deepcopy(schema);changed[0]['fields'].pop()
    check('genuine native identity beside partial catalog refused', not qualifies(changed, valid, root))
    changed = deepcopy(schema);changed[0]['fields'].reverse()
    check('genuine native identity beside reordered catalog refused', not qualifies(changed, valid, root))
    changed = deepcopy(schema);changed[1]['fields'].append({'name': 'cycle', 'type': root, 'array': None})
    cycle = {'root': native(changed[0]), 'references': [native(changed[1])]}
    check('matching-looking cyclic graph refused', not qualifies(changed, cycle, root))
    orphan = {'type': 'uwvm_shape/msg/Orphan', 'fields': fields(('data', 'uint8', None))}
    changed = [*schema, orphan];extra = deepcopy(valid);extra['references'].append(native(orphan))
    check('matching-looking disconnected graph refused', not qualifies(changed, extra, root))
    changed = [*schema, deepcopy(schema[1])]
    check('duplicate expected record refused', not qualifies(changed, valid, root))
    # A diamond reaches one suffix first through a short path, then through a
    # longer path. Depth checking must not reuse the short path's result blindly.
    deep = []
    for index in range(18):
        name = f'uwvm_shape/msg/N{index}'
        data = fields(('data', 'uint8', None)) if index == 17 else fields(('next', f'uwvm_shape/msg/N{index+1}', None))
        deep.append({'type': name, 'fields': data})
    deep[0]['fields'].insert(0, {'name': 'short', 'type': 'uwvm_shape/msg/N16', 'array': None})
    description = {'root': native(deep[0]), 'references': [native(record) for record in deep[1:]]}
    check('memoized shorter path cannot conceal excessive depth', not qualifies(deep, description, deep[0]['type']))
    # Reordering references changes no field order or type graph.
    independent = {'type': 'uwvm_shape/msg/Second', 'fields': fields(('value', 'int16', None))}
    ordered = deepcopy(schema);ordered[0]['fields'].append({'name': 'second', 'type': independent['type'], 'array': None});ordered.append(independent)
    description = {'root': native(ordered[0]), 'references': [native(independent), native(ordered[1])]}
    check('native reference order is irrelevant', qualifies(ordered, description, root))
    repository = Path(__file__).resolve().parents[3]
    paths = [Path(__file__).resolve(), repository / 'gateways/ros2/custom_topics.h',
             repository / 'src/uwvm2/imported/ros/ipc/custom_type.h']
    return {'status': 'SOURCE_AND_MODEL_PASS' if all(row['pass'] for row in checks) else 'FAIL',
            'scope': 'independent synthetic dictionary models; C++ and native TypeSupport not executed',
            'actual_behavior_status': 'NOT_EXECUTED', 'checks_count': len(checks), 'checks': checks,
            'inputs': [{'path': str(path.relative_to(repository)), 'bytes': len(path.read_bytes()),
                        'sha256': hashlib.sha256(path.read_bytes()).hexdigest()} for path in paths]}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__);parser.add_argument('--out', type=Path, required=True)
    arguments = parser.parse_args();report = run()
    with arguments.out.open('x', encoding='utf-8') as output:
        json.dump(report, output, indent=2);output.write('\n')
    print(f"{report['status']} native schema models checks={report['checks_count']}; C++/native NOT_EXECUTED")
    return 0 if report['status'] == 'SOURCE_AND_MODEL_PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
