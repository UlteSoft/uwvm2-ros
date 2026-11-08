from pathlib import Path
import argparse,subprocess,json,hashlib,time
p=argparse.ArgumentParser()
p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True)
p.add_argument('--wasm-tools',type=Path,required=True)
a=p.parse_args();a.vm=a.vm.resolve();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
mount=a.out/'host-directory';mount.mkdir(exist_ok=True)
def compile_wat(path,text):
 path.write_text(text);wasm=path.with_suffix('.wasm')
 subprocess.run([str(a.wasm_tools),'parse',str(path),'-o',str(wasm)],check=True,capture_output=True)
 return wasm
owner=compile_wat(a.out/'owner.wat','(module (memory (export "memory") 1 1 shared))')
def c(value):return f'(i32.const {value})'
def check(expr,value):return '(call $check '+expr+' '+c(value)+')'
def read8(offset):return '(i32.load8_u '+c(offset)+')'
rows=[]
for threaded in [False,True]:
 for engine in ['int','aot']:
  for function in ['fd_prestat_get','fd_prestat_dir_name']:
   for case in ['hidden','deny','allow','absent','bad-fd','stdio-denied','stdio-nondir','short','oversized','long-name','misaligned','bad-range','deny-bad-range']:
    if function=='fd_prestat_get' and case in ['short','oversized']:continue
    if function=='fd_prestat_dir_name' and case=='misaligned':continue
    name=function+'-'+case+'-'+engine+('-shared' if threaded else '-ordinary')
    guest_name='/'+'x'*5000 if case=='long-name' else '/sandbox'
    n=len(guest_name);fd=0x7fffffff if case=='bad-fd' else 1 if case.startswith('stdio') else 3
    destination=65532 if case in ['bad-range','deny-bad-range'] else 33 if case=='misaligned' else 32
    capacity=n-1 if case=='short' else n+8 if case=='oversized' else n
    state='hidden' if case=='hidden' else 'deny' if case.startswith('deny') else 'allow'
    errno=76 if state=='deny' or case=='stdio-denied' else 8 if case in ['absent','bad-fd'] else 54 if case=='stdio-nondir' else 37 if case=='short' else 28 if case=='misaligned' else 0
    expected=127 if state=='hidden' else (121 if threaded else 21) if case=='bad-range' else errno
    params='(param i32 i32)' if function=='fd_prestat_get' else '(param i32 i32 i32)'
    call='(call $query '+c(fd)+c(destination)+(c(capacity) if function=='fd_prestat_dir_name' else '')+')'
    body='(i64.store '+c(32)+'(i64.const -1))(i64.store '+c(40)+'(i64.const -1))'
    if case=='bad-range' or state!='allow' or errno:body+='(call $exit '+call+')'
    else:
     body+=check(call,0)
     if function=='fd_prestat_get':
      body+=check('(i32.load '+c(32)+')',0)+check('(i32.load '+c(36)+')',n)
     else:
      # Compare every output byte with the actual granted virtual name.
      body+=check(read8(32),ord('/'))
      if case=='long-name':
       body+='(local.set $i (i32.const 1))(block $done (loop $next (br_if $done (i32.ge_u (local.get $i) '+c(n)+'))'+check('(i32.load8_u (i32.add '+c(32)+'(local.get $i)))',ord('x'))+'(local.set $i (i32.add (local.get $i)(i32.const 1)))(br $next)))'
      else:
       for i,value in enumerate(guest_name.encode()[1:],1):body+=check(read8(32+i),value)
      if case=='oversized':body+=check(read8(32+n),255)
    memory='(import "env" "memory" (memory 1 1 shared))' if threaded else '(memory 1)'
    wat='(module (import "wasi_snapshot_preview1" "'+function+'" (func $query '+params+'(result i32)))(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))'+memory+'(func $check(param $got i32)(param $expected i32)(if(i32.ne(local.get $got)(local.get $expected))(then(call $exit(i32.const 101)))))(func(export "_start")(local $i i32)'+body+')(func(export "wasi_thread_start")(param i32 i32)))'
    wasm=compile_wat(a.out/(name+'.wat'),wat)
    cmd=[str(a.vm),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-interface','main','proc_exit','allow','--wasip1-single-grant','main','proc-exit','--wasip1-single-interface','main',function,state]
    if case!='absent':
     cmd+=['--wasip1-single-mount-dir','main',guest_name,str(mount),'--wasip1-single-mount-dir-rights','main',guest_name,'0','0']
    if case=='stdio-nondir':cmd+=['--wasip1-single-grant','main','stdout.write']
    if threaded:cmd+=['-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','65536','--wasm-preload-library',str(owner),'env']
    cmd+=['--run',str(wasm)]
    begin=time.monotonic()
    try:r=subprocess.run(cmd,capture_output=True,timeout=60);code=r.returncode;stdout=r.stdout;stderr=r.stderr
    except subprocess.TimeoutExpired as e:code=None;stdout=e.stdout or b'';stderr=e.stderr or b''
    passed=code==expected and stdout==b''
    if case=='bad-range' and threaded:passed=passed and b'"exit_status":121' in stderr
    row=dict(name=name,threaded=threaded,engine=engine,function=function,case=case,exit=code,expected=expected,passed=passed,seconds=time.monotonic()-begin,argv=cmd,stderr=stderr.decode(errors='replace'),wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest())
    rows.append(row);(a.out/'report.json').write_text(json.dumps(rows,indent=2));(a.out/(name+'.stderr')).write_bytes(stderr)
    print(name,code,'PASS' if passed else 'FAIL',flush=True)
    if not passed:print(stderr.decode(errors='replace'),flush=True)
raise SystemExit(any(not r['passed'] for r in rows))
