#!/usr/bin/env python3
"""Qualify private recording/replay with real sealed-buffer authority and failure recovery.

Compile every native variant from retained source snapshots. The transitive
repository/vendor closure is taken from each compiler's dependency output;
concurrent workspace changes invalidate the receipt, never silently qualify a
mixed build. This integration suite exercises transactional failure and authority composition.
This core receipt does not replace real-VM carrier/debug/performance checks.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'

def main():
    global ROOT
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--repository-root',type=Path)
    parser.add_argument('--source-root',type=Path,help='Optional retained source/ tree for exact historical replay')
    parser.add_argument('--thread',action='store_true')
    args=parser.parse_args();guard()
    if args.repository_root:ROOT=args.repository_root.resolve()
    source_root=args.source_root.resolve() if args.source_root else ROOT
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    snapshot=out/'source';source=Path('test/0084.ros-runtime/recording_dispatch.cpp')
    variants=[('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc13','/usr/bin/g++-13',[])]
    if args.thread:variants.append(('thread-sanitized','/usr/bin/clang++-22',['-fsanitize=thread','-fno-omit-frame-pointer']))
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc']
    record=dict(complete=False,scope=__doc__,cgroup=str(GROUP),inode=GROUP.stat().st_ino,boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                memory_max=68719476736,swap_max=0,cpu_affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text(),
                source_root=str(source_root),snapshot_root=str(snapshot),commands=[],inputs={},snapshots={},toolchain={},vm_carrier_qualified=False)
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,env=None,allowed=False):
        guard();start=time.monotonic();result=subprocess.run(argv,cwd=source_root,capture_output=True,text=True,env=env,timeout=240)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-start));save()
        print(label+': '+str(result.returncode),flush=True)
        if result.returncode and not allowed:raise RuntimeError(label+': '+result.stderr[-7000:])
        return result
    def passed(result):assert 'passed 8 recording dispatch security groups' in result.stdout
    success=False
    try:
        dependencies={source}
        for name,compiler,extra in variants:
            record['toolchain'][compiler]=sha(compiler)
            result=run(name+'-dependencies',[compiler,*[flag for flag in flags if not flag.startswith('-Wl,')],*extra,'-I'+str(source_root/'src'),'-I'+str(source_root/'third-parties/fast_io/include'),'-MM','-MT','publication',str(source_root/source)])
            parsed=shlex.split(result.stdout.replace('\\\n',' '));assert parsed[0]=='publication:'
            for item in parsed[1:]:
                path=Path(item);path=path if path.is_absolute() else source_root/path
                resolved=path.resolve();assert resolved.is_relative_to(source_root),('External non-system source dependency',str(path))
                dependencies.add(resolved.relative_to(source_root))
        for name in sorted(dependencies):
            original=source_root/name;target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True)
            digest=sha(original);shutil.copyfile(original,target);assert sha(target)==digest
            record['inputs'][str(original)]=digest;record['snapshots'][str(name)]=digest
        runner=snapshot/'tools/ros_scene/recording_dispatch_checks.py';runner.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(__file__,runner)
        record['inputs'][str(Path(__file__).resolve())]=sha(__file__);record['snapshots']['tools/ros_scene/recording_dispatch_checks.py']=sha(runner)
        record['source_files']=len(record['snapshots']);save()
        for name,compiler,extra in variants:
            binary=out/name
            run(name+'-compile',[compiler,*flags,*extra,'-I'+str(snapshot/'src'),'-I'+str(snapshot/'third-parties/fast_io/include'),str(snapshot/source),
                '-o',str(binary)])
            if name=='thread-sanitized':
                initialized=False
                for attempt in range(1,9):
                    result=run(name+'-'+str(attempt),[str(binary)],dict(os.environ,TSAN_OPTIONS='halt_on_error=1'),True)
                    if not result.returncode:passed(result);assert 'WARNING: ThreadSanitizer:' not in result.stderr;initialized=True;break
                    if not (result.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not result.stdout):raise RuntimeError(result.stderr[-7000:])
                assert initialized,'TSan did not initialize in eight recorded attempts'
            else:passed(run(name,[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1')))
        success=True
    finally:
        record['inputs_unchanged']=all(Path(path).is_file() and sha(path)==digest for path,digest in record['inputs'].items())
        record['snapshots_unchanged']=all(sha(snapshot/name)==digest for name,digest in record['snapshots'].items())
        record['events_after']=(GROUP/'memory.events').read_text();record['complete']=success and record['inputs_unchanged'] and record['snapshots_unchanged'];save()
        if success:assert record['complete'],'Sources changed during qualification'

if __name__=='__main__':main()
