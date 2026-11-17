#!/usr/bin/env python3
"""Qualify local managed lifecycle imports through the actual VM and CLI.

The generated memory32/64 guests import only uwvm_ros_v2. Absolute deadlines
come from clock.steady, never WASI or wall time. Every compiler and VM process
inherits the existing 64 GiB zero-swap cgroup. This checks a logical state
machine, not native ROS nodes, callback code execution or physical safe-stop.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import time
from run_scene_vm import Guest, ROOT, GATE, policy, sha, verify_gate

CONTRACT=ROOT/'src/uwvm2/imported/ros/scene/managed_lifecycle_contract.h'
LIFECYCLE=[dict(name=n,words=int(w),bytes=int(b)) for n,w,b in
    re.findall(r' X\(\w+,\w+,"([^"]+)",(\d+),(\d+)\)',CONTRACT.read_text())]
assert len(LIFECYCLE)==6
OPERATIONS=LIFECYCLE+[dict(name='clock.create',words=1,bytes=8),dict(name='clock.read',words=1,bytes=32),
    dict(name='resource.close',words=1,bytes=0)]
ALIASES=['node','peer','roleszero','capacityzero','resourcezero','clientonly','masked','zerodeadline','error','expire','expireerror','activeclose']

def configured():
    flags=policy(OPERATIONS)
    for alias in ALIASES:
        actions=[op['name'] for op in LIFECYCLE]+['resource.close']
        flags+=['--ros-scene-grant','robot',alias,'lifecycle',alias,'local-lifecycle',','.join(actions),
            '--ros-scene-lifecycle','robot',alias,'0' if alias=='capacityzero' else '2',
            '0' if alias=='zerodeadline' else '60000000000','0' if alias=='masked' else '254',
            '0' if alias=='roleszero' else '1' if alias=='clientonly' else '3']
    flags+=['--ros-scene-limit','robot','resourcezero','max-resources','0',
        '--ros-scene-grant','robot','steady','clock.steady','steady','local','clock.create,clock.read,resource.close']
    return flags

def successful_guest(width,memory):
    g=Guest(OPERATIONS,width,memory)
    def field(index,value,base=512):
        if isinstance(value,str):g.check(g.word(base+8*index)+' '+value+' i64.ne')
        else:g.equal(g.word(base+8*index),value)
    def snapshot(cap,handle,state,generation,status=0,request=0,delivered=0):
        g.call('lifecycle.state',cap,[handle]);field(0,state);field(1,generation);field(2,request);field(5,status);field(6,delivered)
    g.check('call $abi',2);g.open('absent',13)
    caps={alias:g.open(alias) for alias in ALIASES};steady=g.open('steady')
    g.call('clock.create',steady,[0]);clock=g.save('clock')
    def deadline(name='deadline',delta=10000000000):
        g.call('clock.read',steady,[clock],output=768,length=1104)
        g.globals.add(name);g.body.append(g.word(768)+f' i64.const {delta} i64.add global.set ${name}')
        return 'global.get $'+name
    def wait_until(limit):
        g.body.append('block $deadline_done loop $deadline_wait')
        g.call('clock.read',steady,[clock],output=768,length=1104)
        g.body.append(g.word(768)+' '+limit+' i64.lt_u br_if $deadline_wait end end')
    def handles(alias):
        cap=caps[alias];g.call('lifecycle.open',cap,[1]);client=g.save(alias+'-client')
        g.call('lifecycle.open',cap,[2]);server=g.save(alias+'-server');return client,server
    def receipt(key,request,generation,state):
        for n,value in enumerate([key,request,generation,state,0]):field(n,value)
    def transition(alias,client,server,key,transition,generation,target,phase):
        cap=caps[alias];limit=deadline(alias+'-deadline')
        g.call('lifecycle.submit',cap,[client,key,transition,generation,limit]);request=g.save(alias+'-request',520)
        receipt(key,request,generation+1,phase)
        g.call('lifecycle.take',cap,[server]);field(0,request);field(1,transition);field(2,generation+1);field(3,phase);field(4,limit)
        g.call('lifecycle.respond',cap,[server,request,generation+1,97]);receipt(key,request,generation+2,target)
        snapshot(cap,client,target,generation+2)
        return request
    cap=caps['node']
    for role in [0,3,4]:g.call('lifecycle.open',cap,[role],expected=22)
    g.call('lifecycle.open',caps['roleszero'],[1],expected=13)
    g.call('lifecycle.open',caps['capacityzero'],[1],expected=11)
    g.call('lifecycle.open',caps['resourcezero'],[1],expected=11)
    g.call('lifecycle.open',caps['clientonly'],[2],expected=13)
    for kw in [dict(memory=memory+1),dict(size=7),dict(output=65535),dict(output=2**64-1),dict(length=512),dict(length=128)]:
        g.call('lifecycle.open',cap,[1],expected=22,**kw)
    for capacity in [0,7]:
        g.call('lifecycle.open',cap,[1],capacity=capacity,expected=90);g.equal(g.word(1024),8)
    client,server=handles('node');other,_=handles('peer')
    g.call('lifecycle.open',cap,[1],expected=11)
    snapshot(cap,client,1,1)
    g.call('lifecycle.state',caps['peer'],[client],expected=9)
    g.call('lifecycle.state',cap,[other],expected=9)
    g.call('resource.close',caps['peer'],[client],expected=9)
    g.call('lifecycle.state',cap,[client],capacity=55,expected=90);g.equal(g.word(1024),56)
    g.call('lifecycle.transitions',cap,[client],capacity=7,expected=90);g.equal(g.word(1024),8)
    g.call('lifecycle.transitions',cap,[client]);field(0,34)
    g.call('lifecycle.take',cap,[client],expected=9)
    g.call('lifecycle.respond',cap,[client,1,1,97],expected=9)
    limit=deadline()
    for key,action,generation,due,result in [(0,1,1,limit,17),(2,1,1,limit,17),(1,0,1,limit,22),
        (1,8,1,limit,22),(1,3,1,limit,17),(1,1,2,limit,17),(1,1,1,0,22),
        (1,1,1,1,22),(1,1,1,2**64-1,22)]:
        g.call('lifecycle.submit',cap,[client,key,action,generation,due],expected=result)
    g.call('lifecycle.submit',cap,[server,1,1,1,limit],expected=9)
    g.call('lifecycle.submit',cap,[client,1,1,1,limit],capacity=39,expected=90);g.equal(g.word(1024),40)
    snapshot(cap,client,1,1)
    g.call('lifecycle.submit',cap,[client,1,1,1,limit]);request=g.save('first-request',520);receipt(1,request,2,10)
    g.call('lifecycle.submit',cap,[client,1,1,1,limit]);receipt(1,request,2,10)
    g.call('lifecycle.submit',cap,[client,1,1,1,limit+' i64.const 1 i64.sub'],expected=17)
    g.call('lifecycle.submit',cap,[client,2,1,2,limit],expected=11)
    g.call('lifecycle.respond',cap,[server,request,2,97],expected=17)
    snapshot(cap,client,10,2,request=request)
    g.call('lifecycle.take',cap,[server],capacity=39,expected=90);g.equal(g.word(1024),40)
    snapshot(cap,client,10,2,request=request)
    g.call('lifecycle.take',cap,[server]);field(0,request);field(1,1);field(2,2);field(3,10);field(4,limit)
    g.call('lifecycle.take',cap,[server],expected=11)
    g.call('lifecycle.respond',cap,[server,request,1,97],expected=17)
    g.call('lifecycle.respond',cap,[server,request,2,96],expected=22)
    g.call('lifecycle.respond',cap,[server,request,2,97],capacity=39,expected=90)
    snapshot(cap,client,10,2,request=request,delivered=1)
    g.call('lifecycle.respond',cap,[server,request,2,97]);receipt(1,request,3,2)
    g.call('lifecycle.respond',cap,[server,request,2,97]);receipt(1,request,3,2)
    g.call('lifecycle.respond',cap,[server,request,2,98],expected=17)
    g.call('lifecycle.submit',cap,[client,1,1,1,limit]);receipt(1,request,2,10)
    snapshot(cap,client,2,3)
    g.call('lifecycle.transitions',cap,[client]);field(0,76)
    g.call('resource.close',cap,[client]);g.call('resource.close',cap,[client],expected=9)
    g.call('lifecycle.open',cap,[1]);replacement=g.save('replacement-client')
    g.check(replacement+' '+client+' i64.eq');client=replacement;snapshot(cap,client,2,3)
    transition('node',client,server,2,3,3,3,13)
    g.call('lifecycle.respond',cap,[server,request,2,97],expected=17)
    g.call('lifecycle.transitions',cap,[client]);field(0,144)
    transition('node',client,server,3,4,5,2,14)
    transition('node',client,server,4,2,7,1,11)
    transition('node',client,server,5,5,9,4,12)
    g.call('lifecycle.transitions',cap,[client]);field(0,0)
    snapshot(caps['peer'],other,1,1)
    for alias in ['masked','zerodeadline']:
        g.call('lifecycle.open',caps[alias],[1]);handle=g.save(alias+'-client');due=deadline(alias+'-deadline')
        g.call('lifecycle.submit',caps[alias],[handle,1,1,1,due],expected=13 if alias=='masked' else 22)
        snapshot(caps[alias],handle,1,1)
    ec,es=handles('error');ecap=caps['error'];due=deadline('error-deadline')
    g.call('lifecycle.submit',ecap,[ec,1,1,1,due]);er=g.save('error-request',520)
    g.call('lifecycle.take',ecap,[es]);g.call('lifecycle.respond',ecap,[es,er,2,98]);receipt(1,er,3,1)
    due=deadline('error-deadline');g.call('lifecycle.submit',ecap,[ec,2,1,3,due]);er=g.save('error-request',520)
    g.call('lifecycle.take',ecap,[es]);g.call('lifecycle.respond',ecap,[es,er,4,99]);receipt(2,er,5,15)
    g.call('lifecycle.respond',ecap,[es,er,4,99]);receipt(2,er,5,15)
    g.call('lifecycle.respond',ecap,[es,er,5,97],expected=17)
    g.call('lifecycle.take',ecap,[es]);field(0,er);field(1,1);field(2,5);field(3,15);field(4,due)
    g.call('lifecycle.respond',ecap,[es,er,4,97],expected=17)
    g.call('lifecycle.respond',ecap,[es,er,5,97]);receipt(2,er,6,1);snapshot(ecap,ec,1,6)
    transition('error',ec,es,3,1,6,2,10)
    transition('error',ec,es,4,6,8,4,12)
    ac,asrv=handles('activeclose');transition('activeclose',ac,asrv,1,1,1,2,10)
    transition('activeclose',ac,asrv,2,3,3,3,13);transition('activeclose',ac,asrv,3,7,5,4,12)
    for alias,error_phase in [('expire',False),('expireerror',True)]:
        tc,ts=handles(alias);tcap=caps[alias];due=deadline(alias+'-deadline',50000000)
        g.call('lifecycle.submit',tcap,[tc,1,1,1,due]);tr=g.save(alias+'-request',520)
        g.call('lifecycle.take',tcap,[ts])
        if error_phase:g.call('lifecycle.respond',tcap,[ts,tr,2,99]);receipt(1,tr,3,15)
        wait_until(due)
        if error_phase:g.call('lifecycle.take',tcap,[ts],expected=110)
        else:g.call('lifecycle.respond',tcap,[ts,tr,2,97],expected=110)
        snapshot(tcap,tc,4,4 if error_phase else 3,status=110)
        g.call('lifecycle.respond',tcap,[ts,tr,3 if error_phase else 2,97],expected=17)
        g.call('lifecycle.transitions',tcap,[tc]);field(0,0)
    for handle in [client,server]:g.call('resource.close',cap,[handle])
    g.call('lifecycle.state',cap,[client],expected=9)
    g.check(cap+' call $close');g.call('lifecycle.open',cap,[1],expected=9)
    return g

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mode',choices=['prepare','execute','all'],default='all')
    parser.add_argument('--runtime',type=Path);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset(set(range(8,16)))
    if args.mode!='prepare' and args.runtime is None:parser.error('--runtime is required for execution')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    if args.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['preparation_complete']
    else:
        assert not (out/'results.json').exists(),'Use a fresh preparation directory'
        report=dict(complete=False,preparation_complete=False,scope=__doc__,cgroup=str(GATE),memory_max=68719476736,swap_max=0,
            events_before=(GATE/'memory.events').read_text(),sources={str(p):sha(p) for p in [Path(__file__).resolve(),Path(__file__).with_name('run_scene_vm.py').resolve(),CONTRACT]},
            cases=[],guests=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def compile(name,g,mode,width,memory):
        wat,wasm=out/(name+'.wat'),out/(name+'.wasm');wat.write_text(g.render())
        cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
        r=subprocess.run(cmd,capture_output=True);assert r.returncode==0,r.stderr.decode()
        report['guests'].append(dict(name=name,mode=mode,width=width,memory=memory,wat_sha256=sha(wat),sha256=sha(wasm),assertions=len(g.checks),compile=cmd));save()
    if args.mode!='execute':
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for mode in ['success','denied','no-grants']:
                if mode=='success':g=successful_guest(width,memory)
                else:
                    g=Guest(OPERATIONS,width,memory);g.check('call $abi',13 if mode=='denied' else 2);g.open('node',13)
                    for op in LIFECYCLE:g.call(op['name'],1,[1]*op['words'],expected=13 if mode=='denied' else 9)
                compile(f'{mode}-{width}-{memory}',g,mode,width,memory)
        compile('smoke',Guest(OPERATIONS),'smoke',32,0)
        report['preparation_complete']=True;save()
    for name,digest in report['sources'].items():assert sha(Path(name))==digest,name
    for guest in report['guests']:
        assert sha(out/(guest['name']+'.wasm'))==guest['sha256'] and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
    if args.mode=='prepare':return
    runtime=args.runtime.resolve();report['runtime_sha256']=sha(runtime);report['runtime']=str(runtime)
    def run(name,flags,wasm=None,success=True):
        verify_gate();cmd=[str(runtime),'--wasm-set-main-module-name','main',*flags]
        if wasm:cmd+=['--run',str(wasm)]
        start=time.monotonic();r=subprocess.run(cmd,capture_output=True,timeout=120)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        passed=(r.returncode==0)==success
        report['cases'].append(dict(name=name,argv=cmd,exit=r.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,r.stderr.decode(errors='replace')[-5000:]);print('PASS',name,flush=True)
    flags=configured()
    for guest in report['guests']:
        if guest['mode']=='smoke':continue
        active=flags if guest['mode']=='success' else policy(OPERATIONS,'deny' if guest['mode']=='denied' else 'allow')
        for engine in ['int','aot']:run(guest['name']+'-'+engine,['--runtime-'+engine,*active],out/(guest['name']+'.wasm'))
    smoke=out/'smoke.wasm';run('default-hidden',['--runtime-int'],smoke,False)
    run('policy-check',[*flags,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    base=['--ros-scene-create','robot','--ros-scene-grant','robot','node','lifecycle','node','local-lifecycle','lifecycle.open']
    plan=['--ros-scene-lifecycle','robot','node','2','60000000000','254','3']
    run('cli-missing-plan',[*base,'--ros-scene-check'],success=False)
    run('cli-duplicate-plan',[*base,*plan,*plan,'--ros-scene-check'],success=False)
    run('cli-plan-before-grant',['--ros-scene-create','robot',*plan,'--ros-scene-check'],success=False)
    for index,value in [(2,'absent'),(3,'unlimited'),(3,str(2**64-1)),(3,'-1'),(4,str(2**63)),(4,'unlimited'),(5,'1'),(5,'256'),(6,'4'),(6,str(2**64-1))]:
        invalid=plan.copy();invalid[index]=value
        run('cli-invalid-'+str(index)+'-'+value,[*base,*invalid,'--ros-scene-check'],success=False)
    for index,value in [(5,'guard'),(7,'local'),(8,'clock.read')]:
        invalid=base.copy();invalid[index]=value
        run('cli-invalid-grant-'+str(index),[*invalid,*plan,'--ros-scene-check'],success=False)
    assert sha(runtime)==report['runtime_sha256']
    for name,digest in report['sources'].items():assert sha(Path(name))==digest,name
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()

if __name__=='__main__':main()
