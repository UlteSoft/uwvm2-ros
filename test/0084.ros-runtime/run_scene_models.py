#!/usr/bin/env python3
"""Qualify real Wasm model jobs through the production isolated CPU worker.

Guests import only uwvm_ros_v2. Memory32/64 and memory index 0/1 cover the
seven model/request carriers, real 2x2 inference, syscall confinement, exact
retry, cancellation, denied actions and memory preflight. This fixture uses
one approved synchronous CPU backend; it does not qualify arbitrary LLMs.
"""
import argparse,hashlib,json,os,struct,subprocess,time
from pathlib import Path
from run_scene_vm import Guest,ROOT,GATE,policy,sha,verify_gate
from run_scene_buffers import BufferGuest,u64,blob,text,layout

MODEL=['model.'+n for n in ['open','submit','take','cancel','describe']]+['request.describe','request.release']
OPERATIONS=[dict(name=n,words=0,bytes=0) for n in MODEL+['buffer.create','buffer.write','buffer.seal','buffer.read','buffer.describe']]
OPERATIONS += [dict(name='clock.create',words=1,bytes=8),dict(name='clock.read',words=1,bytes=32),dict(name='resource.close',words=1,bytes=0)]
PROFILE_SHA=bytes([42])*32
LIMITS={'image-bytes':67108864,'address-bytes':268435456,'cpu-seconds':5,'file-bytes':4096,'open-files':128,
        'scratch-bytes':65536,'scratch-slots':1,'request-bytes':4096,'response-bytes':4096,'stderr-bytes':1024,
        'startup-ns':5000000000,'poll-ns':1000000,'supervisor-bytes':1048576}
IMAGES=[('bootstrap','bootstrap','/worker/bootstrap'),('backend.so','runtime','/worker/backend.so'),
        ('matrix.weights','weights','/model/weights'),('ld-linux-x86-64.so.2','runtime','/lib64/ld-linux-x86-64.so.2'),
        ('libc.so.6','runtime','/lib/x86_64-linux-gnu/libc.so.6'),('libstdc++.so.6','runtime','/lib/x86_64-linux-gnu/libstdc++.so.6'),
        ('libgcc_s.so.1','runtime','/lib/x86_64-linux-gnu/libgcc_s.so.1'),('libm.so.6','runtime','/lib/x86_64-linux-gnu/libm.so.6')]
def profile(identity):return text(identity)+blob(PROFILE_SHA)
def model_flags(alias,images):
    actions=[n for n in MODEL if not(alias=='denied' and n=='model.submit')]+['resource.close']
    flags=['--ros-scene-grant','robot',alias,'model.vision',alias,'local-model',','.join(actions),
           '--ros-scene-model','robot',alias,'matrix.cpu',PROFILE_SHA.hex(),'matrix.multiply',PROFILE_SHA.hex(),'mode','matrix.result','bytes','terminal',
           '--ros-scene-model-capacity','robot',alias,'2','2','32','1','8','64','4','32768','10000000000']
    for key,value in LIMITS.items():flags+=['--ros-scene-model-worker-limit','robot',alias,key,str(value)]
    launcher=images/'bwrap';flags+=['--ros-scene-model-worker-launcher','robot',alias,str(launcher),str(launcher.stat().st_size),sha(launcher)]
    for filename,role,destination in IMAGES:
        path=images/filename;flags+=['--ros-scene-model-worker-image','robot',alias,role,destination,str(path),str(path.stat().st_size),sha(path)]
    return flags
def configured(images,placement_cpu=None):
    flags=policy(OPERATIONS)+['--ros-scene-model-budget','robot','8','8','128','256','16','131072']
    for alias in ['brain','peer','rate','denied']:
        flags+=model_flags(alias,images)
        if placement_cpu is not None:flags+=['--ros-scene-model-worker-placement','robot',alias,str(placement_cpu),'inherit','none']
    flags+=['--ros-scene-limit','robot','rate','delivery','may-reject','--ros-scene-limit','robot','rate','calls-per-window','0',
        '--ros-scene-grant','robot','steady','clock.steady','steady','local','clock.create,clock.read,resource.close',
        '--ros-scene-grant','robot','data','buffer','tensor','sealed-buffer','buffer.create,buffer.write,buffer.seal,buffer.read,buffer.describe,resource.close',
        '--ros-scene-buffer-budget','robot','32','2','32','2',
        '--ros-scene-buffer','robot','data','tensor',PROFILE_SHA.hex(),'32','2','16','16','32','2','1','16','1','1','1']
    return flags
def successful_guest(width,memory,placement_cpu=None):
    g=BufferGuest(OPERATIONS,width,memory);g.check('call $abi',2);g.open('absent',13)
    caps={alias:g.open(alias) for alias in ['brain','peer','rate','denied','data','steady']};cap=caps['brain']
    g.call('clock.create',caps['steady'],[0]);clock=g.save('clock')
    def deadline(delta=5000000000):
        g.call('clock.read',caps['steady'],[clock],output=768,length=1104)
        g.globals.add('deadline');g.body.append(g.word(768)+f' i64.const {delta} i64.add global.set $deadline')
        return 'global.get $deadline'
    def bounded_wait():
        g.call('clock.read',caps['steady'],[clock],output=768,length=1104)
        g.check(g.word(768)+' global.get $deadline i64.ge_u')
    encoded=profile('matrix.cpu')
    for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512)]:g.raw('model.open',cap,encoded,expected=22,**kw)
    g.raw('model.open',cap,encoded,capacity=7,expected=90);g.equal(g.word(1024),8)
    g.raw('model.open',cap,profile('foreign'),expected=13);g.raw('model.open',cap,encoded+b'x',expected=22)
    g.raw('model.open',caps['rate'],encoded,expected=11)
    g.raw('model.open',cap,encoded);session=g.save('session')
    g.raw('model.open',caps['peer'],encoded);peer=g.save('peer-session')
    g.raw('model.describe',caps['peer'],u64(0),[(0,session)],expected=9)
    g.raw('model.describe',cap,u64(0),[(0,session)],capacity=7,expected=90)
    g.raw('model.describe',cap,u64(0),[(0,session)]);g.exact(512,encoded+u64(2)+text('matrix.multiply')+text('sealed-inputs'))
    data=struct.pack('<2d',5,6);schema=layout(identity='tensor',digest=PROFILE_SHA,shape=(16,),size=16)
    g.raw('buffer.create',caps['data'],schema);buffer=g.save('input-buffer')
    g.raw('buffer.write',caps['data'],u64(0)+u64(0)+blob(data),[(0,buffer)])
    g.raw('buffer.seal',caps['data'],u64(0)+blob(hashlib.sha256(data).digest()),[(0,buffer)])
    def submit(sequence,mode,with_input=True,**kw):
        body=u64(0)+u64(sequence)+u64(0)+profile('matrix.multiply')+text('mode')+text('bytes')+blob(u64(mode))+u64(with_input)
        patches=[(0,session),(16,session)]
        if with_input:
            patches.extend([(len(body),caps['data']),(len(body)+8,buffer)]);body+=bytes(48)
        body+=u64(32)+u64(2)+u64(8192)+u64(0);patches.append((len(body)-8,'global.get $deadline'))
        g.raw('model.submit',kw.pop('cap',cap),body,patches,**kw)
    deadline();submit(1,1,capacity=159,expected=90);g.equal(g.word(1024),160)
    submit(1,1,cap=caps['denied'],expected=13);submit(1,1,cap=caps['rate'],expected=11)
    submit(1,1);request=g.save('request');submit(1,1);g.check(g.word(512)+' '+request+' i64.ne')
    g.raw('request.describe',caps['peer'],u64(0),[(0,request)],expected=9)
    def wait_result(phase):
        expected=text(phase);g.body.append('block $done loop $waiting')
        g.raw('request.describe',cap,u64(0),[(0,request)])
        g.body.append(g.word(520)+f' i64.const {len(phase)} i64.eq br_if $done')
        bounded_wait();g.body.append('br $waiting end end');g.exact(520,expected)
    def release():
        # Cleanup may lag terminal notification; no capacity is refunded until confirmed.
        g.globals.add('release-status');g.write(128,request);g.body.append('block $released loop $release_wait')
        g.body.append(f'{cap} i32.const {memory} i64.const 128 i64.const 8 i64.const 512 i64.const 512 i64.const 1024 call $request.release i64.extend_i32_u global.set $release-status')
        g.body.append('global.get $release-status i64.eqz br_if $released')
        g.equal('global.get $release-status',11);bounded_wait();g.body.append('br $release_wait end end');g.equal(g.word(512),1)
    def take_result(payload):
        g.raw('model.take',cap,bytes(16),[(0,session),(8,request)],capacity=1,expected=90)
        g.raw('model.take',cap,bytes(16),[(0,session),(8,request)])
        for offset in range(0,len(payload),8):
            pointer=g.word(1024)+f' i64.const {512-len(payload)+offset} i64.add'+(' i32.wrap_i64' if width==32 else '')
            g.equal(pointer+' i64.load $work',int.from_bytes(payload[offset:offset+8],'little'))
    wait_result('succeeded');take_result(struct.pack('<2d',17,39));release()
    submit(1,1);g.check(g.word(512)+' '+request+' i64.ne') # Exact admission receipt survives release.
    key=2
    if placement_cpu is not None:
        deadline();submit(key,6,False);request=g.save('request');wait_result('succeeded');take_result(u64(1)+u64(placement_cpu)+u64(3));release();key+=1
    deadline();submit(key,2,False);request=g.save('request');wait_result('succeeded');take_result(u64(1023));release()
    deadline();submit(key+1,3,False);request=g.save('request')
    g.raw('model.cancel',cap,bytes(16),[(0,session),(8,request)]);wait_result('cancelled');release()
    g.call('resource.close',cap,[session]);g.raw('model.describe',cap,u64(0),[(0,session)],expected=9)
    g.call('resource.close',caps['peer'],[peer]);g.call('resource.close',caps['data'],[buffer])
    g.check(cap+' call $close');g.raw('model.open',cap,encoded,expected=9)
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--mode',choices=['prepare','execute','all'],default='all')
    p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--images',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--placement-cpu',type=int,help='Require the isolated backend to observe this one CPU and denied placement setters')
    args=p.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({12,13,14,15})
    assert args.placement_cpu is None or args.placement_cpu in os.sched_getaffinity(0)
    if args.mode!='prepare' and args.runtime is None:p.error('--runtime required')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    images=args.images.resolve();files=[Path(__file__).resolve(),Path(__file__).with_name('run_scene_vm.py'),Path(__file__).with_name('run_scene_buffers.py')]
    files += [images/name for name,_,_ in IMAGES]+[images/'bwrap']
    pins=lambda:{str(path):sha(path) for path in files}
    if args.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['pins']==pins() and report.get('placement_cpu')==args.placement_cpu
    else:
        assert not (out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,pins=pins(),guests=[],cases=[],
            cgroup=str(GATE),cgroup_inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),placement_cpu=args.placement_cpu)
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if args.mode!='execute':
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for mode in ['success','denied','no-grants']:
                if mode=='success':g=successful_guest(width,memory,args.placement_cpu)
                else:
                    g=BufferGuest(OPERATIONS,width,memory);g.check('call $abi',13 if mode=='denied' else 2);g.open('brain',13)
                    for name in MODEL:g.raw(name,1,expected=13 if mode=='denied' else 9)
                name=f'{mode}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                command=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                result=subprocess.run(command,capture_output=True,text=True);assert not result.returncode,result.stderr
                report['guests'].append(dict(name=name,mode=mode,width=width,memory=memory,assertions=len(g.checks),compile=command,wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if args.mode=='prepare':print('PASS model Wasm preparation');return
    runtime=args.runtime.resolve();production=args.source_root.resolve();report['runtime']=str(runtime);report['runtime_sha256']=sha(runtime)
    receipt=json.loads((runtime.parent/'full-build-results.json').read_text());assert receipt['complete'] and receipt['binary_sha256']==sha(runtime) and receipt['source_root']==str(production)
    report['build_receipt_sha256']=sha(runtime.parent/'full-build-results.json');report['source_manifest_sha256']=sha(production/'source-manifest.json')
    def run(name,flags,wasm=None,success=True):
        verify_gate();command=[str(runtime),'--wasm-set-main-module-name','main','--runtime-llvm-jit-cache-path','disable',*flags]
        if wasm:command+=['--run',str(wasm)]
        started=time.monotonic();result=subprocess.run(command,capture_output=True,timeout=120)
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        passed=(result.returncode==0)==success;report['cases'].append(dict(name=name,argv=command,exit=result.returncode,passed=passed,seconds=time.monotonic()-started));save()
        assert passed,(name,result.stderr.decode(errors='replace')[-7000:]);print('PASS '+name,flush=True)
    flags=configured(images,args.placement_cpu)
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256'] and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        selected=flags if guest['mode']=='success' else policy(OPERATIONS,'deny' if guest['mode']=='denied' else 'allow')
        for engine in ['int','aot']:run(guest['name']+'-'+engine,['--runtime-'+engine,*selected],wasm)
    smoke=out/'no-grants-32-0.wasm';run('default-hidden',['--runtime-int'],smoke,False)
    run('complete-policy-check',[*flags,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    for name,extra in [('duplicate-limit',['--ros-scene-model-worker-limit','robot','brain','stderr-bytes','0']),
                       ('unknown-limit',['--ros-scene-model-worker-limit','robot','brain','unknown','1']),
                       ('unlimited',['--ros-scene-model-worker-limit','robot','brain','startup-ns','unlimited'])]:
        run('cli-'+name,[*flags,*extra,'--ros-scene-check'],success=False)
    if args.placement_cpu is not None:
        run('cli-duplicate-placement',[*flags,'--ros-scene-model-worker-placement','robot','brain','inherit','inherit','none','--ros-scene-check'],success=False)
        for index,placement in enumerate([['1,0','inherit','none'],['inherit','bind','none'],['inherit','local','0']]):
            run('cli-malformed-placement-'+str(index),[*configured(images),'--ros-scene-model-worker-placement','robot','brain',*placement,'--ros-scene-check'],success=False)
        unavailable=[*configured(images),'--ros-scene-model-worker-placement','robot','brain','8388607','inherit','none']
        run('placement-static-no-probe',[*unavailable,'--ros-scene-check'])
        run('placement-unavailable-before-guest',['--runtime-int',*unavailable],smoke,False)
    assert pins()==report['pins'] and sha(runtime)==report['runtime_sha256']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
