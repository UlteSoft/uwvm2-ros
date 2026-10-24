#!/usr/bin/env python3
"""Check the ARM L01 entry using source inspection and pure Python helper models.

This deliberately never invokes run_unit.main, a compiler, QEMU, a Guest, or
ROS. Small temporary files model frozen SDK/resource inventory failures only.
Results establish prepared harness behavior and source bindings, not actual
ARM compilation or execution.
"""
import argparse
import ast
import copy
import hashlib
import json
import os
from pathlib import Path
import re
import sys
import tempfile
import threading
import time
import types

sys.dont_write_bytecode = True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[5])
    parser.add_argument('--output', type=Path, required=True, help='New evidence path; never overwrite an existing report')
    args = parser.parse_args()
    root = args.source.resolve()
    entry = root/'test/0073.ros-tests/support/source/ros_aarch64/run_unit.py'
    builder = root/'test/0073.ros-tests/support/source/ros_aarch64/build_worker.py'
    runtime_path = root/'test/0073.ros-tests/support/source/ros_aarch64/runtime.py'
    sampler_path = root/'test/0073.ros-tests/support/source/ros_aarch64/verify_sampler.py'
    source = entry.read_text()
    tree = ast.parse(source)
    helpers = {'selected_suites', 'worker_test_configuration', 'sdk_root_from_flags',
               'legacy_witness', 'semantic_result', 'verify_sdk_inventory', 'resource_inventory'}
    digest = lambda path: hashlib.sha256(Path(path).read_bytes()).hexdigest()
    scope = {'Path': Path, 'ast': ast, 'os': os, 're': re, 'digest': digest}
    kept = [node for node in tree.body if isinstance(node, ast.Assign) and
            any(isinstance(target, ast.Name) and target.id in ('SUITES', 'WASI_REGRESSIONS', 'SEARCH_CONTRACTS') for target in node.targets)
            or isinstance(node, ast.FunctionDef) and node.name in helpers]
    exec(compile(ast.Module(body=kept, type_ignores=[]), str(entry)+'<pure helpers>', 'exec'), scope)
    checks = []

    def check(name, value):
        checks.append({'name': name, 'status': 'PASS' if value else 'FAIL'})

    def rejected(fn, *values):
        try:
            fn(*values)
        except (RuntimeError, FileNotFoundError):
            return True
        return False

    selection = scope['selected_suites']
    all_rows = selection('all')
    l01_rows = selection('wasip1-default-deny')
    check('default all retains six existing rows and adds seventeen L01 rows', len(all_rows) == 23 and len(l01_rows) == 17)
    original_rows = selection('original')
    check('original subset retains exactly six previous rows without repeating L01',
          {row[0] for row in original_rows} == {'security', 'ipc', 'ipc-client-deadline',
          'sorted-rows', 'sorted-rows-cpp17', 'sdk'} and len(original_rows) == 6)
    check('L01 row names are unique', len({row[0] for row in l01_rows}) == 17)
    check('both authority widths selected exactly once', [row[3] for row in l01_rows if row[3] is not None] == [32, 64])
    check('fourteen existing positive regressions preserved', sum(row[4] for row in l01_rows) == 14)
    check('IPC grouping remains compatible', [row[0] for row in selection('ipc')] == ['ipc', 'ipc-client-deadline'])
    check('current search contracts use distinct CPP20 Worker and CPP17 Gateway rows',
          [(row[0],row[2]) for row in selection('ros1-search-contracts')] ==
          [('ros1-search-worker-contract',20),('ros1-search-gateway-contract',17)])
    check('individual search contract selection does not run other native tests',
          len(selection('ros1-search-gateway-contract')) == 1)
    check('CPP17 skip retains all other rows', len(selection('all', True)) == 22)
    original_suites = scope['SUITES']
    scope['SUITES'] = [('action-qos-policy', 'test/0073.ros-tests/support/source/ros_qos/policy.cc')]
    check('existing Action QoS wrapper override selects only its own fixture', selection('all', True) == [('action-qos-policy', 'test/0073.ros-tests/support/source/ros_qos/policy.cc', 26, None, False)])
    scope['SUITES'] = original_suites
    base, directories = scope['worker_test_configuration'](builder)
    check('flags derive current Worker C++26 and wasm64 literals', '-std=c++26' in base and '-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64' in base)
    check('all four current Worker include directories are retained', directories == ['src', 'third-parties/fast_io/include', 'third-parties/bizwen/include', 'third-parties/boost_unordered/include'])
    # Evaluate only the original-unit argv list against synthetic variables.
    # No process is launched; configured NDEBUG must precede the forced undef.
    original_commands = [node.value for node in ast.walk(tree) if isinstance(node, ast.Assign)
        and isinstance(node.value, ast.List)
        and any(isinstance(target, ast.Name) and target.id == 'command' for target in node.targets)
        and any(isinstance(value, ast.Constant) and value.value == '-std=c++'
                for value in ast.walk(node.value))]
    check('one original-unit compile argv list is unambiguous', len(original_commands) == 1)
    if len(original_commands) == 1:
        command = eval(compile(ast.Expression(original_commands[0]), '<synthetic original-unit argv>', 'eval'),
            {'config':{'compiler':['synthetic-cxx'], 'compile_flags':['-DNDEBUG', '-DNDEBUG=1']},
             'standard':17, 'str':str, 'source':Path('/synthetic/source'), 'barrier':Path('/synthetic/barrier')})
        check('configured NDEBUG cannot disable original native unit assertions',
              '-UNDEBUG' in command and command.index('-UNDEBUG') > max(command.index(flag) for flag in ('-DNDEBUG', '-DNDEBUG=1')))
    witness = scope['legacy_witness']('wasip1-positive-clock_time_get_wasm64', True)
    check('legacy main is preserved with linker wrapping', '__real_main()' in witness and '__wrap_main' in witness and '#define main' not in witness)
    check('silent wasm64 stubs are rejected at compile time', '#if !defined(UWVM_IMPORT_WASI_WASIP1_WASM64)' in witness)
    check('disabled WASI regression stubs are rejected at compile time', '#if !defined(UWVM_IMPORT_WASI_WASIP1)' in witness)
    semantic = scope['semantic_result']
    check('configuration marker requires actual full truth table', semantic('wasip1-configuration', 'PASS WASIp1 configuration checks=16777500\n', False) and
          not semantic('wasip1-configuration', 'PASS WASIp1 configuration checks=1\n', False))
    check('another fixture PASS cannot satisfy configuration', not semantic('wasip1-configuration', 'PASS unrelated\n', False))
    check('authority checks require exact width', semantic('wasip1-authority64', 'WASIp1 authority width=64 checks=279 PASS\n', False) and
          not semantic('wasip1-authority64', 'WASIp1 authority width=32 checks=279 PASS\n', False))
    check('duplicate authority markers fail', not semantic('wasip1-authority32', 'WASIp1 authority width=32 checks=279 PASS\n'*2, False))
    check('zero assertion authority marker fails', not semantic('wasip1-authority32', 'WASIp1 authority width=32 checks=0 PASS\n', False))
    name = 'wasip1-positive-random_get'
    check('legacy marker binds selected original test', semantic(name, 'PASS legacy WASIp1 regression '+name+' \n', True) and
          not semantic(name, 'PASS legacy WASIp1 regression another \n', True))
    check('explicit failure cannot be hidden by PASS', not semantic(name, 'FAIL native\nPASS legacy WASIp1 regression '+name+' \n', True))

    with tempfile.TemporaryDirectory(prefix='uwvm-l01-source-model-') as temp:
        sdk = Path(temp)/'sdk'; sdk.mkdir()
        file = sdk/'header.h'; file.write_text('// synthetic source-model input\n')
        link = sdk/'alias.h'; link.symlink_to('header.h')
        row = {'path': 'header.h', 'bytes': file.stat().st_size, 'sha256': digest(file)}
        inventory = {'regular_files': [row], 'symlinks': [{'path': 'alias.h', 'target': 'header.h'}]}
        verify = scope['verify_sdk_inventory']
        check('model frozen SDK files and links accepted', str(file.resolve()) in verify(sdk, inventory))
        check('SDK symlink target mutation rejected', rejected(verify, sdk, {**inventory, 'symlinks': [{'path': 'alias.h', 'target': 'other.h'}]}))
        check('SDK duplicate input paths rejected', rejected(verify, sdk, {**inventory, 'regular_files': [row, row]}))
        check('SDK path traversal rejected', rejected(verify, sdk, {'regular_files': [{**row, 'path': '../header.h'}], 'symlinks': []}))
        check('SDK empty inventory rejected', rejected(verify, sdk, {'regular_files': [], 'symlinks': []}))
        check('SDK byte mismatch rejected', rejected(verify, sdk, {**inventory, 'regular_files': [{**row, 'bytes': 1}]}))
        check('SDK SHA mismatch rejected', rejected(verify, sdk, {**inventory, 'regular_files': [{**row, 'sha256': '0'*64}]}))
        resource = scope['resource_inventory']
        initial, _ = resource(sdk)
        file.write_text('// mutated synthetic source-model input\n')
        check('compiler resource file mutation changes closure', resource(sdk)[0] != initial)
        outside = Path(temp)/'outside.h'; outside.write_text('// outside\n')
        (sdk/'escape.h').symlink_to(outside)
        check('compiler resource escaping symlink rejected', rejected(resource, sdk))
        check('one explicit cross SDK root selected', scope['sdk_root_from_flags'](['--sysroot='+str(sdk)]) == sdk.resolve())
        check('ambiguous cross SDK roots rejected', rejected(scope['sdk_root_from_flags'], ['--sysroot='+str(sdk)]*2))

    # Execute only Evidence methods against explicitly synthetic objects/files.
    # No constructor runs, no real process is observed, and no Linux cgroup is
    # read or mutated. Missing baseline proof must fail rather than bypass checks.
    runtime_model = types.ModuleType('runtime_source_model')
    runtime_model.__dict__.update(Path=Path, threading=threading, time=time, json=json, os=os)
    runtime_tree = ast.parse(runtime_path.read_text())
    runtime_nodes = [node for node in runtime_tree.body if
                     isinstance(node, ast.FunctionDef) and node.name in ('cgroup_oom_deltas','process_child_pids') or
                     isinstance(node, ast.ClassDef) and node.name == 'Evidence']
    exec(compile(ast.Module(body=runtime_nodes, type_ignores=[]), '<synthetic Evidence source model>', 'exec'), runtime_model.__dict__)
    identity = {'path': '/synthetic/leaf', 'device': 29, 'inode': 18873, 'boot_id': 'SYNTHETIC_BOOT_A'}
    facts = {'status': 'PASS', 'membership': ['0::/synthetic/leaf'], 'cgroup_ancestors': [
        {'path': '/synthetic/leaf', 'memory.max': '68719476736', 'memory.events': 'oom 3\noom_kill 2\noom_group_kill 0'}]}
    with tempfile.TemporaryDirectory(prefix='uwvm-l01-incarnation-model-') as temp:
        def finish_model(label, after_identity=None, after_facts=None, no_baseline=False, metadata=None):
            model = object.__new__(runtime_model.Evidence)
            model.out = Path(temp)/label; model.out.mkdir()
            model.before = copy.deepcopy(facts); model.probe = lambda: copy.deepcopy(after_facts or facts)
            model.leaf = Path('/synthetic/leaf')
            current = copy.deepcopy(after_identity or identity)
            model._incarnation = lambda: copy.deepcopy(current)
            if not no_baseline:
                model.incarnation_before = copy.deepcopy(identity)
            model.targets = {}; model.processes = []; model.samples = {}; model.unavailable = []
            model.fatal_errors = []; model.emulator_shas = set()
            model._stop = threading.Event(); model.thread = threading.Thread(target=lambda: None); model.thread.start()
            return model.finish('PASS', {'scope': 'PURE_SYNTHETIC_SOURCE_MODEL_NO_REAL_EXECUTION', **(metadata or {})})
        stable = finish_model('stable')
        check('same incarnation permits zero OOM delta', stable['status'] == 'PASS' and stable['same_cgroup_incarnation'] and
              stable['cgroup_ancestor_event_deltas'][0]['oom'] == 0)
        for field, value in [('device', 30), ('inode', 19435), ('boot_id', 'SYNTHETIC_BOOT_B'), ('path', '/synthetic/other')]:
            changed = {**identity, field: value}
            result = finish_model('changed-'+field, after_identity=changed)
            check('changed '+field+' rejects without cross-incarnation OOM subtraction',
                  result['status'] == 'FAIL' and not result['same_cgroup_incarnation'] and result['cgroup_ancestor_event_deltas'] == [])
        result = finish_model('missing-baseline', no_baseline=True)
        check('manual object without baseline does not bypass identity proof', result['status'] == 'FAIL' and result['cgroup_ancestor_event_deltas'] == [])
        result = finish_model('caller-metadata-cannot-override', after_identity={**identity, 'inode': 19435},
                              metadata={'status': 'PASS', 'same_cgroup_incarnation': True, 'oom_counter_comparison': 'SAME_INCARNATION'})
        check('caller metadata cannot overwrite mandatory identity verdict', result['status'] == 'FAIL' and
              result['same_cgroup_incarnation'] is False and result['oom_counter_comparison'] != 'SAME_INCARNATION')
        changed_facts = copy.deepcopy(facts); changed_facts['membership'] = ['0::/synthetic/other']
        result = finish_model('changed-membership', after_facts=changed_facts)
        check('changed membership rejects without OOM subtraction', result['status'] == 'FAIL' and result['cgroup_ancestor_event_deltas'] == [])
        changed_facts = copy.deepcopy(facts); changed_facts['cgroup_ancestors'][0]['memory.events'] = 'oom 4\noom_kill 2\noom_group_kill 0'
        result = finish_model('actual-counter-delta', after_facts=changed_facts)
        check('same-incarnation nonzero OOM delta rejects', result['status'] == 'FAIL' and result['same_cgroup_incarnation'] and
              result['cgroup_ancestor_event_deltas'][0]['oom'] == 1)
        changed_facts = copy.deepcopy(facts); changed_facts['cgroup_ancestors'][0]['memory.events'] = None
        result = finish_model('missing-counter', after_facts=changed_facts)
        check('missing required OOM counters reject', result['status'] == 'FAIL')
        # Reuse the existing six manual fake-proc rejection/resampling models.
        # Their new baseline callback is explicit and remains synthetic only.
        sampler_tree = ast.parse(sampler_path.read_text())
        sampler_nodes = [node for node in sampler_tree.body if isinstance(node, ast.FunctionDef) and node.name in ('save', 'fake_proc_models')]
        sampler_scope = {'Path': Path, 'runtime': runtime_model, 'Evidence': runtime_model.Evidence,
                         'threading': threading, 'json': json}
        exec(compile(ast.Module(body=sampler_nodes, type_ignores=[]), '<existing manual fake-proc models>', 'exec'), sampler_scope)
        manual_rows = sampler_scope['fake_proc_models'](Path(temp)/'existing-manual-models')
        check('all six existing fake-proc evidence models remain valid', len(manual_rows) == 6 and
              all(row['validation_status'] == 'PASS' and row['positive_samples'] == 0 for row in manual_rows))

    for name, required in {
        'actual compiler system closure uses MD': "'-MD', '-MF'",
        'original object architecture checked': 'require_arm(original_object)',
        'witness object architecture checked': 'require_arm(witness_object)',
        'checks survive NDEBUG configuration': "'-UNDEBUG'",
        'original entry linked without source rename': "'-Wl,--wrap=main'",
        'SDK closure verified after compilation': "verify_sdk_inventory(l01_context['sdk_root']",
        'resource closure verified after compilation': "resource_inventory(l01_context['resource_root'])",
        'cgroup leaf device/inode rechecked': 'after_identity.st_dev, after_identity.st_ino',
        'cgroup zero swap required': "if swap_before != '0'",
        'child cgroup membership checked': "record['membership'] != evidence.before['membership']",
        'live ARM map proof still precedes resume': 'evidence.check([target], label)',
        'private fixture working directory': "work = out/(name+'.work')",
    }.items():
        check(name, required in source)
    runtime_source = runtime_path.read_text()
    check('accepted baseline captured between incarnation proofs', 'self.before = probe()' in runtime_source and
          'self._incarnation() != self.incarnation_before' in runtime_source)
    check('runtime counter comparison receives verified identity', 'cgroup_oom_deltas(self.before, after, same_incarnation)' in runtime_source)
    files = [entry, builder, runtime_path, sampler_path, Path(__file__).resolve(), root/'test/0073.ros-tests/support/source/wasip1_default_deny/configuration.cc',
             root/'test/0073.ros-tests/support/source/wasip1_default_deny/func/authority.cc',
             root/'test/0008.imported/wasi/wasip1/func/fp_control_probe.h']
    files += [root/row[1] for row in l01_rows if row[4]]
    report = {'status': 'PASS' if all(row['status'] == 'PASS' for row in checks) else 'FAIL',
              'scope': 'Source inspection and pure Python helper/filesystem models only; no compiler, QEMU, Guest, ROS or ARM execution',
              'actual_native_matrix_status': 'PENDING_LINUX_CGROUP', 'source_checks_and_models': checks,
              'prepared_native_rows': [row[0] for row in l01_rows],
              'source_hashes': {str(path.relative_to(root)): digest(path) for path in files}}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open('x') as stream:
        json.dump(report, stream, indent=2); stream.write('\n')
    print(json.dumps({'status': report['status'], 'source_checks_and_models': len(checks), 'prepared_native_rows': len(l01_rows),
                      'output': str(args.output.resolve())}))
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
