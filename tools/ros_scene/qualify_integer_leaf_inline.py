#!/usr/bin/env python3
"""Run real LLVM rejection and native semantic controls inside the fixed gate."""
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
def sha(p):
    with Path(p).open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
def gate():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736'
    assert (GROUP/'memory.swap.max').read_text().strip()=='0'
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    gate();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    inputs=[Path(__file__),ROOT/'tools/ros_scene/qualification_integer_leaf_inline.h',ROOT/'test/0084.ros-runtime/qualification_integer_leaf_inline.cpp']
    pins={str(f):sha(f) for f in inputs}
    report=dict(complete=False,scope='closed integer-only private-inline LLVM experiment; production integration false',inputs=pins,
        cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),
        events_before=(GROUP/'memory.events').read_text(),commands=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,args):
        gate();start=time.monotonic();r=subprocess.run([str(x) for x in args],cwd=ROOT,capture_output=True,timeout=300)
        (out/(label+'.stdout')).write_bytes(r.stdout);(out/(label+'.stderr')).write_bytes(r.stderr)
        report['commands'].append(dict(label=label,args=[str(x) for x in args],exit=r.returncode,seconds=time.monotonic()-start));save()
        if r.returncode:raise RuntimeError(label+': '+r.stderr.decode(errors='replace')[-8000:])
        print('PASS',label,flush=True);return r.stdout.decode()
    save()
    try:
        config=shutil.which('llvm-config');clang=shutil.which('clang++')
        flags=shlex.split(run('llvm-cxxflags',[config,'--cxxflags']))
        flags=[v for v in flags if v!='-fno-exceptions' and not v.startswith('-std=')]
        flags=[part for v in flags for part in (['-isystem',v[2:]] if v.startswith('-I') else [v])]
        links=shlex.split(run('llvm-link-flags',[config,'--ldflags','--system-libs','--libs','core','asmparser','executionengine','mcjit','native','transformutils']))
        common=[*flags,'-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc','-Ithird-parties/fast_io/include']
        source=ROOT/'test/0084.ros-runtime/qualification_integer_leaf_inline.cpp'
        for label,compiler,sanitize in [('clang',clang,[]),('asan-ubsan',clang,['-fsanitize=address,undefined','-fno-omit-frame-pointer'])]:
            run(label+'-build',[compiler,*common,*sanitize,source,*links,'-o',out/label]);run(label,[out/label])
        report['inputs_after']={str(f):sha(f) for f in inputs};assert report['inputs_after']==pins
        report['complete']=True
    finally:report['events_after']=(GROUP/'memory.events').read_text();save()
if __name__=='__main__':main()
