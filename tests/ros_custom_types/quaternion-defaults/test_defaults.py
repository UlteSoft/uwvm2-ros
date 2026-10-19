"""Source policy plus separately injected REAL native metadata cases.

No fake accepted native identity/metadata is constructed. The source suite is
Guest-only. native_suite requires an independently admitted actual exporter
body for this exact complete Message family; mutations are negative DATA only.
Neither suite creates ROS entities, grants or compiler/process actors itself.
"""
from pathlib import Path
import copy
import hashlib
import json
import shutil
import unittest


def suite(parent,bounded,*,fixture_root,out):
    root=Path(fixture_root)/'fixture';scratch=Path(out)
    if not root.is_absolute() or not scratch.is_absolute() or not scratch.is_dir():raise ValueError('actual source and existing owned scratch required')
    class Policy(unittest.TestCase):
        def setUp(self):
            self.work=scratch/self._testMethodName;shutil.copytree(root,self.work)
            self.catalog=json.loads((self.work/'catalog.preparation.json').read_bytes())
        def tearDown(self):shutil.rmtree(self.work)
        def load(self):
            p=self.work/'catalog.json';p.write_text(json.dumps(self.catalog));return bounded.load_catalog(parent,p,False)
        def source(self):return self.work/'geometry_msgs/msg/Quaternion.msg'
        def test_complete_official_variant(self):
            records,_,_,_=self.load();q=records[(2,'geometry_msgs/msg/Quaternion')]
            self.assertEqual([m.native_default for m in q.members],['0.0','0.0','0.0','1.0'])
            self.assertTrue(q.has_native_defaults)
        def test_erased_variant_refused(self):
            self.source().write_text('float64 x\nfloat64 y\nfloat64 z\nfloat64 w\n')
            with self.assertRaises(parent.GenerationError):self.load()
        def test_partial_variant_refused(self):
            self.source().write_text('float64 x 0\nfloat64 y 0\nfloat64 z 0\nfloat64 w\n')
            with self.assertRaises(parent.GenerationError):self.load()
        def test_changed_default_refused(self):
            self.source().write_text('float64 x 0\nfloat64 y 0\nfloat64 z 0\nfloat64 w 0\n')
            with self.assertRaises(parent.GenerationError):self.load()
        def test_other_message_declared_default_refused(self):
            (self.work/'geometry_msgs/msg/Point.msg').write_text('float64 x 0\nfloat64 y\nfloat64 z\n')
            with self.assertRaises(parent.GenerationError):self.load()
        def test_nonfinite_or_unknown_literal_refused(self):
            self.source().write_text('float64 x 0\nfloat64 y 0\nfloat64 z 0\nfloat64 w nan\n')
            with self.assertRaises(parent.GenerationError):self.load()
        def test_original_custom_mode_not_relaxed(self):
            self.catalog['format']='uwvm-ros-custom-bounded-v2'
            for row in self.catalog['types']:row.pop('registered')
            with self.assertRaises(parent.GenerationError):self.load()
        def test_normalized_literal_domain(self):
            records,_,_,_=self.load();before=records[(2,'geometry_msgs/msg/Quaternion')].codec_identity
            self.source().write_text('float64 x 0.0\nfloat64 y 0.0\nfloat64 z 0.0\nfloat64 w 1.0\n')
            records,_,_,_=self.load();self.assertEqual(records[(2,'geometry_msgs/msg/Quaternion')].codec_identity,before)
        def test_no_default_canonical_domain_unchanged(self):
            records,_,_,_=self.load();r=records[(2,'geometry_msgs/msg/Point')]
            expected={'profile':'uwvm-ros-official-standard-bounded-v1','ros_version':2,'type':'geometry_msgs/msg/Point',
                'members':[{'name':name,'wire':{'scalar':'float64'}} for name in ('x','y','z')],
                'byte_order':'little-endian','booleans':'0-or-1','floats':'finite-ieee754'}
            self.assertFalse(r.has_native_defaults)
            self.assertEqual(r.codec_identity,'UWVMROS1_'+hashlib.sha256(parent.canonical_json(expected)).hexdigest())
        def test_c_and_cpp_explicit_value_initializers(self):
            records,ordered,_,_=self.load();c=bounded.emit_c(parent,records,ordered);cpp=bounded.emit_cpp(ordered)
            self.assertIn('next.w=0x1.0000000000000p+0;',c);self.assertIn('_Quaternion_default(&next.fixed[i])',c)
            self.assertIn('_Pose_default(&next.pose)',c)
            self.assertIn('static uwvm_custom::Ros2Geometry_msgsMsgQuaternion default_value()',cpp)
            self.assertNotIn('Ros2Geometry_msgsMsgPoint default_value()',cpp)
        def test_managed_and_go_declared_initialization(self):
            records,ordered,_,_=self.load()
            rust=bounded.emit_rust(parent,records,ordered);js=bounded.emit_js(parent,records,ordered)
            assembly=bounded.emit_as(parent,records,ordered);go=bounded.emit_go(parent,records,ordered)
            self.assertIn('w:f64::from_bits(0x3ff0000000000000),',rust);self.assertIn('core::array::from_fn(|_|Default::default())',rust)
            self.assertIn('Ros2Geometry_msgsMsgQuaternionCodec.defaultValue()',js)
            self.assertIn('Array.from({length:2}',js);self.assertIn('w:f64=reinterpret<f64>(u64(0x3ff0000000000000));',assembly)
            self.assertIn('DefaultRos2Geometry_msgsMsgQuaternion()',go)
            self.assertIn('value.Fixed[i]=DefaultRos2Geometry_msgsMsgQuaternion()',go)
        def test_complete_native_schema_keeps_literals(self):
            records,ordered,interfaces,_=self.load()
            # Source-only emission takes no fabricated accepted native identity.
            # A single actual record schema can be inspected without a registry.
            q=records[(2,'geometry_msgs/msg/Quaternion')]
            self.assertIn('float64 w 1.0\n',bounded.native_definition(q))
            self.assertEqual([m.native_default for m in q.members],['0.0','0.0','0.0','1.0'])
        def test_guest_only_generation_keeps_no_native_authority(self):
            p=self.work/'catalog.json';p.write_text(json.dumps(self.catalog))
            report=bounded.generate(parent,p,self.work/'generated',guest_only=True)
            self.assertFalse(report['native_registry_emitted']);self.assertEqual(report['implicit_permissions'],[])
            self.assertTrue(report['native_default_contract']['language_inventory_is_extensible'])
            self.assertFalse(report['native_default_contract']['wire_decode_fills_missing_fields'])
        def test_default_bearing_service_keeps_complete_protocol(self):
            p=self.work/'Probe.srv';p.write_text('geometry_msgs/Quaternion input\n---\ngeometry_msgs/Quaternion output\n')
            name='default_fixture/srv/Probe'
            self.catalog['types'].append({'ros_version':2,'type':name,'file':'Probe.srv'})
            records,_,interfaces,_=self.load();interface=next(i for i in interfaces if i.name==name)
            self.assertEqual(set(interface.components),{'request','response'})
            self.assertTrue(all(v.has_native_defaults for v in interface.components.values()))
            schema=dict(bounded.ros2_interface_schema(parent,records,interface))
            self.assertEqual([field[0] for field in schema[name]],['request_message','response_message','event_message'])
            self.assertEqual([field[0] for field in schema[name+'_Event']],['info','request','response'])
            self.assertIn('service_msgs/msg/ServiceEventInfo',schema);self.assertIn('builtin_interfaces/msg/Time',schema)
            self.assertEqual([field[-1] for field in schema['geometry_msgs/msg/Quaternion']],['0.0','0.0','0.0','1.0'])
            # Source acceptance supplies no native identity or complete modern
            # exporter proof. Native generation must still refuse this catalog.
            with self.assertRaises(parent.GenerationError):bounded.load_catalog(parent,self.work/'catalog.json',True)
        def test_default_bearing_action_keeps_complete_protocol(self):
            p=self.work/'Probe.action';p.write_text('geometry_msgs/Quaternion goal\n---\ngeometry_msgs/Quaternion result\n---\ngeometry_msgs/Quaternion feedback\n')
            name='default_fixture/action/Probe'
            self.catalog['types'].append({'ros_version':2,'type':name,'file':'Probe.action'})
            records,_,interfaces,_=self.load();interface=next(i for i in interfaces if i.name==name)
            self.assertEqual(set(interface.components),{'goal','result','feedback'})
            self.assertTrue(all(v.has_native_defaults for v in interface.components.values()))
            schema=dict(bounded.ros2_interface_schema(parent,records,interface))
            self.assertEqual([field[0] for field in schema[name]],['goal','result','feedback','send_goal_service','get_result_service','feedback_message'])
            for suffix in ('_SendGoal','_GetResult'):
                self.assertEqual([field[0] for field in schema[name+suffix]],['request_message','response_message','event_message'])
                self.assertEqual([field[0] for field in schema[name+suffix+'_Event']],['info','request','response'])
            self.assertIn('unique_identifier_msgs/msg/UUID',schema);self.assertIn('builtin_interfaces/msg/Time',schema)
            self.assertEqual([field[-1] for field in schema['geometry_msgs/msg/Quaternion']],['0.0','0.0','0.0','1.0'])
            with self.assertRaises(parent.GenerationError):bounded.load_catalog(parent,self.work/'catalog.json',True)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policy)


def native_suite(binder,planner,*,catalog,definitions,actual_native_metadata):
    """Caller already admits actual original exporter bytes/full7 separately.

    This positive base must be a REAL exact catalog/export, not this Guest-only
    Bundle fixture unless it also exists in an independently qualified SDK.
    Its values/identities are never filled in or fabricated by this suite.
    """
    class ActualMetadata(unittest.TestCase):
        def proof(self,metadata):
            a=binder.native_default_contract(catalog,definitions,metadata)
            b=planner.native_default_contract(catalog,definitions,metadata)
            self.assertIsNotNone(a);self.assertEqual(a,b);return a
        def test_complete_actual_metadata_and_shared_data_proof(self):
            proof=self.proof(actual_native_metadata)
            self.assertFalse(proof['native_execution_qualified']);self.assertFalse(proof['native_RIHS_binds_defaults'])
        def test_wrong_default_rejected_with_same_actual_hash(self):
            raw=copy.deepcopy(actual_native_metadata);row=raw['types']['geometry_msgs/msg/Quaternion']
            for value in (row['official'],row['components']['message']):value['root']['fields'][3]['default_value']='0.0'
            with self.assertRaises(ValueError):self.proof(raw)
        def test_erased_defaults_rejected(self):
            raw=copy.deepcopy(actual_native_metadata);row=raw['types']['geometry_msgs/msg/Quaternion']
            for value in (row['official'],row['components']['message']):
                for f in value['root']['fields']:f['default_value']=''
            with self.assertRaises(ValueError):self.proof(raw)
        def test_boolean_native_type_id_rejected(self):
            raw=copy.deepcopy(actual_native_metadata);row=raw['types']['geometry_msgs/msg/Quaternion']
            for value in (row['official'],row['components']['message']):value['root']['fields'][0]['type_id']=True
            with self.assertRaises(ValueError):self.proof(raw)
        def test_legacy_derived_provenance_cannot_attest_defaults(self):
            raw=copy.deepcopy(actual_native_metadata);row=raw['types']['geometry_msgs/msg/Quaternion']
            for value in (row['official'],row['components']['message']):
                value['identity_provenance']='LEGACY_NATIVE_INTROSPECTION_DERIVED_RIHS01';value['native_baked_hash_available']=False
            with self.assertRaises(ValueError):self.proof(raw)
        def test_complete_reference_closure_required(self):
            raw=copy.deepcopy(actual_native_metadata);rows=[r for r in raw['types'].values() if r['official']['references']]
            if not rows:raise ValueError('actual positive metadata fixture must include a nested default-bearing Message')
            for value in (rows[0]['official'],rows[0]['components']['message']):value['references'].pop()
            with self.assertRaises(ValueError):self.proof(raw)
    return unittest.defaultTestLoader.loadTestsFromTestCase(ActualMetadata)
