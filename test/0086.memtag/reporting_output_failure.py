#!/usr/bin/env python3
import vm_matrix as m
"""Report failures must produce a controlled internal exit, including EPIPE."""
import argparse,json,os,shlex,subprocess
from pathlib import Path
import reporting_vm

def main():
 p=argparse.ArgumentParser();p.add_argument('--vm',required=True);p.add_argument('--runner',default='');p.add_argument('--out',required=True,type=Path);a=p.parse_args();a.out.mkdir(parents=True,exist_ok=False)
 wasm=a.out/'bad-tag.wasm';wasm.write_bytes(next(reporting_vm.cases())[1]);rows=[]
 for engine in ('int','aot'):
  for mode in ('sync','software-async'):
   for target in ('full','pipe','missing-parent'):
    output='/dev/full' if target=='full' else 'out' if target=='pipe' else str(a.out/'absent'/'report.jsonl')
    cmd=shlex.split(a.runner)+[a.vm,'-R'+engine,'-Rct','0',*m.mode_flags(mode),*m.report_flags(output)]
    if engine=='aot':cmd+=['--runtime-llvm-jit-cache-path','disable']
    cmd+=['--run',str(wasm)]
    if target=='pipe':
     read,write=os.pipe();os.close(read)
     try:r=subprocess.run(cmd,stdout=write,stderr=subprocess.PIPE,timeout=90)
     finally:os.close(write)
    else:r=subprocess.run(cmd,capture_output=True,timeout=90)
    expected=126 if target=='missing-parent' else 123
    message='Unable to open the independent MTE report output' if target=='missing-parent' else 'cannot write the MTE violation report'
    text=r.stderr.decode(errors='replace');name=engine+'-'+mode+'-'+target;(a.out/(name+'.log')).write_text(text)
    passed=r.returncode==expected and message in text;rows.append(dict(name=name,argv=cmd,exit=r.returncode,expected=expected,passed=passed));(a.out/'report.json').write_text(json.dumps(dict(passed=all(x['passed'] for x in rows),rows=rows),indent=2))
    if not passed:raise RuntimeError(name+': '+text)
 print(json.dumps(dict(passed=True,executions=len(rows))))
if __name__=='__main__':main()
