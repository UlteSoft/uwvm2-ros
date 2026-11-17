#!/usr/bin/env python3
"""Qualify independent I1T policy at -O3 -g inside the existing 64 GiB gate."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',required=True,type=Path);args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    out=args.output.resolve();assert out.is_relative_to(ROOT.parent/'uwvm2-ros-sdk/.tmp');out.mkdir(parents=True,exist_ok=False)
    inputs=[Path(__file__).resolve(),ROOT/'test/0084.ros-runtime/wasithread_policy.cpp',ROOT/'src/uwvm2/utils/container/unique.h']
    inputs+=list((ROOT/'src/uwvm2/utils/control').glob('wasithread*.h'))
    inputs+=[ROOT/'src/uwvm2/utils/control'/n for n in ('host_interfaces.h','host_diagnostics.h')]
    inputs+=list((ROOT/'src/uwvm2/utils/allocator/fast_io_strict').glob('*.h'))
    record=dict(complete=False,source_hashes={str(p):sha(p) for p in inputs},cgroup=str(GROUP),inode=GROUP.stat().st_ino,
                memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text(),commands=[])
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv):
        result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=180,
            env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1',TSAN_OPTIONS='halt_on_error=1'))
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=result.returncode));save();print(label+': '+str(result.returncode),flush=True)
        return result
    success=False
    try:
        for name,cc,flags in [('asan','/usr/bin/clang++-22',['-fsanitize=address,undefined']),
                             ('gcc','/usr/bin/g++-13',[]),
                             ('no-exceptions','/usr/bin/clang++-22',['-fno-exceptions','-DI1T_ALLOCATION_FAULTS','-Wl,--wrap=malloc','-Wl,--wrap=calloc']),
                             ('tsan','/usr/bin/clang++-22',['-fsanitize=thread'])]:
            binary=out/name
            built=run(name+'-compile',[cc,'-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc','-Ithird-parties/fast_io/include',
                *flags,'test/0084.ros-runtime/wasithread_policy.cpp','-o',str(binary)])
            if built.returncode:raise RuntimeError(built.stderr[-7000:])
            for attempt in range(8 if name=='tsan' else 1):
                result=run(name+'-execute-'+str(attempt),[str(binary)])
                if not result.returncode:break
                if name!='tsan' or not any(x in result.stderr for x in ('unexpected memory mapping','personality(old_personality')):
                    raise RuntimeError(result.stderr[-7000:])
            assert result.returncode==0 and '6 safety groups passed' in result.stdout
        success=True
    finally:
        record['sources_unchanged']=all(sha(Path(p))==h for p,h in record['source_hashes'].items())
        record['events_after']=(GROUP/'memory.events').read_text();record['complete']=success and record['sources_unchanged'];save()
    assert record['complete']
if __name__=='__main__':main()
