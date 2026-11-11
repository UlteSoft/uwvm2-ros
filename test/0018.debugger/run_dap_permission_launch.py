#!/usr/bin/env python3
"""Real Linux VM + authenticated broker + DAP stdio permission qualification.

No graphical IDE, remote platform, update activation, automatic dump or recovery
is inferred. Every original owned broker reaps its original Guest on cleanup.
"""
import argparse,hashlib,json,os,resource,subprocess,sys,tempfile,time
from pathlib import Path
from run_dap_current_broker import Client


def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--vm',required=True,type=Path);ap.add_argument('--out',required=True,type=Path);a=ap.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));a.out.mkdir(parents=True,exist_ok=False)
    root=Path(__file__).resolve().parents[2];debug=root/'tools/debug';wasm=a.out/'empty.wasm'
    wasm.write_bytes(bytes.fromhex('0061736d0100000001040160000003020100050401010101070a01065f737461727400000a040102000b'))
    rows=[];commands=[]
    def check(level,name,condition,detail):
        rows.append(dict(level=level,name=name,passed=bool(condition),detail=detail));print(level,name,'PASS' if condition else 'FAIL',flush=True)
    for level in ('full','read-only','limited','none'):
        folder=a.out/level;folder.mkdir();directory=Path(tempfile.mkdtemp(prefix='dap-',dir=a.out.parent));client=None
        vm_args=['--runtime-aot' if level=='none' else '-Rdbg','--debug-permissions',level,'--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',
            '--wasip1-single-create','main','--wasip1-single-enable','main','--wasm-set-main-module-name','main','-Wstart','0','--run',str(wasm.resolve())]
        command=[sys.executable,str(debug/'secure_server.py'),'serve','--uwvm',str(a.vm.resolve()),'--socket-dir',str(directory),'--startup-timeout','20','--',*vm_args];commands.append(command)
        with (folder/'broker.raw').open('wb') as log:
            broker=subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT)
            try:
                if level=='none':
                    status=broker.wait(timeout=25)
                    check(level,'no endpoint',status==0 and not list(directory.iterdir()),dict(exit=status,entries=[p.name for p in directory.iterdir()]))
                else:
                    deadline=time.monotonic()+25
                    while not (directory/'control.sock').exists():
                        if broker.poll() is not None or time.monotonic()>deadline:raise RuntimeError('broker listener unavailable')
                        time.sleep(.01)
                    client=Client(debug/'dap_adapter.py',directory,folder,'source')
                    reply=client.request('uwvm/permissions')
                    check(level,'authenticated permission',reply['body'].get('permissions')==level,reply)
                    capabilities=next(e['body']['capabilities'] for e in client.events if e.get('event')=='capabilities')
                    check(level,'negotiated capabilities',capabilities.get('supportsWriteMemoryRequest')==(level=='full') and capabilities.get('supportsReadMemoryRequest')==(level in ('full','read-only')),capabilities)
                    reply=client.request('uwvm/diagnostics')
                    check(level,'diagnostic redaction',reply['body'].get('redacted')==(level=='limited') and (level!='limited' or reply['body']['text'].startswith('limited status=')),reply)
                    for expression in ('memory 0 0 0 1','memtags 0 0 0 1'):
                        reply=client.request('evaluate',dict(expression=expression,context='repl'),success=level!='limited')
                        expected='Wasm memory tags ' if expression.startswith('memtags') else 'Wasm memory '
                        check(level,expression,reply['body']['result'].startswith(expected) if level!='limited' else 'permission' in reply.get('message',''),reply)
                    for expression in ('rebuild /private/absent.wasm','rebuild-module 0 /private/absent.wasm'):
                        reply=client.request('evaluate',dict(expression=expression,context='repl'),success=False)
                        check(level,'independent update authority','independent signed update coordinator' in reply.get('message',''),reply)
                    if level!='full':
                        for request in ('writeMemory','setVariable','next','setBreakpoints','restart'):
                            reply=client.request(request,{},success=False)
                            check(level,request+' denied','permission' in reply.get('message',''),reply)
            except BaseException as error:
                check(level,'session',False,repr(error))
            finally:
                if client is not None:
                    try:client.close()
                    except BaseException as error:check(level,'adapter cleanup',False,repr(error))
                if broker.poll() is None:broker.terminate()
                try:status=broker.wait(timeout=15)
                except subprocess.TimeoutExpired:broker.kill();broker.wait();status=-999
                remaining=[p.name for p in directory.iterdir()]
                check(level,'broker cleanup',status in (0,130) and not remaining,dict(exit=status,entries=remaining))
                if not remaining:directory.rmdir()
    report=dict(binary=str(a.vm.resolve()),binary_sha256=hashlib.sha256(a.vm.read_bytes()).hexdigest(),commands=commands,passed=sum(r['passed'] for r in rows),total=len(rows),cases=rows,
        scope='actual Linux VM, broker, DAP subprocesses; no mocks; no graphical IDE or remote/ARM qualification')
    (a.out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('passed','total','binary_sha256')}));return 0 if all(r['passed'] for r in rows) else 1
if __name__=='__main__':raise SystemExit(main())
