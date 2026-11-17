#!/usr/bin/env python3
"""Official hardware Hotspots/uarch collection for one symbol-only frozen VM.

Reuses immutable -O3 -g XML fixtures, with zero additional compile workers.
Original guest source lines remain unavailable. Whole-process samples are not
a native ROS parity benchmark; microarchitecture CPU-time-derived metrics are
withheld pending independent validation. Address samples may skid.
"""
import argparse,csv,hashlib,json,os,re,subprocess,threading,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def wasm_function_names(path):
    """Read bounded name-section labels only; never resolve a debug path."""
    data=path.read_bytes();assert len(data)<=64*1024*1024 and data[:8]==b'\0asm\x01\0\0\0';names={}
    def number(at,end):
        value=0
        for n in range(5):
            assert at<end;byte=data[at];at+=1;value|=(byte&127)<<(7*n)
            if byte<128:assert value<=0xffffffff;return value,at
        raise AssertionError('Invalid bounded ULEB')
    def string(at,end):
        size,at=number(at,end);assert size<=4096 and at+size<=end
        return data[at:at+size].decode('utf-8'),at+size
    at=8
    while at<len(data):
        kind=data[at];size,at=number(at+1,len(data));end=at+size;assert end<=len(data)
        if kind==0:
            label,content=string(at,end)
            if label=='name':
                while content<end:
                    sub=data[content];length,content=number(content+1,end);sub_end=content+length;assert sub_end<=end
                    if sub==1:
                        count,content=number(content,sub_end);assert count<=65536
                        for _ in range(count):
                            index,content=number(content,sub_end);label,content=string(content,sub_end)
                            assert index not in names and all(ord(x)>=32 for x in label);names[index]=label
                        assert content==sub_end
                    content=sub_end
        at=end
    return names
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path,required=True)
    p.add_argument('--previous-profile',type=Path,required=True);p.add_argument('--qualification',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    vm=a.runtime.resolve();build=json.loads((vm.parent/'full-build-results.json').read_text())
    assert build['complete'] and build['binary_sha256']==sha(vm) and build['intel_jit_qualification']
    qualified=json.loads(a.qualification.read_text());assert qualified['complete'] and qualified['inputs'][str(vm)]==sha(vm)
    previous=a.previous_profile.resolve();prior=json.loads((previous/'profile.json').read_text());assert prior['complete']
    prepared=json.loads((previous/'prepare.json').read_text());assert prepared['complete']
    for path,digest in prepared['inputs'].items():assert sha(path)==digest
    vtune=prior['commands'][0]['argv'][0];vtune_path=Path(vtune).resolve()
    assert vtune_path.parents[1]==Path(build['intel_jit_qualification']['sdk']).parent
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    (out/'prepare.json').write_text(json.dumps(prepared,indent=2)+'\n')
    frozen=Path(build['source_root']);old_frozen=prior['vm_source_root']
    old_vm=next(x['argv'][x['argv'].index('--')+4] for x in prior['commands'] if x['name']=='collect-wasm-hotspots')
    report=dict(complete=False,scope=__doc__,runtime_sha256=sha(vm),vm_source_root=str(frozen),
        build_sha256=sha(vm.parent/'full-build-results.json'),qualification_sha256=sha(a.qualification),
        prepare_sha256=sha(previous/'prepare.json'),previous_profile_sha256=sha(previous/'profile.json'),
        full_source_manifest_sha256=build['full_source_manifest_sha256'],runner_sha256=sha(__file__),
        guest_jit_source_mapping_qualified=False,production_integration=False,native_parity_qualified=False,
        cpu_time_derived_uarch_metrics_qualified=False,commands=[],observations={},tools={str(vtune_path):sha(vtune_path)},
        cgroup=str(GROUP),inode=GROUP.stat().st_ino,boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
        events_before=(GROUP/'memory.events').read_text(),memory_max=68719476736,swap_max=0)
    def gate():
        assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
        assert GROUP.stat().st_ino==report['inode'] and (GROUP/'memory.max').read_text().strip()=='68719476736'
        assert (GROUP/'memory.swap.max').read_text().strip()=='0'
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip()==report['boot_id']
    def save():(out/'profile.json').write_text(json.dumps(report,indent=2)+'\n')
    clean=dict(os.environ,DEBUGINFOD_URLS='')
    for name in ['INTEL_JIT_PROFILER64','UWVM_PROFILE_MARKER','UWVM_TEST_JIT_EVENTS','UWVM_TEST_JIT_REJECT_LOAD']:clean.pop(name,None)
    def run(name,argv):
        gate();start=time.monotonic();r=subprocess.run(argv,capture_output=True,env=clean,timeout=600)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        report['commands'].append(dict(name=name,argv=argv,exit=r.returncode,seconds=time.monotonic()-start));save()
        assert r.returncode==0,(name,r.stderr[-5000:]);print('PASS',name,flush=True);return r.stdout
    stop=threading.Event()
    def monitor():
        with (out/'window.jsonl').open('w') as stream:
            while not stop.is_set():
                stream.write(json.dumps(dict(monotonic=time.monotonic(),loadavg=Path('/proc/loadavg').read_text().strip(),
                    current=int((GROUP/'memory.current').read_text()),events=(GROUP/'memory.events').read_text()))+'\n');stream.flush();stop.wait(1)
    thread=threading.Thread(target=monitor);thread.start()
    try:
        gate();save()
        for when in ['before']:
            (out/('processes-'+when+'.txt')).write_text(subprocess.check_output(['ps','-eo','pid,psr,pcpu,rss,comm,args'],text=True))
        for entry in prior['commands']:
            name=entry['name']
            if name!='vtune-version' and '-wasm-' not in name:continue
            argv=[str(vm) if x==old_vm else str(frozen) if x==old_frozen else x.replace(str(previous/'vtune-wasm-'),str(out/'vtune-wasm-')) for x in entry['argv']]
            if '--runtime-compile-threads' in argv:argv[argv.index('--runtime-compile-threads')+1]='0'
            run(name,argv)
            if name.startswith('collect-wasm-'):
                text=(out/(name+'.stderr')).read_text(errors='replace')+(out/(name+'.stdout')).read_text(errors='replace')
                matches=re.findall(r'finalized-symbols=(\d+)',text)
                assert matches and all(int(x)>0 for x in matches),'Actual VM did not publish symbols to active official collector'
        with (out/'report-wasm-hotspots-hotspots.stdout').open() as stream:rows=list(csv.DictReader(stream,delimiter='\t'))
        sampled=[r for r in rows if r['Function'].startswith('uwvm_m_')]
        assert sampled,'No actual guest JIT symbols received hardware samples'
        total=sum(float(r['CPU Time']) for r in rows);guest=sum(float(r['CPU Time']) for r in sampled)
        unknown=sum(float(r['CPU Time']) for r in rows if 'Outside any known module' in r['Function'])
        report['observations']['hotspots']=dict(guest_symbols=len(sampled),guest_sampled_seconds=guest,all_sampled_seconds=total,
            unknown_sampled_seconds=unknown,guest_fraction=guest/total,unknown_fraction=unknown/total)
        names=wasm_function_names(previous/'profile.wasm')
        report['guest_name_labels']=[]
        for row in sampled:
            match=re.fullmatch(r'uwvm_m_[0-9a-fA-F]+_(raw_)?func_([0-9]+)',row['Function'])
            if match and int(match[2]) in names:
                report['guest_name_labels'].append(dict(symbol=row['Function'],wasm_function_index=int(match[2]),
                    raw_abi_wrapper=bool(match[1]),wasm_name_label=names[int(match[2])],sampled_seconds=float(row['CPU Time']),
                    mapping='exact generated symbol index to immutable Wasm name section; display label, not original source line'))
        targets=sorted(sampled,key=lambda r:-float(r['CPU Time']))[:6];report['guest_instructions']=[]
        for index,row in enumerate(targets):
            function=row['Function'];label='guest-'+str(index)
            dest=out/(label+'.csv')
            run(label,[vtune,'-report','hw-events','-result-dir',str(out/'vtune-wasm-uarch-exploration'),
                '-source-object','function='+function,'-group-by','address','-format','csv','-report-output',str(dest)])
            with dest.open() as stream:instructions=list(csv.DictReader(stream,delimiter='\t'))
            instructions=[x for x in instructions if x['Address'] and not x['Assembly'].startswith('Block ')]
            assert instructions and all(x['Assembly'] for x in instructions)
            assert all(not x.get('Source Line','').isdecimal() or int(x['Source Line'])==0 for x in instructions),'Symbol-only guest acquired invented source rows'
            samples=sum(any(int(v) for k,v in x.items() if k.startswith('Hardware Event Count:') and v) for x in instructions)
            assert samples,'Selected actual guest instructions received no hardware events'
            report['guest_instructions'].append(dict(function=function,instructions=len(instructions),sampled_instructions=samples,
                report=str(dest),sha256=sha(dest),original_source_mapping=False));save()
        report['collection_files']={str(path.relative_to(out)):sha(path) for path in out.rglob('*') if path.is_file() and ('data.0' in path.parts or path.suffix=='.jit')}
        assert sha(vm)==report['runtime_sha256']
        assert all(sha(path)==digest for path,digest in report['tools'].items())
        for path,digest in prepared['inputs'].items():assert sha(path)==digest
        report['complete']=True
    finally:
        stop.set();thread.join();gate();report['events_after']=(GROUP/'memory.events').read_text()
        (out/'processes-after.txt').write_text(subprocess.check_output(['ps','-eo','pid,psr,pcpu,rss,comm,args'],text=True));save()
if __name__=='__main__':main()
