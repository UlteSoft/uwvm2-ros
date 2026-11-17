#!/usr/bin/env python3
"""Exercise real volatile cache transactions with exact authority and bounded retries.

Every compiler/VM process must inherit the existing 64 GiB zero-swap gate.
The fixtures use no WASI or legacy ROS imports and write no native output files.
"""
import argparse,json,subprocess,time
from pathlib import Path
from run_scene_vm import Guest,ROOT,GATE,policy,sha,verify_gate

OPERATIONS=[dict(name='artifact.'+name,words=words,bytes=72) for name,words in
            [('begin',2),('append',3),('commit',3),('abort',2),('remove',2),('status',1),('receipt',1)]]
OPERATIONS += [dict(name='artifact.read',words=3,bytes=-1)]
ALIASES=['out','other','zero','rate','noread','tiny']

def configured():
    flags=policy(OPERATIONS)
    flags += ['--ros-scene-output-budget','robot',str(32*len(ALIASES)),str(len(ALIASES)),str(112*len(ALIASES))]
    for alias in ALIASES:
        actions=[x['name'] for x in OPERATIONS if not(alias=='noread' and x['name']=='artifact.read')]
        flags += ['--ros-scene-grant','robot',alias,'cache',alias,'volatile-output',','.join(actions),
                  '--ros-scene-output','robot',alias,'32','1','32','0' if alias=='tiny' else '16']
    for alias,key,value in [('zero','max-resources','0'),('rate','delivery','may-reject'),
                            ('rate','calls-per-window','1'),('rate','window-ns','3600000000000')]:
        flags += ['--ros-scene-limit','robot',alias,key,value]
    return flags

def successful_guest(width,memory,guest_class=Guest):
    g=guest_class(OPERATIONS,width,memory);caps={a:g.open(a) for a in ALIASES};out=caps['out']
    def field(index,value):
        if isinstance(value,str):g.check(g.word(512+8*index)+' '+value+' i64.ne')
        else:g.equal(g.word(512+8*index),value)
    def effect(status=0,state=None):
        field(3,status)
        if state is not None:field(7,state)
    g.call('artifact.begin',out,[1,32],memory=memory+1,expected=22)
    g.call('artifact.begin',out,[1,32],size=15,expected=22)
    g.call('artifact.begin',out,[1,32],output=65530,expected=22)
    g.call('artifact.begin',out,[1,32],length=512,expected=22)
    g.call('artifact.begin',out,[1,32],length=128,expected=22)
    g.call('artifact.begin',out,[1,32],capacity=71,expected=90);g.equal(g.word(1024),72)
    g.call('artifact.begin',out,[0,32],expected=17)
    g.call('artifact.begin',out,[1,32]);effect(state=1);field(0,1);field(1,1);field(6,32)
    obj=g.save('object',528)
    g.call('artifact.begin',out,[1,32]);field(2,obj)
    g.call('artifact.begin',out,[1,31],expected=17)
    g.call('artifact.begin',out,[3,32],expected=17)
    g.call('artifact.read',out,[obj,0,8],expected=13,capacity=8)
    g.call('artifact.status',out,[obj]);field(0,1);field(4,0)
    payload=0x4142434445464748
    g.call('artifact.append',out,[2,obj,0,payload,payload,payload],size=48,expected=90);g.equal(g.word(1024),0)
    g.call('artifact.append',out,[2,obj,0,payload],size=32,capacity=71,expected=90);g.equal(g.word(1024),72)
    g.call('artifact.append',out,[2,obj,0,payload],size=32);effect(state=1);field(4,8);field(5,8)
    g.call('artifact.append',out,[2,obj,0,payload],size=32);field(4,8);field(5,8)
    g.call('artifact.append',out,[2,obj,0,payload+1],size=32,expected=17)
    g.call('artifact.append',out,[3,obj,0,payload],size=32);effect(17,1);field(5,8)
    g.call('artifact.receipt',out,[3]);effect(17,1);field(0,3)
    g.call('artifact.status',out,[obj]);field(0,2);field(1,2);field(4,0)
    g.call('artifact.commit',out,[4,obj,9]);effect(17,1)
    g.call('artifact.commit',out,[5,obj,8]);effect(state=2);field(6,32);field(8,1)
    g.call('artifact.commit',out,[5,obj,8]);effect(state=2)
    g.call('artifact.append',out,[6,obj,8,payload],size=32);effect(13,2)
    g.call('artifact.read',out,[obj,0,16],capacity=16);g.equal(g.word(1024),8);g.equal(g.word(512),payload)
    g.call('artifact.read',out,[obj,0,17],capacity=17,expected=90);g.equal(g.word(1024),0)
    g.call('artifact.read',out,[obj,9,0],expected=90,capacity=0)
    g.call('artifact.status',caps['other'],[obj],expected=9)
    g.call('artifact.read',caps['other'],[obj,0,8],expected=9,capacity=8)
    g.call('artifact.receipt',caps['other'],[5],expected=17)
    g.call('artifact.read',caps['noread'],[obj,0,8],expected=13,capacity=8)
    g.call('artifact.remove',out,[7,obj]);effect(state=4);field(5,8);field(6,0);field(8,0)
    g.call('artifact.remove',out,[7,obj]);effect(state=4)
    g.call('artifact.commit',out,[5,obj,8],expected=17)
    g.call('artifact.read',out,[obj,0,8],expected=9,capacity=8)
    g.call('artifact.begin',out,[8,32]);effect(state=1);new=g.save('replacement',528)
    g.check(f'{new} {obj} i64.eq')
    g.call('artifact.abort',out,[9,new]);effect(state=3)
    g.call('artifact.begin',caps['zero'],[1,0]);effect(11,0);field(2,0)
    g.call('artifact.begin',caps['rate'],[1,32],capacity=8,expected=90)
    g.call('artifact.begin',caps['rate'],[1,32]);effect()
    g.call('artifact.begin',caps['rate'],[1,32],expected=11)
    g.call('artifact.append',caps['rate'],[2,0,0,payload],size=32,capacity=71,expected=90);g.equal(g.word(1024),72)
    g.check(f"{caps['rate']} call $close");rate=g.open('rate')
    g.call('artifact.receipt',rate,[1],expected=11)
    g.call('artifact.begin',caps['tiny'],[1,0]);empty=g.save('empty',528)
    g.call('artifact.append',caps['tiny'],[2,empty,0],size=24);effect()
    g.call('artifact.commit',caps['tiny'],[3,empty,0]);effect(state=2)
    g.call('artifact.read',caps['tiny'],[empty,0,0],capacity=0);g.equal(g.word(1024),0)
    g.call('artifact.remove',caps['tiny'],[4,empty]);effect(state=4)
    return g

def main():
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--runtime',type=Path,required=True);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();verify_gate();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    report=dict(complete=False,runtime_sha256=sha(args.runtime),cgroup=str(GATE),memory_max=68719476736,swap_max=0,
                events_before=(GATE/'memory.events').read_text(),cases=[],guests=[])
    sources=[Path(__file__).resolve(),Path(__file__).with_name('run_scene_vm.py').resolve()]
    report['sources']={str(path):sha(path) for path in sources}
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def compile(name,g):
        wat,wasm=out/(name+'.wat'),out/(name+'.wasm');wat.write_text(g.render())
        subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True)
        report['guests'].append(dict(name=name,sha256=sha(wasm),assertions=len(g.checks)));return wasm
    def run(name,flags,wasm=None,success=True):
        verify_gate();cmd=[str(args.runtime.resolve()),'--wasm-set-main-module-name','main',*flags]
        if wasm:cmd+=['--run',str(wasm)]
        start=time.monotonic();r=subprocess.run(cmd,capture_output=True,timeout=40)
        (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
        passed=(r.returncode==0)==success;report['cases'].append(dict(name=name,argv=cmd,exit=r.returncode,passed=passed,seconds=time.monotonic()-start));save()
        assert passed,(name,r.stderr.decode(errors='replace')[-3000:]);print('PASS',name,flush=True)
    flags=configured()
    for width,memory in [(32,0),(64,0),(32,1),(64,1)]:
        for mode in ['success','denied','no-grants']:
            if mode=='success':g=successful_guest(width,memory)
            else:
                g=Guest(OPERATIONS,width,memory)
                for op in OPERATIONS:
                    words=op['words'];g.call(op['name'],1,[1]*words,expected=13 if mode=='denied' else 9)
            wasm=compile(f'{mode}-{width}-{memory}',g)
            configured_flags=flags if mode=='success' else policy(OPERATIONS,'deny' if mode=='denied' else 'allow')
            for engine in ['int','aot']:run(f'{mode}-{width}-{memory}-{engine}',['--runtime-'+engine,*configured_flags],wasm)
    smoke=compile('smoke',Guest(OPERATIONS))
    run('default-hidden',[],smoke,False)
    run('policy-check',[*flags,'--ros-scene-check'])
    for mode in ['validation','section-details']:run('static-'+mode,['--mode',mode,*flags],smoke)
    for field,value in [(2,'0'),(3,'0'),(4,'0'),(4,'655'),(2,'unlimited')]:
        bad=flags.copy();bad[bad.index('--ros-scene-output-budget')+field]=value
        run(f'budget-{field}-{value}',[*bad,'--ros-scene-check'],success=False)
    for field,value in [(2,'absent'),(3,'unlimited'),(4,'18446744073709551615'),(5,'33'),(6,str(4*1024*1024+1))]:
        bad=flags.copy();bad[bad.index('--ros-scene-output')+field]=value
        run(f'plan-{field}-{value}',[*bad,'--ros-scene-check'],success=False)
    for purpose in ['map','configuration','log','recording']:
        bad=flags.copy();bad[bad.index('--ros-scene-grant')+3]=purpose
        run('unsupported-purpose-'+purpose,[*bad,'--ros-scene-check'],success=False)
    for action in ['artifact.open','artifact.describe','resource.close','resource.observe']:
        bad=flags.copy();bad[bad.index('--ros-scene-grant')+6]=action
        run('unsupported-action-'+action,[*bad,'--ros-scene-check'],success=False)
    assert sha(args.runtime)==report['runtime_sha256']
    report['sources_unchanged']=all(sha(Path(path))==digest for path,digest in report['sources'].items())
    assert report['sources_unchanged'];report['events_after']=(GATE/'memory.events').read_text();report['complete']=True;save()

if __name__=='__main__':main()
