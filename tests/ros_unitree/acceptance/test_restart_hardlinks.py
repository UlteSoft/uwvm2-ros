#!/usr/bin/env python3
"""Real-filesystem synthetic tests for explicit retained hardlink admission.

Run this script only as an owned actor in the exact current 64-GiB/swap-zero
Linux cgroup. It creates all fixtures below /tmp, starts no child process and
runs no compiler, SDK, ROS node or Runtime. Tests use actual kernel hardlinks,
opened file hashes and the live current cgroup guard; they are not fabricated
inode/nlink observations. Importing this file does not load the target helper
or create fixtures. The author has not executed these tests locally.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import tempfile
import types
import unittest
from unittest import mock

sys.dont_write_bytecode = True
MIB = 1024**2
HELPER = None
GUARD = None
POLICY = 'historical-complete-intra-root'
LIMITS = {'file_bytes': MIB, 'catalog_bytes': MIB, 'catalog_entries': 128,
          'total_bytes': 8*MIB, 'seconds': 600}
CG = {'memory_max':64*1024**3,'memory_swap_max':0,'expected_device':29,'expected_inode':18315,
      'expected_boot_id':'ec0d5f0f-3060-4220-bee2-2a7791ed4e72',
      'expected_leaf':'/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
      'expected_cpus':'0-31'}


def bounded_bytes(path, maximum):
    """Bind small code/control bytes before loading the reviewed target helper."""
    path = Path(path)
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise ValueError('physical absolute test input required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode) or not 0 <= before.st_size <= maximum:
            raise ValueError('bounded regular test metadata required')
        raw = bytearray()
        while True:
            block = os.read(descriptor, min(MIB, maximum - len(raw) + 1))
            if not block:
                break
            raw.extend(block)
            if len(raw) > maximum:
                raise ValueError('test metadata grew beyond its bound')
        after = os.fstat(descriptor)
        named = path.stat(follow_symlinks=False)
        fields = ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')
        if len(raw) != before.st_size or any(getattr(before, key) != getattr(after, key) or
                getattr(before, key) != getattr(named, key) for key in fields):
            raise ValueError('opened test metadata changed')
        return bytes(raw)
    finally:
        os.close(descriptor)


class RetainedHardlinkPolicy(unittest.TestCase):
    def setUp(self):
        self.assertIsNotNone(HELPER, 'invoke this source test through its guarded CLI')
        self.assertIsNotNone(GUARD, 'actual original cgroup admission is required')
        GUARD()
        self.scratch = tempfile.TemporaryDirectory(prefix='uwvm-restart-hardlinks-', dir='/tmp')
        self.base = Path(self.scratch.name)
        self.root = self.base/'input'
        self.root.mkdir()
        self.serial = 0

    def tearDown(self):
        self.scratch.cleanup()
        GUARD()

    def pair(self, *, root=None, nested=False):
        root = root or self.root
        first = root/'first'
        first.write_bytes(b'complete actual hardlinked file bytes\n' * 19)
        second = root/'second'
        if nested:
            (root/'nested').mkdir()
            second = root/'nested'/'second'
        os.link(first, second)
        return first, second

    def catalog(self, *, root=None, alter=None):
        """Write the old common.py schema, which deliberately has no nlink."""
        root = root or self.root
        rows = []
        for directory, dirs, names in os.walk(root, followlinks=False):
            dirs.sort()
            for name in sorted(dirs + names):
                path = Path(directory)/name
                relative = str(path.relative_to(root))
                if path.is_symlink():
                    info = path.lstat()
                    rows.append({'relative':relative,'kind':'symlink','target':os.readlink(path),
                        'resolved':str(path.resolve(strict=False)),'device':info.st_dev,'inode':info.st_ino,
                        'mtime_ns':info.st_mtime_ns,'ctime_ns':info.st_ctime_ns})
                elif path.is_file():
                    pin = HELPER.opened_file(str(path), MIB, GUARD)
                    rows.append({'relative':relative,'kind':'file',
                        **{key:pin[key] for key in ('bytes','sha256','device','inode','mtime_ns','ctime_ns')},
                        'executable':bool(path.stat().st_mode & 0o111)})
        if alter:
            alter(rows)
        self.serial += 1
        path = self.base/('old-catalog-%02d.jsonl' % self.serial)
        path.write_text(''.join(json.dumps(row, sort_keys=True, separators=(',',':'))+'\n' for row in rows))
        return HELPER.opened_file(str(path), MIB, GUARD)

    def admit(self, old, *, policy=POLICY, root=None):
        self.serial += 1
        return HELPER.requalify_tree(old, str(root or self.root),
            self.base/('current-%02d.jsonl' % self.serial), GUARD, LIMITS, hardlink_policy=policy)

    def test_default_policy_still_rejects_real_hardlinks(self):
        self.pair()
        old = self.catalog()
        with self.assertRaisesRegex(ValueError, 'hardlink'):
            self.admit(old, policy='reject')

    def test_complete_nested_historical_class_is_admitted_and_preserved(self):
        first, _ = self.pair(nested=True)
        old = self.catalog()
        result = self.admit(old)
        self.assertEqual(result['hardlink_policy'], POLICY)
        self.assertEqual(len(result['hardlink_topology']), 1)
        group = result['hardlink_topology'][0]
        self.assertEqual(group['historical_members'], ['first', 'nested/second'])
        self.assertEqual(group['historical_nlink'], 'NOT_RECORDED')
        self.assertEqual(group['current_identity']['inode'], first.stat().st_ino)
        self.assertEqual(group['current_identity']['nlink'], 2)
        records = [json.loads(line) for line in Path(result['current_catalog']['path']).read_text().splitlines()]
        self.assertTrue(all(row['current_nlink'] == 2 for row in records if row['kind'] == 'file'))
        HELPER._preserve_tree(result, GUARD, LIMITS)

    def test_unlinked_files_keep_the_strict_default(self):
        (self.root/'single').write_bytes(b'ordinary independent file')
        old = self.catalog()
        result = self.admit(old, policy='reject')
        self.assertEqual(result['hardlink_topology'], [])
        records = [json.loads(line) for line in Path(result['current_catalog']['path']).read_text().splitlines()]
        self.assertNotIn('current_nlink', records[0])
        HELPER._preserve_tree(result, GUARD, LIMITS)

    def test_newly_merged_old_singletons_are_rejected(self):
        first = self.root/'first'
        second = self.root/'second'
        first.write_bytes(b'identical body')
        second.write_bytes(b'identical body')
        old = self.catalog()
        second.unlink()
        os.link(first, second)
        with self.assertRaisesRegex(ValueError, 'hardlink count'):
            self.admit(old)

    def test_broken_historical_class_is_rejected(self):
        first, second = self.pair()
        old = self.catalog()
        second.unlink()
        second.write_bytes(first.read_bytes())
        with self.assertRaisesRegex(ValueError, 'hardlink count'):
            self.admit(old)

    def test_unaccounted_external_current_alias_is_rejected(self):
        first, _ = self.pair()
        old = self.catalog()
        os.link(first, self.base/'external')
        with self.assertRaisesRegex(ValueError, 'hardlink count'):
            self.admit(old)

    def test_unrecorded_extra_in_root_alias_is_rejected(self):
        first, _ = self.pair()
        old = self.catalog()
        os.link(first, self.root/'third')
        with self.assertRaises(ValueError):
            self.admit(old)

    def test_inconsistent_historical_alias_metadata_is_rejected(self):
        self.pair()
        for field, replacement in (('sha256','0'*64),('bytes',1),('executable',True),('mtime_ns',1),('ctime_ns',1)):
            with self.subTest(field=field):
                def alter(rows):
                    rows[-1][field] = replacement
                old = self.catalog(alter=alter)
                with self.assertRaisesRegex(ValueError, 'historical aliases'):
                    self.admit(old)

    def test_new_external_alias_fails_terminal_preservation(self):
        first, _ = self.pair()
        result = self.admit(self.catalog())
        os.link(first, self.base/'late-external')
        with self.assertRaisesRegex(ValueError, 'hardlink count'):
            HELPER._preserve_tree(result, GUARD, LIMITS)

    def test_replaced_member_fails_terminal_preservation(self):
        first, second = self.pair()
        result = self.admit(self.catalog())
        data = first.read_bytes()
        second.unlink()
        second.write_bytes(data)
        with self.assertRaises(ValueError):
            HELPER._preserve_tree(result, GUARD, LIMITS)

    def test_alias_added_after_an_open_cannot_escape_the_first_walk(self):
        first, _ = self.pair()
        old = self.catalog()
        original = HELPER._opened_at
        added = False
        def opened(*args, **kwargs):
            nonlocal added
            value = original(*args, **kwargs)
            if args[2] == str(first) and not added:
                os.link(first, self.base/'during-walk-external')
                added = True
            return value
        with mock.patch.object(HELPER, '_opened_at', side_effect=opened):
            with self.assertRaises(ValueError):
                self.admit(old)
        self.assertTrue(added)

    def test_symlink_targets_are_never_consumed_as_current_bytes(self):
        self.pair()
        outside = self.base/'outside-target'
        outside.write_bytes(b'not part of the selected tree')
        os.symlink('../outside-target', self.root/'alias')
        old = self.catalog()
        original = HELPER._opened_at
        def opened(*args, **kwargs):
            self.assertNotEqual(args[2], str(outside), 'target bytes were followed')
            return original(*args, **kwargs)
        with mock.patch.object(HELPER, '_opened_at', side_effect=opened):
            result = self.admit(old)
            HELPER._preserve_tree(result, GUARD, LIMITS)
        self.assertEqual(result['symlink_target_qualification'], 'NOT_PERFORMED')

    def test_content_bridge_records_a_new_current_inode_without_fabrication(self):
        first, _ = self.pair()
        old_inode = first.stat().st_ino
        old = self.catalog()
        current = self.base/'separate-current-root'
        current.mkdir()
        self.pair(root=current)
        result = self.admit(old, root=current)
        group = result['hardlink_topology'][0]
        self.assertEqual(group['historical_inode'], old_inode)
        self.assertNotEqual(group['current_identity']['inode'], old_inode)
        self.assertEqual(group['current_identity']['inode'], (current/'first').stat().st_ino)
        HELPER._preserve_tree(result, GUARD, LIMITS)

    def test_old_external_link_count_is_not_invented(self):
        first, _ = self.pair()
        outside = self.base/'old-external'
        os.link(first, outside)
        old = self.catalog()
        outside.unlink()
        result = self.admit(old)
        self.assertEqual(result['hardlink_topology'][0]['historical_nlink'], 'NOT_RECORDED')
        self.assertEqual(result['hardlink_topology'][0]['current_identity']['nlink'], 2)

    def test_unknown_or_implicit_policy_values_are_rejected(self):
        (self.root/'single').write_bytes(b'body')
        old = self.catalog()
        for policy in (True, False, None, '', 'allow', 'all'):
            with self.subTest(policy=policy):
                with self.assertRaisesRegex(ValueError, 'hardlink policy'):
                    self.admit(old, policy=policy)


def main():
    global HELPER, GUARD
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--helper', required=True, type=Path)
    parser.add_argument('--helper-sha256', required=True)
    parser.add_argument('--source', type=Path,
        default=Path('/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r39/source'))
    parser.add_argument('--manifest', type=Path,
        default=Path('/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r39/source-manifest.json'))
    parser.add_argument('--out', required=True, type=Path)
    args = parser.parse_args()
    if sys.platform != 'linux':
        raise ValueError('actual guarded Linux tests required; local authoring performs AST checks only')
    helper_path = args.helper.resolve(strict=True)
    raw = bounded_bytes(helper_path, MIB)
    if hashlib.sha256(raw).hexdigest() != args.helper_sha256:
        raise ValueError('explicit reviewed helper bytes required')
    HELPER = types.ModuleType('uwvm_hardlink_policy_test_target')
    HELPER.__file__ = str(helper_path)
    exec(compile(raw, str(helper_path), 'exec'), HELPER.__dict__)
    manifest_raw = bounded_bytes(args.manifest.resolve(strict=True), 16*MIB)
    if hashlib.sha256(manifest_raw).hexdigest() != '54d6c79978e36bd13fa37272d8dd5f2cc94a9d2e62ac437b51dfc4aa4f6ae5b9':
        raise ValueError('actual complete Source39 probe binding required')
    GUARD = HELPER.CurrentGuard(str(args.source.resolve(strict=True)), json.loads(manifest_raw), CG)
    GUARD.maximum_seconds = 600
    before = GUARD.force()
    helper_pin = HELPER.opened_file(str(helper_path), MIB, GUARD)
    if helper_pin['sha256'] != args.helper_sha256:
        raise ValueError('target helper changed after loading')
    output = args.out
    protected = [helper_path, args.source.resolve(strict=True), args.manifest.resolve(strict=True)]
    if not output.is_absolute() or output.parent.resolve(strict=True) != output.parent or output.exists() or any(
            output == path or output in path.parents or path in output.parents for path in protected):
        raise ValueError('fresh absolute test receipt output required')
    output.mkdir(mode=0o700)
    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(RetainedHardlinkPolicy))
    after = GUARD.force()
    if HELPER.opened_file(str(helper_path), MIB, GUARD) != helper_pin:
        raise ValueError('target helper changed during tests')
    report = {'format_version':1,'kind':'restart-hardlink-policy-synthetic-filesystem-tests',
        'status':'PASS' if result.wasSuccessful() and result.testsRun == 15 and not result.skipped else 'FAIL',
        'tests_run':result.testsRun,'failure_count':len(result.failures),'error_count':len(result.errors),
        'skipped_count':len(result.skipped),'helper_current_pin':helper_pin,
        'scratch_root':'/tmp','cgroup_before':before,'cgroup_after':after,
        'scope':'ACTUAL_SYNTHETIC_KERNEL_HARDLINK_POLICY_ONLY_NO_SDK_RUNTIME_OR_ROS_QUALIFICATION',
        'commands':[]}
    HELPER._write_report(output, report)
    print(json.dumps({'status':report['status'],'tests_run':report['tests_run'],'report':str(output/'report.json')}))
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
