"""Guarded DATA regressions for A2's real original C52 contract.

These are not native compilation or execution tests. The Root-only caller
supplies genuinely pinned C52 raw data and its six actual GO documents, plus the
original export verbose log read through the selected current Frame. The same
current64 guard runs before/after every test. This module creates no actor and
executes no test on import. It has no fabricated successful build fixture.
"""
from __future__ import annotations
import copy
import unittest


def suite(native,*,original,original_pin,generation,catalog,profile,metadata,binding,export_verbose,guard):
    """Return18 tests; supplying data never starts a Runtime or compiler."""
    class Policy(unittest.TestCase):
        def setUp(self):
            guard();self.addCleanup(guard)
        def documents(self):
            return copy.deepcopy((generation,catalog,profile,metadata,binding))
        def accepted(self,rows):return native.message_contract(*rows)
        def refused(self,rows):
            with self.assertRaises((ValueError,KeyError)):self.accepted(rows)
        def test_genuine_original_terminal_and_all_three_families(self):
            row=native.terminal_original(original,original_pin)
            self.assertEqual(row['family'],'unitree_go')
        def test_native6_failure_is_never_promoted(self):
            body=copy.deepcopy(original);body['status']='FAIL'
            with self.assertRaises(ValueError):native.terminal_original(body,original_pin)
        def test_changed_original_raw_digest_is_refused(self):
            pin=dict(original_pin);pin['sha256']='0'*64
            with self.assertRaises(ValueError):native.terminal_original(original,pin)
        def test_incomplete_original_command_batch_is_refused(self):
            body=copy.deepcopy(original);body['commands'].pop()
            with self.assertRaises(ValueError):native.terminal_original(body,original_pin)
        def test_original_remaining_mount_is_refused(self):
            body=copy.deepcopy(original);body['remaining_mounts']=['/not-clean']
            with self.assertRaises(ValueError):native.terminal_original(body,original_pin)
        def test_original_incomplete_cleanup_is_refused(self):
            body=copy.deepcopy(original);body['cleanup']['remaining']=[{'pid':1}]
            with self.assertRaises(ValueError):native.terminal_original(body,original_pin)
        def test_actual_complete_go24_documents_are_accepted(self):
            self.assertEqual(len(self.accepted(self.documents())),24)
        def test_missing_complete_definition_is_refused(self):
            rows=self.documents();rows[2]['definitions'].pop();self.refused(rows)
        def test_native_root_identity_mismatch_is_refused(self):
            rows=self.documents();name=rows[1]['types'][0]['type'];rows[3]['types'][name]['native_identity']='RIHS01_'+'0'*64
            self.refused(rows)
        def test_component_identity_mismatch_is_refused(self):
            rows=self.documents();name=rows[1]['types'][0]['type']
            rows[3]['types'][name]['components']['message']['native_identity']='RIHS01_'+'0'*64;self.refused(rows)
        def test_codec_identity_mismatch_is_refused(self):
            rows=self.documents();rows[0]['records'][0]['adapter_codec_identity']='UWVMROS1_'+'0'*64;self.refused(rows)
        def test_bounds_mismatch_is_refused(self):
            rows=self.documents();rows[1]['types'][0]['bounds']={'made_up':{'sequence':1}};self.refused(rows)
        def test_missing_provider_output_is_refused(self):
            rows=self.documents();rows[0]['outputs'].pop('native_ros2.hpp');self.refused(rows)
        def test_extra_native_type_is_refused(self):
            rows=self.documents();rows[3]['types']['unrelated/msg/Other']={};self.refused(rows)
        def test_real_export_make_log_reconstructs_exact_actual_argv(self):
            family=next(row for row in original['families'] if row['family']=='unitree_go');section=family['export_build']
            database=[row['configured_compile_database_row'] for row in section['compile_records']]
            actual=native.make_witness(database,export_verbose,section['compile_records'][0]['actual_compile']['cwd'],
                section['actual_make_directory_events'])
            self.assertEqual(actual,[row['actual_compile'] for row in section['compile_records']])
        def test_missing_actual_compile_line_is_refused(self):
            family=next(row for row in original['families'] if row['family']=='unitree_go');section=family['export_build']
            record=section['compile_records'][0];changed=export_verbose.replace(record['actual_compile']['raw'],'',1)
            with self.assertRaises(ValueError):native.make_witness([record['configured_compile_database_row']],changed,
                record['actual_compile']['cwd'],section['actual_make_directory_events'])
        def test_duplicate_actual_compile_line_is_refused(self):
            family=next(row for row in original['families'] if row['family']=='unitree_go');section=family['export_build']
            record=section['compile_records'][0];raw=record['actual_compile']['raw'];changed=export_verbose.replace(raw,raw+'\n'+raw,1)
            with self.assertRaises(ValueError):native.make_witness([record['configured_compile_database_row']],changed,
                record['actual_compile']['cwd'],section['actual_make_directory_events'])
        def test_unmatched_make_directory_event_is_refused(self):
            family=next(row for row in original['families'] if row['family']=='unitree_go');section=family['export_build']
            record=section['compile_records'][0];events=copy.deepcopy(section['actual_make_directory_events']);events[0]['cwd']='/wrong'
            with self.assertRaises(ValueError):native.make_witness([record['configured_compile_database_row']],export_verbose,
                record['actual_compile']['cwd'],events)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policy)
