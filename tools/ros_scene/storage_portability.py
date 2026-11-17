#!/usr/bin/env python3
"""Run storage security under native sanitizers/GCC and narrow/large-file ABIs.

Every compiler/test inherits the exact existing 64 GiB, zero-swap cgroup.
Cross builds reuse reviewed toolchain recipes; emulation verifies filesystem
semantics only, never performance or native ROS package portability. Test data
uses private tmpfs directories; binaries and evidence remain in SDK .tmp.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import time


ROOT = Path(__file__).resolve().parents[2]
GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--recipes', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--architectures', nargs='*', choices=['aarch64', 'armhf', 'i686'], default=['armhf', 'i686'])
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip() == '68719476736'
    assert (GATE/'memory.swap.max').read_text().strip() == '0'
    assert os.geteuid() != 0, 'permission-failure cases require unprivileged execution'
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    source = ROOT/'test/0084.ros-runtime/storage.cc'
    inputs = [source, ROOT/'src/uwvm2/imported/ros/scene/storage.h',
              ROOT/'src/uwvm2/imported/ros/scene/authority.h', ROOT/'src/uwvm2/imported/ros/runtime/resources.h']
    report = dict(scope='storage backend contracts and native offset widths; not VM imports/ROS package/performance qualification',
                  cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino, memory_max='68719476736', swap_max='0',
                  uid=os.geteuid(), sources={str(p.relative_to(ROOT)): sha(p) for p in inputs},
                  events_before=(GATE/'memory.events').read_text(), runs=[], complete=False)

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(label, command, env=None):
        started = time.monotonic()
        result = subprocess.run(command, cwd=ROOT, env=env, capture_output=True, timeout=240)
        (out/(label+'.stdout')).write_bytes(result.stdout)
        (out/(label+'.stderr')).write_bytes(result.stderr)
        report['runs'].append(dict(label=label, argv=command, returncode=result.returncode, seconds=time.monotonic()-started))
        save()
        if result.returncode:
            raise RuntimeError(label+': '+result.stderr.decode(errors='replace')[-5000:])
        print('PASS', label, result.stdout.decode(errors='replace')[-220:].strip(), flush=True)
        return result.stdout.decode(errors='replace')

    with tempfile.TemporaryDirectory(prefix='uwvm-storage-widths-', dir='/dev/shm') as temporary:
        env = dict(os.environ, UWVM_STORAGE_TEST_TMP=temporary)
        clang = shutil.which('clang++')
        gcc = shutil.which('g++-13')
        assert clang and gcc
        for label, compiler, extra in [
            ('native-sanitized', clang, ['-fsanitize=address,undefined', '-fno-omit-frame-pointer']),
            ('gcc13', gcc, []),
        ]:
            binary = out/label
            run(label+'-compile', [compiler, '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-pthread',
                                  '-Isrc', *extra, str(source), '-o', str(binary)])
            run(label, [str(binary)], env)
        for arch in args.architectures:
            compile_recipe = json.loads((args.recipes/arch/'host_api.default.cpp.o.argv.json').read_text())
            link_recipe = json.loads((args.recipes/arch/'link.argv.json').read_text())
            flags = [s for s in compile_recipe if s.startswith(('--target=', '--sysroot=', '--gcc-install-dir=', '-mfloat-abi=', '-march=', '-mcpu=', '-mfpu=')) or s in ('-m32', '-m64')]
            include = next(s for s in compile_recipe if '/bundled-runtimes/' in s and s.endswith('/include/c++/v1'))
            libs = list(dict.fromkeys(s for s in link_recipe if '/bundled-runtimes/' in s and s.endswith('.a')))
            sysroot = next(s.split('=', 1)[1] for s in flags if s.startswith('--sysroot='))
            assert libs and Path(include).is_dir()
            if arch == 'i686':
                prefix = [str(Path(sysroot)/'lib/ld-linux.so.2'), '--library-path',
                          ':'.join(str(Path(sysroot)/p) for p in ['lib/i386-linux-gnu', 'usr/lib/i386-linux-gnu'])]
            else:
                emulator = shutil.which({'armhf': 'qemu-arm', 'aarch64': 'qemu-aarch64'}[arch])
                assert emulator
                prefix = [emulator, '-L', sysroot]
            for width in [32, 64] if arch != 'aarch64' else [64]:
                label = arch+'-off'+str(width)
                binary = out/label
                command = [compile_recipe[0], '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-pthread',
                           '-nostdinc++', '-isystem', include, '-Isrc', *flags, '-D_FILE_OFFSET_BITS='+str(width), str(source),
                           '-o', str(binary), '-fuse-ld=lld', '-nostdlib++', '--unwindlib=none', *libs, '-lpthread', '-ldl', '-lm', '-latomic']
                run(label+'-compile', command)
                output = run(label, [*prefix, str(binary)], env)
                assert 'native widths off_t='+str(width)+' ' in output, 'requested file ABI was not selected'
    report['sources_unchanged'] = all(sha(ROOT/name) == digest for name, digest in report['sources'].items())
    report['events_after'] = (GATE/'memory.events').read_text()
    report['complete'] = report['sources_unchanged']
    save()
    assert report['complete'], 'Inputs changed during qualification; rerun against stable sources'


if __name__ == '__main__':
    main()
