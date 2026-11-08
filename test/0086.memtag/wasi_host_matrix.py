#!/usr/bin/env python3
from pathlib import Path
import subprocess,json,sys,hashlib,re
import argparse
parser=argparse.ArgumentParser(description='42 actual WASIp1 tagged host-buffer checks; run inside the resource cgroup.')
parser.add_argument('--vm',type=Path,required=True)
parser.add_argument('--out',type=Path,required=True)
inputs=parser.add_mutually_exclusive_group(required=True)
inputs.add_argument('--clang',type=Path,help='existing WasmMTE clang; no installation')
inputs.add_argument('--fixtures',type=Path,help='reuse checked compile-report.json and exact Wasm files')
a=parser.parse_args();vm=a.vm.resolve();out=a.out.resolve();out.mkdir(parents=True,exist_ok=True)
D=a.fixtures.resolve() if a.fixtures else out/'fixtures'
expected=[dict(name=f'{bits}-'+('shared-' if shared else 'ordinary-')+str(case),bits=bits,shared=shared,case=case) for bits,shared in ((32,False),(32,True),(64,False)) for case in range(7)]
if a.clang:
 D.mkdir(parents=True,exist_ok=True);compiled=[];source=Path(__file__).with_name('wasi_host_buffers.c')
 for spec in expected:
  wasm=D/(spec['name']+'.wasm')
  cmd=[str(a.clang.resolve()),'--target=wasm'+str(spec['bits'])+'-unknown-unknown','-O1','-ffreestanding','-nostdlib','-Wl,--no-entry','-Wl,--export=_start','-DCASE='+str(spec['case']),str(source),'-o',str(wasm)]
  if spec['shared']:cmd+=['-matomics','-mbulk-memory','-Wl,--shared-memory','-Wl,--max-memory=131072']
  p=subprocess.run(cmd,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=120)
  (D/(spec['name']+'.compile.log')).write_bytes(p.stdout)
  compiled.append(dict(**spec,compile_exit=p.returncode,args=cmd,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest() if p.returncode==0 else None))
  (D/'compile-report.json').write_text(json.dumps(compiled,indent=2))
  if p.returncode:raise RuntimeError('fixture compilation failed: '+spec['name'])
compiled=json.loads((D/'compile-report.json').read_text())
if len(compiled)!=len(expected):raise ValueError('expected exactly 21 fixture cases')
for spec,contract in zip(compiled,expected):
 if any(spec[k]!=v for k,v in contract.items()):raise ValueError('fixture manifest violates case contract')
 if spec['compile_exit']!=0 or hashlib.sha256((D/(spec['name']+'.wasm')).read_bytes()).hexdigest()!=spec['wasm_sha256']:raise ValueError('fixture compile or hash mismatch')

rows=[]
for spec in compiled:
 for backend in ('int','aot'):
  bits=spec['bits'];shared=spec['shared'];case=spec['case'];wasm=D/(spec['name']+'.wasm')
  name=spec['name']+'-'+backend
  args=[str(vm),'-R'+backend,'-Rct','1','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main']
  if shared:args+=['--wasm-feature-enable-threads']
  if bits==64:args+=['--wasm-feature-enable-memory64','--wasm-feature-enable-table64']
  function='random_get' if case>=5 else 'fd_write'
  # These imports keep the native-width C ABI (64-bit only in wasm64 builds).
  # The compiler source uses import_name explicitly; wasm64 mirrors its name.
  if bits==64:function+='_wasm64'
  args+=['--wasip1-single-interface','main',function,'deny' if case==4 else 'allow','--wasip1-single-grant','main','random' if case>=5 else 'stdout.write','--run',str(wasm)]
  try:p=subprocess.run(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=45);code=p.returncode;stdout=p.stdout;stderr=p.stderr
  except subprocess.TimeoutExpired as error:code=None;stdout=error.stdout or b'';stderr=error.stderr or b''
  (out/(name+'.stdout')).write_bytes(stdout);(out/(name+'.stderr')).write_bytes(stderr)
  mismatch=case in (1,2,3,6)
  if mismatch:passed=code==122 and b'Memory tag mismatch' in stderr and stdout==b'' and b'termination_kind=4' not in stderr
  else:passed=code==0 and stdout==(b'MT-WASI\n' if case==0 else b'') and b'Memory tag mismatch' not in stderr
  row=dict(name=name,bits=bits,shared=shared,backend=backend,case=case,exit=code,expected_exit=122 if mismatch else 0,passed=passed,args=args,stdout=stdout.decode(errors='replace'),stderr=stderr.decode(errors='replace'),wasm_sha256=spec['wasm_sha256'])
  rows.append(row);(out/'report.json').write_text(json.dumps(rows,indent=2));print(name,code,'PASS' if passed else 'FAIL',flush=True)
raise SystemExit(any(not row['passed'] for row in rows))
