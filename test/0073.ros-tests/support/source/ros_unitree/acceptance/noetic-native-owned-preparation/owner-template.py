"""Root's finite outside-owner body; bind SETTINGS only from actual observations.

This file is a non-invoked template. No source hash, control pin or future result
is forecast. Only Root may instantiate/stage it after a complete new freeze.
"""
from pathlib import Path
import hashlib,json,os,resource,signal,stat,sys,time,traceback,types
MIB=1024**2;GIB=1024**3
SETTINGS=ACTUAL_ROOT_OWNER_SETTINGS_LITERAL
START=time.monotonic();FINALIZING=False
resource.setrlimit(resource.RLIMIT_AS,(1536*MIB,1536*MIB));resource.setrlimit(resource.RLIMIT_CORE,(0,0))
def need(ok,why):
 if not ok:raise ValueError(why)
def read(path,maximum=64*MIB):
 p=Path(path);need(p.is_absolute() and p.resolve(strict=True)==p,'physical absolute Root input required')
 fd=os.open(p,os.O_RDONLY|os.O_NOFOLLOW|os.O_NONBLOCK|os.O_CLOEXEC)
 try:
  a=os.fstat(fd);need(stat.S_ISREG(a.st_mode) and 0<=a.st_size<=maximum,'bounded ordinary Root input required');raw=bytearray();h=hashlib.sha256()
  while True:
   data=os.read(fd,min(MIB,maximum+1-len(raw)))
   if not data:break
   raw.extend(data);h.update(data);need(len(raw)<=maximum and time.monotonic()-START<22800,'bounded Root stream required')
  z,n=os.fstat(fd),p.lstat();need(all(getattr(a,k)==getattr(z,k)==getattr(n,k) for k in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),'Root input changed')
  return bytes(raw),dict(path=str(p),bytes=len(raw),sha256=h.hexdigest(),device=a.st_dev,inode=a.st_ino,mtime_ns=a.st_mtime_ns,ctime_ns=a.st_ctime_ns)
 finally:os.close(fd)
def exact_pin(pin,maximum=64*MIB):
 raw,actual=read(pin['path'],maximum);need(actual==pin,'Root selected full-seven input changed');return raw
exact_pin(SETTINGS['operator_template'],MIB)
need(str(Path(__file__).absolute())==SETTINGS['operator_template']['path'],'actual staged outside-owner template binding required')
CG=SETTINGS['cgroup'];leaf=Path(CG['expected_leaf']);state=leaf.stat()
need(CG['memory_max']==64*GIB and CG['memory_swap_max']==0 and CG['expected_cpus']=='0-31' and str(leaf)=='/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope','original task hard64GiB/swap0/CPUs leaf required')
need((state.st_dev,state.st_ino)==(CG['expected_device'],CG['expected_inode']) and Path('/proc/sys/kernel/random/boot_id').read_text().strip()==CG['expected_boot_id'] and Path('/proc/self/cgroup').read_text().splitlines()==['0::'+str(leaf).removeprefix('/sys/fs/cgroup')] and (leaf/'memory.max').read_text().strip()==str(64*GIB) and (leaf/'memory.swap.max').read_text().strip()=='0' and (leaf/'cpuset.cpus.effective').read_text().strip()==CG['expected_cpus'],'actual original cgroup incarnation differs')
S=Path(SETTINGS['source']);MF=SETTINGS['manifest'];raw=exact_pin(MF,16*MIB);frozen=json.loads(raw)
need(Path(MF['path']).parent==S.parent and S.resolve(strict=True)==S,'actual complete current source/MF association required')
def module(relative,name):
 raw,pin=read(S/relative,MIB);need(frozen.get(relative)=={k:pin[k] for k in ('bytes','sha256')},'Root helper differs from complete current source')
 result=types.ModuleType(name);result.__file__=str(S/relative);sys.modules[name]=result;exec(compile(raw,result.__file__,'exec'),result.__dict__);return result
probe=module('tests/ros_cgroup/probe.py','native3_root_current_probe');common=module('tools/ros_distro/common.py','native3_root_owned_primitives');common._CGROUP_PROBE=probe
support=module('tests/ros_unitree/noetic_native_support.py','native3_root_at_rest_readers')
before=common.cgroup_state(CG);need(all(row['events'].get(k,0)==0 for row in before['ancestors'] for k in ('oom','oom_kill','oom_group_kill')),'initial ancestor OOM counter nonzero')
cancelled=common.install_interrupts();OUT=Path(SETTINGS['owner_out']);TARGET=Path(SETTINGS['child_out']);need(OUT.parent.resolve(strict=True)==OUT.parent and TARGET.parent.resolve(strict=True)==TARGET.parent and not OUT.exists() and not TARGET.exists(),'fresh canonical disjoint Root/child outputs required');need(OUT!=TARGET and OUT not in TARGET.parents and TARGET not in OUT.parents and S not in OUT.parents and S not in TARGET.parents,'Root/child/source outputs overlap');OUT.mkdir(mode=0o700)
report={'kind':'root-owned-fresh-noetic-native3-preparation','status':'RUNNING','source':str(S),'source_manifest_sha256':MF['sha256'],'settings':SETTINGS,'cgroup_before':before,'commands':[],'source_preserved':False,'inputs_preserved':False,'products_preserved':False,'native_ROS_contexts_executed':0,'Runtime_executed':False,'Guest_rights_changed':False,'scope':'FRESH_NATIVE3_COMPILE_AND_STOPPED_METADATA_EXPORT_ONLY'}
ROOT_PINS=[MF,SETTINGS['configuration'],SETTINGS['python'],SETTINGS['unshare'],SETTINGS['operator_template'],*SETTINGS['pins']];runner=None;source_before=None;child=None;last_walk=0.0;retired=OUT/'retired-owned-births.jsonl';retirement=retired.open('xb')
def save():
 report['commands']=runner.commands if runner else [];common.atomic_json(OUT/'report.json',report)
def guard():
 global last_walk
 need(FINALIZING or not cancelled['cancelled'],'cooperative Root cancellation requested');need(time.monotonic()-START<(22800 if FINALIZING else 22200),'finite Root active/finalization deadline exhausted');need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024<=768*MIB,'Root coordinator RSS exceeds768MiB');common.cgroup_preserved(before,common.cgroup_state(CG))
 if runner is not None:
  runner.discover();need(runner.rss()<=8*GIB,'entire owned preparation tree exceeds8GiB RSS')
  for key,item in list(runner.owned.items()):
   if common.pidfd_terminal(item['fd']):
    retirement.write((json.dumps({'birth':item['identity'],'after_pidfd':item['identity_after'],'pidfd_terminal':True},sort_keys=True)+'\n').encode());retirement.flush();os.close(item['fd']);del runner.owned[key]
 if not FINALIZING:
  for path in (OUT,*OUT.parents):
   v=os.statvfs(path);need(v.f_bavail*v.f_frsize>=8*GIB,'unchanged8GiB all-backing reserve exhausted')
  if time.monotonic()>=last_walk:
   size=0
   for base in (OUT,TARGET):
    if not base.exists():continue
    for folder,dirs,names in os.walk(base,followlinks=False):
     for name in names:
      p=Path(folder)/name
      try:a=p.lstat()
      except FileNotFoundError:continue
      if stat.S_ISREG(a.st_mode):size+=a.st_size
      elif stat.S_ISLNK(a.st_mode):size+=a.st_size
      else:need(p==TARGET/'sdk-rootfs/dev/null' and stat.S_ISCHR(a.st_mode) and a.st_rdev==os.makedev(1,3),'owned special node outside exact active null overlay')
     # A copied SDK can currently contain only the explicitly recorded private
     # source/work/null overlays. Do not traverse its active mount views here;
     # child records and retires those before complete at-rest inventories.
     if base==TARGET and Path(folder)==TARGET/'sdk-rootfs':
      dirs[:]=[name for name in dirs if name not in ('work','owned')]
   need(size<=5*GIB,'Root plus native preparation output exceeds5GiB');last_walk=time.monotonic()+.5
try:
 guard();need(int((leaf/'memory.current').read_text())+10*GIB<60*GIB,'Root reserves8GiB tree plus2GiB below60GiB')
 for pin in ROOT_PINS:exact_pin(pin)
 source_before=common.source_state(S,Path(MF['path']));need(source_before['manifest']==MF,'actual complete source full-seven differs');report['source_before']=source_before
 cfg=json.loads(exact_pin(SETTINGS['configuration'],16*MIB));need(cfg['cgroup']==CG and cfg['limits']['jobs']==1 and cfg['limits']['rss_bytes']<=6*GIB and cfg['limits']['output_bytes']<=4*GIB,'exact native Frame configuration/current serial budget required')
 entry=S/'tests/ros_unitree/prepare_noetic_native.py';raw,entry_pin=read(entry,MIB);need(frozen.get(str(entry.relative_to(S)))=={k:entry_pin[k] for k in ('bytes','sha256')},'actual native entry differs')
 command=[SETTINGS['unshare']['path'],'--mount','--net','--propagation','private',SETTINGS['python']['path'],'-B',str(entry),'--source',str(S),'--manifest',MF['path'],'--manifest-sha256',MF['sha256'],'--config',SETTINGS['configuration']['path'],'--config-sha256',SETTINGS['configuration']['sha256'],'--selection-input',SETTINGS['selection_input'],'--out',str(TARGET)]
 runner=common.SerialRunner(OUT,{'jobs':1,'rss_bytes':8*GIB,'command_seconds':22200,'log_bytes':16*MIB},guard);save()
 row=runner.run('fresh-native3',command,OUT,{'PATH':'/usr/bin:/bin','LANG':'C.UTF-8','LC_ALL':'C.UTF-8','PYTHONDONTWRITEBYTECODE':'1','HOME':str(OUT),'TMPDIR':str(OUT)})
 report['child_command_completed']=True
except BaseException as error:report.update(status='FAIL',error=repr(error),traceback=traceback.format_exc())
finally:
 FINALIZING=True;errors=[]
 def finish(label,action):
  try:action()
  except BaseException as error:errors.append({'stage':label,'error':repr(error)});report['status']='FAIL'
 def cooperative():
  if runner is None:return
  live=[p for p in runner.children if p.poll() is None]
  need(len(live)<=1,'one direct native coordinator required')
  if not live:return
  current=live[0];first=runner.roots.get(current.pid);now=common.identity(current.pid);need(first is not None and common.birth(first)==common.birth(now) and now['parent_pid']==os.getpid(),'same-birth direct native coordinator required for cleanup grace');runner.acquire(current.pid,os.getpid());key=common.birth(first);need(key in runner.owned and not common.pidfd_terminal(runner.owned[key]['fd']),'held direct native pidfd required');signal.pidfd_send_signal(runner.owned[key]['fd'],signal.SIGTERM);until=time.monotonic()+600
  while current.poll() is None and time.monotonic()<until:guard();time.sleep(.05)
 finish('cooperative-direct-coordinator-600-second-grace',cooperative)
 def cleanup():
  report['cleanup']=runner.close() if runner else {'status':'PASS','remaining':[]};need(report['cleanup']['status']=='PASS' and not report['cleanup']['remaining'],'all Root-owned births must retire')
 finish('final-held-pidfd-cleanup',cleanup);retirement.flush();os.fsync(retirement.fileno());retirement.close();report['retired_births']=common.opened(retired)
 def preserve():
  for pin in ROOT_PINS:exact_pin(pin)
  report['inputs_preserved']=True;after=common.source_state(S,Path(MF['path']));report['source_after']=after;need(source_before is not None and after==source_before,'actual complete current source changed');report['source_preserved']=True
  raw,pin=read(TARGET/'report.json');native=json.loads(raw);report['native_report']=pin;report['native_actual_status']=native.get('status');need(native['schema']=='uwvm-unitree-noetic-native-products-v1' and native['status']=='PREPARED_NOETIC_LEGACY_NATIVE_PRODUCTS_NOT_TCPROS_EXECUTED' and native['source']==str(S) and native['source_manifest_sha256']==MF['sha256'] and native['configuration']==SETTINGS['configuration'] and native['qualified_primary_translation_units']==3 and native['metadata_export_executed'] is True and native['native_contexts_executed']==0 and all(native.get(k) is True for k in ('source_preserved','input_pins_preserved','whole_roots_preserved','execution_view_preserved','original_sdk_preserved','original_official_upstream_preserved','official_source_copy_unmodified','all_target_products_fresh','acquisition_source_preserved')) and not native['remaining_owned_mounts'] and not native['native_finalization_errors'] and not native.get('finalization_errors'),'actual complete native preparation receipt failed')
  need(native['cleanup'] and all(r['status']=='PASS' and not r.get('remaining') for r in native['cleanup']),'actual native cleanup records incomplete')
  need(len(native['products'])==3 and set(native['products'])=={'identity_exporter','gateway','legacy_peer'} and len(native['family']['all_generated_outputs'])==11 and native['family']['message_count']==11,'complete actual native3/official11 products required')
  for name in ('input_pins','product_pins'):
   for selected in native.get(name,[]):guard();exact_pin(selected,512*MIB)
  for role in native['products'].values():exact_pin(role['binary'],512*MIB)
  exact_pin(native['execution_file_catalog'],64*MIB);exact_pin(native['execution_elf_catalog'],64*MIB)
  after={}
  for name,path in (('sdk',TARGET/'sdk-rootfs'),('work',TARGET/'native-work')):
   current=support.tree(common,path,OUT/('independent-'+name+'-after.jsonl'),guard);need(current['physical_sha256']==native['execution_view_after'][name]['physical_sha256'],'Root complete at-rest native '+name+' closure changed');after[name]=current
  report['independent_at_rest_after']=after;exact_pin(pin);report['products_preserved']=True
 finish('complete-native-receipt-and-current-preservation',preserve)
 def terminal_cgroup():
  current=common.cgroup_state(CG);common.cgroup_preserved(before,current);report['cgroup_after']=current
 finish('current-cgroup',terminal_cgroup)
 def terminal_output():
  total=0
  for base in (OUT,TARGET):
   if not base.exists():continue
   for folder,dirs,names in os.walk(base,followlinks=False):
    for name in dirs+names:
     guard();a=(Path(folder)/name).lstat();need(stat.S_ISREG(a.st_mode) or stat.S_ISDIR(a.st_mode) or stat.S_ISLNK(a.st_mode),'terminal owned special node refused')
     if not stat.S_ISDIR(a.st_mode):total+=a.st_size
     need(total<=5*GIB,'fresh terminal Root plus native output exceeds5GiB')
  report['terminal_owned_output_bytes']=total
 finish('fresh-terminal-owned-output-budget',terminal_output)
 def terminal_storage():
  for path in (OUT,*OUT.parents):
   v=os.statvfs(path);need(v.f_bavail*v.f_frsize>=8*GIB,'terminal8GiB backing reserve exhausted')
 finish('fresh-terminal-backing-reserve',terminal_storage)
 report['finalization_errors']=errors
 if report.get('child_command_completed') and report['status']=='RUNNING' and not errors:report['status']='PASS_FRESH_NATIVE3_AND_STOPPED_OFFICIAL11_METADATA_ONLY'
 save();print(json.dumps({'status':report['status'],'report':common.opened(OUT/'report.json')}),flush=True)
raise SystemExit(0 if report['status']=='PASS_FRESH_NATIVE3_AND_STOPPED_OFFICIAL11_METADATA_ONLY' else 1)
