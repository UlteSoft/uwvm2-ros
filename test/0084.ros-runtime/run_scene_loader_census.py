#!/usr/bin/env python3
"""Qualify actual scene loader identity and recoverable fast_io tail allocation.

This metadata fixture uses the real concrete host tuple and provider containers;
owned runtime/debugger admission is verified separately by actual VM tests.
All compilers and tests stay in the existing 64 GiB zero-swap cgroup.
"""
import argparse
import json
from pathlib import Path
import resource
import re
import subprocess
import sys
import time

sys.dont_write_bytecode = True
from run_scene_vm import ROOT, GATE, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compile-argv', type=Path, required=True)
    parser.add_argument('--link-argv', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--source-root', type=Path, default=ROOT,
                        help='Frozen production source tree; test fixtures remain in this checkout')
    args = parser.parse_args(); verify_gate(); resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    production = args.source_root.resolve()
    assert (production/'src/uwvm2/uwvm/imported/scene/host.h').is_file()
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    compile_flags = [flag for flag in json.loads(args.compile_argv.read_text()) if not flag.startswith('-fprebuilt-module-path=')]
    compile_flags = [flag.replace(str(ROOT/'src'),str(production/'src')).replace(str(ROOT/'third-parties'),str(production/'third-parties'))
                     for flag in compile_flags]
    compile_flags = [flag for flag in compile_flags if not re.fullmatch(r'-O(?:[0-3szg]|fast)',flag)
                     and flag not in ('-g','-g0','-g1','-g2','-g3','-gline-tables-only','-gline-directives-only')]
    assert '-I'+str(production/'src') in compile_flags
    compile_flags += ['-UUWVM_RUNTIME_LLVM_JIT', '-UUWVM_USE_LLVM_JIT', '-UUWVM_USE_DEFAULT_JIT',
                      '-DUWVM_DISABLE_JIT', '-UNDEBUG', '-O3', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-builtin-malloc', '-fno-builtin-calloc', '-fno-builtin-realloc']
    link_flags = json.loads(args.link_argv.read_text())
    source = ROOT/'test/0084.ros-runtime/scene_loader_census.cpp'
    paths = [Path(__file__), source, args.compile_argv.resolve(), args.link_argv.resolve(),
             ROOT/'test/0084.ros-runtime/run_scene_vm.py', production/'src/uwvm2/uwvm/runtime/storage/builtin_ros_loader.h', production/'src/uwvm2/uwvm/imported/scene/host.h',
             production/'src/uwvm2/uwvm/cmdline/params/scene.h', production/'src/uwvm2/uwvm/cmdline/params.h', *production.glob('src/uwvm2/imported/ros/scene/*.h')]
    pins = lambda: {str(path): sha(path) for path in paths}
    report = dict(complete=False, scope='actual compiled scene tuple census beyond 64, tail OOM, membership, replacement; ASan/UBSan; no guest execution',
                  inputs=pins(), source_root=str(production), cgroup=str(GATE), inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
                  events_before=(GATE/'memory.events').read_text(), commands=[])

    def save():
        report['events_after'] = (GATE/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(name, argv):
        verify_gate(); start = time.monotonic(); result = subprocess.run(argv, cwd=ROOT, capture_output=True, timeout=300)
        (out/(name+'.stdout')).write_bytes(result.stdout); (out/(name+'.stderr')).write_bytes(result.stderr)
        report['commands'].append(dict(name=name, argv=argv, exit=result.returncode, seconds=time.monotonic()-start))
        save(); assert result.returncode == 0, (name, result.returncode, result.stderr.decode(errors='replace')[-6000:])
        print('PASS', name, flush=True)

    try:
        obj, executable = out/'host.o', out/'host-preflight'
        run('compile', [*compile_flags, '-c', str(source), '-o', str(obj)])
        run('link', [link_flags[0], '-fsanitize=address,undefined', str(obj), *link_flags[1:], '-lcrypto', '-Wl,--wrap=malloc', '-Wl,--wrap=calloc', '-Wl,--wrap=realloc', '-o', str(executable)])
        run('execute', [str(executable)])
        report['inputs_after'] = pins()
        assert report['inputs'] == report['inputs_after'], 'qualified host sources changed'
        report['complete'] = True
    finally:
        save()


if __name__ == '__main__':
    main()
