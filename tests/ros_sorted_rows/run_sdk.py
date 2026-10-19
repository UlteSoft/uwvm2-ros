#!/usr/bin/env python3
"""Actual Noetic/Jazzy CPP17 sorted_rows unit compile/link/run; no ROS network or Gateway runtime."""
import argparse,hashlib,json,os,re,shlex,shutil,signal,subprocess,sys,time,traceback
from pathlib import Path
sys.dont_write_bytecode=True
p=argparse.ArgumentParser(description=__doc__)
for name in ('workspace','baseline','sdk','inputs'):p.add_argument('--'+name,type=Path,required=True)
p.add_argument('--backend',choices=('noetic','jazzy'),required=True)
a=p.parse_args();BACKEND=a.backend;W,BASE,SDK,INPUTS=[getattr(a,name).resolve() for name in ('workspace','baseline','sdk','inputs')];assert not W.exists() and not W.is_relative_to(BASE) and not W.is_relative_to(SDK)
W.mkdir();P=W/'reports';P.mkdir();T=W/'native-temp';T.mkdir();R=W/'rootfs';R.mkdir();children=[];mounted=False
sys.path.insert(0,str(BASE/'source/tests/ros_cgroup'));from probe import probe

def sha(path):
    h = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(1048576), b''): h.update(block)
    return h.hexdigest()

def inventory(root):
    return {str(path.relative_to(root)): sha(path) for path in root.rglob('*') if path.is_file()}

def save(path, value): path.write_text(json.dumps(value, indent=2)+'\n')

def live_cgroup(pid):
    facts = probe(); membership = Path('/proc', str(pid), 'cgroup').read_text().splitlines()
    assert facts['status'] == 'PASS' and membership == facts['membership'] == before['membership']
    live = str(pid) in (Path(facts['cgroup_ancestors'][0]['path'])/'cgroup.procs').read_text().split()
    assert live
    return {**facts, 'pid': pid, 'membership': membership, 'live_cgroup_procs': live,
            'ancestor_sampling': 'Driver and actual ELF PID proved live in exactly the same cgroup'}

def run(name, argv, expected=0, artifact=None, timeout=120):
    log = P/(name+'.log'); pre = P/(name+'-preexec.json'); row = {'name': name, 'argv': argv, 'expected_returncode': expected, 'status': 'FAIL'}; doc['commands'].append(row)
    def preexec(): save(pre, probe())
    with log.open('xb') as stream: child = subprocess.Popen(argv, stdout=stream, stderr=subprocess.STDOUT, preexec_fn=preexec, start_new_session=True)
    children.append(child); row['wrapper_pid'] = child.pid; until = time.monotonic()+timeout
    while child.poll() is None:
        if artifact and 'actual_main' not in row:
            pending = [(child.pid, [child.pid])]; seen = set()
            while pending:
                pid, ancestry = pending.pop()
                if pid in seen: continue
                seen.add(pid)
                try:
                    pending += [(int(value), ancestry+[int(value)]) for value in Path('/proc', str(pid), 'task', str(pid), 'children').read_text().split()]
                    fd = os.open(Path('/proc', str(pid), 'exe'), os.O_RDONLY)
                    with os.fdopen(fd, 'rb') as file:
                        actual = os.fstat(file.fileno()); wanted = artifact.stat()
                        if (actual.st_dev, actual.st_ino) != (wanted.st_dev, wanted.st_ino): continue
                        live = live_cgroup(pid); parent = int(next(line.split()[1] for line in Path('/proc', str(pid), 'status').read_text().splitlines() if line.startswith('PPid:')))
                        exe = os.readlink(Path('/proc', str(pid), 'exe')); captured = time.monotonic_ns()
                        assert live['status'] == 'PASS' and live['membership'] == before['membership'] and live['live_cgroup_procs']
                        assert parent == (os.getpid() if len(ancestry) == 1 else ancestry[-2])
                        digest = hashlib.sha256()
                        for block in iter(lambda: file.read(1048576), b''): digest.update(block)
                        assert digest.hexdigest() == sha(artifact)
                        row['actual_main'] = {**live, 'ancestry': ancestry, 'parent_pid': parent, 'exe': exe, 'device': actual.st_dev, 'inode': actual.st_ino,
                                              'sha256': digest.hexdigest(), 'live_identity_captured_before_fd_hash': True, 'capture_monotonic_ns': captured}
                        if name=='unit-run':os.kill(pid,signal.SIGCONT)
                except (FileNotFoundError, ProcessLookupError): pass
                except PermissionError as error: row.setdefault('sampling_unavailable', []).append({'pid': pid, 'reason': str(error)})
        if time.monotonic() > until: raise TimeoutError(name)
        time.sleep(.005)
    row.update(returncode=child.returncode, log=str(log), log_sha256=sha(log), preexec_sha256=sha(pre))
    if artifact: assert 'actual_main' in row, 'missing actual main ELF proof '+name
    text = log.read_text()
    if expected:
        assert row['returncode'] == expected and 'expired service respond budget accepted native response' in text and 'OBSERVED service.respond ' in text, (name, text)
        row['semantic_expected_failure'] = True
    else: assert row['returncode'] == 0, (name, text)
    row['status'] = 'EXPECTED_SEMANTIC_FAIL' if expected else 'PASS'
    save(P/'progress.json', doc)

before=probe();assert before['status']=='PASS' and before['effective_memory_max']==68719476736
assert int(before['cgroup_ancestors'][0]['memory.current'])+9*1024**3<=68719476736
save(P/'build-budget.json',{'compiler_gib':1,'reserve_gib':8,'jobs':1,'cgroup_before':before})
source_before=inventory(BASE/'source');binaries_before=inventory(BASE/'native-binaries');report_before=sha(BASE/'reports/report.json')
doc={'status':'FAIL','scope':'Actual native SDK CPP17 sorted_rows unit compile/link/run; no ROS network or Gateway runtime','commands':[],'workspace':str(W),'argv':sys.argv,'executed_driver_sha256':sha(__file__),'cgroup_before':before}
shutil.copy2(__file__,W/'executed-driver.py')
def native(argv,stop=False):return ['/usr/bin/chroot',str(R),'/usr/bin/env','-u','CC','-u','CXX','-u','LD_LIBRARY_PATH','PYTHONDONTWRITEBYTECODE=1',*(['ROS_SORTED_ROWS_STOP=1'] if stop else []),'/bin/bash','-lc','source /opt/ros/'+BACKEND+'/setup.bash && exec "$@"','sorted-rows-sdk-check',*argv]
try:
 shutil.copytree(BASE/'source',W/'source',symlinks=True);doc['overlay_inputs']={}
 for name in ('gateways/common/bounded_text.h','tests/ros_sorted_rows/unit.cc','tests/ros_sorted_rows/before-bounded-text.inc'):
  target=W/'source'/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(INPUTS/name,target);doc['overlay_inputs'][name]=sha(target)
 frozen=inventory(W/'source');save(P/'source-before.json',frozen)
 subprocess.run(['mount','--rbind',str(SDK/('gateway/'+BACKEND+'-rootfs')),str(R)],check=True);mounted=True
 subprocess.run(['mount','--make-rprivate',str(R)],check=True);subprocess.run(['mount','--bind',str(W/'source'),str(R/'work')],check=True);subprocess.run(['mount','-o','remount,bind,ro',str(R/'work')],check=True);subprocess.run(['mount','--bind',str(T),str(R/'tmp')],check=True)
 run('compiler-version',native(['/usr/bin/c++','--version']))
 compiler_artifact=SDK/('gateway/'+BACKEND+'-rootfs/usr/bin/x86_64-linux-gnu-g++-'+('9' if BACKEND=='noetic' else '13'));assert compiler_artifact.is_file()
 run('unit-compile',native(['/usr/bin/c++','-std=gnu++17','-O1','-g0','-MMD','-MF','/tmp/unit.d','-I','/work','-I','/work/src','-c','/work/tests/ros_sorted_rows/unit.cc','-o','/tmp/unit.o']),artifact=compiler_artifact,timeout=180)
 run('unit-link',native(['/usr/bin/c++','/tmp/unit.o','-o','/tmp/unit']),artifact=compiler_artifact)
 run('unit-run',native(['/tmp/unit'],stop=True),artifact=T/'unit')
 assert 'PASS sorted_rows exact-wire differential cases=2182 checks=6571' in (P/'unit-run.log').read_text()
 doc['fresh_ELF_sha256']={name:sha(T/name) for name in ('unit',)};doc['raw_MMD']={}
 for name in ('unit.d',):
  dep=T/name;values={}
  for token in re.findall(r'/work/[^\s\\]+',dep.read_text().replace('\\\n',' ')):
   rel=os.path.normpath(token)[6:];assert rel in frozen;values[rel]=sha(W/'source'/rel);assert values[rel]==frozen[rel]
  assert values;doc['raw_MMD'][name]={'path':str(dep),'bytes':dep.stat().st_size,'sha256':sha(dep),'project_inputs':values}
 assert set(doc['raw_MMD']['unit.d']['project_inputs']).issuperset({'gateways/common/bounded_text.h','tests/ros_sorted_rows/unit.cc','tests/ros_sorted_rows/before-bounded-text.inc'})
 for n,v in doc['overlay_inputs'].items():assert doc['raw_MMD']['unit.d']['project_inputs'][n]==v
 assert len(doc['commands'])==4
 doc['current_scoped_inputs']={**doc['overlay_inputs'],'tests/ros_sorted_rows/run_sdk.py':sha(__file__)}
 doc['compiler_actual_main_samples']=2;doc['unit_actual_main_samples']=1;doc['backend']=BACKEND;doc['Gateway_runtime_executed']=False
 doc['status']='PASS'
except BaseException:doc['error']=traceback.format_exc()
finally:
 for child in children:
  if child.poll() is None:os.killpg(child.pid,signal.SIGKILL);child.wait(timeout=5)
 if mounted:
  attempts=[]
  for _ in range(20):
   result=subprocess.run(['umount','-R',str(R)],capture_output=True,text=True);attempts.append({'returncode':result.returncode,'output':result.stdout+result.stderr})
   if result.returncode==0 or 'target is busy' not in result.stderr:break
   time.sleep(.05)
  doc['mount_cleanup']=attempts
 doc['remaining_owned_mounts']=[line for line in Path('/proc/self/mountinfo').read_text().splitlines() if line.split()[4]==str(W) or line.split()[4].startswith(str(W)+'/')];owned=[];unavailable=[]
 for path in Path('/proc').iterdir():
  if not path.name.isdigit():continue
  try:
   exe=os.readlink(path/'exe')
   if exe.startswith(str(W)+'/'):owned.append({'pid':int(path.name),'exe':exe})
  except OSError as error:
   if error.errno not in (2,3):unavailable.append({'pid':int(path.name),'errno':error.errno})
 doc.update(remaining_owned_processes=owned,unavailable_process_samples=unavailable,cgroup_after=probe());after=doc['cgroup_after'];assert after['status']=='PASS' and [row['path'] for row in before['cgroup_ancestors']]==[row['path'] for row in after['cgroup_ancestors']];deltas=[]
 for old,new in zip(before['cgroup_ancestors'],after['cgroup_ancestors']):
  assert old['memory.max']==new['memory.max'] and (old['memory.events'] is None)==(new['memory.events'] is None)
  if old['memory.events'] is None:continue
  events=lambda text:{key:int(value) for key,value in (line.split() for line in text.splitlines())};first,last=events(old['memory.events']),events(new['memory.events']);deltas.append({'path':old['path'],**{key:last[key]-first[key] for key in ('oom','oom_kill','oom_group_kill')}})
 doc['oom_deltas']=deltas;doc['baseline_preserved']={'source':inventory(BASE/'source')==source_before,'18_native_binaries':inventory(BASE/'native-binaries')==binaries_before,'report':sha(BASE/'reports/report.json')==report_before};doc['source_preserved']=inventory(W/'source')==frozen if 'frozen' in globals() else False
 if owned or doc['remaining_owned_mounts'] or not doc['source_preserved'] or not all(doc['baseline_preserved'].values()) or any(any(row[key] for key in ('oom','oom_kill','oom_group_kill')) for row in deltas) or mounted and doc['mount_cleanup'][-1]['returncode']:doc['status']='FAIL'
 save(P/'report.json',doc);print(json.dumps({'status':doc['status'],'report':str(P/'report.json'),'sha256':sha(P/'report.json'),'error':doc.get('error'),'commands':[(row['name'],row['status'],row.get('returncode')) for row in doc['commands']],'cleanup':doc.get('mount_cleanup'),'owned':owned,'mounts':doc['remaining_owned_mounts'],'oom':deltas},indent=2));raise SystemExit(0 if doc['status']=='PASS' else 1)
