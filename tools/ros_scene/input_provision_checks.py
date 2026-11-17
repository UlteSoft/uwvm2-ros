#!/usr/bin/env python3
"""Qualify trusted input snapshots with sanitizers/GCC in the existing 64 GiB gate.

Tests use private tmpfs files as an unprivileged user and do not modify existing
deployment inputs. The runner records exact source hashes before compilation
and requires them unchanged before publishing a successful qualification.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    assert os.geteuid()!=0 and os.sched_getaffinity(0).issubset(set(range(8,16)))
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    source=ROOT/'test/0084.ros-runtime/input_provision.cpp'
    inputs=[ROOT/'src/uwvm2/uwvm/imported/scene/input.h',source]
    inputs += [ROOT/('src/uwvm2/imported/ros/'+name+'.h') for name in
               ('scene/artifacts','scene/artifact_contract','scene/storage','scene/authority','runtime/resources','runtime/contract')]
    def hashes():return {str(path.relative_to(ROOT)):hashlib.sha256(path.read_bytes()).hexdigest() for path in inputs}
    record={'complete':False,'source_sha256':hashes(),'commands':[],'cgroup':str(GROUP),'memory_max':68719476736,
            'memory_swap_max':0,'cpu_affinity':sorted(os.sched_getaffinity(0)),'events_before':(GROUP/'memory.events').read_text()}
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,env=None):
        result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=90)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append({'name':label,'argv':argv,'exit':result.returncode});save()
        if result.returncode:raise RuntimeError(label+': '+result.stderr[-5000:])
        print(label+': '+result.stdout.strip(),flush=True)
    passed=False
    try:
        with tempfile.TemporaryDirectory(prefix='uwvm-provision-',dir='/dev/shm') as temporary:
            environment=dict(os.environ,UWVM_INPUT_TEST_TMP=temporary,ASAN_OPTIONS='detect_leaks=1')
            for label,compiler,flags in [('clang-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined','-fno-omit-frame-pointer']),
                                         ('gcc13','/usr/bin/g++-13',[])]:
                binary=out/label
                run(label+'-compile',[compiler,'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread','-Isrc',
                    '-Ithird-parties/fast_io/include',*flags,str(source),'-o',str(binary)])
                run(label,[str(binary)],environment)
        passed=True
    finally:
        record['sources_unchanged']=hashes()==record['source_sha256']
        record['events_after']=(GROUP/'memory.events').read_text()
        record['complete']=passed and record['sources_unchanged'];save()
        if passed and not record['sources_unchanged']:raise RuntimeError('Source changed during qualification')


if __name__=='__main__':main()
