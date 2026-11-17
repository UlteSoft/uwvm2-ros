#!/usr/bin/env python3
"""Qualify frame byte/tag/growth semantics on four additional Linux ABIs.

Reuse the captured compiler, target-libc and C++ runtime recipes. IA32 executes
with its native loader; AArch64 and ARM soft/hard float use QEMU. Those runs
qualify functional assertions only, never native timing or hardware MTE.
All compilers/loaders/emulators must remain in the existing 64 GiB gate.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import time
from module_exchange_performance import ROOT, GATE, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', required=True, type=Path)
    parser.add_argument('--recipes', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    verify_gate()
    source, recipes, out = args.source_root.resolve(), args.recipes.resolve(), args.output.resolve()
    manifest = json.loads((source/'source-manifest.json').read_text())
    assert all(sha(source/name) == value for name, value in manifest.items())
    prior = json.loads(recipes.read_text())
    assert prior['complete']
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    fixture = source/'test/0084.ros-runtime/module_exchange_frame.cpp'
    pins = {str(path): sha(path) for path in [Path(__file__).resolve(), recipes, source/'source-manifest.json', fixture]}
    report = dict(complete=False, scope=__doc__, source_root=str(source), inputs=pins,
        cgroup=str(GATE), memory_max=68719476736, swap_max=0,
        events_before=(GATE/'memory.events').read_text(), commands=[])

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(label, argv):
        verify_gate()
        argv = list(map(str, argv))
        start = time.monotonic()
        process = subprocess.run(argv, capture_output=True, timeout=240)
        (out/(label+'.stdout')).write_bytes(process.stdout)
        (out/(label+'.stderr')).write_bytes(process.stderr)
        report['commands'].append(dict(name=label, argv=argv, exit=process.returncode, seconds=time.monotonic()-start))
        save()
        assert process.returncode == 0, (label, process.stderr[-5000:])
        return process

    success = False
    try:
        for arch in ('aarch64', 'arm', 'armhf', 'i686'):
            row = next(row for row in prior['commands'] if row['name'] == arch+'-execution_placement')
            argv = row['argv']
            flags = [value for value in argv if value.startswith(('--target=', '--sysroot=', '--gcc-install-dir=',
                      '-mfloat-abi=', '-march=', '-mcpu=', '-mfpu=')) or value in ('-m32', '-m64')]
            includes = next(value for value in argv if '/bundled-runtimes/' in value and value.endswith('/include/c++/v1'))
            library = Path(includes).parents[2]/'lib'
            archives = [library/name for name in ('libc++.a', 'libc++abi.a', 'libunwind.a')]
            sysroot = Path(next(value.split('=', 1)[1] for value in flags if value.startswith('--sysroot=')))
            for path in (Path(argv[0]), *archives):
                pins[str(path)] = sha(path)
            if arch == 'i686':
                loader = sysroot/'lib/ld-linux.so.2'
                runner = [loader, '--library-path', ':'.join(str(sysroot/name) for name in ('lib/i386-linux-gnu', 'usr/lib/i386-linux-gnu'))]
                pins[str(loader)] = sha(loader)
            else:
                emulator = shutil.which('qemu-aarch64' if arch == 'aarch64' else 'qemu-arm')
                assert emulator
                runner = [emulator, '-L', sysroot]
                pins[emulator] = sha(emulator)
            binary = out/arch
            command = [argv[0], '-std=c++26', '-O3', '-g', '-UNDEBUG', '-Wall', '-Wextra', '-Werror', '-pthread',
                       '-nostdinc++', '-isystem', includes, '-I'+str(source/'src'), *flags]
            for vendor in ('fast_io', 'bizwen', 'boost_unordered'):
                command += ['-I'+str(source/'third-parties'/vendor/'include')]
            run(arch+'-compile', [*command, fixture, '-o', binary, '-fuse-ld=lld', '-nostdlib++', '--unwindlib=none',
                                 *archives, '-lpthread', '-ldl', '-lm', '-latomic'])
            pins[str(binary)] = sha(binary)
            assert b'PASS complete exchange frames:' in run(arch+'-execute', [*runner, binary]).stdout
            print('PASS '+arch+' frame semantics (no timing claim)', flush=True)
        success = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(path) == value for path, value in pins.items())
        report['sources_unchanged'] = all(sha(source/name) == value for name, value in manifest.items())
        report['complete'] = success and report['inputs_unchanged'] and report['sources_unchanged']
        report['events_after'] = (GATE/'memory.events').read_text()
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
