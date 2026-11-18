"""Qualify one typed parameter relay with original Fast DDS and Cyclone DDS runtimes."""
import argparse
from contextlib import ExitStack
import json
import os
from pathlib import Path
import secrets
import subprocess
import sys

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
from run_scene_vm import sha, verify_gate


def environment(prefix, system, middleware, output):
    vendor = [p.parent for p in (prefix/'lib').glob('**/libddsc.so') if p.is_file()]
    paths = [prefix/'lib', *vendor, system]
    return dict(os.environ, LD_LIBRARY_PATH=os.pathsep.join(map(str,paths)), AMENT_PREFIX_PATH=str(prefix),
                RMW_IMPLEMENTATION=middleware, ROS_DOMAIN_ID='61' if middleware=='rmw_fastrtps_cpp' else '62',
                HOME=str(output), ROS_HOME=str(output/'ros-home'), ROS_LOG_DIR=str(output/'ros-log'),
                FASTRTPS_DEFAULT_PROFILES_FILE=str(HERE/'fastdds-loopback.xml'),
                CYCLONEDDS_URI='<CycloneDDS><Domain><General><Interfaces><NetworkInterface name="lo"/>'
                    '</Interfaces><AllowMulticast>false</AllowMulticast></General><Discovery>'
                    '<ParticipantIndex>auto</ParticipantIndex><Peers><Peer Address="127.0.0.1"/>'
                    '</Peers></Discovery></Domain></CycloneDDS>')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for key in ('fast-prefix','fast-libs','cyclone-prefix','cyclone-libs','output'):
        parser.add_argument('--'+key,required=True,type=Path)
    parser.add_argument('--network',action='store_true',help=argparse.SUPPRESS)
    args = parser.parse_args()
    verify_gate()
    out = args.output.resolve()
    if '.tmp' not in out.parts:
        raise ValueError('choose a new directory under .tmp')
    settings = [(args.fast_prefix.resolve(strict=True), args.fast_libs.resolve(strict=True), 'rmw_fastrtps_cpp'),
                (args.cyclone_prefix.resolve(strict=True), args.cyclone_libs.resolve(strict=True), 'rmw_cyclonedds_cpp')]
    envs = [environment(*setting,out) for setting in settings]
    if args.network:
        interfaces=[line.split(':')[0].strip() for line in Path('/proc/net/dev').read_text().splitlines()[2:]]
        if interfaces != ['lo']:
            raise ValueError('relay qualification requires only loopback')
        token = str(1 + secrets.randbelow(2**29))
        observations = {}
        for mode in ('with-relay','missing-egress'):
            with ExitStack() as stack:
                started = {}
                def start(label, command, side):
                    log = stack.enter_context((out/(mode+'-'+label+'.log')).open('xb'))
                    p = subprocess.Popen(command,env=envs[side],cwd=out,stdout=log,stderr=subprocess.STDOUT)
                    def close():
                        if p.poll() is None:
                            p.kill()
                        p.wait()
                    stack.callback(close)
                    started[label] = p
                if mode=='with-relay':
                    start('server',[out/'witness-1',token,'echo',settings[1][2]],1)
                    start('egress',[out/'relay-1','egress',token,settings[1][2]],1)
                start('ingress',[out/'relay-0','ingress',token,settings[0][2]],0)
                start('client',[out/'witness-0',token,'request',settings[0][2]],0)
                exits = {name:child.wait(timeout=18) for name,child in started.items()}
            passed = all(rc==0 for rc in exits.values()) if mode=='with-relay' else exits=={'ingress':3,'client':3}
            if mode=='with-relay':
                for name,fragment in [('server',settings[1][2]+' echo'),('client',settings[0][2]+' request'),
                                      ('egress','relay egress '+settings[1][2]),('ingress','relay ingress '+settings[0][2])]:
                    line = f'GetParameters {fragment} {token}-a {token}-b completed'
                    passed &= (out/(mode+'-'+name+'.log')).read_text().splitlines().count(line)==1
            observations[mode]=dict(exits=exits,passed=passed)
            (out/'exchange.json').write_text(json.dumps(dict(challenge=token,cases=observations),indent=2)+'\n')
            if not passed:
                return 1
        return 0
    out.mkdir(parents=True,exist_ok=False)
    report=dict(complete=False,scope='Private plaintext native DDS. Two named string parameters.',
                domains=dict(client=61,server=62,topics=63),inputs={},steps=[])
    def save():
        (out/'results.json').write_text(json.dumps(report,indent=2)+'\n')
    def run(name,cmd):
        argv=list(map(str,cmd))
        with (out/(name+'.log')).open('xb') as log:
            p=subprocess.run(argv,stdout=log,stderr=subprocess.STDOUT,cwd=out,timeout=90)
        report['steps'].append(dict(name=name,argv=argv,exit=p.returncode))
        save()
        p.check_returncode()
    inputs=[HERE/'parameter_relay.cpp',HERE/'parameter_relay.py',HERE/'ros_service.cpp',HERE/'fastdds-loopback.xml']
    for i,(prefix,system,rmw) in enumerate(settings):
        include,lib=prefix/'include',prefix/'lib'
        paths=[include,*sorted(p for p in include.iterdir() if (p/p.name).is_dir())]
        libs=['rclcpp','rcl','rcutils','rmw','rmw_implementation','std_msgs__rosidl_typesupport_cpp',
              'rcl_interfaces__rosidl_typesupport_cpp','rcpputils','tracetools','libstatistics_collector',
              'statistics_msgs__rosidl_typesupport_cpp','rosgraph_msgs__rosidl_typesupport_cpp','builtin_interfaces__rosidl_typesupport_cpp']
        inputs += [lib/('lib'+rmw+'.so'),lib/'librclcpp.so', prefix/'share/rcl_interfaces/srv/GetParameters.idl']
        inputs += list(lib.glob('**/libddsc.so')) if i else [lib/'libfastrtps.so']
        for target,source in [('relay','parameter_relay.cpp'),('witness','ros_service.cpp')]:
            run('build-'+target+'-'+str(i),['/usr/bin/clang++-22','-std=c++20','-g','-O3','-Wall','-Wextra','-Werror',
                *[arg for path in paths for arg in ('-isystem',path)],HERE/source,'-L'+str(lib),'-L'+str(system),
                '-Wl,-rpath-link,'+str(lib),'-Wl,-rpath-link,'+str(system),*['-l'+n for n in libs if (lib/('lib'+n+'.so')).is_file()],
                '-o',out/(target+'-'+str(i))])
    report['inputs']={str(p):sha(p) for p in inputs}
    save()
    run('private-network',['/usr/bin/bwrap','--unshare-net','--unshare-ipc','--die-with-parent','--ro-bind','/','/',
        '--dev','/dev','--proc','/proc','--bind',out,out,sys.executable,'-B',Path(__file__).resolve(),*sys.argv[1:],'--network'])
    report.update(complete=True, exchange=json.loads((out/'exchange.json').read_bytes()))
    save()
    return 0


if __name__=='__main__':
    sys.exit(main())
