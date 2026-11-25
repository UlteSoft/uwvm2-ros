#!/usr/bin/env python3
"""Actual model Wasm FP arithmetic and software MTE host carrier boundaries.

FP guests execute real inference and cancellation, with dynamic arithmetic
immediately after checked host returns. MTE guests independently test all seven
request/result/length ranges, normalized physical alias rejection, and matching
tags. MTE request handles are deliberately invalid so no native job is issued;
actual successful jobs are qualified separately by run_scene_models.py.
"""
import argparse,json,os,resource,signal,subprocess,time
from pathlib import Path
import run_scene_models as models
from run_scene_vm import ROOT,GATE,sha,verify_gate,policy
from run_scene_buffers import BufferGuest,u64
from run_scene_fp import FPGuest
from run_scene_memtag import TaggedGuest,add_private_instructions

class ModelFP(FPGuest,BufferGuest):
    def __init__(self,*args,**kwargs):
        super().__init__(*args,**kwargs)
        self.data=[row.replace('.const 8192)', '.const 49152)') for row in self.data]
    def check(self,*args,**kwargs):
        super().check(*args,**kwargs)
        # Keep the arithmetic operands outside the generated canonical records.
        for old,new in [(8192+i*4,49152+i*4) for i in range(9)]+[(8256+i*8,49216+i*8) for i in range(9)]:
            self.checks[-1]=self.checks[-1].replace(f'.const {old} ',f'.const {new} ')

class ModelTagged(TaggedGuest):
    def raw(self,name,cap,*,expected=9,request_tag=None,output_tag=None,length_tag=None,length=1024):
        self.write(128,0);self.write(136,0)
        self.check(f'{cap} i32.const {self.memory} i64.const {self.pointer(128,request_tag)} i64.const 8 '
            f'i64.const {self.pointer(512,output_tag)} i64.const 512 i64.const {self.pointer(length,length_tag)} call ${name}',expected)

def guest(width,memory,scenario):
    if scenario=='fp':
        original=models.BufferGuest
        try:
            models.BufferGuest=ModelFP;g=models.successful_guest(width,memory)
            g.raw('request.release','i64.const 1',u64(0),expected=9)
            return g
        finally:models.BufferGuest=original
    g=ModelTagged(models.OPERATIONS,width,memory)
    g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
    g.check('call $tag.status'+(' i32.wrap_i64' if width==64 else ''),2)
    if scenario.startswith('mismatch-'):
        _,index,slot=scenario.split('-');g.raw(models.MODEL[int(index)],'i64.const 1',expected=22,**{slot+'_tag':5});return g
    for name in models.MODEL:
        if scenario=='aliases':
            for where in [128,512]:
                g.write(512,0x123456789abcdef);g.write(1024,0x3456789)
                g.raw(name,'i64.const 1',expected=22,length=where,length_tag=0)
                g.equal(g.word(512),0x123456789abcdef);g.equal(g.word(1024),0x3456789)
            # Positive diagnostic control: a mismatched but disjoint length
            # range reaches authority refusal, rather than alias refusal.
            g.raw(name,'i64.const 1',length_tag=0)
        else:g.raw(name,'i64.const 1')
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',required=True,type=Path);p.add_argument('--source-root',required=True,type=Path)
    p.add_argument('--images',required=True,type=Path);p.add_argument('--output',required=True,type=Path);args=p.parse_args();verify_gate()
    assert os.sched_getaffinity(0).issubset({12,13,14,15});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    runtime=args.runtime.resolve();source=args.source_root.resolve();images=args.images.resolve()
    files=[Path(__file__).resolve(),Path(models.__file__).resolve(),*[Path(__file__).with_name(n) for n in ['run_scene_vm.py','run_scene_buffers.py','run_scene_fp.py','run_scene_memtag.py']],runtime,
           source/'source-manifest.json',runtime.parent/'full-build-results.json',*[images/n for n,_,_ in models.IMAGES],images/'bwrap']
    pins=lambda:{str(path):sha(path) for path in files};report=dict(complete=False,scope=__doc__,inputs=pins(),cgroup=str(GATE),inode=GATE.stat().st_ino,
        memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    scenarios=['fp','valid','aliases']+['mismatch-'+str(index)+'-'+slot for index in range(7) for slot in ['request','output','length']]
    try:
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for scenario in scenarios:
                g=guest(width,memory,scenario);name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                r=subprocess.run(cmd,capture_output=True,text=True,timeout=30);assert not r.returncode,r.stderr
                if scenario!='fp':wasm.write_bytes(add_private_instructions(wasm.read_bytes(),len(g.checks),memory))
                report['guests'].append(dict(name=name,compile=cmd,sha256=sha(wasm),wat_sha256=sha(wat),assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0)))
                flags=models.configured(images) if scenario=='fp' else policy(models.OPERATIONS)
                for engine in ['int','aot']:
                    verify_gate();run_name=name+'-'+engine;cmd=[str(runtime),'--wasm-set-main-module-name','main','--runtime-'+engine,'--runtime-compile-threads','1',
                        '--runtime-llvm-jit-cache-path','disable','--log-color','disable',*flags]
                    if scenario!='fp':cmd+=['--wasm-policy-global', 'runtime.memtag-backend','software','--runtime-memtag-mode','sync']
                    if scenario=='aliases':cmd+=['--runtime-memtag-action','silent-log','err']
                    cmd+=['--run',str(wasm)];start=time.monotonic();r=subprocess.run(cmd,capture_output=True,timeout=120)
                    (out/(run_name+'.stdout')).write_bytes(r.stdout);(out/(run_name+'.stderr')).write_bytes(r.stderr)
                    crash=scenario.startswith('mismatch-');expected=-signal.SIGILL if crash else 0;errors=r.stderr.decode(errors='replace');passed=r.returncode==expected
                    if crash:
                        slot={'request':128,'output':512,'length':1024}[scenario.split('-')[2]]
                        passed=passed and all(t in errors for t in ['[crash] memory-protection','Exit status: 122',f'Memory index: {memory}',
                            f'Pointer: {g.pointer(slot,5)}','Pointer tag: 5','Allocation tag: 3'])
                    violations=[]
                    if scenario=='aliases':
                        violations=[json.loads(line) for line in errors.splitlines() if line.startswith('{"schema":"uwvm.memtag.violation.')]
                        passed=passed and len(violations)>=21 and all(v['pointer_tag']==0 and v['allocation_tag']==3 and v['memory']==memory and v['pointer'] in [128,512,1024] for v in violations)
                    report['cases'].append(dict(name=run_name,argv=cmd,exit=r.returncode,expected=expected,passed=passed,seconds=time.monotonic()-start,violations=violations));save()
                    assert passed,(run_name,r.returncode,errors[-5000:]);print('PASS '+run_name,flush=True)
        assert pins()==report['inputs'];report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
