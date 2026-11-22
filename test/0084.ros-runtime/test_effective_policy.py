#!/usr/bin/env python3
"""Check offline ROS policy JSON and the real command parser's isolation boundary."""
import argparse
import json
from pathlib import Path
import subprocess
import tempfile

from run_scene_vm import verify_gate


DUMP = '--ros-scene-dump-effective-policy'


def check_header(report):
    assert report['schemaVersion'] == 1
    assert report['scope'] == 'configured-core-authorization' and report['validation'] == 'declarations-only'
    assert report['completeForScope'] and not report['fullDeploymentPolicy']
    assert 'native-endpoint-identity-and-alias-checks' in report['excluded']
    names = report['registeredOperations']
    assert names == sorted(set(names)) and 'permission.report' in names and 'driver.connect' in names


def check_native(path):
    raw = path.read_bytes()
    report = json.loads(raw)
    check_header(report)
    rows = {r['moduleHex']: r['module'] for r in report['memberships']}
    assert rows[b'extended-\xff'.hex()] is None and rows[b'quoted-"\\'.hex()] == 'quoted-"\\'
    session, = report['environments'][0]['sessions']
    assert session['alias'] == 'joint' and session['module'] == 'main'
    assert session['peerPid'] == '12345' and session['operation'] == 'parameter.exists'
    assert session['sessionId'] == '09000000000000000a00000000000000' and session['schemaDepth'] == '8'
    assert session['transport'] == 'mutual-tls-1.3'
    for number in range(1023456, 1023460):
        assert str(number).encode() not in raw


def check_cli(runtime):
    def run(args, okay=True, fds=()):
        reply = subprocess.run([runtime, *args], capture_output=True, pass_fds=fds, timeout=25)
        if not okay:
            assert reply.returncode > 0 and not reply.stdout, (args, reply)
            return
        assert reply.returncode == 0, (args, reply.stderr)
        report = json.loads(reply.stdout)
        check_header(report)
        return report

    assert run([DUMP])['environments'] == []
    module = 'main'
    root = ['--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', module]
    ids = [bytes(range(i, i + 16)).hex() for i in range(1, 6)]
    digest = bytes(range(32)).hex()
    child = 'uwvm_ros_v2.platform_extension.agilex.v1'
    with tempfile.TemporaryFile() as journal:
        journal_flags = ['--ros-scene-call-buffer-journal', str(journal.fileno()), '10', '4096', '64']
        flags = [*root, '--ros-scene-bind', 'robot', 'worker', '--ros-scene-interface', 'robot', 'driver.connect', 'deny',
                 '--ros-scene-grant', 'robot', 'joint', 'driver', 'elbow', 'registered-driver', 'driver.connect',
                 '--ros-scene-limit', 'robot', 'joint', 'max-handles', '0',
                 '--ros-scene-limit', 'robot', 'joint', 'delivery', 'may-reject',
                 '--ros-scene-limit', 'robot', 'joint', 'calls-per-window', '1',
                 '--ros-scene-driver-budget', 'robot', '1', '2000000',
                 '--ros-scene-driver-channel', 'robot', 'joint', module, '1023456', '12345', '222', '333',
                 *ids, digest, '32', '48', '50000', '9007199254740993', '1023457', '1023458',
                 '--ros-scene-driver-schema', 'robot', 'joint', 'device.describe', '16', '24', '8',
                 '--ros-scene', 'robot', 'platform_extension', child, 'disable',
                 '--ros-scene', 'robot', 'platform_extension.bind', child, 'joint',
                 '--ros-scene', 'robot', 'permission.report', 'wasip1.fd_write', 'deny',
                 '--ros-scene', 'robot', 'permission.report', 'ros1v1', 'deny',
                 '--ros-scene-grant', 'robot', 'cal', 'calibration', 'site', 'sealed-input', 'artifact.read',
                 '--ros-scene-input-budget', 'robot', '32', '1',
                 '--ros-scene-input', 'robot', 'cal', '/absent/secret-calibration.pem', digest, '1', '32', '32',
                 '--ros-scene-call-audit', 'global', '*', 'all', '17', '8192', '4', '2',
                 '--ros-scene-call-budget', 'robot', '65536', '1',
                 '--ros-scene-call', 'robot', 'sample', module, 'worker', 'read', 'input', 'output', '8', '8', '1', '6000',
                 '--ros-scene-call-clear-input', 'robot', 'sample', 'false', *journal_flags,
                 '--ros-scene-call-buffer-capture', 'robot', 'sample', 'take']
        report = run([*flags, DUMP], fds=(journal.fileno(),))
        assert report['defaults']['memberAbi'] == dict(operation='abi', effective='allow')
        assert report['defaults']['memberSelfReport'] == dict(operation='permission.report', effective='allow')
        text = json.dumps(report)
        assert all(value not in text for value in ['secret-calibration', '1023456', '1023457', '1023458'])
        assert report['payloadJournal'] == dict(records='10', bytes='4096', payloadBytes='64')
        assert report['callAuditRules'] == [dict(scope='global', label='*', mode='all', records='17', bytes='8192', rateHz='4', burst='2')]
        env, = report['environments']
        assert env['configuredImports'] == [dict(operation='driver.connect', configured='deny', effective='deny')]
        assert env['platformSelections'] == [dict(module=child, enabled=False)]
        assert env['observationDenials'] == [dict(family='ros1v1', function=None), dict(family='wasip1', function='fd_write')]
        grant = next(g for g in env['grants'] if g['alias'] == 'joint')
        assert grant == dict(alias='joint', purpose='driver', resource='elbow', binding='registered-driver', actions=['driver.connect'],
                             platformOwner=child, delivery='may-reject', maxHandles='0', maxResources='unlimited',
                             maxWaitMembers='unlimited', maxWaitNs='unlimited', infiniteWait=False,
                             callsPerWindow='1', bytesPerWindow='unlimited', windowNs='1000000000')
        session, = env['sessions']
        assert [session[k] for k in ('deploymentId', 'vmId', 'scopeId', 'providerId', 'sessionId')] == ids
        assert session['contractSha256'] == digest and session['sessionUntilNs'] == '9007199254740993'
        assert session['transport'] == 'mutual-tls-1.3' and session['peerClock'] == 'provisioned-namespace'
        route, = env['messageRoutes']
        assert route['captureStages'] == ['take'] and not route['clearInputAfterCopy']
        for suffix in ([DUMP, '--run', '/absent'], ['--help', DUMP], ['--ros-scene-check', DUMP],
                       [DUMP, 'stray'], ['stray', DUMP], [DUMP, DUMP], ['--wasm-register-dl', '/absent', DUMP]):
            run([*journal_flags, *suffix], okay=False, fds=(journal.fileno(),))
        assert journal.seek(0, 2) == 0
        invalid = flags.copy()
        invalid[invalid.index(ids[4])] = '0' * 32
        run([*invalid, DUMP], okay=False)
        # Distinct modes retain their established native checks.
        run([*flags[:flags.index('--ros-scene-driver-schema')], '--ros-scene-check'], okay=False)

    from run_model_attachment_vm import flags as model_flags
    flags = model_flags(1023456, 12345, 9007199254740993)
    channel = flags.index('--ros-scene-model-kernel-channel')
    flags[channel] = '--ros-scene-model-channel'
    flags[channel + 19:channel + 19] = ['1023457', '1023458']
    model = run([*flags, DUMP])['environments'][0]['sessions'][0]
    assert model['kind'] == 'model' and model['alias'] == 'brain' and model['transport'] == 'mutual-tls-1.3'
    assert model['supervisorBytes'] == '1048576' and model['sessionUntilNs'] == '9007199254740993'


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument('--native-report', type=Path)
    source.add_argument('--runtime', type=Path)
    args = parser.parse_args()
    verify_gate()
    if args.runtime:
        check_cli(args.runtime.resolve())
    else:
        check_native(args.native_report)
    print('PASS offline policy export')
