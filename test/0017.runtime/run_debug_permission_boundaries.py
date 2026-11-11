#!/usr/bin/env python3
"""Real authenticated VM command transport for all four debug permission levels.

The intentionally live Guest is owned by this bounded parent and always killed
and reaped after the command probes. No update permit, dump or repair is issued.
This is a headless VM protocol test, not a graphical IDE qualification.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import select
import socket
import shlex
import subprocess


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--vm',required=True,type=Path);ap.add_argument('--out',required=True,type=Path)
    ap.add_argument('--runner',default='');args=ap.parse_args();resource.setrlimit(resource.RLIMIT_CORE,(0,0));args.out.mkdir(parents=True,exist_ok=False)
    wasm=args.out/'held-guest.wasm'
    wasm.write_bytes(bytes.fromhex('0061736d0100000001040160000003020100050401010101070a01065f737461727400000a0901070003400c000b0b'))
    rows=[]
    for level in ('full','read-only','limited','none'):
        host,guest=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
        with host,guest:
            # Match the maintained broker: credentials must already be enabled
            # when a request is queued, even if exec has not adopted it yet.
            guest.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
            command=[*shlex.split(args.runner),str(args.vm.resolve()),'-Rdbg','--debug-permissions',level,'--debug-jit-control-fd',str(guest.fileno()),
                '--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',
                '--wasip1-single-create','main','--wasip1-single-enable','main','--wasm-set-main-module-name','main','-Wstart','0','--run',str(wasm.resolve())]
            child=subprocess.Popen(command,pass_fds=(guest.fileno(),),stdout=subprocess.DEVNULL,stderr=subprocess.PIPE)
            guest.close();host.settimeout(20)
            try:
                if level=='none':
                    _,error=child.communicate(timeout=20)
                    rows.append(dict(level=level,probe='endpoint disabled',passed=child.returncode==126,exit=child.returncode,command=command))
                else:
                    probes=[('permissions',True),('status',True),('help',True),('help rebuild',True),
                        ('memory 0 0 0 1',level in ('full','read-only')),
                        ('memtags 0 0 0 1',level in ('full','read-only')),
                        ('replace 0 0 1 /private/uwvm-absent-permission-fixture.wasm',level=='full'),
                        ('rebuild /private/uwvm-absent-permission-fixture.wasm',False),
                        ('rebuild-module 0 /private/uwvm-absent-permission-fixture.wasm',False)]
                    for text,allowed in probes:
                        host.sendall(text.encode('ascii'));reply=host.recv(65536).decode('utf-8','replace')
                        denied='permission denies' in reply or 'unavailable-capability' in reply or 'error unavailable_capability' in reply
                        if text=='permissions':passed=reply=='permissions '+level+'\n'
                        elif text=='status':passed=reply.startswith('limited status=' if level=='limited' else 'prepared;')
                        elif text=='help':passed=reply.startswith('permissions | status |' if level=='limited' else 'UWVM debugger help\n')
                        elif text=='help rebuild':passed='independent signed update coordinator' in reply
                        elif text.startswith(('rebuild ','rebuild-module ')):passed=reply.startswith('error: module rebuild requires the independent signed update coordinator')
                        elif text.startswith('memory ') and allowed:passed=reply.startswith('Wasm memory module=0 index=0 offset=0 bytes=1\n')
                        elif text.startswith('memtags ') and allowed:passed=reply.startswith('Wasm memory tags module=0 index=0 offset=0 bytes=1 ')
                        elif allowed:passed=reply.startswith('error: ') and not denied
                        else:passed=denied
                        # One request has exactly one response packet. Consume
                        # and record surplus packets before sending anything else.
                        extra=[]
                        while select.select([host],[],[],0.01)[0]:
                            packet=host.recv(65536)
                            if not packet:break
                            extra.append(packet.decode('utf-8','replace'))
                        passed=passed and not extra
                        rows.append(dict(level=level,probe=text,allowed=allowed,passed=passed,reply=reply,extra_packets=extra,command=command))
                        print(level,text,'PASS' if passed else 'FAIL',flush=True)
            except (OSError,subprocess.TimeoutExpired) as error:
                rows.append(dict(level=level,probe='transport',passed=False,error=str(error)))
            finally:
                if child.poll() is None:child.kill()
                _,error=child.communicate(timeout=20)
                (args.out/(level+'.stderr')).write_bytes(error)
    report=dict(binary=str(args.vm.resolve()),binary_sha256=hashlib.sha256(args.vm.read_bytes()).hexdigest(),
        passed=sum(r['passed'] for r in rows),total=len(rows),cases=rows,
        scope='actual authenticated VM transport; no graphical IDE, ARM qualification or update activation claimed')
    (args.out/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:report[k] for k in ('passed','total','binary_sha256')}))
    return 0 if report['passed']==report['total'] else 1


if __name__=='__main__':raise SystemExit(main())
