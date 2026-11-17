#!/usr/bin/env python3
"""Paired frozen XML A/B experiment with identical O3g fixtures and zero workers.

This records batch timing, process/load/memory evidence and per-round variance.
It does not convert a noisy difference into a performance improvement claim.
Both VMs must first pass actual correctness/ownership qualification.
"""
import argparse,hashlib,json,os,re,statistics,subprocess,threading,time
from pathlib import Path
from performance_build_policy import fixture_policy,vm_policy
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--original',type=Path,required=True);p.add_argument('--candidate',type=Path,required=True)
    p.add_argument('--original-qualification',type=Path,required=True);p.add_argument('--candidate-qualification',type=Path,required=True)
    p.add_argument('--profile',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    vms={'original':a.original.resolve(),'candidate':a.candidate.resolve()}
    qualifications={'original':a.original_qualification.resolve(),'candidate':a.candidate_qualification.resolve()}
    builds={}
    for label,vm in vms.items():
        builds[label]=json.loads((vm.parent/'full-build-results.json').read_text())
        q=json.loads(qualifications[label].read_text())
        assert builds[label]['complete'] and q['complete'] and builds[label]['binary_sha256']==q['inputs'][str(vm)]==sha(vm)
    profile=a.profile.resolve();prior=json.loads((profile/'profile.json').read_text());assert prior['complete']
    prepare=json.loads((profile/'prepare.json').read_text());assert prepare['complete']
    fixture_audit=fixture_policy(prepare)
    for path,digest in prepare['inputs'].items():assert sha(path)==digest
    collected=next(x['argv'] for x in prior['commands'] if x['name']=='collect-wasm-hotspots')
    command=collected[collected.index('--')+1:]
    command[command.index('--runtime-compile-threads')+1]='0'
    wasm=Path(command[-1]).with_name('baseline.wasm');native=wasm.with_name('native-baseline')
    assert sha(wasm)==prepare['inputs'][str(wasm)] and sha(native)==prepare['inputs'][str(native)]
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    report=dict(complete=False,scope=__doc__,cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,
        events_before=(GROUP/'memory.events').read_text(),inputs={str(v):sha(v) for v in [*vms.values(),*qualifications.values(),
        profile/'profile.json',profile/'prepare.json',wasm,native,Path(__file__)]},compile_policy={label:vm_policy(vm) for label,vm in vms.items()},
        fixture_compile_policy=fixture_audit,compile_threads=0,cpu=8,commands=[],observations=[],native_ros_parity=False,
        production_integration=False,improvement_claim_qualified=False)
    def gate():
        assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
        assert GROUP.stat().st_ino==report['inode'] and (GROUP/'memory.max').read_text().strip()=='68719476736'
        assert (GROUP/'memory.swap.max').read_text().strip()=='0'
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    stop=threading.Event()
    def monitor():
        with (out/'window.jsonl').open('w') as stream:
            while not stop.is_set():
                stream.write(json.dumps(dict(monotonic=time.monotonic(),loadavg=Path('/proc/loadavg').read_text().strip(),
                    current=int((GROUP/'memory.current').read_text()),events=(GROUP/'memory.events').read_text()))+'\n');stream.flush();stop.wait(1)
    expected=None;thread=threading.Thread(target=monitor);thread.start()
    env=dict(os.environ,DEBUGINFOD_URLS='')
    for key in ['INTEL_JIT_PROFILER64','UWVM_TEST_JIT_EVENTS','UWVM_TEST_JIT_REJECT_LOAD']:env.pop(key,None)
    try:
        gate();save();(out/'processes-before.txt').write_text(subprocess.check_output(['ps','-eo','pid,psr,pcpu,rss,comm,args'],text=True))
        for round_id,order in enumerate([['native','original','candidate'],['candidate','native','original'],['original','candidate','native']]):
            for provider in order:
                argv=command.copy();argv[3]=str(vms.get(provider,vms['original']));argv[-1]=str(wasm)
                if provider=='native':argv=['taskset','-c','8',str(native)]
                gate();start=time.monotonic();r=subprocess.run(argv,capture_output=True,env=env,timeout=180)
                label=f'{round_id}-{provider}';(out/(label+'.stdout')).write_bytes(r.stdout);(out/(label+'.stderr')).write_bytes(r.stderr)
                report['commands'].append(dict(label=label,argv=argv,exit=r.returncode,seconds=time.monotonic()-start));save()
                assert r.returncode==0,(label,r.stderr[-4000:])
                text=r.stdout.decode();header=re.search(r'iterations=(\d+) batches=(\d+) outputs=(\d+) chunk=(\d+) expected=(\d+) checksum=(\d+)',text)
                assert header;values=tuple(map(int,header.groups()));assert values[:4]==(1000,31,14,4096)
                if expected is None:expected=values[4:]
                assert expected==values[4:]
                durations=[int(v)/1000 for v in re.findall(r'elapsed_ns=(\d+)',text)];assert len(durations)==31 and min(durations)>0
                median=statistics.median(durations)
                obs=dict(round=round_id,provider=provider,ns_per_cycle=durations,median_ns=median,mean_ns=statistics.mean(durations),
                    stdev_ns=statistics.stdev(durations),mad_ns=statistics.median(abs(v-median) for v in durations),min_ns=min(durations),max_ns=max(durations))
                if provider=='candidate':
                    match=re.search(r'\[integer-leaf-qualification\] leaves=(\d+) edges=(\d+) rejected-attrs=(\d+) rejected-metadata=(\d+) rejected-tail=(\d+) rejected-shape=(\d+) status=(\d+)',r.stderr.decode())
                    assert match and match[7]=='0';obs['integer_leaf_counts']=list(map(int,match.groups()))
                report['observations'].append(obs);save();print('PASS',label,median,flush=True)
        medians={provider:statistics.median(x['median_ns'] for x in report['observations'] if x['provider']==provider) for provider in ['native','original','candidate']}
        report['comparison']=dict(median_of_round_medians_ns=medians,candidate_original_ratio=medians['candidate']/medians['original'],
            original_native_ratio=medians['original']/medians['native'],candidate_native_ratio=medians['candidate']/medians['native'])
        report['paired_round_ratios']=[]
        for round_id in range(3):
            rows={x['provider']:x['median_ns'] for x in report['observations'] if x['round']==round_id}
            report['paired_round_ratios'].append(dict(round=round_id,candidate_original_ratio=rows['candidate']/rows['original']))
        assert all(sha(path)==digest for path,digest in report['inputs'].items());report['complete']=True
    finally:
        stop.set();thread.join();gate();report['events_after']=(GROUP/'memory.events').read_text()
        (out/'processes-after.txt').write_text(subprocess.check_output(['ps','-eo','pid,psr,pcpu,rss,comm,args'],text=True));save()
if __name__=='__main__':main()
