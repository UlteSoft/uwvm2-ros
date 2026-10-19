"""Injected source-policy cases for the explicit standard dependency mode.

The caller supplies the actual reviewed parent/bounded modules and original
MF-bound fixture root. No native identity or execution is invented. Temporary
source output is limited to the caller-owned out directory. These cases have
not been imported or run by their author; six actual codec builds and native
SDK/Gateway execution remain independent acceptance requirements.
"""
from pathlib import Path
import json
import shutil
import unittest


def suite(parent, bounded, *, fixture_root, out):
    root=Path(fixture_root);scratch=Path(out)
    if not root.is_absolute() or not scratch.is_absolute() or not scratch.is_dir():
        raise ValueError('explicit actual fixture source and owned scratch roots required')
    class Policy(unittest.TestCase):
        def setUp(self):
            self.work=scratch/self._testMethodName;self.work.mkdir()
        def tearDown(self):
            shutil.rmtree(self.work)
        def fixture(self,version):
            source=root/('ros'+str(version));catalog=json.loads((source/'catalog.preparation.json').read_bytes())
            for row in catalog['types']:
                target=self.work/row['file'];target.parent.mkdir(parents=True,exist_ok=True)
                target.write_bytes((source/row['file']).read_bytes())
            return catalog
        def save(self,catalog):
            path=self.work/'catalog.json';path.write_text(json.dumps(catalog));return path
        def load(self,catalog,required=False):
            return bounded.load_catalog(parent,self.save(catalog),required)
        def test_legacy_custom_mode_still_refuses_standard_namespace(self):
            catalog=self.fixture(1);catalog['format']='uwvm-ros-custom-bounded-v2'
            for row in catalog['types']:row.pop('registered')
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_legacy_custom_mode_still_refuses_time_primitive(self):
            path=self.work/'Span.msg';path.write_text('time stamp\n')
            catalog={'format':'uwvm-ros-custom-bounded-v2','types':[{'ros_version':1,'type':'policy_fixture/Span','file':'Span.msg'}]}
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_ros1_complete_header_keeps_seq_and_native_time(self):
            records,_,interfaces,_=self.load(self.fixture(1))
            self.assertEqual([f.name for f in records[(1,'std_msgs/Header')].members],['seq','stamp','frame_id'])
            self.assertEqual(records[(1,'std_msgs/Header')].members[1].type,'time')
            self.assertEqual(records[(1,'time')].native,'::ros::Time')
            self.assertEqual([m.type for m in records[(1,'time')].members],['uint32','uint32'])
            self.assertIsNone(records[(1,'time')].identity)
            time=records[(1,'time')]
            source=bounded.emit_native(parent,{(1,'time'):time},[time],[],1)
            self.assertIn('static constexpr char const* native_identity="";',source)
            self.assertNotIn('native_identity=null;',source)
            self.assertEqual([i.name for i in interfaces if i.registered],['sensor_msgs/JointState'])
        def test_ros2_complete_header_keeps_actual_time_dependency_and_no_seq(self):
            records,_,interfaces,_=self.load(self.fixture(2))
            self.assertEqual([f.name for f in records[(2,'std_msgs/msg/Header')].members],['stamp','frame_id'])
            self.assertEqual(records[(2,'std_msgs/msg/Header')].members[0].type,'builtin_interfaces/msg/Time')
            self.assertEqual([m.type for m in records[(2,'builtin_interfaces/msg/Time')].members],['int32','uint32'])
            self.assertEqual([i.name for i in interfaces if i.registered],['sensor_msgs/msg/JointState'])
        def test_native_source_emission_requires_actual_exported_identity(self):
            with self.assertRaises(parent.GenerationError):self.load(self.fixture(2),True)
        def test_missing_transitive_dependency_is_refused(self):
            catalog=self.fixture(2);catalog['types']=[row for row in catalog['types'] if row['type']!='builtin_interfaces/msg/Time']
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_joint_state_projection_is_refused(self):
            catalog=self.fixture(2);path=self.work/'sensor_msgs/msg/JointState.msg'
            path.write_text(path.read_text().replace('float64[] effort\n',''))
            catalog['types'][1]['bounds'].pop('effort')
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_official_standard_field_order_is_preserved(self):
            catalog=self.fixture(1);path=self.work/'sensor_msgs/msg/JointState.msg'
            text=path.read_text();path.write_text(text.replace('float64[] position\nfloat64[] velocity\n','float64[] velocity\nfloat64[] position\n'))
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_native_nanosecond_field_type_cannot_be_replaced(self):
            catalog=self.fixture(2);path=self.work/'builtin_interfaces/msg/Time.msg'
            path.write_text(path.read_text().replace('uint32 nanosec','uint64 nanosec'))
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_dependency_header_cannot_be_registered_as_a_topic(self):
            catalog=self.fixture(1);catalog['types'][0]['registered']=True
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_existing_builtin_adapter_cannot_be_shadowed_in_new_mode(self):
            path=self.work/'Twist.msg';path.write_text('float64 x\n')
            catalog={'format':'uwvm-ros-official-standard-bounded-v1','types':[{'ros_version':2,'type':'geometry_msgs/msg/Twist','file':'Twist.msg','registered':True}]}
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_registration_marker_is_not_a_truthy_integer(self):
            catalog=self.fixture(2);catalog['types'][1]['registered']=1
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_ros1_duration_has_actual_signed_native_fields_without_a_fake_md5(self):
            path=self.work/'Span.msg';path.write_text('duration elapsed\n')
            catalog={'format':'uwvm-ros-official-standard-bounded-v1','types':[{'ros_version':1,'type':'policy_fixture/Span','file':'Span.msg','registered':True}]}
            records,_,_,_=self.load(catalog);duration=records[(1,'duration')]
            self.assertEqual(duration.native,'::ros::Duration');self.assertIsNone(duration.identity)
            self.assertEqual([m.type for m in duration.members],['int32','int32'])
            source=bounded.emit_native(parent,{(1,'duration'):duration},[duration],[],1)
            self.assertIn('static constexpr char const* native_identity="";',source)
            self.assertNotIn('native_identity=null;',source)
        def test_ros1_definition_preserves_primitive_spelling_and_qualified_header(self):
            records,_,_,_=self.load(self.fixture(1))
            self.assertIn('time stamp\n',bounded.native_definition(records[(1,'std_msgs/Header')]))
            self.assertIn('std_msgs/Header header\n',bounded.native_definition(records[(1,'sensor_msgs/JointState')]))
        def test_guest_only_generation_emits_all_six_complete_record_sources(self):
            catalog=self.fixture(2);path=self.save(catalog)
            report=bounded.generate(parent,path,self.work/'generated',guest_only=True)
            names={'bounded_records.h','bounded_records.hpp','bounded_records.rs','bounded_records.mjs','bounded_records.ts','bounded_records.go'}
            self.assertTrue(names<=set(report['outputs']));self.assertFalse(report['native_registry_emitted'])
            self.assertEqual(report['profile'],'uwvm-ros-official-standard-bounded-v1')
            self.assertEqual(report['registered_interfaces'],['sensor_msgs/msg/JointState'])
            for name in names-{'bounded_records.hpp'}:
                text=(self.work/'generated'/name).read_text()
                self.assertIn('1000000000',text)
                for field in ('name','position','velocity','effort'):self.assertIn(field if not name.endswith('.go') else field.title(),text)
            cpp=(self.work/'generated/bounded_records.hpp').read_text()
            self.assertIn('namespace roswasmcpp {',cpp)
            self.assertIn('template<> struct Codec<uwvm_custom::Ros2Sensor_msgsMsgJointState>',cpp)
        def test_original_custom_record_identity_domain_is_unchanged(self):
            path=self.work/'Point.msg';path.write_text('float64 x\n')
            old={'format':'uwvm-ros-custom-bounded-v2','types':[{'ros_version':1,'type':'policy_fixture/Point','file':'Point.msg'}]}
            records,_,_,_=self.load(old)
            self.assertEqual(records[(1,'policy_fixture/Point')].profile,'uwvm-ros-custom-bounded-v2')
            self.assertEqual(bounded.standard_semantics(records[(1,'policy_fixture/Point')]),'')
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policy)
