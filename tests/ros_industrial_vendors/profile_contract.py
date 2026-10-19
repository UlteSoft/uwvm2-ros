#!/usr/bin/env python3
"""Authored source/operator contracts. Run only in an admitted private test stage.

The operator and both existing generators are compiled from their explicit
captured bytes. No importlib/pyc/plugin resolution and no Native metadata oracle.
Every fixture retains its complete original definition; no filtered input exists.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import tempfile
import types
import unittest

API = None
ROOT = None


def load_selected(args):
    global API, ROOT
    ROOT = Path(args.source_root).resolve(strict=True)
    adapter = Path(args.adapter).resolve(strict=True)
    parent = ROOT/'sdk/tools/custom_types.py'
    bounded = ROOT/'sdk/custom-types/bounded_profile.py'
    rows = [(adapter,args.adapter_sha256),(parent,args.parent_sha256),(bounded,args.bounded_sha256)]
    raw = [p.read_bytes() for p,_ in rows]  # Explicit ordinary immutable test inputs.
    for (path,sha),body in zip(rows,raw):
        if hashlib.sha256(body).hexdigest()!=sha:raise ValueError('test source SHA differs: '+str(path))
    module=types.ModuleType('_industrial_contract_source');module.__file__=str(adapter)
    exec(compile(raw[0],str(adapter),'exec',dont_inherit=True),module.__dict__)
    templates={n:(bounded.parent/n).read_bytes() for n in
               ('interface_templates/bounded_interfaces.'+e+'.in' for e in ('rs','mjs','go','ts'))}
    API=module.API(raw[1],raw[2],parent_path=parent,bounded_path=bounded,
        parent_sha256=args.parent_sha256,bounded_sha256=args.bounded_sha256,
        templates=templates,adapter_raw=raw[0],adapter_sha256=args.adapter_sha256)
    return module


class CompleteOriginalContracts(unittest.TestCase):
    def catalog(self, folder, name, text, bounds=None, *, version=2, kind='msg', extras=()):
        root=Path(folder);pkg='test_industrial'
        relative=pkg+'/'+kind+'/'+name+'.'+kind
        p=root/relative;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(text,encoding='utf-8')
        native=pkg+'/'+kind+'/'+name if version==2 else pkg+'/'+name
        rows=[{'ros_version':version,'type':native,'file':relative,'bounds':bounds or {},'registered':False}]
        rows.extend(extras)
        path=root/'catalog.json';path.write_text(json.dumps({'format':'uwvm-ros-vendor-complete-bounded-v1','types':rows}))
        return path,native

    def test_constant_and_legal_type_are_not_wire_fields_or_erased(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Full','int8 CMD_TYPE_PARALLEL=0\nint8 CMD_TYPE_SERIAL=1\nstring type "J"\nfloat64[6] joints\n',{'type':{'string':8}})
            records,ordered,interfaces,_=API.load_catalog(path,False);r=records[(2,native)]
            self.assertEqual([m.name for m in r.members],['type','joints'])
            self.assertEqual([c['name'] for c in r.constants],['CMD_TYPE_PARALLEL','CMD_TYPE_SERIAL'])
            self.assertEqual(r.members[0].native_default,'J')
            self.assertEqual(API.member_maps(ordered)[native]['rust']['type'],'r#type')
            self.assertEqual(API.member_maps(ordered)[native]['assemblyscript']['type'],'ros_type')
            products=API.records(records,ordered)
            self.assertEqual(set(products),{'bounded_records.'+e for e in ('h','hpp','rs','mjs','go','ts')})
            self.assertIn('next.type.size=1u;',products['bounded_records.h'])
            self.assertIn('pub const CMD_TYPE_SERIAL:i8=1;',products['bounded_records.rs'])
            self.assertIn('static readonly CMD_TYPE_SERIAL:i8=1;',products['bounded_records.ts'])
            self.assertEqual(r.maximum,4+8+6*8)

    def test_rosidl_text_default_preserves_nonquote_backslash_bytes(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Text','string value "line\\n"\n',{'value':{'string':16}})
            record=API.load_catalog(path,False)[0][(2,native)]
            self.assertEqual(record.members[0].native_default,'line\\n')
            fields=API._fields(record)
            self.assertEqual(fields[0][-2],'line\\n')
            self.assertIs(fields[0][-1],True)

    def test_explicit_empty_default_presence_is_retained_not_native_invented(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Text','string value ""\n',{'value':{'string':8}})
            records,ordered,_,_=API.load_catalog(path,False);record=records[(2,native)]
            self.assertEqual(record.members[0].native_default,'')
            self.assertEqual(API._fields(record)[0][-2:],('',True))
            with self.assertRaisesRegex(ValueError,'original native IDL source proof'):
                API.bind(path,b'{}')

    def test_actual_declared_constant_changes_canonical_identity(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Full','uint8 MODE=1\nuint8 mode\n')
            a=API.load_catalog(path,False)[0][(2,native)].codec_identity
            (Path(d)/'test_industrial/msg/Full.msg').write_text('uint8 MODE=2\nuint8 mode\n')
            b=API.load_catalog(path,False)[0][(2,native)].codec_identity
            self.assertNotEqual(a,b)

    def test_ros2_byte_and_char_keep_original_native_type(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Bytes','byte[] opaque\nchar[] chars\n',{'opaque':{'sequence':5},'chars':{'sequence':5}})
            records,ordered,interfaces,_=API.load_catalog(path,False);r=records[(2,native)]
            self.assertEqual([(m.type,m.native_scalar)for m in r.members],[('uint8','byte'),('uint8','char')])
            fields=API.declarations(records,interfaces[0])[0][1]
            self.assertEqual([f[1]for f in fields],['byte','uint8'])

    def test_ros1_byte_is_signed_and_char_unsigned(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Bytes','byte b\nchar c\n',version=1)
            r=API.load_catalog(path,False)[0][(1,native)]
            self.assertEqual([m.type for m in r.members],['int8','uint8'])

    def test_float_constants_use_real_compile_time_go_expression(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Numbers','float32 MIN=-1.25\nfloat64 MAX=2.5\nfloat32 value\n')
            records,ordered,_,_=API.load_catalog(path,False)
            go=API.records(records,ordered)['bounded_records.go']
            self.assertIn('_MIN float32=-0x1.4000000000000p+0',go)
            self.assertNotIn('_MIN float32=math.Float32frombits',go)

    def test_ros1_time_and_duration_are_actual_primitive_shapes(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Timed','time stamp\nduration delay\n',version=1)
            records,ordered,_,_=API.load_catalog(path,False)
            self.assertEqual(records[(1,'time')].native,'::ros::Time')
            self.assertEqual(records[(1,'duration')].native,'::ros::Duration')
            self.assertEqual(records[(1,native)].maximum,16)

    def test_missing_dependency_and_recursive_source_refuse(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,_=self.catalog(d,'Loop','Loop loop\n')
            with self.assertRaises(ValueError):API.load_catalog(path,False)
            (Path(d)/'test_industrial/msg/Loop.msg').write_text('Missing data\n')
            with self.assertRaises(ValueError):API.load_catalog(path,False)

    def test_version_and_capacity_require_exact_integer(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,_=self.catalog(d,'Text','string value\n',{'value':{'string':4}})
            doc=json.loads(path.read_text());doc['types'][0]['ros_version']=2.0;path.write_text(json.dumps(doc))
            with self.assertRaises(ValueError):API.load_catalog(path,False)
            doc['types'][0]['ros_version']=2;doc['types'][0]['bounds']['value']['string']=True;path.write_text(json.dumps(doc))
            with self.assertRaises(ValueError):API.load_catalog(path,False)

    def test_whole_original_source_and_unused_bounds_are_required(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,_=self.catalog(d,'Text','string value\n',{'value':{'string':4},'absent':{'string':4}})
            with self.assertRaises(ValueError):API.load_catalog(path,False)
            doc=json.loads(path.read_text());doc['types'][0]['bounds'].pop('absent');path.write_text(json.dumps(doc))
            original=API.load_catalog(path,False)[3]['definitions'][0]
            p=Path(d)/'test_industrial/msg/Text.msg';p.write_text('# full original source comment\nstring value\n')
            changed=API.load_catalog(path,False)[3]['definitions'][0]
            self.assertNotEqual(original['sha256'],changed['sha256'])

    def test_defaults_cannot_bypass_real_text_domain(self):
        for value in ('string value "'+chr(0)+'"\n','string value "abcdef"\n','string[] value []\n'):
            with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
                path,_=self.catalog(d,'Text',value,{'value':{'string':4,'sequence':4}} if '[]'in value else {'value':{'string':4}})
                with self.assertRaises(ValueError):API.load_catalog(path,False)

    def test_constant_only_message_is_dependency_not_empty_topic(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Names','string MODE="J"\n')
            r=API.load_catalog(path,False)[0][(2,native)];self.assertEqual(r.maximum,0)
            doc=json.loads(path.read_text());doc['types'][0]['registered']=True;path.write_text(json.dumps(doc))
            with self.assertRaises(ValueError):API.load_catalog(path,False)

    def test_keyword_and_generated_method_collisions_refuse_whole_source(self):
        for field in ('class','wire_size','WireSize'):
            with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
                path,_=self.catalog(d,'Fields','uint8 '+field+'\n')
                with self.assertRaises(ValueError):
                    _,ordered,_,_=API.load_catalog(path,False);API.member_maps(ordered)

    def test_unbound_catalog_never_fabricates_native_identity(self):
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Full','uint32 value\n')
            records,ordered,interfaces,_=API.load_catalog(path,False)
            self.assertIsNone(records[(2,native)].identity);self.assertIsNone(interfaces[0].identity)
            with self.assertRaises(ValueError):API.load_catalog(path,True)

    def test_full_modern_metadata_rejects_same_name_wrong_default(self):
        expected={'root':{'type':'test/msg/Text','fields':[{'name':'value','default_value':'J'}]},'references':{}}
        # Refusal is structural before an identity format could become evidence.
        with self.assertRaises(ValueError):API._verify_modern({'native_identity':'RIHS01_'+'a'*64,'root':{'type':'test/msg/Text','fields':[{'name':'value','default_value':'L'}]},'references':[]},expected)

    def test_isolated_functions_have_their_actual_required_globals(self):
        # Exercise the genuine cloned _member/_parse_fields and recursive
        # loader rather than merely asserting a dictionary contains a key.
        with tempfile.TemporaryDirectory(prefix='industrial-source-contract-')as d:
            path,native=self.catalog(d,'Fields','uint8 value\n')
            records,ordered,_,_=API.load_catalog(path,False)
            self.assertEqual(records[(2,native)].members[0].name,'value')
            API.records(records,ordered)
            self.assertIs(API.env['C_KEYWORDS'],API.env['_member'].__globals__['C_KEYWORDS'])
            self.assertIs(API.env['Field'],API.bounded.Field)

    def test_native_json_exact_types_refuse_python_numeric_equalities(self):
        identity='RIHS01_'+'a'*64  # Syntactic negative fixture, never Native proof.
        field={'name':'v','type_id':3,'capacity':0,'string_capacity':0,'nested_type_name':'','default_value':''}
        root={'type':'test/msg/Full','fields':[field]}
        expected={'root':root,'references':{}}
        for key,value in (('type_id',3.0),('capacity',False),('string_capacity',0.0)):
            changed=json.loads(json.dumps(root));changed['fields'][0][key]=value
            with self.assertRaises(ValueError):
                API._verify_modern({'native_identity':identity,'root':changed,'references':[]},expected)
        for value in (True,1.0):
            changed={'type':'test/msg/Child','fields':[dict(field,capacity=value)]}
            refs={'test/msg/Child':{'type':'test/msg/Child','fields':[dict(field,capacity=1)]}}
            with self.assertRaises(ValueError):
                API._verify_modern({'native_identity':identity,'root':root,'references':[changed]},dict(expected,references=refs))

    def test_strict_json_refuses_duplicate_and_exponent_overflow(self):
        parser=API.env['strict_json']
        for raw in (b'{"x":1,"x":2}',b'{"x":1e999}',b'{"x":NaN}'):
            with self.assertRaises(ValueError):parser(raw)


    def test_full_unsigned_byte_storage_keeps_complete_wire_and_native_semantics(self):
        fixture=ROOT/'tests/ros_industrial_vendors/fixtures/byte-sequence/catalog.preparation.json'
        records,ordered,interfaces,_=API.load_catalog(fixture,False)
        r=records[(2,'test_industrial/msg/BytePayload')]
        self.assertEqual([m.name for m in r.members],['payload','sequence'])
        self.assertEqual(r.members[0].count,16384)
        self.assertEqual(r.maximum,16392)
        self.assertLessEqual(r.nodes,4096)
        self.assertFalse(interfaces[0].identity)
        # Canonical capacity and original native declaration must be retained;
        # no text or opaque replacement schema may stand in for the byte field.
        fields=API.declarations(records,interfaces[0])[0][1]
        self.assertEqual(fields[0][0:2],('payload','uint8'))

    def test_byte_node_cost_does_not_bypass_wire_or_nested_graph_limits(self):
        with tempfile.TemporaryDirectory(prefix='industrial-byte-contract-')as d:
            path,_=self.catalog(d,'Bytes','uint8[] payload\n',{'payload':{'sequence':32768}})
            with self.assertRaisesRegex(ValueError,'wire32KiB'):
                API.load_catalog(path,False)
        with tempfile.TemporaryDirectory(prefix='industrial-byte-contract-')as d:
            root=Path(d);leaf=root/'test_industrial/msg/Leaf.msg'
            leaf.parent.mkdir(parents=True);leaf.write_text('uint8 value\n')
            extras=[{'ros_version':2,'type':'test_industrial/msg/Leaf','file':'test_industrial/msg/Leaf.msg','registered':False}]
            path,_=self.catalog(d,'Graph','Leaf[] nodes\n',{'nodes':{'sequence':4096}},extras=extras)
            with self.assertRaisesRegex(ValueError,'node4096'):
                API.load_catalog(path,False)


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--source-root',required=True)
    parser.add_argument('--adapter',required=True)
    for name in ('adapter','parent','bounded'):parser.add_argument('--'+name+'-sha256',required=True)
    args,remaining=parser.parse_known_args();load_selected(args)
    unittest.main(argv=[__file__]+remaining)
