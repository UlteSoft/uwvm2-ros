#!/usr/bin/env python3
"""Challenge source binding using separately retained genuine export DATA.

Run only inside the admitted test owner. This script does not authenticate the
SDK, execute ROS, create entities, or supply replacement positive identities.
The original owner must separately retain the source, compiler dependencies,
link inputs, stopped exporter, and actual native message value witnesses.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import types


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source-root', 'catalog', 'metadata', 'proof'):
        parser.add_argument('--'+name, required=True, type=Path)
    for name in ('adapter', 'parent', 'bounded'):
        parser.add_argument('--'+name+'-sha256', required=True)
    args = parser.parse_args()
    root = args.source_root
    adapter = root/'sdk/origin/robots/industrial/vendor_profile.py'
    raw = adapter.read_bytes()
    if hashlib.sha256(raw).hexdigest() != args.adapter_sha256:
        raise ValueError('actual selected adapter differs')
    module = types.ModuleType('_installed_source_negative_contract')
    module.__file__ = str(adapter)
    exec(compile(raw, str(adapter), 'exec', dont_inherit=True), module.__dict__)
    parent = root/'sdk/origin/tools/custom_types.py'
    bounded = root/'sdk/origin/custom-types/bounded_profile.py'
    api = module.API(parent.read_bytes(), bounded.read_bytes(),
        parent_path=parent, bounded_path=bounded,
        parent_sha256=args.parent_sha256, bounded_sha256=args.bounded_sha256,
        adapter_raw=raw, adapter_sha256=args.adapter_sha256,
        templates={f'interface_templates/bounded_interfaces.{ext}.in':
                   (bounded.parent/f'interface_templates/bounded_interfaces.{ext}.in').read_bytes()
                   for ext in ('rs', 'mjs', 'go', 'ts')})
    metadata_raw = args.metadata.read_bytes()
    proof_raw = args.proof.read_bytes()
    metadata = module.strict_json(metadata_raw)
    proof = module.strict_json(proof_raw)
    protocol = proof['format'] == 'uwvm-ros2-installed-protocol-source-proof-v1'
    records_key = 'source_records' if protocol else 'records'
    # Positive source binding uses only the separately captured genuine export.
    # It remains a source association, never a native execution claim.
    document, receipt = api.bind(args.catalog, metadata_raw, installed_source_proof=proof_raw)
    assert receipt['default_authority'] == [] and document['types']
    refused = []

    def reject(label, data=metadata_raw, source=proof_raw):
        try:
            api.bind(args.catalog, data, installed_source_proof=source)
        except ValueError:
            refused.append(label)
        else:
            raise AssertionError('source mutation accepted: '+label)

    canonical = module.canonical
    reject('implicit legacy source fallback', source=None)
    reject('duplicate native JSON key', data=b'{"format":0,"format":1}')
    reject('duplicate proof JSON key', source=b'{"format":0,"format":1}')
    changed = copy.deepcopy(metadata)
    changed['format'] = 'uwvm-ros-industrial-native-v1'
    reject('modern exporter downgrade', data=canonical(changed))
    for key, value in (('ros_version', True), ('source_proof_sha256', '0'*64),
                       ('extra_role', {})):
        changed = copy.deepcopy(metadata)
        changed[key] = value
        reject('native envelope '+key, data=canonical(changed))
    first = next(iter(metadata['types']))
    changed = copy.deepcopy(metadata)
    del changed['types'][first]
    reject('missing complete native root', data=canonical(changed))
    for key, value in (('native_baked_hash_available', True),
                       ('source_proof_sha256', '0'*64),
                       ('identity_provenance', 'UNPROVED')):
        changed = copy.deepcopy(metadata)
        changed['types'][first]['official'][key] = value
        reject('native root '+key, data=canonical(changed))
    changed = copy.deepcopy(metadata)
    changed['types'][first]['official']['root']['type'] += '_wrong'
    reject('wrong complete native graph', data=canonical(changed))
    for key, value in (('default_authority', ['ros']), ('source_proof_sha256', '0'*64),
                       ('extra_role', {})):
        changed = copy.deepcopy(proof)
        changed[key] = value
        reject('proof envelope '+key, source=canonical(changed))
    # Recompute a public digest after changing its domain: hash consistency is
    # insufficient when original declarations, parser roles or defaults differ.
    for role in ('presence', 'constant', 'parser', 'installed inventory'):
        changed = copy.deepcopy(proof)
        domain = changed['domain']
        if role == 'presence':
            fields = next(r['fields'] for r in domain[records_key].values() if r['fields'])
            fields[0]['has_declared_default'] = not fields[0]['has_declared_default']
        elif role == 'constant':
            domain[records_key][next(iter(domain[records_key]))]['constants'].append(
                dict(name='UNDECLARED', type='uint8', value='0'))
        elif role == 'parser':
            del domain['installed_idl_facts']['parser_sources']['grammar.lark']
        else:
            domain['installed_idl_facts']['records'].pop()
        changed['source_proof_sha256'] = hashlib.sha256(canonical(domain)).hexdigest()
        reject('rehashed '+role, source=canonical(changed))
    for role in ('Boolean presence', 'integer capacity', 'generator byte count', 'IDL presence'):
        changed = copy.deepcopy(proof)
        domain = changed['domain']
        if role == 'generator byte count':
            domain['proof_generator']['bytes'] = float(domain['proof_generator']['bytes'])
        else:
            rows = (domain['installed_idl_facts']['records'] if role == 'IDL presence'
                    else domain[records_key].values())
            fields = next(row['fields'] for row in rows if row['fields'])
            if role == 'integer capacity':
                fields[0]['array_size'] = float(fields[0]['array_size'])
            else:
                fields[0]['has_declared_default'] = int(fields[0]['has_declared_default'])
        changed['source_proof_sha256'] = hashlib.sha256(canonical(domain)).hexdigest()
        reject('rehashed numeric equality '+role, source=canonical(changed))
    for role in ('absent', 'missing', 'floating byte count'):
        changed = copy.deepcopy(proof)
        if role == 'absent':
            changed['outputs'] = None
        elif role == 'missing':
            changed['outputs'].pop(next(iter(changed['outputs'])))
        else:
            pin = next(iter(changed['outputs'].values()))
            pin['bytes'] = float(pin['bytes'])
        reject('emitted source roles '+role, source=canonical(changed))
    if protocol:
        for role in ('missing cancellation', 'missing status', 'changed Action service', 'orphan protocol source'):
            changed = copy.deepcopy(proof);domain = changed['domain'];facts = domain['installed_idl_facts']
            if role == 'missing cancellation':
                facts['interfaces'] = [row for row in facts['interfaces'] if row['type'] != 'action_msgs/srv/CancelGoal']
            elif role == 'missing status':
                facts['records'] = [row for row in facts['records'] if row['type'] != 'action_msgs/msg/GoalStatusArray']
            elif role == 'changed Action service':
                row = next(row for row in facts['interfaces'] if row['kind'] == 'action')
                row['send_goal_service'] = row['get_result_service']
            else:
                facts['roots']['unproved/msg/Orphan'] = dict(declared=[],includes=[],implicit_includes=[])
            changed['source_proof_sha256'] = hashlib.sha256(canonical(domain)).hexdigest()
            reject('rehashed protocol '+role, source=canonical(changed))
    print(json.dumps(dict(scope='Original export DATA source-binding refusals only',
                          rejected=refused, default_authority=[]), sort_keys=True))


if __name__ == '__main__':
    main()
