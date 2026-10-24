"""Source-contract regressions; the admitted Linux driver injects real helpers.

All mutations are private source DATA, never compiled/linked/executed as Guest
or native Host code. These checks cannot establish SDK/ROS/debugger parity.
"""
from pathlib import Path
import hashlib
import json
import re
import unittest


def suite(checker, rebinder, *, inputs, scratch, guard):
    class Bindings(unittest.TestCase):
        def setUp(self):
            guard();self.saved=checker.ROOT
            self.root=Path(scratch)/self._testMethodName;self.root.mkdir()
            for rel in inputs:
                path=self.root/rel;path.parent.mkdir(parents=True,exist_ok=True)
                path.write_bytes((self.saved/rel).read_bytes())
            checker.ROOT=self.root
        def tearDown(self):
            checker.ROOT=self.saved;guard()
        def mutate(self, rel, old, new):
            path=self.root/rel;raw=path.read_text();self.assertIn(old,raw)
            path.write_text(raw.replace(old,new,1));return raw
        def rejected(self, rel, old, new):
            raw=self.mutate(rel,old,new)
            try:
                with self.assertRaises(ValueError):checker.check()
            finally:(self.root/rel).write_text(raw)
        def test_current_all_language_contract(self):
            value=checker.check()
            self.assertEqual(value['status'],'SOURCE_CONTRACT_PASS')
            self.assertEqual(len(value['languages']),15)
            self.assertEqual(len(value['bindings']),22)
            self.assertEqual(value['backend_qualified_declarations_checked'],891)
            self.assertEqual([v['generated_call_expressions'] for v in value['python_generated_calls']],[38,43])
        def test_each_backend_rejects_narrowed_offsets(self):
            for version in (1,2):
                for rel,old,new in [
                    (f'sdk/ros{version}/abi.h','uint64_t alias_offset','uint32_t alias_offset'),
                    (f'sdk/origin/rust/src/raw/ros{version}.rs','alias_offset: u64','alias_offset: u32'),
                    (f'sdk/origin/tinygo/raw_ros{version}.go','alias_offset uint64','alias_offset uint32'),
                    (f'sdk/origin/go/raw_ros{version}.go','alias_offset uint64','alias_offset uint32'),
                    (f'sdk/origin/zig/ros{version}.zig','@"alias_offset": u64','@"alias_offset": u32'),
                    (f'sdk/origin/assemblyscript/ros{version}/raw.ts','alias_offset: u64','alias_offset: u32'),
                    (f'sdk/origin/java/src/main/java/org/uwvm/roswasm/Ros{version}Raw.java','long alias_offset','int alias_offset'),
                    (f'sdk/origin/moonbit/ros{version}/raw.mbt','alias_offset : UInt64','alias_offset : Int'),
                    ('sdk/origin/csharp/RosWasm/SelectedRaw.cs',
                     f'EntryPoint="roswasm_ros{version}_capability_open",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]\n    internal static extern uint capability_open(uint memory, ulong alias_offset',
                     f'EntryPoint="roswasm_ros{version}_capability_open",ExactSpelling=true,CallingConvention=CallingConvention.Cdecl)]\n    internal static extern uint capability_open(uint memory, uint alias_offset')]:
                    with self.subTest(backend=version,path=rel):self.rejected(rel,old,new)
        def test_go_duplicate_and_unknown_module_refused(self):
            rel='sdk/origin/go/raw_ros1.go'
            self.rejected(rel,'//go:wasmimport uwvm_ros1_v1 abi_version','//go:wasmimport uwvm_ros2_v1 abi_version')
            path=self.root/rel;raw=path.read_text()
            path.write_text(raw+'\n//go:wasmimport uwvm_ros1_v1 abi_version\nfunc AbiVersion() uint32\n')
            with self.assertRaises(ValueError):checker.check()
        def test_each_javascript_backend_rejects_narrowed_offset(self):
            path=self.root/'sdk/origin/javascript/abi.mjs';raw=path.read_text()
            match=re.search(r'\bconst inventory\s*=\s*',raw)
            self.assertIsNotNone(match)
            tail=raw[match.end():];value,used=json.JSONDecoder().raw_decode(tail)
            for backend in ('1','2'):
                item=next(f for f in value[backend]['functions'] if f['name']=='capability_open')
                parameter=next(p for p in item['parameters'] if p['name']=='alias_offset')
                parameter['type']='i32'
                path.write_text(raw[:match.end()]+json.dumps(value)+tail[used:])
                with self.subTest(backend=backend):
                    with self.assertRaises(ValueError):checker.check()
                parameter['type']='i64'
            path.write_text(raw)
        def test_managed_module_and_symbol_mismatch_refused(self):
            self.rejected('sdk/origin/java/src/main/java/org/uwvm/roswasm/Ros1Raw.java','name="abi_version"','name="missing_version"')
            self.rejected('sdk/origin/csharp/RosWasm/SelectedRaw.cs','EntryPoint="roswasm_ros1_abi_version"','EntryPoint="roswasm_ros2_abi_version"')
            self.rejected('sdk/origin/moonbit/ros1/raw.mbt','"uwvm_ros1_v1" "abi_version"','"uwvm_ros2_v1" "abi_version"')
        def test_unrecognized_result_shapes_are_refused(self):
            self.rejected('sdk/origin/go/raw_ros1.go','func AbiVersion() uint32','func AbiVersion() uint64')
            self.rejected('sdk/origin/zig/ros1.zig','fn abi_version() u32;','fn abi_version() u64;')
            self.rejected('sdk/origin/java/src/main/java/org/uwvm/roswasm/Ros1Raw.java','native int abi_version()','native long abi_version()')
            self.rejected('sdk/origin/csharp/RosWasm/SelectedRaw.cs','extern uint abi_version()','extern ulong abi_version()')
            self.rejected('sdk/origin/moonbit/ros1/raw.mbt','raw_abi_version() -> Int','raw_abi_version() -> UInt64')
        def test_c_forwarder_argument_order_and_backend_refused(self):
            self.rejected('sdk/origin/csharp/roswasm_bridge.c','return uwvm_ros1_capability_open(memory, alias_offset, alias_length, handle_out_offset);','return uwvm_ros1_capability_open(memory, alias_length, alias_offset, handle_out_offset);')
            self.rejected('sdk/origin/csharp/roswasm_bridge.c','return uwvm_ros1_capability_close(handle);','return uwvm_ros2_capability_close(handle);')
        def test_python_wrong_backend_order_and_output_kind_refused(self):
            self.rejected('sdk/origin/python/roswasm/_native.c','slot = uwvm_ros1_abi_version();','slot = uwvm_ros2_abi_version();')
            self.rejected('sdk/origin/python/roswasm/_native.c','uwvm_ros1_event_watch(handle, other, correlation)','uwvm_ros1_event_watch(handle, correlation, other)')
            self.rejected('sdk/origin/python/roswasm/_native.c','slot = uwvm_ros1_abi_version(); output_kind = 4;','slot = uwvm_ros1_abi_version(); output_kind = 0;')
        def test_refresh_requires_exact_both_input_hashes(self):
            host=(self.root/rebinder.HOST).read_bytes();catalog=(self.root/rebinder.CATALOG).read_bytes()
            sha=lambda b:hashlib.sha256(b).hexdigest()
            for a,b in [('0'*64,sha(catalog)),(sha(host),'0'*64)]:
                with self.assertRaises(ValueError):rebinder.candidate(self.root,a,b,self.root)
            self.assertEqual(rebinder.candidate(self.root,sha(host),sha(catalog),self.root),catalog)
        def test_refresh_preserves_original_and_refuses_signature_drift(self):
            rel=rebinder.CATALOG;catalog=(self.root/rel).read_bytes()
            value=json.loads(catalog);value['authority_source']['sha256']='0'*64
            stale=(json.dumps(value,indent=2)+'\n').encode();(self.root/rel).write_bytes(stale)
            host=(self.root/rebinder.HOST).read_bytes();sha=lambda b:hashlib.sha256(b).hexdigest()
            body=rebinder.candidate(self.root,sha(host),sha(stale),self.root)
            self.assertEqual((self.root/rel).read_bytes(),stale)
            self.assertEqual(json.loads(body)['authority_source']['sha256'],sha(host))
            self.mutate(rebinder.HOST,'client_features(::std::uint64_t a','client_features(::std::uint32_t a')
            changed=(self.root/rebinder.HOST).read_bytes()
            with self.assertRaises(ValueError):rebinder.candidate(self.root,sha(changed),sha(stale),self.root)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Bindings)
