#!/usr/bin/env python3
"""Pure filesystem/report models for mandatory cancellation driver bindings.

No compiler, CMake, ARM ELF, native ROS process, Guest or mount operation runs.
The fake products are plain text. ARM inspection is explicitly mocked only in
these tests; production driver checks retain their actual ELF183 requirements.
"""
import copy
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tests/ros_aarch64'))
import run_action_qos
import run_all


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def file_model(path):
    path = Path(path);stat = path.stat()
    return {'path':str(path),'sha256':sha(path),'device':stat.st_dev,'inode':stat.st_ino,'bytes':stat.st_size}


def preparation():
    incarnation = {'path':'MODEL_ONLY','device':1,'inode':2}
    return {'status':'PASS','format_version':2,'mode':'prepare','source_manifest_sha256':'a'*64,
            'probes':{key:{} for key in run_action_qos.PROBES},
            'probe_contracts':copy.deepcopy(run_action_qos.PROBE_CONTRACTS),
            'same_cgroup_incarnation':True,'cgroup_incarnation_before':incarnation,
            'cgroup_incarnation_after':dict(incarnation),
            'driver_inputs':{name:{'sha256':sha(ROOT/'tests/ros_aarch64'/name)} for name in run_action_qos.DRIVER_INPUTS}}


class CancellationDriverModels(unittest.TestCase):
    def test_four_probe_recipe(self):
        text = run_action_qos.cmake()
        self.assertEqual(len(run_action_qos.PROBES),4)
        self.assertEqual(text.count('add_executable('),4)
        for label,relative in run_action_qos.PROBES.items():
            self.assertIn('add_executable('+label+' /work/'+relative+')',text)
            self.assertIn('target_compile_options('+label+' PRIVATE -O1 -g0 -UNDEBUG -std=gnu++17 -MD -include /tmp/evidence-stop.hpp)',text)
        self.assertIn('target_link_libraries(action-qos-cancel-native pthread ${CMAKE_DL_LIBS})',text)
        self.assertEqual(text.count('${CMAKE_DL_LIBS}'),1)

    def test_old_partial_stale_preparations(self):
        good = preparation();run_action_qos.validate_prepared_contract(good,'a'*64)
        for kind in ('old-two','missing-contract','old-version','wrong-source','driver-change','driver-missing','incarnation-change','failed'):
            bad = copy.deepcopy(good)
            if kind=='old-two':
                for key in list(bad['probes'])[2:]:bad['probes'].pop(key)
            elif kind=='missing-contract':bad['probe_contracts'].pop('action-qos-cancel-native')
            elif kind=='old-version':bad['format_version']=1
            elif kind=='wrong-source':bad['source_manifest_sha256']='b'*64
            elif kind=='driver-change':bad['driver_inputs']['run_action_qos.py']['sha256']='c'*64
            elif kind=='driver-missing':bad['driver_inputs'].pop('runtime.py')
            elif kind=='incarnation-change':bad['cgroup_incarnation_after']['inode']=3
            elif kind=='failed':bad['status']='FAIL'
            with self.subTest(kind=kind),self.assertRaises(RuntimeError):
                run_action_qos.validate_prepared_contract(bad,'a'*64)

    def test_fresh_worker_source_closure(self):
        authority = [
            'src/uwvm2/imported/wasi/wasip1/environment/environment.h',
            'src/uwvm2/uwvm/imported/wasi/wasip1/storage/env.h',
            'src/uwvm2/uwvm/cmdline/params/impl.h',
            'src/uwvm2/uwvm/cmdline/params/wasip1_permissions.h',
            'src/uwvm2/uwvm/cmdline/callback/impl.h',
            'src/uwvm2/uwvm/cmdline/callback/wasip1_permissions.h']
        units = {'runtime':'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp',
                 'main':'src/uwvm2/uwvm/main.default.cpp','host_api':'src/uwvm2/uwvm/host_api.default.cpp'}
        with tempfile.TemporaryDirectory(prefix='uwvm-cancel-worker-model-') as directory:
            root = Path(directory).resolve();source = root/'source';source.mkdir();frozen = {}
            for relative in [*authority,*units.values()]:
                path = source/relative;path.parent.mkdir(parents=True,exist_ok=True);path.write_text('SOURCE MODEL '+relative)
                frozen[relative] = {'sha256':sha(path)}
            report = {'builder_sha256':sha(ROOT/'tests/ros_aarch64/build_worker.py'),'profiles':[]}
            for profile in ('none','ros1','ros2','dual'):
                dest = root/profile;dest.mkdir();binary = dest/'uwvm';binary.write_text('NOT AN EXECUTABLE')
                row = {'profile':profile,'binary':file_model(binary),'commands':[],'dependencies':{}}
                for unit,relative in units.items():
                    names = [relative,*authority] if unit=='main' else [relative]
                    dep = dest/(unit+'.d');dep.write_text('MODEL.o: '+' '.join(str(source/name) for name in names)+'\n')
                    (dest/(unit+'.o')).write_text('NOT AN ARM OBJECT')
                    row['dependencies'][unit] = {name:frozen[name]['sha256'] for name in names}
                    row['commands'].append({'returncode':0,'command':['MODEL-COMPILER','-MMD','-MF',str(dep),'-c',str(source/relative),'-o',str(dest/(unit+'.o'))]})
                row['commands'] += [{'returncode':0,'command':['MODEL-LINK']},{'returncode':0,'command':['MODEL-QEMU','--version']}]
                report['profiles'].append(row)
            with patch.object(run_all,'require_arm',file_model):
                self.assertEqual(len(run_all.validate_fresh_worker_inputs(report,source,frozen)),4)
                for key in ('original_worker_report','reuse_dependency_closure','original_compiler_dependency_records'):
                    bad = copy.deepcopy(report);bad[key] = {'MODEL_ONLY':True}
                    with self.subTest(reuse=key),self.assertRaises(RuntimeError):run_all.validate_fresh_worker_inputs(bad,source,frozen)
                for kind in ('builder','failed-compile','raw-exactset','missing-unit','command-source','missing-authority','source-byte'):
                    bad = copy.deepcopy(report);raw = root/'none/main.d';old_raw = raw.read_bytes()
                    changed = source/authority[0];old_source = changed.read_bytes()
                    try:
                        if kind=='builder':bad['builder_sha256']='e'*64
                        elif kind=='failed-compile':bad['profiles'][0]['commands'][0]['returncode']=1
                        elif kind=='raw-exactset':bad['profiles'][0]['dependencies']['main'].pop(authority[0])
                        elif kind=='missing-unit':bad['profiles'][0]['dependencies'].pop('runtime')
                        elif kind=='command-source':bad['profiles'][0]['commands'][0]['command'][-3]=str(source/'wrong.cpp')
                        elif kind=='missing-authority':
                            bad['profiles'][0]['dependencies']['main'].pop(authority[0]);raw.write_text(raw.read_text().replace(str(changed)+' ',''))
                        elif kind=='source-byte':changed.write_text('CHANGED MODEL SOURCE')
                        with self.subTest(kind=kind),self.assertRaises(RuntimeError):run_all.validate_fresh_worker_inputs(bad,source,frozen)
                    finally:raw.write_bytes(old_raw);changed.write_bytes(old_source)

    def test_mandatory_product_bindings(self):
        with tempfile.TemporaryDirectory(prefix='uwvm-cancel-products-model-') as directory:
            root = Path(directory).resolve();document = preparation()
            for key in run_action_qos.PROBES:
                path = root/key;path.write_text('NOT AN EXECUTABLE '+key)
                document['probes'][key] = {'binary':file_model(path)}
            report = root/'report.json';report.write_text(json.dumps(document))
            with patch.object(run_all,'require_arm',file_model):
                self.assertEqual(len(run_all.validate_action_qos_inputs(report,'a'*64)['products']),4)
                document['probes']['action-qos-cancel-native']['binary']['sha256']='f'*64
                report.write_text(json.dumps(document))
                with self.assertRaises(RuntimeError):run_all.validate_action_qos_inputs(report,'a'*64)

    def test_legacy_and_substituted_pool(self):
        with tempfile.TemporaryDirectory(prefix='uwvm-cancel-pool-model-') as directory:
            root = Path(directory).resolve();products = []
            for index in range(26):
                path = root/str(index);path.write_text('NOT AN EXECUTABLE');products.append(path)
            manifest = root/'pool.json';records = {str(path):{} for path in products};manifest.write_text(json.dumps({'artifacts':records}))
            run_all.validate_mandatory_product_pool(manifest,products)
            for count in (18,22,25):
                manifest.write_text(json.dumps({'artifacts':dict(list(records.items())[:count])}))
                with self.subTest(count=count),self.assertRaises(RuntimeError):run_all.validate_mandatory_product_pool(manifest,products)
            bad = dict(records);bad[str(root/'unknown')] = bad.pop(str(products[-1]));manifest.write_text(json.dumps({'artifacts':bad}))
            with self.assertRaises(RuntimeError):run_all.validate_mandatory_product_pool(manifest,products)


if __name__=='__main__':
    unittest.main()
