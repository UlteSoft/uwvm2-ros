#!/usr/bin/env python3
"""Actual execution oracle for explicitly selected silent-log reporting."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
import vm_matrix as m
import core3_gc_matrix as gc

def cases():
 for wide in (False,True):
  bits=64 if wide else 32
  p=m.Program(wide);p.add(p.tags())
  p.add(p.tagged(15,4)+m.c(77)+m.mem(0x36))
  p.eq(p.tagged(15,4)+m.mem(0x28),77)
  yield f'scalar-{bits}',m.module(p.code,wide),4,0
  p=m.Program(wide);p.add(p.tags());p.add(p.tagged(0,4)+m.c(77)+p.n(64)+m.fc(11))
  p.check_bytes(0,bytes([77])*64)
  yield f'fill-{bits}',m.module(p.code,wide),4,0
  p=m.Program(wide);p.add(p.tags());p.add(p.tagged(0)+m.c(77)+p.n(64)+m.fc(11))
  p.add(p.tagged(128,4)+p.tagged(0,4)+p.n(64)+m.fc(10)+m.u(0));p.check_bytes(128,bytes([77])*64)
  yield f'copy-{bits}',m.module(p.code,wide),8,0
  p=m.Program(wide);p.add(p.tags());p.add(p.tagged(15,4)+b'\xfd\x0c'+bytes(range(16))+m.simd(11))
  p.eq(p.tagged(15,4)+m.simd(0)+b'\xfd\x1b\0',0x03020100)
  yield f'simd-{bits}',m.module(p.code,wide),4,0
  p=m.Program(wide);p.add(p.tags());p.add(p.tagged(32,4)+m.c(77)+b'\xfe\x17\x02\0')
  p.eq(p.tagged(32,4)+b'\xfe\x10\x02\0',77)
  yield f'atomic-{bits}',m.module(p.code,wide),2,0
  yield f'gc-{bits}',gc.tagged_through_gc(bits,True),1,0
  yield f'bounds-{bits}',m.denial(wide,'bounds'),0,121

def main():
 p=argparse.ArgumentParser();p.add_argument('--vm',required=True);p.add_argument('--runner',default='')
 p.add_argument('--engines',default='int,aot');p.add_argument('--modes',default='sync,software-async')
 p.add_argument('--out',required=True,type=Path);a=p.parse_args();a.out.mkdir(parents=True,exist_ok=False)
 rows=[]
 def run(name,cmd,status,expected,destination):
  r=subprocess.run(cmd,capture_output=True,timeout=90);text=r.stderr.decode(errors='replace')
  (a.out/(name+'.stdout')).write_bytes(r.stdout);(a.out/(name+'.stderr')).write_bytes(r.stderr)
  data=(r.stdout if destination=='out' else r.stderr).decode(errors='replace') if destination in ('out','err') else Path(destination).read_text()
  reports=[json.loads(line) for line in data.splitlines() if line.startswith('{"schema":"uwvm.memtag.violation.v1"')]
  passed=r.returncode==status and len(reports)==expected and sorted(x['sequence'] for x in reports)==list(range(expected))
  if status==121:passed=passed and 'out of bounds' in (text+data+r.stdout.decode(errors='replace'))
  if reports:passed=passed and all(x['pointer_tag']!=x['allocation_tag'] and x['action']=='silent-log' for x in reports)
  rows.append(dict(name=name,argv=cmd,exit=r.returncode,expected_exit=status,reports=len(reports),expected_reports=expected,passed=passed))
  (a.out/'report.json').write_text(json.dumps(dict(passed=all(x['passed'] for x in rows),rows=rows),indent=2))
  if not passed:raise RuntimeError(name+': '+text[-2000:])
 for name,raw,count,status in cases():
  wasm=a.out/(name+'.wasm');wasm.write_bytes(raw)
  for engine in a.engines.split(','):
   base=shlex.split(a.runner)+[a.vm,'-R'+engine,'--runtime-compile-threads','0','--wasm-feature-enable-memory64','--wasm-feature-enable-multi-memory','--wasm-feature-enable-threads','-WFE-gc','-WFE-function-references','-WFE-reference-types']
   if engine=='aot':base+=['--runtime-llvm-jit-cache-path','disable']
   for mode in a.modes.split(','):
    for output in ('out','err','file'):
     label=f'{name}-{engine}-{mode}-{output}';destination=str(a.out/(label+'.jsonl')) if output=='file' else output
     cmd=base+[*m.mode_flags(mode),*m.report_flags(destination),'--run',str(wasm)]
     run(label,cmd,status,count,destination)
   if not name.startswith('gc-'):
    run(f'{name}-{engine}-disabled-log',base+['--runtime-disable-memtag',*m.report_flags('err'),'--run',str(wasm)],status,0,'err')
   # Omission of the report option retains fail-closed default behavior.
   if count:run(f'{name}-{engine}-default',base+['--run',str(wasm)],122,0,'err')
 print(json.dumps(dict(passed=True,executions=len(rows))))
if __name__=='__main__':main()
