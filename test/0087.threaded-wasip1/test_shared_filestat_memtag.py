from pathlib import Path
import argparse,subprocess,json,hashlib
p=argparse.ArgumentParser();p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--clang',type=Path,required=True);p.add_argument('--wasm-tools',type=Path,required=True);p.add_argument('--profile',choices=['ordinary','shared','all'],default='all');a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
source=Path(__file__).with_name('shared_filestat_memtag.c');mount=a.out/'host-directory';mount.mkdir(exist_ok=True)
owner=a.out/'owner.wat';owner.write_text('(module(memory(export "memory") 2 4 shared))')
subprocess.run([str(a.wasm_tools),'parse',str(owner),'-o',str(owner.with_suffix('.wasm'))],check=True,capture_output=True)
rows=[]
for threaded in [False,True]:
 if a.profile!='all' and threaded!=(a.profile=='shared'):continue
 for case in range(4):
  prefix=f'case{case}-'+('shared' if threaded else 'ordinary');wasm=a.out/(prefix+'.wasm')
  compile=[str(a.clang),'--target=wasm32-unknown-unknown','-O1','-ffreestanding','-nostdlib','-Wl,--no-entry','-Wl,--export=_start','-Wl,--export=wasi_thread_start','-DCASE='+str(case),str(source),'-o',str(wasm)]
  if threaded:compile+=['-matomics','-mbulk-memory','-Wl,--shared-memory','-Wl,--max-memory=262144','-Wl,--import-memory']
  result=subprocess.run(compile,capture_output=True);(a.out/(prefix+'.compile.log')).write_bytes(result.stdout+result.stderr)
  if result.returncode:raise RuntimeError(result.stderr.decode())
  for disabled in [False,True]:
   for engine in ['int','aot']:
    name=prefix+'-'+engine+('-disabled' if disabled else '-enabled');function='fd_filestat_get'
    cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-interface','main',function,'deny' if case==3 else 'allow','--wasip1-single-mount-dir','main','/sandbox',str(mount),'--wasip1-single-mount-dir-rights','main','/sandbox','2097152','0']
    if disabled:cmd+=['--runtime-disable-memtag']
    if threaded:cmd+=['-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','262144','--wasm-preload-library',str(owner.with_suffix('.wasm')),'env']
    cmd+=['--run',str(wasm)]
    try:r=subprocess.run(cmd,capture_output=True,timeout=60);code=r.returncode;stdout=r.stdout;stderr=r.stderr
    except subprocess.TimeoutExpired as e:code=None;stdout=e.stdout or b'';stderr=e.stderr or b''
    expected=122 if case in (1,2) and not disabled else 0
    passed=code==expected and stdout==b'' and (b'Memory tag mismatch' in stderr if expected==122 else b'Memory tag mismatch' not in stderr)
    rows.append(dict(name=name,exit=code,expected=expected,passed=passed,argv=cmd,compile=compile,stderr=stderr.decode(errors='replace'),wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest()))
    (a.out/'report.json').write_text(json.dumps(rows,indent=2));(a.out/(name+'.stderr')).write_bytes(stderr);print(name,code,'PASS' if passed else 'FAIL',flush=True)
    if not passed:print(stderr.decode(errors='replace'),flush=True)
# The compiler-produced shared memory has max=4 pages. Preserve the earlier
# insufficient-grant failure as a distinct admission negative, never a tag trap.
for engine in ([] if a.profile=='ordinary' else ['int','aot']):
 good=next(r for r in rows if r['name']==f'case0-shared-{engine}-enabled')
 cmd=list(good['argv']);i=cmd.index('--wasi-threads');assert cmd[i+6]=='262144';cmd[i+6]='65536'
 run=subprocess.run(cmd,capture_output=True,timeout=60)
 passed=(run.returncode==126 and b'termination_kind=4, code=22' in run.stderr
         and b'Memory tag mismatch' not in run.stderr and run.stdout==b'')
 name='shared-memory-budget-refused-'+engine
 rows.append(dict(name=name,exit=run.returncode,expected=126,passed=passed,argv=cmd,
                  stderr=run.stderr.decode(errors='replace'),wasm_sha256=good['wasm_sha256']))
 (a.out/'report.json').write_text(json.dumps(rows,indent=2));(a.out/(name+'.stderr')).write_bytes(run.stderr)
 print(name,run.returncode,'PASS' if passed else 'FAIL',flush=True)
raise SystemExit(any(not r['passed'] for r in rows))
