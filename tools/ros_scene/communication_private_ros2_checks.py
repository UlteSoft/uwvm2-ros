#!/usr/bin/env python3
"""Build and qualify actual ROS2 semantics between two sealed peers over private pipes.

All local source dependencies are frozen before compilation. ROS headers/libraries,
bootstrap ELF, input profile and the exact packaged runtime are independently pinned.
This receipt proves only its closed local profile, not arbitrary peers or codecs.
"""
import argparse,fcntl,hashlib,json,os,re,resource,shlex,shutil,subprocess,time
from pathlib import Path
from pack_communication_worker import ros2_prefix, qualification_output
ROOT=Path(__file__).resolve().parents[2]
GROUP=Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
def sha(path):
 with Path(path).open('rb') as stream:return hashlib.file_digest(stream,'sha256').hexdigest()
def guard():
 assert Path('/proc/self/cgroup').read_text().strip()=='0::/'+str(GROUP.relative_to('/sys/fs/cgroup'))
 assert (GROUP/'memory.max').read_text().strip()=='68719476736' and (GROUP/'memory.swap.max').read_text().strip()=='0'
 assert os.sched_getaffinity(0).issubset({4,5,6,7})
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--seed-pack',type=Path,required=True);p.add_argument('--ros-root',type=Path,required=True);p.add_argument('--ros-distro',default='jazzy');p.add_argument('--profile-json',type=Path,required=True);p.add_argument('--reuse-build',type=Path);p.add_argument('--bounded-topics',action='store_true');p.add_argument('--output',type=Path,required=True);a=p.parse_args();guard()
 resource.setrlimit(resource.RLIMIT_CORE,(0,0));out=a.output.resolve();assert qualification_output(ROOT,out);out.mkdir(parents=True,exist_ok=False)
 seed=json.loads(a.seed_pack.read_text());assert seed['complete'];files={row['destination']:row for row in seed['files']}
 for row in [seed['launcher'],*seed['files']]:assert Path(row['path']).stat().st_size==row['bytes'] and sha(row['path'])==row['sha256']
 ros,hooks=ros2_prefix(a.ros_root,a.ros_distro);prefix='/opt/ros/'+a.ros_distro+'/'
 if seed['ros_prefix']+'/'!=prefix or seed.get('ros_environment',hooks)!=hooks:raise ValueError('seed pack and ROS installation disagree')
 system=a.ros_root.resolve()/'system-libs';assert ros.is_dir() and system.is_dir()
 compiler='/usr/bin/clang++-22';flags=['-std=c++23','-O3','-g','-Wall','-Wextra','-Werror','-pthread'];includes=[]
 includes+=['-isystem',str(ros/'include')]
 for folder in sorted((ros/'include').iterdir()):
  if (folder/folder.name).is_dir():includes+=['-isystem',str(folder)]
 backend=Path('tools/ros_scene/communication_worker_native_backend.cpp');fixture=Path('test/0084.ros-runtime/communication_private_ros2.cpp');source=out/'source'
 if a.bounded_topics:fixture=Path('test/0084.ros-runtime/bounded_topics_private.cpp');assert not a.reuse_build
 report=dict(complete=False,ros_distribution=a.ros_distro,ros_environment=hooks,rmw='rmw_fastrtps_cpp',scope=__doc__,cgroup=str(GROUP),inode=GROUP.stat().st_ino,memory_max=68719476736,swap_max=0,affinity=sorted(os.sched_getaffinity(0)),commands=[],sources={},external={},snapshots={},events_before=(GROUP/'memory.events').read_text(),seed_pack=str(a.seed_pack.resolve()),seed_pack_sha256=sha(a.seed_pack),compiler_sha256=sha(compiler))
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
  dependencies={Path(__file__).resolve(),ROOT/'tools/ros_scene/pack_communication_worker.py',ROOT/'tools/ros_scene/make_communication_worker_definition.py',ROOT/'tools/ros_scene/make_communication_definition.py'}
  bounded=[];generated_libraries=[];generated_packages=[]
  if a.bounded_topics:
   import sys
   from make_bounded_topic_registry import generate
   from make_bounded_ros2_schema import compile_description
   sys.path.insert(0,str(ROOT/'tools/ros_ecosystem'))
   from bounded_message_corpus import message
   names=['geometry_msgs/msg/PoseWithCovariance','std_msgs/msg/Float64MultiArray','sensor_msgs/msg/JointState']
   descriptions=[ros/'share'/(name+'.json')for name in names]
   header,metadata=generate(descriptions,65536);generated=out/'approved-bounded-topics.hpp';generated.write_text(header)
   report['bounded_registry']=metadata;report['generated']={str(generated):sha(generated)};report['external'].update(metadata['sources'])
   generated_libraries=metadata['preload'];generated_packages=metadata['packages']
   flags+=['-DUWVM_ROS_BOUNDED_TOPICS_HEADER="'+str(generated)+'"']
   fixture_directory=out/'bounded-fixtures';fixture_directory.mkdir()
   for index,path in enumerate(descriptions):
    schema,binary,meta=compile_description(json.loads(path.read_text()),sequence_bound=8,string_bound=32,message_bytes=4096,work=65536,depth=32,finite_floats=True)
    bounded.append(dict(identity='bounded.'+str(index),schema=schema,type=meta['type_identity']))
    for suffix,big in [('little',False),('big',True)]:
     file=fixture_directory/(str(index)+'.'+suffix);file.write_bytes(message(schema['nodes'],'cdr',big=big,nonempty=True));report['generated'][str(file)]=sha(file)
   dependencies.update(ROOT/name for name in ('tools/ros_scene/make_bounded_topic_registry.py','tools/ros_scene/make_bounded_ros2_schema.py','tools/ros_scene/bounded_message_definition.py','tools/ros_ecosystem/bounded_message_corpus.py'))
  for item in (backend,fixture):
   result=run(item.stem+'-dependencies',[compiler,*flags,'-I'+str(ROOT/'src'),'-I'+str(ROOT/'third-parties/fast_io/include'),*includes,'-MM','-MT','objects',str(ROOT/item)])
   entries=shlex.split(result.stdout.replace('\\\n',' '));assert entries[0]=='objects:'
   for name in entries[1:]:
    path=Path(name);path=(ROOT/path).resolve() if not path.is_absolute() else path.resolve()
    if path.is_relative_to(ROOT):dependencies.add(path)
    else:assert a.bounded_topics and str(path)in report['generated']
  for path in sorted(dependencies):
   name=path.relative_to(ROOT);digest=sha(path);target=source/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target);assert sha(target)==digest
   report['sources'][str(path)]=digest;report['snapshots'][str(name)]=digest
  for directory in (ros/'include',ros/'lib',system):
   for path in sorted(directory.rglob('*')):
    if path.is_file():report['external'][str(path)]=sha(path)
  for row in [seed['launcher'],*seed['files']]:report['external'][row['path']]=row['sha256']
  report['external'][str(a.profile_json.resolve())]=sha(a.profile_json)
  report['source_files']=len(dependencies);save()
  local=['-I'+str(source/'src'),'-I'+str(source/'third-parties/fast_io/include')]
  libraries=['fastrtps','rclcpp','rcl','rmw','rcutils','rosidl_typesupport_cpp','std_msgs__rosidl_typesupport_cpp','std_srvs__rosidl_typesupport_cpp','libstatistics_collector','tracetools','rcl_interfaces__rosidl_typesupport_cpp','rcl_yaml_param_parser','rcpputils','statistics_msgs__rosidl_typesupport_cpp','rosgraph_msgs__rosidl_typesupport_cpp','builtin_interfaces__rosidl_typesupport_cpp','rclcpp_action','rcl_action','tf2_msgs__rosidl_typesupport_cpp','action_msgs__rosidl_typesupport_cpp','unique_identifier_msgs__rosidl_typesupport_cpp']
  if a.bounded_topics:libraries+=['rmw_implementation','rosidl_runtime_c','rosidl_typesupport_introspection_cpp',*[package+'__rosidl_typesupport_'+suffix for package in generated_packages for suffix in ('cpp','introspection_cpp')]]
  link=['-L'+str(ros/'lib'),'-L'+str(system),'-Wl,-rpath-link,'+str(ros/'lib'),'-Wl,-rpath-link,'+str(system),*['-l'+lib for lib in libraries],'-ldl']
  if a.reuse_build:
   previous=a.reuse_build.resolve();prior=json.loads((previous/'results.json').read_text())
   assert prior['compiler_sha256']==report['compiler_sha256'] and prior['snapshot_unchanged'] and prior['external_unchanged']
   assert all(prior['sources'].get(path)==digest for path,digest in report['sources'].items() if path!=str(Path(__file__).resolve()))
   assert prior['external']==report['external']
   for label,name in [('backend-compile','backend.so'),('fixture-compile','native')]:
    commands=[item for item in prior['commands'] if item['label']==label];assert len(commands)==1 and commands[0]['exit']==0
    assert '-O3' in commands[0]['argv'] and '-g' in commands[0]['argv'] and commands[0]['argv'][0]==compiler
    before=sha(previous/name);shutil.copyfile(previous/name,out/name);(out/name).chmod(0o500);assert sha(out/name)==before
   report['reused_build']=dict(receipt=str(previous/'results.json'),receipt_sha256=sha(previous/'results.json'),backend_sha256=sha(out/'backend.so'),fixture_sha256=sha(out/'native'),scope='Previously successful compiler commands; complete transitive frozen C++ inputs, compiler, flags and external closure reverified. Prior later profile-derivation failure is preserved.')
   save()
  else:
   run('backend-compile',[compiler,*flags,*local,*includes,'-shared','-fPIC',str(source/backend),'-o',str(out/'backend.so'),*link])
   run('fixture-compile',[compiler,*flags,*local,*includes,str(source/fixture),'-o',str(out/'native'),*link])
  report['compiled_binaries']={str(out/name):sha(out/name) for name in ['backend.so','native']};save()
  import copy
  import make_communication_worker_definition as worker_definition
  import make_communication_definition as core_definition
  original=json.loads(a.profile_json.read_text());raw,_=worker_definition.compile_definition(original)
  assert hashlib.sha256(raw).hexdigest()==files['/worker/profile']['sha256']
  report['external'][str(a.profile_json.resolve())]=sha(a.profile_json)
  profiles=[]
  for side in range(2):
   definition=copy.deepcopy(original)
   if side:
    network=definition['network'];network['generation']+=1
    for left,right in [('local_participant','peer_participant'),('local_discovery','peer_discovery'),('local_user','peer_user'),('local_prefix','peer_prefix')]:network[left],network[right]=network[right],network[left]
   references={}
   def update(value):
    if isinstance(value,dict):
     if set(value)=={'identity','sha256'} and value['identity'] in references:value['sha256']=references[value['identity']]
     else:
      for item in value.values():update(item)
    elif isinstance(value,list):
     for item in value:update(item)
   for entry in definition['definitions']:
    semantic=entry['definition'];update(semantic)
    if side and entry['family']=='node':semantic['name']='peer'
    if entry['family']=='graph':semantic['nodes']=['/scene/peer','/scene/robot']
    if side and entry['family'] in ('topic','service','action'):
     mapping={'/scene/publisher':'/scene/subscription','/scene/subscription':'/scene/publisher','/scene/bool_publisher':'/scene/bool_subscription','/scene/bool_subscription':'/scene/bool_publisher','/scene/service_client':'/scene/service_server','/scene/service_server':'/scene/service_client','/scene/action_client':'/scene/action_server','/scene/action_server':'/scene/action_client'}
     semantic['name']=mapping[semantic['name']]
    _,metadata=core_definition.compile_definition(entry);references[entry['identity']]=metadata['profile_sha256']
   for index,binding in enumerate(bounded):
    entry=dict(family='type',provider='canonical-schema',identity=binding['identity'],definition=dict(codec='bounded-message',type=binding['type'],schema=binding['schema']),limits={'definition.bytes':4096,'descriptor.bytes':4096})
    _,meta=core_definition.compile_definition(entry);definition['definitions'].append(entry)
    for role in ('publisher','subscription'):
     endpoint=copy.deepcopy(next(item for item in definition['definitions']if item['family']=='topic'and item['identity']==role))
     endpoint['identity']=binding['identity']+'.'+role;endpoint['definition']['type']={'identity':binding['identity'],'sha256':meta['profile_sha256']}
     endpoint['definition']['name']='/scene/bounded_'+str(index)+('_out'if(role=='publisher')!=(side==1)else'_in')
     endpoint['limits']['message.bytes']=4096
     if role=='subscription':endpoint['limits']['queue.items']=4;endpoint['limits']['queue.bytes']=16384
     definition['definitions'].append(endpoint)
    definition['capacity']['types']+=1;definition['capacity']['topics']+=2;definition['capacity']['queue_bytes']+=16384
   if bounded:definition['capacity']['definition_bytes']=16*1024*1024
   raw,metadata=worker_definition.compile_definition(definition);profile=out/('peer-'+str(side)+'.bin');profile.write_bytes(raw)
   (out/('peer-'+str(side)+'.json')).write_text(json.dumps(definition,indent=2)+'\n');profiles.append(profile)
  packs=[]
  for side,profile in enumerate(profiles):
   pack=out/('pack-'+str(side));argv=['/usr/bin/python3',str(ROOT/'tools/ros_scene/pack_communication_worker.py'),'--bootstrap',files['/worker/bootstrap']['path'],'--backend',str(out/'backend.so'),'--profile',str(profile),'--ros-root',str(a.ros_root.resolve()),'--ros-distro',a.ros_distro,'--output',str(pack)]
   for library in sorted(set(re.findall(r'"(lib[A-Za-z0-9_]+\.so)"',(source/backend).read_text()))|set(generated_libraries)):argv+=['--dlopen',library]
   resources=set()
   for row in files.values():
    if row['role']=='configuration' and row['destination'].startswith(prefix):resources.add(row['destination'][len(prefix):])
   resources.update('share/ament_index/resource_index/packages/'+package for package in generated_packages)
   for resource_name in sorted(resources):argv+=['--resource',resource_name]
   run('pack-'+str(side),argv);packed=json.loads((pack/'manifest.json').read_text());assert packed['complete'];packs.append(packed)
  argv=[str(out/'native')]
  if a.bounded_topics:argv.append(str(fixture_directory))
  for packed in packs:
   argv.extend([str(packed['required_image_bytes']),'64','65536','65536',packed['launcher']['path'],packed['launcher']['sha256'],str(len(packed['files']))])
   for row in packed['files']:argv.extend([row['role'],row['destination'],row['path'],row['sha256']])
  env=dict(os.environ,LD_LIBRARY_PATH=str(ros/'lib')+':'+str(system),AMENT_PREFIX_PATH=str(ros),RMW_IMPLEMENTATION='rmw_fastrtps_cpp',ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT')
  result=run('actual-private-ros2',argv,env,timeout=60);assert ('PASS actual sealed bounded topics:'if a.bounded_topics else'PASS actual sealed peer ROS2:') in result.stdout
  report['backend_sha256']=sha(out/'backend.so');report['fixture_sha256']=sha(out/'native');report['pack_sha256']={str(out/('pack-'+str(i))/'manifest.json'):sha(out/('pack-'+str(i))/'manifest.json') for i in range(2)};report['images']={row['path']:row['sha256'] for packed in packs for row in [packed['launcher'],*packed['files']]};success=True
 finally:
  report['sources_unchanged']=all(sha(path)==digest for path,digest in report['sources'].items());report['external_unchanged']=all(sha(path)==digest for path,digest in report['external'].items());report['snapshot_unchanged']=all(sha(source/path)==digest for path,digest in report['snapshots'].items());report['generated_unchanged']=all(sha(path)==digest for path,digest in report.get('generated',{}).items());report['events_after']=(GROUP/'memory.events').read_text();report['complete']=success and report['sources_unchanged'] and report['external_unchanged'] and report['snapshot_unchanged'] and report['generated_unchanged'];save()
  if success:assert report['complete'],'inputs changed during qualification'
if __name__=='__main__':main()
