"""Observe and relay a reliable Int32 exchange across two isolated ROS domains."""
import argparse
import contextlib
import json
import os
from pathlib import Path
import secrets
import selectors
import socket
import subprocess
import sys
import time

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
sys.path.insert(0, str(HERE.parents[2] / 'tools/ros_scene'))
from run_scene_vm import verify_gate
from rmw_native_wire import verify, digest, peer_diagnostics

CYCLONE_PROFILE = ('<CycloneDDS><Domain><General><Interfaces><NetworkInterface name="lo"/>'
                   '</Interfaces><AllowMulticast>false</AllowMulticast></General><Discovery>'
                   '<ParticipantIndex>auto</ParticipantIndex><Peers><Peer Address="127.0.0.1"/>'
                   '</Peers></Discovery></Domain></CycloneDDS>')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def save(path, data):
    path.write_text(json.dumps(data, indent=2) + '\n')


def execute_case(baseline, folder, mode, challenge):
    require(socket.if_nameindex() == [(1, 'lo')], 'expected loopback as the only interface')
    source = json.loads(baseline.read_bytes())
    argv = source['steps'][-1]['argv']
    fast = Path(source['request_peer']['prefix'])
    cyclone = Path(source['reply_peer']['prefix'])
    lib, = [Path(path).parent for path in source['inputs'] if path.endswith('/libddsc.so')]
    profile, = [path for path in source['inputs'] if path.endswith('/fastdds-loopback.xml')]
    require(Path(profile).is_file() and digest(Path(profile)) == source['inputs'][profile], 'Fast DDS profile differs')
    env = {'PATH': '/usr/bin:/bin', 'LANG': 'C', 'ROS_LOG_DIR': str(folder/'ros-log')}
    fast_env = dict(env, ROS_DOMAIN_ID='61', RMW_IMPLEMENTATION='rmw_fastrtps_cpp', AMENT_PREFIX_PATH=str(fast),
                    ROS_AUTOMATIC_DISCOVERY_RANGE='SYSTEM_DEFAULT', FASTRTPS_DEFAULT_PROFILES_FILE=profile,
                    LD_LIBRARY_PATH=f'{fast}/lib:' + argv[argv.index('--ros-system-libs') + 1])
    cyclone_env = dict(env, ROS_DOMAIN_ID='62', RMW_IMPLEMENTATION='rmw_cyclonedds_cpp',
                       AMENT_PREFIX_PATH=str(cyclone), CYCLONEDDS_URI=CYCLONE_PROFILE,
                       LD_LIBRARY_PATH=f'{cyclone}/lib:{lib}:' + argv[argv.index('--peer-system-libs') + 1])
    witness_env = dict(env, CYCLONEDDS_URI=CYCLONE_PROFILE, LD_LIBRARY_PATH=str(lib))
    row = {'mode': mode, 'challenge': challenge, 'expectedResponse': challenge + 1,
           'complete': False, 'exits': {}, 'seconds': {}}
    report = folder/'case.json'
    save(report, row)
    with contextlib.ExitStack() as cleanup:
        def start(command, options, stdout):
            proc = subprocess.Popen(command, env=options, stdout=stdout, stderr=subprocess.STDOUT)
            def stop():
                if proc.poll() is None:
                    proc.kill()
                proc.wait()
            cleanup.callback(stop)
            return proc

        witness = start([folder.parent/'relay', mode, str(challenge)], witness_env, subprocess.PIPE)
        with selectors.DefaultSelector() as events:
            events.register(witness.stdout, selectors.EVENT_READ)
            require(events.select(timeout=8), 'DDS witness startup timed out')
        first = witness.stdout.readline()
        (folder/'witness.log').write_bytes(first)
        require(first == b'ready\n', 'DDS witness failed to create fixed topics')
        started = time.monotonic()
        endpoints = {'echo': (baseline.parent/'ros-echo', 'echo', cyclone_env),
                     'client': (baseline.parent/'ros-client', 'request', fast_env)}
        active = {}
        for name, (binary, operation, environment) in endpoints.items():
            log = cleanup.enter_context((folder/(name+'.log')).open('wb'))
            active[name] = start([binary, str(challenge), operation, environment['RMW_IMPLEMENTATION']], environment, log)
        while active:
            now = time.monotonic() - started
            require(now < 18, 'native endpoint timed out')
            for name, process in list(active.items()):
                if process.poll() is not None:
                    row['exits'][name] = process.returncode
                    row['seconds'][name] = now
                    del active[name]
            time.sleep(0.02)
        output, _ = witness.communicate(timeout=17)
        (folder/'witness.log').write_bytes(first+output)
        row['exits']['witness'] = witness.returncode
    logs = {name: digest(folder/name) for name in ('echo.log', 'client.log', 'witness.log')}
    row['diagnostics'] = {name: peer_diagnostics(folder, logs, name+'.log') for name in ('echo', 'client')}
    row['witnessLogSha256'] = logs['witness.log']
    save(report, row)
    require(row['exits']['witness'] == 0, 'DDS witness failed, see witness.log')
    observed = json.loads(output.splitlines()[-1])
    row['observed'] = observed
    save(report, row)
    require(set(observed) == {'request61','request62','reply62','reply61','forwardWrites','returnWrites'}
            and all(type(n) is int and n >= 0 for n in observed.values()), 'invalid DDS counters')
    require(observed['request61'] > 0, 'missing original request in domain 61')
    if mode == 'observe':
        require(all(n == 0 for key, n in observed.items() if key != 'request61'), 'unexpected cross-domain traffic')
        require(all(row['exits'][name] == 3 and row['seconds'][name] >= 11.5 for name in endpoints),
                'negative control did not reach both native deadlines')
    else:
        require(all(observed.values()), 'missing leg of the relayed roundtrip')
        for name, (_, operation, environment) in endpoints.items():
            status = 'rclcpp ' + environment['RMW_IMPLEMENTATION'] + ' ' + operation + ' completed'
            require(row['exits'][name] == 0 and status in (folder/(name+'.log')).read_text().splitlines(),
                    'rclcpp endpoint did not complete')
    row['complete'] = True
    save(report, row)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--baseline', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--case', choices=('observe','forward'), help=argparse.SUPPRESS)
    parser.add_argument('--challenge', type=int, help=argparse.SUPPRESS)
    args = parser.parse_args()
    verify_gate()
    require(os.sched_getaffinity(0).issubset({4,5,6,7}), 'unexpected qualification CPUs')
    baseline = args.baseline.resolve(strict=True)
    out = args.output.resolve()
    if args.case:
        execute_case(baseline, out/args.case, args.case, args.challenge)
        return
    checked = verify(baseline)
    require(checked['status'] == 'passed' and checked['requestPeer']['distribution'] == 'jazzy'
            and checked['replyPeer']['api'] == 'rclcpp' and checked['replyPeer']['distribution'] == 'humble',
            'expected a verified native Jazzy/Humble rclcpp receipt')
    source = json.loads(baseline.read_bytes())
    headers = Path(source['cyclone_prefix'])/'include'
    lib, = [Path(path).parent for path in source['inputs'] if path.endswith('/libddsc.so')]
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    challenge = 1 + secrets.randbelow(2**29)
    report = {'format': 'uwvm-native-dds-domain-relay-v1', 'complete': False,
              'nativeBaselineSha256': checked['receiptSha256'], 'type': 'std_msgs/msg/Int32',
              'requestPeer': dict(checked['requestPeer'], domain=61, binary=source['outputs']['ros-client']),
              'replyPeer': dict(checked['replyPeer'], domain=62, binary=source['outputs']['ros-echo']),
              'topics': ['/wros_dds_request', '/wros_dds_reply'], 'network': 'isolated UDPv4 loopback',
              'security': 'plaintext', 'rosGraphInteroperability': 'unverified', 'cases': [],
              'sourceSha256': {name: digest(HERE/name) for name in ('relay.c','run.py')}}
    result = out/'results.json'
    save(result, report)
    command = ['/usr/bin/cc', '-std=c11','-O3','-g','-Wall','-Wextra','-Werror','-I'+str(headers),
               '-I'+str(baseline.parent), HERE/'relay.c', baseline.parent/'Int32.c',
               '-L'+str(lib),'-Wl,-rpath-link,'+str(lib),'-lddsc','-o',out/'relay']
    with (out/'build.log').open('wb') as log:
        build = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=90)
    report['build'] = {'argv': list(map(str,command)), 'exit': build.returncode, 'logSha256': digest(out/'build.log')}
    save(result, report)
    build.check_returncode()
    report['relaySha256'] = digest(out/'relay')
    for mode in ('observe','forward'):
        folder = out/mode
        folder.mkdir()
        command = ['/usr/bin/bwrap','--die-with-parent','--unshare-net','--unshare-pid','--unshare-ipc',
                   '--ro-bind','/','/','--bind',out,out,'--proc','/proc','--dev','/dev','--ro-bind','/sys','/sys',
                   sys.executable,'-B',Path(__file__).resolve(),'--baseline',baseline,'--output',out,
                   '--case',mode,'--challenge',str(challenge)]
        with (folder/'run.log').open('wb') as log:
            run = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=55)
        row = json.loads((folder/'case.json').read_bytes()) if (folder/'case.json').exists() else {'complete':False}
        report['cases'].append(dict(row, namespaceExit=run.returncode, runLogSha256=digest(folder/'run.log')))
        save(result, report)
        run.check_returncode()
        require(row['complete'] is True, 'missing case completion')
    report['complete'] = True
    save(result, report)


if __name__ == '__main__':
    main()
