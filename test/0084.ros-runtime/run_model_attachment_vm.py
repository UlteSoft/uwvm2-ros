#!/usr/bin/env python3
"""Exercise attachment-only model IPC with real VM processes and original Wasm carriers.

The harness owns every peer process; the VM only receives preconnected FDs.
This qualifies a deterministic matrix service, not a general model framework.
"""
import argparse,array,contextlib,hashlib,json,os,resource,signal,socket,struct,subprocess,sys,time
from pathlib import Path
from run_scene_vm import ROOT,GATE,sha,verify_gate,policy
from run_scene_buffers import BufferGuest,u64,blob,text,layout
from run_scene_models import MODEL,OPERATIONS,PROFILE_SHA,profile
MAGIC=0x4d534f52564d5755
IDS=[bytes([n])*16 for n in range(1,6)]
DIGEST=hashlib.sha256(b'matrix external model session contract').digest()
BINDING=b''.join(IDS)+DIGEST

def header(kind,sequence,deadline,length,offset=0):
    return struct.pack('<4Q',MAGIC,176,kind,0)+BINDING+struct.pack('<4Q',sequence,deadline,length,offset)

def receiver(fd,mode,secure=None):
    verify_gate();s=socket.socket(fileno=fd);s.settimeout(10);peer=None
    def read():
        nonlocal peer
        if secure:packet=secure.recv()
        else:
            packet,ancillary,flags,_=s.recvmsg(176+65536,socket.CMSG_SPACE(12))
            assert not flags&(socket.MSG_TRUNC|socket.MSG_CTRUNC)
            creds=[struct.unpack('3i',data) for level,kind,data in ancillary if level==socket.SOL_SOCKET and kind==socket.SCM_CREDENTIALS]
            assert len(creds)==1 and len(ancillary)==1 and creds[0][0]>0 and creds[0][1:]==(os.getuid(),os.getgid())
            if peer is None:peer=creds[0]
            assert creds[0]==peer and len(packet)>=176
        magic,size,kind,flags=struct.unpack_from('<4Q',packet);sequence,deadline,length,offset=struct.unpack_from('<4Q',packet,144)
        assert magic==MAGIC and size==176 and flags==0 and packet[32:144]==BINDING and length==len(packet)-176
        return kind,sequence,deadline,offset,packet[176:]
    send=secure.sendall if secure else s.sendall
    kind,sequence,deadline,offset,hello=read();assert (kind,sequence,deadline,offset)==(1,0,0,0)
    expected=profile('matrix.cpu')+profile('matrix.multiply')+text('mode')+text('matrix.result')+text('bytes')+u64(4096)+u64(4096)
    assert hello[:-8]==expected and struct.unpack_from('<Q',hello,len(hello)-8)[0]>time.monotonic_ns()
    if mode=='wrong-ready':hello=hello[:-1]+bytes([hello[-1]^1])
    send(header(2,0,0,len(hello))+hello)
    if mode=='wrong-ready':time.sleep(.1);return
    request=bytearray();key=None
    while True:
        kind,sequence,deadline,offset,body=read()
        if kind==3:return
        if key is None:key=(sequence,deadline)
        assert key==(sequence,deadline) and offset==len(request)
        if kind==5:assert not body;break
        assert kind==4 and body;request+=body
    view=memoryview(request);position=0
    def word():
        nonlocal position
        n=struct.unpack_from('<Q',view,position)[0];position+=8;return n
    def data():
        nonlocal position
        n=word();value=view[position:position+n];position+=n;assert position<=len(view);return value
    job=word();parameters=data();assert bytes(parameters)==u64(1);assert word()==1
    metadata=data();inputs=data();assert position==len(view) and len(metadata)>0 and len(inputs)==16
    a,b=struct.unpack('<2d',inputs)
    if mode=='crash':os.kill(os.getpid(),signal.SIGSEGV)
    if mode=='timeout':time.sleep(2);return
    response=u64(2)+u64(job)+blob(struct.pack('<2d',a+2*b,3*a+4*b))
    if mode=='wrong-sequence':sequence+=1
    send(header(6,sequence,deadline,len(response))+response)
    send(header(7,sequence,deadline,0,len(response)))
    # Release terminates only this registered session, not the provider lifetime.
    while True:
        try:
            kind,*_=read()
            if kind==3:break
        except (AssertionError,ConnectionError,socket.timeout):break
    time.sleep(2)

def flags(fd,pid,expiry):
    f=policy(OPERATIONS)+['--ros-scene-model-budget','robot','2','2','32','64','4','32768',
       '--ros-scene-grant','robot','brain','model.vision','matrix','local-model',','.join(MODEL),
       '--ros-scene-model','robot','brain','matrix.cpu',PROFILE_SHA.hex(),'matrix.multiply',PROFILE_SHA.hex(),'mode','matrix.result','bytes','terminal',
       '--ros-scene-model-capacity','robot','brain','2','2','32','1','8','64','4','32768','10000000000',
       '--ros-scene-model-kernel-channel','robot','brain',str(fd),str(pid),str(os.getuid()),str(os.getgid()),
       *[v.hex() for v in IDS],DIGEST.hex(),'4096','4096','1000000000','100000','1048576',str(expiry),
       '--ros-scene-grant','robot','steady','clock.steady','steady','local','clock.create,clock.read,resource.close',
       '--ros-scene-grant','robot','data','buffer','tensor','sealed-buffer','buffer.create,buffer.write,buffer.seal,buffer.read,buffer.describe,resource.close',
       '--ros-scene-buffer-budget','robot','32','2','32','2',
       '--ros-scene-buffer','robot','data','tensor',PROFILE_SHA.hex(),'32','2','16','16','32','2','1','16','1','1','1']
    return f

def guest(width,memory,mode):
    g=BufferGuest(OPERATIONS,width,memory);g.check('call $abi',2)
    cap=g.open('brain');data_cap=g.open('data');clock_cap=g.open('steady')
    g.call('clock.create',clock_cap,[0]);clock=g.save('clock')
    g.raw('model.open',cap,profile('matrix.cpu'));session=g.save('session')
    values=struct.pack('<2d',5,6)
    g.raw('buffer.create',data_cap,layout(identity='tensor',digest=PROFILE_SHA,shape=(16,),size=16));buf=g.save('input')
    g.raw('buffer.write',data_cap,u64(0)+u64(0)+blob(values),[(0,buf)])
    g.raw('buffer.seal',data_cap,u64(0)+blob(hashlib.sha256(values).digest()),[(0,buf)])
    g.call('clock.read',clock_cap,[clock],output=768,length=1104);g.globals.add('deadline');g.globals.add('wait-until')
    g.body.append(g.word(768)+' i64.const 5000000000 i64.add global.set $wait-until')
    g.body.append(g.word(768)+f' i64.const {100000000 if mode=="timeout" else 4000000000} i64.add global.set $deadline')
    body=u64(0)+u64(1)+u64(0)+profile('matrix.multiply')+text('mode')+text('bytes')+blob(u64(1))+u64(1)
    patches=[(0,session),(16,session),(len(body),data_cap),(len(body)+8,buf)];body+=bytes(48)
    body+=u64(32)+u64(2)+u64(8192)+u64(0);patches.append((len(body)-8,'global.get $deadline'))
    g.raw('model.submit',cap,body,patches);request=g.save('request')
    expected='succeeded' if mode=='success' else 'outcome_unknown'
    g.body.append('block $done loop $waiting');g.raw('request.describe',cap,u64(0),[(0,request)])
    g.body.append(g.word(520)+f' i64.const {len(expected)} i64.eq br_if $done')
    g.call('clock.read',clock_cap,[clock],output=768,length=1104);g.check(g.word(768)+' global.get $wait-until i64.ge_u')
    g.body.append('br $waiting end end');g.exact(520,text(expected))
    if mode=='success':
        g.raw('model.take',cap,bytes(16),[(0,session),(8,request)])
        for offset,value in enumerate((17.0,39.0)):
            ptr=g.word(1024)+f' i64.const {512-16+offset*8} i64.add'+(' i32.wrap_i64' if width==32 else '')
            g.equal(ptr+' i64.load $work',int.from_bytes(struct.pack('<d',value),'little'))
        g.raw('request.release',cap,u64(0),[(0,request)])
    else:g.raw('request.release',cap,u64(0),[(0,request)],expected=11)
    return g

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--receiver-fd',type=int);p.add_argument('--mode');p.add_argument('--runtime',type=Path);p.add_argument('--output',type=Path);p.add_argument('--transport-library',type=Path,required=True);p.add_argument('--key-fd',type=int);p.add_argument('--pid-fd',type=int);a=p.parse_args()
    verify_gate();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    from attachment_peer import issue,SecurePeer
    if a.receiver_fd is not None:
        pid=struct.unpack('=I',os.read(a.pid_fd,4))[0]
        secure=SecurePeer(a.transport_library,a.receiver_fd,a.key_fd,pid,2,BINDING,176+4096)
        try:return receiver(a.receiver_fd,a.mode,secure)
        finally:secure.close()
    runtime=a.runtime.resolve();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    report=dict(complete=False,runtime_sha256=sha(runtime),cgroup=str(GATE),memory_max=68719476736,swap_max=0,cases=[],events_before=(GATE/'memory.events').read_text())
    def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    try:
        for width,memory in ((32,0),(32,1),(64,0),(64,1)):
            for mode in ('success','crash','timeout','wrong-sequence'):
                g=guest(width,memory,mode);name=f'{width}-{memory}-{mode}';wat=out/(name+'.wat');wasm=out/(name+'.wasm');wat.write_text(g.render())
                subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory',wat,'-o',wasm],check=True,timeout=30)
                for engine in ('int','aot'):
                    left,right=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
                    for s in (left,right):s.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
                    label=name+'-'+engine
                    with contextlib.ExitStack() as owned,(out/(label+'.peer.log')).open('w') as peerlog:
                        client_key,peer_key=issue(owned)
                        read_pid,write_pid=os.pipe2(os.O_CLOEXEC)
                        for descriptor in (read_pid,write_pid):owned.callback(os.close,descriptor)
                        peer=subprocess.Popen([sys.executable,__file__,'--receiver-fd',str(right.fileno()),'--mode',mode,'--transport-library',str(a.transport_library.resolve()),'--key-fd',str(peer_key),'--pid-fd',str(read_pid)],pass_fds=[right.fileno(),peer_key,read_pid],stdout=peerlog,stderr=peerlog)
                        right.close()
                        try:
                            argv=[str(runtime),'--wasm-set-main-module-name','main','--runtime-'+engine,'--runtime-compile-threads','1','--runtime-llvm-jit-cache-path','disable',*flags(left.fileno(),peer.pid,time.monotonic_ns()+20000000000),'--run',str(wasm)]
                            index=argv.index('--ros-scene-model-kernel-channel')
                            clock_fd=os.open(f'/proc/{peer.pid}/ns/time',os.O_RDONLY|os.O_CLOEXEC);owned.callback(os.close,clock_fd)
                            peer_clock,own_clock=os.fstat(clock_fd),os.stat('/proc/self/ns/time')
                            assert (peer_clock.st_dev,peer_clock.st_ino)==(own_clock.st_dev,own_clock.st_ino)
                            argv[index]='--ros-scene-model-channel';argv[index+19:index+19]=[str(client_key),str(clock_fd)]
                            result=subprocess.Popen(argv,pass_fds=[left.fileno(),client_key,clock_fd],stdout=subprocess.PIPE,stderr=subprocess.PIPE)
                            try:
                                assert os.write(write_pid,struct.pack('=I',result.pid))==4
                                stdout,stderr=result.communicate(timeout=30)
                            finally:
                                if result.poll() is None:result.kill();result.wait()
                            (out/(label+'.stdout')).write_bytes(stdout);(out/(label+'.stderr')).write_bytes(stderr)
                            report['cases'].append(dict(name=label,exit=result.returncode,assertions=len(g.checks),passed=result.returncode==0,wasm_sha256=sha(wasm),argv=argv));save()
                            assert result.returncode==0,(label,stderr.decode(errors='replace')[-3000:]);print('PASS '+label,flush=True)
                            if mode in ('success','wrong-sequence'):assert peer.poll() is None,'VM killed external provider'
                        finally:
                            left.close()
                            if peer.poll() is None:peer.terminate()
                            peer.wait(timeout=5)
        report['complete']=True
    finally:
        report['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
