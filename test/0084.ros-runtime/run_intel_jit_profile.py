#!/usr/bin/env python3
"""Qualify independent optional JIT helpers; never claim VM guest source mapping.

All descendants stay in the existing 64 GiB zero-swap gate. --profile performs
official hardware-hotspots and microarchitecture collection of a controlled
generated-native-code probe, separately from the frozen VM performance baseline.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def guard():
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736'
    assert (GROUP/'memory.swap.max').read_text().strip() == '0'


def digest(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--profile', action='store_true')
    parser.add_argument('--sdk', type=Path, default=Path('/opt/intel/oneapi/vtune/latest/sdk'))
    parser.add_argument('--vtune', default='/opt/intel/oneapi/vtune/latest/bin64/vtune')
    parser.add_argument('--clang', default=shutil.which('clang++'))
    parser.add_argument('--llvm-config', default=shutil.which('llvm-config'))
    parser.add_argument('--cpu', type=int, default=2)
    args = parser.parse_args()
    guard()
    assert args.cpu in os.sched_getaffinity(0)
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    inputs = [ROOT/p for p in [
        'src/uwvm2/runtime/lib/uwvm_runtime_intel_jit_profile.h',
        'src/uwvm2/runtime/lib/uwvm_runtime_intel_jit_listener.h',
        'test/0084.ros-runtime/intel_jit_profile.cpp',
        'test/0084.ros-runtime/intel_jit_listener.cpp',
        'test/0084.ros-runtime/intel_jit_probe.cpp',
        'test/0084.ros-runtime/intel_jit_probe_source.c',
        'test/0084.ros-runtime/run_intel_jit_profile.py']]
    inputs += [args.sdk/'include/jitprofiling.h', args.sdk/'lib64/libjitprofiling.a']
    pins = lambda: {str(p): digest(p) for p in inputs}
    report = dict(complete=False, vm_guest_mapping_qualified=False, native_ros_parity_qualified=False,
                  scope='independent optional registry/listener and controlled generated native-code probe only',
                  cgroup=str(GROUP), cgroup_inode=GROUP.stat().st_ino, memory_max=68719476736, swap_max=0,
                  events_before=(GROUP/'memory.events').read_text(), inputs=pins(), runs=[], observations={})
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    def run(label, argv, *, env=None, timeout=240):
        guard(); start = time.monotonic()
        result = subprocess.run([str(v) for v in argv], cwd=ROOT, env=env, capture_output=True, timeout=timeout)
        (out/(label+'.stdout')).write_bytes(result.stdout); (out/(label+'.stderr')).write_bytes(result.stderr)
        report['runs'].append(dict(label=label, argv=[str(v) for v in argv], exit=result.returncode, seconds=time.monotonic()-start)); save()
        if result.returncode:
            raise RuntimeError(label+' failed: '+result.stderr.decode(errors='replace')[-6000:])
        print('PASS', label, flush=True)
        return result.stdout.decode(errors='replace')
    common = ['-std=c++23', '-Wall', '-Wextra', '-Werror', '-O3', '-g', '-pthread', '-Isrc', '-Ithird-parties/fast_io/include']
    sdk = ['-DUWVM_RUNTIME_INTEL_JIT_PROFILE=1', '-I'+str(args.sdk/'include')]
    sanitize = ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']
    source = ROOT/'test/0084.ros-runtime/intel_jit_profile.cpp'
    save()
    try:
        run('compiler-version', [args.clang, '--version'])
        run('no-sdk-build', [args.clang, *common, *sanitize, source, '-o', out/'no-sdk'])
        run('no-sdk', [out/'no-sdk'])
        run('registry-build', [args.clang, *common, *sdk, *sanitize, source, '-o', out/'registry'])
        run('registry', [out/'registry'])
        gcc = shutil.which('g++-13')
        if gcc:
            run('gcc-registry-build', [gcc, *common, *sdk, source, '-o', out/'registry-gcc'])
            run('gcc-registry', [out/'registry-gcc'])
        cxxflags = shlex.split(run('llvm-cxxflags', [args.llvm_config, '--cxxflags']))
        cxxflags = [x for x in cxxflags if x != '-fno-exceptions' and not x.startswith('-std=')]
        cxxflags = [part for flag in cxxflags for part in (['-isystem', flag[2:]] if flag.startswith('-I') else [flag])]
        ldflags = shlex.split(run('llvm-link-flags', [args.llvm_config, '--ldflags', '--system-libs', '--libs', 'core', 'executionengine', 'mcjit', 'native']))
        run('listener-build', [args.clang, *cxxflags, *common, *sdk, '-DUWVM_RUNTIME_LLVM_JIT=1',
                               ROOT/'test/0084.ros-runtime/intel_jit_listener.cpp', *ldflags,
                               '-Wl,--wrap=malloc', '-Wl,--wrap=calloc', '-Wl,--wrap=realloc', '-o', out/'listener'])
        run('listener', [out/'listener'])
        run('probe-build', [args.clang, *common, *sdk, '-O3', ROOT/'test/0084.ros-runtime/intel_jit_probe.cpp',
                            args.sdk/'lib64/libjitprofiling.a', '-ldl', '-o', out/'probe'])
        marker = out/'collector-was-loaded'
        sentinel = out/'sentinel.c'
        sentinel.write_text('''#include <stdio.h>
#include <stdlib.h>
__attribute__((constructor)) static void loaded(void) {
  char const* p = getenv("UWVM_PROFILE_MARKER");
  if (p) { FILE* f = fopen(p,"w"); if (f) { fputs("loaded\\n",f); fclose(f); } }
}
unsigned Initialize(void) { return 1; }
unsigned NotifyEvent(unsigned kind, void* data) { (void)kind; (void)data; return 1; }
''')
        run('sentinel-build', [shutil.which('clang'), '-shared', '-fPIC', '-O3', '-g', sentinel, '-o', out/'sentinel.so'])
        source_label = out/'intel_jit_probe_source.c'
        shutil.copyfile(ROOT/'test/0084.ros-runtime/intel_jit_probe_source.c', source_label)
        env = dict(os.environ, INTEL_JIT_PROFILER64=str(out/'sentinel.so'), UWVM_PROFILE_MARKER=str(marker))
        run('default-off-real-sdk', [out/'probe', 'off', source_label, '0.02'], env=env)
        assert not marker.exists(), 'default-off accidentally loaded the collector'
        run('collector-sentinel-positive', [out/'probe', 'profile', source_label, '0.02'], env=env)
        assert marker.read_text() == 'loaded\n', 'positive control failed to detect collector loading'
        report['observations']['default_off_no_collector_load'] = True
        report['observations']['sentinel_positive_control'] = True
        if args.profile:
            report['vtune_sha256'] = digest(args.vtune)
            clean = dict(os.environ, DEBUGINFOD_URLS='')
            clean.pop('INTEL_JIT_PROFILER64', None); clean.pop('UWVM_PROFILE_MARKER', None)
            for kind in ['hotspots', 'uarch-exploration']:
                result = out/kind
                command = [args.vtune, '-collect', kind, '-result-dir', result]
                command += ['-knob', 'sampling-mode=hw', '-knob', 'enable-stack-collection=true'] if kind == 'hotspots' else [
                    '-knob', 'pmu-collection-mode=detailed', '-knob', 'dram-bandwidth-limits=false']
                command += ['--', 'taskset', '-c', str(args.cpu), out/'probe', 'profile', source_label, '3']
                run(kind+'-collect', command, env=clean, timeout=300)
                run(kind+'-summary', [args.vtune, '-report', 'summary', '-r', result, '-format', 'csv'], env=clean)
                symbols = run(kind+'-functions', [args.vtune, '-report', 'hotspots', '-r', result, '-format', 'csv', '-group-by', 'function', '-limit', '100'], env=clean)
                # Intel renders dotted JIT method names as C++-style scopes.
                normalized = symbols.replace('::', '.')
                assert 'uwvm.profile_probe.mix' in normalized and 'uwvm.profile_probe.reused' in normalized, 'both actual JIT generations must receive samples'
                lines = run(kind+'-source', [args.vtune, '-report', 'hotspots', '-r', result, '-format', 'csv', '-group-by', 'source-file,source-line', '-limit', '100'], env=clean)
                assert 'intel_jit_probe_source.c' in lines, 'controlled JIT source absent from report'
                sampled_lines = sorted(set(int(v) for v in re.findall(r'intel_jit_probe_source\.c\t(\d+)\t', lines)))
                assert {8, 9, 10}.issubset(sampled_lines), 'actual multiply/add/xor source rows absent'
                report['observations'][kind] = dict(symbols_sampled=True, controlled_source_sampled=True, sampled_lines=sampled_lines)
        report['inputs_after'] = pins()
        assert report['inputs'] == report['inputs_after'], 'qualification inputs changed during run'
        report['complete'] = True
    finally:
        report['events_after'] = (GROUP/'memory.events').read_text(); save()


if __name__ == '__main__':
    main()
