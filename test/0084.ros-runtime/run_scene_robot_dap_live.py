#!/usr/bin/env python3
"""Qualify all five robot provider DTOs through the actual VM, broker and DAP.

Only configuration and capability opening occur. No device command, model or
component code, native ROS node, or bridge transfer is started. Real media and
process execution are qualified by the separate robot runners. Anonymous PTYs
and sensor pipes are created by a pinned trusted test launcher, then inherited
by the exact qualified VM. Debug reads must disclose no payload or native path,
change no authority accounting, and respect full/read-only/limited permissions.
"""
import argparse,json,os,re,resource,subprocess,sys,time
from pathlib import Path
import run_scene_robot_local as local
from run_scene_vm import ROOT,GATE,verify_gate,sha
sys.path.insert(0,str(ROOT/'test/0018.debugger'))
from run_dap_current_broker import BrokerSession
EXPECTED={'camera':('media','idle'),'relay':('bridge','idle'),'component':('component','prepared'),'drive':('device','prepared'),'controller':('control','prepared')}
def merge(groups):
    result=[];seen={}
    for group in groups:
        starts=[i for i,t in enumerate(group)if t.startswith('--')]+[len(group)]
        for start,end in zip(starts,starts[1:]):
            row=group[start:end];option=row[0]
            if option=='--ros-scene-check':continue
            if option=='--ros-scene-buffer-budget':continue
            key=tuple(row[:3])if option in ['--ros-scene-grant','--ros-scene-interface']else tuple(row)
            if key in seen:
                assert seen[key]==row,(seen[key],row)
                continue
            seen[key]=row;result+=row
    at=result.index('--ros-scene-grant')
    result[at:at]=['--ros-scene-buffer-budget','robot','128','32','512','128']
    return result

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path,required=True);p.add_argument('--source-root',type=Path,required=True)
    p.add_argument('--process-fixture',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--debug-tools-root',type=Path,help='Frozen repository root containing the matching debugger scripts');a=p.parse_args()
    verify_gate();resource.setrlimit(resource.RLIMIT_CORE,(0,0));out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';br=json.loads(build.read_text());assert br['complete']and br['binary_sha256']==sha(runtime)and br['source_root']==str(a.source_root.resolve())
    debug_root=a.debug_tools_root.resolve() if a.debug_tools_root else ROOT
    fixture=a.process_fixture.resolve();fr=json.loads((fixture/'results.json').read_text());assert fr['prepared']
    for name in ['component-static','bridge-static']:assert next(c for c in fr['cases']if c['name']==name)['passed']
    def options(name):
        argv=next(c['argv']for c in fr['cases']if c['name']==name+'-static');return argv[argv.index('--ros-scene-create'):]
    flags=merge([local.media_flags(),local.device_flags([2147483640,2147483641,2147483642],[2147483643,2147483644,2147483645,2147483646]),options('component'),options('bridge')])
    launcher=out/'launch-fixture.py';launcher.write_text('''#!/usr/bin/python3
import os,sys,pty,tty
fds=[]
for _ in range(3):
 r,w=os.pipe2(os.O_NONBLOCK);fds.append(r);os.set_inheritable(r,True);os.set_inheritable(w,True)
for _ in range(4):
 m,s=pty.openpty();tty.setraw(s);os.set_blocking(m,False);os.set_blocking(s,False)
 os.set_inheritable(m,True);os.set_inheritable(s,True);fds.append(m)
replacement={str(2147483640+i):str(fd)for i,fd in enumerate(fds)}
args=[replacement.get(arg,arg)for arg in sys.argv[1:]]
runtime='''+repr(str(runtime))+'''
os.execv(runtime,[runtime,*args])
''');launcher.chmod(0o700)
    guests={}
    for fault in [False,True]:
        g=local.RecordingGuest(local.OPERATIONS);g.check('call $abi',2)
        for alias in EXPECTED:g.open(alias)
        g.body.append('unreachable'if fault else'call $spin');spin=3+len(local.OPERATIONS)+len(g.checks)
        wat=g.render().replace('\n(func (export "_start")','\n(func $spin (loop $again br $again))\n(func (export "_start")',1)
        name='fault'if fault else'breakpoint';source=out/(name+'.wat');source.write_text(wat);wasm=out/(name+'.wasm')
        subprocess.run(['wat2wasm','--enable-multi-memory','--debug-names',str(source),'-o',str(wasm)],check=True);guests[name]=dict(path=wasm,spin=spin)
    pinned=[Path(__file__).resolve(),Path(local.__file__).resolve(),launcher,runtime,build,fixture/'results.json',debug_root/'tools/debug/dap_adapter.py',debug_root/'tools/debug/secure_server.py',ROOT/'test/0018.debugger/run_dap_current_broker.py']
    pinned.extend(item['path']for item in guests.values());pins={str(path):sha(path)for path in pinned}
    report=dict(complete=False,scope=__doc__,pins=pins,runtime_sha256=sha(runtime),source_root=str(a.source_root.resolve()),cgroup=str(GATE),inode=GATE.stat().st_ino,memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    save()
    try:
        for permission in ['full','read-only','limited']:
            folder=out/permission;folder.mkdir();server=None;entry=dict(permission=permission,complete=False);report['cases'].append(entry)
            try:
                chosen=guests['breakpoint'if permission=='full'else'fault'];launch=['--runtime-debug']if permission=='full'else['--runtime-aot','--debug-fault-action','pause']
                argv=[*launch,'--debug-permissions',permission,'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,'--run',str(chosen['path'])]
                server=BrokerSession(launcher,debug_root/'tools/debug/secure_server.py',debug_root/'tools/debug/dap_adapter.py',argv,folder);client=server.connect('wasm');entry['broker_argv']=server.command
                assert client.request('uwvm/permissions')['body']['permissions']==permission
                if permission=='full':
                    answer=client.evaluate(f'breakwasm 0 {chosen["spin"]} 0');point=re.search(rb'breakpoint ([0-9]+)',answer);assert point,answer
                    client.evaluate('continue');stopped=client.evaluate('wait');assert b'stopped: breakpoint'in stopped,stopped;client.evaluate('delete '+point[1].decode())
                else:
                    until=time.monotonic()+30
                    while True:
                        stopped=client.evaluate('status')
                        if (b'limited status=2'in stopped if permission=='limited'else re.search(rb'(?m)^stop-id [1-9][0-9]*$',stopped)):break
                        assert time.monotonic()<until,stopped;time.sleep(.02)
                    entry['execution_refusal']=client.request('continue',{},success=False)
                entry['stop']=stopped.decode()
                if permission=='limited':entry['refusal']=client.request('uwvm/sceneState',{'selection':'resources'},success=False);assert 'body'not in entry['refusal']
                else:
                    views={}
                    for selection in ['authority','resources']:
                        pages=[];start=0
                        while True:
                            query={'selection':selection,'environment':'robot','start':start}
                            if pages:query['ifStop']=pages[0]['stopId']
                            page=client.request('uwvm/sceneState',query)['body'];assert not page['payloadsRead']and not page['nativeOperationsIssued'];pages.append(page)
                            for row in page['rows']:assert not set(row)&{'payload','path','native_pointer','request_id','request_key','pid','fd','sha256','digest','memoryReference'}
                            if not page['more']:break
                            start+=len(page['rows']);assert start<256
                        views[selection]=pages
                    rows=[row for page in views['resources']for row in page['rows']];actual={row['alias']['text']:row for row in rows if row['type']['text']in {t for t,s in EXPECTED.values()}}
                    assert set(actual)==set(EXPECTED),actual
                    for alias,(kind,state)in EXPECTED.items():
                        row=actual[alias];assert row['type']['text']==kind and row['state']['text']==state,(alias,row)
                        assert row['id']=='0'and row['open_handles']=='0'and not row['exit_observed']and not row['armed']
                    for selection,pages in views.items():
                        for page in pages:
                            for _ in range(2):assert client.request('uwvm/sceneState',{'selection':selection,'environment':'robot','start':page['start'],'ifStop':page['stopId']})['body']==page
                    entry['views']=views
                    if permission=='full':
                        thread=re.search(rb'thread ([0-9]+)',stopped);assert thread;client.evaluate('step wasm '+thread[1].decode())
                        entry['stale']=client.request('uwvm/sceneState',{'ifStop':views['resources'][0]['stopId']},success=False);assert 'body'not in entry['stale']
                entry['complete']=True
            finally:
                if server:entry['cleanup']=server.close()
                save()
        assert all(sha(Path(path))==digest for path,digest in pins.items());report['inputs_unchanged']=True;report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
    print('PASS actual robot DAP: five closed provider rows, three permissions, pagination, privacy and nonmutation')
if __name__=='__main__':main()
