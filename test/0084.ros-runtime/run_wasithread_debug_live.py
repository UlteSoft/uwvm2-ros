#!/usr/bin/env python3
"""Authenticate the real stopped VM command, DAP decoding, and stale-stop refusal."""
import argparse
import hashlib
import io
import json
import re
import subprocess
import sys
import time
from pathlib import Path
from run_scene_vm import verify_gate, GATE
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'test/0018.debugger'))
sys.path.insert(0,str(ROOT/'tools/debug'))
from run_dump_launch_policy import VM, require
import dap_adapter as dap

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--runtime',required=True,type=Path);ap.add_argument('--output',required=True,type=Path)
    args=ap.parse_args();verify_gate();out=args.output.resolve()
    assert out.is_relative_to(ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security')
    out.mkdir(parents=True,exist_ok=False);binary=args.runtime.resolve()
    wat=out/'loop.wat';wasm=out/'loop.wasm'
    wat.write_text('(module (func (export "_start") (loop $again nop br $again)))')
    subprocess.run(['wat2wasm','--debug-names',str(wat),'-o',str(wasm)],check=True)
    result=dict(passed=False,runtime_sha256=hashlib.sha256(binary.read_bytes()).hexdigest(),cgroup=str(GATE));vm=None
    try:
        vm=VM([str(binary),'-Raot','-Rdbg','-Rct','0','-Rllvm-cache-path','disable',
            '--wasm-set-main-module-name','main','--wasithread-group-create','workers',
            '--wasithread-group-add-module','workers','main','--wasithread-group-enable','workers',
            '--wasithread-group-interface','workers','thread-spawn','deny','--run',str(wasm)],out,[])
        require(not vm.request('breakwasm 0 0 0').startswith('error:'),'entry breakpoint')
        vm.request('continue');until=time.monotonic()+60
        while True:
            status=vm.request('status');match=re.search(r'stop-id ([1-9][0-9]*)',status)
            if match and re.search(r'^thread [1-9]',status,re.M):break
            require(time.monotonic()<until,status);time.sleep(.01)
        stop=int(match[1]);result['stop']=stop
        for selected in ('policy','members'):
            raw=vm.request(f'info wasithread {selected} 0 8 {stop}')
            options=dap.wasithread_state_options({'selection':selected,'ifStop':stop})
            decoded=dap.parse_wasithread_state(raw,options,stop)
            result[selected]=decoded
            require(decoded['rows'],'empty real state')
        require(any(row.get('Target')=='workers' for row in result['policy']['rows']),'real group missing')
        require(result['members']['rows'][0]['Module']=='main','real membership missing')
        stale=vm.request(f'info wasithread policy 0 8 {stop+1}')
        require('stale' in stale or stale.startswith('error:'),stale)
        require(f'stop-id {stop}\n' in vm.request('status'),'failed read changed execution')
        output=io.BytesIO();adapter=dap.Adapter(output);adapter.broker=vm;adapter.debug_permissions='full'
        adapter.configuration_complete=True;adapter.observe(status,notify=False)
        adapter.handle({'seq':1,'command':'uwvm/wasithreadState','arguments':{'ifStop':str(stop)}})
        output.seek(0);messages=[]
        while line:=output.readline():
            n=int(line.split(b':')[1]);require(output.readline()==b'\r\n','framing');messages.append(json.loads(output.read(n)))
        result['dap']=messages
        require(any(m.get('type')=='response' and m.get('success') for m in messages),'live DAP query failed')
        result['passed']=True
    except Exception as e:result['error']=repr(e)
    finally:
        if vm:result['commands']=vm.commands;vm.close()
        (out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
    print({k:v for k,v in result.items() if k in ('passed','error','stop')},flush=True)
    require(result['passed'],'real I1T inspection failed')
if __name__=='__main__':main()
