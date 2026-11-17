#!/usr/bin/env python3
"""Qualify the real two-worker provider factory with current bilateral authority."""
import argparse,fcntl,hashlib,json,os,resource,shlex,shutil,subprocess,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
 with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def guard():
 assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
 assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
 assert os.sched_getaffinity(0).issubset({4,5,6,7})
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--pair-receipt',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--suite',choices=['provider','topics','services','actions','action_dispatch','endpoint_dispatch','deployment','parameters','action_deployment','observation_deployment','selection_deployment','commit_retry'],default='provider');a=p.parse_args();guard();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
 pair=a.pair_receipt.resolve();evidence=json.loads(pair.read_text());assert evidence['complete']
 packs=[];external={str(pair):sha(pair)}
 for side in range(2):
  path=pair.parent/('pack-'+str(side))/'manifest.json';assert evidence['pack_sha256'][str(path)]==sha(path);external[str(path)]=sha(path)
  packed=json.loads(path.read_text());assert packed['complete'];packs.append(packed)
  for row in [packed['launcher'],*packed['files']]:assert Path(row['path']).stat().st_size==row['bytes'] and sha(row['path'])==row['sha256'];external[row['path']]=row['sha256']
 out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False);source=out/'source'
 compiler='/usr/bin/clang++-22';flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-fno-rtti']
 fixture=Path('test/0084.ros-runtime/communication_worker_'+a.suite+'.cpp')
 scope='Actual sealed Jazzy/FastDDS factory startup, bilateral authority and explicitly reserved nonblocking graph queries using real host node leases' if a.suite=='provider' else 'Actual sealed Jazzy/FastDDS factory-owned topic creation, typed host resource adoption, native DDS String, QoS, receive recovery and per-call authority; not VM carrier qualification'
 if a.suite=='services':scope='Actual sealed Jazzy/FastDDS factory-owned service creation, typed host adoption, native SetBool request/response, separate schema authority, bounded correlations, receive recovery and explicit ACK; not VM carrier qualification'
 if a.suite=='actions':scope='Actual sealed Jazzy/FastDDS factory-owned actions, three independent schemas, distinct host/native goal/proposal/cancel identities, native LookupTransform acceptance, feedback, result and cancellation, retained delivery and verified close; not VM carrier qualification'
 if a.suite=='action_dispatch':scope='Actual sealed Jazzy/FastDDS actions through the canonical action and creation-control dispatchers, dispatcher-minted complete origins, asynchronous creation/adoption, goal/feedback/cancel/result and positive cleanup; trusted context/node setup, not VM carrier qualification'
 if a.suite=='endpoint_dispatch':scope='Actual sealed Jazzy/FastDDS canonical node/topic/service asynchronous creation, actual node evidence, exact typed adoption and full dispatcher-minted authority; native String and SetBool, retained failed-copy recovery, explicit ACK and verified process cleanup; logical context setup, not VM carrier qualification'
 if a.suite=='deployment':scope='Actual sealed startup-definition/image configuration, separately owned worker registries sharing authority, explicit pair supervisors and native node/topic/service publication; not VM carrier qualification'
 if a.suite=='parameters':scope='Actual sealed ROS nodes with exact-alias local parameter stores: zero budget, failed-copy recovery, atomic replay, shared request routing, node revocation and independent cleanup; no native parameter-service replication or VM carrier claim'
 if a.suite=='action_deployment':scope='Actual production action deployment with finite independently owned pair supervision, three exact schema origins, native LookupTransform goals, acceptance, feedback, cancellation and result, failed-delivery recovery and independent cleanup; no VM carrier claim'
 if a.suite=='selection_deployment':scope='Actual isolated ROS cancellation selection with separate primary authority, canonical complete owned-goal set, failed-copy recovery, stable native request correlation, server decision and final cancelled result; no physical stop claim'
 if a.suite=='commit_retry':scope='Actual sealed worker pair with deterministic EINTR at node commit; provisional node destruction must not signal workers, retry activates both participants and actual cleanup is confirmed'
 if a.suite=='observation_deployment':scope='Actual sealed graph capture through production per-worker dispatch and finite shared RPC; host-local events with separately granted cross-worker source origins, failed copies, exact cleanup and revoked-source denial; no VM carrier claim'
 scope=('External deployment-owned Jazzy/Fast DDS peers attached through authenticated control and seven directional pipes; '
        +'suite='+a.suite+'; VM-side provider creates/terminates no process; retirement retains remote cleanup debt; '
        +'test provisioner owns sandbox lifecycle; not Wasm carrier or physical-stop qualification')
 report=dict(complete=False,scope=scope,commands=[],sources={},external=external,snapshots={},cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text(),compiler_flags=flags,compilers={})
 def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
 def run(label,argv,timeout=300):
  guard();started=time.monotonic()
  with (out.parent/'.communication-transport-ports.lock').open('a') as reservation:
   if Path(argv[0]).parent==out:fcntl.flock(reservation,fcntl.LOCK_EX)
   try:result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=timeout,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1'))
   except subprocess.TimeoutExpired as failure:
    for suffix,value in [('stdout',failure.stdout),('stderr',failure.stderr)]:
     (out/(label+'.'+suffix)).write_bytes(value if isinstance(value,bytes) else (value or '').encode())
    report['commands'].append(dict(label=label,argv=argv,exit=None,timed_out=True,seconds=time.monotonic()-started));save()
    raise RuntimeError(label+' exceeded its '+str(timeout)+' second qualification bound') from None
  (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr);report['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-started));save();print(label,result.returncode,flush=True)
  if result.returncode:raise RuntimeError(result.stderr[-10000:])
  return result
 success=False
 try:
  result=run('dependencies',[compiler,*flags,'-I'+str(ROOT/'src'),'-isystem',str(ROOT/'third-parties/fast_io/include'),'-MM','-MT','objects',str(ROOT/fixture)])
  entries=shlex.split(result.stdout.replace('\\\n',' '));assert entries[0]=='objects:';dependencies={Path(__file__).resolve()}
  for name in entries[1:]:
   path=Path(name);path=(ROOT/path).resolve() if not path.is_absolute() else path.resolve();assert path.is_relative_to(ROOT);dependencies.add(path)
  dependencies.update(path.resolve() for path in (ROOT/'third-parties/fast_io/include').rglob('*') if path.is_file())
  for path in sorted(dependencies):
   name=path.relative_to(ROOT);digest=sha(path);target=source/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target);assert sha(target)==digest
   report['sources'][str(path)]=digest;report['snapshots'][str(name)]=digest
  report['source_files']=len(dependencies);save();inputs=[]
  for packed in packs:
   inputs.extend([str(packed['required_image_bytes']),'64','65536','65536',packed['launcher']['path'],packed['launcher']['sha256'],str(len(packed['files']))])
   for row in packed['files']:inputs.extend([row['role'],row['destination'],row['path'],row['sha256']])
  for name,cc,extra in [('sanitized',compiler,['-fsanitize=address,undefined']),('gcc','/usr/bin/g++-13',[])]:
   report['compilers'][cc]=sha(cc);binary=out/name
   run(name+'-compile',[cc,*flags,'-I'+str(source/'src'),'-isystem',str(source/'third-parties/fast_io/include'),*extra,*(['-Wl,--wrap=poll'] if a.suite=='commit_retry' else []),str(source/fixture),'-ldl','-o',str(binary)])
   report.setdefault('binaries',{})[str(binary)]=sha(binary)
   assert {'provider':'PASS actual sealed factory:','topics':'PASS actual sealed typed topics:','services':'PASS actual sealed typed services:','actions':'PASS actual sealed typed actions:','action_dispatch':'PASS actual sealed canonical actions:','endpoint_dispatch':'PASS actual sealed canonical endpoints:','deployment':'PASS actual sealed deployment:','parameters':'PASS actual node-bound parameters:','action_deployment':'PASS actual deployed actions:','observation_deployment':'PASS actual deployed observations:','commit_retry':'PASS actual interrupted node commitment:','selection_deployment':'PASS actual deployed actions:'}[a.suite] in run(name+'-run',[str(binary),*inputs],120).stdout
  success=True
 finally:
  report['sources_unchanged']=all(sha(path)==digest for path,digest in report['sources'].items());report['external_unchanged']=all(sha(path)==digest for path,digest in report['external'].items());report['snapshot_unchanged']=all(sha(source/path)==digest for path,digest in report['snapshots'].items());report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['sources_unchanged'] and report['external_unchanged'] and report['snapshot_unchanged'];save()
  if success:assert report['complete'],'inputs changed during qualification'
if __name__=='__main__':main()
