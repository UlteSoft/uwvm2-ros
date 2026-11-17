#!/usr/bin/env python3
"""Observe actual isolated model jobs through the VM broker and DAP adapter.

The Wasm guest computes a real matrix result and retains its request/output.
Debugger reads must expose only aggregate reservations, never model profiles,
input bytes, result data, hashes, paths or job handles. Full and read-only
permissions are checked independently; limited inspection must be refused.
This is a headless DAP test, not graphical IDE acceptance.
"""
import argparse,json,os,re,resource,subprocess,sys,time
from pathlib import Path
sys.dont_write_bytecode=True
import run_scene_models as models
from run_scene_vm import ROOT,GATE,sha,verify_gate,policy
from run_scene_buffers import BufferGuest,u64,blob,layout

def flags(images):
    result=models.configured(images);groups=[];current=[]
    for value in result:
        if value.startswith('--'):
            if current:groups.append(current)
            current=[value]
        else:current.append(value)
    groups.append(current)
    return [value for group in groups if not(any(alias in group for alias in ['peer','rate','denied'])) for value in group]

def guest(fault):
    g=BufferGuest(models.OPERATIONS);cap=g.open('brain');data=g.open('data');steady=g.open('steady')
    g.call('clock.create',steady,[0]);clock=g.save('clock')
    encoded=layout(identity='tensor',digest=models.PROFILE_SHA,shape=(16,),size=16)
    payload=models.struct.pack('<2d',5,6);g.raw('buffer.create',data,encoded);buffer=g.save('buffer')
    g.raw('buffer.write',data,u64(0)+u64(0)+blob(payload),[(0,buffer)])
    g.raw('buffer.seal',data,u64(0)+blob(models.hashlib.sha256(payload).digest()),[(0,buffer)])
    g.raw('model.open',cap,models.profile('matrix.cpu'));session=g.save('session')
    g.call('clock.read',steady,[clock],output=768,length=1104);g.globals.add('deadline');g.body.append(g.word(768)+' i64.const 5000000000 i64.add global.set $deadline')
    body=u64(0)+u64(1)+u64(0)+models.profile('matrix.multiply')+models.text('mode')+models.text('bytes')+blob(u64(1))+u64(1)
    patches=[(0,session),(16,session),(len(body),data),(len(body)+8,buffer)];body+=bytes(48)+u64(32)+u64(2)+u64(8192)+u64(0)
    patches.append((len(body)-8,'global.get $deadline'));g.raw('model.submit',cap,body,patches);request=g.save('request')
    g.body.append('block $done loop $waiting');g.raw('request.describe',cap,u64(0),[(0,request)])
    g.body.append(g.word(520)+' i64.const 9 i64.eq br_if $done')
    g.call('clock.read',steady,[clock],output=768,length=1104);g.check(g.word(768)+' global.get $deadline i64.ge_u');g.body.append('br $waiting end end')
    g.exact(520,models.text('succeeded'));g.body.append('unreachable' if fault else 'call $spin')
    spin_index=3+len(models.OPERATIONS)+len(g.checks)
    wat=g.render().replace('\n(func (export "_start")','\n(func $spin (export "spin") (loop $again br $again))\n(func (export "_start")',1)
    return wat,spin_index

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',required=True,type=Path);p.add_argument('--source-root',required=True,type=Path)
    p.add_argument('--images',required=True,type=Path);p.add_argument('--output',required=True,type=Path);args=p.parse_args();verify_gate()
    assert os.sched_getaffinity(0).issubset({12,13,14,15});resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
    runtime=args.runtime.resolve();source=args.source_root.resolve();images=args.images.resolve()
    sys.path.insert(0,str(source/'test/0018.debugger'))
    from run_dap_current_broker import BrokerSession
    files=[Path(__file__).resolve(),Path(models.__file__).resolve(),Path(__file__).with_name('run_scene_buffers.py'),Path(__file__).with_name('run_scene_vm.py'),
        source/'tools/debug/dap_adapter.py',source/'tools/debug/secure_server.py',source/'test/0018.debugger/run_dap_current_broker.py',
        source/'test/0017.runtime/run_debug_source_step_cli.py',source/'test/0017.runtime/run_debug_source_inline_metadata_cli.py',
        source/'debug-tool-manifest.json',runtime,source/'source-manifest.json',
        *[images/name for name,_,_ in models.IMAGES],images/'bwrap']
    pins=lambda:{str(path):sha(path) for path in files};report=dict(complete=False,scope=__doc__,inputs=pins(),cgroup=str(GATE),inode=GATE.stat().st_ino,
        memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),guests={},cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    for name,fault in [('breakpoint',False),('fault',True)]:
        wat,spin=guest(fault);path=out/(name+'.wat');image=out/(name+'.wasm');path.write_text(wat)
        subprocess.run(['wat2wasm','--enable-multi-memory','--debug-names',str(path),'-o',str(image)],check=True)
        report['guests'][name]=dict(wat_sha256=sha(path),sha256=sha(image),spin_index=spin)
    save()
    try:
        for permission in ['full','read-only','limited']:
            folder=out/permission;folder.mkdir();server=None;record=dict(permission=permission,complete=False);report['cases'].append(record)
            try:
                name='breakpoint' if permission=='full' else 'fault'
                launch=['--runtime-debug'] if permission=='full' else ['--runtime-aot','--debug-fault-action','pause']
                argv=[*launch,'--wasm-set-main-module-name','main','--debug-permissions',permission,'--runtime-compile-threads','0',
                    '--runtime-llvm-jit-cache-path','disable',*flags(images),'--run',str(out/(name+'.wasm'))]
                server=BrokerSession(runtime,source/'tools/debug/secure_server.py',source/'tools/debug/dap_adapter.py',argv,folder)
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
                    record['refusal']=client.request('uwvm/sceneState',{'selection':'resources','environment':'robot','alias':'brain'},success=False)
                    assert 'body' not in record['refusal']
                else:
                    views={}
                    for selection in ['resources','authority']:
                        query={'selection':selection,'environment':'robot','alias':'brain'}
                        page=client.request('uwvm/sceneState',query)['body'];assert page['total']==1 and len(page['rows'])==1 and not page['more']
                        assert page['payloadsRead'] is False and page['nativeOperationsIssued'] is False
                        for _ in range(3):assert client.request('uwvm/sceneState',{**query,'ifStop':page['stopId']})['body']==page
                        views[selection]=page
                    row=views['resources']['rows'][0]
                    assert row['type']['text']=='model' and row['binding']['text']=='local-model' and row['id']=='0'
                    # Confirmed native completion releases source leases; the
                    # guest still owns its retained receipt/result reservation.
                    for key,value in dict(members='1',open_handles='1',input_bytes='0',output_bytes='32',output_items='2',workspace_bytes='8192').items():
                        assert row[key]==value,(key,row[key],value)
                    assert not set(row)&{'schema','sha256','digest','content','payload','memoryReference','native_pointer','buffer_id','request_id','model','task','parameters','path'}
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
    print('PASS actual model DAP privacy, reservations, nonmutation, permissions and stale-stop rejection')
if __name__=='__main__':main()
