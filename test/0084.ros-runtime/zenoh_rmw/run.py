"""Qualify a native Zenoh ROS Int32 exchange through the package's own TCP router."""
import argparse
from contextlib import ExitStack
import json
import os
from pathlib import Path
import resource
import secrets
import socket
import subprocess
import sys
import time
import xml.etree.ElementTree as ET

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
from packages import MIRROR, prepare, require
from run_archived_dds_peer import SIGNER
from run_scene_vm import GATE, sha, verify_gate
from rmw_native_wire import verify

RMW = 'rmw_zenoh_cpp'
HOST = '0100007F'
PORT = 7447
ENDPOINT = f'tcp/127.0.0.1:{PORT}'
COMMON = 'scouting/multicast/enabled=false;scouting/gossip/enabled=false;transport/shared_memory/enabled=false'
CLIENT = f'mode="client";connect/endpoints=["{ENDPOINT}"];listen/endpoints=[];{COMMON}'
ROUTER = f'mode="router";listen/endpoints=["{ENDPOINT}"];connect/endpoints=[];{COMMON}'


def save(path, value):
    path.write_text(json.dumps(value, indent=2) + '\n')


def installation(args, release, base, system):
    directory = args.packages / release
    package = prepare(args.index, args.keyring, release, directory)
    overlay = directory / 'fs' / 'opt/ros' / release
    members = {directory / 'fs' / member: entry['sha256'] for item in package['packages'].values()
               for member, entry in item['members'].items()}
    def member(name):
        paths = [p for p in members if p.name == name and p.is_relative_to(overlay)]
        require(len(paths) == 1, f'expected one signed {name} in {release}')
        return paths[0]
    plugin = member('librmw_zenoh_cpp.so')
    vendor = member('libzenohc.so')
    router = member('rmw_zenohd')
    router_config = member('DEFAULT_RMW_ZENOH_ROUTER_CONFIG.json5')
    session_config = member('DEFAULT_RMW_ZENOH_SESSION_CONFIG.json5')
    bindings = {'plugin': plugin, 'vendor': vendor, 'router': router, 'routerConfig': router_config,
                'sessionConfig': session_config, 'rclcpp': base / 'lib/librclcpp.so'}
    packages = {}
    for name, item in package['packages'].items():
        xml = overlay / 'share' / name.split('-', 2)[2].replace('-', '_') / 'package.xml'
        require(xml in members, 'missing signed package license declaration')
        licenses = [node.text for node in ET.parse(xml).getroot().findall('license')]
        require(bool(licenses), 'missing package license')
        copyright_file = directory / 'fs/usr/share/doc' / name / 'copyright'
        require(copyright_file in members, 'missing signed package copyright notice')
        packages[name] = {**{key: item[key] for key in ('Version', 'Filename', 'SHA256')},
                          'archiveUrl': MIRROR + item['Filename'], 'packageLicenses': licenses,
                          'copyrightSha256': members[copyright_file], 'packageXmlSha256': members[xml]}
    return {'release': release, 'prefix': str(base), 'system': str(system), 'overlay': str(overlay),
            'signer': SIGNER, 'inReleaseSha256': package['inReleaseSha256'], 'indexSha256': package['indexSha256'],
            'packages': packages, 'paths': {key: str(path) for key, path in bindings.items()},
            'hashes': {key: sha(path) for key, path in bindings.items()}}


def environment(install, output, router=False):
    prefix = Path(install['prefix'])
    paths = {key: Path(p) for key, p in install['paths'].items()}
    libdirs = [paths['plugin'].parent, paths['vendor'].parent, prefix / 'lib', prefix / 'lib/x86_64-linux-gnu', install['system']]
    return {'PATH': '/usr/bin:/bin', 'HOME': str(output), 'ROS_LOG_DIR': str(output / 'ros-log'), 'LC_ALL': 'C',
            'RMW_IMPLEMENTATION': RMW, 'ROS_DOMAIN_ID': '61', 'ZENOH_ROUTER_CHECK_ATTEMPTS': '1',
            'AMENT_PREFIX_PATH': install['overlay'] + ':' + str(prefix), 'LD_LIBRARY_PATH': os.pathsep.join(map(str, libdirs)),
            'ZENOH_SESSION_CONFIG_URI': str(paths['sessionConfig']), 'ZENOH_ROUTER_CONFIG_URI': str(paths['routerConfig']),
            'ZENOH_CONFIG_OVERRIDE': ROUTER if router else CLIENT}


def mapped(pid, paths):
    try:
        lines = Path(f'/proc/{pid}/maps').read_text().splitlines()
    except (FileNotFoundError, PermissionError, ProcessLookupError):
        return set()
    seen = {v[5] for line in lines if len(v := line.split(None, 5)) == 6 and 'x' in v[1]}
    return {label for label, path in paths.items() if str(Path(path).resolve()) in seen}


def tcp_sockets(process):
    inodes = set()
    try:
        for fd in Path(f'/proc/{process.pid}/fd').iterdir():
            try:
                target = os.readlink(fd)
            except (FileNotFoundError, PermissionError, ProcessLookupError):
                continue
            if target.startswith('socket:['):
                inodes.add(target[8:-1])
    except (FileNotFoundError, PermissionError, ProcessLookupError):
        return set()
    found = set()
    for line in Path('/proc/net/tcp').read_text().splitlines()[1:]:
        fields = line.split()
        if fields[9] in inodes:
            local, remote = (tuple(address.split(':')) for address in fields[1:3])
            found.add((local[0], int(local[1], 16), remote[0], int(remote[1], 16), fields[3]))
    return found


def stop(proc):
    if proc.poll() is None:
        proc.terminate()
        try:
            proc.wait(timeout=3)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.wait()


def run_case(config, output, routed):
    output.mkdir()
    challenge = 1 + secrets.randbelow(2**29)
    case = {'routerEnabled': routed, 'challenge': challenge, 'expectedReply': challenge + 1,
            'complete': False, 'passed': False, 'processes': {}}
    processes, observations = {}, {}
    accepted, connections = set(), {'request': set(), 'echo': set()}
    router_listened = False
    try:
        with ExitStack() as stack:
            def start(role, command, install, required):
                log = stack.enter_context((output / (role + '.log')).open('wb'))
                proc = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, cwd=output,
                                        env=environment(install, output, role == 'router'))
                stack.callback(stop, proc)
                processes[role] = proc
                observations[role] = set()
                case['processes'][role] = {'binarySha256': sha(command[0]), 'argv': list(map(str, command)),
                                          'required': required, 'release': install['release']}
                return proc
            left = config['installations'][config['requestRelease']]
            right = config['installations'][config['echoRelease']]
            if routed:
                router = start('router', [left['paths']['router']], left, ['vendor'])
                deadline = time.monotonic() + 8
                while time.monotonic() < deadline and router.poll() is None:
                    if any(local == HOST and port == PORT and state == '0A' for local, port, _, _, state in tcp_sockets(router)):
                        router_listened = True
                        break
                    time.sleep(0.04)
                require(router_listened, 'signed router did not listen on the assigned loopback TCP endpoint')
            for role, inst in [('echo', right), ('request', left)]:
                start(role, [config['binaries'][role], str(challenge), role, RMW], inst, ['plugin', 'vendor', 'rclcpp'])
            deadline = time.monotonic() + 18
            while any(processes[role].poll() is None for role in ('echo', 'request')) and time.monotonic() < deadline:
                for role, proc in processes.items():
                    record = case['processes'][role]
                    inst = config['installations'][record['release']]
                    observations[role].update(mapped(proc.pid, {key: inst['paths'][key] for key in record['required']}))
                    for local, port, remote, dest, state in tcp_sockets(proc):
                        if state == '01' and local == HOST and remote == HOST:
                            if role == 'router' and port == PORT:
                                accepted.add(dest)
                            elif role != 'router' and dest == PORT:
                                connections[role].add(port)
                time.sleep(0.02)
            completed_before_cleanup = {role: proc.poll() for role, proc in processes.items()}
            case['timedOut'] = any(completed_before_cleanup[role] is None for role in ('echo', 'request'))
        case['complete'] = True
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        case['error'] = str(error)
        completed_before_cleanup = {}
    for role, proc in processes.items():
        p = case['processes'][role]
        log = output / (role + '.log')
        text = log.read_text(errors='replace')
        p.update(exit=proc.returncode, observed=sorted(observations[role]), logSha256=sha(log),
                 exitBeforeCleanup=completed_before_cleanup.get(role), invalidOverride='Ignore the invalid configuration' in text,
                 rosErrors=text.count('[ERROR]'), rosWarnings=text.count('[WARN]'))
        if role != 'router':
            p['confirmedSample'] = f'rclcpp {RMW} {role} completed' in text.splitlines()
            p['routerConnectionRefused'] = ENDPOINT in text and 'Connection refused' in text
    case['tcp'] = {'routerListening': router_listened, 'routerAcceptedPorts': sorted(accepted),
                   'clientLocalPorts': {role: sorted(ports) for role, ports in connections.items()}}
    if case['complete']:
        apps = [case['processes'][role] for role in ('request', 'echo')]
        if routed:
            case['passed'] = (not case['timedOut'] and completed_before_cleanup['router'] is None and
                              all(p['exitBeforeCleanup'] == 0 and p['confirmedSample'] for p in apps) and
                              all(set(p['required']) == set(p['observed']) and not p['invalidOverride']
                                  for p in case['processes'].values()) and
                              all(bool(accepted & ports) for ports in connections.values()) and
                              not connections['request'].intersection(connections['echo']))
        else:
            case['passed'] = (not case['timedOut'] and not any(connections.values()) and
                              all(not p['confirmedSample'] and p['exit'] != 0 and p['routerConnectionRefused'] for p in apps))
    save(output / 'case.json', case)
    return case


def worker(output):
    require(socket.if_nameindex() == [(1, 'lo')], 'unexpected network interface')
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    config = json.loads((output / 'inputs.json').read_bytes())
    for install in config['installations'].values():
        require(all(sha(path) == install['hashes'][key] for key, path in install['paths'].items()), 'runtime identity changed')
    for role, binary in config['binaries'].items():
        require(sha(binary) == config['binaryHashes'][role], 'application identity changed')
    result = {'format': 'uwvm-native-zenoh-tcp-v1', 'complete': False, 'inputsSha256': sha(output / 'inputs.json'),
              'scope': 'native rclcpp reliable Int32 via an explicit loopback Zenoh TCP router; plaintext',
              'graphCompatibility': 'unverified', 'requestRelease': config['requestRelease'], 'echoRelease': config['echoRelease'],
              'sessionOverrides': CLIENT, 'routerOverrides': ROUTER, 'cases': {}}
    for name, routed in [('without-router', False), ('with-router', True)]:
        result['cases'][name] = run_case(config, output / name, routed)
        save(output / 'results.json', result)
    result['complete'] = all(case['complete'] for case in result['cases'].values())
    result['passed'] = result['complete'] and all(case['passed'] for case in result['cases'].values())
    save(output / 'results.json', result)
    return int(not result['passed'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('baseline', 'index', 'keyring', 'packages'):
        parser.add_argument('--' + name, type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--same-release', action='store_true', help='Use the requester binary as both applications')
    parser.add_argument('--worker', action='store_true', help=argparse.SUPPRESS)
    args = parser.parse_args()
    verify_gate()
    require(os.sched_getaffinity(0).issubset({4, 5, 6, 7}), 'unexpected qualification CPUs')
    out = args.output.resolve()
    if args.worker:
        return worker(out)
    if any(v is None for v in (args.baseline, args.index, args.keyring, args.packages)):
        parser.error('provide the DDS build receipt, signed index, keyring and package directory')
    args.packages = args.packages.resolve()
    baseline = args.baseline.resolve()
    approved = verify(baseline)
    require(approved['status'] == 'passed' and approved['replyPeer']['api'] == 'rclcpp', 'expected two verified ROS applications')
    prev = json.loads(baseline.read_bytes())
    argv = prev['steps'][-1]['argv']
    config = {'baselineSha256': sha(baseline), 'installations': {}, 'binaries': {}, 'binaryHashes': {},
              'runnerSha256': sha(__file__), 'packageVerifierSha256': sha(HERE / 'packages.py')}
    mounts = {HERE, HERE.parent / 'run_archived_dds_peer.py', HERE.parent / 'run_scene_vm.py',
              HERE.parents[2] / 'tools/ros_scene/rmw_native_wire.py', GATE / 'memory.max', GATE / 'memory.swap.max'}
    for role in ('request', 'echo'):
        src = role if role == 'request' or not args.same_release else 'request'
        binding = prev['request_peer' if src == 'request' else 'reply_peer']
        release, prefix = binding['distribution'], Path(binding['prefix'])
        syslib = Path(argv[argv.index('--ros-system-libs' if src == 'request' else '--peer-system-libs') + 1])
        exe = baseline.parent / ('ros-client' if src == 'request' else 'ros-echo')
        if release not in config['installations']:
            config['installations'][release] = installation(args, release, prefix, syslib)
        config[role + 'Release'] = release
        config['binaries'][role] = str(exe)
        config['binaryHashes'][role] = prev['outputs'][exe.name]
        mounts.update([exe, prefix, syslib, Path(config['installations'][release]['overlay'])])
    out.mkdir(parents=True, exist_ok=False)
    save(out / 'inputs.json', config)
    command = ['/usr/bin/bwrap', '--die-with-parent', '--new-session', '--unshare-net', '--unshare-pid', '--unshare-ipc',
               '--cap-drop', 'ALL', '--clearenv', '--setenv', 'PATH', '/usr/bin:/bin', '--ro-bind', '/usr', '/usr',
               '--symlink', 'usr/bin', '/bin', '--symlink', 'usr/lib', '/lib', '--symlink', 'usr/lib64', '/lib64',
               '--proc', '/proc', '--dev', '/dev', '--tmpfs', '/tmp']
    for path in sorted(mounts):
        if not any(parent in mounts for parent in path.parents):
            command.extend(['--ro-bind', path, path])
    command.extend(['--bind', out, out, '--chdir', out, sys.executable, '-I', '-B', Path(__file__).resolve(),
                    '--worker', '--output', out])
    with (out / 'namespace.log').open('wb') as log:
        return subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=75).returncode


if __name__ == '__main__':
    sys.exit(main())
