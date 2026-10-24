#!/usr/bin/env python3
"""Authored exact-source declaration models; no generator/compiler invocation.

These future LOCAL source checks copy actual sources into private temporary
fixtures and inspect syntax/data. They make no generated file, import a producer,
execute a Guest, select an alias or prove Native behavior. Not run at authoring.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

import ast
import contextlib
import hashlib
import importlib.util
import json
import marshal
import os
from pathlib import Path
import shutil
import stat
import struct
import types
import tempfile
import unittest

SDK = _CHECKOUT_ROOT / 'sdk/origin'
def load_captured_verifier(path):
    """Load only current admitted trusted checker source; never cached pyc.

    This is the fixed verifier under test, not a catalog/operator/plugin. Its
    top level defines source checks only. No producer or model main is invoked.
    """
    limit = 2 * 1024 * 1024
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or first.st_size > limit:
            raise ValueError('bounded ordinary verifier source required')
        parts, count = [], 0
        while True:
            part = os.read(fd, min(65536, limit - count + 1))
            if not part:
                break
            count += len(part)
            if count > limit:
                raise ValueError('verifier source grew beyond bound')
            parts.append(part)
        final, named = os.fstat(fd), os.lstat(path)
        def identity(st):
            return (st.st_dev, st.st_ino, st.st_size, st.st_mode, st.st_mtime_ns, st.st_ctime_ns, st.st_nlink)
        if identity(first) != identity(final) or identity(first) != identity(named) or count != first.st_size:
            raise ValueError('verifier source FD/name/extent changed')
        raw = b''.join(parts)
    finally:
        os.close(fd)
    module = types.ModuleType('control_source_reference_verifier')
    module.__file__ = str(path)
    exec(compile(raw, str(path), 'exec'), module.__dict__)
    return module


verifier = load_captured_verifier(_sdk_path(SDK, 'tools/verify_sources.py'))


def content(path):
    raw = path.read_bytes()
    return {'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}


class ControlGeneratedReferences(unittest.TestCase):
    @contextlib.contextmanager
    def fixture(self):
        original, sdk = verifier.ROOT, verifier.SDK
        whole = json.loads((original / verifier.ADDITIONAL_GENERATED_RECIPE).read_bytes())
        contract = whole['control_managed']
        names = {verifier.ADDITIONAL_GENERATED_RECIPE}
        names.update(row['source'] for row in contract['producers'])
        names.update(row['source'] for row in contract['references'])
        names.update(row['source'] for row in contract['support_sources'])
        names.update(row['source'] for row in contract['catalog']['definitions'])
        names.update((contract['recipe']['source'], contract['catalog']['source']))
        with tempfile.TemporaryDirectory(prefix='uwvm-control-source-declarations-') as directory:
            root = Path(directory)
            for name in sorted(names):
                path = root / name
                path.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(original / name, path)
            verifier.ROOT, verifier.SDK = root, root/'sdk/origin'
            try:
                yield root, whole
            finally:
                verifier.ROOT, verifier.SDK = original, sdk

    def save(self, root, whole):
        (root / verifier.ADDITIONAL_GENERATED_RECIPE).write_text(json.dumps(whole))

    def selected(self):
        return next(iter(verifier.CONTROL_GENERATED_REFERENCES))

    def rebind_producer(self, root, whole, source):
        row = next(row for row in whole['control_managed']['producers'] if row['source'] == source)
        row['content'] = content(root / source)
        raw = (root / source).read_bytes()
        row['functions'] = {name: hashlib.sha256(verifier.generated_function_source(raw, name)).hexdigest()
                            for name in row['functions']}

    def test_bootstrap_ignores_timestamp_valid_stale_bytecode(self):
        with tempfile.TemporaryDirectory(prefix='uwvm-verifier-cache-source-') as directory:
            path = Path(directory) / 'sdk/origin/tools/verify_sources.py'
            path.parent.mkdir(parents=True)
            raw = (_sdk_path(SDK, 'tools/verify_sources.py')).read_bytes()
            path.write_bytes(raw)
            observed = path.stat()
            cache = Path(importlib.util.cache_from_source(str(path)))
            cache.parent.mkdir(parents=True)
            # A timestamp/size-valid local cache deliberately raises if a
            # SourceFileLoader executes it. No cached code is actually loaded.
            stale = compile("raise AssertionError('stale verifier cache used')\n", str(path), 'exec')
            packet = importlib.util.MAGIC_NUMBER + struct.pack('<III', 0, int(observed.st_mtime) & 0xffffffff, len(raw)) + marshal.dumps(stale)
            cache.write_bytes(packet)
            current = load_captured_verifier(path)
            self.assertEqual(current.CONTROL_GENERATED_REFERENCES, verifier.CONTROL_GENERATED_REFERENCES)
            self.assertEqual(current.generated_contract_read.__code__.co_filename, str(path))
            self.assertEqual(cache.read_bytes(), packet)
            self.assertEqual(path.read_bytes(), raw)

    def test_exact_seven_references_cover_four_contract_languages_and_real_sdk_examples(self):
        self.assertEqual(len(verifier.CONTROL_GENERATED_REFERENCES), 7)
        with self.fixture() as (root, whole):
            for (source, target), (output, _, staged) in verifier.CONTROL_GENERATED_REFERENCES.items():
                with self.subTest(source=source):
                    row = verifier.declared_control_generated_input(source, target)
                    self.assertEqual(row['generated_output'], output)
                    self.assertEqual(row['staged_product'], staged)
                    self.assertEqual(row['generator_function'], 'run')
                    self.assertEqual(row['bounded_function'], 'control_managed_products')
                    self.assertEqual(len(row['actual_producer_content']), 3)
                    self.assertIs(row['generated_input_materialized'], False)
                    self.assertEqual(row['actual_compilation'], 'REQUIRED_SEPARATELY')
                    self.assertEqual(row['Native_qualification'], 'REQUIRED_SEPARATELY')
                    self.assertEqual(row['implicit_authority'], [])
                    self.assertFalse((root / staged).exists())

    def test_actual_public_javascript_model_literal_is_exact_and_neighbors_refuse(self):
        source = 'test/0073.ros-tests/support/source/ros_javascript_owned_service/model_contract.mjs'
        target = '../../sdk/origin/custom-types/generated-control-managed/bounded_records.mjs'
        with self.fixture() as (root, whole):
            row = verifier.declared_control_generated_input(source, target)
            self.assertIsNotNone(row)
            self.assertEqual(row['generated_output'], 'bounded_records.mjs')
            self.assertFalse(row['generated_input_materialized'])
            self.assertEqual(row['actual_producer_content'].keys(), verifier.CONTROL_GENERATED_PRODUCERS.keys())
            self.assertIn((source, target), {(r['source'], r['target']) for r in
                verifier.relative_source_links()['declared_generated_build_inputs']})
            for bad_source, bad_target in ((source.replace('model_contract', 'neighbor'), target),
                    (source, './bounded_records.mjs'),
                    (source, target.replace('bounded_records', 'bounded_interfaces'))):
                self.assertIsNone(verifier.declared_control_generated_input(bad_source, bad_target))
            # A complete current source pin alone cannot admit a changed literal.
            path = root / source
            path.write_text(path.read_text().replace(target, target + '_other'))
            declaration = next(r for r in whole['control_managed']['references'] if r['source'] == source)
            declaration['source_content'] = content(path)
            self.save(root, whole)
            self.assertIsNone(verifier.declared_control_generated_input(source, target))

    def test_unknown_source_target_basename_and_rpc_facade_cannot_be_admitted(self):
        for source, target in (
            ('sdk/unrelated/control_client.ts', '../../../../custom-types/generated-control-managed/bounded_records'),
            ('sdk/origin/assemblyscript/ros2/rmw_service/examples/control_client.ts', './bounded_records'),
            ('test/0073.ros-tests/support/source/ros_original_control_managed/codec_contract.mjs', '../../sdk/origin/custom-types/generated-control-managed/bounded_interfaces.mjs'),
            ('test/0073.ros-tests/support/source/ros_original_control_managed/go/codec_contract.go', 'example.local/other/customtypes')):
            self.assertIsNone(verifier.declared_control_generated_input(source, target))

    def test_original_eleven_route_function_and_recipe_pins_are_actual(self):
        whole = json.loads((verifier.ROOT / verifier.ADDITIONAL_GENERATED_RECIPE).read_bytes())
        self.assertEqual(len(whole['references']), 11)
        self.assertEqual(whole['generator']['content'], content(verifier.ROOT / whole['generator']['producer']))
        for name, row in whole['routes'].items():
            self.assertEqual(row['recipe_content'], content(verifier.ROOT / row['recipe']))
            raw = (verifier.ROOT / row['producer']).read_bytes()
            self.assertEqual(row['function_source_sha256'], hashlib.sha256(verifier.generated_function_source(raw, row['function'])).hexdigest())
        for pair in verifier.ADDITIONAL_GENERATED_REFERENCES:
            self.assertIsNotNone(verifier.declared_additional_generated_input(*pair))

    def test_duplicate_missing_or_unknown_reference_poisons_new_closed_route(self):
        for mode in ('duplicate', 'missing', 'unknown', 'product'):
            with self.subTest(mode=mode), self.fixture() as (root, whole):
                rows = whole['control_managed']['references']
                if mode == 'duplicate': rows.append(dict(rows[0]))
                elif mode == 'missing': rows.pop()
                elif mode == 'unknown': rows[0]['source'] = 'sdk/unrelated.ts'
                else: rows[0]['output'] = 'bounded_interfaces.ts'
                self.save(root, whole)
                self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_current_consumer_source_byte_drift_is_not_silently_rebound(self):
        with self.fixture() as (root, whole):
            source, target = self.selected()
            path = root / source
            path.write_bytes(path.read_bytes() + b'\n// changed actual consumer source\n')
            self.assertIsNone(verifier.declared_control_generated_input(source, target))

    def test_rebound_source_pin_does_not_replace_actual_literal_import(self):
        with self.fixture() as (root, whole):
            source, target = self.selected()
            path = root / source
            path.write_text(path.read_text().replace(target, target + '_other'))
            next(row for row in whole['control_managed']['references'] if row['source'] == source)['source_content'] = content(path)
            self.save(root, whole)
            self.assertIsNone(verifier.declared_control_generated_input(source, target))

    def test_all_three_current_producer_hashes_are_required(self):
        for source in verifier.CONTROL_GENERATED_PRODUCERS:
            with self.subTest(source=source), self.fixture() as (root, whole):
                path = root / source
                path.write_bytes(path.read_bytes() + b'\n# unreviewed producer drift\n')
                self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_function_pin_is_not_replaced_by_whole_file_rebind(self):
        source = 'sdk/origin/rmw_wasm/default_control/managed/generate_managed.py'
        with self.fixture() as (root, whole):
            path = root / source
            path.write_text(path.read_text().replace('    templates={}', '    templates=dict()'))
            next(row for row in whole['control_managed']['producers'] if row['source'] == source)['content'] = content(path)
            self.save(root, whole)
            self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_rebound_wrong_emitter_dictionary_still_fails_actual_chain(self):
        source = 'sdk/origin/custom-types/bounded_profile.py'
        with self.fixture() as (root, whole):
            path = root / source
            raw = path.read_text()
            before = "products={'bounded_records.rs':emit_control_rust(parent,records,ordered),"
            self.assertIn(before, raw)
            path.write_text(raw.replace(before, "products={'bounded_records.rs':emit_control_go(parent,records,ordered),"))
            self.rebind_producer(root, whole, source)
            self.save(root, whole)
            self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_rebound_wrong_operator_call_cannot_use_old_cpp_or_rpc_generator(self):
        source = 'sdk/origin/rmw_wasm/default_control/managed/generate_managed.py'
        with self.fixture() as (root, whole):
            path = root / source
            path.write_text(path.read_text().replace('bounded.control_managed_products(parent,records,ordered,interfaces,pins)', 'bounded.generate(parent,records,ordered,interfaces,pins)'))
            self.rebind_producer(root, whole, source)
            self.save(root, whole)
            self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_rebound_recipe_cannot_replace_records_only_or_canonical_argv(self):
        for name in ('canonical_command', 'managed_command'):
            with self.subTest(command=name), self.fixture() as (root, whole):
                row = whole['control_managed']['recipe']
                actual = json.loads((root / row['source']).read_bytes())
                actual[name][-1] = '--invented-combined-mode'
                (root / row['source']).write_text(json.dumps(actual))
                row['content'] = content(root / row['source'])
                row[name] = actual[name]
                self.save(root, whole)
                self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_actual_original_definition_drift_and_missing_component_refuse(self):
        for mode in ('drift', 'missing'):
            with self.subTest(mode=mode), self.fixture() as (root, whole):
                rows = whole['control_managed']['catalog']['definitions']
                if mode == 'missing': rows.pop()
                else:
                    path = root / rows[0]['source']
                    path.write_bytes(path.read_bytes() + b'\n# changed original definition\n')
                self.save(root, whole)
                self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_duplicate_json_and_nonempty_authority_are_refused(self):
        with self.fixture() as (root, whole):
            whole['control_managed']['default_authority'] = ['not-a-grant']
            self.save(root, whole)
            self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))
            path = root / verifier.ADDITIONAL_GENERATED_RECIPE
            path.write_text('{"control_managed":{},"control_managed":{}}')
            self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_exponent_overflow_is_refused_in_the_complete_contract(self):
        for literal in ('1e999', '-1e999', 'NaN', 'Infinity', '-Infinity'):
            with self.subTest(literal=literal), self.fixture() as (root, whole):
                whole['control_managed']['qualification'] = '__NONFINITE_DATA__'
                raw = json.dumps(whole).replace('"__NONFINITE_DATA__"', literal)
                (root / verifier.ADDITIONAL_GENERATED_RECIPE).write_text(raw)
                self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_source_admission_refuses_fifo_and_symlink_before_read(self):
        for kind in ('fifo', 'symlink'):
            with self.subTest(kind=kind), self.fixture() as (root, whole):
                path = root / verifier.CONTROL_SOURCE_RECIPE
                copy = path.with_name('ordinary-copy.json')
                path.rename(copy)
                if kind == 'fifo': os.mkfifo(path)
                else: path.symlink_to(copy.name)
                self.assertIsNone(verifier.declared_control_generated_input(*self.selected()))

    def test_read_bound_precedes_reading_entire_oversized_source(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'too-large'
            with path.open('wb') as output:
                output.truncate(2 * 1024 * 1024 + 1)
            with self.assertRaises(ValueError): verifier.generated_contract_read(path)

    def test_actual_missing_as_rust_go_references_are_declared_in_source_scan(self):
        with self.fixture() as (root, whole):
            result = verifier.relative_source_links()
            expected = set(verifier.CONTROL_GENERATED_REFERENCES)
            observed = {(row['source'], row['target']) for row in result['declared_generated_build_inputs']}
            self.assertEqual(expected, observed & expected)
            # Existing SDK-core relative imports were deliberately not copied;
            # this fixture does not claim a complete source-link PASS.
            self.assertTrue(result['missing'])

    def test_neighbor_rust_path_and_local_go_package_remain_missing(self):
        for pair in [next(pair for pair, value in verifier.CONTROL_GENERATED_REFERENCES.items() if value[1] == syntax)
                     for syntax in ('rust-path', 'go-module')]:
            with self.subTest(pair=pair), self.fixture() as (root, whole):
                source, target = pair
                path = root / source
                path.write_text(path.read_text().replace(target, target + '_other'))
                result = verifier.relative_source_links()
                self.assertIn({'source': source, 'target': target + '_other'}, result['missing'])


if __name__ == '__main__':
    unittest.main(verbosity=2)
