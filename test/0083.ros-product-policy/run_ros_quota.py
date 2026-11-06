"""Real pinned native ROS gateway quota/diagnostic regression; no installation."""
from pathlib import Path
import argparse,hashlib,json,os,signal,socket,subprocess,sys,time,xmlrpc.client
p=argparse.ArgumentParser();p.add_argument('--runtime',type=Path,required=True);p.add_argument('--root',type=Path,required=True);p.add_argument('--config-root',type=Path,required=True);a=p.parse_args()
a.root.mkdir(parents=True,exist_ok=False)
membership=Path('/proc/self/cgroup').read_text().strip();group=Path('/sys/fs/cgroup')/membership[3:].lstrip('/')
assert (group/'memory.max').read_text().strip()==str(64*1024**3) and (group/'memory.swap.max').read_text().strip()=='0'
assert [x[1] for x in socket.if_nameindex()]==['lo']
results=[]
def digest(f):
 with Path(f).open('rb') as s:return hashlib.file_digest(s,'sha256').hexdigest()
def wait_for(check,seconds=15):
 end=time.monotonic()+seconds
 while not check():
  if time.monotonic()>end:raise TimeoutError('native readiness')
  time.sleep(.01)
for family,filename in [('ros1','required-native/vendors-ros1/deployment.json'),('ros2','native-ros/deployment.json')]:
 cfg=json.loads((a.config_root/filename).read_text());ros=Path(cfg['ros_prefix']);system=Path(cfg['system_prefix'])
 gateway=Path(cfg['gateway']['path']);assert digest(gateway)==cfg['gateway']['sha256']
 for item in cfg['native_inputs']:assert digest(item['path'])==item['sha256'],item['path']
 env={'PATH':os.defpath,'LANG':'C.UTF-8','LC_ALL':'C.UTF-8','LD_LIBRARY_PATH':':'.join([str(ros/'lib'),str(system/'lib/x86_64-linux-gnu'),*cfg.get('native_library_paths',[])])}
 if family=='ros1':env.update(PYTHONPATH=':'.join(cfg['python_paths']),PYTHONDONTWRITEBYTECODE='1',ROS_MASTER_URI='http://127.0.0.1:11311',ROS_IP='127.0.0.1')
 else:env.update(AMENT_PREFIX_PATH=str(ros),ROS_DOMAIN_ID='73',RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_AUTOMATIC_DISCOVERY_RANGE='LOCALHOST',ROS_STATIC_PEERS='')
 for engine in ('int','aot'):
  for mode in ('unlimited','silent','family','single','fatal','wait'):
   work=a.root/str(len(results));work.mkdir(mode=0o700);env['ROS_LOG_DIR']=str(work/'roslogs')
   sock=work/'g.sock';expected=0 if mode in ('unlimited','wait') else 11
   wat=work/'guest.wat'
   wat.write_text('(module (import "uwvm_'+family+'_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32))) (memory (export "memory") 1) (data (i32.const 0) "clock") (func (export "_start") i32.const 0 i64.const 0 i64.const 5 i64.const 8 call $open if unreachable end i32.const 0 i64.const 0 i64.const 5 i64.const 16 call $open i32.const '+str(expected)+' i32.ne if unreachable end))')
   wasm=wat.with_suffix('.wasm');subprocess.run(['wat2wasm',str(wat),'-o',str(wasm)],check=True)
   values={'gateway-socket':str(sock),'gateway-uid':str(os.getuid()),'node-name':'quota_guest','namespace':'/'}
   if family=='ros1':values.update({'master-uri':env['ROS_MASTER_URI'],'advertise-host':'127.0.0.1','transport':'tcpros'})
   else:values.update({'domain-id':'73','rmw-implementation':'rmw_fastrtps_cpp','security-mode':'disabled-test','keystore':'not-applicable','enclave':'not-applicable'})
   limits={'audit':'deny-only'}
   if mode!='unlimited':limits.update({'max-rate-hz':'1','burst':'1','max-rate-hz.on-exhausted':'wait' if mode=='wait' else 'reject'})
   prefix='--'+family+'-single-'
   operation='time.read-wall' if family=='ros1' else 'time.read-steady'
   argv=[str(a.runtime),'--runtime-'+engine,'--wasm-set-main-module-name','main',prefix+'create','main',prefix+'enable','main',prefix+'interface','main','uwvm_'+family+'_v1.capability_open','allow',prefix+'grant','main','clock',operation,'/clock','builtin/time-v1','builtin:time-nanoseconds-v1']
   for key,value in values.items():argv += [prefix+key,'main',value]
   for key,value in limits.items():argv += [prefix+'set','main','clock',key,value]
   if mode in ('family','wait'):argv += ['--enable-warning','all_host_limited_'+family]
   if mode in ('single','fatal'):argv += ['--warning-as-error' if mode=='fatal' else '--enable-warning','host_limited_'+family+'.uwvm_'+family+'_v1.capability_open']
   argv += ['--run',str(wasm)]
   actors=[];gatew=None
   def spawn(name,args,**kwargs):
    with (work/(name+'.stdout')).open('wb') as out,(work/(name+'.stderr')).open('wb') as err:
     child=subprocess.Popen(args,env=env,stdout=out,stderr=err,start_new_session=True,**kwargs)
    actors.append(child);return child
   try:
    gater,gatew=os.pipe2(os.O_CLOEXEC)
    helper='import os,sys; g=int(sys.argv[1]); assert os.read(g,1)==b"R"; os.close(g); os.execv(sys.argv[2],sys.argv[2:])'
    guest=spawn('vm',[sys.executable,'-c',helper,str(gater),*argv],pass_fds=(gater,));os.close(gater)
    if family=='ros1':
     master=spawn('master',[cfg['master']['path'],cfg['master_script']['path'],'--core','-p','11311'])
     class Transport(xmlrpc.client.Transport):
      def make_connection(self,host):
       c=super().make_connection(host);c.timeout=.2;return c
     def ready():
      assert master.poll() is None,(work/'master.stderr').read_text()
      try:
       with xmlrpc.client.ServerProxy(env['ROS_MASTER_URI'],transport=Transport()) as proxy:return proxy.getPid('/quota_test')[0]==1
      except (OSError,xmlrpc.client.Error):return False
     wait_for(ready)
    lines=['backend '+family,'worker-uid '+str(os.getuid()),'worker-pid '+str(guest.pid),'socket '+str(sock)]
    lines += ['connection '+k+' '+v for k,v in values.items() if k not in ('gateway-socket','gateway-uid')]
    lines += ['grant clock '+operation+' /clock builtin/time-v1 builtin:time-nanoseconds-v1']
    lines += ['set clock '+k+' '+v for k,v in limits.items()]
    contract=work/'gateway.conf';contract.write_text('\n'.join(lines)+'\n');contract.chmod(0o600)
    bridge=spawn('gateway',[str(gateway),str(contract)])
    wait_for(lambda:sock.exists() or bridge.poll() is not None)
    assert bridge.poll() is None,(work/'gateway.stderr').read_text()
    start=time.monotonic();os.write(gatew,b'R');os.close(gatew);gatew=None
    code=guest.wait(timeout=30);elapsed=time.monotonic()-start
    text=(work/'vm.stderr').read_text()
    assert code==(1 if mode=='fatal' else 0),(family,engine,mode,code,text)
    if mode in ('unlimited','silent'):assert '[warning]' not in text,text
    elif mode=='fatal':assert '[error]' in text and 'max-rate-hz' in text,text
    else:assert '[warning]' in text and 'max-rate-hz' in text,text
    if mode=='wait':assert elapsed>=.9,elapsed
    results.append({'family':family,'engine':engine,'mode':mode,'exit':code,'elapsed':elapsed,'argv':argv,'gateway':str(gateway),'gateway_sha256':digest(gateway)})
    (a.root/'results.json').write_text(json.dumps({'complete':False,'runtime_sha256':digest(a.runtime),'cases':results},indent=2))
    print(family,engine,mode,'PASS',round(elapsed,3),flush=True)
   finally:
    if gatew is not None:os.close(gatew)
    for child in reversed(actors):
     if child.poll() is None:os.killpg(child.pid,signal.SIGTERM)
    for child in reversed(actors):
     try:child.wait(timeout=5)
     except subprocess.TimeoutExpired:os.killpg(child.pid,signal.SIGKILL);child.wait()
(a.root/'results.json').write_text(json.dumps({'complete':True,'runtime_sha256':digest(a.runtime),'cases':results},indent=2))
