#!/usr/bin/env python3
"""Qualify strict transform JSON encoding and independent C++ canonical parsing.

Retain the exact transitive source closure. All compilers and test subprocesses
remain in the existing 64 GiB zero-swap gate and use -O3 -g. Positive fixtures
are decoded, frozen and re-encoded exactly by C++; negative authentic-digest
fixtures must fail parsing or topology freeze. No ROS provider is activated.
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
    args=parser.parse_args();guard()
    if args.repository_root:ROOT=args.repository_root.resolve()
    source_root=args.source_root.resolve() if args.source_root else ROOT
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    snapshot=out/'source';source=Path('test/0084.ros-runtime/transform_definition.cpp')
    variants=[('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),('gcc13','/usr/bin/g++-13',[])]
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
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
    success=False
    try:
        dependencies={source,Path('tools/ros_scene/transform_definition.py'),Path('tools/ros_scene/transform_definition_tool_checks.py')}
        for name,compiler,extra in variants:
            record['toolchain'][compiler]=sha(compiler)
            result=run(name+'-dependencies',[compiler,*flags,*extra,'-I'+str(source_root/'src'),'-I'+str(source_root/'third-parties/fast_io/include'),'-MM','-MT','publication',str(source_root/source)])
            parsed=shlex.split(result.stdout.replace('\\\n',' '));assert parsed[0]=='publication:'
            for item in parsed[1:]:
                path=Path(item);path=path if path.is_absolute() else source_root/path
                resolved=path.resolve();assert resolved.is_relative_to(source_root),('External non-system source dependency',str(path))
                dependencies.add(resolved.relative_to(source_root))
        for name in sorted(dependencies):
            original=source_root/name;target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True)
            digest=sha(original);shutil.copyfile(original,target);assert sha(target)==digest
            record['inputs'][str(original)]=digest;record['snapshots'][str(name)]=digest
        runner=snapshot/'tools/ros_scene/transform_definition_checks.py';runner.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(__file__,runner)
        record['inputs'][str(Path(__file__).resolve())]=sha(__file__);record['snapshots']['tools/ros_scene/transform_definition_checks.py']=sha(runner)
        record['source_files']=len(record['snapshots']);save()
        run('definition-tool',['/usr/bin/python3',str(snapshot/'tools/ros_scene/transform_definition_tool_checks.py'),'--output',str(out/'fixtures')])
        record['definition_tool']=json.loads((out/'fixtures/results.json').read_text());save()
        for name,compiler,extra in variants:
            binary=out/name
            run(name+'-compile',[compiler,*flags,*extra,'-I'+str(snapshot/'src'),'-I'+str(snapshot/'third-parties/fast_io/include'),str(snapshot/source),'-o',str(binary)])
            for index,fixture in enumerate(record['definition_tool']['fixtures']):
                assert sha(fixture['path'])==fixture['sha256']
                result=run(name+'-fixture-'+str(index),[str(binary),fixture['path'],fixture['identity'],fixture['sha256'],str(fixture['bytes']),fixture['expected']],dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))
                expected='passed exact transform definition cross-language roundtrip' if fixture['expected']=='accept' else 'passed invalid canonical transform definition refusal'
                assert expected in result.stdout
        record['definition_tool']['cpp_decode_qualified']=True
        success=True
    finally:
        record['inputs_unchanged']=all(Path(path).is_file() and sha(path)==digest for path,digest in record['inputs'].items())
        record['snapshots_unchanged']=all(sha(snapshot/name)==digest for name,digest in record['snapshots'].items())
        record['events_after']=(GROUP/'memory.events').read_text();record['complete']=success and record['inputs_unchanged'] and record['snapshots_unchanged'];save()
        if success:assert record['complete'],'Sources changed during qualification'

if __name__=='__main__':main()
