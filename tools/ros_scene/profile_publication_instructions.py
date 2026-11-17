#!/usr/bin/env python3
"""Join actual VTune instruction-level hardware events to exact frozen DWARF.

Microarchitecture exploration supplies sampled/scaled event estimates at code
addresses. VTune's assembly is checked against the frozen VM function range;
addr2line independently supplies complete inline source frames. Only files
whose bytes match the build manifest receive source text. Samples can skid:
this evidence identifies sampled instruction locations, not exact individual
instruction latency. Guest JIT source remains explicitly unqualified.
"""
import argparse,csv,hashlib,json,os,re,shutil,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest()
def guard():
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset(set(range(8,12)))
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--profile',type=Path,required=True);p.add_argument('--runtime',type=Path,required=True)
    p.add_argument('--assembly',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--vtune',default='/opt/intel/oneapi/vtune/latest/bin64/vtune');args=p.parse_args();guard()
    profile=args.profile.resolve();runtime=args.runtime.resolve();assembly=args.assembly.resolve();out=args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    pr=json.loads((profile/'profile.json').read_text());ar=json.loads((assembly/'results.json').read_text());assert pr['complete'] and ar['complete']
    assert pr['runtime_sha256']==ar['runtime_sha256']==sha(runtime)
    build=json.loads((runtime.parent/'full-build-results.json').read_text());manifest=Path(build['full_source_manifest']);frozen=Path(build['source_root'])
    assert sha(manifest)==build['full_source_manifest_sha256'];source_pins={str(frozen/name):digest for name,digest in json.loads(manifest.read_text()).items()}
    joined=profile/'source-lines-wasm-hotspots.json';hotspots=json.loads(joined.read_text())
    candidates={row['Function'] for row in hotspots}
    targets=[('publication_registry','uwvm2::imported::ros::scene::outputs::registry::publish'),
        ('output_copy_read','uwvm2::imported::ros::scene::outputs::registry::copy_read'),
        ('output_broker','uwvm2::imported::ros::scene::broker::invoke_output'),
        ('authority_indexed','uwvm2::imported::ros::scene::authority::admit_checked<(bool)1>'),
        ('preload_descriptor','uwvm2::runtime::lib::(anonymous namespace)::try_build_preload_memory_descriptor')]
    assert all(name in candidates for _,name in targets)
    resolver=shutil.which('llvm-addr2line') or shutil.which('addr2line');assert resolver
    report=dict(scope=__doc__,complete=False,profile_sha256=sha(profile/'profile.json'),assembly_sha256=sha(assembly/'results.json'),
        runtime_sha256=sha(runtime),full_source_manifest_sha256=sha(manifest),runner_sha256=sha(__file__),commands=[],functions=[],
        guest_jit_source_mapping_qualified=False,cgroup=str(GATE),cgroup_inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,
        events_before=(GATE/'memory.events').read_text(),tools={str(Path(args.vtune).resolve()):sha(Path(args.vtune).resolve()),str(Path(resolver).resolve()):sha(Path(resolver).resolve())})
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(label,argv):
        guard();start=time.monotonic();r=subprocess.run(argv,capture_output=True,text=True,timeout=120)
        (out/(label+'.stdout')).write_text(r.stdout);(out/(label+'.stderr')).write_text(r.stderr)
        report['commands'].append(dict(label=label,argv=argv,exit=r.returncode,seconds=time.monotonic()-start));save();assert r.returncode==0,(label,r.stderr[-5000:]);return r.stdout
    save();source_cache={}
    try:
        for key,function in targets:
            meta=ar['assembly'][key];start=int(meta['address'],16);end=start+meta['size']
            files={}
            for kind in ['hw-events','hotspots']:
                dest=out/(key+'-'+kind+'.csv');files[kind]=dest
                run(key+'-'+kind,[args.vtune,'-report',kind,'-result-dir',str(profile/'vtune-wasm-uarch-exploration'),
                    '-source-object','function='+function,'-group-by','address','-format','csv','-report-output',str(dest),'-source-search-dir',str(frozen)])
            with files['hw-events'].open() as f:rows=list(csv.DictReader(f,delimiter='\t'))
            instructions=[]
            for row in rows:
                if not row['Address'] or row['Assembly'].startswith('Block '):continue
                address=int(row['Address'],16);assert start<=address<end,(key,address,start,end)
                events={name.removeprefix('Hardware Event Count:'):int(value) for name,value in row.items() if name.startswith('Hardware Event Count:') and value and int(value)}
                instructions.append(dict(address=row['Address'],assembly=row['Assembly'],events=events))
            assert instructions and any(row['events'] for row in instructions)
            addresses=[row['address'] for row in instructions]
            resolved=run(key+'-dwarf',[resolver,'-a','-f','-C','-i','-e',str(runtime),*addresses]);frames={};current=None;pending=[]
            def finish():
                if current is None:return
                assert len(pending)%2==0,(key,current,pending)
                output=[]
                for i in range(0,len(pending),2):
                    location=pending[i+1];match=re.match(r'^(.*?):([0-9]+)(?::[0-9]+)?(?:\s.*)?$',location)
                    frame=dict(function=pending[i],location=location)
                    if match and match[1] in source_pins and int(match[2])>0:
                        path=match[1];line=int(match[2]);digest=source_pins[path]
                        if path not in source_cache:
                            assert sha(path)==digest;source_cache[path]=Path(path).read_text(errors='replace').splitlines()
                        if line<=len(source_cache[path]):frame.update(preserved_source=path,line=line,source_sha256=digest,source_text=source_cache[path][line-1])
                    output.append(frame)
                frames[current]=output
            for line in resolved.splitlines():
                if re.fullmatch(r'0x[0-9a-fA-F]+',line):finish();current=int(line,16);pending=[]
                else:pending.append(line)
            finish()
            for row in instructions:row['inline_frames']=frames[int(row['address'],16)]
            location=out/(key+'-instructions.json');location.write_text(json.dumps(instructions,indent=2)+'\n')
            examples=sorted([row for row in instructions if row['events'] and any('preserved_source' in frame for frame in row['inline_frames'])],
                key=lambda row:-(row['events'].get('CPU_CLK_UNHALTED.THREAD',0)+row['events'].get('CPU_CLK_UNHALTED.THREAD_P',0)))[:8]
            report['functions'].append(dict(key=key,function=function,address=meta['address'],size=meta['size'],instructions=len(instructions),
                sampled_instructions=sum(bool(row['events']) for row in instructions),source_qualified_instructions=sum(any('preserved_source' in f for f in row['inline_frames']) for row in instructions),
                report_sha256={kind:sha(path) for kind,path in files.items()},joined=str(location),joined_sha256=sha(location),examples=examples))
            save();print('PASS instruction events + frozen inline source:',key,flush=True)
        assert sha(runtime)==report['runtime_sha256'] and sha(manifest)==report['full_source_manifest_sha256']
        report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
