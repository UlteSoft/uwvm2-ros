#!/usr/bin/env python3
"""Qualify literal private logs and compound diagnostic pages through actual VM carriers.

No console/native ROS appender or unrestricted file/socket is installed. Four
memory layouts exercise exact retries, explicit loss, finite storage, closed
logger/severity fields, source observation authority and literal zero rates.
FP arithmetic follows actual returns; software MTE checks all three carriers.
Every compiler, VM and checker inherits the existing 64 GiB zero-swap cgroup.
"""
import argparse,hashlib,json,os,resource,signal,subprocess,time
from pathlib import Path
from run_scene_vm import Guest,ROOT,GATE,policy,sha,verify_gate
from run_scene_fp import FPGuest
from run_scene_memtag import TaggedGuest,add_private_instructions
NAMES=['log.emit','diagnostic.read']
OPERATIONS=[dict(name=name,words=0,bytes=0) for name in NAMES]
ALIASES=['log','drop','rate','noobserve']
def u64(value):return (value%(1<<64)).to_bytes(8,'little')
def text(value):
    data=value.encode() if isinstance(value,str) else value
    return u64(len(data))+data
def record(sequence=1,message='literal %n and {}',logger='robot',level='info',session=19,time=None,location=None):
    data=u64(session)+u64(sequence)+text(level)+text(logger)+u64(time is not None)
    if time is not None:data+=b''.join(u64(n) for n in time)
    data+=text(message)+u64(location is not None)
    if location is not None:data+=text(location[0])+text(location[1])+u64(location[2])
    return data
def configured():
    flags=policy(OPERATIONS)
    for name,value in dict(logs=4,diagnostics=1,storage_bytes=65536,request_bytes=1024,workspace_bytes=2048,workspace_calls=2).items():
        flags+=['--ros-scene-log-diagnostic-budget','robot',name,str(value)]
    for alias in ALIASES:
        flags+=['--ros-scene-grant','robot',alias,'log',alias,'private-log','log.emit'+('' if alias=='noobserve' else ',resource.observe')]
        for name,value in dict(records=2,record_bytes=1024,storage_bytes=8192,severity_mask=31).items():flags+=['--ros-scene-log-limit','robot',alias,name,str(value)]
        flags+=['--ros-scene-log-logger','robot',alias,'robot','--ros-scene-log-loss','robot',alias,'drop-oldest' if alias=='drop' else 'reject']
    flags+=['--ros-scene-grant','robot','health','diagnostic','counters','local-diagnostic','diagnostic.read',
            '--ros-scene-diagnostic-source','robot','health','log']
    for name,value in dict(scope=37,fields=7,rows=3,page_items=2,storage_bytes=512,lifetime_ns=60000000000,capture_ns=1000000000).items():
        flags+=['--ros-scene-diagnostic-limit','robot','health',name,str(value)]
    flags+=['--ros-scene-limit','robot','rate','delivery','may-reject','--ros-scene-limit','robot','rate','calls-per-window','0']
    return flags
class LogGuest(Guest):
    def raw(self,name,cap,payload,patches=(),expected=0,capacity=None,output=512,length=2048,memory=None,size=None):
        address=self.cursor;self.cursor+=len(payload)+8
        self.data.append(f'(data (memory $work) (i{self.width}.const {address}) "'+''.join(f'\\{value:02x}' for value in payload)+'")')
        for offset,value in patches:self.write(address+offset,value)
        if isinstance(cap,int):cap='i64.const '+str(cap)
        if memory is None:memory=self.memory
        if size is None:size=len(payload)
        if capacity is None:capacity=48 if name=='log.emit' else 96
        if length=='request':length=address
        if output=='request':output=address
        self.last_request=address
        self.check(f'{cap} i32.const {memory} i64.const {address} i64.const {size} i64.const {output} i64.const {capacity} i64.const {length} call ${name}',expected)
class LogFP(FPGuest,LogGuest):
    def __init__(self,*args,**kwargs):
        super().__init__(*args,**kwargs);self.data=[row.replace('.const 8192)', '.const 49152)') for row in self.data]
    def check(self,*args,**kwargs):
        super().check(*args,**kwargs)
        for old,new in [(8192+i*4,49152+i*4) for i in range(9)]+[(8256+i*8,49216+i*8) for i in range(9)]:
            self.checks[-1]=self.checks[-1].replace(f'.const {old} ',f'.const {new} ')
def successful_guest(width,memory,fp=False):
    g=(LogFP if fp else LogGuest)(OPERATIONS,width,memory);g.check('call $abi',2);g.open('missing',13)
    caps={name:g.open(name) for name in ALIASES+['health']};cap=caps['log'];request=record()
    for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:
        g.raw('log.emit',cap,request,expected=22,**kw)
    g.raw('log.emit',cap,request,capacity=47,expected=90);g.equal(g.word(2048),48)
    for bad in [record(session=0),record(sequence=0),record(level='trace'),record(logger=b'\xff'),record(time=(0,0,0)),request+b'x']:
        g.raw('log.emit',cap,bad,expected=22)
    g.raw('log.emit',cap,record(logger='robot.child'),expected=13)
    g.raw('log.emit',cap,record(sequence=2),expected=17)
    g.raw('log.emit',caps['rate'],request,expected=11)
    g.raw('log.emit',cap,request)
    for index,value in [(1,19),(2,1),(3,1),(4,len(request)),(5,0)]:g.equal(g.word(512+8*index),value)
    scope=g.save('log-scope');g.raw('log.emit',cap,request);g.check(g.word(512)+' '+scope+' i64.ne')
    g.raw('log.emit',cap,record(message='different'),expected=17)
    large=record(sequence=2,message='x'*800,time=(99,0,-1),location=('source.cpp','run',123))
    g.raw('log.emit',cap,large,output='request');g.equal(g.word(g.last_request+16),2)
    g.raw('log.emit',cap,large);g.equal(g.word(528),2);g.equal(g.word(544),len(large))
    g.raw('log.emit',cap,record(sequence=3),expected=11)
    g.raw('log.emit',cap,record(sequence=3,message='x'*1024),expected=90)
    for sequence in [1,2,3]:
        g.raw('log.emit',caps['drop'],record(sequence=sequence));g.equal(g.word(552),1 if sequence==3 else 0)
    g.raw('log.emit',caps['drop'],record(sequence=3));g.equal(g.word(552),1)
    query=u64(37)+u64(0)+u64(0)+u64(2);patches=[(8,cap)]
    g.raw('diagnostic.read',caps['health'],query,patches,capacity=95,expected=90);g.equal(g.word(2048),96)
    for source in [caps['noobserve'],caps['drop'],caps['health']]:g.raw('diagnostic.read',caps['health'],query,[(8,source)],expected=13)
    g.raw('diagnostic.read',caps['health'],u64(38)+query[8:],patches,expected=13)
    g.raw('diagnostic.read',caps['health'],query,patches)
    g.equal(g.word(520),1);cursor=g.save('cursor',528);g.equal(g.word(536),2);g.equal(g.word(544),1);g.equal(g.word(552),2)
    g.raw('diagnostic.read',caps['health'],query,patches,expected=9)
    next_query=u64(37)+u64(0)+u64(1)+u64(0)+u64(1)
    g.raw('diagnostic.read',caps['health'],next_query,[(8,cap),(24,cursor)],capacity=64)
    g.equal(g.word(520),0);g.equal(g.word(528),1);g.equal(g.word(536),3);g.equal(g.word(544),0)
    g.raw('diagnostic.read',caps['health'],next_query,[(8,cap),(24,cursor)],capacity=64,expected=9)
    g.check(cap+' call $close');g.raw('log.emit',cap,request,expected=9)
    g.raw('diagnostic.read',caps['health'],query,patches,expected=9)
    assert g.cursor<45000,'canonical records must remain disjoint from dynamic FP operands'
    return g
class LogTagged(TaggedGuest):
    def raw(self,name,*,expected=9,request_tag=None,output_tag=None,length_tag=None):
        self.write(128,1)
        self.check(f'i64.const 1 i32.const {self.memory} i64.const {self.pointer(128,request_tag)} i64.const 8 '
            f'i64.const {self.pointer(512,output_tag)} i64.const 1024 i64.const {self.pointer(2048,length_tag)} call ${name}',expected)
def tagged_guest(width,memory,scenario):
    g=LogTagged(OPERATIONS,width,memory)
    g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
    g.check('call $tag.status'+(' i32.wrap_i64' if width==64 else ''),2)
    if scenario.startswith('mismatch-'):
        _,index,slot=scenario.split('-');g.raw(NAMES[int(index)],**{slot+'_tag':5})
    else:
        for name in NAMES:g.raw(name,request_tag=0 if scenario=='tag-aliases' else 3,output_tag=0 if scenario=='tag-aliases' else 3,length_tag=0 if scenario=='tag-aliases' else 3)
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--mode',choices=['prepare','execute','all'],default='all')
    p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
    args=p.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({12,13,14,15});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    if args.mode!='prepare' and args.runtime is None:p.error('--runtime required')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    source=args.source_root.resolve();contract=source/'src/uwvm2/imported/ros/scene/log_diagnostic_contract.h'
    files=[Path(__file__).resolve(),*[Path(__file__).with_name(n)for n in ['run_scene_vm.py','run_scene_fp.py','run_scene_memtag.py']]]
    pins=lambda:{str(path):sha(path) for path in files}
    if args.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['inputs']==pins() and report['contract_sha256']==sha(contract)
    else:
        assert not (out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,inputs=pins(),contract_sha256=sha(contract),
            cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if args.mode!='execute':
        scenarios=['success','fp','denied','no-grants','tag-valid','tag-aliases']+['mismatch-'+str(i)+'-'+slot for i in range(2) for slot in ['request','output','length']]
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for scenario in scenarios:
                tagged=scenario.startswith(('tag-','mismatch-'))
                if scenario in ['success','fp']:g=successful_guest(width,memory,scenario=='fp')
                elif tagged:g=tagged_guest(width,memory,scenario)
                else:
                    g=LogGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied' else 2);g.open('log',13)
                    # No provider reservation exists in either case; refusal
                    # precedes request reads and cannot invent a handle table.
                    for name in NAMES:g.raw(name,1,u64(1),expected=13)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                result=subprocess.run(cmd,capture_output=True,text=True,timeout=30);assert not result.returncode,result.stderr
                if tagged:wasm.write_bytes(add_private_instructions(wasm.read_bytes(),len(g.checks),memory))
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,tagged=tagged,compile=cmd,
                    assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0),wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if args.mode=='prepare':print('PASS log/diagnostic guest preparation: '+str(len(report['guests'])));return
    runtime=args.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text())
    assert receipt['complete'] and receipt['binary_sha256']==sha(runtime) and receipt['source_root']==str(source)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),source_root=str(source),build_receipt_sha256=sha(build),source_manifest_sha256=sha(source/'source-manifest.json'))
    def run(name,flags,wasm=None,success=True,crash=None,aliases=False):
        verify_gate();cmd=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','1','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*flags]
        if wasm:cmd+=['--run',str(wasm)]
        start=time.monotonic();r=subprocess.run(cmd,capture_output=True,timeout=120);errors=r.stderr.decode(errors='replace')
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        passed=(r.returncode==0)==success
        if crash:
            width,memory,slot=crash;pointer={'request':128,'output':512,'length':2048}[slot]|(5<<(28 if width==32 else 56))
            passed=r.returncode==-signal.SIGILL and all(t in errors for t in ['[crash] memory-protection','Exit status: 122',f'Memory index: {memory}',f'Pointer: {pointer}','Pointer tag: 5','Allocation tag: 3'])
        violations=[]
        if aliases:
            violations=[json.loads(line)for line in errors.splitlines()if line.startswith('{"schema":"uwvm.memtag.violation.')]
            passed=passed and len(violations)>=6 and all(v['pointer_tag']==0 and v['allocation_tag']==3 and v['pointer'] in [128,512,2048]for v in violations)
        report['cases'].append(dict(name=name,argv=cmd,exit=r.returncode,passed=passed,seconds=time.monotonic()-start,violations=violations));save()
        assert passed,(name,r.returncode,errors[-7000:]);print('PASS '+name,flush=True)
    flags=configured()
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256'] and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        scenario=guest['scenario'];selected=flags if scenario in ['success','fp'] or guest['tagged'] else policy(OPERATIONS,'deny' if scenario=='denied' else 'allow')
        for engine in ['int','aot']:
            args=['--runtime-'+engine,*selected]
            if guest['tagged']:args+=['--wasm-policy-global', 'runtime.memtag-backend','software','--runtime-memtag-mode','sync']
            if scenario=='tag-aliases':args+=['--runtime-memtag-action','silent-log','err']
            crash=(guest['width'],guest['memory'],scenario.split('-')[2]) if scenario.startswith('mismatch-') else None
            run(guest['name']+'-'+engine,args,wasm,crash=crash,aliases=scenario=='tag-aliases')
    smoke=out/'no-grants-32-0.wasm';run('default-hidden',['--runtime-int'],smoke,False)
    run('policy-check',[*flags,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    for name,extra in [('duplicate-limit',['--ros-scene-log-limit','robot','log','records','2']),
        ('unknown-limit',['--ros-scene-log-limit','robot','log','unknown','1']),('unlimited',['--ros-scene-log-limit','robot','log','records','unlimited']),
        ('duplicate-logger',['--ros-scene-log-logger','robot','log','robot']),('duplicate-source',['--ros-scene-diagnostic-source','robot','health','log']),
        ('unknown-loss',['--ros-scene-log-loss','robot','log','silent-drop']),('duplicate-budget',['--ros-scene-log-diagnostic-budget','robot','logs','4'])]:
        run('cli-'+name,[*flags,*extra,'--ros-scene-check'],success=False)
    for missing in ['records','record_bytes','storage_bytes','severity_mask']:
        broken=flags.copy();at=next(i for i in range(len(broken)-4) if broken[i:i+4]==['--ros-scene-log-limit','robot','log',missing]);del broken[at:at+5]
        run('cli-missing-'+missing,[*broken,'--ros-scene-check'],success=False)
    assert pins()==report['inputs'] and sha(runtime)==report['runtime_sha256'];report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
