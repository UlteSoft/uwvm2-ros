#!/usr/bin/env python3
"""Small kernel/model regressions; run only inside Root's exact64GiB leaf.

This is test SOURCE, not a claim that any case ran. The caller must supply the
SHA of a finite current helper selection and a current-cgroup document, the
original complete Source35 manifest, and a fresh scratch path on the owned
Btrfs image. No network, APT, compiler, target program or physical robot runs.
Every generated tar/file is tiny; the suite is serial. Root owns actual runs.
"""
from __future__ import annotations
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import stat
import sys
import tarfile
import types
import unittest
from unittest import mock

MIB, GIB = 1024 ** 2, 1024 ** 3
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
ROLES = {'producer', 'resume', 'model', 'storage', 'restart'}
MODULES = {}; GUARD = COMMON = ROOT = None


def need(value, message):
    if not value: raise ValueError(message)


def read(path, maximum=16 * MIB):
    path = Path(path); need(path.is_absolute() and path.resolve(strict=True) == path, 'physical absolute test input required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum, 'bounded ordinary test bootstrap required')
        body = bytearray()
        while True:
            block = os.read(descriptor, min(MIB, maximum + 1 - len(body)))
            if not block: break
            body.extend(block); need(len(body) <= maximum, 'test input grew')
        last, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(body) == before.st_size and all(getattr(before, key) == getattr(last, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mode', 'st_mtime_ns', 'st_ctime_ns')), 'test input changed')
        return bytes(body), {'path': str(path), 'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest(),
            'device': before.st_dev, 'inode': before.st_ino, 'mtime_ns': before.st_mtime_ns, 'ctime_ns': before.st_ctime_ns}
    finally: os.close(descriptor)


def parse(raw):
    def pairs(rows):
        result = {}
        for key, value in rows:
            need(key not in result, 'duplicate test selection key'); result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: need(False, 'nonfinite test JSON'))


def load(pin):
    need(set(pin) == PIN, 'finite explicit helper full7 required'); body, actual = read(pin['path'], MIB)
    need(actual == pin, 'selected test helper changed')
    module = types.ModuleType('noetic_r6_test_' + Path(pin['path']).stem.replace('-', '_'))
    module.__file__ = pin['path']; sys.modules[module.__name__] = module
    exec(compile(body, module.__file__, 'exec'), module.__dict__)
    return module


LIMITS = {'max_entries': 256, 'max_file_bytes': MIB, 'max_tar_bytes': 4 * MIB,
          'max_sdk_bytes': 8 * MIB, 'max_pending_hardlinks': 64}


class FreshRebuildTests(unittest.TestCase):
    """Exercise independent model, real dirfds/CoW, and meaningful refusals."""
    def setUp(self):
        GUARD(); self.base = ROOT / self._testMethodName; self.base.mkdir(mode=0o700)
        self.producer, self.rebuild, self.model, self.storage = (MODULES[key] for key in ('producer', 'resume', 'model', 'storage'))

    def archive(self):
        path = self.base / 'signed-test.tar'
        with tarfile.open(path, 'w', format=tarfile.USTAR_FORMAT) as output:
            directory = tarfile.TarInfo('usr'); directory.type = tarfile.DIRTYPE; directory.mode = 0o755; output.addfile(directory)
            directory = tarfile.TarInfo('usr/lib'); directory.type = tarfile.DIRTYPE; directory.mode = 0o755; output.addfile(directory)
            body = b'fresh authenticated fixture\n'
            file = tarfile.TarInfo('usr/lib/value'); file.size = len(body); file.mode = 0o644; output.addfile(file, io.BytesIO(body))
            alias = tarfile.TarInfo('value-alias'); alias.type = tarfile.SYMTYPE; alias.linkname = '/usr/lib/value'; alias.mode = 0o777; output.addfile(alias)
            hardlink = tarfile.TarInfo('usr/lib/value-copy'); hardlink.type = tarfile.LNKTYPE; hardlink.linkname = 'usr/lib/value'; hardlink.mode = 0o644; output.addfile(hardlink)
        return path

    def prepared(self):
        archive = self.archive(); model = self.model.VirtualTree(self.producer, LIMITS, GUARD)
        expected = self.rebuild.GeneratedLedger(self.base / 'expected.jsonl')
        model.audit(archive, 'fixture=1', expected, partial=False)
        session = object.__new__(self.rebuild.ResumeSession)
        session.model = model; session.expected = expected; session.hardlink_destinations = set(); session.complete_model(); expected.close()
        sdk = self.base / 'sdk'; sdk.mkdir(mode=0o700)
        tree = self.producer.SafeTree(sdk, self.base / 'actual.jsonl', LIMITS, GUARD)
        tree.extract(archive, 'fixture=1'); tree.finish()
        return session, tree

    def verify(self, session, tree, name='verified.jsonl'):
        self.rebuild.verify_physical(session.model.nodes, session.link_groups, tree.rootfd, self.base / name, GUARD, LIMITS)

    def test_fresh_model_matches_unchanged_SafeTree_ledger_and_hardlinks(self):
        session, tree = self.prepared()
        try:
            self.assertEqual((tree.entries, tree.bytes, tree.sequence), (session.model.entries, session.model.bytes, session.model.sequence))
            self.assertEqual((self.base / 'actual.jsonl').read_bytes(), (self.base / 'expected.jsonl').read_bytes())
            self.verify(session, tree)
            self.assertEqual((self.base / 'sdk/usr/lib/value').stat().st_ino, (self.base / 'sdk/usr/lib/value-copy').stat().st_ino)
            self.assertEqual(os.readlink(self.base / 'sdk/value-alias'), 'usr/lib/value')
        finally: tree.close()

    def test_missing_signed_name_is_refused(self):
        session, tree = self.prepared()
        try:
            (self.base / 'sdk/value-alias').unlink()
            with self.assertRaises(ValueError): self.verify(session, tree)
        finally: tree.close()

    def test_extra_unsigned_name_is_refused(self):
        session, tree = self.prepared()
        try:
            (self.base / 'sdk/foreign').write_bytes(b'not in signed tar model')
            with self.assertRaises(ValueError): self.verify(session, tree)
        finally: tree.close()

    def test_changed_signed_bytes_are_refused(self):
        session, tree = self.prepared()
        try:
            (self.base / 'sdk/usr/lib/value').write_bytes(b'changed')
            with self.assertRaises(ValueError): self.verify(session, tree)
        finally: tree.close()

    def test_split_hardlink_class_is_refused(self):
        session, tree = self.prepared()
        try:
            path = self.base / 'sdk/usr/lib/value-copy'; original = path.read_bytes(); path.unlink(); path.write_bytes(original); path.chmod(0o644)
            with self.assertRaises(ValueError): self.verify(session, tree)
        finally: tree.close()

    def test_unaccounted_external_hardlink_is_refused(self):
        session, tree = self.prepared()
        try:
            os.link(self.base / 'sdk/usr/lib/value', self.base / 'external-hardlink')
            with self.assertRaises(ValueError): self.verify(session, tree)
        finally: tree.close()

    def partial(self):
        archive = self.archive(); model = self.model.VirtualTree(self.producer, LIMITS, GUARD)
        expected = self.rebuild.GeneratedLedger(self.base / 'expected.jsonl'); model.audit(archive, 'fixture=1', expected, partial=False)
        session = object.__new__(self.rebuild.ResumeSession); session.model = model; session.expected = expected
        session.hardlink_destinations = set(); session.complete_model(); expected.close()
        old = self.base / 'old'; old.mkdir(mode=0o700)
        extraction = self.producer.SafeTree(old, self.base / 'old-incomplete-ledger.jsonl', LIMITS, GUARD)
        extraction.extract(archive, 'fixture=1'); extraction.close()  # No finish: emulate interrupted extraction only.
        (old / 'usr/lib/.uwvm-acquire-0000000000000002.pending').write_bytes(b'partial uncommitted temporary')
        session.control = {'old_partial_sdk': {'path': str(old)}}; session.fd = self.rebuild.directory(old)
        session.limits = LIMITS; session.safety_guard = session.guard = GUARD; session.storage_module = self.storage
        return session, old

    def test_authenticated_subset_uses_new_inode_and_excludes_only_new_temp(self):
        session, old = self.partial(); new = self.base / 'new'; new.mkdir(mode=0o700); target = self.rebuild.directory(new)
        try:
            before = list(session.old_rows()); actual = list(session.old_rows(copy_to=target)); self.assertEqual(before, actual)
            source = old / 'usr/lib/value'; clone = new / 'usr/lib/value'
            self.assertEqual(source.read_bytes(), clone.read_bytes()); self.assertEqual(source.stat().st_dev, clone.stat().st_dev)
            self.assertNotEqual(source.stat().st_ino, clone.stat().st_ino); self.assertEqual(clone.stat().st_nlink, 1)
            self.assertTrue((old / 'usr/lib/.uwvm-acquire-0000000000000002.pending').exists())
            self.assertFalse((new / 'usr/lib/.uwvm-acquire-0000000000000002.pending').exists())
            self.assertFalse((new / 'usr/lib/value-copy').exists())
            self.assertEqual(before, list(session.old_rows()))
            clone.write_bytes(b'changed new file'); self.assertEqual(source.read_bytes(), b'fresh authenticated fixture\n')
        finally: os.close(target); session.close()

    def test_old_partial_normal_mismatch_is_refused_before_clone(self):
        session, old = self.partial()
        try:
            (old / 'usr/lib/value').write_bytes(b'old bytes have no signed equality')
            with self.assertRaises(ValueError): list(session.old_rows())
        finally: session.close()

    def test_unknown_old_normal_file_is_refused(self):
        session, old = self.partial()
        try:
            (old / 'unexpected').write_bytes(b'not a reserved pending name')
            with self.assertRaises(ValueError): list(session.old_rows())
        finally: session.close()

    def test_clone_unsupported_has_no_ordinary_copy_fallback(self):
        source = self.base / 'source'; target = self.base / 'target'; source.write_bytes(b'bytes')
        left = os.open(source, os.O_RDONLY); right = os.open(target, os.O_RDWR | os.O_CREAT | os.O_EXCL, 0o600)
        try:
            with mock.patch.object(self.storage.fcntl, 'ioctl', side_effect=OSError(95, 'not supported')):
                with self.assertRaises(OSError): self.storage.clone_descriptors(left, right, GUARD)
            self.assertEqual(target.stat().st_size, 0); self.assertEqual(source.read_bytes(), b'bytes')
        finally: os.close(left); os.close(right)

    def test_read_only_catalog_matches_maintained_rows_without_after_file(self):
        root = self.base / 'catalog-root'; root.mkdir(); (root / 'file').write_bytes(b'content'); os.symlink('file', root / 'alias')
        maintained = COMMON.catalog(root, self.base / 'before.jsonl', [root])
        observed = self.producer.read_only_catalog(COMMON, root, [root], GUARD)
        self.assertEqual({k: v for k, v in maintained.items() if k != 'catalog'},
                         {k: v for k, v in observed.items() if k not in {'catalog', 'catalog_disposition'}})
        self.assertIsNone(observed['catalog']); self.assertFalse((self.base / 'after.jsonl').exists())
        (root / 'file').write_bytes(b'changed')
        self.assertNotEqual(observed['content_sha256'], self.producer.read_only_catalog(COMMON, root, [root], GUARD)['content_sha256'])


def main():
    global GUARD, COMMON, ROOT
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for role in ('source', 'manifest', 'current', 'selection', 'out'):
        parser.add_argument('--' + role, type=Path, required=True)
    for role in ('manifest', 'current', 'selection'): parser.add_argument('--' + role + '-sha256', required=True)
    args = parser.parse_args()
    need(sys.platform == 'linux' and os.geteuid() == 0, 'Root Linux kernel-policy tests required')
    raw, current_pin = read(args.current); current = parse(raw); need(current_pin['sha256'] == args.current_sha256, 'Root-selected current context SHA differs')
    leaf = Path(current['expected_leaf']); info = leaf.stat()
    need(set(current) == {'memory_max', 'memory_swap_max', 'expected_device', 'expected_inode', 'expected_boot_id', 'expected_leaf', 'expected_cpus'} and
         current['memory_max'] == 64 * GIB and current['memory_swap_max'] == 0 and current['expected_cpus'] == '0-31' and
         current['expected_leaf'] == '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope' and
         (info.st_dev, info.st_ino) == (current['expected_device'], current['expected_inode']) and
         Path('/proc/sys/kernel/random/boot_id').read_text().strip() == current['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::' + current['expected_leaf'].removeprefix('/sys/fs/cgroup')] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and (leaf / 'memory.swap.max').read_text().strip() == '0' and
         (leaf / 'cpuset.cpus.effective').read_text().strip() == '0-31', 'exact Root current64GiB zero-swap leaf before project test imports required')
    raw, selection_pin = read(args.selection); selection = parse(raw)
    need(selection_pin['sha256'] == args.selection_sha256 and set(selection) == ROLES, 'exact finite five-role current helper selection required')
    raw, mf_pin = read(args.manifest); frozen = parse(raw)
    need(mf_pin['sha256'] == args.manifest_sha256 == 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396' and mf_pin['bytes'] == 3397553 and len(frozen) == 17882,
         'complete genuine Source35 manifest required')
    MODULES['restart'] = load(selection['restart'])
    GUARD = MODULES['restart'].CurrentGuard(str(args.source), frozen, current); GUARD.maximum_seconds = 600; GUARD.force()
    for role in ('producer', 'resume', 'model', 'storage'):
        GUARD(); MODULES[role] = load(selection[role])
    COMMON = MODULES['producer'].load_common(args.source, args.manifest, args.manifest_sha256)
    ROOT = args.out
    need(ROOT.is_absolute() and ROOT.parent.resolve(strict=True) == ROOT.parent and not ROOT.exists() and not ROOT.is_symlink() and
         str(ROOT).startswith('/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1/'), 'fresh explicitly owned image test scratch required')
    ROOT.mkdir(mode=0o700)
    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(FreshRebuildTests))
    for expected in [current_pin, selection_pin, mf_pin, *selection.values()]:
        actual = MODULES['restart'].opened_file(expected['path'], max(16 * MIB, expected['bytes']), GUARD)
        need(actual == expected, 'kernel test source/control input changed')
    GUARD.force()
    print(json.dumps({'kind': 'noetic-r6-small-kernel-and-signed-model-regressions', 'tests_run': result.testsRun,
        'failures': len(result.failures), 'errors': len(result.errors), 'actual_success': result.wasSuccessful(),
        'network_executed': False, 'compiler_executed': False, 'SDK513_extraction_executed': False}), flush=True)
    return 0 if result.wasSuccessful() else 1


if __name__ == '__main__': raise SystemExit(main())
