#!/usr/bin/env python3
"""Guarded, small filesystem regressions for Noetic continuation source.

Root runs this file explicitly inside the selected 64 GiB/zero-swap leaf after
reviewing its bytes. All fixture files stay below a fresh caller-selected output
directory. Tests use the actual maintained SafeTree methods, current opened
pins and the real no-replace rename primitive. No network/package command,
compiler, target program, ROS, simulator or Runtime is started. Passing these
tests does not authenticate the real partial SDK or complete its extraction.
"""
from __future__ import annotations
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import resource
import stat
import sys
import tarfile
import types
import unittest

MIB, GIB = 1024 ** 2, 1024 ** 3
S = Path('/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r35/source')
MF = S.parent / 'source-manifest.json'
MH = 'eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396'
CG = {'memory_max': 64 * GIB, 'memory_swap_max': 0, 'expected_device': 29,
      'expected_inode': 18315, 'expected_boot_id': 'ec0d5f0f-3060-4220-bee2-2a7791ed4e72',
      'expected_leaf': '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
      'expected_cpus': '0-31'}
LIMITS = {'max_entries': 1000, 'max_pending_hardlinks': 64, 'max_file_bytes': MIB,
          'max_tar_bytes': 4 * MIB, 'max_sdk_bytes': 8 * MIB}
RESTART = RESUME = PRODUCER = COMMON = GUARD = OUT = None
INPUTS = []


def need(value, message):
    if not value: raise ValueError(message)


def bounded(path, maximum):
    """Admit ordinary stable bytes before allocation or helper execution."""
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'explicit physical reviewed helper required')
    descriptor = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= maximum, 'bounded ordinary regression input required')
        body = bytearray()
        while True:
            block = os.read(descriptor, min(65536, maximum + 1 - len(body)))
            if not block: break
            body.extend(block); need(len(body) <= maximum, 'regression input grew')
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(body) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'regression input changed')
        return bytes(body)
    finally: os.close(descriptor)


def load(path, expected, name):
    path = Path(path); raw = bounded(path, MIB)
    need(0 < len(raw) <= MIB and hashlib.sha256(raw).hexdigest() == expected, 'reviewed complete helper SHA required')
    module = types.ModuleType(name); module.__file__ = str(path); sys.modules[name] = module
    exec(compile(raw, str(path), 'exec'), module.__dict__)
    return module


def archive(path, entries):
    """Create only tiny explicit test tar records; never archive ambient files."""
    with tarfile.open(path, 'w', format=tarfile.USTAR_FORMAT) as stream:
        root = tarfile.TarInfo('./'); root.type = tarfile.DIRTYPE; root.mode = 0o755; stream.addfile(root)
        for kind, name, value in entries:
            info = tarfile.TarInfo(name); info.mode = 0o755 if kind == 'directory' else 0o644
            if kind == 'file':
                info.size = len(value); stream.addfile(info, io.BytesIO(value))
            elif kind == 'directory':
                info.type = tarfile.DIRTYPE; stream.addfile(info)
            elif kind == 'symlink':
                info.type = tarfile.SYMTYPE; info.linkname = value; stream.addfile(info)
            elif kind == 'hardlink':
                info.type = tarfile.LNKTYPE; info.linkname = value; stream.addfile(info)
            else:
                raise ValueError('unknown fixture tar kind')


class PartialResumeRegressions(unittest.TestCase):
    def setUp(self):
        GUARD()
        self.root = OUT / self.id().rsplit('.', 1)[-1]; self.root.mkdir(mode=0o700)
        self.sdk = self.root / 'sdk'; self.sdk.mkdir(mode=0o700)
        self.ledger = self.root / 'old-ledger.jsonl'

    def tearDown(self):
        GUARD()
        self.assertLessEqual(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024, 512 * MIB)

    def maintained(self, entries):
        path = self.root / 'maintained.tar'; archive(path, entries)
        tree = PRODUCER.SafeTree(self.sdk, self.ledger, LIMITS, GUARD)
        try:
            tree.extract(path, 'fixture=1')
            tree.ledger.flush(); os.fsync(tree.ledger.fileno())
            state = (tree.entries, tree.bytes, tree.sequence, tree.pending)
        finally:
            tree.close()
        return path, state

    def replay(self, tar, *, partial=False):
        expected = RESTART.opened_file(str(self.ledger), MIB, GUARD)
        ledger = RESUME.Ledger(self.ledger, expected, GUARD)
        virtual = RESUME.VirtualTree(PRODUCER, LIMITS, GUARD)
        try:
            virtual.audit(tar, 'fixture=1', ledger, partial=partial)
            self.assertIsNone(ledger.next_row)
            checkpoint = virtual.checkpoint(ledger)
        finally:
            ledger.close()
        return virtual, checkpoint

    def physical(self, virtual, name='physical.jsonl'):
        descriptor = os.open(self.sdk, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        try:
            return virtual.verify_physical(descriptor, self.root / name, GUARD)
        finally:
            os.close(descriptor)

    def test_usr_merge_alias_and_absolute_symlink_rewrite_match_maintained_tree(self):
        tar, expected = self.maintained([('directory', 'usr', ''), ('directory', 'usr/bin', ''),
            ('symlink', 'bin', 'usr/bin'), ('file', 'bin/tool', b'actual bytes'), ('symlink', 'usr/bin/alias', '/bin/tool')])
        virtual, checkpoint = self.replay(tar)
        self.assertEqual((checkpoint['entries'], checkpoint['bytes'], checkpoint['sequence'], checkpoint['pending']), expected)
        proof = self.physical(virtual); self.assertEqual(proof['files'], 1); self.assertEqual(proof['symlinks'], 2)

    def test_identical_duplicate_regular_overlay_counts_every_payload(self):
        tar, expected = self.maintained([('file', 'deep/a', b'abc'), ('file', 'deep/a', b'abc')])
        virtual, checkpoint = self.replay(tar)
        self.assertEqual((checkpoint['entries'], checkpoint['bytes'], checkpoint['sequence'], checkpoint['pending']), expected)
        self.assertEqual(checkpoint['bytes'], 6); self.assertEqual(self.physical(virtual)['bytes'], 3)

    def test_deferred_hardlinks_restore_once_from_committed_checkpoint(self):
        tar, expected = self.maintained([('file', 'a', b'abc'), ('hardlink', 'b', 'a')])
        virtual, checkpoint = self.replay(tar)
        self.assertEqual(checkpoint['pending'], expected[3]); self.assertFalse((self.sdk / 'b').exists())
        self.physical(virtual)
        old = self.ledger.read_bytes(); resume = RESUME.ResumeSession.__new__(RESUME.ResumeSession)
        resume.maintained = PRODUCER; resume.limits = LIMITS; resume.guard = GUARD
        resume.out = self.root / 'new'; resume.out.mkdir(); resume.report = {'continuation': {}}
        resume.current_ledger = RESTART.opened_file(str(self.ledger), MIB, GUARD)
        resume.sdkfd = os.open(self.sdk, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        try:
            tree = resume.prepare_tree(checkpoint)
            try:
                self.assertEqual(tree.pending, expected[3]); tree.finish()
            finally:
                tree.close()
        finally:
            os.close(resume.sdkfd)
        self.assertEqual((self.sdk / 'a').stat().st_ino, (self.sdk / 'b').stat().st_ino)
        self.assertEqual(self.ledger.read_bytes(), old)
        rows = [json.loads(row) for row in (resume.out / 'payloads.jsonl').read_bytes().splitlines()]
        self.assertEqual(sum(row['kind'] == 'hardlink-resolved' for row in rows), 1)

    def test_partial_suffix_stops_before_next_unrecorded_member(self):
        _, _ = self.maintained([('file', 'a', b'abc')])
        full = self.root / 'full-next-package.tar'
        archive(full, [('file', 'a', b'abc'), ('file', 'never-recorded', b'not admitted')])
        virtual, checkpoint = self.replay(full, partial=True)
        self.assertNotIn('never-recorded', virtual.nodes); self.assertEqual(checkpoint['sequence'], 1)
        self.assertEqual(self.physical(virtual)['files'], 1)

    def test_changed_ledger_digest_is_rejected_by_authenticated_replay(self):
        tar, _ = self.maintained([('file', 'a', b'abc')])
        row = json.loads(self.ledger.read_bytes()); row['sha256'] = '0' * 64
        self.ledger.write_text(json.dumps(row) + '\n')
        with self.assertRaisesRegex(ValueError, 'differs from retained ledger'):
            self.replay(tar)

    def test_unrecorded_actual_name_is_rejected_without_deleting_it(self):
        tar, _ = self.maintained([('file', 'a', b'abc')]); virtual, _ = self.replay(tar)
        extra = self.sdk / 'unrecorded'; extra.write_bytes(b'preserve')
        with self.assertRaisesRegex(ValueError, 'unexpected actual SDK name'):
            self.physical(virtual)
        self.assertEqual(extra.read_bytes(), b'preserve')

    def test_current_actual_regular_bytes_must_match_signed_tar(self):
        tar, _ = self.maintained([('file', 'a', b'abc')]); virtual, _ = self.replay(tar)
        (self.sdk / 'a').write_bytes(b'xyz')
        with self.assertRaisesRegex(ValueError, 'bytes changed or differ'):
            self.physical(virtual)

    def transfer_session(self, *, fail_confirmation=False):
        source = self.root / 'old-output'; destination = self.root / 'new-output'
        source.mkdir(); destination.mkdir(); self.sdk.rename(source / 'sdk')
        (source / 'report.json').write_bytes(b'original failed report')
        (source / 'payloads.jsonl').write_bytes(b'original ledger')
        session = RESUME.ResumeSession.__new__(RESUME.ResumeSession)
        session.guard = GUARD; session.out = destination
        session.source_parent = RESUME.directory_fd(source); session.destination_parent = RESUME.directory_fd(destination)
        session.sdkfd = os.open(source / 'sdk', os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        session.fds = [session.source_parent, session.destination_parent, session.sdkfd]
        session.control = {'old_sdk': RESUME.directory_pin(source / 'sdk', os.fstat(session.sdkfd))}
        def atomic(path, value):
            if fail_confirmation and path.name == 'SDK-directory-transfer-confirmed.json':
                raise OSError('explicit fixture confirmation failure')
            COMMON.atomic_json(path, value)
        session.common = types.SimpleNamespace(atomic_json=atomic)
        session.product = lambda path, maximum: RESTART.opened_file(str(path), maximum, GUARD)
        return session, source, destination

    def test_atomic_transfer_moves_only_the_held_sdk_directory(self):
        session, source, destination = self.transfer_session(); original = os.fstat(session.sdkfd)
        try:
            receipt = session.transfer({'scope': 'SYNTHETIC_FIXTURE'}, {'scope': 'SYNTHETIC_FIXTURE'})
            self.assertFalse((source / 'sdk').exists()); self.assertTrue((destination / 'sdk').is_dir())
            self.assertEqual((destination / 'sdk').stat().st_ino, original.st_ino)
            self.assertEqual((source / 'report.json').read_bytes(), b'original failed report')
            self.assertEqual((source / 'payloads.jsonl').read_bytes(), b'original ledger')
            self.assertTrue(Path(receipt['path']).is_file())
        finally: session.close()

    def test_existing_destination_blocks_transfer_and_preserves_both_names(self):
        session, source, destination = self.transfer_session(); (destination / 'sdk').mkdir()
        try:
            with self.assertRaisesRegex(ValueError, 'already exists'):
                session.transfer({}, {})
            self.assertTrue((source / 'sdk').is_dir()); self.assertTrue((destination / 'sdk').is_dir())
            self.assertFalse((destination / 'SDK-directory-transfer-intent.json').exists())
        finally: session.close()

    def test_failure_after_rename_keeps_durable_intent_without_invented_confirmation(self):
        session, source, destination = self.transfer_session(fail_confirmation=True)
        try:
            with self.assertRaisesRegex(OSError, 'explicit fixture confirmation failure'):
                session.transfer({}, {})
            self.assertFalse((source / 'sdk').exists()); self.assertTrue((destination / 'sdk').is_dir())
            self.assertTrue((destination / 'SDK-directory-transfer-intent.json').is_file())
            self.assertFalse((destination / 'SDK-directory-transfer-confirmed.json').exists())
            self.assertEqual((source / 'payloads.jsonl').read_bytes(), b'original ledger')
        finally: session.close()


def main():
    global RESTART, RESUME, PRODUCER, COMMON, GUARD, OUT
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for role in ('restart', 'resume', 'producer'):
        parser.add_argument('--' + role, type=Path, required=True); parser.add_argument('--' + role + '-sha256', required=True)
    parser.add_argument('--out', type=Path, required=True); args = parser.parse_args()
    need(sys.platform == 'linux' and os.geteuid() == 0, 'Root actual guarded Linux regressions required')
    leaf = Path(CG['expected_leaf']); info = leaf.stat()
    need((info.st_dev, info.st_ino) == (29, 18315) and Path('/proc/sys/kernel/random/boot_id').read_text().strip() == CG['expected_boot_id'] and
         Path('/proc/self/cgroup').read_text().splitlines() == ['0::/' + str(leaf.relative_to('/sys/fs/cgroup'))] and
         (leaf / 'memory.max').read_text().strip() == str(64 * GIB) and (leaf / 'memory.swap.max').read_text().strip() == '0',
         'exact current hard-64-GiB zero-swap membership before helper load required')
    RESTART = load(args.restart, args.restart_sha256, 'noetic_resume_test_restart')
    raw = bounded(MF, 16 * MIB); need(hashlib.sha256(raw).hexdigest() == MH, 'complete Source35 probe manifest required')
    GUARD = RESTART.CurrentGuard(str(S), json.loads(raw), CG); GUARD.maximum_seconds = 300; before = GUARD.force()
    mf_pin, current_manifest = RESTART.opened_file(str(MF), 16 * MIB, GUARD, collect=True)
    need(current_manifest == raw and mf_pin['sha256'] == MH, 'manifest changed across guard bootstrap')
    RESUME = load(args.resume, args.resume_sha256, 'noetic_resume_test_adapter')
    PRODUCER = load(args.producer, args.producer_sha256, 'noetic_resume_test_producer')
    COMMON = PRODUCER.load_common(S, MF, MH)
    for path in (args.restart, args.resume, args.producer, Path(__file__), MF):
        INPUTS.append(RESTART.opened_file(str(path), 16 * MIB, GUARD))
    OUT = args.out
    need(OUT.is_absolute() and OUT.parent.resolve(strict=True) == OUT.parent and not OUT.exists() and not OUT.is_symlink(),
         'fresh canonical fixture output required')
    OUT.mkdir(mode=0o700)
    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(PartialResumeRegressions))
    need(all(RESTART.opened_file(pin['path'], pin['bytes'], GUARD) == pin for pin in INPUTS), 'regression source input changed')
    report = {'format_version': 1, 'kind': 'noetic-partial-resume-small-filesystem-regressions',
        'status': 'PASS' if result.wasSuccessful() and result.testsRun == 10 and not result.skipped else 'FAIL',
        'tests_run': result.testsRun, 'input_pins': INPUTS, 'cgroup_before': before, 'cgroup_after': GUARD.force(),
        'commands': [], 'compiler_executed': False, 'network_executed': False, 'real_partial_SDK_requalified': False,
        'scope': 'SYNTHETIC_SMALL_FILESYSTEM_FIXTURES_ONLY_NO_REAL_SDK_ROS_RUNTIME_OR_PERFORMANCE_QUALIFICATION'}
    COMMON.atomic_json(OUT / 'report.json', report)
    print(json.dumps({'status': report['status'], 'tests_run': result.testsRun, 'report': str(OUT / 'report.json')}), flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__': raise SystemExit(main())
