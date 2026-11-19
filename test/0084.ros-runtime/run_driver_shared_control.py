#!/usr/bin/env python3
"""Two real VM clients contend for one original MuJoCo receiver's control gate.

All processes inherit the existing 64 GiB gate. This test provisions channels
externally, not through a VM lifecycle API. The shared physical effect is a
simulated joint; no physical emergency-stop or vendor qualification is inferred.
"""
import argparse,json,os,socket,struct,subprocess,time
from contextlib import ExitStack
from pathlib import Path
from attachment_peer import issue
from run_scene_vm import ROOT,GATE,verify_gate,sha
from run_module_security_vm import Fixture
from run_driver_attachment_vm import NAMES,IDS,DIGEST,u64
from run_driver_mujoco import profile,text,blob

def identities(index):
 values=list(IDS)
 for n in (1,2,4):values[n]=bytes([n+1,index])+bytes(14)
 return values

def guest(width,memory,deadline,index):
 g=Fixture(NAMES,width,memory);cap=g.open('joint')
 def poll(name,data=b''):
  at=g.blob(data) if data else 128;label='poll'+str(len(g.extra_functions))
  g.extra_functions.append(f'''(func ${label} (result i32) (local $s i32) (local $n i32)
   block $done loop $again {cap} i32.const {memory} i64.const {at} i64.const {len(data)} i64.const 16384 i64.const 512 i64.const 1024 call ${name}
   local.tee $s i32.const 11 i32.ne br_if $done
   local.get $n i32.const 1 i32.add local.tee $n i32.const 10000000 i32.gt_u if unreachable end br $again end end local.get $s)''')
  g.check('call $'+label)
 poll('driver.connect')
 for sequence in (1,2,3):
  epoch=2 if sequence==1 or (index==0 and sequence==2) else 3
  target=-.5 if sequence==2 else .5
  payload=b''.join(u64(v) for v in (1,sequence,1,1,1,epoch))+profile('simulation.position')+profile('simulation.radian.logical')+text('simulation.joint')+text('le64')+blob(struct.pack('<d',target))+u64(0)+u64(0)+u64(deadline)
  poll('driver.submit',u64(sequence)+u64(deadline)+blob(payload));poll('driver.result',u64(sequence))
  accepted=(index==0 and sequence==1) or (index==1 and sequence>1)
  g.equal(g.word(16384),0 if accepted else 13);g.equal(g.word(16400),sequence)
 g.request('driver.disconnect',b'',cap=cap,capacity=8);return g

def main():
 parser=argparse.ArgumentParser(description=__doc__)
 for name in ('runtime','mujoco','output'):parser.add_argument('--'+name,required=True,type=Path)
 parser.add_argument('--openssl-root',type=Path,help='Use pinned mutual TLS for both VM attachments')
 a=parser.parse_args();verify_gate();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
 report=dict(complete=False,scope=__doc__,tls=bool(a.openssl_root),runtime_sha256=sha(a.runtime),cgroup=str(GATE),events_before=(GATE/'memory.events').read_text(),cases=[],commands=[],inputs={})
 def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
 def run(label,argv):
  argv=list(map(str,argv));result=subprocess.run(argv,cwd=ROOT,capture_output=True,timeout=180)
  (out/(label+'.stdout')).write_bytes(result.stdout);(out/(label+'.stderr')).write_bytes(result.stderr)
  report['commands'].append(dict(name=label,argv=argv,exit=result.returncode));save()
  assert result.returncode==0,(label,result.stderr[-4000:]);return result.stdout
 core=ROOT/'test/0084.ros-runtime/driver_control.cpp';fixture=ROOT/'test/0084.ros-runtime/driver_shared_control_receiver.cpp';header=ROOT/'src/uwvm2/imported/ros/scene/driver_control.h'
 model=a.mujoco/'model/humanoid/humanoid.xml';binary=out/'receiver'
 allocation=ROOT/'test/0084.ros-runtime/driver_control_allocation.cpp'
 libraries=[a.openssl_root/'libssl.a',a.openssl_root/'libcrypto.a'] if a.openssl_root else []
 tls_compile=['-DUWVM_ROS_ATTACHMENT_OPENSSL','-I'+str(a.openssl_root/'include')] if a.openssl_root else []
 for path in (Path(__file__),core,fixture,header,allocation,ROOT/'src/uwvm2/imported/ros/scene/control_arbitration.h',
              ROOT/'src/uwvm2/imported/ros/scene/attachment_tls.h',model,a.mujoco.parent/'provenance.json',*libraries):report['inputs'][str(path.resolve())]=sha(path)
 flags=['/usr/bin/clang++-22','-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(ROOT/'src'),'-isystem',ROOT/'third-parties/fast_io/include']
 try:
  run('core-compile',[*flags,'-fsanitize=address,undefined',core,'-o',out/'core']);run('core',[out/'core'])
  run('allocation-compile',[*flags,allocation,'-Wl,--wrap=malloc','-Wl,--wrap=calloc','-Wl,--wrap=realloc','-o',out/'allocation']);run('allocation',[out/'allocation'])
  run('receiver-compile',[*flags,*tls_compile,'-I'+str(a.mujoco/'include'),fixture,'-L'+str(a.mujoco/'lib'),
                          '-Wl,-rpath,'+str(a.mujoco/'lib'),'-lmujoco',*libraries,'-o',binary])
  report['receiver_sha256']=sha(binary);native=json.loads(run('native',[binary,'direct',model]));report['native_reference']=native
  for width in (32,64):
   for memory in (0,1):
    for engine in ('int','aot'):
     label=f'w{width}-m{memory}-{engine}';deadline=time.monotonic_ns()+60000000000;expiry=deadline+1000000000;pairs=[];processes=[];receiver=None;commands=[]
     keys=ExitStack()
     try:
      for _ in range(2):
       pair=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
       for endpoint in pair:endpoint.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
       pairs.append(pair)
      fds=[right.fileno() for _,right in pairs]
      identities_by_client=[issue(keys) for _ in pairs] if a.openssl_root else []
      receiver_fds=fds+[receiver_key for _,receiver_key in identities_by_client]
      receiver=subprocess.Popen([str(binary),'shared',str(model),*map(str,receiver_fds)],pass_fds=tuple(receiver_fds),
                                stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
      for _,right in pairs:right.close()
      if a.openssl_root:
       clock_fd=os.open(f'/proc/{receiver.pid}/ns/time',os.O_RDONLY|os.O_CLOEXEC);keys.callback(os.close,clock_fd)
      for index,(left,_) in enumerate(pairs):
       wat=out/(label+f'-{index}.wat');wasm=wat.with_suffix('.wasm');wat.write_text(guest(width,memory,deadline,index).render())
       run(label+f'-{index}-compile',['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',wat,'-o',wasm])
       flags=['--ros-scene-create','robot','--ros-scene-bind','robot','main']
       for name in ['capability.open']+NAMES:flags+=['--ros-scene-interface','robot',name,'allow']
       secure_fds=(identities_by_client[index][0],clock_fd) if a.openssl_root else ()
       flags+=['--ros-scene-grant','robot','joint','driver','humanoid-abdomen-z','registered-driver',','.join(NAMES),
        '--ros-scene-driver-budget','robot','1','1048576' if a.openssl_root else '65536',
        '--ros-scene-driver-channel' if a.openssl_root else '--ros-scene-driver-kernel-channel',
        'robot','joint','main',str(left.fileno()),str(receiver.pid),str(os.getuid()),str(os.getgid()),*[v.hex() for v in identities(index)],DIGEST.hex(),'4096','256','60000000000',str(expiry),*map(str,secure_fds),
        '--ros-scene-driver-schema','robot','joint','device.submit','128','128','16']
       command=[str(a.runtime),'--wasm-set-main-module-name','main',f'--runtime-{engine}','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,'--run',str(wasm)]
       commands.append(command);processes.append(subprocess.Popen(command,pass_fds=(left.fileno(),*secure_fds),stdout=subprocess.PIPE,stderr=subprocess.PIPE));left.close()
      receiver.stdin.write(f'{processes[0].pid} {processes[1].pid} {expiry}\n'.encode());receiver.stdin.close();receiver.stdin=None
      for index,process in enumerate(processes):
       stdout,stderr=process.communicate(timeout=45);(out/(label+f'-{index}.stderr')).write_bytes(stderr);assert process.returncode==0,(label,index,stderr[-4000:])
      stdout,stderr=receiver.communicate(timeout=5);(out/(label+'-receiver.stderr')).write_bytes(stderr);assert receiver.returncode==0,(label,stderr)
      observed=json.loads(stdout);control=json.loads(stderr);assert observed==native and control==dict(denied=3,handover_epoch=3,stop_reserved=True,foreign_deployment_refused=True)
      report['cases'].append(dict(name=label,passed=True,commands=commands,result=observed,control=control,client_pids=[p.pid for p in processes]));save();print('PASS '+label,flush=True)
     finally:
      keys.close()
      for pair in pairs:
       for endpoint in pair:endpoint.close()
      for process in processes+([receiver] if receiver else []):
       if process.poll() is None:process.kill();process.wait()
  report['complete']=True
 finally:
  report['events_after']=(GATE/'memory.events').read_text();report['inputs_unchanged']=all(sha(path)==digest for path,digest in report['inputs'].items());save()
if __name__=='__main__':main()
