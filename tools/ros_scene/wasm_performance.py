#!/usr/bin/env python3
"""Compare real Wasm scene imports with official native rcl in the existing gate.

Prepare emits and compiles freestanding Wasm32 guests without executing a VM.
Other modes require an -O3 VM with recorded compile argv for all three TUs.
Guest batch timestamps exclude startup/LLVM compilation. Process wall time and
the remaining outside-measurement time are recorded separately; the latter
also includes initialization, warmup, timer calibration, output and teardown.
No measurement is called pure compilation time. VM and guest hashes are fixed
through each run. All compilers, guests, native baselines and VTune run inside
the same existing 64 GiB/zero-swap cgroup.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import statistics
import subprocess
import time


ROOT = Path(__file__).resolve().parents[2]
GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
MEMBERSHIP = '0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
SOURCE = ROOT/'test/0084.ros-runtime/performance_guest.c'
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('mode', choices=['prepare', 'smoke', 'baseline', 'profile'])
parser.add_argument('--runtime', type=Path)
parser.add_argument('--output', type=Path, default=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security/performance-wasm')
parser.add_argument('--native-directory', type=Path, default=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security/performance-typed')
parser.add_argument('--cpu', type=int, default=2)
parser.add_argument('--iterations', type=int, default=20000)
parser.add_argument('--batches', type=int, default=101)
parser.add_argument('--profile-iterations', type=int, default=300000)
parser.add_argument('--profile-batches', type=int, default=101)
parser.add_argument('--rounds', type=int, default=3)
parser.add_argument('--vtune', default='/opt/intel/oneapi/vtune/latest/bin64/vtune')
parser.add_argument('--clang', default='/usr/bin/clang-22')
args = parser.parse_args()


def guard():
    assert Path('/proc/self/cgroup').read_text().strip() == MEMBERSHIP
    assert (GATE/'memory.max').read_text().strip() == '68719476736'
    assert (GATE/'memory.swap.max').read_text().strip() == '0'


def digest(path):
    value = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(1024*1024), b''):
            value.update(block)
    return value.hexdigest()


guard()
if args.cpu not in os.sched_getaffinity(0):
    parser.error('Selected CPU is outside the inherited CPU affinity')
if not (1 <= args.iterations <= 1000000 and 3 <= args.batches <= 501
        and 1 <= args.profile_iterations <= 1000000 and 3 <= args.profile_batches <= 501 and 1 <= args.rounds <= 9):
    parser.error('Iteration/batch/round bounds exceeded')
out = args.output.resolve()
out.mkdir(parents=True, exist_ok=True)
record = dict(mode=args.mode, cgroup=MEMBERSHIP, cgroup_inode=GATE.stat().st_ino, memory_max=68719476736,
              swap_max=0, cpu=args.cpu, uname=list(os.uname()), events_before=(GATE/'memory.events').read_text(),
              loadavg_before=Path('/proc/loadavg').read_text(), runs=[], observations=[], complete=False,
              scope='actual Wasm32 LLVM AOT scene imports and official native rcl local clock/guard workloads',
              native_parity_qualified=False, guest_jit_source_mapping_qualified=False)


def save():
    (out/(args.mode+'.json')).write_text(json.dumps(record, indent=2)+'\n')


def run(label, command, *, env=None, timeout=240):
    guard()
    start = time.monotonic()
    result = subprocess.run(command, cwd=ROOT, capture_output=True, env=env, timeout=timeout)
    elapsed = time.monotonic()-start
    (out/(label+'.stdout')).write_bytes(result.stdout)
    (out/(label+'.stderr')).write_bytes(result.stderr)
    record['runs'].append(dict(label=label, argv=command, exit=result.returncode, wall_seconds=elapsed))
    save()
    print(label, result.returncode, flush=True)
    if result.returncode:
        print(result.stderr.decode(errors='replace')[-8000:], flush=True)
        raise SystemExit(result.returncode)
    return result.stdout, elapsed


def scene_policy(workload):
    result = ['--ros-scene-create', 'benchmark', '--ros-scene-bind', 'benchmark', 'main']
    interfaces = ['abi', 'capability.open']
    grants = []
    if workload != 'guard':
        interfaces += ['clock.create', 'clock.read']
        grants += [('clock', 'clock.steady', ['clock.create', 'clock.read'])]
    if workload != 'clock':
        interfaces += ['guard.create', 'guard.trigger', 'guard.take', 'wait.create', 'wait.add', 'wait.poll']
        grants += [('guard', 'guard', ['guard.create', 'guard.trigger', 'guard.take', 'resource.observe']),
                   ('wait', 'wait', ['wait.create', 'wait.add', 'wait.poll'])]
    for function in interfaces:
        result += ['--ros-scene-interface', 'benchmark', function, 'allow']
    for alias, purpose, actions in grants:
        result += ['--ros-scene-grant', 'benchmark', alias, purpose, alias, 'local', ','.join(actions)]
        for field in ['max-handles', 'max-resources']:
            result += ['--ros-scene-limit', 'benchmark', alias, field, '1']
    if workload != 'clock':
        result += ['--ros-scene-limit', 'benchmark', 'wait', 'max-wait-members', '1',
                   '--ros-scene-limit', 'benchmark', 'wait', 'max-wait-ns', '0']
    # Measurement permissions are independent of the scene environment. They
    # grant no clocks or stdout through the scene namespace and no file/socket IO.
    result += ['--wasip1-single-create', 'main', '--wasip1-single-enable', 'main',
               '--wasip1-single-interface', 'main', 'clock_time_get', 'allow',
               '--wasip1-single-interface', 'main', 'fd_write', 'allow',
               '--wasip1-single-grant', 'main', 'monotonic-clock',
               '--wasip1-single-grant', 'main', 'stdout.write']
    return result


if args.mode == 'prepare':
    snapshot = out/'performance_guest.c'
    shutil.copyfile(SOURCE, snapshot)
    record['source_sha256'] = digest(SOURCE)
    record['fixtures'] = {}
    run('compiler-version', [args.clang, '--version'])
    linker = shutil.which('wasm-ld')
    if not linker:
        raise SystemExit('wasm-ld not available')
    configurations = [('smoke', 1000, 3), ('baseline', args.iterations, args.batches),
                      ('profile', args.profile_iterations, args.profile_batches)]
    for mode, iterations, batches in configurations:
        for number, workload in enumerate(['clock', 'guard', 'mixed']):
            name = mode+'-'+workload
            wasm = out/(name+'.wasm')
            command = [args.clang, '--target=wasm32-unknown-unknown', '-O3', '-g', '-Wall', '-Wextra', '-Werror',
                       '-nostdlib', '-fno-builtin', '-fuse-ld='+linker, '-Wl,--no-entry', '-Wl,--export=_start',
                       '-Wl,--initial-memory=131072', '-Wl,--max-memory=131072',
                       '-DBENCH_WORKLOAD='+str(number), '-DBENCH_ITERATIONS='+str(iterations),
                       '-DBENCH_BATCHES='+str(batches), str(snapshot), '-o', str(wasm)]
            run('compile-'+name, command)
            record['fixtures'][name] = dict(sha256=digest(wasm), iterations=iterations, batches=batches,
                                           policy_argv=scene_policy(workload))
    assert digest(SOURCE) == record['source_sha256'], 'Guest source changed during build'
else:
    if not args.runtime:
        parser.error('--runtime is required after prepare')
    runtime = args.runtime.resolve()
    record['vm_sha256'] = digest(runtime)
    # A file named release is not sufficient evidence: inspect every VM TU's
    # saved compile argv and preserve its hash in the measurement record.
    record['vm_compile_commands'] = {}
    for unit in ['main.default.cpp', 'uwvm_runtime.default.cpp', 'host_api.default.cpp']:
        path = runtime.parent/(unit+'.o.argv.json')
        command = json.loads(path.read_text())
        levels = [arg for arg in command if re.fullmatch(r'-O(?:[0-3szg]|fast)', arg)]
        if not levels or levels[-1] != '-O3' or '-c' not in command or any(arg.startswith('-fsanitize=') for arg in command):
            raise SystemExit('VM translation unit lacks recorded -O3 compilation: '+unit)
        record['vm_compile_commands'][unit] = dict(sha256=digest(path), argv=command)
    prepared = json.loads((out/'prepare.json').read_text())
    record['prepare_sha256'] = digest(out/'prepare.json')
    if not prepared.get('complete') or digest(SOURCE) != prepared['source_sha256']:
        raise SystemExit('Guest source differs from completed preparation')
    record['source_sha256'] = prepared['source_sha256']
    record['vm_build_receipts'] = {path.name: digest(path) for path in sorted(runtime.parent.glob('*results.json'))}
    full_build = json.loads((runtime.parent/'full-build-results.json').read_text())
    final_build = json.loads((runtime.parent/'incremental-results.json').read_text())
    if (full_build.get('optimization') != 'O3' or not full_build.get('complete')
            or {unit['unit'] for unit in full_build['units'] if unit['returncode'] == 0}
               != {'main.default.cpp.o', 'uwvm_runtime.default.cpp.o', 'host_api.default.cpp.o'}
            or not final_build.get('complete') or not final_build.get('sources_unchanged') or final_build.get('link_returncode') != 0):
        raise SystemExit('VM release receipts do not establish a successful unchanged -O3 build')
    record['vm_scene_sources'] = final_build['sources']
    for name, expected in record['vm_scene_sources'].items():
        if digest(ROOT/name) != expected:
            raise SystemExit('VM scene source differs from final compilation: '+name)
        snapshot = out/'sources'/name
        snapshot.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(ROOT/name, snapshot)
    native_dir = args.native_directory.resolve()
    native = native_dir/'native'
    native_manifest = json.loads((native_dir/'prepare.json').read_text())
    if digest(native) != native_manifest['binary_sha256']['native']:
        raise SystemExit('Native executable differs from its prepared build')
    record['native_sha256'] = digest(native)
    record['native_prepare_sha256'] = digest(native_dir/'prepare.json')
    for path, expected in native_manifest['native_libraries'].items():
        if digest(native_dir/'native-libs'/Path(path).name) != expected:
            raise SystemExit('Native library closure changed: '+path)
    environments = {'native': dict(os.environ, LD_LIBRARY_PATH=str(native_dir/'native-libs'), RMW_IMPLEMENTATION='rmw_fastrtps_cpp'),
                    'wasm': dict(os.environ)}

    def command_for(provider, workload):
        fixture = prepared['fixtures'][args.mode+'-'+workload]
        if provider == 'native':
            return ['taskset', '-c', str(args.cpu), str(native), workload, str(fixture['iterations']), str(fixture['batches'])]
        wasm = out/(args.mode+'-'+workload+'.wasm')
        if digest(wasm) != fixture['sha256']:
            raise SystemExit('Guest binary changed: '+str(wasm))
        return ['taskset', '-c', str(args.cpu), str(runtime), '--wasm-set-main-module-name', 'main', '--mode', 'run',
                '--runtime-aot', '--runtime-llvm-jit-policy', 'max', '--runtime-llvm-jit-cache-path', 'disable',
                '--runtime-compile-threads', '1', *fixture['policy_argv'], '--run', str(wasm)]

    def observation(provider, workload, data, wall, round_number):
        candidates = [line for line in data.decode().splitlines() if line.startswith('{"provider":')]
        if len(candidates) != 1:
            raise SystemExit('Expected one benchmark JSON report')
        value = json.loads(candidates[0])
        fixture = prepared['fixtures'][args.mode+'-'+workload]
        assert value['workload'] == workload and value['iterations_per_batch'] == fixture['iterations']
        assert value['batches'] == fixture['batches'] and value['logical_operations'] == fixture['iterations']*fixture['batches']
        if workload == 'guard':
            assert value['checksum'] == value['logical_operations']+1000, 'Guard delivery count differs from the native logical cycle'
        if provider == 'wasm':
            assert value['wasm_executed'] and value['provider'] == 'scene-wasm-aot'
            value['elapsed_seconds'] = value['elapsed_ns']/1e9
            for quantile in [50, 95, 99]:
                value['batch_mean_ns_p'+str(quantile)] = value['batch_ns_p'+str(quantile)]/fixture['iterations']
            value['guest_sha256'] = fixture['sha256']
            value['timer_pair_mean_ns_per_iteration'] = value['timer_pair_ns_p50']/fixture['iterations']
        else:
            assert not value['wasm_executed'] and value['provider'] == 'native-rcl'
        value['process_wall_seconds'] = wall
        value['outside_measured_region_seconds'] = None if wall is None else wall-value['elapsed_seconds']
        value['round'] = round_number
        record['observations'].append(value)
        save()

    if args.mode in ('smoke', 'baseline'):
        for round_number in range(1 if args.mode == 'smoke' else args.rounds):
            for workload in ['clock', 'guard', 'mixed']:
                for provider in (['wasm', 'native'] if round_number % 2 == 0 else ['native', 'wasm']):
                    data, wall = run(provider+'-'+workload+'-'+str(round_number), command_for(provider, workload), env=environments[provider])
                    observation(provider, workload, data, wall, round_number)
        record['comparison'] = []
        for workload in ['clock', 'guard', 'mixed']:
            medians = {provider: statistics.median(value['batch_mean_ns_p50'] for value in record['observations']
                if value['provider'] == provider and value['workload'] == workload) for provider in ['scene-wasm-aot', 'native-rcl']}
            record['comparison'].append(dict(workload=workload, median_of_round_p50_ns=medians,
                                             wasm_to_native_ratio=medians['scene-wasm-aot']/medians['native-rcl']))
    else:
        run('vtune-version', [args.vtune, '-version'])
        record['profile_includes_startup_and_compilation'] = True
        for provider in ['wasm', 'native']:
            for analysis in ['hotspots', 'uarch-exploration']:
                result = out/('vtune-'+provider+'-'+analysis)
                if result.exists():
                    raise SystemExit('Refusing to overwrite existing VTune result '+str(result))
                knobs = (['-knob', 'sampling-mode=hw', '-knob', 'enable-stack-collection=true'] if analysis == 'hotspots'
                         else ['-knob', 'pmu-collection-mode=detailed', '-knob', 'dram-bandwidth-limits=false'])
                command = [args.vtune, '-collect', analysis, *knobs, '-knob', 'sampling-interval=1', '-no-analyze-system',
                           '-data-limit=300', '-return-app-exitcode', '-result-dir', str(result),
                           '-source-search-dir', str(ROOT), '--', *command_for(provider, 'mixed')]
                data, wall = run('collect-'+provider+'-'+analysis, command, env=environments[provider], timeout=360)
                # VTune command wall time includes collection/finalization; it
                # is not the target's startup time. Preserve the guest/native
                # timing report without making a wall-time comparison here.
                record['runs'][-1]['wall_time_includes_profiler'] = True
                observation(provider, 'mixed', data, None, analysis)
                for report_type in ['summary', 'hotspots']:
                    run('report-'+provider+'-'+analysis+'-'+report_type,
                        [args.vtune, '-report', report_type, '-result-dir', str(result), '-format', 'csv'])
        record['native_parity_qualified'] = False
    assert digest(runtime) == record['vm_sha256'], 'VM executable changed during measurement'
    assert digest(native) == record['native_sha256'], 'Native executable changed during measurement'
    assert all(digest(ROOT/name) == expected for name, expected in record['vm_scene_sources'].items()), 'VM scene source changed during measurement'

guard()
record['events_after'] = (GATE/'memory.events').read_text()
record['loadavg_after'] = Path('/proc/loadavg').read_text()
record['complete'] = True
save()
