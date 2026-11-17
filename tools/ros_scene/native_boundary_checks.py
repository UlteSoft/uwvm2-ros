#!/usr/bin/env python3
"""Check native exclusion, disabled ports and atomic scene membership.

This is a compiler/authority regression suite, not vendor-driver confinement
or hardware qualification. Build immutable inputs with -O3 -g in the existing
64 GiB zero-swap cgroup. Every injected allocation failure must leave the
module's imports hidden and allow a later complete binding.
"""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import time

from device_dispatch_checks import ROOT, GROUP, guard, sha


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    guard()
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    source = out/'source'
    fixture = Path('test/0084.ros-runtime/deployment_membership.cpp')
    native = Path('src/uwvm2/imported/ros/scene/native')
    flags = ['-std=c++23', '-O3', '-g', '-Wall', '-Wextra', '-Werror', '-pthread']
    variants = [('clang', '/usr/bin/clang++-22', ['-fsanitize=address,undefined']),
                ('gcc', '/usr/bin/g++-13', [])]
    report = dict(complete=False, scope=__doc__, commands=[], inputs={}, snapshots={},
                  cgroup=str(GROUP), cgroup_inode=GROUP.stat().st_ino,
                  memory_max=68719476736, swap_max=0,
                  events_before=(GROUP/'memory.events').read_text())

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(label, argv, expected=0):
        guard()
        start = time.monotonic()
        result = subprocess.run(argv, cwd=ROOT, capture_output=True, text=True,
                                timeout=360, env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1'))
        (out/(label+'.stdout')).write_text(result.stdout)
        (out/(label+'.stderr')).write_text(result.stderr)
        report['commands'].append(dict(label=label, argv=argv, exit=result.returncode,
                                       seconds=time.monotonic()-start))
        save()
        assert (result.returncode == 0) == (expected == 0), (label, result.stderr[-6000:])
        return result

    success = False
    try:
        deps = {ROOT/fixture, Path(__file__).resolve()}
        for name, compiler, extra in variants:
            result = run(name+'-dependencies', [compiler, *flags, *extra,
                '-I'+str(ROOT/'src'), '-I'+str(ROOT/'third-parties/fast_io/include'),
                '-MM', '-MT', 'boundary', str(ROOT/fixture)])
            words = shlex.split(result.stdout.replace('\\\n', ' '))
            assert words[0] == 'boundary:'
            deps.update(Path(p).resolve() for p in words[1:])
        deps.update((ROOT/native).glob('*.h'))
        ports = [ROOT/'src/uwvm2/imported/ros/scene'/name for name in
                 ('platform_agilex_motion.h', 'platform_agilex_receiver.h')]
        deps.update(ports)
        for original in sorted(deps):
            assert original.is_relative_to(ROOT)
            target = source/original.relative_to(ROOT)
            target.parent.mkdir(parents=True, exist_ok=True)
            digest = sha(original)
            shutil.copyfile(original, target)
            assert sha(target) == digest
            report['inputs'][str(original)] = digest
            report['snapshots'][str(target.relative_to(source))] = digest

        headers = sorted(p.name for p in (source/native).glob('*.h')
                         if p.name != 'external_process_only.h')
        for name, compiler, extra in variants:
            includes = ['-I'+str(source/'src'), '-I'+str(source/'third-parties/fast_io/include')]
            binary = out/(name+'-membership')
            run(name+'-membership-compile', [compiler, *flags, *extra, *includes,
                str(source/fixture), '-Wl,--wrap=malloc', '-Wl,--wrap=calloc',
                '-Wl,--wrap=realloc', '-o', str(binary)])
            result = run(name+'-membership', [str(binary)])
            assert 'PASS exact membership' in result.stdout
            probe = out/(name+'-probe.cpp')
            probe.write_text('#include <uwvm2/imported/ros/scene/native/external_process_only.h>\n')
            run(name+'-external-worker', [compiler, *flags, *includes, '-fsyntax-only', str(probe)])
            rejected = 0
            for header in headers:
                probe.write_text('#include <uwvm2/imported/ros/scene/native/'+header+'>\n')
                for macro in ('UWVM=2', 'UWVM_IMPORT_ROSV2', 'UWVM_IMPORT_ROS1', 'UWVM_IMPORT_ROS2'):
                    result = run(name+'-'+header+'-'+macro.replace('=', '-'),
                        [compiler, *flags, *includes, '-D'+macro,
                         '-ferror-limit=1' if name == 'clang' else '-fmax-errors=1',
                         '-fsyntax-only', str(probe)], expected=1)
                    assert 'ROSv2 native implementations belong in an isolated external worker' in result.stderr
                    rejected += 1
            probe.write_text(''.join('#include <'+str(p.relative_to(ROOT/'src'))+'>\n' for p in ports))
            result = run(name+'-disabled-ports', [compiler, *flags, *includes, '-E', '-P', str(probe)])
            assert not result.stdout.strip(), 'A retained vendor port became active'
            print(f'PASS {name}: membership allocation rollback, {rejected} native import refusals, disabled ports', flush=True)
        success = True
    finally:
        guard()
        report['inputs_unchanged'] = all(sha(p) == h for p, h in report['inputs'].items())
        report['snapshots_unchanged'] = all(sha(source/p) == h for p, h in report['snapshots'].items())
        report['events_after'] = (GROUP/'memory.events').read_text()
        report['complete'] = success and report['inputs_unchanged'] and report['snapshots_unchanged']
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
