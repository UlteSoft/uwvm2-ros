#!/usr/bin/env python3
"""Log-and-continue does not bypass WASI authority or sandbox bounds."""
import argparse,json,shlex,subprocess
from pathlib import Path
import vm_matrix as m

def image(denied=False,bounds=False):
 p=m.Program();p.add(p.tags(256,16,3))
 for address,value in [(32,65535 if bounds else 256),(36,3),(64,123)]:p.add(m.c(address)+m.c(value)+m.mem(0x36))
 p.eq(m.c(1)+m.c(32)+m.c(1)+m.c(64)+b'\x10\0',76 if denied else 21 if bounds else 0)
 p.eq(m.c(64)+m.mem(0x28),123 if denied or bounds else 3)
 body=b'\0'+p.code+b'\x0b'
 name=lambda text:m.u(len(text))+text.encode()
 return (b'\0asm\x01\0\0\0'+m.sec(1,b'\x02\x60\x04'+b'\x7f'*4+b'\x01\x7f\x60\0\0')+
  m.sec(2,b'\x01'+name('wasi_snapshot_preview1')+name('fd_write')+b'\0\0')+m.sec(3,b'\x01\x01')+m.sec(5,b'\x01\x01\x01\x02')+
  m.sec(7,b'\x01\x06_start\0\x01')+m.sec(10,b'\x01'+m.u(len(body))+body)+m.sec(11,b'\x01\0'+m.c(256)+b'\x0b\x03OK\n'))

def main():
 p=argparse.ArgumentParser();p.add_argument('--vm',required=True);p.add_argument('--runner',default='');p.add_argument('--out',required=True,type=Path);a=p.parse_args();a.out.mkdir(parents=True,exist_ok=False);rows=[]
 for engine in ['int','aot']:
  for mode in ['sync','software-async']:
   for kind in ['allowed','interface-denied','resource-denied','bounds']:
    denied=kind.endswith('denied');wasm=a.out/(kind+'.wasm');wasm.write_bytes(image(denied,kind=='bounds'))
    cmd=shlex.split(a.runner)+[a.vm,'-R'+engine,'-Rct','0','-Wname','main','--wasip1-single-create','main','--wasip1-single-enable','main',
     '--wasip1-single-interface','main','fd_write','deny' if kind=='interface-denied' else 'allow',*m.mode_flags(mode),*m.report_flags('err')]
    if kind!='resource-denied':cmd+=['--wasip1-single-grant','main','stdout.write']
    if engine=='aot':cmd+=['--runtime-llvm-jit-cache-path','disable']
    cmd+=['--run',str(wasm)];r=subprocess.run(cmd,capture_output=True,timeout=60)
    text=r.stderr.decode(errors='replace');reports=[json.loads(l) for l in text.splitlines() if l.startswith('{"schema":"uwvm.memtag.violation.v1"')]
    expected=0
    good=r.returncode==expected and r.stdout==(b'OK\n' if kind=='allowed' else b'')
    good=good and (len(reports)>=1 and all(x['pointer']==256 and x['allocation_tag']==3 and x['pointer_tag']==0 for x in reports) if kind=='allowed' else not reports)
    name=engine+'-'+mode+'-'+kind;(a.out/(name+'.stderr')).write_bytes(r.stderr)
    rows.append(dict(name=name,argv=cmd,exit=r.returncode,expected=expected,reports=len(reports),passed=bool(good)));(a.out/'report.json').write_text(json.dumps(dict(passed=all(x['passed'] for x in rows),rows=rows),indent=2))
    if not good:raise RuntimeError(name+': '+text[-1800:])
 print(json.dumps(dict(passed=True,executions=len(rows))))
if __name__=='__main__':main()
