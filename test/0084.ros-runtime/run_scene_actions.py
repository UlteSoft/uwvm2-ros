#!/usr/bin/env python3
"""Qualify six action imports through actual isolated ROS workers in four Wasm layouts.

No WASI import is available to the guest. Three independently granted schemas,
actual node ownership, explicit asynchronous creation/publication, bounded native
goal/feedback/cancel/result and independent cleanup cross the VM boundary.
Every compiler and child stays in the existing 64 GiB zero-swap cgroup.
"""
import argparse,fcntl,json,os,resource,subprocess,time
from pathlib import Path
import run_scene_communication as communication
from run_scene_communication import CommunicationGuest,profile,alias,definition,worker_definition
from run_scene_parameters import ParameterGuest,ParameterFP
from run_scene_recording import RecordingGuest,u64,blob,text
from run_scene_vm import ROOT,GATE,policy,sha,verify_gate

NAMES=['action.open','action.submit','action.take','action.respond','action.cancel','action.describe']
TRANSITIONS=['action.accept','action.reject','action.execute','action.feedback','action.succeed','action.abort','action.finish.cancelled','action.cancel.respond']
OPERATIONS=communication.OPERATIONS+[dict(name=n,words=0,bytes=0)for n in NAMES]
ACTION_BUDGET={'actions':2,'action-workspace-bytes':8<<20,'action-workspace-calls':16}
ACTION_PEER={'action-carrier-bytes':4<<20,'action-bytes':4<<20}
def selected(document,side):return next(row for row in document['definitions']if row['family']=='action'and row['identity']==('action-server'if side else'action-client'))
def configured(packs,documents):
    flags=communication.configured(packs,documents)
    # The common fixture already declares explicit zero graph/event budgets.
    # Refine only the action reservations; duplicate declarations must reject.
    for name in NAMES:flags+=['--ros-scene-interface','robot',name,'allow']
    for field,value in ACTION_BUDGET.items():
        at=next(i for i in range(len(flags)-3)if flags[i:i+3]==['--ros-scene-communication-budget','robot',field]);flags[at+3]=str(value)
    for side,document in enumerate(documents):
        for field,value in ACTION_PEER.items():
            at=next(i for i in range(len(flags)-4)if flags[i:i+4]==['--ros-scene-communication-worker-limit','robot',f'worker.{side}',field]);flags[at+4]=str(value)
        row=selected(document,side);name=alias(side,row['identity'])
        flags+=['--ros-scene-grant','robot',name,'action',name,'native-action',','.join(NAMES+TRANSITIONS+communication.CONTROL+['resource.close']),
                '--ros-scene-limit','robot',name,'max-resources','1','--ros-scene-communication-bind','robot',name,f'worker.{side}',row['identity']]
    return flags

def successful_guest(documents,width,memory,fp=False):
    g=(ParameterFP if fp else ParameterGuest)(OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall'])
    g.check('call $abi',2);g.open('absent',13);g.steady=g.open('steady');g.call('clock.create',g.steady,[0]);g.clock=g.save('clock')
    g.body.append(g.refresh(90000000000)+' global.set $overall');members=[]
    for side,document in enumerate(documents):
        rows={row['identity']:row for row in document['definitions']};a=selected(document,side);p=a['definition']
        names=['context','node',a['identity']]+[p[n]['profile']['identity']for n in ['goal','feedback','result']]
        caps={name:g.open(alias(side,name))for name in names}
        profiles={name:definition.compile_definition(rows[name])[1]for name in names}
        g.raw('context.open',caps['context'],profile(profiles['context']));context=g.save('context'+str(side))
        body=bytes(16)+profile(profiles['node'])+u64(1)+bytes(8)
        g.raw('node.open',caps['node'],body,[(0,caps['context']),(8,context),(len(body)-8,g.refresh(14000000000))],capacity=48)
        request=g.save('node.request'+str(side),520)
        members.append(dict(caps=caps,profiles=profiles,context=context,request=request,a=a,p=p,cap=caps[a['identity']],side=side,sequence=0))
    for m in members:
        g.polling('communication.open.take',m['caps']['node'],bytes(8),[(0,m['request'])],64);m['node']=g.save('node'+str(m['side']),568)
    for m in members:
        p=m['p'];body=bytes(16)+u64(3)+bytes(24)+profile(m['profiles'][m['a']['identity']])+text(p['name'])+text(p['type'])+text(p['role'])+u64(1)+bytes(8)
        patches=[(0,m['caps']['node']),(8,m['node'])]+[(24+8*n,m['caps'][p[name]['profile']['identity']])for n,name in enumerate(['goal','feedback','result'])]
        deadline=g.refresh(min(900000000,m['a']['limits']['deadline.ns']))
        g.globals.add('action.deadline'+str(m['side']));g.body.append(deadline+' global.set $action.deadline'+str(m['side']))
        patches.append((len(body)-8,'global.get $action.deadline'+str(m['side'])))
        for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:g.raw('action.open',m['cap'],body,patches,expected=22,**kw)
        g.raw('action.open',m['cap'],body,patches,capacity=47,expected=90);g.equal(g.word(2048),48)
        g.polling('action.open',m['cap'],body,patches,48);m['opening']=g.save('action.request'+str(m['side']),520)
        g.polling('action.open',m['cap'],body,patches,48);g.check(g.word(520)+' '+m['opening']+' i64.ne')
    for m in members:
        g.polling('communication.open.take',m['cap'],bytes(8),[(0,m['opening'])],64);m['id']=g.save('action'+str(m['side']),568)
        g.raw('action.describe',m['cap'],bytes(8)+u64(0),[(0,m['id'])],capacity=7,expected=90)
        g.polling('action.describe',m['cap'],bytes(8)+u64(0),[(0,m['id'])],1536)
        g.raw('action.take',m['cap'],bytes(8),[(0,m['id'])],capacity=7,expected=90)
    # A bounded discovery interval uses only the explicitly granted steady clock.
    g.globals.add('discovery');g.body.append(g.refresh(2000000000)+' global.set $discovery')
    g.body.append('loop $discovery.wait');now=g.refresh(0);g.body.append(now+' global.get $discovery i64.lt_u br_if $discovery.wait end')
    uuid=bytes([91])+bytes(15)
    def event_condition(name):
        result=g.word(576)+' i64.const '+str(len(name))+' i64.eq'
        for n,value in enumerate(name.encode()):result+=f' i{width}.const {584+n} i32.load8_u $work i32.const {value} i32.eq i32.and'
        return result
    def take_event(m,name):
        g.loop_id+=1;label='event'+str(g.loop_id);g.body.append(f'block ${label}.done loop ${label}')
        g.polling('action.take',m['cap'],bytes(8),[(0,m['id'])],1536)
        g.check(g.word(512)+' '+m['id']+' i64.ne');g.equal(g.word(520),documents[m['side']]['network']['generation'])
        for offset in [536,544,552]:g.equal(g.word(offset),0)
        g.body.append(event_condition(name)+f' br_if ${label}.done')
        g.check(event_condition('action.goal.status')+' '+event_condition('action.goal.acceptance')+' i32.or i32.eqz')
        g.bound();g.body.append(f'br ${label} end end')
        position=512+128+len(name);g.exact(position+8,blob(uuid))
        return g.save(name+'.request.'+str(g.loop_id),568),g.save(name+'.goal.'+str(g.loop_id),position),position
    def mutation(m,name,tail,extra=()):
        m['sequence']+=1;body=bytes(8)+u64(m['sequence'])+bytes(8)+tail+bytes(8)
        deadline='mutation.deadline.'+str(m['side'])+'.'+str(m['sequence']);g.globals.add(deadline)
        g.body.append(g.refresh(900000000)+' global.set $'+deadline)
        patches=[(0,m['id']),(16,m['id']),*extra,(len(body)-8,'global.get $'+deadline)]
        g.polling(name,m['cap'],body,patches,1536);return g.save(name+'.'+str(m['side'])+'.'+str(m['sequence']))
    client,server=members
    goal=text('world')+text('robot')+u64(0)*6+text('')+u64(0)
    client_goal=mutation(client,'action.submit',blob(uuid)+text('tf2_msgs/action/LookupTransform_Goal')+text('scene-canonical')+blob(goal))
    proposal,server_goal,_=take_event(server,'action.goal.proposal')
    g.check(server_goal+' '+client_goal+' i64.eq');g.check(server_goal+' '+proposal+' i64.eq')
    result=u64(0)*2+text('world')+text('robot')+u64(0)*6+u64(0x3ff0000000000000)+u64(0)+text('')
    def respond(tag,request=proposal,accept=False):
        payload=tag in ['feedback','cancelled'];tail=bytes(16)+text(tag)+u64(payload)
        if payload:tail+=text('tf2_msgs/action/LookupTransform_'+('Feedback'if tag=='feedback'else'Result'))+text('scene-canonical')+blob(b''if tag=='feedback'else result)
        tail+=u64(accept)
        patches=[(24,request),(32,server_goal)]
        if accept:patches.append((24+len(tail),server_goal));tail+=bytes(8)
        return mutation(server,'action.respond',tail,patches)
    respond('accept');_,accepted,_=take_event(client,'action.goal.acceptance');g.check(accepted+' '+client_goal+' i64.ne')
    respond('execute');respond('feedback');_,feedback,_=take_event(client,'action.feedback');g.check(feedback+' '+client_goal+' i64.ne')
    cancel=mutation(client,'action.cancel',bytes(8),[(24,client_goal)])
    cancel_request,cancel_goal,_=take_event(server,'action.cancel.request');g.check(cancel_goal+' '+server_goal+' i64.ne')
    respond('cancel_response',cancel_request,True);returned,_,_=take_event(client,'action.cancel.response');g.check(returned+' '+cancel+' i64.ne')
    respond('cancelled');_,completed,position=take_event(client,'action.result');g.check(completed+' '+client_goal+' i64.ne');g.equal(g.word(position+32),5)
    g.raw('action.describe',server['cap'],bytes(8)+u64(0),[(0,client['id'])],expected=9)
    for m in members:
        g.polling('resource.close',m['cap'],bytes(8),[(0,m['id'])],0)
        g.polling('request.release',m['cap'],bytes(8),[(0,m['opening'])],8)
        g.raw('action.describe',m['cap'],bytes(8)+u64(0),[(0,m['id'])],expected=9)
    for m in members:g.polling('resource.close',m['caps']['node'],bytes(8),[(0,m['node'])],0)
    for m in members:
        g.raw('context.shutdown',m['caps']['context'],bytes(8)+u64(1)+bytes(8),[(0,m['context']),(16,m['context'])])
        g.call('resource.close',m['caps']['context'],[m['context']],capacity=0)
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
    if a.mode=='prepare':print('PASS action guest preparation');return
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
    for field in ACTION_BUDGET:
        bad=flags.copy();at=next(i for i in range(len(bad)-3)if bad[i:i+3]==['--ros-scene-communication-budget','robot',field]);del bad[at:at+4]
        run('missing-budget-'+field,[*bad,'--ros-scene-check'],success=False)
    for field in ACTION_PEER:
        bad=flags.copy();at=next(i for i in range(len(bad)-4)if bad[i:i+4]==['--ros-scene-communication-worker-limit','robot','worker.0',field]);del bad[at:at+5]
        run('missing-peer-'+field,[*bad,'--ros-scene-check'],success=False)
    assert all(sha(path)==digest for path,digest in inputs.items())and sha(runtime)==report['runtime_sha256']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
