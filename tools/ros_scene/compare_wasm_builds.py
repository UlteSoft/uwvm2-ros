#!/usr/bin/env python3
"""Compare two recorded release VMs using identical Wasm/native fixtures.

Run through in_existing_cgroup.py. Reserve the selected CPU and its SMT sibling
for the comparison. This tool records topology and alternating before/after/
native runs; it does not alter CPU, PMU, memory or security policy. Source
receipts describe each historical build, so an older source checkout is not
silently presented as the current one. Batch medians measure throughput, not
individual request latency or complete robot performance.
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


ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def digest(path):
    value=hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda:stream.read(1024*1024),b''): value.update(block)
    return value.hexdigest()


def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736'
    assert (GATE/'memory.swap.max').read_text().strip()=='0'


def build(path):
    full=json.loads((path.parent/'full-build-results.json').read_text())
    final=json.loads((path.parent/'incremental-results.json').read_text())
    units=['main.default.cpp','uwvm_runtime.default.cpp','host_api.default.cpp']
    assert full['optimization']=='O3' and full['complete'] and final['complete'] and final['sources_unchanged']
    assert final['link_returncode']==0
    assert {unit['unit'] for unit in full['units'] if unit['returncode']==0}=={unit+'.o' for unit in units}
    commands={}
    for unit in units:
        command=json.loads((path.parent/(unit+'.o.argv.json')).read_text())
        levels=[arg for arg in command if re.fullmatch(r'-O(?:[0-3szg]|fast)',arg)]
        assert levels and levels[-1]=='-O3' and '-c' in command and not any(arg.startswith('-fsanitize=') for arg in command)
        commands[unit]=command
    return dict(path=str(path),sha256=digest(path),compile_commands=commands,sources=final['sources'],
                receipts={name:digest(path.parent/name) for name in ['full-build-results.json','incremental-results.json']})


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--before',type=Path,required=True)
    parser.add_argument('--after',type=Path,required=True)
    parser.add_argument('--prepared-directory',type=Path,required=True)
    parser.add_argument('--native-directory',type=Path,default=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security/performance-typed')
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--cpu',type=int,default=2)
    parser.add_argument('--rounds',type=int,default=3)
    args=parser.parse_args();guard()
    assert args.cpu in os.sched_getaffinity(0) and 3<=args.rounds<=9
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert not (out/'results.json').exists(),'Refusing to overwrite a paired comparison'
    binaries={'before':args.before.resolve(),'after':args.after.resolve()}
    prepared_dir=args.prepared_directory.resolve();prepared=json.loads((prepared_dir/'prepare.json').read_text())
    native_dir=args.native_directory.resolve();native=native_dir/'native'
    native_manifest=json.loads((native_dir/'prepare.json').read_text())
    assert prepared['complete'] and native_manifest['complete']
    assert digest(native)==native_manifest['binary_sha256']['native']
    for name,expected in native_manifest['native_libraries'].items():assert digest(native_dir/'native-libs'/Path(name).name)==expected
    report=dict(complete=False,cgroup=str(GATE),cgroup_inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,
                cpu=args.cpu,smt_siblings=Path(f'/sys/devices/system/cpu/cpu{args.cpu}/topology/thread_siblings_list').read_text().strip(),
                events_before=(GATE/'memory.events').read_text(),builds={name:build(path) for name,path in binaries.items()},
                native_sha256=digest(native),native_prepare_sha256=digest(native_dir/'prepare.json'),
                prepare_sha256=digest(prepared_dir/'prepare.json'),runs=[],observations=[],comparison=[],
                scope='identical real Wasm32 AOT clock/guard fixtures with native rcl; startup excluded from guest interval',
                native_parity_qualified=False,guest_jit_source_mapping_qualified=False)
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    save()
    for round_number in range(args.rounds):
        for workload in ['clock','guard','mixed']:
            fixture=prepared['fixtures']['baseline-'+workload];wasm=prepared_dir/('baseline-'+workload+'.wasm')
            assert digest(wasm)==fixture['sha256']
            for provider in (['before','after','native'] if round_number%2==0 else ['native','after','before']):
                guard();env=dict(os.environ)
                if provider=='native':
                    command=['taskset','-c',str(args.cpu),str(native),workload,str(fixture['iterations']),str(fixture['batches'])]
                    env.update(LD_LIBRARY_PATH=str(native_dir/'native-libs'),RMW_IMPLEMENTATION='rmw_fastrtps_cpp')
                else:
                    command=['taskset','-c',str(args.cpu),str(binaries[provider]),'--wasm-set-main-module-name','main','--mode','run',
                             '--runtime-aot','--runtime-llvm-jit-policy','max','--runtime-llvm-jit-cache-path','disable',
                             '--runtime-compile-threads','1',*fixture['policy_argv'],'--run',str(wasm)]
                start=time.monotonic();load=Path('/proc/loadavg').read_text()
                result=subprocess.run(command,cwd=ROOT,env=env,capture_output=True,timeout=120);wall=time.monotonic()-start
                name=f'{provider}-{workload}-{round_number}'
                (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
                report['runs'].append(dict(name=name,argv=command,exit=result.returncode,wall_seconds=wall,loadavg=load));save()
                assert result.returncode==0,(name,result.stderr.decode(errors='replace')[-3000:])
                lines=[line for line in result.stdout.decode().splitlines() if line.startswith('{"provider":')];assert len(lines)==1
                value=json.loads(lines[0]);assert value['workload']==workload and value['iterations_per_batch']==fixture['iterations']
                assert value['batches']==fixture['batches'] and value['logical_operations']==fixture['iterations']*fixture['batches']
                if workload=='guard':assert value['checksum']==value['logical_operations']+1000
                if provider!='native':
                    assert value['wasm_executed'] and value['provider']=='scene-wasm-aot'
                    value['elapsed_seconds']=value['elapsed_ns']/1e9
                    for percentile in [50,95,99]:value['batch_mean_ns_p'+str(percentile)]=value['batch_ns_p'+str(percentile)]/fixture['iterations']
                    value['guest_sha256']=fixture['sha256']
                else:assert not value['wasm_executed'] and value['provider']=='native-rcl'
                value.update(build=provider,round=round_number,process_wall_seconds=wall,
                             outside_measured_region_seconds=wall-value['elapsed_seconds'])
                report['observations'].append(value);save();print('PASS',name,value['batch_mean_ns_p50'],flush=True)
    for workload in ['clock','guard','mixed']:
        medians={name:statistics.median(row['batch_mean_ns_p50'] for row in report['observations']
                                       if row['build']==name and row['workload']==workload) for name in ['before','after','native']}
        report['comparison'].append(dict(workload=workload,median_of_round_p50_ns=medians,
            after_to_before_ratio=medians['after']/medians['before'],after_to_native_ratio=medians['after']/medians['native']))
    for name,path in binaries.items():assert digest(path)==report['builds'][name]['sha256']
    assert digest(native)==report['native_sha256']
    assert digest(prepared_dir/'prepare.json')==report['prepare_sha256']
    for name,expected in native_manifest['native_libraries'].items():assert digest(native_dir/'native-libs'/Path(name).name)==expected
    guard();report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()


if __name__=='__main__':main()
