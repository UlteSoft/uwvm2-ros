#!/usr/bin/env python3
"""Qualify CPU/NUMA launch placement, allocator rollback and actual CLI callbacks.

Uses the existing 64 GiB, zero-swap gate and immutable transitive source copies.
Native child/exec tests verify actual affinity; NUMA applied/refused outcomes are
retained without claiming cross-node latency or non-Linux runtime qualification.
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
import time

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')

def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()

def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
    guard();resource.setrlimit(resource.RLIMIT_CORE,(0,0));assert os.sched_getaffinity(0).issubset({16,17,18,19}) and len(os.sched_getaffinity(0))>=2
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False);snapshot=out/'source'
    common=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
    cli=['-DUWVM_IMPORT_ROSV2','-DUWVM_USE_THREAD_LOCAL','-DUWVM=2','-DUWVM_USE_UWVM_INT','-DUWVM_DISABLE_JIT']
    wrapped=['-fno-builtin-malloc','-fno-builtin-calloc','-fno-builtin-realloc','-Wl,--wrap=malloc','-Wl,--wrap=calloc','-Wl,--wrap=realloc']
    units=[('execution_placement',[],'PASS execution placement:'),('execution_placement_allocation',wrapped,'PASS placement allocation rollback:'),('model_cli',cli,'PASS actual worker CLI callbacks')]
    variants=[('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc','/usr/bin/g++-13',[])]
    def includes(root):return ['-I'+str(root/'src'),*['-I'+str(root/'third-parties'/name/'include') for name in ('fast_io','bizwen','boost_unordered')]]
    report=dict(complete=False,scope=__doc__,commands=[],sources={},snapshots={},binaries={},compilers={},source_root=str(snapshot),
                cgroup=str(GROUP),inode=GROUP.stat().st_ino,boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
                memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text(),
                cross_numa_performance_qualified=False,windows_macos_qualified=False,vm_carrier_qualified=False)
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=300,
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        report['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-start));save();print(label,result.returncode,flush=True)
        if result.returncode:raise RuntimeError(result.stderr[-8000:])
        return result
    success=False
    try:
        dependencies={Path(__file__).resolve()}
        for name,compiler,extra in variants:
            report['compilers'][compiler]=sha(compiler)
            for unit,flags,_ in units:
                source=ROOT/'test/0084.ros-runtime'/(unit+'.cpp')
                result=run(name+'-'+unit+'-dependencies',[compiler,*common,*extra,*[f for f in flags if not f.startswith('-Wl,')],*includes(ROOT),'-MM','-MT','fixture',str(source)])
                entries=shlex.split(result.stdout.replace('\\\n',' '));assert entries[0]=='fixture:'
                for text in entries[1:]:
                    path=Path(text);path=(ROOT/path).resolve() if not path.is_absolute() else path.resolve()
                    assert path.is_relative_to(ROOT),('External non-system dependency',path);dependencies.add(path)
        for path in sorted(dependencies):
            name=path.relative_to(ROOT);target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True);digest=sha(path);shutil.copyfile(path,target);assert sha(target)==digest
            report['sources'][str(path)]=digest;report['snapshots'][str(name)]=digest
        report['source_files']=len(dependencies);save()
        for name,compiler,extra in variants:
            for unit,flags,marker in units:
                binary=out/(name+'-'+unit);source=snapshot/'test/0084.ros-runtime'/(unit+'.cpp')
                run(name+'-'+unit+'-compile',[compiler,*common,*extra,*flags,*includes(snapshot),str(source),'-o',str(binary)])
                report['binaries'][str(binary)]=sha(binary);result=run(name+'-'+unit+'-run',[str(binary)]);assert marker in result.stdout
                if unit=='execution_placement':report.setdefault('numa_observations',{})[name]=[line for line in result.stdout.splitlines() if line.startswith('NUMA ')]
        success=True
    finally:
        report['sources_unchanged']=all(Path(path).is_file() and sha(path)==digest for path,digest in report['sources'].items())
        report['snapshots_unchanged']=all(sha(snapshot/name)==digest for name,digest in report['snapshots'].items())
        report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['sources_unchanged'] and report['snapshots_unchanged'];save()
        if success:assert report['complete'],'Sources changed during qualification'

if __name__=='__main__':main()
