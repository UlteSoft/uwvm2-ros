#!/usr/bin/env python3
"""Serial Linux-only scoped native qualification in the parent's 64 GiB cgroup."""
import argparse, fcntl, hashlib, json, os, pathlib, signal, socket, statistics, subprocess, sys, time
if sys.platform!='linux':raise SystemExit('Linux only: do not build or run this qualification on macOS')
p=argparse.ArgumentParser();p.add_argument('--source',type=pathlib.Path,required=True);p.add_argument('--out',type=pathlib.Path,required=True);p.add_argument('--cxx',required=True);a=p.parse_args()
S=a.source.resolve();W=a.out.resolve();W.mkdir(parents=True,exist_ok=False);ENV=dict(os.environ,UWVM_AUDIT_HOLD='1',ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1:print_stacktrace=1')
def sha(p):return hashlib.sha256(pathlib.Path(p).read_bytes()).hexdigest()
def log_failure(log):
    lines=log.read_text(errors='replace').splitlines()
    errors=[line for line in lines if ': error:' in line or line.startswith(('FAIL','ERROR:','SUMMARY:'))]
    selected=errors[:8] if errors else lines[-8:]
    return {'log':str(log),'preview':[line[:500] for line in selected]}
def cgroup(pid):
    membership=pathlib.Path('/proc',str(pid),'cgroup').read_text();rel=next(v.split(':',2)[2] for v in membership.splitlines() if v.startswith('0::'))
    group=pathlib.Path('/sys/fs/cgroup')/rel.lstrip('/');q=group;ancestors=[]
    while True:
        row={'path':str(q)}
        for key in ('memory.max','memory.swap.max','memory.events'):
            try:row[key]=(q/key).read_text().strip()
            except FileNotFoundError:row[key]=None
        ancestors.append(row)
        if q==pathlib.Path('/sys/fs/cgroup'):break
        q=q.parent
    cap=min(int(v['memory.max']) for v in ancestors if v['memory.max'] not in (None,'max'))
    swap=min(int(v['memory.swap.max']) for v in ancestors if v['memory.swap.max'] not in (None,'max'))
    events=dict(line.split() for line in (group/'memory.events').read_text().splitlines())
    value={'pid':pid,'membership':membership,'group':str(group),'effective_memory_max':cap,'effective_swap_max':swap,
           'memory_current':int((group/'memory.current').read_text()),'memory_events':events,
           'live_member':str(pid) in (group/'cgroup.procs').read_text().split(),'ancestors':ancestors}
    assert cap==68719476736 and swap==0 and value['live_member'],value
    return value
def budget():
    value=cgroup(os.getpid());assert value['memory_current']+17179869184+8589934592<=value['effective_memory_max'],value
    return value
manifest=json.loads((S/'source-manifest.json').read_text())['files']
def verify_inputs():
    for rel,row in manifest.items():assert sha(S/rel)==row['sha256'],('changed source',rel)
def run_native(binary,args,stem,sink=None,timeout=30):
    log=W/(stem+'.log');process=subprocess.Popen([str(binary),*args],env=ENV,stdout=subprocess.PIPE,stderr=sink if sink is not None else subprocess.PIPE)
    try:
        deadline=time.monotonic()+10
        while True:
            if process.poll() is not None:
                out,err=process.communicate();log.write_bytes(out+(err or b''))
                raise AssertionError(('exited before main ELF proof',stem,log_failure(log)))
            state=pathlib.Path('/proc',str(process.pid),'status').read_text()
            if '\nState:\tT ' in state:break
            assert time.monotonic()<deadline,('main control point deadline',stem);time.sleep(.005)
        actual=cgroup(process.pid);exe=pathlib.Path('/proc',str(process.pid),'exe')
        with exe.open('rb') as f:
            st=os.fstat(f.fileno());digest=hashlib.sha256(f.read()).hexdigest()
        assert digest==sha(binary) and int(next(v.split()[1] for v in state.splitlines() if v.startswith('PPid:')))==os.getpid()
        proof={**actual,'sha256':digest,'device':st.st_dev,'inode':st.st_ino,'exe':os.readlink(exe),'sampled_state':'T (stopped in native main)'}
        start=time.monotonic();os.kill(process.pid,signal.SIGCONT);out,err=process.communicate(timeout=timeout)
        log.write_bytes(out+(err or b''));assert process.returncode==0,(stem,process.returncode,log_failure(log))
        return {'name':stem,'argv':[str(binary),*args],'returncode':process.returncode,'seconds':time.monotonic()-start,'log':str(log),'log_sha256':sha(log),'main_proof':proof},out.decode()
    finally:
        if process.poll() is None:process.kill();process.communicate()
report={'status':'FAIL','scope':'native diagnostic journals, bounded audit/sink and actual builtin WASIp1 wrapper IO only','commands':[],'cases':[],'benchmark':{},'source_manifest_sha256':sha(S/'source-manifest.json')}
try:
    verify_inputs();report['cgroup_before']=budget();q=subprocess.run([a.cxx,'--version'],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=15);(W/'compiler.log').write_bytes(q.stdout);assert q.returncode==0
    includes=['-I'+str(v) for v in (S,S/'src',S/'third-parties/fast_io/include',S/'third-parties/bizwen/include',S/'third-parties/boost_unordered/include')]
    profiles=[('native',['-O2']),('sanitizer',['-O1','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-omit-frame-pointer','-fno-pie','-no-pie'])]
    inputs=['journal_contract','ros_audit_contract','wasip1_io','sink_contract']
    for profile,flags in profiles:
        for name in inputs:
            budget();binary=W/(profile+'-'+name);dependency=W/(binary.name+'.d');cmd=[a.cxx,'-std=c++26','-pthread','-DUWVM_USE_THREAD_LOCAL=1',*flags,*includes,'-MMD','-MF',str(dependency),str(S/'test/0082.audit-qualification'/(name+'.cc')),'-o',str(binary)]
            start=time.monotonic();q=subprocess.run(cmd,env=ENV,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=240);log=W/(binary.name+'-build.log');log.write_bytes(q.stdout)
            report['commands'].append({'argv':cmd,'returncode':q.returncode,'seconds':time.monotonic()-start,'log':str(log),'log_sha256':sha(log)});assert q.returncode==0,(name,profile,log_failure(log))
            if name!='sink_contract':
                args=[]
                if name=='wasip1_io':directory=W/(profile+'-native-io');directory.mkdir();args=[str(directory)]
                result,text=run_native(binary,args,binary.name);assert 'PASS_' in text;report['cases'].append(result)
            else:
                for mode in ('regular','pipe','oversized','full','closed','invalid','readonly','socket','socket-closed'):
                    handles=[];sockets=[]
                    try:
                        if mode in ('regular','invalid','readonly'):
                            path=W/(profile+'-'+mode+'.sink');path.write_bytes(b'');sink=os.open(path,os.O_RDONLY if mode=='readonly' else os.O_WRONLY);handles.append(sink)
                        elif mode.startswith('socket'):
                            left,right=socket.socketpair();sockets=[left,right];sink=left.fileno()
                            if mode=='socket-closed':right.close()
                        else:
                            reader,sink=os.pipe();handles.extend([reader,sink])
                            if mode=='full':
                                os.set_blocking(sink,False)
                                while True:
                                    try:os.write(sink,b'x'*4096)
                                    except BlockingIOError:break
                                os.set_blocking(sink,True)
                            if mode=='closed':os.close(reader);handles.remove(reader)
                        flags=fcntl.fcntl(sink,fcntl.F_GETFL);result,text=run_native(binary,[mode],profile+'-sink-'+mode,sink,3)
                        assert 'PASS_AUDIT_SINK' in text and fcntl.fcntl(sink,fcntl.F_GETFL)==flags;report['cases'].append(result)
                    finally:
                        for fd in handles:os.close(fd)
                        for sock in sockets:sock.close()
        if profile=='native':
            result,text=run_native(W/'native-wasip1_io',['benchmark'],'native-disabled-benchmark');report['cases'].append(result)
            samples=[dict(v.split('=',1) for v in line.split()[1:]) for line in text.splitlines() if line.startswith('BENCH ')]
            assert len(samples)==9;values={key:[int(row[key])/int(row['rounds']) for row in samples] for key in ('baseline_ns','disabled_ns','hook_ns')}
            medians={key:statistics.median(rows) for key,rows in values.items()};base=medians['baseline_ns']
            report['benchmark']={'workload':'actual builtin fd_write wrapper, valid descriptor, zero iovecs, trace disabled; baseline is its original base::call','samples_ns_per_call':values,'median_ns_per_call':medians,
                'disabled_extra_ns_per_call':medians['disabled_ns']-base,'disabled_overhead_percent':100*(medians['disabled_ns']/base-1),
                'installed_hook_extra_ns_per_call':medians['hook_ns']-base,'claim_limit':'native microbenchmark only; no full Wasm/ROS workload performance claim'}
    verify_inputs();report['cgroup_after']=cgroup(os.getpid())
    for key in ('oom','oom_kill','oom_group_kill'):assert int(report['cgroup_after']['memory_events'].get(key,0))==int(report['cgroup_before']['memory_events'].get(key,0))
    report['status']='PASS_SCOPED_COMPONENTS'
except BaseException as error:
    report['error']=repr(error);raise
finally:
    (W/'result.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'status':report['status'],'cases':len(report['cases']),'result':str(W/'result.json'),'benchmark':report['benchmark']}))
