"""Compile a Micro XRCE-DDS client and exercise it with the upstream agent and native Jazzy."""
import argparse
from contextlib import ExitStack
import hashlib
import json
import os
from pathlib import Path
import secrets
import subprocess
import sys
import time

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
from run_scene_vm import verify_gate

parser = argparse.ArgumentParser(description=__doc__)
for key in ('xrce-prefix','microcdr-prefix','native-wire','output'):
    parser.add_argument('--'+key,type=Path,required=True)
parser.add_argument('--network',action='store_true',help=argparse.SUPPRESS)
args = parser.parse_args()
verify_gate()
prefix = args.xrce_prefix.resolve(strict=True)
microcdr = args.microcdr_prefix.resolve(strict=True)
fixture = HERE.parent/'dds_interop'
baseline = args.native_wire.resolve(strict=True)
prior = json.loads(baseline.read_bytes())
if (not prior.get('complete') or prior['request_peer']['rmw'] != 'rmw_fastrtps_cpp' or
    prior['request_peer']['distribution'] != 'jazzy' or prior['ros_type'] != 'std_msgs/msg/Int32'):
    raise ValueError('supply the completed native Jazzy Fast DDS Int32 fixture receipt')
jazzy = Path(prior['ros_prefix']).resolve(strict=True)
network = prior['steps'][-1]['argv']
syslibs = Path(network[network.index('--ros-system-libs')+1]).resolve(strict=True)
client = args.output.resolve()
if '.tmp' not in client.parts:
    raise ValueError('select an unused output under .tmp')
cgroup = Path('/proc/self/cgroup').read_text().strip()
agent = prefix/'bin/MicroXRCEAgent'
peer = baseline.parent/'ros-client'
def sha(path):
    with path.open('rb') as f: return hashlib.file_digest(f,'sha256').hexdigest()

def mapped(process, expected):
    until = time.monotonic()+4
    while time.monotonic()<until and process.poll() is None:
        paths=set()
        for line in Path(f'/proc/{process.pid}/maps').read_text().splitlines():
            fields=line.split(None,5)
            if len(fields)==6 and fields[-1].startswith('/'): paths.add(Path(fields[-1]))
        if all(p.resolve() in paths for p in expected):
            return {str(p.resolve()):sha(p) for p in expected}
        time.sleep(.02)
    raise RuntimeError('missing expected loaded libraries in '+str(process.args))

if args.network:
    interfaces=[line.split(':')[0].strip() for line in Path('/proc/net/dev').read_text().splitlines()[2:]]
    if interfaces != ['lo']:
        raise ValueError('the XRCE client requires an isolated loopback network')
    env=dict(os.environ,PATH='/usr/bin:/bin',HOME=str(client),ROS_HOME=str(client/'ros-home'),ROS_LOG_DIR=str(client/'ros-log'),
        LD_LIBRARY_PATH=':'.join(map(str,(prefix/'lib',agent.parents[1]/'lib',jazzy/'lib',syslibs))), AMENT_PREFIX_PATH=str(jazzy),
        ROS_DOMAIN_ID='61', RMW_IMPLEMENTATION='rmw_fastrtps_cpp',FASTRTPS_DEFAULT_PROFILES_FILE=str(fixture/'fastdds-loopback.xml'))
    token=1+secrets.randbelow(2**29)
    out=dict(complete=False,network='loopback',security='plaintext',challenge=token,loaded={})
    def save(): (client/'exchange.json').write_text(json.dumps(out,indent=2)+'\n')
    with ExitStack() as scope:
        def launch(name,cmd):
            log=scope.enter_context((client/(name+'.log')).open('xb'))
            proc=subprocess.Popen(list(map(str,cmd)),cwd=client,env=env,stdout=log,stderr=subprocess.STDOUT)
            def reap():
                if proc.poll() is None:
                    proc.terminate()
                    try:proc.wait(timeout=2)
                    except subprocess.TimeoutExpired:proc.kill();proc.wait()
            scope.callback(reap)
            return proc
        missing=launch('without-agent',[client/'xrce-client',token])
        out['withoutAgentExit']=missing.wait(timeout=15);save()
        if missing.returncode!=4: raise RuntimeError('client did not reject missing agent')
        gateway=launch('agent',[agent,'udp4','-p','22019'])
        vendor=jazzy/'lib/libfastrtps.so'
        out['loaded']['agent']=mapped(gateway,[vendor,prefix/'lib/libmicroxrcedds_agent.so']);save()
        echo=launch('rclcpp',[peer,token,'echo','rmw_fastrtps_cpp'])
        out['loaded']['rclcpp']=mapped(echo,[vendor,jazzy/'lib/librmw_fastrtps_cpp.so']);save()
        sender=launch('xrce',[client/'xrce-client',token])
        out['clientExit']=sender.wait(timeout=18);out['rclcppExit']=echo.wait(timeout=18)
        out['agentAlive']=gateway.poll() is None
        out['complete']=(out['clientExit']==out['rclcppExit']==0 and out['agentAlive'] and
            f'XRCE sent {token} and received {token+1}' in (client/'xrce.log').read_text().splitlines() and
            'rclcpp rmw_fastrtps_cpp echo completed' in (client/'rclcpp.log').read_text().splitlines())
        save()
    sys.exit(int(not out['complete']))

client.mkdir(parents=True,exist_ok=False)
report=dict(complete=False,cgroup=cgroup,inputs={},steps=[])
def save(): (client/'results.json').write_text(json.dumps(report,indent=2)+'\n')
save()
assert sha(peer)==prior['outputs']['ros-client']
libs = [prefix/'lib/libmicroxrcedds_client.a',microcdr/'lib/libmicrocdr.a']
for p in [Path(__file__),HERE/'roundtrip.c',HERE/'upstream.json',baseline,peer,agent,fixture/'fastdds-loopback.xml',*libs]:
    report['inputs'][str(p)] = sha(p)
def run(name,cmd):
    argv=list(map(str,cmd))
    with (client/(name+'.log')).open('xb')as log:
        proc=subprocess.run(argv,stdout=log,stderr=subprocess.STDOUT,timeout=90)
    report['steps'].append(dict(name=name,argv=argv,exit=proc.returncode));save();proc.check_returncode()
run('compile',['/usr/bin/clang-22','-O3','-g','-Wall','-Wextra','-Werror',HERE/'roundtrip.c','-I',prefix/'include','-I',microcdr/'include',*libs,'-lpthread','-o',client/'xrce-client'])
run('private-network',['/usr/bin/bwrap','--unshare-net','--unshare-ipc','--die-with-parent','--ro-bind','/','/','--proc','/proc','--dev','/dev','--bind',client,client,sys.executable,'-B',Path(__file__).resolve(),*sys.argv[1:],'--network'])
report['exchange']=json.loads((client/'exchange.json').read_bytes())
report['complete']=report['exchange']['complete'];save()
