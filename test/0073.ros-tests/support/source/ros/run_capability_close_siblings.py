#!/usr/bin/env python3
import argparse,hashlib,json,os,pathlib,shutil,signal,subprocess,sys,time
parser=argparse.ArgumentParser(description="Real ROS2 same-alias handle close/reopen regression with frozen Worker/Gateway inputs")
parser.add_argument('--workspace',type=pathlib.Path,required=True)
parser.add_argument('--source',type=pathlib.Path,required=True)
parser.add_argument('--worker',type=pathlib.Path,required=True)
parser.add_argument('--worker-sha',required=True)
parser.add_argument('--gateway',type=pathlib.Path,required=True)
parser.add_argument('--gateway-sha',required=True)
parser.add_argument('--baseline',type=pathlib.Path,default=pathlib.Path('/home/macromodel/.local/share/uwvm-ros-extension/20261012-recovery'))
parser.add_argument('--domain',type=int,default=190)
parser.add_argument('--short-bind',type=pathlib.Path,default=pathlib.Path('/nacr2'))
parser.add_argument('--include-old',action='store_true')
parser.add_argument('--check-inputs',action='store_true')
args=parser.parse_args()
assert 0<=args.domain<=232
W=args.workspace.resolve();B=args.baseline.resolve()
A=B.parent/'20261013-continuation-r6';SOURCE=args.source.resolve();ROOT=Path(__file__).resolve().parents[5]
R=W/'rootfs';T=W/'native-temp';P=W/'reports';SHORT=args.short_bind
assert SHORT.is_absolute() and not SHORT.exists()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
check_files={'wat':SOURCE/'examples/ros2/capability_close_siblings.wat','witness':SOURCE/'test/0073.ros-tests/support/source/ros_handles/capability_close_witness_ros2.cc','policy_fixture':SOURCE/'test/0073.ros-tests/support/source/ros/run_network.py','worker':args.worker,'gateway':args.gateway}
for label,path in check_files.items():assert path.is_file(),label
assert sha(args.worker)==args.worker_sha and sha(args.gateway)==args.gateway_sha
assert (B/'gateway/jazzy-rootfs/opt/ros/jazzy/setup.bash').is_file()
if args.check_inputs:
 print(json.dumps({'status':'PASS','scope':'Read-only input and expected executable hash preparation check; no native/Wasm test run','driver_sha256':sha(pathlib.Path(__file__)),'inputs':{k:{'path':str(p),'sha256':sha(p)} for k,p in check_files.items()},'domain_id':args.domain,'workspace':str(W),'short_bind':str(SHORT)},indent=2));raise SystemExit(0)
W.mkdir(exist_ok=False)
for directory in (R,T,P):directory.mkdir()
shutil.copy2(check_files['witness'],W/'capability_close_witness_ros2.cc')
cmake='cmake_minimum_required(VERSION 3.16)\nproject(uwvm_next_alias_witness)\nset(CMAKE_CXX_STANDARD 20)\nfind_package(ament_cmake REQUIRED)\nfind_package(rclcpp REQUIRED)\nfind_package(std_msgs REQUIRED)\nadd_executable(alias-close-witness capability_close_witness_ros2.cc)\ntarget_include_directories(alias-close-witness PRIVATE /work/third-parties/fast_io/include)\nament_target_dependencies(alias-close-witness rclcpp std_msgs)\n'
(W/'CMakeLists.txt').write_text(cmake)
shutil.copy2(pathlib.Path(__file__),W/'executed-driver.py')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def save(p,x):p.write_text(json.dumps(x,indent=2,sort_keys=True)+'\n')
def inventory(root):return {str(p.relative_to(root)):sha(p) for p in root.rglob('*') if p.is_file()}
def phase_write(path,value):
 pending=path.with_suffix('.pending');pending.write_text(value+'\n');pending.replace(path)
def cg(pid):
 m=pathlib.Path(f'/proc/{pid}/cgroup').read_text().splitlines();g=pathlib.Path('/sys/fs/cgroup')/m[0].split(':',2)[2].lstrip('/');v=[];q=g
 while True:
  p=q/'memory.max'
  if p.is_file() and p.read_text().strip()!='max':v.append(int(p.read_text().strip()))
  if q==pathlib.Path('/sys/fs/cgroup'):break
  q=q.parent
 assert min(v)==68719476736
 return {'pid':pid,'membership':m,'effective_memory_max':min(v),'live_cgroup_procs':str(pid) in (g/'cgroup.procs').read_text().split(),'memory.events':(g/'memory.events').read_text()}
doc={'status':'FAIL','kind':'alias-close-regression','source':str(SOURCE),'source_bindings':{k:{'path':str(p),'sha256':sha(p)} for k,p in check_files.items()},'arguments':{k:str(v) if isinstance(v,pathlib.Path) else v for k,v in vars(args).items()},'driver_sha256':sha(pathlib.Path(__file__)),'domain_id':args.domain,'commands':[],'cases':[],'cgroup_before':cg(os.getpid())}
before=inventory(SOURCE);baseline=inventory(B/'source');prior_report=sha(A/'reports/report.json')
save(P/'w6-source-before.json',before);save(P/'baseline-source-before.json',baseline)
workers={'old':B/'product-final-v8-io/dual/uwvm2-ros','fixed':args.worker.resolve()} if args.include_old else {'fixed':args.worker.resolve()}
worker_shas={k:sha(v) for k,v in workers.items()}
assert worker_shas['fixed']==args.worker_sha
if args.include_old:assert worker_shas['old']=='38b83fa9d79f2eeae32b2bb105bb30abc22ba3f8edac3c18333ea28ba6ac58c6'
doc['workers']={k:{'source':str(v),'sha256':worker_shas[k]} for k,v in workers.items()};mounted=False;short_mounted=False;processes=[]
env=os.environ.copy();env.update({'ROS_DOMAIN_ID':str(args.domain),'RMW_IMPLEMENTATION':'rmw_fastrtps_cpp','ROS_SECURITY_ENABLE':'false','PYTHONDONTWRITEBYTECODE':'1','LD_LIBRARY_PATH':'/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib:/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu'})
def native(native_argv):return ['/usr/bin/chroot',str(R),'/usr/bin/env','-u','CC','-u','CXX','-u','LD_LIBRARY_PATH','PYTHONDONTWRITEBYTECODE=1','ROS_DOMAIN_ID='+str(args.domain),'RMW_IMPLEMENTATION=rmw_fastrtps_cpp','ROS_SECURITY_ENABLE=false','ROS_LOG_DIR=/tmp/ros-logs','/bin/bash','-lc','source /opt/ros/jazzy/setup.bash && exec "$@"','next-alias-close',*native_argv]
def start(name,argv,binary=None,stdin=False):
 row={'name':name,'argv':argv,'status':'RUNNING'};doc['commands'].append(row);log=P/(name+'.log');proof=P/(name+'-preexec.json')
 def preexec():save(proof,cg(os.getpid()))
 with log.open('wb') as f:p=subprocess.Popen(argv,stdout=f,stderr=subprocess.STDOUT,env=env,stdin=subprocess.PIPE if stdin else subprocess.DEVNULL,start_new_session=True,preexec_fn=preexec)
 row.update(pid=p.pid,log=str(log),preexec=str(proof));processes.append((p,row,binary));return p,row
def sample():
 for p,row,binary in processes:
  if not binary or 'actual_main' in row or p.poll() is not None:continue
  s=binary.stat();pending=[(p.pid,[p.pid])];seen=set()
  while pending:
   pid,chain=pending.pop()
   if pid in seen:continue
   seen.add(pid)
   try:
    pending += [(int(v),chain+[int(v)]) for v in pathlib.Path(f'/proc/{pid}/task/{pid}/children').read_text().split()]
    e=pathlib.Path(f'/proc/{pid}/exe');x=e.stat()
    if (x.st_dev,x.st_ino)==(s.st_dev,s.st_ino):
     c=cg(pid);assert c['live_cgroup_procs'] and c['membership']==doc['cgroup_before']['membership'] and sha(e)==sha(binary)
     parent=int(next(v.split()[1] for v in pathlib.Path(f'/proc/{pid}/status').read_text().splitlines() if v.startswith('PPid:')))
     assert len(chain)==1 or parent==chain[-2]
     row['actual_main']={**c,'sha256':sha(e),'exe':os.readlink(e),'ancestry':chain,'parent_pid':parent,'device':x.st_dev,'inode':x.st_ino}
   except (FileNotFoundError,ProcessLookupError):pass
def finish(p,row,timeout=60):
 until=time.monotonic()+timeout
 while p.poll() is None:
  sample()
  if time.monotonic()>until:raise RuntimeError('process deadline '+row['name'])
  time.sleep(.003)
 row.update(returncode=p.returncode,status='PASS' if p.returncode==0 else 'FAIL',log_sha256=sha(pathlib.Path(row['log'])),preexec_sha256=sha(pathlib.Path(row['preexec'])))
 return p.returncode
def run(name,argv,binary=None,timeout=60):
 p,row=start(name,argv,binary);assert finish(p,row,timeout)==0,name;return row
def wait_text(p,row,text,timeout=8):
 until=time.monotonic()+timeout
 while time.monotonic()<until:
  sample()
  if text in pathlib.Path(row['log']).read_text(errors='replace'):return
  if p.poll() is not None:raise RuntimeError(row['name']+' exited before '+text)
  time.sleep(.005)
 raise RuntimeError('marker deadline '+row['name']+' '+text)
def stop(p,row):
 if p.poll() is None:
  try:os.killpg(p.pid,signal.SIGTERM)
  except ProcessLookupError:pass
  try:p.wait(timeout=3)
  except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGKILL);p.wait(timeout=3)
 row.update(returncode=p.returncode,status='PASS' if p.returncode==0 else 'FAIL',log_sha256=sha(pathlib.Path(row['log'])),preexec_sha256=sha(pathlib.Path(row['preexec'])))
try:
 SHORT.mkdir();subprocess.run(['mount','--bind',str(W),str(SHORT)],check=True);short_mounted=True
 subprocess.run(['mount','--rbind',str(B/'gateway/jazzy-rootfs'),str(R)],check=True);mounted=True
 subprocess.run(['mount','--make-rprivate',str(R)],check=True)
 subprocess.run(['mount','--bind',str(T),str(R/'tmp')],check=True)
 subprocess.run(['mount','--bind',str(SOURCE),str(R/'work')],check=True)
 subprocess.run(['mount','-o','remount,bind,ro',str(R/'work')],check=True)
 witness_source=T/'witness-source';witness_source.mkdir()
 for name in ('capability_close_witness_ros2.cc','CMakeLists.txt'):shutil.copy2(W/name,witness_source/name)
 run('witness-cmake',native(['/usr/bin/cmake','-S','/tmp/witness-source','-B','/tmp/witness-build','-DCMAKE_BUILD_TYPE=RelWithDebInfo']),R/'usr/bin/cmake')
 run('witness-build',native(['/usr/bin/cmake','--build','/tmp/witness-build','--parallel','1']),R/'usr/bin/cmake',120)
 witness=T/'witness-build/alias-close-witness';doc['witness_sha256']=sha(witness)
 gateway=T/'gateway';shutil.copy2(args.gateway,gateway)
 assert sha(gateway)==args.gateway_sha
 doc['gateway_sha256']=sha(gateway)
 sys.path.insert(0,str(SOURCE/'test/0073.ros-tests/support/source/ros'));import run_network
 for mode,worker in workers.items():
  case=T/('alias-'+mode);case.mkdir(mode=0o700);resource='/uwvm_next_20261013/same_alias_'+mode+'_'+str(os.getpid())
  phase=case/'native-phase';phase_write(phase,'initial')
  wat=check_files['wat'];wasm=case/'guest.wasm'
  run(mode+'-wat2wasm',['/usr/bin/wat2wasm',str(wat),'-o',str(wasm)],pathlib.Path('/usr/bin/wat2wasm'))
  connection,grants=run_network.policy(2,['subscribe']);connection.update({'domain-id':str(args.domain),'node-name':'uwvm_ros2_next_alias','namespace':'/uwvm_next_20261013'})
  grant=grants[0];grant['resource']=resource;grant['limits'].update({'max-subscribers':'2','max-rate-hz':'100'})
  cli=['--ros2-global-enable']
  for key,value in {**connection,'gateway-socket':str(SHORT/'native-temp'/case.name/'gateway.sock'),'gateway-uid':str(os.getuid()),'connect-timeout-ms':'1000','audit-deny-max-rate-hz':'100','audit-deny-burst':'100','audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'}.items():cli += ['--ros2-global-'+key,value]
  cli += ['--ros2-global-grant',*[grant[k] for k in ('alias','operation','resource','type','identity')]]
  for key,value in grant['limits'].items():cli += ['--ros2-global-set','sub',key,value]
  argv=[str(worker),*cli,'--wasip1-global-noinherit-system-environment','--wasip1-global-grant','monotonic-clock','--wasm-set-main-module-name','main','--run',str(wasm)]
  child,wr=start(mode+'-worker',['/bin/bash','-c','kill -STOP $$; exec "$@"','next-worker',*argv],worker,True)
  config=case/'gateway.txt';inside='/tmp/alias-'+mode
  lines=['backend ros2','worker-uid '+str(os.getuid()),'worker-pid '+str(child.pid),'socket '+inside+'/gateway.sock','timeout-ms 10000','max-sessions 16','audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
  lines += ['connection '+k+' '+v for k,v in connection.items()]
  lines += ['grant '+' '.join(grant[k] for k in ('alias','operation','resource','type','identity'))]
  lines += ['set sub '+k+' '+v for k,v in grant['limits'].items()];config.write_text('\n'.join(lines)+'\n');os.chmod(config,0o600)
  gw,gr=start(mode+'-gateway',native(['/tmp/gateway',inside+'/gateway.txt']),gateway)
  end=time.monotonic()+5
  while not (case/'gateway.sock').exists():
   sample();assert gw.poll() is None
   if time.monotonic()>end:raise RuntimeError('gateway socket deadline')
   time.sleep(.01)
  node,nr=start(mode+'-native-witness',native(['/tmp/witness-build/alias-close-witness',resource,inside+'/native-phase']),witness)
  os.kill(child.pid,signal.SIGCONT)
  wait_text(child,wr,'FIRST_DDS_RECEIVED');wait_text(node,nr,'NATIVE_INITIAL actual_count=1');child.stdin.write(b'x');child.stdin.flush()
  stages=[('SIBLING_QUEUE_LOST error=9','closed',0)] if mode=='old' else [('SIBLING_QUEUE_SURVIVED','survived',1),('LAST_HANDLE_CLOSED','final',0),('REOPENED_SUBSCRIPTION_READY','reopened',1),('REOPENED_LAST_HANDLE_CLOSED','reopenedfinal',0)]
  for marker,stage,count in stages:
   wait_text(child,wr,marker);phase_write(phase,stage);wait_text(node,nr,'NATIVE_PHASE '+stage+' actual_count='+str(count));child.stdin.write(b'x');child.stdin.flush()
  code=finish(child,wr,5);assert code==(209 if mode=='old' else 0)
  phase_write(phase,'finish');assert finish(node,nr,5)==0
  raw=pathlib.Path(gr['log']).read_text();audit=[v for v in raw.splitlines() if 'ros_audit ' in v]
  assert any('operation=capability.close' in v and 'error=0' in v for v in audit)
  if mode=='old':assert any('operation=event.poll' in v and 'error=9' in v for v in audit)
  else:
   assert sum('operation=capability.close' in v and 'error=0' in v for v in audit)==2
   guest=pathlib.Path(wr['log']).read_text();assert 'AFTER_CLOSE_DDS_RECEIVED' in guest and 'REOPENED_DDS_RECEIVED' in guest
   assert not any('operation=event.poll' in v and 'error=9' in v for v in audit)
  assert all('actual_main' in x for x in (wr,gr,nr))
  result={'name':mode,'status':'PASS' if code==0 else 'FAIL','expected_reproduction':mode=='old','worker_returncode':code,'resource':resource,'wat_sha256':sha(wat),'wasm_sha256':sha(wasm),'config_sha256':sha(config),'actual_worker_pid':wr['actual_main']['pid'],'actual_gateway_pid':gr['actual_main']['pid'],'actual_witness_pid':nr['actual_main']['pid'],'native_stages':stages,'gateway_audit':audit}
  doc['cases'].append(result);stop(gw,gr);print(json.dumps({k:result[k] for k in ('name','status','worker_returncode','native_stages')}),flush=True)
 assert len(doc['cases'])==len(workers) and len({x['wasm_sha256'] for x in doc['cases']})==1
 doc['status']='PASS';doc['conclusion']='Frozen Wasm contract passes0 on the selected fresh Worker (when included, the old Worker fails209): sibling close preserves native subscriber and receives exact new after-close DDS; last close removes subscriber, reopen receives exact new DDS, and reopened last close retires it.'
except Exception as error:doc['reason']=str(error)
finally:
 for p,row,_ in reversed(processes):stop(p,row)
 if mounted:
  for attempt in range(20):
   q=subprocess.run(['umount','-R',str(R)],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
   if not q.returncode:break
   time.sleep(.05)
  doc['mount_cleanup']={'returncode':q.returncode,'output':q.stdout}
  if q.returncode:doc['status']='FAIL'
 if short_mounted:
  q=subprocess.run(['umount',str(SHORT)],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);doc['short_mount_cleanup']={'returncode':q.returncode,'output':q.stdout}
  if q.returncode:doc['status']='FAIL'
  else:SHORT.rmdir()
 doc['frozen_source_unchanged']=inventory(SOURCE)==before;doc['baseline_source_unchanged']=inventory(B/'source')==baseline
 doc['w6_report_unchanged']=sha(A/'reports/report.json')==prior_report;doc['workers_unchanged']=all(sha(v)==worker_shas[k] for k,v in workers.items())
 doc['cgroup_after']=cg(os.getpid());events=lambda v:dict((k,int(x)) for k,x in (line.split() for line in v.splitlines()))
 a,b=events(doc['cgroup_before']['memory.events']),events(doc['cgroup_after']['memory.events']);doc['oom_delta']={k:b[k]-a[k] for k in ('oom','oom_kill','oom_group_kill')}
 if not all(doc[k] for k in ('frozen_source_unchanged','baseline_source_unchanged','w6_report_unchanged','workers_unchanged')) or any(doc['oom_delta'].values()):doc['status']='FAIL'
 save(P/'report.json',doc);print(json.dumps({'status':doc['status'],'reason':doc.get('reason'),'report':str(P/'report.json'),'cleanup':doc.get('mount_cleanup'),'oom_delta':doc['oom_delta']}),flush=True)
 if doc['status']!='PASS':raise SystemExit(1)
