"""Owned-filesystem linker evidence policies, not an actual LLD/Runtime run.

Root supplies complete-MF admitted builder/common modules and an exclusive
physical scratch directory. These fixtures write genuine ordinary ar/ELF
header data and statistics files there; they invoke no compiler/linker, Guest
or subprocess. Real selected LLD output remains a separate execution input.
"""
from pathlib import Path
import hashlib
import tempfile
import unittest


def suite(builder, common, *, scratch, guard):
    scratch = Path(scratch)
    if not scratch.is_absolute() or scratch.resolve(strict=True) != scratch or not scratch.is_dir():
        raise ValueError('Root must supply an exclusive physical scratch directory')

    class ArchiveStatistics(unittest.TestCase):
        def setUp(self):
            guard()
            self.owned = tempfile.TemporaryDirectory(prefix='archive-statistics-', dir=str(scratch))
            self.root = Path(self.owned.name)
            self.target = {'machine': 62, 'bits': 64, 'endian': 'little'}
            self.a = self.make_archive('libLLVMUsed.a', 3)
            self.b = self.make_archive('libLLVMUnused.a', 1)
            self.selected = [common.opened(self.a), common.opened(self.b)]
            self.counts = {str(self.a): 3, str(self.b): 1}
            self.offered = [str(self.a), str(self.b)]
            self.trace = {str(self.a)}
            self.stats = self.root / 'archive-statistics.tsv'

        def tearDown(self):
            self.owned.cleanup()
            guard()

        def make_archive(self, name, count, machine=62):
            body = bytearray(64)
            body[:6] = b'\x7fELF\x02\x01'
            body[16:18] = (1).to_bytes(2, 'little')
            body[18:20] = machine.to_bytes(2, 'little')
            raw = bytearray(b'!<arch>\n')
            for index in range(count):
                header = (('member%d.o/' % index).ljust(16) + '0'.ljust(12) +
                          '0'.ljust(6) + '0'.ljust(6) + '100644'.ljust(8) +
                          str(len(body)).ljust(10) + '`\n').encode('ascii')
                self.assertEqual(len(header), 60)
                raw += header + body
            path = self.root / name
            path.write_bytes(raw)
            return path

        def write(self, rows=None, header=b'members\textracted\tarchive\n'):
            if rows is None:
                rows = [(3, 2, self.a), (1, 0, self.b)]
            raw = header + ''.join('%s\t%s\t%s\n' % row for row in rows).encode()
            self.stats.write_bytes(raw)
            return raw

        def qualify(self, **overrides):
            options = dict(archives=self.selected, member_counts=self.counts, trace_paths=self.trace,
                           expected_archive_operands=self.offered, roots=[self.root], target=self.target, guard=guard)
            options.update(overrides)
            return builder.llvm_archive_statistics(common, self.stats, **options)

        def test_used_and_zero_extracted_selected_archives_have_distinct_actual_evidence(self):
            raw = self.write(); result = self.qualify()
            self.assertEqual(result['statistics'], common.opened(self.stats))
            self.assertEqual(result['statistics']['sha256'], hashlib.sha256(raw).hexdigest())
            self.assertEqual(result['selected_archive_count'], 2)
            self.assertEqual(result['selected_archives_missing_from_trace_only_zero_extracted'], [str(self.b)])
            self.assertEqual([row['extracted'] for row in result['archives']], [2, 0])

        def test_repeated_response_archive_keeps_first_total_then_zero_occurrence(self):
            self.write([(3, 2, self.a), (1, 0, self.b), (3, 0, self.a)])
            result = self.qualify(expected_archive_operands=[str(self.a), str(self.b), str(self.a)])
            self.assertEqual(result['archives'][0]['occurrences'], 2)
            self.assertEqual(result['archives'][0]['extracted'], 2)
            self.assertEqual(result['offered_archive_operands'], [str(self.a), str(self.b), str(self.a)])

        def test_every_selected_archive_must_have_a_statistics_row(self):
            self.write([(3, 2, self.a)])
            with self.assertRaises(ValueError): self.qualify()

        def test_extracted_selected_archive_without_actual_trace_is_refused(self):
            self.write([(3, 2, self.a), (1, 1, self.b)])
            with self.assertRaises(ValueError): self.qualify()

        def test_zero_extracted_selected_archive_cannot_claim_traced_members(self):
            self.write()
            with self.assertRaises(ValueError): self.qualify(trace_paths={str(self.a), str(self.b)})

        def test_wrong_or_whitespace_rewritten_header_is_refused(self):
            for header in (b'Members\textracted\tarchive\n', b'members extracted archive\n', b'members\textracted\tarchive\r\n'):
                with self.subTest(header=header):
                    self.write(header=header)
                    with self.assertRaises(ValueError): self.qualify()

        def test_malformed_negative_or_oversized_counters_are_refused(self):
            for value in ('-1', '+1', '01', '1.0', '1000000000', '1\t2'):
                with self.subTest(value=value):
                    self.write([(value, 0, self.a), (1, 0, self.b)])
                    with self.assertRaises(ValueError): self.qualify()

        def test_extracted_count_cannot_exceed_members(self):
            self.write([(3, 4, self.a), (1, 0, self.b)])
            with self.assertRaises(ValueError): self.qualify()

        def test_missing_final_newline_or_blank_extra_row_is_refused(self):
            raw = self.write()
            for invalid in (raw[:-1], raw + b'\n'):
                self.stats.write_bytes(invalid)
                with self.assertRaises(ValueError): self.qualify()

        def test_selected_archive_order_and_multiplicity_are_both_exact(self):
            for rows in ([(1, 0, self.b), (3, 2, self.a)], [(3, 2, self.a), (1, 0, self.b), (3, 0, self.a)]):
                self.write(rows)
                with self.assertRaises(ValueError): self.qualify()

        def test_repeated_row_cannot_repeat_extraction_or_change_member_count(self):
            for repeated in ((3, 1, self.a), (4, 0, self.a)):
                self.write([(3, 2, self.a), (1, 0, self.b), repeated])
                with self.assertRaises(ValueError):
                    self.qualify(expected_archive_operands=[str(self.a), str(self.b), str(self.a)])

        def test_extra_llvm_provider_is_refused_even_with_zero_extracted(self):
            for name in ('libLLVMForeign.a', 'libllvmForeign.a'):
                other = self.make_archive(name, 1)
                self.write([(3, 2, self.a), (1, 0, self.b), (1, 0, other)])
                with self.assertRaises(ValueError): self.qualify()

        def test_relative_or_dot_traversal_statistics_path_is_refused(self):
            for name in ('libLLVMUsed.a', str(self.root / '..' / self.root.name / self.a.name)):
                self.write([(3, 2, name), (1, 0, self.b)])
                with self.assertRaises(ValueError): self.qualify()

        def test_statistics_archive_symlink_is_not_a_physical_input(self):
            alias = self.root / 'libalias.a'; alias.symlink_to(self.a.name)
            self.write([(3, 2, self.a), (1, 0, self.b), (3, 0, alias)])
            with self.assertRaises(ValueError): self.qualify()

        def test_declared_member_count_must_equal_actual_native_members(self):
            self.write([(2, 1, self.a), (1, 0, self.b)])
            with self.assertRaises(ValueError): self.qualify()

        def test_stale_selected_full_seven_cannot_be_replaced_by_same_content_name(self):
            self.write(); self.a.rename(self.root / 'retained-original.a'); self.make_archive(self.a.name, 3)
            with self.assertRaises(ValueError): self.qualify()

        def test_selected_pin_unknown_fields_are_not_projected_away(self):
            self.write()
            with self.assertRaises(ValueError): self.qualify(archives=[dict(self.selected[0], mode=0o100644), self.selected[1]])

        def test_missing_or_boolean_known_member_counts_are_refused(self):
            self.write()
            for counts in ({str(self.a): 3}, {str(self.a): True, str(self.b): 1}):
                with self.assertRaises(ValueError): self.qualify(member_counts=counts)

        def test_implicit_driver_archive_has_native_full_seven_and_separate_scope(self):
            system = self.make_archive('libdriver.a', 2)
            self.write([(3, 2, self.a), (1, 0, self.b), (2, 0, system), (2, 0, system)])
            result = self.qualify(); entry = result['archives'][-1]
            self.assertEqual(entry['archive'], common.opened(system))
            self.assertEqual((entry['occurrences'], entry['extracted'], entry['scope']), (2, 0, 'implicit-driver-system'))
            self.assertTrue(entry['native_architecture_qualified'])

        def test_implicit_extracted_archive_requires_trace_and_no_traced_archive_can_be_omitted(self):
            system = self.make_archive('libdriver.a', 2)
            self.write([(3, 2, self.a), (1, 0, self.b), (2, 1, system)])
            with self.assertRaises(ValueError): self.qualify()
            self.write()
            with self.assertRaises(ValueError): self.qualify(trace_paths={str(self.a), str(system)})

        def test_empty_system_archive_has_no_native_architecture_claim(self):
            system = self.make_archive('libempty.a', 0)
            self.write([(3, 2, self.a), (1, 0, self.b), (0, 0, system)])
            entry = self.qualify()['archives'][-1]
            self.assertEqual(entry['archive']['bytes'], 8)
            self.assertFalse(entry['native_architecture_qualified'])

        def test_implicit_driver_alias_retains_raw_spelling_and_actual_canonical_pin(self):
            system = self.make_archive('libdriver.a', 2)
            alias = self.root / 'driver-alias.a'; alias.symlink_to(system.name)
            raw_name = str(self.root / '..' / self.root.name / alias.name)
            self.write([(3, 2, self.a), (1, 0, self.b), (2, 0, raw_name)])
            result = self.qualify(); row = result['rows'][-1]
            self.assertEqual((row['reported_archive'], row['archive']), (raw_name, str(system)))
            self.assertEqual(result['archives'][-1]['archive'], common.opened(system))
            self.assertTrue(any('target' in entry for entry in row['path_observation']['components']))

        def test_distinct_driver_spellings_aggregate_their_own_first_extraction_counts(self):
            system = self.make_archive('libdriver.a', 2)
            alias = self.root / 'driver-alias.a'; alias.symlink_to(system.name)
            self.write([(3, 2, self.a), (1, 0, self.b), (2, 1, system), (2, 1, alias), (2, 0, alias)])
            result = self.qualify(trace_paths={str(self.a), str(system)})
            self.assertEqual((result['archives'][-1]['extracted'], result['archives'][-1]['occurrences']), (2, 3))

        def test_empty_selected_or_nonempty_zero_member_system_is_refused(self):
            self.b.write_bytes(b'!<arch>\n'); self.selected[1] = common.opened(self.b)
            self.write([(3, 2, self.a), (0, 0, self.b)])
            with self.assertRaises(ValueError): self.qualify()
            system = self.make_archive('libdriver.a', 1)
            self.write([(3, 2, self.a), (0, 0, self.b), (0, 0, system)])
            with self.assertRaises(ValueError): self.qualify(archives=[self.selected[0]], member_counts={str(self.a): 3},
                expected_archive_operands=[str(self.a)])

        def test_foreign_native_archive_members_are_refused(self):
            system = self.make_archive('libforeign.a', 1, machine=183)
            self.write([(3, 2, self.a), (1, 0, self.b), (1, 0, system)])
            with self.assertRaises(ValueError): self.qualify()

        def test_sidecar_alias_size_and_callback_mutation_are_refused(self):
            self.write(); actual = self.root / 'original.tsv'; self.stats.rename(actual); self.stats.symlink_to(actual.name)
            with self.assertRaises(ValueError): self.qualify()
            self.stats.unlink(); self.stats.write_bytes(b'x' * (1024**2 + 1))
            with self.assertRaises(ValueError): self.qualify()
            self.write(); calls = []
            def mutation():
                guard(); calls.append(None)
                if len(calls) == 4: self.stats.write_bytes(b'members\textracted\tarchive\n')
            with self.assertRaises(ValueError): self.qualify(guard=mutation)

        def test_caller_cannot_forward_archive_statistics_output_options(self):
            for tokens in (['--print-archive-stats=/other'], ['-Wl,--print-archive-stats=/other'],
                           ['-Xlinker', '--print-archive-stats', '-Xlinker', '/other']):
                with self.assertRaises(ValueError): builder.no_extra_llvm(tokens, 'policy fixture')

    return unittest.defaultTestLoader.loadTestsFromTestCase(ArchiveStatistics)
