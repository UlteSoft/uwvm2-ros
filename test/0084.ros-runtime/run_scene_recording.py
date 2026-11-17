#!/usr/bin/env python3
"""Qualify seven private recording/replay imports through actual sealed-buffer Wasm flows.

Four memory layouts cover exact immutable input authority, complete receipts,
cursor preservation, explicit attenuation, bounded storage and all seven imports.
FP arithmetic follows actual host returns. Software MTE exercises request,
output and length pointers independently. No native ROS publication, guest
filesystem or durability claim is made. Every child stays in the existing gate.
"""
import argparse,hashlib,json,os,resource,signal,subprocess,time
from pathlib import Path
from run_scene_vm import Guest,ROOT,GATE,policy,sha,verify_gate
from run_scene_buffers import BufferGuest,layout
from run_scene_fp import FPGuest
from run_scene_memtag import TaggedGuest,add_private_instructions
NAMES=['recording.open','recording.append','recording.checkpoint','recording.stop','replay.open','replay.read','replay.seek']
OPERATIONS=[dict(name=n,words=0,bytes=0) for n in NAMES+['buffer.create','buffer.write','buffer.seal','buffer.read']]+[dict(name='resource.close',words=1,bytes=0)]
def u64(value):return (value%(1<<64)).to_bytes(8,'little')
def blob(value):return u64(len(value))+value
def text(value):return blob(value.encode())
BUFFER='serialized.bool';BUFFER_SHA=hashlib.sha256(b'closed ROS Bool serialized bytes').digest()
RECORDING='recorded.bool';REPLAY='recorded.bool.replay';REPLAY_SHA=hashlib.sha256(b'private replay profile').digest()
BODY=text('ros1')+u64(1)+u64(1)+u64(1)+text('/state')+text('ros1-std-bool')+text('std_msgs/Bool')+text('ros1msg')+text('ros1')+text('')+blob(b'bool data\n')+u64(1)+text('input')+text(BUFFER)+blob(BUFFER_SHA)
RECORDING_SHA=hashlib.sha256(BODY).digest()
DEFINITION=text('uwvm.ros.recording.definition')+text(RECORDING)+blob(RECORDING_SHA)+BODY
LIMITS={'segments':3,'segment.bytes':2048,'segment.messages':16,'replays':2,'request.bytes':6072,'read.items':16,'storage.bytes':262144,'definition.bytes':8192,'channels':4}
def configured():
    flags=policy(OPERATIONS)+['--ros-scene-buffer-budget','robot','32','16','64','256']
    for alias in ['input','foreign']:
        flags+=['--ros-scene-grant','robot',alias,'buffer',alias,'sealed-buffer','buffer.create,buffer.write,buffer.seal,buffer.read,resource.close,data.export',
                '--ros-scene-buffer','robot',alias,BUFFER,BUFFER_SHA.hex(),'16','8','4','4','32','128','1','4','1','1','1']
    for field,value in {'recordings':3,'replays':1,'storage.bytes':1048576,'workspace.bytes':65536,'workspace.calls':8}.items():
        flags+=['--ros-scene-recording-budget','robot',field,str(value)]
    for alias in ['recording','peer','rate']:
        flags+=['--ros-scene-grant','robot',alias,'recording',alias,'private-recording',','.join(NAMES[:4]+['resource.close','resource.observe','data.export']),
                '--ros-scene-recording-profile','robot',alias,RECORDING,RECORDING_SHA.hex()]
        for field,value in LIMITS.items():flags+=['--ros-scene-recording-limit','robot',alias,field,str(value)]
        flags+=['--ros-scene-recording-definition-hex','robot',alias,DEFINITION.hex()]
    flags+=['--ros-scene-grant','robot','replay','replay','records','private-replay','replay.open,replay.read,replay.seek,resource.close',
            '--ros-scene-replay','robot','replay','recording',REPLAY,REPLAY_SHA.hex(),'2',
            '--ros-scene-limit','robot','rate','delivery','may-reject','--ros-scene-limit','robot','rate','calls-per-window','0']
    return flags
class RecordingGuest(Guest):
    exact=BufferGuest.exact
    def raw(self,name,cap,payload,patches=(),expected=0,capacity=1024,output=512,length=2048,memory=None,size=None):
        address=self.cursor;self.cursor+=len(payload)+8
        self.data.append(f'(data (memory $work) (i{self.width}.const {address}) "'+''.join(f'\\{b:02x}' for b in payload)+'")')
        for offset,value in patches:self.write(address+offset,value)
        if isinstance(cap,int):cap='i64.const '+str(cap)
        if memory is None:memory=self.memory
        if size is None:size=len(payload)
        if output=='request':output=address
        if length=='request':length=address
        self.last_request=address
        self.check(f'{cap} i32.const {memory} i64.const {address} i64.const {size} i64.const {output} i64.const {capacity} i64.const {length} call ${name}',expected)
class RecordingFP(FPGuest,RecordingGuest):
    def __init__(self,*args,**kwargs):
        super().__init__(*args,**kwargs);self.data=[row.replace('.const 8192)', '.const 49152)') for row in self.data]
    def check(self,*args,**kwargs):
        super().check(*args,**kwargs)
        for bits,old,new in [(32,8192,49152),(64,8256,49216)]:
            for i in range(9):
                before=f'i{self.width}.const {old+i*(bits//8)} f{bits}.load $work'
                after=f'i{self.width}.const {new+i*(bits//8)} f{bits}.load $work'
                self.checks[-1]=self.checks[-1].replace(before,after)
def successful_guest(width,memory,fp=False):
    g=(RecordingFP if fp else RecordingGuest)(OPERATIONS,width,memory);g.check('call $abi',2);g.open('missing',13)
    caps={alias:g.open(alias) for alias in ['input','foreign','recording','peer','rate','replay']};cap=caps['recording']
    profile=text(RECORDING)+blob(RECORDING_SHA)
    for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:g.raw('recording.open',cap,profile,expected=22,**kw)
    g.raw('recording.open',cap,profile,capacity=7,expected=90);g.equal(g.word(2048),8)
    for bad in [profile[:-1],profile+b'x',text('')+blob(RECORDING_SHA)]:g.raw('recording.open',cap,bad,expected=22)
    g.raw('recording.open',cap,text('other')+blob(RECORDING_SHA),expected=17)
    g.raw('recording.open',caps['rate'],profile,expected=11)
    g.raw('recording.open',cap,profile,capacity=8);writer=g.save('writer')
    encoded=layout(identity=BUFFER,digest=BUFFER_SHA,shape=(1,),size=1)
    g.raw('buffer.create',caps['input'],encoded);buffer=g.save('buffer')
    g.raw('buffer.write',caps['input'],u64(0)+u64(0)+blob(b'\x01'),[(0,buffer)]);g.equal(g.word(512),1)
    digest=hashlib.sha256(b'\x01').digest()
    g.raw('buffer.seal',caps['input'],u64(0)+blob(digest),[(0,buffer)])
    def append(key=1,count=1,source=None,expected=0,**kw):
        data=u64(0)+u64(key)+u64(count);patches=[(0,writer)]
        for i in range(count):
            offset=len(data);data+=u64(0)+u64(0)+u64(1)+u64(5+i)+u64(10+i)+u64(20+i)+u64(1)+blob(digest)
            patches.extend([(offset,source or caps['input']),(offset+8,buffer)])
        g.raw('recording.append',cap,data,patches,expected=expected,**kw)
    append(capacity=103,expected=90);g.equal(g.word(2048),104)
    append(source=caps['foreign'],expected=13)
    append(key=2,expected=17);append();g.equal(g.word(512),1);g.equal(g.word(520),1);g.equal(g.word(536),1)
    append();g.equal(g.word(520),1)
    g.raw('recording.checkpoint',cap,u64(0)+u64(2),[(0,writer)],capacity=103,expected=90)
    g.raw('recording.checkpoint',cap,u64(0)+u64(2),[(0,writer)])
    g.equal(g.word(512),2);g.equal(g.word(536),2);g.equal(g.word(568),32)
    obj=g.save('object',544);size=g.save('object-size',552);messages=g.save('object-messages',560)
    hashes=[g.save('object-hash-'+str(i),576+8*i) for i in range(4)]
    g.raw('recording.checkpoint',cap,u64(0)+u64(2),[(0,writer)]);g.check(g.word(544)+' '+obj+' i64.ne')
    replay_profile=text(REPLAY)+blob(REPLAY_SHA)
    request=u64(0)+replay_profile+u64(1)+u64(0)*3+blob(bytes(32));at=8+len(replay_profile)+8
    patches=[(0,cap),(at,obj),(at+8,size),(at+16,messages)]+[(at+32+i*8,value) for i,value in enumerate(hashes)]
    g.raw('replay.open',caps['replay'],request,[(0,caps['peer']),*patches[1:]],expected=13)
    g.raw('replay.open',caps['replay'],request,patches,capacity=7,expected=90)
    g.raw('replay.open',caps['replay'],request,patches);replay=g.save('cursor')
    g.call('resource.close',cap,[obj],expected=11)
    def read(expected=0,capacity=57):g.raw('replay.read',caps['replay'],u64(0)+u64(1),[(0,replay)],expected=expected,capacity=capacity)
    read(expected=90,capacity=56);g.equal(g.word(2048),57);read()
    g.exact(512,u64(0)+u64(1)+u64(1)+u64(5)+u64(10)+u64(20)+blob(b'\x01'))
    read(capacity=16);g.equal(g.word(512),1);g.equal(g.word(520),0)
    g.raw('replay.seek',caps['replay'],u64(0)+u64(0),[(0,replay)],capacity=7,expected=90)
    g.raw('replay.seek',caps['replay'],u64(0)+u64(0),[(0,replay)]);g.equal(g.word(512),0);read()
    append(key=3,count=7);g.equal(g.word(512),3);g.equal(g.word(520),7) # Heap carrier remains finite and owned.
    append(key=3,count=7);g.equal(g.word(520),7)
    g.raw('recording.stop',cap,u64(0)+u64(4)+u64(2),[(0,writer)],expected=22)
    g.raw('recording.stop',cap,u64(0)+u64(4)+u64(1),[(0,writer)])
    g.equal(g.word(512),4);g.equal(g.word(536),3);g.equal(g.word(608),1)
    g.raw('recording.stop',cap,u64(0)+u64(4)+u64(1),[(0,writer)])
    g.call('resource.close',caps['replay'],[replay]);g.call('resource.close',cap,[obj]);g.call('resource.close',cap,[writer])
    g.raw('recording.open',cap,profile);writer=g.save('writer')
    g.raw('recording.stop',cap,u64(0)+u64(1)+u64(0),[(0,writer)]);g.equal(g.word(536),4);g.equal(g.word(608),1)
    g.call('resource.close',cap,[writer]);g.call('resource.close',caps['input'],[buffer]);g.check(cap+' call $close')
    g.raw('recording.open',cap,profile,expected=9)
    assert g.cursor<45000
    return g
class RecordingTagged(TaggedGuest):
    def raw(self,name,*,request_tag=None,output_tag=None,length_tag=None):
        self.write(128,1)
        self.check(f'i64.const 1 i32.const {self.memory} i64.const {self.pointer(128,request_tag)} i64.const 8 '
                   f'i64.const {self.pointer(512,output_tag)} i64.const 1024 i64.const {self.pointer(2048,length_tag)} call ${name}',9)
def tagged_guest(width,memory,scenario):
    g=RecordingTagged(OPERATIONS,width,memory);g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
    g.check('call $tag.status'+(' i32.wrap_i64' if width==64 else ''),2)
    if scenario.startswith('mismatch-'):
        _,index,slot=scenario.split('-');g.raw(NAMES[int(index)],**{slot+'_tag':5})
    else:
        for name in NAMES:g.raw(name,request_tag=0 if scenario=='tag-aliases' else 3,output_tag=0 if scenario=='tag-aliases' else 3,length_tag=0 if scenario=='tag-aliases' else 3)
    return g
def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--mode',choices=['prepare','execute','all'],default='all')
    parser.add_argument('--runtime',type=Path);parser.add_argument('--source-root',type=Path,default=ROOT);parser.add_argument('--output',type=Path,required=True)
    # Keep the shared 64 GiB gate and permit either reserved qualification pool.
    args=parser.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({16,17,18,19,20,21});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    if args.mode!='prepare' and not args.runtime:parser.error('--runtime required')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    source=args.source_root.resolve();contract=source/'src/uwvm2/imported/ros/scene/recording_contract.h'
    paths=[Path(__file__).resolve(),*[Path(__file__).with_name(name) for name in ['run_scene_vm.py','run_scene_buffers.py','run_scene_fp.py','run_scene_memtag.py']]]
    pins=lambda:{str(p):sha(p) for p in paths}
    if args.mode=='execute':
        report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['inputs']==pins() and report['contract_sha256']==sha(contract)
    else:
        assert not (out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,inputs=pins(),contract_sha256=sha(contract),
            cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),events_before=(GATE/'memory.events').read_text(),guests=[],cases=[])
    report['invocation_affinity']=sorted(os.sched_getaffinity(0))
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if args.mode!='execute':
        scenarios=['success','fp','denied','no-grants','tag-valid','tag-aliases']+['mismatch-'+str(i)+'-'+slot for i in range(7) for slot in ['request','output','length']]
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for scenario in scenarios:
                tagged=scenario.startswith(('tag-','mismatch-'))
                if scenario in ['success','fp']:g=successful_guest(width,memory,scenario=='fp')
                elif tagged:g=tagged_guest(width,memory,scenario)
                else:
                    g=RecordingGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied' else 2);g.open('recording',13)
                    for name in NAMES:g.raw(name,1,u64(1),expected=13)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                result=subprocess.run(cmd,capture_output=True,text=True,timeout=30);assert not result.returncode,result.stderr
                if tagged:wasm.write_bytes(add_private_instructions(wasm.read_bytes(),len(g.checks),memory))
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,tagged=tagged,compile=cmd,
                    assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0),wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if args.mode=='prepare':print('PASS recording guest preparation: '+str(len(report['guests'])));return
    runtime=args.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text())
    assert receipt['complete'] and receipt['binary_sha256']==sha(runtime) and receipt['source_root']==str(source)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),source_root=str(source),build_receipt_sha256=sha(build),source_manifest_sha256=sha(source/'source-manifest.json'))
    def run(name,flags,wasm=None,success=True,crash=None,aliases=False):
        verify_gate();cmd=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*flags]
        if wasm:cmd+=['--run',str(wasm)]
        start=time.monotonic();result=subprocess.run(cmd,capture_output=True,timeout=120);errors=result.stderr.decode(errors='replace')
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr);passed=(result.returncode==0)==success
        if crash:
            width,memory,slot=crash;pointer={'request':128,'output':512,'length':2048}[slot]|(5<<(28 if width==32 else 56))
            passed=result.returncode==-signal.SIGILL and all(t in errors for t in ['[crash] memory-protection','Exit status: 122',f'Memory index: {memory}',f'Pointer: {pointer}','Pointer tag: 5','Allocation tag: 3'])
        violations=[]
        if aliases:
            violations=[json.loads(line) for line in errors.splitlines() if line.startswith('{"schema":"uwvm.memtag.violation.')]
            passed=passed and len(violations)>=21 and all(v['pointer_tag']==0 and v['allocation_tag']==3 and v['pointer'] in [128,512,2048] for v in violations)
        report['cases'].append(dict(name=name,argv=cmd,exit=result.returncode,passed=passed,seconds=time.monotonic()-start,violations=violations));save()
        assert passed,(name,result.returncode,errors[-7000:]);print('PASS '+name,flush=True)
    flags=configured()
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256'] and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        scenario=guest['scenario'];selected=flags if scenario in ['success','fp'] or guest['tagged'] else policy(OPERATIONS,'deny' if scenario=='denied' else 'allow')
        for engine in ['int','aot']:
            selected_flags=['--runtime-'+engine,*selected]
            if guest['tagged']:selected_flags+=['--runtime-memtag-backend','software','--runtime-memtag-mode','sync']
            if scenario=='tag-aliases':selected_flags+=['--runtime-memtag-action','silent-log','err']
            crash=(guest['width'],guest['memory'],scenario.split('-')[2]) if scenario.startswith('mismatch-') else None
            run(guest['name']+'-'+engine,selected_flags,wasm,crash=crash,aliases=scenario=='tag-aliases')
    smoke=out/'no-grants-32-0.wasm';run('default-hidden',['--runtime-int'],smoke,False);run('policy-check',[*flags,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    for field in LIMITS:
        broken=flags.copy();at=next(i for i in range(len(broken)-4) if broken[i:i+4]==['--ros-scene-recording-limit','robot','recording',field]);del broken[at:at+5]
        run('cli-missing-'+field,[*broken,'--ros-scene-check'],success=False)
    for name,extra in [('duplicate-limit',['--ros-scene-recording-limit','robot','recording','segments','3']),
        ('unknown-limit',['--ros-scene-recording-limit','robot','recording','unknown','1']),('unlimited',['--ros-scene-recording-limit','robot','recording','segments','unlimited']),
        ('duplicate-definition',['--ros-scene-recording-definition-hex','robot','recording',DEFINITION.hex()]),
        ('duplicate-replay',['--ros-scene-replay','robot','replay','recording',REPLAY,REPLAY_SHA.hex(),'2'])]:
        run('cli-'+name,[*flags,*extra,'--ros-scene-check'],success=False)
    assert pins()==report['inputs'] and sha(runtime)==report['runtime_sha256'];report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
