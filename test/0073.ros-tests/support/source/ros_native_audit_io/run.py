#!/usr/bin/env python3
"""CPP17 native gateway audit sink regressions; no ROS/A15 or rootfs mounts."""
import argparse,hashlib,json,os,pathlib,shlex,signal,socket,subprocess,time,traceback,fcntl,pty,tty,errno,shutil,re
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--source',type=pathlib.Path,default=Path(__file__).resolve().parents[5])
parser.add_argument('--audit-source',type=pathlib.Path,help='Optional explicit audit-header overlay source root; defaults to --source')
parser.add_argument('--work',type=pathlib.Path,required=True,help='Fresh output directory; existing paths are refused')
parser.add_argument('--compiler',default='clang++')
parser.add_argument('--compiler-library-path',help='Optional LD_LIBRARY_PATH for the selected compiler/runtime')
args=parser.parse_args();S=args.source.resolve();A=args.audit_source.resolve() if args.audit_source is not None else S;W=args.work.resolve();W.mkdir(exist_ok=False)
C=shutil.which(args.compiler) or str(pathlib.Path(args.compiler).resolve())
ENV=dict(os.environ)
if args.compiler_library_path is not None:ENV['LD_LIBRARY_PATH']=args.compiler_library_path
P=pathlib.Path(__file__).resolve().parent
H=lambda p:hashlib.sha256(pathlib.Path(p).read_bytes()).hexdigest()
copies={'after_probe.cc':P/'unit.cc','executed-driver.py':pathlib.Path(__file__),'source/src/ros/gateway/common/native_io.h':A/'src/ros/gateway/common/native_io.h','source/src/ros/gateway/common/server.h':A/'src/ros/gateway/common/server.h','source/src/ros/gateway/common/config.h':S/'src/ros/gateway/common/config.h'}
for rel,source in copies.items():
    p=W/rel;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(source.read_bytes())
manifest={rel:{'sha256':H(W/rel),'bytes':(W/rel).stat().st_size} for rel in copies}
(W/'input-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
def cg(pid):
    membership=pathlib.Path('/proc',str(pid),'cgroup').read_text().splitlines();group=pathlib.Path('/sys/fs/cgroup')/next(v.split(':',2)[2] for v in membership if v.startswith('0::')).lstrip('/')
    rows=[];q=group
    while True:
        row={'path':str(q),'memory.max':None,'memory.events':None}
        for key in ['memory.max','memory.events']:
            try:row[key]=(q/key).read_text().strip()
            except (FileNotFoundError,PermissionError):pass
        rows.append(row)
        if q==pathlib.Path('/sys/fs/cgroup'):break
        q=q.parent
    maximum=min(int(v['memory.max']) for v in rows if v['memory.max'] not in (None,'max'))
    return {'pid':pid,'membership':membership,'effective_memory_max':maximum,'ancestors':rows,'memory_current':int((group/'memory.current').read_text()),'live_cgroup_procs':str(pid) in (group/'cgroup.procs').read_text().split()}
def read_all(fd):
    os.set_blocking(fd,False);parts=[]
    while True:
        try:b=os.read(fd,65536)
        except BlockingIOError:break
        except OSError as e:
            if e.errno==errno.EIO:break
            raise
        if not b:break
        parts.append(b)
    return b''.join(parts)
def filled(fd):
    os.set_blocking(fd,False);n=0
    while True:
        try:n+=os.write(fd,b'x'*4096)
        except BlockingIOError:break
    os.set_blocking(fd,True);return n
def record(mode):
    if mode in ['limit','over','long']:
        size={'limit':4095,'over':4096,'long':9000}[mode]
        return ('{"value":"'+'x'*(size-12)+'"}').encode()+b'\n'
    if mode=='empty':return b'\n'
    return b'{"event":"audit-io-test","error":13}\n'
r={'status':'FAIL','scope':'CPP17 native gateway audit helper and exact server audit function; real bounded native processes; no full Gateway/ROS/A15 runtime claim','commands':[],'cases':[],'profiles':[],'cgroup_before':cg(os.getpid()),'executed_driver_sha256':H(__file__),'input_manifest_sha256':H(W/'input-manifest.json'),'source_root':str(S),'audit_source_root':str(A),'actual_argv':__import__('sys').argv}
children=[];fds=[];objects=[]
try:
    assert r['cgroup_before']['effective_memory_max']==68719476736 and r['cgroup_before']['live_cgroup_procs']
    assert (68719476736-r['cgroup_before']['memory_current']-17179869184)//8589934592>=1
    (W/'build-budget.json').write_text(json.dumps({'compiler_estimate':8589934592,'reserve':17179869184,'jobs':1,'before':r['cgroup_before']},indent=2)+'\n')
    plans=[('cpp17-std-san',['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']),
           ('cpp17-std-normal',['-std=c++17','-O2']),
           ('cpp17-libcxx-san',['-std=c++17','-stdlib=libc++','-O1','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie'])]
    for name,options in [('compiler-version',['--version'])]+plans:
        cmd=[C,*options] if name=='compiler-version' else [C,*options,'-pthread','-I'+str(W/'source/src/ros/gateway/common'),'-I'+str(S/'src'),'-I'+str(S/'third-parties/fast_io/include'),'-MMD','-MF',str(W/(name+'.d')),str(W/'after_probe.cc'),'-o',str(W/name)]
        start=time.monotonic();q=subprocess.run(cmd,env=ENV,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=120)
        log=W/(name+'-build.log');log.write_bytes(q.stdout);r['commands'].append({'name':name,'command':cmd,'returncode':q.returncode,'log':str(log),'log_sha256':H(log),'seconds':time.monotonic()-start})
        assert q.returncode==0,(name,q.returncode,log.read_text())
    scenarios=[('regular','one',1),('regular-append','one',1),('regular-long','long',1),('writable-pipe','one',1),('nonblocking-pipe','one',1),
               ('full-pipe','one',0),('near-full-pipe','one',0),('pipe-limit','limit',1),('pipe-over','over',0),('closed-pipe','one',0),
               ('invalid','invalid',0),('readonly','one',0),('null','one',1),('tty','one',1),('tty-full','one',None),
               ('socket','one',1),('socket-full','one',0),('socket-closed','one',0),('signal-guard','guard',1),('signal-pending','pending',1),
               ('pipe-empty','empty',1),('concurrent-pipe','concurrent',1),('server-regular','server',1),('server-pipe','server',1),('server-full','server',0),('server-closed','server',0),('server-quota','quota',0),('saturated-suppress','saturated-suppress',0)]
    for profile,_ in plans:
        binary=W/profile;binary_sha=H(binary);profile_inputs={}
        for name in shlex.split((W/(profile+'.d')).read_text().replace('\\\n',' ').split(':',1)[1]):
            p=pathlib.Path(name)
            if p.is_file():profile_inputs[str(p)]={'sha256':H(p),'bytes':p.stat().st_size}
        r['profiles'].append({'name':profile,'binary_sha256':binary_sha,'raw_mmd_sha256':H(W/(profile+'.d')),'compiled_inputs':profile_inputs})
        for kind,mode,expected in scenarios:
            handles=[];socks=[];reader=None;prefix=b'';target=None
            if kind.startswith('regular') or kind in ['readonly','signal-guard','signal-pending','invalid','server-regular','server-quota','saturated-suppress']:
                target=W/(profile+'-'+kind+'.sink');target.write_bytes(b'prefix:');flags=os.O_RDONLY if kind=='readonly' else os.O_RDWR
                if kind=='regular-append':flags|=os.O_APPEND
                sink=os.open(target,flags);handles.append(sink)
                if kind!='readonly':os.lseek(sink,7,os.SEEK_SET)
                prefix=b'prefix:'
            elif kind=='null':sink=os.open('/dev/null',os.O_WRONLY);handles.append(sink)
            elif kind.startswith('tty'):
                reader,sink=pty.openpty();handles.extend([reader,sink]);tty.setraw(sink)
                if kind=='tty-full':prefix=b'x'*filled(sink)
            elif kind.startswith('socket'):
                left,right=socket.socketpair();socks.extend([left,right]);sink=left.fileno();reader=right.fileno()
                if kind=='socket-full':prefix=b'x'*filled(sink)
                if kind=='socket-closed':right.close();reader=None
            else:
                reader,sink=os.pipe();handles.extend([reader,sink])
                if kind=='nonblocking-pipe':os.set_blocking(sink,False)
                if kind in ['full-pipe','near-full-pipe','pipe-limit','server-full']:
                    prefix=b'x'*filled(sink)
                    if kind=='near-full-pipe':prefix=prefix[len(os.read(reader,16)):]
                    if kind=='pipe-limit':prefix=prefix[len(os.read(reader,4096)):]
                if kind in ['closed-pipe','server-closed']:os.close(reader);handles.remove(reader);reader=None
            before_flags=fcntl.fcntl(sink,fcntl.F_GETFL);before_fdflags=fcntl.fcntl(sink,fcntl.F_GETFD)
            process=subprocess.Popen([str(binary),mode],env=ENV,stdout=subprocess.PIPE,stderr=sink);children.append(process)
            deadline=time.monotonic()+3
            while True:
                assert process.poll() is None,(profile,kind,'exited before ELF control point')
                text=pathlib.Path('/proc',str(process.pid),'status').read_text()
                if '\nState:\tT ' in text:break
                assert time.monotonic()<deadline,(profile,kind,'ELF control point deadline');time.sleep(.001)
            live=cg(process.pid);parent=int(next(v.split()[1] for v in text.splitlines() if v.startswith('PPid:')))
            exe=os.readlink(pathlib.Path('/proc',str(process.pid),'exe'));fd=os.open(pathlib.Path('/proc',str(process.pid),'exe'),os.O_RDONLY)
            with os.fdopen(fd,'rb') as f:st=os.fstat(f.fileno());actual=hashlib.sha256(f.read()).hexdigest()
            assert actual==binary_sha and parent==os.getpid() and live['live_cgroup_procs'] and live['effective_memory_max']==68719476736
            main={**live,'parent_pid':parent,'device':st.st_dev,'inode':st.st_ino,'sha256':actual,'exe':exe,'sampled_native_state':'T (stopped)','live_identity_captured_before_fd_hash':True}
            started=time.monotonic();os.kill(process.pid,signal.SIGCONT);out=process.communicate(timeout=3)[0];elapsed=time.monotonic()-started
            log=W/(profile+'-'+kind+'.log');log.write_bytes(out)
            assert process.returncode==0,(profile,kind,process.returncode,out)
            assert b'PASS native-audit-io' in out and b'preserved_signal_mask_pending_handler=1' in out
            fields=dict(part.split('=',1) for part in out.decode().split() if '=' in part)
            if kind=='tty-full':
                assert int(fields['complete']) in (0,1)
                expected=int(fields['complete'])
            assert int(fields['complete'])==expected,(profile,kind,fields)
            delivered,dropped=int(fields['completed_calls']),int(fields['failed_calls'])
            expect_counts=(0,0) if mode in ['guard','pending','server','quota','saturated-suppress'] else (64,0) if mode=='concurrent' else (expected,1-expected)
            assert (delivered,dropped)==expect_counts,(profile,kind,fields)
            if mode in ['server','quota','saturated-suppress']:
                queued,suppressed=int(fields['queued_records']),int(fields['suppressed'])
                assert queued==(1 if mode=='server' else 0)
                assert suppressed==((0 if expected else 1) if mode=='server' else (2**64-1 if mode=='saturated-suppress' else 1))
            assert fcntl.fcntl(sink,fcntl.F_GETFL)==before_flags and fcntl.fcntl(sink,fcntl.F_GETFD)==before_fdflags
            if mode not in ['concurrent','guard','pending']:
                assert int(fields['cpp_allocations'])==0,(profile,kind,fields)
            expected_bytes=prefix
            server_output=mode=='server' and expected
            if expected and mode not in ['guard','pending','server']:
                expected_bytes+=record(mode)*(64 if mode=='concurrent' else 1)
            if target:
                actual_bytes=target.read_bytes()
                if server_output:assert actual_bytes.startswith(prefix) and re.fullmatch(rb'ros_audit timestamp_ns=-?[0-9]+ backend=2 worker_uid=1000 worker_pid=12345 session=77 operation=time.read-steady alias=clock error=13\n',actual_bytes[len(prefix):])
                else:assert actual_bytes==expected_bytes,(profile,kind,'regular contents differ')
            elif reader is not None:
                actual_bytes=read_all(reader)
                if server_output:assert actual_bytes.startswith(prefix) and re.fullmatch(rb'ros_audit timestamp_ns=-?[0-9]+ backend=2 worker_uid=1000 worker_pid=12345 session=77 operation=time.read-steady alias=clock error=13\n',actual_bytes[len(prefix):])
                elif kind=='tty-full':
                    assert actual_bytes.startswith(prefix)
                    tail=actual_bytes[len(prefix):]
                    if expected:assert tail==record(mode)
                    else:assert record(mode).startswith(tail) and len(tail)<len(record(mode))
                else:assert actual_bytes==expected_bytes,(profile,kind,'stream bytes differ',len(actual_bytes),len(expected_bytes))
            else:actual_bytes=None
            output_log=None
            if actual_bytes is not None:
                output_log=W/(profile+'-'+kind+'-sink-output.log');output_log.write_bytes(actual_bytes)
            r['cases'].append({'profile':profile,'kind':kind,'mode':mode,'command':[str(binary),mode],'returncode':process.returncode,'seconds':elapsed,'log':str(log),'log_sha256':H(log),
                'expected_complete':expected,'test_completed_calls':delivered,'test_failed_calls':dropped,'queued_records':int(fields['queued_records']),'suppressed':int(fields['suppressed']),'cpp_allocations':int(fields['cpp_allocations']),'original_fd_flags':before_flags,'original_fd_descriptor_flags':before_fdflags,
                'original_fd_flags_unchanged':True,'signal_mask_pending_handler_unchanged':True,'exact_output_bytes_checked':actual_bytes is not None,'output_bytes':None if actual_bytes is None else len(actual_bytes),'output_log':None if output_log is None else str(output_log),'output_sha256':None if output_log is None else H(output_log),'actual_main':main})
            for sock in socks:sock.close()
            for fd in handles:os.close(fd)
    assert len(r['cases'])==84 and len(r['profiles'])==3
    r['status']='PASS'
except Exception as e:r.update(error=str(e),traceback=traceback.format_exc())
finally:
    for process in children:
        if process.poll() is None:process.kill();process.wait(timeout=3)
    r['cgroup_after']=cg(os.getpid());r['remaining_owned_processes']=[v.pid for v in children if v.poll() is None]
    r['oom_deltas']=[]
    for x,y in zip(r['cgroup_before']['ancestors'],r['cgroup_after']['ancestors']):
        assert x['path']==y['path'] and x['memory.max']==y['memory.max']
        if x['memory.events'] is None:continue
        a,b=[dict(line.split() for line in z['memory.events'].splitlines()) for z in [x,y]]
        r['oom_deltas'].append({'path':x['path'],**{k:int(b[k])-int(a[k]) for k in ['oom','oom_kill','oom_group_kill']}})
    r['compiled_inputs_preserved']=all(H(path)==v['sha256'] for p in r['profiles'] for path,v in p['compiled_inputs'].items())
    if r['remaining_owned_processes'] or not r['compiled_inputs_preserved'] or any(any(v[k] for k in ['oom','oom_kill','oom_group_kill']) for v in r['oom_deltas']):r['status']='FAIL'
    (W/'report.json').write_text(json.dumps(r,indent=2)+'\n')
    print(json.dumps({'status':r['status'],'profiles':len(r['profiles']),'cases':len(r['cases']),'report':str(W/'report.json'),'report_sha256':H(W/'report.json'),'error':r.get('error'),'oom_deltas':r['oom_deltas']}))
    raise SystemExit(0 if r['status']=='PASS' else 1)
