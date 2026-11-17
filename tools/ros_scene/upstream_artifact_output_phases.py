#!/usr/bin/env python3
"""Replay a frozen output benchmark as separately measured diagnostic phases.

The full baseline remains immutable. This reuses its compiler flags, inputs,
VM, grants and CPU, replacing only the benchmark loop. Serialization and
transport times are not an additive decomposition of the complete workflow.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import statistics
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset(set(range(8,16)))
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--baseline',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();guard()
    base=args.baseline.resolve();prep=json.loads((base/'prepare.json').read_text());baseline=json.loads((base/'baseline.json').read_text())
    assert prep['complete'] and baseline['complete']
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    source=ROOT/'test/0084.ros-runtime/upstream_artifact_output_phases.cpp';shutil.copyfile(source,out/source.name)
    pins={**prep['inputs'],**prep['toolchain'],str(base/'prepare.json'):sha(base/'prepare.json'),str(base/'baseline.json'):sha(base/'baseline.json'),
          str(source):sha(source),str(Path(__file__).resolve()):sha(__file__)}
    for name,digest in pins.items():assert sha(name)==digest,name
    old='upstream_artifact_output_benchmark.cpp'
    record=dict(complete=False,scope=__doc__,cgroup=str(GATE),memory_max=68719476736,swap_max=0,
        events_before=(GATE/'memory.events').read_text(),inputs=pins,commands=[],observations=[],comparison={})
    def save():(out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    def run(name,argv):
        guard();start=time.monotonic();r=subprocess.run(argv,cwd=ROOT,capture_output=True,timeout=180)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        record['commands'].append(dict(name=name,argv=argv,exit=r.returncode,seconds=time.monotonic()-start));save()
        assert r.returncode==0,(name,r.stderr.decode(errors='replace')[-4000:]);print('PASS',name,flush=True);return r.stdout.decode()
    runtime=Path(next(row['argv'][3] for row in baseline['commands'] if row['name'].startswith('wasm-')))
    assert sha(runtime)==baseline['runtime_sha256'];record['runtime_sha256']=sha(runtime)
    expected=None
    for phase,number in [('serialize',1),('transport',2)]:
        binaries={}
        for provider in ['native','wasm']:
            compile_argv=list(next(row['argv'] for row in prep['commands'] if row['name']=='compile-'+provider+'-baseline'))
            compile_argv=[str(out/source.name) if Path(arg).name==old else arg for arg in compile_argv]
            compile_argv.insert(1,'-DOUTPUT_PHASE='+str(number))
            target=out/(phase+('-native' if provider=='native' else '.wasm'));compile_argv[compile_argv.index('-o')+1]=str(target)
            run('compile-'+phase+'-'+provider,compile_argv);binaries[provider]=str(target)
        for round_number in range(3):
            for provider in (['native','wasm'] if round_number%2==0 else ['wasm','native']):
                argv=list(next(row['argv'] for row in baseline['commands'] if row['name']==provider+'-0'))
                argv[-1]=binaries[provider]
                text=run(phase+'-'+provider+'-'+str(round_number),argv)
                values=tuple(map(int,re.search(r'phase=(\d+) iterations=(\d+) batches=(\d+) outputs=(\d+) chunk=(\d+) expected=(\d+) checksum=(\d+)',text).groups()))
                assert values[:5]==(number,1000,31,14,prep['fixtures']['baseline']['chunk'])
                if expected is None:expected=values[5:]
                else:assert expected==values[5:]
                samples=list(map(int,re.findall(r'elapsed_ns=(\d+)',text)));assert len(samples)==31 and min(samples)>0
                record['observations'].append(dict(phase=phase,provider=provider,round=round_number,samples_ns=samples,
                    p50_ns_per_cycle=statistics.median(samples)/1000));save()
        medians={provider:statistics.median(r['p50_ns_per_cycle'] for r in record['observations'] if r['phase']==phase and r['provider']==provider) for provider in ['native','wasm']}
        record['comparison'][phase]=dict(median_of_round_p50_ns=medians,wasm_native_ratio=medians['wasm']/medians['native'])
    assert sha(runtime)==record['runtime_sha256']
    for name,digest in pins.items():assert sha(name)==digest,name
    record['events_after']=(GATE/'memory.events').read_text();record['complete']=True;save();print(json.dumps(record['comparison'],indent=2))
if __name__=='__main__':main()
