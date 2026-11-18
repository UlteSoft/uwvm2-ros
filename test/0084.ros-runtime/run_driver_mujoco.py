#!/usr/bin/env python3
"""Actual registered-driver VM traffic to a separate official MuJoCo process.

Qualified scope: fixed humanoid joint, canonical device.submit shape, exact
source credentials, physical range refusal and a crash after mj_step. The
receiver enforces semantic limits. This fixture is not a deployer or a vendor
adapter. Optional --namespace qualifies the recorded sealed closure, explicit
namespace topology and seven attempted escapes under stacked syscall filters.
Ordinary mode has process separation only. Neither mode qualifies hardware
emergency stop. All processes inherit the existing 64 GiB zero-swap gate.
"""
import argparse,hashlib,json,os,re,signal,socket,struct,subprocess,time
from pathlib import Path
from contextlib import ExitStack
from run_scene_vm import ROOT,GATE,verify_gate,sha
from run_module_security_vm import Fixture
from run_driver_attachment_vm import NAMES,IDS,DIGEST,u64

def text(value):return blob(value.encode())
def blob(value):return u64(len(value))+value
def profile(name):return text(name)+blob(hashlib.sha256(name.encode()).digest())

def guest(width,memory,deadline,mode,count):
 g=Fixture(NAMES,width,memory);cap=g.open('joint');g.globals.add('sequence')
 def expression(name,at=128,size=0):return f'{cap} i32.const {memory} i64.const {at} i64.const {size} i64.const 16384 i64.const 512 i64.const 1024 call ${name}'
 def poll(name,at=128,size=0,crash=False):
  label='poll'+str(len(g.extra_functions));g.extra_functions.append(f'''(func ${label} (result i32) (local $s i32) (local $n i32)
   block $done loop $again {expression(name,at,size)} local.tee $s i32.const 11 i32.ne br_if $done
   local.get $n i32.const 1 i32.add local.tee $n i32.const 10000000 i32.gt_u if unreachable end br $again end end local.get $s)''')
  if crash:
   check=label+'check';g.extra_functions.append('(func $'+check+' (param $s i32) local.get $s i32.const 5 i32.eq local.get $s i32.const 13 i32.eq i32.or i32.eqz if unreachable end)');g.body.append('call $'+label+' call $'+check)
  else:g.check('call $'+label)
 poll('driver.connect')
 prefix=b''.join(u64(x) for x in (1,1,1,1,1,1))+profile('simulation.position')+profile('simulation.radian.logical')+text('simulation.joint')+text('le64')
 payload=prefix+blob(struct.pack('<d',2 if mode=='range' else .5))+u64(0)+u64(0)+u64(deadline)
 request=g.blob(u64(1)+u64(deadline)+blob(payload));key=g.blob(u64(1))
 g.body+=['i64.const 1 global.set $sequence','loop $cycle']
 g.write(request,'global.get $sequence');g.write(request+32,'global.get $sequence');g.write(key,'global.get $sequence')
 if mode=='success':g.write(request+24+len(prefix)+8,'global.get $sequence i64.const 1 i64.and i64.eqz if (result i64) i64.const -4620693217682128896 else i64.const 4602678819172646912 end')
 poll('driver.submit',request,24+len(payload));poll('driver.result',key,8,mode=='crash')
 if mode!='crash':g.equal(g.word(16384),13 if mode=='range' else 0);g.check(g.word(16400)+' global.get $sequence i64.ne')
 g.body+=['global.get $sequence i64.const 1 i64.add global.set $sequence',f'global.get $sequence i64.const {count} i64.le_u br_if $cycle','end']
 if mode=='crash':g.request('driver.describe',b'',cap=cap,capacity=32);g.equal(g.word(16384),6);g.equal(g.word(16408),1)
 g.request('driver.disconnect',b'',cap=cap,capacity=8);return g

def main(argv=None, *, compile_guest=None, guest_name="WAT", guest_inputs=(), guest_widths=(32,64)):
 parser=argparse.ArgumentParser(description=__doc__)
 for name in ('runtime','source-root','mujoco','output'):parser.add_argument('--'+name,required=True,type=Path)
 parser.add_argument('--namespace',action='store_true',help='Use test-only external mount/user/network/IPC/UTS namespaces and syscall filters')
 parser.add_argument('--openssl-root',type=Path,help='Bundled TLS 1.3 build prefix for production VM driver sessions')
 parser.add_argument('--audit-calls',action='store_true',help='Check four bounded host-result records and compare physics with direct native execution')
 a=parser.parse_args(argv)
 if bool(a.openssl_root)==bool(a.namespace):parser.error('select --openssl-root for production TLS, or --namespace for the historical fixture-only VM')
 if not guest_widths or any(w not in (32,64) for w in guest_widths) or len(set(guest_widths))!=len(guest_widths):
  parser.error('guest widths must be unique and drawn from wasm32 and wasm64')
 if compile_guest is None and guest_widths!=(32,64):parser.error('the WAT reference covers both widths')
 verify_gate();out=a.output;out.mkdir(parents=True,exist_ok=False)
 report=dict(complete=False,scope=__doc__,guest_language=guest_name,namespace_fixture=a.namespace,tls=bool(a.openssl_root),runtime_sha256=sha(a.runtime),cgroup=str(GATE),events_before=(GATE/'memory.events').read_text(),cases=[],inputs={},commands=[])
 def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
 def run(label,cmd,env=None):
  p=subprocess.run(list(map(str,cmd)),capture_output=True,env=env,timeout=180);(out/(label+'.stdout')).write_bytes(p.stdout);(out/(label+'.stderr')).write_bytes(p.stderr)
  report['commands'].append(dict(name=label,argv=list(map(str,cmd)),exit=p.returncode));save();assert p.returncode==0,(label,p.stderr[-3000:]);return p.stdout
 fixture=ROOT/'test/0084.ros-runtime/driver_mujoco_receiver.cpp';binary=out/'receiver';model=a.mujoco/'model/humanoid/humanoid.xml'
 for path in (Path(__file__),fixture,model,a.mujoco.parent/'provenance.json',*guest_inputs):report['inputs'][str(path.resolve())]=sha(path)
 try:
  compile_options=[]
  libraries=[]
  if a.openssl_root:
   from attachment_peer import issue
   compile_options=['-DUWVM_ROS_ATTACHMENT_OPENSSL','-I'+str(a.openssl_root/'include')]
   libraries=[a.openssl_root/'libssl.a',a.openssl_root/'libcrypto.a','-ldl']
   for path in libraries[:2]:report['inputs'][str(path.resolve())]=sha(path)
  run('compile',['/usr/bin/clang++-22','-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread',*compile_options,'-I'+str(a.source_root/'src'),'-isystem',a.source_root/'third-parties/fast_io/include','-I'+str(a.mujoco/'include'),fixture,'-L'+str(a.mujoco/'lib'),'-Wl,-rpath,'+str(a.mujoco/'lib'),'-lmujoco',*libraries,'-o',binary])
  report['inputs'][str(binary.resolve())]=sha(binary)
  native=json.loads(run('native',[binary,'0','direct',model,'128']));report['native_reference']=native
  if a.namespace:
   from run_driver_namespace import launch
   report['inputs'][str(Path(__file__).with_name('run_driver_namespace.py').resolve())]=sha(Path(__file__).with_name('run_driver_namespace.py'))
   closure=[(binary,'/worker/receiver'),(model,'/fixture/model.xml')]
   for line in run('libraries',['/usr/bin/ldd',binary]).decode().splitlines():
    match=re.search(r'(?:=> )?(/[^ ]+)',line)
    if match:
     path=Path(match.group(1));closure.append((path,'/lib/'+path.name if 'mujoco' in path.name else str(path)))
   assert any('libmujoco' in destination for _,destination in closure)
  layouts=((32,0),(32,1),(64,0),(64,1)) if compile_guest is None else tuple((width,0) for width in guest_widths)
  report["layouts"]=[dict(address_bits=w,memory_index=m) for w,m in layouts]
  for width,memory in layouts:
   for engine in ('int','aot'):
    for mode in ('success','range','crash'):
     label=f'{mode}-{width}-{memory}-{engine}';count=128 if mode=='success' else 1;deadline=time.monotonic_ns()+60000000000
     wasm=out/(label+'.wasm')
     if compile_guest is None:
      wat=wasm.with_suffix('.wat');wat.write_text(guest(width,memory,deadline,mode,count).render())
      run(label+'-compile',['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',wat,'-o',wasm])
      guest_flags=[]
     else:
      guest_flags=compile_guest(wasm,width,deadline,mode,count,run)
     report['inputs'][str(wasm.resolve())]=sha(wasm)
     left,right=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
     for endpoint in (left,right):endpoint.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
     keys=ExitStack()
     vm_key,peer_key=issue(keys) if a.openssl_root else (-1,-1)
     if a.namespace:
      filters=run(label+'-filter',[binary,'--filter',str(right.fileno())]);peer,peer_pid,confinement=launch(binary,model,mode,count,right.fileno(),closure,filters)
     else:
      peer=subprocess.Popen([str(binary),str(right.fileno()),mode,str(model),str(count),'--identity-fd',str(peer_key)],pass_fds=(right.fileno(),peer_key),stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE);peer_pid=peer.pid;confinement=None
     right.close();vm=None
     try:
      if a.openssl_root:
       clock_fd=os.open(f'/proc/{peer_pid}/ns/time',os.O_RDONLY|os.O_CLOEXEC);keys.callback(os.close,clock_fd)
       peer_clock,own_clock=os.fstat(clock_fd),os.stat('/proc/self/ns/time')
       assert (peer_clock.st_dev,peer_clock.st_ino)==(own_clock.st_dev,own_clock.st_ino)
      flags=['--ros-scene-create','robot','--ros-scene-bind','robot','main']
      if a.audit_calls:flags+=['--ros-scene-call-audit','single','main','all','4','8192','1000000','4']
      for name in ['capability.open']+NAMES:flags+=['--ros-scene-interface','robot',name,'allow']
      flags+=['--ros-scene-grant','robot','joint','driver','humanoid-abdomen-z','registered-driver',','.join(NAMES),'--ros-scene-driver-budget','robot','1','1048576' if a.openssl_root else '65536',
       '--ros-scene-driver-channel' if a.openssl_root else '--ros-scene-driver-kernel-channel','robot','joint','main',str(left.fileno()),str(peer_pid),str(os.getuid()),str(os.getgid()),*[x.hex() for x in IDS],DIGEST.hex(),'4096','256','60000000000',str(deadline+1000000000),*([str(vm_key),str(clock_fd)] if a.openssl_root else []),
       '--ros-scene-driver-schema','robot','joint','device.submit','128','128','16']
      cmd=[str(a.runtime),*guest_flags,'--wasm-set-main-module-name','main',f'--runtime-{engine}','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,'--run',str(wasm)]
      vm=subprocess.Popen(cmd,pass_fds=((left.fileno(),vm_key,clock_fd) if a.openssl_root else (left.fileno(),)),stdout=subprocess.PIPE,stderr=subprocess.PIPE);left.close();peer.stdin.write((str(vm.pid)+'\n').encode());peer.stdin.close();peer.stdin=None
      stdout,stderr=vm.communicate(timeout=45);ps,pe=peer.communicate(timeout=5)
      if confinement:pe=confinement['ready_line'].encode()+pe
      (out/(label+'.vm.stderr')).write_bytes(stderr);(out/(label+'.peer.stderr')).write_bytes(pe)
      expected_exit=128+signal.SIGSEGV if a.namespace else -signal.SIGSEGV
      assert vm.returncode==0 and peer.returncode==(expected_exit if mode=='crash' else 0),(label,peer.returncode,stderr[-3000:],pe[-3000:])
      audit_rows=[json.loads(line) for line in stderr.splitlines() if line.startswith(b'{"ros":"uwvm_ros_v2",')]
      if a.audit_calls:
       assert [row["sequence"] for row in audit_rows]==[1,2,3,4],(label,audit_rows)
       assert all(row["caller_hex"]=="main".encode().hex() and row["group_hex"]=="robot".encode().hex()
                  and row["phase"]=="return" and row["generation"]>0 for row in audit_rows)
       assert any(row["function"]=="capability.open" and row["result"]==0 for row in audit_rows)
      else:assert not audit_rows
      observed=json.loads(ps) if ps else None;rows=[json.loads(line) for line in pe.splitlines() if line.startswith(b'{')];effects=[row for row in rows if 'sequence' in row]
      if a.namespace:assert [row for row in rows if row.get('kind')=='confined']==[dict(kind='confined',escape_checks=7)]
      assert len(effects)==count and effects[-1]['steps']==(0 if mode=='range' else count)
      if mode=='success':assert all(observed[k]==native[k] for k in ('steps','time','position'))
      report['cases'].append(dict(name=label,passed=True,argv=cmd,vm_exit=vm.returncode,driver_exit=peer.returncode,result=observed,effects=len(effects),confinement=confinement,host_audit=audit_rows));save();print('PASS '+label,flush=True)
     finally:
      keys.close();left.close()
      for process in ([vm] if vm else [])+[peer]:
       if process.poll() is None:process.kill();process.wait()
  report['complete']=True
 finally:report['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
