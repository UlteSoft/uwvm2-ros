#!/usr/bin/env python3
"""Physical, bounded ELF decoder regressions for a guarded Linux test owner.

Load this file and the selected support module only after the owning process
has admitted their exact source bytes, its complete manifest and current64-GiB
zero-swap context. Call ``suite(support, scratch=OWNED_EMPTY_DIR, guard=GUARD)``.
There are no candidate imports, standalone entry, process launches or ROS claims.

Fixtures use at most a few KiB of written data plus1-MiB streaming buffers. The
large valid fixture and the2-GiB+1 rejection fixture are sparse ordinary files.
The owner must budget their LOGICAL output sizes (at least3GiB), even though
neither allocates a corresponding Python byte array or backing-file body.
Each test removes its files before the next one. No fixture escapes scratch.
"""
import hashlib
import os
from pathlib import Path
import stat
import struct
import unittest

MIB = 1024 ** 2
GIB = 1024 ** 3
BASE = 0x400000
INTERP = b'/lib64/ld-linux-x86-64.so.2\0'
STRINGS = b'\0libfixture.so\0/owned/fixture/lib\0'
RPATH = STRINGS.index(b'/owned')


def image(*, length=4096, machine=62, file_type=3, extra=(), tags=None, strings=STRINGS, interpreter=INTERP):
    """Return only the small actual loader-layout prefix, never ``length`` bytes.

    A single file-backed PT_LOAD covers the declared ordinary file. The dynamic
    table resolves its real DT_STRTAB virtual address through that segment.
    Additional program headers and explicit tags form malformed counterexamples.
    """
    entries = list(tags) if tags is not None else [(5, BASE + 0x500), (10, len(strings)), (1, 1), (15, RPATH), (0, 0)]
    programs = [(1, 0, BASE, length), (3, 0x200, BASE + 0x200, len(interpreter)),
                (2, 0x300, BASE + 0x300, 16 * len(entries)), *extra]
    raw = bytearray(4096); raw[:7] = b'\x7fELF\x02\x01\x01'
    struct.pack_into('<HHIQQQIHHHHHH', raw, 16, file_type, machine, 1, 0, 64, 0, 0, 64, 56, len(programs), 0, 0, 0)
    for index, (tag, offset, address, size) in enumerate(programs):
        struct.pack_into('<IIQQQQQQ', raw, 64 + 56 * index, tag, 4, offset, address, address, size, size, 4096)
    raw[0x200:0x200 + len(interpreter)] = interpreter
    for index, (tag, value) in enumerate(entries): struct.pack_into('<qQ', raw, 0x300 + 16 * index, tag, value)
    raw[0x500:0x500 + len(strings)] = strings
    return raw


def suite(support, *, scratch, guard):
    """Return explicit tests against the owner's admitted actual ``support.elf``.

    ``guard`` remains active at creation, each decoder observation and teardown.
    It may enforce cgroup/source/backing/RSS/deadline policy and may raise. This
    factory neither imports a candidate module nor relaxes those caller checks.
    """
    scratch = Path(scratch)
    if not (scratch.is_absolute() and scratch.resolve(strict=True) == scratch and scratch.is_dir()
            and not list(scratch.iterdir())):
        raise ValueError('actual fresh empty owned physical scratch directory required')

    class PhysicalELF(unittest.TestCase):
        def setUp(self):
            guard(); self.paths = []; self.ordinal = 0

        def tearDown(self):
            for path in reversed(self.paths):
                if path.exists() or path.is_symlink():
                    mode = path.lstat().st_mode
                    if stat.S_ISDIR(mode): path.rmdir()
                    else: path.unlink()
            guard()

        def file(self, body=None, length=4096):
            path = scratch / ('fixture-%d' % self.ordinal); self.ordinal += 1; self.paths.append(path)
            fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW | os.O_CLOEXEC, 0o600)
            try:
                raw = image(length=length) if body is None else body
                view = memoryview(raw)
                while view:
                    done = os.write(fd, view)
                    if done <= 0: raise RuntimeError('short owned fixture write')
                    view = view[done:]
                os.ftruncate(fd, length); os.fsync(fd)
            finally: os.close(fd)
            return path

        def decode(self, path, kind=(3,), live=None):
            return support.elf(None, path, kind, guard=guard if live is None else live)

        def test_valid_layout_and_complete_digest(self):
            path = self.file(); value = self.decode(path)
            self.assertEqual(value['PT_INTERP'], [INTERP[:-1].decode()])
            self.assertEqual(value['DT_NEEDED'], ['libfixture.so'])
            self.assertEqual(value['DT_RPATH'], ['/owned/fixture/lib']); self.assertEqual(value['DT_RUNPATH'], [])
            self.assertEqual((value['machine'], value['bits'], value['endian'], value['type']), (62, 64, 'little', 3))
            self.assertEqual(value['input']['sha256'], hashlib.sha256(path.read_bytes()).hexdigest())

        def test_valid_larger_than_historical64MiB_is_streamed(self):
            length = 65 * MIB + 17; path = self.file(length=length); calls = []
            def live(): guard(); calls.append(None)
            value = self.decode(path, live=live); digest = hashlib.sha256()
            with path.open('rb') as stream:
                while True:
                    guard(); block = stream.read(MIB)
                    if not block: break
                    digest.update(block)
            self.assertEqual(value['input']['bytes'], length)
            self.assertEqual(value['input']['sha256'], digest.hexdigest())
            self.assertGreater(len(calls), 64)

        def test_program_table_beyond_file(self):
            raw = image(); struct.pack_into('<Q', raw, 32, len(raw) - 1)
            with self.assertRaises(ValueError): self.decode(self.file(raw))

        def test_excess_program_header_count(self):
            raw = image(); struct.pack_into('<H', raw, 56, 1025)
            with self.assertRaises(ValueError): self.decode(self.file(raw))

        def test_wrong_program_header_entry_width(self):
            raw = image(); struct.pack_into('<H', raw, 54, 55)
            with self.assertRaises(ValueError): self.decode(self.file(raw))

        def test_segment_region_beyond_file(self):
            raw = image(); struct.pack_into('<Q', raw, 64 + 32, len(raw) + 1)
            with self.assertRaises(ValueError): self.decode(self.file(raw))

        def test_duplicate_dynamic_segment(self):
            with self.assertRaises(ValueError): self.decode(self.file(image(extra=[(2, 0x300, BASE + 0x300, 80)])))

        def test_duplicate_interpreter(self):
            with self.assertRaises(ValueError): self.decode(self.file(image(extra=[(3, 0x200, BASE + 0x200, len(INTERP))])))

        def test_ambiguous_file_backed_load_for_strings(self):
            with self.assertRaises(ValueError): self.decode(self.file(image(extra=[(1, 0, BASE, 4096)])))

        def test_string_virtual_address_without_backing(self):
            tags = [(5, BASE + 8192), (10, len(STRINGS)), (1, 1), (0, 0)]
            with self.assertRaises(ValueError): self.decode(self.file(image(tags=tags)))

        def test_dynamic_string_index_outside_table(self):
            tags = [(5, BASE + 0x500), (10, len(STRINGS)), (1, len(STRINGS)), (0, 0)]
            with self.assertRaises(ValueError): self.decode(self.file(image(tags=tags)))

        def test_unterminated_dynamic_table(self):
            tags = [(5, BASE + 0x500), (10, len(STRINGS)), (1, 1), (15, RPATH)]
            with self.assertRaises(ValueError): self.decode(self.file(image(tags=tags)))

        def test_unterminated_referenced_string(self):
            with self.assertRaises(ValueError): self.decode(self.file(image(strings=STRINGS[:-1])))

        def test_duplicate_search_path_tag(self):
            tags = [(5, BASE + 0x500), (10, len(STRINGS)), (15, RPATH), (15, RPATH), (0, 0)]
            with self.assertRaises(ValueError): self.decode(self.file(image(tags=tags)))

        def test_ascii_interpreter_is_required(self):
            with self.assertRaises(ValueError): self.decode(self.file(image(interpreter=b'/bad/\xff\0')))

        def test_ascii_dynamic_string_is_required(self):
            strings = STRINGS.replace(b'libfixture', b'libfixtur\xff')
            with self.assertRaises(ValueError): self.decode(self.file(image(strings=strings)))

        def test_wrong_selected_file_type(self):
            path = self.file()
            with self.assertRaises(ValueError): self.decode(path, kind=(2,))

        def test_wrong_machine_class_and_endian(self):
            for offset, value in ((18, 183), (4, 1), (5, 2)):
                raw = image(); raw[offset] = value
                with self.assertRaises(ValueError): self.decode(self.file(raw))

        def test_oversized_string_region_does_not_allocate_it(self):
            tags = [(5, BASE + 0x500), (10, 16 * MIB + 1), (1, 1), (0, 0)]
            with self.assertRaises(ValueError): self.decode(self.file(image(tags=tags)))

        def test_dynamic_region_budget_does_not_allocate_it(self):
            length = 2 * MIB; raw = image(length=length)
            struct.pack_into('<Q', raw, 64 + 2 * 56 + 32, MIB + 16)
            with self.assertRaises(ValueError): self.decode(self.file(raw, length))

        def test_ordinary_size_above_twoGiB_is_refused_before_stream(self):
            path = self.file(length=2 * GIB + 1)
            with self.assertRaisesRegex(ValueError, 'bounded ordinary ELF input required'): self.decode(path)

        def test_symlink_cannot_replace_physical_selection(self):
            path = self.file(); alias = scratch / 'alias'; self.paths.append(alias); alias.symlink_to(path.name)
            with self.assertRaises(ValueError): self.decode(alias)

        def test_nonblocking_FIFO_is_not_an_ordinary_ELF(self):
            path = scratch / 'fifo'; self.paths.append(path); os.mkfifo(path, 0o600)
            with self.assertRaises(ValueError): self.decode(path)

        def test_same_byte_named_replacement_is_detected(self):
            path = self.file(); replacement = self.file(); fired = []; calls = []
            old = path.stat()
            def live():
                guard(); calls.append(None)
                if len(calls) == 3:
                    os.replace(replacement, path); fired.append(True)
            with self.assertRaisesRegex(ValueError, 'opened or named ELF changed'): self.decode(path, live=live)
            self.assertEqual(fired, [True]); self.assertNotEqual(path.stat().st_ino, old.st_ino)

        def test_guard_cancellation_closes_held_descriptor(self):
            path = self.file(); calls = []; count = len(list(Path('/proc/self/fd').iterdir()))
            class Cancel(RuntimeError): pass
            def live():
                guard(); calls.append(None)
                if len(calls) == 3: raise Cancel('explicit owned-test cancellation')
            with self.assertRaises(Cancel): self.decode(path, live=live)
            self.assertEqual(len(list(Path('/proc/self/fd').iterdir())), count)

    return unittest.defaultTestLoader.loadTestsFromTestCase(PhysicalELF)
