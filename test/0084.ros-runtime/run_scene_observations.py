#!/usr/bin/env python3
"""Qualify filtered graph and host-local readiness imports over real isolated ROS workers.

Four Wasm memory layouts, both execution engines, zero authority, invalid ranges,
finite reservations, cross-domain source capabilities and explicit stream cleanup.
All compilation and execution stays in the existing 64 GiB zero-swap cgroup.
"""
import argparse,fcntl,json,os,resource,subprocess,time,hashlib
from pathlib import Path
import run_scene_communication as communication
from run_scene_communication import profile,alias,definition,worker_definition
from run_scene_parameters import ParameterGuest,ParameterFP
from run_scene_recording import RecordingGuest,u64,blob,text
from run_scene_vm import ROOT,GATE,policy,sha,verify_gate
NAMES=['graph.snapshot','graph.watch','event.stream.open','event.stream.poll','event.stream.close']
OPERATIONS=communication.OPERATIONS+[dict(name=n,words=0,bytes=0)for n in NAMES]+[dict(name='capability.revoke',words=2,bytes=32)]
OBSERVATION_BUDGET={'graphs':2,'graph-workspace-bytes':8<<20,'graph-workspace-calls':16,
    'event-bindings':2,'event-workspace-bytes':8<<20,'event-workspace-calls':32}
OBSERVATION_PEER={'graph-query-bytes':4<<20}
EVENT_SHA=hashlib.sha256(b'bounded readiness: two sources, eight events, selected ready/terminal').digest()
def configured(packs,documents):
    flags=communication.configured(packs,documents)
    for name in NAMES+['capability.revoke']:flags+=['--ros-scene-interface','robot',name,'allow']
    for field,value in OBSERVATION_BUDGET.items():
        at=next(i for i in range(len(flags)-3)if flags[i:i+3]==['--ros-scene-communication-budget','robot',field])
        flags[at+3]=str(value)
    for side,document in enumerate(documents):
        for field,value in OBSERVATION_PEER.items():
            at=next(i for i in range(len(flags)-4)if flags[i:i+4]==['--ros-scene-communication-worker-limit','robot',f'worker.{side}',field])
            flags[at+4]=str(value)
        graph=alias(side,'graph');events=alias(side,'events')
        flags+=['--ros-scene-grant','robot',graph,'discovery',graph,'native-graph','graph.snapshot,graph.watch,resource.observe,resource.close,capability.revoke',
            '--ros-scene-limit','robot',graph,'max-resources','1','--ros-scene-communication-bind','robot',graph,f'worker.{side}','graph',
            '--ros-scene-grant','robot',events,'event',events,'event-stream','event.stream.open,event.stream.poll,event.stream.close,resource.close',
            '--ros-scene-limit','robot',events,'max-resources','2',
            '--ros-scene-event-stream','robot',events,f'worker.{side}','events',EVENT_SHA.hex(),'2','2','8','1536','65536','1000000000','3']
    return flags

def successful_guest(documents,width,memory,fp=False):
    g=(ParameterFP if fp else ParameterGuest)(OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall'])
    g.check('call $abi',2);g.open('absent',13);g.steady=g.open('steady');g.call('clock.create',g.steady,[0]);g.clock=g.save('clock')
    g.body.append(g.refresh(90000000000)+' global.set $overall');members=[]
    for side,document in enumerate(documents):
        rows={row['identity']:row for row in document['definitions']};names=['context','node','graph']
        caps={name:g.open(alias(side,name))for name in names};caps['events']=g.open(alias(side,'events'))
        profiles={name:definition.compile_definition(rows[name])[1]for name in names}
        g.raw('context.open',caps['context'],profile(profiles['context']));context=g.save('context'+str(side))
        body=bytes(16)+profile(profiles['node'])+u64(1)+bytes(8)
        g.raw('node.open',caps['node'],body,[(0,caps['context']),(8,context),(len(body)-8,g.refresh(14000000000))],capacity=48)
        request=g.save('node.request'+str(side),520)
        members.append(dict(caps=caps,profiles=profiles,context=context,request=request,side=side))
    for m in members:
        g.polling('communication.open.take',m['caps']['node'],bytes(8),[(0,m['request'])],64);m['node']=g.save('node'+str(m['side']),568)
    for m in members:
        cap=m['caps']['graph'];base=bytes(32)+profile(m['profiles']['graph']);patch=[(0,m['caps']['context']),(8,m['context']),(16,m['caps']['node']),(24,m['node'])]
        for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:g.raw('graph.watch',cap,base,patch,expected=22,**kw)
        g.raw('graph.watch',cap,base,patch,capacity=7,expected=90);g.equal(g.word(2048),8)
        g.polling('graph.watch',cap,base,patch,8);m['watch']=g.save('watch'+str(m['side']))
        body=base+u64(0)+u64(2)
        g.raw('graph.snapshot',cap,body,patch,capacity=23,expected=90);g.equal(g.word(2048),24)
        g.polling('graph.snapshot',cap,body,patch,1536);g.check(g.word(512)+' i64.eqz')
        g.body.append(g.word(520)+' i64.eqz if');g.check(g.word(528)+' i64.const 2 i64.gt_u');g.body.append('else');g.check(g.word(528)+' i64.eqz');g.check(g.word(536)+' i64.const 2 i64.gt_u');g.body.append('end')
    for m in members:
        body=u64(2)+bytes(32)+text('events')+blob(EVENT_SHA)+u64(1536)+u64(8)+u64(65536)
        patch=[(8,members[0]['caps']['graph']),(16,members[0]['watch']),(24,members[1]['caps']['graph']),(32,members[1]['watch'])]
        m['event_body']=body;m['event_patch']=patch;cap=m['caps']['events']
        for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:g.raw('event.stream.open',cap,body,patch,expected=22,**kw)
        g.raw('event.stream.open',cap,body,patch,capacity=7,expected=90);g.equal(g.word(2048),8)
        g.polling('event.stream.open',cap,body,patch,8);m['stream']=g.save('stream'+str(m['side']))
        deadline='event.deadline'+str(m['side']);g.globals.add(deadline);g.body.append(g.refresh(900000000)+' global.set $'+deadline)
        polling=bytes(8)+u64(8)+bytes(8);patches=[(0,m['stream']),(16,'global.get $'+deadline)]
        g.polling('event.stream.poll',cap,polling,patches,1536);g.check(g.word(512)+' i64.eqz');g.check(g.word(512)+' i64.const 2 i64.gt_u')
        # A complete first readiness record contains a sequence, one-based
        # source index, incarnation, selected schema and no payload authority.
        g.check(g.word(520)+' i64.eqz');g.check(g.word(528)+' i64.eqz');g.check(g.word(528)+' i64.const 2 i64.gt_u');g.check(g.word(536)+' i64.eqz')
        g.exact(544,text('readiness.snapshot'));g.equal(g.word(570),0)
        g.raw('event.stream.close',cap,bytes(8),[(0,m['stream'])],capacity=7,expected=90);g.equal(g.word(2048),8)
        g.polling('event.stream.close',cap,bytes(8),[(0,m['stream'])],8);g.equal(g.word(512),1)
        g.polling('event.stream.close',cap,bytes(8),[(0,m['stream'])],8);g.equal(g.word(512),1)
        g.polling('event.stream.open',cap,m['event_body'],m['event_patch'],8);m['stream']=g.save('second.stream'+str(m['side']))
        g.polling('resource.close',cap,bytes(8),[(0,m['stream'])],0)
    for m in members:g.polling('resource.close',m['caps']['graph'],bytes(8),[(0,m['watch'])],0)
    for m in members:
        g.call('capability.revoke',m['caps']['graph'],[1,m['caps']['graph']],capacity=32)
        g.raw('graph.watch',m['caps']['graph'],bytes(8),expected=13)
        # Stop the actual process pair; a private graph binding conservatively
        # retains its counted observer until deployment retirement.
        g.raw('context.shutdown',m['caps']['context'],bytes(8)+u64(1)+bytes(8),[(0,m['context']),(16,m['context'])])
    assert g.cursor<45000,g.cursor
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--pair-receipt',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mode',choices=['all','prepare','execute'],default='all');p.add_argument('--first-layout',action='store_true');a=p.parse_args()
    verify_gate();assert os.sched_getaffinity(0).issubset({4,5,6,7});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    source=a.source_root.resolve();pair=a.pair_receipt.resolve();evidence=json.loads(pair.read_text());assert evidence['complete']
    packs=[];documents=[];inputs={str(pair):sha(pair),str(source/'source-manifest.json'):sha(source/'source-manifest.json')}
    for i in range(2):
        path=pair.parent/f'pack-{i}/manifest.json';assert evidence['pack_sha256'][str(path)]==sha(path)
        pack=json.loads(path.read_text());packs.append(pack);inputs[str(path)]=sha(path)
        for row in [pack['launcher'],*pack['files']]:assert Path(row['path']).stat().st_size==row['bytes']and sha(row['path'])==row['sha256'];inputs[row['path']]=row['sha256']
        path=pair.parent/f'peer-{i}.json';document=json.loads(path.read_text());documents.append(document);inputs[str(path)]=sha(path)
        data,_=worker_definition.compile_definition(document);assert data==Path(next(x['path']for x in pack['files']if x['destination']=='/worker/profile')).read_bytes()
    for path in [Path(__file__),Path(definition.__file__),Path(worker_definition.__file__),*[Path(__file__).with_name(n)for n in ['run_scene_vm.py','run_scene_communication.py','run_scene_parameters.py','run_scene_recording.py','run_scene_buffers.py','run_scene_fp.py']]]:inputs[str(path.resolve())]=sha(path)
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    if a.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared']and report['inputs']==inputs
    else:
        assert not(out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,source_root=str(source),inputs=inputs,
            cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),commands=[],guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if a.mode!='execute':
        for width,memory in ([(32,0)]if a.first_layout else[(32,0),(64,0),(32,1),(64,1)]):
            for scenario in ['success','fp','denied','no-grants']:
                if scenario in ['success','fp']:g=successful_guest(documents,width,memory,scenario=='fp')
                else:
                    g=RecordingGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied'else 2);g.open('missing',13)
                    for name in NAMES:g.raw(name,1,bytes(8),expected=13)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                argv=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                done=subprocess.run(argv,capture_output=True,text=True,timeout=30);report['commands'].append(dict(name=name,argv=argv,exit=done.returncode));save();assert done.returncode==0,done.stderr
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0),wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if a.mode=='prepare':print('PASS observation guest preparation');return
    if not a.runtime:p.error('--runtime required for execution')
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text())
    assert receipt['complete']and receipt['source_root']==str(source)and receipt['binary_sha256']==sha(runtime)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),build_sha256=sha(build));flags=configured(packs,documents)
    def run(name,options,wasm=None,success=True):
        verify_gate();argv=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*options]
        if wasm:argv+=['--run',str(wasm)]
        start=time.monotonic()
        with (out.parent/'.communication-transport-ports.lock').open('a')as reservation:
            fcntl.flock(reservation,fcntl.LOCK_EX)
            try:done=subprocess.run(argv,capture_output=True,text=True,timeout=150)
            except subprocess.TimeoutExpired as failure:
                report['cases'].append(dict(name=name,argv=argv,exit=None,passed=False,timed_out=True));save();raise RuntimeError(name+' timed out')from failure
        (out/(name+'.stdout')).write_text(done.stdout);(out/(name+'.stderr')).write_text(done.stderr)
        passed=(done.returncode==0)==success;report['cases'].append(dict(name=name,argv=argv,exit=done.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,done.returncode,done.stderr[-7000:]);print('PASS '+name,flush=True)
    run('policy-check',[*flags,'--ros-scene-check'])
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256']and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        selected_flags=flags if guest['scenario']in ['success','fp']else policy(OPERATIONS,'deny'if guest['scenario']=='denied'else'allow')
        for engine in ['int','aot']:run(guest['name']+'-'+engine,['--runtime-'+engine,*selected_flags],wasm)
    smoke=out/'no-grants-32-0.wasm';run('hidden',['--runtime-int'],smoke,False)
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    for field in OBSERVATION_BUDGET:
        bad=flags.copy();at=next(i for i in range(len(bad)-3)if bad[i:i+3]==['--ros-scene-communication-budget','robot',field]);del bad[at:at+4]
        run('missing-budget-'+field,[*bad,'--ros-scene-check'],success=False)
    for field in OBSERVATION_PEER:
        bad=flags.copy();at=next(i for i in range(len(bad)-4)if bad[i:i+4]==['--ros-scene-communication-worker-limit','robot','worker.0',field]);del bad[at:at+5]
        run('missing-peer-'+field,[*bad,'--ros-scene-check'],success=False)
    assert all(sha(path)==digest for path,digest in inputs.items())and sha(runtime)==report['runtime_sha256']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
