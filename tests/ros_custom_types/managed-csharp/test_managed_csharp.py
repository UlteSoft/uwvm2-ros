"""Injected source-generation policies; never executes a C# compiler or ROS.

Actual C# semantics are tested separately by RecordContract/NamesContract and
six compile-time FamilyContract roles. A future caller admits the real modules
and SDK source root, supplies private scratch and owns any process/Frame limits.
Nothing is loaded, generated or tested merely by importing this suite source.
"""
from pathlib import Path
import hashlib
import json
import shutil
import sys
import types
import unittest


CATALOGS = (
    'tests/ros_custom_types/official-standard/ros1/catalog.preparation.json',
    'tests/ros_custom_types/official-standard/ros2/catalog.preparation.json',
    'sdk/robots/xarm/catalogs/ros1/catalog.preparation.json',
    'sdk/robots/xarm/catalogs/ros2/catalog.preparation.json',
    'sdk/robots/xarm/catalogs/cio_state/ros1/catalog.preparation.json',
    'sdk/robots/xarm/catalogs/cio_state/ros2/catalog.preparation.json',
    'sdk/custom-types/official-geometry-nav/catalogs/ros2/catalog.preparation.json',
    'tests/ros_custom_types/quaternion-defaults/fixture/catalog.preparation.json',
)


def suite(managed, csharp, parent, bounded, *, scratch, source_root, guard=lambda: None):
    scratch = Path(scratch).resolve(strict=True)
    source_root = Path(source_root).resolve(strict=True)
    if not scratch.is_dir() or scratch.is_relative_to(source_root):
        raise ValueError('private owned scratch outside selected source required')
    for module, relative in ((managed, 'sdk/custom-types/managed_profile.py'),
            (csharp, 'sdk/custom-types/managed_csharp.py'),
            (parent, 'sdk/tools/custom_types.py'),
            (bounded, 'sdk/custom-types/bounded_profile.py')):
        if Path(module.__file__).resolve() != source_root / relative:
            raise ValueError('suite requires the actual coherent selected source modules')
    guard()

    class ManagedCsharp(unittest.TestCase):
        def setUp(self):
            guard()
            self.work = scratch / self._testMethodName
            self.work.mkdir(mode=0o700)
            self.catalog = self.work / 'fixture/catalog.preparation.json'
            shutil.copytree(source_root / 'tests/ros_custom_types/managed-records/fixture', self.catalog.parent)

        def tearDown(self):
            guard()
            shutil.rmtree(self.work)

        def generate(self, languages=('csharp',), catalog=None, name='generated'):
            guard()
            out = self.work / name
            proof = managed.generate(parent, bounded, catalog or self.catalog, out, languages=languages)
            guard()
            return out, proof

        def test_actual_IR_identity_bounds_defaults_are_reused(self):
            records, ordered, interfaces, pins = bounded.load_catalog(parent, self.catalog, False)
            out, proof = self.generate()
            self.assertEqual(proof['source_inputs'], pins)
            self.assertEqual([(r.version, r.name, r.codec_identity, r.minimum, r.maximum) for r in ordered],
                [(r['ros_version'], r['type'], r['adapter_codec_identity'], r['minimum_wire_bytes'], r['maximum_wire_bytes']) for r in proof['records']])
            self.assertEqual(proof['csharp_source_types'], csharp.source_types(ordered))
            quaternion = next(r for r in proof['records'] if r['ros_version'] == 2 and r['type'] == 'geometry_msgs/msg/Quaternion')
            self.assertEqual(quaternion['native_defaults'], {'x':'0.0', 'y':'0.0', 'z':'0.0', 'w':'1.0'})
            self.assertEqual(set(proof['outputs']), {'csharp/BoundedRecords.cs'})
            self.assertTrue((out / 'generation.managed.json').is_file())

        def test_python_java_source_products_unchanged_when_csharp_selected(self):
            a, pa = self.generate(('python', 'java'), name='original-two')
            b, pb = self.generate(('python', 'java', 'csharp'), name='with-csharp')
            self.assertEqual(pa['records'], pb['records'])
            for relative in ('bounded_records.py', 'org/uwvm/roswasm/generated/BoundedRecords.java'):
                self.assertEqual((a / relative).read_bytes(), (b / relative).read_bytes())
            self.assertNotIn('csharp_source_types', pa)

        def test_exact_native_keyword_symbol_and_metadata_fields_supported(self):
            catalog = source_root / 'tests/ros_custom_types/managed-csharp/names/catalog.preparation.json'
            out, proof = self.generate(catalog=catalog)
            body = (out / 'csharp/BoundedRecords.cs').read_text()
            for name in ('event','checked','lock','out','params','ulong','Ros1Csharp_fixtureValue','CODEC_IDENTITY'):
                self.assertIn(' @' + name + ' = ', body)
            self.assertEqual(proof['csharp_source_types']['Ros1Csharp_fixtureValue']['value'], '_Ros1Csharp_fixtureValue')
            self.assertEqual(proof['records'][0]['maximum_wire_bytes'], 8)

        def test_csharp_field_acceptance_does_not_relax_other_emitters(self):
            catalog = source_root / 'tests/ros_custom_types/managed-csharp/names/catalog.preparation.json'
            for selected in (('python',), ('java',), ('csharp','python'), ('csharp','java')):
                destination = self.work / ('refused-' + '-'.join(selected))
                with self.assertRaises(parent.GenerationError):
                    managed.generate(parent, bounded, catalog, destination, languages=selected)
                self.assertFalse(destination.exists())

        def test_actual_Csharp_foundation_and_emitter_bytes_are_pinned(self):
            out, proof = self.generate()
            for role, relative in (('csharp_emitter','sdk/custom-types/managed_csharp.py'),
                    ('csharp_codec_interface','sdk/csharp/RosWasm/Result.cs'),
                    ('csharp_codecs','sdk/csharp/RosWasm/Codecs.cs')):
                raw = (source_root / relative).read_bytes()
                self.assertEqual(proof['generator_and_foundation_sources'][role], {
                    'path':str(source_root / relative), 'bytes':len(raw), 'sha256':hashlib.sha256(raw).hexdigest()})

        def test_all_eight_actual_catalogs_emit_complete_record_sets(self):
            for index, relative in enumerate(CATALOGS):
                catalog = source_root / relative
                _, ordered, _, _ = bounded.load_catalog(parent, catalog, False)
                out, proof = self.generate(catalog=catalog, name='catalog-' + str(index))
                body = (out / 'csharp/BoundedRecords.cs').read_text()
                self.assertEqual(len(proof['records']), len(ordered))
                for record in ordered:
                    self.assertIn('public sealed class _' + record.symbol + ' {', body)
                    self.assertIn('public sealed class ' + record.symbol + 'Codec :', body)
                self.assertTrue(all(r['native_identity'] is None for r in proof['records']))
                self.assertFalse(proof['native_registry_emitted'])
                self.assertFalse(proof['historical_six_record_emitters_called'])
                self.assertEqual(proof['implicit_permissions'], [])

        def test_repeated_generation_retires_only_its_private_module(self):
            self.assertNotIn('_uwvm_optional_managed_csharp', sys.modules)
            a, _ = self.generate(name='first')
            self.assertNotIn('_uwvm_optional_managed_csharp', sys.modules)
            b, _ = self.generate(name='second')
            self.assertNotIn('_uwvm_optional_managed_csharp', sys.modules)
            self.assertEqual((a / 'csharp/BoundedRecords.cs').read_bytes(), (b / 'csharp/BoundedRecords.cs').read_bytes())

        def test_foreign_private_module_is_not_overwritten(self):
            key = '_uwvm_optional_managed_csharp'
            self.assertNotIn(key, sys.modules)
            foreign = types.ModuleType(key)
            sys.modules[key] = foreign
            try:
                with self.assertRaisesRegex(RuntimeError, 'already occupied'):
                    self.generate()
                self.assertIs(sys.modules[key], foreign)
                self.assertFalse((self.work / 'generated').exists())
            finally:
                if sys.modules.get(key) is foreign:
                    del sys.modules[key]

        def test_existing_output_is_never_overwritten(self):
            out = self.work / 'generated'
            out.mkdir()
            (out / 'owned.txt').write_text('retain exact bytes')
            with self.assertRaises(parent.GenerationError):
                self.generate()
            self.assertEqual((out / 'owned.txt').read_text(), 'retain exact bytes')
            self.assertEqual(len(list(out.iterdir())), 1)

        def test_native_identity_required_is_not_fabricated(self):
            out = self.work / 'required-native'
            with self.assertRaises(parent.GenerationError):
                managed.generate(parent, bounded, self.catalog, out, languages=('csharp',), require_native_identity=True)
            self.assertFalse(out.exists())

        def test_unknown_duplicate_language_refuses_before_outputs(self):
            for index, selected in enumerate((('not-a-language',), ('csharp','csharp'))):
                out = self.work / ('bad-' + str(index))
                with self.assertRaises(parent.GenerationError):
                    managed.generate(parent, bounded, self.catalog, out, languages=selected)
                self.assertFalse(out.exists())

        def test_real_typed_API_no_reflection_or_encode_full_decode(self):
            out, _ = self.generate()
            body = (out / 'csharp/BoundedRecords.cs').read_text()
            self.assertIn('global::Uwvm.RosWasm.ICodec<', body)
            self.assertIn('unchecked((uint)BitConverter.SingleToInt32Bits(value))', body)
            self.assertIn('OwnedWire(bytes, MIN_WIRE_BYTES, MAX_WIRE_BYTES)', body)
            self.assertIn('this.@i[index]', body)
            self.assertNotIn('System.Reflection', body)
            self.assertNotIn('Decode(wire)', body)
            self.assertNotIn('NativeTransport', body)
            self.assertIn('nonempty JointState arrays must have equal length', csharp.RUNTIME + (source_root / 'sdk/custom-types/managed_csharp.py').read_text())

    return unittest.defaultTestLoader.loadTestsFromTestCase(ManagedCsharp)
