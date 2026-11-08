from pathlib import Path
import argparse,hashlib,json,subprocess,time
p=argparse.ArgumentParser();p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--wasm-tools',type=Path,required=True);p.add_argument('--prepare-only',action='store_true');p.add_argument('--reuse-fixtures',action='store_true');a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
fixture_path=a.out/'fixtures.json';previous=json.loads(fixture_path.read_text()) if a.reuse_fixtures and fixture_path.exists() else {};fixtures={}
def make(name,text):
 source=a.out/(name+'.wat');source.write_text(text);target=source.with_suffix('.wasm');wat_sha=hashlib.sha256(source.read_bytes()).hexdigest();old=previous.get(name,{})
 reused=old.get('wat_sha256')==wat_sha and target.exists() and old.get('wasm_sha256')==hashlib.sha256(target.read_bytes()).hexdigest()
 if not reused:subprocess.run([str(a.wasm_tools),'parse',str(source),'-o',str(target)],capture_output=True,check=True)
 fixtures[name]={'wat_sha256':wat_sha,'wasm_sha256':hashlib.sha256(target.read_bytes()).hexdigest(),'reused':reused};fixture_path.write_text(json.dumps(fixtures,indent=2));return target
owner=make('owner','(module(memory(export "memory") 1 1 shared))')
def const(kind,value):
 if kind=='v128':return '(v128.const i32x4 '+' '.join(str(value+i) for i in range(4))+')'
 return '('+kind+'.const '+str(value)+')'
def equal(kind,value):
 actual='(global.get $local)';expected=const(kind,value)
 if kind=='v128':return '(i8x16.all_true(i32x4.eq'+actual+expected+'))'
 return '('+kind+'.eq'+actual+expected+')'
def require(test,code):return '(if(i32.eqz'+test+')(then(call $exit(i32.const '+str(code)+'))))'
rows=[]
for kind in ['i32','i64','f32','f64','v128']:
 values=(0x112233445566778,0x223344556677889,0x33445566778899a) if kind=='i64' else (1.25,2.5,3.75) if kind in ('f32','f64') else (17,29,43)
 for mutable in [False,True]:
  initial,parent,child=values;name=kind+('-mutable' if mutable else '-constant');definition='(global $local '+('(mut '+kind+')' if mutable else kind)+const(kind,initial)+')'
  before='(global.set $local'+const(kind,parent)+')' if mutable else ''
  change='(global.set $local'+const(kind,child)+')' if mutable else ''
  body='(func(export "_start")(local $tid i32)'+before+'(local.set $tid(call $spawn(i32.const 0)))(if(i32.lt_s(local.get $tid)(i32.const 0))(then(call $exit(i32.const 90))))(block $done(loop $wait(br_if $done(i32.atomic.load(i32.const 4096)))(drop(memory.atomic.wait32(i32.const 4096)(i32.const 0)(i64.const -1)))(br $wait)))'+require(equal(kind,parent if mutable else initial),92)+')'
  entry='(func(export "wasi_thread_start")(param i32 i32)'+require(equal(kind,initial),91)+change+'(i32.atomic.store(i32.const 4096)(i32.const 1))(drop(memory.atomic.notify(i32.const 4096)(i32.const 1))))'
  text='(module(import "env" "memory"(memory 1 1 shared))(import "wasi" "thread-spawn"(func $spawn(param i32)(result i32)))(import "wasi_snapshot_preview1" "proc_exit"(func $exit(param i32)))'+definition+body+entry+')';wasm=make(name,text)
  if a.prepare_only:continue
  for engine in ['int','aot']:
   for disabled in [False,True]:
    label=name+'-'+engine+('-disabled' if disabled else '-enabled');command=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','--runtime-llvm-jit-policy','debug','-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','65536','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-interface','main','thread-spawn','allow','--wasip1-single-grant','main','thread-spawn','--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit','--wasm-preload-library',str(owner),'env']
    if disabled:command+=['--runtime-disable-memtag']
    command+=['--run',str(wasm)];begin=time.monotonic()
    try:r=subprocess.run(command,capture_output=True,timeout=120);code=r.returncode;stdout=r.stdout;stderr=r.stderr
    except subprocess.TimeoutExpired as error:code=None;stdout=error.stdout or b'';stderr=error.stderr or b''
    passed=code==0 and not stdout and not stderr;rows.append(dict(name=label,kind=kind,mutable=mutable,engine=engine,memtag_disabled=disabled,exit=code,expected=0,passed=passed,seconds=time.monotonic()-begin,argv=command,wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest(),stderr=stderr.decode(errors='replace')));(a.out/'report.json').write_text(json.dumps(rows,indent=2));print(label,code,'PASS' if passed else 'FAIL',flush=True)
    if not passed:print(stderr.decode(errors='replace'),flush=True)
raise SystemExit(any(not r['passed'] for r in rows))
