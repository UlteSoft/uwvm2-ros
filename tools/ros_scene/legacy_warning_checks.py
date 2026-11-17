#!/usr/bin/env python3
"""Qualify deprecated ROS enablement and unisolated native preload warning gates.

Calls production CLI callbacks and the real native loader. Two distinct benign
DSOs record constructor execution, including warning promotion after a previous
load. This is not a native-provider isolation or ROSv2 migration-coverage claim.
"""
import argparse
import hashlib
import itertools
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
    guard();resource.setrlimit(resource.RLIMIT_CORE,(0,0));assert os.sched_getaffinity(0).issubset({16,17,18,19})
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    snapshot=out/'source';common=['-O3','-g','-Wall','-Wextra','-Werror','-pthread']
    features=['-DUWVM_IMPORT_ROS1','-DUWVM_IMPORT_ROS2','-DUWVM_ENABLE_PRELOAD_DL=1','-DUWVM_USE_THREAD_LOCAL','-DUWVM=2','-DUWVM_USE_UWVM_INT','-DUWVM_DISABLE_JIT']
    units=['legacy_warning','preload_warning','preload_warning_library']
    variants=[('clang-sanitized','/usr/bin/clang++-22',['-std=c++26','-fsanitize=address,undefined','-fno-omit-frame-pointer'],units),
              ('clang','/usr/bin/clang++-22',['-std=c++26'],units),
              ('gcc','/usr/bin/g++-13',['-std=c++23'],['legacy_warning'])]
    def includes(root):return ['-I'+str(root/'src'),*['-I'+str(root/'third-parties'/name/'include') for name in ('fast_io','bizwen','boost_unordered')]]
    report=dict(complete=False,scope=__doc__,commands=[],cases=[],sources={},snapshots={},binaries={},compilers={},source_root=str(snapshot),
        cgroup=str(GROUP),inode=GROUP.stat().st_ino,boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
        memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv,expected=0):
        guard();start=time.monotonic();result=subprocess.run(list(map(str,argv)),cwd=ROOT,capture_output=True,text=True,timeout=300,
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        report['commands'].append(dict(label=label,argv=list(map(str,argv)),exit=result.returncode,expected=expected,seconds=time.monotonic()-start));save()
        if result.returncode!=expected:raise RuntimeError((label,result.returncode,result.stderr[-8000:]))
        return result
    success=False
    try:
        dependencies={Path(__file__).resolve()}
        for name,compiler,extra,selected in variants:
            report['compilers'][compiler]=sha(compiler)
            for unit in selected:
                source=ROOT/'test/0084.ros-runtime'/(unit+'.cpp')
                result=run(name+'-'+unit+'-dependencies',[compiler,*common,*extra,*features,'-DSCENE_WARNING_LIBRARY_ID="A"',*includes(ROOT),'-MM','-MT','fixture',source])
                entries=shlex.split(result.stdout.replace('\\\n',' '));assert entries[0]=='fixture:'
                for text in entries[1:]:
                    path=Path(text);path=(ROOT/path).resolve() if not path.is_absolute() else path.resolve()
                    assert path.is_relative_to(ROOT),('External non-system dependency',path);dependencies.add(path)
        for path in sorted(dependencies):
            name=path.relative_to(ROOT);target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True);digest=sha(path)
            shutil.copyfile(path,target);assert sha(target)==digest;report['sources'][str(path)]=digest;report['snapshots'][str(name)]=digest
        report['source_files']=len(dependencies);save()
        for name,compiler,extra,selected in variants:
            for unit in [unit for unit in selected if unit!='preload_warning_library']:
                binary=out/(name+'-'+unit);source=snapshot/'test/0084.ros-runtime'/(unit+'.cpp')
                run(name+'-'+unit+'-compile',[compiler,*common,*extra,*features,*includes(snapshot),source,'-ldl','-o',binary]);report['binaries'][str(binary)]=sha(binary)
            libraries=[]
            for identity in (('A','B') if 'preload_warning' in selected else ()):
                library=out/(name+'-'+identity+'.so');libraries.append(library)
                run(name+'-'+identity+'-compile',[compiler,*common,*extra,'-fPIC','-shared','-DSCENE_WARNING_LIBRARY_ID="'+identity+'"',
                    snapshot/'test/0084.ros-runtime/preload_warning_library.cpp','-o',library]);report['binaries'][str(library)]=sha(library)
            modes=('default','suppressed','reenabled','fatal','suppressed-fatal','reenabled-fatal','all-reenabled','all-fatal')
            for version,scope,mode in itertools.product(('ros1','ros2'),('global','single','group'),modes):
                label='-'.join((name,version,scope,mode));result=run(label,[out/(name+'-legacy_warning'),version,scope,mode])
                warnings=0 if mode in ('suppressed','suppressed-fatal') else 1
                assert result.stderr.count('(deprecated-ros)')==warnings,(label,result.stderr)
                assert 'PASS legacy enable warning and mutation boundary' in result.stdout
                report['cases'].append(dict(name=label,passed=True,diagnostic_count=warnings));save()
            preload_modes=('default','suppressed','reenabled','fatal','suppressed-fatal','second-fatal','second-reenabled-fatal','all-fatal')
            for mode in (preload_modes if 'preload_warning' in selected else ()):
                label=name+'-preload-'+mode;fatal='fatal' in mode and mode!='suppressed-fatal'
                result=run(label,[out/(name+'-preload_warning'),mode,*libraries],126 if fatal else 0)
                expected_a=not fatal or mode.startswith('second-');expected_b=not fatal
                assert result.stdout.count('CONSTRUCTOR_A\n')==int(expected_a),(label,result.stdout)
                assert result.stdout.count('CONSTRUCTOR_B\n')==int(expected_b),(label,result.stdout)
                diagnostic_count=0 if mode in ('suppressed','suppressed-fatal') else 2 if fatal else 1
                assert result.stderr.count('(untrusted-dl)')==diagnostic_count,(label,result.stderr)
                if not fatal:assert 'PASS real preloaded native constructors' in result.stdout
                report['cases'].append(dict(name=label,passed=True,first_constructor=expected_a,second_constructor=expected_b,diagnostic_count=diagnostic_count));save()
            print('PASS '+name+' 48 legacy configurations'+(' and 8 real native constructor gates' if libraries else ''),flush=True)
        success=True
    finally:
        report['sources_unchanged']=all(Path(path).is_file() and sha(path)==digest for path,digest in report['sources'].items())
        report['snapshots_unchanged']=all(sha(snapshot/name)==digest for name,digest in report['snapshots'].items())
        report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['sources_unchanged'] and report['snapshots_unchanged'];save()
        if success:assert report['complete'],'Sources changed during qualification'
if __name__=='__main__':main()
