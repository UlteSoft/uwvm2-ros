#!/usr/bin/env python3
"""Exercise immutable artifact guest records and inert deployment plans.

All workload execution requires the existing 64 GiB zero-swap cgroup. Guests
import only the new scene namespace, covering memory32/64 and memory indices
zero/one. An inotify witness distinguishes policy checks from actual input I/O.
This qualifies immutable input delivery, not output stores or model inference.
"""
import argparse
import ctypes
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import time

HERE=Path(__file__).resolve().parent
spec=importlib.util.spec_from_file_location('scene_fixture',HERE/'run_scene_vm.py')
base=importlib.util.module_from_spec(spec);spec.loader.exec_module(base)
ROOT,GATE=base.ROOT,base.GATE
OPERATIONS=[dict(name='artifact.open',words=5,bytes=8),dict(name='artifact.read',words=3,bytes=-1),
            dict(name='artifact.describe',words=1,bytes=64),dict(name='resource.close',words=1,bytes=0),
            dict(name='guard.create',words=0,bytes=8)]
PAYLOAD=bytes(range(96))
DIGEST=hashlib.sha256(PAYLOAD).digest()
GENERATION=17


def opening(generation=GENERATION,digest=DIGEST):
    return [generation]+[int.from_bytes(digest[n:n+8],'little') for n in range(0,32,8)]


def byte_check(guest,address,value):
    guest.check(f'i{guest.width}.const {address} i32.load8_u $work',value)


def successful_guest(width,memory):
    g=base.Guest(OPERATIONS,width,memory)
    caps={name:g.open(name) for name in ['config','other','weights','zero','no-read','rate','no-transfer','signal']}
    g.call('artifact.open',caps['config'],opening(),size=39,expected=22)
    g.call('artifact.open',caps['config'],opening(),memory=memory+1,expected=22)
    g.call('artifact.open',caps['config'],opening(),output=65535,expected=22)
    g.call('artifact.open',caps['config'],opening(),length=512,expected=22)
    g.call('artifact.open',caps['config'],opening(),length=128,expected=22)
    g.call('artifact.open',caps['config'],opening(GENERATION+1),expected=13)
    g.call('artifact.open',caps['config'],opening(digest=b'\0'*32),expected=13)
    g.call('artifact.open',caps['config'],opening(),capacity=7,expected=90)
    g.equal(g.word(1024),8)
    g.call('artifact.open',caps['config'],opening());artifact=g.save('artifact')
    g.call('artifact.open',caps['config'],opening(),expected=11)
    g.call('artifact.open',caps['zero'],opening(),expected=11)
    g.call('artifact.describe',caps['config'],[artifact],capacity=63,expected=90)
    g.equal(g.word(1024),64)
    g.call('artifact.describe',caps['config'],[artifact]);g.equal(g.word(1024),64)
    for index,value in enumerate([GENERATION,len(PAYLOAD),2,32,*opening()[1:]]):g.equal(g.word(512+index*8),value)
    g.call('artifact.read',caps['other'],[artifact,0,8],expected=9)
    g.call('artifact.read',caps['config'],[0,0,8],expected=9)
    g.call('artifact.read',caps['config'],[artifact,0,33],capacity=64,expected=90)
    g.call('artifact.read',caps['config'],[artifact,0,32],capacity=31,expected=90)
    g.call('artifact.read',caps['config'],[artifact,0,32],capacity=64)
    g.equal(g.word(1024),32)
    for index in range(4):g.equal(g.word(512+index*8),int.from_bytes(PAYLOAD[index*8:index*8+8],'little'))
    g.call('artifact.read',caps['config'],[artifact,90,32]);g.equal(g.word(1024),6)
    for index in range(6):byte_check(g,512+index,90+index)
    g.call('artifact.read',caps['config'],[artifact,96,32]);g.equal(g.word(1024),0)
    g.call('artifact.read',caps['config'],[artifact,97,0],capacity=0,expected=90)
    g.call('artifact.read',caps['config'],[artifact,2**64-1,1],expected=90)
    g.call('artifact.read',caps['config'],[artifact,0,2**64-1],expected=90)
    g.call('artifact.read',caps['config'],[artifact,0,0],capacity=0);g.equal(g.word(1024),0)
    g.call('artifact.read',caps['config'],[artifact,0,8],memory=memory+1,expected=22)
    g.call('artifact.read',caps['config'],[artifact,0,8],output=65532,expected=22)
    g.call('artifact.read',caps['config'],[artifact,0,8],length=512,expected=22)
    g.call('artifact.read',caps['config'],[artifact,0,8],length=128,expected=22)
    g.call('artifact.read',caps['config'],[artifact,0,8],output=2**64-1,expected=22)
    g.call('artifact.read',caps['config'],[artifact,0,8]);g.equal(g.word(512),int.from_bytes(PAYLOAD[:8],'little'))
    g.call('guard.create',caps['signal']);guard=g.save('guard')
    g.call('artifact.read',caps['config'],[guard,0,8],expected=9)
    g.call('resource.close',caps['config'],[guard],expected=9)
    g.call('resource.close',caps['signal'],[artifact],expected=9)
    g.call('artifact.open',caps['weights'],opening());weights=g.save('weights-object')
    g.call('artifact.describe',caps['weights'],[weights]);g.equal(g.word(528),5)
    g.call('artifact.open',caps['no-read'],opening());unread=g.save('unread')
    g.call('artifact.read',caps['no-read'],[unread,0,8],expected=13)
    g.call('artifact.open',caps['no-transfer'],opening());empty=g.save('no-transfer-object')
    g.call('artifact.read',caps['no-transfer'],[empty,0,1],expected=90)
    g.call('artifact.read',caps['no-transfer'],[empty,0,0],capacity=0)
    g.call('artifact.open',caps['rate'],opening());limited=g.save('limited')
    g.call('artifact.read',caps['rate'],[limited,0,1],expected=11)
    g.check(caps['rate']+' call $close');caps['rate']=g.open('rate')
    g.call('artifact.open',caps['rate'],opening(),expected=11)
    g.call('resource.close',caps['config'],[artifact])
    g.call('artifact.read',caps['config'],[artifact,0,8],expected=9)
    g.call('artifact.open',caps['config'],opening());reopened=g.save('reopened')
    g.call('artifact.read',caps['config'],[reopened,0,8]);g.equal(g.word(512),int.from_bytes(PAYLOAD[:8],'little'))
    g.call('resource.close',caps['config'],[reopened]);g.call('resource.close',caps['signal'],[guard])
    return g


def configured(path,*,budget=True):
    flags=base.policy(OPERATIONS)
    aliases=['config','other','weights','zero','no-read','rate','no-transfer']
    if budget:flags+=['--ros-scene-input-budget','robot',str(len(PAYLOAD)*len(aliases)),str(len(aliases))]
    for alias in aliases:
        actions=['artifact.open','artifact.describe','resource.close']
        if alias!='no-read':actions+=['artifact.read']
        flags+=['--ros-scene-grant','robot',alias,'model-weights' if alias=='weights' else 'configuration',alias,'sealed-input',','.join(actions)]
        flags+=['--ros-scene-input','robot',alias,str(path),DIGEST.hex(),str(GENERATION),str(len(PAYLOAD)),str(0 if alias=='no-transfer' else 32)]
    flags+=['--ros-scene-limit','robot','config','max-resources','1',
            '--ros-scene-limit','robot','zero','max-resources','0',
            '--ros-scene-limit','robot','rate','delivery','may-reject',
            '--ros-scene-limit','robot','rate','calls-per-window','1',
            '--ros-scene-limit','robot','rate','window-ns','3600000000000']
    flags+=base.grant('signal','guard',['guard.create','resource.close'])
    return flags


class InputWitness:
    """Observe source opens/accesses without consuming or modifying its bytes."""
    def __init__(self,path):
        libc=ctypes.CDLL(None,use_errno=True)
        self.fd=libc.inotify_init1(os.O_NONBLOCK|os.O_CLOEXEC);assert self.fd>=0
        assert libc.inotify_add_watch(self.fd,os.fsencode(path),0x1|0x20)>=0
    def finish(self):
        try:
            try:data=os.read(self.fd,65536)
            except BlockingIOError:data=b''
            return len(data)
        finally:os.close(self.fd)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime',type=Path)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--source-root',type=Path,default=ROOT,
                        help='Production source snapshot corresponding to the executed VM; fixtures stay in the checkout')
    parser.add_argument('--prepare-only',action='store_true')
    parser.add_argument('--static-only',action='store_true',help='Record both static-mode regressions before failing; execute no guest')
    args=parser.parse_args();base.verify_gate()
    assert not(args.prepare_only and args.static_only)
    assert os.sched_getaffinity(0).issubset(set(range(8,16)))
    if not args.prepare_only and args.runtime is None:parser.error('--runtime is required')
    production=args.source_root.resolve()
    out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    source=out/'robot-input.bin';source.write_bytes(PAYLOAD)
    inputs=[Path(__file__),HERE/'run_scene_vm.py']
    if not args.prepare_only:
        inputs += [production/'src/uwvm2/imported/ros/scene'/name for name in
                   ['artifact_contract.h','artifacts.h','storage.h','authority.h','broker.h','deployment.h']]
        inputs += [production/'src/uwvm2/uwvm/imported/scene/host.h']
    def source_key(path):
        return str(path.relative_to(production)) if path.is_relative_to(production/'src') else str(path.relative_to(ROOT))
    report=dict(complete=False,mode='prepare' if args.prepare_only else 'static' if args.static_only else 'execute',cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,
                events_before=(GATE/'memory.events').read_text(),runtime_sha256=base.sha(args.runtime) if args.runtime else None,
                payload_sha256=DIGEST.hex(),fixture_sha256=base.sha(__file__),
                source_root=str(production),sources={source_key(path):base.sha(path) for path in inputs},cases=[],guests=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def compile(name,guest):
        wat,wasm=out/(name+'.wat'),out/(name+'.wasm');wat.write_text(guest.render())
        subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True)
        report['guests'].append(dict(name=name,sha256=base.sha(wasm),assertions=len(guest.checks)));save();return wasm
    def run(name,flags,wasm=None,success=True,witness=None,io_expected=None):
        base.verify_gate();command=[str(args.runtime.resolve()),'--wasm-set-main-module-name','main',*flags]
        if wasm:command+=['--run',str(wasm)]
        watch=InputWitness(witness) if witness else None;start=time.monotonic()
        try:result=subprocess.run(command,capture_output=True,timeout=30)
        finally:observed=watch.finish() if watch else None
        (out/(name+'.stdout')).write_bytes(result.stdout);(out/(name+'.stderr')).write_bytes(result.stderr)
        passed=(result.returncode==0)==success and (io_expected is None or bool(observed)==io_expected)
        report['cases'].append(dict(name=name,argv=command,exit=result.returncode,passed=passed,seconds=time.monotonic()-start,
                                    observed_input_event_bytes=observed,expected_input_io=io_expected));save()
        if not args.static_only:assert passed,(name,result.returncode,observed,result.stderr.decode(errors='replace')[-3000:])
        print('PASS' if passed else 'FAIL',name,flush=True)
    smoke=compile('imports',base.Guest(OPERATIONS))
    policy=configured(source)
    for width,memory in ([] if args.static_only else [(32,0),(64,0),(32,1),(64,1)]):
        for mode in ['success','denied','no-grants']:
            if mode=='success':guest=successful_guest(width,memory)
            else:
                guest=base.Guest(OPERATIONS,width,memory)
                for name,arguments in [('artifact.open',opening()),('artifact.read',[1,0,8]),('artifact.describe',[1])]:
                    guest.call(name,1,arguments,expected=13 if mode=='denied' else 9)
            wasm=compile(f'{mode}-{width}-memory{memory}',guest)
            if args.prepare_only:continue
            flags=policy if mode=='success' else base.policy(OPERATIONS,'deny' if mode=='denied' else 'allow')
            for engine in ['int','aot']:
                run(f'{mode}-{width}-memory{memory}-{engine}',['--runtime-'+engine,*flags],wasm)
    if not args.prepare_only and not args.static_only:
        run('hidden-default',['--runtime-int'],smoke,False)
        run('inert-plan-check',[*policy,'--ros-scene-check'],witness=source,io_expected=False)
        no_budget=configured(source,budget=False)
        run('zero-budget-no-io',['--runtime-int',*no_budget],smoke,False,witness=source,io_expected=False)
        late_budget=no_budget+['--ros-scene-input-budget','robot',str(len(PAYLOAD)*7),'7']
        run('late-budget-inert',[*late_budget,'--ros-scene-check'],witness=source,io_expected=False)
        for name,field,value in [('byte-budget-short',2,str(len(PAYLOAD)*7-1)),('object-budget-short',3,'6')]:
            bad=policy.copy();bad[bad.index('--ros-scene-input-budget')+field]=value
            run(name,['--runtime-int',*bad],smoke,False,witness=source,io_expected=False)
        for name,field,value in [('digest-short',4,DIGEST.hex()[:-1]),('digest-nonhex',4,'z'*64),
                                 ('generation-negative',5,'-1'),('transfer-over-maximum',7,str(4*1024*1024+1))]:
            bad=policy.copy();bad[bad.index('--ros-scene-input')+field]=value
            run(name,['--runtime-int',*bad],smoke,False,witness=source,io_expected=False)
        bad=policy.copy();at=bad.index('--ros-scene-input');bad+=bad[at:at+8]
        run('duplicate-input-plan',['--runtime-int',*bad],smoke,False,witness=source,io_expected=False)
        # An inotify positive control rules out a broken no-I/O witness.
        run('authorized-input-io',['--runtime-int',*policy],smoke,witness=source,io_expected=True)
        invalid=[('bad-digest',DIGEST.hex(),'0'*64),('bad-generation',str(GENERATION),'0'),
                 ('size-short',str(len(PAYLOAD)),str(len(PAYLOAD)-1))]
        for name,old,new in invalid:
            bad=policy.copy();at=bad.index('--ros-scene-input')
            index={'bad-digest':4,'bad-generation':5,'size-short':6}[name];assert bad[at+index]==old;bad[at+index]=new
            run(name,['--runtime-int',*bad],smoke,False)
        truncated=out/'truncated.bin';truncated.write_bytes(PAYLOAD[:-1]);bad=policy.copy();bad[bad.index('--ros-scene-input')+3]=str(truncated)
        run('source-size-mismatch',['--runtime-int',*bad],smoke,False)
        changed=out/'changed.bin';changed.write_bytes(bytes([255])+PAYLOAD[1:]);bad=policy.copy();bad[bad.index('--ros-scene-input')+3]=str(changed)
        run('source-digest-mismatch',['--runtime-int',*bad],smoke,False)
        link=out/'input-symlink';link.symlink_to(source.name)
        bad=policy.copy();bad[bad.index('--ros-scene-input')+3]=str(link)
        run('source-symlink',['--runtime-int',*bad],smoke,False)
        fifo=out/'input-fifo';os.mkfifo(fifo);bad=policy.copy();bad[bad.index('--ros-scene-input')+3]=str(fifo)
        run('source-nonregular',['--runtime-int',*bad],smoke,False)
    if not args.prepare_only:
        # Static validation must check policy shape/budgets without activating
        # any input backend, even when native input is unusable or absent.
        for mode in ['validation','section-details']:
            flags=['--mode',mode,*policy]
            run('static-'+mode+'-valid',flags,smoke,witness=source,io_expected=False)
            bad=policy.copy();bad[bad.index('--ros-scene-input')+4]='0'*64
            run('static-'+mode+'-wrong-digest',['--mode',mode,*bad],smoke,witness=source,io_expected=False)
            missing=out/'never-created-input.bin';assert not missing.exists()
            absent=configured(missing)
            run('static-'+mode+'-missing-source',['--mode',mode,*absent],smoke)
            run('static-'+mode+'-zero-budget',['--mode',mode,*configured(source,budget=False)],smoke,False,witness=source,io_expected=False)
            for name,field,value in [('byte-budget-short',2,str(len(PAYLOAD)*7-1)),('object-budget-short',3,'6')]:
                bad=policy.copy();bad[bad.index('--ros-scene-input-budget')+field]=value
                run('static-'+mode+'-'+name,['--mode',mode,*bad],smoke,False,witness=source,io_expected=False)
        assert base.sha(args.runtime)==report['runtime_sha256']
    assert base.sha(source)==DIGEST.hex() and base.sha(__file__)==report['fixture_sha256']
    report['sources_after']={source_key(path):base.sha(path) for path in inputs}
    assert report['sources_after']==report['sources']
    report['events_after']=(GATE/'memory.events').read_text();report['complete']=all(case['passed'] for case in report['cases']);save()
    assert report['complete'],'Static regression failures recorded in results.json'


if __name__=='__main__':main()
