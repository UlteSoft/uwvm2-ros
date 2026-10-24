#!/usr/bin/env python3
"""Check the deletion-only adapter through actual global/single/group CLI."""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
sys.dont_write_bytecode = True
import events


def options(cap, version=2, scope='global'):
    prefix = f'--ros{version}-{scope}-'
    target = [] if scope == 'global' else ['deletion_cli']
    argv = []
    if target:
        argv += ['--wasm-set-main-module-name', target[0], prefix + 'create', *target]
        if scope == 'group':
            argv += [prefix + 'add-module', *target, target[0]]
    connection = {'gateway-socket': '/tmp/deletion-cli-unused.sock', 'gateway-uid': str(os.getuid()),
                  'node-name': 'deletion_cli', 'namespace': '/cli', 'connect-timeout-ms': '1000',
                  'audit-deny-max-rate-hz': '100', 'audit-deny-burst': '10',
                  'audit-deny-max-records': '100', 'audit-deny-max-bytes': '65536'}
    connection.update({'domain-id': '192', 'rmw-implementation': 'rmw_fastrtps_cpp',
                       'security-mode': 'disabled-test', 'keystore': 'not-applicable', 'enclave': 'not-applicable'}
                      if version == 2 else {'master-uri': 'http://127.0.0.1:11339', 'advertise-host': '127.0.0.1', 'transport': 'tcpros'})
    argv += [prefix + 'enable', *target]
    for key, value in connection.items():
        argv += [prefix + key, *target, value]
    argv += [prefix + 'grant', *target, *[cap[k] for k in ('alias', 'operation', 'resource', 'type', 'identity')]]
    for key, value in cap['limits'].items():
        argv += [prefix + 'set', *target, cap['alias'], key, value]
    return [*argv, f'--ros{version}-check-policy']


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--uwvm', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True)
    a = p.parse_args()
    assert not a.output.exists()
    rows = []
    for scope in ('global', 'single', 'group'):
        cases = [('legal', events.grant(), True, None)]
        for field, value in (('max-parameter-elements', '8'), ('parameter-min-value', '0'),
                             ('parameter-max-value', '1'), ('validator', 'finite')):
            c = copy.deepcopy(events.grant());c['limits'][field] = value
            cases.append((field, c, False, None))
        c = events.grant();c['identity'] = 'builtin:param-event-bool-v1'
        cases.append(('wrong-identity', c, False, 'type identity differs'))
        c = events.grant();del c['limits']['qos.reliability']
        cases.append(('missing-qos', c, False, 'missing explicit field'))
        for operation in ('param.get', 'param.set', 'param.declare', 'param.undeclare', 'topic.publish'):
            c = events.grant();c['operation'] = operation
            cases.append(('no-implied-' + operation, c, False, None))
        for name, cap, success, marker in cases:
            argv = [str(a.uwvm.resolve()), *options(cap, scope=scope)]
            r = subprocess.run(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=15)
            valid = (r.returncode == 0) == success and (marker is None or marker in r.stdout + r.stderr)
            rows.append(dict(name=scope + '-' + name, status='PASS' if valid else 'FAIL', command=argv,
                             returncode=r.returncode, stdout=r.stdout, stderr=r.stderr))
    argv = [str(a.uwvm.resolve()), *options(events.grant(), version=1)]
    r = subprocess.run(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, timeout=15)
    rows.append(dict(name='ros1-rejects-deletion-projection', status='PASS' if r.returncode != 0 else 'FAIL',
                     command=argv, returncode=r.returncode, stdout=r.stdout, stderr=r.stderr))
    status = 'PASS' if all(v['status'] == 'PASS' for v in rows) else 'FAIL'
    a.output.parent.mkdir(parents=True, exist_ok=True)
    a.output.write_text(json.dumps(dict(status=status, kind='deletion-only-cli', worker_sha256=hashlib.sha256(a.uwvm.read_bytes()).hexdigest(),
        executed_driver_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(), tests=rows), indent=2) + '\n')
    print(status, len(rows), str(a.output), flush=True)
    return 0 if status == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
