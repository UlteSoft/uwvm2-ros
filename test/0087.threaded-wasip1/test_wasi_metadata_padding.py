from pathlib import Path
import argparse,subprocess,json,hashlib,struct
p=argparse.ArgumentParser();p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--wasm-tools',type=Path,required=True);a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
directory=a.out/'authorized-directory';directory.mkdir(exist_ok=True)
def make(name,text):
 source=a.out/(name+'.wat');source.write_text(text);target=source.with_suffix('.wasm');subprocess.run([str(a.wasm_tools),'parse',str(source),'-o',str(target)],check=True,capture_output=True);return target
owner=make('owner','(module(memory(export "memory") 1 1 shared))')
rows=[];base=(1<<21)|(1<<18)
for bits,shared in [(32,False),(64,False),(32,True)]:
 for function in ['fd_fdstat_get','fd_filestat_get','path_filestat_get']:
  if shared and function!='fd_fdstat_get':continue
  width='i'+str(bits);suffix='_wasm64' if bits==64 else ''
  c=lambda value:'('+width+'.const '+str(value)+')'
  size=24 if function=='fd_fdstat_get' else 64
  signature='(param i32 '+width+')(result i32)' if not function.startswith('path_') else '(param i32 i32 '+width+' '+width+' '+width+')(result i32)'
  call='(call $get(i32.const 3)'+c(32)+')' if not function.startswith('path_') else '(call $get(i32.const 3)(i32.const 0)'+c(128)+c(1)+c(32)+')'
  memory='(import "env" "memory"(memory 1 1 shared))' if shared else '(memory '+('i64 ' if bits==64 else '')+'1)'
  wat='(module'+memory+'(import "wasi_snapshot_preview1" "'+function+suffix+'"(func $get'+signature+'))(import "wasi_snapshot_preview1" "fd_write'+suffix+'"(func $write(param i32 '+width+' '+width+' '+width+')(result i32)))(import "wasi_snapshot_preview1" "proc_exit"(func $exit(param i32)))(func(export "_start")(local $error i32)(i32.store8'+c(128)+'(i32.const 46))(local.set $error'+call+')(if(local.get $error)(then(call $exit(local.get $error))))('+width+'.store'+c(0)+c(32)+')('+width+'.store'+c(bits//8)+c(size)+')(drop(call $write(i32.const 1)'+c(0)+c(1)+c(16)+')))(func(export "wasi_thread_start")(param i32 i32)))'
  # Import declarations must precede a local memory definition.
  if not shared:
   wat=wat.replace('(module'+memory,'(module',1).replace('(func(export "_start")',memory+'(func(export "_start")',1)
  file=make(function+'-'+str(bits)+('-shared' if shared else ''),wat)
  for engine in ['int','aot']:
   name=file.stem+'-'+engine
   cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-grant','main','stdout.write','--wasip1-single-grant','main','proc-exit']
   for interface in [function+suffix,'fd_write'+suffix,'proc_exit']:cmd+=['--wasip1-single-interface','main',interface,'allow']
   cmd+=['--wasip1-single-mount-dir','main','/sandbox',str(directory),'--wasip1-single-mount-dir-rights','main','/sandbox',str(base),'66']
   if bits==64:cmd+=['--wasm-feature-enable-memory64','--wasm-feature-enable-table64']
   if shared:cmd+=['-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','65536','--wasm-preload-library',str(owner),'env']
   cmd+=['--run',str(file)]
   run=subprocess.run(cmd,capture_output=True,timeout=60);raw=run.stdout
   padding=[1,4,5,6,7] if size==24 else list(range(17,24))
   good=run.returncode==0 and len(raw)==size and all(raw[i]==0 for i in padding)
   if good and size==24:good=raw==struct.pack('<BxH4xQQ',3,0,base,66)
   elif good:
    st=directory.stat();decoded=struct.unpack('<QQB7xQQQQQ',raw)
    good=decoded==(st.st_dev,st.st_ino,3,st.st_nlink,st.st_size,st.st_atime_ns,st.st_mtime_ns,st.st_ctime_ns)
   rows.append(dict(name=name,bits=bits,shared=shared,engine=engine,function=function,exit=run.returncode,expected=0,passed=good,argv=cmd,output_hex=raw.hex(),padding_zero=len(raw)==size and all(raw[i]==0 for i in padding),stderr=run.stderr.decode(errors='replace'),wasm_sha256=hashlib.sha256(file.read_bytes()).hexdigest()))
   (a.out/'report.json').write_text(json.dumps(rows,indent=2));(a.out/(name+'.output')).write_bytes(raw);print(name,run.returncode,'PASS' if good else 'FAIL',flush=True)
   if not good:print(run.stderr.decode(errors='replace'),flush=True)
raise SystemExit(any(not x['passed'] for x in rows))
