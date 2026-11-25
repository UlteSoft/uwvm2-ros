#!/usr/bin/env python3
"""Qualify the closed volatile-cache FP whitelist in the existing memory gate.

Core tests use Clang ASan/UBSan and GCC 13 against real broker/provider code
with 48 control profiles. Wasm tests verify dynamic arithmetic immediately
after the actual seven output wrappers and independently guarded read. Exact
replay, short receipts, denied interfaces and absent grants are included.
No WASI, SDK or runtime implementation is modified by this driver.
"""
import argparse
import json
from pathlib import Path
import resource
import shutil
import subprocess
import sys
import time

sys.dont_write_bytecode = True
import run_scene_output_boundaries as boundary
base = boundary.base
ROOT, GATE = base.ROOT, base.GATE


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode', choices=('core', 'prepare', 'wasm', 'all'), default='all')
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Frozen production source tree used to build the qualified executable')
    args = parser.parse_args(); base.verify_gate(); resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    if args.mode in ('wasm', 'all') and args.runtime is None: parser.error('--runtime is required for actual VM qualification')
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    sources = [Path(__file__), ROOT/'test/0084.ros-runtime/output_fp_control.cc',
               *[ROOT/'test/0084.ros-runtime'/name for name in ('run_scene_output_boundaries.py', 'run_scene_vm.py', 'run_scene_fp.py', 'run_scene_memtag.py')],
               ROOT/'test/0008.imported/wasi/wasip1/func/fp_control_probe.h', ROOT/'src/uwvm2/uwvm/imported/scene/host.h',
               *ROOT.glob('src/uwvm2/imported/ros/scene/*.h'), *ROOT.glob('src/uwvm2/imported/ros/runtime/*.h')]
    if args.runtime: sources.append(args.runtime.resolve())
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    sources = [production_root/path.resolve().relative_to(ROOT) if path.resolve().is_relative_to(ROOT/'src') else path.resolve() for path in sources]
    pins = lambda: {str(path): base.sha(path) for path in sources}
    report = dict(complete=False, mode=args.mode, scope='audited volatile output FP controls; no external provider or all-architecture claim',
                  inputs=pins(), cgroup=str(GATE), inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
                  events_before=(GATE/'memory.events').read_text(), commands=[], guests=[])

    report['production_source_root'] = str(production_root)
    report['fixture_source_root'] = str(ROOT)

    def save():
        report['events_after'] = (GATE/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(name, argv):
        base.verify_gate(); start = time.monotonic(); result = subprocess.run(argv, cwd=ROOT, capture_output=True, timeout=180)
        (out/(name+'.stdout')).write_bytes(result.stdout); (out/(name+'.stderr')).write_bytes(result.stderr)
        report['commands'].append(dict(name=name, argv=argv, exit=result.returncode, seconds=time.monotonic()-start))
        save(); assert result.returncode == 0, (name, result.returncode, result.stderr.decode(errors='replace')[-6000:])
        print('PASS', name, flush=True)

    try:
        if args.mode in ('core', 'all'):
            source = ROOT/'test/0084.ros-runtime/output_fp_control.cc'
            for compiler, flags, name in [('clang++', ['-fsanitize=address,undefined', '-fno-omit-frame-pointer'], 'clang-asan'),
                                          ('g++-13', [], 'gcc13')]:
                tool = shutil.which(compiler); assert tool
                binary = out/name
                run(name+'-compile', [tool, '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-pthread',
                                     '-I'+str(production_root/'src'), *flags, str(source), '-o', str(binary)])
                run(name, [str(binary)])
        if args.mode in ('prepare', 'wasm', 'all'):
            for width, memory in ((32, 0), (64, 0), (32, 1), (64, 1)):
                for mode in ('fp', 'denied', 'no-grants'):
                    guest = boundary.guest(width, memory, mode); stem = f'{mode}-memory{width}-index{memory}'
                    wat, wasm = out/(stem+'.wat'), out/(stem+'.wasm'); wat.write_text(guest.render())
                    run(stem+'-compile', ['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)])
                    report['guests'].append(dict(name=stem, sha256=base.sha(wasm), fp_sites=guest.fp_sites,
                                                checks_per_site=12, assertions=len(guest.checks)))
                    save()
                    if args.mode == 'prepare': continue
                    flags = boundary.configured() if mode == 'fp' else base.policy(boundary.OPERATIONS, 'deny' if mode == 'denied' else 'allow')
                    for engine in ('int', 'aot'):
                        argv = [str(args.runtime.resolve()), '--wasm-set-main-module-name', 'main', '--runtime-'+engine,
                                '--runtime-compile-threads', '1', '--log-color', 'disable', *flags]
                        if engine == 'aot': argv += ['--wasm-policy-global', 'runtime.llvm.policy', 'max', '--runtime-llvm-jit-cache-path', 'disable']
                        run(stem+'-'+engine, [*argv, '--run', str(wasm)])
        report['inputs_after'] = pins()
        assert report['inputs'] == report['inputs_after'], 'FP qualification sources changed'
        report['complete'] = True
    finally:
        save()


if __name__ == '__main__':
    main()
