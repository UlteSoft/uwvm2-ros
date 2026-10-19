"""Maintained optional-codec source/real-Python cases, authored but not run.

The injected suite uses real admitted schema modules and actual Python SDK
codecs. It generates only into its caller-owned scratch. It never creates a
Backend, capability, native identity, process, compiler or native ROS fixture.
Java compilation/execution uses the separate real ManagedContract.java recipe.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import sys
import unittest
import typing


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


def load_source(name, filename):
    """Only used by an actual future suite invocation, never at module import."""
    if name in sys.modules:
        raise RuntimeError('isolated source-test module name already occupied')
    spec = importlib.util.spec_from_file_location(name, filename)
    if spec is None or spec.loader is None:
        raise RuntimeError('source loader unavailable')
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    try:
        spec.loader.exec_module(module)
    except BaseException:
        sys.modules.pop(name, None)
        raise
    return module


def suite(managed, parent, bounded, *, scratch, source_root, guard=lambda: None):
    scratch, source_root = Path(scratch).resolve(strict=True), Path(source_root).resolve(strict=True)
    if not scratch.is_dir() or scratch.is_relative_to(source_root):
        raise ValueError('owned ordinary scratch outside source required')
    guard()
    import roswasm.codecs as foundation
    if Path(foundation.__file__).resolve() != source_root / 'sdk/python/roswasm/codecs.py':
        raise ValueError('suite requires the actual selected Python SDK foundation')
    guard()
    fixture = source_root / 'tests/ros_custom_types/managed-records/fixture'

    class ManagedProfile(unittest.TestCase):
        def setUp(self):
            guard()
            self.work = scratch / self._testMethodName
            self.work.mkdir(mode=0o700)
            self.catalog = self.work / 'fixture/catalog.preparation.json'
            shutil.copytree(fixture, self.catalog.parent)
            self.modules = []

        def tearDown(self):
            for name in self.modules:
                sys.modules.pop(name, None)
            guard()
            shutil.rmtree(self.work)

        def generate(self, languages=('python', 'java'), catalog=None, name='generated'):
            guard()
            out = self.work / name
            proof = managed.generate(parent, bounded, catalog or self.catalog, out, languages=languages)
            guard()
            return out, proof

        def python(self, out):
            # Refuse an accidentally preloaded foreign SDK instead of testing
            # a stale package. Actual source/Frame admission belongs to caller.
            import roswasm.codecs as codecs
            self.assertEqual(Path(codecs.__file__).resolve(), source_root / 'sdk/python/roswasm/codecs.py')
            name = '_managed_generated_' + self._testMethodName
            self.modules.append(name)
            return load_source(name, out / 'bounded_records.py')

        def scalar(self, module):
            return module.Ros2Managed_fixtureMsgScalars(flag=True, u8=255, i8=-128,
                u16=65535, i16=-32768, u32=4294967295, i32=-2147483648,
                u64=18446744073709551615, i64=-9223372036854775808,
                f32=-0.0, f64=1.25, text='π')

        def keyword_catalog(self, field):
            root = self.work / ('keyword_' + field)
            root.mkdir()
            (root / 'Value.msg').write_text('uint8 ' + field + '\n')
            path = root / 'catalog.json'
            path.write_text(json.dumps({'format': bounded.PROFILE, 'types': [
                {'ros_version': 2, 'type': 'language_fixture/msg/Value', 'file': 'Value.msg'}]}))
            return path

        def test_resolved_IR_identities_and_bounds_are_reused(self):
            records, order, interfaces, pins = bounded.load_catalog(parent, self.catalog, False)
            _, proof = self.generate()
            self.assertEqual(proof['source_inputs'], pins)
            self.assertEqual([(r.version, r.name, r.codec_identity, r.minimum, r.maximum) for r in order],
                [(r['ros_version'], r['type'], r['adapter_codec_identity'], r['minimum_wire_bytes'], r['maximum_wire_bytes']) for r in proof['records']])
            self.assertFalse(proof['native_registry_emitted'])
            self.assertFalse(proof['historical_six_record_emitters_called'])
            self.assertEqual(proof['implicit_permissions'], [])

        def test_same_catalog_selects_separate_optional_outputs(self):
            py, pp = self.generate(('python',), name='python')
            java, jp = self.generate(('java',), name='java')
            self.assertEqual(set(pp['outputs']), {'bounded_records.py'})
            self.assertEqual(set(jp['outputs']), {'org/uwvm/roswasm/generated/BoundedRecords.java'})
            self.assertFalse((py / 'generation.json').exists())
            self.assertFalse((java / 'bounded_records.h').exists())

        def test_duplicate_unknown_or_empty_language_refuses(self):
            for index, languages in enumerate(((), ('python', 'python'), ('unimplemented',))):
                with self.assertRaises(parent.GenerationError):
                    self.generate(languages, name='bad' + str(index))
                self.assertFalse((self.work / ('bad' + str(index))).exists())

        def test_existing_revision_is_not_overwritten(self):
            out = self.work / 'generated'
            out.mkdir()
            (out / 'sentinel').write_bytes(b'original')
            with self.assertRaises(parent.GenerationError):
                self.generate()
            self.assertEqual((out / 'sentinel').read_bytes(), b'original')

        def test_all_scalars_match_independent_struct_wire(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            value = self.scalar(module)
            expected = struct.pack('<BBbHhIiQqfdI', 1,255,-128,65535,-32768,
                4294967295,-2147483648,18446744073709551615,-9223372036854775808,
                -0.0,1.25,2) + b'\xcf\x80'
            self.assertEqual(value.encode(), expected)
            decoded = type(value).decode(expected)
            self.assertEqual(decoded.u64, 18446744073709551615)
            self.assertEqual(decoded.i64, -9223372036854775808)
            self.assertEqual(struct.pack('<f', decoded.f32), b'\0\0\0\x80')

        def test_integer_widths_and_bool_types_refuse(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            for name, invalid in (('u8',256),('u16',65536),('u32',1<<32),('u64',-1),
                    ('i8',-129),('i16',32768),('i32',1<<31),('i64',1<<63),('flag',1),('u8',True)):
                value = self.scalar(module)
                setattr(value, name, invalid)
                with self.assertRaises((TypeError, ValueError, OverflowError)):
                    value.encode()

        def test_float_nonfinite_overflow_and_signed_zero(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            for name, invalid in (('f32', float('inf')), ('f32', 1e40), ('f64', float('nan'))):
                value = self.scalar(module)
                setattr(value, name, invalid)
                with self.assertRaises((ValueError, OverflowError)):
                    value.encode()
            value = self.scalar(module)
            value.f64 = -0.0
            decoded = type(value).decode(value.encode())
            self.assertEqual(struct.pack('<d', decoded.f64), b'\0\0\0\0\0\0\0\x80')

        def test_custom_UTF8_byte_bounds_NUL_and_surrogate(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            value = self.scalar(module)
            value.text = '🙂' * 4
            self.assertEqual(type(value).decode(value.encode()).text, value.text)
            for invalid in ('🙂' * 5, 'bad\0text', '\ud800'):
                value.text = invalid
                with self.assertRaises((ValueError, UnicodeError)):
                    value.encode()

        def test_full_decoder_rejects_truncation_trailing_bool_UTF8(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            value = self.scalar(module)
            wire = value.encode()
            for invalid in (wire[:-1], wire+b'\0', b'\2'+wire[1:], wire[:-2]+b'\xc0\x80'):
                with self.assertRaises((ValueError, UnicodeError)):
                    type(value).decode(invalid)

        def test_nested_fixed_and_sequence_complete_values(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            value = module.Ros2Managed_fixtureMsgOuter()
            value.inner.scalar = self.scalar(module)
            value.fixed[1].pair = [3.0, -2.0]
            value.fixed[0].tags = ['first', 'π']
            value.items = [module.Ros2Managed_fixtureMsgInner()]
            value.items[0].scalar.i64 = -7
            decoded = type(value).decode(value.encode())
            self.assertEqual(decoded.inner.scalar.u64, (1<<64)-1)
            self.assertEqual(decoded.fixed[1].pair, [3.0, -2.0])
            self.assertEqual(decoded.fixed[0].tags, ['first', 'π'])
            self.assertEqual(decoded.items[0].scalar.i64, -7)

        def test_fixed_and_dynamic_array_limits_refuse(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            value = module.Ros2Managed_fixtureMsgOuter()
            value.fixed = []
            with self.assertRaises(ValueError): value.encode()
            value = module.Ros2Managed_fixtureMsgOuter()
            value.items = [module.Ros2Managed_fixtureMsgInner() for _ in range(3)]
            with self.assertRaises(ValueError): value.encode()
            value = module.Ros2Managed_fixtureMsgOuter()
            value.inner.tags[0] = '🙂' * 3
            with self.assertRaises(ValueError): value.encode()

        def test_independent_nested_and_fixed_defaults(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            first, second = module.Ros2Managed_fixtureMsgOuter(), module.Ros2Managed_fixtureMsgOuter()
            self.assertEqual(first.rotation.w, 1.0)
            self.assertEqual(first.pose.orientation.w, 1.0)
            self.assertEqual([v.w for v in first.rotations], [1.0,1.0])
            self.assertEqual(first.dynamic, [])
            first.rotations[0].w = 0.0
            first.fixed[0].pair[0] = 9.0
            self.assertEqual(first.rotations[1].w, 1.0)
            self.assertEqual(second.rotations[0].w, 1.0)
            self.assertEqual(second.fixed[0].pair[0], 0.0)

        def test_declared_defaults_never_fill_missing_wire(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            quaternion = module.Ros2Geometry_msgsMsgQuaternion()
            quaternion.w = 0.0
            wire = quaternion.encode()
            self.assertEqual(len(wire), 32)
            self.assertEqual(type(quaternion).decode(wire).w, 0.0)
            with self.assertRaises(ValueError): type(quaternion).decode(wire[:-1])

        def test_empty_Service_and_Action_components_are_records(self):
            out, proof = self.generate(('python',))
            module = self.python(out)
            request = module.Ros2Managed_fixtureSrvProbe_Request()
            self.assertEqual(request.encode(), b'')
            self.assertIsInstance(type(request).decode(b''), type(request))
            with self.assertRaises(ValueError): type(request).decode(b'\0')
            goal = module.Ros2Managed_fixtureActionTrajectory_Goal()
            goal.step = 17
            self.assertEqual(type(goal).decode(goal.encode()).step, 17)
            self.assertFalse(proof['RPC_facades_emitted'])

        def test_ROS1_primitive_time_duration_normalization(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            value = module.Ros1Managed_fixtureTimes()
            value.stamp.nsec = 1000000000
            with self.assertRaises(ValueError): value.encode()
            value.stamp.nsec = 999999999
            value.duration_value.sec = -1
            value.duration_value.nsec = -1
            with self.assertRaises(ValueError): value.encode()
            value.duration_value.nsec = 1
            self.assertEqual(type(value).decode(value.encode()).duration_value.sec, -1)

        def test_JointState_complete_optional_equal_lengths(self):
            for version in (1,2):
                catalog = source_root / ('tests/ros_custom_types/official-standard/ros'+str(version)+'/catalog.preparation.json')
                out, _ = self.generate(('python',), catalog, 'ros'+str(version))
                # Distinct module name per actual catalog in this one case.
                name = '_jointstate_managed_' + str(version)
                self.modules.append(name)
                module = load_source(name, out/'bounded_records.py')
                cls = next(v for v in vars(module).values() if isinstance(v,type) and getattr(v,'NATIVE_TYPE',None) == ('sensor_msgs/JointState' if version==1 else 'sensor_msgs/msg/JointState'))
                value = cls();value.name=['joint'];value.position=[1.0,2.0]
                with self.assertRaises(ValueError): value.encode()
                value.position=[1.0]
                self.assertEqual(cls.decode(value.encode()).position, [1.0])

        def test_all_actual_target_catalogs_share_complete_IR(self):
            for index, relative in enumerate(CATALOGS):
                catalog = source_root / relative
                _, order, _, _ = bounded.load_catalog(parent, catalog, False)
                _, proof = self.generate(catalog=catalog, name='catalog'+str(index))
                self.assertEqual([(r.name,r.codec_identity) for r in order], [(r['type'],r['adapter_codec_identity']) for r in proof['records']])
                self.assertTrue(all(r['native_identity'] is None for r in proof['records']))

        def test_RobotMsg_all_original_fields_both_ROS_versions(self):
            for version in (1,2):
                out, _ = self.generate(('python',), source_root/('sdk/robots/xarm/catalogs/ros'+str(version)+'/catalog.preparation.json'), 'robot'+str(version))
                name = '_managed_robot_'+str(version);self.modules.append(name)
                module = load_source(name,out/'bounded_records.py')
                cls = getattr(module,'Ros'+str(version)+'Xarm_msgs'+('Msg' if version==2 else '')+'RobotMsg')
                value = cls()
                if version==1: value.header.seq=77
                value.header.stamp.sec=3
                setattr(value.header.stamp,'nsec' if version==1 else 'nanosec',4)
                value.header.frame_id='robot'
                value.state,value.mode,value.cmdnum,value.mt_brake,value.mt_able,value.err,value.warn=1,2,3,4,5,-6,7
                value.angle=[i/2 for i in range(7)];value.pose=[float(-i) for i in range(1,7)];value.offset=[float(i) for i in range(10,16)]
                header=(struct.pack('<III',77,3,4) if version==1 else struct.pack('<iI',3,4))+struct.pack('<I',5)+b'robot'
                expected=header+struct.pack('<7hI7f6f6f',1,2,3,4,5,-6,7,7,*value.angle,*value.pose,*value.offset)
                self.assertEqual(value.encode(),expected)
                decoded=cls.decode(expected)
                self.assertEqual(decoded.angle,value.angle)
                self.assertEqual(decoded.offset,value.offset)

        def test_CIOState_all_original_fields_both_ROS_versions(self):
            for version in (1,2):
                out, _ = self.generate(('python',), source_root/('sdk/robots/xarm/catalogs/cio_state/ros'+str(version)+'/catalog.preparation.json'), 'cio'+str(version))
                name = '_managed_cio_'+str(version);self.modules.append(name)
                module = load_source(name,out/'bounded_records.py')
                cls = getattr(module,'Ros'+str(version)+'Xarm_msgs'+('Msg' if version==2 else '')+'CIOState')
                value=cls()
                if version==1:value.header.seq=77
                value.header.stamp.sec=3
                setattr(value.header.stamp,'nsec' if version==1 else 'nanosec',4)
                value.header.frame_id='robot';value.state=-1;value.code=2
                value.input_digitals=[0,65535];value.output_digitals=[42,32768]
                value.input_analogs=[-0.0,1.25];value.output_analogs=[2.5,-3.5]
                value.input_conf=list(range(16));value.output_conf=list(range(240,256))
                header=(struct.pack('<III',77,3,4) if version==1 else struct.pack('<iI',3,4))+struct.pack('<I',5)+b'robot'
                expected=header+struct.pack('<hh2H2H2f2f16B16B',-1,2,*value.input_digitals,*value.output_digitals,*value.input_analogs,*value.output_analogs,*value.input_conf,*value.output_conf)
                self.assertEqual(value.encode(),expected)
                decoded=cls.decode(expected)
                self.assertEqual(decoded.output_conf,value.output_conf)
                self.assertEqual(struct.pack('<f',decoded.input_analogs[0]),b'\0\0\0\x80')

        def test_python_keyword_refuses_only_selected_language(self):
            catalog = self.keyword_catalog('except')
            with self.assertRaises(parent.GenerationError): self.generate(('python',), catalog, 'refuse')
            self.generate(('java',), catalog, 'legal_java')

        def test_java_keyword_refuses_only_selected_language(self):
            catalog = self.keyword_catalog('transient')
            with self.assertRaises(parent.GenerationError): self.generate(('java',), catalog, 'refuse')
            self.generate(('python',), catalog, 'legal_python')

        def test_ROS1_native_metadata_member_is_never_overwritten(self):
            root = self.work / 'metadata_collision'
            root.mkdir()
            (root / 'Value.msg').write_text('uint8 CODEC_IDENTITY\n')
            catalog = root / 'catalog.json'
            catalog.write_text(json.dumps({'format': bounded.PROFILE, 'types': [
                {'ros_version':1, 'type':'language_fixture/Value', 'file':'Value.msg'}]}))
            records, _, _, _ = bounded.load_catalog(parent, catalog, False)
            self.assertEqual(records[(1,'language_fixture/Value')].members[0].name, 'CODEC_IDENTITY')
            for language in ('python', 'java'):
                with self.assertRaises(parent.GenerationError):
                    self.generate((language,), catalog, 'collision_'+language)

        def test_native_list_field_cannot_replace_default_factory(self):
            root = self.work / 'class_scope'
            root.mkdir()
            (root / 'Value.msg').write_text('string list\nuint8[] values\n')
            catalog = root / 'catalog.json'
            catalog.write_text(json.dumps({'format': bounded.PROFILE, 'types': [
                {'ros_version':2, 'type':'language_fixture/msg/Value', 'file':'Value.msg',
                 'bounds':{'list':{'string':8},'values':{'sequence':2}}}]}))
            out, _ = self.generate(('python',), catalog)
            module = self.python(out)
            value = module.Ros2Language_fixtureMsgValue()
            self.assertEqual(value.list, '')
            self.assertEqual(value.values, [])
            self.assertEqual(type(value).decode(value.encode()).values, [])

        def test_float_prefix_package_is_a_nested_record(self):
            root = self.work / 'float_package'
            root.mkdir()
            (root/'Child.msg').write_text('float64 reading\n')
            (root/'Parent.msg').write_text('float_package/Child child\n')
            catalog = root/'catalog.json'
            catalog.write_text(json.dumps({'format':bounded.PROFILE, 'types':[
                {'ros_version':2,'type':'float_package/msg/Child','file':'Child.msg'},
                {'ros_version':2,'type':'language_fixture/msg/Parent','file':'Parent.msg'}]}))
            out, _ = self.generate(('python',), catalog)
            module = self.python(out)
            value = module.Ros2Language_fixtureMsgParent()
            value.child.reading = 1.25
            self.assertEqual(type(value).decode(value.encode()).child.reading, 1.25)
            (root/'LegacyChild.msg').write_text('float64 reading\n')
            (root/'LegacyParent.msg').write_text('float_package/Child Ros1Float_packageChild\n')
            legacy_catalog = root/'legacy.json'
            legacy_catalog.write_text(json.dumps({'format':bounded.PROFILE, 'types':[
                {'ros_version':1,'type':'float_package/Child','file':'LegacyChild.msg'},
                {'ros_version':1,'type':'language_fixture/Parent','file':'LegacyParent.msg'}]}))
            legacy_out, _ = self.generate(('python',), legacy_catalog, 'legacy')
            name = '_managed_dependency_annotation';self.modules.append(name)
            legacy = load_source(name, legacy_out/'bounded_records.py')
            parent_cls = legacy.Ros1Language_fixtureParent
            self.assertEqual(typing.get_type_hints(parent_cls)['Ros1Float_packageChild'], legacy.Ros1Float_packageChild)
            value = parent_cls();value.Ros1Float_packageChild.reading = 2.5
            self.assertEqual(parent_cls.decode(value.encode()).Ros1Float_packageChild.reading, 2.5)

        def test_base_encode_decode_names_remain_refused(self):
            # Actual current parser already reserves both names. No admitted
            # current-schema compatibility or renamed-field claim is made here.
            for name in ('encode', 'decode'):
                self.assertIn(name, parent.RESERVED)
                catalog = self.keyword_catalog(name)
                with self.assertRaises(parent.GenerationError):
                    self.generate(('python',), catalog, 'base_refused_' + name)
                self.assertFalse((self.work / ('base_refused_' + name)).exists())

        def test_class_scope_fields_keep_names_and_type_hints(self):
            out, _ = self.generate(('python',))
            module = self.python(out)
            cls = module.Ros2Managed_fixtureMsgClassScope
            value = cls(classmethod=17, list='a', str='π', values=[0,255],
                        record=1, sealed=2, permits=3)
            expected = (b'\x11' + struct.pack('<I',1) + b'a' +
                        struct.pack('<I',2) + b'\xcf\x80' + struct.pack('<I',2) +
                        b'\x00\xff\x01\x02\x03')
            self.assertEqual(value.encode(), expected)
            decoded = cls.decode(expected)
            self.assertEqual((decoded.classmethod, decoded.list, decoded.str), (17,'a','π'))
            self.assertEqual((decoded.record,decoded.sealed,decoded.permits), (1,2,3))
            hints = typing.get_type_hints(cls)
            self.assertEqual(hints['list'], str)
            self.assertEqual(hints['str'], str)
            self.assertEqual(hints['values'], list[int])

        def test_encode_never_reconstructs_a_decode_tree(self):
            out, _ = self.generate(('python', 'java'))
            module = self.python(out)
            cls = module.Ros2Managed_fixtureMsgScalars
            original = cls.__dict__['decode']
            original_reader = module._Reader
            def forbidden(*args):
                raise AssertionError('encoding must not allocate a decoder tree')
            cls.decode = classmethod(forbidden)
            module._Reader = forbidden
            try:
                expected = struct.pack('<BBbHhIiQqfdI', 1,255,-128,65535,-32768,
                    4294967295,-2147483648,18446744073709551615,-9223372036854775808,
                    -0.0,1.25,2) + b'\xcf\x80'
                self.assertEqual(self.scalar(module).encode(), expected)
                outer = module.Ros2Managed_fixtureMsgOuter
                self.assertEqual(len(outer().encode()), outer.MIN_WIRE_BYTES)
                invalid = self.scalar(module);invalid.u8 = 256
                with self.assertRaises((ValueError,OverflowError)):
                    invalid.encode()
            finally:
                cls.decode = original
                module._Reader = original_reader
            java = (out/'org/uwvm/roswasm/generated/BoundedRecords.java').read_text()
            self.assertNotIn('decode(wire);', java)
            self.assertIn('wire.length < MIN_WIRE_BYTES || wire.length > MAX_WIRE_BYTES', java)

        def test_fixed_i_fields_have_complete_wire_and_qualified_Java_targets(self):
            out, _ = self.generate()
            module = self.python(out)
            strings = module.Ros2Managed_fixtureMsgStringIndex()
            self.assertEqual(strings.i, ['', ''])
            strings.i = ['a', 'π']
            expected = struct.pack('<I',1)+b'a'+struct.pack('<I',2)+b'\xcf\x80'
            self.assertEqual(strings.encode(), expected)
            self.assertEqual(type(strings).decode(expected).i, ['a','π'])
            records = module.Ros2Managed_fixtureMsgRecordIndex()
            records.i[0].pair[0] = 3.0
            self.assertEqual(records.i[1].pair[0], 0.0)
            self.assertEqual(type(records).decode(records.encode()).i[0].pair[0], 3.0)
            java = (out/'org/uwvm/roswasm/generated/BoundedRecords.java').read_text()
            self.assertIn('this.i[i] = "";', java)
            self.assertIn('this.i[i] = new Ros2Managed_fixtureMsgInner();', java)

        def test_changed_catalog_refuses_before_output(self):
            original = managed.EMITTERS['python']
            def changing(*args):
                result = original(*args)
                self.catalog.write_bytes(self.catalog.read_bytes()+b' ')
                return result
            managed.EMITTERS['python'] = changing
            try:
                with self.assertRaises(parent.GenerationError): self.generate(('python',))
                self.assertFalse((self.work/'generated').exists())
            finally: managed.EMITTERS['python'] = original

    return unittest.defaultTestLoader.loadTestsFromTestCase(ManagedProfile)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--scratch', type=Path, required=True)
    args = parser.parse_args()
    source = args.source_root.resolve(strict=True)
    sys.path.insert(0, str(source/'sdk/python'))
    parent = load_source('_managed_test_parent', source/'sdk/tools/custom_types.py')
    bounded = load_source('_managed_test_bounded', source/'sdk/custom-types/bounded_profile.py')
    managed = load_source('_managed_test_generator', source/'sdk/custom-types/managed_profile.py')
    result = unittest.TextTestRunner(verbosity=2).run(suite(managed,parent,bounded,scratch=args.scratch,source_root=source))
    return 0 if result.wasSuccessful() and not result.skipped and result.testsRun==29 else 1


if __name__ == '__main__':
    sys.dont_write_bytecode = True
    raise SystemExit(main())
