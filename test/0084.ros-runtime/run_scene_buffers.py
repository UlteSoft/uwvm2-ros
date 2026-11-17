#!/usr/bin/env python3
"""Qualify sealed-buffer imports and closed CLI policy through actual Wasm guests.

Memory32/64 and both memory indices exercise canonical records, exact profile
bindings, initialization/SHA sealing/replay, metadata, range checks and output
preflight. The five imports do not expose native pin(): immutable pin lifetime
and downstream consumer admission require separate core/provider qualification.
Every compiler and VM subprocess must inherit the existing 64 GiB zero-swap gate.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import time
from run_scene_vm import Guest,ROOT,GATE,policy,sha,verify_gate

OPERATIONS=[dict(name='buffer.'+name,words=0,bytes=0) for name in ['create','write','seal','read','describe']]
OPERATIONS += [dict(name='resource.close',words=1,bytes=0)]
PROFILE='test.sensor.bytes';PROFILE_SHA=hashlib.sha256(b'bounded sensor byte profile').digest()
PAYLOAD=b'robotics';PAYLOAD_SHA=hashlib.sha256(PAYLOAD).digest()
ALIASES=['data','peer','zero','workspacezero','no-write','rate','resourcezero','large','large-tight','large-zero','large-quota0']
def u64(value):return value.to_bytes(8,'little')
def blob(value):return u64(len(value))+value
def text(value):return blob(value.encode())
def layout(*,identity=PROFILE,digest=PROFILE_SHA,element='uint8',shape=(8,),strides=(1,),order='little',color=None,size=8):
    return text(identity)+blob(digest)+text(element)+u64(len(shape))+b''.join(map(u64,shape))+u64(len(strides))+b''.join(map(u64,strides))+text(order)+u64(color is not None)+(text(color) if color else b'')+u64(size)
def plan(alias):
    if alias.startswith('large'):
        return ['--ros-scene-buffer','robot',alias,PROFILE,PROFILE_SHA.hex(),'2048','2','1024','1024',
                '623' if alias=='large-tight' else '0' if alias=='large-zero' else '624','2','2','1024','1','1','1']
    return ['--ros-scene-buffer','robot',alias,PROFILE,PROFILE_SHA.hex(),'64','0' if alias=='zero' else '2','32','16',
            '0' if alias=='workspacezero' else '32','2','2','32','1','1','1']
def configured():
    flags=policy(OPERATIONS)+['--ros-scene-buffer-budget','robot','10000','32','10000','32']
    for alias in ALIASES:
        actions=[op['name'] for op in OPERATIONS if not(alias=='no-write' and op['name']=='buffer.write')]
        flags+=['--ros-scene-grant','robot',alias,'buffer',alias,'sealed-buffer',','.join(actions),*plan(alias)]
    for alias,key,value in [('resourcezero','max-resources','0'),('rate','delivery','may-reject'),('rate','calls-per-window','1'),('rate','window-ns','3600000000000'),('large-quota0','delivery','may-reject'),('large-quota0','calls-per-window','0')]:
        flags+=['--ros-scene-limit','robot',alias,key,value]
    return flags
class BufferGuest(Guest):
    def raw(self,name,cap,payload=b'',patches=(),expected=0,capacity=512,output=512,length=1024,memory=None,size=None):
        address=self.cursor;self.cursor+=len(payload)+8
        self.data.append(f'(data (memory $work) (i{self.width}.const {address}) "'+''.join(f'\\{b:02x}' for b in payload)+'")')
        for offset,value in patches:self.write(address+offset,value)
        if memory is None:memory=self.memory
        if size is None:size=len(payload)
        if isinstance(cap,int):cap='i64.const '+str(cap)
        self.check(f'{cap} i32.const {memory} i64.const {address} i64.const {size} i64.const {output} i64.const {capacity} i64.const {length} call ${name}',expected)
    def describe(self,cap,handle,sealed,format=None):
        expected=(format or layout())+u64(sealed)+text('guest')
        self.raw('buffer.describe',cap,u64(0),[(0,handle)],capacity=len(expected))
        self.equal(self.word(1024),len(expected));self.exact(512,expected)
    def exact(self,address,payload):
        for offset in range(0,len(payload),8):
            part=payload[offset:offset+8]
            if len(part)==8:self.equal(self.word(address+offset),int.from_bytes(part,'little'))
            else:
                for n,byte in enumerate(part):self.check(f'i{self.width}.const {address+offset+n} i32.load8_u $work',byte)
def successful_guest(width,memory):
    g=BufferGuest(OPERATIONS,width,memory);g.check('call $abi',2);g.open('absent',13)
    caps={a:g.open(a) for a in ALIASES};cap=caps['data'];encoded=layout()
    for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512)]:
        g.raw('buffer.create',cap,encoded,expected=22,**kw)
    g.raw('buffer.create',cap,encoded,capacity=7,expected=90);g.equal(g.word(1024),8)
    for malformed,code in [(encoded+b'x',22),(encoded[:-1],22),(layout(identity='foreign'),13),
        (layout(digest=bytes(32)),13),(layout(element='uint16'),13),(layout(order='big'),13),(layout(color='rgb'),13),
        (layout(shape=(8,8),strides=(1,1),size=8),90),(layout(strides=(0,)),22),(layout(size=7),22)]:
        g.raw('buffer.create',cap,malformed,expected=code)
    g.raw('buffer.create',caps['zero'],encoded,expected=11);g.raw('buffer.create',caps['resourcezero'],encoded,expected=11)
    g.raw('buffer.create',cap,encoded);handle=g.save('object');g.describe(cap,handle,False)
    g.raw('buffer.create',cap,encoded);other=g.save('other-object')
    g.raw('buffer.create',cap,encoded,expected=11)
    g.call('resource.close',cap,[other]);g.call('resource.close',cap,[other],expected=9)
    def write(payload=PAYLOAD,offset=0,**kw):g.raw('buffer.write',cap,u64(0)+u64(offset)+blob(payload),[(0,handle)],**kw)
    def seal(digest=PAYLOAD_SHA,**kw):g.raw('buffer.seal',cap,u64(0)+blob(digest),[(0,handle)],**kw)
    def read(offset,size,**kw):g.raw('buffer.read',cap,u64(0)+u64(offset)+u64(size),[(0,handle)],**kw)
    read(0,8,expected=90);seal(expected=17)
    write(offset=1,expected=90);write(PAYLOAD[:4],offset=1,expected=17)
    write(capacity=7,expected=90);g.equal(g.word(1024),8);read(0,8,expected=90)
    write(PAYLOAD[:4]);g.equal(g.word(512),4)
    write(PAYLOAD[:4]);g.equal(g.word(512),4)
    write(b'ROBO',expected=17);write(PAYLOAD,expected=17)
    read(0,4,capacity=11,expected=90);g.equal(g.word(1024),12)
    read(0,4,capacity=12);g.equal(g.word(512),4);g.exact(520,PAYLOAD[:4])
    read(4,1,expected=90);seal(expected=17)
    write(PAYLOAD[4:],offset=4);write(PAYLOAD[4:],offset=4);g.equal(g.word(512),4)
    seal(bytes(32),expected=17);g.describe(cap,handle,False)
    seal(capacity=15,expected=90);g.equal(g.word(1024),16);g.describe(cap,handle,False)
    seal();g.check(g.word(512)+' '+cap+' i64.ne');g.check(g.word(520)+' '+handle+' i64.ne')
    seal();seal(bytes(32),expected=17);write(expected=13);g.describe(cap,handle,True)
    read(0,8,capacity=16);g.equal(g.word(512),8);g.exact(520,PAYLOAD)
    read(8,0,capacity=8);g.equal(g.word(512),0);read(9,0,expected=90);read(0,17,expected=90)
    for name,payload in [('buffer.write',u64(0)+u64(0)+blob(PAYLOAD)),('buffer.seal',u64(0)+blob(PAYLOAD_SHA)),
                         ('buffer.read',u64(0)+u64(0)+u64(8)),('buffer.describe',u64(0))]:
        g.raw(name,caps['peer'],payload,[(0,handle)],expected=9)
    g.call('resource.close',caps['peer'],[handle],expected=9)
    g.raw('buffer.write',caps['no-write'],u64(0)+u64(0)+blob(PAYLOAD),[(0,handle)],expected=13)
    g.call('resource.close',cap,[handle]);g.raw('buffer.describe',cap,u64(0),[(0,handle)],expected=9)
    empty=layout(shape=(0,),size=0);g.raw('buffer.create',cap,empty);handle=g.save('empty');g.describe(cap,handle,False,empty)
    g.raw('buffer.seal',cap,u64(0)+blob(hashlib.sha256(b'').digest()),[(0,handle)]);g.describe(cap,handle,True,empty)
    read(0,0,capacity=8);g.call('resource.close',cap,[handle])
    wc=caps['workspacezero'];g.raw('buffer.create',wc,encoded);wh=g.save('workspace-object')
    g.raw('buffer.write',wc,u64(0)+u64(0)+blob(PAYLOAD),[(0,wh)])
    g.raw('buffer.read',wc,u64(0)+u64(0)+u64(8),[(0,wh)],expected=11)
    rate=caps['rate'];g.raw('buffer.create',rate,encoded,capacity=7,expected=90)
    g.raw('buffer.create',rate,encoded);rh=g.save('rate-object');g.raw('buffer.describe',rate,u64(0),[(0,rh)],expected=11)
    g.check(rate+' call $close');rate=g.open('rate');g.raw('buffer.describe',rate,u64(0),[(0,rh)],expected=11)
    large_bytes=bytes(n%251 for n in range(600));large_layout=layout(shape=(600,),size=600)
    for alias in ['large','large-tight','large-zero']:
        lc=caps[alias];g.raw('buffer.create',lc,large_layout);lh=g.save(alias+'-object')
        large_request=u64(0)+u64(0)+blob(large_bytes)
        if alias=='large':
            g.raw('buffer.write',lc,large_request,[(0,lh)]);g.equal(g.word(512),600)
            g.raw('buffer.write',lc,large_request,[(0,lh)]);g.equal(g.word(512),600)
            g.raw('buffer.seal',lc,u64(0)+blob(hashlib.sha256(large_bytes).digest()),[(0,lh)])
            g.raw('buffer.read',lc,u64(0)+u64(0)+u64(600),[(0,lh)],capacity=608,output=1200,length=2048)
            g.equal(g.word(1200),600);g.exact(1208,large_bytes)
        else:
            # The complete 624-byte encoded request is reserved, not just its 600-byte payload.
            g.raw('buffer.write',lc,large_request,[(0,lh)],expected=11)
            g.raw('buffer.read',lc,u64(0)+u64(0)+u64(1),[(0,lh)],expected=90)
            g.raw('buffer.write',lc,u64(0)+u64(0)+blob(large_bytes[:8]),[(0,lh)])
            g.equal(g.word(512),8)
    g.raw('buffer.write',caps['large-quota0'],u64(1)+u64(0)+blob(large_bytes),expected=11)
    g.check(cap+' call $close');g.raw('buffer.create',cap,encoded,expected=9)
    return g

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--mode',choices=['prepare','execute','all'],default='all')
    parser.add_argument('--runtime',type=Path);parser.add_argument('--source-root',type=Path,default=ROOT);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset(set(range(8,12)))
    if args.mode!='prepare' and args.runtime is None:parser.error('--runtime is required')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    paths=[Path(__file__).resolve(),Path(__file__).with_name('run_scene_vm.py').resolve()]
    pins=lambda:{str(p):sha(p) for p in paths}
    if args.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['preparation_complete'] and report['sources']==pins()
    else:
        assert not (out/'results.json').exists(),'fresh directory required'
        report=dict(complete=False,preparation_complete=False,scope=__doc__,cgroup=str(GATE),cgroup_inode=GATE.stat().st_ino,
            memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),sources=pins(),cases=[],guests=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if args.mode!='execute':
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for mode in ['success','denied','no-grants']:
                if mode=='success':g=successful_guest(width,memory)
                else:
                    g=BufferGuest(OPERATIONS,width,memory);g.check('call $abi',13 if mode=='denied' else 2);g.open('data',13)
                    for op in OPERATIONS:
                        if op['name']=='resource.close':g.call(op['name'],1,[1],expected=13 if mode=='denied' else 9)
                        else:g.raw(op['name'],1,expected=13 if mode=='denied' else 9)
                name=f'{mode}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                result=subprocess.run(cmd,capture_output=True);assert result.returncode==0,result.stderr.decode()
                report['guests'].append(dict(name=name,mode=mode,width=width,memory=memory,assertions=len(g.checks),compile=cmd,wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['preparation_complete']=True;save()
    if args.mode=='prepare':print('PASS buffer Wasm preparation',flush=True);return
    runtime=args.runtime.resolve();report['runtime']=str(runtime);report['runtime_sha256']=sha(runtime)
    production=args.source_root.resolve();report['source_root']=str(production)
    report['production_pins']={str(p):sha(p) for p in (production/'src/uwvm2/imported/ros/scene').glob('*.h')}
    def run(name,flags,wasm=None,success=True):
        verify_gate();cmd=[str(runtime),'--wasm-set-main-module-name','main',*flags]
        if wasm:cmd+=['--run',str(wasm)]
        start=time.monotonic();r=subprocess.run(cmd,capture_output=True,timeout=120)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        passed=(r.returncode==0)==success;report['cases'].append(dict(name=name,argv=cmd,exit=r.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,r.stderr.decode(errors='replace')[-5000:]);print('PASS',name,flush=True)
    flags=configured()
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256'] and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        active=flags if guest['mode']=='success' else policy(OPERATIONS,'deny' if guest['mode']=='denied' else 'allow')
        for engine in ['int','aot']:run(guest['name']+'-'+engine,['--runtime-'+engine,*active],wasm)
    smoke=out/'no-grants-32-0.wasm';run('default-hidden',['--runtime-int'],smoke,False);run('policy-check',[*flags,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    grant=['--ros-scene-create','robot','--ros-scene-buffer-budget','robot','64','2','32','2',
           '--ros-scene-grant','robot','data','buffer','data','sealed-buffer','buffer.create']
    spec=plan('data');run('cli-missing-plan',[*grant,'--ros-scene-check'],success=False)
    run('cli-duplicate-plan',[*grant,*spec,*spec,'--ros-scene-check'],success=False)
    run('cli-before-grant',['--ros-scene-create','robot',*spec,'--ros-scene-check'],success=False)
    for index,value in [(2,'absent'),(3,''),(4,'0'*63),(4,'A'*64),(5,'unlimited'),(5,str(2**63)),(6,str(2**64-1)),
                        (7,'65'),(8,str(4*1024*1024+1)),(9,str(2**63)),(10,str(2**64-1)),(11,'9'),(12,str(2**63)),(13,'8192'),(14,'4'),(15,'256')]:
        invalid=spec.copy();invalid[index]=value;run('cli-plan-'+str(index)+'-'+str(len(value)),[*grant,*invalid,'--ros-scene-check'],success=False)
    for index,value in [(11,'cache'),(13,'local'),(14,'resource.observe')]:
        invalid=grant.copy();invalid[index]=value;run('cli-grant-'+str(index),[*invalid,*spec,'--ros-scene-check'],success=False)
    for n in range(4):
        invalid=grant.copy();invalid[4+n]='0';run('cli-budget-'+str(n),[*invalid,*spec,'--ros-scene-check'],success=False)
    assert sha(runtime)==report['runtime_sha256'] and pins()==report['sources']
    assert all(sha(Path(p))==digest for p,digest in report['production_pins'].items())
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
