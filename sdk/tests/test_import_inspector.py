#!/usr/bin/env python3
"""Test import inspection with synthetic bytes; never instantiate a Wasm guest.

These tiny fixtures have no defined functions, code bodies or native calls.
They exercise exact i64 arguments, backend separation, import allowances and
malformed metadata refusal. They are binary-reader models, not compiled SDK or
Runtime/cgroup evidence.
"""
import json
from pathlib import Path
import sys
import unittest

sys.dont_write_bytecode=True
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from check_wasm_imports import check,Reader,inspect


def leb(value):
    """Encode fixture-only nonnegative integer fields without a compiler."""
    result=bytearray()
    while True:
        part=value&127;value>>=7;result.append(part|(128 if value else 0))
        if not value:return bytes(result)


def name(value):
    """Write a fixture UTF-8 name with its exact byte length."""
    body=value.encode();return leb(len(body))+body


def module(rows):
    """Build only type/import sections from explicit synthetic signatures."""
    values={'i32':0x7f,'i64':0x7e,'f32':0x7d,'f64':0x7c}
    types=[];imports=[]
    for mod,field,params,results in rows:
        types.append(b'\x60'+leb(len(params))+bytes(values[x] for x in params)+leb(len(results))+bytes(values[x] for x in results))
        imports.append(name(mod)+name(field)+b'\0'+leb(len(types)-1))
    t=leb(len(types))+b''.join(types);i=leb(len(imports))+b''.join(imports)
    return b'\0asm\1\0\0\0'+b'\1'+leb(len(t))+t+b'\2'+leb(len(i))+i


class ImportInspectorModels(unittest.TestCase):
    def setUp(self):
        """Load the reviewed ABI metadata; test expectations are source-pinned."""
        self.schema=json.loads((Path(__file__).resolve().parents[1]/'abi/abi-v1.json').read_text())

    def rows(self,version):
        """Produce the complete synthetic import inventory for one backend."""
        mod=f'uwvm_ros{version}_v1'
        return [(mod,r['name'],[p['type'] for p in r['params']],r['results']) for r in self.schema['modules'][mod]['functions']]

    def test_complete_inventory_and_separation(self):
        """Both full inventories match; a one-backend check rejects the other."""
        for version,count in ((1,38),(2,43)):
            with self.subTest(version=version):
                result=check(module(self.rows(version)),str(version),require_all=True)
                self.assertEqual(result['ROS_functions_checked'],count)
                with self.assertRaises(ValueError):check(module(self.rows(version)),str(3-version))
        self.assertEqual(check(module(self.rows(1)+self.rows(2)),'dual',require_all=True)['ROS_functions_checked'],81)

    def test_width_name_and_output_refusals(self):
        """Reject names/types omitted or narrowed by incorrect language stubs."""
        row=('uwvm_ros2_v1','capability_open',['i32','i64','i64','i64'],['i32'])
        self.assertEqual(check(module([row]),'2')['ROS_functions_checked'],1)
        bad=[('uwvm_ros2_v1','capability_open',['i32']*4,['i32']),
             ('uwvm_ros2_v1','capability_open',row[2],['i64']),
             ('uwvm_ros2_v1','invented_operation',row[2],['i32']),
             ('uwvm_ros99_v1','capability_open',row[2],['i32'])]
        for item in bad:
            with self.subTest(item=item):
                with self.assertRaises(ValueError):check(module([item]),'2')
        with self.assertRaises(ValueError):check(module([row]),'2',require_all=True)
        with self.assertRaises(ValueError):check(module([row,row]),'2')
        with self.assertRaises(ValueError):check(module([]),'2')

    def test_explicit_non_ros_allowance(self):
        """A WASI/env module is accepted only when explicitly listed for inspection."""
        rows=self.rows(2)+[('env','abort',['i32','i32','i32','i32'],[])]
        with self.assertRaises(ValueError):check(module(rows),'2')
        self.assertEqual(check(module(rows),'2',['env'])['ROS_functions_checked'],43)

    def test_bounded_reader(self):
        """Reject truncated/overwide fields while preserving exact uint64 bits."""
        self.assertEqual(Reader(leb((1<<64)-1)).uint(64),(1<<64)-1)
        for data in (b'',b'\x80',b'\xff'*10+b'\1'):
            with self.subTest(data=data):
                with self.assertRaises(ValueError):Reader(data).uint(64)
        with self.assertRaises(ValueError):Reader(leb(1<<32)).uint()
        with self.assertRaises(ValueError):Reader(b'\1').take(2)
        with self.assertRaises(ValueError):Reader(b'\1').end()
        with self.assertRaises(ValueError):check(b'not-wasm','2')




def typed_module(groups,imports):
    """Model explicit GC type groups without a compiler or defined function.

    These fixtures isolate index/signature parsing only. Full Wasm validity,
    compiled MoonBit modules and Runtime execution require independent gates.
    """
    body=leb(len(groups))+b''.join(groups)
    imported=leb(len(imports))+b''.join(imports)
    return b'\0asm\1\0\0\0'+b'\1'+leb(len(body))+body+b'\2'+leb(len(imported))+imported


def function_import(type_index,field='capability_open',mod='uwvm_ros2_v1'):
    """Select an exact expanded type index, never a recursive-group ordinal."""
    return name(mod)+name(field)+b'\0'+leb(type_index)


class GarbageCollectedTypeModels(unittest.TestCase):
    """New syntax/index refusal models, deliberately separate from native tests."""
    OPEN=b'\x60\x04\x7f\x7e\x7e\x7e\x01\x7f'

    def test_recursive_group_keeps_struct_array_function_slots(self):
        # Struct0 has a nullable reference to array1. Array1 has packed i8.
        group=b'\x4e\x03\x5f\x01\x63\x01\x00\x5e\x78\x01'+self.OPEN
        data=typed_module([group],[function_import(2)])
        self.assertEqual(check(data,'2')['ROS_functions_checked'],1)
        for index in (0,1,3):
            with self.subTest(index=index),self.assertRaises(ValueError):
                check(typed_module([group],[function_import(index)]),'2')

    def test_subtype_prefixes_and_unary_shorthands_keep_indices(self):
        parent=b'\x50\x00\x5f\x00'
        child=b'\x4f\x01\x00\x5f\x00'
        data=typed_module([parent,child,self.OPEN],[function_import(2)])
        self.assertEqual(check(data,'2')['ROS_functions_checked'],1)

    def test_gc_group_never_relaxes_numeric_ros_signatures(self):
        for wrong in (self.OPEN.replace(b'\x7e',b'\x7f',1),
                      b'\x60\x04\x7f\x7e\x7e\x7e\x01\x63\x00'):
            with self.subTest(wrong=wrong),self.assertRaises(ValueError):
                check(typed_module([b'\x5f\x00',b'\x4e\x01'+wrong],[function_import(1)]),'2')

    def test_references_are_explicit_in_foreign_signatures(self):
        signature=b'\x60\x02\x63\x00\x64\x6d\x01\x69'
        rows=inspect(typed_module([b'\x5f\x00',signature],
             [function_import(1,'reference','foreign')]))
        self.assertEqual(rows[0]['params'],['ref null #0','ref eq'])
        self.assertEqual(rows[0]['results'],['ref null exn'])
        # A reference is not a numeric ABI version return.
        with self.assertRaises(ValueError):
            check(typed_module([signature],[function_import(0,'abi_version')]),'2')

    def test_malformed_gc_storage_recursion_and_tag_refuse(self):
        for group in (b'\x4e\x02\x60\x00\x00',b'\x5f\x01\x78\x02',
                      b'\x5e\x78',b'\x65\x60\x00\x00',
                      b'\x60\x01\x78\x00',b'\x60\x01\x63\x80',
                      b'\x4f\x01',b'\x4e'+leb(100001)):
            with self.subTest(group=group),self.assertRaises(ValueError):
                inspect(typed_module([group],[]))
        tag=name('foreign')+name('tag')+b'\x04\x00\x00'
        with self.assertRaises(ValueError):inspect(typed_module([b'\x5f\x00'],[tag]))

    def test_signed_heap_widths_and_padded_forms_are_bounded(self):
        for encoded,expected in ((b'\x7f',-1),(b'\xff\x7f',-1),
                (b'\xff\xff\xff\xff\x0f',(1<<32)-1),
                (b'\x80\x80\x80\x80\x70',-(1<<32))):
            self.assertEqual(Reader(encoded).sint(33),expected)
        for encoded in (b'',b'\x80',b'\x80'*6,b'\x80\x80\x80\x80\x10'):
            with self.subTest(encoded=encoded),self.assertRaises(ValueError):Reader(encoded).sint(33)
        with self.assertRaises(ValueError):Reader(b'\x63\x68').value()


if __name__=='__main__':unittest.main()
