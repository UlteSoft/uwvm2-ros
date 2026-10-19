#!/usr/bin/env python3
"""Compare sorted_rows against its exact before implementation and measure scoped ordinary-new calls."""
import hashlib,json,os,re,signal,subprocess,sys,time,traceback
from pathlib import Path
import argparse,shutil
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--source',type=Path,required=True);p.add_argument('--bounded-header',type=Path);p.add_argument('--out',type=Path,required=True);p.add_argument('--compiler',required=True);p.add_argument('--llvm-libs',required=True);a=p.parse_args()
BASE=a.source.resolve();W=a.out.resolve();assert not W.exists();W.mkdir();S=W/'source';S.mkdir();P=W/'results';P.mkdir()
sys.path.insert(0,str(BASE/'tests/ros_cgroup'));from probe import probe
C=a.compiler;LIB=a.llvm_libs
H=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
for name in ('gateways/common/bounded_text.h','src/uwvm2/imported/ros/ipc/parameter.h','src/uwvm2/imported/ros/ipc/protocol.h','src/uwvm2/imported/ros/common/types.h'):
 src=(a.bounded_header if name=='gateways/common/bounded_text.h' and a.bounded_header else BASE/name);dst=S/name;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(src,dst)
for name in ('unit.cc','before-bounded-text.inc','run.py'):
 src=Path(__file__).resolve().parent/name;dst=S/'tests/ros_sorted_rows'/name;dst.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(src,dst)
M={str(p.relative_to(S)):{'sha256':H(p),'bytes':p.stat().st_size} for p in S.rglob('*') if p.is_file()};(P/'source-manifest.json').write_text(json.dumps(M,indent=2)+'\n')
before=probe();doc={'status':'FAIL','scope':'Actual CPP17 sorted_rows scoped replacement ordinary-new calls only; no ROS/network/throughput claim','cgroup_before':before,'commands':[],'profiles':[],'executed_driver_sha256':H(__file__),'source':str(S),'actual_argv':sys.argv};children=[]
assert before['status']=='PASS' and before['effective_memory_max']==68719476736
assert int(before['cgroup_ancestors'][0]['memory.current'])+9*1024**3<=68719476736
(P/'build-budget.json').write_text(json.dumps({'jobs':1,'compiler_gib':1,'reserve_gib':8,'cgroup_before':before},indent=2)+'\n')
def run(name,argv,elf=None):
 log=P/(name+'.log');row={'name':name,'argv':argv,'status':'FAIL'};doc['commands'].append(row);env=os.environ.copy();env.update(LD_LIBRARY_PATH=LIB,ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
 if elf:env['ROS_SORTED_ROWS_STOP']='1'
 with log.open('xb') as f:
  p=subprocess.Popen(argv,env=env,stdout=f,stderr=subprocess.STDOUT,start_new_session=True);children.append(p);row['pid']=p.pid
  if elf:
   until=time.monotonic()+10
   while True:
    got,state=os.waitpid(p.pid,os.WNOHANG|os.WUNTRACED)
    if got:assert os.WIFSTOPPED(state) and os.WSTOPSIG(state)==signal.SIGSTOP;break
    assert time.monotonic()<until,'ELF did not stop';time.sleep(.002)
   proc=Path('/proc',str(p.pid));fd=os.open(proc/'exe',os.O_RDONLY)
   with os.fdopen(fd,'rb') as actual:
    identity=os.fstat(actual.fileno());expected=elf.stat();membership=(proc/'cgroup').read_text().splitlines();parent=int((proc/'stat').read_text().rsplit(')',1)[1].split()[1]);state=next(x for x in (proc/'status').read_text().splitlines() if x.startswith('State:'));live=str(p.pid) in (Path(before['cgroup_ancestors'][0]['path'])/'cgroup.procs').read_text().split();exe=os.readlink(proc/'exe');captured=time.monotonic_ns();facts=probe()
    assert (identity.st_dev,identity.st_ino)==(expected.st_dev,expected.st_ino) and membership==before['membership'] and parent==os.getpid() and live and 'T (stopped)' in state
    h=hashlib.sha256()
    for block in iter(lambda:actual.read(1048576),b''):h.update(block)
    assert h.hexdigest()==H(elf)
    row['actual_main']={'pid':p.pid,'parent_pid':parent,'device':identity.st_dev,'inode':identity.st_ino,'sha256':h.hexdigest(),'exe':exe,'membership':membership,'live_cgroup_procs':live,'live_identity_captured_before_fd_hash':True,'capture_monotonic_ns':captured,'state':state,'cgroup':facts}
   os.kill(p.pid,signal.SIGCONT)
  row['returncode']=p.wait(timeout=120)
 row.update(log=str(log),log_sha256=H(log));assert row['returncode']==0,log.read_text();row['status']='PASS'
try:
 run('compiler-version',[C,'--version'])
 for name,lib,san in [('cpp17-std-san','libstdc++',True),('cpp17-std-normal','libstdc++',False),('cpp17-libcxx-san','libc++',True)]:
  elf=P/name;dep=P/(name+'.d');argv=[C,'-std=c++17','-stdlib='+lib,'-O1','-g0','-MMD','-MF',str(dep),'-I',str(S),'-I',str(S/'src'),str(S/'tests/ros_sorted_rows/unit.cc'),'-o',str(elf)]
  if san:argv[1:1]=['-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie']
  run(name+'-build',argv);run(name+'-run',[str(elf)],elf)
  inputs={}
  for token in re.findall(re.escape(str(S))+r'/[^\s\\]+',dep.read_text().replace('\\\n',' ')):
   path=Path(os.path.normpath(token));rel=str(path.relative_to(S));assert rel in M and H(path)==M[rel]['sha256'];inputs[rel]=M[rel]
  text=(P/(name+'-run.log')).read_text();rows=[]
  for line in text.splitlines():
   if line.startswith('OBSERVED '):rows.append(dict(part.split('=',1) for part in line.split()[1:]))
  assert len(rows)==6 and 'PASS sorted_rows exact-wire differential' in text
  doc['profiles'].append({'name':name,'standard':'c++17','library':lib,'sanitized':san,'binary_sha256':H(elf),'raw_MMD_sha256':H(dep),'compiled_project_inputs':inputs,'observations':rows})
 doc['status']='PASS'
except BaseException:doc['error']=traceback.format_exc()
finally:
 for p in children:
  if p.poll() is None:os.killpg(p.pid,signal.SIGKILL);p.wait(timeout=5)
 after=probe();doc['cgroup_after']=after;deltas=[]
 assert after['status']=='PASS' and after['membership']==before['membership']
 for a,b in zip(before['cgroup_ancestors'],after['cgroup_ancestors']):
  assert a['path']==b['path'] and a['memory.max']==b['memory.max'] and (a['memory.events'] is None)==(b['memory.events'] is None)
  if a['memory.events'] is None:continue
  x,y=[dict(line.split() for line in r['memory.events'].splitlines()) for r in (a,b)];deltas.append({'path':a['path'],**{k:int(y[k])-int(x[k]) for k in ('oom','oom_kill','oom_group_kill')}})
 doc['oom_deltas']=deltas;doc['source_preserved']=all(H(S/n)==v['sha256'] for n,v in M.items());doc['remaining_owned_children']=[p.pid for p in children if p.poll() is None]
 if not doc['source_preserved'] or doc['remaining_owned_children'] or any(any(r[k] for k in ('oom','oom_kill','oom_group_kill')) for r in deltas):doc['status']='FAIL'
 (P/'report.json').write_text(json.dumps(doc,indent=2)+'\n');print(json.dumps({'status':doc['status'],'report':str(P/'report.json'),'sha256':H(P/'report.json'),'error':doc.get('error'),'profiles':[{k:r[k] for k in ('name','observations')} for r in doc['profiles']]},indent=2));sys.exit(0 if doc['status']=='PASS' else 1)
