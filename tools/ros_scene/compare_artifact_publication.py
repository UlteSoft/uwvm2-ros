#!/usr/bin/env python3
"""Pair actual staged/publish/native XML timings on one frozen qualified VM.

Both VM paths serialize the same unmodified TinyXML2 documents and read back
identical bytes. Native uses the established finite-memory copy baseline. This
compares real carriers, not prototype call-count projections. Preparation and
correctness receipts are required; no native ROS or industrial certification is
implied. All processes inherit the existing 64 GiB zero-swap test cgroup.
"""
import argparse,hashlib,json,os,re,statistics,struct,subprocess,time
from pathlib import Path
from performance_build_policy import fixture_policy, vm_policy
ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
def native_code(path):
    data=Path(path).read_bytes();assert data[:6]==b'\x7fELF\x02\x01','native reference must be ELF64 little endian'
    headers=struct.unpack_from('<Q',data,40)[0];size,count,names=struct.unpack_from('<HHH',data,58)
    sections=[struct.unpack_from('<IIQQQQIIQQ',data,headers+i*size) for i in range(count)]
    names_section=sections[names];labels=data[names_section[4]:names_section[4]+names_section[5]];result={}
    for section in sections:
        name=labels[section[0]:labels.index(0,section[0])].decode()
        if name in ('.text','.rodata'):result[name]=hashlib.sha256(data[section[4]:section[4]+section[5]]).hexdigest()
    assert len(result)==2;return result
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0)=={8},'formal paired timing requires exclusive CPU8 affinity; keep CPU9 sibling idle'
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--staged',required=True,type=Path);p.add_argument('--publication',required=True,type=Path)
    p.add_argument('--output',required=True,type=Path);p.add_argument('--rounds',type=int,default=3);args=p.parse_args();guard();assert 3<=args.rounds<=9
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    inputs={};commands={};fixtures={};compile_audits={};runtime=None;native_digest=None;expected=None
    for label,directory in [('staged',args.staged.resolve()),('publication',args.publication.resolve())]:
        prep=directory/'prepare.json';base=directory/'baseline.json';pr=json.loads(prep.read_text());br=json.loads(base.read_text())
        compile_audits[label]=fixture_policy(pr)
        assert pr['complete'] and br['complete'];inputs[str(prep)]=sha(prep);inputs[str(base)]=sha(base)
        for name,digest in {**pr['inputs'],**pr['toolchain']}.items():assert sha(name)==digest;inputs[name]=digest
        correctness=Path(pr['prepared'])/'results.json';cr=json.loads(correctness.read_text());assert cr['complete'];inputs[str(correctness)]=sha(correctness)
        description=pr['fixtures']['baseline'];fixtures[label]=description
        assert br['runtime_sha256']==cr['runtime']['sha256']
        if runtime is None:runtime=cr['runtime']
        else:assert runtime['path']==cr['runtime']['path'] and runtime['sha256']==cr['runtime']['sha256']
        cmd=next(row['argv'] for row in br['commands'] if row['name']=='wasm-0');commands[label]=cmd
        native_cmd=next(row['argv'] for row in br['commands'] if row['name']=='native-0')
        # Separate debug build paths may change DWARF/build-id. Require identical
        # actual native machine text and constants before sharing one control.
        current=native_code(native_cmd[-1])
        if native_digest is None:native_digest=current;commands['native']=native_cmd
        else:assert native_digest==current,'native machine text/constants differ; qualify separate native controls instead'
        observed=cr['outputs']
        if expected is None:expected=observed
        else:assert expected==observed,'XML output corpus differs'
    assert {k:fixtures['staged'][k] for k in ['iterations','batches','chunk']}=={k:fixtures['publication'][k] for k in ['iterations','batches','chunk']}
    inputs[runtime['path']]=runtime['sha256'];assert sha(runtime['path'])==runtime['sha256']
    build=Path(runtime['path']).parent/'full-build-results.json';build_data=json.loads(build.read_text());assert build_data['complete'] and build_data['full_source_unchanged']
    compile_audits['vm']=vm_policy(runtime['path'])
    inputs[str(Path(__file__).with_name('performance_build_policy.py'))]=sha(Path(__file__).with_name('performance_build_policy.py'))
    for row in compile_audits['vm'].values():inputs[row['argv_file']]=row['argv_sha256']
    inputs[str(build)]=sha(build);manifest=Path(build_data['full_source_manifest']);inputs[str(manifest)]=sha(manifest)
    assert inputs[str(manifest)]==build_data['full_source_manifest_sha256']
    for name,digest in json.loads(manifest.read_text()).items():assert sha(Path(build_data['source_root'])/name)==digest
    report=dict(scope=__doc__,complete=False,cgroup=str(GATE),cgroup_inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,
        events_before=(GATE/'memory.events').read_text(),cpu=8,thread_siblings=Path('/sys/devices/system/cpu/cpu8/topology/thread_siblings_list').read_text().strip(),
        native_machine_sections=native_digest,compile_policy=compile_audits,inputs=inputs,runner_sha256=sha(__file__),observations=[],fixtures=fixtures,runtime=runtime,native_parity_qualified=False)
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    save();checksum=None
    try:
        for round in range(args.rounds):
            order=[['native','staged','publication'],['publication','native','staged'],['staged','publication','native']][round%3]
            for label in order:
                guard();cmd=commands[label];start=time.monotonic();r=subprocess.run(cmd,cwd=ROOT,capture_output=True,timeout=240)
                name=f'{label}-{round}';(out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
                row=dict(provider=label,round=round,argv=cmd,exit=r.returncode,seconds=time.monotonic()-start,passed=False);report['observations'].append(row);save()
                assert r.returncode==0,(name,r.stderr.decode(errors='replace')[-5000:])
                body=r.stdout.decode();header=re.search(r'iterations=(\d+) batches=(\d+) outputs=(\d+) chunk=(\d+) expected=(\d+) checksum=(\d+)',body);assert header
                values=tuple(map(int,header.groups()));config=fixtures['staged'];assert values[:4]==(config['iterations'],config['batches'],14,config['chunk'])
                if checksum is None:checksum=values[4:]
                else:assert checksum==values[4:]
                durations=list(map(int,re.findall(r'elapsed_ns=(\d+)',body)));assert len(durations)==config['batches'] and min(durations)>0
                row.update(samples_ns=durations,batch_p50_ns_per_cycle=statistics.median(durations)/config['iterations'],passed=True);save();print('PASS',name,flush=True)
        medians={label:statistics.median(row['batch_p50_ns_per_cycle'] for row in report['observations'] if row['provider']==label) for label in commands}
        report['comparison']=dict(median_of_round_p50_ns=medians,publication_staged_ratio=medians['publication']/medians['staged'],
            publication_native_ratio=medians['publication']/medians['native'],staged_native_ratio=medians['staged']/medians['native'])
        assert all(sha(name)==digest for name,digest in inputs.items());report['inputs_unchanged']=True;report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
