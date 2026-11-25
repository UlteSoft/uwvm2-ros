#!/usr/bin/env python3
"""Actual Wasm receive-only sensors: trusted pipes, four memories, FP and MTE.

The writer supplies complete framed native bytes through inherited read-only
nonblocking descriptors. No WASI import, generic socket or filesystem is used.
This qualifies the closed octets binding, not native ROS messages or devices.
All compilers and VM children must remain in the existing 64 GiB zero-swap gate.
"""
import argparse,array,fcntl,hashlib,json,os,resource,signal,subprocess,termios,time
from pathlib import Path
from run_scene_vm import ROOT,GATE,policy,sha,verify_gate
from run_scene_recording import RecordingGuest,RecordingFP,RecordingTagged,u64,blob,text
from run_scene_memtag import add_private_instructions

NAMES=['sensor.open','sensor.take','sensor.describe']
OPERATIONS=[dict(name=n,words=0,bytes=0) for n in NAMES+['buffer.read']]+[dict(name='resource.close',words=1,bytes=0)]
ALIASES=['sensor','no-export','rate']
SCHEMA='ros.sensor.octets'
SCHEMA_SHA=hashlib.sha256(b'uwvm.ros.sensor.octets:nonempty opaque bytes; maximum=4194304; no implicit ROS message or physical schema').digest()
BUFFER='sensor.buffer';BUFFER_SHA=bytes([7])+bytes(31)
PROFILE='sensor.actual';TYPE='ros.sensor.bytes';ENCODING='octets';PAYLOAD=bytes([9,8,7,6])
DEFINITION=text('uwvm.ros.sensor.definition')+text(PROFILE)+text(SCHEMA)+blob(SCHEMA_SHA)+text(TYPE)+text(ENCODING)+text('logical')+text('count')+text('frames')+text(BUFFER)+blob(BUFFER_SHA)+b''.join(map(u64,[101,1,102,1,0,0,1,0]))+text('reject')
PROFILE_SHA=hashlib.sha256(DEFINITION).digest()
PROFILE_WIRE=text(PROFILE)+blob(PROFILE_SHA)
LIMITS={'source.handles':2,'source.records':2,'frame.bytes':4,'source.storage.bytes':65536,'history.frames':2,'buffer.objects':8,
        'ingress.frame.bytes':4,'ingress.read.bytes':4096,'ingress.retained.bytes':4096,'workspace.bytes':65536,'workspace.calls':8}
PREFIX=b''.join(map(u64,[0x524f534e45535255,4,101,1,1,0,0,0,0]))+blob(SCHEMA_SHA)
PACKET=PREFIX+blob(hashlib.sha256(PREFIX+PAYLOAD).digest())+PAYLOAD
assert len(PREFIX)==112 and len(PACKET)==156

def configured(descriptors):
    flags=policy(OPERATIONS)+['--ros-scene-buffer-budget','robot','32','8','64','64',
        '--ros-scene-grant','robot','frames','buffer','frames','sealed-buffer','buffer.read,resource.close',
        '--ros-scene-buffer','robot','frames',BUFFER,BUFFER_SHA.hex(),'32','8','4','4','64','64','1','4','1','1','1',
        '--ros-scene-sensor-budget','robot','3','1048576']
    for alias,fd in zip(ALIASES,descriptors):
        actions=NAMES+['resource.observe','resource.close']+([] if alias=='no-export' else ['data.export'])
        flags+=['--ros-scene-grant','robot',alias,'sensor',alias,'bounded-sensor',','.join(actions),
            '--ros-scene-sensor-profile','robot',alias,PROFILE,PROFILE_SHA.hex(),'octets',
            '--ros-scene-sensor-definition-hex','robot',alias,DEFINITION.hex(),
            '--ros-scene-sensor-fd','robot',alias,str(fd)]
        for field,value in LIMITS.items():flags+=['--ros-scene-sensor-limit','robot',alias,field,str(value)]
    flags+=['--ros-scene-limit','robot','rate','delivery','may-reject','--ros-scene-limit','robot','rate','calls-per-window','0']
    return flags

def successful_guest(width,memory,fp=False):
    g=(RecordingFP if fp else RecordingGuest)(OPERATIONS,width,memory);g.check('call $abi',2);g.open('missing',13)
    caps={alias:g.open(alias) for alias in ALIASES+['frames']};cap=caps['sensor']
    for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:
        g.raw('sensor.open',cap,PROFILE_WIRE,expected=22,**kw)
    g.raw('sensor.open',cap,PROFILE_WIRE,capacity=7,expected=90);g.equal(g.word(2048),8)
    for malformed in [PROFILE_WIRE[:-1],PROFILE_WIRE+b'x',text('')+blob(PROFILE_SHA)]:g.raw('sensor.open',cap,malformed,expected=22)
    g.raw('sensor.open',cap,text('wrong')+blob(PROFILE_SHA),expected=13)
    g.raw('sensor.open',caps['rate'],PROFILE_WIRE,expected=11)
    g.raw('sensor.open',caps['no-export'],PROFILE_WIRE);denied=g.save('denied-sensor')
    g.raw('sensor.take',caps['no-export'],u64(0),[(0,denied)],expected=13)
    g.raw('sensor.open',cap,PROFILE_WIRE);sensor=g.save('sensor-resource')
    g.raw('sensor.describe',caps['no-export'],u64(0),[(0,sensor)],expected=9)
    def take(**kwargs):g.raw('sensor.take',cap,u64(0),[(0,sensor)],**kwargs)
    take(capacity=1,expected=90)
    take(expected=11) # Exactly one native read consumes the header only.
    take()
    g.check(g.word(512)+' '+sensor+' i64.ne');g.exact(520,b''.join(map(u64,[1,1,0,1,102,1])))
    g.check(g.word(568)+' i64.eqz');g.exact(576,bytes(24))
    record=text('sensor.frame')+text(TYPE)+text(ENCODING)+blob(PAYLOAD)
    g.exact(600,record);buffer_offset=600+len(record);buffer=g.save('buffer',buffer_offset)
    suffix=text(SCHEMA)+blob(SCHEMA_SHA)+text(BUFFER)+blob(BUFFER_SHA)+text('logical')+text('count')+u64(101)+u64(1)+u64(1)
    g.exact(buffer_offset+8,suffix);g.equal(g.word(2048),buffer_offset+8+len(suffix)-512)
    g.raw('buffer.read',caps['frames'],u64(0)+u64(0)+u64(4),[(0,buffer)],capacity=12);g.exact(512,u64(4)+PAYLOAD)
    take(expected=11)
    g.raw('sensor.describe',cap,u64(0),[(0,sensor)],capacity=1,expected=90)
    g.raw('sensor.describe',cap,u64(0),[(0,sensor)]);g.exact(512,PROFILE_WIRE+text('ros.sensor.state')+text('le64'))
    g.call('resource.close',cap,[sensor]);take(expected=9)
    g.call('resource.close',caps['no-export'],[denied]);g.check(cap+' call $close')
    g.raw('sensor.open',cap,PROFILE_WIRE,expected=9)
    assert g.cursor<45000
    return g

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
    args=parser.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({20,21});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    if args.mode!='prepare' and not args.runtime:parser.error('--runtime required')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    source=args.source_root.resolve();contracts=[source/'src/uwvm2/imported/ros/scene'/n for n in ['sensor_contract.h','sensor_codec.h','sensor_configuration.h','sensor_deployment_configuration.h','sensor_ingress.h']]
    paths=[Path(__file__).resolve(),*[Path(__file__).with_name(n) for n in ['run_scene_vm.py','run_scene_recording.py','run_scene_buffers.py','run_scene_fp.py','run_scene_memtag.py']]]
    pins=lambda:{str(p):sha(p)for p in paths};contract_pins=lambda:{p.name:sha(p)for p in contracts}
    if args.mode=='execute':
        report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['inputs']==pins() and report['contracts']==contract_pins()
    else:
        assert not(out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,inputs=pins(),contracts=contract_pins(),
            cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if args.mode!='execute':
        scenarios=['success','fp','denied','no-grants','tag-valid','tag-aliases']+['mismatch-'+str(i)+'-'+slot for i in range(3)for slot in ['request','output','length']]
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for scenario in scenarios:
                tagged=scenario.startswith(('tag-','mismatch-'))
                if scenario in ['success','fp']:g=successful_guest(width,memory,scenario=='fp')
                elif tagged:g=tagged_guest(width,memory,scenario)
                else:
                    g=RecordingGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied' else 2);g.open('sensor',13)
                    for name in NAMES:g.raw(name,1,u64(1),expected=13)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                result=subprocess.run(cmd,capture_output=True,text=True,timeout=30);assert not result.returncode,result.stderr
                if tagged:wasm.write_bytes(add_private_instructions(wasm.read_bytes(),len(g.checks),memory))
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,tagged=tagged,compile=cmd,
                    assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0),wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if args.mode=='prepare':print('PASS sensor guest preparation: '+str(len(report['guests'])));return
    runtime=args.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text())
    assert receipt['complete'] and receipt['binary_sha256']==sha(runtime) and receipt['source_root']==str(source)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),source_root=str(source),build_receipt_sha256=sha(build),source_manifest_sha256=sha(source/'source-manifest.json'))
    def run(name,flags,wasm=None,success=True,descriptors=(),crash=None,aliases=False,queues=None):
        verify_gate();cmd=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*flags]
        if wasm:cmd+=['--run',str(wasm)]
        start=time.monotonic();result=subprocess.run(cmd,capture_output=True,pass_fds=descriptors,timeout=120);errors=result.stderr.decode(errors='replace')
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr);passed=(result.returncode==0)==success
        if crash:
            width,memory,slot=crash;pointer={'request':128,'output':512,'length':2048}[slot]|(5<<(28 if width==32 else 56))
            passed=result.returncode==-signal.SIGILL and all(t in errors for t in ['[crash] memory-protection','Exit status: 122',f'Memory index: {memory}',f'Pointer: {pointer}','Pointer tag: 5','Allocation tag: 3'])
        violations=[]
        if aliases:
            violations=[json.loads(line)for line in errors.splitlines()if line.startswith('{"schema":"uwvm.memtag.violation.')]
            passed=passed and len(violations)>=9 and all(v['pointer_tag']==0 and v['allocation_tag']==3 and v['pointer']in[128,512,2048]for v in violations)
        pending=[]
        if queues is not None:
            for fd in descriptors:
                value=array.array('i',[0]);fcntl.ioctl(fd,termios.FIONREAD,value,True);pending.append(value[0])
            passed=passed and pending==queues
        report['cases'].append(dict(name=name,argv=cmd,exit=result.returncode,passed=passed,seconds=time.monotonic()-start,violations=violations,pending_bytes=pending));save()
        assert passed,(name,result.returncode,pending,errors[-7000:]);print('PASS '+name,flush=True)
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256'] and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        scenario=guest['scenario']
        for engine in ['int','aot']:
            pipes=[]
            try:
                native=scenario in ['success','fp'] or guest['tagged']
                if native:
                    pipes=[os.pipe2(os.O_NONBLOCK|os.O_CLOEXEC)for _ in ALIASES]
                    for rd,wr in pipes:assert os.write(wr,PACKET)==len(PACKET)
                descriptors=tuple(rd for rd,wr in pipes)
                selected=configured(descriptors)if native else policy(OPERATIONS,'deny'if scenario=='denied'else'allow')
                flags=['--runtime-'+engine,*selected]
                if guest['tagged']:flags+=['--wasm-policy-global', 'runtime.memtag-backend','software','--runtime-memtag-mode','sync']
                if scenario=='tag-aliases':flags+=['--runtime-memtag-action','silent-log','err']
                crash=(guest['width'],guest['memory'],scenario.split('-')[2])if scenario.startswith('mismatch-')else None
                queues=[0,len(PACKET),len(PACKET)]if scenario in ['success','fp']else [len(PACKET)]*3 if native else None
                run(guest['name']+'-'+engine,flags,wasm,descriptors=descriptors,crash=crash,aliases=scenario=='tag-aliases',queues=queues)
            finally:
                for pair in pipes:
                    for fd in pair:os.close(fd)
    # Deliberately invalid descriptor identities remain unprobed by static mode.
    static=configured([2147483645,2147483646,2147483647]);smoke=out/'no-grants-32-0.wasm'
    run('policy-check',[*static,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*static],smoke)
    run('default-hidden',['--runtime-int'],smoke,False)
    for field in LIMITS:
        broken=static.copy();at=next(i for i in range(len(broken)-4)if broken[i:i+4]==['--ros-scene-sensor-limit','robot','sensor',field]);del broken[at:at+5]
        run('missing-'+field,[*broken,'--ros-scene-check'],success=False)
    assert pins()==report['inputs'] and contract_pins()==report['contracts'] and sha(runtime)==report['runtime_sha256']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
