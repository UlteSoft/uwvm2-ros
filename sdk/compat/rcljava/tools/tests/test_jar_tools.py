#!/usr/bin/env python3
"""Authored DATA/reader contracts only; no compiler, producer or Guest executes.

Run only after coherent source integration in a real bounded process owner.
These small Core Wasm byte strings test structural gates; they are not native
providers, generated SDKs, runtime performance samples or ROS acceptance.
"""
import importlib.util
import copy
import hashlib
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest import mock
import warnings
import zipfile

TOOLS = Path(__file__).resolve().parents[1]
ROOT = TOOLS.parents[3]


def load(name):
    path = TOOLS / (name + '.py')
    spec = importlib.util.spec_from_file_location('_uwvm_jar_test_' + name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def u32(value):
    out = bytearray()
    while True:
        byte = value & 127; value >>= 7
        out.append(byte | (128 if value else 0))
        if not value:
            return bytes(out)


def name(value):
    raw = value.encode('utf-8')
    return u32(len(raw)) + raw


def section(kind, payload):
    return bytes([kind]) + u32(len(payload)) + payload


def module_memory(flags=1, count=1, exported=True):
    limits = u32(flags) + u32(1) + (u32(4) if flags & 1 else b'')
    memory = section(5, u32(count) + limits * count)
    export = section(7, b'\x01' + name('memory') + b'\x02\x00') if exported else b''
    return b'\0asm\1\0\0\0' + memory + export


class JarToolDataContracts(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.plan = load('plan_jar')
        cls.gate = load('check_jar_wasm')
        cls.manifest = json.loads((TOOLS / 'jar-input-source-v1.json').read_text())
        cls.reader, cls.fixed, cls.abi = cls.gate.load_fixed_reader(ROOT, cls.manifest)

    def test_ordinary_bound_and_actual_pin(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / 'source'; p.write_bytes(b'abc')
            raw, pin, names = self.plan.admit(p, limit=3, text=True)
            self.assertEqual((raw, pin['bytes'], names), (b'abc', 3, None))
            self.assertEqual(pin['inode'], p.stat().st_ino)

    @unittest.skipUnless(hasattr(os, 'mkfifo'), 'actual POSIX ordinary-file gate')
    def test_fifo_refused_before_blocking_read(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / 'fifo'; os.mkfifo(p)
            with self.assertRaises(ValueError):
                self.plan.admit(p)

    def test_oversize_refused_before_read(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / 'source'; p.write_bytes(b'1234')
            with mock.patch.object(self.plan.os, 'read', side_effect=AssertionError('must not read')):
                with self.assertRaises(ValueError):
                    self.plan.admit(p, limit=3)

    def test_growth_during_read_refused_at_cumulative_bound(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / 'source'; p.write_bytes(b'1')
            original = os.read; once = []
            def grow(fd, size):
                if not once:
                    once.append(True)
                    with p.open('ab') as f:
                        f.write(b'234')
                return original(fd, size)
            with mock.patch.object(self.plan.os, 'read', side_effect=grow):
                with self.assertRaises(ValueError):
                    self.plan.admit(p, limit=3)

    def test_duplicate_archive_members_refused(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / 'input.jar'
            with warnings.catch_warnings():
                warnings.simplefilter('ignore', UserWarning)
                with zipfile.ZipFile(p, 'w') as z:
                    z.writestr('a.class', b'x'); z.writestr('a.class', b'y')
            with self.assertRaises(ValueError):
                self.plan.admit(p, jar=True)

    def test_defined_exported_unshared_memory32(self):
        result = self.gate.memory_contract(module_memory(), self.reader)
        self.assertEqual(result['memory']['address_width'], 32)
        self.assertEqual(result['memory']['maximum_pages'], 4)
        self.assertTrue(result['runtime_linear_memory_cap_still_required'])
        self.assertFalse(result['allocator_or_code_validity_proved'])

    def test_missing_encoded_memory_maximum_is_not_runtime_budget(self):
        result = self.gate.memory_contract(module_memory(flags=0), self.reader)
        self.assertIsNone(result['memory']['maximum_pages'])
        self.assertTrue(result['runtime_linear_memory_cap_still_required'])

    def test_memory64_refused(self):
        with self.assertRaises(ValueError):
            self.gate.memory_contract(module_memory(flags=4), self.reader)

    def test_shared_memory_refused(self):
        with self.assertRaises(ValueError):
            self.gate.memory_contract(module_memory(flags=3), self.reader)

    def test_two_or_unexported_memories_refused(self):
        for data in (module_memory(count=2), module_memory(exported=False)):
            with self.assertRaises(ValueError):
                self.gate.memory_contract(data, self.reader)

    def test_complete_numeric_offset_signature_matches_real_abi(self):
        params = b'\x7f\x7e\x7e\x7e'  # memory32, three full i64 offsets/lengths
        types = section(1, b'\x01\x60\x04' + params + b'\x01\x7f')
        imports = section(2, b'\x01' + name('uwvm_ros2_v1') + name('capability_open') + b'\x00\x00')
        raw = b'\0asm\1\0\0\0' + types + imports + module_memory()[8:]
        result = self.gate.check_admitted_abi(raw, '2', self.abi, self.reader.inspect,
                                               ('wasi_snapshot_preview1',), False)
        self.assertEqual(result['ROS_functions_checked'], 1)
        self.gate.memory_contract(raw, self.reader)

    def test_i32_offset_cannot_match_i64_or_become_bridge_success(self):
        params = b'\x7f\x7f\x7e\x7e'
        types = section(1, b'\x01\x60\x04' + params + b'\x01\x7f')
        imports = section(2, b'\x01' + name('uwvm_ros2_v1') + name('capability_open') + b'\x00\x00')
        raw = b'\0asm\1\0\0\0' + types + imports + module_memory()[8:]
        with self.assertRaises(ValueError):
            self.gate.check_admitted_abi(raw, '2', self.abi, self.reader.inspect,
                                               ('wasi_snapshot_preview1',), False)


    @unittest.skipUnless(hasattr(os, 'mkfifo'), 'actual POSIX ABI input admission')
    def test_fixed_loader_refuses_fifo_abi_before_reader_execution(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d); reader = root / 'sdk/tools/check_wasm_imports.py'
            reader.parent.mkdir(parents=True)
            reader.write_bytes((ROOT / 'sdk/tools/check_wasm_imports.py').read_bytes())
            expected = next(x for x in self.manifest['support_sources']
                            if x['relative'] == 'sdk/tools/check_wasm_imports.py')
            os.chmod(reader, expected['mode'])
            abi = root / 'sdk/abi/abi-v1.json'; abi.parent.mkdir()
            os.mkfifo(abi)
            with self.assertRaises(ValueError):
                self.gate.load_fixed_reader(root, self.manifest)

    def test_signature_adapter_does_not_delegate_to_unbounded_sdk_check(self):
        params = b'\x7f\x7e\x7e\x7e'
        types = section(1, b'\x01\x60\x04' + params + b'\x01\x7f')
        imports = section(2, b'\x01' + name('uwvm_ros2_v1') + name('capability_open') + b'\x00\x00')
        raw = b'\0asm\1\0\0\0' + types + imports + module_memory()[8:]
        with mock.patch.object(self.reader, 'check', side_effect=AssertionError('no ABI path reopen')):
            result = self.gate.check_admitted_abi(raw, '2', self.abi, self.reader.inspect,
                                                ('wasi_snapshot_preview1',), False)
        self.assertEqual(result['ROS_functions_checked'], 1)


def core_imports(rows):
    """Small real Core type/import bytes; no code, Native or SDK is simulated."""
    values = {'i32':0x7f, 'i64':0x7e, 'f32':0x7d, 'f64':0x7c}
    types = []; imports = []
    for i, row in enumerate(rows):
        types.append(b'\x60' + u32(len(row['params'])) + bytes(values[x] for x in row['params'])
                     + u32(len(row['results'])) + bytes(values[x] for x in row['results']))
        imports.append(name(row['module']) + name(row['name']) + b'\x00' + u32(i))
    return b'\0asm\1\0\0\0' + section(1, u32(len(types)) + b''.join(types)) + section(2, u32(len(imports)) + b''.join(imports)) + module_memory()[8:]


class OptionalClientDataContracts(unittest.TestCase):
    """Seventeen additional source-authored contracts; all unexecuted here.

    These inspect tiny Core bytes and local bounded catalog inputs, never launch
    a producer, compiler, Guest, ROS process, async query or a native provider.
    """
    @classmethod
    def setUpClass(cls):
        cls.plan = load('plan_jar'); cls.gate = load('check_jar_wasm')
        cls.manifest = json.loads((TOOLS / 'jar-input-source-v1.json').read_text())
        cls.reader, cls.fixed, cls.abi = cls.gate.load_fixed_reader(ROOT, cls.manifest)
        cls.catalog_raw = (ROOT / 'sdk/abi/service-client-v1.json').read_bytes()
        cls.catalog = json.loads(cls.catalog_raw)
        cls.client = cls.gate.client_abi_contract(cls.catalog, '2')

    def rows(self, optional=True):
        base = [{'module':'uwvm_ros2_v1','name':'abi_version','params':[],'results':['i32']}]
        return base + ([dict(module=self.client['module'], **row) for row in self.client['functions']] if optional else [])

    def gate_module(self, raw, **kw):
        return type(self).gate.check_with_optional_client(raw, '2', self.abi, self.reader.inspect,
                   self.client, **kw)

    def test_catalog_actual_three_numeric_tuples_both_backends(self):
        for backend in ('1','2'):
            a = self.plan.client_abi_contract(self.catalog, backend)
            b = type(self).gate.client_abi_contract(self.catalog, backend)
            self.assertEqual(a,b); self.assertEqual(len(a['functions']),3)
            self.assertEqual(a['functions'][2]['params'],['i64','i64','i64','i32','i64'])

    def test_catalog_rejects_wrong_output_bytes_or_wait(self):
        for key in ('features_bytes','availability_bytes'):
            d=copy.deepcopy(self.catalog);d['output_contract'][key]+=1
            with self.assertRaises(ValueError): type(self).gate.client_abi_contract(d,'2')
        d=copy.deepcopy(self.catalog);d['maximum_wait_ns']+=1
        with self.assertRaises(ValueError): type(self).gate.client_abi_contract(d,'2')

    def test_catalog_rejects_altered_width_even_if_other_fields_match(self):
        d=copy.deepcopy(self.catalog);d['modules']['uwvm_ros2_client_v1']['functions'][1]['params'][0]='i32'
        with self.assertRaises(ValueError): type(self).gate.client_abi_contract(d,'2')

    def test_catalog_rejects_extra_module_or_function(self):
        for mode in ('module','function'):
            d=copy.deepcopy(self.catalog)
            if mode=='module':d['modules']['uwvm_manage_v1']={}
            else:d['modules']['uwvm_ros2_client_v1']['functions'].append({'name':'grant','params':[],'results':['i32']})
            with self.assertRaises(ValueError):type(self).gate.client_abi_contract(d,'2')

    def test_producer_flags_are_explicit_pair_no_default(self):
        self.assertEqual(self.plan.client_abi_flags(None,None,'2'),[])
        for path,sha in ((ROOT/'sdk/abi/service-client-v1.json',None),(None,'a'*64)):
            with self.assertRaises(ValueError):self.plan.client_abi_flags(path,sha,'2')
        f=self.plan.client_abi_flags(ROOT/'sdk/abi/service-client-v1.json','a'*64,'2')
        self.assertEqual(f,['--ros-client-abi',str(ROOT/'sdk/abi/service-client-v1.json'),'--ros-client-abi-sha256','a'*64])

    def test_producer_flags_reject_relative_catalog_or_invalid_digest(self):
        for path,sha in ((Path('abi.json'),'a'*64),(ROOT/'sdk/abi/service-client-v1.json','A'*64)):
            with self.assertRaises(ValueError):self.plan.client_abi_flags(path,sha,'2')

    def test_no_optin_still_rejects_all_optional_imports(self):
        raw=core_imports(self.rows())
        with self.assertRaises(ValueError):type(self).gate.check_with_optional_client(raw,'2',self.abi,self.reader.inspect)

    def test_exact_three_with_optin_preserves_business_and_memory(self):
        raw=core_imports(self.rows());r=self.gate_module(raw,require_all_client=True)
        self.assertEqual(r['ROS_functions_checked'],1)
        self.assertEqual(r['optional_service_client']['functions_checked'],3)
        self.assertEqual(len(r['imports']),4)
        self.assertFalse(r['optional_service_client']['host_authority_granted'])
        type(self).gate.memory_contract(raw,self.reader)

    def test_wrong_backend_optional_import_remains_refused(self):
        rows=self.rows();rows[1]['module']='uwvm_ros1_client_v1'
        with self.assertRaises(ValueError):self.gate_module(core_imports(rows))

    def test_optional_duplicate_and_unknown_names_refused(self):
        rows=self.rows();rows.append(dict(rows[1]))
        with self.assertRaises(ValueError):self.gate_module(core_imports(rows))
        rows=self.rows();rows[1]['name']='policy_install'
        with self.assertRaises(ValueError):self.gate_module(core_imports(rows))

    def test_optional_nonfunction_row_refused(self):
        rows=self.reader.inspect(core_imports(self.rows()));rows[1]['kind']=3
        with self.assertRaises(ValueError):type(self).gate.check_with_optional_client(b'not-executed','2',self.abi,lambda data:rows,self.client)

    def test_actual_i32_cannot_replace_complete_i64_argument(self):
        rows=self.rows();rows[2]['params'][0]='i32'
        with self.assertRaises(ValueError):self.gate_module(core_imports(rows))

    def test_require_all_business_is_not_weakened_by_optional_presence(self):
        with self.assertRaises(ValueError):self.gate_module(core_imports(self.rows()),require_all=True)
        rows=[dict(module='uwvm_ros2_v1',name=x['name'],params=[v['type']for v in x['params']],results=x['results']) for x in self.abi['modules']['uwvm_ros2_v1']['functions']]
        rows += [dict(module=self.client['module'],**x) for x in self.client['functions']]
        self.assertEqual(self.gate_module(core_imports(rows),require_all=True,require_all_client=True)['ROS_functions_checked'],43)

    def test_require_all_optional_is_independent_and_explicit(self):
        self.assertEqual(self.gate_module(core_imports(self.rows(False)))['optional_service_client']['functions_checked'],0)
        with self.assertRaises(ValueError):self.gate_module(core_imports(self.rows(False)),require_all_client=True)
        with self.assertRaises(ValueError):type(self).gate.check_with_optional_client(core_imports(self.rows(False)),'2',self.abi,self.reader.inspect,None,require_all_client=True)

    def test_optional_only_is_not_original_business_coverage(self):
        with self.assertRaises(ValueError):self.gate_module(core_imports(self.rows()[1:]))

    @unittest.skipUnless(hasattr(os,'mkfifo'),'actual nonblocking optional catalog admission')
    def test_catalog_fixed_role_sha_and_fifo_admission(self):
        with tempfile.TemporaryDirectory()as d:
            root=Path(d);file=root/'sdk/abi/service-client-v1.json';file.parent.mkdir(parents=True)
            file.write_bytes(self.catalog_raw);sha=hashlib.sha256(self.catalog_raw).hexdigest()
            manifest={'support_sources':[dict(relative='sdk/abi/service-client-v1.json',bytes=len(self.catalog_raw),sha256=sha,mode=file.stat().st_mode&0o7777)]}
            contract,pin=type(self).gate.load_optional_catalog(root,manifest,file,sha,'2');self.assertEqual(contract,self.client);self.assertEqual(pin['bytes'],len(self.catalog_raw))
            with self.assertRaises(ValueError):type(self).gate.load_optional_catalog(root,manifest,file,'a'*64,'2')
            other=root/'other.json';other.write_bytes(self.catalog_raw)
            with self.assertRaises(ValueError):type(self).gate.load_optional_catalog(root,manifest,other,sha,'2')
            file.unlink();os.mkfifo(file)
            with self.assertRaises(ValueError):type(self).gate.load_optional_catalog(root,manifest,file,sha,'2')

    def test_optional_path_never_reopens_abi_or_delegates_to_reader_check(self):
        with mock.patch.object(self.reader,'check',side_effect=AssertionError('no ABI path reopen or alternate checker')):
            r=self.gate_module(core_imports(self.rows()))
        self.assertEqual(r['optional_service_client']['functions_checked'],3)


if __name__ == '__main__':
    unittest.main()
