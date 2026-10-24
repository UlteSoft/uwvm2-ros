#!/usr/bin/env python3
"""Pure source-emission regressions; run only under Root's bounded test owner.

This suite imports the actual selected generator only when explicitly executed.
Its synthetic native identities exercise syntax, never native ROS qualification.
It starts no compiler, Runtime, native ROS process, socket or thread. All outputs
and deliberately modified template copies live in fresh /tmp directories.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

from pathlib import Path
import hashlib
import importlib.util
import json
import shutil
import sys
import tempfile
import unittest

ROOT=_CHECKOUT_ROOT
sys.dont_write_bytecode=True

def load(path,name):
    spec=importlib.util.spec_from_file_location(name,path)
    module=importlib.util.module_from_spec(spec)
    sys.modules[name]=module
    spec.loader.exec_module(module)
    return module

class ManagedFacadeModels(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.parent=load(_sdk_path(ROOT / 'sdk/origin', 'tools/custom_types.py'),'custom_rpc_parent_models')
        cls.profile=load(_sdk_path(ROOT / 'sdk/origin', 'custom-types/bounded_profile.py'),'custom_rpc_facade_models')
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory(prefix='uwvm-rpc-facades-',dir='/tmp')
        self.addCleanup(self.temp.cleanup)
        self.root=Path(self.temp.name)
        shutil.copytree(_sdk_path(ROOT / 'sdk/origin', 'custom-types/interface-examples'),self.root/'schema')
        self.catalog=self.root/'schema/catalog.preparation.json'
    def generate(self):
        return self.profile.generate(self.parent,self.catalog,self.root/'generated',True)
    def test_service_action_add_exactly_four_modules(self):
        proof=self.generate()
        self.assertEqual(len(proof['outputs']),12)
        for suffix in ('rs','mjs','go','ts'):
            name='bounded_interfaces.'+suffix
            body=(self.root/'generated'/name).read_bytes()
            self.assertEqual(proof['outputs'][name],{'bytes':len(body),'sha256':hashlib.sha256(body).hexdigest()})
            self.assertIn(b'Ros2Demo_rpcSrvIncrementClient',body)
            self.assertIn(b'Ros2Demo_rpcActionCountServer',body)
    def test_message_only_native_inventory_stays_eleven_without_templates(self):
        (self.root/'schema/Only.msg').write_text('int32 value\n')
        self.catalog.write_text(json.dumps({'format':'uwvm-ros-custom-bounded-v2','types':[
            {'ros_version':2,'type':'demo_rpc/msg/Only','file':'Only.msg','native_identity':'RIHS01_'+'0'*64}]}))
        proof=self.profile.generate(self.parent,self.catalog,self.root/'generated')
        self.assertEqual(len(proof['outputs']),11)
        self.assertNotIn('facade_templates',proof['source_inputs'])
        self.assertFalse((self.root/'generated/bounded_interfaces.rs').exists())
    def test_every_used_template_full_body_hash_is_bound(self):
        proof=self.generate()
        pins=proof['source_inputs']['facade_templates']
        self.assertEqual(len(pins),4)
        for pin in pins:
            body=(_sdk_path(ROOT / 'sdk/origin', 'custom-types', pin['generator_relative'])).read_bytes()
            self.assertEqual(set(pin),{'generator_relative','bytes','sha256'})
            self.assertEqual(pin['bytes'],len(body))
            self.assertEqual(pin['sha256'],hashlib.sha256(body).hexdigest())
    def test_template_mutation_between_emit_and_commit_refuses_output(self):
        copied=self.root/'generator'
        shutil.copytree(_sdk_path(ROOT / 'sdk/origin', 'custom-types/interface_templates'),copied/'interface_templates')
        shutil.copy2(_sdk_path(ROOT / 'sdk/origin', 'custom-types/bounded_profile.py'),copied/'bounded_profile.py')
        profile=load(copied/'bounded_profile.py','custom_rpc_mutating_generator')
        original=self.parent.bounded_read
        counts={}
        def mutate(path,maximum):
            raw=original(path,maximum)
            if Path(path).name=='bounded_interfaces.rs.in':
                counts[str(path)]=counts.get(str(path),0)+1
                if counts[str(path)]==2:return raw+b'\n// deliberate source change\n'
            return raw
        self.parent.bounded_read=mutate
        try:
            with self.assertRaisesRegex(self.parent.GenerationError,'template changed'):
                profile.generate(self.parent,self.catalog,self.root/'generated',True)
        finally:self.parent.bounded_read=original
        self.assertFalse((self.root/'generated').exists())
    def test_unknown_template_placeholder_is_not_silently_emitted(self):
        copied=self.root/'generator'
        shutil.copytree(_sdk_path(ROOT / 'sdk/origin', 'custom-types/interface_templates'),copied/'interface_templates')
        shutil.copy2(_sdk_path(ROOT / 'sdk/origin', 'custom-types/bounded_profile.py'),copied/'bounded_profile.py')
        path=copied/'interface_templates/bounded_interfaces.ts.in'
        path.write_text(path.read_text()+'\n@@UNREVIEWED@@\n')
        profile=load(copied/'bounded_profile.py','custom_rpc_bad_placeholder')
        with self.assertRaisesRegex(self.parent.GenerationError,'unknown RPC template'):
            profile.generate(self.parent,self.catalog,self.root/'generated',True)
        self.assertFalse((self.root/'generated').exists())
    def test_reusing_output_never_replaces_existing_receipt(self):
        self.generate()
        before=(self.root/'generated/generation.json').read_bytes()
        with self.assertRaisesRegex(self.parent.GenerationError,'already exists'):
            self.generate()
        self.assertEqual(before,(self.root/'generated/generation.json').read_bytes())
    def test_ros1_names_remain_distinct_without_builtin_fibonacci_aliases(self):
        data=json.loads(self.catalog.read_text())
        for row in data['types']:
            row['ros_version']=1
            row['type']='demo_rpc/'+row['type'].split('/')[-1]
        self.catalog.write_text(json.dumps(data))
        self.generate()
        body=(self.root/'generated/bounded_interfaces.mjs').read_text()
        self.assertIn('Ros1Demo_rpcIncrementClient',body)
        self.assertNotIn('FibonacciGoal',body)
        self.assertNotIn('createActionClient',body)

if __name__=='__main__':unittest.main()
