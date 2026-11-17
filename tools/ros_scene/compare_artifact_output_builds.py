#!/usr/bin/env python3
"""Measure old/new VMs and native on identical output bytes in alternating rounds.

This preserves historical baseline receipts. Each round runs all three
providers on one CPU using one frozen native/Wasm fixture and exact policy.
Startup is outside guest batch timing. This is volatile-cache throughput,
not native ROS parity, individual request latency or an exclusive-host test.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import statistics
import subprocess
import time
from performance_build_policy import fixture_policy,vm_policy

ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset(set(range(8,16)))
def build(runtime,qualification):
    full=runtime.parent/'full-build-results.json';record=json.loads(full.read_text())
    assert record['complete'] and record['optimization']=='O3' and record['sources_unchanged'] and record['link_returncode']==0
    assert record['binary_sha256']==sha(runtime)
    assert {x['unit'] for x in record['units'] if x['returncode']==0}=={'main.default.cpp.o','uwvm_runtime.default.cpp.o','host_api.default.cpp.o'}
    compile_audit=vm_policy(runtime)
    for unit in ['main.default.cpp','uwvm_runtime.default.cpp','host_api.default.cpp']:
        argv=json.loads((runtime.parent/(unit+'.o.argv.json')).read_text())
        assert [x for x in argv if re.fullmatch(r'-O(?:[0-3szg]|fast)',x)][-1]=='-O3'
        assert not any(x.startswith('-fsanitize=') for x in argv)
    qualified=json.loads(qualification.read_text());assert qualified['complete'] and qualified['runtime']['sha256']==sha(runtime)
    manifest=Path(record['full_source_manifest']);source=Path(record['source_root'])
    assert sha(manifest)==record['full_source_manifest_sha256'] and record['full_source_unchanged']
    for name,digest in json.loads(manifest.read_text()).items():assert sha(source/name)==digest
    return dict(path=str(runtime),sha256=sha(runtime),full_build_sha256=sha(full),qualification=str(qualification),qualification_sha256=sha(qualification),
        compile_policy=compile_audit,source_root=str(source),source_manifest=str(manifest),source_manifest_sha256=sha(manifest))
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--before',type=Path,required=True);parser.add_argument('--after',type=Path,required=True)
    parser.add_argument('--before-qualification',type=Path,required=True);parser.add_argument('--after-qualification',type=Path,required=True)
    parser.add_argument('--baseline',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--cpu',type=int,default=8);parser.add_argument('--rounds',type=int,default=3)
    args=parser.parse_args();guard();assert args.cpu in os.sched_getaffinity(0) and 3<=args.rounds<=9
    base=args.baseline.resolve();prepared=json.loads((base/'prepare.json').read_text());baseline=json.loads((base/'baseline.json').read_text())
    assert prepared['complete'] and baseline['complete']
    compile_audit=fixture_policy(prepared)
    binaries={'before':args.before.resolve(),'after':args.after.resolve()}
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    pins={**prepared['inputs'],**prepared['toolchain'],str(base/'prepare.json'):sha(base/'prepare.json'),str(base/'baseline.json'):sha(base/'baseline.json')}
    for name,digest in pins.items():assert sha(name)==digest,name
    report=dict(complete=False,scope=__doc__,cgroup=str(GATE),memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),
        cpu=args.cpu,smt_siblings=Path(f'/sys/devices/system/cpu/cpu{args.cpu}/topology/thread_siblings_list').read_text().strip(),
        builds={name:build(path,getattr(args,name+'_qualification').resolve()) for name,path in binaries.items()},
        inputs=pins,runner_sha256=sha(__file__),policy_runner_sha256=sha(Path(__file__).with_name('performance_build_policy.py')),
        fixture_compile_policy=compile_audit,commands=[],observations=[],native_parity_qualified=False)
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    fixture=prepared['fixtures']['baseline'];expected=None
    orders=[['before','after','native'],['native','after','before'],['after','before','native']]
    for number in range(args.rounds):
        for provider in orders[number%len(orders)]:
            template='native-0' if provider=='native' else 'wasm-0'
            argv=list(next(row['argv'] for row in baseline['commands'] if row['name']==template))
            assert argv[:2]==['taskset','-c'];argv[2]=str(args.cpu)
            if provider!='native':argv[3]=str(binaries[provider])
            guard();load=Path('/proc/loadavg').read_text();start=time.monotonic()
            r=subprocess.run(argv,cwd=ROOT,capture_output=True,timeout=180);label=provider+'-'+str(number)
            (out/(label+'.stdout')).write_bytes(r.stdout);(out/(label+'.stderr')).write_bytes(r.stderr)
            report['commands'].append(dict(name=label,argv=argv,exit=r.returncode,seconds=time.monotonic()-start,loadavg=load));save()
            assert r.returncode==0,(label,r.stderr.decode(errors='replace')[-5000:])
            text=r.stdout.decode();match=re.search(r'iterations=(\d+) batches=(\d+) outputs=(\d+) chunk=(\d+) expected=(\d+) checksum=(\d+)',text);assert match
            values=tuple(map(int,match.groups()));assert values[:4]==(fixture['iterations'],fixture['batches'],14,fixture['chunk'])
            if expected is None:expected=values[4:]
            else:assert expected==values[4:]
            samples=list(map(int,re.findall(r'elapsed_ns=(\d+)',text)));assert len(samples)==fixture['batches'] and min(samples)>0
            value=statistics.median(samples)/fixture['iterations']
            report['observations'].append(dict(provider=provider,round=number,chunk=fixture['chunk'],samples_ns=samples,p50_ns_per_cycle=value));save()
            print('PASS',label,value,flush=True)
    medians={provider:statistics.median(row['p50_ns_per_cycle'] for row in report['observations'] if row['provider']==provider) for provider in ['before','after','native']}
    report['comparison']=dict(median_of_round_p50_ns=medians,after_before_ratio=medians['after']/medians['before'],
        after_native_ratio=medians['after']/medians['native'],before_native_ratio=medians['before']/medians['native'])
    for name,path in binaries.items():
        entry=report['builds'][name];assert sha(path)==entry['sha256'] and sha(path.parent/'full-build-results.json')==entry['full_build_sha256']
        for audit in entry['compile_policy'].values():assert sha(audit['argv_file'])==audit['argv_sha256']
        assert sha(entry['source_manifest'])==entry['source_manifest_sha256']
        for relative,digest in json.loads(Path(entry['source_manifest']).read_text()).items():assert sha(Path(entry['source_root'])/relative)==digest
    for name,digest in pins.items():assert sha(name)==digest,name
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save();print(json.dumps(report['comparison'],indent=2))
if __name__=='__main__':main()
