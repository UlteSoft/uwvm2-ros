#!/usr/bin/env python3
"""Build independent interpreter products without any ROS development libraries."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import time


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for chunk in iter(lambda: f.read(1048576), b''):
            h.update(chunk)
    return h.hexdigest()


def inputs(root):
    directories = ['src', 'third-parties/fast_io/include',
                   'third-parties/bizwen/include', 'third-parties/boost_unordered/include']
    return {str(p.relative_to(root)): digest(p)
            for d in directories for p in sorted((root/d).rglob('*')) if p.is_file()}


def build_budget():
    """Conservatively bound compiler count from live inherited cgroup headroom."""
    # One translation unit at a time is the default delivery contract. Live
    # headroom may refuse a build; it never silently increases parallelism.
    result = {'jobs':1, 'memory_source':'unavailable', 'cpu_count':os.cpu_count()}
    membership = Path('/proc/self/cgroup')
    if not membership.exists():
        return result
    result['membership'] = membership.read_text().splitlines()
    relative = next((line[3:] for line in result['membership'] if line.startswith('0::')), None)
    if relative is None:
        return result
    directory = Path('/sys/fs/cgroup')/relative.lstrip('/')
    limits = []
    parent = directory
    while True:
        file = parent/'memory.max'
        if file.exists() and file.read_text().strip() != 'max':
            limits.append(int(file.read_text().strip()))
        if parent == Path('/sys/fs/cgroup'):
            break
        parent = parent.parent
    if limits and (directory/'memory.current').exists():
        maximum, current = min(limits), int((directory/'memory.current').read_text())
        estimate = min(8*1024**3, max(1024**3, maximum//4))
        reserve = min(16*1024**3, maximum//4)
        result.update(memory_source='live-cgroup-v2', memory_max=maximum, memory_current=current,
                      estimated_bytes_per_compiler=estimate, reserved_headroom_bytes=reserve)
        result['jobs'] = min(result['jobs'], max(0,(maximum-current-reserve)//estimate))
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--profile', choices=['none', 'ros1', 'ros2', 'dual', 'all'], default='all')
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--opt', choices=['0', '1', '2'], default='1')
    p.add_argument('--stdlib', choices=['libc++', 'libstdc++'], default='libc++')
    p.add_argument('--compile-timeout', type=int, default=300)
    a = p.parse_args()
    if not 1 <= a.compile_timeout <= 600:
        p.error('compile timeout must be 1..600 seconds')
    root = Path(__file__).resolve().parents[2]
    out = a.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    budget = build_budget()
    (out/'build-budget.json').write_text(json.dumps(budget,indent=2)+'\n')
    if not budget['jobs']:
        (out/'results.json').write_text(json.dumps({'status':'BLOCKED','reason':'insufficient live cgroup build headroom','budget':budget})+'\n')
        return 77
    snapshot = out/'source'
    if snapshot.exists():
        p.error('output source snapshot already exists; use a fresh output directory')
    before = inputs(root)
    for directory in ['src', 'third-parties/fast_io/include',
                      'third-parties/bizwen/include', 'third-parties/boost_unordered/include']:
        shutil.copytree(root/directory, snapshot/directory)
    copied = inputs(snapshot)
    if copied != before or inputs(root) != before:
        (out/'results.json').write_text(json.dumps({'status':'FAIL', 'reason':'source changed during snapshot'})+'\n')
        return 1
    (out/'source-manifest.json').write_text(json.dumps(copied, sort_keys=True, indent=2)+'\n')
    compiler = shlex.split(os.environ.get('CXX', 'clang++'))
    extra = shlex.split(os.environ.get('ROS_PRODUCT_CXXFLAGS', ''))
    base = ['-std=c++26', '-stdlib='+a.stdlib, '-O'+a.opt, '-g0', '-pthread',
            '-Wno-undefined-inline', '-fconstexpr-steps=8388608', '-DUWVM=2',
            '-DUWVM_USE_UWVM_INT', '-DUWVM_DISABLE_JIT', '-DUWVM_USE_THREAD_LOCAL',
            '-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64', '-DUWVM_ENABLE_WASIP1_SOCKET',
            '-DUWVM_VERSION_X=2', '-DUWVM_VERSION_Y=0', '-DUWVM_VERSION_Z=3', '-DUWVM_VERSION_S=0']
    for directory in ['src', 'third-parties/fast_io/include',
                      'third-parties/bizwen/include', 'third-parties/boost_unordered/include']:
        base += ['-I', str(snapshot/directory)]
    rows = []
    profiles = ['none', 'ros1', 'ros2', 'dual'] if a.profile == 'all' else [a.profile]
    for profile in profiles:
        dest = out/profile
        dest.mkdir()
        flags = [*base, *extra]
        if profile in ('ros1', 'dual'):
            flags += ['-DUWVM_IMPORT_ROS1']
        if profile in ('ros2', 'dual'):
            flags += ['-DUWVM_IMPORT_ROS2']
        commands = []
        processes = []
        started = time.monotonic()

        def compile_one(item):
            name, source = item
            command = [*compiler, *flags, '-c', str(snapshot/source), '-o', str(dest/(name+'.o'))]
            commands.append(command)
            with (dest/(name+'.log')).open('wb') as log:
                process = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
                member = Path(f'/proc/{process.pid}/cgroup')
                processes.append({'translation_unit':name,'pid':process.pid,
                                  'membership':member.read_text().splitlines() if member.exists() else None})
                try:
                    return process.wait(timeout=a.compile_timeout)
                except subprocess.TimeoutExpired:
                    process.kill(); process.wait(timeout=5); raise

        try:
            codes = [compile_one(item) for item in [
                ('runtime', 'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp'),
                ('main', 'src/uwvm2/uwvm/main.default.cpp'),
                ('host_api', 'src/uwvm2/uwvm/host_api.default.cpp')]]
            row = {'profile':profile, 'status':'FAIL', 'compile_returncodes':codes}
            if all(code == 0 for code in codes):
                binary = dest/'uwvm'
                command = [*compiler, '-stdlib='+a.stdlib, '-pthread',
                           *[str(dest/(n+'.o')) for n in ['main', 'runtime', 'host_api']],
                           '-ldl', '-o', str(binary)]
                commands.append(command)
                with (dest/'link.log').open('wb') as log:
                    result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=60)
                row['link_returncode'] = result.returncode
                if result.returncode == 0:
                    result = subprocess.run([str(binary), '--version'], stdout=subprocess.PIPE,
                                            stderr=subprocess.STDOUT, timeout=15)
                    (dest/'version.log').write_bytes(result.stdout)
                    row.update(status='PASS' if result.returncode == 0 else 'FAIL',
                               version_returncode=result.returncode, binary_sha256=digest(binary))
            row['seconds'] = round(time.monotonic()-started, 3)
        except FileNotFoundError as error:
            row = {'profile':profile, 'status':'BLOCKED', 'reason':str(error)}
        except subprocess.TimeoutExpired:
            row = {'profile':profile, 'status':'FAIL', 'reason':'bounded build timeout'}
        (dest/'commands.json').write_text(json.dumps(commands, indent=2)+'\n')
        (dest/'compiler-processes.json').write_text(json.dumps(processes,indent=2)+'\n')
        rows.append(row)
        print(row['status'], profile, flush=True)
    status = 'PASS' if all(r['status']=='PASS' for r in rows) else 'FAIL' if any(r['status']=='FAIL' for r in rows) else 'BLOCKED'
    (out/'results.json').write_text(json.dumps({'status':status, 'profiles':rows}, indent=2)+'\n')
    return 0 if status == 'PASS' else 77 if status == 'BLOCKED' else 1


if __name__ == '__main__':
    sys.exit(main())
