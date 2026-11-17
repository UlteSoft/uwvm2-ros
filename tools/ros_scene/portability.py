#!/usr/bin/env python3
"""Cross-compile core security contracts using existing reviewed build recipes.

Run through in_existing_cgroup.py. All compilers and QEMU children inherit the
same exact 64 GiB gate. Emulation qualifies correctness only, never performance
or real device behavior. No package or SDK binding coverage is inferred.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--recipes', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--architectures', nargs='+', choices=['aarch64','armhf','i686'], default=['aarch64','armhf','i686'])
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip() == '68719476736'
    assert (GATE/'memory.swap.max').read_text().strip() == '0'
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    tests = ['resources', 'authority', 'broker_security', 'deployment']
    sources = [ROOT/'src/uwvm2/imported/ros/scene'/name for name in
               ['authority.h','broker.h','deployment.h','debug.h','artifact_contract.h','artifacts.h','storage.h','output_contract.h','outputs.h','byte_buffer.h','managed_lifecycle.h','managed_lifecycle_contract.h']]
    sources += sorted((ROOT/'src/uwvm2/imported/ros/runtime').glob('*.h'))
    sources += [ROOT/'test/0084.ros-runtime'/(name+'.cc') for name in tests]
    report = dict(scope='QEMU/native-IA32 core contract correctness; no VM/provider/hardware performance claim',
                  cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino, memory_max='68719476736', swap_max='0',
                  events_before=(GATE/'memory.events').read_text(), sources={str(p.relative_to(ROOT)):sha(p) for p in sources},
                  runs=[], complete=False)
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    def run(label, command):
        started = time.monotonic()
        result = subprocess.run(command, cwd=ROOT, capture_output=True, timeout=180)
        (out/(label+'.stdout')).write_bytes(result.stdout)
        (out/(label+'.stderr')).write_bytes(result.stderr)
        report['runs'].append(dict(label=label, argv=command, returncode=result.returncode, seconds=time.monotonic()-started))
        save()
        if result.returncode:
            raise RuntimeError(label+': '+result.stderr.decode(errors='replace')[-4000:])
        print('PASS', label, flush=True)
    for arch in args.architectures:
        compile_recipe = json.loads((args.recipes/arch/'host_api.default.cpp.o.argv.json').read_text())
        link_recipe = json.loads((args.recipes/arch/'link.argv.json').read_text())
        flags = [s for s in compile_recipe if s.startswith(('--target=','--sysroot=','--gcc-install-dir=','-mfloat-abi=','-march=','-mcpu=','-mfpu=')) or s in ('-m32','-m64')]
        include = next(s for s in compile_recipe if '/bundled-runtimes/' in s and s.endswith('/include/c++/v1'))
        libs = list(dict.fromkeys(s for s in link_recipe if '/bundled-runtimes/' in s and s.endswith('.a')))
        sysroot = next((s.split('=',1)[1] for s in flags if s.startswith('--sysroot=')), None)
        assert libs and Path(include).is_dir()
        prefix = []
        if arch in ('aarch64','armhf'):
            emulator = shutil.which({'aarch64':'qemu-aarch64','armhf':'qemu-arm','i686':'qemu-i386'}[arch])
            assert emulator and sysroot
            prefix = [emulator, '-L', sysroot]
        elif arch == 'i686':
            assert sysroot
            prefix = [str(Path(sysroot)/'lib/ld-linux.so.2'), '--library-path',
                      ':'.join(str(Path(sysroot)/p) for p in ['lib/i386-linux-gnu','usr/lib/i386-linux-gnu'])]
        for name in tests:
            binary = out/(arch+'-'+name)
            command = [compile_recipe[0], '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-pthread',
                       '-nostdinc++', '-isystem', include, '-Isrc', *flags, str(ROOT/'test/0084.ros-runtime'/(name+'.cc')),
                       '-o', str(binary), '-fuse-ld=lld', '-nostdlib++', '--unwindlib=none', *libs, '-lpthread', '-ldl', '-lm', '-latomic']
            run(arch+'-'+name+'-compile', command)
            run(arch+'-'+name, [*prefix, str(binary)])
    report['sources_unchanged'] = all(sha(ROOT/name)==digest for name,digest in report['sources'].items())
    report['events_after'] = (GATE/'memory.events').read_text()
    report['complete'] = report['sources_unchanged']
    save()
    assert report['complete'], 'Sources changed while qualifying; rerun changed contracts'


if __name__ == '__main__':
    main()
