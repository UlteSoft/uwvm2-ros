#!/usr/bin/env python3
"""Actual Wasm sensor -> isolated echo model -> inert lifecycle proposal chain.

The native worker receives the real sealed input and emits a complete proposal.
It grants no authority. Publication authenticates the actual successful model
completion; acceptance records a receipt without submitting a lifecycle change.
This qualifies a bounded nonphysical test backend, not LLM accuracy or hardware.
All compilation and execution stays in the existing 64 GiB zero-swap cgroup.
"""
import argparse,array,fcntl,hashlib,json,os,resource,shutil,signal,subprocess,termios,time
from pathlib import Path
from run_scene_vm import ROOT,GATE,policy,sha,verify_gate
from run_scene_recording import RecordingGuest,RecordingFP,RecordingTagged,u64,blob,text
from run_scene_memtag import add_private_instructions
import run_scene_sensors as sensor
from run_scene_models import IMAGES,LIMITS as WORKER_LIMITS

NAMES=['proposal.capture','proposal.publish','proposal.read','proposal.accept']
EXTRA=['model.open','model.submit','model.take','model.cancel','model.describe','request.describe','request.release',
       'lifecycle.open','lifecycle.state','lifecycle.submit',*NAMES]
OPERATIONS=sensor.OPERATIONS+[dict(name=n,words=0,bytes=0)for n in EXTRA]+[
    dict(name='clock.create',words=1,bytes=8),dict(name='clock.read',words=1,bytes=32)]
SHA=bytes([7])+bytes(31)
def profile(name,digest=SHA):return text(name)+blob(digest)
LIFECYCLE=(4,60000000000,254,3)
TARGET_SHA=hashlib.sha256(text('uwvm.ros.local-lifecycle.target')+text('local.node')+text('node')+b''.join(map(u64,LIFECYCLE))).digest()
SCHEMA_SHA=hashlib.sha256(b'uwvm.ros.local-lifecycle.submit:le64(handle>0,key>0,transition=1..7,expected_generation>0,steady_deadline=1..INT64_MAX)').digest()
DEFINITION=(text('uwvm.ros.proposal.definition')+text('proposal.policy')+text('brain')+profile('local.echo')+profile('proposal.task')+u64(1)+
    text('sensor')+sensor.PROFILE_WIRE+text('frames')+profile(sensor.BUFFER,sensor.BUFFER_SHA)+text('logical')+u64(1)+
    profile('lifecycle.configure')+profile('ros.lifecycle.submit',SCHEMA_SHA)+profile('local.node',TARGET_SHA)+text('node')+
    text('lifecycle.submit')+text('non_physical')+text('latest_required')+text('ros.lifecycle.submit')+text('le64')+text('logical')+u64(60000000000)+u64(1))
POLICY_SHA=hashlib.sha256(DEFINITION).digest()
PROPOSAL_LIMITS=dict(zip(['proposals','argument.bytes','observations','readers','tools','sources','lifetime.ns','accept.deadline.ns','captures','workspace.bytes','workspace.calls'],
    [4,4096,1,4,1,1,60000000000,60000000000,4,65536,16]))

def configured(descriptors,images):
    flags=policy(OPERATIONS)+sensor.configured(descriptors)[len(policy(sensor.OPERATIONS)):]
    flags+=['--ros-scene-grant','robot','steady','clock.steady','steady','local','clock.create,clock.read,resource.close',
        '--ros-scene-grant','robot','brain','model.language','brain','local-model',','.join(EXTRA[:7]+['resource.close']),
        '--ros-scene-model','robot','brain','local.echo',SHA.hex(),'proposal.task',SHA.hex(),'proposal.envelope','ros.proposal','bytes','terminal',
        '--ros-scene-model-capacity','robot','brain','1','4','16','1','1024','8192','4','65536','60000000000',
        '--ros-scene-model-budget','robot','1','4','16','8192','4','65536']
    for key,value in dict(WORKER_LIMITS,**{'request-bytes':16384,'response-bytes':16384}).items():
        flags+=['--ros-scene-model-worker-limit','robot','brain',key,str(value)]
    launcher=images/'bwrap';flags+=['--ros-scene-model-worker-launcher','robot','brain',str(launcher),str(launcher.stat().st_size),sha(launcher)]
    for filename,role,destination in IMAGES:
        path=images/filename;flags+=['--ros-scene-model-worker-image','robot','brain',role,destination,str(path),str(path.stat().st_size),sha(path)]
    flags+=['--ros-scene-grant','robot','node','lifecycle','node','local-lifecycle','lifecycle.open,lifecycle.state,lifecycle.submit,resource.close',
        '--ros-scene-lifecycle','robot','node',*map(str,LIFECYCLE),
        '--ros-scene-grant','robot','suggestions','proposal','suggestions','model-proposals','proposal.capture,proposal.publish,proposal.read,resource.close',
        '--ros-scene-grant','robot','judge','arbiter','judge','proposal-arbiter','proposal.accept,request.describe,request.release',
        '--ros-scene-proposal-budget','robot','1','1','2','4','1048576',
        '--ros-scene-proposal-profile','robot','suggestions','proposal.policy',POLICY_SHA.hex(),'brain',
        '--ros-scene-proposal-definition-hex','robot','suggestions',DEFINITION.hex(),
        '--ros-scene-proposal-arbiter','robot','judge','suggestions','node','1']
    for name,value in PROPOSAL_LIMITS.items():flags+=['--ros-scene-proposal-limit','robot','suggestions',name,str(value)]
    return flags

def successful_guest(width,memory,fp=False):
    g=(RecordingFP if fp else RecordingGuest)(OPERATIONS,width,memory);g.check('call $abi',2)
    caps={n:g.open(n)for n in ['sensor','frames','brain','node','suggestions','judge','steady']};cap=caps['suggestions']
    g.call('clock.create',caps['steady'],[0]);clock=g.save('clock')
    g.call('clock.read',caps['steady'],[clock],output=3072,length=3104)
    g.globals.add('deadline');g.body.append(g.word(3072)+' i64.const 50000000000 i64.add global.set $deadline');deadline='global.get $deadline'
    def bounded_wait():
        g.call('clock.read',caps['steady'],[clock],output=3072,length=3104)
        g.check(g.word(3072)+' '+deadline+' i64.ge_u')
    g.raw('sensor.open',caps['sensor'],sensor.PROFILE_WIRE);source=g.save('source')
    g.raw('sensor.take',caps['sensor'],bytes(8),[(0,source)],expected=11)
    g.raw('sensor.take',caps['sensor'],bytes(8),[(0,source)])
    g.raw('lifecycle.open',caps['node'],u64(1));target=g.save('target')
    def unchanged():
        g.raw('lifecycle.state',caps['node'],bytes(8),[(0,target)]);g.exact(512,u64(1)+u64(1)+bytes(40))
    unchanged()
    capture=bytes(8)+u64(1)+profile('proposal.policy',POLICY_SHA)+u64(1)+u64(0)+bytes(32)
    source_offset=len(capture)-32
    patches=[(0,cap),(source_offset,caps['sensor']),(source_offset+8,source),(source_offset+16,caps['frames']),(source_offset+24,deadline)]
    for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:
        g.raw('proposal.capture',cap,capture,patches,expected=22,**kw)
    g.raw('proposal.capture',cap,capture,patches,capacity=31,expected=90);g.equal(g.word(2048),32)
    g.raw('proposal.capture',caps['frames'],capture,patches,expected=13)
    g.raw('proposal.capture',cap,capture,patches);captured=g.save('capture');g.equal(g.word(520),1)
    buffer_cap=g.save('captured-cap',528);buffer=g.save('captured-buffer',536)
    g.check(buffer_cap+' '+caps['frames']+' i64.ne')
    g.raw('proposal.capture',cap,capture,patches);g.check(g.word(512)+' '+captured+' i64.ne')
    g.raw('model.open',caps['brain'],profile('local.echo'));session=g.save('session')
    envelope_prefix=text('uwvm.ros.model.proposal')+profile('lifecycle.configure')+profile('ros.lifecycle.submit',SCHEMA_SHA)+text('node')+profile('local.node',TARGET_SHA)
    envelope_middle=text('lifecycle.submit')+text('logical')+text('ros.lifecycle.submit')+text('le64')
    argument=bytes(8)+u64(1)+u64(1)+u64(1)+bytes(8)
    envelope=envelope_prefix+bytes(8)+envelope_middle+blob(argument)
    argument_offset=len(envelope)-40
    # Scratch covers the encoded request AND the independently bounded worker
    # response frame. A 16 KiB response limit alone cannot fit a 16 KiB scratch.
    submit=bytes(8)+u64(1)+bytes(8)+profile('proposal.task')+text('proposal.envelope')+text('bytes')+blob(envelope)+u64(1)+bytes(48)+u64(8192)+u64(1)+u64(32768)+bytes(8)
    envelope_offset=len(submit)-(8+48+32)-len(envelope)
    input_offset=len(submit)-32-48
    submit_patches=[(0,session),(16,session),(envelope_offset+len(envelope_prefix),target),
        (envelope_offset+argument_offset,target),(envelope_offset+argument_offset+32,deadline),
        (input_offset,buffer_cap),(input_offset+8,buffer),(len(submit)-8,deadline)]
    g.raw('model.submit',caps['brain'],submit,submit_patches);model_request=g.save('model-request')
    g.body.append('block $complete loop $waiting')
    g.raw('request.describe',caps['brain'],bytes(8),[(0,model_request)])
    state_byte=f'i{width}.const 528 i32.load8_u $work'
    g.body.append(state_byte+' i32.const 113 i32.eq '+state_byte+' i32.const 114 i32.eq i32.or i32.eqz br_if $complete')
    bounded_wait();g.body.append('br $waiting end end')
    g.exact(520,text('succeeded'))
    g.raw('model.take',caps['brain'],bytes(16),[(0,session),(8,model_request)],capacity=8192,output=40000,length=2048)
    # Preserve the actual complete terminal output, not a regenerated argument hash.
    actual=g.word(2048)+f' i64.const {40000-len(envelope)} i64.add'+(' i32.wrap_i64'if width==32 else '')
    g.body.append(f'i{width}.const 33000 {actual} i{width}.const {len(envelope)} memory.copy $work $work')
    g.exact(33000,envelope_prefix);g.check(g.word(33000+len(envelope_prefix))+' '+target+' i64.ne')
    g.exact(33000+len(envelope_prefix)+8,envelope_middle+u64(40))
    def publish(**kwargs):
        request=bytes(8)+u64(1)+bytes(16)+blob(bytes(len(envelope)))+bytes(8)
        g.raw('proposal.publish',cap,request,[(0,cap),(16,captured),(24,model_request),(len(request)-8,deadline)],**kwargs)
        g.body.insert(-1,f'i{width}.const {g.last_request+40} i{width}.const 33000 i{width}.const {len(envelope)} memory.copy $work $work')
    publish(capacity=7,expected=90)
    # A different complete output must not be accepted as the completed job.
    g.write(33000+argument_offset+8,2);publish(expected=17);g.write(33000+argument_offset+8,1)
    publish();proposal=g.save('proposal');publish();g.check(g.word(512)+' '+proposal+' i64.ne')
    g.raw('proposal.read',caps['frames'],bytes(8),[(0,proposal)],expected=13)
    g.raw('proposal.read',cap,bytes(8),[(0,proposal)],capacity=1,expected=90)
    g.raw('proposal.read',cap,bytes(8),[(0,proposal)]);g.check(g.word(512)+' '+model_request+' i64.ne')
    g.exact(520,profile('lifecycle.configure')+text('ros.lifecycle.submit')+text('le64')+u64(40))
    accept=bytes(8)+u64(1)+bytes(40);accept_patches=[(0,caps['judge']),(16,cap),(24,proposal),(32,caps['node']),(40,target),(48,deadline)]
    g.raw('proposal.accept',caps['judge'],accept,accept_patches,capacity=1,expected=90)
    g.raw('proposal.accept',cap,accept,accept_patches,expected=13)
    wrong=[(o,caps['frames']if o==32 else v)for o,v in accept_patches]
    g.raw('proposal.accept',caps['judge'],accept,wrong,expected=13)
    g.raw('proposal.accept',caps['judge'],accept,accept_patches);receipt=g.save('receipt')
    receipt_text=text('admitted')+text('locally_accepted');g.exact(520,receipt_text);unchanged()
    g.raw('proposal.accept',caps['judge'],accept,accept_patches);g.check(g.word(512)+' '+receipt+' i64.ne')
    g.raw('request.describe',caps['judge'],bytes(8),[(0,receipt)]);g.exact(520,receipt_text)
    g.raw('request.describe',caps['brain'],bytes(8),[(0,receipt)],expected=9)
    g.raw('request.release',caps['judge'],bytes(8),[(0,receipt)]);g.equal(g.word(512),1)
    g.raw('request.release',caps['judge'],bytes(8),[(0,receipt)]);g.equal(g.word(512),0)
    g.raw('request.describe',caps['judge'],bytes(8),[(0,receipt)],expected=9)
    g.raw('proposal.accept',caps['judge'],accept,accept_patches);g.check(g.word(512)+' '+receipt+' i64.ne');unchanged()
    g.call('resource.close',cap,[captured]);g.call('resource.close',cap,[proposal])
    g.raw('proposal.read',cap,bytes(8),[(0,proposal)],expected=9)
    assert g.cursor<30000,(width,memory,fp,g.cursor)
    return g

class ProposalTagged(RecordingTagged):
    def raw(self,name,*,request_tag=None,output_tag=None,length_tag=None):
        # accept has seven u64 fields. Use its valid carrier extent so this
        # negative test reaches capability rejection rather than size rejection.
        # All three tagged memory ranges are still checked before authority.
        self.write(128,1)
        size=56 if name=='proposal.accept'else 8
        self.check(f'i64.const 1 i32.const {self.memory} i64.const {self.pointer(128,request_tag)} i64.const {size} '
                   f'i64.const {self.pointer(512,output_tag)} i64.const 1024 i64.const {self.pointer(2048,length_tag)} call ${name}',9)

def tagged_guest(width,memory,scenario):
    g=ProposalTagged(OPERATIONS,width,memory);g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
    g.check('call $tag.status'+(' i32.wrap_i64'if width==64 else ''),2)
    if scenario.startswith('mismatch-'):
        _,index,slot=scenario.split('-');g.raw(NAMES[int(index)],**{slot+'_tag':5})
    else:
        for name in NAMES:g.raw(name,request_tag=0 if scenario=='tag-aliases'else 3,output_tag=0 if scenario=='tag-aliases'else 3,length_tag=0 if scenario=='tag-aliases'else 3)
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--mode',choices=['prepare','execute','all'],default='all')
    p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--images',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    args=p.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({16,17});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    if args.mode!='prepare'and not args.runtime:p.error('--runtime required')
    source=args.source_root.resolve();out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    inputs=[Path(__file__).resolve(),*[Path(__file__).with_name(n)for n in ['proposal_worker_backend.cpp','run_scene_vm.py','run_scene_recording.py','run_scene_buffers.py','run_scene_fp.py','run_scene_memtag.py','run_scene_sensors.py','run_scene_models.py']]]
    pins=lambda:{str(path):sha(path)for path in inputs};images=out/'images';manifest=source/'source-manifest.json'
    def gate():
        verify_gate();assert (GATE/'memory.max').read_text().strip()=='68719476736'and(GATE/'memory.swap.max').read_text().strip()=='0'
    if args.mode=='execute':
        report=json.loads((out/'results.json').read_text());assert report['prepared']and report['inputs']==pins()and report['source_manifest_sha256']==sha(manifest)
    else:
        assert not(out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,inputs=pins(),source_root=str(source),source_manifest_sha256=sha(manifest),
            cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests=[],cases=[],commands=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if args.mode!='execute':
        base=args.images.resolve();base_receipt=json.loads((base.parent/'results.json').read_text());assert base_receipt['complete']
        report['base_images_receipt_sha256']=sha(base.parent/'results.json');images.mkdir();report['base_images']={}
        for filename,_,_ in IMAGES+[('bwrap','','')]:
            if filename=='backend.so':continue
            original=base/filename;report['base_images'][str(original)]=sha(original);shutil.copyfile(original,images/filename);(images/filename).chmod(0o500)
        backend=out/'proposal_worker_backend.cpp';shutil.copyfile(Path(__file__).with_name(backend.name),backend)
        cmd=['/usr/bin/clang++-22','-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-fPIC','-shared','-I'+str(source/'src'),'-I'+str(source/'third-parties/fast_io/include'),str(backend),'-o',str(images/'backend.so')]
        gate();done=subprocess.run(cmd,capture_output=True,text=True,timeout=120);(out/'backend.stderr').write_text(done.stderr);report['commands'].append(dict(name='backend',argv=cmd,exit=done.returncode));save();assert done.returncode==0,done.stderr
        report['images']={str(path):sha(path)for path in images.iterdir()};scenarios=['success','fp','denied','no-grants','tag-valid','tag-aliases']+['mismatch-'+str(i)+'-'+slot for i in range(4)for slot in ['request','output','length']]
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for scenario in scenarios:
                tagged=scenario.startswith(('tag-','mismatch-'))
                if scenario in ['success','fp']:g=successful_guest(width,memory,scenario=='fp')
                elif tagged:g=tagged_guest(width,memory,scenario)
                else:
                    g=RecordingGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied'else 2);g.open('suggestions',13)
                    for name in NAMES:g.raw(name,1,bytes(8),expected=13)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                gate();done=subprocess.run(cmd,capture_output=True,text=True,timeout=30);assert done.returncode==0,done.stderr
                if tagged:wasm.write_bytes(add_private_instructions(wasm.read_bytes(),len(g.checks),memory))
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,tagged=tagged,compile=cmd,assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0),wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if args.mode=='prepare':print('PASS proposal Wasm preparation');return
    assert all(sha(path)==digest for path,digest in report['images'].items())
    runtime=args.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text())
    assert receipt['complete']and receipt['binary_sha256']==sha(runtime)and receipt['source_root']==str(source)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),build_receipt_sha256=sha(build))
    def run(name,flags,wasm=None,success=True,descriptors=(),crash=None,aliases=False,queues=None):
        gate();cmd=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*flags]
        if wasm:cmd+=['--run',str(wasm)]
        start=time.monotonic();done=subprocess.run(cmd,capture_output=True,pass_fds=descriptors,timeout=120)
        (out/(name+'.stdout')).write_bytes(done.stdout);(out/(name+'.stderr')).write_bytes(done.stderr);errors=done.stderr.decode(errors='replace')
        passed=(done.returncode==0)==success;violations=[json.loads(line)for line in errors.splitlines()if line.startswith('{"schema":"uwvm.memtag.violation.')]
        if crash:
            width,memory,slot=crash;pointer={'request':128,'output':512,'length':2048}[slot]|(5<<(28 if width==32 else 56))
            passed=done.returncode==-signal.SIGILL and all(t in errors for t in ['[crash] memory-protection','Exit status: 122',f'Memory index: {memory}',f'Pointer: {pointer}','Pointer tag: 5','Allocation tag: 3'])
        if aliases:passed=passed and len(violations)>=12 and all(v['pointer_tag']==0 and v['allocation_tag']==3 and v['pointer']in[128,512,2048]for v in violations)
        pending=[]
        if queues is not None:
            for fd in descriptors:
                value=array.array('i',[0]);fcntl.ioctl(fd,termios.FIONREAD,value,True);pending.append(value[0])
            passed=passed and pending==queues
        report['cases'].append(dict(name=name,argv=cmd,exit=done.returncode,passed=passed,seconds=time.monotonic()-start,violations=violations,pending_bytes=pending));save()
        assert passed,(name,done.returncode,pending,errors[-7000:]);print('PASS '+name,flush=True)
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256']and sha(out/(guest['name']+'.wat'))==guest['wat_sha256'];scenario=guest['scenario']
        for engine in ['int','aot']:
            pipes=[]
            try:
                native=scenario in ['success','fp']or guest['tagged']
                if native:
                    pipes=[os.pipe2(os.O_NONBLOCK|os.O_CLOEXEC)for _ in sensor.ALIASES]
                    for rd,wr in pipes:assert os.write(wr,sensor.PACKET)==len(sensor.PACKET)
                descriptors=tuple(rd for rd,wr in pipes);selected=configured(descriptors,images)if native else policy(OPERATIONS,'deny'if scenario=='denied'else'allow')
                flags=['--runtime-'+engine,*selected]
                if guest['tagged']:flags+=['--runtime-memtag-backend','software','--runtime-memtag-mode','sync']
                if scenario=='tag-aliases':flags+=['--runtime-memtag-action','silent-log','err']
                crash=(guest['width'],guest['memory'],scenario.split('-')[2])if scenario.startswith('mismatch-')else None
                queues=[0,len(sensor.PACKET),len(sensor.PACKET)]if scenario in ['success','fp']else[len(sensor.PACKET)]*3 if native else None
                run(guest['name']+'-'+engine,flags,wasm,descriptors=descriptors,crash=crash,aliases=scenario=='tag-aliases',queues=queues)
            finally:
                for pair in pipes:
                    for fd in pair:os.close(fd)
    static=configured([2147483645,2147483646,2147483647],images);smoke=out/'no-grants-32-0.wasm'
    run('policy-check',[*static,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*static],smoke)
    run('default-hidden',['--runtime-int'],smoke,False)
    for field in PROPOSAL_LIMITS:
        broken=static.copy();at=next(i for i in range(len(broken)-4)if broken[i:i+4]==['--ros-scene-proposal-limit','robot','suggestions',field]);del broken[at:at+5]
        run('missing-'+field,[*broken,'--ros-scene-check'],success=False)
    assert pins()==report['inputs']and all(sha(path)==digest for path,digest in report['images'].items())and sha(runtime)==report['runtime_sha256']and sha(manifest)==report['source_manifest_sha256']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
