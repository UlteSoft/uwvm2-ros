#!/usr/bin/env python3
"""Actual CLI owner/alias/global-section regression cases; run in the resource cgroup."""
import argparse,json,subprocess,hashlib
from pathlib import Path
from vm_matrix import u,c,sec,op,mem,assertion
from vm_diagnostics import fault_matches
HEADER=b"\0asm\x01\0\0\0"
def text(s):
 b=s.encode();return u(len(b))+b
def exports(items):
 return sec(7,u(len(items))+b"".join(text(name)+bytes([kind])+u(index) for name,kind,index in items))
def bodies(items):
 return sec(10,u(len(items))+b"".join(u(len(code)+2)+b"\0"+code+b"\x0b" for code in items))
def owner():
 return HEADER+sec(1,b"\x02\x60\x01\x7f\0\x60\x01\x7f\x01\x7f")+sec(3,b"\x02\0\x01")+sec(5,b"\x01\x01\x01\x02")+exports([("mem",2,0),("mark",0,0),("read",0,1)])+bodies([b"\x20\0"+c(16)+op(0x2b),b"\x20\0"+mem(0x28)])
def alias():
 return HEADER+sec(2,b"\x01"+text("owner")+text("mem")+b"\x02\x01\x01\x02")+exports([("mem",2,0)])
def consumer(through_alias=False,bad=False,callee=False):
 imports=[text("owner")+text("mark")+b"\0\0"]
 if callee:imports.append(text("owner")+text("read")+b"\0\x01")
 imports.append(text("alias" if through_alias else "owner")+text("mem")+b"\x02\x01\x01\x02")
 code=c(0x70000020)+b"\x10\0"+c(0x70000020)+c(42)+mem(0x36)
 pointer=c(32 if bad else 0x70000020)
 code+=assertion(pointer+(b"\x10\x01" if callee else mem(0x28)),42)
 code+=assertion(c(32)+mem(0x28,index=1),0)
 return HEADER+sec(1,b"\x03\x60\x01\x7f\0\x60\x01\x7f\x01\x7f\x60\0\0")+sec(2,u(len(imports))+b"".join(imports))+sec(3,b"\x01\x02")+sec(5,b"\x01\x01\x01\x02")+exports([("_start",0,2 if callee else 1)])+bodies([code])
def large(tagged=False,immediate=False):
 code=(b"\x43\xfc\x20\xff\x7f\x1a" if immediate else op(0x20,0 if tagged else 1)+b"\x1a")
 code+=c(0x10000000)+c(42)+mem(0x36)+assertion(c(0x10000000)+mem(0x28),42)
 return HEADER+sec(1,b"\x01\x60\0\0")+sec(3,b"\x01\0")+sec(5,b"\x02\x01"+u(4097)+u(4097)+b"\x01\x01\x02")+exports([("_start",0,0)])+bodies([code])
def global_case(payload,duplicate=False,empty_memory=False,section="uwvm.memtag.linear.globals"):
 result=HEADER+sec(1,b"\x01\x60\0\0")+sec(3,b"\x01\0")
 if not empty_memory:result+=sec(5,b"\x01\x01\x01\x02")
 result+=exports([("_start",0,0)])+bodies([b""])+sec(0,text(section)+payload)
 if duplicate:result+=sec(0,text(section)+payload)
 return result
def main():
 p=argparse.ArgumentParser();p.add_argument("--vm",type=Path,required=True);p.add_argument("--out",type=Path,required=True);a=p.parse_args();a.out.mkdir(parents=True,exist_ok=True)
 for name,data in [("owner",owner()),("alias",alias())]:(a.out/(name+".wasm")).write_bytes(data)
 cases=[]
 for mid in [False,True]:
  for bad in [False,True]:
   for callee in [False,True]:
    cases.append((f"alias-{mid}-bad-{bad}-callee-{callee}",consumer(mid,bad,callee),122 if bad else 0,True,False))
 cases += [("ordinary-large-memory",large(),0,False,False),("false-positive-immediate",large(immediate=True),0,False,False),
           ("tagged-unrepresentable",large(tagged=True),127,False,False),
           ("disabled-full-domain",large(tagged=True),0,False,True)]
 for name,payload in [("good",b"\x02\x01\0\x20\x10"),("version",b"\x01\0"),
                      ("range",b"\x02\x01\0"+u(65536)+b"\x01"),("truncated",b"\x02\x01\0\x80"),
                      ("trailing",b"\x02\0\0"),("count",b"\x02\xff\x7f")]:
  cases.append(("globals-"+name,global_case(payload),0 if name=="good" else 127,False,False))
 cases.extend([("globals-duplicate",global_case(b"\x02\0",True),127,False,False),
               ("globals-no-memory",global_case(b"\x02\x01\0\0\0",empty_memory=True),127,False,False),
               ("globals-empty-no-memory",global_case(b"\x02\0",empty_memory=True),0,False,False)])
 for disabled in (False,True):
  cases.append(("globals-obsolete-v1-disabled"+str(int(disabled)),global_case(b"\x01\x01\x20\x10",section="memtag.globals"),127,False,disabled))
 rows=[]
 for name,data,expected,preload,disabled in cases:
  wasm=a.out/(name+".wasm");wasm.write_bytes(data)
  for engine in ["int","aot"]:
   cmd=[str(a.vm),"-R"+engine,"--runtime-compile-threads","0","--wasm-feature-enable-multi-memory"]
   if preload:cmd+=["--wasm-preload-library",str(a.out/"owner.wasm"),"owner","--wasm-preload-library",str(a.out/"alias.wasm"),"alias"]
   if disabled:cmd+=["--runtime-disable-memtag"]
   cmd+=["--run",str(wasm)]
   run=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=120)
   log=a.out/(name+"-"+engine+".log");log.write_bytes(run.stdout)
   output=run.stdout.decode("utf8","replace")
   passed=fault_matches(run.returncode,output,122,"memory tag mismatch") if expected==122 else run.returncode==expected
   if "obsolete-v1" in name:passed=passed and "obsolete memtag.globals" in output and "regenerate" in output
   rows.append(dict(name=name,engine=engine,exit=run.returncode,expected=expected,passed=passed,args=cmd,wasm_sha256=hashlib.sha256(data).hexdigest(),log=str(log)))
   print(name,engine,run.returncode,"PASS" if passed else "FAIL",flush=True)
   (a.out/"report.json").write_text(json.dumps(rows,indent=2))
 if not all(r["passed"] for r in rows):raise SystemExit(1)
if __name__=="__main__":main()
