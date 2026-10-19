#!/usr/bin/env python3
"""Authored source/operator contracts; not executed during source authoring.

Future caller supplies a frozen private source tree and explicit reviewed hashes.
Only a new TemporaryDirectory under --scratch is written. No compiler, subprocess,
Guest, ROS library, Native getter, SDK replacement or invented identity is used.
"""
from pathlib import Path
import argparse
import ast
import hashlib
import json
import os
import shutil
import sys
import tempfile
import types
import unittest

BASELINE_SHA='2fee79f0a4b20707f352d7cdf02e3f848327a8928fac014dae58bad9af369d58'
ARGS=None

def module(name,path,raw):
    value=types.ModuleType(name);value.__file__=str(path);sys.modules[name]=value
    exec(compile(raw,str(path),'exec'),value.__dict__)
    return value

class SourcePolicy(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        a=ARGS;cls.root=a.source_root
        operator_path=a.source_root/'sdk/rmw_wasm/default_control/managed/generate_managed.py'
        cls.op=module('_control_contract_operator',operator_path,operator_path.read_bytes())
        parent_path=a.source_root/'sdk/tools/custom_types.py';bounded_path=a.source_root/'sdk/custom-types/bounded_profile.py'
        cls.parent_raw=cls.op.read(parent_path);cls.bounded_raw=cls.op.read(bounded_path);cls.baseline_raw=cls.op.read(a.baseline_bounded)
        if cls.op.sha(cls.parent_raw)!=a.parent_sha256 or cls.op.sha(cls.bounded_raw)!=a.bounded_sha256 or cls.op.sha(cls.baseline_raw)!=BASELINE_SHA:
            raise ValueError('actual caller-reviewed source pins required')
        cls.parent=module('_control_contract_parent',parent_path,cls.parent_raw)
        cls.bounded=module('_control_contract_new',bounded_path,cls.bounded_raw)
        cls.baseline=module('_control_contract_original',a.baseline_bounded,cls.baseline_raw)
        cls.scratch=tempfile.TemporaryDirectory(prefix='control-record-policy-',dir=a.scratch)
        cls.temp=Path(cls.scratch.name);cls.canonical=cls.temp/'canonical';cls.old=cls.temp/'baseline';cls.managed=cls.temp/'managed'
        for implementation,out in [(cls.bounded,cls.canonical),(cls.baseline,cls.old)]:
            records,ordered,interfaces,pins=implementation.load_catalog(cls.parent,a.catalog,False)
            implementation.generate_control(cls.parent,a.catalog,out,records,ordered,interfaces,pins,True)
        cls.proof=cls.op.run(a.source_root,a.catalog,cls.canonical,cls.managed,a.parent_sha256,a.bounded_sha256)
    @classmethod
    def tearDownClass(cls):cls.scratch.cleanup()
    def run_to(self,out,**kw):
        return self.op.run(ARGS.source_root,ARGS.catalog,kw.get('canonical',self.canonical),out,kw.get('parent_sha',ARGS.parent_sha256),ARGS.bounded_sha256)
    def test_entire_original_source_and_functions_unchanged(self):
        self.assertTrue(self.bounded_raw.startswith(self.baseline_raw))
        old=ast.parse(self.baseline_raw);new=ast.parse(self.bounded_raw);rows={n.name:n for n in new.body if isinstance(n,ast.FunctionDef)}
        for n in old.body:
            if isinstance(n,ast.FunctionDef):self.assertEqual(ast.dump(n,include_attributes=False),ast.dump(rows[n.name],include_attributes=False))
    def test_authoritative_old_eight_outputs_and_proof_unchanged(self):
        self.assertEqual({p.name for p in self.old.iterdir()},{p.name for p in self.canonical.iterdir()})
        self.assertEqual(len(list(self.canonical.iterdir())),8)
        for p in self.old.iterdir():
            if p.name!='generation.json':self.assertEqual(p.read_bytes(),(self.canonical/p.name).read_bytes())
        a=json.loads((self.old/'generation.json').read_bytes());b=json.loads((self.canonical/'generation.json').read_bytes())
        a.pop('bounded_generator_sha256');b.pop('bounded_generator_sha256');self.assertEqual(a,b)
    def test_managed_records_only_not_legacy_service_facades(self):
        self.assertEqual({p.name for p in self.managed.iterdir()},{'bounded_records.'+e for e in('rs','mjs','go','ts')}|{'generation.control-managed.json'})
        self.assertEqual(self.proof['implemented_service_outputs'],[])
        self.assertFalse(self.proof['native_registry_emitted']);self.assertEqual(self.proof['implicit_permissions'],[])
        self.assertEqual(len(self.proof['record_names']),32)
        self.assertEqual(len(self.proof['services']),7)
        for row in self.proof['services']:self.assertNotEqual(row['codec_identity'],row['request']['codec_identity'])
    def test_member_maps_complete_injective_without_native_type_erasure(self):
        schema=json.loads((self.canonical/'control-schema.json').read_bytes())
        for r in schema['records']:
            maps=self.proof['complete_member_spelling_maps'][r['type']];names={m['name']for m in r['members']}
            for row in maps.values():self.assertEqual(set(row),names);self.assertEqual(len(set(row.values())),len(row))
        maps=self.proof['complete_member_spelling_maps']['rcl_interfaces/msg/ParameterValue'];self.assertEqual(maps['rust']['type'],'r#type');self.assertEqual(maps['assemblyscript']['type'],'ros_type')
    def test_raw_domain_is_only_original_type_source_field(self):
        actual=[(n,f)for n,r in self.proof['record_contracts'].items()for f,v in r['string_encodings'].items()if v=='raw_bytes']
        self.assertEqual(actual,[('type_description_interfaces/msg/TypeSource','raw_file_contents')])
    def test_logical_aggregate_distinct_from_complete_cartesian_capacity(self):
        r=self.proof['record_contracts']['type_description_interfaces/srv/GetTypeDescription_Response']
        self.assertEqual(r['aggregate_wire_bytes'],32768);self.assertEqual(r['maximum_wire_bytes'],32768);self.assertGreater(r['cartesian_maximum_wire_bytes'],32768)
    def test_constants_and_declared_true_default_retained(self):
        self.assertTrue(self.proof['constants']['type_description_interfaces/msg/FieldType'])
        schema=json.loads((self.canonical/'control-schema.json').read_bytes());r=next(r for r in schema['records']if r['type']=='type_description_interfaces/srv/GetTypeDescription_Request')
        self.assertEqual(next(m for m in r['members']if m['name']=='include_type_sources')['native_default'],'True')
    def test_explicit_wrong_source_hash_refuses_before_output(self):
        out=self.temp/'wrong-sha'
        with self.assertRaises(ValueError):self.run_to(out,parent_sha='0'*64)
        self.assertFalse(out.exists())
    def test_fifo_is_rejected_before_any_blocking_file_read(self):
        path=self.temp/'fifo';os.mkfifo(path)
        with self.assertRaises(ValueError):self.op.read(path)
    def test_canonical_product_corruption_refuses_before_output(self):
        path=self.temp/'corrupt';shutil.copytree(self.canonical,path);p=path/'bounded_records.h';p.write_bytes(p.read_bytes()+b'\n')
        out=self.temp/'bad-output'
        with self.assertRaises(ValueError):self.run_to(out,canonical=path)
        self.assertFalse(out.exists())
    def test_duplicate_json_and_nonfinite_refuse(self):
        for raw in (b'{"x":1,"x":2}',b'{"x":NaN}'):
            with self.assertRaises(ValueError):self.op.strict(raw)
    def staged_sources(self,name):
        # Only new private test trees are changed. The reviewed SDK/source paths
        # are never rewritten by these A/B/A fault-injection contracts.
        root=self.temp/name;source=root/'source';catalog=root/'catalog'/'catalog.json'
        catalog.parent.mkdir(parents=True);raw=self.op.read(ARGS.catalog);catalog.write_bytes(raw)
        declared=self.op.strict(raw);base=ARGS.catalog.resolve().parent
        for row in declared['types']:
            target=catalog.parent/row['file'];target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(self.op.read((base/row['file']).resolve()))
        pp=source/'sdk/tools/custom_types.py';bp=source/'sdk/custom-types/bounded_profile.py'
        pp.parent.mkdir(parents=True);bp.parent.mkdir(parents=True);pp.write_bytes(self.parent_raw);bp.write_bytes(self.bounded_raw)
        parent=module('_control_fault_parent_'+name,pp,self.parent_raw);bounded=module('_control_fault_bounded_'+name,bp,self.bounded_raw)
        return root,source,catalog,raw,parent,bounded
    def test_captured_catalog_A_B_A_mismatch_refuses_before_output(self):
        root,source,catalog,a,parent,bounded=self.staged_sources('catalog_association')
        changed=self.op.strict(a);row=next(r for r in changed['types']if r['type']=='rcl_interfaces/msg/Log');row['bounds']['msg']['string']-=1
        b=(json.dumps(changed,indent=2)+'\n').encode();self.assertNotEqual(self.op.sha(a),self.op.sha(b));catalog.write_bytes(b)
        records,ordered,interfaces,pins=bounded.load_catalog(parent,catalog,False)
        canonical=root/'canonical_B';bounded.generate_control(parent,catalog,canonical,records,ordered,interfaces,pins,True)
        catalog.write_bytes(a);original_pair=self.op.paired_module
        def swapped(name,path,raw):
            admitted=original_pair(name,path,raw)
            if name=='_uwvm_managed_control_bounded':
                original_load=admitted.load_catalog
                def load_b(parent,path,required):
                    catalog.write_bytes(b)
                    try:return original_load(parent,path,required)
                    finally:catalog.write_bytes(a)
                admitted.load_catalog=load_b
            return admitted
        self.op.paired_module=swapped;out=root/'refused'
        try:
            with self.assertRaises(ValueError):self.op.run(source,catalog,canonical,out,ARGS.parent_sha256,ARGS.bounded_sha256)
        finally:self.op.paired_module=original_pair
        self.assertEqual(catalog.read_bytes(),a);self.assertFalse(out.exists())
    def test_captured_generator_A_B_A_proof_mismatch_refuses(self):
        root,source,catalog,a,parent,bounded=self.staged_sources('generator_association')
        records,ordered,interfaces,pins=bounded.load_catalog(parent,catalog,False)
        canonical=root/'canonical_A';bounded.generate_control(parent,catalog,canonical,records,ordered,interfaces,pins,True)
        original_pair=self.op.paired_module
        for role in('parent','bounded'):
            def swapped(name,path,raw):
                admitted=original_pair(name,path,raw)
                if name=='_uwvm_managed_control_bounded':
                    products_a=admitted.control_managed_products
                    def changed_proof(parent,records,ordered,interfaces,pins):
                        path=Path(parent.__file__)if role=='parent'else Path(admitted.__file__)
                        captured=self.op.read(path);path.write_bytes(captured+b'\n# private provenance-read fault B; never executed\n')
                        try:return products_a(parent,records,ordered,interfaces,pins)
                        finally:path.write_bytes(captured)
                    admitted.control_managed_products=changed_proof
                return admitted
            self.op.paired_module=swapped;out=root/('refused_'+role)
            try:
                with self.assertRaises(ValueError):self.op.run(source,catalog,canonical,out,ARGS.parent_sha256,ARGS.bounded_sha256)
            finally:self.op.paired_module=original_pair
            self.assertFalse(out.exists())
            self.assertEqual((source/'sdk/tools/custom_types.py').read_bytes(),self.parent_raw)
            self.assertEqual((source/'sdk/custom-types/bounded_profile.py').read_bytes(),self.bounded_raw)
    def test_existing_output_never_overwritten(self):
        before={p.name:p.read_bytes()for p in self.managed.iterdir()}
        with self.assertRaises(ValueError):self.run_to(self.managed)
        self.assertEqual(before,{p.name:p.read_bytes()for p in self.managed.iterdir()})

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in('source-root','baseline-bounded','catalog','scratch'):parser.add_argument('--'+name,type=Path,required=True)
    for name in('parent-sha256','bounded-sha256'):parser.add_argument('--'+name,required=True)
    ARGS,remaining=parser.parse_known_args()
    if not all(p.is_absolute()for p in(ARGS.source_root,ARGS.baseline_bounded,ARGS.catalog,ARGS.scratch)):parser.error('absolute reviewed paths required')
    unittest.main(argv=[sys.argv[0]]+remaining)
