#!/usr/bin/env python3
"""Qualify all twelve parameter imports against actual isolated ROS node origins.

The guest uses only ROSv2. Tests cover both value algebras, exact node ownership,
atomic/non-atomic updates, deferred validation, cursor fencing and cleanup.
Native parameter-service replication is deliberately outside this local store.
Every compiler and child runs inside the existing 64 GiB, zero-swap cgroup.
"""
import argparse,fcntl,json,os,resource,subprocess,sys,time
from pathlib import Path
import run_scene_communication as communication
from run_scene_communication import CommunicationGuest,profile,alias,definition,worker_definition
from run_scene_recording import RecordingGuest,RecordingFP,u64,blob,text
from run_scene_vm import ROOT,GATE,policy,sha,verify_gate
sys.path.insert(0,str(ROOT/'tools/ros_scene'))
import encode_parameter_definition as encoder

NAMES=['parameter.open','parameter.declare','parameter.undeclare','parameter.get','parameter.describe','parameter.list',
       'parameter.set','parameter.set.atomic','parameter.take','parameter.respond','parameter.exists','parameter.search']
OPERATIONS=communication.OPERATIONS+[dict(name=n,words=0,bytes=0)for n in NAMES]
LIMITS={'scopes':2,'parameters':64,'value_bytes':65536,'event_slots':64,'event_bytes':262144,'cursors':4,
        'workspace_bytes':262144,'batch':32,'request_bytes':8192,'receipt_bytes':65536,'pending_bytes':65536,'cursor_bytes':65536,
        'validation_timeout_ns':5000000000,'cursor_timeout_ns':5000000000,
        'value.bytes':8192,'value.nodes':512,'value.elements':512,'value.depth':16,'definition.bytes':65536,'definition.keys':32}
BUDGET={'bindings':4,'scopes':8,'parameters':256,'value-bytes':262144,'event-slots':256,'event-bytes':1048576,'cursors':16,
        'workspace-bytes':1048576,'request-bytes':32768,'receipt-bytes':262144,'pending-bytes':262144,'cursor-bytes':262144,'definition-bytes':1048576}
VALUES={'not_set':None,'boolean':True,'integer':7,'floating':1.25,'string':'robot\u03bb','bytes':'AAH/',
        'booleans':[True,False],'integers':[0,-1,2147483647],'floats':[0.25,-1.5],'strings':['robot','\u03bb'],
        'xmlrpc_array':[{'type':'integer','value':7},{'type':'string','value':'nested'}],
        'xmlrpc_struct':[{'name':'enabled','value':{'type':'boolean','value':True}}],'xmlrpc_datetime':'20260102T03:04:05'}

def descriptor(name,kind):
    return dict(name=name,type=kind,description='',additional_constraints='',read_only=False,dynamic_type=False,range=None)
def encoded_descriptor(value):
    out=encoder.Writer();encoder.descriptor(out,value);return bytes(out.data)
def encoded_value(kind,value,algebra):
    out=encoder.Writer();encoder.encode_value(out,dict(type=kind,value=value),algebra,False,encoder.ValueBudget());return bytes(out.data)
def parameter_definitions(documents):
    row=next(row for row in documents[0]['definitions']if row['family']=='node')
    node=definition.compile_definition(row)[1];result={}
    for name,algebra,deferred in [('ros2','ros2',False),('ros1','ros1',False),('deferred','ros2',True),('zero','ros2',False)]:
        kinds=([kind for kind,tag in encoder.KINDS.items()if tag<=9]if algebra=='ros2'else
               [kind for kind,tag in encoder.KINDS.items()if tag in [1,2,3,4,5,32,33,34]])
        if name in ['deferred','zero']:kinds=['integer']
        keys=[]
        for kind in kinds:
            key=('/plant/'if algebra=='ros1'else'')+kind
            keys.append(dict(descriptor=descriptor(key,kind),types=[kind],integer_range=None,floating_range=None,override=None))
        if algebra=='ros1':keys.append(dict(descriptor=descriptor('/plant/branch/integer','integer'),types=['integer'],integer_range=None,floating_range=None,override=None))
        document=dict(profile=dict(identity='parameters.'+name),node_profile=dict(identity=node['identity'],sha256=node['profile_sha256']),
            algebra=algebra,nonfinite=False,deferred_validation=deferred,floating_tolerance=0.0,
            search_root='/plant'if algebra=='ros1'else'',search_candidates=2 if algebra=='ros1'else 0,
            keys=sorted(keys,key=lambda key:key['descriptor']['name']))
        data,metadata=encoder.encode(document);result[name]=dict(document=document,data=data,metadata=metadata,kinds=kinds)
    return result

def configured(packs,documents,definitions):
    flags=communication.configured(packs,documents)
    for name in NAMES:flags+=['--ros-scene-interface','robot',name,'allow']
    for name,value in BUDGET.items():flags+=['--ros-scene-parameter-budget','robot',name,str(value)]
    for name,entry in definitions.items():
        metadata=entry['metadata'];selected=metadata['profile'];node=metadata['node_profile'];ident='parameters.'+name
        flags+=['--ros-scene-grant','robot',ident,'parameter',ident,'local-parameters',','.join(NAMES+['request.describe','request.release','resource.close']),
            '--ros-scene-parameter-profile','robot',ident,alias(0,'node'),selected['identity'],selected['sha256'],node['identity'],node['sha256'],metadata['algebra']]
        for field,value in LIMITS.items():flags+=['--ros-scene-parameter-limit','robot',ident,field,str(value)]
        flags+=['--ros-scene-parameter-definition-hex','robot',ident,entry['data'].hex()]
    flags+=['--ros-scene-limit','robot','parameters.zero','delivery','may-reject',
            '--ros-scene-limit','robot','parameters.zero','calls-per-window','0']
    return flags

class ParameterGuest(CommunicationGuest):
    def polling(self,name,cap,payload,patches=(),capacity=1024,output=512,length=2048):
        self.loop_id+=1;label='poll'+str(self.loop_id)
        self.body.append(f'block ${label}.done loop ${label}')
        self.dynamic(name,cap,payload,patches,capacity,output,length)
        self.body.append(f'global.get $status i32.eqz br_if ${label}.done')
        # Distinct named assertions identify an unexpected provider status in a
        # Guest without adding WASI stdout or another diagnostic permission.
        for code in [5,9,12,13,16,17,22,90,95,110,125]:self.check(f'global.get $status i32.const {code} i32.eq')
        self.check('global.get $status',11);self.bound();self.body.append(f'br ${label} end end')
    def dynamic(self,name,cap,payload,patches=(),capacity=1024,output=512,length=2048):
        # Store the result in a Wasm global before checking FP arithmetic. Do not
        # extract/modify assertion text: an FP assertion contains extra checks.
        address=self.cursor;self.cursor+=len(payload)+8
        self.data.append(f'(data (memory $work) (i{self.width}.const {address}) "'+''.join(f'\\{b:02x}'for b in payload)+'")')
        for offset,value in patches:self.write(address+offset,value)
        if isinstance(cap,int):cap='i64.const '+str(cap)
        self.check(f'{cap} i32.const {self.memory} i64.const {address} i64.const {len(payload)} i64.const {output} '
                   f'i64.const {capacity} i64.const {length} call ${name} global.set $status i32.const 0')
class ParameterFP(RecordingFP,ParameterGuest):
    # The FP fixture imports its own isolated Guest class; use the explicit
    # communication renderer so the retained call status remains i32.
    render=CommunicationGuest.render

def successful_guest(documents,definitions,width,memory,fp=False):
    g=(ParameterFP if fp else ParameterGuest)(OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall'])
    g.check('call $abi',2);g.open('absent',13);g.steady=g.open('steady');g.call('clock.create',g.steady,[0]);g.clock=g.save('clock')
    g.body.append(g.refresh(90000000000)+' global.set $overall');members=[]
    for side,document in enumerate(documents):
        rows={row['family']:row for row in document['definitions']if row['family']in ['node','context']}
        caps={family:g.open(alias(side,rows[family]['identity']))for family in ['context','node']}
        profiles={family:definition.compile_definition(row)[1]for family,row in rows.items()}
        g.raw('context.open',caps['context'],profile(profiles['context']));context=g.save('context'+str(side))
        body=bytes(16)+profile(profiles['node'])+u64(1)+bytes(8)
        g.raw('node.open',caps['node'],body,[(0,caps['context']),(8,context),(len(body)-8,g.refresh(14000000000))],capacity=48)
        request=g.save('node.request'+str(side),520)
        members.append(dict(cap=caps['node'],request=request,context=context,context_cap=caps['context']))
    # Bilateral worker startup requires both actual node origins before either
    # asynchronous opening can complete. Submit both, then publish each result.
    for side,member in enumerate(members):
        g.polling('communication.open.take',member['cap'],bytes(8),[(0,member['request'])],64)
        member['node']=g.save('node'+str(side),568)
    caps={name:g.open('parameters.'+name)for name in definitions}
    scopes={}
    for name,entry in definitions.items():
        p=entry['metadata']['profile'];body=bytes(16)+text(p['identity'])+blob(bytes.fromhex(p['sha256']))
        patches=[(0,members[0]['cap']),(8,members[0]['node'])];cap=caps[name]
        if name=='zero':g.raw('parameter.open',cap,body,patches,expected=11);continue
        for kw in [dict(memory=memory+1),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length='request')]:
            g.raw('parameter.open',cap,body,patches,expected=22,**kw)
        g.raw('parameter.open',cap,body,[(0,members[1]['cap']),(8,members[1]['node'])],expected=13)
        g.raw('parameter.open',cap,body,patches,capacity=7,expected=90);g.equal(g.word(2048),8)
        g.raw('parameter.open',cap,body,patches);scopes[name]=g.save('scope.'+name)
    def scoped(name,operation,tail=b'',**kwargs):
        g.raw(operation,caps[name],bytes(8)+tail,[(0,scopes[name])],**kwargs)
    def mutate(name,operation,sequence,tail,**kwargs):
        g.raw(operation,caps[name],bytes(8)+u64(sequence)+bytes(8)+tail,[(0,scopes[name]),(16,scopes[name])],**kwargs)
    for name in ['ros2','ros1','deferred']:
        entry=definitions[name];algebra=entry['metadata']['algebra'];prefix='/plant/'if algebra=='ros1'else''
        revision=1;sequence=0
        for kind in entry['kinds']:
            key=prefix+kind;value=encoded_value(kind,VALUES[kind],algebra);d=encoded_descriptor(descriptor(key,kind));sequence+=1
            mutate(name,'parameter.declare',sequence,d+value,capacity=7,expected=90)
            mutate(name,'parameter.declare',sequence,d+value);request=g.save(name+'.declare.'+kind)
            g.exact(520,text('succeeded')+text('committed'));revision+=1
            mutate(name,'parameter.declare',sequence,d+value);g.check(g.word(512)+' '+request+' i64.ne')
            scoped(name,'parameter.get',u64(1)+text(key));g.exact(512,u64(1)+text(key)+value+u64(1)+u64(revision))
            scoped(name,'parameter.describe',u64(1)+text(key));g.exact(512,u64(1)+d)
            scoped(name,'parameter.exists',text(key),capacity=23,expected=90);g.equal(g.word(2048),24)
            scoped(name,'parameter.exists',text(key));g.exact(512,u64(1)+u64(kind!='not_set')+u64(revision))
            scoped(name,'parameter.take',capacity=7,expected=90)
            scoped(name,'parameter.take');g.check(g.word(512)+' '+request+' i64.ne');g.exact(520,text('committed')+text('declare'))
        scoped(name,'parameter.take',expected=11)
        scoped(name,'parameter.get',u64(1)+text(prefix+'forbidden'),expected=13)
        scoped(name,'parameter.list',u64(0)+u64(0)+u64(0)+u64(1));g.equal(g.word(512),revision)
        cursor=g.save(name+'.cursor',528)if name!='deferred'else None
        if name!='deferred':g.equal(g.word(520),1)
        integer_key=prefix+'integer';new_value=encoded_value('integer',9,algebra)
        sequence+=1;tail=u64(0)+u64(1)+text(integer_key)+new_value
        mutate(name,'parameter.set.atomic',sequence,tail);request=g.save(name+'.set')
        if name=='deferred':
            g.exact(520,text('queued')+text('locally_accepted'))
            scoped(name,'parameter.get',u64(1)+text(integer_key));g.exact(512,u64(1)+text(integer_key)+encoded_value('integer',7,algebra)+u64(1)+u64(revision))
            response=bytes(16)+u64(1)+u64(0)+text('')
            patches=[(0,scopes[name]),(8,request)]
            g.raw('parameter.respond',caps[name],response,patches,expected=11)
            scoped(name,'parameter.take');g.check(g.word(512)+' '+request+' i64.ne');g.exact(520,text('validate')+text('set'))
            g.raw('parameter.respond',caps[name],response,patches,capacity=7,expected=90)
            g.raw('parameter.respond',caps[name],response,patches);g.exact(520,text('succeeded')+text('committed'))
            g.raw('parameter.respond',caps[name],response,patches);g.check(g.word(512)+' '+request+' i64.ne')
        else:g.exact(520,text('succeeded')+text('committed'))
        revision+=1;mutate(name,'parameter.set.atomic',sequence,tail);g.check(g.word(512)+' '+request+' i64.ne')
        mutate(name,'parameter.set.atomic',sequence,tail+b'\0',expected=22)
        g.raw('request.describe',caps[name],bytes(8),[(0,request)]);g.exact(520,text('succeeded')+text('committed'))
        g.raw('request.release',caps[name],bytes(8),[(0,request)],capacity=7,expected=90)
        g.raw('request.release',caps[name],bytes(8),[(0,request)]);g.equal(g.word(512),1)
        g.raw('request.release',caps[name],bytes(8),[(0,request)]);g.equal(g.word(512),0)
        g.raw('request.describe',caps[name],bytes(8),[(0,request)],expected=9)
        mutate(name,'parameter.set.atomic',sequence,tail);g.check(g.word(512)+' '+request+' i64.ne')
        scoped(name,'parameter.get',u64(1)+text(integer_key));g.exact(512,u64(1)+text(integer_key)+new_value+u64(1)+u64(revision))
        if cursor:
            body=bytes(8)+u64(0)+u64(0)+u64(1)+bytes(8)+u64(1)
            g.raw('parameter.list',caps[name],body,[(0,scopes[name]),(32,cursor)],expected=17)
        if name=='ros1':
            scoped(name,'parameter.search',text('integer')+text('/plant/branch'));g.exact(512,u64(1)+text('/plant/integer')+u64(revision))
            scoped(name,'parameter.search',text('integer')+text('/outside'),expected=13)
        else:scoped(name,'parameter.search',text('integer')+text('/plant'),expected=13)
        if name!='deferred':
            sequence+=1;tail=u64(0)+u64(1)+text(integer_key)+encoded_value('integer',11,algebra)
            mutate(name,'parameter.set',sequence,tail);g.exact(520,text('succeeded')+text('committed'));revision+=1
            sequence+=1;mutate(name,'parameter.undeclare',sequence,text(integer_key)+u64(1)+u64(revision));revision+=1
            scoped(name,'parameter.exists',text(integer_key));g.exact(512,u64(0)+u64(0)+u64(revision))
    # One store cannot use another store's retained scope, even on the same node.
    g.raw('parameter.get',caps['ros2'],bytes(8)+u64(1)+text('integer'),[(0,scopes['ros1'])],expected=9)
    for name in scopes:g.call('resource.close',caps[name],[scopes[name]],capacity=0)
    for member in members:
        g.polling('resource.close',member['cap'],bytes(8),[(0,member['node'])],0)
    for member in members:
        g.raw('context.shutdown',member['context_cap'],bytes(8)+u64(1)+bytes(8),[(0,member['context']),(16,member['context'])])
        g.call('resource.close',member['context_cap'],[member['context']],capacity=0)
    assert g.cursor<45000,g.cursor
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--pair-receipt',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mode',choices=['all','prepare','execute'],default='all');p.add_argument('--first-layout',action='store_true')
    a=p.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset({4,5,6,7});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    source=a.source_root.resolve();pair=a.pair_receipt.resolve();evidence=json.loads(pair.read_text());assert evidence['complete']
    packs=[];documents=[];inputs={str(pair):sha(pair),str(source/'source-manifest.json'):sha(source/'source-manifest.json')}
    for i in range(2):
        path=pair.parent/f'pack-{i}/manifest.json';assert evidence['pack_sha256'][str(path)]==sha(path)
        pack=json.loads(path.read_text());packs.append(pack);inputs[str(path)]=sha(path)
        for image in [pack['launcher'],*pack['files']]:
            assert Path(image['path']).stat().st_size==image['bytes']and sha(image['path'])==image['sha256'];inputs[image['path']]=image['sha256']
        path=pair.parent/f'peer-{i}.json';document=json.loads(path.read_text());documents.append(document);inputs[str(path)]=sha(path)
        data,_=worker_definition.compile_definition(document);assert data==Path(next(x['path']for x in pack['files']if x['destination']=='/worker/profile')).read_bytes()
    for path in [Path(__file__),Path(communication.__file__),Path(encoder.__file__),Path(definition.__file__),Path(worker_definition.__file__),
                 *[Path(__file__).with_name(x)for x in ['run_scene_vm.py','run_scene_recording.py','run_scene_buffers.py','run_scene_fp.py']]]:inputs[str(path.resolve())]=sha(path)
    definitions=parameter_definitions(documents);out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    if a.mode=='execute':
        report=json.loads((out/'results.json').read_text());assert report['prepared']and report['inputs']==inputs
    else:
        assert not(out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,source_root=str(source),inputs=inputs,
            cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),commands=[],guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if a.mode!='execute':
        for width,memory in ([(32,0)]if a.first_layout else[(32,0),(64,0),(32,1),(64,1)]):
            for scenario in ['success','fp','denied','no-grants']:
                if scenario in ['success','fp']:g=successful_guest(documents,definitions,width,memory,scenario=='fp')
                else:
                    g=RecordingGuest(OPERATIONS,width,memory);g.check('call $abi',13 if scenario=='denied'else 2);g.open('missing',13)
                    for name in NAMES:g.raw(name,1,bytes(8),expected=13)
                name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                argv=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                done=subprocess.run(argv,capture_output=True,text=True,timeout=30);report['commands'].append(dict(name=name,argv=argv,exit=done.returncode));save();assert done.returncode==0,done.stderr
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0),wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if a.mode=='prepare':print('PASS parameter guest preparation');return
    if not a.runtime:p.error('--runtime required for execution')
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text())
    assert receipt['complete']and receipt['source_root']==str(source)and receipt['binary_sha256']==sha(runtime)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),build_sha256=sha(build));flags=configured(packs,documents,definitions)
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
        selected=flags if guest['scenario']in ['success','fp']else policy(OPERATIONS,'deny'if guest['scenario']=='denied'else'allow')
        for engine in ['int','aot']:run(guest['name']+'-'+engine,['--runtime-'+engine,*selected],wasm)
    smoke=out/'no-grants-32-0.wasm';run('hidden',['--runtime-int'],smoke,False)
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    for field in BUDGET:
        bad=flags.copy();at=next(i for i in range(len(bad)-3)if bad[i:i+3]==['--ros-scene-parameter-budget','robot',field]);del bad[at:at+4]
        run('missing-budget-'+field,[*bad,'--ros-scene-check'],success=False)
    for field in LIMITS:
        bad=flags.copy();at=next(i for i in range(len(bad)-4)if bad[i:i+4]==['--ros-scene-parameter-limit','robot','parameters.ros2',field]);del bad[at:at+5]
        run('missing-limit-'+field,[*bad,'--ros-scene-check'],success=False)
    for name,extra in [('duplicate-budget',['--ros-scene-parameter-budget','robot','bindings','4']),
        ('unknown-budget',['--ros-scene-parameter-budget','robot','unknown','1']),
        ('duplicate-limit',['--ros-scene-parameter-limit','robot','parameters.ros2','scopes','2']),
        ('unknown-limit',['--ros-scene-parameter-limit','robot','parameters.ros2','unknown','1']),
        ('duplicate-definition',['--ros-scene-parameter-definition-hex','robot','parameters.ros2',definitions['ros2']['data'].hex()])]:
        run(name,[*flags,*extra,'--ros-scene-check'],success=False)
    assert all(sha(path)==digest for path,digest in inputs.items())and sha(runtime)==report['runtime_sha256']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
