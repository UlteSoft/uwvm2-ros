#!/usr/bin/env python3
"""Actual artifact.publish carrier and configuration security qualification.

Whole-object publication shares the old begin/append/commit mutation stream.
The fixture checks exact retries and receipts, four independent permissions,
finite carrier scratch, compound three-unit budget admission and default denial.
All compiler/VM processes must inherit the existing 64 GiB zero-swap gate.
This driver does not measure performance or claim native-device durability.
"""
import argparse,json,os,subprocess,time
from pathlib import Path
from run_scene_vm import ROOT,GATE,policy,sha,verify_gate
from run_scene_buffers import BufferGuest,u64
OPERATIONS=[dict(name='artifact.'+name,words=words,bytes=72) for name,words in
    [('begin',2),('append',3),('commit',3),('abort',2),('remove',2),('status',1),('receipt',1),('publish',0)]]
OPERATIONS+=[dict(name='artifact.read',words=3,bytes=0)]
ALIASES=['out','peer','zero','scratchzero','scratchsmall','callzero','units2','units3','bytes287','bytes288','noread',
         'missing-begin','missing-append','missing-commit','missing-publish']
PAYLOAD=b'robotics'
def configured():
    flags=policy(OPERATIONS)+['--ros-scene-output-budget','robot',str(32*len(ALIASES)),str(len(ALIASES)),str(112*len(ALIASES)),
        '--ros-scene-publication-budget','robot',str(512*len(ALIASES)),str(len(ALIASES))]
    for alias in ALIASES:
        actions=[op['name'] for op in OPERATIONS if not(alias=='noread' and op['name']=='artifact.read') and
                 not(alias.startswith('missing-') and op['name']=='artifact.'+alias[8:])]
        flags+=['--ros-scene-grant','robot',alias,'cache',alias,'volatile-output',','.join(actions),
            '--ros-scene-output','robot',alias,'32','1','32','16',
            '--ros-scene-publication','robot',alias,'0' if alias=='scratchzero' else '511' if alias=='scratchsmall' else '512','0' if alias=='callzero' else '1']
    flags+=['--ros-scene-limit','robot','zero','max-resources','0']
    for alias,field,limit in [('units2','calls-per-window',2),('units3','calls-per-window',3),('bytes287','bytes-per-window',287),('bytes288','bytes-per-window',288)]:
        flags+=['--ros-scene-limit','robot',alias,'delivery','may-reject','--ros-scene-limit','robot',alias,field,str(limit),
            '--ros-scene-limit','robot',alias,'window-ns','3600000000000']
    return flags
class PublicationGuest(BufferGuest):
    def publish(self,cap,key=1,payload=PAYLOAD,declared=None,**kw):
        self.raw('artifact.publish',cap,u64(key)+u64(len(payload) if declared is None else declared)+payload,capacity=kw.pop('capacity',72),**kw)
    def field(self,n,value):
        if isinstance(value,str):self.check(self.word(512+8*n)+' '+value+' i64.ne')
        else:self.equal(self.word(512+8*n),value)
    def receipt(self,key,status=0,state=2,size=8):
        self.field(0,key);self.field(1,8);self.field(3,status);self.field(4,size if status==0 else 0)
        self.field(5,size if status==0 else 0);self.field(6,size if status==0 else 0);self.field(7,state);self.field(8,int(state==2))
def successful_guest(width,memory):
    g=PublicationGuest(OPERATIONS,width,memory);g.check('call $abi',2);caps={a:g.open(a) for a in ALIASES};cap=caps['out']
    for kw in [dict(memory=memory+1),dict(output=65535),dict(length=512)]:g.publish(cap,expected=22,**kw)
    g.publish(cap,capacity=71,expected=90);g.equal(g.word(1024),72)
    g.publish(cap,key=0,expected=17);g.publish(cap,key=2,expected=17);g.publish(cap,declared=7,expected=22)
    g.publish(cap,payload=PAYLOAD*3,expected=90)
    g.publish(cap);g.receipt(1);obj=g.save('published',528)
    g.publish(cap);g.receipt(1);g.field(2,obj)
    g.publish(cap,payload=b'ROBOTICS',expected=17)
    g.call('artifact.read',cap,[obj,0,8],capacity=8);g.equal(g.word(1024),8);g.exact(512,PAYLOAD)
    g.call('artifact.receipt',cap,[1]);g.receipt(1);g.field(2,obj)
    g.call('artifact.status',cap,[obj]);g.field(0,1);g.field(1,8);g.field(4,0);g.field(7,2)
    g.publish(cap,key=2);g.receipt(2,status=11,state=0,size=0)
    g.call('artifact.read',cap,[obj,0,8],capacity=8);g.exact(512,PAYLOAD)
    g.call('artifact.remove',cap,[3,obj]);g.field(7,4)
    g.publish(cap,key=4,payload=b'');g.receipt(4,size=0);empty=g.save('empty',528)
    g.call('artifact.read',cap,[empty,0,0],capacity=0);g.equal(g.word(1024),0)
    g.call('artifact.remove',cap,[5,empty]);g.field(7,4)
    g.call('artifact.begin',cap,[6,16]);staged=g.save('staged',528)
    payload=int.from_bytes(PAYLOAD,'little');g.call('artifact.append',cap,[7,staged,0,payload,payload],size=40);g.field(5,16)
    g.call('artifact.append',cap,[7,staged,0,payload,payload],size=40);g.field(5,16)
    g.call('artifact.commit',cap,[8,staged,16]);g.field(7,2)
    g.call('artifact.remove',cap,[9,staged]);g.field(7,4)
    g.publish(cap,key=10,payload=PAYLOAD*2);g.receipt(10,size=16);g.publish(cap,key=10,payload=PAYLOAD*2);g.receipt(10,size=16)
    fresh=g.save('fresh',528);g.call('artifact.read',caps['peer'],[fresh,0,8],capacity=8,expected=9)
    g.call('artifact.read',caps['noread'],[fresh,0,8],capacity=8,expected=13)
    for alias in ['scratchzero','scratchsmall','callzero']:
        g.publish(caps[alias],expected=11);g.call('artifact.receipt',caps[alias],[1],expected=17)
        g.call('artifact.begin',caps[alias],[1,0]);g.field(3,0);g.field(0,1)
    for alias in ['missing-begin','missing-append','missing-commit','missing-publish']:g.publish(caps[alias],expected=13)
    g.publish(caps['zero']);g.receipt(1,status=11,state=0,size=0)
    for alias in ['units2','bytes287']:g.publish(caps[alias],expected=11)
    # A rejected three-unit admission debits nothing: old begin+commit still fit two units.
    g.call('artifact.begin',caps['units2'],[1,0]);ue=g.save('unit-empty',528)
    g.call('artifact.commit',caps['units2'],[2,ue,0]);g.field(3,0)
    for alias in ['units3','bytes288']:
        g.publish(caps[alias],capacity=71,expected=90);g.equal(g.word(1024),72)
        g.publish(caps[alias]);g.receipt(1);g.publish(caps[alias],expected=11)
        g.check(caps[alias]+' call $close');again=g.open(alias);g.publish(again,expected=11)
    g.check(cap+' call $close');g.publish(cap,expected=9)
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mode',choices=['prepare','execute','all'],default='all');p.add_argument('--source-root',type=Path,default=ROOT)
    args=p.parse_args();verify_gate();assert os.sched_getaffinity(0).issubset(set(range(8,12)))
    if args.mode!='prepare' and args.runtime is None:p.error('--runtime is required')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    sources=[Path(__file__).resolve(),Path(__file__).with_name('run_scene_vm.py'),Path(__file__).with_name('run_scene_buffers.py')]
    pins=lambda:{str(path):sha(path) for path in sources}
    if args.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['sources']==pins()
    else:
        assert not (out/'results.json').exists();report=dict(scope=__doc__,complete=False,prepared=False,cgroup=str(GATE),cgroup_inode=GATE.stat().st_ino,
            memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),sources=pins(),guests=[],cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    save()
    if args.mode!='execute':
        for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
            for mode in ['success','denied','no-grants','dependencies']:
                if mode=='success':g=successful_guest(width,memory)
                elif mode=='dependencies':
                    g=PublicationGuest(OPERATIONS,width,memory);cap=g.open('out');g.publish(cap,expected=13)
                else:
                    g=PublicationGuest(OPERATIONS,width,memory);g.check('call $abi',13 if mode=='denied' else 2);g.open('out',13)
                    g.publish(1,expected=13 if mode=='denied' else 9)
                name=f'{mode}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                cmd=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                r=subprocess.run(cmd,capture_output=True);assert r.returncode==0,r.stderr.decode()
                report['guests'].append(dict(name=name,mode=mode,assertions=len(g.checks),compile=cmd,wat_sha256=sha(wat),sha256=sha(wasm)));save()
        report['prepared']=True;save()
    if args.mode=='prepare':print('PASS publication Wasm preparation');return
    runtime=args.runtime.resolve();report['runtime']=str(runtime);report['runtime_sha256']=sha(runtime);source=args.source_root.resolve();report['source_root']=str(source)
    report['production_pins']={str(p):sha(p) for p in (source/'src/uwvm2/imported/ros/scene').glob('*.h')}
    def run(name,flags,wasm=None,success=True):
        verify_gate();cmd=[str(runtime),'--wasm-set-main-module-name','main',*flags]
        if wasm:cmd+=['--run',str(wasm)]
        start=time.monotonic();r=subprocess.run(cmd,capture_output=True,timeout=120)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        passed=(r.returncode==0)==success;report['cases'].append(dict(name=name,argv=cmd,exit=r.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,r.stderr.decode(errors='replace')[-5000:]);print('PASS',name,flush=True)
    flags=configured()
    for guest in report['guests']:
        wasm=out/(guest['name']+'.wasm');assert sha(wasm)==guest['sha256'] and sha(out/(guest['name']+'.wat'))==guest['wat_sha256']
        mode=guest['mode'];active=flags if mode in ['success','dependencies'] else policy(OPERATIONS,'deny' if mode=='denied' else 'allow')
        for engine in ['int','aot']:
            if mode=='dependencies':
                for dependency in ['begin','append','commit','publish']:
                    altered=active.copy();i=altered.index('artifact.'+dependency);altered[i+1]='deny'
                    run(guest['name']+'-'+dependency+'-'+engine,['--runtime-'+engine,*altered],wasm)
            else:run(guest['name']+'-'+engine,['--runtime-'+engine,*active],wasm)
    smoke=out/'no-grants-32-0.wasm';run('default-hidden',['--runtime-int'],smoke,False);run('policy-check',[*flags,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    base=['--ros-scene-create','robot','--ros-scene-output-budget','robot','32','1','112','--ros-scene-grant','robot','out','cache','out','volatile-output','artifact.publish,artifact.begin,artifact.append,artifact.commit',
        '--ros-scene-output','robot','out','32','1','32','16']
    budget=['--ros-scene-publication-budget','robot','512','1'];plan=['--ros-scene-publication','robot','out','512','1']
    run('cli-no-publication-scratch',[*base,'--ros-scene-check'])
    run('cli-missing-budget',[*base,*plan,'--ros-scene-check'],success=False)
    run('cli-duplicate-plan',[*base,*budget,*plan,*plan,'--ros-scene-check'],success=False)
    run('cli-duplicate-budget',[*base,*budget,*budget,*plan,'--ros-scene-check'],success=False)
    run('cli-plan-before-output',[*base[:-7],*plan,*base[-7:],*budget,'--ros-scene-check'],success=False)
    for index,value in [(2,'missing'),(3,'unlimited'),(3,str(2**63)),(4,str(2**64-1)),(4,'-1')]:
        invalid=plan.copy();invalid[index]=value;run('cli-plan-'+str(index)+'-'+str(len(value)),[*base,*budget,*invalid,'--ros-scene-check'],success=False)
    for index,value in [(2,'0'),(3,'0'),(2,'unlimited'),(2,str(2**63)),(3,str(2**64-1))]:
        invalid=budget.copy();invalid[index]=value;run('cli-budget-'+str(index)+'-'+str(len(value)),[*base,*invalid,*plan,'--ros-scene-check'],success=False)
    assert sha(runtime)==report['runtime_sha256'] and pins()==report['sources'];assert all(sha(Path(p))==h for p,h in report['production_pins'].items())
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()
if __name__=='__main__':main()
