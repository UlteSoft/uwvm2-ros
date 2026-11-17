#!/usr/bin/env python3
"""Measure receiver ownership admission versus its native arbitration core.

This fixed-clock hot-loop benchmark measures added identity/sequence/deadline
checks, not IPC, physical device latency or Wasm/native equivalence. Both modes
retain final ownership and effect checks; -O3 -g assertions remain enabled.
"""
import argparse,json,statistics,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'test/0084.ros-runtime'))
from run_scene_vm import verify_gate,sha,GATE

def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);a=p.parse_args();verify_gate()
 out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
 fixture=ROOT/'test/0084.ros-runtime/driver_control_performance.cpp';header=ROOT/'src/uwvm2/imported/ros/scene/driver_control.h';core=header.with_name('control_arbitration.h')
 report=dict(complete=False,scope=__doc__,cgroup=str(GATE),events_before=(GATE/'memory.events').read_text(),inputs={str(v):sha(v) for v in (Path(__file__),fixture,header,core)},commands=[],trials=[])
 def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
 def run(label,argv):
  verify_gate();argv=list(map(str,argv));r=subprocess.run(argv,cwd=ROOT,capture_output=True,timeout=180);(out/(label+'.stdout')).write_bytes(r.stdout);(out/(label+'.stderr')).write_bytes(r.stderr)
  report['commands'].append(dict(label=label,argv=argv,exit=r.returncode));save();assert r.returncode==0,(label,r.stderr[-3000:]);return r.stdout
 try:
  run('compile',['/usr/bin/clang++-22','-std=c++23','-O3','-g','-UNDEBUG','-pthread','-Wall','-Wextra','-Werror','-Isrc','-isystem','third-parties/fast_io/include',fixture,'-o',out/'benchmark'])
  report['binary_sha256']=sha(out/'benchmark')
  for n in range(11):
   for kind in (['arbiter','receiver'] if n%2==0 else ['receiver','arbiter']):
    report['trials'].append(json.loads(run(f'{n}-{kind}',['taskset','-c','2',out/'benchmark',kind,'1000000'])))
  report['median_ns']={kind:statistics.median(t['ns_per_pair'] for t in report['trials'] if t['mode']==kind) for kind in ('arbiter','receiver')}
  report['incremental_ns']=report['median_ns']['receiver']-report['median_ns']['arbiter']
  assembly=run('assembly',['/usr/bin/objdump','-Cd','--no-show-raw-insn',out/'benchmark']).decode()
  assert 'receiver_control::begin' in assembly and 'receiver_control::finish' in assembly
  report['assembly_sha256']=sha(out/'assembly.stdout');report['complete']=True;print(json.dumps(report['median_ns']))
 finally:
  report['events_after']=(GATE/'memory.events').read_text();report['inputs_unchanged']=all(sha(path)==digest for path,digest in report['inputs'].items());report['complete']=report['complete'] and report['inputs_unchanged'];save()
if __name__=='__main__':main()
