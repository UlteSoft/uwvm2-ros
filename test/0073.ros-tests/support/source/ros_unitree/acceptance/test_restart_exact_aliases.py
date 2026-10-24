#!/usr/bin/env python3
"""Guarded actual-kernel regressions for finite exact current file aliases.

The six tests use only real regular files, os.link, descriptor reads and tiny
metadata catalogs below /tmp. They launch no compiler, target or subprocess.
Invoke the explicit CLI inside Root's actual selected64GiB/swap0 Linux cgroup;
this source performs no candidate import or test execution merely by import.
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


BASE = UNION = EXACT = GUARD = None
MIB = 1024**2
LIMITS = {'file_bytes': MIB, 'catalog_bytes': MIB, 'catalog_entries': 256,
          'total_bytes': 8 * MIB, 'seconds': 600}
sys.dont_write_bytecode = True


def require(value, reason):
    if not value:
        raise ValueError(reason)


def bounded(path, maximum):
    """Read size-first, stable ordinary bootstrap bytes before candidate code."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute test metadata required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(descriptor)
        require(stat.S_ISREG(first.st_mode) and 0 < first.st_size <= maximum, 'bounded ordinary test metadata required')
        body = bytearray()
        while True:
            block = os.read(descriptor, min(65536, maximum + 1 - len(body)))
            if not block:
                break
            body.extend(block)
            require(len(body) <= maximum, 'test metadata grew')
        last, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        require(len(body) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode', 'st_nlink')),
                'test metadata physical identity changed')
        return bytes(body)
    finally:
        os.close(descriptor)


class ExactAliasKernelPolicy(unittest.TestCase):
    """Use genuine kernel identities; never fabricate successful old nlink."""
    def setUp(self):
        self.assertIsNotNone(BASE, 'invoke the guarded CLI')
        self.assertIsNotNone(EXACT, 'explicit source-bound exact-alias helper required')
        self.assertIsNotNone(GUARD, 'actual selected current Linux cgroup required')
        GUARD()
        self.scratch = tempfile.TemporaryDirectory(prefix='uwvm-exact-alias-', dir='/tmp')
        self.root = Path(self.scratch.name)
        self.old, self.debug, self.arm, self.proof = [self.root / name for name in ('old', 'debug', 'arm', 'proof')]
        for path in (self.old, self.debug, self.arm, self.proof):
            path.mkdir()
        self.library = self.old / 'library'
        self.library.write_bytes(b'actual shared library fixture bytes\n' * 11)
        self.alias = self.debug / 'elf'
        os.link(self.library, self.alias)
        self.arm_anchor = self.old / 'ar'
        self.arm_anchor.write_bytes(b'actual complete auxiliary tool\n')
        os.link(self.arm_anchor, self.arm / 'ar')

    def tearDown(self):
        self.scratch.cleanup()
        GUARD()

    def pin(self, path):
        return BASE.opened_file(str(path), MIB, GUARD)

    def catalog(self):
        """The genuine old catalog format records executable, not full mode/nlink."""
        rows = []
        for path in sorted(self.old.iterdir()):
            pin = self.pin(path)
            rows.append({'relative': path.name, 'kind': 'file',
                **{key: pin[key] for key in ('bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns')},
                'executable': bool(path.stat().st_mode & 0o111)})
        catalog = self.root / 'original.jsonl'
        catalog.write_text(''.join(json.dumps(row, sort_keys=True, separators=(',', ':')) + '\n' for row in rows))
        return self.pin(catalog)

    def selected(self):
        info = self.library.stat()
        return {'current_pin': self.pin(self.alias),
                'historical_anchor': {'device': info.st_dev, 'inode': info.st_ino}}

    def union(self, catalog, selected):
        arm = self.arm_anchor.stat()
        return EXACT.ExactAliasUnion(BASE, UNION,
            [{'root': str(self.old), 'catalog': catalog}], GUARD, LIMITS,
            device_transitions={arm.st_dev: arm.st_dev},
            auxiliary_roots=[{'root': str(self.arm), 'historical_anchors': [
                {'device': arm.st_dev, 'inode': arm.st_ino}]}], exact_aliases=selected)

    def admit(self, union, catalog):
        complete = union.qualify_auxiliary(self.proof)
        self.assertEqual([item['current_root'] for item in complete], [str(self.arm)])
        union.requalify_tree(catalog, str(self.old), self.proof / 'old-current.jsonl')
        return union.finish()

    def test_exact_alias_preserves_without_authorizing_unrelated_multilink_sibling(self):
        sibling = self.debug / 'debug'
        sibling.write_bytes(b'unrelated debug bytes\n')
        os.link(sibling, self.root / 'other-debug-alias')
        (self.debug / 'probes').write_bytes(b'unselected ordinary sibling\n')
        self.assertEqual(sibling.stat().st_nlink, 2)
        catalog = self.catalog()
        union = self.union(catalog, [self.selected()])
        evidence = self.admit(union, catalog)
        rows = evidence['exact_current_aliases']
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]['current_pin'], self.pin(self.alias))
        self.assertEqual(rows[0]['current_nlink'], 2)
        self.assertEqual(rows[0]['historical_auxiliary_names'], 'NOT_RECORDED')
        self.assertEqual(rows[0]['historical_auxiliary_nlink'], 'NOT_RECORDED')
        self.assertEqual(rows[0]['sibling_directories'], 'NOT_SELECTED')
        self.assertEqual(rows[0]['execution_qualification'], 'NOT_PERFORMED')
        observed = {pin['path'] for row in evidence['current_classes'] for pin in row['current_members']}
        self.assertNotIn(str(sibling), observed)
        self.assertNotIn(str(sibling), union.by_name)
        # The unselected sibling can change without becoming a hidden input.
        sibling.write_bytes(b'changed unrelated debug bytes\n')
        self.assertEqual(union.preserve(), evidence)
        parent, leaf = BASE._parent(str(sibling))
        try:
            pin, _, info = BASE._opened_at(parent, leaf, str(sibling), MIB, GUARD, no_hardlinks=False)
        finally:
            os.close(parent)
        with self.assertRaisesRegex(ValueError, 'no selected historical union name'):
            union.observe(str(self.debug), sibling.name, pin, info)
        # In contrast, the explicitly selected complete ARM root stays complete.
        (self.arm / 'unexpected').write_bytes(b'new complete-root entry\n')
        with self.assertRaisesRegex(ValueError, 'current-only auxiliary ordinary file identity/mode changed'):
            union.preserve()

    def test_unknown_additional_link_of_selected_class_is_refused(self):
        os.link(self.library, self.root / 'unselected-library-alias')
        catalog = self.catalog()  # Capture genuine current ctime after os.link.
        union = self.union(catalog, [self.selected()])
        with self.assertRaisesRegex(ValueError, 'current nlink differs'):
            union.qualify_auxiliary(self.proof)

    def test_caller_selected_full7_must_equal_actual_complete_file_read(self):
        catalog = self.catalog()
        selected = self.selected()
        digest = selected['current_pin']['sha256']
        selected['current_pin']['sha256'] = ('0' if digest[0] != '0' else '1') + digest[1:]
        union = self.union(catalog, [selected])
        with self.assertRaisesRegex(ValueError, 'selected exact current alias full7 changed'):
            union.qualify_auxiliary(self.proof)

    def test_duplicate_exact_names_and_historical_names_are_refused(self):
        catalog = self.catalog()
        selected = self.selected()
        with self.assertRaisesRegex(ValueError, 'unique physical new name'):
            self.union(catalog, [selected, selected])
        selected['current_pin'] = self.pin(self.library)
        with self.assertRaisesRegex(ValueError, 'unique physical new name'):
            self.union(catalog, [selected])

    def test_symlink_spelling_cannot_select_a_regular_alias(self):
        catalog = self.catalog()
        selected = self.selected()
        link = self.debug / 'symlink-to-elf'
        link.symlink_to('elf')
        selected['current_pin']['path'] = str(link)
        with self.assertRaisesRegex(ValueError, 'unique physical new name'):
            self.union(catalog, [selected])

    def test_terminal_alias_content_mutation_is_refused(self):
        catalog = self.catalog()
        union = self.union(catalog, [self.selected()])
        self.admit(union, catalog)
        body = self.alias.read_bytes()
        self.alias.write_bytes(bytes([body[0] ^ 1]) + body[1:])
        with self.assertRaisesRegex(ValueError, 'selected exact current alias full7 changed'):
            union.preserve()


def main():
    global BASE, UNION, EXACT, GUARD
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('base-helper', 'union-helper', 'exact-helper', 'source', 'manifest', 'current-cgroup', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    for name in ('base-helper-sha256', 'union-helper-sha256', 'exact-helper-sha256',
                 'manifest-sha256', 'current-cgroup-sha256', 'test-sha256'):
        parser.add_argument('--' + name, required=True)
    args = parser.parse_args()
    require(sys.platform == 'linux', 'actual guarded Linux execution required')
    manifest = bounded(args.manifest, 16 * MIB)
    require(hashlib.sha256(manifest).hexdigest() == args.manifest_sha256, 'explicit complete source manifest digest differs')
    frozen = json.loads(manifest)
    source = args.source.resolve(strict=True)
    this = Path(__file__).resolve(strict=True)
    raw = bounded(this, MIB)
    require(hashlib.sha256(raw).hexdigest() == args.test_sha256, 'explicit reviewed test entry SHA differs')
    helpers = [this, args.base_helper, args.union_helper, args.exact_helper]
    require(len(set(helpers)) == len(helpers) and
            all(path.is_absolute() and path.resolve(strict=True) == path and
                not path.is_relative_to(source) and not source.is_relative_to(path) for path in helpers),
            'distinct physical external test/helper files disjoint from complete Runtime source required')
    def load(name, path, digest):
        raw = bounded(path, MIB)
        require(hashlib.sha256(raw).hexdigest() == digest, 'explicit reviewed helper source SHA differs')
        module = types.ModuleType(name)
        module.__file__ = str(path)
        sys.modules[name] = module
        exec(compile(raw, str(path), 'exec'), module.__dict__)
        return module
    # Only the exact previously reviewed base loads before its actual guard.
    # Union and new wrapper code load after the closed current context passes.
    require(args.base_helper_sha256 == '0f14a3e8b122286f5d934f385d8ca582aa14039cd0b97531dad19b528a1b3260',
            'unchanged strict base helper required for actual guard bootstrap')
    BASE = load('exact_test_base', args.base_helper, args.base_helper_sha256)
    context = bounded(args.current_cgroup, MIB)
    require(hashlib.sha256(context).hexdigest() == args.current_cgroup_sha256, 'explicit current cgroup selection digest differs')
    GUARD = BASE.CurrentGuard(str(source), frozen, json.loads(context))
    GUARD.maximum_seconds = 600
    before = GUARD.force()
    GUARD()
    UNION = load('exact_test_union', args.union_helper, args.union_helper_sha256)
    GUARD()
    EXACT = load('exact_test_candidate', args.exact_helper, args.exact_helper_sha256)
    inputs = [BASE.opened_file(str(path), maximum, GUARD) for path, maximum in
        ((this, MIB), (args.base_helper, MIB), (args.union_helper, MIB), (args.exact_helper, MIB),
         (args.current_cgroup, MIB), (args.manifest, 16 * MIB))]
    output = args.out
    require(output.is_absolute() and output.resolve() == output and not output.exists() and not output.is_symlink() and
            all(output != Path(pin['path']) and output not in Path(pin['path']).parents and
                Path(pin['path']) not in output.parents for pin in inputs) and
            not output.is_relative_to(source) and not source.is_relative_to(output), 'fresh disjoint test receipt output required')
    output.mkdir(mode=0o700, parents=True, exist_ok=False)
    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(ExactAliasKernelPolicy))
    after = GUARD.force()
    require(all(BASE.opened_file(pin['path'], pin['bytes'], GUARD) == pin for pin in inputs), 'test/source/context changed during actual tests')
    report = {'format_version': 1, 'kind': 'exact-current-alias-kernel-filesystem-tests',
        'status': 'PASS' if result.wasSuccessful() and result.testsRun == 6 and not result.skipped else 'FAIL',
        'tests_run': result.testsRun, 'failures': len(result.failures), 'errors': len(result.errors), 'skipped': len(result.skipped),
        'input_pins': inputs, 'cgroup_before': before, 'cgroup_after': after, 'scratch_root': '/tmp', 'commands': [],
        'scope': 'ACTUAL_SMALL_KERNEL_EXACT_ALIAS_POLICY_ONLY_NO_SDK_COMPILER_RUNTIME_ROS_OR_RESTART_EXECUTION_QUALIFICATION'}
    BASE._write_report(output, report)
    print(json.dumps({'status': report['status'], 'tests_run': result.testsRun, 'report': str(output / 'report.json')}))
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
