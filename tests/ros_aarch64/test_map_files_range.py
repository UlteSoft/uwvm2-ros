"""Nine unexecuted range/DATA/real-filesystem regressions, not live ARM proof.

The guarded owner injects its MF-bound runtime and an exclusive owned scratch.
The last two tests use real regular-file descriptors under a modeled proc tree;
pidfd and birth facts are explicitly mocked. No process is started and no result
can count as a real kernel map, native executable, ARM load or ROS acceptance.
"""
import contextlib
import copy
import hashlib
import os
from pathlib import Path
import tempfile
import unittest
from unittest import mock


@contextlib.contextmanager
def mapped_fixture(runtime, scratch, guard):
    guard()
    with tempfile.TemporaryDirectory(prefix='map-range-', dir=scratch) as temporary:
        root = Path(temporary); proc = root/'proc'; maps = proc/'424242'/'maps'
        directory = maps.parent/'map_files'; directory.mkdir(parents=True)
        image = root/'selected-arm-policy-fixture'
        header = bytearray(64); header[:6] = b'\x7fELF\x02\x01'; header[18:20] = (183).to_bytes(2, 'little')
        body = bytes(header) + b'EXPLICIT_FS_MODEL_NOT_AN_ACTUAL_EXECUTABLE'
        image.write_bytes(body)
        # Only the kernel-style canonical basename exists. The original
        # padded maps spelling must still remain in the returned raw receipt.
        raw_range = '00400000-0050c000'
        os.link(image, directory/'400000-50c000')
        maps.write_text(raw_range+' r--p 00000000 00:00 7 /modeled/selected\n')
        state = image.stat(); target = {**runtime.mapping_file_facts(state),
            'sha256': hashlib.sha256(body).hexdigest(), 'elf_class': 2, 'elf_data': 1, 'e_machine': 183}
        host = {'device': 1, 'inode': 2, 'bytes': 64, 'mtime_ns': 3, 'ctime_ns': 4}
        identity = {'pid': 424242, 'parent_pid': 11, 'starttime_ticks': 12, 'state': 'T',
                    'membership': ['0::/MODELED_ONLY'], 'host_identity': host, 'live_cgroup_procs': True}
        original_path = runtime.Path
        def path(value):
            return proc if str(value) == '/proc' else original_path(value)
        class NonterminalModeledPidfd:
            def register(self, *unused): pass
            def poll(self, *unused): return []
        # A real opened regular FD stands in for pidfd only in this explicit
        # filesystem model; production opens the actual kernel pidfd unchanged.
        def pidfd(pid):
            if pid != 424242: raise AssertionError('unexpected modeled PID')
            return os.open(image, os.O_RDONLY)
        with mock.patch.object(runtime, 'Path', path), \
             mock.patch.object(runtime.os, 'pidfd_open', pidfd, create=True), \
             mock.patch.object(runtime.select, 'poll', NonterminalModeledPidfd):
            yield target, identity, host, raw_range
        guard()


def suite(runtime, *, scratch, guard=lambda: None):
    """Use only the already admitted runtime; do not discover project imports."""
    class RangePolicy(unittest.TestCase):
        def setUp(self): guard()

        def test_actual_driver_low_range_has_unpadded_basename(self):
            self.assertEqual(runtime.canonical_map_files_range('00400000-0050c000'), '400000-50c000')

        def test_actual_frontend_low_range_has_unpadded_basename(self):
            self.assertEqual(runtime.canonical_map_files_range('00400000-016e2000'), '400000-16e2000')

        def test_high_pie_and_uppercase_numeric_spellings(self):
            self.assertEqual(runtime.canonical_map_files_range('7f1234000000-7f1234010000'), '7f1234000000-7f1234010000')
            self.assertEqual(runtime.canonical_map_files_range('0000ABCD-0000BCDE'), 'abcd-bcde')

        def test_zero_and_native64_extent_are_bounded(self):
            self.assertEqual(runtime.canonical_map_files_range('00000000-00001000'), '0-1000')
            self.assertEqual(runtime.canonical_map_files_range('fffffffffffffffe-ffffffffffffffff'), 'fffffffffffffffe-ffffffffffffffff')

        def test_nonstring_or_nonhex_path_spellings_are_refused(self):
            for value in (None, True, 7, b'1-2', [], '', '1/2', '../1-2', '0x1-2', '+1-2', '-1-2', '1- 2', '1-2\n', '1-2-3'):
                with self.subTest(value=value), self.assertRaises(RuntimeError): runtime.canonical_map_files_range(value)

        def test_empty_equal_or_reverse_ranges_are_refused(self):
            for value in ('-2', '1-', '1-1', '2-1', '0000-0000'):
                with self.subTest(value=value), self.assertRaises(RuntimeError): runtime.canonical_map_files_range(value)

        def test_over_native64_width_is_refused_before_numeric_conversion(self):
            for value in ('0'*17+'1-2', '1-'+'f'*17, '1-'+'f'*4096):
                with self.subTest(length=len(value)), self.assertRaises(RuntimeError): runtime.canonical_map_files_range(value)

        def test_real_regular_fd_uses_canonical_name_and_keeps_raw_receipt(self):
            with mapped_fixture(runtime, scratch, guard) as (target, identity, host, raw_range):
                receipt = runtime.opened_target_mappings(424242, {'target': target}, lambda: copy.deepcopy(identity))
                row = receipt['targets']['target'][0]
                self.assertEqual(row['range'], raw_range)
                self.assertTrue(row['raw_mapping'].startswith(raw_range+' '))
                self.assertEqual(row['opened_file']['sha256'], target['sha256'])
                sample = {**identity, 'kernel_exe_elf': host, 'opened_target_mappings': receipt}
                self.assertTrue(runtime.validate_mapping_receipts(sample, 'target', target))

        def test_canonical_name_never_replaces_opened_hash_or_inode_proof(self):
            with mapped_fixture(runtime, scratch, guard) as (target, identity, host, unused):
                wrong = {**target, 'sha256': '0'*64}
                with self.assertRaises(RuntimeError):
                    runtime.opened_target_mappings(424242, {'target': wrong}, lambda: copy.deepcopy(identity))
                wrong = {**target, 'inode': target['inode']+1}
                receipt = runtime.opened_target_mappings(424242, {'target': wrong}, lambda: copy.deepcopy(identity))
                self.assertEqual(receipt['targets']['target'], [])
                sample = {**identity, 'kernel_exe_elf': host, 'opened_target_mappings': receipt}
                self.assertFalse(runtime.validate_mapping_receipts(sample, 'target', wrong))
    return unittest.defaultTestLoader.loadTestsFromTestCase(RangePolicy)
