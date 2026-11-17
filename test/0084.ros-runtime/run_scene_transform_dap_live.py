#!/usr/bin/env python3
"""Inspect cached TF queries through actual VM, broker and DAP permissions.

A Wasm guest leaves a pending query with a real 50ms steady deadline. After
stopping and waiting past that deadline, repeated inspection must retain the
cached pending count. Inspection never calls lookup/describe, expires queries,
publishes transforms or exports frame/query payloads. The headless editor
transport check is separate from graphical IDE acceptance.
"""
import argparse,json,os,re,resource,subprocess,sys,time
from pathlib import Path
sys.dont_write_bytecode=True
import run_scene_transforms as fixture
from run_scene_vm import ROOT,GATE,sha,verify_gate

def guest(fault):
    g=fixture.TransformGuest(fixture.OPERATIONS);cap=g.open('frames');clock_cap=g.open('clock');steady_cap=g.open('steady')
    g.call('clock.create',clock_cap,[2]);clock=g.save('ros-clock')
    g.call('clock.update',clock_cap,[clock_cap,clock,1000000000,1])
    g.call('clock.create',steady_cap,[0]);steady=g.save('steady-clock')
    body=fixture.text('frames')+fixture.blob(fixture.DIGEST)+bytes(16)
    g.raw('transform.open',cap,body,[(len(body)-16,clock_cap),(len(body)-8,clock)]);session=g.save('session')
    g.call('clock.read',steady_cap,[steady],output=3072,length=3200);g.globals.add('deadline')
    g.body.append(g.word(3072)+' i64.const 50000000 i64.add global.set $deadline')
    body=fixture.u64(0)+fixture.u64(1)+fixture.u64(0)+fixture.text('map')+fixture.u64(1);at=len(body)
    body+=fixture.u64(0)+fixture.u64(1)+fixture.u64(250000000)+fixture.text('base')+fixture.u64(0)+fixture.u64(0)+fixture.u64(0)
    g.raw('transform.submit',cap,body,[(0,session),(16,session),(at,clock),(len(body)-8,'global.get $deadline')]);g.exact(520,fixture.text('queued'))
    g.body.append('unreachable' if fault else 'call $spin');spin=3+len(fixture.OPERATIONS)+len(g.checks)
    wat=g.render().replace('\n(func (export "_start")','\n(func $spin (export "spin") (loop $again br $again))\n(func (export "_start")',1)
    return wat,spin

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',required=True,type=Path);p.add_argument('--source-root',required=True,type=Path)
    p.add_argument('--debug-tools',required=True,type=Path);p.add_argument('--output',required=True,type=Path);args=p.parse_args();verify_gate()
    assert os.sched_getaffinity(0).issubset({12,13,14,15});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    runtime=args.runtime.resolve();source=args.source_root.resolve();debug=args.debug_tools.resolve()
    sys.path.insert(0,str(debug/'test/0018.debugger'));from run_dap_current_broker import BrokerSession
    manifest=json.loads((debug/'source-manifest.json').read_text());assert all(sha(debug/name)==digest for name,digest in manifest.items())
    files=[Path(__file__).resolve(),Path(fixture.__file__).resolve(),Path(__file__).with_name('run_scene_vm.py'),Path(__file__).with_name('run_scene_fp.py'),
           Path(__file__).with_name('run_scene_memtag.py'),runtime,runtime.parent/'full-build-results.json',source/'source-manifest.json',debug/'source-manifest.json',
           *[debug/name for name in manifest]]
    build=json.loads((runtime.parent/'full-build-results.json').read_text());assert build['complete'] and build['binary_sha256']==sha(runtime) and build['source_root']==str(source)
    pins=lambda:{str(path):sha(path) for path in files};report=dict(complete=False,scope=__doc__,inputs=pins(),cgroup=str(GATE),inode=GATE.stat().st_ino,
        memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests={},cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    for name,fault in [('breakpoint',False),('fault',True)]:
        wat,spin=guest(fault);path=out/(name+'.wat');image=out/(name+'.wasm');path.write_text(wat)
        cmd=['wat2wasm','--enable-multi-memory','--debug-names',str(path),'-o',str(image)];subprocess.run(cmd,check=True)
        report['guests'][name]=dict(compile=cmd,wat_sha256=sha(path),sha256=sha(image),spin_index=spin)
    save()
    try:
        for permission in ['full','read-only','limited']:
            folder=out/permission;folder.mkdir();server=None;record=dict(permission=permission,complete=False);report['cases'].append(record)
            try:
                name='breakpoint' if permission=='full' else 'fault'
                launch=['--runtime-debug'] if permission=='full' else ['--runtime-aot','--debug-fault-action','pause']
                argv=[*launch,'--wasm-set-main-module-name','main','--debug-permissions',permission,'--runtime-compile-threads','0',
                    '--runtime-llvm-jit-cache-path','disable',*fixture.configured(),'--run',str(out/(name+'.wasm'))]
                server=BrokerSession(runtime,debug/'tools/debug/secure_server.py',debug/'tools/debug/dap_adapter.py',argv,folder)
                record['broker_argv']=server.command;client=server.connect('wasm');record['permissions']=client.request('uwvm/permissions')['body']
                assert record['permissions']['permissions']==permission
                if permission=='full':
                    point=client.evaluate(f'breakwasm 0 {report["guests"][name]["spin_index"]} 0');match=re.search(rb'breakpoint ([0-9]+)',point);assert match,point
                    client.evaluate('continue');stopped=client.evaluate('wait');assert b'stopped: breakpoint' in stopped,stopped;client.evaluate('delete '+match[1].decode())
                else:
                    deadline=time.monotonic()+30
                    while True:
                        stopped=client.evaluate('status')
                        ready=(re.fullmatch(rb'limited status=2 stop-id [1-9][0-9]* participants=[0-9]+ reason=9 exit-status=0\n',stopped)
                               if permission=='limited' else re.search(rb'(?m)^stop-id [1-9][0-9]*$',stopped))
                        if ready:break
                        assert time.monotonic()<deadline,stopped;time.sleep(.02)
                    record['execution_refusal']=client.request('continue',{},success=False)
                record['stop']=stopped.decode(errors='replace')
                # This is an actual deadline-crossing test, not a scheduling wait.
                waited=time.monotonic();time.sleep(.1);record['stopped_wait_seconds']=time.monotonic()-waited
                assert record['stopped_wait_seconds']>.05
                if permission=='limited':
                    record['refusal']=client.request('uwvm/sceneState',{'selection':'resources','environment':'robot','alias':'frames'},success=False)
                    assert 'body' not in record['refusal']
                else:
                    views={}
                    for selection in ['resources','authority']:
                        query={'selection':selection,'environment':'robot','alias':'frames'}
                        page=client.request('uwvm/sceneState',query)['body'];assert page['total']==1 and len(page['rows'])==1 and not page['more']
                        assert page['payloadsRead'] is False and page['nativeOperationsIssued'] is False
                        for _ in range(3):assert client.request('uwvm/sceneState',{**query,'ifStop':page['stopId']})['body']==page
                        views[selection]=page
                    row=views['resources']['rows'][0]
                    assert row['type']['text']=='transform' and row['binding']['text']=='local-transform' and row['id']=='0'
                    for key,value in dict(members='1',open_handles='1',sample_count='0',query_count='1',pending_queries='1',carrier_bytes='0').items():assert row[key]==value,(key,row[key],value)
                    for key in ['reserved_bytes','workspace_bytes','query_bytes']:assert 0<int(row[key])<1000000
                    for key in ['input_bytes','output_bytes','output_items','lease_count','generation','deadline_ns','sequence']:assert row[key]=='0'
                    assert not set(row)&{'target','source','frame','timestamp','epoch','query_id','translation','quaternion','content','payload','memoryReference','profile_digest'}
                    record['views']=views
                    if permission=='full':
                        thread=re.search(rb'thread ([0-9]+)',stopped);assert thread;client.evaluate('step wasm '+thread[1].decode())
                        record['stale']=client.request('uwvm/sceneState',{'ifStop':views['resources']['stopId']},success=False);assert 'body' not in record['stale']
                record['complete']=True
            finally:
                if server is not None:record['cleanup']=server.close()
                save()
        assert pins()==report['inputs'];report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
    print('PASS actual transform DAP cached deadline, reservations, privacy, permissions and stale-stop rejection')
if __name__=='__main__':main()
