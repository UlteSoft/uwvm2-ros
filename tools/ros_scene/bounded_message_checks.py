#!/usr/bin/env python3
"""Qualify bounded ROS message descriptions in the existing 64 GiB test gate.

Compile from exact compiler-derived source snapshots with O3/full debug info.
Sanitizers and recoverable allocation injection exercise hostile schemas,
message layouts, transactional conversion and immutable validator ownership.
This does not qualify native middleware or a real robot.
"""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import time
from platform_agilex_checks import ROOT,GROUP,guard,sha

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();guard()
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    snapshot=out/'source';source=Path('test/0084.ros-runtime/bounded_message.cpp')
    flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-fno-builtin-malloc']
    variants=[('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),
              ('gcc13','/usr/bin/g++-13',[]),
              ('allocation','/usr/bin/clang++-22',['-DUWVM_BOUNDED_MESSAGE_ALLOCATION','-Wl,--wrap=malloc','-Wl,--wrap=calloc','-Wl,--wrap=realloc'])]
    record=dict(complete=False,scope=__doc__,commands=[],inputs={},snapshots={},binaries={},cgroup=str(GROUP),
                memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,env=None):
        guard();start=time.monotonic();result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=240)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-start));save()
        print(label+': '+str(result.returncode),flush=True)
        assert result.returncode==0,(label,result.stderr[-7000:]);return result
    success=False
    try:
        dependencies={source,Path(__file__).resolve().relative_to(ROOT)}
        for name,compiler,extra in variants:
            record['inputs'][compiler]=sha(compiler)
            found=run(name+'-dependencies',[compiler,*flags,*[p for p in extra if not p.startswith('-Wl,')],
                '-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include'),'-MM','-MT','schema',str(ROOT/source)])
            parsed=shlex.split(found.stdout.replace('\\\n',' '));assert parsed[0]=='schema:'
            for item in parsed[1:]:
                path=Path(item).resolve();assert path.is_relative_to(ROOT);dependencies.add(path.relative_to(ROOT))
        for name in sorted(dependencies):
            path=ROOT/name;target=snapshot/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
            record['inputs'][str(path)]=sha(path);record['snapshots'][str(name)]=sha(target)
        for name,compiler,extra in variants:
            binary=out/name
            run(name+'-compile',[compiler,*flags,*extra,'-I'+str(snapshot/'src'),'-I'+str(snapshot/'third-parties/fast_io/include'),str(snapshot/source),'-o',str(binary)])
            record['binaries'][name]=sha(binary)
            result=run(name,[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1'))
            assert 'PASS bounded ROS messages:' in result.stdout
        success=True
    finally:
        record['inputs_unchanged']=all(Path(p).is_file() and sha(p)==h for p,h in record['inputs'].items())
        record['snapshots_unchanged']=all(sha(snapshot/p)==h for p,h in record['snapshots'].items())
        record['events_after']=(GROUP/'memory.events').read_text()
        record['complete']=success and record['inputs_unchanged'] and record['snapshots_unchanged'];save()
    assert record['complete']

if __name__=='__main__':main()
