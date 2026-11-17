#!/usr/bin/env python3
"""Qualify actual VM self-revocation, zero-rate authority reduction and checked carriers.

Both engines execute memory32/64 and memory indices zero/one. Dynamic FP
arithmetic follows every import, and independent software MTE guests exercise
each pointer boundary. A receipt proves local notification, not physical stop.
All preparation/execution requires the unchanged existing 64 GiB zero-swap gate.
"""
import argparse,json,os,resource,signal,subprocess,time
from pathlib import Path
from run_scene_vm import Guest,ROOT,GATE,policy,grant,sha,verify_gate
from run_scene_fp import FPGuest
from run_scene_memtag import TaggedGuest,add_private_instructions
OPERATIONS=[dict(name='capability.revoke',words=2,bytes=32),dict(name='guard.create',words=0,bytes=8),
            dict(name='guard.trigger',words=1,bytes=0),dict(name='guard.take',words=1,bytes=8)]
def configured():
    flags=policy(OPERATIONS)
    for alias in ('signal','other','zero','forbidden'):
        actions=[x['name'] for x in OPERATIONS if alias!='forbidden' or x['name']!='capability.revoke']
        flags+=grant(alias,'guard',actions)
    for key,value in [('delivery','may-reject'),('calls-per-window','0'),('bytes-per-window','0')]:
        flags+=['--ros-scene-limit','robot','zero',key,value]
    return flags
def guest(width,memory,scenario):
    if scenario.startswith('mte-'):
        g=TaggedGuest(OPERATIONS,width,memory)
        g.body.append(f'i{width}.const {g.pointer(0)} i{width}.const 65536 call $tag.store')
        g.check('call $tag.status'+(' i32.wrap_i64' if width==64 else ''),2)
        cap=g.open('signal');g.call('guard.create',cap);handle=g.save('resource')
        if scenario!='mte-valid':
            g.call('capability.revoke',cap,[1,cap],expected=22,**{scenario[4:]+'_tag':5});return g
        g.call('capability.revoke',cap,[1,cap]);g.equal(g.word(536),2)
        g.call('capability.revoke',cap,[1,cap]);g.call('guard.trigger',cap,[handle],expected=13);return g
    g=FPGuest(OPERATIONS,width,memory)
    if scenario in ('deny','absent'):
        g.check('call $abi',13 if scenario=='deny' else 2)
        g.call('capability.revoke',1,[1,1],expected=13 if scenario=='deny' else 9);return g
    g.check('call $abi',2);cap=g.open('signal');other=g.open('other');zero=g.open('zero');forbidden=g.open('forbidden')
    # Preserve the first handle when opening another handle of the same alias.
    first=g.save('first',512)  # overwritten below by the explicitly retained primary
    g.body.append(cap+' global.set $first');sibling=g.open('signal');cap='global.get $first'
    g.call('guard.create',cap);handle=g.save('resource');g.call('guard.trigger',cap,[handle])
    for kwargs in (dict(memory=memory+1),dict(size=8),dict(size=24),dict(output=65535),dict(length=512),dict(length=128),dict(output=2**64-1)):
        g.call('capability.revoke',cap,[1,cap],expected=22,**kwargs)
    g.call('capability.revoke',cap,[0,cap],expected=22)
    g.call('capability.revoke',cap,[1,other],expected=13)
    g.call('capability.revoke',forbidden,[1,forbidden],expected=13)
    g.call('capability.revoke',cap,[1,cap],capacity=31,expected=90);g.equal(g.word(1024),32)
    g.call('guard.take',cap,[handle]);g.equal(g.word(512),1)
    g.call('guard.create',zero,expected=11)
    g.call('capability.revoke',zero,[1,zero]);g.equal(g.word(536),2)
    g.call('capability.revoke',zero,[1,zero]);g.call('guard.create',zero,expected=13)
    g.call('capability.revoke',cap,[9,cap]);identity=g.save('receipt',528)
    g.equal(g.word(512),9);g.check(g.word(520)+' '+cap+' i64.ne');g.equal(g.word(536),2)
    g.call('capability.revoke',cap,[9,cap]);g.check(g.word(528)+' '+identity+' i64.ne')
    g.call('capability.revoke',cap,[10,cap],expected=17)
    g.call('capability.revoke',sibling,[9,sibling],expected=13)
    g.call('guard.trigger',sibling,[handle],expected=13);g.open('signal',13)
    g.call('guard.create',other);g.check(cap+' call $close');g.call('capability.revoke',cap,[9,cap],expected=9)
    return g
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--mode',choices=('prepare','execute','all'),default='all')
    p.add_argument('--runtime',type=Path);p.add_argument('--source-root',type=Path,default=ROOT);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();verify_gate();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    if a.mode!='prepare' and a.runtime is None:p.error('--runtime is required')
    out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    source=a.source_root.resolve();inputs=[Path(__file__).resolve(),*[Path(__file__).with_name(x).resolve() for x in ('run_scene_vm.py','run_scene_fp.py','run_scene_memtag.py')],
        *[source/x for x in ('src/uwvm2/imported/ros/scene/revocation_contract.h','src/uwvm2/imported/ros/scene/authority.h','src/uwvm2/imported/ros/scene/broker.h','src/uwvm2/imported/ros/scene/deployment.h','src/uwvm2/uwvm/imported/scene/host.h')]]
    pins=lambda:{str(x):sha(x) for x in inputs}
    if a.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['inputs']==pins()
    else:
        assert not (out/'results.json').exists()
        report=dict(complete=False,prepared=False,scope=__doc__,inputs=pins(),source_root=str(source),cgroup=str(GATE),inode=GATE.stat().st_ino,
            boot_id=Path('/proc/sys/kernel/random/boot_id').read_text().strip(),affinity=sorted(os.sched_getaffinity(0)),memory_max=68719476736,swap_max=0,
            events_before=(GATE/'memory.events').read_text(),guests=[],cases=[])
    def save():report['events_after']=(GATE/'memory.events').read_text();(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    try:
        if a.mode!='execute':
            for width,memory in ((32,0),(64,0),(32,1),(64,1)):
                for scenario in ('success','deny','absent','mte-valid','mte-request','mte-output','mte-length'):
                    g=guest(width,memory,scenario);name=f'{scenario}-{width}-{memory}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                    argv=['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)]
                    r=subprocess.run(argv,capture_output=True,timeout=30);assert not r.returncode,r.stderr.decode()
                    if scenario.startswith('mte-'):wasm.write_bytes(add_private_instructions(wasm.read_bytes(),len(g.checks),memory))
                    report['guests'].append(dict(name=name,scenario=scenario,width=width,memory=memory,compile=argv,sha256=sha(wasm),wat_sha256=sha(wat),assertions=len(g.checks),fp_sites=getattr(g,'fp_sites',0)));save()
            report['prepared']=True;save()
        if a.mode=='prepare':return
        vm=a.runtime.resolve();report['runtime']=str(vm);report['runtime_sha256']=sha(vm)
        build=vm.parent/'full-build-results.json';evidence=json.loads(build.read_text())
        assert evidence['complete'] and evidence['full_source_unchanged'] and evidence['binary_sha256']==sha(vm) and Path(evidence['source_root'])==source
        report['build_receipt']=str(build);report['build_receipt_sha256']=sha(build)
        def run(name,flags,wasm=None,expected=0,diagnostics=()):
            verify_gate();argv=[str(vm),'--wasm-set-main-module-name','main','--log-color','disable',*flags]
            if wasm:argv+=['--run',str(wasm)]
            start=time.monotonic();r=subprocess.run(argv,capture_output=True,timeout=120)
            (out/(name+'.stdout')).write_bytes(r.stdout);(out/(name+'.stderr')).write_bytes(r.stderr)
            passed=(r.returncode==expected if expected is not None else r.returncode!=0) and all(x in r.stderr.decode(errors='replace') for x in diagnostics)
            report['cases'].append(dict(name=name,argv=argv,exit=r.returncode,expected=expected,passed=passed,seconds=time.monotonic()-start));save()
            assert passed,(name,r.stderr.decode(errors='replace')[-5000:]);print('PASS '+name,flush=True)
        for entry in report['guests']:
            wasm=out/(entry['name']+'.wasm');assert sha(wasm)==entry['sha256']
            scenario=entry['scenario'];flags=policy(OPERATIONS,'deny' if scenario=='deny' else 'allow') if scenario in ('deny','absent') else configured()
            for engine in ('int','aot'):
                selected=['--runtime-'+engine,'--runtime-compile-threads','1',*flags]
                if engine=='aot':selected+=['--runtime-llvm-jit-policy','max','--runtime-llvm-jit-cache-path','disable']
                if scenario.startswith('mte-'):selected+=['--runtime-memtag-backend','software','--runtime-memtag-mode','sync']
                fault=scenario.startswith('mte-') and scenario!='mte-valid';diagnostics=()
                if fault:diagnostics=('[crash] memory-protection','Exit status: 122',f'Memory index: {entry["memory"]}','Pointer tag: 5','Allocation tag: 3')
                run(entry['name']+'-'+engine,selected,wasm,-signal.SIGILL if fault else 0,diagnostics)
        run('policy-check',[*configured(),'--ros-scene-check'])
        run('default-hidden',['--runtime-int'],out/'success-32-0.wasm',None)
        for mode in ('validation','section-details'):run('static-'+mode,['--mode',mode,*configured()],out/'success-32-0.wasm')
        assert sha(vm)==report['runtime_sha256'] and pins()==report['inputs'];report['complete']=True
    finally:save()
if __name__=='__main__':main()
