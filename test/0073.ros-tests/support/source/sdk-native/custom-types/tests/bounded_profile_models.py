#!/usr/bin/env python3
"""Pure source-generation adversarial models for the finite bounded profile.

Root runs this stdlib-only suite after implementation freeze with Python -B.
Synthetic identity strings test syntax/admission contracts only; they are never
installed as native type support or substituted for an actual official export.
No compiler, library, ROS process, Wasm or remote command is launched here.
"""
from __future__ import annotations

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

import argparse
import copy
import importlib.util
import json
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
sys.dont_write_bytecode=True
ROOT=_CHECKOUT_ROOT

def load(path,name):
    spec=importlib.util.spec_from_file_location(name,path);module=importlib.util.module_from_spec(spec);sys.modules[name]=module;spec.loader.exec_module(module);return module

class ProfileModels(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.parent=load(_sdk_path(ROOT / 'sdk/origin', 'tools/custom_types.py'),'bounded_parent_models')
        cls.profile=load(_sdk_path(ROOT / 'sdk/origin', 'custom-types/bounded_profile.py'),'bounded_profile_models_owned')
        cls.composer=load(_sdk_path(ROOT / 'sdk/origin', 'custom-types/compose_profiles.py'),'bounded_composer_models_owned')
    def setUp(self):
        self.directory=tempfile.TemporaryDirectory(prefix='uwvm-bounded-pure-',dir='/tmp');self.root=Path(self.directory.name)
        self.addCleanup(self.directory.cleanup)
        source=ROOT/'test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/ros2';shutil.copytree(source,self.root/'inputs')
        self.path=self.root/'inputs/catalog.preparation.json';self.catalog=json.loads(self.path.read_text())
    def check(self):
        self.path.write_text(json.dumps(self.catalog));return self.profile.check(self.parent,self.path)
    def refuses(self):
        with self.assertRaises(self.parent.GenerationError):self.check()
    def source(self,relative,body):
        (self.root/'inputs/uwvm_ros_bounded_v2'/relative).write_text(body)
    def synthetic(self):
        for row in self.catalog['types']:
            row['native_identity']='RIHS01_'+'0'*64
            if row['file'].endswith('.srv'):row['component_identities']={r:'RIHS01_'+'0'*64 for r in ('request','response')}
            if row['file'].endswith('.action'):row['component_identities']={r:'RIHS01_'+'0'*64 for r in ('goal','result','feedback')}
        self.path.write_text(json.dumps(self.catalog))
    def test_complete_shapes_and_empty_service(self):
        proof=self.check();self.assertEqual(len(proof['types']),5)
        envelope=next(x for x in proof['types'] if x['type'].endswith('/Envelope'))['components']['message']
        self.assertEqual((envelope['minimum_wire_bytes'],envelope['maximum_wire_bytes']),(20,120))
        empty=next(x for x in proof['types'] if x['type'].endswith('/Empty'))
        self.assertTrue(all(r['minimum_wire_bytes']==r['maximum_wire_bytes']==0 for r in empty['components'].values()))
    def test_ros1_explicit_dynamic_bounds(self):
        shutil.rmtree(self.root/'inputs');shutil.copytree(ROOT/'test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/ros1',self.root/'inputs');self.catalog=json.loads(self.path.read_text());proof=self.check()
        envelope=next(x for x in proof['types'] if x['type'].endswith('/Envelope'))['components']['message'];self.assertEqual(envelope['maximum_wire_bytes'],120)
    def test_unbounded_string_refused(self):
        self.source('msg/Envelope.msg','string label\n');self.refuses()
    def test_unbounded_sequence_refused(self):
        self.source('msg/Envelope.msg','uint16[] samples\n');self.refuses()
    def test_narrower_limits_allowed_but_not_native_expansion(self):
        self.catalog['types'][1]['bounds']={'label':{'string':8},'samples':{'sequence':2}};self.check()
        self.catalog['types'][1]['bounds']['samples']['sequence']=9;self.refuses()
    def test_unused_or_wrong_kind_bound_refused(self):
        self.catalog['types'][0]['bounds']={'value':{'string':2}};self.refuses()
        self.catalog['types'][0]['bounds']={'missing':{'sequence':2}};self.refuses()
    def test_unknown_nested_type_refused(self):
        self.source('msg/Envelope.msg','Unknown unknown\n');self.refuses()
    def test_recursive_nested_type_refused(self):
        self.source('msg/Point.msg','Point value\n');self.refuses()
    def test_component_not_a_catalog_topic(self):
        row=copy.deepcopy(self.catalog['types'][0]);row.update(type='uwvm_ros_bounded_v2/msg/Echo_Request');self.catalog['types'].append(row);self.refuses()
    def test_duplicate_interface_and_member_refused(self):
        self.catalog['types'].append(copy.deepcopy(self.catalog['types'][0]));self.refuses();self.catalog['types'].pop()
        self.source('msg/Point.msg','int32 value\nint32 value\n');self.refuses()
    def test_empty_topic_refused(self):
        self.source('msg/Point.msg','');self.refuses()
    def test_defaults_constants_aliases_wide_and_idl_refused(self):
        for body in ('int32 value 7\n','int32 VALUE=7\n','wstring value\n','byte value\n','Header header\n','time stamp\n'):
            self.source('msg/Point.msg',body);self.refuses()
        self.catalog['types'][0]['file']='uwvm_ros_bounded_v2/msg/Point.idl';self.source('msg/Point.idl','module arbitrary {};');self.refuses()
    def test_bound_literal_and_expansion_ceilings(self):
        for body in ('uint64[4097] values\n','uint64[999999999999999999999999] values\n','string<=32768 text\n'):
            self.source('msg/Point.msg',body);self.refuses()
    def test_component_official_identities_required_for_native_emission(self):
        self.synthetic();self.catalog['types'][2]['component_identities'].pop('response');self.path.write_text(json.dumps(self.catalog))
        with self.assertRaises(self.parent.GenerationError):self.profile.generate(self.parent,self.path,self.root/'bad-native')
        self.assertFalse((self.root/'bad-native').exists())
    def test_canonical_fingerprint_is_bound_and_version_sensitive(self):
        before=self.check()['types'][1]['adapter_codec_identity'];self.catalog['types'][1]['bounds']={'label':{'string':31}}
        after=self.check()['types'][1]['adapter_codec_identity'];self.assertNotEqual(before,after);self.assertRegex(after,r'^UWVMROS1_[0-9a-f]{64}$')
    def test_six_guest_codecs_and_typed_servers_emit_without_native_registry(self):
        proof=self.profile.generate(self.parent,self.path,self.root/'guest',True);self.assertFalse(proof['native_registry_emitted']);self.assertEqual(len(proof['outputs']),12)
        for name in ('bounded_records.h','bounded_records.hpp','bounded_records.rs','bounded_records.go','bounded_records.mjs','bounded_records.ts'):self.assertIn(name,proof['outputs'])
        header=(self.root/'guest/bounded_interfaces.hpp').read_text();self.assertIn('uint32_t poll_once(){return process_once();}',header);self.assertIn('std::shared_ptr<Handler>',header);self.assertIn('if(busy_)return 17',header)
        with self.assertRaises(self.parent.GenerationError):self.profile.generate(self.parent,self.path,self.root/'guest',True)
    def test_native_source_outputs_and_composite_tamper_refusal(self):
        self.synthetic();proof=self.profile.generate(self.parent,self.path,self.root/'bounded')
        self.assertTrue(proof['native_registry_emitted']);self.assertEqual(len(proof['outputs']),15)
        _,bodies=self.composer.profile(self.root/'bounded','uwvm-ros-custom-bounded-v2');self.assertEqual(len(bodies),16)
        text=(self.root/'bounded/native_ros2.hpp').read_text();self.assertIn('encoded_size',text);self.assertIn('UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES',text);self.assertIn('UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES',text)
        with (self.root/'bounded/bounded_records.h').open('a') as stream:stream.write('// deliberate pure mutation\n')
        with self.assertRaises(ValueError):self.composer.profile(self.root/'bounded','uwvm-ros-custom-bounded-v2')

if __name__=='__main__':unittest.main()
