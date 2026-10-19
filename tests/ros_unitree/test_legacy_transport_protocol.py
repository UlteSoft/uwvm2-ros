#!/usr/bin/env python3
"""Guarded format/policy regressions for the legacy ROS1 acceptance harness.

These cases execute no Runtime, ROS master, Gateway, native witness, compiler,
network request or namespace operation. Synthetic wire records test strict
parsing and refusal; they are not ROS serialization or transport evidence.
Root must still execute the twelve genuine TCPROS cases separately.
Invoke only in the explicitly selected current64GiB/zero-swap cgroup using a
complete source manifest and ordinary Unitree metadata-only configuration.
"""
from __future__ import annotations
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import stat
import sys
import types
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.dont_write_bytecode = True

def require(value, reason):
    if not value:
        raise ValueError(reason)

def bounded(path, limit):
    """Read one stable ordinary bootstrap file without importing project code."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute bootstrap file required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= limit, 'bounded bootstrap file required')
        chunks, size = [], 0
        while True:
            block = os.read(descriptor, min(65536, limit + 1 - size))
            if not block:
                break
            chunks.append(block)
            size += len(block)
            require(size <= limit, 'bootstrap file grew')
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        require(size == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'bootstrap file changed')
        return b''.join(chunks)
    finally:
        os.close(descriptor)

def bootstrap(args):
    """Bind this entrypoint and first helper before any project helper executes."""
    source = args.source.resolve(strict=True)
    require(source == ROOT, 'invoke actual complete frozen source entrypoint')
    raw = bounded(args.manifest, 16 * 1024**2)
    require(hashlib.sha256(raw).hexdigest() == args.manifest_sha256, 'explicit current source manifest hash differs')
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, 'duplicate source manifest key')
            result[key] = value
        return result
    frozen = json.loads(raw, object_pairs_hook=pairs)
    require(type(frozen) is dict and frozen, 'complete frozen source manifest required')
    helper = None
    for relative in ('tests/ros_unitree/test_legacy_transport_protocol.py', 'tests/ros_javascript_guest/run.py'):
        body = bounded(source / relative, 1024**2)
        require(frozen.get(relative) == {'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()},
                'actual entry/helper missing from complete frozen manifest')
        if relative.endswith('ros_javascript_guest/run.py'):
            helper = body
    api = types.ModuleType('legacy_protocol_regression_execution')
    api.__file__ = str(source / 'tests/ros_javascript_guest/run.py')
    sys.modules[api.__name__] = api
    exec(compile(helper, api.__file__, 'exec'), api.__dict__)
    unitree = api.frozen_module(source, frozen, 'tests/ros_unitree/common.py', 'legacy_protocol_regression_owners')
    return source, frozen, api, unitree


def suite(frame, api, session, admission):
    """Construct independent positive/negative fixtures; importing runs none."""
    class Protocol(unittest.TestCase):
        def native_record(self, size=1069, crc=0x13572468):
            raw = bytes(index % 251 for index in range(size))
            digest = 14695981039346656037
            for byte in raw:
                digest = ((digest ^ byte) * 1099511628211) & ((1 << 64) - 1)
            return 'LEGACY_STATE %d %d %s %d' % (size, crc, raw.hex(), digest)

        def audit(self, records):
            path = frame.out / (self._testMethodName + '.stderr')
            raw = b''.join((json.dumps(item, separators=(',', ':')) + '\n').encode() for item in records)
            with path.open('xb') as stream:
                stream.write(raw)
            return {'stderr': str(path)}

        def denied(self):
            return {'wall_ns': 123, 'ros': 'ros1', 'module': 'main', 'runtime_generation': 4,
                'observation_instance': 5, 'event': 'unresolved-denial', 'cross_module': False, 'error': 13}

        def test_complete_payload_and_independent_fnv(self):
            result = session.payload(self.native_record(), 'LEGACY_STATE', 1069, 0x13572468)
            self.assertEqual(result['bytes'], 1069)
            self.assertEqual(len(result['canonical_hex']), 2138)

        def test_changed_middle_payload_cannot_keep_old_digest(self):
            parts = self.native_record().split(' ')
            offset = 177
            parts[3] = parts[3][:offset] + ('1' if parts[3][offset] != '1' else '2') + parts[3][offset + 1:]
            with self.assertRaisesRegex(ValueError, 'FNV differs'):
                session.payload(' '.join(parts), 'LEGACY_STATE', 1069, 0x13572468)

        def test_trailing_or_uppercase_payload_is_not_canonical(self):
            for changed in (self.native_record() + ' 0', self.native_record().replace('0a', '0A', 1)):
                with self.assertRaisesRegex(ValueError, 'payload shape differs'):
                    session.payload(changed, 'LEGACY_STATE', 1069, 0x13572468)

        def test_crc_marker_and_complete_length_both_required(self):
            for changed in (self.native_record(size=1068), self.native_record(crc=0)):
                with self.assertRaisesRegex(ValueError, 'payload shape differs'):
                    session.payload(changed, 'LEGACY_STATE', 1069, 0x13572468)

        def test_noncanonical_or_overflowing_numbers_refused(self):
            for value in ('00', '+1', '-1', '1.0', str(1 << 64), ' 1'):
                with self.assertRaises(ValueError):
                    session.unsigned(value)

        def test_quiet_requires_elapsed_samples_and_exact_callbacks(self):
            self.assertEqual(session.quiet('LEGACY_QUIET 1 30 2000000000 1 1', True)['samples'], 30)
            for record in ('LEGACY_QUIET 1 29 2000000000 1 1', 'LEGACY_QUIET 1 30 1999999999 1 1',
                    'LEGACY_QUIET 1 30 2000000000 1 2', 'LEGACY_QUIET 0 30 2000000000 0 1'):
                with self.assertRaisesRegex(ValueError, 'quiet interval/callback counts differ'):
                    session.quiet(record, record.split(' ')[1] == '1')

        def test_default_denial_is_two_observations_without_ipc(self):
            result = session.denial_audit(api, frame, self.audit([self.denied(), self.denied()]))
            self.assertEqual(len(result['records']), 2)
            self.assertIs(result['ROS_target_enabled'], False)
            self.assertIs(result['Gateway_started'], False)
            self.assertEqual(result['alias_grants'], [])

        def test_missing_or_extra_actual_denial_is_refused(self):
            # Distinct owned files let both failures remain in the final ledger.
            for count in (1, 3):
                path = frame.out / (self._testMethodName + '-%d.stderr' % count)
                with path.open('x') as stream:
                    for _ in range(count):
                        stream.write(json.dumps(self.denied()) + '\n')
                with self.assertRaisesRegex(ValueError, 'exactly two'):
                    session.denial_audit(api, frame, {'stderr': str(path)})

        def test_denials_cannot_change_principal_generation_or_status(self):
            variants = [('module', 'application'), ('runtime_generation', 6), ('error', 11), ('cross_module', True)]
            for index, (key, value) in enumerate(variants):
                changed = self.denied(); changed[key] = value
                path = frame.out / (self._testMethodName + '-%d.stderr' % index)
                with path.open('x') as stream:
                    stream.write(json.dumps(self.denied()) + '\n' + json.dumps(changed) + '\n')
                with self.assertRaises(ValueError):
                    session.denial_audit(api, frame, {'stderr': str(path)})

        def test_disabled_target_policy_grants_no_alias_or_gateway(self):
            fake = types.SimpleNamespace(workers={'dual': Path('/synthetic/dual-worker')})
            product = {'artifact': {'path': '/synthetic/main.wasm'}, 'boundary': {'local_entry': 7}}
            argv = session.runtime_argv(api, fake, product, 'dual', False, {}, [], Path('/synthetic/s0'))
            self.assertIn('--ros1-global-disable', argv)
            self.assertIn('--ros2-global-disable', argv)
            self.assertEqual(argv.count('--ros1-single-disable'), 1)
            self.assertEqual(argv.count('--ros2-single-disable'), 1)
            self.assertFalse(any(token in ('--ros1-single-grant', '--ros1-single-gateway-socket',
                '--ros1-single-enable', '--ros2-single-enable') for token in argv))
            self.assertEqual(sum(token.startswith('--ros1-single-audit-deny-') for token in argv), 4)
            self.assertEqual(argv[-2:], ['--run', '/synthetic/main.wasm'])

        def test_positive_policy_exact_native_and_canonical_identities(self):
            identities = {'unitree_legged_msgs/LowState': '1' * 32, 'unitree_legged_msgs/LowCmd': '2' * 32}
            codecs = {name: 'UWVMROS1_' + str(index) * 64 for index, name in enumerate(identities, 3)}
            connection, grants = admission.policy(identities, 11881, True, codecs)
            self.assertEqual(connection['master-uri'], 'http://127.0.0.1:11881')
            self.assertEqual({row['alias'] for row in grants}, {'legacy-lowstate', 'legacy-lowcmd'})
            for row in grants:
                self.assertEqual(row['identity'], identities[row['type']])
                self.assertEqual(row['limits']['adapter-codec-identity'], codecs[row['type']])
                self.assertEqual(row['limits']['queue-depth'], '1')
                self.assertEqual(row['limits']['burst'], '1')
            self.assertEqual(grants[1]['limits']['latch'], 'true')
            fake = types.SimpleNamespace(workers={'dual': Path('/synthetic/dual-worker')})
            product = {'artifact': {'path': '/synthetic/main.wasm'}, 'boundary': {'local_entry': 9}}
            argv = session.runtime_argv(api, fake, product, 'dual', True, connection, grants, Path('/synthetic/s0'))
            self.assertEqual(argv.count('--ros1-single-enable'), 1)
            self.assertEqual(argv.count('--ros1-single-grant'), 2)
            self.assertEqual(argv.count('--ros2-single-disable'), 1)
            self.assertNotIn('--ros2-single-enable', argv)

        def test_xmlrpc_transport_refuses_an_alternate_host(self):
            transport = session.LoopbackTransport(11881)
            for address in ('localhost:11881', '127.0.0.1:11882', 'example.invalid:11881', '[::1]:11881'):
                with self.assertRaisesRegex(ValueError, 'literal IPv4 loopback'):
                    transport.make_connection(address)

        def test_xmlrpc_refuses_large_or_compressed_body_before_reading(self):
            for headers in ({'Content-Length': '65537'}, {'Content-Length': '12', 'Content-Encoding': 'gzip'},
                            {'Content-Length': '12', 'Transfer-Encoding': 'chunked'}):
                reads = []
                def body_read(size):
                    reads.append(size)
                    return b'x' * min(size, 12)
                response = types.SimpleNamespace(getheader=lambda key, default=None: headers.get(key, default), read=body_read)
                with self.assertRaises(ValueError):
                    session.LoopbackTransport(11881).parse_response(response)
                self.assertEqual(reads, [], 'unqualified response must be refused before body allocation')

        def test_positive_audit_requires_both_main_owners_closed(self):
            records = []
            for operation, resource in (('topic.subscribe', 'lowstate'), ('topic.publish', 'lowcmd')):
                for event in ('resource-create', 'resource-close'):
                    records.append({'time_unix_ns': 123, 'ros_version': 'ros1', 'module': 'main', 'instance': 9,
                        'event': event, 'operation': operation, 'resource': '/uwvm_unitree_legacy/' + resource,
                        'result': 'allow', 'error': 0})
            row = self.audit(records)
            self.assertEqual(len(session.positive_audit(api, frame, row)['records']), 4)
            for index, changed in enumerate((records[:-1], [*records[:-1], {**records[-1], 'module': 'library'}])):
                path = frame.out / (self._testMethodName + '-%d.stderr' % index)
                with path.open('x') as stream:
                    for item in changed:
                        stream.write(json.dumps(item) + '\n')
                with self.assertRaises(ValueError):
                    session.positive_audit(api, frame, {'stderr': str(path)})

    return unittest.defaultTestLoader.loadTestsFromTestCase(Protocol)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('source', 'manifest', 'config', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--manifest-sha256', required=True)
    parser.add_argument('--config-sha256', required=True)
    args = parser.parse_args()
    frame = None
    try:
        source, frozen, api, unitree = bootstrap(args)
        frame = unitree.Frame(args, 'unitree-legacy-protocol-format-regression-only')
        require(frame.config['environment'] == {} and all(frame.config[key] is None for key in
                ('simulation', 'benchmark', 'profiler')), 'metadata-only ordinary configuration required')
        frame.unitree, frame.frozen = unitree, frozen
        session = api.frozen_module(source, frozen, 'tests/ros_unitree/legacy_transport_session.py', 'legacy_protocol_regression_session')
        admission = api.frozen_module(source, frozen, 'tests/ros_unitree/legacy_transport_inputs.py', 'legacy_protocol_regression_admission')
        log = io.StringIO()
        result = unittest.TextTestRunner(stream=log, verbosity=2).run(suite(frame, api, session, admission))
        raw = log.getvalue().encode()
        require(len(raw) <= 1024**2, 'bounded regression log required')
        path = frame.out / 'tests.log'
        with path.open('xb') as stream:
            stream.write(raw)
        frame.report.update(tests=result.testsRun, failures=len(result.failures), errors=len(result.errors),
            test_log=frame.pin(path), Runtime_executed=False, ROS_executed=False,
            scope='Fourteen finite format/policy regressions only; synthetic bytes are not native ROS or TCPROS evidence')
        require(result.testsRun == 14 and result.wasSuccessful() and not frame.actors, 'all fourteen guarded source regressions required')
        return frame.finish('PASS_LEGACY_TRANSPORT_PROTOCOL_FORMAT_REGRESSIONS_ONLY')
    except BaseException as error:
        if frame is not None:
            return frame.finish('FAIL', repr(error))
        print(repr(error), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())

