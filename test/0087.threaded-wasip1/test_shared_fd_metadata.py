from pathlib import Path
import argparse,subprocess,json,hashlib,time
p=argparse.ArgumentParser();p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--wasm-tools',type=Path,required=True);a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
mount=a.out/'directory';mount.mkdir(exist_ok=True)
def make(name,wat):
 source=a.out/(name+'.wat');source.write_text(wat);target=source.with_suffix('.wasm')
 subprocess.run([str(a.wasm_tools),'parse',str(source),'-o',str(target)],check=True,capture_output=True);return target
owner=make('owner','(module(memory(export "memory") 1 1 shared))')
def c(v):return f'(i32.const {v})'
def l(v):return f'(i64.const {v})'
def call(fn,*values):return '(call $'+fn+''.join(values)+')'
def check(expr,expected):return call('check',expr,c(expected))
def get(fd=3,dst=32):return call('get',c(fd),c(dst))
def rights(base,inherit,fd=3):return call('set',c(fd),l(base),l(inherit))
def eq64(at,value):return check('(i64.ne(i64.load'+c(at)+')'+l(value)+')',0)
init=''.join('(i64.store'+c(at)+l(-1)+')' for at in (32,40,48))
verify_dir=lambda base,inherit:check('(i32.load'+c(32)+')',3)+check('(i32.load'+c(36)+')',0)+eq64(40,base)+eq64(48,inherit)
specs=[]
def add(name,body,expected=0,**kw):specs.append(dict(name=name,body=body,expected=expected,**kw))
add('get-hidden',call('exit',get()),127,get_state='hidden')
add('get-denied',call('exit',get()),76,get_state='deny')
add('get-denied-bad-output',call('exit',get(dst=65536)),76,get_state='deny')
add('get-directory',check(get(),0)+verify_dir(9216,66))
add('get-zero-rights',check(get(),0)+verify_dir(0,0),base=0,inherit=0)
add('get-unmounted',call('exit',get()),8,mount=False)
add('get-invalid-fd',call('exit',get(2147483647)),8)
add('get-stdio-ungranted',call('exit',get(1)),76)
add('get-stdio-metadata-only',call('exit',get(1)),76,grants=['stdio.metadata'])
add('get-stdio-write-only',call('exit',get(1)),76,grants=['stdout.write'])
add('get-stdio-granted',check(get(1),0),grants=['stdout.write','stdio.metadata'])
add('get-stdin-file',check(get(0),0)+check('(i32.load'+c(32)+')',4)+eq64(40,2097154)+eq64(48,0),grants=['stdin.read','stdio.metadata'],stdin_file=True)
add('get-renumber-origin-denied',check(call('rename',c(1),c(3)),0)+call('exit',get()),76,grants=['stdout.write'])
add('get-renumber-origin-granted',check(call('rename',c(1),c(3)),0)+check(get(),0),grants=['stdout.write','stdio.metadata'])
add('get-misaligned',call('exit',get(dst=33)),28)
add('get-oob',call('exit',get(dst=65520)),21,shared_expected=121)
add('get-total-quota',check(get(),0)+init+check(get(),6)+eq64(32,-1)+eq64(40,-1)+eq64(48,-1),limits=['--wasip1-single-limit','main','filesystem.total-calls','1'])
add('get-window-wait',check(get(),0)+check(get(),0),limits=['--wasip1-single-limit','main','filesystem.window-calls','1','--wasip1-single-limit-action','main','filesystem.window-calls','wait'],wait=True)
add('set-hidden',call('exit',rights(0,0)),127,set_state='hidden')
add('set-denied',call('exit',rights(0,0)),76,set_state='deny')
add('set-denied-invalid-fd',call('exit',rights(0,0,2147483647)),76,set_state='deny')
add('set-unmounted',call('exit',rights(0,0)),8,mount=False)
add('set-stdio-discard-ungranted',call('exit',rights(0,0,1)),8)
add('set-stdio-discard-granted',check(rights(0,0,1),0),grants=['stdout.write'])
add('set-reduce',check(rights(1024,2),0)+check(get(),0)+verify_dir(1024,2))
add('set-independent-inherit',check(rights(0,64),0)+check(get(),0)+verify_dir(0,64))
add('set-identical',check(rights(9216,66),0)+check(get(),0)+verify_dir(9216,66))
add('set-cannot-restore-base',check(rights(1024,2),0)+check(rights(9216,2),76)+check(get(),0)+verify_dir(1024,2))
add('set-cannot-restore-inherit',check(rights(1024,2),0)+check(rights(1024,66),76)+check(get(),0)+verify_dir(1024,2))
add('set-discard',check(rights(0,0),0)+check(get(),0)+verify_dir(0,0))
for key in ('base','inherit'):
 for high in (1<<40,-(1<<63)):
  newbase,newinherit=(high,66) if key=='base' else (9216,high)
  add(f'set-wide-{key}-{high}',check(rights(newbase,newinherit),76)+check(get(),0)+verify_dir(9216,66))
for which in (1,2):
 add('set-wrong-signature-'+str(which),'',127,wrong_signature=which)
clock_call=call('clock',c(1),l(0x1122334455667788),c(32))
add('clock-existing-wide-allowed',check(clock_call,0)+check('(i64.eq(i64.load'+c(32)+')'+l(-1)+')',0),grants=['monotonic-clock'])
add('clock-existing-wide-ungranted',call('exit',clock_call),76)
add('clock-existing-wide-hidden',call('exit',clock_call),127,clock_state='hidden')
add('clock-existing-wide-denied',call('exit',clock_call),76,clock_state='deny',grants=['monotonic-clock'])
rows=[]
for shared in (False,True):
 for engine in ('int','aot'):
  for spec in specs:
   name=spec['name']+'-'+engine+('-shared' if shared else '-ordinary')
   body=spec['body'];imports={'exit':'(param i32)'}
   signatures={'get':'(param i32 i32)(result i32)','set':'(param i32 i64 i64)(result i32)','rename':'(param i32 i32)(result i32)','clock':'(param i32 i64 i32)(result i32)'}
   for key in signatures:
    if '$'+key in body:imports[key]=signatures[key]
   if spec.get('wrong_signature'):
    imports['set']='(param i32 '+('i32 i64' if spec['wrong_signature']==1 else 'i64 i32')+')(result i32)'
   names={'get':'fd_fdstat_get','set':'fd_fdstat_set_rights','rename':'fd_renumber','clock':'clock_time_get','exit':'proc_exit'}
   memory='(import "env" "memory"(memory 1 1 shared))' if shared else '(memory 1)'
   wat='(module'+''.join('(import "wasi_snapshot_preview1" "'+names[k]+'"(func $'+k+' '+v+'))' for k,v in imports.items())+memory+'(func $check(param $actual i32)(param $expected i32)(if(i32.ne(local.get $actual)(local.get $expected))(then(call $exit(i32.const 101)))))(func(export "_start")'+init+body+')(func(export "wasi_thread_start")(param i32 i32)))'
   target=make(name,wat)
   cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-grant','main','proc-exit']
   for k in imports:cmd+=['--wasip1-single-interface','main',names[k],spec.get(k+'_state','allow')]
   if spec.get('mount',True):cmd+=['--wasip1-single-mount-dir','main','/sandbox',str(mount),'--wasip1-single-mount-dir-rights','main','/sandbox',str(spec.get('base',9216)),str(spec.get('inherit',66))]
   for grant in spec.get('grants',[]):cmd+=['--wasip1-single-grant','main',grant]
   cmd+=spec.get('limits',[])
   if shared:cmd+=['-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','65536','--wasm-preload-library',str(owner),'env']
   cmd+=['--run',str(target)];begin=time.monotonic()
   source_input=None
   if spec.get('stdin_file'):
    input_path=a.out/(name+'.input');input_path.write_bytes(b'authorized metadata input');source_input=input_path.open('rb')
   try:r=subprocess.run(cmd,stdin=source_input,capture_output=True,timeout=60);status=r.returncode;stdout=r.stdout;stderr=r.stderr
   except subprocess.TimeoutExpired as e:status=None;stdout=e.stdout or b'';stderr=e.stderr or b''
   finally:
    if source_input is not None:source_input.close()
   elapsed=time.monotonic()-begin;expected=spec.get('shared_expected',spec['expected']) if shared else spec['expected']
   passed=status==expected and not stdout and (not spec.get('wait') or elapsed>=.9)
   if expected==121:passed=passed and b'"exit_status":121' in stderr
   rows.append(dict(name=name,case=spec['name'],engine=engine,shared=shared,exit=status,expected=expected,passed=passed,seconds=elapsed,argv=cmd,stderr=stderr.decode(errors='replace'),wasm_sha256=hashlib.sha256(target.read_bytes()).hexdigest()))
   (a.out/'report.json').write_text(json.dumps(rows,indent=2));(a.out/(name+'.stderr')).write_bytes(stderr);print(name,status,'PASS' if passed else 'FAIL',flush=True)
   if not passed:print(stderr.decode(errors='replace'),flush=True)
raise SystemExit(any(not x['passed'] for x in rows))
