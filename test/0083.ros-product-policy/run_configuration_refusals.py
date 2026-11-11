#!/usr/bin/env python3
"""Launch errors are controlled refusals; valid empty modules stay valid Wasm."""
import argparse,json,resource,shlex,subprocess
from pathlib import Path

def main():
 p=argparse.ArgumentParser();p.add_argument('--vm',required=True);p.add_argument('--runner',default='');p.add_argument('--engine',default='int');p.add_argument('--out',required=True,type=Path);p.add_argument('--observe',action='store_true');a=p.parse_args();a.out.mkdir(parents=True,exist_ok=False)
 resource.setrlimit(resource.RLIMIT_CORE,(0,0));rows=[]
 def wat(name,source):
  path=a.out/(name+'.wat');path.write_text(source);binary=path.with_suffix('.wasm');subprocess.run(['wat2wasm',str(path),'-o',str(binary)],check=True);return binary
 good=wat('good','(module (memory 1 1) (func (export "_start")))')
 noentry=wat('noentry','(module (func (export "f")))')
 tiny=[]
 for name,data in [('empty',b'\0asm\x01\0\0\0'),('type-only',bytes.fromhex('0061736d01000000010401600000')),('truncated',bytes.fromhex('0061736d010000000104016000'))]:
  file=a.out/(name+'.wasm');file.write_bytes(data);tiny.append((name,file))
 cases=[('no-entry',[],noentry,126),('unknown-reset',['-Wresimp','ghost','a','b','c','d'],good,126),('unmatched-reset',['-Wresimp','main','a','b','c','d'],good,126),('negative-workers',['-Rct','-2000000'],good,126),('warning-escalation',['-Rct','2000000','--warning-as-error','runtime-compile-threads'],good,126),('memory-warning-escalation',['-Wmemlim','main','all','2','2','--warning-as-error','runtime'],good,126),('memory-widen-control',['-Wmemlim','main','all','2','2'],good,0),('memory-limit',['-Wmemlim','main','all','100000','100000'],good,126)]
 base=shlex.split(a.runner)+[a.vm,'-R'+a.engine,'-Wname','main']
 if a.engine=='aot':base+=['--runtime-llvm-jit-cache-path','disable']
 def run(name,cmd,expected):
  r=subprocess.run(cmd,capture_output=True,text=True,timeout=45);(a.out/(name+'.log')).write_text(r.stdout+r.stderr)
  passed=r.returncode==expected;rows.append(dict(name=name,argv=cmd,exit=r.returncode,expected=expected,passed=passed));(a.out/'report.json').write_text(json.dumps(dict(passed=all(x['passed'] for x in rows),rows=rows),indent=2))
  if not passed and not a.observe:raise RuntimeError(name+': '+str(r.returncode)+' '+r.stderr[-1500:])
 for name,options,file,status in cases:run(name,base+options+['--run',str(file)],status)
 for name,file in tiny:
  run(name+'-validation',base+['--mode','validation','--run',str(file)],127 if name=='truncated' else 0)
  if name!='truncated':run(name+'-execution',base+['--run',str(file)],126)
 for option in ['--wasm-list-weak-symbol-module','-Wlsweak']:
  run('removed-'+option.replace('-',''),base+[option],126)
 print(json.dumps(dict(passed=all(x['passed'] for x in rows),executions=len(rows))))
if __name__=='__main__':main()
