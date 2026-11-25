#!/usr/bin/env python3
"""Actual VM and DAP execution-recording integration, no synthetic VM replies."""
import argparse
import io
import json
from pathlib import Path
import re
import subprocess
import sys
import time
from run_dump_launch_policy import VM, require
sys.path.insert(0,str(Path(__file__).resolve().parents[2]/'tools/debug'))
import dap_adapter as dap


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--binary',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
    p.add_argument('--runner-json',type=Path);args=p.parse_args()
    args.out=args.out.resolve();args.out.mkdir(parents=True,exist_ok=False)
    require('/docker-' in Path('/proc/self/cgroup').read_text(),'resource cgroup required')
    prefix=json.loads(args.runner_json.read_text()) if args.runner_json else []
    wat=args.out/'program.wat';wasm=wat.with_suffix('.wasm')
    wat.write_text(r'''(module
      (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
      (memory 1) (data (i32.const 0) "\10\00\00\00\04\00\00\00") (data (i32.const 16) "DAP\0a")
      (func (export "_start") (loop i32.const 1 i32.const 0 i32.const 1 i32.const 32 call $write drop br 0)))''')
    subprocess.run(['/usr/bin/wat2wasm',str(wat),'-o',str(wasm)],check=True)
    result={'passed':False,'dap':[]};vm=None
    try:
        vm=VM([str(args.binary.resolve()),'-Raot','-Rdbg','-Rct','0','-Rllvm-cache-path','disable',
            '--wasm-policy-global', 'runtime.memtag-checks', 'off','--wasip1-global-enable','--wasip1-global-interface','fd_write','allow',
            '--wasip1-global-grant','stdout.write','--run',str(wasm)],args.out,prefix)
        vm.request('breakwasm 0 1 2');vm.request('continue')
        def stopped():
            deadline=time.monotonic()+120
            while True:
                status=vm.request('status');stop=re.search(r'stop-id ([1-9][0-9]*)',status)
                if stop and re.search(r'^thread [1-9]',status,re.M):return stop[1],status
                require(time.monotonic()<deadline,status);time.sleep(.01)
        stop,status=stopped();output=io.BytesIO();adapter=dap.Adapter(output)
        adapter.broker=vm;adapter.debug_permissions='full';adapter.configuration_complete=True
        adapter.observe(status,notify=False)
        def request(command,arguments):
            output.seek(0);output.truncate()
            adapter.handle({'seq':len(result['dap'])+1,'command':command,'arguments':arguments})
            output.seek(0);messages=[]
            while line:=output.readline():
                size=int(line.split(b':')[1]);require(output.readline()==b'\r\n','DAP framing')
                messages.append(json.loads(output.read(size)))
            result['dap'].append({'command':command,'arguments':arguments,'messages':messages})
            reply=next(m for m in messages if m['type']=='response')
            require(reply['success'],reply);return reply.get('body',{})
        started=request('uwvm/recordSnapshot',{'action':'start','stopId':stop});require(started['mode']==1,started)
        vm.request('continue');stop,status=stopped();adapter.observe(status,notify=False)
        path=args.out/'recording.uwsnap'
        saved=request('uwvm/recordSnapshot',{'action':'save','stopId':stop,'path':str(path)})
        require(saved['mode']==2 and saved['saved'],saved)
        frames=request('stackTrace',{'threadId':adapter.threads[0]['id']})['stackFrames'];require(frames,'actual frame')
        old_frame=frames[0]['id']
        restored=request('uwvm/restoreSnapshot',{'environment':'replay','stopId':stop,'path':str(path)})
        for _ in range(12):
            if not restored.get('pending'):break
            restored=request('uwvm/restoreSnapshot',{'continue':True})
        require(restored['applied'] and not restored['pending'] and old_frame not in adapter.frames,restored)
        require(any(m.get('event')=='invalidated' for x in result['dap'] for m in x['messages']),'no retired-reference event')
        vm.request('continue');stop,status=stopped();adapter.observe(status,notify=False)
        endpoint=request('uwvm/recordSnapshot',{'action':'status'});require(endpoint['mode']==4,endpoint)
        require((args.out/'vm.log').read_bytes().count(b'DAP\n')==1,'DAP replay repeated external write')
        require(vm.request('continue').startswith('error:'),'endpoint continued')
        result['passed']=True
    except Exception as error:result['error']=repr(error)
    finally:
        if vm:result['commands']=vm.commands;vm.close()
        (args.out/'results.json').write_text(json.dumps(result,indent=2))
    print(json.dumps({k:v for k,v in result.items() if k not in ('commands','dap')}),flush=True)
    require(result['passed'],'actual DAP replay failed; inspect results.json')

if __name__=='__main__':main()
