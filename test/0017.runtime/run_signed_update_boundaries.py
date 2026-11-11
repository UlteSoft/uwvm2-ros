#!/usr/bin/env python3
"""Real Linux VM signed update channel, independent of all debugger levels.

The deterministic Ed25519 key is a disposable fixture key. No real deployment,
network service, guest dump, automatic recovery or unsigned activation occurs.
"""
import argparse,hashlib,json,os,resource,shlex,socket,struct,subprocess,time
from pathlib import Path
from cryptography.hazmat.primitives.asymmetric.ed25519 import Ed25519PrivateKey
from cryptography.hazmat.primitives.serialization import Encoding,PublicFormat

def sha(value):return hashlib.sha256(value).digest()
def module(changed):
    # _start: () -> (); value: () -> i32. Publication is verified by an
    # observable value change, not solely by the installed image digest.
    return bytes.fromhex('0061736d010000000108026000006000017f0303020001050401010101071202065f737461727400000576616c756500010a090202000b040041')+bytes([9 if changed else 7])+b'\x0b'
def exact(channel,count):
    data=b''
    while len(data)<count:
        part=channel.recv(count-len(data))
        if not part:raise EOFError('authenticated update endpoint closed')
        data+=part
    return data
def request(channel,operation,payload=b''):
    channel.sendall(b'UUP1'+struct.pack('<II',operation,len(payload))+payload)
    header=exact(channel,8)
    if header[:4]!=b'UUR1':raise ValueError('invalid update response magic')
    size=struct.unpack('<I',header[4:])[0]
    if not 296<=size<=1048576:raise ValueError('unbounded update response')
    data=exact(channel,size);status,state,active,connected=struct.unpack('<IIII',data[:16]);epoch,committed,sequence=struct.unpack('<QQQ',data[16:40])
    result=dict(status=status,state=state,active=active,connected=connected,epoch=epoch,committed=committed,sequence=sequence)
    for i,name in enumerate(('nonce','installed','prepared','ready','transaction','contract','scope','issuer')):result[name]=data[40+i*32:72+i*32].hex()
    result['result']=data[296:].hex()
    return result

def call_export(channel,payload):
    # The authenticated launch receipt opens admission before the initial
    # _start necessarily releases its execution pin. Busy is an explicit
    # transient result; authorization or fault refusals must never be retried.
    deadline=time.monotonic()+5
    while True:
        reply=request(channel,5,payload)
        if reply['status']!=8 or not reply['active'] or time.monotonic()>=deadline:return reply
        time.sleep(.002)

def permit(key,facts,operation,target,transaction,*,prepared=None,ready=None,declaration=None):
    zero=bytes(32);launch=operation==4;decision=operation in (2,3)
    fields=[bytes.fromhex(facts['issuer']),bytes.fromhex(facts['nonce']),transaction,bytes.fromhex(facts['installed']),target,bytes.fromhex(facts['contract']),prepared or zero,ready or zero,bytes.fromhex(facts['scope']),zero if launch else sha(struct.pack('<Q',1) if declaration is None else declaration)]
    raw=b'uwvm.local-update.activation.v1\0'.ljust(32,b'\0')+bytes([operation])+bytes(7)+b''.join(fields)+struct.pack('<QQQQQQ',0 if launch else facts['epoch'],1 if launch else facts['epoch']+1,1,0,int(time.time())+300,1 if launch else facts['sequence']+1 if decision else 0)
    if len(raw)!=408:raise ValueError('permit layout mismatch')
    return raw+key.sign(raw)

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--vm',required=True,type=Path);ap.add_argument('--out',required=True,type=Path)
    ap.add_argument('--engine',choices=('aot','int'),default='aot')
    ap.add_argument('--runner',default='',help='Optional argv prefix, parsed with shlex (for example the actual QEMU and target sysroot).')
    a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=False);resource.setrlimit(resource.RLIMIT_CORE,(0,0));runner=shlex.split(a.runner)
    key=Ed25519PrivateKey.from_private_bytes(bytes(range(1,33)));public=key.public_key().public_bytes(Encoding.Raw,PublicFormat.Raw)
    initial=module(False);target=module(True);image=a.out/'initial.wasm';image.write_bytes(initial);(a.out/'target.wasm').write_bytes(target);rows=[];commands=[];processes=[]
    def check(level,name,passed,detail):
        rows.append(dict(level=level,name=name,passed=bool(passed),detail=detail));print(level,name,'PASS' if passed else 'FAIL',flush=True)
    for level in ('full','read-only','limited','none'):
        host,guest=socket.socketpair(socket.AF_UNIX,socket.SOCK_STREAM)
        with host,guest:
            guest.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1);host.settimeout(20)
            command=[*runner,str(a.vm.resolve()),'--runtime-'+a.engine,'--debug-permissions',level,'--runtime-compile-threads','0',*(['--runtime-llvm-jit-cache-path','disable'] if a.engine=='aot' else []),
                '--update-control-fd',str(guest.fileno()),'--update-public-key',public.hex(),'--update-contract',sha(b'test approved contract').hex(),'--update-scope',sha(b'test isolated deployment scope').hex(),'--update-call-export','value',
                '--wasip1-single-create','main','--wasip1-single-enable','main','--wasm-set-main-module-name','main','-Wstart','0','--run',str(image.resolve())];commands.append(command)
            child=subprocess.Popen(command,pass_fds=(guest.fileno(),),stdout=subprocess.PIPE,stderr=subprocess.PIPE);guest.close()
            try:
                state=request(host,0);check(level,'signed launch required',state['status']==0 and state['state']==8 and not state['active'] and state['epoch']==0,state)
                reply=request(host,5,struct.pack('<III',1,0,4));check(level,'business blocked before launch',reply['status']==2 and not reply['active'],reply)
                launch=permit(key,state,4,bytes.fromhex(state['installed']),sha(b'launch'))
                invalid=launch[:-1]+bytes([launch[-1]^1]);reply=request(host,9,invalid);check(level,'tampered signature denied',reply['status']==4 and not reply['active'],reply)
                reply=request(host,9,launch);check(level,'authenticated launch',reply['status']==0 and reply['active'] and reply['epoch']==1,reply)
                current=request(host,0);check(level,'initial measured image',current['installed']==sha(initial).hex(),current)
                reply=call_export(host,struct.pack('<III',1,0,4));check(level,'initial approved export',reply['status']==0 and reply['result']=='07000000',reply)
                tx=sha(b'update');prepare=permit(key,current,1,sha(target),tx)
                invalid=prepare[:-1]+bytes([prepare[-1]^1]);reply=request(host,1,invalid+struct.pack('<IQ',1,1)+target);check(level,'unsigned replacement denied',reply['status']==4 and reply['active'],reply)
                reply=request(host,1,prepare+struct.pack('<IQ',1,1)+target);check(level,'signed prepare',reply['status']==0 and reply['state']==3,reply)
                ready=request(host,2);check(level,'ready measurement',ready['status']==0 and ready['state']==4 and int(ready['ready'],16)!=0,ready)
                reply=request(host,4);check(level,'ready cannot cancel privately',reply['status']==2 and reply['state']==4,reply)
                commit=permit(key,current,2,sha(target),tx,prepared=bytes.fromhex(ready['prepared']),ready=bytes.fromhex(ready['ready']))
                activated=request(host,3,commit);check(level,'signed commit activated',activated['status']==0 and activated['active'] and activated['epoch']==2 and activated['installed']==sha(target).hex(),activated)
                reply=call_export(host,struct.pack('<III',1,0,4));check(level,'updated approved export',reply['status']==0 and reply['active'] and reply['result']=='09000000',reply)
                reply=request(host,6);check(level,'authorized normal stop',reply['status']==0,reply)
                output,error=child.communicate(timeout=20);check(level,'process exit',child.returncode==0,dict(exit=child.returncode))
            except (OSError,ValueError,EOFError,subprocess.TimeoutExpired) as error:check(level,'transport/operation',False,repr(error))
            finally:
                if child.poll() is None:host.close();child.terminate()
                try:output,error=child.communicate(timeout=20)
                except subprocess.TimeoutExpired:child.kill();output,error=child.communicate(timeout=10)
                (a.out/(level+'.stderr')).write_bytes(error)
                processes.append(dict(level=level,exit=child.returncode,stdout=output.decode(errors='replace'),stderr=error.decode(errors='replace')))
    report=dict(binary=str(a.vm.resolve()),binary_sha256=sha(a.vm.read_bytes()).hex(),engine=a.engine,runner=runner,commands=commands,processes=processes,passed=sum(r['passed'] for r in rows),total=len(rows),expected=56,cases=rows,
        complete=len(rows)==56 and all(r['passed'] for r in rows),
        scope='actual VM signed update request matrix; runner prefix is recorded without claiming native target hardware; unsupported launch is a failure, not a signed update pass; no faulted-state restoration claim')
    (a.out/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('passed','total','binary_sha256')}));return 0 if all(r['passed'] for r in rows) else 1
if __name__=='__main__':raise SystemExit(main())
