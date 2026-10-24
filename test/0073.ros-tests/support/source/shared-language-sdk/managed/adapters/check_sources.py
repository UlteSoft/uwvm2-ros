#!/usr/bin/env python3
"""Finite source-generation regression cases, with no compiler/Guest credit.

The caller admits this source closure and invokes it inside resource_owner.py.
Real original catalog files are copied into the unique run's input tree; no
native identity or simulated ROS import is manufactured by this suite.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import unittest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    sdk = args.source.resolve() / 'sdk/origin'
    spec = importlib.util.spec_from_file_location('_owned_adapters', sdk / 'language/managed/adapters.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    args.out.mkdir(exist_ok=False)

    class Sources(unittest.TestCase):
        def test_complete_fixture_and_all_roles(self):
            proof = module.generate(args.source / 'test/0073.ros-tests/support/source/ros_custom_types/managed-records/fixture/catalog.preparation.json', args.out / 'fixture', module.LANGUAGES)
            self.assertEqual(set(proof['foundations']), {'java', 'csharp'})
            self.assertEqual(proof['default_authority'], [])
            self.assertFalse(proof['native_registry_emitted'])
            self.assertTrue(all(row['native_identity'] is None for row in proof['records']))
            scalars = next(row for row in proof['records'] if row['native_type'] == 'managed_fixture/msg/Scalars')
            self.assertEqual({field['type'] for field in scalars['fields']}, set(module.SCALARS) | {'string'})
            for language in module.LANGUAGES:
                self.assertTrue((args.out / 'fixture' / module.FILES[language]).is_file())
            roles = {row['native_type'].rsplit('_', 1)[-1] for row in proof['records']}
            self.assertTrue({'Request', 'Response', 'Goal', 'Result', 'Feedback'} <= roles)

        def test_actual_upstream_scalar_defaults(self):
            proof = module.generate(sdk / 'custom-types/scalar-defaults/catalog/catalog.preparation.json', args.out / 'defaults', module.LANGUAGES)
            request = next(row for row in proof['records'] if row['native_type'].endswith('_Request'))
            defaults = {f['name']: f['native_default'] for f in request['fields'] if f['native_default'] is not None}
            self.assertEqual(defaults, {'modbus_length': '0', 'host_id': '9',
                'is_transparent_transmission': 'False', 'use_503_port': 'False'})

        def test_unknown_and_duplicate_language_refuse_before_write(self):
            for index, languages in enumerate((('unknown',), ('kotlin', 'kotlin'), ())):
                out = args.out / ('refused-language-' + str(index))
                with self.assertRaises(ValueError):
                    module.generate(Path('/absent/catalog'), out, languages)
                self.assertFalse(out.exists())

        def test_existing_output_refuses_without_mutation(self):
            out = args.out / 'existing'
            out.mkdir()
            (out / 'keep').write_bytes(b'original')
            with self.assertRaises(ValueError):
                module.generate(sdk / 'custom-types/scalar-defaults/catalog/catalog.preparation.json', out, ('kotlin',))
            self.assertEqual((out / 'keep').read_bytes(), b'original')

        def test_missing_native_identity_refuses_before_output(self):
            out = args.out / 'refused-native'
            with self.assertRaises(Exception) as caught:
                module.generate(sdk / 'custom-types/scalar-defaults/catalog/catalog.preparation.json', out, ('kotlin',), True)
            self.assertIn('identit', str(caught.exception).lower())
            self.assertFalse(out.exists())

        def test_case_collision_refuses_before_write(self):
            source = args.out / 'case-collision'
            source.mkdir()
            (source / 'Value.msg').write_text('uint8 field\nuint8 Field\n')
            catalog = source / 'catalog.json'
            catalog.write_text(json.dumps({'format': 'uwvm-ros-custom-bounded-v2', 'types': [
                {'ros_version': 1, 'type': 'collision_fixture/Value', 'file': 'Value.msg'}]}))
            out = args.out / 'refused-case'
            with self.assertRaises(Exception) as caught:
                module.generate(catalog, out, ('vbnet',))
            # Today's authoritative parser already refuses this collision in
            # its mandatory Go namespace. A future less restrictive parser
            # must still reach the explicit VB.NET case-insensitive guard.
            self.assertIn('colli', str(caught.exception))
            self.assertFalse(out.exists())

    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(Sources))
    report = {'kind': 'managed-adapter-source-regressions-v1', 'tests': result.testsRun,
              'failures': len(result.failures), 'errors': len(result.errors),
              'wasm_execution': 'NOT_RUN', 'native_ros': 'NOT_RUN', 'default_authority': []}
    (args.out / 'source-results.json').write_text(json.dumps(report, indent=2) + '\n')
    return 0 if result.wasSuccessful() else 1


if __name__ == '__main__':
    raise SystemExit(main())
