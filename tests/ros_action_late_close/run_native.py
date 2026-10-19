#!/usr/bin/env python3
"""Bounded ROS2 late acceptance/close probes with an immutable prebuilt Gateway."""
import argparse,hashlib,json,os,re,shlex,shutil,signal,subprocess,sys,time,traceback
from pathlib import Path
sys.dont_write_bytecode=True
p=argparse.ArgumentParser(description=__doc__)
for name in ('workspace','source','gateway','gateway-source','gateway-mmd','flags','link','sdk','inputs','origin-report'):
    p.add_argument('--'+name,type=Path,required=True)
p.add_argument('--phase',choices=('before','after'),required=True)
p.add_argument('--modes',default='accept,reject,accepted,disconnect,refuse,expired,expired-owner')
a=p.parse_args()
W,S,G,GS,GM,F,L,SDK,I,OR=[getattr(a,x).resolve() for x in ('workspace','source','gateway','gateway_source','gateway_mmd','flags','link','sdk','inputs','origin_report')]
assert not W.exists() and G.is_file() and GM.is_file()
W.mkdir();P=W/'reports';P.mkdir();T=W/'native-temp';T.mkdir();R=W/'rootfs';R.mkdir()
def sha(p):
    h=hashlib.sha256()
    with Path(p).open('rb') as f:
        for b in iter(lambda:f.read(1048576),b''):h.update(b)
    return h.hexdigest()
def inventory(root):return {str(f.relative_to(root)):sha(f) for f in root.rglob('*') if f.is_file()}
def save(p,d):p.write_text(json.dumps(d,indent=2)+'\n')
def inputs(dep,root):
    values={}
    for token in re.findall(r'/work/[^\s\\]+',dep.read_text().replace('\\\n',' ')):
        name=os.path.normpath(token)[6:];assert not name.startswith('../');values[name]=sha(root/name)
    assert values;return values
sys.path.insert(0,str(S/'tests/ros_cgroup'))
from probe import probe
before=probe();assert before['status']=='PASS' and before['effective_memory_max']==68719476736
source_before=inventory(S);gsha=sha(G);msha=sha(GM);originsha=sha(OR)
old_inputs=inputs(GM,GS)
assert all(sha(S/n)==v for n,v in old_inputs.items()),'prebuilt Gateway differs from selected source'
save(P/'source-origin-before.json',source_before)
shutil.copy2(__file__,W/'executed-driver.py')
doc={'status':'FAIL','scope':'ROS2 timeout110 plus alias close or disconnect/rebind during native acceptance gate; accepted outgoing expiry110 retires Guest ID but retains live owner quota through native terminal; expired-owner explicit max_sessions=2, owner limit1 and global capacity2; other six modes capacity1; accepted handle control; prebuilt CPP17 Gateway; CPP20 testcase probe; no broad deadline race or Worker/A15 claim','phase':a.phase,'argv':sys.argv,'workspace':str(W),'executed_driver_sha256':sha(__file__),'commands':[],'cgroup_before':before,'gateway_origin':{'binary':str(G),'sha256':gsha,'source':str(GS),'MMD':str(GM),'MMD_sha256':msha,'project_inputs':old_inputs,'report':str(OR),'report_sha256':originsha},'source_origin':str(S)}
mounted=False;children=[]
def native(argv):
    return ['/usr/bin/chroot',str(R),'/usr/bin/env','-u','CC','-u','CXX','-u','LD_LIBRARY_PATH','PYTHONDONTWRITEBYTECODE=1','ROS_DOMAIN_ID=199','RMW_IMPLEMENTATION=rmw_fastrtps_cpp','ROS_SECURITY_ENABLE=false','ROS_LOG_DIR=/tmp/ros-logs','/bin/bash','-lc','source /opt/ros/jazzy/setup.bash && exec "$@"','action-late-close',*argv]
def live(pid):
    d=probe();membership=Path('/proc',str(pid),'cgroup').read_text().splitlines()
    assert d['status']=='PASS' and membership==d['membership']==before['membership']
    assert str(pid) in (Path(d['cgroup_ancestors'][0]['path'])/'cgroup.procs').read_text().split()
    return {**d,'pid':pid,'membership':membership,'live_cgroup_procs':True}
def run(name,argv,expected=0,artifacts=(),timeout=120):
    log=P/(name+'.log');pre=P/(name+'-preexec.json')
    row={'name':name,'argv':argv,'expected_returncode':expected,'status':'FAIL'};doc['commands'].append(row)
    def preexec():save(pre,probe())
    with log.open('xb') as out:child=subprocess.Popen(argv,stdout=out,stderr=subprocess.STDOUT,preexec_fn=preexec,start_new_session=True)
    children.append(child);row['wrapper_pid']=child.pid;end=time.monotonic()+timeout
    while child.poll() is None:
        if len(row.get('actual_mains',{}))<len(artifacts):
            pending=[(child.pid,[child.pid])];seen=set()
            while pending:
                pid,ancestry=pending.pop()
                if pid in seen:continue
                seen.add(pid)
                try:
                    pending.extend((int(x),ancestry+[int(x)]) for x in Path('/proc',str(pid),'task',str(pid),'children').read_text().split())
                    fd=os.open(Path('/proc',str(pid),'exe'),os.O_RDONLY)
                    with os.fdopen(fd,'rb') as file:
                        st=os.fstat(file.fileno())
                        found=next((f for f in artifacts if (f.stat().st_dev,f.stat().st_ino)==(st.st_dev,st.st_ino)),None)
                        if found is None or str(found) in row.get('actual_mains',{}):continue
                        facts=live(pid);parent=int(next(x.split()[1] for x in Path('/proc',str(pid),'status').read_text().splitlines() if x.startswith('PPid:')))
                        exe=os.readlink(Path('/proc',str(pid),'exe'));captured=time.monotonic_ns()
                        assert parent==(os.getpid() if len(ancestry)==1 else ancestry[-2])
                        now=Path('/proc',str(pid),'exe').stat();assert (now.st_dev,now.st_ino)==(st.st_dev,st.st_ino)
                        h=hashlib.sha256()
                        for b in iter(lambda:file.read(1048576),b''):h.update(b)
                        assert h.hexdigest()==sha(found)
                        row.setdefault('actual_mains',{})[str(found)]={**facts,'ancestry':ancestry,'parent_pid':parent,'exe':exe,'device':st.st_dev,'inode':st.st_ino,'sha256':h.hexdigest(),'live_identity_captured_before_fd_hash':True,'capture_monotonic_ns':captured}
                except (FileNotFoundError,ProcessLookupError):pass
                except PermissionError as error:row.setdefault('sampling_unavailable',[]).append({'pid':pid,'reason':str(error)})
        if time.monotonic()>end:raise TimeoutError(name)
        time.sleep(.005)
    row.update(returncode=child.returncode,log=str(log),log_sha256=sha(log),preexec_sha256=sha(pre))
    assert set(row.get('actual_mains',{}))=={str(x) for x in artifacts},'missing actual probe/Gateway witnesses'
    text=log.read_text();assert row['returncode']==expected,(name,text)
    if expected:
        assert 'OBSERVED action-late-close ' in text and 'closed pending Action acceptance was orphaned or released its quota' in text,(name,text)
        row['semantic_expected_failure']=True
    elif artifacts:assert 'PASS action-late-close mode=' in text,(name,text)
    row['status']='EXPECTED_SEMANTIC_FAIL' if expected else 'PASS';save(P/'progress.json',doc)
try:
    shutil.copytree(S,W/'source',symlinks=True)
    cpp='tests/ros_action_late_close/native_ros2.cc';target=W/'source'/cpp;target.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(I/cpp,target)
    frozen=inventory(W/'source');save(P/'source-before.json',frozen)
    doc['overlay_inputs']={cpp:sha(target)}
    for name,src in [('flags.make',F),('link.txt',L),('gateway-origin.d',GM)]:shutil.copy2(src,P/name)
    shutil.copy2(G,T/'gateway');assert sha(T/'gateway')==gsha
    doc['mount_namespace']=os.readlink('/proc/self/ns/mnt')
    subprocess.run(['mount','--rbind',str(SDK/'gateway/jazzy-rootfs'),str(R)],check=True);mounted=True
    subprocess.run(['mount','--make-rprivate',str(R)],check=True)
    subprocess.run(['mount','--bind',str(W/'source'),str(R/'work')],check=True)
    subprocess.run(['mount','-o','remount,bind,ro',str(R/'work')],check=True)
    subprocess.run(['mount','--bind',str(T),str(R/'tmp')],check=True)
    subprocess.run(['mount','--make-rprivate',str(R)],check=True)
    doc['active_mounts']=[x for x in Path('/proc/self/mountinfo').read_text().splitlines() if x.split()[4]==str(R) or x.split()[4].startswith(str(R)+'/')]
    work_mount=[x for x in doc['active_mounts'] if x.split()[4]==str(R/'work') and x.split()[3]==str(W/'source')];assert len(work_mount)==1 and 'ro' in work_mount[0].split()[5].split(',')
    candidate_stat=(W/'source'/cpp).stat();visible_stat=(R/'work'/cpp).stat();assert (candidate_stat.st_dev,candidate_stat.st_ino)==(visible_stat.st_dev,visible_stat.st_ino)
    doc['visible_source_mount']={'mountinfo':work_mount[0],'source_probe_path':str(W/'source'/cpp),'visible_probe_path':str(R/'work'/cpp),'device':candidate_stat.st_dev,'inode':candidate_stat.st_ino}
    assert all(not any(v.startswith(('shared:','master:','propagate_from:')) for v in x.split()[6:x.split().index('-')]) for x in doc['active_mounts'])
    doc['source_mounted_read_only']=True;doc['rootfs_mounts_private']=True
    run('compiler-version',native(['/usr/bin/c++','--version']))
    flags={}
    for line in F.read_text().splitlines():
        if ' = ' in line:k,v=line.split(' = ',1);flags[k]=shlex.split(v)
    label='native-action-late-close'
    command=['/usr/bin/c++',*flags['CXX_DEFINES'],*flags['CXX_INCLUDES'],'-I','/work/third-parties/fast_io/include','-std=gnu++20','-O1','-g0','-MMD','-MF','/tmp/'+label+'.d','-c','/work/'+cpp,'-o','/tmp/'+label+'.o']
    run('probe-compile',native(command),timeout=180)
    link=shlex.split(L.read_text());link[link.index('CMakeFiles/uwvm-ros2-gateway.dir/main.cc.o')]='/tmp/'+label+'.o';link[link.index('-o')+1]='/tmp/'+label
    run('probe-link',native(link))
    dep=T/(label+'.d');values=inputs(dep,W/'source')
    assert all(frozen[n]==v for n,v in values.items())
    doc['raw_MMD']={'probe':{'path':str(dep),'bytes':dep.stat().st_size,'sha256':sha(dep),'project_inputs':values},'gateway_origin':{'path':str(P/'gateway-origin.d'),'bytes':GM.stat().st_size,'sha256':msha,'project_inputs':old_inputs}}
    doc['binary_sha256']={'gateway':gsha,label:sha(T/label)};(T/'ros-logs').mkdir()
    for mode in a.modes.split(','):
        run('native-'+mode,native(['/tmp/'+label,'/tmp/gateway',mode]),expected=1 if a.phase=='before' and mode=='expired-owner' else 0,artifacts=[T/label,T/'gateway'],timeout=30)
    doc['status']='CONFIRMED_BEFORE_SEMANTIC_FAILURES' if a.phase=='before' else 'PASS'
except BaseException:doc['error']=traceback.format_exc()
finally:
    for child in children:
        if child.poll() is None:os.killpg(child.pid,signal.SIGKILL);child.wait(timeout=5)
    if mounted:
        doc['mount_cleanup']=[]
        for _ in range(20):
            result=subprocess.run(['umount','-R',str(R)],text=True,capture_output=True)
            doc['mount_cleanup'].append({'returncode':result.returncode,'output':result.stdout+result.stderr})
            if result.returncode==0 or 'target is busy' not in result.stderr:break
            time.sleep(.05)
    doc['remaining_owned_mounts']=[x for x in Path('/proc/self/mountinfo').read_text().splitlines() if x.split()[4]==str(W) or x.split()[4].startswith(str(W)+'/')]
    owned=[];unavailable=[]
    for f in Path('/proc').iterdir():
        if not f.name.isdigit():continue
        try:
            exe=os.readlink(f/'exe')
            if exe.startswith(str(W)+'/'):owned.append({'pid':int(f.name),'exe':exe})
        except OSError as error:
            if error.errno not in (2,3):unavailable.append({'pid':int(f.name),'errno':error.errno})
    after=probe();doc.update(remaining_owned_processes=owned,unavailable_process_samples=unavailable,cgroup_after=after)
    assert after['status']=='PASS' and after['membership']==before['membership']
    assert len(before['cgroup_ancestors'])==len(after['cgroup_ancestors'])
    deltas=[]
    for first,last in zip(before['cgroup_ancestors'],after['cgroup_ancestors']):
        assert first['path']==last['path'] and first['memory.max']==last['memory.max'] and (first['memory.events'] is None)==(last['memory.events'] is None)
        if first['memory.events'] is None:continue
        old,new=[dict(line.split() for line in x['memory.events'].splitlines()) for x in (first,last)]
        deltas.append({'path':first['path'],**{k:int(new[k])-int(old[k]) for k in ('oom','oom_kill','oom_group_kill')}})
    doc['oom_deltas']=deltas
    doc['origin_preserved']={'source':inventory(S)==source_before,'gateway_binary':sha(G)==gsha,'gateway_MMD':sha(GM)==msha,'gateway_report':sha(OR)==originsha}
    doc['source_preserved']=inventory(W/'source')==frozen if 'frozen' in globals() else False
    if owned or doc['remaining_owned_mounts'] or not doc['source_preserved'] or not all(doc['origin_preserved'].values()) or any(any(d[k] for k in ('oom','oom_kill','oom_group_kill')) for d in deltas) or mounted and doc['mount_cleanup'][-1]['returncode']:doc['status']='FAIL'
    save(P/'report.json',doc)
    print(json.dumps({'status':doc['status'],'report':str(P/'report.json'),'sha256':sha(P/'report.json'),'error':doc.get('error'),'commands':[(x['name'],x['status'],x.get('returncode')) for x in doc['commands']],'cleanup':doc.get('mount_cleanup'),'owned':owned,'mounts':doc['remaining_owned_mounts'],'oom':deltas},indent=2))
    raise SystemExit(0 if doc['status'] in ('PASS','CONFIRMED_BEFORE_SEMANTIC_FAILURES') else 1)
