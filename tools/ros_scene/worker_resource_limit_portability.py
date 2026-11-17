#!/usr/bin/env python3
"""Qualify real target-libc limit widths; emulation never measures performance.

Reuse the retained placement cross-compile receipt for AArch64, ARM soft/hard
float and IA32. Execute ARM targets with matching QEMU/sysroots and IA32 with
its real loader. Narrow targets additionally use _FILE_OFFSET_BITS=64. All
compiler, loader and emulator processes inherit the existing 64 GiB gate.
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
    parser.add_argument('--source-root', type=Path, required=True)
    parser.add_argument('--recipes', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    a = parser.parse_args()
    verify_gate()
    source = a.source_root.resolve()
    manifest = json.loads((source/'source-manifest.json').read_text())
    assert all(sha(source/name) == value for name, value in manifest.items())
    recipe = a.recipes.resolve()
    previous = json.loads(recipe.read_text())
    assert previous['complete']
    out = a.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    fixture = ROOT/'test/0084.ros-runtime/linux_resource_limit_abi.cpp'
    retained = out/fixture.name
    shutil.copyfile(fixture, retained)
    pins = {str(p): sha(p) for p in [Path(__file__), fixture, retained, recipe, source/'source-manifest.json']}
    report = dict(complete=False, scope=__doc__, inputs=pins, source_root=str(source),
                  cgroup=str(GATE), memory_max=68719476736, swap_max=0,
                  events_before=(GATE/'memory.events').read_text(), commands=[])

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(name, argv):
        verify_gate()
        argv = list(map(str, argv))
        started = time.monotonic()
        result = subprocess.run(argv, capture_output=True, timeout=180)
        (out/(name+'.stdout')).write_bytes(result.stdout)
        (out/(name+'.stderr')).write_bytes(result.stderr)
        report['commands'].append(dict(name=name, argv=argv, exit=result.returncode, seconds=time.monotonic()-started))
        save()
        assert result.returncode == 0, (name, result.stderr[-4000:])
        print(name+': '+result.stdout.decode().strip(), flush=True)
        return result

    success = False
    try:
        for arch in ['aarch64', 'arm', 'armhf', 'i686']:
            row = next(row for row in previous['commands'] if row['name'] == arch+'-execution_placement')
            argv = list(row['argv'])
            # Keep only reviewed architecture/libc flags; replace all source inputs.
            flags = [x for x in argv if x.startswith(('--target=', '--sysroot=', '--gcc-install-dir=', '-mfloat-abi=', '-march=', '-mcpu=', '-mfpu=')) or x in ('-m32', '-m64')]
            includes = next(x for x in argv if '/bundled-runtimes/' in x and x.endswith('/include/c++/v1'))
            library = Path(includes).parents[2]/'lib'
            archives = [library/name for name in ['libc++.a', 'libc++abi.a', 'libunwind.a']]
            sysroot = Path(next(x.split('=', 1)[1] for x in flags if x.startswith('--sysroot=')))
            for path in [Path(argv[0]), *archives]:
                assert path.is_file()
                pins[str(path)] = sha(path)
            if arch == 'i686':
                loader = sysroot/'lib/ld-linux.so.2'
                prefix = [loader, '--library-path', ':'.join(str(sysroot/p) for p in ['lib/i386-linux-gnu', 'usr/lib/i386-linux-gnu'])]
                pins[str(loader)] = sha(loader)
            else:
                emulator = shutil.which('qemu-aarch64' if arch == 'aarch64' else 'qemu-arm')
                assert emulator
                pins[emulator] = sha(emulator)
                prefix = [emulator, '-L', sysroot]
            for wide in ([False] if arch == 'aarch64' else [False, True]):
                name = arch+('-large-file' if wide else '-native-width')
                binary = out/name
                command = [argv[0], '-std=c++23', '-O3', '-g', '-UNDEBUG', '-Wall', '-Wextra', '-Werror',
                           '-nostdinc++', '-isystem', includes, '-I'+str(source/'src'), *flags]
                if wide:
                    command.append('-D_FILE_OFFSET_BITS=64')
                run(name+'-compile', [*command, retained, '-o', binary, '-fuse-ld=lld', '-nostdlib++',
                    '--unwindlib=none', *archives, '-lpthread', '-ldl', '-lm', '-latomic'])
                pins[str(binary)] = sha(binary)
                assert b'PASS actual libc' in run(name, [*prefix, binary]).stdout
        success = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(path) == value for path, value in pins.items())
        report['sources_unchanged'] = all(sha(source/name) == value for name, value in manifest.items())
        report['events_after'] = (GATE/'memory.events').read_text()
        report['complete'] = success and report['inputs_unchanged'] and report['sources_unchanged']
        save()
    assert report['complete']


if __name__ == '__main__':
    main()
