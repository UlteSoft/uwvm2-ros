from pathlib import Path
import json,subprocess,socket,threading,time,sys,hashlib
import argparse
p=argparse.ArgumentParser()
p.add_argument('--vm',type=Path,required=True)
p.add_argument('--out',type=Path,required=True)
p.add_argument('--wasm-tools',type=Path,required=True)
a=p.parse_args()
vm=a.vm.resolve(); B=a.out.resolve(); B.mkdir(parents=True,exist_ok=True)
compiler=a.wasm_tools.resolve()
report={'vm':str(vm),'cases':[]}
threaded=True
if threaded:
 memory=B/'memory.wat';memory.write_text('(module (memory (export "memory") 1 1 shared))')
 subprocess.run([str(compiler),'parse',str(memory),'-o',str(memory.with_suffix('.wasm'))],check=True)
def wasm(name,imports,body,init=''):
 p=B/(name+'.wat')
 p.write_text('(module '+''.join('(import "wasi_snapshot_preview1" "'+n+'" (func $'+n+' '+t+'))' for n,t in imports.items())+'(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))(memory (export "memory") 1)(data (i32.const 128) "socket-payload!")(func $check (param $v i32)(param $expected i32)(if (i32.ne (local.get $v)(local.get $expected))(then (call $exit (i32.const 101)))))(func (export "_start")(local $fd i32)'+init+body+'))')
 if threaded:
  text=p.read_text().replace('(module ','(module (import "env" "memory" (memory 1 1 shared)) ',1).replace('(memory (export "memory") 1)','(export "memory" (memory 0))(func (export "wasi_thread_start") (param i32 i32))')
  text=text.replace('(data (i32.const 128) "socket-payload!")','')
  text=text.replace('(local $fd i32)','(local $fd i32)'+''.join('(i32.store8 (i32.const '+str(128+i)+')(i32.const '+str(x)+'))' for i,x in enumerate(b'socket-payload!')))
  p.write_text(text)
 subprocess.run([str(compiler),'parse',str(p),'-o',str(p.with_suffix('.wasm'))],check=True,capture_output=True)
 return p.with_suffix('.wasm')
send={'sock_send':'(param i32 i32 i32 i32 i32)(result i32)'}
recv={'sock_recv':'(param i32 i32 i32 i32 i32 i32)(result i32)'}
init='(i32.store (i32.const 0)(i32.const 128))(i32.store (i32.const 4)(i32.const 15))'
sendcall='(call $sock_send (i32.const 3)(i32.const 0)(i32.const 1)(i32.const 0)(i32.const 16))'
def run(name,engine,imports,body,rights='none',state='allow',extra=(),peer=None,expected=0,expect_payload=b'',init=init,kind='tcp-connect'):
 target=wasm(name,imports,body,init)
 errors=[];received=[]
 with socket.socket(socket.AF_INET,socket.SOCK_DGRAM if kind=='udp-connect' else socket.SOCK_STREAM) as server:
  server.bind(('127.0.0.1',0));server.settimeout(20)
  if kind=='tcp-connect':server.listen()
  port=server.getsockname()[1]
  def remote():
   try:
    if kind=='tcp-connect':
     conn,_=server.accept()
     with conn:
      conn.settimeout(10)
      if peer:peer(conn)
      chunks=[]
      while data:=conn.recv(65536):chunks.append(data)
      received.append(b''.join(chunks))
    else:
     data,address=server.recvfrom(65536);received.append(data)
     if peer:peer(server,address)
   except TimeoutError:received.append(b'')
   except (BrokenPipeError,ConnectionResetError):received.append(b'')
   except BaseException as e:errors.append(repr(e))
  thread=threading.Thread(target=remote,daemon=True) if kind=='tcp-connect' else None
  if thread:thread.start()
  args=[str(vm),'-R'+engine,'--runtime-compile-threads','0','--runtime-llvm-jit-policy','debug','--wasm-set-main-module-name','main',
    '--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-interface','main','proc_exit','allow',
    '--wasip1-single-grant','main','proc-exit','--wasip1-single-socket-'+kind,'main','3','127.0.0.1:'+str(port),
    '--wasip1-single-socket-rights','main','3',rights,'none']
  for n in imports:args+=['--wasip1-single-interface','main',n,state]
  if threaded:args+=['-WFE-threads','--wasi-threads','3','3','1048576','16777216','8388608','65536','--wasm-preload-library',str(memory.with_suffix('.wasm')),'env']
  args+=[str(x).replace('{port}',str(port)) for x in extra]+['--run',str(target)]
  begin=time.monotonic();p=subprocess.run(args,capture_output=True,timeout=45);elapsed=time.monotonic()-begin
  if thread:
   thread.join(1)
   if thread.is_alive():server.close();thread.join(1)
 row={'name':name,'engine':engine,'exit':p.returncode,'seconds':elapsed,'payload_hex':b''.join(received).hex(),'argv':args,'stderr':p.stderr.decode(errors='replace'),'peer_errors':errors}
 row['pass']=p.returncode==expected and b''.join(received)==expect_payload and not errors
 if expected==121:row['pass']=row['pass'] and '"exit_status":121' in row['stderr']
 report['cases'].append(row);(B/'report.json').write_text(json.dumps(report,indent=2))
 print(name,engine,p.returncode,round(elapsed,3),row['payload_hex'],'PASS' if row['pass'] else 'FAIL',flush=True)
 if not row['pass']:print(row,flush=True);raise AssertionError(name)
get={'fd_fdstat_get':'(param i32 i32)(result i32)'}
both={**get,'fd_fdstat_set_rights':'(param i32 i64 i64)(result i32)','sock_send':'(param i32 i32 i32 i32 i32)(result i32)'}
for engine in ['int','aot']:
 for kind,filetype in [('tcp-connect',6),('udp-connect',5)]:
  for rights,mask in [('none',0),('write',64)]:
   body=f'(call $check(call $fd_fdstat_get(i32.const 3)(i32.const 32))(i32.const 0))(call $check(i32.load8_u(i32.const 32))(i32.const {filetype}))(call $check(i64.ne(i64.load(i32.const 40))(i64.const {mask}))(i32.const 0))'
   run('metadata-'+kind+'-'+rights,engine,get,body,rights=rights,kind=kind)
 for name,base,inherit,expected in [('reduce',0,0,0),('increase',66,0,76),('wide-base',1099511627776,0,76),('wide-inherit',64,1099511627776,76)]:
  wanted=0 if name=='reduce' else 64
  body=f'(call $check(call $fd_fdstat_set_rights(i32.const 3)(i64.const {base})(i64.const {inherit}))(i32.const {expected}))(call $check(call $fd_fdstat_get(i32.const 3)(i32.const 32))(i32.const 0))(call $check(i64.ne(i64.load(i32.const 40))(i64.const {wanted}))(i32.const 0))'
  if name=='reduce':body+='(call $check'+sendcall+'(i32.const 76))'
  run('socket-rights-'+name,engine,both,body,rights='write')
