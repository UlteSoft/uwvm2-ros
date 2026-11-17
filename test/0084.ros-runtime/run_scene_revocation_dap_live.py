#!/usr/bin/env python3
"""Inspect guest self-revocation through the actual VM, authenticated broker and DAP.

Checks cached authority metadata, independent debugger permissions and stale-stop
rejection. Local retirement notification is not a physical/native stop claim.
"""
import argparse,json,os,re,resource,subprocess,sys,time
from pathlib import Path
sys.dont_write_bytecode=True
import run_scene_revocation as fixture
from run_scene_vm import ROOT,GATE,sha,verify_gate

def guest(fault):
    g=fixture.Guest(fixture.OPERATIONS);cap=g.open('signal');zero=g.open('zero')
    g.call('guard.create',cap)
    g.call('capability.revoke',cap,[1,cap]);g.call('capability.revoke',cap,[1,cap])
    g.call('capability.revoke',zero,[1,zero]);g.call('capability.revoke',zero,[1,zero])
    g.body.append('unreachable' if fault else 'call $spin');spin=3+len(fixture.OPERATIONS)+len(g.checks)
    wat=g.render().replace('\n(func (export "_start")','\n(func $spin (export "spin") (loop $again br $again))\n(func (export "_start")',1)
    return wat,spin

def configured():return fixture.configured()

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
                if permission=='limited':
                    record['refusal']=client.request('uwvm/sceneState',{'selection':'authority','environment':'robot','alias':'signal'},success=False)
                    assert 'body' not in record['refusal']
                else:
                    views={}
                    for alias,admissions in [('signal',2),('zero',1)]:
                        query={'selection':'authority','environment':'robot','alias':alias}
                        page=client.request('uwvm/sceneState',query)['body'];assert page['total']==1 and len(page['rows'])==1 and not page['more']
                        assert page['payloadsRead'] is False and page['nativeOperationsIssued'] is False
                        row=page['rows'][0];assert row['revoked'] is True and row['admissions']==str(admissions)
                        assert row['attenuated'] is (alias=='zero')
                        for _ in range(3):assert client.request('uwvm/sceneState',{**query,'ifStop':page['stopId']})['body']==page
                        views[alias]=page
                    record['views']=views
                    if permission=='full':
                        thread=re.search(rb'thread ([0-9]+)',stopped);assert thread;client.evaluate('step wasm '+thread[1].decode())
                        record['stale']=client.request('uwvm/sceneState',{'ifStop':views['signal']['stopId']},success=False);assert 'body' not in record['stale']
                record['complete']=True
            finally:
                if server is not None:record['cleanup']=server.close()
                save()
        assert pins()==report['inputs'];report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
    print('PASS actual revocation DAP cached authority, unchanged replay count, permissions and stale-stop rejection')
if __name__=='__main__':main()
