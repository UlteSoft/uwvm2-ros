from pathlib import Path
import argparse,subprocess,json,hashlib,time
p=argparse.ArgumentParser();p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--clang',type=Path,required=True);p.add_argument('--wasm-tools',type=Path,required=True);p.add_argument('--prepare-only',action='store_true');p.add_argument('--reuse-fixtures',action='store_true');a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
source=Path(__file__).with_name('shared_filestat_parallel.c');mount=a.out/'directory';mount.mkdir(exist_ok=True)
owner=a.out/'owner.wat';owner.write_text('(module(memory(export "memory") 2 4 shared))');subprocess.run([str(a.wasm_tools),'parse',str(owner),'-o',str(owner.with_suffix('.wasm'))],check=True,capture_output=True)
rows=[];fixture_path=a.out/'fixtures.json';source_sha=hashlib.sha256(source.read_bytes()).hexdigest();previous=json.loads(fixture_path.read_text()) if a.reuse_fixtures and fixture_path.exists() else {};fixtures={'source_sha256':source_sha,'cases':{}}
for case in (0,1):
 wasm=a.out/('case'+str(case)+'.wasm');compile=[str(a.clang),'--target=wasm32-unknown-unknown','-O1','-ffreestanding','-nostdlib','-matomics','-mbulk-memory','-Wl,--shared-memory','-Wl,--max-memory=262144','-Wl,--import-memory','-Wl,--no-entry','-Wl,--export=_start','-Wl,--export=wasi_thread_start','-DCASE='+str(case),str(source),'-o',str(wasm)]
 saved=previous.get('cases',{}).get(str(case),{});reuse=(previous.get('source_sha256')==source_sha and saved.get('compile')==compile and wasm.exists() and saved.get('sha256')==hashlib.sha256(wasm.read_bytes()).hexdigest())
 if not reuse:
  run=subprocess.run(compile,capture_output=True);(a.out/('case'+str(case)+'.compile.log')).write_bytes(run.stdout+run.stderr)
  if run.returncode:raise RuntimeError(run.stderr.decode())
 fixtures['cases'][str(case)]={'sha256':hashlib.sha256(wasm.read_bytes()).hexdigest(),'compile':compile};fixture_path.write_text(json.dumps(fixtures,indent=2));print('fixture',case,'reused' if reuse else 'compiled',flush=True)
 if a.prepare_only:continue
 for engine in ['int','aot']:
  for disabled in [False,True]:
   name='case'+str(case)+'-'+engine+('-disabled' if disabled else '-enabled')
   cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','262144','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-grant','main','thread-spawn']
   for interface in ['thread-spawn','fd_filestat_get','fd_fdstat_set_rights','fd_renumber']:cmd+=['--wasip1-single-interface','main',interface,'allow']
   cmd+=['--wasip1-single-mount-dir','main','/sandbox',str(mount),'--wasip1-single-mount-dir-rights','main','/sandbox','2097152','0','--wasm-preload-library',str(owner.with_suffix('.wasm')),'env']
   if case==1:cmd+=['--wasip1-single-grant','main','stdout.write','--wasip1-single-grant','main','stdio.metadata']
   if disabled:cmd+=['--runtime-disable-memtag']
   cmd+=['--run',str(wasm)];start=time.monotonic()
   try:r=subprocess.run(cmd,capture_output=True,timeout=120);code=r.returncode;out=r.stdout;err=r.stderr
   except subprocess.TimeoutExpired as e:code=None;out=e.stdout or b'';err=e.stderr or b''
   lines=err.splitlines();diagnostic_ok=(1<=len(lines)<=2 and all(line.startswith(b'uwvm: [warning]') and line.endswith(b'[host_denied_wasip1.fd_filestat_get]') for line in lines)) if case==0 else not err
   passed=code==0 and not out and diagnostic_ok
   rows.append(dict(name=name,exit=code,expected=0,passed=passed,fixture_reused=reuse,seconds=time.monotonic()-start,argv=cmd,compile=compile,wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest(),stderr=err.decode(errors='replace')));(a.out/'report.json').write_text(json.dumps(rows,indent=2));print(name,code,'PASS' if passed else 'FAIL',flush=True)
   if not passed:print(err.decode(errors='replace'),flush=True)
raise SystemExit(any(not row['passed'] for row in rows))
