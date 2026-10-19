#!/usr/bin/env python3
"""Guarded Linux kernel tests for finite cross-root/auxiliary hardlink proof.

All fixtures use genuine os.link/stat/opened bytes below /tmp. No compiler,
SDK, ROS endpoint, Runtime, mount or child process runs. The only success claim
is this small filesystem policy. Invoke the CLI in Root's actual selected
64GiB/swap0 cgroup; importing this source neither loads a candidate nor tests it.
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


BASE = UNION = GUARD = None
MIB = 1024**2
LIMITS = {'file_bytes': MIB, 'catalog_bytes': MIB, 'catalog_entries': 256,
          'total_bytes': 8 * MIB, 'seconds': 600}
sys.dont_write_bytecode = True


def require(value, reason):
    if not value:
        raise ValueError(reason)


def bounded(path, maximum):
    """Stable source/control bytes before any candidate code is loaded."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute test metadata required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= maximum, 'bounded ordinary test metadata required')
        body = bytearray()
        while True:
            block = os.read(fd, min(65536, maximum + 1 - len(body)))
            if not block:
                break
            body.extend(block)
            require(len(body) <= maximum, 'test metadata grew')
        after, named = os.fstat(fd), path.stat(follow_symlinks=False)
        require(len(body) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode', 'st_nlink')),
                'test metadata physical identity changed')
        return bytes(body)
    finally:
        os.close(fd)


class FiniteUnionKernelPolicy(unittest.TestCase):
    def setUp(self):
        self.assertIsNotNone(BASE, 'invoke the guarded CLI')
        self.assertIsNotNone(UNION, 'explicit source-bound union helper required')
        self.assertIsNotNone(GUARD, 'actual selected current Linux cgroup required')
        GUARD()
        self.scratch = tempfile.TemporaryDirectory(prefix='uwvm-finite-union-', dir='/tmp')
        self.base = Path(self.scratch.name)
        self.a, self.b, self.aux, self.proofs = [self.base / name for name in ('a', 'b', 'aux', 'proofs')]
        for path in (self.a, self.b, self.aux, self.proofs):
            path.mkdir()
        self.serial = 0

    def tearDown(self):
        self.scratch.cleanup()
        GUARD()

    def pair(self):
        first, second = self.a / 'first', self.b / 'second'
        first.write_bytes(b'actual kernel hardlink bytes\n' * 13)
        os.link(first, second)
        return first, second

    def catalog(self, root, alter=None):
        """Original common catalog schema: no historical nlink/full mode."""
        rows = []
        for directory, dirs, names in os.walk(root, followlinks=False):
            dirs.sort()
            for name in sorted(dirs + names):
                path = Path(directory) / name
                relative = str(path.relative_to(root))
                if path.is_symlink():
                    info = path.lstat()
                    rows.append({'relative': relative, 'kind': 'symlink', 'target': os.readlink(path),
                        'resolved': str(path.resolve(strict=False)), 'device': info.st_dev, 'inode': info.st_ino,
                        'mtime_ns': info.st_mtime_ns, 'ctime_ns': info.st_ctime_ns})
                elif path.is_file():
                    pin = BASE.opened_file(str(path), MIB, GUARD)
                    rows.append({'relative': relative, 'kind': 'file',
                        **{key: pin[key] for key in ('bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns')},
                        'executable': bool(path.stat().st_mode & 0o111)})
        if alter is not None:
            alter(rows)
        self.serial += 1
        output = self.base / ('old-%02d.jsonl' % self.serial)
        output.write_text(''.join(json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n' for row in rows))
        return BASE.opened_file(str(output), MIB, GUARD)

    def union(self, selected, auxiliary=None):
        device = self.a.stat().st_dev
        return UNION.HistoricalUnion(BASE,
            [{'root': str(root), 'catalog': pin} for root, pin in selected], GUARD, LIMITS,
            device_transitions={device: device}, auxiliary_roots=auxiliary)

    def admit(self, union, selected):
        for root, old in selected:
            self.serial += 1
            union.requalify_tree(old, str(root), self.proofs / ('current-%02d.jsonl' % self.serial))
        return union.finish()

    def auxiliary(self, first):
        info = first.stat()
        return [{'root': str(self.aux), 'historical_anchors': [{'device': info.st_dev, 'inode': info.st_ino}]}]

    def test_real_cross_root_class_qualifies_and_preserves(self):
        first, second = self.pair()
        selected = [(self.a, self.catalog(self.a)), (self.b, self.catalog(self.b))]
        union = self.union(selected)
        result = self.admit(union, selected)
        row = result['current_classes'][0]
        self.assertEqual(row['historical_members'], sorted((str(first), str(second))))
        self.assertEqual(row['current_complete_union_member_count'], 2)
        self.assertEqual(row['current_identity']['nlink'], 2)
        self.assertEqual(row['historical_nlink'], 'NOT_RECORDED')
        self.assertEqual(union.preserve(), result)

    def test_unselected_second_root_is_an_external_alias(self):
        self.pair()
        selected = [(self.a, self.catalog(self.a))]
        with self.assertRaises(ValueError):
            self.admit(self.union(selected), selected)

    def test_missing_registered_root_cannot_finish(self):
        self.pair()
        selected = [(self.a, self.catalog(self.a)), (self.b, self.catalog(self.b))]
        union = self.union(selected)
        union.requalify_tree(selected[0][1], str(self.a), self.proofs / 'first.jsonl')
        with self.assertRaisesRegex(ValueError, 'all registered'):
            union.finish()

    def test_overlapping_old_roots_count_physical_names_once(self):
        nested = self.a / 'nested'
        nested.mkdir()
        first = self.a / 'first'
        first.write_bytes(b'genuine overlap')
        os.link(first, nested / 'second')
        selected = [(self.a, self.catalog(self.a)), (nested, self.catalog(nested))]
        union = self.union(selected)
        row = self.admit(union, selected)['current_classes'][0]
        self.assertEqual(row['current_complete_union_member_count'], 2)
        union.preserve()

    def test_unknown_external_third_alias_is_refused(self):
        first, _ = self.pair()
        os.link(first, self.base / 'outside')
        selected = [(self.a, self.catalog(self.a)), (self.b, self.catalog(self.b))]
        with self.assertRaises(ValueError):
            self.admit(self.union(selected), selected)

    def test_changed_source_bytes_or_ctime_are_refused(self):
        first, _ = self.pair()
        selected = [(self.a, self.catalog(self.a)), (self.b, self.catalog(self.b))]
        first.write_bytes(b'changed actual payload')
        with self.assertRaises(ValueError):
            self.admit(self.union(selected), selected)

    def test_mutated_selected_old_catalog_fails_before_walk(self):
        self.pair()
        selected = [(self.a, self.catalog(self.a)), (self.b, self.catalog(self.b))]
        Path(selected[0][1]['path']).write_bytes(b'{}\n')
        with self.assertRaises(ValueError):
            self.union(selected)

    def test_conflicting_same_inode_old_metadata_is_refused(self):
        self.pair()
        old_a = self.catalog(self.a)
        old_b = self.catalog(self.b, lambda rows: rows[0].update(sha256='0' * 64))
        with self.assertRaises(ValueError):
            self.union([(self.a, old_a), (self.b, old_b)])

    def test_split_old_class_is_refused(self):
        first, second = self.pair()
        selected = [(self.a, self.catalog(self.a)), (self.b, self.catalog(self.b))]
        data = first.read_bytes()
        second.unlink()
        second.write_bytes(data)
        with self.assertRaises(ValueError):
            self.admit(self.union(selected), selected)

    def test_new_merge_of_old_independent_inodes_is_refused(self):
        first, second = self.a / 'first', self.b / 'second'
        first.write_bytes(b'identical bytes')
        second.write_bytes(b'identical bytes')
        selected = [(self.a, self.catalog(self.a)), (self.b, self.catalog(self.b))]
        second.unlink()
        os.link(first, second)
        with self.assertRaises(ValueError):
            self.admit(self.union(selected), selected)

    def test_new_external_alias_after_finish_breaks_preservation(self):
        first, _ = self.pair()
        selected = [(self.a, self.catalog(self.a)), (self.b, self.catalog(self.b))]
        union = self.union(selected)
        self.admit(union, selected)
        os.link(first, self.base / 'later')
        with self.assertRaises(ValueError):
            union.preserve()

    def test_auxiliary_current_names_are_separate_from_history(self):
        first = self.a / 'old-anchor'
        first.write_bytes(b'actual anchored bytes')
        extra = self.aux / 'current-extra'
        os.link(first, extra)
        (self.aux / 'ordinary').write_bytes(b'current-only ordinary bytes')
        (self.a / 'link-is-not-an-alias').symlink_to(extra)
        selected = [(self.a, self.catalog(self.a))]
        union = self.union(selected, self.auxiliary(first))
        roots = union.qualify_auxiliary(self.proofs)
        self.assertEqual(roots[0]['historical_auxiliary_names'], 'NOT_RECORDED')
        row = self.admit(union, selected)['current_classes'][0]
        self.assertEqual(row['historical_members'], [str(first)])
        self.assertEqual(row['current_auxiliary_members'], [str(extra)])
        self.assertEqual(row['historical_auxiliary_names'], 'NOT_RECORDED')
        self.assertEqual(row['current_complete_union_member_count'], 2)
        union.preserve()

    def test_auxiliary_does_not_allow_an_outside_third_alias(self):
        first = self.a / 'old-anchor'
        first.write_bytes(b'anchored')
        os.link(first, self.aux / 'extra')
        os.link(first, self.base / 'outside')
        selected = [(self.a, self.catalog(self.a))]
        union = self.union(selected, self.auxiliary(first))
        with self.assertRaises(ValueError):
            union.qualify_auxiliary(self.proofs)

    def test_auxiliary_must_be_explicitly_selected(self):
        first = self.a / 'old-anchor'
        first.write_bytes(b'anchored')
        os.link(first, self.aux / 'extra')
        selected = [(self.a, self.catalog(self.a))]
        with self.assertRaises(ValueError):
            self.admit(self.union(selected), selected)

    def test_auxiliary_wrong_historical_anchor_is_refused(self):
        first = self.a / 'old-anchor'
        first.write_bytes(b'anchored')
        os.link(first, self.aux / 'extra')
        other = self.base / 'unrelated'
        other.write_bytes(b'not selected by any old catalog')
        selected = [(self.a, self.catalog(self.a))]
        with self.assertRaises(ValueError):
            self.union(selected, self.auxiliary(other))

    def test_unanchored_auxiliary_multi_name_file_is_refused(self):
        first = self.a / 'old-anchor'
        first.write_bytes(b'anchored')
        os.link(first, self.aux / 'extra')
        unrelated = self.aux / 'unknown-first'
        unrelated.write_bytes(b'unanchored class')
        os.link(unrelated, self.aux / 'unknown-second')
        selected = [(self.a, self.catalog(self.a))]
        union = self.union(selected, self.auxiliary(first))
        with self.assertRaises(ValueError):
            union.qualify_auxiliary(self.proofs)

    def test_auxiliary_mutation_breaks_terminal_preservation(self):
        first = self.a / 'old-anchor'
        first.write_bytes(b'anchored')
        os.link(first, self.aux / 'extra')
        ordinary = self.aux / 'ordinary'
        ordinary.write_bytes(b'before')
        selected = [(self.a, self.catalog(self.a))]
        union = self.union(selected, self.auxiliary(first))
        union.qualify_auxiliary(self.proofs)
        self.admit(union, selected)
        ordinary.write_bytes(b'after')
        with self.assertRaises(ValueError):
            union.preserve()

    def test_auxiliary_discovery_is_required_before_historical_walk(self):
        first = self.a / 'old-anchor'
        first.write_bytes(b'anchored')
        os.link(first, self.aux / 'extra')
        selected = [(self.a, self.catalog(self.a))]
        union = self.union(selected, self.auxiliary(first))
        with self.assertRaises(ValueError):
            union.requalify_tree(selected[0][1], str(self.a), self.proofs / 'premature.jsonl')


def main():
    global BASE, UNION, GUARD
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('base-helper', 'union-helper', 'source', 'manifest', 'current-cgroup', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    for name in ('base-helper-sha256', 'union-helper-sha256', 'manifest-sha256', 'current-cgroup-sha256'):
        parser.add_argument('--' + name, required=True)
    args = parser.parse_args()
    require(sys.platform == 'linux', 'actual guarded Linux execution required')
    manifest = bounded(args.manifest, 16 * MIB)
    require(hashlib.sha256(manifest).hexdigest() == args.manifest_sha256, 'explicit complete source manifest digest differs')
    frozen = json.loads(manifest)
    this = Path(__file__).resolve()
    source = args.source.resolve(strict=True)
    this_body = bounded(this, MIB)
    require(this == source / 'tests/ros_unitree/acceptance/test_restart_hardlink_union.py' and
            frozen.get(str(this.relative_to(source))) == {'bytes': len(this_body), 'sha256': hashlib.sha256(this_body).hexdigest()},
            'actual reviewed test entry must belong to the selected complete source')
    modules = []
    for name, path, digest in (('union_test_base', args.base_helper, args.base_helper_sha256),
                               ('union_test_candidate', args.union_helper, args.union_helper_sha256)):
        body = bounded(path, MIB)
        require(hashlib.sha256(body).hexdigest() == digest, 'explicit reviewed helper digest differs')
        module = types.ModuleType(name)
        module.__file__ = str(path)
        sys.modules[name] = module
        exec(compile(body, str(path), 'exec'), module.__dict__)
        modules.append(module)
    BASE, UNION = modules
    context = bounded(args.current_cgroup, MIB)
    require(hashlib.sha256(context).hexdigest() == args.current_cgroup_sha256, 'explicit current cgroup selection digest differs')
    GUARD = BASE.CurrentGuard(str(source), frozen, json.loads(context))
    GUARD.maximum_seconds = 600
    before = GUARD.force()
    inputs = [BASE.opened_file(str(path), maximum, GUARD) for path, maximum in
        ((this, MIB), (args.base_helper, MIB), (args.union_helper, MIB), (args.current_cgroup, MIB), (args.manifest, 16 * MIB))]
    output = args.out
    require(output.is_absolute() and output.resolve() == output and not output.exists() and not output.is_symlink() and
            all(output != Path(pin['path']) and output not in Path(pin['path']).parents and
                Path(pin['path']) not in output.parents for pin in inputs) and
            not output.is_relative_to(source) and not source.is_relative_to(output), 'fresh disjoint test receipt output required')
    output.mkdir(mode=0o700, parents=True, exist_ok=False)
    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(FiniteUnionKernelPolicy))
    after = GUARD.force()
    require(all(BASE.opened_file(pin['path'], pin['bytes'], GUARD) == pin for pin in inputs), 'test/source/context bytes changed during actual tests')
    report = {'format_version': 1, 'kind': 'finite-hardlink-union-kernel-filesystem-tests',
        'status': 'PASS' if result.wasSuccessful() and result.testsRun == 18 and not result.skipped else 'FAIL',
        'tests_run': result.testsRun, 'failures': len(result.failures), 'errors': len(result.errors), 'skipped': len(result.skipped),
        'input_pins': inputs, 'cgroup_before': before, 'cgroup_after': after, 'scratch_root': '/tmp', 'commands': [],
        'scope': 'ACTUAL_SMALL_KERNEL_HARDLINK_POLICY_ONLY_NO_SDK_COMPILER_RUNTIME_ROS_OR_RESTART_EXECUTION_QUALIFICATION'}
    BASE._write_report(output, report)
    print(json.dumps({'status': report['status'], 'tests_run': result.testsRun, 'report': str(output / 'report.json')}))
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
