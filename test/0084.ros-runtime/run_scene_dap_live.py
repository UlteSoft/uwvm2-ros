#!/usr/bin/env python3
"""Qualify actual Linux VM, authenticated broker and scene DAP subprocesses.

Run through tools/ros_scene/in_existing_cgroup.py. This fixture exercises local
scene metadata, not ROS network providers or the graphical editor application.
Owned broker teardown reaps its own guest; no PID lookup or global kill is used.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'test/0018.debugger'))
from run_dap_current_broker import BrokerSession
from run_scene_debug import gate,GROUP

WAT='''(module
 (import "uwvm_ros_v2" "capability.open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros_v2" "guard.create" (func $create (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (import "uwvm_ros_v2" "guard.trigger" (func $trigger (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (memory 1) (data (i32.const 0) "signal")
 (func $spin (export "spin") (loop $again br $again))
 (func (export "_start")
  i32.const 0 i64.const 0 i64.const 6 i64.const 64 call $open if unreachable end
  i32.const 64 i64.load i32.const 0 i64.const 0 i64.const 0 i64.const 72 i64.const 8 i64.const 128 call $create if unreachable end
  i32.const 64 i64.load i32.const 0 i64.const 72 i64.const 8 i64.const 96 i64.const 0 i64.const 128 call $trigger if unreachable end
  call $spin))'''


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime',required=True,type=Path);parser.add_argument('--output',required=True,type=Path)
    args=parser.parse_args();gate();out=args.output.resolve();out.mkdir(parents=True,exist_ok=False)
    wat=out/'scene.wat';wasm=out/'scene.wasm';wat.write_text(WAT)
    subprocess.run(['wat2wasm','--debug-names',str(wat),'-o',str(wasm)],check=True)
    flags=['--wasm-set-main-module-name','main','--ros-scene-create','robot','--ros-scene-bind','robot','main']
    for action in ['capability.open','guard.create','guard.trigger']:
        flags+=['--ros-scene-interface','robot',action,'allow']
    flags+=['--ros-scene-grant','robot','signal','guard','signal','local','guard.create,guard.trigger,resource.observe']
    paths=[args.runtime.resolve(),ROOT/'tools/debug/dap_adapter.py',ROOT/'tools/debug/secure_server.py',Path(__file__),wasm]
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}
    report=dict(scope='actual Linux VM + broker + DAP scene metadata, no graphical IDE/native ROS claim',
                pins=pins,cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max='68719476736',swap_max='0',
                events_before=(GROUP/'memory.events').read_text(),cases=[],complete=False)
    for permission in ('full','read-only','limited'):
        folder=out/permission;folder.mkdir();server=None
        record=dict(permission=permission,complete=False);report['cases'].append(record)
        try:
            argv=['--runtime-debug','--debug-permissions',permission,'--runtime-compile-threads','0',
                  '--runtime-llvm-jit-cache-path','disable',*flags,'--run',str(wasm)]
            server=BrokerSession(args.runtime.resolve(),ROOT/'tools/debug/secure_server.py',ROOT/'tools/debug/dap_adapter.py',argv,folder)
            record['broker_argv']=server.command
            client=server.connect('wasm')
            record['permissions']=client.request('uwvm/permissions')['body']
            record['startup']=client.request('uwvm/diagnostics')['body']
            assert record['permissions']['permissions']==permission
            if permission=='limited':
                reply=client.request('uwvm/sceneState',{},success=False)
                assert 'body' not in reply
                record['refusal']=reply
            else:
                # A prepared entry has no identified Wasm participant yet;
                # configuration metadata is not a live cooperative snapshot.
                initial=client.request('uwvm/sceneState',{'selection':'authority'},success=False)
                assert 'body' not in initial
                record['initial_unstarted_refusal']=initial
                if permission=='full':
                    point=client.evaluate('breakwasm 0 3 0')
                    match=re.search(rb'breakpoint ([0-9]+)',point);assert match,point
                    client.evaluate('continue')
                    stopped=client.evaluate('wait')
                    assert b'stopped: breakpoint' in stopped,stopped
                    client.evaluate('delete '+match[1].decode())
                    authority=client.request('uwvm/sceneState',{'selection':'authority','environment':'robot','alias':'signal'})['body']
                    resources=client.request('uwvm/sceneState',{'selection':'resources'})['body']
                    assert authority['rows'][0]['open_handles']=='1' and authority['rows'][0]['admissions']=='2'
                    assert len(resources['rows'])==1 and resources['rows'][0]['type']['text']=='guard'
                    assert resources['rows'][0]['sequence']=='1' and not resources['nativeOperationsIssued'] and not resources['payloadsRead']
                    again=client.request('uwvm/sceneState',{'selection':'resources','ifStop':resources['stopId']})['body']
                    assert again==resources,'metadata inspection changed resources'
                    repeated=client.request('uwvm/sceneState',{'selection':'authority','environment':'robot','alias':'signal'})['body']
                    assert repeated==authority,'metadata inspection admitted calls or refreshed authority'
                    record.update(authority=authority,resources=resources)
                    thread=re.search(rb'thread ([0-9]+)',stopped);assert thread
                    client.evaluate('step wasm '+thread[1].decode())
                    stale=client.request('uwvm/sceneState',{'ifStop':resources['stopId']},success=False)
                    assert 'body' not in stale;record['stale']=stale
                else:
                    denied=client.request('continue',{},success=False)
                    assert 'body' not in denied;record['execution_refusal']=denied
                forbidden=client.request('uwvm/sceneState',{'selection':'revoke'},success=False)
                assert 'body' not in forbidden
            record['complete']=True
        finally:
            failed=sys.exc_info()[0] is not None
            try:
                if server is not None:record['cleanup']=server.close()
            except BaseException as error:
                record['cleanup_error']=repr(error)
                if not failed:raise
            finally:
                report['events_after']=(GROUP/'memory.events').read_text()
                (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    assert pins=={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in paths}
    report['complete']=True;(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    print('PASS actual scene DAP full/read-only/limited, metadata coherence and stale-stop rejection')


if __name__=='__main__':main()
