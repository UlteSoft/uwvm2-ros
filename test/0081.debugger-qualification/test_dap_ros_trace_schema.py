#!/usr/bin/env python3
"""Actual-formatter roundtrips and strict detached ROS trace parser contracts.

Only the C++ fixture executes the real trace_store/console formatter. Synthetic
packets and the mathematical broker below qualify parsing/fences, not ROS calls.
Run on Linux after transferring the complete thin packet; never on macOS.
"""
import argparse
import base64
import importlib.util
import io
import json
from pathlib import Path
import sys
import unittest

parser = argparse.ArgumentParser(add_help=False)
parser.add_argument('--adapter', type=Path, default=Path(__file__).resolve().parents[2]/'tools/debug/dap_adapter.py')
parser.add_argument('--packets', type=Path)
parser.add_argument('--legacy-root', type=Path)
parser.add_argument('--report', type=Path)
args, unittest_args = parser.parse_known_args()
spec = importlib.util.spec_from_file_location('qualified_dap_trace', args.adapter)
dap = importlib.util.module_from_spec(spec)
spec.loader.exec_module(dap)
U64 = (1 << 64)-1
REASONS = {0:'success', 2:'unknown_module', 9:'bad_handle', 11:'rate_limited', 13:'permission_denied',
           17:'conflict', 22:'invalid_argument', 90:'too_large', 95:'unsupported', 107:'disconnected',
           110:'timeout', 125:'revoked'}
STOP = 'stopped: cooperative pause\nstop-id 19\nthread 7 module=0 function=0 byte-offset=0 generation=1\n'


def call(seq=7, errno=13, known=True, resource='"/topic_42"', reason=None):
    if reason is None:
        reason = REASONS.get(errno, 'unclassified') if known else 'unobserved'
    return (f'call seq={seq} ns=100 backend=ros2 instance=3 generation=5 module="main" function="publish" '
            f'op="topic.publish" alias="pub" resource={resource} errno={errno} reason={reason} '
            f'result_known={int(known)} unresolved=0')


def packet(rows=(), *, after=0, total=None, more=0, gap=0, overwritten=0, busy=0, sequence=0):
    total = len(rows) if total is None else total
    next_value = int(rows[-1].split('seq=')[1].split()[0]) if rows else after
    return ('ros scope=process-owned stop=19 selection=trace status=available\n'+
            ''.join(row+'\n' for row in rows)+f'total={total} next={next_value} more={more} gap={gap} '
            f'overwritten={overwritten} dropped_busy={busy} dropped_sequence={sequence}\n')


def parse(body, **options):
    return dap.parse_ros_state(body, dap.ros_state_options(dict(selection='trace', **options)), 19)


class TraceSchema(unittest.TestCase):
    def test_public_redacted_empty_and_truncated_are_distinct(self):
        for literal, raw, redacted, truncated in [('"/topic_42"', b'/topic_42', False, False),
                ('<redacted>', None, True, False), ('""', b'', False, False),
                ('"/'+('n'*63)+'"[truncated]', b'/'+b'n'*63, False, True)]:
            with self.subTest(literal=literal):
                row = parse(packet([call(resource=literal)]))['rows'][0]
                self.assertEqual(row['resourceRedacted'], redacted)
                if raw is None:
                    self.assertIsNone(row['resource'])
                else:
                    self.assertEqual(base64.b64decode(row['resource']['bytesBase64']), raw)
                    self.assertEqual(row['resource']['truncated'], truncated)

    def test_all_stable_reasons_and_unobserved_results(self):
        for errno, reason in REASONS.items():
            self.assertEqual(parse(packet([call(errno=errno)]))['rows'][0]['reason'], reason)
        self.assertEqual(parse(packet([call(errno=(1 << 32)-1)]))['rows'][0]['reason'], 'unclassified')
        row = parse(packet([call(errno=0, known=False)]))['rows'][0]
        self.assertIsNone(row['errno']); self.assertEqual(row['reason'], 'unobserved')
        for row in (call(reason='success'), call(errno=0, known=False, reason='success'),
                    call(known=False), call(errno=1 << 32)):
            with self.assertRaises(ValueError): parse(packet([row]))
        with self.assertRaises(ValueError): parse(packet([call(errno=0, known=False)]), error=0)

    def test_new_fields_are_atomic_and_legacy_schema_is_explicit(self):
        old = call().replace(' resource="/topic_42"', '').replace(' reason=permission_denied', '')
        old_packet = packet([old]).replace(' dropped_sequence=0', '')
        row = parse(old_packet)['rows'][0]
        self.assertNotIn('resource', row); self.assertNotIn('reason', row)
        self.assertNotIn('dropped_sequence', parse(old_packet))
        for body in (packet([old]), packet([call()]).replace(' dropped_sequence=0', ''),
                     packet([call().replace(' reason=permission_denied', '')]),
                     packet([call().replace(' resource="/topic_42"', '')]),
                     packet([old, call(seq=8)])):
            with self.assertRaises(ValueError): parse(body)

    def test_unsafe_or_invalid_public_resource_is_refused(self):
        for literal in ('"https://secret/token"', '"relative"', '"/bad-name"', '"/bad\\x00"',
                        '"/bad\\xff"', '"/'+('a'*64)+'"', '"/short"[truncated]', '<invalid>'):
            with self.subTest(literal=literal), self.assertRaises(ValueError):
                parse(packet([call(resource=literal)]))

    def test_loss_counters_preserve_unsigned_bounds_without_fabricating_zero(self):
        value = parse(packet([call()], overwritten=U64, busy=U64, sequence=U64))
        for key in ('overwritten', 'dropped_busy', 'dropped_sequence'): self.assertEqual(value[key], U64)
        for key in ('overwritten', 'dropped_busy', 'dropped_sequence'):
            for text in ('-1', str(1 << 64), '01', 'true', '"0"'):
                with self.subTest(key=key, text=text), self.assertRaises(ValueError):
                    parse(packet([call()]).replace(key+'=0', key+'='+text))

    def test_filtered_gaps_and_empty_cursor_are_not_ring_loss(self):
        value = parse(packet([call(seq=5), call(seq=9)], total=3, more=1), count=2)
        self.assertEqual([row['seq'] for row in value['rows']], [5,9])
        self.assertFalse(value['gap']); self.assertEqual(value['next'], 9)
        for after in (39, U64):
            value = parse(packet(after=after), after=after)
            self.assertEqual(value['next'], after); self.assertEqual(value['rows'], [])
            with self.assertRaises(ValueError): parse(packet(after=0), after=after)
        for rows in ([call(seq=7),call(seq=7)], [call(seq=9),call(seq=7)]):
            with self.assertRaises(ValueError): parse(packet(rows))

    def test_unknown_duplicate_and_private_epoch_fields_fail_closed(self):
        for suffix in (' reason=permission_denied', ' resource_redacted=1', ' epoch=3', ' native_pointer=1'):
            with self.assertRaises(ValueError): parse(packet([call()+suffix]))
        for suffix in (' dropped_sequence=0', ' epoch=3', ' dropped_payload=1'):
            with self.assertRaises(ValueError): parse(packet([call()])[:-1]+suffix+'\n')

    def test_malformed_or_partial_packet_is_never_published(self):
        body = packet([call()])
        for text in (body[:-1], body+'extra\n', body.replace('stop=19','stop=20'),
                     body.replace('errno=13','errno=01'), body.replace('total=1','total=2'),
                     body.replace('reason=permission_denied','reason="permission_denied"'),
                     body.replace('\n','\r\n')):
            with self.assertRaises(ValueError): parse(text)

    def test_query_preserves_loss_but_retires_changed_stop(self):
        class MathematicalBroker:
            def __init__(self, replies): self.replies=list(replies); self.commands=[]
            def request(self, text): self.commands.append(text); return self.replies.pop(0)
        for changed in (False, True):
            value = dap.Adapter(io.BytesIO())
            value.broker = MathematicalBroker([STOP, packet([call()], busy=2, sequence=3),
                STOP.replace('stop-id 19', 'stop-id 20') if changed else STOP])
            if changed:
                value.ros_scope_stops[1]=('old','trace')
                with self.assertRaises(ValueError): value.query_ros_state({'selection':'trace'})
                self.assertIsNone(value.stop_key); self.assertEqual(value.ros_scope_stops,{})
            else:
                result = value.query_ros_state({'selection':'trace'})
                self.assertEqual((result['dropped_busy'],result['dropped_sequence']),(2,3))
                self.assertFalse(result['payloadsRead']); self.assertFalse(result['nativeOperationsIssued'])
                self.assertEqual(value.broker.commands,
                    ['status','trace ros backend=all count=16 if-stop=19','status'])


@unittest.skipUnless(args.packets, 'requires compiled actual formatter/store packets')
class ActualFormatter(unittest.TestCase):
    def body(self, name): return (args.packets/(name+'.packet')).read_text(encoding='ascii')
    def test_actual_public_escaped_redacted_truncated_and_unobserved(self):
        self.assertEqual(parse(self.body('public'))['rows'][0]['resource']['text'], '/topic_42')
        row = parse(self.body('escaped'))['rows'][0]
        self.assertEqual(base64.b64decode(row['module']['bytesBase64']), b'm\n\xff"')
        for name in ('automatic-redaction','explicit-redaction'):
            row = parse(self.body(name))['rows'][0]
            self.assertTrue(row['resourceRedacted']); self.assertIsNone(row['resource'])
            self.assertNotIn('secret', self.body(name))
        row = parse(self.body('truncated'))['rows'][0]
        self.assertTrue(row['resource']['truncated']); self.assertEqual(row['resource']['text'],'/'+'n'*63)
        row = parse(self.body('unobserved'))['rows'][0]
        self.assertIsNone(row['errno']); self.assertEqual(row['reason'],'unobserved')
        self.assertEqual(row['resource']['text'],''); self.assertFalse(row['resourceRedacted'])

    def test_actual_stable_errno_mapping(self):
        for errno, reason in list(REASONS.items())+[((1 << 32)-1,'unclassified')]:
            row = parse(self.body('errno-'+str(errno)))['rows'][0]
            self.assertEqual((row['errno'],row['reason']),(errno,reason))

    def test_actual_ring_overwrite_nonconsuming_continuation_and_empty_cursor(self):
        self.assertEqual(self.body('ring-first'),self.body('ring-repeat'))
        value = parse(self.body('ring-first'),after=1,count=2)
        self.assertEqual([row['seq'] for row in value['rows']],[3,4])
        self.assertEqual((value['total'],value['next'],value['more'],value['gap'],value['overwritten']),
                         (256,4,True,True,2))
        value = parse(self.body('ring-next'),after=4,count=2)
        self.assertEqual([row['seq'] for row in value['rows']],[5,6])
        self.assertEqual((value['total'],value['more'],value['gap'],value['overwritten']), (254,True,False,2))
        for name, after in (('empty-after',39),('empty-max-after',U64)):
            self.assertEqual(parse(self.body(name),after=after)['next'],after)

    def test_actual_boundary_loss_and_nontrace_current_footers(self):
        value = parse(self.body('saturated-loss'))
        for key in ('next','overwritten','dropped_busy','dropped_sequence'): self.assertEqual(value[key],U64)
        for name, selected in (('capability','capabilities'),('calls','calls')):
            value = dap.parse_ros_state(self.body(name),dap.ros_state_options({'selection':selected}),19)
            self.assertEqual(value['dropped_sequence'],0)
            if selected == 'calls':
                self.assertEqual((value['dropped_busy'],value['dropped_full'],value['dropped_labels']),(1,2,3))
                with self.assertRaises(ValueError):
                    dap.parse_ros_state(self.body(name).replace('dropped_sequence=0','dropped_sequence=1'),
                                        dap.ros_state_options({'selection':selected}),19)


def load_tests(loader, tests, pattern):
    if args.legacy_root:
        for relative in ('test/0073.ros-tests/support/source/ros_debugger/test_dap_ros_state.py',
                         'test/0073.ros-tests/support/source/ros_debugger/test_dap_ros_interface_statistics.py',
                         'test/0073.ros-tests/support/source/ros_service_availability/test_debug_protocol.py'):
            spec = importlib.util.spec_from_file_location('legacy_'+Path(relative).stem, args.legacy_root/relative)
            module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
            bound = module.suite(dap) if hasattr(module,'suite') else module.cases(dap)
            tests.addTests(loader.loadTestsFromTestCase(bound) if isinstance(bound,type) else bound)
    return tests


if __name__ == '__main__':
    program = unittest.main(argv=[sys.argv[0],*unittest_args], exit=False)
    result = program.result
    passed = result.wasSuccessful() and (not args.packets or not result.skipped)
    if args.report:
        args.report.write_text(json.dumps({'passed':passed, 'tests':result.testsRun,
            'failures':len(result.failures), 'errors':len(result.errors), 'skipped':len(result.skipped),
            'actual_formatter':bool(args.packets), 'legacy_compatibility':bool(args.legacy_root),
            'ros_runtime_qualified':False},indent=2)+'\n')
    raise SystemExit(0 if passed else 1)
