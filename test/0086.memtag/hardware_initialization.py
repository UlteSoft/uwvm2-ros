#!/usr/bin/env python3
"""CLI ordering regression: preloaded start, tagged owners, deferred active data."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import vm_matrix as m

def build(code,wide,*,owner=False,alias=False,globals_tag=False):
 body=b"\0"+code+b"\x0b"
 result=b"\0asm\x01\0\0\0"+m.sec(1,b"\x01\x60\0\0")
 limits=m.u(5 if wide else 1)+m.u(1)+m.u(2)
 if alias:result+=m.sec(2,b"\x01\x05owner\x06memory\x02"+limits)
 result+=m.sec(3,b"\x01\0")
 if not alias:result+=m.sec(5,b"\x01"+limits)
 result+=m.sec(7,b"\x01"+(b"\x06memory\x02\0" if owner else b"\x06_start\0\0"))
 if owner:result+=m.sec(8,b"\0")
 result+=m.sec(10,b"\x01"+m.u(len(body))+body)
 if not owner:
  payload=b"deferred active data spans tags!"
  assert len(payload)==32
  result+=m.sec(11,b"\x01\0"+m.c(80,64 if wide else 32)+b"\x0b"+m.u(len(payload))+payload)
 if globals_tag:result+=m.sec(0,m.u(len(b"uwvm.memtag.linear.globals"))+b"uwvm.memtag.linear.globals"+m.u(2)+m.u(1)+m.u(0)+m.u(80)+m.u(32))
 return result

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--vm',type=Path,required=True);parser.add_argument('--out',type=Path,required=True)
 parser.add_argument('--engines',default='int,aot');args=parser.parse_args();args.out.mkdir(parents=True,exist_ok=True);rows=[]
 for wide in [False,True]:
  owner_code=m.Program(wide);owner_code.add(owner_code.tags(80,32,7))
  owner=args.out/('owner'+str(64 if wide else 32)+'.wasm');owner.write_bytes(build(owner_code.code,wide,owner=True))
  for alias in [False,True]:
   for enabled in [False,True]:
    code=m.Program(wide)
    # Alias: the preload start has set tag 7. Independent: the descriptor has
    # assigned a nonzero random tag before the first preload start activates all owners.
    for pos,value in enumerate(b"deferred active data spans tags!"):
     expression=code.n(80+pos)+m.op(0x27)+m.mem(0x2d);code.eq(expression,value)
    if enabled:
     code.add(code.n(80)+m.op(0x27)+m.op(0x21)+bytes([0x50 if wide else 0x45])+b'\x04\x40\x00\x0b')
     if alias:code.eq(code.n(80)+m.op(0x27)+m.op(0x21),7,code.bits)
    raw=build(code.code,wide,alias=alias,globals_tag=not alias)
    name=('alias' if alias else 'independent')+'-'+str(code.bits)+'-'+str(int(enabled));file=args.out/(name+'.wasm');file.write_bytes(raw)
    for engine in args.engines.split(','):
     command=[str(args.vm),'-R'+engine,'--runtime-compile-threads','0',
      *(['--wasm-feature-enable-memory64'] if wide else []),'--wasm-preload-library',str(owner),'owner']
     if engine=='aot':command+=['--runtime-llvm-jit-cache-path','disable']
     if not enabled:command+=['--runtime-disable-memtag']
     command+=['--run',str(file)]
     done=subprocess.run(command,capture_output=True,timeout=90);log=(done.stdout+done.stderr).decode('utf8','replace')
     (args.out/(name+'-'+engine+'.log')).write_text(log)
     row=dict(name=name,engine=engine,passed=done.returncode==0,exit=done.returncode,command=command,sha256=hashlib.sha256(raw).hexdigest())
     rows.append(row);(args.out/'report.json').write_text(json.dumps(rows,indent=2));print(name,engine,done.returncode,row['passed'],flush=True)
 raise SystemExit(any(not r['passed'] for r in rows))
if __name__=='__main__':main()
