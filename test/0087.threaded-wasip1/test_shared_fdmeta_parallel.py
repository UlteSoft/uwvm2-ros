from pathlib import Path
import argparse,subprocess,json,hashlib,time
p=argparse.ArgumentParser();p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--clang',type=Path,required=True);p.add_argument('--wasm-tools',type=Path,required=True);a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
source=Path(__file__).with_name('shared_fdmeta_parallel.c');wasm=a.out/'parallel.wasm';mount=a.out/'directory';mount.mkdir(exist_ok=True)
cmd=[str(a.clang),'--target=wasm32-unknown-unknown','-O1','-ffreestanding','-nostdlib','-matomics','-mbulk-memory','-Wl,--shared-memory','-Wl,--max-memory=262144','-Wl,--import-memory','-Wl,--no-entry','-Wl,--export=_start','-Wl,--export=wasi_thread_start',str(source),'-o',str(wasm)]
run=subprocess.run(cmd,capture_output=True);(a.out/'compile.log').write_bytes(run.stdout+run.stderr)
if run.returncode:raise RuntimeError(run.stderr.decode())
owner=a.out/'owner.wat';owner.write_text('(module(memory(export "memory") 2 4 shared))');subprocess.run([str(a.wasm_tools),'parse',str(owner),'-o',str(owner.with_suffix('.wasm'))],check=True,capture_output=True)
rows=[]
for engine in ['int','aot']:
 for disabled in [False,True]:
  name=engine+('-disabled' if disabled else '-enabled')
  cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','262144','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-grant','main','thread-spawn']
  for interface in ['thread-spawn','fd_fdstat_get','fd_fdstat_set_rights']:cmd+=['--wasip1-single-interface','main',interface,'allow']
  cmd+=['--wasip1-single-mount-dir','main','/sandbox',str(mount),'--wasip1-single-mount-dir-rights','main','/sandbox','9216','66','--wasm-preload-library',str(owner.with_suffix('.wasm')),'env']
  if disabled:cmd+=['--runtime-disable-memtag']
  cmd+=['--run',str(wasm)];start=time.monotonic()
  try:r=subprocess.run(cmd,capture_output=True,timeout=120);code=r.returncode;out=r.stdout;err=r.stderr
  except subprocess.TimeoutExpired as e:code=None;out=e.stdout or b'';err=e.stderr or b''
  passed=code==0 and not out and not err
  rows.append(dict(name=name,exit=code,expected=0,passed=passed,seconds=time.monotonic()-start,argv=cmd,wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest(),stderr=err.decode(errors='replace')));(a.out/'report.json').write_text(json.dumps(rows,indent=2));print(name,code,'PASS' if passed else 'FAIL',flush=True)
  if not passed:print(err.decode(errors='replace'),flush=True)
raise SystemExit(any(not row['passed'] for row in rows))
