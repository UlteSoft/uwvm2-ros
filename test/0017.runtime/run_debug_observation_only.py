#!/usr/bin/env python3
"""Real VM: observation permissions cannot change execution, even on detach.

Every child is owned and bounded by this parent. Deliberate cleanup SIGKILL is
not a VM success oracle. This is authenticated socketpair protocol coverage,
not a graphical IDE or synthetic controller test.
"""
import argparse, hashlib, json, os, resource, select, socket, subprocess, time, shlex
from pathlib import Path


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--vm',required=True,type=Path);ap.add_argument('--out',required=True,type=Path)
    ap.add_argument('--runner',default='');a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=False);resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    common=bytes.fromhex('0061736d0100000001040160000003020100050401010101070a01065f73746172740000')
    images={'return':common+bytes.fromhex('0a040102000b'),
            'loop':common+bytes.fromhex('0a0901070003400c000b0b'),
            'trap':common+bytes.fromhex('0a05010300000b')}
    for name,data in images.items():(a.out/(name+'.wasm')).write_bytes(data)
    rows=[]
    def run(level,probe,mode='prepared'):
        host,guest=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
        guest.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1);host.settimeout(3)
        args=[*shlex.split(a.runner),str(a.vm.resolve()),'-Rdbg' if mode=='prepared' else '--runtime-aot',
              '--debug-permissions',level,'--debug-jit-control-fd',str(guest.fileno()),
              '--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable']
        if mode=='terminal':args+=['--debug-fault-action','pause']
        args+=['--wasip1-single-create','main','--wasip1-single-enable','main','--wasm-set-main-module-name','main',
               '-Wstart','0','--run',str((a.out/({'prepared':'return','running':'loop','terminal':'trap'}[mode]+'.wasm')).resolve())]
        child=subprocess.Popen(args,pass_fds=(guest.fileno(),),stdout=subprocess.DEVNULL,stderr=subprocess.PIPE);guest.close()
        row=dict(level=level,probe=probe,mode=mode,command=args,passed=False)
        def exchange(text):
            host.sendall(text.encode('ascii'));return host.recv(65536).decode('utf-8','replace')
        try:
            before=exchange('status');deadline=time.monotonic()+2
            while mode=='terminal' and 'Wasm trap' not in before and 'reason=9 ' not in before and time.monotonic()<deadline:
                time.sleep(.01);before=exchange('status')
            row['before']=before
            if mode=='terminal' and 'Wasm trap' not in before and 'reason=9 ' not in before:
                raise RuntimeError('launcher fault policy did not expose a real terminal stop')
            if probe=='EOF':host.close();reply='physical observer EOF'
            else:reply=exchange(probe)
            row['reply']=reply
            if probe in ('quit','EOF'):
                if probe!='EOF':host.close()
                try:child.wait(timeout=.25);alive=False
                except subprocess.TimeoutExpired:alive=True
                row['still_alive_after_observation_close']=alive
                row['passed']=alive and (probe=='EOF' or 'Guest state unchanged' in reply)
            elif probe=='bt 1' and level=='limited':
                after=exchange('status');row['after']=after
                row['passed']=('limited Wasm backtrace (numeric coordinates only)' in reply and
                    'module=0 function=0 wasm-offset=0' in reply and
                    all(secret not in reply for secret in ('native','_start','.wasm','0x','locals','memory=')) and
                    before==after and child.poll() is None)
            elif level=='full':
                child.wait(timeout=3);row['exit']=child.returncode
                row['passed']=child.returncode==0 and 'permission denies' not in reply
            else:
                denied='permission denies' in reply or 'unavailable_capability' in reply or 'unavailable-capability' in reply
                after=exchange('status');row['after']=after
                row['passed']=denied and before==after and child.poll() is None
                extra=[]
                while select.select([host],[],[],0)[0]:
                    p=host.recv(65536)
                    if not p:break
                    extra.append(p.decode('utf-8','replace'))
                row['extra_packets']=extra;row['passed']=row['passed'] and not extra
        except (OSError,RuntimeError,subprocess.TimeoutExpired) as e:row['error']=str(e)
        finally:
            host.close()
            if child.poll() is None:child.kill()
            _,stderr=child.communicate(timeout=5);row['cleanup_exit']=child.returncode
            name=f'{len(rows):02d}-{level}-{mode}'
            (a.out/(name+'.stderr')).write_bytes(stderr);row['stderr_file']=name+'.stderr'
        rows.append(row);print(level,mode,probe,'PASS' if row['passed'] else 'FAIL',flush=True)
    for level in ('read-only','limited'):
        # Explicit limited never enables the -Rdbg inspection/initial-stop
        # convenience. Keep a real loop alive instead of mistaking a short
        # program's authorized natural exit for an observer mutation.
        mode='prepared' if level=='read-only' else 'running'
        for probe in ('pause','continue','c','step 1','run','startw','kill','trace host on','trace host off','trace host clear','quit','EOF'):
            run(level,probe,mode)
        if level=='read-only':run(level,'pause','running')
        for probe in ('continue','quit','EOF'):run(level,probe,'terminal')
    run('full','continue')
    # This zero-host ROS launch does not expose the standalone stdin console.
    # Verify the authenticated-channel requirement; do not count its refusal as
    # execution of console EOF/alias code that this product cannot reach.
    for level in ('full','read-only'):
        args=[*shlex.split(a.runner),str(a.vm.resolve()),'-Rdbg','--debug-permissions',level,
              '--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',
              '--wasip1-single-create','main','--wasip1-single-enable','main','--wasm-set-main-module-name','main',
              '-Wstart','0','--run',str((a.out/'return.wasm').resolve())]
        child=subprocess.run(args,input=b'continue\nquit force\n',capture_output=True,timeout=5)
        reason=b'debugger requires a separate authenticated control FD/HANDLE'
        name=f'{len(rows):02d}-{level}-unauthenticated-console'
        (a.out/(name+'.stdout')).write_bytes(child.stdout);(a.out/(name+'.stderr')).write_bytes(child.stderr)
        row=dict(level=level,probe='unauthenticated stdin rejected',mode='launch-refusal',command=args,
                 passed=child.returncode==126 and reason in child.stderr,exit=child.returncode,
                 stdout_file=name+'.stdout',stderr_file=name+'.stderr')
        rows.append(row);print(level,'launch-refusal','PASS' if row['passed'] else 'FAIL',flush=True)
    run('limited','bt 1','terminal')
    report=dict(binary=str(a.vm.resolve()),binary_sha256=hashlib.sha256(a.vm.read_bytes()).hexdigest(),
                passed=sum(x['passed'] for x in rows),total=len(rows),cases=rows)
    (a.out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({key:report[key] for key in ('passed','total','binary_sha256')}))
    return 0 if report['passed']==report['total'] else 1
if __name__=='__main__':raise SystemExit(main())
