#!/usr/bin/env python3
"""Build and qualify real sealed ROS2 worker RPC against one controlled native peer.

All local source dependencies are frozen before compilation. ROS headers/libraries,
bootstrap ELF, input profile and the exact packaged runtime are independently pinned.
This receipt proves only its closed local profile, not arbitrary peers or codecs.
"""
import argparse,fcntl,hashlib,json,os,re,resource,shlex,shutil,subprocess,time
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
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--seed-pack',type=Path,required=True);p.add_argument('--ros-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();guard()
 resource.setrlimit(resource.RLIMIT_CORE,(0,0));out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
 seed=json.loads(a.seed_pack.read_text());assert seed['complete'];files={row['destination']:row for row in seed['files']}
 for row in [seed['launcher'],*seed['files']]:assert Path(row['path']).stat().st_size==row['bytes'] and sha(row['path'])==row['sha256']
 ros=a.ros_root.resolve()/'opt/ros/jazzy';system=a.ros_root.resolve()/'system-libs';assert ros.is_dir() and system.is_dir()
 compiler='/usr/bin/clang++-22';flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread'];includes=[]
 for folder in sorted((ros/'include').iterdir()):
  if folder.is_dir():includes+=['-isystem',str(folder)]
 backend=Path('tools/ros_scene/communication_worker_native_backend.cpp');fixture=Path('test/0084.ros-runtime/communication_worker_ros2.cpp');source=out/'source'
 report=dict(complete=False,scope=__doc__,cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),commands=[],sources={},external={},snapshots={},events_before=(GROUP/'memory.events').read_text(),seed_pack=str(a.seed_pack.resolve()),seed_pack_sha256=sha(a.seed_pack),compiler_sha256=sha(compiler))
 def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
 def run(label,argv,env=None,timeout=300,ports=False):
  guard();started=time.monotonic()
  with (out.parent/'.communication-transport-ports.lock').open('a') as lock:
   if ports:fcntl.flock(lock,fcntl.LOCK_EX)
   result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,env=env,timeout=timeout)
  (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr);report['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-started));save();print(label,result.returncode,flush=True)
  if result.returncode:raise RuntimeError(result.stderr[-10000:])
  return result
 success=False
 try:
  dependencies={Path(__file__).resolve(),ROOT/'tools/ros_scene/pack_communication_worker.py'}
  for item in (backend,fixture):
   result=run(item.stem+'-dependencies',[compiler,*flags,'-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include'),*includes,'-MM','-MT','objects',str(ROOT/item)])
   entries=shlex.split(result.stdout.replace('\\\n',' '));assert entries[0]=='objects:'
   for name in entries[1:]:
    path=Path(name);path=(ROOT/path).resolve() if not path.is_absolute() else path.resolve();assert path.is_relative_to(ROOT);dependencies.add(path)
  for path in sorted(dependencies):
   name=path.relative_to(ROOT);digest=sha(path);target=source/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target);assert sha(target)==digest
   report['sources'][str(path)]=digest;report['snapshots'][str(name)]=digest
  for directory in (ros/'include',ros/'lib',system):
   for path in sorted(directory.rglob('*')):
    if path.is_file():report['external'][str(path)]=sha(path)
  for row in [seed['launcher'],*seed['files']]:report['external'][row['path']]=row['sha256']
  report['source_files']=len(dependencies);save()
  local=['-I'+str(source/'src'),'-I'+str(source/'third-parties/fast_io/include')]
  libraries=['fastrtps','rclcpp','rcl','rmw','rcutils','rosidl_typesupport_cpp','std_msgs__rosidl_typesupport_cpp','std_srvs__rosidl_typesupport_cpp','libstatistics_collector','tracetools','rcl_interfaces__rosidl_typesupport_cpp','rcl_yaml_param_parser','rcpputils','statistics_msgs__rosidl_typesupport_cpp','rosgraph_msgs__rosidl_typesupport_cpp','builtin_interfaces__rosidl_typesupport_cpp','rclcpp_action','rcl_action','tf2_msgs__rosidl_typesupport_cpp','action_msgs__rosidl_typesupport_cpp','unique_identifier_msgs__rosidl_typesupport_cpp']
  link=['-L'+str(ros/'lib'),'-L'+str(system),'-Wl,-rpath-link,'+str(ros/'lib'),'-Wl,-rpath-link,'+str(system),*['-l'+lib for lib in libraries],'-ldl']
  run('backend-compile',[compiler,*flags,*local,*includes,'-shared','-fPIC',str(source/backend),'-o',str(out/'backend.so'),*link])
  run('fixture-compile',[compiler,*flags,*local,*includes,str(source/fixture),'-o',str(out/'native'),*link])
  pack=out/'pack';argv=['/usr/bin/python3',str(ROOT/'tools/ros_scene/pack_communication_worker.py'),'--bootstrap',files['/worker/bootstrap']['path'],'--backend',str(out/'backend.so'),'--profile',files['/worker/profile']['path'],'--ros-root',str(a.ros_root.resolve()),'--output',str(pack)]
  for library in sorted(set(re.findall(r'"(lib[A-Za-z0-9_]+\.so)"',(source/backend).read_text()))):argv+=['--dlopen',library]
  for row in files.values():
   if row['role']=='configuration' and row['destination'].startswith('/opt/ros/jazzy/'):
    argv+=['--resource',row['destination'][len('/opt/ros/jazzy/'):]]
  run('pack',argv);packed=json.loads((pack/'manifest.json').read_text());assert packed['complete']
  argv=[str(out/'native'),packed['launcher']['path'],packed['launcher']['sha256'],str(packed['required_image_bytes']),'64','65536','65536']
  for row in packed['files']:argv.extend([row['role'],row['destination'],row['path'],row['sha256']])
  env=dict(os.environ,LD_LIBRARY_PATH=str(ros/'lib')+':'+str(system),AMENT_PREFIX_PATH=str(ros),RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT')
  result=run('actual-rpc',argv,env,timeout=60,ports=True);assert 'PASS actual sealed ROS2 worker RPC:' in result.stdout
  report['backend_sha256']=sha(out/'backend.so');report['fixture_sha256']=sha(out/'native');report['pack_sha256']=sha(pack/'manifest.json');report['images']={row['path']:row['sha256'] for row in [packed['launcher'],*packed['files']]};success=True
 finally:
  report['sources_unchanged']=all(sha(path)==digest for path,digest in report['sources'].items());report['external_unchanged']=all(sha(path)==digest for path,digest in report['external'].items());report['snapshot_unchanged']=all(sha(source/path)==digest for path,digest in report['snapshots'].items());report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['sources_unchanged'] and report['external_unchanged'] and report['snapshot_unchanged'];save()
  if success:assert report['complete'],'inputs changed during qualification'
if __name__=='__main__':main()
