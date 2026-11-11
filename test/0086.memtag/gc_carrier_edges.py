#!/usr/bin/env python3
"""Full GC carriers preserve tags; standard lossy integer carriers cannot invent them."""
import argparse,json,shlex,subprocess
from pathlib import Path
import vm_matrix as m
import core3_gc_matrix as g

def fixture(kind,bits,tag):
 p=m.Program(bits==64);pointer=p.tagged(16,tag);p.add(p.tags(0,64,tag));p.add(pointer+m.c(77)+m.mem(0x36))
 value_type=g.I64 if bits==64 else g.I32;types=[]
 if kind=='full':types=[g.struct_type(value_type)];carried=pointer+g.gc(0,1)+g.gc(2,1,0)
 elif kind=='i31':carried=pointer+g.gc(28)+g.gc(30)
 elif kind=='wrap':carried=pointer+b'\xa7\xad'
 else:types=[g.array_type(g.I8)];carried=pointer+m.c(1)+g.gc(6,1)+m.c(0)+g.gc(13,1)
 p.eq(carried+m.mem(0x28),77)
 return g.make_module(p.code,types,memory=bits)

def main():
 p=argparse.ArgumentParser();p.add_argument('--vm',required=True);p.add_argument('--runner',default='');p.add_argument('--out',required=True,type=Path);a=p.parse_args();a.out.mkdir(parents=True,exist_ok=False);rows=[]
 for kind,bits,tag,loss in [('full',32,15,False),('full',64,250,False),('i31',32,7,False),('i31',32,15,True),('wrap',64,250,True),('packed',32,15,True)]:
  name=f'{kind}-{bits}-{tag}';file=a.out/(name+'.wasm');file.write_bytes(fixture(kind,bits,tag))
  for engine in ['int','aot']:
   for action in ['crash','silent-log']:
    cmd=shlex.split(a.runner)+[a.vm,'-R'+engine,'-Rct','0','-WFE-gc','-WFE-function-references','-WFE-reference-types','-WFE-memory64',*(['--runtime-memtag-action','crash'] if action=='crash' else m.report_flags('err'))]
    if engine=='aot':cmd+=['--runtime-llvm-jit-cache-path','disable']
    cmd+=['--run',str(file)];r=subprocess.run(cmd,capture_output=True,text=True,timeout=90)
    expected=122 if loss and action=='crash' else 0;reports=[json.loads(l) for l in r.stderr.splitlines() if l.startswith('{"schema":"uwvm.memtag.violation.v1"')]
    ok=r.returncode==expected and len(reports)==(1 if loss and action=='silent-log' else 0)
    label=name+'-'+engine+'-'+action;(a.out/(label+'.log')).write_text(r.stdout+r.stderr);rows.append(dict(name=label,argv=cmd,exit=r.returncode,expected=expected,reports=len(reports),passed=bool(ok)))
    (a.out/'report.json').write_text(json.dumps(dict(passed=all(x['passed'] for x in rows),rows=rows),indent=2))
    if not ok:raise RuntimeError(label+': '+r.stderr[-1800:])
 print(json.dumps(dict(passed=True,executions=len(rows))))
if __name__=='__main__':main()
