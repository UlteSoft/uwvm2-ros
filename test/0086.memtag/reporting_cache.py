#!/usr/bin/env python3
"""Do not mix returning diagnostic IR with terminal MTE IR in the object cache.
Cache admission refusals are recorded, never relabelled as verified cache hits.
"""
import argparse,json,subprocess
from pathlib import Path
import vm_matrix as m

def main():
 p=argparse.ArgumentParser();p.add_argument('--vm',required=True);p.add_argument('--out',required=True,type=Path);a=p.parse_args();a.out.mkdir(mode=0o700,parents=True,exist_ok=False)
 wasm=(a.out/'fault.wasm').resolve();program=m.Program();program.add(program.tags());program.add(program.tags(16,16,4));program.add(b'\x20\0\x04\x40'+program.tagged(0)+m.c(17,64)+m.mem(0x37,15)+b'\x0b')
 body=b'\0'+program.code+b'\x0b'
 wasm.write_bytes(b'\0asm\x01\0\0\0'+m.sec(1,b'\x01\x60\x01\x7f\0')+m.sec(3,b'\x01\0')+m.sec(5,b'\x01\x01\x01\x02')+m.sec(7,b'\x01\x06_start\0\0')+m.sec(10,b'\x01'+m.u(len(body))+body));cache=(a.out/'cache').resolve();cache.mkdir(mode=0o700);rows=[]
 for index,(action,trigger) in enumerate([('crash',0),('silent-log',0),('crash',1),('silent-log',1),('crash',1),('silent-log',1)]):
  report_path=a.out/(str(index)+'-violations.jsonl')
  cmd=[a.vm,'-Raot','-Rct','0','--runtime-llvm-jit-cache-seal-process','-Rllvm-cache-path','path',str(cache),'--log-verbose','--runtime-compiler-log','err',*(['--runtime-memtag-action','crash'] if action=='crash' else m.report_flags(report_path)),'--wasm-set-start-func','0',str(trigger),'--run',str(wasm)]
  r=subprocess.run(cmd,capture_output=True,text=True,timeout=120);text=r.stdout+r.stderr;(a.out/(str(index)+'-'+action+'.log')).write_text(text)
  reports=[json.loads(l) for l in report_path.read_text().splitlines()] if report_path.exists() else []
  hit='object-cache-hit module=' in text
  passed=r.returncode==(122 if action=='crash' and trigger else 0) and (len(reports)>0 if action=='silent-log' and trigger else not reports)
  rows.append(dict(action=action,argv=cmd,exit=r.returncode,reports=len(reports),cache_hit=hit,passed=bool(passed)))
  (a.out/'report.json').write_text(json.dumps(dict(semantic_checks_passed=all(x['passed'] for x in rows),cache_hits=sum(x['cache_hit'] for x in rows),cache_qualified=len(rows)==6 and all(x['cache_hit'] for x in rows[2:]),rows=rows),indent=2))
  if not passed:raise RuntimeError(str(index)+' '+text[-1800:])
 print(json.dumps(dict(semantic_checks_passed=True,cache_hits=sum(x['cache_hit'] for x in rows))))
if __name__=='__main__':main()
