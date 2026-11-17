#!/usr/bin/env python3
"""Actual VM bounded type-description carriers, default denial and inert inspection.

Reuse pinned native images only as inert deployment inputs. The guest receives
type.describe and capability management, with no context/node/endpoint operation
exposure. Process tracing verifies that no worker executable is started. The
reused backend is not qualified for the new schema; no native adapter claim is
made. All preparation and execution remain inside the existing resource gate.
"""
import argparse
import copy
import json
import os
from pathlib import Path
import resource
import subprocess
import sys
import time
from run_scene_vm import ROOT,GATE,policy,sha,verify_gate
from run_scene_recording import RecordingGuest,u64,blob,text
from run_scene_communication import configured,alias
sys.path.insert(0,str(ROOT/'tools/ros_scene'))
import make_communication_definition as definition
import make_communication_worker_definition as worker
from bounded_message_definition import compile_schema

OPS=[dict(name='type.describe',words=0,bytes=0)]

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--runtime',type=Path)
    parser.add_argument('--pair-receipt',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--types-per-worker',type=int,default=1)
    parser.add_argument('--mode',choices=['prepare','execute','all'],default='all');args=parser.parse_args();verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    assert 1<=args.types_per_worker<=512
    out.mkdir(parents=True,exist_ok=True);pair=args.pair_receipt.resolve();evidence=json.loads(pair.read_text());assert evidence['complete']
    inputs={str(pair):sha(pair)}
    modules=['run_scene_vm.py','run_scene_recording.py','run_scene_buffers.py','run_scene_fp.py','run_scene_communication.py','run_scene_bounded_types.py']
    for name in modules:inputs[str(Path(__file__).with_name(name).resolve())]=sha(Path(__file__).with_name(name))
    for tool in [definition,worker,sys.modules['bounded_message_definition']]:inputs[str(Path(tool.__file__).resolve())]=sha(Path(tool.__file__))
    packs=[];documents=[];descriptors=[]
    for side in (0,1):
        path=pair.parent/f'pack-{side}/manifest.json';assert sha(path)==evidence['pack_sha256'][str(path)];inputs[str(path)]=sha(path)
        pack=json.loads(path.read_text())
        for image in [pack['launcher'],*pack['files']]:
            assert Path(image['path']).stat().st_size==image['bytes'] and sha(image['path'])==image['sha256'];inputs[image['path']]=image['sha256']
        original=pair.parent/f'peer-{side}.json';inputs[str(original)]=sha(original);document=json.loads(original.read_text())
        document['definitions']=[row for row in document['definitions']if row['family']in ('context','node')]
        schema=dict(layout='cdr'if side==0 else 'ros1',message_bytes=4096,work=1024,depth=4,nodes=[
            dict(kind='float64',bound=0,flags=['finite'],members=[]),
            dict(kind='record',bound=0,flags=[],members=[dict(name=axis,type=0)for axis in ('x','y','z')]),
            dict(kind='sequence',bound=16,flags=[],members=[dict(name='element',type=1)]),
            dict(kind='record',bound=0,flags=[],members=[dict(name='positions',type=2)])])
        descriptors.append(compile_schema(schema))
        for index in range(args.types_per_worker):
            suffix='.'+str(index)if index else'';type_suffix=str(index)if index else''
            document['definitions'].append(dict(family='type',provider='canonical-schema',identity='bounded.message'+suffix,
                limits={'definition.bytes':4096,'descriptor.bytes':4096},definition={'codec':'bounded-message','type':'qualification_msgs/msg/Positions'+type_suffix,'schema':schema}))
        document['capacity'].update(types=args.types_per_worker,topics=0,services=0,actions=0,graphs=0,queue_bytes=0,definition_bytes=16*1024*1024)
        raw,_=worker.compile_definition(document);profile=out/f'worker-{side}.bin'
        if args.mode=='execute':assert profile.read_bytes()==raw
        else:profile.write_bytes(raw)
        startup=next(row for row in pack['files']if row['destination']=='/worker/profile');previous=startup['bytes']
        startup.update(path=str(profile),bytes=len(raw),sha256=sha(profile));pack['required_image_bytes']+=len(raw)-previous
        packs.append(pack);documents.append(document)
    all_flags=configured(packs,documents)
    for field,value in [('types',2*args.types_per_worker),('bindings',2*(args.types_per_worker+2))]:
        at=next(i for i in range(len(all_flags)-3)if all_flags[i:i+3]==['--ros-scene-communication-budget','robot',field]);all_flags[at+3]=str(value)
    # Every reservation is explicit, including families absent from this inert
    # deployment. Endpoint storage must be zero when no endpoint is selected.
    # Graph/event storage is already explicitly zero in the common fixture.
    for side in (0,1):
        name=f'worker.{side}'
        for field in ('topic-carrier-bytes','topic-bytes','service-carrier-bytes','service-bytes'):
            at=next(i for i in range(len(all_flags)-4)if all_flags[i:i+4]==['--ros-scene-communication-worker-limit','robot',name,field])
            all_flags[at+4]='0'
    # Keep the required deployment bindings but expose only inert type lookup.
    flags=[];index=0
    while index<len(all_flags):
        if all_flags[index]=='--ros-scene-interface':
            row=all_flags[index:index+4]
            if row[2]in ('abi','capability.open','capability.close','type.describe'):flags+=row
            index+=4
        else:flags.append(all_flags[index]);index+=1
    if args.mode=='execute':
        report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['inputs']==inputs and report['types_per_worker']==args.types_per_worker
    else:
        assert not(out/'results.json').exists()
        report=dict(complete=False,prepared=False,scope=__doc__,inputs=inputs,cases=[],guests=[],types_per_worker=args.types_per_worker,
                    cgroup=str(GATE),memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,argv,success=True):
        verify_gate();start=time.monotonic();done=subprocess.run(list(map(str,argv)),capture_output=True,text=True,timeout=90)
        (out/(name+'.stdout')).write_text(done.stdout);(out/(name+'.stderr')).write_text(done.stderr)
        passed=(done.returncode==0) if success else done.returncode>0
        report['cases'].append(dict(name=name,argv=list(map(str,argv)),exit=done.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,done.stderr[-7000:]);print('PASS',name,flush=True)
    if args.mode!='execute':
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            guest=RecordingGuest(OPS,width,memory);guest.check('call $abi',2);guest.open('absent',13)
            for side,index in [(side,index)for side in (0,1)for index in sorted({0,args.types_per_worker-1})]:
                suffix='.'+str(index)if index else'';identity='qualification_msgs/msg/Positions'+(str(index)if index else'')
                cap=guest.open(alias(side,'bounded.message'+suffix));request=text(identity)+u64(0)+u64(4096)
                prefix=text('uwvm/type/Descriptor')+text('bytes')+blob(descriptors[side]);extent=len(prefix)+24
                options=dict(output=32768,length=40000,capacity=4096)
                guest.raw('type.describe',cap,text('other_msgs/msg/Positions')+u64(0)+u64(4096),expected=13,**options)
                guest.raw('type.describe',cap,text(identity)+u64(1)+u64(1)+u64(4096),expected=17,**options)
                guest.raw('type.describe',cap,request,expected=22,**{**options,'memory':memory+1})
                guest.raw('type.describe',cap,request,expected=22,**{**options,'length':32768})
                guest.raw('type.describe',cap,request,expected=90,**{**options,'capacity':extent-1});guest.equal(guest.word(40000),extent)
                guest.raw('type.describe',cap,request,**options);guest.equal(guest.word(40000),extent);guest.exact(32768,prefix)
                guest.check(guest.word(32768+len(prefix))+' i64.eqz');guest.exact(32768+len(prefix)+8,bytes(16))
                guest.check(cap+' call $close');guest.raw('type.describe',cap,request,expected=9,**options)
            name=f'types-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(guest.render())
            run('compile-'+name,['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',wat,'-o',wasm])
            report['guests'].append(dict(name=name,wasm=str(wasm),sha256=sha(wasm),assertions=len(guest.checks)))
        report['prepared']=True;save()
    if args.mode=='prepare':return
    assert args.runtime;runtime=args.runtime.resolve();build=runtime.parent/'full-build-results.json';receipt=json.loads(build.read_text());assert receipt['complete'] and receipt['binary_sha256']==sha(runtime)
    report['runtime_sha256']=sha(runtime);report['build_sha256']=sha(build)
    common=[runtime,'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable']
    run('configuration-check',[*common,*flags,'--ros-scene-check'])
    insufficient=list(flags)
    at=next(i for i in range(len(insufficient)-3)if insufficient[i:i+3]==['--ros-scene-communication-budget','robot','definition-bytes'])
    insufficient[at+3]=str(sum((out/f'worker-{side}.bin').stat().st_size for side in (0,1)))
    run('definition-budget-excludes-compiled-storage',[*common,*insufficient,'--ros-scene-check'],False)
    for row in report['guests']:
        assert sha(row['wasm'])==row['sha256']
        for engine in ['--runtime-int','--runtime-aot']:
            name=row['name']+'-'+engine[10:];trace=out/(name+'.execve')
            run(name,['strace','-f','-qq','-e','trace=execve','-o',trace,*common,engine,*flags,'--run',row['wasm']])
            calls=[line for line in trace.read_text().splitlines()if 'execve('in line]
            assert len(calls)==1 and str(runtime)in calls[0],calls
    report['inputs_unchanged']=all(sha(path)==digest for path,digest in report['inputs'].items())
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=report['inputs_unchanged'];save();assert report['complete']

if __name__=='__main__':main()
