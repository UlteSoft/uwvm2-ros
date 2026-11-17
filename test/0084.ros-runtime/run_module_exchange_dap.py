#!/usr/bin/env python3
"""Observe a retained cross-Wasm delivery through the actual VM/broker/DAP.

Full and read-only views contain detached route counters only. Repeated reads
must not consume messages, clear guest bytes or change admission accounting.
Limited inspection is denied. All commands inherit the existing 64 GiB gate.
"""
import argparse,base64,json,re,resource,struct,subprocess,sys,time
from pathlib import Path
from run_scene_vm import ROOT,GATE,sha,verify_gate
from run_module_security_vm import Fixture,CALLS
sys.path.insert(0,str(ROOT/'test/0018.debugger'))
from run_dap_current_broker import BrokerSession

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--clear-input',choices=('default','true','false'),default='default')
    a=p.parse_args();verify_gate();resource.setrlimit(resource.RLIMIT_CORE,(0,0));out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    runtime=a.runtime.resolve();build=runtime.parent/'full-build-results.json';proof=json.loads(build.read_text());assert proof['complete'] and proof['binary_sha256']==sha(runtime)
    worker=Fixture(CALLS,shared=True);worker.globals.add('exchange')
    worker.call('module.call.take','global.get $exchange',[0,0],expected=11,output=2048,capacity=128,length=3072)
    worker.extra_functions.append('(func (export "init") (param i64) local.get 0 global.set $exchange '+'\n'.join(worker.body)+')');worker.body=[]
    worker.call('module.call.take','global.get $exchange',[0,0],output=2048,capacity=128,length=3072)
    worker.extra_functions.append('(func (export "take") '+'\n'.join(worker.body)+')');worker.body=[]
    def compile(name,text):
        wat=out/(name+'.wat');wat.write_text(text);wasm=out/(name+'.wasm')
        subprocess.run(['wat2wasm','--enable-threads','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True);return wasm
    recipient=compile('worker',worker.render(None));guests={}
    for fault in [False,True]:
        g=Fixture(CALLS,shared=True);g.extra_imports=['(import "worker" "init" (func $init (param i64)))','(import "worker" "take" (func $take))']
        cap=g.open('exchange');g.body.append(cap+' call $init')
        g.call('module.call.submit',cap,[0,8,0x123456789abcdef],size=24);g.body.append('call $take')
        g.extra_functions.append('(func $spin (loop $again br $again))');g.body.append('unreachable' if fault else 'call $spin')
        name='fault' if fault else 'breakpoint';guests[name]=(compile(name,g.render()),len(CALLS)+4+len(g.checks))
    flags=['--ros-scene-create','robot','--ros-scene-bind','robot','main','--ros-scene-bind','robot','worker']
    for name in ['capability.open',*CALLS]:flags+=['--ros-scene-interface','robot',name,'allow']
    flags+=['--ros-scene-call-budget','robot','65536','1','--ros-scene-call','robot','solver','main','worker','solve','request-octets','response-octets','64','64','2','60000000000',
            '--ros-scene-grant','robot','exchange','module.call','solver','registered-message',','.join(CALLS),
            '--wasm-preload-library',str(recipient),'worker']
    if a.clear_input!='default':flags+=['--ros-scene-call-clear-input','robot','solver',a.clear_input]
    paths=[Path(__file__).resolve(),runtime,build,Path(__file__).with_name('run_module_security_vm.py'),ROOT/'tools/debug/secure_server.py',ROOT/'tools/debug/dap_adapter.py',ROOT/'test/0018.debugger/run_dap_current_broker.py',recipient,*[g[0] for g in guests.values()]]
    report=dict(complete=False,clear_input=a.clear_input,cgroup=str(GATE),memory_max=68719476736,swap_max=0,inputs={str(p):sha(p) for p in paths},events_before=(GATE/'memory.events').read_text(),cases=[])
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    try:
        for permission,stop_kind in [('full','fault'),('read-only','fault'),('limited','fault'),('full','breakpoint')]:
            folder=out/(permission+'-'+stop_kind);folder.mkdir();server=None;entry=dict(permission=permission,stop_kind=stop_kind,complete=False);report['cases'].append(entry);save()
            try:
                # The live value observer is distinct from resumable replay.
                # It may inspect detached counters while a delivery is retained.
                # The deliberate final fault must follow successful submit/take.
                guest,spin=guests[stop_kind]
                mode=['--runtime-aot','--debug-fault-action','pause']+(['--runtime-debug'] if stop_kind=='breakpoint' else [])
                args=[*mode,'--debug-permissions',permission,'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,'--run',str(guest)]
                server=BrokerSession(runtime,ROOT/'tools/debug/secure_server.py',ROOT/'tools/debug/dap_adapter.py',args,folder);client=server.connect('wasm');entry['broker_argv']=server.command
                assert client.request('uwvm/permissions')['body']['permissions']==permission
                if stop_kind=='breakpoint':
                    assert b'breakpoint ' in client.evaluate(f'breakwasm 1 {spin} 0')
                    client.evaluate('continue')
                deadline=time.monotonic()+30
                while True:
                    stopped=client.evaluate('status')
                    ready=b'limited status=2' in stopped if permission=='limited' else re.search(rb'(?m)^stop-id [1-9][0-9]*$',stopped)
                    if ready and (stop_kind!='breakpoint' or b'stopped: breakpoint' in stopped):break
                    assert time.monotonic()<deadline,stopped;time.sleep(.02)
                if permission!='full':entry['execution_refusal']=client.request('continue',{},success=False)
                entry['stop']=stopped.decode()
                if permission=='limited':
                    entry['refusal']=client.request('uwvm/sceneState',{'selection':'resources'},success=False);assert 'body' not in entry['refusal']
                    entry['memory_refusal']=client.request('readMemory',{'memoryReference':'wasm-memory:0:0:2048','count':128},success=False)
                else:
                    # Independently authorized debugger reads prove that scene
                    # metadata queries do not consume/erase a retained delivery.
                    # sceneState itself must never contain these payload bytes.
                    def delivery():
                        reply=client.request('readMemory',{'memoryReference':'wasm-memory:0:0:2048','count':128})['body']
                        assert reply['unreadableBytes']==0,reply
                        return base64.b64decode(reply['data'],validate=True)
                    before=delivery();token,size,value=struct.unpack('<QQQ',before[:24])
                    assert token and size==8 and value==0x123456789abcdef,before.hex()
                    views={}
                    for selection in ['resources','authority']:
                        query={'selection':selection,'environment':'robot'};page=client.request('uwvm/sceneState',query)['body'];assert not page['payloadsRead'] and not page['nativeOperationsIssued'] and not page['more']
                        assert len(page['rows'])==1,page
                        for row in page['rows']:assert not set(row)&{'payload','path','token','pid','fd','memoryReference','native_pointer','request_id'}
                        for _ in range(3):assert client.request('uwvm/sceneState',{**query,'ifStop':page['stopId']})['body']==page
                        views[selection]=page
                    assert delivery()==before,'metadata inspection changed the retained delivery'
                    row=views['resources']['rows'][0]
                    assert row['clear_after_copy']==(a.clear_input!='false'),row
                    assert row['type']['text']=='module.call' and row['state']['text']=='active' and row['binding']['text']=='registered-message',row
                    assert row['members']=='2' and row['open_handles']=='1' and row['sample_count']=='1' and row['query_count']=='0' and int(row['reserved_bytes'])>0,row
                    entry['views']=views;entry['delivery_unchanged']=True
                    if stop_kind=='breakpoint':
                        journal=client.request('uwvm/recordSnapshot',{'action':'status'})['body']
                        assert journal['mode']==0,journal
                        entry['recording_refusal']=client.request('uwvm/recordSnapshot',{'action':'start','stopId':views['resources']['stopId']},success=False)
                        assert client.request('uwvm/recordSnapshot',{'action':'status'})['body']==journal
                        assert client.evaluate('status')==stopped
                        assert delivery()==before,'refused recording changed the retained delivery'
                        assert client.request('uwvm/sceneState',{'selection':'resources','environment':'robot'})['body']==views['resources']
                entry['complete']=True
            finally:
                if server:entry['cleanup']=server.close()
                save()
        assert all(sha(Path(p))==v for p,v in report['inputs'].items());report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()
    print('PASS live retained message DAP: full/read-only/limited, payload exclusion, nonmutation and recording refusal')

if __name__=='__main__':main()
