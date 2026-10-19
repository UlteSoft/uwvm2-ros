"""Source-only result-accounting regression factory, with no candidate imports.

Root calls suite(formats, differential) only after admitting these three files
against its actual current complete freeze and current64 owner. These are DATA
format/policy tests, not ROS, Wasm, Gateway, QEMU or official-suite execution.
The factory has no filesystem fixtures, subprocesses or project imports.
"""
import json
import unittest


def suite(formats, differential):
    def encoded(value): return json.dumps(value, allow_nan=False).encode()
    def pytest_case(outcome='passed', xfail='', *, deselected=None):
        inventory = encoded({'format_version':1, 'kind':'actual-upstream-pytest-collection',
            'nodes':[{'id':'test_original.py::case', 'location':['test_original.py',1,'case']}],
            'deselected':deselected or []})
        events = b'\n'.join(encoded({'nodeid':'test_original.py::case','when':phase,
            'outcome':outcome if phase == 'call' else 'passed','wasxfail':xfail if phase == 'call' else ''})
            for phase in ('setup','call','teardown'))
        return formats.pytest_events(inventory,events)
    class Accounting(unittest.TestCase):
        def test_one_actual_junit_case(self):
            value=formats.junit(b'<testsuite tests="1" failures="0"><testcase name="actual"/></testsuite>')
            self.assertTrue(formats.summary(value)['complete_success'])
        def test_missing_nine_declared_cases(self):
            value=formats.junit(b'<testsuite tests="10"><testcase name="actual"/></testsuite>')
            self.assertFalse(formats.summary(value)['complete_success']);self.assertTrue(value['accounting_gaps'])
        def test_claimed_failures_without_rows(self):
            value=formats.junit(b'<testsuite tests="1" failures="1"><testcase name="actual"/></testsuite>')
            self.assertFalse(formats.summary(value)['complete_success'])
        def test_nested_counters_are_not_summed(self):
            value=formats.junit(b'<testsuites tests="2"><testsuite tests="1"><testcase name="a"/></testsuite><testsuite tests="1"><testcase name="b"/></testsuite></testsuites>')
            self.assertEqual(len(value['cases']),2);self.assertEqual(len(value['declared_counters']),3)
            self.assertTrue(formats.summary(value)['complete_success'])
        def test_nested_parent_mismatch_remains(self):
            value=formats.junit(b'<testsuites tests="3"><testsuite tests="1"><testcase name="a"/></testsuite></testsuites>')
            self.assertFalse(formats.summary(value)['complete_success'])
        def test_negative_declared_counter_refused(self):
            with self.assertRaises(ValueError):formats.junit(b'<testsuite tests="-1"><testcase name="a"/></testsuite>')
        def test_xpass_is_not_pass(self):
            value=pytest_case(xfail='original expected failure')
            self.assertEqual(value['cases'][0]['status'],'XPASS');self.assertFalse(formats.summary(value)['complete_success'])
        def test_xfail_is_explicit(self):
            value=pytest_case('skipped','original expected failure')
            self.assertEqual(value['cases'][0]['status'],'XFAIL');self.assertFalse(formats.summary(value)['complete_success'])
        def test_missing_pytest_phases(self):
            collection=encoded({'format_version':1,'kind':'actual-upstream-pytest-collection','nodes':[{'id':'x'}],'deselected':[]})
            value=formats.pytest_events(collection,b'')
            self.assertEqual(value['cases'][0]['status'],'UNEXECUTED_OR_INCOMPLETE')
        def test_pytest_deselected_stays_gap(self):
            value=pytest_case(deselected=['test_original.py::unrun'])
            self.assertFalse(formats.summary(value)['complete_success'])
        def test_ctest_cannot_drop_counter_mismatch(self):
            discovered={'universe':'ctest-registered-wrapper','cases':[{'id':'actual','disabled':False}]}
            result=formats.junit(b'<testsuite tests="2"><testcase name="actual"/></testsuite>')
            self.assertFalse(formats.summary(formats.reconcile_ctest(discovered,result))['complete_success'])
        def test_gtest_cannot_drop_counter_mismatch(self):
            discovered=formats.gtest(b'Original.\n  actual\n')
            result=formats.junit(b'<testsuite tests="2"><testcase classname="Original" name="actual"/></testsuite>')
            self.assertFalse(formats.summary(formats.reconcile_gtest(discovered,result,include_disabled=False))['complete_success'])
        def test_original_disabled_even_when_forced_is_explicit(self):
            discovered=formats.gtest(b'Original.\n  DISABLED_actual\n')
            result=formats.junit(b'<testsuite tests="1"><testcase classname="Original" name="DISABLED_actual"/></testsuite>')
            self.assertFalse(formats.summary(formats.reconcile_gtest(discovered,result,include_disabled=True))['complete_success'])
        def test_mapping_key_requires_closed_fields(self):
            with self.assertRaises(ValueError):differential.key({'suite':'a','universe':'gtest','id':'x','PASS':True})
        def test_distinct_universes_keep_same_display_name(self):
            rows=differential.discovered([{'id':'original','universes':[
                {'universe':'ctest-registered-wrapper','cases':[{'id':'same','status':'PASS'}]},
                {'universe':'gtest-listed-inner-case','cases':[{'id':'same','status':'PASS'}]}]}])
            self.assertEqual(len(rows),2);self.assertNotEqual(rows[0]['key'],rows[1]['key'])
        def test_duplicate_same_discovered_universe_refused(self):
            with self.assertRaises(ValueError):differential.discovered([{'id':'original','universes':[
                {'universe':'gtest-listed-inner-case','cases':[{'id':'same','status':'PASS'},{'id':'same','status':'PASS'}]}]}])
        def test_xml_entity_refused(self):
            with self.assertRaises(ValueError):formats.junit(b'<!DOCTYPE testsuite [<!ENTITY external SYSTEM "file:///etc/passwd">]><testsuite/>')
        def test_duplicate_actual_pytest_phase_refused(self):
            collection=encoded({'format_version':1,'kind':'actual-upstream-pytest-collection','nodes':[{'id':'x'}],'deselected':[]})
            event=encoded({'nodeid':'x','when':'call','outcome':'passed','wasxfail':''})
            with self.assertRaises(ValueError):formats.pytest_events(collection,event+b'\n'+event)
        def test_nested_aggregate_counter_cannot_hide_missing_cases(self):
            value=formats.junit(b'<testsuites tests="1"><testsuites tests="10"><testsuite tests="1"><testcase name="x"/></testsuite></testsuites></testsuites>')
            self.assertFalse(formats.summary(value)['complete_success'])
        def authority_argv(self,positive=True):
            values=['--ros2-global-disable','--ros2-single-create','main',
                '--ros2-single-enable' if positive else '--ros2-single-disable','main']
            if positive:values+=['--ros2-single-grant','main','state','topic.subscribe','/original','pkg/msg/State','identity']
            return values
        def test_actual_positive_global_default_stays_disabled(self):
            records=differential.authority(self.authority_argv(),2,'positive',[['state','topic.subscribe','/original','pkg/msg/State','identity']])
            self.assertEqual(records[0]['action'],'global-disable')
        def test_actual_default_denial_zero_grants(self):
            differential.authority(self.authority_argv(False),2,'default-denied',[])
        def test_extra_global_grant_cannot_hide_behind_main(self):
            args=self.authority_argv()+['--ros2-global-grant','any','topic.publish','*','*','*']
            with self.assertRaises(ValueError):differential.authority(args,2,'positive',[['state','topic.subscribe','/original','pkg/msg/State','identity']])
        def test_enabled_preload_principal_is_refused(self):
            args=self.authority_argv()+['--ros2-single-create','preload','--ros2-single-enable','preload']
            with self.assertRaises(ValueError):differential.authority(args,2,'positive',[['state','topic.subscribe','/original','pkg/msg/State','identity']])
        def test_duplicate_or_contradictory_main_toggle_is_refused(self):
            for flag in ('--ros2-single-enable','--ros2-single-disable'):
                with self.assertRaises(ValueError):differential.authority(self.authority_argv()+[flag,'main'],2,'positive',[['state','topic.subscribe','/original','pkg/msg/State','identity']])
        def test_other_backend_and_global_enable_are_refused(self):
            for suffix in (['--ros1-global-enable'],['--ros2-global-enable']):
                with self.assertRaises(ValueError):differential.authority(self.authority_argv()+suffix,2,'positive',[['state','topic.subscribe','/original','pkg/msg/State','identity']])
        def test_declared_grants_must_cover_every_actual_grant(self):
            extra=['--ros2-single-grant','main','command','topic.publish','/command','pkg/msg/Command','identity']
            with self.assertRaises(ValueError):differential.authority(self.authority_argv()+extra,2,'positive',[['state','topic.subscribe','/original','pkg/msg/State','identity']])
        def test_close_exception_cannot_mark_scope_closed(self):
            class FailedOwner:
                def close(self):raise OSError('DATA-only injected close failure')
            scope=differential.Scope.__new__(differential.Scope)
            scope.closed=False;scope.owner=FailedOwner();scope.report={};scope.save=lambda:None
            with self.assertRaises(OSError):scope.close()
            self.assertFalse(scope.closed);self.assertEqual(scope.report['cleanup']['status'],'CLEANUP_INCOMPLETE')
    return unittest.defaultTestLoader.loadTestsFromTestCase(Accounting)
