"""Actual ordinary32/64/shared Wasm file calls, full outputs/effects and cursor syscall proof."""
from pathlib import Path
import argparse,hashlib,json,os,re,shutil,subprocess
from owned_process import run_owned
p=argparse.ArgumentParser()
for k in ('vm','out','wasm-tools'):p.add_argument('--'+k,type=Path,required=True)
p.add_argument('--profile',choices=('all','ordinary32','ordinary64','shared32'),default='all');p.add_argument('--case',action='append');p.add_argument('--prepare-only',action='store_true');p.add_argument('--fixtures',type=Path);a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=False)
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();previous=json.loads(a.fixtures.read_text()) if a.fixtures else {};fixtures={};rows=[]
def make(name,wat):
 source=a.out/(name+'.wat');source.write_text(wat);key=hashlib.sha256(wat.encode()).hexdigest();old=previous.get(name)
 if old and old['wat_sha256']==key and sha(old['wasm'])==old['wasm_sha256']:target=Path(old['wasm']);reused=True
 else:
  target=source.with_suffix('.wasm');subprocess.run([str(a.wasm_tools),'parse',str(source),'-o',str(target)],check=True,capture_output=True);reused=False
 fixtures[name]=dict(wat_sha256=key,wasm=str(target),wasm_sha256=sha(target));(a.out/'fixtures.json').write_text(json.dumps(fixtures,indent=2));return target,reused
owner,_=make('owner','(module(memory(export "memory") 2 4 shared))')
profiles=('ordinary32','ordinary64','shared32') if a.profile=='all' else (a.profile,)
cases=['seek-allow','open-rights','open-call-deny','open-hidden','create-parent','truncate-parent','inherit','fdlimit-create','fdlimit-truncate','slot-reuse','path-oob','result-oob','seek-oob','tell-oob','seek-deny','tell-deny','seek-rights','tell-rights','seek-quota','renumber-quota','flags','escape','symlink','long-path','close-deny','close-hidden']
for profile in profiles:
 wide=profile=='ordinary64';shared=profile=='shared32';t='i64' if wide else 'i32';addr=lambda n:'('+t+'.const '+str(n)+')';c=lambda n:'(i32.const '+str(n)+')';q=lambda n:'(i64.const '+str(n)+')'
 invoke=lambda op,*xs:'(call $'+op+''.join(xs)+')';check=lambda value,want=0:invoke('check',value,c(want));loadfd='(i32.load'+addr(64)+')';pos='(i64.load'+addr(128)+')';reset='(i64.store'+addr(128)+q(-1)+')';untouched=check('(i64.ne'+pos+q(-1)+')');position=lambda n:check('(i64.ne'+pos+q(n)+')');close=lambda fd='(local.get $fd)':check(invoke('close',fd));seek=lambda off,whence=0,out=128:invoke('seek','(local.get $fd)',q(off),c(whence),addr(out));tell=lambda out=128:invoke('tell','(local.get $fd)',addr(out));function=lambda name:name+('_wasm64' if wide and name in ('path_open','fd_seek','fd_tell','fd_filestat_get') else '')
 for case in cases:
  if a.case and case not in a.case:continue
  name=profile+'-'+case;work=a.out/name;mount=work/'mount';mount.mkdir(parents=True);fixture=mount/'fixture';fixture.write_bytes(b'abcdef');outside=work/'outside';outside.write_bytes(b'secret');created=mount/'new';(mount/'inside').symlink_to('fixture')
  path='fixture';flags=0;lookup=0;rights=102;base=533504;inherit=2097254;limit=None;exposure={};extra=[];expected=0;expected_seeks=[];expected_created=False;long_fd=None
  if case in ('open-rights','open-call-deny','open-hidden','create-parent','inherit','fdlimit-create','result-oob','path-oob','flags'):path='new';flags=1
  if case=='open-rights':base=0
  if case=='create-parent':base=8192
  if case=='truncate-parent':base=8192;flags=8
  if case=='inherit':inherit&=~4
  if case.startswith('fdlimit-'):limit=4
  if case=='fdlimit-truncate':flags=8
  if case=='open-call-deny':exposure['path_open']='deny'
  if case=='open-hidden':exposure['path_open']='hidden';expected=127
  if case=='seek-deny':exposure['fd_seek']='deny'
  if case=='tell-deny':exposure['fd_tell']='deny'
  if case=='close-deny':exposure['fd_close']='deny'
  if case=='close-hidden':exposure['fd_close']='hidden';expected=127
  if case=='seek-rights':rights&=~4
  if case=='tell-rights':rights&=~32
  if case=='flags':flags=16
  if case=='escape':path='../outside';flags=8
  if case=='symlink':path='inside';lookup=1
  if case in ('symlink','long-path'):rights|=2097152
  if case=='long-path':
   component='component-abcdefghijklmn';parts=[component]*200;path='/'.join(parts+['leaf']);assert len(path)>4096
   directory=os.open(mount,os.O_RDONLY|os.O_DIRECTORY)
   for part in parts:os.mkdir(part,dir_fd=directory);child=os.open(part,os.O_RDONLY|os.O_DIRECTORY,dir_fd=directory);os.close(directory);directory=child
   long_fd=os.open('leaf',os.O_RDWR|os.O_CREAT,0o600,dir_fd=directory);os.write(long_fd,b'long');os.close(directory)
  raw=path.encode();text=''.join('\\'+format(b,'02x') for b in raw);open_call=lambda pointer=256,length=len(raw),output=64,mode=flags:invoke('open',c(3),c(lookup),addr(pointer),addr(length),c(mode),q(rights),q(0),c(0),addr(output));open_good=check(open_call())+'(local.set $fd'+loadfd+')';sentinel='(i64.store'+addr(64)+q(-1)+')';fd_untouched=check('(i64.ne(i64.load'+addr(64)+')'+q(-1)+')');body='';trace_file=fixture
  if case in ('open-rights','open-call-deny','create-parent','truncate-parent','inherit','fdlimit-create','fdlimit-truncate','flags','escape'):
   errno=33 if case.startswith('fdlimit-') else 28 if case=='flags' else 76;body=check(open_call(),errno)+fd_untouched
  elif case=='open-hidden':body=invoke('exit',open_call())
  elif case in ('path-oob','result-oob'):
   body=check(open_call(pointer=131070,length=5) if case=='path-oob' else open_call(output=131072),21)+fd_untouched;expected=121 if shared else 0
  elif case in ('seek-oob','tell-oob'):
   body=open_good+check(seek(3))+position(3)+check(seek(4294967301,out=131072) if case=='seek-oob' else tell(131072),21)+check(tell())+position(3)+close();expected=121 if shared else 0;expected_seeks=[(3,'SEEK_SET',3)]+([] if shared else [(0,'SEEK_CUR',3)])
  elif case in ('seek-deny','seek-rights'):
   body=open_good+reset+check(seek(4294967301),76)+untouched+check(tell())+position(0)+close();expected_seeks=[(0,'SEEK_CUR',0)]
  elif case in ('tell-deny','tell-rights'):
   body=open_good+check(seek(3))+position(3)+reset+check(tell(),76)+untouched+check(seek(0,1))+position(3)+close();expected_seeks=[(3,'SEEK_SET',3),(0,'SEEK_CUR',3)]
  elif case=='seek-quota':
   extra=['--wasip1-single-limit','main','filesystem.total-calls','2'];body=open_good+check(seek(3))+position(3)+reset+check(seek(4294967301),6)+untouched+close();expected_seeks=[(3,'SEEK_SET',3)]
  elif case=='renumber-quota':
   extra=['--wasip1-single-limit','main','filesystem.total-calls','1','--wasip1-single-grant','main','stdout.write','--wasip1-single-grant','main','stdio.metadata'];rights=2097254;open_good=check(open_call())+'(local.set $fd'+loadfd+')';body=open_good+check(invoke('rename','(local.get $fd)',c(1)))+'(local.set $fd'+c(1)+')'+''.join('(i64.store'+addr(512+i)+q(-1)+')' for i in range(0,64,8))+check(invoke('stat',c(1),addr(512)),6)+''.join(check('(i64.ne(i64.load'+addr(512+i)+')'+q(-1)+')') for i in range(0,64,8))+reset+check(seek(3),76)+untouched+close()
  elif case=='slot-reuse':
   limit=5;body=open_good+check('(i32.ne(local.get $fd)'+c(4)+')')+close()+check(open_call(pointer=8192,length=3,mode=1))+'(local.set $fd'+loadfd+')'+check('(i32.ne(local.get $fd)'+c(4)+')')+close()+check(invoke('close',c(4)),8);expected_created=True
  elif case=='close-deny':body=open_good+check(invoke('close','(local.get $fd)'),76)+check(tell())+position(0)+check(seek(4294967301))+position(4294967301);expected_seeks=[(0,'SEEK_CUR',0),(4294967301,'SEEK_SET',4294967301)]
  elif case=='close-hidden':body=open_good+close()
  elif case=='seek-allow':body=open_good+check(seek(4294967301))+position(4294967301)+check(tell())+position(4294967301)+close()+check(invoke('close','(local.get $fd)'),8);expected_seeks=[(4294967301,'SEEK_SET',4294967301),(0,'SEEK_CUR',4294967301)]
  elif case in ('symlink','long-path'):
   body=open_good+check(invoke('stat','(local.get $fd)',addr(512)))+check('(i64.ne(i64.load'+addr(528)+')'+q(4)+')')+check('(i64.ne(i64.load'+addr(544)+')'+q(4 if case=='long-path' else 6)+')')+close()
  else:body=open_good+close()
  names={'open':function('path_open'),'seek':function('fd_seek'),'tell':function('fd_tell'),'close':'fd_close','rename':'fd_renumber','stat':function('fd_filestat_get'),'exit':'proc_exit'};signatures={'open':'(param i32 i32 '+t+' '+t+' i32 i64 i64 i32 '+t+')(result i32)','seek':'(param i32 i64 i32 '+t+')(result i32)','tell':'(param i32 '+t+')(result i32)','close':'(param i32)(result i32)','rename':'(param i32 i32)(result i32)','stat':'(param i32 '+t+')(result i32)','exit':'(param i32)'}
  payload=('(data $path "'+text+'")(data $new "new")' if shared else '(data'+addr(256)+'"'+text+'")(data'+addr(8192)+'"new")');initialize=('(memory.init $path(i32.const 256)(i32.const 0)(i32.const '+str(len(raw))+'))(data.drop $path)(memory.init $new(i32.const 8192)(i32.const 0)(i32.const 3))(data.drop $new)' if shared else '')
  imports=['exit']+[key for key in names if key!='exit' and ('$'+key) in body];memory='(import "env" "memory"(memory 2 4 shared))' if shared else '(memory '+('i64 ' if wide else '')+'2 4)';wat='(module'+''.join('(import "wasi_snapshot_preview1" "'+names[key]+'"(func $'+key+signatures[key]+'))' for key in imports)+memory+payload+'(func $check(param $got i32)(param $want i32)(if(i32.ne(local.get $got)(local.get $want))(then(call $exit(i32.const 101)))))(func(export "_start")(local $fd i32)'+initialize+sentinel+body+')(func(export "wasi_thread_start")(param i32 i32)))';target,reused=make(name,wat)
  if a.prepare_only:
   if long_fd is not None:os.close(long_fd)
   continue
  if not shutil.which('strace'):raise RuntimeError('strace required for actual cursor/no-effect qualification')
  for engine in ('int','aot'):
   trace=work/(engine+'.trace');cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-grant','main','proc-exit']
   for key in imports:cmd+=['--wasip1-single-interface','main',names[key],exposure.get(names[key].removesuffix('_wasm64'),'allow')]
   cmd+=['--wasip1-single-mount-dir','main','/sandbox',str(mount),'--wasip1-single-mount-dir-rights','main','/sandbox',str(base),str(inherit)]+extra
   if limit is not None:cmd+=['--wasip1-single-set-fd-limit','main',str(limit)]
   if wide:cmd+=['--wasm-feature-enable-memory64']
   if shared:cmd+=['-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','262144','--wasm-preload-library',str(owner),'env']
   cmd+=['--run',str(target)];traced=['/usr/bin/strace','-f','-yy','-e','trace=lseek,read,write,readv,writev,pread64,pwrite64,openat,close','-o',str(trace)]+cmd;code,stdout,stderr,timeout,leader,remaining,cleanup=run_owned(traced,60)
   selected=[line for line in trace.read_text().splitlines() if '<'+str(trace_file)+'>' in line];seeks=[]
   for line in selected:
    match=re.search(r'lseek\(\d+<[^>]+>, (-?\d+), (SEEK_\w+)\)\s+= (-?\d+)',line)
    if match:seeks.append((int(match[1]),match[2],int(match[3])))
   unexpected_data=[line for line in selected if re.search(r'\b(?:read|write|readv|writev|pread64|pwrite64)\(',line)];native_open=any('openat(' in line for line in selected);expected_open=case in ('seek-allow','slot-reuse','seek-oob','tell-oob','seek-deny','tell-deny','seek-rights','tell-rights','seek-quota','renumber-quota','symlink','close-deny');effects=fixture.read_bytes()==b'abcdef' and outside.read_bytes()==b'secret' and (created.exists() and created.stat().st_size==0 if expected_created else not created.exists())
   if long_fd is not None:effects=effects and os.pread(long_fd,5,0)==b'long'
   fault_ok=expected!=121 or (b'"exit_status":121' in stderr and b'Memory tag mismatch' not in stderr)
   passed=fault_ok and code==expected and not stdout and effects and seeks==expected_seeks and native_open==expected_open and not unexpected_data and not timeout and not remaining and cleanup['complete'] and cleanup.get('identity_preserved_until_group_cleanup') and cleanup.get('leader_reaped')
   row=dict(name=name+'-'+engine,fault_ok=fault_ok,exit=code,expected=expected,passed=bool(passed),argv=cmd,trace=str(trace),trace_sha256=sha(trace),native_cursor_operations=seeks,expected_native_cursor_operations=expected_seeks,native_open_observed=native_open,expected_native_open=expected_open,unexpected_data_operations=unexpected_data,effects_ok=effects,wasm_sha256=sha(target),fixture_reused=reused,stderr=stderr.decode(errors='replace'),cleanup=cleanup);rows.append(row);(a.out/'report.json').write_text(json.dumps(rows,indent=2));print(row['name'],'PASS' if passed else 'FAIL',flush=True)
   if expected_created and created.exists():created.unlink()
   if not passed:print(json.dumps(row),flush=True)
  if long_fd is not None:os.close(long_fd)
if not fixtures or len(fixtures)==1:raise RuntimeError('no selected cases')
raise SystemExit(any(not x['passed'] for x in rows))
