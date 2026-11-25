#!/usr/bin/env python3
"""Qualify actual frozen symbol-only VM ownership and pinned upstream XML.

The test collector links through the official SDK and synchronously hashes real
finalized code on both LOAD and UNLOAD. It is an explicit trusted test fixture,
never the collector used for official VTune reports. Source rows must be absent.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time

ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
    with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--runtime',type=Path,required=True);p.add_argument('--xml',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    vm=a.runtime.resolve();build=json.loads((vm.parent/'full-build-results.json').read_text())
    assert build['complete'] and build['binary_sha256']==sha(vm) and build['intel_jit_qualification']
    source=Path(build['source_root']);xml=a.xml.resolve();previous=json.loads((xml/'results.json').read_text());assert previous['complete']
    for name,digest in previous['input_sha256'].items():assert sha(xml/name)==digest
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    sdk=Path(build['intel_jit_qualification']['sdk'])
    inputs=[vm,vm.parent/'full-build-results.json',xml/'results.json',ROOT/'test/0084.ros-runtime/intel_profile_vm_owner.cpp',
        ROOT/'test/0084.ros-runtime/intel_profile_vm_collector.c',Path(__file__)]
    report=dict(complete=False,scope=__doc__,inputs={str(x):sha(x) for x in inputs},commands=[],observations={},
        cgroup=str(GROUP),inode=GROUP.stat().st_ino,boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
        memory_max=68719476736,swap_max=0,events_before=(GROUP/'memory.events').read_text(),
        production_integration=False,guest_source_mapping=False,performance_parity=False)
    def gate():
        assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
        assert GROUP.stat().st_ino==report['inode'] and (GROUP/'memory.max').read_text().strip()=='68719476736'
        assert (GROUP/'memory.swap.max').read_text().strip()=='0'
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    clean=dict(os.environ,DEBUGINFOD_URLS='');clean.pop('INTEL_JIT_PROFILER64',None)
    clean.pop('UWVM_TEST_JIT_EVENTS',None);clean.pop('UWVM_TEST_JIT_REJECT_LOAD',None)
    def run(name,argv,expected=0,env=None,timeout=900):
        gate();start=time.monotonic();argv=list(map(str,argv))
        result=subprocess.run(argv,cwd=ROOT,capture_output=True,env=clean if env is None else env,timeout=timeout)
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        report['commands'].append(dict(name=name,argv=argv,exit=result.returncode,seconds=time.monotonic()-start));save()
        assert (result.returncode==expected if expected is not None else result.returncode!=0),(name,result.stderr[-4000:])
        print('PASS',name,flush=True);return result
    def events(name,expect_unload):
        path=out/(name+'.events');rows=[x.split() for x in path.read_text().splitlines()];loads={};unloads={}
        for r in rows:
            assert r[0] in ['LOAD','UNLOAD'],r
            target=loads if r[0]=='LOAD' else unloads;assert r[1] not in target
            target[r[1]]=r
            if r[0]=='LOAD':assert r[5]=='0',r
        assert loads and any(r[6].startswith('uwvm_m_') for r in loads.values())
        if expect_unload:
            assert loads.keys()==unloads.keys(),(loads.keys(),unloads.keys())
            for key in loads:assert loads[key][2:5]==unloads[key][2:5],'Retirement bytes changed or unreadable'
        else:assert not unloads,'Retained executable arena must not announce UNLOAD'
        report['observations'][name]=dict(loads=len(loads),unloads=len(unloads),original_source_rows=0,event_sha256=sha(path));save()
        return loads
    def collector_env(name,reject=False):
        env=dict(clean,INTEL_JIT_PROFILER64=str(out/'collector.so'),UWVM_TEST_JIT_EVENTS=str(out/(name+'.events')))
        if reject:env['UWVM_TEST_JIT_REJECT_LOAD']='1'
        return env
    try:
        gate()
        run('collector-build',['/usr/bin/clang-22','-std=c17','-O3','-g','-Wall','-Wextra','-Werror','-shared','-fPIC',
            '-I'+str(sdk/'include'),ROOT/'test/0084.ros-runtime/intel_profile_vm_collector.c','-o',out/'collector.so'])
        commands={x['name']:x['argv'] for x in previous['commands']}
        def xml_command(name):
            argv=commands[name].copy();argv[0]=str(vm)
            argv[1:1]=['--wasm-policy-global', 'runtime.llvm.policy','max','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable']
            return argv
        good=xml_command('vm-aot');denied=xml_command('vm-denied-aot');baseline=(xml/'native.stdout').read_bytes()
        assert run('xml-without-collector',good).stdout==baseline
        assert run('xml-live-collector',good,env=collector_env('xml-live-collector')).stdout==baseline
        # The ordinary CLI explicitly drains/resets at run.h's completion
        # boundary. Terminal retained-code behavior uses the embedding below.
        events('xml-live-collector',True)
        assert run('xml-zero-authority',denied).stdout==b'checks:56\n'
        for label,key,value in [('worker','--runtime-compile-threads','1'),('policy','runtime.llvm.policy','balanced'),
                               ('cache','--runtime-llvm-jit-cache-path','default')]:
            argv=good.copy();argv[argv.index(key)+1]=value
            r=run('reject-'+label,argv,78);assert b'unsupported runtime' in r.stderr
        argv=good.copy();argv[argv.index('--runtime-aot')]='--runtime-int'
        assert b'unsupported runtime' in run('reject-interpreter',argv,78).stderr
        rejected=run('xml-rejected-registration',good,None,collector_env('xml-rejected-registration',True))
        assert not rejected.stdout and b'finalized code observation failed' in rejected.stderr
        events('xml-rejected-registration',True)
        (out/'owner.wat').write_text('(module (func (export "work") (param i32) (result i32) local.get 0 i32.const 7 i32.add))\n')
        run('owner-wasm',['wat2wasm',out/'owner.wat','-o',out/'owner.wasm'])
        argv=json.loads((vm.parent/'main.default.cpp.o.argv.json').read_text())
        argv=[str(out/'owner.o') if x==str(vm.parent/'main.default.cpp.o') else
            str(ROOT/'test/0084.ros-runtime/intel_profile_vm_owner.cpp') if x.endswith('/src/uwvm2/uwvm/main.default.cpp') else x for x in argv]
        assert str(out/'owner.o') in argv
        run('owner-build',argv)
        link=json.loads((vm.parent/'link.argv.json').read_text())
        link=[str(out/'owner.o') if x==str(vm.parent/'main.default.cpp.o') else str(out/'owner') if x==str(vm) else x for x in link]
        run('owner-link',link)
        for mode in ['reset','retain']:
            run('owner-'+mode,[out/'owner',out/'owner.wasm',mode],env=collector_env('owner-'+mode))
            loads=events('owner-'+mode,mode=='reset')
            if mode=='reset':assert len(loads)>=4,'Two real generations need separate identities'
        report['outputs']={str(out/x):sha(out/x) for x in ['collector.so','owner','owner.wasm']}
        assert all(sha(x)==digest for x,digest in report['inputs'].items())
        report['complete']=True
    finally:gate();report['events_after']=(GROUP/'memory.events').read_text();save()
if __name__=='__main__':main()
