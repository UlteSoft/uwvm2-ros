#!/usr/bin/env python3
"""Actual Wasm bridge forwarding through two sealed ROS 2 workers and isolated component lifecycle.

Four dedicated topic endpoints form source -> bridge -> destination; the test
checks delivered payload bytes and explicit acknowledgement. Component code
runs in its separate process with zero topic roles and immutable configuration.
Core component tests separately qualify typed role IPC and malicious probes.
"""
import argparse, fcntl, json, os, resource, subprocess, time
from pathlib import Path
import run_scene_communication as comm
from run_scene_parameters import ParameterGuest, ParameterFP
from run_scene_robot_local import boundaries, deadline, receipt_done, profile, digest, grant
from run_scene_recording import u64, blob, text
from run_scene_vm import ROOT, GATE, policy, sha, verify_gate

NAMES=['bridge.create','bridge.start','bridge.stop','bridge.describe','component.open','component.start','component.stop','component.describe']
OPERATIONS=comm.OPERATIONS+[dict(name=n,words=0,bytes=0)for n in NAMES]
COMPONENT_LIMITS={'configuration.bytes':1,'ipc.bytes':8192,'message.bytes':0,'deadline.ns':1000000000,'lifetime.ns':15000000000,'shutdown.ns':2000000000,'retained.bytes':1048576}
CONFIG=b'*'
COMPONENT_DEFINITION=(text('uwvm.ros.component.roles')+text('component.idle')+profile('component.kill')+text('configuration')+text('configuration')+u64(1)+blob(digest(CONFIG))+u64(0))
COMPONENT_PROFILE=text('component.idle')+blob(digest(COMPONENT_DEFINITION))
def component_flags(images,config):
    flags=policy(OPERATIONS)+grant('steady','clock.steady','local',['clock.create','clock.read','resource.close'])
    flags+=grant('component','component','isolated-roles',[n for n in NAMES if n.startswith('component.')]+['resource.close'])
    flags+=grant('configuration','configuration','sealed-input',['artifact.open','artifact.read','artifact.describe','resource.close'])
    for action in ['artifact.open','artifact.read','artifact.describe']:flags+=['--ros-scene-interface','robot',action,'allow']
    flags+=['--ros-scene-input-budget','robot','1','1','--ros-scene-input','robot','configuration',str(config),digest(CONFIG).hex(),'1','1','1',
        '--ros-scene-component-budget','robot','1','1048576','--ros-scene-component-profile','robot','component','component.idle',digest(COMPONENT_DEFINITION).hex(),
        '--ros-scene-component-definition-hex','robot','component',COMPONENT_DEFINITION.hex()]
    for key,value in COMPONENT_LIMITS.items():flags+=['--ros-scene-component-limit','robot','component',key,str(value)]
    limits={'image-bytes':64<<20,'address-bytes':256<<20,'cpu-seconds':10,'file-bytes':4096,'open-files':128,'scratch-bytes':65536,'scratch-slots':0,
        'request-bytes':8192,'response-bytes':8192,'stderr-bytes':4096,'startup-ns':5000000000,'poll-ns':1000000,'supervisor-bytes':1048576}
    for key,value in limits.items():flags+=['--ros-scene-component-worker-limit','robot','component',key,str(value)]
    for destination,path in images.items():
        if destination=='launcher':flags+=['--ros-scene-component-worker-launcher','robot','component',str(path),str(path.stat().st_size),sha(path)]
        else:flags+=['--ros-scene-component-worker-image','robot','component','bootstrap'if destination=='/worker/bootstrap'else'runtime',destination,str(path),str(path.stat().st_size),sha(path)]
    return flags
def component_guest(width,memory,fp=False):
    g=(ParameterFP if fp else ParameterGuest)(OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall']);g.check('call $abi',2)
    g.steady=g.open('steady');g.call('clock.create',g.steady,[0]);g.clock=g.save('clock');g.body.append(g.refresh(30000000000)+' global.set $overall')
    cap=g.open('component');config=g.open('configuration');opening=COMPONENT_PROFILE+bytes(8)+text('configuration')+u64(1)+blob(digest(CONFIG))+u64(0)
    patch=[(len(COMPONENT_PROFILE),config)];boundaries(g,[n for n in NAMES if n.startswith('component.')],cap,opening)
    g.raw('component.open',cap,opening,patch,capacity=7,expected=90);g.raw('component.open',cap,opening,patch);instance=g.save('component.instance')
    until=deadline(g,'startup',4000000000);start=bytes(8)+u64(1)+bytes(16);patch=[(0,instance),(16,instance),(24,until)]
    g.raw('component.start',cap,start,patch,capacity=255,expected=90);g.raw('component.start',cap,start,patch);g.raw('component.start',cap,start,patch)
    at=512+len(COMPONENT_PROFILE);g.body.append('loop $ready');g.raw('component.describe',cap,bytes(8),[(0,instance)])
    g.body.append(g.word(at)+' i64.const 5 i64.ne if');g.bound();g.body.append('br $ready end end');g.exact(at+8,b'ready')
    g.equal(g.word(at+13+48),0)
    until=deadline(g,'stop.deadline',1900000000);stop=bytes(8)+u64(1)+bytes(8)+profile('component.kill')+bytes(8)
    patch=[(0,instance),(16,instance),(len(stop)-8,until)];receipt_done(g,'component.stop',cap,stop,patch)
    g.raw('component.describe',cap,bytes(8),[(0,instance)]);g.exact(at,text('exited'));g.equal(g.word(at+14+8),0);g.equal(g.word(at+14+40),1)
    g.equal(g.word(at+14+48),0)
    g.call('resource.close',cap,[instance]);return g
def bridge_definition(documents):
    rows={r['identity']:comm.definition.compile_definition(r)[1]for r in documents[1]['definitions']}
    return (text('uwvm.ros.bridge.topic')+text('relay')+text(comm.alias(1,'subscription'))+comm.profile(rows['subscription'])+
        text(comm.alias(1,'publisher'))+comm.profile(rows['publisher'])+profile('relay.stop')+text('std-string'))
def bridge_flags(packs,documents):
    flags=comm.configured(packs,documents)
    # Graph/event reservations are explicitly zero in the common fixture.
    for n in NAMES:flags+=['--ros-scene-interface','robot',n,'allow']
    for side in range(2):
        name='subscription'if side==0 else'publisher';alias=comm.alias(side,name)
        flags+=grant(alias,'topic','native-communication',comm.actions('topic')+['data.export'])
        flags+=['--ros-scene-limit','robot',alias,'max-resources','1','--ros-scene-communication-bind','robot',alias,f'worker.{side}',name]
    # The existing bridge input subscription needs explicit export authority.
    for i in range(len(flags)-6):
        if flags[i:i+3]==['--ros-scene-grant','robot',comm.alias(1,'subscription')]:flags[i+6]+=',data.export'
    encoded=bridge_definition(documents)
    flags+=grant('relay','bridge','topic-forwarder',[n for n in NAMES if n.startswith('bridge.')]+['resource.close'])
    flags+=['--ros-scene-bridge-budget','robot','1','1048576','1048576','1000000','--ros-scene-bridge-profile','robot','relay','relay',digest(encoded).hex(),
        '--ros-scene-bridge-definition-hex','robot','relay',encoded.hex()]
    for key,value in {'message.bytes':1024,'retained.bytes':1048576,'deadline.ns':1000000000}.items():flags+=['--ros-scene-bridge-limit','robot','relay',key,str(value)]
    return flags
def bridge_guest(documents,width,memory,fp=False):
    g=(ParameterFP if fp else ParameterGuest)(OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall']);g.check('call $abi',2)
    g.steady=g.open('steady');g.call('clock.create',g.steady,[0]);g.clock=g.save('clock');g.body.append(g.refresh(60000000000)+' global.set $overall');members=[]
    for side,document in enumerate(documents):
        rows={r['identity']:r for r in document['definitions']};names=['context','node','std-string','publisher','subscription'];caps={n:g.open(comm.alias(side,n))for n in names}
        profiles={n:comm.definition.compile_definition(rows[n])[1]for n in names};g.raw('context.open',caps['context'],comm.profile(profiles['context']));context=g.save('context'+str(side))
        body=bytes(16)+comm.profile(profiles['node'])+u64(1)+bytes(8)
        g.raw('node.open',caps['node'],body,[(0,caps['context']),(8,context),(len(body)-8,g.refresh(14000000000))],capacity=48)
        request=g.save('node.request'+str(side),520);members.append(dict(caps=caps,profiles=profiles,rows=rows,context=context,request=request,side=side))
    for m in members:
        side=m['side'];g.polling('communication.open.take',m['caps']['node'],bytes(8),[(0,m['request'])],64);m['node']=g.save('node'+str(side),568)
        for name in ('publisher','subscription'):
            p=m['rows'][name]['definition'];body=bytes(16)+u64(1)+bytes(8)+comm.profile(m['profiles'][name])+text(p['name'])+text('std_msgs/msg/String')+text(p['role'])+u64(1)+bytes(8)
            patch=[(0,m['caps']['node']),(8,m['node']),(24,m['caps']['std-string']),(len(body)-8,g.refresh(900000000))]
            g.raw('topic.open',m['caps'][name],body,patch,capacity=48);m[name+'.request']=g.save(name+'.request'+str(side),520)
    for m in members:
        for name in ('publisher','subscription'):
            g.polling('communication.open.take',m['caps'][name],bytes(8),[(0,m[name+'.request'])],64);m[name]=g.save(name+str(m['side']),568)
    pause=deadline(g,'discovery',2000000000);g.body.append('loop $discovery');g.body.append(g.refresh(0)+' '+pause+' i64.lt_u br_if $discovery end')
    cap=g.open('relay');selected=text('relay')+blob(digest(bridge_definition(documents)));source=members[1];target=members[0]
    opening=selected+u64(1)+u64(1)+bytes(32);offset=len(selected)+16
    patch=[(offset,source['caps']['subscription']),(offset+8,source['subscription']),(offset+16,source['caps']['publisher']),(offset+24,source['publisher'])]
    boundaries(g,[n for n in NAMES if n.startswith('bridge.')],cap,opening)
    g.raw('bridge.create',cap,opening,patch,capacity=7,expected=90);g.raw('bridge.create',cap,opening,patch);bridge=g.save('bridge')
    until=deadline(g,'bridge.start.deadline',900000000);start=bytes(8)+u64(1)+bytes(16);patch=[(0,bridge),(16,bridge),(24,until)]
    g.raw('bridge.start',cap,start,patch);g.raw('bridge.start',cap,start,patch);g.raw('bridge.describe',cap,bytes(8),[(0,bridge)])
    payload=text('actual Wasm directed bridge');message=text('std_msgs/msg/String')+text('scene-canonical')+blob(payload)
    publish=bytes(8)+u64(1)+bytes(8)+message+bytes(8);until=deadline(g,'publish.deadline',900000000)
    g.raw('topic.publish',target['caps']['publisher'],publish,[(0,target['publisher']),(16,target['publisher']),(len(publish)-8,until)])
    until=deadline(g,'receive.deadline',900000000);g.polling('topic.reserve',target['caps']['subscription'],bytes(16),[(0,target['subscription']),(8,until)],capacity=4096,output=32000)
    refs=[g.save('reservation'+str(i),32000+8*i)for i in range(4)]
    address='i64.const 32000 '+g.word(2048)+f' i64.add i64.const {len(payload)} i64.sub'+(' i32.wrap_i64'if width==32 else'')
    g.body.append(f'i{width}.const 28000 {address} i{width}.const {len(payload)} memory.copy $work $work');g.exact(28000,payload)
    ack=bytes(8)+u64(1)+bytes(32);patch=[(0,target['subscription'])]+[(16+8*i,value)for i,value in enumerate(refs)]
    g.polling('topic.acknowledge',target['caps']['subscription'],ack,patch)
    until=deadline(g,'bridge.stop.deadline',900000000);stop=bytes(8)+u64(1)+bytes(8)+profile('relay.stop')+bytes(8)
    receipt_done(g,'bridge.stop',cap,stop,[(0,bridge),(16,bridge),(len(stop)-8,until)])
    g.raw('bridge.describe',cap,bytes(8),[(0,bridge)]);g.call('resource.close',cap,[bridge])
    for m in members:
        for name in ('publisher','subscription'):
            g.polling('resource.close',m['caps'][name],bytes(8),[(0,m[name])],0)
            g.polling('request.release',m['caps'][name],bytes(8),[(0,m[name+'.request'])],8)
    for m in members:g.polling('resource.close',m['caps']['node'],bytes(8),[(0,m['node'])],0)
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--pair-receipt',type=Path,required=True);p.add_argument('--component-receipt',type=Path,required=True);p.add_argument('--mode',choices=['prepare','execute','all'],default='all');p.add_argument('--first-layout',action='store_true');a=p.parse_args()
    verify_gate();assert os.sched_getaffinity(0).issubset({4,5,6,7});resource.setrlimit(resource.RLIMIT_CORE,(0,0));source=a.source_root.resolve();out=a.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    pair=a.pair_receipt.resolve();component=a.component_receipt.resolve();evidence=json.loads(pair.read_text());ce=json.loads(component.read_text());assert evidence['complete']and ce['complete']
    packs=[];documents=[];inputs={str(pair):sha(pair),str(component):sha(component),str(source/'source-manifest.json'):sha(source/'source-manifest.json')}
    for side in range(2):
        path=pair.parent/f'pack-{side}/manifest.json';assert evidence['pack_sha256'][str(path)]==sha(path);pack=json.loads(path.read_text());packs.append(pack);inputs[str(path)]=sha(path)
        for row in [pack['launcher'],*pack['files']]:assert sha(Path(row['path']))==row['sha256'];inputs[row['path']]=row['sha256']
        path=pair.parent/f'peer-{side}.json';documents.append(json.loads(path.read_text()));inputs[str(path)]=sha(path)
    for name in ['run_scene_robot_process.py','run_scene_robot_local.py','robot_vm_component_backend.cpp','run_scene_vm.py','run_scene_recording.py','run_scene_communication.py','run_scene_parameters.py','run_scene_fp.py','run_scene_buffers.py']:
        path=Path(__file__).with_name(name).resolve();inputs[str(path)]=sha(path)
    images={'/worker/bootstrap':component.parent/'images/bootstrap','/worker/backend.so':out/'backend.so','launcher':component.parent/'images/bwrap'}
    for name in ['ld-linux-x86-64.so.2','libc.so.6','libstdc++.so.6','libgcc_s.so.1','libm.so.6']:
        images[('/lib64/'if name.startswith('ld-')else'/lib/x86_64-linux-gnu/')+name]=component.parent/'images'/name
    for name,path in images.items():
        if name=='/worker/backend.so':continue
        if name=='/worker/bootstrap':assert ce['binaries'][str(path)]==sha(path)
        else:assert sha(path)in ce['external'].values()
        inputs[str(path)]=sha(path)
    config=out/'configuration.bin'
    if a.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared']and report['inputs']==inputs
    else:
        assert not(out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,inputs=inputs,cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if a.mode!='execute':
        config.write_bytes(CONFIG);argv=['/usr/bin/clang++-22','-std=c++23','-O3','-g','-fPIC','-shared','-Wall','-Wextra','-Werror','-Isrc','test/0084.ros-runtime/robot_vm_component_backend.cpp','-o',str(images['/worker/backend.so'])]
        done=subprocess.run(argv,capture_output=True,text=True,timeout=60);assert not done.returncode,done.stderr;report['backend_compile']=argv;report['backend_sha256']=sha(images['/worker/backend.so'])
        for width,memory in ([(32,0)]if a.first_layout else[(32,0),(64,0),(32,1),(64,1)]):
            for scenario in ('component','component-fp','bridge','bridge-fp'):
                g=component_guest(width,memory,scenario.endswith('-fp'))if scenario.startswith('component')else bridge_guest(documents,width,memory,scenario.endswith('-fp'))
                assert g.cursor<45000;name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                done=subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],capture_output=True,text=True,timeout=30);assert not done.returncode,done.stderr
                report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,sha256=sha(wasm),wat_sha256=sha(wat),assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0)));save()
        report['prepared']=True;save()
    if a.mode=='prepare':print('PASS robot process preparation');return
    assert sha(images['/worker/backend.so'])==report['backend_sha256']and config.read_bytes()==CONFIG
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';br=json.loads(build.read_text());assert br['complete']and br['binary_sha256']==sha(runtime)and br['source_root']==str(source)
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),build_sha256=sha(build))
    def run(name,flags,wasm=None,success=True):
        verify_gate();argv=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*flags]
        if wasm:argv+=['--run',str(wasm)]
        start=time.monotonic()
        with(out.parent/'.communication-transport-ports.lock').open('a')as lock:
            fcntl.flock(lock,fcntl.LOCK_EX);done=subprocess.run(argv,capture_output=True,text=True,timeout=100)
        (out/(name+'.stdout')).write_text(done.stdout);(out/(name+'.stderr')).write_text(done.stderr);passed=(done.returncode==0)==success
        report['cases'].append(dict(name=name,argv=argv,exit=done.returncode,passed=passed,seconds=time.monotonic()-start));save();assert passed,(name,done.returncode,done.stderr[-7000:]);print('PASS '+name,flush=True)
    flags={'component':component_flags(images,config),'bridge':bridge_flags(packs,documents)}
    for kind,options in flags.items():run(kind+'-static',options+['--ros-scene-check'])
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256']and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        for engine in ('int','aot'):run(guest['name']+'-'+engine,['--runtime-'+engine,*flags[guest['scenario'].split('-')[0]]],wasm)
    for key in COMPONENT_LIMITS:
        bad=flags['component'].copy();at=next(i for i in range(len(bad)-4)if bad[i:i+4]==['--ros-scene-component-limit','robot','component',key]);del bad[at:at+5]
        run('missing-component-'+key,bad+['--ros-scene-check'],success=False)
    report['inputs_unchanged']=all(sha(Path(path))==value for path,value in inputs.items());assert report['inputs_unchanged']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()

if __name__=='__main__':main()
