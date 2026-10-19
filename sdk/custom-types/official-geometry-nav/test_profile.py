"""Complete official geometry/nav source policy, authored but not executed.

The actual source owner injects its admitted modules and exact immutable family
root. No fake native identities/providers are constructed. Generated Guest
source policy is independent of compiler/SDK/Gateway/delivery acceptance.
"""
from pathlib import Path
import json
import shutil
import unittest


def suite(parent, bounded, *, fixture_root, out):
    root=Path(fixture_root);scratch=Path(out)
    if not root.is_absolute() or not scratch.is_absolute() or not scratch.is_dir():
        raise ValueError('actual source root and fresh caller-owned scratch required')
    class Policy(unittest.TestCase):
        def setUp(self):
            self.work=scratch/self._testMethodName;self.work.mkdir()
        def tearDown(self):
            shutil.rmtree(self.work)
        def fixture(self,version):
            source=root/'catalogs'/('ros'+str(version));catalog=json.loads((source/'catalog.preparation.json').read_bytes())
            for row in catalog['types']:
                p=self.work/row['file'];p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((source/row['file']).read_bytes())
            return catalog
        def save(self,catalog):
            p=self.work/'catalog.json';p.write_text(json.dumps(catalog));return p
        def load(self,catalog,required=False):
            return bounded.load_catalog(parent,self.save(catalog),required)
        def test_ros1_complete_family_closure(self):
            records,_,interfaces,_=self.load(self.fixture(1))
            self.assertEqual(len(interfaces),29)
            self.assertEqual(len([i for i in interfaces if i.registered]),27)
            self.assertIn((1,'time'),records)
            self.assertEqual([m.name for m in records[(1,'nav_msgs/Odometry')].members],['header','child_frame_id','pose','twist'])
            self.assertEqual([m.name for m in records[(1,'geometry_msgs/Pose')].members],['position','orientation'])
        def test_actual_jazzy_complete_supported_closure(self):
            records,_,interfaces,_=self.load(self.fixture(2))
            self.assertEqual(len(interfaces),30)
            self.assertEqual(len([i for i in interfaces if i.registered]),27)
            self.assertIn((2,'geometry_msgs/msg/Quaternion'),records)
            self.assertIn((2,'nav_msgs/msg/Odometry'),records)
            self.assertIn((2,'geometry_msgs/msg/TwistWithCovarianceStamped'),records)
        def test_covariance_is_full_fixed_36_not_diagonal(self):
            records,_,_,_=self.load(self.fixture(1));m=records[(1,'geometry_msgs/PoseWithCovariance')].members[1]
            self.assertEqual((m.name,m.type,m.array,m.count),('covariance','float64','array',36))
        def test_projected_covariance_is_refused(self):
            catalog=self.fixture(1);p=self.work/'geometry_msgs/msg/PoseWithCovariance.msg'
            p.write_bytes(p.read_bytes().replace(b'float64[36]',b'float64[6]'))
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_transposed_native_field_order_is_refused(self):
            catalog=self.fixture(1);p=self.work/'geometry_msgs/msg/Point.msg';p.write_text('float64 y\nfloat64 x\nfloat64 z\n')
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_missing_point32_dependency_is_refused(self):
            catalog=self.fixture(2);catalog['types']=[r for r in catalog['types']if r['type']!='geometry_msgs/msg/Point32']
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_existing_twist_builtin_cannot_be_registered(self):
            catalog=self.fixture(2)
            for row in catalog['types']:
                if row['type']=='geometry_msgs/msg/Twist':row['registered']=True
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_complete_native_hashes_remain_required(self):
            with self.assertRaises(parent.GenerationError):self.load(self.fixture(2),True)
        def test_original_custom_mode_is_not_relaxed(self):
            catalog=self.fixture(1);catalog['format']='uwvm-ros-custom-bounded-v2'
            for row in catalog['types']:row.pop('registered')
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_actual_modern_quaternion_defaults_are_not_erased(self):
            p=self.work/'Quaternion.msg';p.write_bytes((root/'upstream/ros2/geometry_msgs/msg/Quaternion.msg').read_bytes())
            catalog={'format':'uwvm-ros-official-standard-bounded-v1','types':[{'ros_version':2,'type':'geometry_msgs/msg/Quaternion','file':'Quaternion.msg','registered':True}]}
            records,_,_,_=self.load(catalog)
            self.assertEqual([m.native_default for m in records[(2,'geometry_msgs/msg/Quaternion')].members],['0.0','0.0','0.0','1.0'])
        def test_unknown_standard_type_is_not_a_wildcard(self):
            p=self.work/'Unreviewed.msg';p.write_text('float64 x\n')
            catalog={'format':'uwvm-ros-official-standard-bounded-v1','types':[{'ros_version':2,'type':'nav_msgs/msg/Unreviewed','file':'Unreviewed.msg'}]}
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_bounds_cannot_project_child_frame_or_nested_points(self):
            catalog=self.fixture(1)
            for row in catalog['types']:
                if row['type']=='nav_msgs/Odometry':row['bounds'].pop('child_frame_id')
            with self.assertRaises(parent.GenerationError):self.load(catalog)
        def test_six_guest_sources_preserve_full_family(self):
            catalog=self.fixture(2);path=self.save(catalog)
            report=bounded.generate(parent,path,self.work/'generated',guest_only=True)
            self.assertFalse(report['native_registry_emitted'])
            for name in ('bounded_records.h','bounded_records.hpp','bounded_records.rs','bounded_records.mjs','bounded_records.ts','bounded_records.go'):
                text=(self.work/'generated'/name).read_text()
                self.assertIn('TwistWithCovarianceStamped',text)
                if name!='bounded_records.hpp':self.assertIn('covariance'if not name.endswith('.go')else'Covariance',text)
        def test_trusted_registry_lists_only_explicit_new_endpoint_names(self):
            for version in (1,2):
                prefix='geometry_msgs/'if version==1 else'geometry_msgs/msg/'
                self.assertIn((version,prefix+'Point'),bounded.standard_topic_names())
                self.assertNotIn((version,prefix+'Twist'),bounded.standard_topic_names())
                self.assertIn((version,prefix+'Twist'),bounded.standard_names())
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policy)
