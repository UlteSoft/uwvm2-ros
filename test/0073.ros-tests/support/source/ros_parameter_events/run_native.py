#!/usr/bin/env python3
import argparse,hashlib,json,os,pathlib,re,shutil,signal,subprocess,time,traceback
p=argparse.ArgumentParser(description='Build expanded ParameterEvent probes in a fresh private Jazzy rootfs; optional final Worker runs 70 Wasm cases.')
p.add_argument('--out','--workspace',dest='workspace',type=pathlib.Path,required=True)
p.add_argument('--source',type=pathlib.Path,required=True,help='Frozen production source snapshot')
p.add_argument('--probe-source',type=pathlib.Path,help='Frozen complete source containing the actual probe revision (defaults to --source)')
p.add_argument('--rootfs-workspace','--baseline',dest='baseline',type=pathlib.Path,required=True,help='Workspace with gateway/jazzy-rootfs; never written')
p.add_argument('--gateway',type=pathlib.Path,help='Final prebuilt Gateway to snapshot; omitted means build it')
p.add_argument('--uwvm',type=pathlib.Path,help='Final dual Worker to snapshot and actually run all 70 Wasm cases')
p.add_argument('--llvm-libs',default=os.environ.get('LD_LIBRARY_PATH',''))
p.add_argument('--short-bind',default='/pev191',help='Fresh host path for the private short IPC socket bind')
p.add_argument('--wat2wasm',default='/usr/bin/wat2wasm')
a=p.parse_args()
import sys
sys.dont_write_bytecode=True
production_source=a.source.resolve()

W=a.workspace.resolve();S=(a.probe_source or a.source).resolve();B=a.baseline.resolve();assert not W.is_relative_to(S) and not W.is_relative_to(production_source) and not W.is_relative_to(B/'gateway')
W.mkdir(mode=0o700);R=W/'rootfs';T=W/'native-temp';P=W/'reports'
for d in (R,T,P):d.mkdir()
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def save(p,x):p.write_text(json.dumps(x,indent=2,sort_keys=True)+'\n')
def inventory(root):return {str(p.relative_to(root)):H(p) for p in root.rglob('*') if p.is_file()}
def cg(pid):
 membership=pathlib.Path('/proc',str(pid),'cgroup').read_text().splitlines();g=pathlib.Path('/sys/fs/cgroup')/next(v.split(':',2)[2] for v in membership if v.startswith('0::')).lstrip('/');ancestors=[];q=g
 while True:
  if (q/'memory.max').is_file():ancestors.append({'path':str(q),'memory.max':(q/'memory.max').read_text().strip(),'memory.events':(q/'memory.events').read_text() if (q/'memory.events').is_file() else None})
  if q==pathlib.Path('/sys/fs/cgroup'):break
  q=q.parent
 maximum=min(int(v['memory.max']) for v in ancestors if v['memory.max']!='max');assert maximum==68719476736
 return {'pid':pid,'membership':membership,'effective_memory_max':maximum,'ancestors':ancestors,'live_cgroup_procs':str(pid) in (g/'cgroup.procs').read_text().split()}
doc={'status':'FAIL','kind':'expanded-parameter-events-reproducible','production_source':str(production_source),'argv':sys.argv,'source':str(S),'domain_id':191,'ROS_LOG_DIR':'/tmp/ros-logs','executed_driver_sha256':H(pathlib.Path(__file__)),'commands':[],'cgroup_before':cg(os.getpid())}
env=os.environ.copy();env['PYTHONDONTWRITEBYTECODE']='1'
shutil.copy2(__file__,W/'executed-driver.py');before=inventory(S);save(P/'source-before.json',before)
production_before=inventory(production_source);save(P/'production-source-before.json',production_before)
manifest=S.parent/'source-manifest.json';doc['source_manifest_sha256']=H(manifest);children=[];mounted=False;gateway=None;probe=None;witness=None

def wasm_matrix(worker_origin):
    import sys
    sys.path.insert(0,str(S/'test/0073.ros-tests/support/source/ros'))
    sys.path.insert(0,str(S/'test/0073.ros-tests/support/source/ros_parameter_events'))
    import events
    from run_rpc import scalar_grant
    cases=T/'cases';cases.mkdir(mode=0o700)
    worker=T/'snapshots/uwvm2-ros';worker.parent.mkdir(exist_ok=True);shutil.copy2(worker_origin,worker)
    assert H(worker)==H(worker_origin)
    doc['worker']={'source':str(worker_origin),'snapshot':str(worker),'sha256':H(worker)}
    short=pathlib.Path(a.short_bind);assert short.is_absolute() and len(str(short))<32 and not short.exists()
    short.mkdir();subprocess.run(['mount','--bind',str(cases),str(short)],check=True);doc['short_bind']=str(short)
    doc['wasm_cases']=[]
    def start(name,command,binary,worker_env=None):
        log=P/(name+'.log');proof=P/(name+'-preexec.json');row={'name':name,'command':command,'log':str(log),'status':'RUNNING'};doc['commands'].append(row)
        def preexec():save(proof,cg(os.getpid()))
        with log.open('wb') as output:
            process=subprocess.Popen(command,stdout=output,stderr=subprocess.STDOUT,start_new_session=True,preexec_fn=preexec,env=worker_env or env)
        row.update(pid=process.pid);children.append((process,row,binary));return process,row
    def wait_for(process,row,timeout):
        deadline=time.monotonic()+timeout
        while process.poll() is None:
            sample()
            if time.monotonic()>deadline:raise RuntimeError('command deadline '+row['name'])
            time.sleep(.005)
        row.update(returncode=process.returncode,log_sha256=H(pathlib.Path(row['log'])),preexec_sha256=H(P/(row['name']+'-preexec.json')),status='PASS' if process.returncode==0 else 'FAIL')
        assert process.returncode==0,(row['name'],process.returncode,pathlib.Path(row['log']).read_text(errors='replace'))
    def terminate(process,row):
        if process.poll() is None:
            os.killpg(process.pid,signal.SIGTERM)
            try:process.wait(timeout=3)
            except subprocess.TimeoutExpired:os.killpg(process.pid,signal.SIGKILL);process.wait(timeout=5)
        if row['status']=='RUNNING':row['status']='STOPPED'
        row.update(returncode=process.returncode,log_sha256=H(pathlib.Path(row['log'])),preexec_sha256=H(P/(row['name']+'-preexec.json')))
    for index,description in enumerate(events.cases()):
        name=description['name'];kind=description['kind'];scenario=description['scenario'];denied=scenario in ('permission-denied','no-capability')
        directory=cases/('c'+str(index).zfill(2));directory.mkdir(mode=0o700)
        selected=events.grants(description,scalar_grant)
        connection={'node-name':'worker_gateway','namespace':'/typed_events','domain-id':'191','rmw-implementation':'rmw_fastrtps_cpp','security-mode':'disabled-test','keystore':'not-applicable','enclave':'not-applicable'}
        argv=[str(worker),'--ros2-global-enable']
        for key,value in {**connection,'gateway-socket':str(short/directory.name/'ros2.sock'),'gateway-uid':str(os.getuid()),'connect-timeout-ms':'1000','audit-deny-max-rate-hz':'100','audit-deny-burst':'100','audit-deny-max-records':'10000','audit-deny-max-bytes':'1048576'}.items():argv += ['--ros2-global-'+key,value]
        for grant in selected:
            argv += ['--ros2-global-grant',*[grant[key] for key in ('alias','operation','resource','type','identity')]]
            for key,value in grant['limits'].items():argv += ['--ros2-global-set',grant['alias'],key,value]
        guest=W/'fixtures'/(name+'.wasm');argv += ['--wasip1-global-noinherit-system-environment','--wasip1-global-grant','monotonic-clock','--wasm-set-main-module-name','main','--run',str(guest)]
        save(directory/'worker-argv.json',argv)
        worker_env=env.copy();worker_env['LD_LIBRARY_PATH']=a.llvm_libs
        worker_process,worker_row=start(name+'-worker',['/bin/bash','-c','kill -STOP $$; exec "$@"','expanded-event-worker',*argv],worker,worker_env)
        deadline=time.monotonic()+3
        while True:
            assert worker_process.poll() is None,'worker exited before STOP'
            if '\nState:\tT ' in pathlib.Path('/proc',str(worker_process.pid),'status').read_text():break
            assert time.monotonic()<deadline,'worker STOP deadline';time.sleep(.005)
        config=directory/'gateway.txt'
        lines=['backend ros2',f'worker-uid {os.getuid()}',f'worker-pid {worker_process.pid}',f'socket /tmp/cases/{directory.name}/ros2.sock','timeout-ms 10000','max-sessions 16','audit-max-records-per-second 100','audit-max-records 10000','audit-max-bytes 1048576']
        lines += [f'connection {key} {value}' for key,value in connection.items()]
        for grant in selected:
            lines.append('grant '+' '.join(grant[key] for key in ('alias','operation','resource','type','identity')))
            lines += [f"set {grant['alias']} {key} {value}" for key,value in grant['limits'].items()]
        config.write_text('\n'.join(lines)+'\n');config.chmod(0o600)
        gateway_process,gateway_row=start(name+'-gateway',native([gateway_inside,f'/tmp/cases/{directory.name}/gateway.txt']),gateway)
        deadline=time.monotonic()+5
        while not (directory/'ros2.sock').exists():
            sample();assert gateway_process.poll() is None,(name,'gateway startup');assert time.monotonic()<deadline,(name,'gateway socket deadline');time.sleep(.01)
        witness_process,witness_row=start(name+'-witness',native(['/tmp/event-build/witness_ros2',kind,'deny-subscription' if denied else scenario,'/typed_events/worker_gateway']),witness)
        deadline=time.monotonic()+8;baseline=None
        while baseline is None:
            sample();assert witness_process.poll() is None,(name,'native witness exited before baseline')
            markers=[line for line in pathlib.Path(witness_row['log']).read_text().splitlines() if line.startswith('BASELINE_READY ')]
            if markers:baseline=markers[0]
            else:assert time.monotonic()<deadline,(name,'native framework baseline deadline');time.sleep(.01)
        os.kill(worker_process.pid,signal.SIGCONT)
        deadline=time.monotonic()+5
        while str(worker) not in worker_row.get('actual_mains',{}):
            sample(worker_process.pid)
            if str(worker) in worker_row.get('actual_mains',{}):break
            assert worker_process.poll() is None,(name,'worker exited before actual main capture')
            assert time.monotonic()<deadline,(name,'worker actual main capture deadline')
        wait_for(worker_process,worker_row,20);wait_for(witness_process,witness_row,15)
        native_log=pathlib.Path(witness_row['log']).read_text();assert 'PASS expanded ParameterEvent witness' in native_log,(name,'missing real witness PASS')
        for row,binary in ((worker_row,worker),(gateway_row,gateway),(witness_row,witness)):
            assert str(binary) in row.get('actual_mains',{}),(name,'missing actual main',str(binary))
        if denied:assert 'DENIED_TYPED_SUBSCRIPTION projection_readers=0' in native_log,(name,'missing actual projection absence')
        terminate(gateway_process,gateway_row)
        record={**description,'status':'PASS','wasm_exit':0,'native_exit':0,'grants':selected,'connection':connection,'native_baseline':baseline,'expected_event_hex':[] if denied else [value.hex() for value in events.expected_events(kind,scenario)],'guest':{'wat':str(guest.with_suffix('.wat')),'wat_sha256':H(guest.with_suffix('.wat')),'wasm':str(guest),'wasm_sha256':H(guest)},'command_names':[worker_row['name'],gateway_row['name'],witness_row['name']],'native_log':witness_row['log'],'native_log_sha256':witness_row['log_sha256']}
        doc['wasm_cases'].append(record);save(P/'wasm-progress.json',doc['wasm_cases']);print('PASS Wasm expanded ParameterEvent '+name,flush=True)
    assert len(doc['wasm_cases'])==70


def native(argv):return ['/usr/bin/chroot',str(R),'/usr/bin/env','-u','CC','-u','CXX','-u','LD_LIBRARY_PATH','PYTHONDONTWRITEBYTECODE=1','ROS_DOMAIN_ID=191','RMW_IMPLEMENTATION=rmw_fastrtps_cpp','ROS_SECURITY_ENABLE=false','ROS_LOG_DIR=/tmp/ros-logs','/bin/bash','-lc','source /opt/ros/jazzy/setup.bash && exec "$@"','expanded-events-native',*argv]
def sample(target_pid=None):
 for process,row,binary in children:
  if target_pid is not None and process.pid!=target_pid:continue
  if process.poll() is not None:continue
  expected={str(v):v.stat() for v in (binary,gateway) if v is not None and v.exists()};pending=[(process.pid,[process.pid])];seen=set()
  while pending:
   pid,ancestry=pending.pop()
   if pid in seen:continue
   seen.add(pid)
   try:
    pending += [(int(v),ancestry+[int(v)]) for v in pathlib.Path('/proc',str(pid),'task',str(pid),'children').read_text().split()]
    fd=os.open(pathlib.Path('/proc',str(pid),'exe'),os.O_RDONLY)
    with os.fdopen(fd,'rb') as file:
     actual=os.fstat(file.fileno());match=next((name for name,st in expected.items() if (st.st_dev,st.st_ino)==(actual.st_dev,actual.st_ino)),None)
     if match and match not in row.setdefault('actual_mains',{}):
      # Capture live identity before hashing the pinned ELF FD: a deny guest can
      # exit while its large executable is hashed, but the FD stays valid.
      c=cg(pid);parent=int(next(v.split()[1] for v in pathlib.Path('/proc',str(pid),'status').read_text().splitlines() if v.startswith('PPid:')))
      exe=os.readlink(pathlib.Path('/proc',str(pid),'exe'));captured=time.monotonic_ns()
      assert c['membership']==doc['cgroup_before']['membership'] and c['live_cgroup_procs']
      assert parent==(os.getpid() if len(ancestry)==1 else ancestry[-2])
      digest=hashlib.sha256()
      for block in iter(lambda:file.read(1048576),b''):digest.update(block)
      assert digest.hexdigest()==H(pathlib.Path(match))
      row['actual_mains'][match]={**c,'parent_pid':parent,'ancestry':ancestry,'sha256':digest.hexdigest(),'exe':exe,'device':actual.st_dev,'inode':actual.st_ino,'live_identity_captured_before_fd_hash':True,'capture_monotonic_ns':captured}
   except (FileNotFoundError,ProcessLookupError):pass
   except PermissionError as error:row.setdefault('sampling_unavailable',[]).append({'pid':pid,'reason':str(error)})
def run(name,argv,binary=None,timeout=180):
 log=P/(name+'.log');proof=P/(name+'-preexec.json');row={'name':name,'command':argv,'log':str(log),'status':'FAIL'};doc['commands'].append(row)
 def preexec():save(proof,cg(os.getpid()))
 with log.open('wb') as stream:process=subprocess.Popen(argv,stdout=stream,stderr=subprocess.STDOUT,start_new_session=True,preexec_fn=preexec,env=env)
 children.append((process,row,binary));row['pid']=process.pid;deadline=time.monotonic()+timeout
 while process.poll() is None:
  sample()
  if time.monotonic()>deadline:raise RuntimeError('command deadline '+name)
  time.sleep(.005)
 row.update(returncode=process.returncode,log_sha256=H(log),preexec_sha256=H(proof));assert process.returncode==0,(name,log.read_text(errors='replace'))
 if binary:assert str(binary) in row.get('actual_mains',{}),'missing actual main '+name
 row['status']='PASS';print('PASS',name,flush=True);return row
def interrupted(signum,frame):raise RuntimeError("interrupted by signal "+str(signum))
signal.signal(signal.SIGTERM,interrupted);signal.signal(signal.SIGINT,interrupted)
try:
 subprocess.run(['mount','--rbind',str(B/'gateway/jazzy-rootfs'),str(R)],check=True);mounted=True
 subprocess.run(['mount','--make-rprivate',str(R)],check=True);subprocess.run(['mount','--bind',str(T),str(R/'tmp')],check=True)
 subprocess.run(['mount','--bind',str(S),str(R/'work')],check=True);subprocess.run(['mount','-o','remount,bind,ro',str(R/'work')],check=True)
 run('compiler-version',native(['/usr/bin/c++','--version']))
 if a.gateway:
  (T/'snapshots').mkdir();gateway=T/'snapshots/uwvm-ros2-gateway';shutil.copy2(a.gateway,gateway);assert H(gateway)==H(a.gateway)
  gateway_inside='/tmp/snapshots/uwvm-ros2-gateway';doc['gateway_origin']=str(a.gateway.resolve())
 else:
  run('gateway-cmake',native(['/usr/bin/cmake','-S','/work/src/ros/gateway/ros2','-B','/tmp/gateway-build','-DCMAKE_BUILD_TYPE=RelWithDebInfo']))
  run('gateway-build',native(['/usr/bin/cmake','--build','/tmp/gateway-build','--target','uwvm-ros2-gateway','--parallel','1']),timeout=300)
  gateway=T/'gateway-build/uwvm-ros2-gateway';gateway_inside='/tmp/gateway-build/uwvm-ros2-gateway'
 doc['gateway_sha256']=H(gateway)
 run('events-cmake',native(['/usr/bin/cmake','-S','/work/test/0073.ros-tests/support/source/ros_parameter_events','-B','/tmp/event-build','-DUWVM_SOURCE=/work','-DCMAKE_BUILD_TYPE=RelWithDebInfo']))
 run('events-build',native(['/usr/bin/cmake','--build','/tmp/event-build','--parallel','1']),timeout=300)
 probe=T/'event-build/native_ros2';witness=T/'event-build/witness_ros2';doc['native_probe_sha256']=H(probe);doc['witness_sha256']=H(witness)
 run('fixtures-generate',['/usr/bin/python3',str(S/'test/0073.ros-tests/support/source/ros_parameter_events/generate_fixtures.py'),'--output',str(W/'fixtures')])
 doc['fixture_compilations']=[]
 for path in sorted((W/'fixtures').glob('*.wat')):
  run('fixture-compile-'+path.stem,[a.wat2wasm,'--enable-all',str(path),'-o',str(path.with_suffix('.wasm'))])
  doc['fixture_compilations'].append({'name':path.stem,'wat':str(path),'wat_sha256':H(path),'wasm':str(path.with_suffix('.wasm')),'wasm_sha256':H(path.with_suffix('.wasm'))})
 assert len(doc['fixture_compilations'])==70
 row=run('native-event-matrix',native(['/tmp/event-build/native_ros2',gateway_inside]),probe,180)
 check=row.get('actual_mains',{});assert str(gateway) in check,'missing actual child gateway';text=pathlib.Path(row['log']).read_text();assert 'PASS expanded ROS2 ParameterEvents native ' in text and 'automatic=6 overflow_contracts=18' in text
 if a.uwvm:wasm_matrix(a.uwvm.resolve())
 deps={};union={}
 for path in T.rglob('*.o.d'):
  raw=path.read_text().replace('\\\n',' ');values={}
  for name in re.findall(r'/work/[^\s\\]+',raw):
   q=(S/name.removeprefix('/work/')).resolve();assert q.is_relative_to(S) and q.is_file();relative=str(q.relative_to(S));values[relative]=H(q);union[relative]=values[relative]
  deps[str(path.relative_to(W))]={'sha256':H(path),'bytes':path.stat().st_size,'project_inputs':values}
 assert len(deps)==(2 if a.gateway else 3) and all(values['project_inputs'] for values in deps.values());doc.update(status='PASS',raw_compiler_dependencies=deps,compiled_project_inputs=union,compiled_project_input_count=len(union),native_matrix_marker=next(v for v in text.splitlines() if v.startswith('PASS expanded ROS2 ParameterEvents native ')))
except Exception as error:doc.update(reason=str(error),traceback=traceback.format_exc())
finally:
 for process,row,_ in reversed(children):
  if process.poll() is None:
   os.killpg(process.pid,signal.SIGTERM)
   try:process.wait(timeout=3)
   except subprocess.TimeoutExpired:os.killpg(process.pid,signal.SIGKILL);process.wait(timeout=5)
 if doc.get('short_bind'):
  short=pathlib.Path(doc['short_bind']);result=subprocess.run(['umount',str(short)],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);doc['short_bind_cleanup']={'returncode':result.returncode,'output':result.stdout}
  if result.returncode:doc['status']='FAIL'
  else:short.rmdir()
 if mounted:
  attempts=[]
  for attempt in range(20):
   q=subprocess.run(['umount','-R',str(R)],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True);attempts.append({'returncode':q.returncode,'output':q.stdout})
   if not q.returncode:break
   time.sleep(.05)
  doc['cleanup']=attempts
  if q.returncode:doc['status']='FAIL'
 doc['remaining_mounts']=[v for v in pathlib.Path('/proc/self/mountinfo').read_text().splitlines() if (' '+str(R)+'/') in v or (' '+str(R)+' ') in v]
 doc['remaining_owned_processes']=[v.pid for v,_,_ in children if v.poll() is None];doc['production_source_preserved']=inventory(production_source)==production_before;doc['source_preserved']=inventory(S)==before;doc['source_manifest_preserved']=H(manifest)==doc['source_manifest_sha256'];doc['cgroup_after']=cg(os.getpid());deltas=[]
 def events(v):return dict((k,int(n)) for k,n in (line.split() for line in v.splitlines()))
 for first,last in zip(doc['cgroup_before']['ancestors'],doc['cgroup_after']['ancestors']):
  assert first['path']==last['path'] and first['memory.max']==last['memory.max']
  if first['memory.events'] is None:continue
  old,new=events(first['memory.events']),events(last['memory.events']);deltas.append({'path':first['path'],**{k:new[k]-old[k] for k in ('oom','oom_kill','oom_group_kill')}})
 doc['oom_deltas']=deltas
 if doc['remaining_mounts'] or doc['remaining_owned_processes'] or not doc['production_source_preserved'] or not doc['source_preserved'] or not doc['source_manifest_preserved'] or any(any(v[k] for k in ('oom','oom_kill','oom_group_kill')) for v in deltas):doc['status']='FAIL'
 save(P/'report.json',doc);print(json.dumps({'status':doc['status'],'reason':doc.get('reason'),'report':str(P/'report.json'),'cleanup':doc.get('cleanup'),'remaining_mounts':doc['remaining_mounts'],'remaining_owned_processes':doc['remaining_owned_processes']}),flush=True)
 if doc['status']!='PASS':raise SystemExit(1)
