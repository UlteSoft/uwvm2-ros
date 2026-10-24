#!/usr/bin/env python3
"""Cross-build frozen ROS unit sources and prove their actual ARM execution."""
import argparse
import ast
import importlib.util
import json
import os
from pathlib import Path
import shlex
import signal
import re
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, digest, process_child_pids, require_arm


SUITES = [
    ('security', 'test/0073.ros-tests/support/source/ros_security/capability_policy_test.cc'),
    ('ipc', 'test/0073.ros-tests/support/source/ros_ipc/protocol_test.cc'),
    ('ipc-client-deadline', 'test/0073.ros-tests/support/source/ros_ipc_client_deadline/unit.cc'),
    ('sorted-rows', 'test/0073.ros-tests/support/source/ros_sorted_rows/unit.cc'),
    ('sdk', 'test/0073.ros-tests/support/source/ros/sdk_codec.cpp'),
    ('wasip1-default-deny', 'test/0073.ros-tests/support/source/wasip1_default_deny/configuration.cc'),
]
WASI_REGRESSIONS = [
    ('wasip1-positive-'+name+suffix, 'test/0008.imported/wasi/wasip1/func/'+name+suffix+'.cc')
    for name in ('clock_res_get', 'clock_time_get', 'random_get', 'proc_raise', 'sched_yield', 'poll_oneoff')
    for suffix in ('', '_wasm64')
] + [
    ('wasip1-positive-hybrid-mt', 'test/0008.imported/wasi/wasip1/hybrid/wasi_hybrid_mt.cc'),
    ('wasip1-positive-hybrid-mixed-stress', 'test/0008.imported/wasi/wasip1/hybrid/wasi_hybrid_mixed_stress.cc'),
]
SEARCH_CONTRACTS = [
    ('ros1-search-worker-contract', 'test/0073.ros-tests/support/source/ros1_search/contract_test.cc', 20, None, False),
    ('ros1-search-gateway-contract', 'test/0073.ros-tests/support/source/ros1_search/gateway_contract_test.cc', 17, None, False),
]
CUSTOM_CONTRACTS = [
    ('custom-topic-scanner', 'test/0073.ros-tests/support/source/ros_custom_types/contract_test.cc', 20, None, False),
    ('custom-topic-default-deny', 'test/0073.ros-tests/support/source/ros_custom_types/policy_test.cc', 26, None, False),
    ('custom-topic-explicit-grants', 'test/0073.ros-tests/support/source/ros_custom_types/policy_test.cc', 26, None, False),
]
AUDIT_CACHE_CONTRACTS = [
    ('native-audit-cache', 'test/0073.ros-tests/support/source/ros_native_audit_io/cache_unit.cc', 17, None, False),
]
ROS2_ENVIRONMENT_CONTRACTS = [
    ('ros2-environment-contract', 'test/0073.ros-tests/support/source/ros2_environment/unit.cc', 17, None, False),
]
UNIFIED_EVENT_CONTRACTS = [
    ('unified-event-queue', 'test/0073.ros-tests/support/source/ros_unified_events/queue_contract.cc', 20, None, False),
    ('unified-event-async', 'test/0073.ros-tests/support/source/ros_unified_events/async_contract.cc', 20, None, False),
    ('unified-event-manager', 'test/0073.ros-tests/support/source/ros_unified_events/manager_contract.cc', 26, None, False),
    ('unified-event-server', 'test/0073.ros-tests/support/source/ros_unified_events/server_contract.cc', 20, None, False),
]
FULL_CONTRACT_NAMES = {row[0] for row in SEARCH_CONTRACTS + CUSTOM_CONTRACTS + AUDIT_CACHE_CONTRACTS + ROS2_ENVIRONMENT_CONTRACTS + UNIFIED_EVENT_CONTRACTS}


def selected_suites(requested, skip_cpp17=False):
    """Expand L01 without changing wrappers that replace the original SUITES list."""
    # This opt-in contract uses the same full SDK/-MD/barrier evidence path.
    # Keep both the original six units and separate seventeen WASI rows intact.
    if requested == 'unified-events' or requested in {row[0] for row in UNIFIED_EVENT_CONTRACTS}:
        return [row for row in UNIFIED_EVENT_CONTRACTS if requested in ('unified-events', row[0])]
    if requested in {row[0] for row in AUDIT_CACHE_CONTRACTS}:
        return list(AUDIT_CACHE_CONTRACTS)
    if requested in {row[0] for row in ROS2_ENVIRONMENT_CONTRACTS}:
        return list(ROS2_ENVIRONMENT_CONTRACTS)
    if requested == 'ros1-search-contracts' or requested in {row[0] for row in SEARCH_CONTRACTS}:
        return [row for row in SEARCH_CONTRACTS if requested in ('ros1-search-contracts', row[0])]
    if requested == 'custom-topic-contracts' or requested in {row[0] for row in CUSTOM_CONTRACTS}:
        return [row for row in CUSTOM_CONTRACTS if requested in ('custom-topic-contracts', row[0])]
    selected = []
    for name, relative in SUITES:
        if not (requested in ('all', name)
                or requested == 'original' and name != 'wasip1-default-deny'
                or requested == 'ipc' and name == 'ipc-client-deadline'):
            continue
        if name == 'wasip1-default-deny':
            selected += [('wasip1-configuration', relative, 26, None, False),
                         ('wasip1-authority32', 'test/0073.ros-tests/support/source/wasip1_default_deny/func/authority.cc', 26, 32, False),
                         ('wasip1-authority64', 'test/0073.ros-tests/support/source/wasip1_default_deny/func/authority.cc', 26, 64, False)]
            selected += [(suite, path, 26, None, True) for suite, path in WASI_REGRESSIONS]
        else:
            selected.append((name, relative, 26, None, False))
    if not skip_cpp17 and requested in ('all', 'original', 'sorted-rows'):
        selected.append(('sorted-rows-cpp17', 'test/0073.ros-tests/support/source/ros_sorted_rows/unit.cc', 17, None, False))
    if requested == 'all': selected += UNIFIED_EVENT_CONTRACTS
    return selected


def worker_test_configuration(builder):
    """Read the actual frozen Worker literals without importing/executing its code."""
    tree = ast.parse(Path(builder).read_text())
    function = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == 'main')
    bases, directories = [], []
    for node in ast.walk(function):
        if isinstance(node, ast.Assign) and any(isinstance(target, ast.Name) and target.id == 'base' for target in node.targets):
            bases.append(ast.literal_eval(node.value))
        if isinstance(node, ast.For) and isinstance(node.target, ast.Name) and node.target.id == 'directory':
            directories.append(ast.literal_eval(node.iter))
    if len(bases) != 1 or len(directories) != 1 or not all(isinstance(value, str) for value in bases[0]+directories[0]):
        raise RuntimeError('Worker base flags/include literal binding is ambiguous')
    if '-std=c++26' not in bases[0] or '-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64' not in bases[0]:
        raise RuntimeError('current Worker C++26/wasm64 flags are required')
    return bases[0], directories[0]


def sdk_root_from_flags(flags):
    roots = [flag.split('=', 1)[1] for flag in flags if flag.startswith('--sysroot=')]
    if len(roots) != 1:
        raise RuntimeError('exact one explicit Worker cross SDK sysroot is required')
    return Path(roots[0]).resolve(strict=True)


def legacy_witness(name, wide):
    """Linker wrapping preserves the unchanged original main's implicit return zero."""
    return '''// Generated successful-return witness; original source/main is unchanged.
#include <fast_io.h>
#include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>
#if !defined(UWVM_IMPORT_WASI_WASIP1)
#error "The original WASI positive test may not compile as a disabled stub."
#endif
''' + ('''#if !defined(UWVM_IMPORT_WASI_WASIP1_WASM64)
#error "The original wasm64 positive test may not compile as an empty main."
#endif
''' if wide else '') + '''#include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>
extern "C" int __real_main();
extern "C" int __wrap_main(int, char**) {
    int const status{__real_main()};
    if(status != 0) { return status; }
    ::fast_io::io::println("PASS legacy WASIp1 regression '''+name+''' ");
    return 0;
}
'''


def semantic_result(name, log, legacy):
    """Require the selected fixture's own result; another line cannot satisfy it."""
    if 'FAIL ' in log:
        return False
    if legacy:
        return log.splitlines().count('PASS legacy WASIp1 regression '+name+' ') == 1
    if name == 'wasip1-configuration':
        matches = re.findall(r'^PASS WASIp1 configuration checks=(\d+)$', log, re.M)
        return len(matches) == 1 and int(matches[0]) >= 16777216
    if name.startswith('wasip1-authority'):
        width = name.removeprefix('wasip1-authority')
        return len(re.findall(r'^WASIp1 authority width='+width+r' checks=[1-9]\d* PASS$', log, re.M)) == 1
    if name == 'ros1-search-worker-contract':
        return len(re.findall(r'^PASS ROS1 explicit parent-search Worker/Gateway contract checks=[1-9]\d*$', log, re.M)) == 1
    if name == 'ros1-search-gateway-contract':
        return log.splitlines().count('PASS C++17 Gateway explicit parent-search contract') == 1
    if name == 'custom-topic-scanner':
        return len(re.findall(r'^PASS custom fixed Topic scanner contracts checks=[1-9]\d*$', log, re.M)) == 1
    if name in ('custom-topic-default-deny', 'custom-topic-explicit-grants'):
        count = '0' if name == 'custom-topic-default-deny' else '2'
        return len(re.findall(r'^PASS custom Topic policy explicit grants checks=[1-9]\d* registered='+count+'$', log, re.M)) == 1
    if name == 'native-audit-cache':
        return len(re.findall(r'^PASS C\+\+17 native audit numeric cache checks=[1-9]\d* cached_aliases=3 hot_cpp_allocations=0$', log, re.M)) == 1
    if name == 'ros2-environment-contract':
        return len(re.findall(r'^PASS C\+\+17 ROS2 checked environment contract checks=[1-9]\d*$', log, re.M)) == 1
    if name in {row[0] for row in UNIFIED_EVENT_CONTRACTS}:
        marker = {'unified-event-queue':'PASS_UNIFIED_QUEUE', 'unified-event-async':'PASS_UNIFIED_ASYNC',
                  'unified-event-manager':'PASS_UNIFIED_MANAGER', 'unified-event-server':'PASS_UNIFIED_SERVER'}[name]
        return log == marker+'\n'
    return 'PASS ' in log


def verify_sdk_inventory(root, inventory):
    """Check frozen regular files and symlink spellings, rejecting path escapes."""
    root = Path(root).resolve(strict=True)
    files = {}
    seen = set()
    for kind in ('regular_files', 'symlinks'):
        for row in inventory[kind]:
            relative = Path(row['path'])
            if relative.is_absolute() or '..' in relative.parts or str(relative) in seen:
                raise RuntimeError('invalid or duplicate SDK inventory path')
            seen.add(str(relative))
            path = root/relative
            if kind == 'symlinks':
                if not path.is_symlink() or os.readlink(path) != row['target']:
                    raise RuntimeError('frozen SDK symlink changed: '+str(relative))
                continue
            resolved = path.resolve(strict=True)
            if not resolved.is_relative_to(root) or not resolved.is_file() or path.is_symlink():
                raise RuntimeError('frozen SDK file escaped or changed kind: '+str(relative))
            actual = {'sha256': digest(resolved), 'bytes': resolved.stat().st_size}
            if actual != {'sha256': row['sha256'], 'bytes': row['bytes']}:
                raise RuntimeError('frozen SDK file changed: '+str(relative))
            files[str(resolved)] = actual
    if not files:
        raise RuntimeError('empty frozen cross SDK inventory')
    return files


def resource_inventory(root):
    """Freeze compiler builtin headers before compilation, with symlinks explicit."""
    root = Path(root).resolve(strict=True)
    rows, files = {}, {}
    for path in sorted(root.rglob('*')):
        name = str(path.relative_to(root))
        if path.is_symlink():
            rows[name] = {'target': os.readlink(path)}
        if path.is_file():
            actual = path.resolve(strict=True)
            if not actual.is_relative_to(root):
                raise RuntimeError('compiler resource input escaped its declared root')
            facts = {'sha256': digest(actual), 'bytes': actual.stat().st_size}
            rows[name] = {**rows.get(name, {}), **facts}
            files[str(actual)] = facts
    if not files:
        raise RuntimeError('compiler resource header snapshot is empty')
    return rows, files
BARRIER = '''#include <signal.h>
// Stop before the unchanged test main so even a fast test has live map proof.
__attribute__((constructor)) static void uwvm_arm_unit_evidence_stop() {
    if (::raise(SIGSTOP) != 0) __builtin_trap();
}
'''


def process_tree_rss(pid):
    pending, seen, total = [pid], set(), 0
    while pending:
        current = pending.pop()
        if current in seen:
            continue
        seen.add(current)
        base = Path('/proc') / str(current)
        try:
            pending.extend(process_child_pids(current))
            for line in (base/'status').read_text().splitlines():
                if line.startswith('VmRSS:'):
                    total += int(line.split()[1]) * 1024
        except (FileNotFoundError, ProcessLookupError):
            pass
    return total


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--manifest', type=Path, required=True)
    p.add_argument('--config', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--suite', choices=[name for name, _ in SUITES] + ['all', 'original', 'ros1-search-contracts', 'custom-topic-contracts', 'unified-events']
                   + [row[0] for row in SEARCH_CONTRACTS + CUSTOM_CONTRACTS + AUDIT_CACHE_CONTRACTS + ROS2_ENVIRONMENT_CONTRACTS + UNIFIED_EVENT_CONTRACTS], default='all')
    p.add_argument('--custom-registry-header', type=Path,
                   help='Reviewed manifest within the frozen source, only for custom-topic-explicit-grants')
    p.add_argument('--skip-cpp17-sorted-rows', action='store_true')
    p.add_argument('--sdk-manifest', type=Path,
                   help='L01 requires the frozen cross SDK inventory; defaults to sysroot.parent/sdk-files.json')
    p.add_argument('--compiler-resource-root', type=Path,
                   help='L01 compiler builtin header root; defaults to compiler.parent.parent/lib/clang')
    args = p.parse_args()
    source = args.source.resolve(strict=True)
    out = args.out.resolve()
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    manifest = json.loads(args.manifest.read_text())
    config = json.loads(args.config.read_text())
    sys.path.insert(0, str(source/'test/0073.ros-tests/support/source/ros_cgroup'))
    spec = importlib.util.spec_from_file_location('arm_unit_cgroup', source/'test/0073.ros-tests/support/source/ros_cgroup/probe.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    evidence = Evidence(out/'architecture', module.probe)
    leaf_identity = evidence.leaf.stat()
    swap_before = (evidence.leaf/'memory.swap.max').read_text().strip()
    if swap_before != '0':
        raise RuntimeError('BLOCKED exact provided cgroup zero-swap limit is required')
    emulator = evidence.emulator(config['qemu_argv'][0])
    barrier = out/'evidence-stop.hpp'
    barrier.write_text(BARRIER)
    generated = {'path': str(barrier), 'sha256': digest(barrier),
                 'purpose': 'SIGSTOP before unchanged test main; live ARM mapping captured before SIGCONT'}
    report = {'status': 'FAIL', 'scope': 'ARM unit behavior; no ROS network execution claim',
              'source_root': str(source), 'source_manifest_sha256': digest(args.manifest),
              'config_sha256': digest(args.config), 'driver_sha256': digest(__file__),
              'runtime_sha256': digest(Path(__file__).with_name('runtime.py')),
              'compiler_sha256': digest(config['compiler'][0]), 'emulator': emulator,
              'compiler_parallelism': 1, 'compiler_aggregate_rss_budget_bytes': 2*1024**3,
              'compile_timeout_seconds': 180, 'qemu_timeout_seconds': 60,
              'generated_evidence_barrier': generated, 'suites': []}
    report['cgroup_incarnation'] = {'path': str(evidence.leaf), 'device': leaf_identity.st_dev,
                                   'inode': leaf_identity.st_ino, 'memory.swap.max': swap_before}
    environment = dict(os.environ)
    environment.pop('LD_PRELOAD', None)
    environment.pop('QEMU_LD_PREFIX', None)
    environment.update(config.get('environment', {}))
    active = None

    def checked(relative):
        expected = manifest[relative]
        expected_sha = expected['sha256'] if isinstance(expected, dict) else expected
        path = source/relative
        actual = digest(path)
        if actual != expected_sha or isinstance(expected, dict) and path.stat().st_size != expected['bytes']:
            raise RuntimeError('frozen source changed: '+relative)
        return {'sha256': actual, 'bytes': path.stat().st_size}

    def stop_active():
        nonlocal active
        if active is not None and active.poll() is None:
            os.killpg(active.pid, signal.SIGKILL)
            active.wait(timeout=5)
        active = None

    def terminate(signum, frame):
        stop_active()
        raise RuntimeError('interrupted by signal '+str(signum))

    signal.signal(signal.SIGTERM, terminate)
    signal.signal(signal.SIGINT, terminate)

    def run(command, label, log, timeout, target=None, cwd=None):
        nonlocal active
        started = time.monotonic()
        record = {'command': command, 'log': str(log), 'timeout_seconds': timeout, 'cwd': str(cwd) if cwd else None}
        try:
            with log.open('wb') as stream:
                active = subprocess.Popen(command, stdout=stream, stderr=subprocess.STDOUT,
                                          env=environment, start_new_session=True, cwd=cwd)
            record['pid'] = active.pid
            record['membership'] = Path(f'/proc/{active.pid}/cgroup').read_text().splitlines()
            if record['membership'] != evidence.before['membership']:
                raise RuntimeError('command left the inherited cgroup')
            evidence.start(active, label, command)
            if target is not None:
                deadline = started + min(timeout, 15)
                while active.poll() is None and time.monotonic() < deadline:
                    try:
                        state = Path(f'/proc/{active.pid}/stat').read_text().rsplit(')', 1)[1].split()[0]
                    except FileNotFoundError:
                        break
                    if state in ('T', 't'):
                        evidence.observe(active.pid, label)
                        evidence.check([target], label)
                        record['live_arm_mapping_before_resume'] = True
                        os.kill(active.pid, signal.SIGCONT)
                        break
                    time.sleep(.002)
                else:
                    raise RuntimeError('evidence barrier did not stop ARM test '+label)
                if not record.get('live_arm_mapping_before_resume'):
                    raise RuntimeError('ARM test exited before evidence barrier '+label)
            peak = 0
            while active.poll() is None:
                peak = max(peak, process_tree_rss(active.pid))
                if target is None and peak > report['compiler_aggregate_rss_budget_bytes']:
                    raise RuntimeError('compiler aggregate RSS exceeded 2 GiB')
                if time.monotonic()-started > timeout:
                    raise RuntimeError('bounded command timeout: '+label)
                time.sleep(.01)
            record.update(returncode=active.returncode, max_observed_process_tree_rss_bytes=peak)
            if target is not None:
                evidence.check([target], label)
        finally:
            stop_active()
            record.update(seconds=round(time.monotonic()-started, 3), log_sha256=digest(log))
        return record

    def dependencies(dep, relative, generated_inputs=None, system_inputs=None):
        text = dep.read_text().replace('\\\n', ' ')
        tokens = shlex.split(text.split(':', 1)[1])
        inputs = {}
        seen_barrier = False
        generated_seen, system_seen = {}, {}
        generated_inputs = generated_inputs or {}
        for token in tokens:
            path = Path(token).resolve(strict=True)
            if path == barrier:
                if digest(path) != generated['sha256']:
                    raise RuntimeError('generated barrier changed')
                seen_barrier = True
                continue
            if str(path) in generated_inputs:
                recorded = generated_inputs[str(path)]
                if digest(path) != recorded['sha256']:
                    raise RuntimeError('generated successful-return witness changed')
                generated_seen[str(path)] = recorded
                continue
            try:
                name = str(path.relative_to(source))
            except ValueError:
                if system_inputs is None or str(path) not in system_inputs:
                    raise RuntimeError('unexpected unqualified compiler input: '+str(path))
                actual = {'sha256': digest(path), 'bytes': path.stat().st_size}
                if actual != system_inputs[str(path)]:
                    raise RuntimeError('prequalified system compiler input changed: '+str(path))
                system_seen[str(path)] = actual
                continue
            inputs[name] = checked(name)
        if relative is not None and (relative not in inputs or not seen_barrier):
            raise RuntimeError('compiler dependencies lack original source or generated barrier')
        if relative is None and set(generated_seen) != set(generated_inputs):
            raise RuntimeError('compiler dependencies lack the generated successful-return witness')
        return {'path': str(dep), 'sha256': digest(dep), 'project_inputs': inputs,
                'generated_inputs': ([generated] if seen_barrier else [])+list(generated_seen.values()),
                'system_inputs': system_seen,
                'scope': 'Actual -MD project+prequalified system dependencies' if system_inputs is not None else
                         'Actual -MMD project dependencies; system headers supplied by frozen cross SDK'}

    l01_context = None
    try:
        for relative in manifest:
            checked(relative)
        selected = selected_suites(args.suite, args.skip_cpp17_sorted_rows)
        if any(name.startswith('wasip1-') or name in FULL_CONTRACT_NAMES for name, *_ in selected):
            builder = Path(__file__).with_name('build_worker.py').resolve(strict=True)
            base, include_directories = worker_test_configuration(builder)
            sdk_root = sdk_root_from_flags(config['compile_flags'])
            sdk_manifest = (args.sdk_manifest or sdk_root.parent/'sdk-files.json').resolve(strict=True)
            sdk_inventory = json.loads(sdk_manifest.read_text())
            system_inputs = verify_sdk_inventory(sdk_root, sdk_inventory)
            resource_input = args.compiler_resource_root or config.get('compiler_resource_root')
            resource_root = (Path(resource_input) if resource_input else
                             Path(config['compiler'][0]).parent.parent/'lib/clang').resolve(strict=True)
            resource_rows, resource_files = resource_inventory(resource_root)
            system_inputs.update(resource_files)
            l01_context = {'builder': builder, 'builder_sha256': digest(builder), 'base': base,
                           'include_directories': include_directories, 'sdk_root': sdk_root,
                           'sdk_manifest': sdk_manifest, 'sdk_manifest_sha256': digest(sdk_manifest),
                           'sdk_inventory': sdk_inventory, 'resource_root': resource_root,
                           'resource_rows': resource_rows, 'system_inputs': system_inputs}
            report['source_compiler_closure'] = {
                'scope': 'Actual native test compile closure; no Guest or ROS network claim',
                'worker_builder': str(builder), 'worker_builder_sha256': l01_context['builder_sha256'],
                'worker_base_flags': base, 'worker_include_directories': include_directories,
                'compile_flags': config['compile_flags'], 'link_flags': config['link_flags'],
                'sdk_root': str(sdk_root), 'sdk_manifest': str(sdk_manifest),
                'sdk_manifest_sha256': l01_context['sdk_manifest_sha256'],
                'sdk_regular_files': len(sdk_inventory['regular_files']), 'sdk_symlinks': len(sdk_inventory['symlinks']),
                'compiler_resource_root': str(resource_root), 'compiler_resource_snapshot': resource_rows,
                'mandatory_rows': [name for name, *_ in selected],
            }
            if any(name.startswith('wasip1-') for name, *_ in selected):
                report['wasip1_default_deny'] = {**report['source_compiler_closure'],
                    'separate_mandatory_regressions':['LLVM clock FP test', 'current-policy checkpoint restore']}
        for name, relative, standard, width, legacy in selected:
            row = {'suite': name, 'original_source': relative, 'standard': standard, 'status': 'FAIL'}
            report['suites'].append(row)
            binary, dep = out/name, out/(name+'.d')
            try:
                checked(relative)
                if name.startswith('wasip1-') or name in FULL_CONTRACT_NAMES:
                    work = out/(name+'.work'); work.mkdir(mode=0o700)
                    # Preserve the recorded Worker defines, then explicitly
                    # select this fixture's standard after the base C++26 flag.
                    # The common Gateway contract is intentionally C++17.
                    flags = [*config['compile_flags'], *l01_context['base'], '-std=c++'+str(standard),
                             '-UNDEBUG', '-I', str(source)]
                    for directory in l01_context['include_directories']:
                        flags += ['-I', str(source/directory)]
                    if name == 'custom-topic-explicit-grants':
                        if args.custom_registry_header is None:
                            raise RuntimeError('explicit-grant contract requires a reviewed custom registry header')
                        header = args.custom_registry_header.resolve(strict=True)
                        relative_header = str(header.relative_to(source))
                        checked(relative_header)
                        if any(char in str(header) for char in ('"', '\n', '\r')):
                            raise RuntimeError('custom registry include path contains unsupported characters')
                        flags += ['-DUWVM_CUSTOM_TEST_REGISTERED=1', '-DUWVM_ROS_CUSTOM_TOPIC_MANIFEST="'+str(header)+'"']
                        row['custom_registry'] = {'path':str(header), 'sha256':digest(header)}
                    if width is not None:
                        flags += ['-DWASIP1_DEFAULT_DENY_WIDTH='+str(width)]
                    original_object = out/(name+'.o')
                    command = [*config['compiler'], *flags, '-include', str(barrier), '-MD', '-MF', str(dep),
                               '-c', str(source/relative), '-o', str(original_object)]
                    row['build'] = run(command, name+'-build', out/(name+'.build.log'), 180, cwd=work)
                    if row['build']['returncode']:
                        raise RuntimeError('compiler failed')
                    row['dependencies'] = dependencies(dep, relative, system_inputs=l01_context['system_inputs'])
                    row['object_elf'] = require_arm(original_object)
                    objects = [str(original_object)]
                    if legacy:
                        witness = out/(name+'.witness.cc')
                        witness.write_text(legacy_witness(name, '_wasm64' in relative or '/hybrid/' in relative))
                        witness_fact = {'path': str(witness), 'sha256': digest(witness),
                                        'purpose': 'Successful unchanged original main return witness; compile-time WASI feature guards'}
                        witness_object, witness_dep = out/(name+'.witness.o'), out/(name+'.witness.d')
                        command = [*config['compiler'], *flags, '-MD', '-MF', str(witness_dep),
                                   '-c', str(witness), '-o', str(witness_object)]
                        row['witness_build'] = run(command, name+'-witness-build', out/(name+'.witness.build.log'), 180, cwd=work)
                        if row['witness_build']['returncode']:
                            raise RuntimeError('successful-return witness compiler failed')
                        row['witness_dependencies'] = dependencies(witness_dep, None,
                            generated_inputs={str(witness): witness_fact}, system_inputs=l01_context['system_inputs'])
                        row['witness_object_elf'] = require_arm(witness_object)
                        row['successful_return_witness'] = witness_fact
                        objects.append(str(witness_object))
                    command = [*config['compiler'], *config['link_flags'], '-pthread', *objects,
                               *(['-Wl,--wrap=main'] if legacy else []), '-o', str(binary)]
                    row['link'] = run(command, name+'-link', out/(name+'.link.log'), 180, cwd=work)
                    if row['link']['returncode']:
                        raise RuntimeError('linker failed')
                else:
                    work = None
                    command = [*config['compiler'], *config['compile_flags'], '-UNDEBUG', '-std=c++'+str(standard),
                               '-O1', '-g0', '-pthread', '-include', str(barrier),
                               '-I', str(source), '-I', str(source/'src')]
                    if standard == 26:
                        command += ['-I', str(source/'third-parties/fast_io/include')]
                    command += ['-MMD', '-MF', str(dep), str(source/relative), '-o', str(binary)]
                    row['build'] = run(command, name+'-build', out/(name+'.build.log'), 180)
                    if row['build']['returncode']:
                        raise RuntimeError('compiler failed')
                    row['dependencies'] = dependencies(dep, relative)
                row['artifact'] = evidence.target(name, binary)
                row['run'] = run([*config['qemu_argv'], str(binary)], name+'-run', out/(name+'.log'), 60, name, cwd=work)
                log = (out/(name+'.log')).read_text(errors='replace')
                if row['run']['returncode'] or not semantic_result(name, log, legacy):
                    raise RuntimeError('ARM unit failed or missing semantic PASS marker')
                row['status'] = 'PASS'
            except Exception:
                row['error'] = traceback.format_exc()
            print(row['status'], name, flush=True)
        for relative in manifest:
            checked(relative)
        if digest(args.manifest) != report['source_manifest_sha256'] or digest(args.config) != report['config_sha256']:
            raise RuntimeError('manifest or compiler config changed')
        if digest(config['compiler'][0]) != report['compiler_sha256'] or digest(config['qemu_argv'][0]) != emulator['sha256']:
            raise RuntimeError('compiler or emulator changed')
        if l01_context is not None:
            if digest(l01_context['builder']) != l01_context['builder_sha256'] or digest(l01_context['sdk_manifest']) != l01_context['sdk_manifest_sha256']:
                raise RuntimeError('Worker flag builder or cross SDK inventory changed')
            verify_sdk_inventory(l01_context['sdk_root'], l01_context['sdk_inventory'])
            if resource_inventory(l01_context['resource_root'])[0] != l01_context['resource_rows']:
                raise RuntimeError('compiler resource header closure changed')
            report['source_compiler_closure']['source_sdk_resource_closure_unchanged'] = True
            if 'wasip1_default_deny' in report:
                report['wasip1_default_deny']['source_sdk_resource_closure_unchanged'] = True
        report['status'] = 'PASS' if report['suites'] and all(row['status'] == 'PASS' for row in report['suites']) else 'FAIL'
    except Exception:
        report['error'] = traceback.format_exc()
    finally:
        stop_active()
        facts = evidence.finish(report['status'], {'unit_driver_sha256': report['driver_sha256']})
        try:
            after_identity = evidence.leaf.stat()
            report['same_cgroup_incarnation'] = (facts['same_cgroup_incarnation'] and
                (after_identity.st_dev, after_identity.st_ino) == (leaf_identity.st_dev, leaf_identity.st_ino))
            report['memory_swap_max_after'] = (evidence.leaf/'memory.swap.max').read_text().strip()
        except OSError as error:
            report['same_cgroup_incarnation'] = False
            report['memory_swap_max_after'] = None
            report['cgroup_incarnation_error'] = str(error)
        report['architecture_evidence'] = {'path': str(evidence.out/'architecture-evidence.json'),
                                           'sha256': digest(evidence.out/'architecture-evidence.json'),
                                           'status': facts['status']}
        if facts['status'] != 'PASS' or not report['same_cgroup_incarnation'] or report['memory_swap_max_after'] != swap_before:
            report['status'] = 'FAIL'
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    print(report['status'], out/'results.json', flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
