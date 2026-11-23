#!/usr/bin/env python3
"""Actual opcode/import replay, with independently observed external writes."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import struct
import time
from run_dump_launch_policy import VM, require


def corrupt_argument(data, mode='argument'):
    """Keep checksums valid; exercise the execution transaction's own checks."""
    data=bytearray(data);at=64
    for _ in range(struct.unpack_from('<Q',data,56)[0]):
        kind=struct.unpack_from('<H',data,at)[0];size=struct.unpack_from('<Q',data,at+24)[0];begin=at+32
        if kind==2:
            state=bytearray(data[begin:begin+size]);objects,roots,retained=struct.unpack_from('<QQQ',state,88)
            pos=128+roots*8+retained*48;changed=False
            for _ in range(objects):
                k,flags=struct.unpack_from('<HH',state,pos)
                links,values,payload=struct.unpack_from('<QQQ',state,pos+72)
                if k==22 and ((mode=='argument' and flags==11) or (mode=='memory-before' and flags==12)) and values:
                    if mode=='argument':
                        value=pos+96+links*8+32
                        struct.pack_into('<Q',state,value,struct.unpack_from('<Q',state,value)[0]^128)
                    elif mode=='memory-before':
                        memory=pos+96+links*8+values*48
                        require(payload>=96 and state[memory:memory+8]==b'UWRMEM1\0', 'memory transaction payload')
                        # First memory input digest, after magic/count/id/size.
                        state[memory+32]^=1
                    else:
                        raise AssertionError('unknown mutation')
                    changed=True;break
                pos+=96+links*8+values*48+payload
            require(changed,'recorded import not found')
            state[-32:]=hashlib.sha256(state[:-48]).digest();data[begin:begin+size]=state
            data[-32:]=hashlib.sha256(data[:-32]).digest();return bytes(data)
        at=begin+size
    raise AssertionError('typed state missing')


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--binary',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    p.add_argument('--runner-json',type=Path)
    p.add_argument('--indirect-host-calls',action='store_true')
    p.add_argument('--tamper-mode',choices=('argument','memory-before'),default='argument')
    args=p.parse_args()
    args.out=args.out.resolve();args.out.mkdir(parents=True,exist_ok=False)
    require('/docker-' in Path('/proc/self/cgroup').read_text(),'resource cgroup required')
    prefix=json.loads(args.runner_json.read_text()) if args.runner_json else []
    wat=args.out/'replay.wat';wasm=wat.with_suffix('.wasm')
    fixture=r'''(module
      (import "wasi_snapshot_preview1" "random_get" (func $random (param i32 i32) (result i32)))
      (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
      (memory 1) (global $count (mut i32) (i32.const 0))
      (data (i32.const 64) "\80\00\00\00\0e\00\00\00")
      (data (i32.const 128) "REPLAY-EFFECT\0a")
      (func (export "_start")
        (loop $again
          i32.const 0 i32.const 16 call $random drop
          i32.const 1 i32.const 64 i32.const 1 i32.const 80 call $write drop
          global.get $count i32.const 1 i32.add global.set $count
          br $again)))'''
    if args.indirect_host_calls:
        fixture=fixture.replace('(module','(module (type $rt (func (param i32 i32) (result i32))) (type $wt (func (param i32 i32 i32 i32) (result i32)))',1)
        fixture=fixture.replace('(memory 1)', '(table 2 funcref) (elem (i32.const 0) $random $write) (memory 1)',1)
        fixture=fixture.replace('call $random','i32.const 0 call_indirect (type $rt)').replace('call $write','i32.const 1 call_indirect (type $wt)')
    wat.write_text(fixture)
    subprocess.run(['/usr/bin/wat2wasm','--debug-names',str(wat),'-o',str(wasm)],check=True)
    result={'passed':False};vm=None
    try:
        vm=VM([str(args.binary.resolve()),'-Raot','-Rdbg','-Rct','0','-Rllvm-cache-path','disable',
            '--runtime-disable-memtag','--wasip1-global-enable',
            '--wasip1-global-interface','random_get','allow','--wasip1-global-grant','random',
            '--wasip1-global-interface','fd_write','allow','--wasip1-global-grant','stdout.write',
            '--run',str(wasm)],args.out,prefix)
        require(not vm.request('breakwasm 0 2 2').startswith('error:'),'loop boundary breakpoint')
        def stopped():
            deadline=time.monotonic()+120
            while True:
                status=vm.request('status');m=re.search(r'stop-id ([1-9][0-9]*)',status)
                if m and re.search(r'^thread [1-9]',status,re.M):return m[1],status
                require(time.monotonic()<deadline,status);time.sleep(.01)
        vm.request('continue');stop,status=stopped()
        original=args.out/'original.uwsnap'
        require('wasm-dump saved' in vm.request(f'dump wasm full {stop} {original}'),'origin export')
        stale=vm.request(f'dump record start {int(stop)+1}');require(stale.startswith('error:'),stale)
        started=vm.request(f'dump record start {stop}');require('mode=1 ' in started and not started.startswith('error:'),started)
        thread=re.search(r'thread ([1-9][0-9]*)',status)[1]
        require(vm.request(f'set wasm memory 0 0 {thread} 0 bytes ff').startswith('error:'),'recording mutation denied')
        vm.request('continue');stop,status=stopped()
        memory=vm.request('memory 0 0 0 16');require(not memory.startswith('error:'),memory)
        forbidden=args.out/'missing-parent'/'interval.uwsnap'
        failed=vm.request(f'dump record save {stop} {forbidden}')
        require(failed.startswith('error:') and not forbidden.exists(), 'failed save published an archive')
        require(vm.request('status')==status,'failed save moved the stop')
        journal=args.out/'interval.uwsnap';saved=vm.request(f'dump record save {stop} {journal}')
        require('saved=1' in saved and not saved.startswith('error:'),saved)
        saved_bytes=journal.read_bytes()
        duplicate=vm.request(f'dump record save {stop} {journal}')
        require(duplicate.startswith('error:') and journal.read_bytes()==saved_bytes, 'duplicate save overwrote its archive')
        copied=args.out/'interval-copy.uwsnap'
        require('saved=1' in vm.request(f'dump record save {stop} {copied}') and copied.read_bytes()==saved_bytes,
                'sealed journal changed on retry')
        # Each successful host call must be present, not merely opcode stops.
        counts=re.search(r'instructions=(\d+) events=(\d+)',saved)
        require(counts and int(counts[2])==int(counts[1])+10, 'missing or duplicate import journal events: '+saved)
        require((args.out/'vm.log').read_bytes().count(b'REPLAY-EFFECT')==1,'recording external write exactly once')
        ordinary=vm.request(f'dump restore current {stop} {journal}')
        require(ordinary.startswith('error:') and 'retained=1' in ordinary,'ordinary restore must reject a journal')
        rounds=[]
        for _ in range(2):
            restored=vm.request(f'dump replay {stop} {journal}')
            for attempt in range(10):
                if not restored.startswith('snapshot-restore pending'):break
                restored=vm.request('dump restore continue')
            require(restored.startswith('snapshot-restored'),restored)
            stop,status=stopped();vm.request('continue');stop,status=stopped()
            observed=vm.request('memory 0 0 0 16');require(observed==memory,{'recorded':memory,'replayed':observed})
            state=vm.request('dump record status');require('mode=4 ' in state,state)
            require((args.out/'vm.log').read_bytes().count(b'REPLAY-EFFECT')==1,'replay duplicated an external write')
            require(vm.request('continue').startswith('error:'),'endpoint must not resume live imports')
            rounds.append({'status':state,'memory':observed})
        # Returning to live execution is an explicit, ordinary world restore.
        restored=vm.request(f'dump restore current {stop} {original}')
        require(restored.startswith('snapshot-restored'),restored)
        vm.request('continue');stop,status=stopped()
        require((args.out/'vm.log').read_bytes().count(b'REPLAY-EFFECT')==2,'ordinary restoration explicitly reenters live execution')
        tampered=args.out/'divergent-transaction.uwsnap';tampered.write_bytes(corrupt_argument(journal.read_bytes(),args.tamper_mode))
        folder=args.out/'divergence';folder.mkdir();other=None
        try:
            other=VM([str(args.binary.resolve()),'-Raot','-Rdbg','-Rct','0','-Rllvm-cache-path','disable',
                '--runtime-disable-memtag','--wasip1-global-enable',
                '--wasip1-global-interface','random_get','allow','--wasip1-global-grant','random',
                '--wasip1-global-interface','fd_write','allow','--wasip1-global-grant','stdout.write',
                '--run',str(wasm)],folder,prefix)
            other.request('breakwasm 0 2 2');other.request('continue')
            deadline=time.monotonic()+120
            while True:
                other_status=other.request('status');matched=re.search(r'stop-id ([1-9][0-9]*)',other_status)
                if matched and re.search(r'^thread [1-9]',other_status,re.M):break
                require(time.monotonic()<deadline,other_status);time.sleep(.01)
            other_stop=matched[1]
            restored=other.request(f'dump replay {other_stop} {tampered}')
            require(restored.startswith('snapshot-restored'),restored)
            try:other.request('continue')
            except (AssertionError,ConnectionError,OSError):pass
            code=other.process.wait(timeout=120)
            require(code==125,{'divergenceExit':code})
            require(b'uwvm: [error] execution replay refused; exit=125' in (folder/'vm.log').read_bytes(), 'missing replay refusal diagnostic')
            require(b'REPLAY-EFFECT' not in (folder/'vm.log').read_bytes(),'divergence executed an external provider')
            result['divergenceExit']=code;result['tamperMode']=args.tamper_mode
        finally:
            if other:result['divergenceCommands']=other.commands;other.close()
        result.update(passed=True,rounds=rounds,journalSha256=hashlib.sha256(journal.read_bytes()).hexdigest())
    except Exception as error:result['error']=repr(error)
    finally:
        if vm:result['commands']=vm.commands;vm.close()
        (args.out/'results.json').write_text(json.dumps(result,indent=2))
    print(json.dumps({k:v for k,v in result.items() if k not in ('commands','rounds','divergenceCommands')}),flush=True)
    require(result['passed'],'actual execution replay failed; inspect results.json')

if __name__=='__main__':main()
