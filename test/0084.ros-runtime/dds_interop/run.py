"""Compile independent vendor peers, then exchange an actual ROS Int32 over isolated DDS/RTPS."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import secrets
import subprocess
import sys

HERE = Path(__file__).resolve().parent
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('ros-prefix', 'ros-system-libs', 'cyclone-prefix', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--peer-ros-prefix', type=Path, help='Use rclcpp with rmw_cyclonedds_cpp in the second process')
    parser.add_argument('--peer-system-libs', type=Path, help='System-library directory for the second ROS installation')
    parser.add_argument('--run-peers', action='store_true', help=argparse.SUPPRESS)
    a = parser.parse_args()
    if bool(a.peer_ros_prefix) != bool(a.peer_system_libs):
        parser.error('supply both second-peer paths together')
    member = Path('/proc/self/cgroup').read_text().strip()
    if (member != '0::/'+str(GROUP.relative_to('/sys/fs/cgroup')) or
            (GROUP/'memory.max').read_text().strip() != '68719476736' or
            (GROUP/'memory.swap.max').read_text().strip() != '0'):
        raise RuntimeError('expected the configured 64 GiB test cgroup')
    ros = a.ros_prefix.resolve(strict=True)
    system = a.ros_system_libs.resolve(strict=True)
    cyclone = a.cyclone_prefix.resolve(strict=True)
    out = a.output.resolve()
    if '.tmp' not in out.parts:
        raise ValueError('choose an output below the project .tmp')
    ros_lib = ros/'lib'
    dds_lib, = [p.parent for p in (cyclone/'lib').glob('**/libddsc.so') if p.is_file()]
    dds_env = dict(os.environ, LD_LIBRARY_PATH=str(dds_lib))
    ros_env = dict(os.environ, LD_LIBRARY_PATH=os.pathsep.join(map(str,(ros_lib,system))),
                   AMENT_PREFIX_PATH=str(ros), RMW_IMPLEMENTATION='rmw_fastrtps_cpp',
                   ROS_DOMAIN_ID='61', ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT',
                   ROS_LOG_DIR=str(out/'ros-log'), FASTRTPS_DEFAULT_PROFILES_FILE=str(HERE/'fastdds-loopback.xml'))
    ros_env.pop('ROS_LOCALHOST_ONLY', None)
    peer_opts = []
    if a.peer_ros_prefix:
        peer_ros = a.peer_ros_prefix.resolve(strict=True)
        peer_system = a.peer_system_libs.resolve(strict=True)
        peer_opts = ['--peer-ros-prefix',peer_ros,'--peer-system-libs',peer_system]
    if a.run_peers:
        # Bubblewrap creates an isolated network namespace with loopback up.
        interfaces = [row.split(':', 1)[0].strip() for row in Path('/proc/net/dev').read_text().splitlines()[2:]]
        if interfaces != ['lo']:
            raise RuntimeError('the DDS network must only contain loopback')
        challenge = str(1+secrets.randbelow(2**29))
        dds_env['CYCLONEDDS_URI'] = ('<CycloneDDS><Domain><General><Interfaces><NetworkInterface name="lo"/>'
            '</Interfaces><AllowMulticast>false</AllowMulticast></General><Discovery><ParticipantIndex>auto</ParticipantIndex>'
            '<Peers><Peer Address="127.0.0.1"/></Peers></Discovery></Domain></CycloneDDS>')
        with (out/'cyclone.log').open('wb') as log:
            peer_command = [out/'cyclone-echo', '61', challenge]
            if peer_opts:
                dds_env.update(ros_env, AMENT_PREFIX_PATH=str(peer_ros), RMW_IMPLEMENTATION='rmw_cyclonedds_cpp',
                               LD_LIBRARY_PATH=os.pathsep.join(map(str,(peer_ros/'lib',dds_lib,peer_system))))
                peer_command = [out/'ros-echo',challenge,'echo','rmw_cyclonedds_cpp']
            peer = subprocess.Popen(peer_command, env=dds_env, stdout=log, stderr=subprocess.STDOUT)
            try:
                with (out/'ros.log').open('wb') as roslog:
                    client = subprocess.run([out/'ros-client', challenge, 'request', 'rmw_fastrtps_cpp'], env=ros_env, stdout=roslog,
                                            stderr=subprocess.STDOUT, timeout=18)
                server = peer.wait(timeout=16)
            finally:
                if peer.poll() is None:
                    peer.kill()
                    peer.wait()
        (out/'exchange.json').write_text(json.dumps(dict(domain=61, challenge=int(challenge),
            expected_response=int(challenge)+1, network='isolated loopback', ros_exit=client.returncode,
            cyclone_exit=server), indent=2)+'\n')
        return int(bool(client.returncode or server))
    out.mkdir(parents=True)
    inputs = [Path(__file__).resolve(), HERE/'Int32.idl', HERE/'fastdds-loopback.xml', HERE/'cyclone.c', HERE/'ros_client.cpp',
              cyclone/'bin/idlc', dds_lib/'libddsc.so', ros_lib/'librmw_fastrtps_cpp.so',
              ros_lib/'librclcpp.so', ros/'share/std_msgs/msg/Int32.idl']
    for lib in ['libfastrtps.so','libfastdds.so']:
        if (ros_lib/lib).is_file():
            inputs.append(ros_lib/lib)
    if peer_opts:
        inputs += [peer_ros/'lib/librmw_cyclonedds_cpp.so', peer_ros/'lib/librclcpp.so', peer_ros/'share/std_msgs/msg/Int32.idl']
    report = dict(complete=False, cgroup=member, ros_prefix=str(ros), cyclone_prefix=str(cyclone),
                  scope='Native DDS wire baseline. Reliable Int32 pub/sub on isolated loopback. Plaintext.',
                  request_peer=dict(language='C++', api='rclcpp', rmw='rmw_fastrtps_cpp', distribution=ros.name, prefix=str(ros)),
                  reply_peer=dict(language='C++' if peer_opts else 'C', api='rclcpp' if peer_opts else 'DDS C',
                                  rmw='rmw_cyclonedds_cpp' if peer_opts else None,
                                  distribution=(peer_ros if peer_opts else cyclone).name, prefix=str(peer_ros if peer_opts else cyclone)),
                  transport='UDPv4', ros_type='std_msgs/msg/Int32', network_security='plaintext',
                  inputs={str(p):digest(p) for p in inputs}, steps=[])
    def save():
        (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name, argv, env=None):
        args = list(map(str,argv))
        with (out/(name+'.log')).open('wb') as log:
            result = subprocess.run(args,env=env,cwd=out,stdout=log,stderr=subprocess.STDOUT,timeout=90)
        report['steps'].append(dict(name=name, argv=args, exit=result.returncode))
        save()
        if result.returncode:
            raise RuntimeError(name+' failed; see '+str(out/(name+'.log')))
    save()
    run('idlc',[cyclone/'bin/idlc','-o',out,HERE/'Int32.idl'],dds_env)
    flags = ['-O3','-g','-Wall','-Wextra','-Werror']
    run('build-cyclone',['/usr/bin/cc',*flags,'-I'+str(cyclone/'include'),'-I'+str(out),HERE/'cyclone.c',
        out/'Int32.c','-L'+str(dds_lib),'-Wl,-rpath-link,'+str(dds_lib),'-lddsc','-o',out/'cyclone-echo'],dds_env)
    def compile_ros(prefix, system_lib, output):
        ros_lib = prefix/'lib'
        include = prefix/'include'
        # Ament installs package-scoped headers under include/pkg/pkg. Adding
        # include/dds itself would let Cyclone's features.h shadow glibc.
        includes = [str(include), *[str(p) for p in sorted(include.iterdir()) if (p/p.name).is_dir()]]
        run('build-'+output,['/usr/bin/clang++-22','-std=c++20',*flags,*[v for p in includes for v in ('-isystem',p)],
            HERE/'ros_client.cpp','-L'+str(ros_lib),'-L'+str(system_lib),'-Wl,-rpath-link,'+str(ros_lib),
            '-Wl,-rpath-link,'+str(system_lib),'-lrclcpp','-lrcl','-lrcutils','-lrmw','-lrmw_implementation',
            '-lstd_msgs__rosidl_typesupport_cpp','-llibstatistics_collector','-ltracetools',
            '-lrcl_interfaces__rosidl_typesupport_cpp','-lrcpputils','-lstatistics_msgs__rosidl_typesupport_cpp',
            '-lrosgraph_msgs__rosidl_typesupport_cpp','-lbuiltin_interfaces__rosidl_typesupport_cpp','-o',out/output],ros_env)
    compile_ros(ros,system,'ros-client')
    if peer_opts:
        compile_ros(peer_ros,peer_system,'ros-echo')
    run('isolated-network',['/usr/bin/bwrap','--die-with-parent','--unshare-net','--ro-bind','/','/',
        '--bind',out,out,'--proc','/proc','--dev','/dev','--ro-bind','/sys','/sys',sys.executable,'-B',
        Path(__file__).resolve(),'--ros-prefix',ros,'--ros-system-libs',system,'--cyclone-prefix',cyclone,
        '--output',out,*peer_opts,'--run-peers'])
    report['outputs']={n:digest(out/n) for n in ('ros-client','cyclone-echo','Int32.c','Int32.h','exchange.json','ros.log','cyclone.log')}
    if peer_opts:
        report['outputs']['ros-echo']=digest(out/'ros-echo')
    report['complete']=True
    save()
    return 0


if __name__ == '__main__':
    sys.exit(main())
