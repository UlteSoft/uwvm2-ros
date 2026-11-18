"""Run two unmodified upstream ros1_bridge String examples against Noetic and Humble."""
import argparse
from contextlib import ExitStack
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import subprocess
import sys
import time
import xmlrpc.client
import xml.etree.ElementTree as ET

HERE = Path(__file__).resolve().parent
SOURCES = {'1_to_2': '61af2af0d1bf8576595cf3d4d83b38ba74019bfbebf5635b90153ce9d4eb9f88',
           '2_to_1': '3052dd3b836a86ee91c5cf511bedd037f31b2f6921cb46b4e6d5ed885a4e3e4d'}
GROUP = 'system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope'
URI = 'http://127.0.0.1:11311'
CYCLONE = ('<CycloneDDS><Domain><General><Interfaces><NetworkInterface name="lo"/></Interfaces>'
           '<AllowMulticast>false</AllowMulticast></General><Discovery><ParticipantIndex>auto</ParticipantIndex>'
           '<Peers><Peer Address="127.0.0.1"/></Peers></Discovery></Domain></CycloneDDS>')


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def gate():
    group = Path('/sys/fs/cgroup')/GROUP
    if (Path('/proc/self/cgroup').read_text().strip() != '0::/' + GROUP or
            (group/'memory.max').read_text().strip() != '68719476736' or
            (group/'memory.swap.max').read_text().strip() != '0' or
            not os.sched_getaffinity(0) <= {4,5,6,7}):
        raise RuntimeError('the shared 64 GiB qualification group and CPU set are required')


def compile_nodes(args, report):
    out, ros2 = args.output, args.humble_prefix
    ros1 = args.noetic_root / 'opt/ros/noetic'
    # Select only the Focal ABI dependencies of Noetic's C++ libraries. Adding
    # all of usr/lib would also replace the host compiler's libc/loader.
    focal_lib = args.noetic_root/'usr/lib/x86_64-linux-gnu'
    compat = out/'focal-dependencies'
    compat.mkdir()
    (compat/'boost').symlink_to(args.noetic_root/'usr/include/boost', target_is_directory=True)
    for pattern in ['libboost_*.so.1.71.0','libicu*.so.66','libapr-1.so.0','libaprutil-1.so.0',
                    'liblog4cxx.so.10','libconsole_bridge.so.0.4']:
        for lib in sorted(focal_lib.glob(pattern)):
            (compat/lib.name).symlink_to(lib.resolve(strict=True))
    noetic_hook = ros1/'etc/catkin/profile.d/1.ros_distro.sh'
    humble_hook = ros2/'share/ros_environment/environment/1.ros_distro.dsv'
    if ('export ROS_DISTRO=noetic' not in noetic_hook.read_text().splitlines() or
            'set;ROS_DISTRO;humble' not in humble_hook.read_text().splitlines()):
        raise ValueError('unrecognized Noetic or Humble installation')
    packages = [(ros1, name) for name in ('roscpp','rospy','rosmaster','std_msgs')]
    packages.extend((ros2, name) for name in ('rclcpp','rmw_cyclonedds_cpp','std_msgs'))
    manifests = [prefix/'share'/name/'package.xml' for prefix, name in packages]
    report['packageVersions'] = {prefix.name+'/'+name: ET.parse(manifest).getroot().findtext('version')
                                 for (prefix, name), manifest in zip(packages, manifests)}
    report['upstreamBridgeCommit'] = '611755fd917285316051cbea80507e8b2f6b7ec1'
    compiler = Path('/usr/bin/clang++-22')
    includes = [ros2/'include', *[p for p in sorted((ros2/'include').iterdir()) if (p/p.name).is_dir()]]
    common = [compiler, '-std=c++17', '-O3', '-g', '-Wall', '-Wextra', '-Werror',
              *[item for p in includes for item in ('-isystem', p)]]
    ros2_names = ['rclcpp','rcl','rcutils','rmw','rmw_implementation','std_msgs__rosidl_typesupport_cpp',
                  'libstatistics_collector','tracetools','rcl_interfaces__rosidl_typesupport_cpp','rcpputils',
                  'statistics_msgs__rosidl_typesupport_cpp','rosgraph_msgs__rosidl_typesupport_cpp',
                  'builtin_interfaces__rosidl_typesupport_cpp']
    ros1_names = ['roscpp','rosconsole','roscpp_serialization','rostime','cpp_common']
    inputs = [noetic_hook, humble_hook, args.noetic_root/'usr/bin/python3.8', focal_lib/'ld-2.31.so', *manifests, Path(__file__).resolve(), HERE/'ros2_witness.cpp', HERE/'ros1_witness.py', compiler,
              ros1/'lib/libroscpp.so', ros1/'share/std_msgs/msg/String.msg', ros1/'bin/rosmaster',
              ros1/'lib/python3/dist-packages/rospy/__init__.py', ros2/'lib/librclcpp.so',
              ros2/'lib/librmw_cyclonedds_cpp.so', ros2/'lib/x86_64-linux-gnu/libddsc.so',
              ros2/'share/std_msgs/msg/String.idl']
    for name in ['1_to_2','2_to_1','ros2_witness']:
        bridge = name in SOURCES
        source = args.bridge_source/'src'/('simple_bridge_'+name+'.cpp') if bridge else HERE/'ros2_witness.cpp'
        if bridge and digest(source) != SOURCES[name]:
            raise ValueError('unexpected upstream bridge source: '+str(source))
        inputs.append(source)
        libs = [ros2/'lib', args.humble_system_libs]
        if bridge:
            libs.extend([ros1/'lib', compat])
        argv = [*common, *(['-isystem', ros1/'include', '-isystem', compat] if bridge else []), source,
                *[item for p in libs for item in ('-L'+str(p), '-Wl,-rpath-link,'+str(p))],
                *['-l'+n for n in ros2_names + (ros1_names if bridge else [])], '-o', out/name]
        if bridge:
            argv.extend([compat/'libboost_thread.so.1.71.0'])
        if name == '2_to_1':
            # Upstream uses a mutable shared_ptr callback deprecated in Humble.
            # Emit the upstream warning and preserve the exact upstream source.
            argv.append('-Wno-error=deprecated-declarations')
        with (out/(name+'.build.log')).open('wb') as log:
            rc = subprocess.run(argv,stdout=log,stderr=subprocess.STDOUT,timeout=90).returncode
        report['build'].append({'name': name, 'argv': list(map(str,argv)), 'exit': rc})
        if rc:
            raise RuntimeError(name+' build failed')
    report['inputs'] = {str(p): digest(p) for p in inputs}
    report['executables'] = {name:digest(out/name) for name in [*SOURCES,'ros2_witness']}


def exchange(args):
    names = [line.split(':')[0].strip() for line in Path('/proc/net/dev').read_text().splitlines()[2:]]
    if names != ['lo']:
        raise RuntimeError('loopback-only network required')
    out, mode, enabled = args.output, args.direction, args.enabled
    case = out/(mode+('-bridge' if enabled else '-control'))
    case.mkdir()
    challenge = secrets.token_hex(16)
    noetic = args.noetic_root/'opt/ros/noetic'
    ros2 = args.humble_prefix
    focal = args.noetic_root/'usr'
    focal_lib = focal/'lib/x86_64-linux-gnu'
    ros1_args = [focal_lib/'ld-2.31.so','--library-path',os.pathsep.join(map(str,(focal_lib,noetic/'lib'))),
                 focal/'bin/python3.8','-B','-s']
    ros1_env = {'PATH':'/usr/bin:/bin','HOME':str(case),'ROS_HOME':str(case),
                'ROS_MASTER_URI':URI,'ROS_IP':'127.0.0.1','ROS_DISTRO':'noetic','ROS_VERSION':'1',
                'ROS_PACKAGE_PATH':str(noetic/'share'),'PYTHONHOME':str(focal),
                'PYTHONPATH':os.pathsep.join(map(str,(noetic/'lib/python3/dist-packages',focal/'lib/python3/dist-packages')))}
    env = {'PATH':'/usr/bin:/bin','HOME':str(case),'ROS_MASTER_URI':URI,'ROS_IP':'127.0.0.1','ROS_DOMAIN_ID':'61',
           'AMENT_PREFIX_PATH':str(ros2),'RMW_IMPLEMENTATION':'rmw_cyclonedds_cpp','ROS_DISTRO':'humble',
           'ROS_HOME':str(case),'ROS_LOG_DIR':str(case/'ros-log'),'CYCLONEDDS_URI':CYCLONE,
           'LD_LIBRARY_PATH':os.pathsep.join(map(str,(ros2/'lib',ros2/'lib/x86_64-linux-gnu',
                               args.humble_system_libs,noetic/'lib',out/'focal-dependencies')))}
    direction = 'send' if mode == '1_to_2' else 'receive'
    timeout = '6'
    def stop(process):
        if process.poll() is None:
            process.terminate()
            try:
                process.wait(timeout=2)
            except subprocess.TimeoutExpired:
                process.kill()
                process.wait()
    with ExitStack() as resources:
        def start(name,argv,env=None):
            log = resources.enter_context((case/(name+'.log')).open('wb'))
            proc = subprocess.Popen(list(map(str,argv)),env=env,stdout=log,stderr=subprocess.STDOUT)
            resources.callback(stop,proc)
            return proc
        master = start('master',[*ros1_args,noetic/'bin/rosmaster','--core','-p','11311'],ros1_env)
        master_api = xmlrpc.client.ServerProxy(URI)
        for _ in range(80):
            if master.poll() is not None:
                raise RuntimeError('Noetic master exited early')
            try:
                if master_api.getPid('/wros_bridge_test')[0] == 1:
                    break
            except OSError:
                time.sleep(0.05)
        else:
            raise RuntimeError('Noetic master unavailable')
        bridge = start('bridge',[out/mode],env) if enabled else None
        ros1 = start('ros1',[*ros1_args,HERE/'ros1_witness.py',direction,challenge,timeout],ros1_env)
        ros2_node = start('ros2',[out/'ros2_witness','receive' if direction=='send' else 'send',challenge,timeout],env)
        expected = ((0,0) if enabled else ((0,3) if direction == 'send' else (3,0)))
        result = ros1.wait(timeout=12), ros2_node.wait(timeout=12)
        if result != expected or (bridge and bridge.poll() is not None):
            raise RuntimeError('unexpected result: '+str(result))
        texts = [(case/(name+'.log')).read_text() for name in ('ros1','ros2')]
        tx, rx = texts if direction == 'send' else texts[::-1]
        if not re.search(r'role=send sent=[1-9]\d* received=0 unexpected=0 value='+challenge,tx):
            raise RuntimeError('publisher did not send challenge')
        wanted = 'role=receive sent=0 received=%d unexpected=0 value=%s' % (enabled,challenge if enabled else '')
        if wanted not in rx or 'rmw=rmw_cyclonedds_cpp' not in texts[1]:
            raise RuntimeError('unexpected receiving node or middleware')
        if enabled and ('Passing along: ['+challenge+']') not in (case/'bridge.log').read_text():
            raise RuntimeError('upstream bridge did not report the known transfer')
        (case/'observed.json').write_text(json.dumps({'direction':mode,'bridge':enabled,'challenge':challenge,
            'ros1Exit':result[0],'ros2Exit':result[1],'networkInterfaces':names,
            'ddsMiddleware':'rmw_cyclonedds_cpp','ros1Topic':'/chatter','ros2Topic':'/chatter',
            'logs':{p.name:digest(p) for p in (case/'ros1.log',case/'ros2.log')}} ,indent=2)+'\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for key in ['noetic-root','humble-prefix','humble-system-libs','bridge-source','output']:
        parser.add_argument('--'+key,type=lambda p:Path(p).resolve(),required=True)
    parser.add_argument('--direction',choices=SOURCES, help=argparse.SUPPRESS)
    parser.add_argument('--enabled',action='store_true',help=argparse.SUPPRESS)
    args = parser.parse_args()
    gate()
    if '.tmp' not in args.output.parts:
        parser.error('use a fresh directory in project .tmp')
    if args.direction:
        exchange(args)
        return
    args.output.mkdir(parents=True)
    report = {'complete':False,'build':[],'cases':[],'scope':'Native upstream fixed String bridge. Plaintext local loopback.',
              'cgroup':Path('/proc/self/cgroup').read_text().strip()}
    try:
        compile_nodes(args,report)
        for mode in SOURCES:
            for enabled in (False, True):
                argv=['/usr/bin/bwrap','--die-with-parent','--unshare-net','--ro-bind','/','/','--bind',args.output,args.output,
                      '--proc','/proc','--dev','/dev',sys.executable,'-B',Path(__file__).resolve(),*sys.argv[1:],
                      '--direction',mode,*(['--enabled'] if enabled else [])]
                # Re-exec arguments may be relative to the caller's cwd.
                subprocess.run(argv,check=True,timeout=38)
                case=args.output/(mode+('-bridge' if enabled else '-control'))
                report['cases'].append(json.loads((case/'observed.json').read_bytes()))
        report['complete'] = len(report['cases']) == 4
    finally:
        (args.output/'results.json').write_text(json.dumps(report,indent=2)+'\n')


if __name__=='__main__':
    main()
