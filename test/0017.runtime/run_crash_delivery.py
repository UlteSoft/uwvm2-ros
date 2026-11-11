#!/usr/bin/env python3
"""Finite real VM crashes: human stderr, typed host event and exact process status.

Only an explicit pause case requests SIGSTOP; the parent observes then SIGCONTs
terminal handling. No dump, state modification, recovery or update is requested.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import select
import signal
import socket
import subprocess
import time


def u(value):
    result=bytearray()
    while True:
        byte=value&127;value>>=7;result.append(byte|(128 if value else 0))
        if not value:return bytes(result)


def section(number, data):return bytes([number])+u(len(data))+data


def module(code, memory=True):
    body=b'\0'+code+b'\x0b'
    result=b'\0asm\1\0\0\0'+section(1,b'\1\x60\0\0')+section(3,b'\1\0')
    if memory:result+=section(5,b'\1\1\1\1')
    return result+section(7,b'\1\6_start\0\0')+section(10,b'\1'+u(len(body))+body)


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--vm',required=True,type=Path);ap.add_argument('--out',required=True,type=Path)
    ap.add_argument('--engine',choices=('int','aot'),required=True)
    ap.add_argument('--event-fd',action='store_true',help='Real Linux LLVM-full authenticated diagnostic endpoint')
    args=ap.parse_args();resource.setrlimit(resource.RLIMIT_CORE,(0,0));args.out.mkdir(parents=True,exist_ok=False)
    fixtures={'ok':(b'',0,''),'unreachable':(b'\0',120,'unreachable'),
        'division':(b'\x41\1\x41\0\x6d\x1a',121,'execution'),
        'bounds':(b'\x41\x80\x80\4\x2d\0\0\x1a',121,'execution'),
        'memtag':(b'\x41\0\x41\1\xff'+u(0x01010022)+b'\0\x28\2\0\x1a',122,'memory-protection')}
    for name,(code,_,__) in fixtures.items():(args.out/(name+'.wasm')).write_bytes(module(code))
    base=[str(args.vm.resolve()),'--runtime-'+args.engine,'--runtime-compile-threads','0',
        '--wasip1-single-create','main','--wasip1-single-enable','main','--wasm-set-main-module-name','main','-Wstart','0']
    if args.engine=='aot':base+=['--runtime-llvm-jit-cache-path','disable']
    rows=[]
    for name,(_,expected,category) in fixtures.items():
        for delivery in ('capture','closed-pipe','closed-fd'):
            command=base+['--run',str((args.out/(name+'.wasm')).resolve())]
            kw={};writer=None
            if delivery=='closed-pipe':
                reader,writer=os.pipe();os.close(reader);kw['stderr']=writer
            elif delivery=='closed-fd':kw.update(stderr=subprocess.DEVNULL,preexec_fn=lambda:os.close(2))
            else:kw['stderr']=subprocess.PIPE
            try:done=subprocess.run(command,stdout=subprocess.PIPE,timeout=30,**kw)
            finally:
                if writer is not None:os.close(writer)
            text=(done.stderr or b'').decode('utf-8','replace')
            human=not expected or delivery!='capture' or (f'uwvm: [crash] {category}\n' in text and
                f'\n  Exit status: {expected}\n' in text and '\n  Reason: ' in text and '"schema"' not in text)
            rows.append(dict(name=name+'-'+delivery,command=command,exit=done.returncode,expected=expected,
                passed=done.returncode==expected and human))
            (args.out/(name+'-'+delivery+'.stderr')).write_text(text)
    # Explicit stream selection uses a separate dup/reopen path from the
    # default pre-main sink. EBADF there must not throw out of a noexcept CLI
    # callback; a selected closed pipe must preserve 122 instead of SIGPIPE.
    for option in ('--log-output','--runtime-compiler-log'):
        for target,target_fd in (('out',1),('err',2)):
            for delivery in ('closed-fd','closed-pipe'):
                command=base+[option,target,'--run',str((args.out/'memtag.wasm').resolve())]
                kw=dict(stdout=subprocess.PIPE,stderr=subprocess.PIPE);writer=None
                if delivery=='closed-fd':
                    kw['preexec_fn']=lambda fd=target_fd:os.close(fd)
                else:
                    reader,writer=os.pipe();os.close(reader)
                    kw['stdout' if target_fd==1 else 'stderr']=writer
                try:done=subprocess.run(command,timeout=30,**kw)
                finally:
                    if writer is not None:os.close(writer)
                text=(done.stderr or b'').decode('utf-8','replace')
                human=target_fd==2 or ('uwvm: [crash] memory-protection\n' in text and
                    '\n  Exit status: 122\n' in text and '"schema"' not in text)
                name='explicit-'+option[2:]+'-'+target+'-'+delivery
                rows.append(dict(name=name,command=command,exit=done.returncode,expected=122,
                    passed=done.returncode==122 and human))
                (args.out/(name+'.stderr')).write_text(text)
                (args.out/(name+'.stdout')).write_bytes(done.stdout or b'')
    for flags in (['--debug','pause'],['--debug=pause'],['--debug-fault-action'],
                  ['--debug-fault-action','none'],['--debug-fault-action','dump'],
                  ['--debug-fault-action','pause','--debug-fault-action','pause']):
        command=base+flags+['--run',str((args.out/'ok.wasm').resolve())]
        done=subprocess.run(command,capture_output=True,timeout=30)
        rows.append(dict(name='reject-'+'-'.join(flags),exit=done.returncode,expected=126,passed=done.returncode==126,command=command))
    # The explicit default action remains omission, even with no debugger grants.
    command=base+['--debug-fault-action','pause','--debug-permissions','none','--run',str((args.out/'memtag.wasm').resolve())]
    child=subprocess.Popen(command,stdout=subprocess.DEVNULL,stderr=subprocess.PIPE)
    stopped=False;deadline=time.monotonic()+30
    while time.monotonic()<deadline:
        pid,status=os.waitpid(child.pid,os.WNOHANG|os.WUNTRACED)
        if pid:
            if os.WIFSTOPPED(status):stopped=os.WSTOPSIG(status)==signal.SIGSTOP;os.kill(child.pid,signal.SIGCONT)
            else:child.returncode=os.waitstatus_to_exitcode(status);break
        if stopped:break
        select.select([],[],[],0.01)
    if not stopped and child.poll() is None:child.kill()
    _,err=child.communicate(timeout=30)
    rows.append(dict(name='explicit-pause',exit=child.returncode,expected=122,stopped=stopped,
        passed=stopped and child.returncode==122 and b'Fault action: pause for the authorized host' in err,command=command))
    if args.event_fd:
        if args.engine!='aot':raise SystemExit('--event-fd requires LLVM-full; no interface capability inferred')
        for permission in ('full','read-only','limited','none'):
            host,guest=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
            with host,guest:
                command=base+['--debug-permissions',permission,'--diagnostic-control-fd',str(guest.fileno()),'--run',str((args.out/'memtag.wasm').resolve())]
                child=subprocess.Popen(command,pass_fds=(guest.fileno(),),stdout=subprocess.PIPE,stderr=subprocess.PIPE)
                guest.close();output,error=child.communicate(timeout=30);host.setblocking(False);events=[]
                while True:
                    try:data=host.recv(65536)
                    except BlockingIOError:break
                    if not data:break
                    try:events.append(json.loads(data))
                    except json.JSONDecodeError:pass
                crash=next((x for x in events if x.get('schema')=='uwvm.crash.v2'),None)
                expected=126 if permission=='none' else 122
                passed=child.returncode==expected and (crash is None if permission=='none' else crash is not None and
                    crash.get('exit_status')==122 and crash.get('category')=='memory-protection' and crash.get('memory') is None and
                    crash.get('module')==0 and crash.get('function')==0 and crash.get('wasm_offset') is None)
                rows.append(dict(name='host-event-'+permission,exit=child.returncode,expected=expected,passed=passed,event=crash,command=command))
                (args.out/('host-event-'+permission+'.stderr')).write_bytes(error)
    report=dict(binary=str(args.vm.resolve()),binary_sha256=hashlib.sha256(args.vm.read_bytes()).hexdigest(),
        passed=sum(row['passed'] for row in rows),total=len(rows),cases=rows)
    (args.out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('passed','total','binary_sha256')}))
    for row in rows:
        if not row['passed']:print('FAIL',row['name'],'exit',row['exit'],'expected',row['expected'])
    return 0 if report['passed']==report['total'] else 1


if __name__=='__main__':raise SystemExit(main())
