#!/usr/bin/env python3
"""Actual isolated native crash, retained ROSv2 failure and nonmutating DAP views.

Runs a native shared object that faults on a PROT_NONE write during a call or
its dlopen constructor, exits without the required finished frame, reports
failure, or sends an unknown frame kind. Positive controls complete normally
or wait for an authorized stop. Launch cases inherit a kernel filter refusing
pidfd acquisition and/or authorization send. All run in the production component sandbox.
Public Wasm calls must observe the correct retained first cause and confirmed exit
when a process was created; a refusal before creation must not invent an exit;
the observation bound is two seconds after start admission, shorter than both
startup and execution expiry. An exact start retry must not launch again.
Full/read-only DAP reports the same
failure without payload, process identifiers or paths; limited access is denied.
Namespace-identity refusal and namespace-release peer death use an external
seccomp notification supervisor. Release failure allows real pidfd acquisition
and liveness checks, then kills and waits for both peers before continuing the
real one-byte release syscall, which receives kernel EPIPE.
The resource-readback case refuses only RLIMIT_NOFILE reads in a direct born
namespace child; its setter and the VM's own readbacks remain intact.
The original VM is unmodified; independent fixture cleanup occurs only after
VM exit and never supplies product cleanup evidence.
All compilers, VM, native sandbox and debugger children inherit the existing
64 GiB zero-swap gate. No physical robot or vendor driver is claimed.
With --original-realman-receipt, the metadata-only adapter instead links an
unchanged original vendor ELF verified against its pinned source archive. It
calls rm_api_version without connecting a robot. Faults are injected in the
external adapter, never attributed to a defect in the original vendor library.
"""
import argparse
import ctypes
import json
import os
from pathlib import Path
import re
import resource
import subprocess
import sys
import time
from run_scene_vm import ROOT,GATE,sha,verify_gate
from run_scene_robot_local import deadline,u64,text,blob,profile
from run_scene_robot_process import component_flags,component_guest,COMPONENT_PROFILE,CONFIG,digest,OPERATIONS,ParameterGuest
sys.path.insert(0,str(ROOT/'test/0018.debugger'))
from run_dap_current_broker import BrokerSession

def guest(width,memory,fault=False,expected_failure=4,terminal_state='failed',cleanup_unknown=False,exit_observed=True):
    g=ParameterGuest(OPERATIONS,width,memory);g.loop_id=0;g.globals.update(['status','overall']);g.check('call $abi',2)
    g.steady=g.open('steady');g.call('clock.create',g.steady,[0]);g.clock=g.save('clock')
    g.body.append(g.refresh(30000000000)+' global.set $overall')
    cap=g.open('component');config=g.open('configuration')
    opening=COMPONENT_PROFILE+bytes(8)+text('configuration')+u64(1)+blob(digest(CONFIG))+u64(0)
    g.raw('component.open',cap,opening,[(len(COMPONENT_PROFILE),config)]);instance=g.save('component.instance')
    until=deadline(g,'startup',4900000000);start=bytes(8)+u64(1)+bytes(16)
    patch=[(0,instance),(16,instance),(24,until)];g.raw('component.start',cap,start,patch)
    # Do not accept a later lifetime timeout as evidence of prompt native exit.
    g.body.append(g.refresh(2000000000)+' global.set $overall')
    at=512+len(COMPONENT_PROFILE);fields=at+8+len(terminal_state);processes=int(cleanup_unknown);exited=int(exit_observed)
    g.body.append('loop $exit');g.polling('component.describe',cap,bytes(8),[(0,instance)])
    g.body.append(g.word(at)+f' i64.const {len(terminal_state)} i64.ne if');g.bound();g.body.append('br $exit end')
    g.exact(at+8,terminal_state.encode())
    g.body.append(g.word(fields+40)+f' i64.const {exited} i64.ne if');g.bound();g.body.append('br $exit end end')
    g.equal(g.word(fields+8),processes)
    g.equal(g.word(fields+48),expected_failure)
    g.raw('component.start',cap,start,patch)
    g.polling('component.describe',cap,bytes(8),[(0,instance)])
    g.exact(at,text(terminal_state));g.equal(g.word(fields+8),processes);g.equal(g.word(fields+40),exited)
    g.equal(g.word(fields+48),expected_failure)
    until=deadline(g,'stop.deadline',1900000000)
    stop=bytes(8)+u64(1)+bytes(8)+profile('component.kill')+bytes(8)
    g.raw('component.stop',cap,stop,[(0,instance),(16,instance),(len(stop)-8,until)])
    for _ in range(3):
        g.polling('component.describe',cap,bytes(8),[(0,instance)])
        g.exact(at,text(terminal_state));g.equal(g.word(fields+40),exited);g.equal(g.word(fields+48),expected_failure)
    if fault:g.body.append('unreachable')
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--runtime',type=Path,required=True)
    p.add_argument('--source-root',type=Path,required=True);p.add_argument('--component-receipt',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--original-realman-receipt',type=Path,
                   help='Pinned acquisition receipt for an unchanged original vendor ELF; metadata-only isolation fixture')
    p.add_argument('--expect-no-created-process',action='store_true',
                   help='Assert pre-spawn pidfd refusal: zero members and no observed exit, including DAP')
    p.add_argument('--fault',choices=('call','constructor','unannounced-exit','reported-failure','malformed-frame','completion','controlled-stop','launch-pidfd','launch-gate','launch-both','namespace-identity','namespace-release','domain-pin','domain-release','domain-launcher','resource-readback'),default='call');a=p.parse_args()
    assert not a.expect_no_created_process or a.fault in ('launch-pidfd','launch-both')
    assert not a.original_realman_receipt or a.fault in ('completion','controlled-stop','call','constructor')
    verify_gate();assert ctypes.CDLL(None,use_errno=True).prctl(36,1,0,0,0)==0
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));out=a.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(exist_ok=False)
    runtime=a.runtime.resolve();source=a.source_root.resolve();build=runtime.parent/'full-build-results.json'
    br=json.loads(build.read_text());assert br['complete'] and br['binary_sha256']==sha(runtime) and br['source_root']==str(source)
    receipt=a.component_receipt.resolve();cr=json.loads(receipt.read_text());assert cr['complete']
    images={'/worker/bootstrap':receipt.parent/'images/bootstrap','/worker/backend.so':out/'backend.so','launcher':receipt.parent/'images/bwrap'}
    for name in ('ld-linux-x86-64.so.2','libc.so.6','libstdc++.so.6','libgcc_s.so.1','libm.so.6'):
        images[('/lib64/' if name.startswith('ld-') else '/lib/x86_64-linux-gnu/')+name]=receipt.parent/'images'/name
    config=out/'configuration.bin';config.write_bytes(CONFIG)
    backend=out/'backend.cpp';backend_source=Path(__file__).with_name('original_realman_component.cpp' if a.original_realman_receipt else 'robot_vm_component_backend.cpp')
    backend.write_bytes(backend_source.read_bytes())
    original_pins={};original_evidence=None;original_flags=[]
    if a.original_realman_receipt:
        from original_realman_images import prepare
        includes,original_pins,original_evidence=prepare(a.original_realman_receipt,out,images)
        original_flags=['-isystem',str(includes),'-L'+str(out/'original-driver'),'-Wl,-rpath,/worker','-l:libapi_cpp.so','-Wl,-z,defs']
    define={'call':'UWVM_COMPONENT_FAULT_FIXTURE','constructor':'UWVM_COMPONENT_CONSTRUCTOR_FAULT_FIXTURE',
            'unannounced-exit':'UWVM_COMPONENT_UNANNOUNCED_EXIT_FIXTURE','reported-failure':'UWVM_COMPONENT_REPORTED_FAILURE_FIXTURE',
            'malformed-frame':'UWVM_COMPONENT_MALFORMED_FRAME_FIXTURE','completion':'UWVM_COMPONENT_COMPLETION_FIXTURE',
            'controlled-stop':'UWVM_COMPONENT_IDLE_FIXTURE','launch-pidfd':'UWVM_COMPONENT_COMPLETION_FIXTURE',
            'launch-gate':'UWVM_COMPONENT_COMPLETION_FIXTURE','launch-both':'UWVM_COMPONENT_COMPLETION_FIXTURE',
            'namespace-identity':'UWVM_COMPONENT_COMPLETION_FIXTURE','namespace-release':'UWVM_COMPONENT_COMPLETION_FIXTURE',
            'domain-pin':'UWVM_COMPONENT_COMPLETION_FIXTURE','domain-release':'UWVM_COMPONENT_COMPLETION_FIXTURE',
            'domain-launcher':'UWVM_COMPONENT_COMPLETION_FIXTURE','resource-readback':'UWVM_COMPONENT_COMPLETION_FIXTURE'}[a.fault]
    expected_failure={'reported-failure':9,'malformed-frame':5,'completion':0,'controlled-stop':0,
                      'launch-pidfd':2,'launch-gate':2,'launch-both':2,'namespace-identity':3,'namespace-release':3,
                      'domain-pin':3,'domain-release':3,'domain-launcher':3,'resource-readback':3}.get(a.fault,4)
    unknown=a.fault=='namespace-identity'
    exit_observed=not (unknown or a.expect_no_created_process)
    terminal_state='outcome_unknown' if unknown else 'failed' if expected_failure else 'exited'
    def make_guest(width,memory,pause=False):
        if a.fault!='controlled-stop':return guest(width,memory,pause,expected_failure,terminal_state,unknown,exit_observed)
        g=component_guest(width,memory)
        # Logical close retains detached retired metadata for DAP inspection.
        if pause:
            g.body.append('unreachable')
        return g
    compile=['/usr/bin/clang++-22','-std=c++23','-O3','-g','-fPIC','-shared','-Wall','-Wextra','-Werror',
             '-D'+define,'-I'+str(source/'src'),str(backend),*original_flags,'-o',str(images['/worker/backend.so'])]
    done=subprocess.run(compile,capture_output=True,text=True,timeout=60)
    (out/'compile.log').write_text(done.stdout+done.stderr);assert done.returncode==0,done.stderr
    product=runtime;launch_compile=None;launch_inputs=[]
    namespace_fault=a.fault.startswith('namespace-')
    marker=('FIXTURE_NAMESPACE_RELEASE_PEERS_EXITED_BEFORE_SEND' if a.fault=='namespace-release'
            else 'FIXTURE_NAMESPACE_PIDFD_REFUSED_AND_EXTERNALLY_CLEANED') if namespace_fault else None
    resource_fault=a.fault=='resource-readback'
    if resource_fault:marker='FIXTURE_RESOURCE_READBACK_REFUSED_AND_PRODUCT_CLEANUP'
    domain_fault=a.fault.startswith('domain-')
    if domain_fault:marker='FIXTURE_BORN_DOMAIN_FAULT_AND_PRODUCT_TREE_CLEANUP'
    if a.fault.startswith('launch-') or namespace_fault or domain_fault or resource_fault:
        original_wrapper=Path(__file__).with_name('component_resource_failure_wrapper.cpp' if resource_fault else
            'component_domain_failure_wrapper.cpp' if domain_fault else
            'component_identity_failure_wrapper.cpp' if namespace_fault else 'component_launch_failure_wrapper.cpp')
        wrapper=out/original_wrapper.name;wrapper.write_bytes(original_wrapper.read_bytes());product=out/'launch-failure-wrapper'
        defines=['-DUWVM_TEST_RUNTIME_PATH='+json.dumps(str(runtime))]
        if a.fault in ('launch-pidfd','launch-both'):defines.append('-DUWVM_TEST_LAUNCH_PIDFD_FAILURE')
        if a.fault in ('launch-gate','launch-both'):defines.append('-DUWVM_TEST_LAUNCH_GATE_FAILURE')
        if a.fault=='namespace-release':defines.append('-DUWVM_TEST_NAMESPACE_RELEASE_FAILURE')
        if a.fault=='domain-release':defines.append('-DUWVM_TEST_DOMAIN_RELEASE_FAILURE')
        if a.fault=='domain-launcher':defines.append('-DUWVM_TEST_DOMAIN_LAUNCHER_FAILURE')
        launch_compile=['/usr/bin/clang++-22','-std=c++23','-O3','-g','-UNDEBUG','-Wall','-Wextra','-Werror',*defines,str(wrapper),'-o',str(product)]
        wrapped=subprocess.run(launch_compile,capture_output=True,text=True,timeout=60)
        (out/'compile-wrapper.log').write_text(wrapped.stdout+wrapped.stderr);assert wrapped.returncode==0,wrapped.stderr
        launch_inputs=[original_wrapper,wrapper,product]
    flags=component_flags(images,config);pins={str(path):sha(path) for path in [Path(__file__).resolve(),backend,config,receipt,build,runtime,source/'source-manifest.json',
        *[path for path in (ROOT/'tools/debug').iterdir() if path.suffix in ('.py','.json')],
        ROOT/'test/0018.debugger/run_dap_current_broker.py',ROOT/'test/0017.runtime/run_debug_source_step_cli.py',*images.values(),*launch_inputs,
        *Path(__file__).parent.glob('run_scene_*.py'),backend_source]}
    pins.update(original_pins)
    retained_inputs={}
    for original,digest_value in list(pins.items()):
        path=Path(original).resolve()
        if path.is_relative_to(ROOT):
            retained=out/'fixture-source'/path.relative_to(ROOT)
            retained.parent.mkdir(parents=True,exist_ok=True);retained.write_bytes(path.read_bytes())
            assert sha(retained)==digest_value
            retained_inputs[original]=str(retained);pins[str(retained)]=digest_value
    report=dict(complete=False,scope=__doc__,fault=a.fault,original_vendor=original_evidence,runtime_sha256=sha(runtime),pins=pins,retained_inputs=retained_inputs,compile=compile,launch_wrapper_compile=launch_compile,cgroup=str(GATE),
                memory_max=68719476736,swap_max=0,observation_bound_ns=2000000000,
                expected_created_process=not a.expect_no_created_process,expected_exit_observed=exit_observed,
                events_before=(GATE/'memory.events').read_text(),cases=[])
    def reap_descendants():
        values=[];until=time.monotonic()+3
        while True:
            try:pid,status=os.waitpid(-1,os.WNOHANG)
            except ChildProcessError:return values
            if pid:
                assert os.waitstatus_to_exitcode(status) in (0,-9),(pid,status)
                values.append(dict(pid=pid,exit=os.waitstatus_to_exitcode(status)))
            else:
                assert time.monotonic()<until,'Fixture descendant remained after VM exit'
                time.sleep(.01)
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    prefix=[str(product),'--wasm-set-main-module-name','main','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable','--log-color','disable']
    try:
        for width,memory in ((32,0),(64,0),(32,1),(64,1)):
            name=f'crash-{width}-{memory}';wasm=out/(name+'.wasm');wat=out/(name+'.wat');g=make_guest(width,memory)
            wat.write_text(g.render());subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True,timeout=30)
            pins[str(wat)]=sha(wat);pins[str(wasm)]=sha(wasm)
            for engine in ('int','aot'):
                label=name+'-'+engine;argv=prefix+['--runtime-'+engine,*flags,'--run',str(wasm)]
                started=time.monotonic();done=subprocess.run(argv,capture_output=True,text=True,timeout=45)
                (out/(label+'.stdout')).write_text(done.stdout);(out/(label+'.stderr')).write_text(done.stderr)
                report['cases'].append(dict(name=label,argv=argv,exit=done.returncode,wall_seconds=time.monotonic()-started,
                                           passed=done.returncode==0));save()
                assert done.returncode==0,(label,done.stderr[-3000:])
                report['cases'][-1]['descendants']=reap_descendants()
                if marker:assert marker in done.stderr
                print('PASS '+label,flush=True)
        g=make_guest(32,0,True);wat=out/'debug.wat';wasm=out/'debug.wasm';wat.write_text(g.render())
        subprocess.run(['wat2wasm','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True,timeout=30)
        pins[str(wat)]=sha(wat);pins[str(wasm)]=sha(wasm)
        for permission in ('full','read-only','limited'):
            folder=out/permission;folder.mkdir();server=None;entry=dict(name='dap-'+permission,passed=False);report['cases'].append(entry)
            try:
                args=prefix[1:]+['--runtime-aot','--debug-fault-action','pause','--debug-permissions',permission,*flags,'--run',str(wasm)]
                server=BrokerSession(product,ROOT/'tools/debug/secure_server.py',ROOT/'tools/debug/dap_adapter.py',args,folder)
                client=server.connect('wasm');end=time.monotonic()+30
                while True:
                    stopped=client.evaluate('status')
                    paused=(b'limited status=2' in stopped) if permission=='limited' else bool(re.search(rb'(?m)^stop-id [1-9][0-9]*$',stopped))
                    if paused:break
                    assert time.monotonic()<end,stopped;time.sleep(.02)
                entry['stop']=stopped.decode()
                if original_evidence:
                    # Independent host observation, not guest/debugger disclosure.
                    # The original broker owns a stopped direct VM child here.
                    children=Path(f'/proc/{server.child.pid}/task/{server.child.pid}/children').read_text().split()
                    matches=[Path('/proc')/pid for pid in children if (Path('/proc')/pid/'exe').resolve()==runtime]
                    assert len(matches)==1
                    process=matches[0];before=(process/'stat').read_text()
                    maps=(process/'maps').read_text()
                    assert (process/'stat').read_text().split(') ',1)[1].split()[19]==before.split(') ',1)[1].split()[19]
                    assert (process/'cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
                    assert 'libapi_cpp' not in maps and '/original-driver/' not in maps
                    (folder/'vm-maps.txt').write_text(maps)
                    entry['vm_maps_sha256']=sha(folder/'vm-maps.txt')
                    entry['original_vendor_named_mapping_in_vm']=False
                if permission=='limited':
                    entry['refusal']=client.request('uwvm/sceneState',{'selection':'resources'},success=False);assert 'body' not in entry['refusal']
                else:
                    pages=[];start=0
                    while True:
                        query={'selection':'resources','environment':'robot','start':start}
                        if pages:query['ifStop']=pages[0]['stopId']
                        page=client.request('uwvm/sceneState',query)['body'];pages.append(page)
                        assert not page['payloadsRead'] and not page['nativeOperationsIssued']
                        if not page['more']:break
                        start+=len(page['rows']);assert start<256
                    row=next(row for page in pages for row in page['rows'] if row['type']['text']=='component')
                    expected_state='retired' if a.fault=='controlled-stop' else terminal_state
                    assert row['state']['text']==expected_state and row['exit_observed']==exit_observed and row['members']==str(int(unknown)) and row['retired']==(a.fault=='controlled-stop') and row['status_code']==str(expected_failure),row
                    for page in pages:
                        for _ in range(2):
                            assert client.request('uwvm/sceneState',{'selection':'resources','environment':'robot','start':page['start'],'ifStop':page['stopId']})['body']==page
                    entry['pages']=pages
                entry['passed']=True;print('PASS '+entry['name'],flush=True)
            finally:
                if server:
                    entry['cleanup']=server.close();entry['descendants']=reap_descendants()
                    if marker:assert marker in (folder/'broker.raw').read_text(errors='replace')
                save()
        report['inputs_unchanged']=all(sha(Path(path))==digest for path,digest in pins.items());assert report['inputs_unchanged']
        report['complete']=True
    finally:report['events_after']=(GATE/'memory.events').read_text();save()

if __name__=='__main__':main()
