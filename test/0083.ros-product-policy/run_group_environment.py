#!/usr/bin/env python3
"""Real cross-module argv/env/memory/FD sharing and isolation, not policy mocks."""
import argparse,json,resource,shlex,subprocess
from pathlib import Path
APIS={'environ_sizes_get':2,'environ_get':2,'args_sizes_get':2,'args_get':2,'fd_fdstat_get':2,'fd_close':1}
def imports():return ''.join(f'(import "wasi_snapshot_preview1" "{name}" (func ${name} (param {" ".join(["i32"]*n)}) (result i32)))' for name,n in APIS.items())
def check(expr,expected=0):return f'{expr} i32.const {expected} i32.ne if unreachable end '
def body(arg,env):
 code=check('i32.const 64 i32.const 68 call $args_sizes_get')+check('i32.const 64 i32.load',2)+check('i32.const 68 i32.load',11)
 code+=check('i32.const 128 i32.const 512 call $args_get')+check('i32.const 128 i32.load',512)+check('i32.const 132 i32.load',518)+check('i32.const 512 i32.load8_u',ord(arg))
 code+=check('i32.const 64 i32.const 68 call $environ_sizes_get')+check('i32.const 64 i32.load',1)+check('i32.const 68 i32.load',10)
 code+=check('i32.const 256 i32.const 768 call $environ_get')+check('i32.const 256 i32.load',768)+check('i32.const 772 i32.load8_u',ord(env))
 return code

def main():
 p=argparse.ArgumentParser();p.add_argument('--vm',required=True);p.add_argument('--runner',default='');p.add_argument('--engines',default='int,aot');p.add_argument('--out',required=True,type=Path);a=p.parse_args();a.out.mkdir(parents=True,exist_ok=False);resource.setrlimit(resource.RLIMIT_CORE,(0,0));rows=[]
 for scope in ['group','singles','groups']:
  same=scope=='group'
  worker_arg='a' if same else 'o';worker_env='V' if same else 'W';main_env='V'
  worker='(module '+imports()+' (memory 1) (func (export "forward") '+body(worker_arg,worker_env)+' i32.const 400 i32.const 7 i32.store '+check('i32.const 1 call $fd_close')+'))'
  main='(module '+imports()+' (import "worker" "forward" (func $forward)) (memory 1) (func (export "_start") '+body('a',main_env)+' i32.const 400 i32.const 9 i32.store '+check('i32.const 1 i32.const 32 call $fd_fdstat_get')+' call $forward '+check('i32.const 400 i32.load',9)+check('i32.const 1 i32.const 32 call $fd_fdstat_get',8 if same else 0)+'))'
  files=[]
  for name,source in [('worker',worker),('main',main)]:
   wat=a.out/(scope+'-'+name+'.wat');wat.write_text(source);wasm=wat.with_suffix('.wasm');subprocess.run(['wat2wasm',str(wat),'-o',str(wasm)],check=True);files.append(wasm)
  flags=[]
  targets=[('group','team',['main','worker'],'alpha','VALUE')] if same else [('group' if scope=='groups' else 'single',name,[name],'alpha' if name=='main' else 'omega','VALUE' if name=='main' else 'WORLD') for name in ['main','worker']]
  for kind,target,members,arg,env in targets:
   prefix='--wasip1-'+kind+'-';flags += [prefix+'create',target,prefix+'enable',target]
   if kind=='group':
    for member in members:flags += [prefix+'add-module',target,member]
   for api in APIS:flags += [prefix+'interface',target,api,'allow']
   for permission in ['stdout.write','stdio.metadata','args','environ']:flags += [prefix+'grant',target,permission]
   flags += [prefix+'force-args',target,'2',arg,'beta']
   flags += [prefix+'add-or-replace-environment',target,'KEY',env]
  for engine in a.engines.split(','):
   cmd=shlex.split(a.runner)+[a.vm,'-R'+engine,'-Rct','0','-Wname','main',*flags,'--wasm-preload-library',str(files[0]),'worker']
   if engine=='aot':cmd+=['--runtime-llvm-jit-cache-path','disable']
   cmd+=['--run',str(files[1])];r=subprocess.run(cmd,capture_output=True,text=True,timeout=90)
   name=scope+'-'+engine;(a.out/(name+'.log')).write_text(r.stdout+r.stderr);ok=r.returncode==0
   rows.append(dict(name=name,argv=cmd,exit=r.returncode,passed=ok));(a.out/'report.json').write_text(json.dumps(dict(passed=all(x['passed'] for x in rows),rows=rows),indent=2))
   if not ok:raise RuntimeError(name+': '+r.stderr[-3000:])
 print(json.dumps(dict(passed=True,executions=len(rows))))
if __name__=='__main__':main()
