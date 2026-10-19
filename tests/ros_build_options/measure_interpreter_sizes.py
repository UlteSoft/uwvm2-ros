#!/usr/bin/env python3
"""Serial, cgroup-admitted interpreter size builds from one frozen source tree.

This direct compiler acceptance recipe is deliberately independent of xmake's
LLVM and OpenSSL dependency build. It omits LLVM execution, WSEA verification,
vendor registries and native debugging. It does not qualify those features or
claim that its result is a default xmake product. The source owner must freeze
the input tree and retain its complete manifest and the compiler's dependencies.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import resource
import signal
import subprocess
import time


def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for data in iter(lambda: f.read(1024 * 1024), b''):
            h.update(data)
    return h.hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--compiler', type=Path, required=True)
    p.add_argument('--profiles', nargs='+', choices=('none', 'ros1', 'ros2', 'dual'), default=['none', 'dual'])
    p.add_argument('--opt', choices=('s', '2'), default='s')
    a = p.parse_args()
    root, compiler = a.source.resolve(strict=True), a.compiler.absolute()
    compiler.resolve(strict=True)  # Keep the clang++ driver spelling for linking.
    group = Path('/sys/fs/cgroup') / next(line[3:].lstrip('/') for line in
            Path('/proc/self/cgroup').read_text().splitlines() if line.startswith('0::'))
    identity = (group.stat().st_dev, group.stat().st_ino)
    events = (group / 'memory.events').read_text()
    boot = Path('/proc/sys/kernel/random/boot_id').read_text()
    def admit():
        if (group.stat().st_dev, group.stat().st_ino) != identity or (
                (group / 'memory.max').read_text().strip() != str(64 * 1024**3) or
                (group / 'memory.swap.max').read_text().strip() != '0' or
                (group / 'memory.events').read_text() != events or
                Path('/proc/sys/kernel/random/boot_id').read_text() != boot or
                int((group / 'memory.current').read_text()) > 56 * 1024**3):
            raise RuntimeError('original 64 GiB/swap0 cgroup or live headroom changed')
    admit()
    out = a.out.absolute()
    out.mkdir(mode=0o700)
    spec = importlib.util.spec_from_file_location('observed_elf_size', root / 'sdk/tools/ros_binary_size.py')
    size = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(size)
    directories = ('src', 'third-parties/fast_io/include', 'third-parties/bizwen/include',
                   'third-parties/boost_unordered/include')
    def inventory():
        return {str(f.relative_to(root)): sha(f) for name in directories
                for f in sorted((root / name).rglob('*')) if f.is_file()}
    before = inventory()
    (out / 'source-manifest.json').write_text(json.dumps(before, sort_keys=True, indent=2) + '\n')
    commands = []
    def run(argv, label, timeout):
        admit()
        start = time.monotonic()
        def limits():
            resource.setrlimit(resource.RLIMIT_AS, (8 * 1024**3, 8 * 1024**3))
            resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
            resource.setrlimit(resource.RLIMIT_FSIZE, (512 * 1024**2, 512 * 1024**2))
        with (out / (label + '.log')).open('xb') as log:
            proc = subprocess.Popen(argv, stdin=subprocess.DEVNULL, stdout=log,
                    stderr=subprocess.STDOUT, start_new_session=True, preexec_fn=limits)
            membership = Path('/proc/%d/cgroup' % proc.pid).read_text()
            try:
                rc = proc.wait(timeout=timeout)
            except BaseException:
                try: os.killpg(proc.pid, signal.SIGKILL)
                except ProcessLookupError: pass
                proc.wait()
                raise
        commands.append(dict(argv=list(map(str, argv)), label=label, returncode=rc,
                             seconds=time.monotonic() - start, membership=membership))
        (out / 'commands.json').write_text(json.dumps(commands, indent=2) + '\n')
        if rc:
            raise RuntimeError('actual command failed: ' + label)
        admit()
    base = [str(compiler), '-std=c++26', '-stdlib=libc++', '-O' + a.opt, '-g0',
            '-pthread', '-fconstexpr-steps=8388608', '-Wno-undefined-inline',
            '-ffunction-sections', '-fdata-sections', '-DUWVM=2', '-DNDEBUG',
            '-DUWVM_USE_UWVM_INT', '-DUWVM_DISABLE_JIT', '-DUWVM_USE_THREAD_LOCAL',
            '-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64', '-DUWVM_ENABLE_WASIP1_SOCKET',
            '-DUWVM_VERSION_X=2', '-DUWVM_VERSION_Y=0', '-DUWVM_VERSION_Z=3', '-DUWVM_VERSION_S=0']
    for name in directories:
        base += ['-I', str(root / name)]
    results = []
    try:
        for profile in a.profiles:
            dest = out / profile
            dest.mkdir()
            flags = base + (['-DUWVM_IMPORT_ROS1'] if profile in ('ros1', 'dual') else [])
            flags += ['-DUWVM_IMPORT_ROS2'] if profile in ('ros2', 'dual') else []
            for name, source in (('runtime', 'src/uwvm2/runtime/lib/uwvm_runtime.default.cpp'),
                                 ('main', 'src/uwvm2/uwvm/main.default.cpp'),
                                 ('host_api', 'src/uwvm2/uwvm/host_api.default.cpp')):
                run(flags + ['-MD', '-MF', str(dest / (name + '.d')), '-c', str(root / source),
                             '-o', str(dest / (name + '.o'))], profile + '-' + name, 600)
            binary = dest / 'uwvm'
            run([str(compiler), '-stdlib=libc++', '-fuse-ld=lld', '-pthread',
                 *[str(dest / (n + '.o')) for n in ('main', 'runtime', 'host_api')],
                 '-Wl,--gc-sections', '-ldl', '-o', str(binary)], profile + '-link', 90)
            original = size.measure(binary)
            stripped = dest / 'uwvm.stripped'
            run(['/usr/bin/strip', '--strip-all', '-o', str(stripped), str(binary)], profile + '-strip', 30)
            run([str(stripped), '--version'], profile + '-version', 15)
            results.append(dict(profile=profile, original=original, stripped=size.measure(stripped)))
            print(json.dumps({'completed': profile, 'bytes': results[-1]['stripped']['bytes']}), flush=True)
        if inventory() != before:
            raise RuntimeError('frozen source changed during compilation')
        status, error = 'PASS', None
    except Exception as e:
        status, error = 'FAIL', str(e)
    report = dict(kind='serial-direct-interpreter-size-build-v1', status=status, error=error,
                  profiles=results, compiler=dict(path=str(compiler), sha256=sha(compiler)),
                  source_files=len(before), cgroup=dict(path=str(group), identity=identity,
                  memory_max=(group / 'memory.max').read_text(), swap_max=(group / 'memory.swap.max').read_text(),
                  events_before=events, events_after=(group / 'memory.events').read_text(), boot=boot),
                  children_peak_RSS_bytes=resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss * 1024,
                  default_authority=[], commands=commands,
                  exclusions=['LLVM execution', 'WSEA verifier', 'vendor schemas', 'native debugger',
                              'positive ROS delivery', 'xmake default-product qualification'])
    (out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')
    return 0 if status == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
