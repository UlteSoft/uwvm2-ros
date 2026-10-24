"""Authored binary signature contracts, NOT RUN. No Runtime/native simulation.

Run only after an independent owner admits this complete SDK/source closure.
The fixture builds numeric Wasm import bytes; it executes no Guest/import.
"""
import importlib.util
import json
from pathlib import Path
import sys
import unittest
SDK=Path(__file__).resolve().parents[5]/'sdk/origin'
# Fixed neighboring repository tool closure, not a user/plugin/catalog path.
sys.path.insert(0,str(SDK/'tools'))
from check_rmw_service_imports import check

def uleb(n):
    out=bytearray()
    while True:
        b=n&127;n>>=7;out.append(b|(128 if n else 0))
        if not n:return bytes(out)
def text(s):
    raw=s.encode('utf-8');return uleb(len(raw))+raw
def section(kind,data):return bytes([kind])+uleb(len(data))+data
def module(rows):
    codes={'i32':0x7f,'i64':0x7e};types=[];imports=[]
    for i,(mod,name,params,results)in enumerate(rows):
        types.append(b'\x60'+uleb(len(params))+bytes(codes[x]for x in params)+uleb(len(results))+bytes(codes[x]for x in results))
        imports.append(text(mod)+text(name)+b'\0'+uleb(i))
    return b'\0asm\x01\0\0\0'+section(1,uleb(len(types))+b''.join(types))+section(2,uleb(len(imports))+b''.join(imports))
def rows():
    data=json.loads((SDK/'abi/rmw-service-v1.json').read_text())
    return [('uwvm_ros2_rmw_srv_v1',x['name'],x['params'],x['results'])for x in data['modules']['uwvm_ros2_rmw_srv_v1']['functions']]
class ImportContracts(unittest.TestCase):
    def test_all_twelve_exact(self):self.assertEqual(check(module(rows()),require_all=True)['optional_functions_checked'],12)
    def test_partial_only_when_not_require_all(self):
        self.assertEqual(check(module(rows()[:1]))['optional_functions_checked'],1)
        with self.assertRaises(ValueError):check(module(rows()[:1]),require_all=True)
    def test_memory_index_must_be_i32(self):
        values=rows();m,n,p,r=values[1];p=list(p);p[2]='i64';values[1]=(m,n,p,r)
        with self.assertRaises(ValueError):check(module(values))
    def test_offsets_handles_lengths_are_i64(self):
        for pos in (0,1,3,4,5):
            values=rows();m,n,p,r=values[4];p=list(p);p[pos]='i32';values[4]=(m,n,p,r)
            with self.assertRaises(ValueError):check(module(values))
    def test_wrong_backend_or_unknown_ros_module(self):
        for mod in ('uwvm_ros1_rmw_srv_v1','uwvm_ros2_rmw_srv_v2'):
            values=rows();m,n,p,r=values[0];values[0]=(mod,n,p,r)
            with self.assertRaises(ValueError):check(module(values))
    def test_duplicate_and_non_errno_result_refused(self):
        values=rows()
        with self.assertRaises(ValueError):check(module(values+[values[0]]))
        m,n,p,r=values[0];values[0]=(m,n,p,['i64'])
        with self.assertRaises(ValueError):check(module(values))
    def test_extensions_require_explicit_selection(self):
        values=rows()+[('uwvm_ros2_rmw_sub_v1','abi_version',[],['i32'])]
        with self.assertRaises(ValueError):check(module(values))
        self.assertEqual(check(module(values),with_subscription=True)['other_selected_functions_checked'],1)
    def test_nonros_requires_explicit_allowance(self):
        values=rows()+[('fixture_numeric_only','bounded',[],['i32'])]
        with self.assertRaises(ValueError):check(module(values))
        self.assertEqual(check(module(values),allowed_modules=('fixture_numeric_only',))['optional_functions_checked'],12)
if __name__=='__main__':unittest.main()
