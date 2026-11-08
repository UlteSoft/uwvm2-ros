from pathlib import Path
import argparse,subprocess,json,hashlib,time
p=argparse.ArgumentParser();p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--wasm-tools',type=Path,required=True);p.add_argument('--profile',choices=['ordinary','shared','all'],default='all');p.add_argument('--case',action='append');a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
mount=a.out/'directory';mount.mkdir(exist_ok=True)
def make(name,wat):
 source=a.out/(name+'.wat');source.write_text(wat);target=source.with_suffix('.wasm')
 subprocess.run([str(a.wasm_tools),'parse',str(source),'-o',str(target)],check=True,capture_output=True);return target
owner=make('owner','(module(memory(export "memory") 1 2 shared))')
def c(v):return f'(i32.const {v})'
def l(v):return f'(i64.const {v})'
def call(fn,*values):return '(call $'+fn+''.join(values)+')'
def check(expr,expected):return call('check',expr,c(expected))
def get(fd=3,dst=32):return call('get',c(fd),c(dst))
def rights(base,inherit,fd=3):return call('set',c(fd),l(base),l(inherit))
def eq64(at,value):return check('(i64.ne(i64.load'+c(at)+')'+l(value)+')',0)
init=''.join('(i64.store'+c(at)+l(-1)+')' for at in range(32,96,8))
input_path=a.out/'authorized-input';input_path.write_bytes(b'authorized metadata input')
def verify(path,type,where=32):
 st=path.stat(); values={0:st.st_dev,8:st.st_ino,16:type,24:st.st_nlink,32:st.st_size,40:st.st_atime_ns,48:st.st_mtime_ns,56:st.st_ctime_ns}
 return ''.join(eq64(where+offset,value) for offset,value in values.items())
unchanged=''.join(eq64(at,-1) for at in range(32,96,8))
specs=[]
def add(name,body,expected=0,**kw):specs.append(dict(name=name,body=body,expected=expected,**kw))
add('hidden',call('exit',get()),127,get_state='hidden')
add('denied',call('exit',get()),76,get_state='deny')
add('denied-bad-output',call('exit',get(dst=65536)),76,get_state='deny')
add('directory',check(get(),0)+verify(mount,3))
add('zero-rights',check(get(),76)+unchanged,base=0)
add('wrong-rights',check(get(),76)+unchanged,base=1<<18)
add('rights-discarded',check(rights(0,0),0)+check(get(),76)+unchanged)
add('rights-cannot-restore',check(rights(0,0),0)+check(rights(1<<21,0),76)+check(get(),76)+unchanged)
add('unmounted',call('exit',get()),8,mount=False)
add('invalid-fd',call('exit',get(2147483647)),8)
add('negative-fd',call('exit',get(4294967295)),8)
add('stdio-ungranted',call('exit',get(1)),76)
add('stdio-metadata-only',call('exit',get(1)),76,grants=['stdio.metadata'])
add('stdio-write-only',call('exit',get(1)),76,grants=['stdout.write'])
add('stdio-granted',check(get(1),0),grants=['stdout.write','stdio.metadata'])
add('stdin-file',check(get(0),0)+verify(input_path,4),grants=['stdin.read','stdio.metadata'],stdin_file=True)
add('renumber-origin-denied',check(call('rename',c(1),c(3)),0)+call('exit',get()),76,grants=['stdout.write'])
add('renumber-origin-granted',check(call('rename',c(1),c(3)),0)+check(get(),0),grants=['stdout.write','stdio.metadata'])
add('misaligned',call('exit',get(dst=33)),28)
add('oob-last-granule',call('exit',get(dst=65504)),21,shared_expected=121)
add('last-valid-output',check(get(dst=65472),0)+verify(mount,3,65472))
add('grown-output',check('(memory.grow'+c(1)+')',1)+check(get(dst=131008),0)+verify(mount,3,131008))
add('grown-oob-last-granule',check('(memory.grow'+c(1)+')',1)+call('exit',get(dst=131040)),21,shared_expected=121)
add('total-quota',check(get(),0)+init+check(get(),6)+unchanged,limits=['--wasip1-single-limit','main','filesystem.total-calls','1'])
add('window-reject',check(get(),0)+init+check(get(),6)+unchanged,limits=['--wasip1-single-limit','main','filesystem.window-calls','1'])
# The first get may enter a partly spent process-wide window. Three
# successes cross two boundaries and include one full window in Guest time.
elapsed_wait=check(call('clock',c(1),l(0),c(128)),0)+check(get(),0)+verify(mount,3)+init+check(get(),0)+verify(mount,3)+init+check(get(),0)+verify(mount,3)+check(call('clock',c(1),l(0),c(136)),0)+check('(i64.lt_u(i64.sub(i64.load'+c(136)+')(i64.load'+c(128)+'))'+l(900000000)+')',0)
add('window-wait',elapsed_wait,limits=['--wasip1-single-limit','main','filesystem.window-calls','1','--wasip1-single-limit-action','main','filesystem.window-calls','wait'],wait=True,grants=['monotonic-clock'])
add('wrong-signature','',127,wrong_signature=True)
rows=[]
for shared in (False,True):
 if a.profile!='all' and shared!=(a.profile=='shared'):continue
 for engine in ('int','aot'):
  for spec in specs:
   if a.case and spec['name'] not in a.case:continue
   name=spec['name']+'-'+engine+('-shared' if shared else '-ordinary')
   body=spec['body'];imports={'exit':'(param i32)'}
   signatures={'get':'(param i32 i32)(result i32)','set':'(param i32 i64 i64)(result i32)','rename':'(param i32 i32)(result i32)','clock':'(param i32 i64 i32)(result i32)'}
   for key in signatures:
    if '$'+key in body:imports[key]=signatures[key]
   if spec.get('wrong_signature'):
    imports['get']='(param i32 i64)(result i32)'
   names={'get':'fd_filestat_get','set':'fd_fdstat_set_rights','rename':'fd_renumber','clock':'clock_time_get','exit':'proc_exit'}
   memory='(import "env" "memory"(memory 1 2 shared))' if shared else '(memory 1 2)'
   wat='(module'+''.join('(import "wasi_snapshot_preview1" "'+names[k]+'"(func $'+k+' '+v+'))' for k,v in imports.items())+memory+'(func $check(param $actual i32)(param $expected i32)(if(i32.ne(local.get $actual)(local.get $expected))(then(call $exit(i32.const 101)))))(func(export "_start")'+init+body+')(func(export "wasi_thread_start")(param i32 i32)))'
   target=make(name,wat)
   cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-grant','main','proc-exit']
   for k in imports:cmd+=['--wasip1-single-interface','main',names[k],spec.get(k+'_state','allow')]
   if spec.get('mount',True):cmd+=['--wasip1-single-mount-dir','main','/sandbox',str(mount),'--wasip1-single-mount-dir-rights','main','/sandbox',str(spec.get('base',1<<21)),str(spec.get('inherit',0))]
   for grant in spec.get('grants',[]):cmd+=['--wasip1-single-grant','main',grant]
   cmd+=spec.get('limits',[])
   if shared:cmd+=['-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','131072','--wasm-preload-library',str(owner),'env']
   cmd+=['--run',str(target)];begin=time.monotonic()
   source_input=None
   if spec.get('stdin_file'):
    source_input=input_path.open('rb')
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
