#!/usr/bin/env python3
"""Test-only external namespace provisioning; never part of the VM or deployer.

Every process inherits the existing 64 GiB test gate. The fixture shares the
PID and time namespaces so that both endpoints can validate exact credentials
and absolute deadlines; user/mount/network/IPC/UTS namespaces are distinct.
The closure is sealed and read-only, capabilities are dropped, and the native
fixture applies the final syscall filter before receiving application data.
"""
import fcntl,json,os,resource,select,subprocess,time
from pathlib import Path
from run_scene_vm import GATE,sha,verify_gate

def launch(binary,model,mode,count,fd,closure,filter_bytes):
    verify_gate();owned=[];process=None
    def sealed(data):
        value=os.memfd_create('driver-fixture-closure',os.MFD_CLOEXEC|os.MFD_ALLOW_SEALING);owned.append(value)
        with os.fdopen(os.dup(value),'wb') as stream:stream.write(data)
        os.lseek(value,0,0);fcntl.fcntl(value,fcntl.F_ADD_SEALS,fcntl.F_SEAL_WRITE|fcntl.F_SEAL_GROW|fcntl.F_SEAL_SHRINK|fcntl.F_SEAL_SEAL)
        return value
    info=os.memfd_create('driver-fixture-pid',os.MFD_CLOEXEC);owned.append(info)
    release_read,release_write=os.pipe2(os.O_CLOEXEC);owned.extend((release_read,release_write))
    argv=['/usr/bin/bwrap','--unshare-user','--unshare-ipc','--unshare-net','--unshare-uts',
          '--disable-userns','--assert-userns-disabled','--clearenv','--new-session','--die-with-parent','--cap-drop','ALL']
    directories=sorted({str(parent) for _,dest in closure for parent in Path(dest).parents if str(parent)!='/'},key=lambda p:(len(Path(p).parts),p))
    for directory in directories:argv+=['--dir',directory]
    for path,destination in closure:argv+=['--perms','0500','--ro-bind-data',str(sealed(path.read_bytes())),destination]
    argv+=['--info-fd',str(info),'--block-fd',str(release_read),'--remount-ro','/','--chdir','/worker','--setenv','LD_LIBRARY_PATH','/lib',
           '--seccomp',str(sealed(filter_bytes)),'--','/worker/receiver',str(fd),mode,'/fixture/model.xml',str(count),'confined']
    def limits():
        resource.setrlimit(resource.RLIMIT_CORE,(0,0));resource.setrlimit(resource.RLIMIT_AS,(1073741824,1073741824))
        resource.setrlimit(resource.RLIMIT_CPU,(10,10));resource.setrlimit(resource.RLIMIT_NOFILE,(128,128));resource.setrlimit(resource.RLIMIT_FSIZE,(134217728,134217728))
    try:
        process=subprocess.Popen(argv,pass_fds=tuple(owned)+(fd,),stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,preexec_fn=limits)
        until=time.monotonic()+10
        while True:
            try:identity=json.loads(os.pread(info,4096,0));break
            except json.JSONDecodeError:
                if process.poll() is not None:raise RuntimeError(('external namespace startup failed',process.returncode,process.stderr.read()[-3000:]))
                assert time.monotonic()<until,'external namespace startup timeout'
                time.sleep(.01)
        pid=identity['child-pid'];base=Path('/proc',str(pid));names={}
        assert (base/'cgroup').read_text().strip()=='0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
        for name in ('user','mnt','net','ipc','uts','pid','time'):
            observed=(base/'ns'/name).stat().st_ino;parent=Path('/proc/self/ns',name).stat().st_ino
            assert (observed==parent)==(name in ('pid','time')),(name,observed,parent)
            names[name]=dict(driver=observed,harness=parent)
        os.write(release_write,b'x')
        assert select.select([process.stderr],[],[],10)[0],'external fixture readiness timeout'
        ready=process.stderr.readline()
        assert ready.startswith(b'{') and json.loads(ready)==dict(kind='confined',escape_checks=7),('external fixture readiness failed',ready,process.poll())
        return process,pid,dict(argv=argv,namespaces=names,cgroup=str(GATE),ready_line=ready.decode(),closure=[dict(source=str(path),destination=dest,sha256=sha(path)) for path,dest in closure],filter_sha256=__import__('hashlib').sha256(filter_bytes).hexdigest())
    except BaseException:
        if process is not None and process.poll() is None:process.kill();process.wait()
        raise
    finally:
        for value in owned:os.close(value)
