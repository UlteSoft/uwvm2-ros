#!/usr/bin/env python3
"""Pure include-declaration contracts; never execute a generator or compiler.

Generated native selections and bounded codecs are external build inputs. A
source-only check can verify their literal producer declarations, while actual
materialization and compilation remain independent required acceptance stages.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

import importlib.util
import contextlib
import hashlib
import json
from pathlib import Path
import shutil
import tempfile
import unittest


SDK = _CHECKOUT_ROOT / 'sdk/origin'
spec = importlib.util.spec_from_file_location('sdk_generated_source_verifier', _sdk_path(SDK, 'tools/verify_sources.py'))
verifier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verifier)


class GeneratedIncludeModels(unittest.TestCase):
    @contextlib.contextmanager
    def additional_fixture(self):
        """Copy only the exact source-only contract inputs; never run producers."""
        original = verifier.ROOT
        contract = json.loads((original / verifier.ADDITIONAL_GENERATED_RECIPE).read_text())
        names = {verifier.ADDITIONAL_GENERATED_RECIPE, contract['generator']['producer']}
        for row in contract['routes'].values():
            names.update((row['producer'], row['recipe']))
        names.update('sdk/origin/custom-types/interface_templates/bounded_interfaces.' + suffix + '.in'
                     for suffix in ('rs', 'mjs', 'go', 'ts'))
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for name in names:
                path = root / name
                path.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(original / name, path)
            verifier.ROOT = root
            try:
                yield root, contract
            finally:
                verifier.ROOT = original

    def save_contract(self, root, contract):
        (root / verifier.ADDITIONAL_GENERATED_RECIPE).write_text(json.dumps(contract))

    def declaration(self, text, name='selection.hpp'):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'producer.py'
            path.write_text(text)
            return verifier.declares_generated_product(path, 'prepare', name)

    def test_exact_five_maintained_source_contracts(self):
        self.assertEqual(len(verifier.GENERATED_INCLUDES), 5)
        for relative, name in verifier.GENERATED_INCLUDES:
            with self.subTest(source=relative, header=name):
                row = verifier.declared_generated_include(relative, name)
                self.assertEqual(row['status'], 'DECLARED_GENERATED_BUILD_INPUT')
                self.assertIs(row['generated_header_materialized'], False)
                self.assertEqual(row['actual_compilation'], 'REQUIRED_SEPARATELY')

    def test_basename_does_not_allow_an_unrelated_missing_include(self):
        self.assertIsNone(verifier.declared_generated_include('sdk/elsewhere.cpp', 'bounded_records.hpp'))
        self.assertIsNone(verifier.declared_generated_include('sdk/origin/robots/unitree/examples/direct_go2.cpp', 'selection.hpp'))
        self.assertIsNone(verifier.declared_generated_include('sdk/origin/integrations/unitree/native_identity.cpp', 'missing.hpp'))

    def test_literal_product_dictionary_and_pair_list(self):
        self.assertTrue(self.declaration("def prepare():\n products = {'selection.hpp': make_bytes()}\n"))
        self.assertTrue(self.declaration("def prepare():\n products = [('selection.hpp', make_bytes())]\n"))

    def test_comment_unrelated_function_and_dynamic_product_are_not_proofs(self):
        for text in ("# selection.hpp\ndef prepare():\n pass\n",
                     "def other():\n products = {'selection.hpp': b''}\n",
                     "def prepare():\n products = {filename: b''}\n",
                     "def prepare():\n unrelated = {'selection.hpp': b''}\n",
                     "def prepare():\n def other():\n  products = {'selection.hpp': b''}\n",
                     "def prepare():\n products = {'wrong.hpp': b''}\n"):
            with self.subTest(source=text):
                self.assertFalse(self.declaration(text))

    def test_invalid_or_missing_producer_is_refused(self):
        self.assertFalse(self.declaration('not valid Python !!!'))
        with tempfile.TemporaryDirectory() as directory:
            self.assertFalse(verifier.declares_generated_product(Path(directory) / 'absent.py', 'prepare', 'selection.hpp'))

    def test_exact_seven_typed_references_require_real_closed_recipe_and_output(self):
        self.assertEqual(len(verifier.TYPED_GENERATED_REFERENCES), 7)
        for (relative, target), output in verifier.TYPED_GENERATED_REFERENCES.items():
            with self.subTest(source=relative, target=target):
                row = verifier.declared_typed_generated_input(relative, target)
                self.assertEqual(row['generated_output'], output)
                self.assertEqual(row['generator'], 'sdk/origin/custom-types/bounded_profile.py')
                self.assertEqual(row['generator_function'], 'generate')
                self.assertIs(row['generated_input_materialized'], False)
                self.assertEqual(row['actual_compilation'], 'REQUIRED_SEPARATELY')

    def test_typed_reference_does_not_allow_other_source_or_other_module(self):
        self.assertIsNone(verifier.declared_typed_generated_input(
            'sdk/origin/robots/unitree/examples/javascript/unrelated.mjs', './bounded_records.mjs'))
        self.assertIsNone(verifier.declared_typed_generated_input(
            'sdk/origin/robots/unitree/examples/javascript/lowstate.mjs', './other.mjs'))
        self.assertIsNone(verifier.declared_typed_generated_input(
            'sdk/origin/robots/unitree/examples/assemblyscript/lowstate_go.ts', '../../../../custom-types/another/bounded_records'))

    def test_exact_eleven_additional_references_have_real_reviewed_routes(self):
        self.assertEqual(len(verifier.ADDITIONAL_GENERATED_REFERENCES), 11)
        for (source, target), (output, route) in verifier.ADDITIONAL_GENERATED_REFERENCES.items():
            with self.subTest(source=source, target=target):
                row = verifier.declared_additional_generated_input(source, target)
                self.assertEqual(row['generated_output'], output)
                self.assertEqual(row['staging_function'], verifier.ADDITIONAL_GENERATED_ROUTES[route][1])
                self.assertIs(row['generated_input_materialized'], False)
                self.assertEqual(row['actual_compilation'], 'REQUIRED_SEPARATELY')

    def test_additional_basename_and_directory_neighbors_are_not_admitted(self):
        for source, target in (
                ('sdk/unrelated/example.ts', '../generated-rpc/bounded_interfaces'),
                ('sdk/origin/custom-types/interface-examples/example.ts', '../other/bounded_records'),
                ('sdk/origin/robots/unitree/examples/legacy/unrelated.mjs', './bounded_records.mjs'),
                ('sdk/origin/robots/unitree/examples/direct_go2.c', 'bounded_records.hpp')):
            self.assertIsNone(verifier.declared_additional_generated_input(source, target))

    def test_duplicate_missing_or_unknown_contract_reference_is_refused(self):
        pair = next(iter(verifier.ADDITIONAL_GENERATED_REFERENCES))
        for mutation in ('duplicate', 'missing', 'unknown'):
            with self.subTest(mutation=mutation), self.additional_fixture() as (root, contract):
                if mutation == 'duplicate':
                    contract['references'].append(contract['references'][0])
                elif mutation == 'missing':
                    contract['references'].pop()
                else:
                    contract['references'][0]['source'] = 'sdk/unrelated.mjs'
                self.save_contract(root, contract)
                self.assertIsNone(verifier.declared_additional_generated_input(*pair))

    def test_another_producer_or_changed_staging_function_is_refused(self):
        pair = next(iter(verifier.ADDITIONAL_GENERATED_REFERENCES))
        for mutation in ('generator', 'staging'):
            with self.subTest(mutation=mutation), self.additional_fixture() as (root, contract):
                if mutation == 'generator':
                    contract['generator']['producer'] = 'sdk/origin/tools/custom_types.py'
                else:
                    contract['routes']['custom-rpc']['function'] = 'main'
                self.save_contract(root, contract)
                self.assertIsNone(verifier.declared_additional_generated_input(*pair))

    def test_compiler_recipe_byte_change_is_not_silently_rebound(self):
        pair = next(iter(verifier.ADDITIONAL_GENERATED_REFERENCES))
        with self.additional_fixture() as (root, contract):
            path = root / contract['routes']['custom-rpc']['recipe']
            path.write_text(path.read_text().replace('wasm32-unknown-unknown', 'host-native'))
            self.assertIsNone(verifier.declared_additional_generated_input(*pair))

    def test_unknown_conditional_output_or_broken_emit_chain_is_refused(self):
        pair = ('sdk/origin/custom-types/interface-examples/example.mjs', '../generated-rpc/bounded_interfaces.mjs')
        for before, after in (("products.update(facade_products)", "products.update({})"),
                              ("for extension in ('rs', 'mjs', 'go', 'ts'):", "for extension in ('rs', 'go', 'ts'):")):
            with self.subTest(change=before), self.additional_fixture() as (root, contract):
                path = root / contract['generator']['producer']
                original = path.read_text()
                self.assertIn(before, original)
                path.write_text(original.replace(before, after))
                raw = path.read_bytes()
                # Rebind the data pin deliberately, so the real emitter-chain
                # check, not merely the whole-file hash, must reject the model.
                contract['generator']['content'] = {'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}
                self.save_contract(root, contract)
                self.assertIsNone(verifier.declared_additional_generated_input(*pair))
        self.assertFalse(verifier.declares_managed_product(_sdk_path(SDK, 'custom-types/bounded_profile.py'), 'invented.mjs'))

    def test_missing_required_literal_record_output_is_refused(self):
        pair = ('sdk/origin/robots/unitree/examples/direct_go2.c', 'bounded_records.h')
        with self.additional_fixture() as (root, contract):
            path = root / contract['generator']['producer']
            original = path.read_text()
            self.assertIn("products={'bounded_records.h':", original)
            path.write_text(original.replace("products={'bounded_records.h':", "products={'unrelated.h':"))
            raw = path.read_bytes()
            contract['generator']['content'] = {'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}
            self.save_contract(root, contract)
            self.assertIsNone(verifier.declared_additional_generated_input(*pair))

    def test_engine_header_is_an_actual_source_relative_header(self):
        source = _sdk_path(SDK, 'robots/unitree/examples/javascript/control_kernel.c')
        header = '../../../../javascript/guest/engine.h'
        self.assertIn('#include "' + header + '"', source.read_text())
        self.assertEqual((source.parent / header).resolve(), _sdk_path(SDK, 'javascript/guest/engine.h'))
        self.assertTrue((source.parent / header).is_file())
        self.assertIsNone(verifier.declared_generated_include(str(source.relative_to(_CHECKOUT_ROOT)), 'engine.h'))

    def test_javascript_sdk_discovery_keeps_bootstrap_under_tests_named_owner(self):
        spec = importlib.util.spec_from_file_location('sdk_rpc_source_preparer_models',
            _sdk_path(SDK, 'custom-types/interface-examples/prepare.py'))
        preparer = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(preparer)
        with tempfile.TemporaryDirectory() as directory:
            # These are filesystem-selection DATA, not replacement SDK modules
            # or generated code. No engine/compiler or preparation runs here.
            stage = Path(directory) / 'tests/owned/source'
            base = stage / 'sdk/origin/javascript'
            for name in ('abi.mjs', 'guest/bootstrap.mjs', 'nested/tests-name.mjs',
                         'tests/private.mjs', 'nested/tests/private.mjs'):
                path = base / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text('// filesystem selection DATA only\n')
            rows = preparer.javascript_modules(stage)
            self.assertEqual([row['name'] for row in rows], [
                'sdk/origin/javascript/abi.mjs', 'sdk/origin/javascript/guest/bootstrap.mjs',
                'sdk/origin/javascript/nested/tests-name.mjs'])
            self.assertTrue(all(row['name'] == row['source'] for row in rows))


if __name__ == '__main__':
    unittest.main(verbosity=2)
