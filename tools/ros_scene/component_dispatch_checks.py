#!/usr/bin/env python3
"""Qualify actual isolated component role IPC from immutable source and ELF snapshots."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import struct
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
    units = [ROOT/path for path in ['tools/ros_scene/component_worker_bootstrap.cpp',
        'test/0084.ros-runtime/component_worker_backend.cpp','test/0084.ros-runtime/component_dispatch.cpp']]
    sources = {Path(__file__).resolve(), ROOT/'documents/runtime/ros-robot-provider-procedures.md'}
    for unit in units:
        done = subprocess.run(['/usr/bin/clang++-22', '-std=c++23', '-Isrc', '-isystem', 'third-parties/fast_io/include', '-MM', str(unit)],
                              cwd=ROOT, capture_output=True, text=True, check=True, timeout=60)
        (output/(unit.stem+'.dependencies.txt')).write_text(done.stdout)
        sources.update((ROOT/path).resolve() for path in shlex.split(done.stdout.replace('\\\n', '').split(':', 1)[1]))
    sources.update(path.resolve() for path in (ROOT/'third-parties/fast_io/include').rglob('*') if path.is_file())
    record = dict(complete=False, scope='real_linux_x86_64_isolated_component_roles', commands=[],
        cgroup=str(GROUP), cgroup_inode=GROUP.stat().st_ino, boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
        memory_max=68719476736, swap_max=0, events_before=(GROUP/'memory.events').read_text(),
        affinity=sorted(os.sched_getaffinity(0)), sources={str(path.relative_to(ROOT)): digest(path) for path in sorted(sources)},
        safety_limitations=['approved fixed component image closure', 'role-only topic proxy; injected native topic factory, not native ROS interoperability',
            'not industrial certification', 'installed launcher has known advisory; specific directory/symlink trigger excluded'],
        launcher_advisory='https://github.com/containers/bubblewrap/security/advisories/GHSA-pxhw-h44j-8pfx')
    snapshot=output/'source'
    for path in sorted(sources):
        target=snapshot/path.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(path.read_bytes())
        assert digest(target)==record['sources'][str(path.relative_to(ROOT))]
    (snapshot/'manifest.json').write_text(json.dumps(record['sources'],indent=2)+'\n')
    record['source_root']=str(snapshot)
    images=output/'images';images.mkdir()
    external=[Path(path) for path in ['/lib64/ld-linux-x86-64.so.2','/lib/x86_64-linux-gnu/libc.so.6',
        '/lib/x86_64-linux-gnu/libstdc++.so.6','/lib/x86_64-linux-gnu/libgcc_s.so.1','/lib/x86_64-linux-gnu/libm.so.6','/usr/bin/bwrap']]
    record['external']={str(path):digest(path) for path in external}
    copied=[]
    for path in external:
        target=images/path.name;target.write_bytes(path.read_bytes());target.chmod(0o500);assert digest(target)==record['external'][str(path)];copied.append(target)
    record['launcher_mode']=oct(external[-1].stat().st_mode&0o7777)
    version=subprocess.run([str(copied[-1]),'--version'],capture_output=True,text=True,check=True,timeout=10)
    record['launcher_version']=version.stdout.strip()
    weights=images/'matrix.weights';weights.write_bytes(struct.pack('<4d',1,2,3,4));weights.chmod(0o400)
    def save():
        (output/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,allowed=False):
        done=subprocess.run([str(arg) for arg in argv],cwd=ROOT,capture_output=True,text=True,timeout=300,
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        (output/(label+'.stdout')).write_text(done.stdout);(output/(label+'.stderr')).write_text(done.stderr)
        record['commands'].append(dict(label=label,argv=[str(arg) for arg in argv],exit=done.returncode));save()
        print(label+': '+str(done.returncode),flush=True)
        if done.returncode and not allowed:raise RuntimeError(done.stderr[-8000:])
        return done
    includes=['-I'+str(snapshot/'src'),'-isystem',str(snapshot/'third-parties/fast_io/include')]
    def compile(label,unit,flags,compiler='/usr/bin/clang++-22'):
        binary=images/label if label in ('bootstrap','backend.so') else output/label
        run(label+'-compile',[compiler,'-std=c++23','-Wall','-Wextra','-Werror','-pthread',*includes,*flags,snapshot/unit,'-ldl','-o',binary])
        record.setdefault('binaries',{})[str(binary)]=digest(binary);return binary
    complete=False
    try:
        bootstrap=compile('bootstrap','tools/ros_scene/component_worker_bootstrap.cpp',['-O3','-g'])
        backend=compile('backend.so','test/0084.ros-runtime/component_worker_backend.cpp',['-O3','-g','-fPIC','-shared'])
        inputs=[bootstrap,backend,weights,*copied]
        for compiler,name,flags in [('/usr/bin/clang++-22','sanitized',['-O3','-g','-fsanitize=address,undefined']),('/usr/bin/g++-13','gcc',['-O3','-g'])]:
            unit='component_dispatch'
            binary=compile(name+'-'+unit,'test/0084.ros-runtime/'+unit+'.cpp',flags,compiler)
            assert 'passed 7 real isolated component safety groups' in run(name+'-'+unit+'-run',[binary,*inputs]).stdout
        threaded=compile('threaded-component_dispatch','test/0084.ros-runtime/component_dispatch.cpp',['-O3','-g','-fsanitize=thread','-fno-omit-frame-pointer'])
        initialized=False
        for attempt in range(1,9):
            result=run('threaded-component_dispatch-run-'+str(attempt),[threaded,*inputs],True)
            if not result.returncode:
                assert 'passed 7 real isolated component safety groups' in result.stdout and 'WARNING: ThreadSanitizer:' not in result.stderr
                initialized=True;break
            if not (result.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not result.stdout):raise RuntimeError(result.stderr[-8000:])
        assert initialized,'ThreadSanitizer did not initialize in eight recorded attempts'
        complete=True
    finally:
        record['source_snapshot_unchanged']=all(digest(snapshot/name)==sha for name,sha in record['sources'].items())
        record['workspace_sources_unchanged']=all(digest(ROOT/name)==sha for name,sha in record['sources'].items())
        record['external_unchanged']=all(digest(path)==sha for path,sha in record['external'].items())
        record['events_after']=(GROUP/'memory.events').read_text()
        record['memory_limits_unchanged']=(GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
        record['complete']=complete and record['source_snapshot_unchanged'] and record['workspace_sources_unchanged'] and record['external_unchanged'] and record['memory_limits_unchanged']
        save()
    assert record['complete']


if __name__=='__main__':
    main()
