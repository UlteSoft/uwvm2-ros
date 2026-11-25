#!/usr/bin/env python3
"""Qualify nine transform imports and generic requests through real Wasm guests.

Memory32/64 and indices zero/one exercise explicit profiles, actual ROS/steady
clocks, interpolation/velocity, static CAS, pending queries, exact retries,
deadlines and source retirement. FP checks run after actual host returns;
software MTE fixtures qualify every request/result/length boundary separately.
Guests import only uwvm_ros_v2 and require no model worker or image files.
"""
import argparse,hashlib,json,os,resource,signal,struct,subprocess,time
from pathlib import Path
from run_scene_vm import Guest,ROOT,GATE,policy,sha,verify_gate
from run_scene_fp import FPGuest
from run_scene_memtag import TaggedGuest,add_private_instructions

TRANSFORMS=['transform.'+name for name in ['open','publish','lookup','available','velocity','submit','take','cancel','clear']]
REQUESTS=['request.describe','request.release']
OPERATIONS=[dict(name=name,words=0,bytes=0) for name in TRANSFORMS+REQUESTS]
OPERATIONS += [dict(name='clock.create',words=1,bytes=8),dict(name='clock.read',words=1,bytes=32),
               dict(name='clock.update',words=4,bytes=0),dict(name='resource.close',words=1,bytes=0)]
MINIMUM=dict(zip(TRANSFORMS,[8,160,622,64,48,160,790,160,160]));MINIMUM.update({'request.describe':160,'request.release':8})
LIMITS={'cache.samples':16,'cache.samples-per-child':8,'publish.batch':8,'cache.history-ns':10000000000,
        'publish.future-ns':0,'velocity.window-ns':1000000000,'session.requests':4,'query.deadline-ns':1000000000,
        'sessions':2,'budget.data-bytes':1000000,'budget.workspace-bytes':1000000,'budget.request-bytes':1000000,'budget.carrier-bytes':65536}
ALIASES=['frames','peer','fixed','rate','denied','zero']
def u64(value):return (value%(1<<64)).to_bytes(8,'little')
def blob(value):return u64(len(value))+value
def text(value):return blob(value.encode())
DEFINITION=(text('uwvm.ros.transform.definition')+text('frames')+u64(3)+text('base')+text('camera')+text('map')+u64(2)+
            b''.join(u64(n) for n in [2,0,1,0,0,1,0,1])+struct.pack('<4d',1000,1e-12,100,100))
DIGEST=hashlib.sha256(DEFINITION).digest()
def configured():
    flags=policy(OPERATIONS)+['--ros-scene-transform-budget','robot','12','48','6000000','6000000','6000000','393216']
    for alias in ALIASES:
        actions=[name for name in TRANSFORMS+REQUESTS+['resource.close'] if not(alias=='denied' and name=='transform.publish')]
        if alias!='fixed':actions+=['transform.static.replace']
        flags+=['--ros-scene-grant','robot',alias,'transform',alias,'local-transform',','.join(actions),
                '--ros-scene-transform-profile','robot',alias,'frames',DIGEST.hex()]
        for key,value in LIMITS.items():
            flags+=['--ros-scene-transform-limit','robot',alias,key,str(0 if alias=='zero' and key=='sessions' else value)]
        flags+=['--ros-scene-transform-definition-hex','robot',alias,DEFINITION.hex()]
    flags+=['--ros-scene-limit','robot','rate','delivery','may-reject','--ros-scene-limit','robot','rate','calls-per-window','0',
            '--ros-scene-grant','robot','clock','clock.ros','clock','local','clock.create,clock.read,clock.update,resource.close',
            '--ros-scene-grant','robot','steady','clock.steady','steady','local','clock.create,clock.read,resource.close']
    return flags

class TransformGuest(Guest):
    def raw(self,name,cap,payload,patches=(),expected=0,capacity=None,output=512,length=2048,memory=None,size=None):
        address=self.cursor;self.cursor+=len(payload)+8
        self.data.append(f'(data (memory $work) (i{self.width}.const {address}) "'+''.join(f'\\{value:02x}' for value in payload)+'")')
        for offset,value in patches:self.write(address+offset,value)
        if isinstance(cap,int):cap='i64.const '+str(cap)
        if memory is None:memory=self.memory
        if size is None:size=len(payload)
        if capacity is None:capacity=max(MINIMUM[name],1024)
        self.check(f'{cap} i32.const {memory} i64.const {address} i64.const {size} i64.const {output} i64.const {capacity} i64.const {length} call ${name}',expected)
    def exact(self,address,payload):
        for index in range(0,len(payload),8):
            part=payload[index:index+8]
            if len(part)==8:self.equal(self.word(address+index),int.from_bytes(part,'little'))
            else:
                for offset,value in enumerate(part):self.check(f'i{self.width}.const {address+index+offset} i32.load8_u $work',value)
    def short(self,name,cap,payload,patches=(),**kw):
        self.raw(name,cap,payload,patches,capacity=MINIMUM[name]-1,expected=90,**kw);self.equal(self.word(2048),MINIMUM[name])

class TransformFP(FPGuest,TransformGuest):
    def __init__(self,*args,**kwargs):
        super().__init__(*args,**kwargs);self.data=[row.replace('.const 8192)', '.const 49152)') for row in self.data]
    def check(self,*args,**kwargs):
        super().check(*args,**kwargs)
        for old,new in [(8192+i*4,49152+i*4) for i in range(9)]+[(8256+i*8,49216+i*8) for i in range(9)]:
            self.checks[-1]=self.checks[-1].replace(f'.const {old} ',f'.const {new} ')

class TransformTagged(TaggedGuest):
    def raw(self,name,*,expected=9,request_tag=None,output_tag=None,length_tag=None,length=2048):
        self.write(128,1)
        self.check(f'i64.const 1 i32.const {self.memory} i64.const {self.pointer(128,request_tag)} i64.const 8 '
            f'i64.const {self.pointer(512,output_tag)} i64.const 1024 i64.const {self.pointer(length,length_tag)} call ${name}',expected)

def successful_guest(width,memory,fp=False):
    g=(TransformFP if fp else TransformGuest)(OPERATIONS,width,memory);g.check('call $abi',2);g.open('absent',13)
    caps={name:g.open(name) for name in ALIASES+['clock','steady']};cap=caps['frames']
    g.globals.add('clock-cap');g.body.append(caps['clock']+' global.set $clock-cap');timing='global.get $clock-cap'
    g.call('clock.create',timing,[2]);clock=g.save('ros-clock')
    g.call('clock.create',caps['steady'],[0]);steady=g.save('steady-clock')
    g.call('clock.update',timing,[timing,clock,1000000000,1])
    def opening(alias,expected=0,save=None,clock_cap=None,identity='frames'):
        encoded=text(identity)+blob(DIGEST)+bytes(16);patches=[(len(encoded)-16,clock_cap or timing),(len(encoded)-8,clock)]
        g.raw('transform.open',caps[alias],encoded,patches,expected=expected)
        return g.save(save or alias+'-session') if expected==0 else None
    encoded=text('frames')+blob(DIGEST)+bytes(16);opening_patches=[(len(encoded)-16,timing),(len(encoded)-8,clock)]
    for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512)]:
        g.raw('transform.open',cap,encoded,opening_patches,expected=22,**kw)
    g.short('transform.open',cap,encoded,opening_patches)
    opening('frames',13,identity='foreign');opening('rate',11);opening('zero',90)
    session=opening('frames');peer=opening('peer');fixed=opening('fixed')
    def query(time=250000000,source='base',target='map',epoch=1,stamped=True):
        body=text(target)+u64(stamped);patches=[]
        if stamped:patches.append((len(body),clock));body+=u64(0)+u64(epoch)+u64(time)
        body+=text(source)+u64(0)+u64(0);return body,patches
    def lookup(name='transform.lookup',time=250000000,selected=cap,handle=session,source='base',target='map',epoch=1,stamped=True,short=False,**kw):
        body,patches=query(time,source,target,epoch,stamped);body=u64(0)+body;patches=[(0,handle)]+[(at+8,value) for at,value in patches]
        (g.short if short else g.raw)(name,selected,body,patches,**kw)
    def publish(key,values,selected=cap,handle=session,generation=0,epoch=1,short=False,**kw):
        body=u64(0)+u64(key)+u64(0)+u64(generation)+u64(len(values));patches=[(0,handle),(16,handle)]
        for time,x,fixed in values:
            body+=text('base' if fixed else 'map')+text('camera' if fixed else 'base');patches.append((len(body),clock))
            body+=u64(0)+u64(epoch)+u64(time)+struct.pack('<7d',x,0,0,0,0,0,1)+u64(fixed)
        (g.short if short else g.raw)('transform.publish',selected,body,patches,**kw)
    def pose(x,source='base',target='map',time=250000000,epoch=1):
        offset=512+len(text(target))+len(text(source));g.exact(512,text(target)+text(source))
        g.check(g.word(offset)+' '+clock+' i64.ne');g.equal(g.word(offset+8),epoch);g.equal(g.word(offset+16),time)
        g.check(f'i{width}.const {offset+24} f64.load $work f64.const {x} f64.ne')
    lookup('transform.available');g.equal(g.word(512),0)
    samples=[(i*100000000,float(i),False) for i in range(6)]
    publish(1,samples,short=True);publish(1,samples,selected=caps['denied'],expected=13);publish(1,samples,selected=caps['rate'],expected=11)
    publish(1,samples);mutation=g.save('mutation');publish(1,samples);g.check(g.word(512)+' '+mutation+' i64.ne')
    publish(1,[(0,123.0,False)],expected=17)
    lookup(selected=caps['peer'],expected=9);lookup(source='private',expected=13);lookup(short=True);lookup();pose(2.5)
    lookup('transform.available',short=True);lookup('transform.available');g.equal(g.word(512),1)
    # A static replacement requires a separate action and exact current generation.
    publish(2,[(0,1.0,True)]);publish(3,[(0,2.0,True)],expected=17);publish(3,[(0,2.0,True)],generation=2,expected=17)
    publish(3,[(0,2.0,True)],generation=3);publish(3,[(0,2.0,True)],generation=3)
    lookup(source='camera');pose(4.5,source='camera')
    publish(1,[(0,1.0,True)],selected=caps['fixed'],handle=fixed)
    publish(2,[(0,2.0,True)],selected=caps['fixed'],handle=fixed,generation=2,expected=13)
    # Velocity retains its native physical units and exact averaging interval.
    velocity=u64(0)+text('base')+text('map')+text('map')+struct.pack('<3d',0,0,0)+text('base');at=len(velocity)
    velocity+=u64(0)+u64(1)+u64(500000000)+u64(500000000);patches=[(0,session),(at,clock)]
    g.short('transform.velocity',cap,velocity,patches);g.raw('transform.velocity',cap,velocity,patches)
    for index,value in enumerate([10,0,0,0,0,0]):g.check(f'i{width}.const {512+index*8} f64.load $work f64.const {value} f64.ne')
    def deadline(delta=500000000):
        g.call('clock.read',caps['steady'],[steady],output=3072,length=3200)
        g.globals.add('deadline');g.body.append(g.word(3072)+f' i64.const {delta} i64.add global.set $deadline')
    def wait_deadline():
        g.body.append('loop $wait_deadline');g.call('clock.read',caps['steady'],[steady],output=3072,length=3200)
        g.body.append(g.word(3072)+' global.get $deadline i64.lt_u br_if $wait_deadline end')
    def submit(key,time,epoch=1,short=False,**kw):
        body,patches=query(time,epoch=epoch);body=u64(0)+u64(key)+u64(0)+body
        patches=[(0,session),(16,session)]+[(at+24,value)for at,value in patches];patches.append((len(body),'global.get $deadline'));body+=u64(0)
        (g.short if short else g.raw)('transform.submit',cap,body,patches,**kw)
    def request_call(name,request,short=False,**kw):
        body=u64(0) if name.startswith('request.') else bytes(16)
        patches=[(0,request)] if name.startswith('request.') else [(0,session),(8,request)]
        (g.short if short else g.raw)(name,cap,body,patches,**kw)
    def phase(value):g.exact(520,text(value))
    deadline();submit(4,250000000,short=True);submit(4,250000000);request=g.save('request');phase('succeeded')
    request_call('transform.take',request,short=True);request_call('transform.take',request)
    request_call('transform.take',request,expected=11)
    request_call('request.describe',request,short=True);request_call('request.describe',request);phase('succeeded')
    request_call('request.release',request,short=True);request_call('request.release',request);g.equal(g.word(512),1)
    request_call('request.release',request);g.equal(g.word(512),0)
    submit(4,250000000);g.check(g.word(512)+' '+request+' i64.ne')
    deadline();submit(5,900000000);request=g.save('request');phase('queued')
    request_call('transform.take',request,expected=11);request_call('request.release',request,expected=11)
    publish(6,[(900000000,9.0,False)]);request_call('transform.take',request);phase('succeeded')
    request_call('request.release',request);g.equal(g.word(512),1)
    deadline();submit(7,950000000);request=g.save('request');phase('queued')
    request_call('transform.cancel',request,short=True);request_call('transform.cancel',request);phase('cancelled')
    request_call('transform.take',request);phase('cancelled');request_call('request.release',request)
    deadline(50000000);submit(8,950000000);request=g.save('request');wait_deadline()
    request_call('request.describe',request);phase('failed');request_call('transform.take',request);phase('failed');request_call('request.release',request)
    deadline();submit(9,950000000);request=g.save('request')
    g.call('clock.update',timing,[timing,clock,0,1]);lookup(expected=17);request_call('transform.take',request,expected=17)
    def clear(key,generation,short=False,**kw):
        (g.short if short else g.raw)('transform.clear',cap,u64(0)+u64(key)+u64(0)+u64(generation),[(0,session),(16,session)],**kw)
    clear(10,5,short=True);clear(10,4,expected=17);clear(10,5);clear(10,5)
    request_call('transform.take',request);phase('cancelled');request_call('request.release',request)
    lookup('transform.available',expected=17);lookup('transform.available',epoch=2);g.equal(g.word(512),0)
    publish(11,[(0,0.0,False)],epoch=2);lookup(time=0,epoch=2);pose(0,time=0,epoch=2)
    lookup(stamped=False,epoch=2);pose(0,time=0,epoch=2)
    g.call('clock.update',timing,[timing,clock,0,0]);lookup(time=0,epoch=2,expected=11)
    clear(12,7,expected=11);g.call('clock.update',timing,[timing,clock,0,1]);lookup(time=0,epoch=4,expected=17)
    clear(12,7);publish(13,[(0,0.0,False)],epoch=4);mutation=g.save('last-mutation')
    # A separate captured source capability can disappear without a global fallback.
    another_clock=g.open('clock',output=1024);fenced=opening('peer',save='fenced-session',clock_cap=another_clock)
    g.check(another_clock+' call $close');lookup(handle=fenced,selected=caps['peer'],time=0,epoch=4,expected=9)
    g.call('resource.close',caps['peer'],[fenced])
    g.call('resource.close',timing,[clock]);publish(13,[(0,0.0,False)],epoch=4);g.check(g.word(512)+' '+mutation+' i64.ne')
    lookup(time=0,epoch=4,expected=9);publish(14,[(0,0.0,False)],epoch=4,expected=9)
    for selected,handle in [(cap,session),(caps['peer'],peer),(caps['fixed'],fixed)]:g.call('resource.close',selected,[handle])
    g.check(cap+' call $close');g.raw('transform.open',cap,encoded,opening_patches,expected=9)
    assert g.cursor<45000,'keep canonical records disjoint from FP operands'
    return g

def tagged_guest(width,memory,scenario):
    g=TransformTagged(OPERATIONS,width,memory);g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
    g.check('call $tag.status'+(' i32.wrap_i64' if width==64 else ''),2)
    if scenario.startswith('mismatch-'):
        _,index,slot=scenario.split('-');g.raw((TRANSFORMS+REQUESTS)[int(index)],expected=22,**{slot+'_tag':5});return g
    for name in TRANSFORMS+REQUESTS:
        if scenario=='tag-aliases':
            for where in [128,512]:
                g.write(512,0x123456789abcdef);g.write(2048,0x3456789);g.raw(name,expected=22,length=where,length_tag=0)
                g.equal(g.word(512),0x123456789abcdef);g.equal(g.word(2048),0x3456789)
            g.raw(name,length_tag=0)
        else:g.raw(name)
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--mode',choices=['prepare','execute','all'],default='all')
    p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
    args=p.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({12,13,14,15});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    if args.mode!='prepare' and args.runtime is None:p.error('--runtime required')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    source=args.source_root.resolve();contract=source/'src/uwvm2/imported/ros/scene/transform_contract.h'
    files=[Path(__file__).resolve(),*[Path(__file__).with_name(n)for n in ['run_scene_vm.py','run_scene_fp.py','run_scene_memtag.py']]]
    pins=lambda:{str(path):sha(path) for path in files}
    if args.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['inputs']==pins() and report['contract_sha256']==sha(contract)
    else:
        assert not (out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,inputs=pins(),contract_sha256=sha(contract),
            cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if args.mode!='execute':
        scenarios=['success','fp','denied','no-grants','tag-valid','tag-aliases']+['mismatch-'+str(i)+'-'+slot for i in range(11) for slot in ['request','output','length']]
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for scenario in scenarios:
                tagged=scenario.startswith(('tag-','mismatch-'))
                if scenario in ['success','fp']:g=successful_guest(width,memory,scenario=='fp')
                elif tagged:g=tagged_guest(width,memory,scenario)
                else:
                    g=TransformGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied' else 2);g.open('frames',13)
                    for name in TRANSFORMS+REQUESTS:g.raw(name,1,u64(1),expected=13 if scenario=='denied' else 9)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                result=subprocess.run(cmd,capture_output=True,text=True,timeout=30);assert not result.returncode,result.stderr
                if tagged:wasm.write_bytes(add_private_instructions(wasm.read_bytes(),len(g.checks),memory))
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,tagged=tagged,compile=cmd,
                    assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0),wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if args.mode=='prepare':print('PASS transform guest preparation: '+str(len(report['guests'])));return
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
            passed=passed and len(violations)>=33 and all(v['pointer_tag']==0 and v['allocation_tag']==3 and v['pointer'] in [128,512,2048]for v in violations)
        report['cases'].append(dict(name=name,argv=cmd,exit=r.returncode,passed=passed,seconds=time.monotonic()-start,violations=violations));save()
        assert passed,(name,r.returncode,errors[-7000:]);print('PASS '+name,flush=True)
    flags=configured()
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256'] and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        scenario=guest['scenario'];selected=flags if scenario in ['success','fp'] else policy(OPERATIONS,'deny' if scenario=='denied' else 'allow')
        for engine in ['int','aot']:
            args=['--runtime-'+engine,*selected]
            if guest['tagged']:args+=['--wasm-policy-global', 'runtime.memtag-backend','software','--runtime-memtag-mode','sync']
            if scenario=='tag-aliases':args+=['--runtime-memtag-action','silent-log','err']
            crash=(guest['width'],guest['memory'],scenario.split('-')[2]) if scenario.startswith('mismatch-') else None
            run(guest['name']+'-'+engine,args,wasm,crash=crash,aliases=scenario=='tag-aliases')
    smoke=out/'no-grants-32-0.wasm';run('default-hidden',['--runtime-int'],smoke,False)
    run('policy-check',[*flags,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    for name,extra in [('duplicate-limit',['--ros-scene-transform-limit','robot','frames','sessions','2']),
        ('unknown-limit',['--ros-scene-transform-limit','robot','frames','unknown','1']),('unlimited',['--ros-scene-transform-limit','robot','frames','sessions','unlimited']),
        ('duplicate-definition',['--ros-scene-transform-definition-hex','robot','frames',DEFINITION.hex()])]:run('cli-'+name,[*flags,*extra,'--ros-scene-check'],success=False)
    assert pins()==report['inputs'] and sha(runtime)==report['runtime_sha256'];report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
