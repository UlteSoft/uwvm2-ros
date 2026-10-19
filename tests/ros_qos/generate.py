#!/usr/bin/env python3
"""Derive immutable Action QoS CLI/config inputs without executing a product.

The matrix is independent of production helper keys. Actual compilation and
behavior require the ARM/cgroup driver; generation is not a runtime verdict.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys

sys.dont_write_bytecode = True

TYPE = 'action_tutorials_interfaces/action/Fibonacci'
IDENTITY = 'RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0'
DIAGNOSTIC = 'action response QoS differs from its explicit server contract'


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def matrix():
    path = Path(__file__).with_name('action_matrix.json')
    rows = json.loads(path.read_text())['rows']
    if len(rows) != 40 or len({row['key'] for row in rows}) != 40:
        raise RuntimeError('independent 40-field matrix is incomplete')
    if sum(row['valid'] for row in rows) != 35:
        raise RuntimeError('expected exactly 35 legal and five unsupported-history mutations')
    expected = ['// Fixed independent G04 expectation matrix. Do not derive from production keys.',
                'struct mutation {char const* key;char const* baseline;char const* value;bool valid;};',
                'inline constexpr std::array<mutation,40> mutations{{']
    for row in rows:
        expected.append('    {'+','.join((json.dumps(row['key']), json.dumps(row['baseline']),
            json.dumps(row['mutation']), 'true' if row['valid'] else 'false'))+'},')
    expected.append('}};')
    if Path(__file__).with_name('action_matrix.inc').read_text() != '\n'.join(expected)+'\n':
        raise RuntimeError('committed native matrix differs from independent JSON expectations')
    return rows


def capability(alias, operation, rows):
    limits = {'max-active-goals':'1', 'max-actions':'1', 'timeout-ms':'15000',
              'max-goal-bytes':'256', 'max-feedback-bytes':'256', 'max-result-bytes':'256',
              'fibonacci.max-order':'46', 'cancel-scope':'not-applicable',
              'max-rate-hz':'100', 'burst':'8', 'max-bytes-per-second':'1048576',
              'max-event-queue-depth':'8', 'max-event-queue-bytes':'2048',
              'overflow-policy':'drop-oldest', 'validator':'none', 'audit':'allow-and-deny',
              'audit-max-rate-hz':'100', 'audit-burst':'8', 'audit-max-records':'10000',
              'audit-max-bytes':'1048576'}
    limits.update({row['key']: row['baseline'] for row in rows})
    return {'alias':alias, 'operation':operation, 'resource':'/uwvm_action_qos/action',
            'type':TYPE, 'identity':IDENTITY, 'limits':limits}


def connection(uid, socket, domain):
    return {'gateway-socket':socket, 'gateway-uid':str(uid), 'node-name':'uwvm_action_qos',
            'namespace':'/uwvm_action_qos', 'connect-timeout-ms':'1000',
            'audit-deny-max-rate-hz':'100', 'audit-deny-burst':'8',
            'audit-deny-max-records':'10000', 'audit-deny-max-bytes':'1048576',
            'domain-id':str(domain), 'rmw-implementation':'rmw_fastrtps_cpp',
            'security-mode':'disabled-test', 'keystore':'not-applicable', 'enclave':'not-applicable'}


def cli(grants, settings, enabled):
    prefix = '--ros2-global-'
    result = [prefix+'enable' if enabled else prefix+'disable']
    for key, value in settings.items():
        result.extend((prefix+key, value))
    for cap in grants:
        result.extend((prefix+'grant', *(cap[key] for key in ('alias','operation','resource','type','identity'))))
        for key, value in cap['limits'].items():
            result.extend((prefix+'set', cap['alias'], key, value))
    return result+['--ros2-check-policy']


def config(grants, settings, uid, pid, socket):
    lines = ['backend ros2', 'worker-uid '+str(uid), 'worker-pid '+str(pid),
             'socket '+socket, 'timeout-ms 15000', 'max-sessions 1',
             'audit-max-records-per-second 100', 'audit-max-records 10000', 'audit-max-bytes 1048576']
    lines.extend('connection '+key+' '+settings[key] for key in
                 ('node-name','namespace','domain-id','rmw-implementation','security-mode','keystore','enclave'))
    for cap in grants:
        lines.append('grant '+' '.join(cap[key] for key in ('alias','operation','resource','type','identity')))
        lines.extend('set '+cap['alias']+' '+key+' '+value for key, value in cap['limits'].items())
    return '\n'.join(lines)+'\n'


def derive(rows):
    pair = [capability('advertise','action.advertise',rows),capability('respond','action.respond',rows)]
    yield 'matching-40-fields', pair, True, 0, None
    for row in rows:
        label = row['key'].replace('.', '-')
        for enabled in (True, False):
            for position in (0, 1):
                grants = copy.deepcopy(pair);grants[position]['limits'][row['key']] = row['mutation']
                yield label+('-advertiser' if position == 0 else '-responder')+('-enabled' if enabled else '-disabled'), grants, enabled, row['enabled_error'] if enabled else row['disabled_error'], DIAGNOSTIC if row['valid'] else 'invalid explicit Action QoS'
            grants = copy.deepcopy(pair)
            for cap in grants:cap['limits'][row['key']] = row['mutation']
            yield label+'-matching-mutation'+('-enabled' if enabled else '-disabled'), grants, enabled, 95 if enabled and not row['valid'] else 0, None if row['valid'] else 'invalid explicit Action QoS'
        if row['valid']:
            grants = copy.deepcopy(pair);grants[1]['resource'] = '/uwvm_action_qos/independent';grants[1]['limits'][row['key']] = row['mutation']
            yield label+'-independent-resource', grants, True, 0, None
        if row['key'].endswith('.depth'):
            grants = copy.deepcopy(pair);grants[1]['limits'][row['key']] = '08'
            yield label+'-exact-8-vs-08', grants, True, 17, DIAGNOSTIC
            grants[0]['limits'][row['key']] = '08'
            yield label+'-matching-08', grants, True, 0, None
    yield 'advertiser-only', [pair[0]], True, 0, None
    yield 'responder-only', [pair[1]], True, 0, None


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--uid', type=int, required=True)
    parser.add_argument('--pid', type=int, required=True)
    parser.add_argument('--socket-dir', type=Path, required=True)
    parser.add_argument('--domain', type=int, default=74)
    args = parser.parse_args()
    if not 0 <= args.uid <= 4294967294 or args.pid <= 0 or not 0 <= args.domain <= 232:
        parser.error('explicit uid, positive pid and ROS domain 0..232 required')
    if not args.socket_dir.is_absolute() or any(ch.isspace() for ch in str(args.socket_dir)):
        parser.error('canonical absolute socket directory without whitespace required')
    args.out.mkdir(parents=True, mode=0o700, exist_ok=False)
    rows = matrix();cases = []
    for index, (name, grants, enabled, expected, error) in enumerate(derive(rows)):
        socket = str(args.socket_dir/('q'+str(index)+'.sock'))
        if len(socket.encode()) > 107:
            raise RuntimeError('fixture socket exceeds Unix path bound')
        settings = connection(args.uid, socket, args.domain)
        path = args.out/(name+'.txt');path.write_text(config(grants,settings,args.uid,args.pid,socket));path.chmod(0o400)
        cases.append({'name':name, 'enabled':enabled, 'grants':grants,
            'Worker_cli':cli(grants,settings,enabled), 'Worker_expected_error':expected,
            'Worker_expected_exit_success':expected == 0,
            'Gateway_config':{'path':str(path.resolve()), 'sha256':digest(path)},
            'Gateway_expected_startup_accepted':error is None,
            'Gateway_expected_exit_code':None if error is None else 1, 'Gateway_error_fragment':error,
            'Gateway_forbidden_socket':socket,
            'scope':'Worker CLI policy validation and native trusted startup input; no Guest execution claim'})
    manifest = {'status':'FIXTURES_PREPARED_RUNTIME_NOT_EXECUTED', 'matrix_sha256':digest(Path(__file__).with_name('action_matrix.json')),
        'generator_sha256':digest(__file__), 'uid':args.uid, 'pid':args.pid, 'domain':args.domain,
        'field_count':40, 'legal_mismatch_fields':35, 'unsupported_history_fields':5,
        'case_count':len(cases), 'cases':cases}
    (args.out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
    print(manifest['status'], 'cases='+str(len(cases)), args.out/'manifest.json')


if __name__ == '__main__':
    main()
