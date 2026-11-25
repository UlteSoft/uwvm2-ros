#!/usr/bin/env python3
"""Actual media, bridge and directional device/control boundary census.

The device peer is a real framed PTY test receiver, not a physical robot. It
checks complete frame hashes and the exact scalar body before acknowledging.
All children inherit the existing 64 GiB zero-swap cgroup. There is no WASI
import, generic socket, guest path or guest-selected native symbol.
"""
import argparse, hashlib, json, os, pty, resource, select, signal, struct, subprocess, threading, time, tty
from pathlib import Path
from run_scene_vm import ROOT, GATE, policy, sha, verify_gate
from run_scene_recording import RecordingGuest, RecordingTagged, u64, blob, text
from run_scene_parameters import ParameterGuest, ParameterFP
from run_scene_memtag import add_private_instructions
from run_scene_buffers import layout
import run_scene_sensors as sensors

FAMILIES={'device':['open','submit','result','stop','describe'], 'control':['open','sample','submit','result','stop'],
          'media':['open','submit','take','close'], 'bridge':['create','start','stop','describe']}
NAMES=[family+'.'+op for family,ops in FAMILIES.items() for op in ops]
OPERATIONS=[dict(name=n,words=0,bytes=0) for n in NAMES+['buffer.create','buffer.write','buffer.seal','buffer.read','sensor.open','sensor.take']]
OPERATIONS += [dict(name='clock.create',words=1,bytes=8),dict(name='clock.read',words=1,bytes=32),dict(name='resource.close',words=1,bytes=0)]
def digest(value):return hashlib.sha256(value).digest()
def profile(name):return text(name)+blob(digest(name.encode()))
def grant(alias,kind,binding,actions):return ['--ros-scene-grant','robot',alias,kind,alias,binding,','.join(actions)]
def steady(flags):return flags+grant('steady','clock.steady','local',['clock.create','clock.read','resource.close'])
def base_guest(width,memory,fp=False):
    g=(ParameterFP if fp else ParameterGuest)(OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall'])
    g.check('call $abi',2);g.open('missing',13);g.steady=g.open('steady');g.call('clock.create',g.steady,[0]);g.clock=g.save('steady.clock')
    g.body.append(g.refresh(30000000000)+' global.set $overall');return g
def deadline(g,name,span):
    g.globals.add(name);g.body.append(g.refresh(span)+' global.set $'+name);return 'global.get $'+name
def boundaries(g,names,cap,wire):
    for name in names:
        for kw in [dict(memory=g.memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:
            g.raw(name,cap,wire,expected=22,**kw)

class RobotTagged(RecordingTagged):
    def raw(self,name,*,request_tag=None,output_tag=None,length_tag=None):
        # No family provider is configured. Memory validation still precedes
        # the closed, default-denied provider selection.
        self.write(128,1)
        self.check(f'i64.const 1 i32.const {self.memory} i64.const {self.pointer(128,request_tag)} i64.const 8 '
                   f'i64.const {self.pointer(512,output_tag)} i64.const 1024 i64.const {self.pointer(2048,length_tag)} call ${name}',13)

MEDIA_DEFINITION=(text('uwvm.ros.media.raw')+text('camera.convert')+text('rgb')+profile('rgb.profile')+text('bgr')+profile('bgr.profile')+
                  text('swap_red_blue')+text('rgb')+text('bgr')+u64(2)+u64(1)+u64(3))
MEDIA_PROFILE=text('camera.convert')+blob(digest(MEDIA_DEFINITION))
MEDIA_LIMITS={'streams':2,'frames':2,'retained.bytes':1048576,'workspace.bytes':65536,'workspace.calls':8,'deadline.ns':5000000000}
def media_flags():
    flags=steady(policy(OPERATIONS))+['--ros-scene-buffer-budget','robot','48','8','128','16','--ros-scene-media-budget','robot','1','1048576']
    for alias in ('rgb','bgr'):
        flags+=grant(alias,'buffer','sealed-buffer',['buffer.create','buffer.write','buffer.seal','buffer.read','resource.close','data.export'])
        flags+=['--ros-scene-buffer','robot',alias,alias+'.profile',digest((alias+'.profile').encode()).hex(),'24','4','6','6','64','8','3','6','1','1','255']
    flags+=grant('camera','media','immutable-buffer-media',['media.'+x for x in FAMILIES['media']]+['resource.close'])
    flags+=['--ros-scene-media-profile','robot','camera','camera.convert',digest(MEDIA_DEFINITION).hex(),
            '--ros-scene-media-definition-hex','robot','camera',MEDIA_DEFINITION.hex()]
    for key,value in MEDIA_LIMITS.items():flags+=['--ros-scene-media-limit','robot','camera',key,str(value)]
    return flags
def media_guest(width,memory,fp=False):
    g=base_guest(width,memory,fp);cap=g.open('camera');source=g.open('rgb');target=g.open('bgr')
    raw=bytes([1,2,3,4,5,6]);fmt=layout(identity='rgb.profile',digest=digest(b'rgb.profile'),shape=(1,2,3),strides=(6,3,1),color='rgb',size=6)
    g.raw('buffer.create',source,fmt);obj=g.save('rgb.frame')
    g.raw('buffer.write',source,bytes(16)+blob(raw),[(0,obj)]);g.raw('buffer.seal',source,bytes(8)+blob(digest(raw)),[(0,obj)])
    opening=MEDIA_PROFILE+bytes(24)+u64(0);patch=[(len(MEDIA_PROFILE),source),(len(MEDIA_PROFILE)+8,obj),(len(MEDIA_PROFILE)+16,target)]
    boundaries(g,['media.'+x for x in FAMILIES['media']],cap,opening)
    g.raw('media.open',cap,opening,patch,capacity=7,expected=90);g.raw('media.open',cap,opening,patch);stream=g.save('media.stream')
    until=deadline(g,'media.deadline',4000000000)
    shape=profile('rgb.profile')+text('image')+b''.join(map(u64,[1,2,1,3,0,1,0,6,6,1]))
    body=bytes(8)+u64(1)+bytes(24)+shape+bytes(8);patch=[(0,stream),(16,stream),(24,source),(32,obj),(len(body)-8,until)]
    g.raw('media.submit',cap,body,patch,capacity=1,expected=90);g.raw('media.submit',cap,body,patch);g.raw('media.submit',cap,body,patch)
    g.raw('media.take',cap,bytes(8),[(0,stream)],capacity=1,expected=90);g.raw('media.take',cap,bytes(8),[(0,stream)])
    g.exact(520,text('succeeded')+text('committed'))
    result_shape=profile('bgr.profile')+text('image')+b''.join(map(u64,[1,2,1,3,0,1,0,6,6,1]))
    g.exact(512+58+56,result_shape);at=512+58+56+len(result_shape)
    g.check(g.word(at)+' '+target+' i64.ne');result=g.save('bgr.frame',at+8)
    g.raw('buffer.read',target,bytes(8)+u64(0)+u64(6),[(0,result)],capacity=14);g.exact(512,u64(6)+bytes([3,2,1,6,5,4]))
    g.raw('media.take',cap,bytes(8),[(0,stream)],expected=11)
    close=bytes(8)+u64(1)+bytes(16);patch=[(0,stream),(16,stream),(24,until)]
    g.raw('media.close',cap,close,patch);g.raw('media.close',cap,close,patch)
    g.call('resource.close',source,[obj]);g.call('resource.close',target,[result]);return g

SENSOR_FRAME='logical'
DEVICE_DEFINITION=(text('uwvm.ros.device.scalar')+text('drive.plan')+profile('drive.mode')+profile('drive.watchdog')+profile('drive.stop')+
                   profile('drive.schema')+text(SENSOR_FRAME)+u64(1)+text('joint')+text('rad')+struct.pack('<dd',-1,1))
DEVICE_PROFILE=text('drive.plan')+blob(digest(DEVICE_DEFINITION))
CONTROL_DEFINITION=(text('uwvm.ros.control.scalar')+text('control.plan')+text('drive')+profile('drive.stop')+u64(1)+text('sensor')+
                    sensors.PROFILE_WIRE+text('frames')+text(sensors.BUFFER)+blob(sensors.BUFFER_SHA)+text(SENSOR_FRAME))
CONTROL_PROFILE=text('control.plan')+blob(digest(CONTROL_DEFINITION))
SCALAR=text(SENSOR_FRAME)+u64(1)+text('joint')+text('rad')+struct.pack('<d',0.5)
DEVICE_LIMITS={'handles':2,'requests':4,'command.bytes':256,'observations':1,'observations.minimum':1,'observation.age.ns':10000000000,
    'command.deadline.ns':2000000000,'ledger.bytes':1048576,'retained.bytes':2097152,'transport.write.bytes':4096,'stop.ack.ns':1000000000,
    'controller.lease.ns':10000000000,'controller.prepare.ns':1000000000,'controller.priority':10}
CONTROL_LIMITS={'lease.ns':8000000000,'priority':1,'write.window.ns':1000000000,'retained.bytes':2097152}
def device_flags(sensor_fds,device_fds):
    flags=sensors.configured(sensor_fds)
    for op in OPERATIONS:
        if op['name'] not in {o['name']for o in sensors.OPERATIONS}:flags+=['--ros-scene-interface','robot',op['name'],'allow']
    flags=steady(flags)
    # Trusted source buffers require independent read/export authority for control.
    at=flags.index('buffer.read,resource.close');flags[at]='buffer.read,resource.close,data.export'
    flags+=['--ros-scene-device-budget','robot','1','1','4194304','1048576','1000000','2000000000']
    for alias,kind,binding,definition,limits in [('drive','device','framed-scalar',DEVICE_DEFINITION,DEVICE_LIMITS),('controller','control','framed-control',CONTROL_DEFINITION,CONTROL_LIMITS)]:
        flags+=grant(alias,kind,binding,[kind+'.'+op for op in FAMILIES[kind]]+['resource.close'])
        ident='drive.plan'if kind=='device'else'control.plan'
        flags+=['--ros-scene-'+kind+'-profile','robot',alias,ident,digest(definition).hex(),'--ros-scene-'+kind+'-definition-hex','robot',alias,definition.hex()]
        for key,value in limits.items():flags+=['--ros-scene-'+kind+'-limit','robot',alias,key,str(value)]
    flags+=['--ros-scene-device-endpoints','robot','drive',*map(str,device_fds)]
    return flags
def receipt_done(g,name,cap,payload,patch):
    g.loop_id+=1;label='terminal'+str(g.loop_id)
    g.body.append('loop $'+label);g.raw(name,cap,payload,patch)
    g.body.append(g.word(520)+' i64.const 9 i64.eq if')
    g.exact(528,b'succeeded');g.body.append('else');g.bound();g.body.append('br $'+label+' end end')
def device_guest(width,memory,direct=False,fp=False):
    g=base_guest(width,memory,fp);sensor=g.open('sensor');frames=g.open('frames');drive=g.open('drive');control=g.open('controller')
    g.raw('sensor.open',sensor,sensors.PROFILE_WIRE);source=g.save('sensor.resource')
    g.polling('sensor.take',sensor,bytes(8),[(0,source)])
    boundaries(g,['device.'+x for x in FAMILIES['device']],drive,DEVICE_PROFILE)
    boundaries(g,['control.'+x for x in FAMILIES['control']],control,CONTROL_PROFILE)
    g.raw('device.open',drive,DEVICE_PROFILE,capacity=7,expected=90);g.raw('device.open',drive,DEVICE_PROFILE);device=g.save('device')
    opening=CONTROL_PROFILE+bytes(16)+u64(1)+u64(1)+bytes(24);offset=len(CONTROL_PROFILE)
    patch=[(offset,drive),(offset+8,device),(offset+32,sensor),(offset+40,source),(offset+48,frames)]
    g.raw('control.open',control,opening,patch,capacity=31,expected=90)
    g.dynamic('control.open',control,opening,patch)
    for code in [5,9,11,12,13,16,17,22,90,95,110,125]:g.check(f'global.get $status i32.const {code} i32.eq')
    g.check('global.get $status')
    cycle=g.save('cycle');epoch=g.save('controller.epoch',520)
    state_at=512+len(DEVICE_PROFILE)+len(profile('drive.schema'))+len(profile('drive.stop'))
    g.body.append('loop $ready');g.polling('device.describe',drive,bytes(8),[(0,device)])
    g.body.append(g.word(state_at)+' i64.const 5 i64.ne if');g.bound();g.body.append('br $ready end end');g.exact(state_at+8,b'ready')
    g.raw('control.sample',control,bytes(8),[(0,cycle)],capacity=1,expected=90)
    g.raw('control.sample',control,bytes(8),[(0,cycle)]);sequence=g.save('cycle.sequence',520)
    # Retain the exact original observation in guest memory for direct commands.
    observation_bytes=72+8+len(SENSOR_FRAME);g.body.append(f'i{width}.const 28000 i{width}.const 576 i{width}.const {observation_bytes} memory.copy $work $work')
    body=bytes(8)+u64(1)+bytes(24)+profile('drive.schema')+text('robot.scalar.command')+text('le64')+blob(SCALAR)
    patch=[(0,cycle),(16,cycle),(24,sequence),(32,epoch)]
    action='control.submit';result='control.result';result_patch=[(0,cycle),(8,sequence)]
    if direct:
        until=deadline(g,'command.deadline',900000000)
        body=bytes(8)+u64(1)+bytes(32)+profile('drive.mode')+profile('drive.schema')+text('robot.scalar.command')+text('le64')+blob(SCALAR)+u64(1)
        where=len(body);body+=bytes(observation_bytes)+u64(0)+bytes(8)
        patch=[(0,device),(16,device),(24,control),(32,cycle),(40,epoch),(len(body)-8,until)]
        patch += [(where+n,g.word(28000+n))for n in range(0,observation_bytes-7,8)]
        # The tail is the literal frame name; only complete LE words are dynamic.
        body=body[:where+80]+SENSOR_FRAME.encode()+body[where+observation_bytes:]
        action='device.submit';result='device.result'
    g.raw(action,drive if direct else control,body,patch,capacity=255,expected=90)
    g.raw(action,drive if direct else control,body,patch);request=g.save('command.request')
    g.raw(action,drive if direct else control,body,patch)
    if direct:result_patch=[(0,device),(8,request)]
    receipt_done(g,result,drive if direct else control,bytes(16),result_patch)
    stop=bytes(8)+u64(1)+bytes(8)+profile('drive.stop')
    receipt_done(g,'control.stop',control,stop,[(0,cycle),(16,cycle)])
    g.raw('device.stop',drive,stop,[(0,device),(16,device)])
    g.call('resource.close',drive,[request]);g.call('resource.close',control,[cycle]);g.call('resource.close',drive,[device]);return g

class Receiver:
    def __init__(self):
        self.pairs=[pty.openpty()for _ in range(4)];self.stop=threading.Event();self.errors=[];self.frames=[]
        for master,slave in self.pairs:tty.setraw(slave);os.set_blocking(master,False);os.set_blocking(slave,False)
        self.thread=threading.Thread(target=self.run);self.thread.start()
    def run(self):
        buffers={0:bytearray(),2:bytearray()}
        try:
            while not self.stop.is_set():
                ready,_,_=select.select([self.pairs[i][1]for i in buffers],[],[],0.01)
                for i,buffer in buffers.items():
                    if self.pairs[i][1]not in ready:continue
                    buffer+=os.read(self.pairs[i][1],16384)
                    while len(buffer)>=128:
                        fields=struct.unpack('<11Q',buffer[:88]);size=fields[-1];assert size<=9000
                        if len(buffer)<128+size:break
                        packet=bytes(buffer[:128+size]);del buffer[:128+size];body=packet[128:]
                        assert fields[0]==0x4543495645445255 and packet[88:96]==u64(32) and digest(packet[:88]+body)==packet[96:128]
                        kind=fields[1];assert kind in ((4,1)if i==0 else(2,))
                        if kind==1:assert body==profile('drive.schema')+SCALAR
                        if kind==2:assert body==profile('drive.stop')
                        ack_body=u64(kind)+u64({4:5,1:2,2:3}[kind])+blob(packet[96:128])
                        ack_prefix=b''.join(map(u64,[fields[0],3,fields[2],fields[3],fields[4],0,0,0,0,0,len(ack_body)]))
                        ack=ack_prefix+blob(digest(ack_prefix+ack_body))+ack_body
                        assert os.write(self.pairs[i+1][1],ack)==len(ack);self.frames.append(kind)
        except BaseException as error:self.errors.append(repr(error))
    def close(self):
        self.stop.set();self.thread.join()
        for pair in self.pairs:
            for fd in pair:os.close(fd)

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--runtime',type=Path)
    p.add_argument('--output',type=Path,required=True);p.add_argument('--mode',choices=['prepare','execute','all'],default='all');p.add_argument('--first-layout',action='store_true');a=p.parse_args()
    verify_gate();assert os.sched_getaffinity(0).issubset({4,5,6,7});resource.setrlimit(resource.RLIMIT_CORE,(0,0));source=a.source_root.resolve()
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    paths=[Path(__file__),*[Path(__file__).with_name(n)for n in ['run_scene_vm.py','run_scene_recording.py','run_scene_buffers.py','run_scene_parameters.py','run_scene_communication.py','run_scene_sensors.py','run_scene_fp.py','run_scene_memtag.py']]]
    inputs={str(p.resolve()):sha(p)for p in paths};inputs[str(source/'source-manifest.json')]=sha(source/'source-manifest.json')
    if a.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared']and report['inputs']==inputs
    else:
        assert not(out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,inputs=inputs,guests=[],cases=[],cgroup=str(GATE),inode=GATE.stat().st_ino,
            memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if a.mode!='execute':
        for width,memory in ([(32,0)]if a.first_layout else[(32,0),(64,0),(32,1),(64,1)]):
            scenarios=['media','media-fp','cycle','cycle-fp','device','denied','no-grants','tag-valid','tag-aliases']+['mismatch-'+str(i)+'-'+slot for i in range(len(NAMES))for slot in ('request','output','length')]
            for scenario in scenarios:
                tagged=scenario.startswith(('tag-','mismatch-'))
                if scenario.startswith('media'):g=media_guest(width,memory,scenario.endswith('-fp'))
                elif scenario.startswith(('cycle','device')):g=device_guest(width,memory,scenario=='device',scenario.endswith('-fp'))
                elif tagged:
                    g=RobotTagged(OPERATIONS,width,memory);g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
                    g.check('call $tag.status'+(' i32.wrap_i64'if width==64 else''),2)
                    if scenario.startswith('mismatch-'):
                        _,index,slot=scenario.split('-');g.raw(NAMES[int(index)],**{slot+'_tag':5})
                    else:
                        for name in NAMES:g.raw(name,request_tag=0 if scenario=='tag-aliases'else 3,output_tag=0 if scenario=='tag-aliases'else 3,length_tag=0 if scenario=='tag-aliases'else 3)
                else:
                    g=RecordingGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied'else 2);g.open('missing',13)
                    for name in NAMES:g.raw(name,1,bytes(8),expected=13)
                assert g.cursor<45000,(scenario,g.cursor)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                argv=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                done=subprocess.run(argv,capture_output=True,text=True,timeout=30);assert not done.returncode,done.stderr
                if tagged:wasm.write_bytes(add_private_instructions(wasm.read_bytes(),len(g.checks),memory))
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,tagged=tagged,assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0),sha256=sha(wasm),wat_sha256=sha(wat)));save()
        report['prepared']=True;save()
    if a.mode=='prepare':print('PASS robot local preparation: '+str(len(report['guests'])));return
    if not a.runtime:p.error('--runtime required')
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text())
    assert receipt['complete']and receipt['binary_sha256']==sha(runtime)and receipt['source_root']==str(source)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),build_sha256=sha(build))
    def run(name,flags,wasm=None,expected=0,fds=(),crash=None,aliases=False):
        verify_gate();argv=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*flags]
        if wasm:argv+=['--run',str(wasm)]
        start=time.monotonic();done=subprocess.run(argv,capture_output=True,text=True,pass_fds=fds,timeout=90)
        (out/(name+'.stdout')).write_text(done.stdout);(out/(name+'.stderr')).write_text(done.stderr)
        passed=done.returncode==expected if expected is not None else done.returncode!=0
        if crash:
            width,memory,slot=crash;pointer={'request':128,'output':512,'length':2048}[slot]|(5<<(28 if width==32 else 56))
            passed=done.returncode==-signal.SIGILL and all(t in done.stderr for t in ['[crash] memory-protection','Exit status: 122',f'Memory index: {memory}',f'Pointer: {pointer}','Pointer tag: 5','Allocation tag: 3'])
        if aliases:
            violations=[json.loads(line)for line in done.stderr.splitlines()if line.startswith('{"schema":"uwvm.memtag.violation.')]
            passed=passed and len(violations)>=66 and all(v['pointer_tag']==0 and v['allocation_tag']==3 for v in violations)
        report['cases'].append(dict(name=name,argv=argv,exit=done.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,done.returncode,done.stderr[-6000:]);print('PASS '+name,flush=True)
    run('media-static',[*media_flags(),'--ros-scene-check'])
    run('device-static',device_flags([2147483640,2147483641,2147483642],[2147483643,2147483644,2147483645,2147483646])+['--ros-scene-check'])
    for guest in report['guests']:
        scenario=guest['scenario'];wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256']and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        for engine in ('int','aot'):
            pipes=[];receiver=None
            try:
                fds=()
                if scenario.startswith('media'):flags=media_flags()
                elif scenario.startswith(('cycle','device')):
                    pipes=[os.pipe2(os.O_NONBLOCK|os.O_CLOEXEC)for _ in sensors.ALIASES]
                    for rd,wr in pipes:assert os.write(wr,sensors.PACKET)==len(sensors.PACKET)
                    receiver=Receiver();sensor_fds=tuple(rd for rd,wr in pipes);device_fds=tuple(master for master,slave in receiver.pairs)
                    fds=sensor_fds+device_fds;flags=device_flags(sensor_fds,device_fds)
                else:flags=policy(OPERATIONS,'deny'if scenario=='denied'else'allow')
                if guest['tagged']:flags+=['--wasm-policy-global', 'runtime.memtag-backend','software','--runtime-memtag-mode','sync']
                if scenario=='tag-aliases':flags+=['--runtime-memtag-action','silent-log','err']
                crash=(guest['width'],guest['memory'],scenario.split('-')[2])if scenario.startswith('mismatch-')else None
                run(guest['name']+'-'+engine,['--runtime-'+engine,*flags],wasm,fds=fds,crash=crash,aliases=scenario=='tag-aliases')
                if receiver:assert not receiver.errors and receiver.frames==[4,1,2],(receiver.errors,receiver.frames)
            finally:
                if receiver:receiver.close()
                for pair in pipes:
                    for fd in pair:os.close(fd)
    for family,limits in [('media',MEDIA_LIMITS),('device',DEVICE_LIMITS),('control',CONTROL_LIMITS)]:
        original=media_flags()if family=='media'else device_flags([101,102,103],[104,105,106,107]);alias={'media':'camera','device':'drive','control':'controller'}[family]
        for key in limits:
            bad=original.copy();at=next(i for i in range(len(bad)-4)if bad[i:i+4]==['--ros-scene-'+family+'-limit','robot',alias,key]);del bad[at:at+5]
            run('missing-'+family+'-'+key,[*bad,'--ros-scene-check'],expected=None)
    report['inputs_unchanged']=all(sha(Path(p))==value for p,value in inputs.items());assert report['inputs_unchanged']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()

if __name__=='__main__':main()
