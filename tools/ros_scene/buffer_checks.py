#!/usr/bin/env python3
"""Build and execute production buffer safety tests in the existing 64 GiB gate.

Compiles only archived source snapshots; includes the real strict allocator and
fast_io SHA256 implementation. Native/cross tests do not imply guest ABI coverage.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import shlex
import shutil
import subprocess
import threading

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--recipes', type=Path)
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736' and (GROUP/'memory.swap.max').read_text().strip() == '0'
    assert os.sched_getaffinity(0).issubset({12, 13, 14, 15})
    output = args.output.resolve()
    assert output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    output.mkdir(parents=True, exist_ok=True)
    assert not (output/'results.json').exists(), 'Preserve existing receipts'
    test = ROOT/'test/0084.ros-runtime/buffers.cpp'
    dependencies = subprocess.run(['/usr/bin/clang++-22', '-std=c++23', '-Isrc', '-isystem',
        'third-parties/fast_io/include', '-MM', str(test)], cwd=ROOT, capture_output=True, text=True, timeout=60, check=True)
    (output/'dependencies.txt').write_text(dependencies.stdout)
    local = {(ROOT/path).resolve() for path in shlex.split(dependencies.stdout.replace('\\\n', '').split(':', 1)[1])}
    local.add(Path(__file__).resolve())
    # Different cross targets select different platform/hash headers. Archive
    # the complete bundled fast_io include tree instead of only native branches.
    local.update(path.resolve() for path in (ROOT/'third-parties/fast_io/include').rglob('*') if path.is_file())
    sources = sorted(local)
    record = dict(complete=False, scope='production_buffer_core', commands=[], cgroup=str(GROUP), cgroup_inode=GROUP.stat().st_ino,
                  boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(), memory_max=68719476736, swap_max=0,
                  affinity=sorted(os.sched_getaffinity(0)), events_before=(GROUP/'memory.events').read_text(),
                  sources={str(path.relative_to(ROOT)): digest(path) for path in sources})
    snapshot = output/'source';assert not snapshot.exists();snapshot.mkdir()
    for path in sources:
        target = snapshot/path.relative_to(ROOT)
        target.parent.mkdir(parents=True, exist_ok=True);target.write_bytes(path.read_bytes())
        assert digest(target) == record['sources'][str(path.relative_to(ROOT))], 'Source changed while freezing'
    record['source_root'] = str(snapshot)
    (snapshot/'manifest.json').write_text(json.dumps(record['sources'], indent=2)+'\n')
    test = snapshot/test.relative_to(ROOT)
    includes = ['-I'+str(snapshot/'src'), '-isystem', str(snapshot/'third-parties/fast_io/include')]
    if args.recipes:
        record['recipes'] = {str((args.recipes/arch/name).resolve()): digest(args.recipes/arch/name)
                             for arch in ('aarch64', 'armhf', 'i686') for name in ('host_api.default.cpp.o.argv.json', 'link.argv.json')}
    def save():
        (output/'results.json').write_text(json.dumps(record, indent=2)+'\n')
    def run(label, argv, allow_failure=False):
        environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1',
                           TSAN_OPTIONS='halt_on_error=1:allocator_may_return_null=1')
        fault_gate = None
        if '-compile' in label:
            done = subprocess.run(argv, cwd=ROOT, capture_output=True, text=True, timeout=300, env=environment)
        else:
            ready_read, ready_write = os.pipe();continue_read, continue_write = os.pipe()
            environment.update(UWVM_BUFFER_FAULT_READY_FD=str(ready_write), UWVM_BUFFER_FAULT_CONTINUE_FD=str(continue_read))
            process = subprocess.Popen(argv, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
                env=environment, pass_fds=(ready_write, continue_read))
            os.close(ready_write);os.close(continue_read)
            fault_gate = dict(complete=False,
                mechanism='host_child_RLIMIT_DATA_plus_test_malloc_injection' if label == 'armhf-run' else 'host_child_RLIMIT_DATA',
                cgroup_unchanged=True)
            def lower_child_data_limit():
                try:
                    identity = b''
                    while len(identity)<8:
                        chunk = os.read(ready_read, 8-len(identity))
                        if not chunk:
                            raise RuntimeError('Test exited before allocator fault rendezvous')
                        identity += chunk
                    pid = int.from_bytes(identity, 'little')
                    status = dict(line.split(':', 1) for line in Path('/proc', str(pid), 'status').read_text().splitlines())
                    assert int(status['PPid'].strip()) == process.pid, 'Fault target is not the test child'
                    assert Path('/proc', str(pid), 'cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
                    data_bytes = int(status['VmData'].split()[0])*1024
                    previous = resource.prlimit(pid, resource.RLIMIT_DATA)
                    limit = data_bytes+8*1024*1024
                    for value in previous:
                        if value != resource.RLIM_INFINITY:
                            limit = min(limit, value)
                    resource.prlimit(pid, resource.RLIMIT_DATA, (limit, limit))
                    fault_gate.update(complete=True, pid=pid, previous=list(previous), vm_data_bytes=data_bytes, limit_bytes=limit)
                    os.write(continue_write, b'\x01')
                except Exception as exc:
                    fault_gate['error'] = str(exc)
                    try:
                        os.write(continue_write, b'\x00')
                    except BrokenPipeError:
                        pass
                finally:
                    os.close(ready_read);os.close(continue_write)
            worker = threading.Thread(target=lower_child_data_limit, daemon=True);worker.start()
            stdout, stderr = process.communicate(timeout=300);worker.join(timeout=10)
            assert not worker.is_alive(), 'Allocator fault supervisor did not finish'
            done = subprocess.CompletedProcess(argv, process.returncode, stdout, stderr)
            if done.returncode == 0:
                assert fault_gate['complete'], fault_gate
        (output/(label+'.stdout')).write_text(done.stdout);(output/(label+'.stderr')).write_text(done.stderr)
        record['commands'].append(dict(label=label, argv=argv, exit=done.returncode, allocation_fault=fault_gate));save()
        print(label+': '+str(done.returncode), flush=True)
        if done.returncode and not allow_failure:
            raise RuntimeError(done.stderr[-6000:])
        return done
    marker = 'passed 9 production buffer safety groups';passed = False
    try:
        for name, compiler, flags in [('sanitized', '/usr/bin/clang++-22', ['-fsanitize=address,undefined']),
                                     ('gcc', '/usr/bin/g++-13', []), ('thread', '/usr/bin/clang++-22', ['-fsanitize=thread'])]:
            binary = output/name
            run(name+'-compile', [compiler, '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-pthread',
                '-fno-omit-frame-pointer', *includes, *flags, str(test), '-o', str(binary)])
            initialized = False
            for attempt in range(1, 9 if name == 'thread' else 2):
                done = run(name+'-run-'+str(attempt), [str(binary)], name == 'thread')
                if not done.returncode:
                    assert marker in done.stdout and 'WARNING: ThreadSanitizer:' not in done.stderr
                    initialized = True;break
                if not (done.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not done.stdout):
                    raise RuntimeError(done.stderr[-6000:])
            assert initialized, 'ThreadSanitizer failed to initialize'
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
                    emulator = shutil.which({'aarch64': 'qemu-aarch64', 'armhf': 'qemu-arm'}[arch]);assert emulator
                    triple = {'aarch64': 'aarch64-linux-gnu', 'armhf': 'arm-linux-gnueabihf'}[arch]
                    loader = sysroot/{'aarch64': 'lib/aarch64-linux-gnu/ld-linux-aarch64.so.1', 'armhf': 'usr/lib/arm-linux-gnueabihf/ld-linux-armhf.so.3'}[arch]
                    prefix = [emulator, str(loader), '--library-path', ':'.join(str(sysroot/path/triple) for path in ('lib', 'usr/lib'))]
                binary = output/arch
                allocation_flags = ['-DUWVM_BUFFER_FAULT_INJECTION', '-fno-builtin-malloc', '-Wl,--wrap=malloc'] if arch == 'armhf' else []
                run(arch+'-compile', [compile_recipe[0], '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror', '-pthread',
                    '-nostdinc++', '-isystem', include, *includes, *flags, *allocation_flags, str(test), '-o', str(binary), '-fuse-ld=lld', '-nostdlib++',
                    '--unwindlib=none', *libraries, '-lpthread', '-ldl', '-lm', '-latomic'])
                assert marker in run(arch+'-run', [*prefix, str(binary)]).stdout
        passed = True
    finally:
        record['sources_unchanged'] = all(digest(ROOT/path) == value for path, value in record['sources'].items())
        record['snapshot_unchanged'] = all(digest(snapshot/path) == value for path, value in record['sources'].items())
        record['events_after'] = (GROUP/'memory.events').read_text()
        record['gate_unchanged'] = (GROUP.stat().st_ino == record['cgroup_inode'] and
            (GROUP/'memory.max').read_text().strip() == '68719476736' and (GROUP/'memory.swap.max').read_text().strip() == '0' and
            Path('/proc/sys/kernel/random/boot_id').read_text().strip() == record['boot_id'])
        record['complete'] = passed and record['sources_unchanged'] and record['snapshot_unchanged'] and record['gate_unchanged']
        save()
        if passed and not record['complete']:
            raise RuntimeError('Source, snapshot or resource gate changed during qualification')


if __name__ == '__main__':
    main()
