#!/usr/bin/env python3
"""Qualify aggregate buffer debugger metadata against actual VM, broker and DAP.

The guest builds writable/sealed/closed resources across eleven private aliases.
Debugger pages observe only counters and never schema, digests, bytes or native
pointers. Repeated reads cannot charge authority, seal, pin or read buffers.
This is a headless actual DAP check, not a graphical editor acceptance run.
All subprocesses inherit the existing 64 GiB zero-swap cgroup.
"""
import argparse,json,re,resource,subprocess,sys,time
from pathlib import Path
sys.dont_write_bytecode=True
from run_scene_buffers import BufferGuest,OPERATIONS,layout,plan,PAYLOAD,PAYLOAD_SHA,u64,blob
import run_scene_vm as base
ROOT,GROUP=base.ROOT,base.GATE
sys.path.insert(0,str(ROOT/'test/0018.debugger'))
from run_dap_current_broker import BrokerSession
ALIASES=['buffer'+str(n) for n in range(11)]
def guest(fault):
    g=BufferGuest(OPERATIONS);g.check('call $abi',2);expected={}
    for n,alias in enumerate(ALIASES):
        cap=g.open(alias);count=1;objects=1;g.raw('buffer.create',cap,layout());handle=g.save(alias+'-object')
        if n%3:
            g.raw('buffer.write',cap,u64(0)+u64(0)+blob(PAYLOAD),[(0,handle)]);count+=1
        if n%3==2:
            g.raw('buffer.seal',cap,u64(0)+blob(PAYLOAD_SHA),[(0,handle)]);count+=1
        if n%2:
            g.raw('buffer.create',cap,layout());objects+=1;count+=1
        if n==8:
            g.call('resource.close',cap,[handle]);count+=1;objects-=1
        expected[alias]=dict(members=str(objects),open_handles=str(objects),reserved_bytes=str(8*objects),admissions=str(count))
    g.body.append('unreachable' if fault else 'call $spin');spin_index=3+len(OPERATIONS)+len(g.checks)
    wat=g.render().replace('\n(func (export "_start")','\n(func $spin (export "spin") (loop $again br $again))\n(func (export "_start")',1)
    return wat,spin_index,expected
def flags():
    result=['--wasm-set-main-module-name','main',*base.policy(OPERATIONS),'--ros-scene-buffer-budget','robot','704','22','352','22']
    for alias in ALIASES:
        result+=['--ros-scene-grant','robot',alias,'buffer','frames-'+alias,'sealed-buffer',','.join(op['name'] for op in OPERATIONS),*plan(alias)]
    return result
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--mode',choices=['prepare','execute','all'],default='all');p.add_argument('--source-root',type=Path,default=ROOT)
    args=p.parse_args();base.verify_gate();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    if args.mode!='prepare' and args.runtime is None:p.error('--runtime is required')
    out=args.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=True)
    fixture_paths=[Path(__file__).resolve(),Path(__file__).with_name('run_scene_buffers.py'),Path(__file__).with_name('run_scene_vm.py'),
                   ROOT/'tools/debug/dap_adapter.py',ROOT/'tools/debug/secure_server.py',ROOT/'test/0018.debugger/run_dap_current_broker.py']
    pins=lambda:{str(path):base.sha(path) for path in fixture_paths}
    if args.mode=='execute':report=json.loads((out/'results.json').read_text());assert report['prepared'] and report['pins']==pins()
    else:
        assert not (out/'results.json').exists()
        report=dict(scope=__doc__,pins=pins(),cgroup=str(GROUP),cgroup_inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,
            events_before=(GROUP/'memory.events').read_text(),cases=[],guests={},complete=False,prepared=False)
    def save():report['events_after']=(GROUP/'memory.events').read_text();(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    save()
    if args.mode!='execute':
        for name,fault in [('breakpoint',False),('fault',True)]:
            source,spin_index,expected=guest(fault);wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(source)
            subprocess.run(['wat2wasm','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True)
            report['guests'][name]=dict(wat_sha256=base.sha(wat),sha256=base.sha(wasm),spin_index=spin_index)
        report['expected']=expected;report['prepared']=True;save()
    if args.mode=='prepare':print('PASS buffer DAP guest preparation');return
    production=args.source_root.resolve();report['source_root']=str(production);report['runtime']=str(args.runtime.resolve());report['runtime_sha256']=base.sha(args.runtime)
    report['production_pins']={str(production/path):base.sha(production/path) for path in
        ['src/uwvm2/imported/ros/scene/debug.h','src/uwvm2/imported/ros/scene/deployment.h','src/uwvm2/imported/ros/scene/buffers.h','src/uwvm2/uwvm/debugger/scene_state.h']}
    for name,item in report['guests'].items():assert base.sha(out/(name+'.wasm'))==item['sha256'] and base.sha(out/(name+'.wat'))==item['wat_sha256']
    try:
        for permission in ['full','read-only','limited']:
            folder=out/permission;folder.mkdir();server=None;record=dict(permission=permission,complete=False);report['cases'].append(record)
            try:
                name='breakpoint' if permission=='full' else 'fault'
                launch=['--runtime-debug'] if permission=='full' else ['--runtime-aot','--debug-fault-action','pause']
                argv=[*launch,'--debug-permissions',permission,'--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags(),'--run',str(out/(name+'.wasm'))]
                server=BrokerSession(args.runtime.resolve(),ROOT/'tools/debug/secure_server.py',ROOT/'tools/debug/dap_adapter.py',argv,folder)
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
                    record['refusal']=client.request('uwvm/sceneState',{'selection':'resources'},success=False);assert 'body' not in record['refusal']
                else:
                    views={}
                    for selection in ['resources','authority']:
                        first=client.request('uwvm/sceneState',{'selection':selection,'environment':'robot'})['body'];assert first['total']==11 and len(first['rows'])==8 and first['more']
                        second=client.request('uwvm/sceneState',{'selection':selection,'environment':'robot','start':8,'ifStop':first['stopId']})['body']
                        assert len(second['rows'])==3 and not second['more'];views[selection]=[first,second]
                    rows=[row for page in views['resources'] for row in page['rows']]
                    for row in rows:
                        alias=row['alias']['text'];wanted=report['expected'][alias]
                        assert row['type']['text']=='buffer' and row['binding']['text']=='sealed-buffer' and row['id']=='0'
                        assert row['resource']['text']=='frames-'+alias and row['state']['text']==''
                        for key in ['members','open_handles','reserved_bytes']:assert row[key]==wanted[key],(alias,key,row[key],wanted[key])
                        assert all(row[key]=='0' for key in ['lease_count','workspace_bytes','size_bytes','accepted_bytes','retained_receipts','generation','transition','deadline_ns','status_code'])
                        assert not any(row[key] for key in ['immutable','published','durable','callback_pending','callback_delivered','retired'])
                    for page in views['authority']:
                        for row in page['rows']:
                            assert row['open_handles']=='1';assert row['admissions']==report['expected'][row['alias']['text']]['admissions']
                    for selection,pages in views.items():
                        for page in pages:
                            assert page['payloadsRead'] is False and page['nativeOperationsIssued'] is False
                            for row in page['rows']:assert not set(row)&{'schema','sha256','digest','content','payload','memoryReference','native_pointer','buffer_id'}
                            for _ in range(3):
                                repeated=client.request('uwvm/sceneState',{'selection':selection,'environment':'robot','start':page['start'],'ifStop':page['stopId']})['body']
                                assert repeated==page,'inspection changed buffer/accounting state'
                    single=[client.request('uwvm/sceneState',{'selection':'resources','environment':'robot','start':i,'count':1,'ifStop':views['resources'][0]['stopId']})['body'] for i in range(11)]
                    assert [page['rows'][0] for page in single]==rows;record.update(views=views,single_row_pages=single)
                    if permission=='full':
                        thread=re.search(rb'thread ([0-9]+)',stopped);assert thread;client.evaluate('step wasm '+thread[1].decode())
                        record['stale']=client.request('uwvm/sceneState',{'ifStop':views['resources'][0]['stopId']},success=False);assert 'body' not in record['stale']
                record['complete']=True
            finally:
                failed=sys.exc_info()[0] is not None
                try:
                    if server is not None:record['cleanup']=server.close()
                except BaseException as error:
                    record['cleanup_error']=repr(error)
                    if not failed:raise
                finally:save()
        assert pins()==report['pins'] and base.sha(args.runtime)==report['runtime_sha256']
        assert all(base.sha(Path(path))==digest for path,digest in report['production_pins'].items())
        report['complete']=True
    finally:save()
    print('PASS actual buffer DAP aggregate privacy, nonmutation, permissions, pagination and stale-stop rejection')
if __name__=='__main__':main()
