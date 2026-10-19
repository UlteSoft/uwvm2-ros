"""Injected DATA regressions for actual unittest lifecycle accounting.

Root supplies the complete-MF-admitted formats module. No observer, original
test module, subprocess, compiler, ROS or filesystem is invoked by this factory.
The separate fixture file can be run only by Root through the real SDK lane.
"""
import json
import unittest


def suite(formats):
    def raw(value): return json.dumps(value,allow_nan=False).encode()
    def record(event,identity=None,status=None,detail=None):
        return {'event':event,'id':identity,'status':status,'detail':detail}
    identity='original_module.Original.test_echo'
    inventory={'format_version':1,'kind':'actual-upstream-unittest-collection',
        'modules':['original_module'],'nodes':[{'id':identity,'file':'original_module.py','line':12}], 'loader_errors':[]}
    def observe(records, collection=None):
        return formats.unittest_events(raw(collection or inventory),b'\n'.join(raw(value) for value in records))
    def lifecycle(status='PASS'):
        return [record('run-start'),record('start',identity),record('outcome',identity,status),
                record('stop',identity),record('run-stop')]
    class Accounting(unittest.TestCase):
        def test_complete_original_case(self):
            value=observe(lifecycle());self.assertTrue(formats.summary(value)['complete_success'])
        def test_skipped_not_pass(self):
            self.assertFalse(formats.summary(observe(lifecycle('SKIP')))['complete_success'])
        def test_xfail_not_pass(self):
            self.assertEqual(observe(lifecycle('XFAIL'))['cases'][0]['status'],'XFAIL')
        def test_xpass_not_pass(self):
            self.assertFalse(formats.summary(observe(lifecycle('XPASS')))['complete_success'])
        def test_failure_then_teardown_error(self):
            events=lifecycle('FAIL');events.insert(3,record('outcome',identity,'ERROR','tearDown'))
            self.assertEqual(observe(events)['cases'][0]['status'],'ERROR')
        def test_missing_run_stop(self):
            self.assertEqual(observe(lifecycle()[:-1])['cases'][0]['status'],'UNEXECUTED_OR_INCOMPLETE')
        def test_missing_case_stop(self):
            events=lifecycle();del events[3]
            self.assertFalse(formats.summary(observe(events))['complete_success'])
        def test_fixture_error_does_not_invent_method(self):
            value=observe([record('run-start'),record('fixture','setUpClass (original_module.Original)','ERROR','bad fixture'),record('run-stop')])
            self.assertEqual(len(value['cases']),1);self.assertTrue(value['accounting_gaps'])
        def test_subtest_failure_retained(self):
            value=observe([record('run-start'),record('start',identity),record('subtest',identity,'FAIL',{'id':identity+' (x=1)','error':'x'}),record('stop',identity),record('run-stop')])
            self.assertEqual(value['cases'][0]['status'],'FAIL')
        def test_passing_subtests_not_extra_cases(self):
            events=lifecycle();events.insert(2,record('subtest',identity,'PASS',{'id':identity+' (x=1)','error':None}))
            self.assertEqual(len(observe(events)['cases']),1)
        def test_unknown_case_refused(self):
            with self.assertRaises(ValueError):observe([record('run-start'),record('start','invented')])
        def test_duplicate_case_start_refused(self):
            events=lifecycle();events.insert(2,record('start',identity))
            with self.assertRaises(ValueError):observe(events)
        def test_events_after_run_stop_refused(self):
            with self.assertRaises(ValueError):observe(lifecycle()+[record('run-stop')])
        def test_loader_error_is_gap(self):
            value=dict(inventory,loader_errors=['original import failed'])
            self.assertFalse(formats.summary(observe(lifecycle(),value))['complete_success'])
        def test_empty_universe_not_pass(self):
            self.assertFalse(formats.summary(observe([record('run-start'),record('run-stop')],dict(inventory,nodes=[])))['complete_success'])
    return unittest.defaultTestLoader.loadTestsFromTestCase(Accounting)
