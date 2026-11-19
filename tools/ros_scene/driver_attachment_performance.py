#!/usr/bin/env python3
"""Matched native/VM registered driver round trips; serial cgroup-gated trials.

Receiver timestamp interval excludes process startup/VM compilation. The native
reference uses the same dispatcher, schemas, bounds and separate process. This
is a 17-byte request / 24-byte response, not a direct function-call benchmark.
"""
import argparse,contextlib,fcntl,json,math,os,socket,struct,subprocess,sys,time,statistics
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'test/0084.ros-runtime'))
from run_scene_vm import verify_gate,sha,GATE
from run_module_security_vm import Fixture
from run_driver_attachment_vm import NAMES,IDS,DIGEST,u64
from performance_build_policy import vm_policy,compile_policy
from hardware_sampling import vtune_periods
VTUNE='/opt/intel/oneapi/vtune/latest/bin64/vtune'
D='$'
def fixture(count,deadline):
 g=Fixture(NAMES);g.open_import=True;cap=g.open('arm-query');g.globals.add('sequence')
 def expr(name,at,size):return f'{cap} i32.const 0 i64.const {at} i64.const {size} i64.const 16384 i64.const 64 i64.const 1024 call {D}{name}'
 def poll(name,at,size):
  label='poll'+str(len(g.extra_functions));g.extra_functions.append(f'''(func {D}{label} (local $s i32)
   block $done loop $again {expr(name,at,size)} local.tee $s i32.const 11 i32.ne br_if $done br $again end end
   local.get $s if unreachable end)''');return 'call $'+label
 g.body.append(poll('driver.connect',128,0))
 payload=u64(7)+u64(1)+b'x';request=g.blob(u64(0)+u64(deadline)+u64(len(payload))+payload);key=g.blob(u64(0))
 g.body+=['i64.const 1 global.set $sequence','loop $roundtrip']
 g.write(request,'global.get $sequence');g.write(key,'global.get $sequence')
 g.body.append(poll('driver.submit',request,41));g.body.append(poll('driver.result',key,8));g.equal(g.word(16400),1)
 g.body+=['global.get $sequence i64.const 1 i64.add global.set $sequence',f'global.get $sequence i64.const {count} i64.le_u br_if $roundtrip','end']
 g.body.append(poll('driver.disconnect',128,0));return g

def ephemeral_identity(own,peer,descriptors):
 from cryptography.hazmat.primitives.serialization import Encoding,NoEncryption,PrivateFormat,PublicFormat
 fd=os.memfd_create('wros-test-identity',os.MFD_CLOEXEC|os.MFD_ALLOW_SEALING)
 descriptors.callback(os.close,fd)
 seed=own.private_bytes(Encoding.Raw,PrivateFormat.Raw,NoEncryption())
 public=peer.public_key().public_bytes(Encoding.Raw,PublicFormat.Raw)
 assert os.write(fd,seed+public)==64
 fcntl.fcntl(fd,fcntl.F_ADD_SEALS,fcntl.F_SEAL_WRITE|fcntl.F_SEAL_GROW|fcntl.F_SEAL_SHRINK|fcntl.F_SEAL_SEAL)
 return fd

def trial(a,kind,label):
 with contextlib.ExitStack() as descriptors:
  return connected_trial(a,kind,label,descriptors)

def connected_trial(a,kind,label,descriptors):
 verify_gate();out=a.output;deadline=time.monotonic_ns()+60000000000
 left,right=socket.socketpair(socket.AF_UNIX,socket.SOCK_SEQPACKET)
 for sock in [left,right]:sock.setsockopt(socket.SOL_SOCKET,socket.SO_PASSCRED,1)
 client_fds=[left.fileno()];server_fds=[right.fileno()];extra=[];mode='server'
 if a.tls:
  from cryptography.hazmat.primitives.asymmetric.ed25519 import Ed25519PrivateKey
  client_key=Ed25519PrivateKey.generate();server_key=Ed25519PrivateKey.generate()
  client_fds.append(ephemeral_identity(client_key,server_key,descriptors))
  server_fd=ephemeral_identity(server_key,client_key,descriptors)
  read_pid,write_pid=os.pipe2(os.O_CLOEXEC)
  for fd in (read_pid,write_pid):descriptors.callback(os.close,fd)
  server_fds.extend((server_fd,read_pid));extra=[str(server_fd),str(read_pid)];mode='tls-server'
 server=subprocess.Popen(['taskset','-c',str(a.peer_cpu),str(a.native),mode,str(right.fileno()),str(a.iterations),*extra],pass_fds=server_fds,stdout=subprocess.PIPE,stderr=subprocess.PIPE);right.close()
 if a.tls:
  clock_fd=os.open(f'/proc/{server.pid}/ns/time',os.O_RDONLY|os.O_CLOEXEC);descriptors.callback(os.close,clock_fd)
  peer_clock,own_clock=os.fstat(clock_fd),os.stat('/proc/self/ns/time')
  assert (peer_clock.st_dev,peer_clock.st_ino)==(own_clock.st_dev,own_clock.st_ino)
  client_fds.append(clock_fd)
 if kind=='native':command=['taskset','-c',str(a.cpu),str(a.native),'client',str(left.fileno()),str(a.iterations),str(server.pid),str(deadline)]+([str(client_fds[1])] if a.tls else [])
 else:
  g=fixture(a.iterations,deadline);wat=out/(label+'.wat');wasm=out/(label+'.wasm');wat.write_text(g.render())
  try:subprocess.run(['wat2wasm','--enable-multi-memory','--debug-names',str(wat),'-o',str(wasm)],check=True)
  except BaseException:
   left.close();server.kill();server.wait();raise
  flags=['--ros-scene-create','robot','--ros-scene-bind','robot','main']
  for name in ['capability.open']+NAMES:flags+=['--ros-scene-interface','robot',name,'allow']
  flags+=['--ros-scene-grant','robot','arm-query','driver','arm-parameters','registered-driver',','.join(NAMES),
   '--ros-scene-driver-budget','robot','1','65536','--ros-scene-driver-kernel-channel','robot','arm-query','main',str(left.fileno()),str(server.pid),str(os.getuid()),str(os.getgid()),
   *[v.hex() for v in IDS],DIGEST.hex(),'32','32','60000000000',str(deadline+1000000000),
   '--ros-scene-driver-schema','robot','arm-query','parameter.exists','64','64','8']
  if a.tls:
   flags[flags.index('--ros-scene-driver-kernel-channel')]='--ros-scene-driver-channel'
   flags[flags.index('--ros-scene-driver-budget')+3]='1048576'
   flags.insert(flags.index('--ros-scene-driver-schema'),str(client_fds[1]))
   flags.insert(flags.index('--ros-scene-driver-schema'),str(clock_fd))
  command=['taskset','-c',str(a.cpu),str(a.runtime),'--wasm-set-main-module-name','main','--runtime-aot','--runtime-compile-threads','0','--runtime-llvm-jit-cache-path','disable',*flags,'--run',str(wasm)]
 client=subprocess.Popen(command,pass_fds=client_fds,stdout=subprocess.PIPE,stderr=subprocess.PIPE);left.close()
 if a.tls:assert os.write(write_pid,struct.pack('=I',client.pid))==4
 try:
  co,ce=client.communicate(timeout=60);so,se=server.communicate(timeout=5)
 finally:
  for p in [client,server]:
   if p.poll() is None:p.kill();p.wait()
 (out/(label+'.stderr')).write_bytes(ce+se)
 assert client.returncode==0 and server.returncode==0,(label,ce[-3000:],se)
 value=json.loads(so);value.update(kind=kind,label=label,mutual_tls=a.tls,argv=command,client_pid=client.pid,receiver_pid=server.pid);print(json.dumps(value),flush=True);return value

def positive_interval(value):
    """@brief Parse a positive, finite VTune sampling interval in milliseconds."""
    interval = float(value)
    if not math.isfinite(interval) or interval <= 0:
        raise argparse.ArgumentTypeError("expected a positive finite sampling interval")
    return interval


def main():
 p=argparse.ArgumentParser();p.add_argument('--runtime',required=True,type=Path);p.add_argument('--native',required=True,type=Path);p.add_argument('--output',required=True,type=Path)
 p.add_argument('--iterations',type=int,default=10000);p.add_argument('--rounds',type=int,default=7);p.add_argument('--cpu',type=int,default=2);p.add_argument('--peer-cpu',type=int,default=4)
 p.add_argument('--tls',action='store_true');p.add_argument('--one',choices=['native','wasm']);p.add_argument('--profile',action='store_true')
 p.add_argument('--hotspots-interval-ms',type=positive_interval,default=0.1)
 p.add_argument('--microarchitecture-interval-ms',type=positive_interval,default=1.0)
 a=p.parse_args();verify_gate();a.output.mkdir(parents=True,exist_ok=True)
 if a.one:trial(a,a.one,'profile-'+a.one);return
 build_path=a.runtime.parent/'full-build-results.json';build=json.loads(build_path.read_text())
 native_path=a.native.parent/'native-build.json';native=json.loads(native_path.read_text())
 assert build['complete'] and build['binary_sha256']==sha(a.runtime)
 assert native['complete'] and native['native_sha256']==sha(a.native) and native['source_root']==build['source_root']
 native_compile=next(row['argv'] for row in native['commands'] if row['name']=='compile')
 result=dict(complete=False,benchmark_complete=False,profiling_requested=a.profile,profiling_complete=False,scope=__doc__,runtime_sha256=sha(a.runtime),native_sha256=sha(a.native),cgroup=str(GATE),events_before=(GATE/'memory.events').read_text(),trials=[],profiles=[],
  source_root=build['source_root'],build_policy=vm_policy(a.runtime),native_build_policy=compile_policy(native_compile),
  inputs={str(path):sha(path) for path in (build_path,native_path,Path(__file__),Path(__file__).with_name('hardware_sampling.py'))})
 def save():(a.output/'results.json').write_text(json.dumps(result,indent=2)+'\n')
 try:
  for i in range(a.rounds):
   for kind in (['native','wasm'] if i%2==0 else ['wasm','native']):result['trials'].append(trial(a,kind,f'{i}-{kind}'));save()
  medians={kind:statistics.median(r['ns_per_roundtrip'] for r in result['trials'] if r['kind']==kind) for kind in ['native','wasm']}
  result['median_ns']=medians;result['ratio']=medians['wasm']/medians['native'];result['benchmark_complete']=True;save()
  if a.profile:
   for kind in ['native','wasm']:
    for analysis in ['hotspots','uarch-exploration']:
     dest=a.output/(kind+'-'+analysis);knobs=['-knob','sampling-mode=hw','-knob','enable-stack-collection=true'] if analysis=='hotspots' else ['-knob','pmu-collection-mode=detailed','-knob','dram-bandwidth-limits=false']
     interval=a.hotspots_interval_ms if analysis=='hotspots' else a.microarchitecture_interval_ms
     command=[VTUNE,'-collect',analysis,*knobs,'-knob',f'sampling-interval={interval:g}','-no-analyze-system','-no-summary','-data-limit=400','-return-app-exitcode','-result-dir',str(dest),'-source-search-dir',build['source_root'],'-source-search-dir',str(ROOT),'--',sys.executable,__file__,'--one',kind,'--runtime',str(a.runtime),'--native',str(a.native),'--output',str(a.output/(kind+'-'+analysis+'-run')),'--iterations','200000','--cpu',str(a.cpu),'--peer-cpu',str(a.peer_cpu)]+(['--tls'] if a.tls else [])
     with (a.output/(kind+'-'+analysis+'.log')).open('w') as log:r=subprocess.run(command,stdout=log,stderr=subprocess.STDOUT,timeout=180)
     row=dict(kind=kind,analysis=analysis,requested_interval_ms=interval,argv=command,exit=r.returncode,reports=[],client_identified=False,source_mapping=False,assembly_mapping=False);result['profiles'].append(row);save()
     if r.returncode:continue
     row['counter_periods']=vtune_periods(dest)
     for line in (a.output/(kind+'-'+analysis+'.log')).read_text().splitlines():
      if line.startswith('{"calls":'):
       profiled=json.loads(line);row.update(client_pid=profiled['client_pid'],receiver_pid=profiled['receiver_pid'],client_identified=True)
     if not row['client_identified']:continue
     selected=['-filter','process-id='+str(row['client_pid'])]
     for report,group in [('hotspots','function'),('hotspots','function,source-file,source-line'),('hw-events','function')]:
      output=a.output/(kind+'-'+analysis+'-'+report+'-'+group+'.tsv')
      cmd=[VTUNE,'-report',report,'-result-dir',str(dest),*selected,'-group-by',group,'-format','csv','-csv-delimiter','tab','-report-output',str(output)]
      with (a.output/(kind+'-'+analysis+'-report.log')).open('a') as log:reported=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,timeout=120)
      row['reports'].append(dict(argv=cmd,exit=reported.returncode))
      if reported.returncode==0 and 'source-line' in group:row['source_mapping']='driver_' in output.read_text()
     output=a.output/(kind+'-'+analysis+'-assembly.tsv')
     cmd=[VTUNE,'-report','hw-events','-result-dir',str(dest),*selected,'-source-object','function=uwvm2::imported::ros::scene::drivers::channel::receive',
      '-group-by','address','-column','Assembly,Source Line,Hardware Event Count','-format','csv','-report-output',str(output)]
     with (a.output/(kind+'-'+analysis+'-report.log')).open('a') as log:reported=subprocess.run(cmd,stdout=log,stderr=subprocess.STDOUT,timeout=120)
     row['reports'].append(dict(argv=cmd,exit=reported.returncode))
     row['assembly_mapping']=reported.returncode==0 and '0x' in output.read_text();save()
  result['profiling_complete']=a.profile and len(result['profiles'])==4 and all(row['exit']==0 and row['client_identified'] and row['source_mapping'] and row['assembly_mapping'] and all(r['exit']==0 for r in row['reports']) for row in result['profiles'])
  result['complete']=result['benchmark_complete'] and (not a.profile or result['profiling_complete'])
 finally:
  result['inputs_unchanged']=all(sha(path)==digest for path,digest in result['inputs'].items()) and sha(a.runtime)==result['runtime_sha256'] and sha(a.native)==result['native_sha256']
  result['complete']=result['complete'] and result['inputs_unchanged']
  result['events_after']=(GATE/'memory.events').read_text();save()
if __name__=='__main__':main()
