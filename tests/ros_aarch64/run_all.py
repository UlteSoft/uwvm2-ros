#!/usr/bin/env python3
"""Run current ARM64 ROS acceptance in a private namespace and verified temporary QEMU lease."""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import signal
import subprocess
import sys
import time
import traceback

from artifacts import create
from runtime import cgroup_incarnation, cgroup_oom_deltas, digest, read_json_binding, require_arm, validate_mapping_receipts, validate_architecture_mappings
from run_action_qos import PROBES as ACTION_QOS_PROBES, validate_prepared_contract
from run_regression import expected_commands as expected_regression_commands
import run_ros1_search as ros1_features
import run_local_parameter_events as ros2_features
from gateway_qualification import architecture_binding, pins_preserved, qualify_gateway, read_file_binding
from qualify_current_native import assemble as assemble_native, qualify_base

sys.dont_write_bytecode = True


ORIGINAL_UNIT_NAMES = {'security', 'ipc', 'ipc-client-deadline', 'sorted-rows',
                       'sorted-rows-cpp17', 'sdk'}
SEARCH_CONTRACT_ROWS = {
    'ros1-search-worker-contract':('tests/ros1_search/contract_test.cc',20),
    'ros1-search-gateway-contract':('tests/ros1_search/gateway_contract_test.cc',17),
}
FEATURE_DRIVERS = {'ros1-search':ros1_features, 'ros2-local-parameter-events':ros2_features}
REGRESSION_PROFILES = ('none', 'ros1', 'ros2', 'dual')
MANDATORY_SUITES = ('action-qos', 'wasip1-default-deny', 'cli', 'original-units', 'ros1-search-contracts',
                    *FEATURE_DRIVERS,
                    *('regression-'+profile for profile in REGRESSION_PROFILES))


def pinned_cgroup_probe(probe, expected_identity=None):
    """Bracket accepted counters with leaf and boot identity checks.

    The first probe discovers the leaf only. Its counters are never a baseline.
    A changed or unreadable incarnation fails before counter subtraction, and
    final probes use the originally pinned path even if membership changed.
    """
    discovery = probe() if expected_identity is None else None
    if discovery is not None and discovery.get('status') != 'PASS':
        raise RuntimeError('existing 64 GiB cgroup discovery failed')
    leaf = Path(expected_identity['path'] if expected_identity is not None
                else discovery['cgroup_ancestors'][0]['path'])
    first = cgroup_incarnation(leaf)
    if expected_identity is not None and first != expected_identity:
        raise RuntimeError('cgroup incarnation changed before final counter probe')
    result = probe()
    last = cgroup_incarnation(leaf)
    if (first != last or result.get('status') != 'PASS'
            or not result.get('cgroup_ancestors')
            or result['cgroup_ancestors'][0]['path'] != str(leaf)
            or discovery is not None and result.get('membership') != discovery.get('membership')):
        raise RuntimeError('cgroup incarnation or membership changed while accepting counters')
    return result, first


def validate_native_rows(document, directory, manifest_sha256, expected, require_closure=False,
                         closure_key='wasip1_default_deny'):
    """Require the exact native inventory and each actual ARM image identity."""
    rows = document.get('suites', [])
    names = [item.get('suite') for item in rows]
    nested = directory/'architecture/architecture-evidence.json'
    if (document.get('status') != 'PASS' or len(names) != len(expected) or set(names) != expected
            or document.get('source_manifest_sha256') != manifest_sha256
            or document.get('same_cgroup_incarnation') is not True
            or document.get('memory_swap_max_after') != '0'
            or require_closure and document.get(closure_key, {}).get('source_sdk_resource_closure_unchanged') is not True
            or not nested.is_file()
            or document.get('architecture_evidence', {}).get('sha256') != digest(nested)):
        raise RuntimeError('mandatory native scope/inventory/closure proof is incomplete')
    architecture = json.loads(nested.read_text())
    validate_architecture_mappings(architecture)
    observed = {label for sample in architecture.get('actual_process_samples', [])
                for label in sample.get('mapped_arm_targets', [])}
    if architecture.get('status') != 'PASS' or not expected <= observed:
        raise RuntimeError('mandatory native rows lack actual ARM mapping evidence')
    for row in rows:
        artifact = row.get('artifact', {})
        if (row.get('status') != 'PASS' or row.get('run', {}).get('returncode') != 0
                or artifact.get('e_machine') != 183 or not artifact.get('path')
                or row.get('run', {}).get('live_arm_mapping_before_resume') is not True
                or digest(Path(artifact['path'])) != artifact.get('sha256')):
            raise RuntimeError('mandatory native row did not execute its qualified artifact')
        # Equal bytes at a replaced inode cannot reuse an earlier mapped-image
        # witness. Bind the current file and the sampled target to the same
        # complete ELF fact recorded before that test resumed.
        if (require_arm(artifact['path']) != artifact
                or architecture.get('arm_targets', {}).get(row['suite']) != artifact):
            raise RuntimeError('mandatory native artifact or mapping identity changed')
    return {'required_rows': len(expected), 'observed_labels': sorted(expected),
            'nested_sha256': digest(nested)}


def validate_wasip1_native(document, directory, manifest_sha256):
    """Require all17 current L01 native rows and their actual ARM witnesses."""
    expected = {'wasip1-configuration', 'wasip1-authority32', 'wasip1-authority64',
                'wasip1-positive-hybrid-mt', 'wasip1-positive-hybrid-mixed-stress'}
    expected.update('wasip1-positive-'+name+suffix
                    for name in ('clock_res_get', 'clock_time_get', 'random_get',
                                 'proc_raise', 'sched_yield', 'poll_oneoff')
                    for suffix in ('', '_wasm64'))
    return validate_native_rows(document, directory, manifest_sha256, expected, True)


def validate_search_contracts(document, directory, manifest_sha256, source):
    """Require both current native policy contracts and their actual MD inputs."""
    proof = validate_native_rows(document, directory, manifest_sha256, set(SEARCH_CONTRACT_ROWS),
                                 True, 'source_compiler_closure')
    for row in document['suites']:
        relative, standard = SEARCH_CONTRACT_ROWS[row['suite']]
        dependency = row.get('dependencies', {})
        object_fact = row.get('object_elf', {})
        if (row.get('original_source') != relative or row.get('standard') != standard
                or dependency.get('scope') != 'Actual -MD project+prequalified system dependencies'
                or dependency.get('project_inputs', {}).get(relative) !=
                    {'sha256':digest(source/relative), 'bytes':(source/relative).stat().st_size}
                or not dependency.get('path') or digest(dependency['path']) != dependency.get('sha256')
                or not object_fact.get('path') or require_arm(object_fact['path']) != object_fact
                or row.get('build', {}).get('returncode') != 0 or row.get('link', {}).get('returncode') != 0):
            raise RuntimeError('current search contract compiler/source/object/dependency proof is incomplete')
    return proof


def validate_original_regression(document, profile, worker, manifest_sha256, addon_manifest_sha256,
                                 source, config, config_sha256, wat2wasm):
    """Admit nine existing commands only with four independently mapped launches.

    This is the existing WASI/Trusted-DL/tail-call subset. It does not establish
    the complete historic interpreter/JIT/checkpoint regression requirement.
    """
    expected = {'wasi_preview1_compile', 'wasi_preview1', 'wasi_memory64_compile',
                'wasi_memory64', 'runtime_tail_compile', 'runtime_tail',
                'trusted_dl_compile', 'trusted_dl_guest_compile', 'trusted_dl'}
    rows = document.get('original_results', {}).get('result', {}).get('tests', [])
    launches = document.get('launches', [])
    targets = document.get('arm_targets', {})
    samples = document.get('actual_process_samples', [])
    validate_architecture_mappings(document)
    if (document.get('status') != 'PASS' or document.get('profile') != profile
            or document.get('source_manifest_sha256') != manifest_sha256
            or document.get('source_addon_manifest_sha256') != addon_manifest_sha256
            or document.get('input_worker') != str(worker)
            or document.get('input_worker_sha256') != digest(worker)
            or document.get('config_sha256') != config_sha256
            or document.get('original_driver_sha256') != digest(source/'tests/ros/regression.py')
            or document.get('same_cgroup_incarnation') is not True
            or document.get('remaining_owned_processes') != []
            or len(rows) != 9 or {row.get('name') for row in rows} != expected
            or any(row.get('status') != 'PASS' or row.get('returncode') != 0 for row in rows)
            or len(launches) != 4 or {row.get('name') for row in launches} !=
                {'regression-launch-'+str(index)+'/' for index in range(4)}
            or set(targets) != {'worker', 'trusted-dl-plugin'}):
        raise RuntimeError('existing regression inventory/profile/source/execution proof is incomplete')
    for label, artifact in targets.items():
        if require_arm(artifact['path']) != artifact:
            raise RuntimeError('existing regression mapped artifact identity changed: '+label)
    if targets['worker']['sha256'] != digest(worker):
        raise RuntimeError('existing regression executed a different Worker snapshot')
    original = document['original_results']
    if (digest(original['path']) != original.get('sha256')
            or json.loads(Path(original['path']).read_text()) != original['result']
            or original['result'].get('status') != 'PASS'):
        raise RuntimeError('existing regression raw result bytes differ from their recorded binding')
    commands = document.get('original_commands') or {}
    output = Path(original['path']).parent
    dependency = output.parent/'trusted-dl.d'
    compile_prefix = [*config['compiler'], *config['compile_flags'], '-x', 'c', '-MMD', '-MF', str(dependency)]
    expected_commands = expected_regression_commands(targets['worker']['path'], output, source, wat2wasm, compile_prefix)
    if (commands.get('path') != str(output/'commands.json')
            or commands.get('wat2wasm') != wat2wasm or commands.get('compile_prefix') != compile_prefix
            or commands.get('commands') != expected_commands
            or digest(commands['path']) != commands.get('sha256')
            or json.loads(Path(commands['path']).read_text()) != expected_commands):
        raise RuntimeError('existing regression original command recipes/binding changed or were omitted')
    for index, launch in enumerate(launches):
        required = ['worker', 'trusted-dl-plugin'] if index == 3 else ['worker']
        observed = {label for sample in samples if sample.get('name', '').startswith(launch['name'])
                    for label in sample.get('mapped_arm_targets', [])}
        command = expected_commands[(1, 3, 5, 8)[index]]
        if (launch.get('name') != 'regression-launch-'+str(index)+'/'
                or launch.get('required') != required or not set(required) <= observed
                or launch.get('original_command') != command
                or launch.get('qemu_command') != [*config['qemu_argv'], *command]):
            raise RuntimeError('existing regression launch lacks its exact native mapping witnesses')
    return {'profile': profile, 'required_commands': 9, 'required_arm_launches': 4}


def validate_wasip1_cli(directory, source):
    """Require the exact140 recipes,32 WAT compilations and immutable Guest inputs.

    The original CLI runner retains WAT text and checks the produced binary hash
    before and after every invocation. This verifier rejects a PASS label that
    omitted the new recipes. Binary references may outlive temporary paths via
    their recorded hashes; this is not a second compilation of their source.
    """
    import importlib.util
    specification = importlib.util.spec_from_file_location('qualified_wasip1_cli_recipes',
        source/'tests/wasip1_default_deny/cli_cases.py')
    recipes = importlib.util.module_from_spec(specification)
    specification.loader.exec_module(recipes)
    path = directory/'cli-results.json'
    data = json.loads(path.read_text())
    rows = data.get('cases', [])
    indexed = {row.get('name'): row for row in rows}
    if data.get('status') != 'PASS' or len(indexed) != len(rows) or any(row.get('status') != 'PASS' for row in rows):
        raise RuntimeError('complete CLI report is missing, failed or has duplicate rows')
    benign = indexed.get('benign-wasi-fixture-binding', {})
    bindings = {str(Path(benign['binding']['artifact_path']).resolve()): benign['binding']}
    compiled = {}
    for width in (32, 64):
        for name, text in recipes.fixtures(width).items():
            row = indexed.get(f'wasi{width}-{name}-compile', {})
            expected_sha = hashlib.sha256(text.encode()).hexdigest()
            binding = row.get('binding', {})
            if (row.get('returncode') != 0 or row.get('source_text') != text
                    or row.get('source_sha256') != expected_sha
                    or binding.get('source_sha256') != expected_sha
                    or not re.fullmatch('[a-f0-9]{64}', binding.get('artifact_sha256', ''))
                    or not 0 < binding.get('artifact_bytes', 0) <= 1048576):
                raise RuntimeError('mandatory WASI compilation/input binding is incomplete')
            artifact = Path(binding['artifact_path'])
            compiled[(width, name)] = artifact
            bindings[str(artifact.resolve())] = binding
    required = recipes.cases(Path(benign['binding']['artifact_path']), compiled)
    if len(compiled) != 32 or len(required) != 140:
        raise RuntimeError('mandatory WASI CLI inventory changed without qualification')
    for name, command, success in required:
        row = indexed.get(name, {})
        key = str(Path(command[command.index('--run')+1]).resolve())
        if (row.get('command') != command or (row.get('returncode') == 0) != success
                or row.get('generated_fixture_before') != bindings[key]
                or row.get('generated_fixture_after') != bindings[key]):
            raise RuntimeError('mandatory WASI CLI row was omitted, changed or lacked immutable inputs')
    return {'required_rows': len(required), 'compiled_wat_fixtures': len(compiled),
            'results_sha256': digest(path)}


def validate_product_source(worker_report, gateway_report, manifest_sha256):
    """Bind qualified product reports to the exact frozen acceptance source."""
    profiles = worker_report.get('profiles', [])
    if (worker_report.get('status') != 'PASS' or gateway_report.get('status') != 'PASS'
            or len(gateway_report.get('targets', {})) != 18
            or len(profiles) != 4
            or {row.get('profile') for row in profiles} != {'none', 'ros1', 'ros2', 'dual'}
            or any(row.get('status') != 'PASS' for row in profiles)):
        raise RuntimeError('four qualified Workers and eighteen qualified ARM native targets required')
    for name, report in (('Worker', worker_report), ('Gateway', gateway_report)):
        if report.get('source_manifest_sha256') != manifest_sha256:
            raise RuntimeError(name+' products are not qualified for the exact acceptance source manifest')


def validate_fresh_worker_inputs(worker_report, source, frozen):
    """Require newly compiled Workers consuming the current WASIp1 authority code.

    Earlier dependency reuse qualifications are valid historical artifacts, but
    cannot qualify this source after its default authority and CLI changes.
    Bind each of the three real compile commands to its raw dependency file,
    exact current source bytes, and ELF183 object before accepting the product.
    """
    if any(key in worker_report for key in ('original_worker_report','reuse_dependency_closure',
                                            'original_compiler_dependency_records')):
        raise RuntimeError('current WASIp1 authority changes require four fresh Workers, not dependency-reused originals')
    if worker_report.get('builder_sha256')!=digest(Path(__file__).with_name('build_worker.py')):
        raise RuntimeError('fresh Worker builder does not match the current executed recipe')
    if (worker_report.get('artifact_helper_sha256') != digest(Path(__file__).with_name('runtime.py'))
            or worker_report.get('binding_helper_sha256') != digest(Path(__file__).with_name('gateway_qualification.py'))
            or worker_report.get('driver_inputs_preserved') is not True):
        raise RuntimeError('fresh Worker complete-image helper is absent, changed or not preserved')
    if (worker_report.get('same_cgroup_incarnation') is not True
            or not worker_report.get('cgroup_incarnation_before')
            or worker_report.get('cgroup_incarnation_before') != worker_report.get('cgroup_incarnation_after')
            or worker_report.get('cgroup_incarnation_before') != worker_report.get('cgroup_incarnation_after_before_probe')
            or worker_report.get('memory_swap_max_before') != '0' or worker_report.get('memory_swap_max_after') != '0'
            or worker_report.get('cgroup_before',{}).get('status') != 'PASS'
            or worker_report.get('cgroup_after',{}).get('status') != 'PASS'
            or worker_report.get('cgroup_before',{}).get('effective_memory_max') != 68719476736
            or worker_report.get('cgroup_after',{}).get('effective_memory_max') != 68719476736):
        raise RuntimeError('fresh Worker compilation cgroup incarnation differs or is unavailable')
    valid,deltas = cgroup_oom_deltas(worker_report['cgroup_before'],worker_report['cgroup_after'],True)
    if not valid or deltas != worker_report.get('memory_events'):
        raise RuntimeError('fresh Worker original compilation zero-OOM counters differ or failed')
    if worker_report.get('tools_preserved') is not True:
        raise RuntimeError('fresh Worker compiler/QEMU input preservation is unavailable')
    for label in ('compiler','qemu'):
        item = worker_report.get(label,{})
        if (not item.get('argv') or not item.get('input')
                or item['input'].get('path') != item['argv'][0]
                or item.get('sha256') != item['input'].get('sha256')
                or read_file_binding(item['argv'][0]) != item['input']):
            raise RuntimeError('fresh Worker compiler/QEMU bytes or opened identity changed: '+label)
    required = {
        'src/uwvm2/imported/wasi/wasip1/environment/environment.h',
        'src/uwvm2/uwvm/imported/wasi/wasip1/storage/env.h',
        'src/uwvm2/uwvm/cmdline/params/impl.h',
        'src/uwvm2/uwvm/cmdline/params/wasip1_permissions.h',
        'src/uwvm2/uwvm/cmdline/callback/impl.h',
        'src/uwvm2/uwvm/cmdline/callback/wasip1_permissions.h',
    }
    units = {'runtime':'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp',
             'main':'src/uwvm2/uwvm/main.default.cpp',
             'host_api':'src/uwvm2/uwvm/host_api.default.cpp'}
    source = Path(source).resolve(strict=True);bindings = {}
    for row in worker_report['profiles']:
        binary = Path(row['binary']['path']);directory = binary.parent
        if (directory.name!=row['profile'] or set(row.get('dependencies',{}))!=set(units)
                or set(row.get('objects',{})) != set(units)):
            raise RuntimeError('fresh Worker profile/unit output layout differs')
        commands = row.get('commands',[])
        if len(commands)!=5 or any(record.get('returncode')!=0 for record in commands):
            raise RuntimeError('three fresh compiles, link and actual ARM version invocation required per Worker')
        consumed = set();records = {}
        for index,(unit,relative) in enumerate(units.items()):
            command = commands[index].get('command',[]);raw = directory/(unit+'.d')
            try:
                valid = (command[command.index('-c')+1]==str(source/relative)
                         and command[command.index('-MF')+1]==str(raw)
                         and command[command.index('-o')+1]==str(directory/(unit+'.o'))
                         and '-MMD' in command)
            except (ValueError,IndexError):valid = False
            if not valid or not raw.is_file():
                raise RuntimeError('fresh Worker compile/raw dependency binding differs: '+row['profile']+'/'+unit)
            text = raw.read_text().replace('\\\n',' ')
            names = {str(Path(os.path.normpath(name)).relative_to(source)) for name in
                     re.findall(re.escape(str(source))+r'/[^\s\\]+',text)}
            recorded = row['dependencies'][unit]
            if not names or names!=set(recorded) or relative not in names:
                raise RuntimeError('fresh Worker raw dependency exactset differs: '+row['profile']+'/'+unit)
            for name,sha in recorded.items():
                expected = frozen.get(name)
                expected = expected.get('sha256') if isinstance(expected,dict) else expected
                if expected!=sha or digest(source/name)!=sha:
                    raise RuntimeError('fresh Worker did not consume the current source byte: '+name)
            obj = require_arm(directory/(unit+'.o'))
            if obj != row['objects'][unit]:
                raise RuntimeError('fresh Worker original compiled object identity changed: '+row['profile']+'/'+unit)
            consumed.update(names)
            records[unit] = {'path':str(raw),'sha256':digest(raw),'project_input_count':len(names),'object':obj}
        if not required.issubset(consumed):
            raise RuntimeError('fresh Worker dependencies omit current WASIp1 authority/CLI inputs: '+row['profile'])
        bindings[row['profile']] = {'raw_compile_dependencies':records,
                                   'authority_inputs':{name:row['dependencies'][next(unit for unit in units if name in row['dependencies'][unit])][name]
                                                       for name in sorted(required)}}
    return bindings


def validate_action_qos_inputs(path, manifest_sha256):
    """Bind all four mandatory extra products to the current source and recipe."""
    path = Path(path).resolve(strict=True);report, binding = read_json_binding(path)
    validate_prepared_contract(report,manifest_sha256)
    products = {}
    for label in ACTION_QOS_PROBES:
        row = report['probes'][label];actual = require_arm(row['binary']['path'])
        if actual!=row['binary']:
            raise RuntimeError('mandatory Action QoS product identity changed: '+label)
        products[label] = actual
    return {**binding,'products':products}


def validate_feature_preparation(path, manifest_sha256, module, sdk):
    """Bind exactly two feature products to their qualified current preparation."""
    path = Path(path).resolve(strict=True)
    report, binding = read_json_binding(path)
    module.validate_prepared_contract(report,manifest_sha256)
    pins = [binding]
    architecture, architecture_pin = architecture_binding(report['architecture_evidence']['path'],pins)
    if architecture_pin['sha256'] != report['architecture_evidence']['sha256']:
        raise RuntimeError('current feature original compilation architecture bytes changed')
    module.validate_compilation_architecture(report,architecture,Path(sdk),pins)
    products = {}
    for label in module.PROBES:
        recorded = report['probes'][label]['binary']
        actual = require_arm(recorded['path'])
        if actual != recorded:
            raise RuntimeError('current feature prepared product identity changed: '+label)
        products[label] = actual
    return {**binding,'products':products,'input_pins':pins}


def validate_feature_execution(document, directory, manifest_sha256, module, preparation, gateway_binding):
    """A behavior PASS must retain both compiled inputs and actual ARM witnesses."""
    behavior = {row.get('name'):row for row in document.get('commands',[])
                if row.get('name','').startswith('behavior-')}
    expected = {'behavior-'+name for name in module.PROBES}
    preserved = ('source_preserved','sdk_preserved','driver_inputs_preserved',
                 'prepared_inputs_preserved','gateway_inputs_preserved','tools_preserved')
    native_status = document.get('native_behavior_status',document.get('actual_behavior_status'))
    if (document.get('status') != 'PASS' or document.get('mode') != 'execute'
            or native_status != 'PASS' or document.get('actual_native_behavior_probes') != 2
            or document.get('source_manifest_sha256') != manifest_sha256
            or document.get('probe_contracts') != module.PROBE_CONTRACTS
            or set(document.get('probes',{})) != set(module.PROBES) or set(behavior) != expected
            or sum(row.get('name','').startswith('behavior-') for row in document.get('commands',[])) != 2
            or not all(document.get(key) is True for key in preserved)
            or document.get('same_cgroup_incarnation') is not True
            or not document.get('cgroup_incarnation_before')
            or document.get('cgroup_incarnation_before') != document.get('cgroup_incarnation_after')
            or document.get('memory_swap_max_before') != '0' or document.get('memory_swap_max_after') != '0'
            or document.get('remaining_owned_mounts') != [] or document.get('remaining_owned_processes') != []
            or not document.get('mount_cleanup')
            or any(row.get('returncode') != 0 for row in document['mount_cleanup'])):
        raise RuntimeError('current feature behavior inventory/preservation/cgroup/cleanup proof incomplete')
    nested = directory/'execution/architecture-evidence.json'
    pins = []
    architecture, nested_binding = architecture_binding(nested,pins)
    if document.get('architecture_evidence',{}).get('sha256') != nested_binding['sha256']:
        raise RuntimeError('current feature nested actual architecture evidence incomplete')
    membership = architecture['cgroup_before']['membership']
    samples = architecture.get('actual_process_samples',[])
    emulators = set(architecture.get('expected_host_emulator_sha256',[]))

    def stopped_proof(proof, row, label):
        """Correlate the stopped PID/birth/map proof with the same launch sample."""
        actual, sample = proof.get('actual_stopped_ARM',{}), proof.get('target_proof',{})
        ancestry = proof.get('ancestry',[])
        target = architecture.get('arm_targets',{}).get(label,{})
        mapped = validate_mapping_receipts(sample, label, target)
        mapped = mapped and sample.get('opened_target_mappings',{}).get('identity_after',{}).get('state') in ('T','t')
        if (proof.get('current_mapping_check') is not True or actual.get('state') not in ('T','t')
                or any(type(value) is not int or value <= 0 for value in
                       (actual.get('pid'),actual.get('parent_pid'),actual.get('starttime_ticks'),
                        row.get('pid'),row.get('launcher_starttime_ticks')))
                or len(set(ancestry)) != len(ancestry)
                or not ancestry or ancestry[0] != row.get('pid') or ancestry[-1] != actual.get('pid')
                or sample not in samples or sample.get('name') != row['name']+'/'
                or sample.get('ancestry') != ancestry or sample.get('pid') != actual.get('pid')
                or sample.get('starttime_ticks') != actual.get('starttime_ticks')
                or sample.get('parent_pid') != actual.get('parent_pid')
                or sample.get('live_cgroup_procs') is not True or sample.get('membership') != membership
                or label not in sample.get('mapped_arm_targets',[]) or not mapped
                or sample.get('kernel_exe_elf',{}).get('e_machine') != 62
                or sample.get('kernel_exe_elf',{}).get('sha256') not in emulators
                or sample.get('execution_model') != 'QEMU user host process'):
            raise RuntimeError('current feature stopped PID/birth/cgroup/emulator/map proof differs: '+label)
        return actual, ancestry
    gateway = document.get('gateway_snapshot',document.get('gateway',{}))
    if (not gateway.get('path') or require_arm(gateway['path']) != gateway
            or gateway.get('sha256') != gateway_binding['binary']['sha256']):
        raise RuntimeError('current feature did not retain its exact qualified Gateway snapshot')
    for label, contract in module.PROBE_CONTRACTS.items():
        row = behavior['behavior-'+label]; product = document['probes'][label]['binary']
        actual = require_arm(product['path'])
        if (row.get('status') != 'PASS' or row.get('returncode') != 0
                or not row.get('actual_ARM_mapping_before_resume') or actual != product
                or product.get('sha256') != preparation['products'][label]['sha256']
                or architecture.get('arm_targets',{}).get(label) != product):
            raise RuntimeError('current feature fixture lacks qualified product or actual stopped ARM proof: '+label)
        fixture, ancestry = stopped_proof(row['actual_ARM_mapping_before_resume'],row,label)
        launcher = row['actual_ARM_mapping_before_resume'].get('launcher',{})
        if (launcher.get('pid') != row.get('pid') or launcher.get('starttime_ticks') != row.get('launcher_starttime_ticks')
                or launcher.get('state') in ('Z','X') or len(ancestry) not in (1,2)
                or (len(ancestry)==1 and fixture['starttime_ticks'] != row['launcher_starttime_ticks'])
                or (len(ancestry)==2 and (fixture['parent_pid'] != row['pid']
                                         or fixture['starttime_ticks'] < row['launcher_starttime_ticks']))):
            raise RuntimeError('current feature stopped fixture is not its owned launcher/direct child')
        raw, log_pin = read_file_binding(directory/(row['name']+'.log'),limit=16*1024**2,retain=True)
        pins.append(log_pin)
        lines = raw.decode('utf-8',errors='replace').splitlines();marker = contract['marker']
        selected = [line for line in lines if line==marker or
                    (line.startswith(marker) and (marker.endswith(' ') or
                     marker.endswith('checks=') and line[len(marker):].isdecimal() and int(line[len(marker):])>0))]
        if (log_pin['sha256'] != row.get('log_sha256') or len(selected) != 1
                or any(line.startswith('FAIL ') for line in lines)):
            raise RuntimeError('current feature exact unique semantic completion marker/log binding differs: '+label)
        if contract.get('gateway_required',contract.get('requires_gateway',False)):
            proof = row.get('actual_gateway_ARM_mapping_before_resume') or {}
            targets = {name for name,fact in architecture.get('arm_targets',{}).items() if fact == gateway}
            if len(targets) != 1:
                raise RuntimeError('current feature lacks live stopped qualified Gateway mapping proof')
            stopped, gateway_ancestry = stopped_proof(proof,row,next(iter(targets)))
            if (gateway_ancestry != ancestry+[stopped['pid']] or stopped['parent_pid'] != fixture['pid']
                    or stopped['starttime_ticks'] < fixture['starttime_ticks']):
                raise RuntimeError('current feature Gateway is not the proven fixture direct child')
    return {'mandatory_behavior_probes':2,'nested_sha256':nested_binding['sha256'],'input_pins':pins,
            'products':{label:document['probes'][label]['binary'] for label in module.PROBES},'gateway':gateway}


def validate_mandatory_product_pool(path, qualified_paths):
    """A legacy or substituted pool cannot omit any of the current 30 products."""
    expected = {str(Path(value).resolve(strict=True)) for value in qualified_paths}
    actual = set(json.loads(Path(path).read_text()).get('artifacts',{}))
    if len(expected)!=30 or actual!=expected:
        raise RuntimeError('full current acceptance requires the exact thirty qualified product paths')


def validate_native_views(sdk, gateway_report):
    kinds = {1: ('gateway', 'native-probe', 'resources-probe', 'tf-probe', 'action-probe',
                 'perf-probe', 'ipc-service-probe', 'five-module-probe'),
             2: ('gateway', 'native-probe', 'resources-probe', 'tf-probe', 'action-probe',
                 'lifecycle-probe', 'secure-probe', 'perf-probe', 'ipc-service-probe', 'five-module-probe')}
    expected = {f'uwvm-ros{version}-{kind}' for version, names in kinds.items() for kind in names}
    if set(gateway_report['targets']) != expected:
        raise RuntimeError('qualified native target names differ from the eighteen original products')
    bindings = {}
    for version, distro in ((1, 'noetic'), (2, 'jazzy')):
        canonical = Path(sdk)/'gateway'/(distro+'-rootfs')/'tmp'/f'ros{version}-build'
        for kind in kinds[version]:
            name = f'uwvm-ros{version}-{kind}'
            row = gateway_report['targets'][name]
            selected = (canonical/name).resolve(strict=True)
            qualified = Path(row['path']).resolve(strict=True)
            if selected != qualified:
                raise RuntimeError('canonical native view selects an unqualified product: '+name)
            actual = require_arm(selected)
            if actual != row:
                raise RuntimeError('canonical native view identity mismatch: '+name)
            bindings[name] = {'canonical': str(canonical/name), 'qualified': str(qualified),
                              'sha256': actual['sha256'], 'device': actual['device'], 'inode': actual['inode']}
    return bindings


def workspace_directory_binding(path):
    """Bind directory identity without treating mutable timestamps as inputs.

    The owned workspace gains actual logs during execution. Its directory
    inode must stay the same, but those legitimate writes change timestamps.
    """
    path = Path(path); state = path.stat()
    if not path.is_dir():
        raise RuntimeError('native witness workspace must be a directory')
    return {'path':str(path), 'device':state.st_dev, 'inode':state.st_ino}


def bind_native_witness_logs(document, workspace_binding, bindings):
    """Open alias and physical logs while the private alias still exists.

    Original reports retain their original paths and hashes. This additional
    association never copies a log or fabricates a historical opened-file pin:
    both names must open the same regular inode with identical bounded bytes
    and metadata. Later consumers use the physical pin after alias cleanup.
    Only the source-owned native_witnesses field is traversed; arbitrary log
    names elsewhere in a JSON report do not gain a path-rewrite rule.
    """
    alias, origin = Path(workspace_binding['alias']), Path(workspace_binding['origin'])
    if (workspace_directory_binding(alias) != workspace_binding['alias_directory']
            or workspace_directory_binding(origin) != workspace_binding['origin_directory']
            or any(workspace_binding['alias_directory'][key] != workspace_binding['origin_directory'][key]
                   for key in ('device','inode'))):
        raise RuntimeError('native witness workspace alias identity changed')
    pending = [document]
    while pending:
        value = pending.pop()
        if isinstance(value, list):
            pending.extend(value)
        elif isinstance(value, dict):
            if 'native_witnesses' in value:
                witnesses = value['native_witnesses']
                if not isinstance(witnesses, list):
                    raise RuntimeError('native witness inventory must be a list')
                for witness in witnesses:
                    if not isinstance(witness, dict) or not isinstance(witness.get('log'), dict):
                        raise RuntimeError('native witness lacks its original log record')
                    old = witness['log']; selected = old.get('path')
                    if (not isinstance(selected, str) or not selected or '\0' in selected
                            or not Path(selected).is_absolute() or os.path.normpath(selected) != selected
                            or type(old.get('bytes')) is not int or not 0 <= old['bytes'] <= 16*1024**2
                            or not isinstance(old.get('sha256'), str)
                            or re.fullmatch('[a-f0-9]{64}', old['sha256']) is None):
                        raise RuntimeError('native witness original log pin is malformed')
                    alias_path = Path(selected)
                    try:
                        relative = alias_path.relative_to(alias)
                    except ValueError:
                        raise RuntimeError('native witness log is outside the bound workspace alias')
                    physical = (origin/relative).resolve(strict=True)
                    if not physical.is_relative_to(origin):
                        raise RuntimeError('native witness log escapes the physical workspace')
                    alias_pin = read_file_binding(alias_path, limit=16*1024**2)
                    physical_pin = read_file_binding(physical, limit=16*1024**2)
                    if (alias_pin['sha256'] != old['sha256'] or alias_pin['bytes'] != old['bytes']
                            or {key:item for key,item in alias_pin.items() if key != 'path'} !=
                               {key:item for key,item in physical_pin.items() if key != 'path'}):
                        raise RuntimeError('native witness alias/physical/original log identity differs')
                    entry = {'alias_path':selected, 'alias_log':alias_pin, 'physical_log':physical_pin,
                        'alias_chain':[{'alias_root':str(alias), 'origin_root':str(origin),
                                        'alias_log':alias_pin, 'physical_log':physical_pin}],
                        'producer_reports':[]}
                    if selected in bindings and bindings[selected] != entry:
                        raise RuntimeError('duplicate native witness log association changed')
                    bindings[selected] = entry
            pending.extend(value.values())


def compose_native_witness_logs(child, child_report_pin, native_document, workspace_binding, bindings, manifest_sha256):
    """Extend a child wrapper's already-opened alias chain without rewriting it.

    The child has retired its own shorter alias. Its captured physical name is
    our still-live alias, so reopen that exact pinned inode and then open the
    final canonical workspace name. The first alias pin is retained as actual
    child evidence, never reconstructed from the surviving physical file.
    """
    child_binding = child.get('workspace_alias_binding', {})
    incoming = workspace_binding['alias']
    if (child.get('status') != 'PASS' or child.get('source_manifest_sha256') != manifest_sha256
            or child.get('native_witness_logs_preserved') is not True
            or child.get('workspace_origin') != incoming
            or set(child_binding) != {'alias','origin','alias_directory','origin_directory'}
            or child_binding['alias'] not in ('/gp64','/pg64','/rg64','/gr64')
            or child_binding['origin'] != incoming
            or child_binding['origin_directory'] != workspace_directory_binding(incoming)
            or set(child_binding['alias_directory']) != {'path','device','inode'}
            or child_binding['alias_directory']['path'] != child_binding['alias']
            or any(child_binding['alias_directory'][key] != child_binding['origin_directory'][key]
                   for key in ('device','inode'))):
        raise RuntimeError('nested native witness alias producer is not the exact current wrapper')
    child_logs = child.get('native_witness_log_bindings')
    if not isinstance(child_logs, dict):
        raise RuntimeError('nested native witness alias inventory is missing')
    expected, pending = {}, [native_document]
    while pending:
        value = pending.pop()
        if isinstance(value, list): pending.extend(value)
        elif isinstance(value, dict):
            if 'native_witnesses' in value:
                if not isinstance(value['native_witnesses'], list):
                    raise RuntimeError('nested native witness original inventory is malformed')
                for witness in value['native_witnesses']:
                    old = witness['log']; selected = old['path']
                    if selected in expected and expected[selected] != old:
                        raise RuntimeError('nested native witness original duplicate changed')
                    expected[selected] = old
            pending.extend(value.values())
    if set(expected) != set(child_logs):
        raise RuntimeError('nested native witness captured inventory differs from actual adapter document')
    for selected, entry in child_logs.items():
        if (not isinstance(entry, dict)
                or set(entry) != {'alias_path','alias_log','physical_log','alias_chain','producer_reports'}
                or selected != entry['alias_path'] or entry['producer_reports'] != []
                or not isinstance(entry['alias_chain'], list) or len(entry['alias_chain']) != 1):
            raise RuntimeError('nested native witness alias entry differs')
        step = entry['alias_chain'][0]
        if (set(step) != {'alias_root','origin_root','alias_log','physical_log'}
                or step['alias_root'] != child_binding['alias'] or step['origin_root'] != incoming
                or step['alias_log'] != entry['alias_log'] or step['physical_log'] != entry['physical_log']
                or entry['alias_log'].get('path') != selected
                or entry['alias_log'].get('sha256') != expected[selected]['sha256']
                or entry['alias_log'].get('bytes') != expected[selected]['bytes']
                or {key:item for key,item in entry['alias_log'].items() if key != 'path'} !=
                   {key:item for key,item in entry['physical_log'].items() if key != 'path'}):
            raise RuntimeError('nested native witness captured chain differs')
        relative = Path(selected).relative_to(child_binding['alias'])
        current_path = Path(incoming)/relative
        if (entry['physical_log'].get('path') != str(current_path)
                or read_file_binding(current_path, limit=16*1024**2) != entry['physical_log']):
            raise RuntimeError('nested native witness physical input changed before composition')
        extra = {}
        bind_native_witness_logs({'native_witnesses':[{'log':entry['physical_log']}]},workspace_binding,extra)
        outer = extra[str(current_path)]
        combined = {'alias_path':selected, 'alias_log':entry['alias_log'], 'physical_log':outer['physical_log'],
            'alias_chain':[step,*outer['alias_chain']], 'producer_reports':[child_report_pin]}
        if selected in bindings and bindings[selected] != combined:
            raise RuntimeError('nested native witness duplicate chain changed')
        bindings[selected] = combined


def preserved_native_witness_logs(bindings):
    """Reopen only the final physical names after all private aliases retire."""
    return all(read_file_binding(entry['physical_log']['path'], limit=16*1024**2) == entry['physical_log']
               and pins_preserved(entry['producer_reports']) for entry in bindings.values())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('workspace', 'sdk-workspace', 'source', 'config', 'worker-build',
                 'gateway-build', 'prepared-resources', 'binfmt-evidence', 'out'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--qemu', type=Path, required=True)
    parser.add_argument('--wat2wasm', default='/usr/bin/wat2wasm')
    parser.add_argument('--source-addon', type=Path, required=True)
    parser.add_argument('--source-addon-manifest', type=Path, required=True)
    parser.add_argument('--prepared-action-qos', type=Path, required=True,
                        help='Fresh current-source four-probe Action response/cancellation preparation report')
    for name in FEATURE_DRIVERS:
        parser.add_argument('--prepared-'+name,type=Path,required=True,
                            help='Exact current two-product compilation PREPARED report for '+name)
    for version in (1,2):
        parser.add_argument('--ros'+str(version)+'-gateway-report',type=Path,required=True,
                            help='Fresh strict targeted -MD Gateway report selected by the current native wrapper')
    parser.add_argument('--container-started-at', required=True,
                        help='Exact current Docker State.StartedAt for the nested mandatory QoS execution')
    parser.add_argument('--topic-observers-manifest', type=Path,
                        help='Qualified test-owned ARM discovery observers for multi-memory and original topic readiness')
    parser.add_argument('--performance-cache-sdk-environment', action='store_true',
                        help='Capture the real SDK setup environment before performance observers start')
    parser.add_argument('--network-cache-sdk-environment', action='store_true',
                        help='Capture the real SDK environment before original network observers start')
    parser.add_argument('--rpc-denial-gate', action='store_true',
                        help='Keep the two denial Guests alive after assertions until native observation completes')
    parser.add_argument('--suites', nargs='+', default=['native', 'multi-memory', 'network', 'rpc',
        'five-module', 'additional-security', 'ingress-trap', 'revoke-race', 'flood',
        'payload-security', 'performance', 'current-resources', 'action-qos',
        'wasip1-default-deny', 'cli', 'original-units', 'ros1-search-contracts',
        *('regression-'+profile for profile in REGRESSION_PROFILES)])
    args = parser.parse_args()
    # Supplementary acceptance invocations also retain this mandatory regression;
    # omitting its suite cannot make an old eighteen-product subset pass.
    for mandatory in MANDATORY_SUITES:
        if mandatory not in args.suites:
            args.suites.append(mandatory)
    if os.readlink('/proc/self/ns/mnt') == os.readlink(f'/proc/{os.getppid()}/ns/mnt'):
        raise RuntimeError('launch this runner with unshare -m inside the temporary binfmt lease')
    work, sdk, source = args.workspace.resolve(), args.sdk_workspace.resolve(), args.source.resolve()
    out = args.out.resolve()
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    drivers = Path(__file__).resolve().parent
    config = json.loads(args.config.read_text())
    config_sha256 = digest(args.config)
    worker_report, worker_report_binding = read_json_binding(args.worker_build/'report.json')
    gateway_report, gateway_report_binding = read_json_binding(args.gateway_build/'build-report.json')
    manifest = source.parent/'source-manifest.json'
    manifest_sha256 = digest(manifest)
    validate_product_source(worker_report, gateway_report, manifest_sha256)
    frozen = json.loads(manifest.read_text())
    # Current feature drivers bind their executed helpers to this same source
    # manifest. Old external driver overlays remain historical artifacts.
    if drivers != source/'tests/ros_aarch64':
        raise RuntimeError('current acceptance must execute drivers inside the exact frozen source tree')
    for path in drivers.glob('*.py'):
        name = str(path.relative_to(source))
        expected = frozen.get(name)
        if (not isinstance(expected, dict) or digest(path) != expected.get('sha256')
                or path.stat().st_size != expected.get('bytes')):
            raise RuntimeError('executed controller is absent from the complete source manifest: '+name)
    addon_manifest_sha256 = digest(args.source_addon_manifest)
    addon_frozen = json.loads(args.source_addon_manifest.read_text())
    for name, row in addon_frozen.items():
        expected = row['sha256'] if isinstance(row, dict) else row
        if digest(args.source_addon/name) != expected:
            raise RuntimeError('frozen regression addon mismatch: '+name)
    fresh_worker_bindings = validate_fresh_worker_inputs(worker_report,source,frozen)
    action_qos_binding = validate_action_qos_inputs(args.prepared_action_qos,manifest_sha256)
    feature_bindings = {name:validate_feature_preparation(
        getattr(args,'prepared_'+name.replace('-','_')),manifest_sha256,module,
        sdk/'gateway'/('noetic-rootfs' if name=='ros1-search' else 'jazzy-rootfs'))
        for name,module in FEATURE_DRIVERS.items()}
    gateway_bindings = {}
    for version,distro in ((1,'noetic'),(2,'jazzy')):
        child_report = getattr(args,'ros'+str(version)+'_gateway_report')
        target = gateway_report['targets']['uwvm-ros'+str(version)+'-gateway']
        qualified = qualify_gateway(target['path'],child_report,manifest,source,
                                     sdk/'gateway'/(distro+'-rootfs'),version)
        if gateway_report.get('targeted_reports',{}).get(str(version)) != qualified['report']:
            raise RuntimeError('native wrapper did not select the exact strict Gateway report')
        gateway_bindings[version] = qualified
    native_base = qualify_base(gateway_report['base_report']['path'],manifest,source,sdk)
    combined_targets, combined_methods = assemble_native(native_base,gateway_bindings)
    if (gateway_report['base_report'] != native_base['report']
            or gateway_report['targets'] != combined_targets
            or gateway_report.get('qualified_source_dependency_methods') != combined_methods):
        raise RuntimeError('native wrapper differs from independent current base/targeted qualifications')
    worker = Path(next(row['binary']['path'] for row in worker_report['profiles']
                       if row['profile'] == 'dual'))
    spec = importlib.util.spec_from_file_location('arm_acceptance_cgroup', source/'tests/ros_cgroup/probe.py')
    probe_module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(probe_module)
    before, baseline_identity = pinned_cgroup_probe(probe_module.probe)
    membership = before['membership']
    cgroup_path = Path(before['cgroup_ancestors'][0]['path'])
    lease = json.loads(args.binfmt_evidence.read_text())
    entry = Path('/proc/sys/fs/binfmt_misc')/lease['name']
    if os.environ.get('UWVM_ROS_ARM_BINFMT_ENTRY') != lease['name'] or entry.read_text() != lease['entry'] or digest(args.qemu) != lease['qemu_sha256']:
        raise RuntimeError('active verified temporary QEMU lease required')
    for name, row in frozen.items():
        if digest(source/name) != row['sha256']:
            raise RuntimeError('frozen source mismatch: '+name)
    for row in worker_report['profiles']:
        if row['status'] != 'PASS' or require_arm(row['binary']['path']) != row['binary']:
            raise RuntimeError('Worker qualification mismatch')
    for name, row in gateway_report['targets'].items():
        if require_arm(row['path']) != row:
            raise RuntimeError('Gateway target qualification mismatch: '+name)
    native_bindings = validate_native_views(sdk, gateway_report)
    state = {'status': 'RUNNING', 'source_manifest_sha256': digest(manifest), 'cgroup_before': before,
        'cgroup_incarnation': baseline_identity,
        'worker_build': worker_report_binding,
        'gateway_build': gateway_report_binding,
        'driver_sha256': digest(__file__), 'driver_inputs': {str(p.name): digest(p) for p in drivers.glob('*.py')},
        'canonical_native_bindings': native_bindings,
        'fresh_worker_compile_bindings':fresh_worker_bindings,
        'mandatory_action_qos_preparation':action_qos_binding,
        'mandatory_product_counts':{'Workers':4,'original_native':18,'action_qos':4,
                                    'ROS1_features':2,'ROS2_features':2,'total_distinct':30},
        'mandatory_feature_preparations':feature_bindings,
        'native_base_qualification':native_base,
        'strict_current_gateways':gateway_bindings,
        'mandatory_suites': list(MANDATORY_SUITES),
        'source_addon': {'path':str(args.source_addon),
                         'manifest_sha256':addon_manifest_sha256},
        'config_sha256':config_sha256,
        'suites': [], 'mounts': [], 'binfmt_lease_at_start': lease,
        'workspace_origin':str(work.resolve(strict=True)), 'native_witness_log_bindings':{},
        'native_witness_log_binding_document_pins':[], 'native_witness_log_bindings_preserved':False,
        'native_witness_logs_preserved':False}
    child_environment = dict(os.environ)
    child_environment['PYTHONDONTWRITEBYTECODE'] = '1'
    child_environment['PYTHONPYCACHEPREFIX'] = str(work/'task-cache')
    mounts, alias, active = [], None, None
    observed = {}

    def process_fact(pid):
        proc = Path('/proc')/str(pid)
        fields = (proc/'stat').read_text().rsplit(')', 1)[1].split()
        return {'pid': pid, 'state': fields[0], 'parent': int(fields[1]), 'group': int(fields[2]),
                'starttime': int(fields[19]), 'membership': (proc/'cgroup').read_text().splitlines()}

    def sample_tree(root):
        pending, seen = [root], set()
        while pending:
            pid = pending.pop()
            if pid in seen:
                continue
            seen.add(pid)
            try:
                fact = process_fact(pid)
                if fact['membership'] != membership:
                    raise RuntimeError('owned process left inherited cgroup: '+str(pid))
                observed[(pid, fact['starttime'])] = fact
                for task in (Path('/proc')/str(pid)/'task').iterdir():
                    try:
                        pending.extend(int(child) for child in (task/'children').read_text().split())
                    except OSError:
                        pass
            except (FileNotFoundError, ProcessLookupError):
                pass

    def live_owned():
        living = []
        for (pid, starttime), fact in observed.items():
            try:
                actual = process_fact(pid)
                if actual['starttime'] == starttime and actual['state'] != 'Z':
                    living.append(actual)
            except (FileNotFoundError, ProcessLookupError):
                pass
        return living

    def stop_owned():
        # Each Runner may create its own sessions. The sampled PID/starttime
        # tree, rather than one process group, determines cleanup ownership.
        cleanup = []
        for sig, grace in ((signal.SIGTERM, 10), (signal.SIGKILL, 5)):
            living = live_owned()
            for fact in reversed(living):
                descriptor = None
                try:
                    descriptor = os.pidfd_open(fact['pid'])
                    actual = process_fact(fact['pid'])
                    if actual['starttime'] != fact['starttime']:
                        continue
                    signal.pidfd_send_signal(descriptor, sig)
                    cleanup.append({'pid': fact['pid'], 'starttime': fact['starttime'], 'signal': int(sig)})
                except (ProcessLookupError, FileNotFoundError):
                    pass
                finally:
                    if descriptor is not None:
                        os.close(descriptor)
            deadline = time.monotonic()+grace
            while live_owned() and time.monotonic() < deadline:
                time.sleep(.05)
            if not live_owned():
                break
        return {'signals': cleanup, 'remaining': live_owned()}

    def save():
        (out/'report.json').write_text(json.dumps(state, indent=2)+'\n')

    def checked(command):
        subprocess.run(command, check=True, capture_output=True, timeout=30)

    def interrupted(signum, frame):
        raise RuntimeError('acceptance interrupted by signal '+str(signum))

    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    try:
        checked(['mount', '--make-rprivate', '/'])
        for candidate in ('/a64', '/ar6', '/r64', '/z64'):
            if not Path(candidate).exists():
                alias = Path(candidate)
                alias.mkdir(mode=0o700)
                break
        if alias is None:
            raise RuntimeError('no fresh short private workspace alias available')
        checked(['mount', '--bind', str(work), str(alias)])
        mounts.append(alias)
        state['workspace_alias_binding'] = {'alias':str(alias), 'origin':state['workspace_origin'],
            'alias_directory':workspace_directory_binding(alias),
            'origin_directory':workspace_directory_binding(work)}
        if any(state['workspace_alias_binding']['alias_directory'][key] !=
               state['workspace_alias_binding']['origin_directory'][key] for key in ('device','inode')):
            raise RuntimeError('private workspace alias is not the exact physical workspace')
        roots = {1: sdk/'gateway/noetic-rootfs', 2: sdk/'gateway/jazzy-rootfs'}
        for root in roots.values():
            for name in ('proc', 'sys', 'dev', 'work'):
                (root/name).mkdir(exist_ok=True)
            for name in ('proc', 'sys', 'dev'):
                checked(['mount', '--rbind', '/'+name, str(root/name)])
                mounts.append(root/name)
                checked(['mount', '--make-rprivate', str(root/name)])
            checked(['mount', '--bind', str(source), str(root/'work')])
            mounts.append(root/'work')
            checked(['mount', '-o', 'remount,bind,ro', str(root/'work')])
        checked(['mount', '--make-rprivate', '/'])
        state['mounts'] = [str(path) for path in mounts]
        state['active_mountinfo'] = Path('/proc/self/mountinfo').read_text().splitlines()
        qualified_products = ([row['binary']['path'] for row in worker_report['profiles']]+
                              [row['path'] for row in gateway_report['targets'].values()]+
                              [row['path'] for row in action_qos_binding['products'].values()]+
                              [row['path'] for item in feature_bindings.values() for row in item['products'].values()])
        pool = create(out/'artifact-pool',qualified_products+
                      [row['canonical'] for row in native_bindings.values()])
        validate_mandatory_product_pool(pool,qualified_products)
        state['artifact_pool'] = {'path': str(pool), 'sha256': digest(pool)}
        prefix = json.dumps(config['qemu_argv'])
        common = ['--source', str(source), '--workspace', str(alias), '--uwvm', str(worker),
            '--worker-prefix-json', prefix, '--binfmt-evidence', str(args.binfmt_evidence),
            '--wat2wasm', args.wat2wasm,
            '--ros1-rootfs', str(roots[1]), '--ros2-rootfs', str(roots[2]), '--artifact-pool', str(pool)]
        if args.topic_observers_manifest:
            common += ['--topic-observers-manifest', str(args.topic_observers_manifest)]
        stages = {
            'ros1-search-contracts': (['run_unit.py', '--source', str(source),
                '--manifest', str(manifest), '--config', str(args.config),
                '--suite', 'ros1-search-contracts'], 1800),
            'original-units': (['run_unit.py', '--source', str(source),
                '--manifest', str(manifest), '--config', str(args.config),
                '--suite', 'original'], 1800),
            'wasip1-default-deny': (['run_unit.py', '--source', str(source),
                '--manifest', str(manifest), '--config', str(args.config),
                '--suite', 'wasip1-default-deny'], 7200),
            'cli': (['run_cli.py', '--source', str(source), '--uwvm', str(worker),
                '--config', str(args.config), '--wat2wasm', args.wat2wasm], 3600),
            'native': (['run_native.py', '--source', str(source), '--workspace', str(sdk),
                '--qemu', str(args.qemu), '--binfmt-evidence', str(args.binfmt_evidence),
                '--artifact-pool', str(pool), '--suite', 'all'], 1800),
            'multi-memory': (['run.py', '--suite', 'multi-memory', *common], 180),
            'network': (['run.py', '--suite', 'network', *common, '--mode', 'all',
                         *(['--cache-native-sdk-environment'] if args.network_cache_sdk_environment else [])], 3600),
            'rpc': (['run_rpc_denial_gate.py' if args.rpc_denial_gate else 'run.py',
                     '--suite', 'rpc', *common, '--mode', 'all'], 5400),
            'current-resources': (['run_resources.py', '--mode', 'execute', '--reuse-prepared',
                str(args.prepared_resources/'manifest.json'), '--source', str(source),
                '--rootfs', str(roots[2]), '--qemu', str(args.qemu), '--binfmt-evidence', str(args.binfmt_evidence),
                '--container-started-at', args.container_started_at,
                '--gateway', str(roots[2]/'tmp/ros2-build/uwvm-ros2-gateway'),
                '--gateway-origin-report', str(args.gateway_build/'build-report.json')], 1200),
            'action-qos': (['run_action_qos.py','--mode','execute','--source',str(source),
                '--rootfs',str(roots[2]),'--qemu',str(args.qemu),'--binfmt-evidence',str(args.binfmt_evidence),
                '--container-started-at',args.container_started_at,
                '--prepared',str(args.prepared_action_qos)],600),
        }
        for version,name in ((1,'ros1-search'),(2,'ros2-local-parameter-events')):
            stages[name] = (['run_ros1_search.py' if version == 1 else 'run_local_parameter_events.py',
                '--mode','execute','--source',str(source),'--rootfs',str(roots[version]),
                '--qemu',str(args.qemu),'--binfmt-evidence',str(args.binfmt_evidence),
                '--container-started-at',args.container_started_at,
                '--prepared',str(getattr(args,'prepared_'+name.replace('-','_'))),
                '--gateway',gateway_bindings[version]['binary']['path'],
                '--gateway-report',str(getattr(args,'ros'+str(version)+'_gateway_report'))],1200)
        profile_workers = {row['profile']: Path(row['binary']['path']).resolve(strict=True)
                           for row in worker_report['profiles']}
        for profile in REGRESSION_PROFILES:
            stages['regression-'+profile] = (['run_regression.py', '--profile', profile,
                '--source', str(source), '--manifest', str(manifest),
                '--uwvm', str(profile_workers[profile]), '--config', str(args.config),
                '--source-addon', str(args.source_addon),
                '--source-addon-manifest', str(args.source_addon_manifest),
                '--wat2wasm', args.wat2wasm], 1800)
        for name in ('five-module', 'additional-security', 'ingress-trap', 'revoke-race', 'flood', 'payload-security', 'performance'):
            extra = ['--mode', 'all'] if name in ('additional-security', 'ingress-trap', 'revoke-race') else []
            if name == 'revoke-race':
                extra += ['--version', 'both']
            if name == 'performance' and args.performance_cache_sdk_environment:
                extra += ['--cache-sdk-environment']
            stages[name] = (['run_extended.py', '--suite', name, '--config', str(args.config),
                '--source-addon', str(args.source_addon), '--source-addon-manifest', str(args.source_addon_manifest),
                *common, *extra], 1200)
        save()
        for name in args.suites:
            arguments, timeout = stages[name]
            command = [sys.executable, str(drivers/arguments[0]), *arguments[1:], '--out', str(out/name)]
            if name=='action-qos' or name in FEATURE_DRIVERS:
                command = ['/usr/bin/unshare','-m',*command]
            row = {'suite': name, 'command': command, 'status': 'RUNNING', 'timeout': timeout}
            state['suites'].append(row)
            save()
            start = time.monotonic()
            observed.clear()
            with (out/(name+'.log')).open('wb') as log:
                active = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT,
                                          env=child_environment, start_new_session=True)
            row['pid'] = active.pid
            row['membership'] = Path(f'/proc/{active.pid}/cgroup').read_text().splitlines()
            if row['membership'] != membership:
                raise RuntimeError('suite left inherited cgroup')
            while active.poll() is None:
                sample_tree(active.pid)
                if time.monotonic()-start >= timeout:
                    row['timeout_reached'] = True
                    row['process_cleanup'] = stop_owned()
                    active.wait(timeout=5)
                    break
                time.sleep(.05)
            row['returncode'] = active.returncode
            row['actual_process_tree'] = list(observed.values())
            row['remaining_at_stage_return'] = live_owned()
            if row['remaining_at_stage_return']:
                row['process_cleanup'] = stop_owned()
            active = None
            row['seconds'] = round(time.monotonic()-start, 3)
            evidence = out/name/'architecture-evidence.json'
            if name == 'current-resources':
                evidence = out/name/'report.json'
            if name == 'action-qos' or name in FEATURE_DRIVERS:
                evidence = out/name/'report.json'
            if name in ('wasip1-default-deny', 'original-units', 'ros1-search-contracts'):
                evidence = out/name/'results.json'
            if name == 'rpc' and args.rpc_denial_gate:
                evidence = out/name/'report.json'
            row['status'] = 'FAIL'
            if evidence.is_file():
                document, evidence_binding = read_json_binding(evidence,limit=64*1024**2)
                row['evidence'] = {**evidence_binding,'status': document['status']}
                if row['returncode'] == 0 and document['status'] == 'PASS' and not row['remaining_at_stage_return']:
                    row['status'] = 'PASS'
                try:
                    state['native_witness_log_binding_document_pins'].append(evidence_binding)
                    if name == 'rpc' and args.rpc_denial_gate:
                        # The wrapper has already retired /gp64. Its direct
                        # capture is current source-bound evidence; reopen the
                        # referenced adapter document without altering it.
                        reference = document['adapter_evidence']
                        nested = out/name/'architecture/architecture-evidence.json'
                        if Path(reference['path']) != nested:
                            raise RuntimeError('nested RPC witness document path differs')
                        nested_document, nested_pin = read_json_binding(nested, limit=64*1024**2)
                        if nested_pin['sha256'] != reference['sha256'] or nested_document.get('status') != 'PASS':
                            raise RuntimeError('nested RPC witness document changed or failed')
                        state['native_witness_log_binding_document_pins'].append(nested_pin)
                        compose_native_witness_logs(document,evidence_binding,nested_document,state['workspace_alias_binding'],
                            state['native_witness_log_bindings'],manifest_sha256)
                    else:
                        bind_native_witness_logs(document,state['workspace_alias_binding'],state['native_witness_log_bindings'])
                except (OSError, ValueError, KeyError, TypeError, RuntimeError) as error:
                    row.update(status='FAIL', native_witness_log_binding_error=str(error))
                if name in ('wasip1-default-deny', 'cli', 'original-units', 'ros1-search-contracts'):
                    try:
                        proof = (validate_wasip1_native(document, out/name, manifest_sha256)
                                 if name == 'wasip1-default-deny'
                                 else validate_wasip1_cli(out/name, source) if name == 'cli'
                                 else validate_search_contracts(document, out/name, manifest_sha256, source)
                                      if name == 'ros1-search-contracts'
                                 else validate_native_rows(document, out/name, manifest_sha256, ORIGINAL_UNIT_NAMES))
                        row['mandatory_native_or_cli_checks'] = proof
                    except (OSError, ValueError, KeyError, RuntimeError) as error:
                        row.update(status='FAIL', mandatory_native_or_cli_error=str(error))
                if name.startswith('regression-'):
                    profile = name.removeprefix('regression-')
                    try:
                        row['mandatory_original_regression_checks'] = validate_original_regression(
                            document, profile, profile_workers[profile], manifest_sha256,
                            addon_manifest_sha256, source, config, config_sha256, args.wat2wasm)
                    except (OSError, ValueError, KeyError, TypeError, RuntimeError) as error:
                        row.update(status='FAIL', mandatory_original_regression_error=str(error))
                if name in FEATURE_DRIVERS:
                    try:
                        row['mandatory_current_feature_checks'] = validate_feature_execution(
                            document,out/name,manifest_sha256,FEATURE_DRIVERS[name],feature_bindings[name],
                            gateway_bindings[1 if name == 'ros1-search' else 2])
                    except (OSError,ValueError,KeyError,TypeError,RuntimeError) as error:
                        row.update(status='FAIL',mandatory_current_feature_error=str(error))
                if name=='action-qos':
                    nested = out/name/'execution/architecture-evidence.json'
                    behavior = {item['name']:item for item in document.get('commands',[])
                                if item['name'].startswith('behavior-')}
                    valid = (document.get('format_version')==2
                             and document.get('source_manifest_sha256')==manifest_sha256
                             and set(document.get('probes',{}))==set(ACTION_QOS_PROBES)
                             and document.get('actual_native_behavior_probes')==4
                             and set(behavior)=={'behavior-'+label for label in ACTION_QOS_PROBES}
                             and all(item.get('status')=='PASS' and item.get('actual_ARM_mapping_before_resume')
                                     for item in behavior.values())
                             and document.get('source_preserved') is True and document.get('sdk_preserved') is True
                             and document.get('driver_inputs_preserved') is True and document.get('prepared_inputs_preserved') is True
                             and document.get('same_cgroup_incarnation') is True
                             and document.get('remaining_owned_mounts')==[]
                             and nested.is_file() and json.loads(nested.read_text()).get('status')=='PASS'
                             and document.get('architecture_evidence',{}).get('sha256')==digest(nested))
                    row['mandatory_action_qos_checks_passed'] = valid
                    if not valid:row['status'] = 'FAIL'
                if name == 'rpc' and args.rpc_denial_gate:
                    nested = out/name/'architecture/architecture-evidence.json'
                    gates = document.get('post_denial_gates', {})
                    rows = gates.get('rows', [])
                    compilations = gates.get('derived_compilations', [])
                    expected = {case['name'] for case in document.get('selected_original_typed_cases', [])
                                if case['scenario'] in ('permission-denied', 'no-capability')}
                    valid = (nested.is_file() and json.loads(nested.read_text())['status'] == 'PASS'
                             and document.get('adapter_evidence', {}).get('sha256') == digest(nested)
                             and document.get('remaining_owned_mounts') == []
                             and document.get('source_preserved') is True
                             and len(rows) == len(expected) == 2
                             and {item['name'] for item in rows} == expected
                             and all(item['status'] == 'RELEASED' and item['observer_thread_joined']
                                     and item['native_returncode'] == 0 and item['release_byte'] == 'G'
                                     and item['guest_assertions_observed_at'] < item['native_pass_observed_at']
                                     <= item['released_at_monotonic'] for item in rows)
                             and len(compilations) == 2
                             and {item['name'] for item in compilations} == expected
                             and all(item['returncode'] == 0 and item['reverse_derivation_exact'] is True
                                     for item in compilations)
                             and gates.get('original_guest_wait_seconds') == 20
                             and gates.get('original_native_wait_seconds') == 15
                             and gates.get('original_native_absence_observation_seconds') == 5
                             and gates.get('original_native_baseline_deadline_seconds') == 8)
                    row['denial_gate_checks_passed'] = valid
                    if not valid:
                        row['status'] = 'FAIL'
            save()
            print(row['status'], name, flush=True)
        state['status'] = 'PASS' if state['suites'] and all(row['status'] == 'PASS' for row in state['suites']) else 'FAIL'
    except BaseException:
        state.update(status='FAIL', error=traceback.format_exc())
    finally:
        if active is not None and active.poll() is None:
            sample_tree(active.pid)
        state['final_process_cleanup'] = stop_owned()
        if state['final_process_cleanup']['remaining']:
            state['status'] = 'FAIL'
        if active is not None:
            active.wait(timeout=5)
        state['mount_cleanup'] = []
        for path in reversed(mounts):
            result = subprocess.run(['umount', '-R', str(path)], capture_output=True, text=True, timeout=30)
            state['mount_cleanup'].append({'path': str(path), 'returncode': result.returncode, 'stderr': result.stderr})
            if result.returncode:
                state['status'] = 'FAIL'
        state['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines()
            if any(line.split()[4] == str(path) or line.split()[4].startswith(str(path)+'/') for path in mounts)]
        if state['remaining_owned_mounts']:
            state['status'] = 'FAIL'
        if alias is not None and not state['remaining_owned_mounts']:
            alias.rmdir()
        try:
            state['native_witness_logs_preserved'] = (
                preserved_native_witness_logs(state['native_witness_log_bindings'])
                and pins_preserved(state['native_witness_log_binding_document_pins'])
                and workspace_directory_binding(work) == state['workspace_alias_binding']['origin_directory'])
            state['native_witness_log_bindings_preserved'] = state['native_witness_logs_preserved']
            if not state['native_witness_logs_preserved']:state['status'] = 'FAIL'
        except (OSError, ValueError, KeyError, RuntimeError) as error:
            state.update(status='FAIL',native_witness_logs_preserved=False,
                         native_witness_log_bindings_preserved=False,native_witness_log_preservation_error=str(error))
        state['same_cgroup_incarnation'] = False
        state['ancestor_memory_event_deltas'] = []
        try:
            after, final_identity = pinned_cgroup_probe(probe_module.probe, baseline_identity)
            state['cgroup_after'] = after
            state['cgroup_incarnation_after'] = final_identity
            same = after.get('membership') == before.get('membership')
            state['same_cgroup_incarnation'] = same
            valid, deltas = cgroup_oom_deltas(before, after, same)
            state['ancestor_memory_event_deltas'] = deltas
            if not valid:
                state['status'] = 'FAIL'
        except (OSError, ValueError, KeyError, RuntimeError) as error:
            state.update(status='FAIL', cgroup_final_probe_error=str(error))
        state['source_preserved'] = all(digest(source/name) == row['sha256'] for name, row in frozen.items())
        state['source_addon_preserved'] = (digest(args.source_addon_manifest) == addon_manifest_sha256
            and all(digest(args.source_addon/name) == (row['sha256'] if isinstance(row, dict) else row)
                    for name, row in addon_frozen.items()))
        state['driver_inputs_preserved'] = all(digest(drivers/name) == sha
                                               for name, sha in state['driver_inputs'].items())
        state['build_reports_preserved'] = all(read_json_binding(state[name]['path'])[1] == state[name]
                                               for name in ('worker_build', 'gateway_build'))
        state['prepared_products_preserved'] = (
            all(require_arm(row['binary']['path']) == row['binary'] for row in worker_report['profiles'])
            and all(require_arm(obj['path']) == obj for row in worker_report['profiles'] for obj in row['objects'].values())
            and all(require_arm(row['path']) == row for row in gateway_report['targets'].values())
            and all(require_arm(row['path']) == row for row in action_qos_binding['products'].values())
            and all(require_arm(row['path']) == row for item in feature_bindings.values() for row in item['products'].values()))
        state['feature_preparations_preserved'] = all(read_json_binding(item['path'])[1]
            == {key:value for key,value in item.items() if key not in ('products','input_pins')}
            and pins_preserved(item['input_pins']) for item in feature_bindings.values())
        state['strict_gateway_inputs_preserved'] = all(read_json_binding(item['report']['path'])[1] == item['report']
            and require_arm(item['binary']['path']) == item['binary'] and pins_preserved(item['input_pins'])
            for item in gateway_bindings.values())
        state['native_base_inputs_preserved'] = pins_preserved(native_base['input_pins'])
        state['worker_tool_inputs_preserved'] = pins_preserved([worker_report[label]['input'] for label in ('compiler','qemu')])
        state['suite_reports_preserved'] = all(read_json_binding(row['evidence']['path'],limit=64*1024**2)[1]
            == {key:value for key,value in row['evidence'].items() if key != 'status'}
            for row in state['suites'] if 'evidence' in row)
        state['feature_execution_inputs_preserved'] = all(pins_preserved(row['mandatory_current_feature_checks']['input_pins'])
            and all(require_arm(fact['path']) == fact for fact in row['mandatory_current_feature_checks']['products'].values())
            and require_arm(row['mandatory_current_feature_checks']['gateway']['path']) == row['mandatory_current_feature_checks']['gateway']
            for row in state['suites'] if 'mandatory_current_feature_checks' in row)
        state['mandatory_qos_preparation_preserved'] = (read_json_binding(args.prepared_action_qos)[1]
            == {key:value for key,value in action_qos_binding.items() if key != 'products'})
        if not state['mandatory_qos_preparation_preserved']:
            state['status'] = 'FAIL'
        state['config_preserved'] = digest(args.config) == config_sha256
        if (not state['source_preserved'] or not state['source_addon_preserved']
                or not state['driver_inputs_preserved'] or not state['config_preserved']
                or not state['build_reports_preserved'] or not state['prepared_products_preserved']
                or not state['feature_preparations_preserved'] or not state['strict_gateway_inputs_preserved']
                or not state['suite_reports_preserved'] or not state['feature_execution_inputs_preserved']):
            state['status'] = 'FAIL'
        if not state['native_base_inputs_preserved'] or not state['worker_tool_inputs_preserved']:
            state['status'] = 'FAIL'
        save()
    print(state['status'], out/'report.json', flush=True)
    return 0 if state['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
