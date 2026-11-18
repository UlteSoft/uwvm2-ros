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
 (memory 1) (data (i32.const 0) "signal") (data (i32.const 16) "denied")
 (func $spin (export "spin") (loop $again br $again))
 (func (export "_start")
  i32.const 0 i64.const 16 i64.const 6 i64.const 64 call $open i32.const 13 i32.ne if unreachable end
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
    # Permit one metadata record and exercise end-of-stream losses at a real stop.
    flags+=['--ros-scene-call-audit','global','*','all','1','8192','1000','1']
    for action in ['capability.open','guard.create','guard.trigger']:
        flags+=['--ros-scene-interface','robot',action,'allow']
    flags+=['--ros-scene-grant','robot','signal','guard','signal','local','guard.create,guard.trigger,resource.observe']
    paths=[args.runtime.resolve(),ROOT/'tools/debug/dap_adapter.py',ROOT/'tools/debug/export_host_trace.py',ROOT/'tools/debug/host_trace_view.py',ROOT/'tools/debug/secure_server.py',Path(__file__),wasm]
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
            server=BrokerSession(args.runtime.resolve(),ROOT/'tools/debug/secure_server.py',ROOT/'tools/debug/dap_adapter.py',argv,folder,
                                 socket_parent=next(p for p in out.parents if p.name=='.tmp'))
            record['broker_argv']=server.command
            client=server.connect('wasm')
            record['permissions']=client.request('uwvm/permissions')['body']
            record['startup']=client.request('uwvm/diagnostics')['body']
            assert record['permissions']['permissions']==permission
            if permission=='limited':
                reply=client.request('uwvm/sceneState',{},success=False)
                assert 'body' not in reply
                assert 'body' not in client.request('uwvm/sceneState',{'selection':'audit'},success=False)
                record['refusal']=reply
            else:
                # A prepared entry has no identified Wasm participant yet;
                # configuration metadata is not a live cooperative snapshot.
                initial=client.request('uwvm/sceneState',{'selection':'authority'},success=False)
                assert 'body' not in initial
                record['initial_unstarted_refusal']=initial
                if permission=='full':
                    # Arm recording at the guest's _start before its first host call.
                    entry=client.evaluate('breakwasm 0 4 0')
                    entry_id=re.search(rb'breakpoint ([0-9]+)',entry);assert entry_id,entry
                    client.evaluate('continue')
                    assert b'stopped: breakpoint' in client.evaluate('wait')
                    client.evaluate('delete '+entry_id[1].decode())
                    initial_calls=client.request('uwvm/hostTrace',{'family':'rosv2'})['body']
                    assert not initial_calls['calls']
                    assert b'host-trace enabled=8' in client.evaluate('trace host on family=rosv2')
                    point=client.evaluate('breakwasm 0 3 0')
                    match=re.search(rb'breakpoint ([0-9]+)',point);assert match,point
                    client.evaluate('continue')
                    stopped=client.evaluate('wait')
                    assert b'stopped: breakpoint' in stopped,stopped
                    client.evaluate('delete '+match[1].decode())
                    totals=client.request('uwvm/sceneState',{'selection':'audit'})['body']
                    row=totals['rows'][0]
                    assert row['configured'] and row['admitted']=='1' and row['limited']=='3'
                    assert row['busy']=='0' and row['clock_failed']=='0' and int(row['sink_failed'])<=1
                    assert totals==client.request('uwvm/sceneState',{'selection':'audit','ifStop':totals['stopId']})['body']
                    record['audit_counters']=totals
                    authority=client.request('uwvm/sceneState',{'selection':'authority','environment':'robot','alias':'signal'})['body']
                    resources=client.request('uwvm/sceneState',{'selection':'resources'})['body']
                    assert authority['rows'][0]['open_handles']=='1' and authority['rows'][0]['admissions']=='2'
                    assert len(resources['rows'])==1 and resources['rows'][0]['type']['text']=='guard'
                    assert resources['rows'][0]['sequence']=='1' and not resources['nativeOperationsIssued'] and not resources['payloadsRead']
                    again=client.request('uwvm/sceneState',{'selection':'resources','ifStop':resources['stopId']})['body']
                    assert again==resources,'metadata inspection changed resources'
                    repeated=client.request('uwvm/sceneState',{'selection':'authority','environment':'robot','alias':'signal'})['body']
                    assert repeated==authority,'metadata inspection admitted calls or refreshed authority'
                    calls=client.request('uwvm/hostTrace',{'family':'rosv2'})['body']
                    assert calls['stopId']==resources['stopId'] and calls['journal']['rosv2-dropped']=='0'
                    assert [r['function'] for r in calls['calls']]==['capability.open','capability.open','guard.create','guard.trigger']
                    assert [r['result'] for r in calls['calls']]==['13','0','0','0']
                    assert all(r['namespace']=='uwvm_ros_v2' and r['family']=='rosv2'
                               and r['args']==[] and r['segments']=='0' for r in calls['calls'])
                    assert all(r['moduleName']['utf8IfValid']=='main' and int(r['generation'])>0 for r in calls['calls'])
                    selected={'family':'rosv2','module':'main','function':'capability.open',
                              'namespace':'uwvm_ros_v2','result':'13','ifStop':calls['stopId']}
                    rejected=client.request('uwvm/hostTrace',selected)['body']
                    assert rejected['calls']==calls['calls'][:1]
                    assert client.request('uwvm/hostTrace',{**selected,'module':'other'})['body']['calls']==[]
                    assert client.request('uwvm/hostTrace',{**selected,'after':rejected['next']})['body']['calls']==[]
                    assert 'body' not in client.request('uwvm/hostTrace',{**selected,'alias':'signal'},success=False)
                    filtered_archive=folder/'denied-calls.json'
                    client.request('uwvm/exportHostTrace',{**selected,'path':str(filtered_archive)})
                    binding=json.loads(filtered_archive.read_text())
                    assert binding['summary']['calls']=='1' and binding['calls'][0]['resultUnsigned']=='13'
                    saved=binding['calls'][0]; live=rejected['calls'][0]
                    assert 'complete verified caller label' in binding['semantics']['rosv2']
                    assert saved['generation']==live['generation'] and saved['moduleName']['byteLength']=='4'
                    assert saved['moduleName']['chunks']==[{'offset':'0',**live['moduleName']}]
                    archive=folder/'scene-calls.json'
                    client.request('uwvm/exportHostTrace',{'family':'rosv2','path':str(archive)})
                    exported=json.loads(archive.read_text())
                    assert exported['summary']['calls']=='4' and exported['summary']['payloadBytes']=='0'
                    record.update(authority=authority,resources=resources,host_completions=calls,denied_completions=rejected)
                    thread=re.search(rb'thread ([0-9]+)',stopped);assert thread
                    client.evaluate('step wasm '+thread[1].decode())
                    stale=client.request('uwvm/sceneState',{'ifStop':resources['stopId']},success=False)
                    assert 'body' not in stale;record['stale']=stale
                    assert 'body' not in client.request('uwvm/hostTrace',selected,success=False)
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
