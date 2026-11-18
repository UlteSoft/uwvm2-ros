"""Compile independent vendor peers, then exchange a ROS topic or service over isolated DDS/RTPS."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import secrets
import subprocess
import sys

HERE = Path(__file__).resolve().parent
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def cyclone_version_header(prefix):
    header, = [p for p in (prefix/'include/dds/version.h', prefix/'include/CycloneDDS/dds/version.h') if p.is_file()]
    return header


def cyclone_version(prefix):
    header = cyclone_version_header(prefix)
    version = re.search(r'^#define DDS_VERSION "(\d+)\.(\d+)\.',header.read_text(),re.M)
    if not version:
        raise RuntimeError('unrecognized Cyclone DDS version header')
    return tuple(map(int,version.groups()))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('ros-prefix', 'ros-system-libs', 'cyclone-prefix', 'output'):
        parser.add_argument('--'+name, type=Path, required=True)
    parser.add_argument('--peer-ros-prefix', type=Path, help='Use rclcpp with rmw_cyclonedds_cpp in the second process')
    parser.add_argument('--peer-system-libs', type=Path, help='System-library directory for the second ROS installation')
    parser.add_argument('--peer-cxx-standard', choices=('17','20'), default='20',
                        help='C++ mode for the second rclcpp installation')
    parser.add_argument('--cyclone-system-libs', type=Path, help='System SONAMEs needed by the Cyclone installation')
    parser.add_argument('--service', action='store_true', help='Exercise rcl_interfaces/srv/GetParameters with two native rclcpp installations')
    parser.add_argument('--service-peer-rmw', choices=('rmw_cyclonedds_cpp','rmw_fastrtps_cpp'),
                        help='Middleware for the service server; uses the second ROS installation')
    parser.add_argument('--service-control', choices=('request','reply'),
                        help='Use the same service binary, RMW and installation for both roles')
    parser.add_argument('--run-peers', action='store_true', help=argparse.SUPPRESS)
    a = parser.parse_args()
    if bool(a.peer_ros_prefix) != bool(a.peer_system_libs):
        parser.error('supply both second-peer paths together')
    if a.service and not a.peer_ros_prefix:
        parser.error('--service requires a second rclcpp installation')
    if (a.service_peer_rmw or a.service_control) and not a.service:
        parser.error('service controls require --service')
    peer_rmw = a.service_peer_rmw or 'rmw_cyclonedds_cpp'
    client_source = HERE / ('ros_service.cpp' if a.service else 'ros_client.cpp')
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
    # Some installations scope all Cyclone headers under include/CycloneDDS.
    dds_include, = [p for p in (cyclone/'include', cyclone/'include/CycloneDDS') if (p/'dds/dds.h').is_file()]
    dds_search = list(dict.fromkeys([dds_lib,cyclone/'lib']))
    if a.cyclone_system_libs:
        dds_search.append(a.cyclone_system_libs.resolve(strict=True))
    elif a.peer_ros_prefix and a.peer_ros_prefix.resolve() == cyclone:
        dds_search.append(a.peer_system_libs.resolve(strict=True))
    dds_env = dict(os.environ, LD_LIBRARY_PATH=os.pathsep.join(map(str,dds_search)))
    ros_env = dict(os.environ, LD_LIBRARY_PATH=os.pathsep.join(map(str,(ros_lib,system))),
                   AMENT_PREFIX_PATH=str(ros), RMW_IMPLEMENTATION='rmw_fastrtps_cpp',
                   ROS_DOMAIN_ID='61', ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT',
                   ROS_LOG_DIR=str(out/'ros-log'), FASTRTPS_DEFAULT_PROFILES_FILE=str(HERE/'fastdds-loopback.xml'))
    ros_env.pop('ROS_LOCALHOST_ONLY', None)
    peer_opts = []
    if a.peer_ros_prefix:
        peer_ros = a.peer_ros_prefix.resolve(strict=True)
        peer_system = a.peer_system_libs.resolve(strict=True)
        peer_dds_lib, = [p.parent for p in (peer_ros/'lib').glob('**/libddsc.so') if p.is_file()]
        peer_opts = ['--peer-ros-prefix',peer_ros,'--peer-system-libs',peer_system,'--peer-cxx-standard',a.peer_cxx_standard]
    if a.run_peers:
        # Bubblewrap creates an isolated network namespace with loopback up.
        interfaces = [row.split(':', 1)[0].strip() for row in Path('/proc/net/dev').read_text().splitlines()[2:]]
        if interfaces != ['lo']:
            raise RuntimeError('the DDS network must only contain loopback')
        # Eloquent's logger writes under HOME even when ROS_LOG_DIR is set.
        # Keep both endpoints' runtime files inside this run's writable mount.
        home = out/'process-home'
        home.mkdir(mode=0o700)
        ros_env.update(HOME=str(home), ROS_HOME=str(home/'.ros'))
        challenge = str(1+secrets.randbelow(2**29))
        # Cyclone 0.7/0.8 use General/NetworkInterfaceAddress for a single
        # interface. Select the schema of the runtime peer, including when an
        # independent installation provided the auxiliary IDL compiler.
        runtime_prefix = peer_ros if peer_opts else cyclone
        version = cyclone_version(runtime_prefix)
        interface = ('<NetworkInterfaceAddress>lo</NetworkInterfaceAddress>'
                     if version < (0,9) else
                     '<Interfaces><NetworkInterface name="lo"/></Interfaces>')
        dds_env['CYCLONEDDS_URI'] = ('<CycloneDDS><Domain><General>'+interface+
            '<AllowMulticast>false</AllowMulticast></General><Discovery><ParticipantIndex>auto</ParticipantIndex>'
            '<Peers><Peer Address="127.0.0.1"/></Peers></Discovery></Domain></CycloneDDS>')
        with (out/'cyclone.log').open('wb') as log:
            peer_command = [out/'cyclone-echo', '61', challenge]
            if peer_opts:
                dds_env.update(ros_env, AMENT_PREFIX_PATH=str(peer_ros), RMW_IMPLEMENTATION='rmw_cyclonedds_cpp',
                               LD_LIBRARY_PATH=os.pathsep.join(map(str,(peer_ros/'lib',peer_dds_lib,peer_system))))
                peer_command = [out/'ros-echo',challenge,'echo','rmw_cyclonedds_cpp']
            client_command = [out/'ros-client',challenge,'request','rmw_fastrtps_cpp']
            server_env, client_env = dds_env, ros_env
            if a.service:
                server_env = dict(dds_env, RMW_IMPLEMENTATION=peer_rmw)
                peer_command[-1] = peer_rmw
                if a.service_control == 'request':
                    server_env = client_env
                    peer_command = [client_command[0],challenge,'echo',client_command[-1]]
                elif a.service_control == 'reply':
                    client_env = server_env
                    client_command = [peer_command[0],challenge,'request',peer_command[-1]]
            peer = subprocess.Popen(peer_command, env=server_env, stdout=log, stderr=subprocess.STDOUT)
            try:
                with (out/'ros.log').open('wb') as roslog:
                    client = subprocess.run(client_command, env=client_env, stdout=roslog,
                                            stderr=subprocess.STDOUT, timeout=18)
                server = peer.wait(timeout=16)
            finally:
                if peer.poll() is None:
                    peer.kill()
                    peer.wait()
        exchange = dict(domain=61, challenge=int(challenge), network='isolated loopback',
                        ros_exit=client.returncode, cyclone_exit=server)
        if a.service:
            names = [challenge+'-a', challenge+'-b']
            exchange['request_names'] = names
            if not (client.returncode or server):
                for log, rmw, role in [('ros.log',client_command[-1],'request'), ('cyclone.log',peer_command[-1],'echo')]:
                    expected = f'GetParameters {rmw} {role} {names[0]} {names[1]} completed'
                    if (out/log).read_text().splitlines().count(expected) != 1:
                        raise RuntimeError('missing correlated service responses in '+log)
                exchange['response_values'] = ['reply/'+name for name in names]
        else:
            exchange['expected_response'] = int(challenge)+1
        (out/'exchange.json').write_text(json.dumps(exchange,indent=2)+'\n')
        return int(bool(client.returncode or server))
    out.mkdir(parents=True)
    inputs = [Path(__file__).resolve(), HERE/'Int32.idl', HERE/'fastdds-loopback.xml', HERE/'cyclone.c', HERE/'ros_client.cpp',
              cyclone/'bin/idlc', cyclone_version_header(cyclone), dds_lib/'libddsc.so', ros_lib/'librmw_fastrtps_cpp.so',
              ros_lib/'librclcpp.so', ros/'share/std_msgs/msg/Int32.idl']
    for lib in ['libfastrtps.so','libfastdds.so']:
        if (ros_lib/lib).is_file():
            inputs.append(ros_lib/lib)
    if peer_opts:
        inputs += [peer_dds_lib/'libddsc.so', cyclone_version_header(peer_ros), peer_ros/'lib/librmw_cyclonedds_cpp.so', peer_ros/'lib/librclcpp.so', peer_ros/'share/std_msgs/msg/Int32.idl']
    if a.service:
        inputs = [Path(__file__).resolve(), HERE/'fastdds-loopback.xml', client_source,
                  cyclone_version_header(peer_ros), peer_dds_lib/'libddsc.so', ros_lib/'librmw_fastrtps_cpp.so',
                  peer_ros/('lib/lib'+peer_rmw+'.so')]
        for prefix in (ros, peer_ros):
            inputs.extend([prefix/'share/rcl_interfaces/srv/GetParameters.idl', prefix/'lib/librclcpp.so',
                           prefix/'lib/librcl_interfaces__rosidl_typesupport_cpp.so'])
        if peer_rmw == 'rmw_fastrtps_cpp':
            vendor = [peer_ros/'lib'/n for n in ('libfastrtps.so','libfastdds.so') if (peer_ros/'lib'/n).is_file()]
            if not vendor:
                raise ValueError('second Fast DDS installation lacks its vendor library')
            inputs.extend(vendor)
        inputs += [ros_lib/lib for lib in ('libfastrtps.so','libfastdds.so') if (ros_lib/lib).is_file()]
    report = dict(complete=False, cgroup=member, ros_prefix=str(ros), cyclone_prefix=str(cyclone),
                  scope='Native DDS wire baseline. Reliable Int32 pub/sub on isolated loopback. Plaintext.',
                  request_peer=dict(language='C++', api='rclcpp', rmw='rmw_fastrtps_cpp', distribution=ros.name, prefix=str(ros)),
                  reply_peer=dict(language='C++' if peer_opts else 'C', api='rclcpp' if peer_opts else 'DDS C',
                                  rmw='rmw_cyclonedds_cpp' if peer_opts else None,
                                  distribution=(peer_ros if peer_opts else cyclone).name, prefix=str(peer_ros if peer_opts else cyclone)),
                  transport='UDPv4', ros_type='std_msgs/msg/Int32', network_security='plaintext',
                  inputs={str(p):digest(p) for p in inputs}, steps=[])
    if a.service:
        report.update(scope='Native GetParameters service on isolated loopback. Plaintext.',
                      ros_type='rcl_interfaces/srv/GetParameters', control=a.service_control)
        report['reply_peer']['rmw'] = peer_rmw
        if a.service_control:
            selected = report[a.service_control+'_peer']
            report.update(request_peer=dict(selected),reply_peer=dict(selected))
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
    # Cyclone 0.8 writes generated files into the working directory. run()
    # already starts every build step in the selected output directory.
    flags = ['-O3','-g','-Wall','-Wextra','-Werror']
    if not a.service:
        idl_output = ['-o',out] if cyclone_version(cyclone) >= (0,9) else []
        run('idlc',[cyclone/'bin/idlc',*idl_output,HERE/'Int32.idl'],dds_env)
        run('build-cyclone',['/usr/bin/cc',*flags,'-I'+str(dds_include),'-I'+str(out),HERE/'cyclone.c',
            out/'Int32.c','-L'+str(dds_lib),*['-Wl,-rpath-link,'+str(directory) for directory in dds_search],'-lddsc','-o',out/'cyclone-echo'],dds_env)
    def compile_ros(prefix, system_lib, output, standard):
        ros_lib = prefix/'lib'
        include = prefix/'include'
        # Ament installs package-scoped headers under include/pkg/pkg. Adding
        # include/dds itself would let Cyclone's features.h shadow glibc.
        includes = [str(include), *[str(p) for p in sorted(include.iterdir()) if (p/p.name).is_dir()]]
        # Header-generated symbols vary across rclcpp releases. Dashing and
        # Eloquent predate the statistics and tracing libraries used by Jazzy.
        optional = ('libstatistics_collector','tracetools','rcl_interfaces__rosidl_typesupport_cpp',
                    'rcpputils','statistics_msgs__rosidl_typesupport_cpp','rosgraph_msgs__rosidl_typesupport_cpp',
                    'builtin_interfaces__rosidl_typesupport_cpp')
        extra = ['-l'+name for name in optional if (ros_lib/('lib'+name+'.so')).is_file()]
        run('build-'+output,['/usr/bin/clang++-22','-std=c++'+standard,*flags,*[v for p in includes for v in ('-isystem',p)],
            client_source,'-L'+str(ros_lib),'-L'+str(system_lib),'-Wl,-rpath-link,'+str(ros_lib),
            '-Wl,-rpath-link,'+str(system_lib),'-lrclcpp','-lrcl','-lrcutils','-lrmw','-lrmw_implementation',
            '-lstd_msgs__rosidl_typesupport_cpp',*extra,'-o',out/output],ros_env)
    compile_ros(ros,system,'ros-client','20')
    if peer_opts:
        compile_ros(peer_ros,peer_system,'ros-echo', a.peer_cxx_standard)
    run('isolated-network',['/usr/bin/bwrap','--die-with-parent','--unshare-net','--ro-bind','/','/',
        '--bind',out,out,'--proc','/proc','--dev','/dev','--ro-bind','/sys','/sys',sys.executable,'-B',
        Path(__file__).resolve(),'--ros-prefix',ros,'--ros-system-libs',system,'--cyclone-prefix',cyclone,
        '--output',out,*peer_opts,*(['--service'] if a.service else []),
        *(['--service-peer-rmw',a.service_peer_rmw] if a.service_peer_rmw else []),
        *(['--service-control',a.service_control] if a.service_control else []),*(['--cyclone-system-libs',a.cyclone_system_libs] if a.cyclone_system_libs else []),'--run-peers'])
    names = ['ros-client','exchange.json','ros.log','cyclone.log']
    if not a.service:
        names.extend(['cyclone-echo','Int32.c','Int32.h'])
    report['outputs']={n:digest(out/n) for n in names}
    if peer_opts:
        report['outputs']['ros-echo']=digest(out/'ros-echo')
    report['complete']=True
    save()
    return 0


if __name__ == '__main__':
    sys.exit(main())
