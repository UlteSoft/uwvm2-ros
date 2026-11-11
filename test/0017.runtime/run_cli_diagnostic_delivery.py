#!/usr/bin/env python3
"""Execute the real pre-main/CLI diagnostic component with failing inherited sinks."""
import argparse,hashlib,json,os,socket,subprocess
from pathlib import Path

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--component',required=True,type=Path);p.add_argument('--out',required=True,type=Path);a=p.parse_args()
    a.out.mkdir(parents=True,exist_ok=False);rows=[]
    for mode in ('capture','closed-pipe','closed-fd','read-only','closed-socket','full-nonblocking-pipe'):
        for reopen in (False,True):
            owned=[];kw={}
            if mode=='capture':kw['stderr']=subprocess.PIPE
            elif mode=='closed-fd':kw.update(stderr=subprocess.DEVNULL,preexec_fn=lambda:os.close(2))
            elif mode=='closed-socket':
                peer,writer=socket.socketpair();peer.close();owned.append(writer.fileno());kw['stderr']=writer.detach()
            else:
                reader,writer=os.pipe()
                if mode=='closed-pipe':os.close(reader);kw['stderr']=writer;owned.append(writer)
                elif mode=='read-only':os.close(writer);kw['stderr']=reader;owned.append(reader)
                else:
                    os.set_blocking(writer,False)
                    try:
                        while True:os.write(writer,b'x'*4096)
                    except BlockingIOError:pass
                    kw['stderr']=writer;owned.extend((reader,writer))
            command=[str(a.component.resolve())]+(['reopen'] if reopen else [])
            try:r=subprocess.run(command,stdout=subprocess.PIPE,timeout=10,**kw)
            finally:
                for fd in owned:os.close(fd)
            passed=r.returncode==122 and b'compiler log survives' in r.stdout and (not reopen or b'explicit log output restored' in r.stdout)
            if mode=='capture':passed=passed and b'uwvm: [warn] boot warning' in r.stderr and b'uwvm: [crash] memory-protection' in r.stderr
            rows.append(dict(mode=mode,reopen=reopen,exit=r.returncode,passed=passed,command=command,stdout=r.stdout.decode(),stderr=(r.stderr or b'').decode()))
    report=dict(binary=str(a.component.resolve()),binary_sha256=hashlib.sha256(a.component.read_bytes()).hexdigest(),passed=sum(r['passed'] for r in rows),total=len(rows),cases=rows)
    (a.out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('passed','total','binary_sha256')}));return 0 if all(r['passed'] for r in rows) else 1
if __name__=='__main__':raise SystemExit(main())
