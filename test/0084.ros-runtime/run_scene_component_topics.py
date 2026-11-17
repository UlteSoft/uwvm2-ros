#!/usr/bin/env python3
"""Real Wasm -> isolated component roles -> sealed ROS 2 workers -> Wasm roundtrip.

Four dedicated native endpoints transfer one exact String through the isolated
component's subscriber and publisher roles. The child acknowledges its retained
input after known publication acceptance. The guest observes confirmed child
exit with no unknown/retained debt and checks exact forwarded bytes, then
explicitly closes endpoints and releases their creation receipts.
"""
import argparse
import fcntl
import json
import os
from pathlib import Path
import subprocess
import time

import run_scene_robot_process as process
from run_scene_robot_local import deadline, digest, profile, receipt_done
from run_scene_recording import blob, text, u64
from run_scene_parameters import ParameterGuest, ParameterFP
from run_scene_vm import ROOT, GATE, sha, verify_gate
comm=process.comm
CONFIG=b'+'


def definition(documents):
    rows={row['identity']:comm.definition.compile_definition(row)[1] for row in documents[1]['definitions']}
    return (text('uwvm.ros.component.roles')+text('component.forward')+profile('component.kill')+
        text('configuration')+text('configuration')+u64(1)+blob(digest(CONFIG))+u64(2)+
        text(comm.alias(1,'publisher'))+comm.profile(rows['publisher'])+u64(1)+text('std-string')+
        text(comm.alias(1,'subscription'))+comm.profile(rows['subscription'])+u64(0)+text('std-string'))


def rows(argv):
    starts=[i for i,token in enumerate(argv) if token.startswith('--')]+[len(argv)]
    return [argv[a:b] for a,b in zip(starts,starts[1:])]


def flags(packs,documents,images,configuration):
    native=process.bridge_flags(packs,documents)
    native=[row for row in rows(native) if not row[0].startswith('--ros-scene-bridge-') and row[:3]!=['--ros-scene-grant','robot','relay']]
    for row in native:
        if row[:3]==['--ros-scene-communication-budget','robot','carrier-bytes']:row[3]='4096'
    component=rows(process.component_flags(images,configuration));encoded=definition(documents)
    for row in component:
        if row[0]=='--ros-scene-input':row[4]=digest(CONFIG).hex()
        elif row[0]=='--ros-scene-component-profile':row[3:]=['component.forward',digest(encoded).hex()]
        elif row[0]=='--ros-scene-component-definition-hex':row[3]=encoded.hex()
        elif row[:4]==['--ros-scene-component-limit','robot','component','message.bytes']:row[4]='1024'
    result=[];seen={}
    for row in native+component:
        key=tuple(row[:3]) if row[0] in ['--ros-scene-interface','--ros-scene-grant'] else tuple(row)
        if key in seen:assert seen[key]==row;continue
        seen[key]=row;result+=row
    return result


def close_endpoints(g,members):
    for member in members:
        for name in ('publisher','subscription'):
            g.polling('resource.close',member['caps'][name],bytes(8),[(0,member[name])],0)
            g.polling('request.release',member['caps'][name],bytes(8),[(0,member[name+'.request'])],8)
    for member in members:g.polling('resource.close',member['caps']['node'],bytes(8),[(0,member['node'])],0)


def guest(documents,width,memory,fp,denied=False):
    g=(ParameterFP if fp else ParameterGuest)(process.OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall']);g.check('call $abi',2)
    g.steady=g.open('steady');g.call('clock.create',g.steady,[0]);g.clock=g.save('clock');g.body.append(g.refresh(60000000000)+' global.set $overall');members=[]
    for side,document in enumerate(documents):
        definitions={row['identity']:row for row in document['definitions']};names=['context','node','std-string','publisher','subscription'];caps={n:g.open(comm.alias(side,n)) for n in names}
        profiles={n:comm.definition.compile_definition(definitions[n])[1] for n in names}
        g.raw('context.open',caps['context'],comm.profile(profiles['context']));context=g.save('context'+str(side))
        body=bytes(16)+comm.profile(profiles['node'])+u64(1)+bytes(8)
        g.raw('node.open',caps['node'],body,[(0,caps['context']),(8,context),(len(body)-8,g.refresh(14000000000))],capacity=48)
        members.append(dict(caps=caps,profiles=profiles,rows=definitions,request=g.save('node.request'+str(side),520),side=side))
    for member in members:
        side=member['side'];g.polling('communication.open.take',member['caps']['node'],bytes(8),[(0,member['request'])],64);member['node']=g.save('node'+str(side),568)
        for name in ('publisher','subscription'):
            d=member['rows'][name]['definition'];body=bytes(16)+u64(1)+bytes(8)+comm.profile(member['profiles'][name])+text(d['name'])+text('std_msgs/msg/String')+text(d['role'])+u64(1)+bytes(8)
            patch=[(0,member['caps']['node']),(8,member['node']),(24,member['caps']['std-string']),(len(body)-8,g.refresh(900000000))]
            g.raw('topic.open',member['caps'][name],body,patch,capacity=48);member[name+'.request']=g.save(name+'.request'+str(side),520)
    for member in members:
        for name in ('publisher','subscription'):
            g.polling('communication.open.take',member['caps'][name],bytes(8),[(0,member[name+'.request'])],64);member[name]=g.save(name+str(member['side']),568)
    until=deadline(g,'discovery',2000000000);g.body.append('loop $discovery');g.body.append(g.refresh(0)+' '+until+' i64.lt_u br_if $discovery end')
    target,source=members;cap=g.open('component');config=g.open('configuration');selected=text('component.forward')+blob(digest(definition(documents)))
    opening=selected+bytes(8)+text('configuration')+u64(1)+blob(digest(CONFIG))+u64(2);patch=[(len(selected),config)]
    for slot,name in [(1,'publisher'),(2,'subscription')]:
        offset=len(opening);opening+=u64(slot)+bytes(16);patch.extend([(offset+8,source['caps'][name]),(offset+16,source[name])])
    g.raw('component.open',cap,opening,patch,expected=13 if denied else 0)
    if denied:
        close_endpoints(g,members);return g
    instance=g.save('component.instance')
    payload=text('actual Wasm isolated component');message=text('std_msgs/msg/String')+text('scene-canonical')+blob(payload)
    publishing=bytes(8)+u64(1)+bytes(8)+message+bytes(8);until=deadline(g,'publish.deadline',900000000)
    g.raw('topic.publish',target['caps']['publisher'],publishing,[(0,target['publisher']),(16,target['publisher']),(len(publishing)-8,until)])
    until=deadline(g,'start.deadline',4000000000);starting=bytes(8)+u64(1)+bytes(16);patch=[(0,instance),(16,instance),(24,until)]
    g.raw('component.start',cap,starting,patch);g.raw('component.start',cap,starting,patch)
    # Wait for actual worker exit before opening the guest's bounded receive.
    at=512+len(selected);g.body.append('loop $exited');g.raw('component.describe',cap,bytes(8),[(0,instance)])
    g.body.append(g.word(at)+' i64.const 6 i64.ne if');g.bound();g.body.append('br $exited end end');g.exact(at,text('exited'))
    base=at+14;g.equal(g.word(base+8),0);g.equal(g.word(base+16),1048576);g.equal(g.word(base+24),0);g.equal(g.word(base+32),0);g.equal(g.word(base+40),1)
    until=deadline(g,'receive.deadline',900000000);g.polling('topic.reserve',target['caps']['subscription'],bytes(16),[(0,target['subscription']),(8,until)],capacity=4096,output=32000)
    references=[g.save('reservation'+str(i),32000+8*i) for i in range(4)]
    address='i64.const 32000 '+g.word(2048)+f' i64.add i64.const {len(payload)} i64.sub'+(' i32.wrap_i64' if width==32 else '')
    g.body.append(f'i{width}.const 28000 {address} i{width}.const {len(payload)} memory.copy $work $work');g.exact(28000,payload)
    ack=bytes(8)+u64(1)+bytes(32);patch=[(0,target['subscription'])]+[(16+8*i,value) for i,value in enumerate(references)]
    g.polling('topic.acknowledge',target['caps']['subscription'],ack,patch)
    until=deadline(g,'stop.deadline',1900000000);stop=bytes(8)+u64(1)+bytes(8)+profile('component.kill')+bytes(8)
    receipt_done(g,'component.stop',cap,stop,[(0,instance),(16,instance),(len(stop)-8,until)]);g.call('resource.close',cap,[instance])
    close_endpoints(g,members)
    return g


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--process-fixture',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mode',choices=['prepare','execute','all'],default='all');p.add_argument('--first-layout',action='store_true');a=p.parse_args()
    verify_gate();assert os.sched_getaffinity(0).issubset({4,5,6,7});out=a.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    fixture=a.process_fixture.resolve();fr=json.loads((fixture/'results.json').read_text());assert fr['complete']
    pair=next(Path(k).parent for k in fr['inputs'] if k.endswith('/peer-0.json'))
    packs=[json.loads((pair/f'pack-{i}/manifest.json').read_text()) for i in range(2)];documents=[json.loads((pair/f'peer-{i}.json').read_text()) for i in range(2)]
    static=next(c['argv'] for c in fr['cases'] if c['name']=='component-static');images={}
    for row in rows(static[static.index('--ros-scene-create'):]):
        if row[0]=='--ros-scene-component-worker-launcher':images['launcher']=Path(row[3])
        elif row[0]=='--ros-scene-component-worker-image':images[row[4]]=Path(row[5])
    inputs={path:value for path,value in fr['inputs'].items() if not path.endswith('/source-manifest.json')}
    for path,value in inputs.items():assert sha(Path(path))==value,path
    for name in ['run_scene_component_topics.py','component_vm_topic_backend.cpp','run_scene_robot_process.py']:
        path=Path(__file__).with_name(name).resolve();inputs[str(path)]=sha(path)
    source=a.source_root.resolve();inputs[str(source/'source-manifest.json')]=sha(source/'source-manifest.json')
    configuration=out/'configuration.bin';images['/worker/backend.so']=out/'backend.so'
    if a.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['inputs']==inputs
    else:
        assert not(out/'results.json').exists();report=dict(complete=False,prepared=False,scope=__doc__,inputs=inputs,cgroup=str(GATE),inode=GATE.stat().st_ino,
            memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    if a.mode!='execute':
        configuration.write_bytes(CONFIG);argv=['/usr/bin/clang++-22','-std=c++23','-O3','-g','-fPIC','-shared','-Wall','-Wextra','-Werror','-Isrc','test/0084.ros-runtime/component_vm_topic_backend.cpp','-o',str(images['/worker/backend.so'])]
        done=subprocess.run(argv,capture_output=True,text=True,timeout=60);assert done.returncode==0,done.stderr;report['backend_compile']=argv;report['backend_sha256']=sha(images['/worker/backend.so'])
        for width,memory in ([(32,0)] if a.first_layout else [(32,0),(64,0),(32,1),(64,1)]):
            for scenario in (['normal','fp','no-export','no-publish','no-configuration'] if (width,memory)==(32,0) else ['normal','fp']):
                g=guest(documents,width,memory,scenario=='fp',scenario.startswith('no-'));assert g.cursor<45000;name=f'component-topics-{width}-{memory}-{scenario}'
                wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True)
                report['guests'].append(dict(name=name,scenario=scenario,sha256=sha(wasm),wat_sha256=sha(wat),assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0)));save()
        report['prepared']=True;save()
    if a.mode=='prepare':return
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';br=json.loads(build.read_text());assert br['complete'] and br['binary_sha256']==sha(runtime) and br['source_root']==str(source)
    assert sha(images['/worker/backend.so'])==report['backend_sha256'] and configuration.read_bytes()==CONFIG
    report.update(runtime=str(runtime),runtime_sha256=sha(runtime),build_sha256=sha(build));options=flags(packs,documents,images,configuration)
    def run(name,args,selected=None):
        verify_gate();start=time.monotonic();argv=[str(runtime),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable',*(options if selected is None else selected),*args]
        with(out.parent/'.communication-transport-ports.lock').open('a') as lock:
            fcntl.flock(lock,fcntl.LOCK_EX);result=subprocess.run(argv,capture_output=True,text=True,timeout=100)
        (out/(name+'.stdout')).write_text(result.stdout);(out/(name+'.stderr')).write_text(result.stderr)
        report['cases'].append(dict(name=name,argv=argv,exit=result.returncode,passed=result.returncode==0,seconds=time.monotonic()-start));save()
        assert result.returncode==0,(name,result.stderr[-6000:]);print('PASS '+name,flush=True)
    run('static',['--ros-scene-check'])
    for g in report['guests']:
        wasm=out/(g['name']+'.wasm');assert sha(wasm)==g['sha256'] and sha(out/(g['name']+'.wat'))==g['wat_sha256']
        selected=options.copy()
        if g['scenario'].startswith('no-'):
            alias,action={'no-export':(comm.alias(1,'subscription'),'data.export'),'no-publish':(comm.alias(1,'publisher'),'topic.publish'),
                'no-configuration':('configuration','artifact.read')}[g['scenario']]
            at=next(i for i in range(len(selected)-6) if selected[i:i+3]==['--ros-scene-grant','robot',alias])
            actions=selected[at+6].split(',');assert action in actions;actions.remove(action);selected[at+6]=','.join(actions)
        for engine in ('int','aot'):run(g['name']+'-'+engine,['--runtime-'+engine,'--run',str(wasm)],selected)
    report['inputs_unchanged']=all(sha(Path(k))==v for k,v in inputs.items());assert report['inputs_unchanged']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()


if __name__=='__main__':main()
