#!/usr/bin/env python3
"""Actual Wasm -> externally provisioned ROS gateways -> DDS String / SetBool qualification.

Every image and startup definition is reused by exact hash from an existing
qualified pair. The CLI has no ambient network or filesystem grant. Creation
receipts, publication, message reservations and acknowledgements are separate.
The test is a bounded semantic exercise, not a real-time or platform guarantee.
All compilers and processes remain in the existing 64 GiB zero-swap cgroup.
"""
import argparse,fcntl,json,os,resource,signal,subprocess,sys,time
from pathlib import Path
from run_scene_vm import ROOT,GATE,policy,sha,verify_gate
from run_scene_recording import RecordingGuest,RecordingFP,u64,blob,text
sys.path.insert(0,str(ROOT/'tools/ros_scene'))
import make_communication_definition as definition
import make_communication_worker_definition as worker_definition
import communication_test_deployer as deployment_fixture

MAIN=['context.open','context.describe','context.shutdown','node.open','node.describe','type.describe',
      'topic.open','topic.publish','topic.take','topic.describe','qos.describe','qos.liveliness.assert',
      'qos.acknowledgements.wait','service.open','service.submit','service.take','service.respond',
      'service.available','service.abandon','service.describe']
RECEIVE=['topic.reserve','topic.acknowledge','topic.release','service.reserve','service.acknowledge','service.release']
CONTROL=['communication.open.take','communication.open.cancel','request.describe','request.release']
OPERATIONS=[dict(name=n,words=0,bytes=0)for n in MAIN+RECEIVE+CONTROL]+[
    dict(name='clock.create',words=1,bytes=8),dict(name='clock.read',words=1,bytes=32),dict(name='resource.close',words=1,bytes=0)]
BUDGET={'workers':2,'pairs':1,'contexts':2,'nodes':2,'types':32,'topics':4,'services':4,
    'queue-bytes':8<<20,'receipt-bytes':65536,'bindings':32,'carrier-bytes':2048,'output-bytes':65536,
    'workspace-bytes':8<<20,'workspace-calls':16,'node-deadline-ns':15000000000,
    'creation-bindings':32,'creation-workspace-bytes':8<<20,'creation-workspace-calls':16,
    'definition-bytes':8<<20,'provider-bytes':16<<30,'supervisor-stack-bytes':8<<20,'supervisor-poll-ns':1000000,
    'actions':0,'action-workspace-bytes':0,'action-workspace-calls':0,
    'graphs':0,'graph-workspace-bytes':0,'graph-workspace-calls':0,
    'event-bindings':0,'event-workspace-bytes':0,'event-workspace-calls':0}
PEER={'rpc-retained-bytes':4<<20,'rpc-stderr-bytes':65536,'rpc-deadline-ns':5000000000,'rpc-poll-ns':1000000,
      'topic-carrier-bytes':4<<20,'topic-bytes':4<<20,'service-carrier-bytes':4<<20,'service-bytes':4<<20,
      'action-carrier-bytes':0,'action-bytes':0,'graph-query-bytes':0}
def profile(value):return text(value['identity'])+blob(bytes.fromhex(value['profile_sha256']))
def selected(document,side):
    return [row for row in document['definitions'] if row['family']in['context','node','type']or
        (row['family']=='topic'and row['identity']==('subscription'if side else'publisher'))or
        (row['family']=='service'and row['identity']==('service-server'if side else'service-client'))]
def alias(side,identity):return f'worker.{side}.{identity}'
def actions(kind):
    prefix={'context':'context.','node':'node.','type':'type.','topic':'topic.','service':'service.'}[kind]
    result=[n for n in MAIN+RECEIVE if n.startswith(prefix)]
    if kind in ['topic','service']:result+=['qos.describe']
    if kind=='topic':result+=['qos.liveliness.assert','qos.acknowledgements.wait']
    if kind in ['node','topic','service']:result+=CONTROL
    if kind!='type':result+=['resource.close']
    return result
def configured(packs,documents):
    flags=policy(OPERATIONS)
    flags+=['--ros-scene-grant','robot','steady','clock.steady','steady','local','clock.create,clock.read,resource.close']
    for name,value in BUDGET.items():flags+=['--ros-scene-communication-budget','robot',name,str(value)]
    for i,pack in enumerate(packs):
        name=f'worker.{i}';startup=next(row for row in pack['files']if row['destination']=='/worker/profile')
        flags+=['--ros-scene-communication-worker','robot',name,'startup',pack['ros_prefix'],startup['path'],str(startup['bytes']),startup['sha256']]
        for key,value in PEER.items():flags+=['--ros-scene-communication-worker-limit','robot',name,key,str(value)]
        for row in selected(documents[i],i):
            ident=alias(i,row['identity']);kind=row['family']
            flags+=['--ros-scene-grant','robot',ident,kind,ident,'native-communication',','.join(actions(kind)),
                '--ros-scene-limit','robot',ident,'max-resources','1',
                '--ros-scene-communication-bind','robot',ident,name,row['identity']]
    flags+=['--ros-scene-communication-pair','robot','robot','worker.0','worker.1','5000000000']
    return flags

class CommunicationGuest(RecordingGuest):
    def dynamic(self,name,cap,payload,patches=(),capacity=1024,output=512,length=2048):
        """Issue one call retaining its actual error instead of assuming readiness."""
        self.raw(name,cap,payload,patches,capacity=capacity,output=output,length=length)
        self.body.pop();call=self.checks.pop().split('(func $',1)[1]
        expression=call.split(' ',1)[1].rsplit(' i32.const 0 i32.ne if unreachable end)',1)[0]
        # This helper deliberately uses the plain RecordingGuest assertion; FP
        # qualification is applied to all ordinary successful/error boundaries.
        self.body.append(expression+' global.set $status')
    def polling(self,name,cap,payload,patches=(),capacity=1024,output=512,length=2048):
        self.loop_id+=1;label='poll'+str(self.loop_id)
        self.body.append(f'block ${label}.done loop ${label}')
        self.dynamic(name,cap,payload,patches,capacity,output,length)
        self.body.append(f'global.get $status i32.eqz br_if ${label}.done')
        self.check('global.get $status',11)
        self.bound();self.body.append(f'br ${label} end end')
    def refresh(self,span):
        self.call('clock.read',self.steady,[self.clock],output=3072,length=3104)
        return self.word(3072)+' i64.const '+str(span)+' i64.add'
    def bound(self):
        now=self.refresh(0);self.check(now+' global.get $overall i64.ge_u')
    def render(self):
        result=super().render()
        return result.replace('(global $status (mut i64) (i64.const 0))','(global $status (mut i32) (i32.const 0))')

def successful_guest(documents,width,memory):
    g=CommunicationGuest(OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall'])
    g.check('call $abi',2);g.open('absent',13);g.steady=g.open('steady')
    g.call('clock.create',g.steady,[0]);g.clock=g.save('steady-clock')
    g.body.append(g.refresh(90000000000)+' global.set $overall')
    members=[]
    for i,document in enumerate(documents):
        rows={row['identity']:row for row in selected(document,i)}
        caps={name:g.open(alias(i,name)) for name in rows}
        profiles={name:definition.compile_definition(row)[1] for name,row in rows.items()}
        topic='subscription'if i else'publisher';service='service-server'if i else'service-client'
        members.append(dict(rows=rows,caps=caps,profiles=profiles,topic=topic,service=service))
    for i,m in enumerate(members):
        cap=m['caps']['context'];body=profile(m['profiles']['context'])
        for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:
            g.raw('context.open',cap,body,expected=22,**kw)
        g.raw('context.open',cap,body,capacity=7,expected=90);g.equal(g.word(2048),8)
        g.raw('context.open',cap,body);m['context']=g.save(f'context-{i}')
        g.raw('context.describe',cap,bytes(8),[(0,m['context'])]);g.exact(512,body)
        # Same handle numbers never confer another worker's origin authority.
        foreign=members[1-i]['caps']['context']
        g.raw('context.describe',foreign,bytes(8),[(0,m['context'])],expected=9)
        node_body=bytes(16)+profile(m['profiles']['node'])+u64(1)+bytes(8)
        patches=[(0,cap),(8,m['context']),(len(node_body)-8,g.refresh(14000000000))]
        node_cap=m['caps']['node']
        g.raw('node.open',node_cap,node_body,patches,capacity=47,expected=90);g.equal(g.word(2048),48)
        g.raw('node.open',node_cap,node_body,patches,capacity=48);g.equal(g.word(512),1)
        m['node_request']=g.save(f'node-request-{i}',520)
    for i,m in enumerate(members):
        cap=m['caps']['node'];body=bytes(8);patches=[(0,m['node_request'])]
        g.raw('communication.open.take',cap,body,patches,capacity=63,expected=90);g.equal(g.word(2048),64)
        g.polling('communication.open.take',cap,body,patches,64);g.equal(g.word(560),1);m['node']=g.save(f'node-{i}',568)
        g.raw('communication.open.take',cap,body,patches,capacity=64);g.check(g.word(568)+' '+m['node']+' i64.ne')
        g.raw('node.describe',cap,bytes(8),[(0,m['node'])])
        g.exact(512,text(m['rows']['node']['definition']['name'])+text(m['rows']['node']['definition']['namespace']))
        for family,name,codecs in [('topic',m['topic'],['std-string']),('service',m['service'],['set-bool-request','set-bool-response'])]:
            row=m['rows'][name];p=row['definition'];schema_caps=[m['caps'][c] for c in codecs]
            body=bytes(16)+u64(len(codecs))+bytes(8*len(codecs))+profile(m['profiles'][name])+text(p['name'])+text('std_srvs/srv/SetBool'if family=='service'else'std_msgs/msg/String')+text(p['role'])+u64(1)+bytes(8)
            patches=[(0,cap),(8,m['node'])]+[(24+8*n,value)for n,value in enumerate(schema_caps)]+[(len(body)-8,g.refresh(900000000))]
            target=m['caps'][name]
            g.raw(family+'.open',target,body,patches,capacity=47,expected=90)
            g.raw(family+'.open',target,body,patches,capacity=48);m[family+'_request']=g.save(f'{family}-request-{i}',520)
    for i,m in enumerate(members):
        for family,kind in [('topic',2),('service',3)]:
            cap=m['caps'][m[family]];patches=[(0,m[family+'_request'])]
            g.polling('communication.open.take',cap,bytes(8),patches,64);g.equal(g.word(560),kind)
            m[family+'_id']=g.save(f'{family}-{i}',568)
            g.raw(family+'.describe',cap,bytes(8),[(0,m[family+'_id'])]);g.check(g.word(512)+' '+m['node']+' i64.ne')
            g.polling('qos.describe',cap,bytes(8),[(0,m[family+'_id'])])
    # Let native DDS discovery progress; this is semantic setup, not a latency claim.
    g.globals.add('discovery');g.body.append(g.refresh(2000000000)+' global.set $discovery')
    g.body.append('loop $discovery.wait');g.body.append(g.refresh(0)+' global.get $discovery i64.lt_u br_if $discovery.wait end')
    def mutate(side,family,body,correlation=None):
        m=members[side];endpoint=m[family+'_id'];cap=m['caps'][m[family]]
        codec='std-string'if family=='topic'else'set-bool-response'if correlation else'set-bool-request'
        message=text(definition.CODECS[codec][0])+text('scene-canonical')+blob(body)
        request=bytes(8)+u64(1)+bytes(8)+(bytes(8)if correlation else b'')+message+bytes(8)
        patches=[(0,endpoint),(16,endpoint)]+([(24,correlation)]if correlation else[])+[(len(request)-8,g.refresh(900000000))]
        name='topic.publish'if family=='topic'else'service.respond'if correlation else'service.submit'
        g.raw(name,cap,request,patches,capacity=1,expected=90)
        g.raw(name,cap,request,patches);return g.save(f'{family}-mutation-{side}')
    def receive(side,family,payload,correlation=None):
        m=members[side];endpoint=m[family+'_id'];cap=m['caps'][m[family]]
        # An outstanding queued reservation retains its exact deadline, even
        # before the first reply. Changing it would conflict with that request.
        g.loop_id+=1;label='receive'+str(g.loop_id);g.globals.add(label+'.deadline')
        g.body.append(g.refresh(900000000)+' global.set $'+label+'.deadline')
        g.body.append(f'block ${label}.done loop ${label}')
        g.dynamic(family+'.reserve',cap,bytes(16),[(0,endpoint),(8,'global.get $'+label+'.deadline')],output=33000,capacity=4096)
        g.body.append(f'global.get $status i32.eqz br_if ${label}.done');g.check('global.get $status',11)
        g.bound();g.body.append(f'br ${label} end end')
        refs=[g.save(f'{family}-ref-{side}-{n}',33000+8*n)for n in range(4)]
        g.check(refs[0]+' '+endpoint+' i64.ne')
        # Parse optional timestamps and publisher identity without assuming
        # their presence. RMW clock numbers are not authenticated ROS clocks.
        cursor=f'{family}-cursor-{side}';g.globals.add(cursor);g.body.append('i64.const 33064 global.set $'+cursor)
        load='global.get $'+cursor+(' i32.wrap_i64'if width==32 else'')+' i64.load $work'
        for _ in range(2):
            g.body.append(load+' i64.eqz i32.eqz if global.get $'+cursor+' i64.const 24 i64.add global.set $'+cursor+' end')
            g.body.append('global.get $'+cursor+' i64.const 8 i64.add global.set $'+cursor)
        g.body.append(load+' i64.eqz i32.eqz if global.get $'+cursor+' i64.const 8 i64.add global.set $'+cursor+' '+load+' global.get $'+cursor+' i64.add global.set $'+cursor+' end')
        g.body.append('global.get $'+cursor+' i64.const 16 i64.add global.set $'+cursor)
        corr=f'{family}-correlation-{side}';g.globals.add(corr);g.body.append(load+' global.set $'+corr)
        if correlation:g.check('global.get $'+corr+' '+correlation+' i64.ne')
        kind='topic.message'if family=='topic'else'service.request'if side else'service.response'
        codec='std-string'if family=='topic'else'set-bool-request'if side else'set-bool-response'
        expected=text(kind)+text(definition.CODECS[codec][0])+text('scene-canonical')+blob(payload)
        address='global.get $'+cursor+' i64.const 8 i64.add'+(' i32.wrap_i64'if width==32 else'')
        g.body.append(f'i{width}.const 28000 {address} i{width}.const {len(expected)} memory.copy $work $work');g.exact(28000,expected)
        ack=bytes(8)+u64(1)+bytes(32);patches=[(0,endpoint)]+[(16+8*n,r)for n,r in enumerate(refs)]
        g.polling(family+'.acknowledge',cap,ack,patches)
        g.raw(family+'.acknowledge',cap,ack,patches)
        return 'global.get $'+corr
    message=text('Wasm isolated DDS message');mutate(0,'topic',message);receive(1,'topic',message)
    submitted=mutate(0,'service',b'\x01');incoming=receive(1,'service',b'\x01')
    response=b'\x01'+text('Wasm isolated service reply');mutate(1,'service',response,incoming);receive(0,'service',response,submitted)
    for i,m in enumerate(members):
        for family in ['topic','service']:
            cap=m['caps'][m[family]];g.polling('resource.close',cap,bytes(8),[(0,m[family+'_id'])])
            g.polling('request.release',cap,bytes(8),[(0,m[family+'_request'])])
    # Pair retirement must not begin while its peer still has endpoint work.
    for m in members:g.polling('resource.close',m['caps']['node'],bytes(8),[(0,m['node'])])
    assert g.cursor<26000,g.cursor
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--pair-receipt',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--gateway',type=Path,required=True,help='Test-only native gateway owned by the scripted provisioner')
    p.add_argument('--mode',choices=['all','prepare','execute'],default='all');p.add_argument('--first-layout',action='store_true')
    a=p.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({4,5,6,7});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    source=a.source_root.resolve();pair=a.pair_receipt.resolve();evidence=json.loads(pair.read_text());assert evidence['complete']
    packs=[];documents=[];inputs={str(pair):sha(pair),str(source/'source-manifest.json'):sha(source/'source-manifest.json'),str(a.gateway.resolve()):sha(a.gateway.resolve()),str(Path(deployment_fixture.__file__).resolve()):sha(deployment_fixture.__file__)}
    for i in range(2):
        path=pair.parent/f'pack-{i}/manifest.json';assert evidence['pack_sha256'][str(path)]==sha(path)
        pack=json.loads(path.read_text());packs.append(pack);inputs[str(path)]=sha(path)
        for image in [pack['launcher'],*pack['files']]:
            assert Path(image['path']).stat().st_size==image['bytes']and sha(image['path'])==image['sha256'];inputs[image['path']]=image['sha256']
        path=pair.parent/f'peer-{i}.json';document=json.loads(path.read_text());documents.append(document);inputs[str(path)]=sha(path)
        encoded,_=worker_definition.compile_definition(document);assert encoded==Path(next(x['path']for x in pack['files']if x['destination']=='/worker/profile')).read_bytes()
    for path in [Path(__file__),Path(definition.__file__),Path(worker_definition.__file__),*[Path(__file__).with_name(x)for x in ['run_scene_vm.py','run_scene_recording.py','run_scene_buffers.py','run_scene_fp.py']]]:inputs[str(path.resolve())]=sha(path)
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    if a.mode=='execute':
        report=json.loads((out/'results.json').read_text());assert report['prepared']and report['inputs']==inputs
    else:
        assert not(out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,source_root=str(source),inputs=inputs,
            cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),commands=[],guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if a.mode!='execute':
        for width,memory in ([(32,0)]if a.first_layout else[(32,0),(64,0),(32,1),(64,1)]):
            for scenario in ['success','denied','no-grants']:
                if scenario=='success':g=successful_guest(documents,width,memory)
                else:
                    g=RecordingGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied'else 2);g.open('missing',13)
                    for name in MAIN+RECEIVE+CONTROL:
                        g.raw(name,1,bytes(8),expected=9 if scenario=='no-grants'and name.startswith('request.')else 13)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                argv=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                done=subprocess.run(argv,capture_output=True,text=True,timeout=30);report['commands'].append(dict(name=name,argv=argv,exit=done.returncode));save();assert done.returncode==0,done.stderr
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,assertions=len(g.checks),wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if a.mode=='prepare':print('PASS communication guest preparation');return
    if not a.runtime:p.error('--runtime required for execution')
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text())
    assert receipt['complete']and receipt['source_root']==str(source)and receipt['binary_sha256']==sha(runtime)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),build_sha256=sha(build));flags=configured(packs,documents)
    def run(name,options,wasm=None,success=True,needle=None):
        verify_gate();argv=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*options]
        if wasm:argv+=['--run',str(wasm)]
        start=time.monotonic()
        with (out.parent/'.communication-transport-ports.lock').open('a')as reservation:
            fcntl.flock(reservation,fcntl.LOCK_EX)
            try:
                done=deployment_fixture.execute(argv,packs,a.gateway.resolve()) if '--ros-scene-communication-worker' in options else subprocess.run(argv,capture_output=True,text=True,timeout=150)
            except subprocess.TimeoutExpired as failure:
                report['cases'].append(dict(name=name,argv=argv,exit=None,passed=False,timed_out=True));save();raise RuntimeError(name+' timed out')from failure
        (out/(name+'.stdout')).write_text(done.stdout);(out/(name+'.stderr')).write_text(done.stderr)
        passed=(done.returncode==0)==success and(needle is None or needle in done.stdout+done.stderr)
        report['cases'].append(dict(name=name,argv=done.args,exit=done.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,done.returncode,done.stderr[-7000:]);print('PASS '+name,flush=True)
    run('policy-check',[*flags,'--ros-scene-check'])
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256']and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        selected_flags=flags if guest['scenario']=='success'else policy(OPERATIONS,'deny'if guest['scenario']=='denied'else'allow')
        for engine in ['int','aot']:run(guest['name']+'-'+engine,['--runtime-'+engine,*selected_flags],wasm)
    smoke=out/'no-grants-32-0.wasm'
    run('hidden',['--runtime-int'],smoke,False,needle='ROS scene interface is hidden')
    run('static-validation',['--mode','validation',*flags],smoke)
    run('static-sections',['--mode','section-details',*flags],smoke)
    for field in BUDGET:
        bad=flags.copy();at=bad.index(field,bad.index('--ros-scene-communication-budget'))
        del bad[at-2:at+2];run('missing-'+field,[*bad,'--ros-scene-check'],success=False)
    for extra in [('--ros-scene-communication-budget','robot','workers','2'),('--ros-scene-communication-budget','robot','unknown','1'),
                  ('--ros-scene-communication-worker-limit','robot','worker.0','threads','64')]:
        run('duplicate-'+str(len(report['cases'])),[*flags,*extra,'--ros-scene-check'],success=False)
    assert all(sha(path)==digest for path,digest in inputs.items())and sha(runtime)==report['runtime_sha256']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
