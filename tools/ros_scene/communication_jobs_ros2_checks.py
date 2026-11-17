#!/usr/bin/env python3
"""Qualify the actual native owned operation queue over admitted nonblocking RPC."""
import argparse,hashlib,json,os,resource,shlex,shutil,subprocess,time
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
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--pair-receipt',type=Path,required=True);p.add_argument('--ros-root',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--suite',choices=['jobs','receives'],default='jobs');a=p.parse_args();guard();resource.setrlimit(resource.RLIMIT_CORE,(0,0))
 pair=a.pair_receipt.resolve();evidence=json.loads(pair.read_text());assert evidence['complete']
 packs=[];external={str(pair):sha(pair)}
 for side in range(2):
  path=pair.parent/('pack-'+str(side))/'manifest.json';assert evidence['pack_sha256'][str(path)]==sha(path);external[str(path)]=sha(path)
  packed=json.loads(path.read_text());assert packed['complete'];packs.append(packed)
  for row in [packed['launcher'],*packed['files']]:assert Path(row['path']).stat().st_size==row['bytes'] and sha(row['path'])==row['sha256'];external[row['path']]=row['sha256']
 out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False);source=out/'source'
 compiler='/usr/bin/clang++-22';flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread']
 fixture=Path('test/0084.ros-runtime/communication_'+a.suite+'_ros2.cpp')
 ros=a.ros_root.resolve()/'opt/ros/jazzy';system=a.ros_root.resolve()/'system-libs';assert ros.is_dir() and system.is_dir()
 includes=[]
 for folder in sorted((ros/'include').iterdir()):
  if folder.is_dir():includes+=['-isystem',str(folder)]
 for directory in (ros/'include',ros/'lib',system):
  for path in sorted(directory.rglob('*')):
   if path.is_file():external[str(path)]=sha(path)
 libraries=['fastrtps','rclcpp','rcl','rmw','rcutils','rosidl_typesupport_cpp','std_msgs__rosidl_typesupport_cpp','std_srvs__rosidl_typesupport_cpp','libstatistics_collector','tracetools','rcl_interfaces__rosidl_typesupport_cpp','rcl_yaml_param_parser','rcpputils','statistics_msgs__rosidl_typesupport_cpp','rosgraph_msgs__rosidl_typesupport_cpp','builtin_interfaces__rosidl_typesupport_cpp','rclcpp_action','rcl_action','tf2_msgs__rosidl_typesupport_cpp','action_msgs__rosidl_typesupport_cpp','unique_identifier_msgs__rosidl_typesupport_cpp']
 link=['-L'+str(ros/'lib'),'-L'+str(system),'-Wl,-rpath-link,'+str(ros/'lib'),'-Wl,-rpath-link,'+str(system),*['-l'+lib for lib in libraries],'-ldl']
 report=dict(complete=False,scope='Actual owned operation queue with per-call full admission, asynchronous creation and private DDS traffic using fixture publication handles; real VM integration remains separate',commands=[],sources={},external=external,snapshots={},cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text(),compiler_flags=flags,compilers={})
 def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
 def run(label,argv,timeout=300):
  guard();started=time.monotonic();result=subprocess.run(argv,cwd=ROOT,capture_output=True,text=True,timeout=timeout,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:allocator_may_return_null=1',LD_LIBRARY_PATH=str(ros/'lib')+':'+str(system),AMENT_PREFIX_PATH=str(ros),RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT'))
  (out/(label+'.stdout')).write_text(result.stdout);(out/(label+'.stderr')).write_text(result.stderr);report['commands'].append(dict(label=label,argv=argv,exit=result.returncode,seconds=time.monotonic()-started));save();print(label,result.returncode,flush=True)
  if result.returncode:raise RuntimeError(result.stderr[-10000:])
  return result
 success=False
 try:
  result=run('dependencies',[compiler,*flags,'-I'+str(ROOT/'src'),'-isystem',str(ROOT/'third-parties/fast_io/include'),*includes,'-MM','-MT','objects',str(ROOT/fixture)])
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
  for name,cc,extra in [('native',compiler,[])]:
   report['compilers'][cc]=sha(cc);binary=out/name
   run(name+'-compile',[cc,*flags,'-I'+str(source/'src'),'-isystem',str(source/'third-parties/fast_io/include'),*includes,*extra,str(source/fixture),*link,'-o',str(binary)])
   report.setdefault('binaries',{})[str(binary)]=sha(binary)
   assert ('PASS actual native queued operations:' if a.suite=='jobs' else 'PASS actual native owned receives:') in run(name+'-run',[str(binary),*inputs],120).stdout
  success=True
 finally:
  report['sources_unchanged']=all(sha(path)==digest for path,digest in report['sources'].items());report['external_unchanged']=all(sha(path)==digest for path,digest in report['external'].items());report['snapshot_unchanged']=all(sha(source/path)==digest for path,digest in report['snapshots'].items());report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['sources_unchanged'] and report['external_unchanged'] and report['snapshot_unchanged'];save()
  if success:assert report['complete'],'inputs changed during qualification'
if __name__=='__main__':main()
