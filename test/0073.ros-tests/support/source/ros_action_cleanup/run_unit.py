#!/usr/bin/env python3
"""Standalone C++17 cleanup_pool ownership/capacity tests; no ROS/DDS claims."""
import argparse,hashlib,json,os,pathlib,re,signal,subprocess,sys,time,traceback
sys.dont_write_bytecode=True
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--source',type=pathlib.Path,required=True)
p.add_argument('--out',type=pathlib.Path,required=True)
p.add_argument('--compiler',default='/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/bin/clang++')
a=p.parse_args();S=a.source.resolve();W=a.out.resolve();W.mkdir(mode=0o700)
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
def save(p,d):p.write_text(json.dumps(d,indent=2,sort_keys=True)+'\n')
def cg(pid):
    membership=pathlib.Path('/proc',str(pid),'cgroup').read_text().splitlines()
    leaf=pathlib.Path('/sys/fs/cgroup')/next(x[3:] for x in membership if x.startswith('0::')).lstrip('/')
    rows=[];q=leaf
    while True:
        def field(name):
            try:return (q/name).read_text().strip()
            except (OSError,PermissionError):return None
        rows.append({'path':str(q),'memory.max':field('memory.max'),'memory.events':field('memory.events')})
        if q==pathlib.Path('/sys/fs/cgroup'):break
        q=q.parent
    maximum=min(int(x['memory.max']) for x in rows if x['memory.max'] not in (None,'max'))
    assert maximum==68719476736
    return {'pid':pid,'membership':membership,'effective_memory_max':maximum,'ancestors':rows,'live_cgroup_procs':str(pid) in (leaf/'cgroup.procs').read_text().split()}
env=os.environ.copy();env['LD_LIBRARY_PATH']='/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib:/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu'
doc={'status':'FAIL','kind':'standalone-action-cleanup-pool','source':str(S),'argv':sys.argv,'executed_driver_sha256':H(pathlib.Path(__file__)),'commands':[],'cgroup_before':cg(os.getpid()),'scope':'CPP17 normal and ASan/UBSan ownership/capacity stub unit; no SDK/ROS/DDS/network qualification'}
shutil=None
(W/'executed-driver.py').write_bytes(pathlib.Path(__file__).read_bytes())
inputs={str(q.relative_to(S)):H(q) for q in S.rglob('*') if q.is_file()};save(W/'source-before.json',inputs)
children=[]
def run(name,command,binary=None):
    log=W/(name+'.log');pre=W/(name+'-preexec.json');row={'name':name,'command':command,'log':str(log),'status':'FAIL'};doc['commands'].append(row)
    with log.open('wb') as output:
        proc=subprocess.Popen(command,stdout=output,stderr=subprocess.STDOUT,env=env,start_new_session=True,preexec_fn=lambda:save(pre,cg(os.getpid())))
    children.append(proc);row['pid']=proc.pid;until=time.monotonic()+90
    if binary:
        while proc.poll() is None:
            try:
                status=pathlib.Path('/proc',str(proc.pid),'status').read_text()
                if 'State:\tT' in status:
                    fd=os.open(pathlib.Path('/proc',str(proc.pid),'exe'),os.O_RDONLY)
                    with os.fdopen(fd,'rb') as stream:
                        st=os.fstat(stream.fileno());expected=binary.stat();c=cg(proc.pid)
                        parent=int(next(x.split()[1] for x in status.splitlines() if x.startswith('PPid:')))
                        exe=os.readlink(pathlib.Path('/proc',str(proc.pid),'exe'));captured=time.monotonic_ns()
                        assert (st.st_dev,st.st_ino)==(expected.st_dev,expected.st_ino) and c['live_cgroup_procs'] and parent==os.getpid()
                        digest=hashlib.sha256()
                        for block in iter(lambda:stream.read(1048576),b''):digest.update(block)
                        assert digest.hexdigest()==H(binary)
                        row['actual_main']={**c,'sha256':digest.hexdigest(),'parent_pid':parent,'device':st.st_dev,'inode':st.st_ino,'exe':exe,'capture_monotonic_ns':captured,'live_identity_captured_before_fd_hash':True}
                    os.kill(proc.pid,signal.SIGCONT);break
            except FileNotFoundError:pass
            if time.monotonic()>=until:raise RuntimeError('main stop handshake timeout '+name)
            time.sleep(.005)
    while proc.poll() is None:
        if time.monotonic()>=until:raise RuntimeError('command deadline '+name)
        time.sleep(.005)
    row.update(returncode=proc.returncode,log_sha256=H(log),preexec_sha256=H(pre))
    assert proc.returncode==0,(name,log.read_text())
    if binary:assert row.get('actual_main') and 'PASS action-cleanup unit groups=8 retained_capacity_release_duplicate_concurrency_destructor' in log.read_text()
    row['status']='PASS';print('PASS',name,flush=True);return row
try:
    run('compiler-version',[a.compiler,'--version'])
    union={};deps={}
    for name,san in (('normal',False),('asan-ubsan',True)):
        binary=W/name;dep=W/(name+'.d')
        flags=['-std=c++17','-stdlib=libstdc++','-O1','-g','-pthread','-UNDEBUG','-MMD','-MF',str(dep),'-I',str(S/'src/ros/gateway/ros2'),'-I',str(S/'src/ros/gateway/common')]
        if san:flags+=['-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie']
        run(name+'-build',[a.compiler,*flags,str(S/'test/0073.ros-tests/support/source/ros_action_cleanup/unit.cc'),'-o',str(binary)])
        values={}
        for item in re.findall(re.escape(str(S))+r'/[^\s\\]+',dep.read_text().replace('\\\n',' ')):
            q=pathlib.Path(item).resolve();assert q.is_relative_to(S);key=str(q.relative_to(S));values[key]=H(q);union[key]=values[key]
        assert 'src/ros/gateway/ros2/action_cleanup.h' in values
        deps[str(dep)]={'sha256':H(dep),'bytes':dep.stat().st_size,'project_inputs':values}
        row=run(name+'-run',[str(binary)],binary);row['binary_sha256']=H(binary)
    doc.update(status='PASS',raw_compiler_dependencies=deps,compiled_project_inputs=union,unit_groups_per_profile=8,actual_group_runs=16)
except Exception as error:doc.update(reason=str(error),traceback=traceback.format_exc())
finally:
    for proc in children:
        if proc.poll() is None:
            os.killpg(proc.pid,signal.SIGCONT);os.killpg(proc.pid,signal.SIGTERM)
            try:proc.wait(timeout=3)
            except subprocess.TimeoutExpired:os.killpg(proc.pid,signal.SIGKILL);proc.wait(timeout=3)
    doc['remaining_owned_processes']=[q.pid for q in children if q.poll() is None]
    doc['source_preserved']={str(q.relative_to(S)):H(q) for q in S.rglob('*') if q.is_file()}==inputs
    doc['cgroup_after']=cg(os.getpid());deltas=[]
    for first,last in zip(doc['cgroup_before']['ancestors'],doc['cgroup_after']['ancestors']):
        assert first['path']==last['path'] and first['memory.max']==last['memory.max']
        if first['memory.events'] is None:
            assert last['memory.events'] is None;continue
        def events(text):return dict((k,int(v)) for k,v in (line.split() for line in text.splitlines()))
        old,new=events(first['memory.events']),events(last['memory.events'])
        deltas.append({'path':first['path'],**{k:new[k]-old[k] for k in ('oom','oom_kill','oom_group_kill')}})
    doc['oom_deltas']=deltas
    if not doc['source_preserved'] or doc['remaining_owned_processes'] or any(any(x[k] for k in ('oom','oom_kill','oom_group_kill')) for x in deltas):doc['status']='FAIL'
    save(W/'report.json',doc);print(json.dumps({'status':doc['status'],'reason':doc.get('reason'),'report':str(W/'report.json'),'report_sha256':H(W/'report.json')}),flush=True)
    raise SystemExit(doc['status']!='PASS')
