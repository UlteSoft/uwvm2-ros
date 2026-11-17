#!/usr/bin/env python3
"""Tie frozen output Hotspots samples to actual host machine-code call sites.

No live process, VM source or policy is changed. Unknown JIT PCs remain unknown.
The inspected executable must exactly match the completed profile receipt.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

GATE=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
ROOT=Path(__file__).resolve().parents[2]
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--profile',type=Path,required=True)
    parser.add_argument('--runtime',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--require-scene-fp-guard',action='store_true')
    parser.add_argument('--require-artifact-fp-guard',action='store_true',help='Require actual open/read/describe dispatch thunks with native control save and restore around each body call')
    parser.add_argument('--publication',action='store_true',help='Also inspect the actual complete-publication carrier, scratch and commit path')
    args=parser.parse_args()
    assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip()=='68719476736' and (GATE/'memory.swap.max').read_text().strip()=='0'
    assert os.sched_getaffinity(0).issubset(set(range(8,16)))
    profile=args.profile.resolve();runtime=args.runtime.resolve();out=args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    receipt=json.loads((profile/'profile.json').read_text());assert receipt['complete'] and receipt['runtime_sha256']==sha(runtime)
    attribution=json.loads((profile/'source-line-attribution.json').read_text());assert attribution['complete'] and attribution['profile_sha256']==sha(profile/'profile.json')
    input_rows=profile/'source-lines-wasm-hotspots.json'
    assert sha(input_rows)==next(r['joined_sha256'] for r in attribution['reports'] if r['provider']=='wasm' and r['analysis']=='hotspots')
    record=dict(complete=False,scope=__doc__,runtime_sha256=sha(runtime),profile_sha256=sha(profile/'profile.json'),
        attribution_sha256=sha(profile/'source-line-attribution.json'),runner_sha256=sha(__file__),commands=[],assembly={},hotspot_functions=[],
        guest_jit_assembly_qualified=False,cgroup=str(GATE),memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text())
    def run(name,argv,input=None):
        r=subprocess.run(argv,input=input,capture_output=True,text=True,timeout=90)
        (out/(name+'.stdout')).write_text(r.stdout);(out/(name+'.stderr')).write_text(r.stderr)
        record['commands'].append(dict(name=name,argv=argv,exit=r.returncode));assert r.returncode==0,(name,r.stderr)
        return r.stdout
    raw=run('symbols',['nm','-S','--defined-only',str(runtime)])
    dem=run('symbols-demangled',['c++filt'],raw)
    specs={'scene_append':'uwvm2::uwvm::imported::scene::invoke_output<(uwvm2::imported::ros::output_contract::operation)1>(',
        'scene_read':'uwvm2::uwvm::imported::scene::invoke_artifact<(uwvm2::imported::ros::artifact_contract::operation)1>(',
        'preload_descriptor':'try_build_preload_memory_descriptor(',
        'preload_resolve':'resolve_preload_memory(',
        'authority':'scene::authority::admit_checked<false>',
        'deployment_select':'scene::deployment::select_exposed(',
        'fp_boundary':'invoke_host_with_llvm_wasm_fp_control_policy<'}
    # The original accepted output build predates indexed artifact admission
    # and copy_read. Keep its reproduction usable; the guarded safe build must
    # additionally expose the exact optimized registry and broker call sites.
    if args.require_scene_fp_guard:specs.update({
        'authority_indexed':'scene::authority::admit_checked<true>',
        'output_preflight':'scene::broker::preflight_output(',
        'output_broker':'scene::broker::invoke_output(',
        'artifact_broker':'scene::broker::invoke_artifact(',
        'output_bound':'scene::outputs::registry::request_size(',
        'output_mutate':'scene::outputs::registry::mutate(',
        'output_copy_read':'scene::outputs::registry::copy_read('})
    if args.publication:specs.update({
        'scene_publish':'uwvm2::uwvm::imported::scene::invoke_output<(uwvm2::imported::ros::output_contract::operation)7>(',
        'publication_registry':'scene::outputs::registry::publish(',
        'publication_scratch':'scene::outputs::registry::reserve_publication('})
    if args.require_artifact_fp_guard:specs.update({
        'scene_open':'uwvm2::uwvm::imported::scene::invoke_artifact<(uwvm2::imported::ros::artifact_contract::operation)0>(',
        'scene_describe':'uwvm2::uwvm::imported::scene::invoke_artifact<(uwvm2::imported::ros::artifact_contract::operation)2>('})
    for name,fragment in specs.items():
        rows=[(a,b) for a,b in zip(raw.splitlines(),dem.splitlines()) if fragment in b and a.split()[2] in ['t','T','W'] and
            'std::__1::__function' not in b and 'operator()' not in b and '::rollback::' not in b and ('::{lambda' not in b or name=='fp_boundary')]
        assert len(rows)==1,(name,len(rows))
        a,b=rows[0];address,size,kind,symbol=a.split(None,3)
        text=run(name,['objdump','-d','-C','--no-show-raw-insn','--start-address=0x'+address,
            '--stop-address='+hex(int(address,16)+int(size,16)),str(runtime)])
        record['assembly'][name]=dict(symbol=b,address=address,size=int(size,16),sha256=sha(out/(name+'.stdout')),
            call_sites=[line.strip() for line in text.splitlines() if re.search(r'\b(call|syscall|lock|fnstenv|fldenv|stmxcsr|ldmxcsr)\b',line)])
    # The scene wrapper is commonly inlined into the tuple's dispatch thunk,
    # outside invoke_output itself. Use the real emitted scene tuple symbols,
    # not a guessed address or the unrelated outer runtime FP boundary.
    wrapper_rows=[(a,b) for a,b in zip(raw.splitlines(),dem.splitlines()) if 'artifact_append_name' in a and
        a.split()[2] in ['t','T','W'] and ('call_func_index_impl' in a or '4callE' in a)]
    assert wrapper_rows,'No actual scene wrapper or dispatch thunk found'
    record['scene_fp_guard_sites']=[];record['append_wrapper_fp_guard_sites']=[]
    append_address=format(int(record['assembly']['scene_append']['address'],16),'x')
    for n,(a,b) in enumerate(wrapper_rows):
        address,size,kind,symbol=a.split(None,3);name='scene-wrapper-'+str(n)
        text=run(name,['objdump','-d','-C','--no-show-raw-insn','--start-address=0x'+address,
            '--stop-address='+hex(int(address,16)+int(size,16)),str(runtime)])
        sites=[line.strip() for line in text.splitlines() if re.search(r'\b(fnstcw|fnstsw|stmxcsr|fldcw|ldmxcsr)\b',line)]
        routes_append=bool(re.search(r'\bcall\s+0*'+append_address+r'\b',text))
        record['assembly'][name]=dict(symbol=b,address=address,size=int(size,16),sha256=sha(out/(name+'.stdout')),
            fp_control_sites=sites,calls_append_body=routes_append)
        record['scene_fp_guard_sites']+=sites
        if routes_append:record['append_wrapper_fp_guard_sites']+=sites
    observed={word for line in record['scene_fp_guard_sites'] for word in ['fnstcw','fnstsw','stmxcsr','fldcw','ldmxcsr'] if re.search(r'\b'+word+r'\b',line)}
    record['scene_fp_control_instructions']=sorted(observed)
    if args.require_scene_fp_guard:
        append_observed={word for line in record['append_wrapper_fp_guard_sites'] for word in ['fnstcw','fnstsw','stmxcsr','fldcw','ldmxcsr'] if re.search(r'\b'+word+r'\b',line)}
        assert append_observed=={'fnstcw','fnstsw','stmxcsr','fldcw','ldmxcsr'},append_observed
    if args.require_artifact_fp_guard:
        record['artifact_control_guard_routes']={}
        for operation in ['open','read','describe']:
            address=format(int(record['assembly']['scene_'+operation]['address'],16),'x');routes=[]
            for n in range(len(wrapper_rows)):
                name='scene-wrapper-'+str(n);lines=(out/(name+'.stdout')).read_text().splitlines()
                for index,line in enumerate(lines):
                    if not re.search(r'\bcall\s+0*'+address+r'\b',line):continue
                    before='\n'.join(lines[:index]);after='\n'.join(lines[index+1:])
                    saved=all(re.search(r'\b'+instruction+r'\b',before) for instruction in ['fnstcw','stmxcsr'])
                    restored=all(re.search(r'\b'+instruction+r'\b',after) for instruction in ['fnstsw','fldcw','ldmxcsr'])
                    if saved and restored:routes.append(dict(wrapper=name,call=line.strip(),assembly_sha256=record['assembly'][name]['sha256']))
            assert routes,('No actual artifact control guard route',operation)
            record['artifact_control_guard_routes'][operation]=routes
    for name,path,symbol in [('libm-fegetenv','/lib/x86_64-linux-gnu/libm.so.6','fegetenv'),
        ('libm-fesetenv','/lib/x86_64-linux-gnu/libm.so.6','fesetenv'),('libc-getpid','/lib/x86_64-linux-gnu/libc.so.6','getpid')]:
        dynamic=run(name+'-symbols',['nm','-D','-S','--defined-only',path])
        rows=[row.split() for row in dynamic.splitlines() if row.split()[-1].split('@')[0]==symbol];assert len(rows)==1
        address,size=rows[0][:2]
        run(name,['objdump','-d','--no-show-raw-insn','--start-address=0x'+address,
            '--stop-address='+hex(int(address,16)+int(size,16)),path])
        record['assembly'][name]=dict(module=path,module_sha256=sha(path),sha256=sha(out/(name+'.stdout')),
            scope='current installed system library, separately hashed; not the frozen VM executable')
    totals={}
    for row in json.loads(input_rows.read_text()):
        function=row['Function (Full)'];totals[function]=totals.get(function,0)+float(row['CPU Time'])
    total=sum(totals.values());record['sampled_cpu_seconds']=total
    record['hotspot_functions']=[dict(function=name,cpu_seconds=value,percent=100*value/total) for name,value in sorted(totals.items(),key=lambda p:-p[1])]
    assert sha(runtime)==record['runtime_sha256'];record['events_after']=(GATE/'memory.events').read_text();record['complete']=True
    (out/'results.json').write_text(json.dumps(record,indent=2)+'\n')
    print(json.dumps(dict(complete=True,assembly=list(record['assembly']),hotspots=record['hotspot_functions'][:16]),indent=2))
if __name__=='__main__':main()
