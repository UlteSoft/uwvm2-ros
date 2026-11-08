"""True compiled Guest worker races and complete-range MTE before native effects."""
from pathlib import Path
import argparse,hashlib,json,os,re,subprocess
from owned_process import run_owned
p=argparse.ArgumentParser()
for k in ('vm','out','clang','wasm-tools'):p.add_argument('--'+k,type=Path,required=True)
p.add_argument('--prepare-only',action='store_true');p.add_argument('--fixtures',type=Path);p.add_argument('--group',choices=('all','parallel','memtag'),default='all');a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=False);sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();rows=[];old=json.loads(a.fixtures.read_text()) if a.fixtures else {};old=old.get('fixtures',[]) if isinstance(old,dict) else old;fixtures=[]
owner=a.out/'owner.wat';owner.write_text('(module(memory(export "memory") 2 4 shared))');subprocess.run([str(a.wasm_tools),'parse',str(owner),'-o',str(owner.with_suffix('.wasm'))],check=True,capture_output=True)
for group,source,count in [('parallel','shared_files_parallel.c',2),('memtag','shared_files_memtag.c',10)]:
 if a.group!='all' and a.group!=group:continue
 src=Path(__file__).with_name(source);source_sha=sha(src)
 for case in range(count):
  saved=next((r for r in old if Path(r['source']).name==source and r['case']==case and r['source_sha256']==source_sha and r.get('wasm_sha256') and Path(r['wasm']).is_file() and sha(r['wasm'])==r['wasm_sha256']),None)
  if saved:wasm=Path(saved['wasm']);reused=True;command=saved['command']
  else:
   wasm=a.out/(src.stem+'-'+str(case)+'.wasm');command=[str(a.clang),'--target=wasm32-unknown-unknown','-O1','-ffreestanding','-nostdlib','-matomics','-mbulk-memory','-Wl,--shared-memory','-Wl,--initial-memory=131072','-Wl,--max-memory=262144','-Wl,--import-memory','-Wl,--no-entry','-Wl,--export=_start','-Wl,--export=wasi_thread_start','-DCASE='+str(case),str(src),'-o',str(wasm)];r=subprocess.run(command,capture_output=True);(a.out/(src.stem+'-'+str(case)+'.compile.log')).write_bytes(r.stdout+r.stderr)
   if r.returncode:raise RuntimeError(r.stderr.decode(errors='replace'))
   reused=False
  fixtures.append(dict(source=str(src),source_sha256=source_sha,case=case,command=command,wasm=str(wasm),wasm_sha256=sha(wasm)));(a.out/'fixtures.json').write_text(json.dumps(dict(state='prepared-execution-pending',fixtures=fixtures),indent=2))
  if a.prepare_only:continue
  for engine in ('int','aot'):
   for disabled in (False,True):
    name=group+'-'+str(case)+'-'+engine+('-disabled' if disabled else '-enabled');work=a.out/name;mount=work/'mount';mount.mkdir(parents=True);target=mount/'long-file-component'
    if group=='parallel' and case==1:
     for n in ('first','other'):(mount/n).write_bytes(b'preserve')
    if group=='memtag':target.write_bytes(b'abcdef')
    cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','262144','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main']
    interfaces=['path_open','fd_seek','fd_tell','fd_close']
    if group=='parallel':interfaces+=['fd_read','fd_write','thread-spawn'];cmd+=['--wasip1-single-grant','main','thread-spawn']
    for interface in interfaces:cmd+=['--wasip1-single-interface','main',interface,'deny' if group=='memtag' and case==9 and interface=='path_open' else 'allow']
    cmd+=['--wasip1-single-mount-dir','main','/sandbox',str(mount),'--wasip1-single-mount-dir-rights','main','/sandbox','533504','102','--wasip1-single-set-fd-limit','main','5','--wasm-preload-library',str(owner.with_suffix('.wasm')),'env']
    if disabled:cmd+=['--runtime-disable-memtag']
    cmd+=['--run',str(wasm)];trace=work/'native.trace';expected=0;expected_seeks=[];opened=None
    if group=='memtag':
     expected=121 if case in (4,7,8) else 122 if not disabled and case in (1,2,3,5,6) else 0
     opened=not(case in (4,9) or (not disabled and case in (1,2,3)))
     if case in (5,6,7,8):
      expected_seeks=[(3,'SEEK_SET',3)]
      if disabled and case==5:expected_seeks.append((0,'SEEK_CUR',3))
      if disabled and case==6:expected_seeks.append((4294967301,'SEEK_SET',4294967301))
     elif opened:expected_seeks=[(0,'SEEK_CUR',0),(4294967301,'SEEK_SET',4294967301)]
     actual=['/usr/bin/strace','-f','-yy','-e','trace=lseek,read,write,readv,writev,pread64,pwrite64,openat,close','-o',str(trace)]+cmd
    else:actual=cmd
    code,out,err,timeout,leader,remaining,cleanup=run_owned(actual,90);seeks=[];trace_ok=True;details={}
    if group=='memtag':
     selected=[line for line in trace.read_text().splitlines() if '<'+str(target)+'>' in line]
     for line in selected:
      m=re.search(r'lseek\(\d+<[^>]+>, (-?\d+), (SEEK_\w+)\)\s+= (-?\d+)',line)
      if m:seeks.append((int(m[1]),m[2],int(m[3])))
     native_open=any('openat(' in line for line in selected);unexpected=[line for line in selected if re.search(r'\b(?:read|write|readv|writev|pread64|pwrite64)\(',line)];trace_ok=seeks==expected_seeks and native_open==opened and not unexpected;content_ok=target.read_bytes()==b'abcdef';details=dict(trace=str(trace),trace_sha256=sha(trace),native_open_observed=native_open,expected_native_open=opened,unexpected_data_operations=unexpected)
    else:
     paths=[mount/n for n in ('first','other')];large=[p for p in paths if p.exists() and p.stat().st_size==4294967302];content_ok=len(large)==1
     if content_ok:
      winner=large[0];loser=next(p for p in paths if p!=winner)
      with winner.open('rb') as stream:prefix=stream.read(8);stream.seek(4294967301);tail=stream.read(2)
      content_ok=prefix==b'\0'*8 and tail==b'R' and (loser.read_bytes()==b'preserve' if case==1 else not loser.exists());details=dict(winner=winner.name,loser=loser.name,winner_size=winner.stat().st_size)
    fault_ok=True;fault=None;expected_fault=None
    if expected in (121,122):
     fault_code=1463898881 if case in (4,7,8) else 1463898883
     fault_ok=('"exit_status":'+str(expected)).encode() in err
     if fault_code==1463898883:
      expected_fault={1:(0x30010000,3,0x10000,0),2:(0x30010000,3,0x10010,0),3:(0x60010020,6,0x10020,4),5:(0x70010030,7,0x10030,5),6:(0x70010030,7,0x10030,5)}[case]
      match=re.search(rb'pointer 0x([0-9a-f]+) \(tag 0x([0-9a-f]+)\); granule 0x([0-9a-f]+) \(tag 0x([0-9a-f]+)\)',err)
      fault=tuple(int(v,16) for v in match.groups()) if match else None;fault_ok=fault_ok and fault==expected_fault
     else:fault_ok=fault_ok and b'Memory tag mismatch' not in err
    elif group=='memtag' and case==9:fault_ok=b'host_denied_wasip1.path_open' in err and b'Memory tag mismatch' not in err
    else:fault_ok=b'Memory tag mismatch' not in err and b'Unable to run the requested Wasm threads profile' not in err
    passed=fault_ok and code==expected and not out and content_ok and trace_ok and not timeout and not remaining and cleanup['complete'] and cleanup.get('identity_preserved_until_group_cleanup') and cleanup.get('leader_reaped')
    row=dict(name=name,fault_ok=bool(fault_ok),memtag_fault=fault,expected_memtag_fault=expected_fault,passed=bool(passed),exit=code,expected=expected,argv=cmd,wasm_sha256=sha(wasm),source_sha256=source_sha,fixture_reused=reused,content_ok=content_ok,native_cursor_operations=seeks,expected_native_cursor_operations=expected_seeks,trace_ok=trace_ok,stderr=err.decode(errors='replace'),cleanup=cleanup,**details);rows.append(row);(a.out/'report.json').write_text(json.dumps(rows,indent=2));print(name,'PASS' if passed else 'FAIL',flush=True)
    if not passed:print(json.dumps(row),flush=True)
if not fixtures:raise RuntimeError('no selected cases')
raise SystemExit(any(not r['passed'] for r in rows))
