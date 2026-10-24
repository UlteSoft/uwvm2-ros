#!/usr/bin/env python3
"""Root-run pure contracts; importing official/native ROS code is not required.

Synthetic definitions below prove parser/namespace behavior, not native identity
or firmware compatibility.  The profile checks verify complete locked inventories
and explicit finite bounds without executing a generator.  Source-only authoring
does not imply these tests have run; Root retains actual test receipts separately.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

from pathlib import Path
import importlib.util
import hashlib
import json
import sys
import tempfile
import unittest

ROOT = _CHECKOUT_ROOT / 'sdk/origin'
HERE = _CHECKOUT_ROOT / 'sdk/origin/robots/unitree'


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


class SourceContracts(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.parent = load('_unitree_custom_types_test', _sdk_path(ROOT, 'tools/custom_types.py'))
        cls.bounded = load('_unitree_bounded_test', _sdk_path(ROOT, 'custom-types/bounded_profile.py'))

    def fields(self, text, version):
        return self.bounded.parse_fields(self.parent, text, version, 'robot_msgs', {}, set())

    def test_ros1_case_is_preserved(self):
        names = [f.name for f in self.fields('float32 Kp\nfloat32 Kd\nuint32[2] SN\nuint8 levelFlag\n', 1)]
        self.assertEqual(names, ['Kp', 'Kd', 'SN', 'levelFlag'])

    def test_ros2_case_remains_strict(self):
        for text in ('float32 Kp', 'uint8 levelFlag', 'uint32 SN'):
            with self.subTest(text=text), self.assertRaises(self.parent.GenerationError):
                self.fields(text, 2)

    def test_fixed_v1_parser_is_unchanged(self):
        with self.assertRaises(self.parent.GenerationError):
            self.parent.parse_msg('float32 Kp', 1, 'robot_msgs/Motor')
        self.assertEqual(self.parent.parse_msg('float32 kp', 1, 'robot_msgs/Motor')[0].name, 'kp')

    def test_native_error_member_is_not_a_go_keyword(self):
        self.assertEqual(self.fields('uint32[2] error', 2)[0].name, 'error')
        with self.assertRaises(self.parent.GenerationError):
            self.parent.field_name('error')  # fixed-v1 behavior unchanged

    def test_go_export_collisions_refuse(self):
        for text in ('float32 kp\nfloat32 Kp', 'uint8 wireSize', 'uint8 Encode'):
            with self.subTest(text=text), self.assertRaises(self.parent.GenerationError):
                self.fields(text, 1)

    def test_assemblyscript_method_collisions_refuse(self):
        for field in ('validValue', 'writeBounded', 'readBounded'):
            with self.subTest(field=field), self.assertRaises(self.parent.GenerationError):
                self.fields('uint8 ' + field, 1)

    def test_keyword_union_refuses(self):
        for name in ('class', 'goto', 'unsigned', 'unsafe', 'typeof', 'fallthrough', 'constructor'):
            with self.subTest(name=name), self.assertRaises(self.parent.GenerationError):
                self.fields('uint8 ' + name, 1)

    def test_hg_bms_state_fn_uses_only_rust_source_escape(self):
        # Exact pinned ROS2 BmsState shape, not PressSensorState. Verify the
        # fixture against the external-source lock without importing ROS or
        # assigning a fabricated native identity. The original field is fn.
        definition = ('uint8 version_high\nuint8 version_low\nuint8 fn\n'
                      'uint16[40] cell_vol\nuint32[3] bmsvoltage\nint32 current\n'
                      'uint8 soc\nuint8 soh\nint16[12] temperature\nuint16 cycle\n'
                      'uint16 manufacturer_date\nuint32[5] bmsstate\nuint32[3] reserve\n')
        lock = json.loads((_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'robots/unitree', 'upstream-lock.json')).read_text())
        native_path = 'cyclonedds_ws/src/unitree/unitree_hg/msg/BmsState.msg'
        locked = [row for row in lock['files'] if row['repository'] == 'unitreerobotics/unitree_ros2' and row['path'] == native_path]
        self.assertEqual(len(locked), 1)
        self.assertEqual((len(definition.encode()), hashlib.sha256(definition.encode()).hexdigest()),
                         (locked[0]['bytes'], locked[0]['sha256']))
        with tempfile.TemporaryDirectory() as raw:
            root = Path(raw); (root / 'BmsState.msg').write_text(definition)
            catalog = {'format': self.bounded.PROFILE, 'types': [
                {'ros_version': 2, 'type': 'unitree_hg/msg/BmsState', 'file': 'BmsState.msg'}]}
            path = root / 'catalog.json'; path.write_text(json.dumps(catalog))
            records, ordered, _, _ = self.bounded.load_catalog(self.parent, path, False)
            record = records[(2, 'unitree_hg/msg/BmsState')]
            self.assertEqual([field.name for field in record.members][2], 'fn')
            self.assertEqual(self.bounded.native_definition(record), definition)
            self.assertEqual(record.minimum, record.maximum)
            original_identity = record.codec_identity
            rust = self.bounded.emit_rust(self.parent, records, ordered)
            for spelling in ('pub r#fn:u8,', 'r#fn:Default::default(),', 'self.r#fn.to_le_bytes()', 'value.r#fn=decoded;'):
                self.assertIn(spelling, rust)
            self.assertNotIn('pub fn:u8,', rust)
            self.assertEqual(rust.count('r#fn'), 4)  # declaration/default/write/read; u8 size is constant
            self.assertEqual(record.codec_identity, original_identity)
            self.assertIn('uint8_t fn;', self.bounded.emit_c(self.parent, records, ordered))
            self.assertIn('"fn"', self.bounded.emit_js(self.parent, records, ordered))
            self.assertIn('Fn uint8', self.bounded.emit_go(self.parent, records, ordered))
            self.assertIn('fn:u8=', self.bounded.emit_as(self.parent, records, ordered))
        with self.assertRaises(self.parent.GenerationError):
            self.parent.field_name('fn')  # historical fixed-v1 refusal unchanged
        with self.assertRaises(self.parent.GenerationError):
            self.parent.parse_msg('uint8 fn', 1, 'robot_msgs/Bms')
        with self.assertRaises(self.parent.GenerationError):
            self.fields('uint8 fn\nuint8 Fn', 1)  # Go exported-name collision

    def test_rust_fn_mapping_covers_validation_and_container_paths(self):
        # Unlike the real u8 field, float/string/sequence members must access
        # their value while validating. Exercise that shared source mapping too.
        variants = [('Float', 'float32 fn\n', {}), ('Array', 'float32[2] fn\n', {}),
                    ('Text', 'string fn\n', {'fn': {'string': 4}}),
                    ('Sequence', 'uint8[] fn\n', {'fn': {'sequence': 3}})]
        with tempfile.TemporaryDirectory() as raw:
            root = Path(raw); entries = []
            for name, definition, bounds in variants:
                (root / (name + '.msg')).write_text(definition)
                entries.append({'ros_version': 2, 'type': 'robot_msgs/msg/' + name,
                                'file': name + '.msg', 'bounds': bounds})
            path = root / 'catalog.json'; path.write_text(json.dumps({'format': self.bounded.PROFILE, 'types': entries}))
            records, ordered, _, _ = self.bounded.load_catalog(self.parent, path, False)
            for record in ordered:
                rust = self.bounded.emit_rust(self.parent, records, [record])
                self.assertIn('self.r#fn', rust)
                self.assertIn('value.r#fn', rust)
                self.assertNotIn('self.fn', rust)
                self.assertNotIn('value.fn', rust)
                self.assertEqual(record.members[0].name, 'fn')

    def test_name_collision_between_records_refuses(self):
        with tempfile.TemporaryDirectory() as raw:
            root = Path(raw)
            (root / 'A.msg').write_text('uint8 value\n')
            (root / 'ACodec.msg').write_text('uint8 value\n')
            catalog = {'format': self.bounded.PROFILE, 'types': [
                {'ros_version': 1, 'type': 'robot_msgs/A', 'file': 'A.msg'},
                {'ros_version': 1, 'type': 'robot_msgs/ACodec', 'file': 'ACodec.msg'}]}
            path = root / 'catalog.json'; path.write_text(json.dumps(catalog))
            with self.assertRaises(self.parent.GenerationError):
                self.bounded.load_catalog(self.parent, path, False)

    def test_original_ceilings_are_unchanged(self):
        self.assertEqual((self.bounded.MAX_TYPES, self.bounded.MAX_NODES,
                          self.bounded.MAX_TOTAL_NODES, self.bounded.MAX_WIRE, self.bounded.MAX_DEPTH),
                         (128, 4096, 16384, 32768, 16))

    def test_profiles_have_exact_locked_complete_inventory(self):
        lock = json.loads((_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'robots/unitree', 'upstream-lock.json')).read_text())
        files = {(f['repository'], f['commit'], f['path']): f for f in lock['files']}
        counts = {'unitree_go': 24, 'unitree_hg': 11, 'unitree_api': 8, 'unitree_legged_msgs': 11}
        for family, count in counts.items():
            profile = json.loads((_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'robots/unitree', 'profiles', family + '.json')).read_text())
            self.assertEqual(profile['default_authority'], [])
            self.assertEqual(profile['native_identity'], 'REQUIRES_OFFICIAL_EXPORT')
            self.assertEqual(len(profile['definitions']), count)
            self.assertEqual(len({r['type'] for r in profile['definitions']}), count)
            for row in profile['definitions']:
                entry = files[(profile['repository'], profile['commit'], row['path'])]
                self.assertGreater(entry['bytes'], 0)
                self.assertEqual(len(entry['sha256']), 64)
                self.assertNotIn('native_identity', row)

    def test_recipe_has_all_six_languages_no_validation_claim(self):
        recipe = json.loads((_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'robots/unitree', 'recipes/six-language-codecs.json')).read_text())
        self.assertEqual(set(recipe['languages']), {'c', 'cpp', 'rust', 'javascript', 'tinygo', 'assemblyscript'})
        self.assertEqual(recipe['actual_build_and_native_tests'], 'NOT_RUN_BY_AUTHOR')

    def test_controller_recipes_match_optimized_float_contract(self):
        recipe = json.loads((_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'robots/unitree', 'recipes/compile-models.json')).read_text())
        controllers = [row for row in recipe['commands'] if row['id'].startswith('direct-') and row['id'].endswith('-compile-link')]
        self.assertEqual(len(controllers), 7)
        for row in controllers:
            self.assertTrue({'-O3', '-fno-fast-math', '-ffp-contract=off', '-MD', '-MF'} <= set(row['argv']))
            self.assertFalse({'-ffast-math', '-Ofast', '-DNDEBUG'} & set(row['argv']))

    def test_hold_family_and_denied_recipes_are_distinct(self):
        rows = {row['id']: row for row in json.loads((_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'robots/unitree', 'recipes/compile-models.json')).read_text())['commands']}
        for family, number in (('go', 1), ('hg', 2)):
            for role, enabled in (('default-denied', 0), ('positive', 1)):
                argv = rows['direct-hold-' + family + '-' + role + '-compile-link']['argv']
                self.assertIn('-DUWVM_UNITREE_HOLD_FAMILY=' + str(number), argv)
                self.assertIn('-DUWVM_UNITREE_HOLD_ENABLED=' + str(enabled), argv)

    def test_modular_and_inline_recipes_are_explicit(self):
        rows = {row['id']: row for row in json.loads((_sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'robots/unitree', 'recipes/compile-models.json')).read_text())['commands']}
        for row, enabled, modular in (('direct-go2-default-denied', 0, 0), ('direct-go2-positive', 1, 0), ('direct-go2-modular-positive', 1, 1)):
            argv = rows[row + '-compile-link']['argv']
            self.assertIn('-DUWVM_UNITREE_DIRECT_ENABLED=' + str(enabled), argv)
            self.assertIn('-DUWVM_UNITREE_MODULAR_MOTION=' + str(modular), argv)


class MaterializerContracts(unittest.TestCase):
    """Synthetic external files exercise preparation, never native ROS code."""
    @classmethod
    def setUpClass(cls):
        cls.materializer = load('_unitree_materializer_test', _sdk_path(_CHECKOUT_ROOT / 'sdk/origin', 'robots/unitree', 'materialize_catalog.py'))

    def setup_inputs(self, raw):
        root = Path(raw); upstream = root / 'external'; upstream.mkdir()
        definition = upstream / 'Motor.msg'; body = b'float32 Kp\n'
        definition.write_bytes(body)
        profile = {'format': 'uwvm-unitree-profile-v1', 'family': 'robot_msgs',
                   'repository': 'official/example', 'commit': 'a'*40, 'ros_version': 1,
                   'protocol': 'bounded-complete-topic-values', 'native_identity': 'REQUIRES_OFFICIAL_EXPORT',
                   'default_authority': [], 'sim_defaults': {'domain_id': 1, 'interface': 'lo'},
                   'definitions': [{'type': 'robot_msgs/Motor', 'path': 'Motor.msg', 'bounds': {}}]}
        lock = {'format': 'uwvm-unitree-upstream-lock-v1', 'files': [
            {'repository': 'official/example', 'commit': 'a'*40, 'path': 'Motor.msg',
             'bytes': len(body), 'sha256': hashlib.sha256(body).hexdigest()}]}
        pp, lp = root/'profile.json', root/'lock.json'
        pp.write_text(json.dumps(profile)); lp.write_text(json.dumps(lock))
        return upstream, pp, lp, root/'fresh', profile

    def test_preparation_copies_exact_bytes_without_identity(self):
        with tempfile.TemporaryDirectory() as raw:
            upstream, pp, lp, out, _ = self.setup_inputs(raw)
            result = self.materializer.prepare(upstream, pp, lp, out)
            self.assertEqual(result['status'], 'PREPARED_NOT_EXECUTED')
            self.assertTrue(result['inputs_preserved'])
            self.assertFalse(result['generators_compilers_tests_executed'])
            catalog = json.loads((out/'catalog.preparation.json').read_text())
            self.assertNotIn('native_identity', catalog['types'][0])
            self.assertEqual((out/catalog['types'][0]['file']).read_bytes(), b'float32 Kp\n')

    def test_tampered_definition_does_not_create_output(self):
        with tempfile.TemporaryDirectory() as raw:
            upstream, pp, lp, out, _ = self.setup_inputs(raw)
            (upstream/'Motor.msg').write_bytes(b'float32 kp\n')
            with self.assertRaises(ValueError): self.materializer.prepare(upstream, pp, lp, out)
            self.assertFalse(out.exists())

    def test_complete_vendor_preparation_is_explicit_and_unbound(self):
        with tempfile.TemporaryDirectory() as raw:
            upstream, pp, lp, out, _ = self.setup_inputs(raw)
            result = self.materializer.prepare(upstream, pp, lp, out,
                catalog_format=self.materializer.COMPLETE_VENDOR_CATALOG)
            catalog = json.loads((out/'catalog.preparation.json').read_text())
            self.assertEqual(catalog['format'], self.materializer.COMPLETE_VENDOR_CATALOG)
            self.assertEqual(result['catalog_format'], catalog['format'])
            self.assertEqual(result['runtime_permissions'], [])
            self.assertEqual(result['native_identity'], 'NOT_ACQUIRED')
            self.assertNotIn('native_identity', catalog['types'][0])
            self.assertEqual((out/catalog['types'][0]['file']).read_bytes(), b'float32 Kp\n')

    def test_unknown_catalog_domain_is_refused_before_writes(self):
        with tempfile.TemporaryDirectory() as raw:
            upstream, pp, lp, out, _ = self.setup_inputs(raw)
            with self.assertRaises(ValueError):
                self.materializer.prepare(upstream, pp, lp, out, catalog_format='automatic')
            self.assertFalse(out.exists())

    def test_unsafe_native_type_cannot_escape_output(self):
        with tempfile.TemporaryDirectory() as raw:
            upstream, pp, lp, out, profile = self.setup_inputs(raw)
            profile['definitions'][0]['type'] = '../Motor'; pp.write_text(json.dumps(profile))
            with self.assertRaises(ValueError): self.materializer.prepare(upstream, pp, lp, out)
            self.assertFalse(out.exists())

    def test_external_symlink_escape_is_refused(self):
        with tempfile.TemporaryDirectory() as raw:
            upstream, pp, lp, out, _ = self.setup_inputs(raw)
            target = Path(raw)/'outside.msg'; target.write_bytes(b'float32 Kp\n')
            (upstream/'Motor.msg').unlink(); (upstream/'Motor.msg').symlink_to(target)
            with self.assertRaises(ValueError): self.materializer.prepare(upstream, pp, lp, out)
            self.assertFalse(out.exists())

    def test_existing_output_is_not_overwritten(self):
        with tempfile.TemporaryDirectory() as raw:
            upstream, pp, lp, out, _ = self.setup_inputs(raw)
            out.mkdir(); (out/'sentinel').write_text('preserved')
            with self.assertRaises(ValueError): self.materializer.prepare(upstream, pp, lp, out)
            self.assertEqual((out/'sentinel').read_text(), 'preserved')

    def test_preparation_cannot_add_default_grants(self):
        with tempfile.TemporaryDirectory() as raw:
            upstream, pp, lp, out, profile = self.setup_inputs(raw)
            profile['default_authority'] = ['topic.publish']; pp.write_text(json.dumps(profile))
            with self.assertRaises(ValueError): self.materializer.prepare(upstream, pp, lp, out)
            self.assertFalse(out.exists())

    def test_duplicate_json_keys_are_refused(self):
        with self.assertRaises(ValueError):
            self.materializer.document(b'{"format":1,"format":2}')


if __name__ == '__main__':
    unittest.main()
