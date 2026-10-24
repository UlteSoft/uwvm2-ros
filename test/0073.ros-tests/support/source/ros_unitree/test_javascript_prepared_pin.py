"""Twelve injected filesystem policies for the genuine JS closed8 bridge.

Root supplies a complete-current-MF-loaded producer, live guard and owned out
to suite(producer, guard, out). No helper, test or process starts on import.
These tiny ordinary fixtures exercise schema/descriptor association; they do
not certify a real engine, compiler, SDK, Guest or successful Legacy build.
"""
import hashlib
import os
from pathlib import Path
import shutil
import stat
from types import SimpleNamespace
import unittest


def suite(producer, guard, out):
    out=Path(out)

    def pin(path):
        # Fixture-only metadata builder. The production helper independently
        # streams and admits the file; this ordinary tiny read is not evidence
        # of a real SDK or an alternative implementation of its reader.
        info=path.stat(follow_symlinks=False);body=path.read_bytes()
        return {'path':str(path),'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest(),
                'device':info.st_dev,'inode':info.st_ino,'mtime_ns':info.st_mtime_ns,
                'ctime_ns':info.st_ctime_ns,'mode':info.st_mode}

    class JavascriptPreparedPin(unittest.TestCase):
        def setUp(self):
            guard();self.cell=out/self._testMethodName;self.cell.mkdir(mode=0o700)
            self.source=self.cell/'source';self.source.mkdir();self.output=self.cell/'output';self.output.mkdir()
            self.input=self.cell/'input';self.input.mkdir();self.file=self.source/'fixture.c'
            self.file.write_bytes(b'/* data-only source identity fixture */\n');self.file.chmod(0o400)
            self.original=pin(self.file);self.observed=[]
            def current(path):
                guard();value=pin(Path(path));value.pop('mode');self.observed.append(value);return value
            self.frame=SimpleNamespace(source=self.source,out=self.output,roots=[self.input],guard=guard,pin=current)
        def tearDown(self):
            # Every test owns only its fresh leaf. No Root temporary root or
            # actual source/tool/catalog input is removed by this policy suite.
            shutil.rmtree(self.cell);guard()
        def qualify(self,value=None,**kwargs):
            return producer.javascript_prepared_file(self.frame,self.original if value is None else value,**kwargs)
        def test_closed8_and_actual_full7_remain_distinct(self):
            result=self.qualify()
            self.assertEqual(result['original_sdk_pin'],self.original)
            self.assertEqual(result['descriptor_mode'],self.original['mode'])
            self.assertEqual(set(result['current_file']),set(self.original)-{'mode'})
            self.assertEqual(self.observed,[result['current_file']])
            self.assertNotEqual(result['original_sdk_pin'],result['current_file'])
        def test_source_output_and_explicit_input_roots_admitted(self):
            for root in (self.output,self.input):
                path=root/'fixture.c';path.write_bytes(b'physical association, not compiler evidence\n')
                self.assertEqual(self.qualify(pin(path))['current_file']['path'],str(path))
        def test_unknown_metadata_field_refused(self):
            with self.assertRaisesRegex(ValueError,'closed8'):self.qualify({**self.original,'unexpected':False})
        def test_seven_fields_are_not_a_preparer_observation(self):
            value=dict(self.original);value.pop('mode')
            with self.assertRaisesRegex(ValueError,'closed8'):self.qualify(value)
        def test_bool_and_nonregular_mode_refused(self):
            for mode in (True,stat.S_IFDIR|0o400):
                with self.assertRaisesRegex(ValueError,'raw mode'):self.qualify({**self.original,'mode':mode})
        def test_raw_permissions_are_not_silently_dropped(self):
            with self.assertRaisesRegex(ValueError,'raw mode'):
                self.qualify({**self.original,'mode':self.original['mode']^0o100})
        def test_hash_and_identity_mismatch_refused(self):
            for value in ({**self.original,'sha256':'0'*64},{**self.original,'inode':self.original['inode']+1}):
                with self.assertRaisesRegex(ValueError,'full7'):self.qualify(value)
        def test_unselected_scope_refused_before_frame_observation(self):
            path=self.cell/'outside.c';path.write_bytes(b'not an admitted input\n')
            with self.assertRaisesRegex(ValueError,'selected Frame scope'):self.qualify(pin(path))
            self.assertEqual(self.observed,[])
        def test_symlink_is_not_an_ordinary_selected_name(self):
            path=self.source/'alias.c';path.symlink_to(self.file)
            with self.assertRaisesRegex(ValueError,'physical'):
                self.qualify({**self.original,'path':str(path)})
        def test_size_bound_is_checked_before_streaming(self):
            with self.assertRaisesRegex(ValueError,'bounded JavaScript'):self.qualify(maximum=1)
            self.assertEqual(self.observed,[])
        def test_guard_mutation_during_read_refused(self):
            calls=0
            def mutating_guard():
                nonlocal calls
                guard();calls+=1
                if calls==2:self.file.chmod(0o600)
            self.frame.guard=mutating_guard
            with self.assertRaisesRegex(ValueError,'full7|Frame file|changed'):self.qualify()
        def test_mutation_during_frame_association_refused(self):
            current=self.frame.pin
            def changing_current(path):
                value=current(path);self.file.chmod(0o600);return value
            self.frame.pin=changing_current
            with self.assertRaisesRegex(ValueError,'changed'):self.qualify()

    return unittest.defaultTestLoader.loadTestsFromTestCase(JavascriptPreparedPin)
