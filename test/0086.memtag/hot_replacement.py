#!/usr/bin/env python3
"""Actual host API hot replacement: startup policy survives new JIT generation."""
import argparse,hashlib,json,subprocess
from pathlib import Path
import vm_matrix as m
def fixture(wide):
 p=m.Program(wide)
 root=p.tags(0,32,3)+p.tagged(0)+m.c(42)+m.mem(0x36)+b'\x10\x01'
 worker=p.tagged(0)+m.mem(0x28)
 bodies=[b'\0'+x+b'\x0b' for x in [root,worker]]
 wasm=b'\0asm\x01\0\0\0'+m.sec(1,b'\x01\x60\0\x01\x7f')+m.sec(3,b'\x02\0\0')
 wasm+=m.sec(5,b'\x01'+m.u(5 if wide else 1)+b'\x01\x02')
 wasm+=m.sec(10,b'\x02'+b''.join(m.u(len(x))+x for x in bodies))
 return wasm
def replacement(wide,kind):
 p=m.Program(wide)
 tag=4 if kind=='mismatch' else 3
 code=p.tagged(0,tag)+m.mem(0x28)
 # Distinguishes original42 from replacement143/145 and queries the new body policy.
 code+=m.op(0x20)+(b'\xa7' if wide else b'')+m.c(101)+b'\x6a\x6a'
 if kind=='zero':
  code=p.tagged(0)+p.n(32)+m.op(0x2c)+p.tagged(0)+m.mem(0x28)+m.c(17)+b'\x6a'
 return b'\0'+code+b'\x0b'
def main():
 a=argparse.ArgumentParser();a.add_argument('--fixture',required=True);a.add_argument('--out',required=True)
 a.add_argument('--timeout',type=int,default=180);args=a.parse_args()
 out=Path(args.out);out.mkdir(parents=True,exist_ok=True);cases=[]
 for wide in [False,True]:
  wasm=out/('input'+str(64 if wide else 32)+'.wasm');wasm.write_bytes(fixture(wide))
  for kind in ['read','mismatch','zero']:
   body=out/(wasm.stem+'-'+kind+'.body');body.write_bytes(replacement(wide,kind))
   for enabled in [True,False]:
    expected=17 if kind=='zero' else 145 if enabled else 143
    cmd=[args.fixture,str(wasm),str(body),'enabled' if enabled else 'disabled',str(expected)]
    name=wasm.stem+'-'+kind+('-enabled' if enabled else '-disabled')
    try:
     r=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=args.timeout)
     text=r.stdout.decode('utf-8','replace');exit_code=r.returncode
     bad=kind=='mismatch' and enabled
     passed=exit_code==(122 if bad else 0) and ('memory tag mismatch' in text.lower() if bad else 'memtag replacement PASS' in text)
    except subprocess.TimeoutExpired as e:
     text=(e.stdout or b'').decode('utf-8','replace');exit_code=None;passed=False
    (out/(name+'.log')).write_text(text)
    cases.append(dict(name=name,argv=cmd,exit=exit_code,passed=passed,
      wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest(),body_sha256=hashlib.sha256(body.read_bytes()).hexdigest()))
    print(name,'PASS' if passed else 'FAIL',exit_code,flush=True)
    (out/'report.json').write_text(json.dumps(dict(cases=cases,passed=sum(x['passed'] for x in cases),total=len(cases)),indent=2))
 return int(not all(x['passed'] for x in cases))
if __name__=='__main__':raise SystemExit(main())
