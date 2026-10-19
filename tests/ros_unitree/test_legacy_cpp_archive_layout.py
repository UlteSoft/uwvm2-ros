#!/usr/bin/env python3
"""Six finite filesystem policy cases for explicit WASI C++ archive layouts.

This module starts no test on import. Root supplies its current-MF-loaded
producer, live original64GiB guard and a fresh owned temporary directory to
suite(producer, guard, out). Dummy files below are layout policy fixtures only:
they do not certify a real SDK, archive target, compiler or Wasm product. The
actual producer separately requires selected whole SDK inputs/full-seven pins,
actual WASIp1 compile/link targets, both real trace entries and full validation.
"""
from pathlib import Path
import unittest


def suite(producer, guard, out):
    """Return six guarded policy tests; do not run or import project modules."""
    out=Path(out)

    class ExplicitCppArchiveLayout(unittest.TestCase):
        def setUp(self):
            guard();self.root=out/self._testMethodName
            self.root.mkdir(mode=0o700);self.sdk=self.root/'sdk';self.sdk.mkdir()
        def pair(self,relative='lib/wasm32-wasip1/noeh',*,sdk=None):
            base=(self.sdk if sdk is None else sdk)/relative;base.mkdir(parents=True,exist_ok=True)
            result={}
            for key,name in (('libcxx','libc++.a'),('libcxxabi','libc++abi.a')):
                path=base/name;path.write_bytes(b'layout policy fixture only; not an archive\n');result[key]=path
            return result
        def choose(self,pair):
            guard();return producer.wasi_cpp_archive_profile(self.sdk,pair)
        def test_each_explicit_same_profile_pair(self):
            for suffix,expected in (('', 'flat'),('/noeh','noeh'),('/eh','eh')):
                self.assertEqual(self.choose(self.pair('lib/wasm32-wasip1'+suffix)),expected)
        def test_mixed_profiles_never_choose_a_fallback(self):
            noeh,eh=self.pair(),self.pair('lib/wasm32-wasip1/eh')
            with self.assertRaisesRegex(ValueError,'same-profile'):
                self.choose({'libcxx':noeh['libcxx'],'libcxxabi':eh['libcxxabi']})
        def test_lto_thread_and_other_wasi_profiles_refused(self):
            for relative in ('lib/wasm32-wasip1/noeh/llvm-lto/22.1.0-wasi-sdk',
                             'lib/wasm32-wasip1-threads/noeh','lib/wasm32-wasip2/noeh'):
                with self.assertRaisesRegex(ValueError,'exact non-LTO nonthread WASIp1'):
                    self.choose(self.pair(relative))
        def test_other_sdk_pair_is_not_current_sdk_authority(self):
            other=self.root/'other-sdk';other.mkdir()
            with self.assertRaisesRegex(ValueError,'exact non-LTO nonthread WASIp1'):
                self.choose(self.pair(sdk=other))
        def test_missing_wrong_named_or_incomplete_pair_refused(self):
            pair=self.pair();pair['libcxx'].unlink()
            with self.assertRaises(FileNotFoundError):self.choose(pair)
            pair['libcxx'].write_bytes(b'layout policy fixture only\n')
            wrong=pair['libcxx'].parent/'alternate.a';wrong.write_bytes(b'not an admitted role\n')
            with self.assertRaisesRegex(ValueError,'same-profile'):
                self.choose({'libcxx':wrong,'libcxxabi':pair['libcxxabi']})
            with self.assertRaisesRegex(ValueError,'two explicit'):
                self.choose({'libcxx':pair['libcxx']})
        def test_alias_does_not_manufacture_a_supported_layout(self):
            pair=self.pair();alias=self.sdk/'lib/wasm32-wasip1/libc++.a';alias.symlink_to(pair['libcxx'])
            with self.assertRaisesRegex(ValueError,'physical ordinary'):
                self.choose({'libcxx':alias,'libcxxabi':pair['libcxxabi']})
            alias.unlink()
            parent=self.sdk/'lib/wasm32-wasip1/fake';parent.symlink_to(pair['libcxx'].parent,target_is_directory=True)
            with self.assertRaisesRegex(ValueError,'physical ordinary'):
                self.choose({'libcxx':parent/'libc++.a','libcxxabi':parent/'libc++abi.a'})

    return unittest.defaultTestLoader.loadTestsFromTestCase(ExplicitCppArchiveLayout)
