#!/usr/bin/env python3
"""Pin and qualify the independent communication sandbox with a real threaded backend."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import subprocess

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip() == '68719476736' and (GROUP/'memory.swap.max').read_text().strip() == '0'
    assert os.sched_getaffinity(0).issubset({12, 13, 14, 15})
    output = args.output.resolve()
    assert output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    output.mkdir(parents=True, exist_ok=True)
    assert not (output/'results.json').exists() and not (output/'source').exists()
    units = ['tools/ros_scene/communication_worker_bootstrap.cpp'] + [
        'test/0084.ros-runtime/communication_worker_' + name + '.cpp'
        for name in ('backend', 'syscalls', 'sandbox', 'allocation')]
    owned = units + ['tools/ros_scene/communication_worker_checks.py',
        'documents/runtime/ros-communication-worker.md'] + [
        'src/uwvm2/imported/ros/scene/communication_worker_' + name + '.h'
        for name in ('backend', 'bootstrap_protocol', 'syscalls', 'sandbox')]
    sources = {ROOT/name for name in owned}
    for name in units:
        done = subprocess.run(['/usr/bin/clang++-22', '-std=c++23', '-Isrc', '-isystem',
            'third-parties/fast_io/include', '-MM', name], cwd=ROOT, capture_output=True,
            text=True, check=True, timeout=60)
        (output/(Path(name).stem+'.dependencies.txt')).write_text(done.stdout)
        sources.update((ROOT/path).resolve() for path in shlex.split(done.stdout.replace('\\\n', '').split(':', 1)[1]))
    sources.update(path.resolve() for path in (ROOT/'third-parties/fast_io/include').rglob('*') if path.is_file())
    record = dict(complete=False, scope='actual_linux_x86_64_threaded_communication_sandbox_fixture',
        commands=[], cgroup=str(GROUP), cgroup_inode=GROUP.stat().st_ino,
        boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(), memory_max=68719476736,
        swap_max=0, events_before=(GROUP/'memory.events').read_text(), affinity=sorted(os.sched_getaffinity(0)),
        compiler_flags=['-O3', '-g'], sources={str(path.relative_to(ROOT)): digest(path) for path in sorted(sources)},
        limitations=['fixture backend, not a ROS/FastDDS closure qualification',
            'trusted startup thread count is verified after TSYNC, not a hard peak thread quota',
            'supervisor must enforce stderr budget, RPC deadlines and cleanup debt',
            'no direct sockets, native device access, GPU, JIT or dynamic plugin discovery',
            'not industrial certification'])
    snapshot = output/'source'
    for path in sorted(sources):
        target = snapshot/path.relative_to(ROOT)
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(path.read_bytes())
        assert digest(target) == record['sources'][str(path.relative_to(ROOT))]
    (snapshot/'manifest.json').write_text(json.dumps(record['sources'], indent=2)+'\n')
    record['source_root'] = str(snapshot)
    images = output/'images'
    images.mkdir()
    external = [Path(path) for path in ['/lib64/ld-linux-x86-64.so.2', '/lib/x86_64-linux-gnu/libc.so.6',
        '/lib/x86_64-linux-gnu/libstdc++.so.6', '/lib/x86_64-linux-gnu/libgcc_s.so.1',
        '/lib/x86_64-linux-gnu/libm.so.6', '/usr/bin/bwrap']]
    record['external'] = {str(path): digest(path) for path in external}
    copied = []
    for path in external:
        target = images/path.name
        target.write_bytes(path.read_bytes())
        target.chmod(0o500)
        assert digest(target) == record['external'][str(path)]
        copied.append(target)
    record['launcher_version'] = subprocess.run([str(copied[-1]), '--version'], capture_output=True,
        text=True, check=True, timeout=10).stdout.strip()
    def save():
        (output/'results.json').write_text(json.dumps(record, indent=2)+'\n')
    def run(label, argv):
        done = subprocess.run([str(arg) for arg in argv], cwd=ROOT, capture_output=True, text=True, timeout=300,
            env=dict(os.environ, ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        (output/(label+'.stdout')).write_text(done.stdout)
        (output/(label+'.stderr')).write_text(done.stderr)
        record['commands'].append(dict(label=label, argv=[str(arg) for arg in argv], exit=done.returncode))
        save()
        print(label+': '+str(done.returncode), flush=True)
        if done.returncode:
            raise RuntimeError(done.stderr[-10000:])
        return done
    includes = ['-I'+str(snapshot/'src'), '-isystem', str(snapshot/'third-parties/fast_io/include')]
    def compile(label, unit, flags=(), compiler='/usr/bin/clang++-22'):
        binary = images/label if label in ('bootstrap', 'backend.so') else output/label
        run(label+'-compile', [compiler, '-std=c++23', '-O3', '-g', '-Wall', '-Wextra', '-Werror',
            '-pthread', *includes, *flags, snapshot/unit, '-ldl', '-o', binary])
        record.setdefault('binaries', {})[str(binary)] = digest(binary)
        return binary
    complete = False
    try:
        bootstrap = compile('bootstrap', units[0])
        backend = compile('backend.so', units[1], ['-fPIC', '-shared'])
        inputs = [bootstrap, backend, *copied]
        for name, compiler, flags in [('sanitized', '/usr/bin/clang++-22', ['-fsanitize=address,undefined']),
                                     ('gcc', '/usr/bin/g++-13', [])]:
            for fixture, marker in [('syscalls', 'PASS communication BPF'), ('sandbox', 'PASS sealed communication process')]:
                binary = compile(name+'-'+fixture, 'test/0084.ros-runtime/communication_worker_'+fixture+'.cpp', flags, compiler)
                assert marker in run(name+'-'+fixture+'-run', [binary, *inputs]).stdout
        allocation = compile('sanitized-allocation', units[-1], ['-fsanitize=address,undefined',
            '-fno-builtin-malloc', '-fno-builtin-calloc', '-fno-builtin-realloc',
            '-Wl,--wrap=malloc', '-Wl,--wrap=calloc', '-Wl,--wrap=realloc'])
        assert 'PASS recoverable communication allocation rollback' in run('sanitized-allocation-run', [allocation, *inputs]).stdout
        complete = True
    finally:
        record['source_snapshot_unchanged'] = all(digest(snapshot/name)==sha for name, sha in record['sources'].items())
        record['workspace_sources_unchanged'] = all(digest(ROOT/name)==sha for name, sha in record['sources'].items())
        record['owned_sources_unchanged'] = all(digest(ROOT/name)==record['sources'][name] for name in owned)
        record['external_unchanged'] = all(digest(path)==sha for path, sha in record['external'].items())
        record['events_after'] = (GROUP/'memory.events').read_text()
        record['memory_limits_unchanged'] = (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
        record['complete'] = complete and record['source_snapshot_unchanged'] and record['owned_sources_unchanged'] and record['external_unchanged'] and record['memory_limits_unchanged']
        save()
    assert record['complete']


if __name__ == '__main__':
    main()
