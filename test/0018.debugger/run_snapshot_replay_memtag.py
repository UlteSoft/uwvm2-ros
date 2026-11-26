#!/usr/bin/env python3
"""Real tagged-memory/entropy replay; external WASI effects occur only once."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys
import time
from run_dump_launch_policy import VM, require
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / '0086.memtag'))
import vm_matrix as mte


def fixture(log=False,wide=False,shared=False,tag_value=3,*,memory32_adm=4):
    bits=64 if wide else 32;word=bits//8;kind=bytes([0x7e if wide else 0x7f])
    n=lambda value:mte.c(value,bits)
    op=mte.op;sec=mte.sec;u=mte.u
    code=b'\x03\x40'+n(256)+n(32)+op(0x2f)+b'\x21\x00'
    code+=b'\x20\x00'+mte.c(0x12345678)+mte.mem(0x36)
    code+=n(512)+n(tag_value)+op(0x22)+n(32)+op(0x2b)
    if log:code+=n(512)+n(0 if tag_value else 1)+op(0x22)+mte.mem(0x28)+b'\x1a'
    code+=n(0)+n(16)+b'\x10\x00\x1a'
    code+=mte.c(1)+n(64)+n(1)+n(80)+b'\x10\x01\x1a'
    code+=b'\x0c\x00\x0b\x0b'
    body=b'\x01\x01'+kind+code
    module=b'\0asm\x01\0\0\0'+sec(1,b'\x03\x60\x02'+kind*2+b'\x01\x7f\x60\x04\x7f'+kind*3+b'\x01\x7f\x60\0\0')
    imports=b'\x02'
    for name,index in [('random_get',0),('fd_write',1)]:
        if wide:name+='_wasm64'
        imports+=u(22)+b'wasi_snapshot_preview1'+u(len(name))+name.encode()+b'\x00'+u(index)
    module+=sec(2,imports)+sec(3,b'\x01\x02')+sec(5,b'\x01'+mte.memory_limits(wide,shared,memory32_adm=memory32_adm))
    module+=sec(7,b'\x01\x06_start\x00\x02')+sec(10,b'\x01'+u(len(body))+body)
    payload=b'MTE-REPLAY-EFFECT\n'
    data=b'\x02\x00'+n(64)+b'\x0b'+u(word*2)+(128).to_bytes(word,'little')+len(payload).to_bytes(word,'little')
    data+=b'\x00'+n(128)+b'\x0b'+u(len(payload))+payload
    return module+sec(11,data)


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--binary',required=True,type=Path);parser.add_argument('--out',required=True,type=Path)
    parser.add_argument('--runner-json',type=Path);parser.add_argument('--backend',choices=('software','arm-mte'),default='software')
    parser.add_argument('--mode',choices=('sync','async'),default='sync');parser.add_argument('--log',action='store_true')
    parser.add_argument('--memory64',action='store_true');parser.add_argument('--shared',action='store_true')
    parser.add_argument('--tag',type=lambda x:int(x,0),default=3)
    args=parser.parse_args();require(0<=args.tag<=15,'tag width');args.out=args.out.resolve();args.out.mkdir(parents=True,exist_ok=False)
    require('/docker-' in Path('/proc/self/cgroup').read_text(),'resource cgroup required')
    wasm=args.out/'tagged.wasm'
    wasm.write_bytes(fixture(args.log,args.memory64,args.shared,args.tag,memory32_adm=8 if args.backend=='arm-mte' else 4))
    vm=None;result={'passed':False}
    try:
        flags=['--wasm-policy-global', 'runtime.memtag-backend',args.backend,'--runtime-memtag-mode',args.mode]
        if args.log:flags+=['--runtime-memtag-action','silent-log','path',str(args.out/'violations.log')]
        prefix=json.loads(args.runner_json.read_text()) if args.runner_json else []
        vm=VM([str(args.binary.resolve()),'-Raot','-Rdbg','-Rct','0','-Rllvm-cache-path','disable',*flags,
            '--wasip1-global-enable','--wasip1-global-interface','random_get_wasm64' if args.memory64 else 'random_get','allow','--wasip1-global-grant','random',
            '--wasip1-global-interface','fd_write_wasm64' if args.memory64 else 'fd_write','allow','--wasip1-global-grant','stdout.write','--run',str(wasm)],args.out,prefix)
        require(not vm.request('breakwasm 0 2 2').startswith('error:'),'loop breakpoint')
        def stopped():
            deadline=time.monotonic()+120
            while True:
                status=vm.request('status');match=re.search(r'stop-id ([1-9][0-9]*)',status)
                if match and re.search(r'^thread [1-9]',status,re.M):return match[1],status
                require(time.monotonic()<deadline,status);time.sleep(.01)
        vm.request('continue');stop,_=stopped()
        started=vm.request(f'dump record start {stop}');require('mode=1 ' in started,started)
        vm.request('continue');stop,_=stopped()
        requests=['memory 0 0 0 16','memory 0 0 256 32','memtags 0 0 256 32','memtags 0 0 512 32']
        expected=[vm.request(command) for command in requests]
        require(all(not value.startswith('error:') for value in expected),expected)
        require(f'value=0x{args.tag:02x}' in expected[-1], 'full allocation tag was not retained')
        archive=args.out/'interval.uwsnap';saved=vm.request(f'dump record save {stop} {archive}')
        require('saved=1' in saved,saved)
        rounds=[]
        for _ in range(2):
            restored=vm.request(f'dump replay {stop} {archive}')
            for attempt in range(10):
                if not restored.startswith('snapshot-restore pending'):break
                restored=vm.request('dump restore continue')
            require(restored.startswith('snapshot-restored'),restored)
            stop,_=stopped();continued=vm.request('continue');require(not continued.startswith('error:'),continued);stop,_=stopped()
            actual=[vm.request(command) for command in requests];require(actual==expected,{'expected':expected,'actual':actual})
            journal=vm.request('dump record status');require('mode=4 ' in journal,journal)
            require((args.out/'vm.log').read_bytes().count(b'MTE-REPLAY-EFFECT')==1,'replay duplicated real WASI output')
            rounds.append({'status':journal,'observations':actual})
        if args.log:require((args.out/'violations.log').stat().st_size>0,'MTE violation never reached report')
        result.update(passed=True,rounds=rounds,record=saved,journalSha256=hashlib.sha256(archive.read_bytes()).hexdigest())
    except Exception as error:result['error']=repr(error)
    finally:
        if vm:result['commands']=vm.commands;vm.close()
        (args.out/'results.json').write_text(json.dumps(result,indent=2))
    print(json.dumps({key:value for key,value in result.items() if key not in ('commands','rounds')}),flush=True)
    require(result['passed'],'MTE replay failed; inspect results.json')


if __name__=='__main__':main()
