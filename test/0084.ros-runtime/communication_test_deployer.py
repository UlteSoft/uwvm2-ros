"""Test-only provisioning of independent ROS gateways and one VM process.

No production deployment service is implemented. The harness owns lifecycle,
retains exact child PIDs, supplies exclusive directional channels, and verifies
that VM exit did not terminate either external ROS gateway. All descendants
inherit the mandatory existing cgroup; this does not create resource domains.
"""
import os,select,signal,socket,struct,subprocess,time
from run_scene_vm import verify_gate

def execute(argv,packs,gateway,timeout=150):
    verify_gate();resources=[];sockets=[];peers=[];pass_fds=[];extra=[];runtime=None
    def pipe(flags):
        pair=os.pipe2(flags);resources.extend(pair);return pair
    def close(fd):
        if fd in resources:os.close(fd);resources.remove(fd)
    try:
        for i,pack in enumerate(packs):
            local,remote=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET|socket.SOCK_CLOEXEC)
            sockets.extend([local,remote])
            for endpoint in (local,remote):endpoint.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
            local_fds=[];remote_fds=[]
            for n in range(7):
                read,write=pipe(os.O_NONBLOCK|os.O_CLOEXEC);writing=n in (0,3,5)
                local_fds.append(write if writing else read);remote_fds.append(read if writing else write)
            ready_read,ready_write=pipe(os.O_CLOEXEC);identity_read,identity_write=pipe(os.O_CLOEXEC)
            launcher=pack['launcher'];args=[str(gateway),str(remote.fileno()),str(ready_write),str(identity_read),
                *map(str,remote_fds),str(pack['required_image_bytes']),'64','65536','65536',
                launcher['path'],launcher['sha256'],str(len(pack['files']))]
            for row in pack['files']:args.extend([row['role'],row['destination'],row['path'],row['sha256']])
            process=subprocess.Popen(args,pass_fds=[remote.fileno(),ready_write,identity_read,*remote_fds],stdout=subprocess.DEVNULL,stderr=subprocess.PIPE)
            peers.append((process,ready_read,identity_write));remote.close();close(ready_write);close(identity_read)
            for fd in remote_fds:close(fd)
            pass_fds.extend([local.fileno(),*local_fds])
            ids=[bytes([1])*16,bytes([2])*16,bytes([3+i])*16,bytes([5+i])*16,bytes([7+i])*16]
            expiry=time.monotonic_ns()+180_000_000_000
            extra+=['--ros-scene-communication-channel','robot',f'worker.{i}',str(local.fileno()),str(process.pid),str(os.getuid()),str(os.getgid()),
                *map(str,local_fds),*[value.hex() for value in ids],bytes([11+i]).hex()*32,'10000000000',str(expiry),'65536']
        # The waiting exec wrapper preserves the PID authenticated by gateways.
        start_read,start_write=pipe(os.O_CLOEXEC)
        target=list(argv)
        position=min((target.index(option) for option in ('--run','--ros-scene-check') if option in target),default=len(target))
        target[position:position]=extra
        wrapper='import os,sys; fd=int(sys.argv[1]); assert os.read(fd,1)==b"R"; os.close(fd); os.execv(sys.argv[2],sys.argv[2:])'
        runtime=subprocess.Popen(['/usr/bin/python3','-c',wrapper,str(start_read),*target],pass_fds=[start_read,*pass_fds],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        close(start_read)
        # Only the stopped VM and designated gateway retain each channel end.
        # A parent copy would suppress EOF after either child dies and make a
        # deployment test falsely depend on its timeout instead of pipe state.
        for endpoint in sockets:
            endpoint.close()
        for fd in pass_fds:
            if fd in resources:
                close(fd)
        for peer,ready,identity in peers:
            assert os.write(identity,struct.pack('<Q',runtime.pid))==8;close(identity)
            assert select.select([ready],[],[],16)[0] and os.read(ready,1)==b'R','external gateway startup failed'
            close(ready)
        assert os.write(start_write,b'R')==1;close(start_write)
        stdout,stderr=runtime.communicate(timeout=timeout)
        assert all(peer.poll() is None for peer,_,_ in peers),'VM exit terminated an externally owned gateway'
        return subprocess.CompletedProcess(target,runtime.returncode,stdout.decode(),stderr.decode())
    finally:
        if runtime is not None and runtime.poll() is None:runtime.kill();runtime.communicate()
        for peer,_,_ in peers:
            if peer.poll() is None:peer.terminate()
        failures=[]
        for peer,_,_ in peers:
            try:_,stderr=peer.communicate(timeout=15)
            except subprocess.TimeoutExpired:peer.kill();_,stderr=peer.communicate();failures.append('gateway cleanup timed out')
            if peer.returncode:failures.append('gateway exit '+str(peer.returncode)+': '+stderr.decode(errors='replace')[-3000:])
        for endpoint in sockets:endpoint.close()
        for fd in resources:os.close(fd)
        if failures:raise RuntimeError('\n'.join(failures))
