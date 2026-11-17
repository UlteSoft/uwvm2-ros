#!/usr/bin/env python3
"""Actual ROSv2-only external driver IPC; run in the existing 64 GiB cgroup."""
import argparse,json,os,resource,signal,socket,struct,subprocess,sys,time
from pathlib import Path
from run_scene_vm import verify_gate,sha,GATE
from run_module_security_vm import Fixture
NAMES=['driver.'+s for s in ('connect','submit','result','describe','disconnect')]
IDS=[bytes([n])+bytes(15) for n in range(1,6)]
DIGEST=bytes(range(1,33));D='$'
def u64(v):return struct.pack('<Q',v)
def worker(fd,mode):
 verify_gate();expected=int(sys.stdin.readline());s=socket.socket(fileno=fd);s.settimeout(20);released=False
 while True:
  packet,anc,flags,_=s.recvmsg(65712,socket.CMSG_SPACE(12))
  if not packet:break
  assert not flags
  creds=[struct.unpack('3i',data) for level,kind,data in anc if level==socket.SOL_SOCKET and kind==socket.SCM_CREDENTIALS]
  assert creds==[(expected,os.getuid(),os.getgid())],creds
  h=list(struct.unpack('<22Q',packet[:176]));assert h[:2]==[0x44534f52564d5755,176] and h[3]==0
  assert packet[32:112]==b''.join(IDS) and packet[112:144]==DIGEST and h[20]==len(packet)-176
  if h[2]==1:
   assert not any(h[18:]) and len(packet)==176
   h[2]=2;s.send(struct.pack('<22Q',*h))
  elif h[2]==3:
   assert h[18]==1 and h[19]>time.monotonic_ns() and h[21]==0 and packet[176:]==u64(7)+u64(1)+b'x'
   if mode=='crash':
    resource.setrlimit(resource.RLIMIT_CORE,(0,0));os.kill(os.getpid(),signal.SIGSEGV)
   h[2]=4;body=u64(2 if mode=='schema' else 1)+u64(0)+u64(3)
   if mode=='identity':h[12]+=1
   if mode=='sequence':h[18]+=1
   if mode=='deadline':h[19]+=1
   if mode=='oversized':body+=bytes(16)
   h[20]=len(body);s.send(b'x' if mode=='truncated' else struct.pack('<22Q',*h)+body)
  elif h[2]==5:released=True;break
  else:raise AssertionError(h)
 s.close()
 if mode=='success':assert released

def guest(width,memory,deadline,mode):
 g=Fixture(NAMES,width,memory);g.open_import=True;cap=g.open('arm-query')
 def expression(name,at=128,size=0,capacity=64,output=16384,length=1024):
  return f'{cap} i32.const {memory} i64.const {at} i64.const {size} i64.const {output} i64.const {capacity} i64.const {length} call {D}{name}'
 def poll(name,at=128,size=0,expected=0):
  label='poll'+str(len(g.extra_functions));g.extra_functions.append(f'''(func {D}{label} (result i32) (local $s i32) (local $n i32)
   block $done loop $again
   {expression(name,at,size)} local.tee $s i32.const 11 i32.ne br_if $done
   local.get $n i32.const 1 i32.add local.tee $n i32.const 10000000 i32.gt_u if unreachable end
   br $again end end local.get $s)''')
  if expected=='transport':
   check=label+'check';g.extra_functions.append('(func $'+check+' (param $s i32) local.get $s i32.const 5 i32.eq local.get $s i32.const 13 i32.eq i32.or i32.eqz if unreachable end)')
   g.body.append('call $'+label+' call $'+check)
  else:g.check('call $'+label,expected)
 if mode=='denied':g.check(expression('driver.connect'),13);return g
 g.check(expression('driver.connect',capacity=0),90);g.equal(g.word(1024),8)
 g.check(expression('driver.connect',output=65535),22)
 g.request('driver.describe',b'',cap=cap,capacity=32);g.equal(g.word(16384),0)
 poll('driver.connect');g.request('driver.describe',b'',cap=cap,capacity=32);g.equal(g.word(16384),2)
 bad=g.blob(u64(1)+u64(deadline)+u64(1)+b'x');g.check(expression('driver.submit',bad,25),22)
 payload=u64(7)+u64(1)+b'x';at=g.blob(u64(1)+u64(deadline)+u64(len(payload))+payload)
 g.check(expression('driver.submit',at,24+len(payload),capacity=7),90)
 poll('driver.submit',at,24+len(payload));g.check(expression('driver.submit',at,24+len(payload)))
 key=g.blob(u64(1));g.check(expression('driver.result',key,8,capacity=47),90)
 poll('driver.result',key,8,0 if mode=='success' else 22 if mode=='schema' else 'transport' if mode=='crash' else 13)
 if mode=='success':
  for address,value in [(1024,40),(16384,0),(16392,24),(16400,1),(16408,0),(16416,3)]:g.equal(g.word(address),value)
 else:g.request('driver.describe',b'',cap=cap,capacity=32);g.equal(g.word(16384),6);g.equal(g.word(16408),1)
 g.request('driver.disconnect',b'',cap=cap,capacity=8)
 g.request('driver.describe',b'',cap=cap,capacity=32);g.equal(g.word(16384),5)
 return g

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--worker',type=int);ap.add_argument('--worker-mode',default='success');ap.add_argument('--runtime',type=Path);ap.add_argument('--output',type=Path)
 ap.add_argument('--fp-controls',action='store_true',help='Check actual Wasm rounding, subnormal and NaN behavior after every driver call sequence')
 a=ap.parse_args()
 if a.worker is not None:worker(a.worker,a.worker_mode);return
 if a.fp_controls:
  from run_module_security_vm import ObservationFPFixture
  global Fixture
  Fixture=ObservationFPFixture
 verify_gate();runtime=a.runtime.resolve();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
 result=dict(complete=False,runtime=str(runtime),runtime_sha256=sha(runtime),fp_controls=a.fp_controls,cgroup=str(GATE),memory_max=68719476736,swap_max=0,events_before=(GATE/'memory.events').read_text(),cases=[])
 def save():(out/'results.json').write_text(json.dumps(result,indent=2)+'\n')
 try:
  for width,memory in [(32,0),(32,1),(64,0),(64,1)]:
   for engine in ['int','aot']:
    for mode in ['success','denied','schema','identity','sequence','deadline','truncated','oversized','crash']:
     label=f'w{width}-m{memory}-{engine}-{mode}';deadline=time.monotonic_ns()+60000000000
     g=guest(width,memory,deadline,mode);wat=out/(label+'.wat');wasm=out/(label+'.wasm');wat.write_text(g.render())
     subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True)
     left,right=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
     for sock in (left,right):sock.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
     peer=subprocess.Popen([sys.executable,__file__,'--worker',str(right.fileno()),'--worker-mode',mode],pass_fds=(right.fileno(),),stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE);right.close()
     flags=['--ros-scene-create','robot','--ros-scene-bind','robot','main']
     for name in ['capability.open']+NAMES:flags+=['--ros-scene-interface','robot',name,'allow']
     actions=NAMES if mode!='denied' else ['driver.describe']
     flags+=['--ros-scene-grant','robot','arm-query','driver','arm-parameters','registered-driver',','.join(actions),
      '--ros-scene-driver-budget','robot','1','65536','--ros-scene-driver-kernel-channel','robot','arm-query','main',str(left.fileno()),str(peer.pid),str(os.getuid()),str(os.getgid()),
      *[v.hex() for v in IDS],DIGEST.hex(),'32','32','60000000000',str(deadline+1000000000),
      '--ros-scene-driver-schema','robot','arm-query','parameter.exists','64','64','8']
     command=[str(runtime),'--wasm-set-main-module-name','main',f'--runtime-{engine}','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,'--run',str(wasm)]
     vm=subprocess.Popen(command,pass_fds=(left.fileno(),),stdout=subprocess.PIPE,stderr=subprocess.PIPE);left.close()
     peer.stdin.write((str(vm.pid)+'\n').encode());peer.stdin.flush();peer.stdin.close();peer.stdin=None
     try:stdout,stderr=vm.communicate(timeout=45);ps,pe=peer.communicate(timeout=5)
     finally:
      for process in [vm,peer]:
       if process.poll() is None:process.kill();process.wait()
     (out/(label+'.stderr')).write_bytes(stderr);(out/(label+'.peer.stderr')).write_bytes(pe)
     passed=vm.returncode==0 and peer.returncode==(-signal.SIGSEGV if mode=='crash' else 0)
     result['cases'].append(dict(name=label,vm=vm.returncode,peer=peer.returncode,passed=passed,argv=command,guest_sha256=sha(wasm)));save()
     if not passed:raise RuntimeError(label+'\n'+stderr.decode(errors='replace')[-4000:]+'\n'+pe.decode(errors='replace'))
     print('PASS '+label,flush=True)
  result['complete']=True
 finally:result['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
