#!/usr/bin/env python3
"""Inspect cached private logs and diagnostic pages through actual VM, broker and DAP permissions.

A Wasm guest leaves a partially read diagnostic snapshot with a finite 200ms
steady lifetime. After that lifetime passes, repeated inspection must retain its
cached rows without capture, expiry, drain or content access. The headless editor
transport check is separate from graphical IDE acceptance.
"""
import argparse,json,os,re,resource,subprocess,sys,time
from pathlib import Path
sys.dont_write_bytecode=True
import run_scene_log_diagnostic as fixture
from run_scene_vm import ROOT,GATE,sha,verify_gate

def guest(fault):
    g=fixture.LogGuest(fixture.OPERATIONS);cap=g.open('log');diagnostic=g.open('health')
    for sequence in [1,2]:g.raw('log.emit',cap,fixture.record(sequence=sequence,message='private robot log must never enter debug metadata'))
    body=fixture.u64(37)+fixture.u64(0)+fixture.u64(0)+fixture.u64(2)
    g.raw('diagnostic.read',diagnostic,body,[(8,cap)]);g.equal(g.word(520),1)
    g.body.append('unreachable' if fault else 'call $spin');spin=3+len(fixture.OPERATIONS)+len(g.checks)
    wat=g.render().replace('\n(func (export "_start")','\n(func $spin (export "spin") (loop $again br $again))\n(func (export "_start")',1)
    return wat,spin

def configured():
    flags=fixture.configured()
    for key,value in [('lifetime_ns','200000000'),('capture_ns','100000000')]:
        at=next(i for i in range(len(flags)-4) if flags[i:i+4]==['--ros-scene-diagnostic-limit','robot','health',key]);flags[at+4]=value
    return flags

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
                    '--runtime-llvm-jit-cache-path','disable',*configured(),'--run',str(out/(name+'.wasm'))]
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
                waited=time.monotonic();time.sleep(.3);record['stopped_wait_seconds']=time.monotonic()-waited
                assert record['stopped_wait_seconds']>.2
                if permission=='limited':
                    record['refusal']=client.request('uwvm/sceneState',{'selection':'resources','environment':'robot','alias':'health'},success=False)
                    assert 'body' not in record['refusal']
                else:
                    views={}
                    for selection in ['resources','authority']:
                        query={'selection':selection,'environment':'robot','alias':'health'}
                        page=client.request('uwvm/sceneState',query)['body'];assert page['total']==1 and len(page['rows'])==1 and not page['more']
                        assert page['payloadsRead'] is False and page['nativeOperationsIssued'] is False
                        for _ in range(3):assert client.request('uwvm/sceneState',{**query,'ifStop':page['stopId']})['body']==page
                        views[selection]=page
                    row=views['resources']['rows'][0]
                    assert row['type']['text']=='diagnostic' and row['binding']['text']=='local-diagnostic' and row['id']=='0'
                    assert row['members']=='3' and int(row['reserved_bytes'])>0 and not row['pending_page'] and not row['busy']
                    for key in ['input_bytes','output_bytes','output_items','lease_count','generation','deadline_ns','sequence','emitted_records','retained_bytes']:
                        assert row[key]=='0'
                    assert views['authority']['rows'][0]['admissions']=='1'
                    for selection in ['resources','authority']:
                        query={'selection':selection,'environment':'robot','alias':'log'}
                        page=client.request('uwvm/sceneState',query)['body'];assert page['total']==1 and len(page['rows'])==1
                        for _ in range(3):assert client.request('uwvm/sceneState',{**query,'ifStop':page['stopId']})['body']==page
                        views['log_'+selection]=page
                    row=views['log_resources']['rows'][0]
                    assert row['type']['text']=='log' and row['binding']['text']=='private-log' and row['id']=='0'
                    assert row['emitted_records']=='2' and row['members']=='2' and row['lost_records']=='0' and row['exported_records']=='0'
                    assert int(row['retained_bytes'])>0 and int(row['reserved_bytes'])>=int(row['retained_bytes'])
                    assert views['log_authority']['rows'][0]['admissions']=='3'
                    for page in views.values():
                        for item in page['rows']:assert not set(item)&{'content','payload','message','logger','cursor','revision','expires','file','function','location','counter_value'}
                    assert 'private robot log' not in json.dumps(views)
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
    print('PASS actual log/diagnostic DAP cached expiry, private counters, permissions and stale-stop rejection')
if __name__=='__main__':main()
