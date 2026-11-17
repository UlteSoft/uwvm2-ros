#!/usr/bin/env python3
"""Qualify the bounded communication core and real ROS2 backend in the existing gate."""
import argparse,hashlib,json,os,re,resource,shutil,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--ros-root',type=Path);p.add_argument('--mode',choices=['core','native','all'],default='all');p.add_argument('--trace-native',action='store_true');p.add_argument('--suite',choices=['communication','actions','static-transport'],default='communication');a=p.parse_args();assert a.suite!='static-transport' or a.mode=='native'
 assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
 assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
 assert os.sched_getaffinity(0).issubset({4,5,6,7})
 resource.setrlimit(resource.RLIMIT_CORE,(0,0))
 out=a.output.resolve();assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve());out.mkdir(parents=True,exist_ok=False)
 paths=[*[ROOT/'src/uwvm2/imported/ros/scene'/x for x in ['communication.h','communication_operation.h','communication_identity.h','../runtime/resources.h','communication_codecs.h','event_streams.h','communication_events.h','event_contract.h','event_broker.h','wire.h','communication_graph.h','communication_graph_events.h','communication_graph_contract.h','communication_actions.h','communication_contract.h','communication_action_contract.h','schema_contract.h']],ROOT/'test/0084.ros-runtime/communication_allocator.cpp',ROOT/'test/0084.ros-runtime/event_streams.cpp',ROOT/'test/0084.ros-runtime/communication_graph.cpp',*[ROOT/'src/uwvm2/imported/ros/scene/native'/x for x in ['external_process_only.h','communication_ros2.h','communication_allocator.h','communication_network.h','communication_graph_ros2.h','communication_action_codecs.h','communication_actions_ros2.h']],ROOT/'test/0084.ros-runtime/communication_native_actions_ros2.cpp',ROOT/'test/0084.ros-runtime/communication_actions.cpp',ROOT/'test/0084.ros-runtime/communication_core.cpp',ROOT/'test/0084.ros-runtime/communication_native_ros2.cpp',Path(__file__).resolve()]
 paths+=[ROOT/'src/uwvm2/imported/ros/scene'/x for x in ('bounded_message.h','communication_validation.h')]
 paths+=sorted(x for x in (ROOT/'src/uwvm2/utils').rglob('*') if x.is_file())
 paths+=sorted(x for x in (ROOT/'third-parties/fast_io/include').rglob('*') if x.is_file())
 if a.suite=='static-transport':
  paths+=[ROOT/'test/0084.ros-runtime/communication_static_ros2.cpp',ROOT/'src/uwvm2/imported/ros/scene/native/communication_static_transport.h',*[ROOT/'src/uwvm2/imported/ros/scene'/x for x in ('communication_transport_broker.h','communication_transport_protocol.h','communication_worker_backend.h')]]
 pins={str(x.relative_to(ROOT)):sha(x) for x in paths}
 for path in paths:
  target=out/'source'/path.relative_to(ROOT);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)
 assert pins=={str(x.relative_to(ROOT)):sha(out/'source'/x.relative_to(ROOT)) for x in paths},'source changed while freezing'
 report=dict(complete=False,mode=a.mode,suite=a.suite,scope='provider core and explicit loopback ROS2 native objects; not registered VM imports',sources=pins,commands=[],cgroup=str(GROUP),cgroup_inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),events_before=(GROUP/'memory.events').read_text())
 def save():(out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
 def run(name,cmd,env=None,allow=False,timeout=180):
  result=subprocess.run(cmd,capture_output=True,text=True,timeout=timeout,cwd=out,env=env)
  (out/(name+'.stdout')).write_text(result.stdout);(out/(name+'.stderr')).write_text(result.stderr)
  report['commands'].append(dict(name=name,argv=cmd,exit=result.returncode));save();print(name,result.returncode,flush=True)
  if result.returncode and not allow:raise RuntimeError(result.stderr[-9000:])
  return result
 try:
  source=out/'source';flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(source/'src'),'-I'+str(source/'third-parties/fast_io/include')]
  if a.mode in ('core','all') and a.suite=='actions':
   for name,cc,extra in [('action-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined']),('action-gcc','/usr/bin/g++-13',[]),('action-thread','/usr/bin/clang++-22',['-fsanitize=thread'])]:
    binary=out/name;run(name+'-compile',[cc,*flags,*extra,str(source/'test/0084.ros-runtime/communication_actions.cpp'),'-o',str(binary)])
    for attempt in range(8):
     r=run(name+'-run-'+str(attempt),[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1',TSAN_OPTIONS='halt_on_error=1'),name=='action-thread')
     if not r.returncode:assert 'passed action deferred' in r.stdout;break
     assert r.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not r.stdout
    else:raise RuntimeError('action TSan could not initialize')
  if a.mode in ('core','all') and a.suite=='communication':
   for name,cc,extra in [('sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined']),('gcc','/usr/bin/g++-13',[]),('thread','/usr/bin/clang++-22',['-fsanitize=thread'])]:
    binary=out/('core-'+name);run(name+'-compile',[cc,*flags,*extra,str(source/'test/0084.ros-runtime/communication_core.cpp'),'-o',str(binary)])
    for attempt in range(8):
     r=run(name+'-run-'+str(attempt),[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1',TSAN_OPTIONS='halt_on_error=1'),name=='thread')
     if not r.returncode:assert 'passed communication' in r.stdout;break
     assert r.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not r.stdout
    else:raise RuntimeError('TSan could not initialize')
   for name,cc,extra in [('event-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined']),('event-gcc','/usr/bin/g++-13',[]),('event-thread','/usr/bin/clang++-22',['-fsanitize=thread'])]:
    binary=out/name;run(name+'-compile',[cc,*flags,*extra,str(source/'test/0084.ros-runtime/event_streams.cpp'),'-o',str(binary)])
    for attempt in range(8):
     r=run(name+'-run-'+str(attempt),[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1',TSAN_OPTIONS='halt_on_error=1'),name=='event-thread')
     if not r.returncode:assert 'passed event readiness' in r.stdout;break
     assert r.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not r.stdout
    else:raise RuntimeError('event TSan could not initialize')
   for name,cc,extra in [('graph-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined']),('graph-gcc','/usr/bin/g++-13',[]),('graph-thread','/usr/bin/clang++-22',['-fsanitize=thread'])]:
    binary=out/name;run(name+'-compile',[cc,*flags,*extra,str(source/'test/0084.ros-runtime/communication_graph.cpp'),'-o',str(binary)])
    for attempt in range(8):
     r=run(name+'-run-'+str(attempt),[str(binary)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1',TSAN_OPTIONS='halt_on_error=1'),name=='graph-thread')
     if not r.returncode:assert 'passed graph exact filter' in r.stdout;break
     assert r.stderr.startswith('FATAL: ThreadSanitizer: unexpected memory mapping') and not r.stdout
    else:raise RuntimeError('graph TSan could not initialize')
  if a.mode in ('native','all'):
   assert a.ros_root
   ros=a.ros_root.resolve()/'opt/ros/jazzy';syslib=a.ros_root.resolve()/'system-libs';assert ros.is_dir() and syslib.is_dir()
   report['native_dependencies']={str(x):sha(x) for base in (ros/'lib',ros/'include',syslib) for x in sorted(base.rglob('*')) if x.is_file()}
   for label,compiler,extra in [('allocator-sanitized','/usr/bin/clang++-22',['-fsanitize=address,undefined']),('allocator-gcc','/usr/bin/g++-13',[])]:
    allocated=out/label
    run(label+'-compile',[compiler,*flags,*extra,'-isystem',str(ros/'include/rcutils'),str(source/'test/0084.ros-runtime/communication_allocator.cpp'),'-Wl,--wrap=malloc','-Wl,--wrap=realloc','-o',str(allocated)])
    result=run(label+'-run',[str(allocated)],dict(os.environ,ASAN_OPTIONS='detect_leaks=1'));assert 'passed recoverable bounded ROS C allocator' in result.stdout
   cmd=['/usr/bin/clang++-22',*flags]
   for folder in sorted((ros/'include').iterdir()):
    if folder.is_dir():cmd+=['-isystem',str(folder)]
   native_source={'communication':'communication_native_ros2.cpp','actions':'communication_native_actions_ros2.cpp','static-transport':'communication_static_ros2.cpp'}[a.suite]
   obj=out/'native.o';binary=out/'native';run('native-compile',[*cmd,'-c',str(source/'test/0084.ros-runtime'/native_source),'-o',str(obj)],timeout=300)
   libs=['fastrtps','rclcpp','rcl','rmw','rcutils','rosidl_typesupport_cpp','std_msgs__rosidl_typesupport_cpp','std_srvs__rosidl_typesupport_cpp','libstatistics_collector','tracetools','rcl_interfaces__rosidl_typesupport_cpp','rcl_yaml_param_parser','rcpputils','statistics_msgs__rosidl_typesupport_cpp','rosgraph_msgs__rosidl_typesupport_cpp','builtin_interfaces__rosidl_typesupport_cpp']
   if a.suite in ('actions','static-transport'):libs+=['rclcpp_action','rcl_action','tf2_msgs__rosidl_typesupport_cpp','action_msgs__rosidl_typesupport_cpp','unique_identifier_msgs__rosidl_typesupport_cpp']
   link=['/usr/bin/clang++-22','-pthread',str(obj),'-o',str(binary)]
   for folder in (ros/'lib',syslib):link+=['-L'+str(folder),'-Wl,-rpath,'+str(folder),'-Wl,-rpath-link,'+str(folder)]
   run('native-link',[*link,*['-l'+x for x in libs]])
   env=dict(os.environ,LD_LIBRARY_PATH=str(ros/'lib')+':'+str(syslib),AMENT_PREFIX_PATH=str(ros),RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT',ROS_LOG_DIR=str(out/'ros-logs'))
   r=run('native-run',[str(binary)],env,timeout=45);assert 'passed actual ROS2' in r.stdout
   report['native_binary_sha256']=sha(binary)
   if a.trace_native:
    trace=out/'native-network.strace'
    run('native-network-trace',['/usr/bin/strace','-f','-qq','-e','trace=network','-o',str(trace),str(binary)],env,timeout=45)
    outgoing=[];local_probes=[]
    for line in trace.read_text().splitlines():
     if not re.search(r'\b(bind|connect|sendto|sendmsg)\(',line):continue
     if 'sa_family=AF_NETLINK' in line:continue
     if 'sa_family=AF_UNIX' in line:local_probes.append(line);continue
     assert 'sa_family=AF_INET6' not in line,'unexpected IPv6 network operation'
     if 'sa_family=AF_INET' not in line:continue
     addresses=re.findall(r'inet_addr\("([^"]+)"\)',line)
     assert addresses and all(x=='127.0.0.1' for x in addresses),'non-loopback network operation: '+line
     outgoing.extend(addresses)
    assert outgoing
    report['network_trace']=dict(sha256=sha(trace),ipv4_operation_count=len(outgoing),ipv4_destinations=sorted(set(outgoing)),local_socket_probes=local_probes,
      scope='actual bind/connect/send calls; local dynamic-library socket probes are recorded, not sandboxed')
  report['sources_unchanged']=all(sha(out/'source'/name)==digest for name,digest in pins.items());assert report['sources_unchanged']
  report['complete']=True
 finally:
  report['events_after']=(GROUP/'memory.events').read_text();save()
if __name__=='__main__':main()
