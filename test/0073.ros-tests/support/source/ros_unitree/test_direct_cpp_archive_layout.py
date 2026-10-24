"""Injected filesystem policies for the Direct explicit C++ archive pair.

Root supplies Direct and Legacy modules loaded from one complete current MF,
a live guard and an empty owned output directory. The dummy ordinary files
below exercise layout/input association only; they are not real archives,
compiler/linker results, Guest executions or official upstream test cases.
No candidate module is imported and no test runs when this file is loaded.
"""
import hashlib
from pathlib import Path
from types import SimpleNamespace
import unittest


def suite(direct, legacy, guard, out):
    """Return ten guarded policies for the production Direct association."""
    out=Path(out)

    def current(path):
        guard();path=Path(path);raw=path.read_bytes();s=path.stat(follow_symlinks=False)
        return {'path':str(path),'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),
                'device':s.st_dev,'inode':s.st_ino,'mtime_ns':s.st_mtime_ns,'ctime_ns':s.st_ctime_ns}

    class DirectCppArchives(unittest.TestCase):
        def setUp(self):
            guard();self.cell=out/self._testMethodName;self.cell.mkdir(mode=0o700)
            self.source=self.cell/'source';self.source.mkdir();self.sdk=self.cell/'sdk';self.sdk.mkdir()
            self.output=self.cell/'out';self.output.mkdir();self.observed=[];self.loads=[]
            self.frame=SimpleNamespace(source=self.source,out=self.output,roots=[self.sdk],
                frozen={'fixture':'identity only; the real owner supplies the complete MF'},inputs={})
            def pin(path):
                value=current(path);self.observed.append(value);return value
            def load(source,frozen,relative,name):
                self.loads.append((source,frozen,relative,name));guard();return legacy
            self.frame.pin=pin;self.api=SimpleNamespace(frozen_module=load)
        def pair(self,suffix='noeh',*,sdk=None):
            parent=(self.sdk if sdk is None else sdk)/'lib/wasm32-wasip1'
            if suffix:parent/=suffix
            parent.mkdir(parents=True,exist_ok=True);paths={}
            for key,name in (('libcxx','libc++.a'),('libcxxabi','libc++abi.a')):
                path=parent/name;path.write_bytes(b'explicit layout fixture only; no archive claim\n');paths[key]=path
            return paths
        def select(self,pair):
            self.labels={key:'selected-'+key for key in pair}
            self.frame.inputs={self.labels[key]:current(path) for key,path in pair.items()}
        def choose(self,labels=None):
            guard();return direct.selected_cpp_archives(self.frame,self.api,self.sdk,self.labels if labels is None else labels)
        def test_current_noeh_pair_retains_actual_full7_and_target(self):
            pair=self.pair();self.select(pair);actual=self.choose()
            self.assertEqual(actual['profile'],'noeh');self.assertEqual(actual['target'],'wasm32-wasip1')
            self.assertEqual(actual['archives'],{key:self.frame.inputs[label] for key,label in self.labels.items()})
            self.assertEqual(self.observed,list(actual['archives'].values()))
        def test_flat_and_eh_are_only_explicit_alternatives(self):
            for suffix in ('','eh'):
                self.select(self.pair(suffix));self.assertEqual(self.choose()['profile'],suffix or 'flat')
        def test_current_manifest_loader_is_the_only_policy_source(self):
            self.select(self.pair());self.choose();self.assertEqual(len(self.loads),1)
            source,frozen,relative,name=self.loads[0]
            self.assertEqual(source,self.frame.source);self.assertIs(frozen,self.frame.frozen)
            self.assertEqual(relative,'test/0073.ros-tests/support/source/ros_unitree/prepare_legacy_guests.py')
            self.assertEqual(name,'direct_explicit_cpp_archive_layout')
        def test_missing_selected_file_cannot_fall_back_to_another_profile(self):
            pair=self.pair();self.select(pair);self.pair('eh');pair['libcxx'].unlink()
            with self.assertRaises(FileNotFoundError):self.choose()
            self.assertEqual(self.observed,[])
        def test_mixed_pair_is_refused(self):
            pair=self.pair();other=self.pair('eh');self.select({'libcxx':pair['libcxx'],'libcxxabi':other['libcxxabi']})
            with self.assertRaisesRegex(ValueError,'same-profile'):self.choose()
        def test_other_sdk_pair_is_refused(self):
            other=self.cell/'other-sdk';other.mkdir();self.select(self.pair(sdk=other))
            with self.assertRaisesRegex(ValueError,'exact non-LTO nonthread WASIp1'):self.choose()
        def test_lto_thread_and_wasip2_pairs_are_refused(self):
            for relative in ('lib/wasm32-wasip1/noeh/llvm-lto/release',
                             'lib/wasm32-wasip1-threads/noeh','lib/wasm32-wasip2/noeh'):
                parent=self.sdk/relative;parent.mkdir(parents=True,exist_ok=True)
                pair={key:parent/name for key,name in (('libcxx','libc++.a'),('libcxxabi','libc++abi.a'))}
                for path in pair.values():path.write_bytes(b'not a qualified archive\n')
                self.select(pair)
                with self.assertRaisesRegex(ValueError,'exact non-LTO nonthread WASIp1'):self.choose()
        def test_changed_selected_full7_is_not_rebased(self):
            pair=self.pair();self.select(pair);pair['libcxx'].write_bytes(b'changed actual bytes\n')
            with self.assertRaisesRegex(ValueError,'selected whole-SDK full-seven'):self.choose()
        def test_unselected_whole_root_is_not_manufactured(self):
            self.select(self.pair());self.frame.roots=[]
            with self.assertRaisesRegex(ValueError,'complete source/tool/generated inventory'):self.choose()
        def test_closed_labels_and_physical_archive_spelling_required(self):
            pair=self.pair();self.select(pair)
            for labels in ({'libcxx':self.labels['libcxx']},
                           {**self.labels,'extra':'unused'},
                           {**self.labels,'libcxx':'unselected'}):
                with self.assertRaisesRegex(ValueError,'two explicit current'):self.choose(labels)
            alias=pair['libcxx'].with_name('alias.a');alias.symlink_to(pair['libcxx'])
            self.select({**pair,'libcxx':alias})
            with self.assertRaisesRegex(ValueError,'physical ordinary'):self.choose()

    return unittest.defaultTestLoader.loadTestsFromTestCase(DirectCppArchives)
