#!/usr/bin/env python3
"""Two actual VM processes share an external fixture; no deployment lifecycle API.

The first VM releases its session before the second receives readiness. Both
VMs and the fixture inherit the existing 64 GiB test gate; this does not claim
separate cgroups, namespace confinement, vendor ABI or physical arbitration.
"""
import argparse,json,os,select,socket,struct,subprocess,sys,time
from pathlib import Path
from run_scene_vm import verify_gate,sha,GATE
from run_driver_attachment_vm import NAMES,IDS,DIGEST,guest,u64

def identities(index):
 values=list(IDS)
 for n in (1,2,4):values[n]=bytes([n+1,index])+bytes(14)
 return values

def worker(fds):
 verify_gate();expected=json.loads(sys.stdin.readline());channels=[socket.socket(fileno=fd) for fd in fds]
 active={s:i for i,s in enumerate(channels)};released=[];held=None;requests=[]
 while active:
  readable,_,_=select.select(list(active),[],[],20);assert readable,'client deadline'
  for channel in readable:
   index=active[channel];packet,anc,flags,_=channel.recvmsg(65712,socket.CMSG_SPACE(12))
   assert packet and not flags
   credentials=[struct.unpack('3i',data) for level,kind,data in anc if level==socket.SOL_SOCKET and kind==socket.SCM_CREDENTIALS]
   assert credentials==[(expected[index],os.getuid(),os.getgid())]
   h=list(struct.unpack('<22Q',packet[:176]))
   assert h[:2]==[0x44534f52564d5755,176] and h[3]==0 and packet[32:112]==b''.join(identities(index)) and packet[112:144]==DIGEST
   assert h[20]==len(packet)-176
   if h[2]==1:
    assert not any(h[18:])
    h[2]=2;ready=struct.pack('<22Q',*h)
    if index==1 and 0 not in released:held=(channel,ready)
    else:channel.send(ready)
   elif h[2]==3:
    assert h[18]==1 and h[19]>time.monotonic_ns() and h[21]==0 and packet[176:]==u64(7)+u64(1)+b'x'
    assert index not in requests and (index==0 or 0 in released)
    requests.append(index);h[2]=4;body=u64(1)+u64(0)+u64(3);h[20]=len(body);channel.send(struct.pack('<22Q',*h)+body)
   else:
    assert h[2]==5 and not any(h[18:]) and index in requests
    released.append(index);del active[channel];channel.close()
    if index==0 and held:held[0].send(held[1]);held=None
 assert requests==released==[0,1];print(json.dumps(dict(requests=requests,released=released,client_pids=expected)),flush=True)

def run(runtime,out,width,engine):
 deadline=time.monotonic_ns()+60000000000;label=f'w{width}-{engine}';pairs=[];processes=[];driver=None
 try:
  for index in range(2):
   pair=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
   for endpoint in pair:endpoint.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
   pairs.append(pair)
  fds=[pair[1].fileno() for pair in pairs]
  driver=subprocess.Popen([sys.executable,__file__,'--worker',*map(str,fds)],pass_fds=tuple(fds),stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
  for left,right in pairs:right.close()
  commands=[]
  for index,(left,_) in enumerate(pairs):
   wat=out/(label+f'-{index}.wat');wasm=wat.with_suffix('.wasm');wat.write_text(guest(width,1,deadline,'success').render())
   subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True)
   flags=['--ros-scene-create','robot','--ros-scene-bind','robot','main']
   for name in ['capability.open']+NAMES:flags+=['--ros-scene-interface','robot',name,'allow']
   flags+=['--ros-scene-grant','robot','arm-query','driver','arm-parameters','registered-driver',','.join(NAMES),
    '--ros-scene-driver-budget','robot','1','65536','--ros-scene-driver-kernel-channel','robot','arm-query','main',str(left.fileno()),str(driver.pid),str(os.getuid()),str(os.getgid()),
    *[value.hex() for value in identities(index)],DIGEST.hex(),'32','32','60000000000',str(deadline+1000000000),
    '--ros-scene-driver-schema','robot','arm-query','parameter.exists','64','64','8']
   command=[str(runtime),'--wasm-set-main-module-name','main',f'--runtime-{engine}','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,'--run',str(wasm)]
   commands.append(command);processes.append(subprocess.Popen(command,pass_fds=(left.fileno(),),stdout=subprocess.PIPE,stderr=subprocess.PIPE));left.close()
  driver.stdin.write((json.dumps([p.pid for p in processes])+'\n').encode());driver.stdin.close();driver.stdin=None
  for index,process in enumerate(processes):
   stdout,stderr=process.communicate(timeout=45);(out/(label+f'-{index}.stderr')).write_bytes(stderr)
   assert process.returncode==0,(label,index,stderr[-4000:])
  stdout,stderr=driver.communicate(timeout=5);(out/(label+'-driver.stderr')).write_bytes(stderr);assert driver.returncode==0,stderr
  result=dict(name=label,passed=True,commands=commands,driver=json.loads(stdout));print('PASS '+label,flush=True);return result
 finally:
  for pair in pairs:
   for endpoint in pair:endpoint.close()
  for process in processes+([driver] if driver else []):
   if process.poll() is None:process.kill();process.wait()

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--worker',nargs=2,type=int);parser.add_argument('--runtime',type=Path);parser.add_argument('--output',type=Path);a=parser.parse_args()
 if a.worker:return worker(a.worker)
 verify_gate();a.output.mkdir(parents=True,exist_ok=False)
 report=dict(complete=False,scope=__doc__,runtime_sha256=sha(a.runtime),runner_sha256=sha(__file__),cgroup=str(GATE),events_before=(GATE/'memory.events').read_text(),cases=[])
 try:
  for width in (32,64):
   for engine in ('int','aot'):report['cases'].append(run(a.runtime,a.output,width,engine))
  report['complete']=True
 finally:
  report['events_after']=(GATE/'memory.events').read_text();(a.output/'results.json').write_text(json.dumps(report,indent=2)+'\n')
if __name__=='__main__':main()
