#!/usr/bin/env python3
"""Check borrowed callback allocation/lifetimes in the existing resource gate.

Use the production VM's saved compiler and libc++ paths for one ASan/UBSan run,
the compiler's default library for another, GCC for portability, and TSan for
blocking wait lifetimes. TSan owns operator new, so its build intentionally
omits fault injection; both ASan builds and GCC check allocation refusal.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def sha(path):return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736'
    assert (GATE/'memory.swap.max').read_text().strip()=='0'


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime-directory',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--container',default='uwvm-sdk-resource-gate-20261024')
    args=parser.parse_args();guard()
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    runtime=args.runtime_directory.resolve()
    compile_argv=json.loads((runtime/'main.default.cpp.o.argv.json').read_text())
    link_argv=json.loads((runtime/'link.argv.json').read_text())
    compiler=compile_argv[0]
    headers=[x for x in compile_argv if x.endswith('/include/c++/v1')];assert len(headers)==1
    libraries=list(dict.fromkeys(x for x in link_argv if Path(x).name in ['libc++.a','libc++abi.a','libunwind.a']))
    libraries.sort(key=lambda x:['libc++.a','libc++abi.a','libunwind.a'].index(Path(x).name));assert len(libraries)==3
    paths=['test/0084.ros-runtime/borrowed_sink.cc','test/0084.ros-runtime/run_borrowed_sink.py',
           'src/uwvm2/imported/ros/scene/broker.h','src/uwvm2/imported/ros/scene/authority.h',
           'src/uwvm2/imported/ros/runtime/resources.h','src/uwvm2/imported/ros/runtime/contract.h',
           'src/uwvm2/uwvm/imported/scene/host.h']
    def hashes():return {name:sha(ROOT/name) for name in paths}
    record=dict(complete=False,cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,
                events_before=(GATE/'memory.events').read_text(),sources=hashes(),
                production_libraries={name:sha(name) for name in libraries},runs=[])
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(name,command,data=None):
        guard();start=time.monotonic()
        result=subprocess.run(command,cwd=ROOT,input=data,capture_output=True,timeout=180,
                              env=dict(os.environ,ASAN_OPTIONS='detect_stack_use_after_return=1'))
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        record['runs'].append(dict(name=name,argv=command,exit=result.returncode,seconds=time.monotonic()-start));save()
        assert result.returncode==0,(name,result.stderr.decode(errors='replace')[-4000:]);print('PASS',name,flush=True)
    for label in ['asan-default','asan-production','tsan-lifetimes']:
        binary=out/label;thread=label.startswith('tsan')
        command=[compiler,'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc',
                 '-fsanitize='+('thread' if thread else 'address,undefined'),'-fno-omit-frame-pointer']
        if thread:command+=['-DUWVM_TEST_CALLBACK_LIFETIMES_ONLY']
        else:command+=['-fsanitize-address-use-after-scope']
        if label=='asan-production':command+=['-nostdinc++','-isystem',headers[0],'-nostdlib++']
        command+=[str(ROOT/paths[0]),'-o',str(binary)]
        if label=='asan-production':command+=[*libraries,'-ldl','-latomic']
        run('compile-'+label,command);run('run-'+label,[str(binary)])
    script='''import pathlib,sys,json,subprocess,os
assert pathlib.Path('/sys/fs/cgroup/memory.max').read_text().strip()=='68719476736'
assert pathlib.Path('/sys/fs/cgroup/memory.swap.max').read_text().strip()=='0'
root=pathlib.Path('/dev/uwvm-scene-borrowed');root.mkdir(exist_ok=True)
for name,text in json.load(sys.stdin).items():
 p=root/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(text)
print(subprocess.check_output(['g++','--version'],text=True),flush=True)
subprocess.run(['g++','-std=c++23','-O2','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(root/'src'),str(root/'test/0084.ros-runtime/borrowed_sink.cc'),'-o',str(root/'borrowed-sink')],check=True,env=dict(os.environ,TMPDIR='/dev/shm'))
subprocess.run([str(root/'borrowed-sink')],check=True)
'''
    script='import os;os.sched_setaffinity(0,'+repr(sorted(os.sched_getaffinity(0)))+')\n'+script
    inputs=[name for name in paths if name.endswith(('.cc','.h')) and '/uwvm/imported/' not in name]
    run('gcc',['docker','exec','-i',args.container,'python3','-c',script],json.dumps({name:(ROOT/name).read_text() for name in inputs}).encode())
    record['sources_after']=hashes();assert record['sources_after']==record['sources']
    assert all(sha(name)==value for name,value in record['production_libraries'].items())
    guard();record['events_after']=(GATE/'memory.events').read_text();record['complete']=True;save()


if __name__=='__main__':main()
