#!/usr/bin/env python3
"""Run actual scene wrappers with hostile allocation hooks in the 64 GiB gate.

Uses the real production C++ runtime recipe. Standard allocator failures are
induced only by reducing RLIMIT_AS in the child, never by raising cgroup limits.
The separate injection build targets exact first-enrollment/scheduling sites.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compile-argv',type=Path,required=True)
    parser.add_argument('--link-argv',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset({0,1,2,3})
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    source=ROOT/'test/0084.ros-runtime/wrapper_allocator_fp.cpp'
    raw=json.loads(args.compile_argv.read_text());compile_flags=[];skip=False
    for flag in raw:
        if skip:skip=False;continue
        if flag=='-o':skip=True;continue
        if flag in ('-c','-DNDEBUG','-fno-exceptions') or flag.endswith(('.cpp','.cc','.o')) or flag.startswith('-fprebuilt-module-path='):continue
        compile_flags.append(flag)
    compile_flags[1:1]=['-I'+str(ROOT/'src')]
    compile_flags+=['-UNDEBUG','-O1','-g','-fexceptions']
    raw_link=json.loads(args.link_argv.read_text());link_flags=[raw_link[0]]
    link_flags += ['-fuse-ld=lld','-nostdlib++','--unwindlib=none']
    link_flags += list(dict.fromkeys(flag for flag in raw_link if flag.endswith('.a') and '/bundled-runtimes/' in flag))
    link_flags += ['-lpthread','-ldl','-lm','-latomic','-lcrypto']
    inputs=[Path(__file__),source,args.compile_argv.resolve(),args.link_argv.resolve(),ROOT/'src/uwvm2/uwvm/imported/scene/host.h',
            ROOT/'src/uwvm2/runtime/lib/uwvm_runtime_wasm_fp_native_control.h',*ROOT.glob('src/uwvm2/imported/ros/scene/*.h')]
    def hashes():return {str(path):hashlib.sha256(path.read_bytes()).hexdigest() for path in inputs}
    record=dict(complete=False,inputs=hashes(),commands=[],cgroup=str(GROUP),memory_max=68719476736,swap_max=0,
                affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv):
        result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=240)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=result.returncode));save()
        print(label+': '+str(result.returncode),flush=True)
        if result.returncode:raise RuntimeError(result.stderr[-6000:])
    passed=False
    try:
        for name,extra in [('standard',[]),('injected',['-DUWVM_SCENE_ALLOCATOR_INJECTION=1'])]:
            obj=out/(name+'.o');binary=out/name
            run(name+'-compile',[*compile_flags,*extra,'-c',str(source),'-o',str(obj)])
            run(name+'-link',[link_flags[0],str(obj),*link_flags[1:],'-o',str(binary)])
            run(name,[str(binary)])
        passed=True
    finally:
        record['inputs_unchanged']=hashes()==record['inputs'];record['events_after']=(GROUP/'memory.events').read_text()
        record['complete']=passed and record['inputs_unchanged'];save()
        if passed and not record['inputs_unchanged']:raise RuntimeError('Source changed during qualification')


if __name__=='__main__':main()
