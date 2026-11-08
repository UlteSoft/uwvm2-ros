from pathlib import Path
import argparse,hashlib,json,socket,subprocess,threading,time,os,signal
p=argparse.ArgumentParser();p.add_argument('--vm',type=Path,required=True);p.add_argument('--out',type=Path,required=True);p.add_argument('--wasm-tools',type=Path,required=True);p.add_argument('--profile',choices=['ordinary','shared','all'],default='all');p.add_argument('--case',action='append');p.add_argument('--kind',choices=['tcp-connect','udp-connect']);p.add_argument('--exercise-peer-lifecycle',action='store_true');a=p.parse_args();a.out=a.out.resolve();a.out.mkdir(parents=True,exist_ok=True)
def make(name,wat):
 source=a.out/(name+'.wat');source.write_text(wat);target=source.with_suffix('.wasm');subprocess.run([str(a.wasm_tools),'parse',str(source),'-o',str(target)],check=True,capture_output=True);return target
owner=make('owner','(module(memory(export "memory") 1 1 shared))')
c=lambda value:f'(i32.const {value})'
l=lambda value:f'(i64.const {value})'
def call(name,*arguments):return '(call $'+name+''.join(arguments)+')'
def check(value,want=0):return call('check',value,c(want))
def get(fd=3,pointer=32):return call('get',c(fd),c(pointer))
def reduce(base=0):return call('rights',c(3),l(base),l(0))
def eq64(address,value):return check('(i64.ne(i64.load'+c(address)+')'+l(value)+')')
init=''.join('(i64.store'+c(i)+l(-1)+')' for i in range(32,96,8));unchanged=''.join(eq64(i,-1) for i in range(32,96,8))
# The peer has no independent deadline while the actual VM remains live.
# The existing45s VM bound controls the case; EOF cleanup is bounded only after
# the VM has physically exited. Forced cleanup always fails qualification.
def run_guest(server,kind,command,expected,require_connection,vm_timeout=45):
 begin=time.monotonic();vm_done=threading.Event();abort=threading.Event();peer_failure=threading.Event();lock=threading.Lock();payload=[];errors=[];active=[None]
 state={'vm_started':False,'vm_reaped':False,'vm_timeout':False,'peer_forced_cleanup':False,'accepted':0,'peer_eof':False,'peer_finished':kind!='tcp-connect','vm_done_at':None,'events':[]}
 def mark(event):
  with lock:state['events'].append({'event':event,'seconds':time.monotonic()-begin})
 def peer():
  try:
   connection=None
   while not abort.is_set():
    try:connection,_=server.accept();break
    except TimeoutError:
     if vm_done.is_set():break
   if connection is not None:
    with lock:active[0]=connection;state['accepted']+=1
    mark('tcp-accepted');connection.settimeout(.1)
    with connection:
     while not abort.is_set():
      try:chunk=connection.recv(65536)
      except TimeoutError:
       if vm_done.is_set() and time.monotonic()-state['vm_done_at']>=5:
        errors.append('TCP EOF missing after the actual VM exited');peer_failure.set();break
       continue
      if not chunk:state['peer_eof']=True;mark('tcp-eof');break
      payload.append(chunk);errors.append('Unexpected TCP payload in metadata-only case');peer_failure.set();break
    with lock:active[0]=None
   # An early EOF cannot hide a later reconnect. Inspect the accept queue only
   # after the real VM lifecycle has ended, including on expected Guest errors.
   while not vm_done.wait(.1):
    if abort.is_set():break
   if vm_done.is_set() and not abort.is_set():
    server.setblocking(False)
    while True:
     try:extra,_=server.accept()
     except BlockingIOError:break
     with extra:state['accepted']+=1
     errors.append('Unexpected additional TCP connection');peer_failure.set()
  except BaseException as error:
   if not abort.is_set():errors.append(repr(error));peer_failure.set()
  finally:state['peer_finished']=True;mark('peer-finished')
 thread=None
 if kind=='tcp-connect':
  server.listen(1);server.settimeout(.1);thread=threading.Thread(target=peer);thread.start()
 code=None;stdout=b'';stderr=b'';process=None
 def reap_after_abort():
  # This is a new session owned solely by this fixture, including descendants.
  for sig,wait in [(signal.SIGCONT,.01),(signal.SIGTERM,1),(signal.SIGKILL,5)]:
   try:os.killpg(process.pid,sig)
   except ProcessLookupError:pass
   if sig==signal.SIGCONT:continue
   try:return process.communicate(timeout=wait)
   except subprocess.TimeoutExpired:continue
  raise RuntimeError('Owned VM did not reap after SIGKILL')
 try:
  process=subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.PIPE,start_new_session=True);state['vm_started']=True;state['pid']=process.pid;mark('vm-started');deadline=begin+vm_timeout;exit_seen=None
  while True:
   now=time.monotonic()
   if peer_failure.is_set() or now>=deadline:
    state['vm_timeout']=now>=deadline;abort.set();mark('vm-abort');stdout,stderr=reap_after_abort();code=process.returncode;break
   try:stdout,stderr=process.communicate(timeout=.05);code=process.returncode;break
   except subprocess.TimeoutExpired:
    if process.poll() is not None:
     if exit_seen is None:exit_seen=now
     elif now-exit_seen>=1:
      errors.append('VM exited but owned descendants kept captured streams open');abort.set();stdout,stderr=reap_after_abort();code=process.returncode;break
  state['vm_reaped']=process.poll() is not None
 except BaseException as error:
  errors.append(repr(error));abort.set()
  if process is not None:
   try:stdout,stderr=reap_after_abort();code=process.returncode;state['vm_reaped']=process.poll() is not None
   except BaseException as cleanup:errors.append(repr(cleanup))
 finally:
  state['vm_done_at']=time.monotonic();mark('vm-reaped' if state['vm_reaped'] else 'vm-not-reaped');vm_done.set()
  if code!=expected or state['vm_timeout']:abort.set()
  if thread:
   thread.join(6)
   if thread.is_alive():
    state['peer_forced_cleanup']=True;errors.append('Peer did not finish after VM cleanup');abort.set()
    with lock:connection=active[0]
    if connection is not None:
     try:connection.shutdown(socket.SHUT_RDWR)
     except OSError:pass
    thread.join(1)
    if thread.is_alive():errors.append('Peer thread still alive after forced cleanup')
  else:
   server.setblocking(False)
   while True:
    try:packet=server.recv(65536)
    except BlockingIOError:break
    payload.append(packet)
    if len(payload)>=16:errors.append('Unexpected repeated UDP datagrams');break
 if kind=='tcp-connect':
  if require_connection and state['accepted']!=1:errors.append('Required exactly one TCP connection; observed '+str(state['accepted']))
  if not require_connection and state['accepted'] not in (0,1):errors.append('Hidden import admitted extra TCP connections')
  if state['accepted'] and not state['peer_eof']:errors.append('Accepted TCP connection lacked genuine EOF')
 state['required_connection']=require_connection;state['peer_thread_alive']=bool(thread and thread.is_alive())
 return code,stdout,stderr,time.monotonic()-begin,payload,errors,state

rows=[]
for shared in (False,True):
 if a.profile!='all' and shared!=(a.profile=='shared'):continue
 for engine in ('int','aot'):
  for kind,filetype in (('tcp-connect',6),('udp-connect',5)):
   if a.kind and kind!=a.kind:continue
   if a.exercise_peer_lifecycle and kind!='tcp-connect':continue
   observed=check(get())+eq64(48,filetype)
   specs=[('no-rights','none',check(get(),76)+unchanged,0,'allow',[]),('write-only','write',check(get(),76)+unchanged,0,'allow',[]),('metadata','metadata',observed,0,'allow',[]),('data-and-metadata','write,metadata',observed,0,'allow',[]),('discarded','metadata',check(reduce())+check(get(),76)+unchanged,0,'allow',[]),('cannot-restore','metadata',check(reduce())+check(reduce(1<<21),76)+check(get(),76)+unchanged,0,'allow',[]),('renumber','metadata',check(call('rename',c(3),c(1)))+check(get(),8)+unchanged+check(get(1))+eq64(48,filetype),0,'allow',[]),('filesystem-quota','metadata',observed+init+check(get(),6)+unchanged,0,'allow',['--wasip1-single-limit','main','filesystem.total-calls','1']),('denied-bad-output','metadata',call('exit',get(pointer=65536)),76,'deny',[]),('hidden','metadata',call('exit',get()),127,'hidden',[])]
   if a.exercise_peer_lifecycle:
    # Five charged successes cross at least three complete quota windows after
    # connect, so a valid metadata-only peer must remain idle for over 2s.
    specs=[('peer-wait','metadata',(observed+init)*5,0,'allow',['--wasip1-single-limit','main','filesystem.window-calls','1','--wasip1-single-limit-action','main','filesystem.window-calls','wait'])]
   for case,rights,body,expected,state,extra in specs:
    if a.case and case not in a.case:continue
    name=case+'-'+kind+'-'+engine+('-shared' if shared else '-ordinary');imports={'get':'(param i32 i32)(result i32)','exit':'(param i32)'}
    if '$rights' in body:imports['rights']='(param i32 i64 i64)(result i32)'
    if '$rename' in body:imports['rename']='(param i32 i32)(result i32)'
    names={'get':'fd_filestat_get','rights':'fd_fdstat_set_rights','rename':'fd_renumber','exit':'proc_exit'}
    memory='(import "env" "memory"(memory 1 1 shared))' if shared else '(memory 1)'
    wat='(module'+''.join('(import "wasi_snapshot_preview1" "'+names[k]+'"(func $'+k+v+'))' for k,v in imports.items())+memory+'(func $check(param $got i32)(param $want i32)(if(i32.ne(local.get $got)(local.get $want))(then(call $exit(i32.const 101)))))(func(export "_start")'+init+body+')(func(export "wasi_thread_start")(param i32 i32)))';target=make(name,wat)
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM if kind=='tcp-connect' else socket.SOCK_DGRAM) as server:
     server.bind(('127.0.0.1',0))
     cmd=[str(a.vm.resolve()),'-R'+engine,'-Rct','0','-Rllvm-cache-path','disable','--wasm-set-main-module-name','main','--wasip1-single-create','main','--wasip1-single-enable','main','--wasip1-single-grant','main','proc-exit','--wasip1-single-socket-'+kind,'main','3','127.0.0.1:'+str(server.getsockname()[1]),'--wasip1-single-socket-rights','main','3',rights,'none']
     for key in imports:cmd+=['--wasip1-single-interface','main',names[key],state if key=='get' else 'allow']
     cmd+=extra
     if case=='renumber':cmd+=['--wasip1-single-grant','main','stdout.write','--wasip1-single-grant','main','stdio.metadata']
     if shared:cmd+=['-WFE-threads','--wasi-threads','3','3','1048576','16777216','67108864','65536','--wasm-preload-library',str(owner),'env']
     cmd+=['--run',str(target)]
     code,stdout,stderr,elapsed,payload,errors,lifecycle=run_guest(server,kind,cmd,expected,case!='hidden')
    if a.exercise_peer_lifecycle:
     events={event['event']:event['seconds'] for event in lifecycle['events']}
     idle=events.get('tcp-eof',0)-events.get('tcp-accepted',0);lifecycle['accepted_to_eof_seconds']=idle
     if idle<2.8:errors.append('Controlled quota-wait Guest did not keep its real TCP connection idle across the former 2s deadline')
    passed=code==expected and not stdout and not payload and not errors and lifecycle['vm_reaped'] and lifecycle['peer_finished'] and not lifecycle['vm_timeout'] and not lifecycle['peer_forced_cleanup'] and not lifecycle['peer_thread_alive']
    row=dict(name=name,engine=engine,shared=shared,case=case,exit=code,expected=expected,passed=passed,seconds=elapsed,argv=cmd,wasm_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),payload_hex=b''.join(payload).hex(),peer_errors=errors,peer_lifecycle=lifecycle,stderr=stderr.decode(errors='replace'));rows.append(row);(a.out/'report.json').write_text(json.dumps(rows,indent=2));print(name,code,'PASS' if passed else 'FAIL',flush=True)
    if not passed:print(row,flush=True)
raise SystemExit(any(not row['passed'] for row in rows))
