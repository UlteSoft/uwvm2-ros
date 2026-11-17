#!/usr/bin/env python3
"""Qualify the isolated bounded proposal core in the existing 64 GiB cgroup.

This is a native core receipt, not a VM/import/native robot qualification.
The small standalone source set excludes unrelated concurrent workspace edits.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--recipes', type=Path)
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736'
    assert (GROUP/'memory.swap.max').read_text().strip() == '0'
    assert os.sched_getaffinity(0).issubset({12, 13, 14, 15})
    output = args.output.resolve()
    assert output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    output.mkdir(parents=True, exist_ok=True)
    assert not (output/'results.json').exists(), 'Preserve previous receipts in a new directory'
    source = ROOT/'test/0084.ros-runtime/proposal_admission.cpp'
    compiled_inputs = [source, Path(__file__).resolve(), ROOT/'src/uwvm2/imported/ros/scene/proposal_admission.h',
               ROOT/'src/uwvm2/imported/ros/scene/proposal_contract.h']
    sources = list(compiled_inputs)
    if args.recipes:
        sources += [(args.recipes/arch/name).resolve() for arch in ('aarch64', 'armhf', 'i686')
                    for name in ('host_api.default.cpp.o.argv.json', 'link.argv.json')]
    def hashes():
        return {str(path.relative_to(ROOT) if path.is_relative_to(ROOT) else path):
                hashlib.sha256(path.read_bytes()).hexdigest() for path in sources}
    record = dict(complete=False, scope='isolated_native_proposal_admission_core', registered_imports=0,
                  sources=hashes(), commands=[], cgroup=str(GROUP), memory_max=68719476736, swap_max=0,
                  cgroup_inode=GROUP.stat().st_ino, boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                  affinity=sorted(os.sched_getaffinity(0)), events_before=(GROUP/'memory.events').read_text())
    snapshot = output/'source'
    assert not snapshot.exists(), 'Source snapshot must be newly created'
    snapshot.mkdir()
    for path in compiled_inputs:
        relative = path.relative_to(ROOT)
        destination = snapshot/relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(path.read_bytes())
        assert hashlib.sha256(destination.read_bytes()).hexdigest() == record['sources'][str(relative)], 'Input changed while freezing source'
    record['source_root'] = str(snapshot)
    record['source_snapshot'] = {str(path.relative_to(ROOT)): record['sources'][str(path.relative_to(ROOT))] for path in compiled_inputs}
    (snapshot/'manifest.json').write_text(json.dumps(record['source_snapshot'], indent=2)+'\n')
    compiled_source = snapshot/source.relative_to(ROOT)
    source_include = '-I'+str(snapshot/'src')
    def save():
        (output/'results.json').write_text(json.dumps(record, indent=2)+'\n')
    def run(label, argv, allow_failure=False):
        done = subprocess.run(argv, cwd=ROOT, capture_output=True, text=True, timeout=240,
                              env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1', TSAN_OPTIONS='halt_on_error=1'))
        (output/(label+'.stdout')).write_text(done.stdout)
        (output/(label+'.stderr')).write_text(done.stderr)
        record['commands'].append(dict(label=label, argv=argv, exit=done.returncode))
        save()
        print(label+': '+str(done.returncode), flush=True)
        if done.returncode and not allow_failure:
            raise RuntimeError(done.stderr[-6000:])
        return done
    marker = 'passed 10 bounded proposal-admission safety groups'
    passed = False
    try:
        for name, compiler, flags in [('sanitized', '/usr/bin/clang++-22', ['-fsanitize=address,undefined']),
                                     ('gcc', '/usr/bin/g++-13', []), ('thread', '/usr/bin/clang++-22', ['-fsanitize=thread'])]:
            binary = output/name
            run(name+'-compile', [compiler, '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-pthread',
                                 source_include, '-fno-omit-frame-pointer', *flags, str(compiled_source), '-o', str(binary)])
            initialized = False
            for attempt in range(1, 9 if name == 'thread' else 2):
                done = run(name+'-run-'+str(attempt), [str(binary)], name == 'thread')
                if not done.returncode:
                    assert marker in done.stdout
                    assert ('allocation_trap=disabled under TSan' if name == 'thread' else 'allocation_trap=enabled') in done.stdout
                    assert 'WARNING: ThreadSanitizer:' not in done.stderr
                    initialized = True
                    break
                if not (done.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not done.stdout):
                    raise RuntimeError(done.stderr[-6000:])
            assert initialized, 'ThreadSanitizer never initialized'
        if args.recipes:
            for arch in ('aarch64', 'armhf', 'i686'):
                compile_recipe = json.loads((args.recipes/arch/'host_api.default.cpp.o.argv.json').read_text())
                link_recipe = json.loads((args.recipes/arch/'link.argv.json').read_text())
                flags = [flag for flag in compile_recipe if flag.startswith(('--target=', '--sysroot=', '--gcc-install-dir=',
                         '-mfloat-abi=', '-march=', '-mcpu=', '-mfpu=')) or flag in ('-m32', '-m64')]
                include = next(flag for flag in compile_recipe if '/bundled-runtimes/' in flag and flag.endswith('/include/c++/v1'))
                libraries = list(dict.fromkeys(flag for flag in link_recipe if '/bundled-runtimes/' in flag and flag.endswith('.a')))
                sysroot = Path(next(flag.split('=', 1)[1] for flag in flags if flag.startswith('--sysroot=')))
                if arch == 'i686':
                    prefix = [str(sysroot/'lib/ld-linux.so.2'), '--library-path',
                              ':'.join(str(sysroot/path) for path in ('lib/i386-linux-gnu', 'usr/lib/i386-linux-gnu'))]
                else:
                    emulator = shutil.which({'aarch64': 'qemu-aarch64', 'armhf': 'qemu-arm'}[arch])
                    assert emulator
                    triple = {'aarch64': 'aarch64-linux-gnu', 'armhf': 'arm-linux-gnueabihf'}[arch]
                    loader = sysroot/{'aarch64': 'lib/aarch64-linux-gnu/ld-linux-aarch64.so.1', 'armhf': 'usr/lib/arm-linux-gnueabihf/ld-linux-armhf.so.3'}[arch]
                    prefix = [emulator, str(loader), '--library-path', ':'.join(str(sysroot/path/triple) for path in ('lib', 'usr/lib'))]
                binary = output/arch
                run(arch+'-compile', [compile_recipe[0], '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-pthread',
                    '-nostdinc++', '-isystem', include, source_include, *flags, str(compiled_source), '-o', str(binary), '-fuse-ld=lld', '-nostdlib++',
                    '--unwindlib=none', *libraries, '-lpthread', '-ldl', '-lm', '-latomic'])
                assert marker in run(arch+'-run', [*prefix, str(binary)]).stdout
        passed = True
    finally:
        record['sources_unchanged'] = hashes() == record['sources']
        record['snapshot_unchanged'] = all(hashlib.sha256((snapshot/path).read_bytes()).hexdigest() == digest
                                         for path, digest in record['source_snapshot'].items())
        record['events_after'] = (GROUP/'memory.events').read_text()
        record['gate_unchanged'] = (GROUP.stat().st_ino == record['cgroup_inode'] and
            (GROUP/'memory.max').read_text().strip() == '68719476736' and (GROUP/'memory.swap.max').read_text().strip() == '0' and
            Path('/proc/sys/kernel/random/boot_id').read_text().strip() == record['boot_id'])
        record['complete'] = passed and record['sources_unchanged'] and record['snapshot_unchanged'] and record['gate_unchanged']
        save()
        if passed and not record['sources_unchanged']:
            raise RuntimeError('Source changed during qualification')
        if passed and not record['gate_unchanged']:
            raise RuntimeError('Resource gate changed during qualification')
        if passed and not record['snapshot_unchanged']:
            raise RuntimeError('Frozen source changed during qualification')


if __name__ == '__main__':
    main()
