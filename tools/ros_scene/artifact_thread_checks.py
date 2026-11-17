#!/usr/bin/env python3
"""Run only artifact safety contracts under TSan in the existing 64 GiB gate.

Retry solely the sanitizer's pre-main unexpected-memory-mapping startup failure.
Every attempt is preserved; race reports, assertions and other failures stop.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--compiler',default=shutil.which('clang++'))
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
    assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
    assert os.geteuid()!=0 and os.sched_getaffinity(0).issubset(set(range(0,4)))
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    source=ROOT/'test/0084.ros-runtime/artifact_security.cpp'
    inputs=[source,Path(__file__).resolve(),ROOT/'src/uwvm2/uwvm/imported/scene/input.h']
    inputs+=list(sorted((ROOT/'src/uwvm2/imported/ros/scene').glob('*.h')))
    inputs+=list(sorted((ROOT/'src/uwvm2/imported/ros/runtime').glob('*.h')))
    def hashes():return {str(path.relative_to(ROOT)):hashlib.sha256(path.read_bytes()).hexdigest() for path in inputs}
    record={'complete':False,'source_sha256':hashes(),'commands':[],'cgroup':str(GROUP),'memory_max':68719476736,
            'memory_swap_max':0,'cpu_affinity':sorted(os.sched_getaffinity(0)),'events_before':(GROUP/'memory.events').read_text()}
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(label,argv,env=None):
        result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=180)
        (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr)
        record['commands'].append({'name':label,'argv':argv,'exit':result.returncode});save()
        return result
    passed=False
    try:
        binary=out/'artifact-thread'
        result=run('compile',[args.compiler,'-std=c++23','-O1','-g','-Wall','-Wextra','-Werror','-pthread',
                   '-fsanitize=thread','-fno-omit-frame-pointer','-Isrc','-Ithird-parties/fast_io/include',str(source),'-o',str(binary)])
        if result.returncode:raise RuntimeError(result.stderr[-5000:])
        record['binary_sha256']=hashlib.sha256(binary.read_bytes()).hexdigest()
        with tempfile.TemporaryDirectory(prefix='uwvm-artifact-thread-',dir='/dev/shm') as temporary:
            environment=dict(os.environ,UWVM_INPUT_TEST_TMP=temporary,TSAN_OPTIONS='halt_on_error=1')
            for attempt in range(1,9):
                result=run('run-'+str(attempt),[str(binary)],environment)
                if result.returncode==0:
                    assert 'passed 7 artifact broker/deployment safety groups' in result.stdout
                    assert 'WARNING: ThreadSanitizer:' not in result.stderr
                    print(result.stdout,flush=True);passed=True;break
                startup=result.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not result.stdout
                if not startup:raise RuntimeError(result.stderr[-5000:])
                print('Retry sanitizer address-layout startup failure '+str(attempt),flush=True)
            if not passed:raise RuntimeError('TSan could not initialize after eight recorded address-layout attempts')
    finally:
        record['sources_unchanged']=hashes()==record['source_sha256']
        record['events_after']=(GROUP/'memory.events').read_text()
        record['complete']=passed and record['sources_unchanged'];save()
        if passed and not record['sources_unchanged']:raise RuntimeError('Source changed during qualification')


if __name__=='__main__':main()
