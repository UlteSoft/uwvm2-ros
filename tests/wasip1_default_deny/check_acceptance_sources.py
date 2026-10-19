"""Pure structural admission models; never compile or execute an ARM/Wasm file.

The positive records below are explicitly synthetic. They test that the current
acceptance verifier rejects incomplete/changed inventories and input bindings,
not that any generated binary or report proves real native/Guest behavior.
"""
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
from unittest import mock

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT/'tests/ros_aarch64'))
import run_all as acceptance
import cli_cases as recipes


class AdmissionModels(unittest.TestCase):
    def test_native_missing_row_mapping_and_changed_artifact(self):
        names = {'wasip1-configuration', 'wasip1-authority32', 'wasip1-authority64',
                 'wasip1-positive-hybrid-mt', 'wasip1-positive-hybrid-mixed-stress'}
        names.update('wasip1-positive-'+name+suffix
                     for name in ('clock_res_get', 'clock_time_get', 'random_get',
                                  'proc_raise', 'sched_yield', 'poll_oneoff')
                     for suffix in ('', '_wasm64'))
        with tempfile.TemporaryDirectory(prefix='wasi-native-admission-model-') as temporary:
            base = Path(temporary).resolve()
            nested = base/'architecture/architecture-evidence.json'
            nested.parent.mkdir()
            proof = {'status':'PASS', 'actual_process_samples':[{'mapped_arm_targets':sorted(names)}]}
            nested.write_text(json.dumps(proof))
            rows = []
            for name in sorted(names):
                # Handwritten ELF header for file-format admission only. It has
                # no program segments or machine code and is never executed.
                data = bytearray(64)
                data[:6] = b'\x7fELF\x02\x01'
                data[18:20] = (183).to_bytes(2, 'little')
                path = base/name
                path.write_bytes(data)
                rows.append({'suite':name, 'status':'PASS',
                             'run':{'returncode':0, 'live_arm_mapping_before_resume':True},
                             'artifact':acceptance.require_arm(path)})
            document = {'status':'PASS', 'source_manifest_sha256':'model-manifest',
                        'same_cgroup_incarnation':True, 'memory_swap_max_after':'0',
                        'wasip1_default_deny':{'source_sdk_resource_closure_unchanged':True},
                        'architecture_evidence':{'sha256':acceptance.digest(nested)}, 'suites':rows}
            proof['arm_targets'] = {row['suite']:row['artifact'] for row in rows}
            nested.write_text(json.dumps(proof))
            document['architecture_evidence']['sha256'] = acceptance.digest(nested)
            self.assertEqual(acceptance.validate_wasip1_native(document, base, 'model-manifest')['required_rows'], 17)
            for change in ('missing', 'duplicate', 'wrong-manifest', 'wrong-swap', 'unqualified-closure'):
                bad = copy.deepcopy(document)
                if change == 'missing': bad['suites'].pop()
                elif change == 'duplicate': bad['suites'][-1] = bad['suites'][0]
                elif change == 'wrong-manifest': bad['source_manifest_sha256'] = 'old'
                elif change == 'wrong-swap': bad['memory_swap_max_after'] = 'max'
                else: bad['wasip1_default_deny']['source_sdk_resource_closure_unchanged'] = False
                with self.assertRaises(RuntimeError): acceptance.validate_wasip1_native(bad, base, 'model-manifest')
            proof['actual_process_samples'][0]['mapped_arm_targets'].pop()
            nested.write_text(json.dumps(proof))
            document['architecture_evidence']['sha256'] = acceptance.digest(nested)
            with self.assertRaises(RuntimeError): acceptance.validate_wasip1_native(document, base, 'model-manifest')
            proof['actual_process_samples'][0]['mapped_arm_targets'] = sorted(names)
            nested.write_text(json.dumps(proof))
            document['architecture_evidence']['sha256'] = acceptance.digest(nested)
            artifact = Path(rows[0]['artifact']['path'])
            replacement = artifact.with_suffix('.replacement')
            replacement.write_bytes(artifact.read_bytes())
            self.assertNotEqual(replacement.stat().st_ino, artifact.stat().st_ino)
            replacement.replace(artifact)
            with self.assertRaises(RuntimeError): acceptance.validate_wasip1_native(document, base, 'model-manifest')
            Path(rows[0]['artifact']['path']).write_bytes(b'changed')
            with self.assertRaises(RuntimeError): acceptance.validate_wasip1_native(document, base, 'model-manifest')

    def test_original_unit_inventory_and_live_witness(self):
        with tempfile.TemporaryDirectory(prefix='original-unit-admission-model-') as temporary:
            base = Path(temporary)
            nested = base/'architecture/architecture-evidence.json'
            nested.parent.mkdir()
            rows = []
            for name in sorted(acceptance.ORIGINAL_UNIT_NAMES):
                data = bytearray(64); data[:6] = b'\x7fELF\x02\x01'
                data[18:20] = (183).to_bytes(2, 'little')
                path = base/name; path.write_bytes(data)
                rows.append({'suite':name, 'status':'PASS',
                             'run':{'returncode':0, 'live_arm_mapping_before_resume':True},
                             'artifact':acceptance.require_arm(path)})
            nested.write_text(json.dumps({'status':'PASS',
                'actual_process_samples':[{'mapped_arm_targets':sorted(acceptance.ORIGINAL_UNIT_NAMES)}],
                'arm_targets':{row['suite']:row['artifact'] for row in rows}}))
            document = {'status':'PASS', 'source_manifest_sha256':'synthetic',
                        'same_cgroup_incarnation':True, 'memory_swap_max_after':'0',
                        'architecture_evidence':{'sha256':acceptance.digest(nested)}, 'suites':rows}
            check = lambda value: acceptance.validate_native_rows(value, base, 'synthetic', acceptance.ORIGINAL_UNIT_NAMES)
            self.assertEqual(check(document)['required_rows'], 6)
            for change in ('missing', 'duplicate', 'failed', 'no-live-barrier', 'changed-incarnation'):
                bad = copy.deepcopy(document)
                if change == 'missing': bad['suites'].pop()
                elif change == 'duplicate': bad['suites'][-1] = bad['suites'][0]
                elif change == 'failed': bad['suites'][0]['run']['returncode'] = 1
                elif change == 'no-live-barrier': bad['suites'][0]['run'].pop('live_arm_mapping_before_resume')
                else: bad['same_cgroup_incarnation'] = False
                with self.assertRaises(RuntimeError): check(bad)

    def test_search_contract_rows_require_standards_and_actual_dependency_bindings(self):
        with tempfile.TemporaryDirectory(prefix='search-contract-admission-model-') as temporary:
            base = Path(temporary)
            nested = base/'architecture/architecture-evidence.json';nested.parent.mkdir()
            rows = []
            for name,(relative,standard) in acceptance.SEARCH_CONTRACT_ROWS.items():
                data = bytearray(64);data[:6] = b'\x7fELF\x02\x01';data[18:20] = (183).to_bytes(2,'little')
                binary, obj, dep = base/name, base/(name+'.o'), base/(name+'.d')
                binary.write_bytes(data);obj.write_bytes(data)
                dep.write_text('synthetic dependency record; no compilation\n')
                rows.append({'suite':name,'status':'PASS','original_source':relative,'standard':standard,
                    'run':{'returncode':0,'live_arm_mapping_before_resume':True},
                    'build':{'returncode':0},'link':{'returncode':0},
                    'artifact':acceptance.require_arm(binary),'object_elf':acceptance.require_arm(obj),
                    'dependencies':{'path':str(dep),'sha256':acceptance.digest(dep),
                        'scope':'Actual -MD project+prequalified system dependencies',
                        'project_inputs':{relative:{'sha256':acceptance.digest(ROOT/relative),
                                                    'bytes':(ROOT/relative).stat().st_size}}}})
            nested.write_text(json.dumps({'status':'PASS',
                'actual_process_samples':[{'mapped_arm_targets':list(acceptance.SEARCH_CONTRACT_ROWS)}],
                'arm_targets':{r['suite']:r['artifact'] for r in rows}}))
            document = {'status':'PASS','source_manifest_sha256':'synthetic',
                'same_cgroup_incarnation':True,'memory_swap_max_after':'0',
                'architecture_evidence':{'sha256':acceptance.digest(nested)},
                'source_compiler_closure':{'source_sdk_resource_closure_unchanged':True},'suites':rows}
            check = lambda value: acceptance.validate_search_contracts(value,base,'synthetic',ROOT)
            self.assertEqual(check(document)['required_rows'],2)
            for change in ('missing','wrong-standard','wrong-source','no-system-closure','wrong-input','failed-link'):
                bad = copy.deepcopy(document)
                if change == 'missing':bad['suites'].pop()
                elif change == 'wrong-standard':bad['suites'][0]['standard'] = 26
                elif change == 'wrong-source':bad['suites'][0]['original_source'] = 'other.cc'
                elif change == 'no-system-closure':bad['source_compiler_closure'] = {}
                elif change == 'wrong-input':bad['suites'][0]['dependencies']['project_inputs'] = {}
                else:bad['suites'][0]['link']['returncode'] = 1
                with self.assertRaises(RuntimeError):check(bad)
            dep = Path(rows[0]['dependencies']['path']);original = dep.read_bytes();dep.write_bytes(b'changed')
            with self.assertRaises(RuntimeError):check(document)
            dep.write_bytes(original)
            obj = Path(rows[0]['object_elf']['path']);replacement = obj.with_suffix('.replacement')
            replacement.write_bytes(obj.read_bytes());replacement.replace(obj)
            with self.assertRaises(RuntimeError):check(document)

    def test_existing_regression_profiles_commands_and_native_mappings(self):
        with tempfile.TemporaryDirectory(prefix='existing-regression-admission-model-') as temporary:
            base = Path(temporary)
            targets = {}
            for name in ('worker', 'trusted-dl-plugin'):
                data = bytearray(64); data[:6] = b'\x7fELF\x02\x01'
                data[18:20] = (183).to_bytes(2, 'little')
                path = base/name; path.write_bytes(data)
                targets[name] = acceptance.require_arm(path)
            worker = base/'worker'
            names = ('wasi_preview1_compile', 'wasi_preview1', 'wasi_memory64_compile',
                     'wasi_memory64', 'runtime_tail_compile', 'runtime_tail',
                     'trusted_dl_compile', 'trusted_dl_guest_compile', 'trusted_dl')
            raw = base/'results.json'
            result = {'status':'PASS', 'tests':[{'name':name, 'status':'PASS', 'returncode':0} for name in names]}
            raw.write_text(json.dumps(result))
            launches = [{'name':'regression-launch-'+str(index)+'/',
                         'required':['worker', 'trusted-dl-plugin'] if index == 3 else ['worker']}
                        for index in range(4)]
            config = {'compiler':['/synthetic/clang'], 'compile_flags':['--target=aarch64-linux-gnu'],
                      'qemu_argv':['/synthetic/qemu', '-L', '/synthetic/rootfs']}
            compile_prefix = [*config['compiler'], *config['compile_flags'], '-x', 'c', '-MMD', '-MF', str(base.parent/'trusted-dl.d')]
            commands = acceptance.expected_regression_commands(worker, base, ROOT, '/synthetic/wat2wasm', compile_prefix)
            command_path = base/'commands.json'; command_path.write_text(json.dumps(commands))
            for index, launch in enumerate(launches):
                launch['original_command'] = commands[(1, 3, 5, 8)[index]]
                launch['qemu_command'] = [*config['qemu_argv'], *launch['original_command']]
            document = {'status':'PASS', 'source_manifest_sha256':'synthetic',
                'source_addon_manifest_sha256':'synthetic-addon', 'input_worker':str(worker),
                'input_worker_sha256':acceptance.digest(worker), 'same_cgroup_incarnation':True,
                'remaining_owned_processes':[], 'arm_targets':targets, 'launches':launches,
                'config_sha256':'synthetic-config',
                'original_driver_sha256':acceptance.digest(ROOT/'tests/ros/regression.py'),
                'original_commands':{'path':str(command_path), 'sha256':acceptance.digest(command_path),
                                     'commands':commands, 'wat2wasm':'/synthetic/wat2wasm', 'compile_prefix':compile_prefix},
                'actual_process_samples':[{'name':row['name'], 'mapped_arm_targets':row['required']} for row in launches],
                'original_results':{'path':str(raw), 'sha256':acceptance.digest(raw), 'result':result}}
            for profile in acceptance.REGRESSION_PROFILES:
                document['profile'] = profile
                check = lambda value: acceptance.validate_original_regression(value, profile, worker, 'synthetic',
                    'synthetic-addon', ROOT, config, 'synthetic-config', '/synthetic/wat2wasm')
                self.assertEqual(check(document)['required_commands'], 9)
                for change in ('wrong-profile', 'missing-command', 'duplicate-command', 'failed-command',
                               'missing-launch', 'missing-plugin-map', 'wrong-worker', 'wrong-addon', 'owned-child',
                               'missing-command-record', 'changed-command', 'changed-qemu', 'wrong-config'):
                    bad = copy.deepcopy(document)
                    if change == 'wrong-profile': bad['profile'] = 'other'
                    elif change == 'missing-command': bad['original_results']['result']['tests'].pop()
                    elif change == 'duplicate-command': bad['original_results']['result']['tests'][-1] = bad['original_results']['result']['tests'][0]
                    elif change == 'failed-command': bad['original_results']['result']['tests'][0]['returncode'] = 1
                    elif change == 'missing-launch': bad['launches'].pop()
                    elif change == 'missing-plugin-map': bad['actual_process_samples'][-1]['mapped_arm_targets'] = ['worker']
                    elif change == 'wrong-worker': bad['input_worker_sha256'] = '0'*64
                    elif change == 'wrong-addon': bad['source_addon_manifest_sha256'] = 'old-addon'
                    elif change == 'missing-command-record': bad.pop('original_commands')
                    elif change == 'changed-command': bad['launches'][0]['original_command'] = ['/bin/true']
                    elif change == 'changed-qemu': bad['launches'][0]['qemu_command'] = ['/bin/true']
                    elif change == 'wrong-config': bad['config_sha256'] = 'other-config'
                    else: bad['remaining_owned_processes'] = [123]
                    with self.assertRaises(RuntimeError): check(bad)
            raw.write_text(json.dumps({**result, 'status':'changed'}))
            with self.assertRaises(RuntimeError): check(document)
            raw.write_text(json.dumps(result))
            command_path.write_text(json.dumps([['/bin/true']]*9))
            with self.assertRaises(RuntimeError): check(document)

    def test_counter_baseline_is_pinned_before_acceptance(self):
        identity = {'path':'/synthetic/leaf', 'device':1, 'inode':2, 'boot_id':'SYNTHETIC'}
        facts = {'status':'PASS', 'membership':['0::/synthetic/leaf'],
                 'cgroup_ancestors':[{'path':'/synthetic/leaf', 'memory.events':'oom 0'}]}
        discovery = copy.deepcopy(facts)
        discovery['cgroup_ancestors'][0]['memory.events'] = 'oom 999'
        with mock.patch.object(acceptance, 'cgroup_incarnation', side_effect=[identity, identity]):
            result, pinned = acceptance.pinned_cgroup_probe(mock.Mock(side_effect=[discovery, facts]))
            self.assertEqual(result, facts)
            self.assertEqual(pinned, identity)
        for changed in ({**identity, 'boot_id':'OTHER'}, {**identity, 'inode':3}):
            with mock.patch.object(acceptance, 'cgroup_incarnation', side_effect=[identity, changed]):
                with self.assertRaises(RuntimeError): acceptance.pinned_cgroup_probe(mock.Mock(side_effect=[facts, facts]))
            probe = mock.Mock()
            with mock.patch.object(acceptance, 'cgroup_incarnation', return_value=changed):
                with self.assertRaises(RuntimeError): acceptance.pinned_cgroup_probe(probe, identity)
            probe.assert_not_called()
        bad = copy.deepcopy(facts); bad['membership'] = ['0::/other']
        with mock.patch.object(acceptance, 'cgroup_incarnation', side_effect=[identity, identity]):
            with self.assertRaises(RuntimeError): acceptance.pinned_cgroup_probe(mock.Mock(side_effect=[facts, bad]))

    def test_cli_missing_recipe_wat_and_binary_binding(self):
        with tempfile.TemporaryDirectory(prefix='wasi-cli-admission-model-') as temporary:
            base = Path(temporary).resolve()
            source = base/'source'
            target = source/'tests/wasip1_default_deny/cli_cases.py'
            target.parent.mkdir(parents=True)
            shutil.copyfile(ROOT/'tests/wasip1_default_deny/cli_cases.py', target)
            benign = {'artifact_path':str(base/'benign.wasm'), 'source_path':str(base/'benign.wat'),
                      'artifact_sha256':'a'*64, 'source_sha256':'b'*64, 'artifact_bytes':8, 'source_bytes':10}
            rows = [{'name':'benign-wasi-fixture-binding', 'status':'PASS', 'binding':benign}]
            compiled, bindings = {}, {benign['artifact_path']:benign}
            for width in (32, 64):
                for name, text in recipes.fixtures(width).items():
                    artifact = base/f'wasi{width}-{name}.wasm'
                    binding = {'artifact_path':str(artifact), 'source_path':str(artifact.with_suffix('.wat')),
                               'artifact_sha256':'c'*64, 'source_sha256':hashlib.sha256(text.encode()).hexdigest(),
                               'artifact_bytes':8, 'source_bytes':len(text.encode())}
                    rows.append({'name':f'wasi{width}-{name}-compile', 'status':'PASS', 'returncode':0,
                                 'source_text':text, 'source_sha256':binding['source_sha256'], 'binding':binding})
                    compiled[(width, name)] = artifact
                    bindings[str(artifact)] = binding
            required = recipes.cases(Path(benign['artifact_path']), compiled)
            self.assertEqual(len(required), 140)
            for name, command, success in required:
                binding = bindings[command[command.index('--run')+1]]
                rows.append({'name':name, 'status':'PASS', 'command':command,
                             'returncode':0 if success else 126,
                             'generated_fixture_before':binding, 'generated_fixture_after':binding})
            report = {'status':'PASS', 'cases':rows}
            path = base/'cli-results.json'
            path.write_text(json.dumps(report))
            self.assertEqual(acceptance.validate_wasip1_cli(base, source)['required_rows'], 140)
            for change in ('missing', 'duplicate', 'wat-changed', 'hash-changed', 'skipped-positive', 'oversized'):
                bad = copy.deepcopy(report)
                if change == 'missing': bad['cases'].pop()
                elif change == 'duplicate': bad['cases'].append(bad['cases'][-1])
                elif change == 'wat-changed': bad['cases'][1]['source_text'] += ' changed'
                elif change == 'oversized': bad['cases'][1]['binding']['artifact_bytes'] = 1048577
                elif change == 'skipped-positive':
                    row = next(r for r in bad['cases'] if r.get('name') == 'wasi32-monotonic-clock-granted')
                    row['returncode'] = 126
                else:
                    row = next(r for r in bad['cases'] if r.get('name') == 'wasi32-monotonic-clock-granted')
                    row['generated_fixture_after'] = dict(row['generated_fixture_after'], artifact_sha256='d'*64)
                path.write_text(json.dumps(bad))
                with self.assertRaises(RuntimeError): acceptance.validate_wasip1_cli(base, source)


if __name__ == '__main__':
    unittest.main()
