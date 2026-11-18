"""Exercise signed historical ROS String examples with their own release and Jazzy."""
import argparse
import json
import os
import errno
import pty
from pathlib import Path
import re
import secrets
import signal
import shutil
import socket
import subprocess
import sys
import time
import threading

from packages import DEMOS, SIGNER, archives, signed_plan
from run_archived_dds_peer import match_payloads, require
from run_scene_vm import sha, verify_gate
from rmw_native_wire import peer_diagnostics

HERE = Path(__file__).resolve()
LIBRARY = re.compile(r'lib(fastrtps|fastdds|fastcdr|rclcpp|rmw_fastrtps_cpp|rmw_fastrtps_shared_cpp)\.so(\..*)?$')
MODES = ('legacy-to-jazzy', 'jazzy-to-legacy', 'native-pair')


def write_json(path, data):
    path.write_text(json.dumps(data, indent=2) + '\n')


def mapped_paths(process, prefix):
    result = set()
    try:
        lines = Path(f'/proc/{process.pid}/maps').read_text().splitlines()
    except FileNotFoundError:
        return result
    for line in lines:
        fields = line.split(None, 5)
        if len(fields) == 6 and 'x' in fields[1]:
            path = Path(fields[5])
            if path.is_relative_to(prefix) and LIBRARY.fullmatch(path.name):
                result.add(path)
    return result


def messages(text, marker):
    # Beta examples used brackets. Ardent and later releases used single quotes.
    pattern = re.escape(marker) + r"""\s*(?:'([^'\r\n]+)'|\[([^\]\r\n]+)\])"""
    return {quoted or bracketed for quoted, bracketed in re.findall(pattern, text)}


def matched_messages(mode, nonce, subscriber, publisher):
    if mode == 'legacy-to-jazzy':
        received = {bytes.fromhex(s).decode() for s in re.findall(r'^received ([a-f0-9]+)$', subscriber, re.M)}
        return sorted(received & messages(publisher, 'Publishing:'))
    if mode == 'jazzy-to-legacy':
        return [nonce] if nonce in messages(subscriber, 'I heard:') and 'sent 10\n' in publisher else []
    return sorted(messages(subscriber, 'I heard:') & messages(publisher, 'Publishing:'))


def capture_terminal(fd, path):
    # Old example nodes use line-buffered console output and can keep spinning after SIGINT.
    with path.open('wb') as log, os.fdopen(fd, 'rb', buffering=0) as terminal:
        while True:
            try:
                block = terminal.read(65536)
            except OSError as error:
                if error.errno == errno.EIO:  # the last PTY slave has closed
                    break
                raise
            if not block:
                break
            log.write(block)


def finish(process):
    sent = []
    for sig in (signal.SIGINT, signal.SIGTERM, signal.SIGKILL):
        if process.poll() is not None:
            break
        sent.append(int(sig))
        process.send_signal(sig)
        try:
            process.wait(timeout=2)
        except subprocess.TimeoutExpired:
            pass
    process.wait()
    return sent


def run_case(mode, args, prefix, programs, environments, packages):
    folder = args.output / mode
    folder.mkdir()
    nonce = 'uwvm_string_' + secrets.token_hex(8)
    peers = {'subscriber': ('legacy', [programs['subscriber']]), 'publisher': ('legacy', [programs['publisher']])}
    if mode == 'legacy-to-jazzy':
        peers['subscriber'] = ('jazzy', [args.witness, 'receive', nonce])
    elif mode == 'jazzy-to-legacy':
        peers['publisher'] = ('jazzy', [args.witness, 'send', nonce])
    row = {'mode': mode, 'nonce': nonce, 'success': False, 'processes': {}, 'diagnostics': {}}
    processes = {}
    captures = []
    observed = {role: set() for role in peers}
    try:
        for role, (kind, command) in peers.items():
            primary, secondary = pty.openpty()
            capture = threading.Thread(target=capture_terminal, args=(primary, folder / (role + '.log')))
            captures.append(capture)
            capture.start()
            with os.fdopen(secondary, 'wb') as log:
                workdir = Path('/tmp/legacy-config') if kind == 'legacy' and getattr(args, 'legacy_profile', None) else None
                processes[role] = subprocess.Popen(command, env=environments[kind], stdout=log, cwd=workdir,
                                                   stderr=subprocess.STDOUT, start_new_session=True)
            row['processes'][role] = {'kind': kind, 'binary': command[0].name, 'binarySha256': sha(command[0])}
        deadline = time.monotonic() + (5 if mode == 'native-pair' else 10)
        while time.monotonic() < deadline:
            for role, process in processes.items():
                observed[role].update(mapped_paths(process, prefix if peers[role][0] == 'legacy' else args.jazzy_prefix))
            if any(process.poll() is not None for process in processes.values()):
                break
            time.sleep(0.04)
        time.sleep(0.15)
    finally:
        for role, process in processes.items():
            sent = finish(process)
            row['processes'][role].update(stoppedByHarness=bool(sent), harnessSignals=sent, exit=process.returncode)
        for capture in captures:
            capture.join()

    runnable = True
    logs = {}
    for role, (kind, command) in peers.items():
        record = row['processes'][role]
        record['mapped'] = {str(path): sha(path) for path in sorted(observed[role])}
        libs = {path.name.split('.so')[0] for path in observed[role]}
        runnable &= {'librclcpp', 'librmw_fastrtps_cpp'} <= libs and bool({'libfastrtps', 'libfastdds'} & libs)
        if kind == 'legacy':
            signed = {**record['mapped'], str(command[0]): record['binarySha256']}
            record['signedPayloads'] = match_payloads(prefix, packages, signed)
            runnable &= record['exit'] == 0 or -record['exit'] in record['harnessSignals']
        else:
            runnable &= record['exit'] == 0 and not record['stoppedByHarness']
        name = role + '.log'
        logs[role] = (folder / name).read_text(errors='replace')
        row['diagnostics'][role] = peer_diagnostics(folder, {name: sha(folder / name)}, name)
    matches = matched_messages(mode, nonce, logs['subscriber'], logs['publisher'])
    row.update(matchedPayloads=matches, success=bool(runnable and len(matches) >= (1 if mode == 'jazzy-to-legacy' else 3)))
    return row


def ros_environment(prefix, system, home, profile):
    libraries = [prefix / 'lib', prefix / 'lib/x86_64-linux-gnu', system]
    env = {'PATH': '/usr/bin:/bin', 'HOME': str(home), 'ROS_HOME': str(home / '.ros'), 'LC_ALL': 'C',
            'ROS_DOMAIN_ID': '0', 'RMW_IMPLEMENTATION': 'rmw_fastrtps_cpp', 'ROS_LOG_DIR': str(home / 'log'),
            'AMENT_PREFIX_PATH': str(prefix), 'LD_LIBRARY_PATH': os.pathsep.join(map(str, libraries))}
    if profile:
        env.update(FASTRTPS_DEFAULT_PROFILES_FILE=str(profile), FASTDDS_DEFAULT_PROFILES_FILE=str(profile))
    return env


def worker(args):
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGTERM, signal.SIG_DFL)
    interfaces = socket.if_nameindex()
    network = 'loopback'
    if getattr(args, 'guest_ethernet', False):
        require(interfaces == [(1, 'lo'), (2, 'eth0')], 'unexpected interfaces on private guest hub')
        network = 'isolated-guest-ethernet-hub'
    else:
        require(interfaces == [(1, 'lo')], 'expected isolated loopback')
    plan = json.loads(args.plan.read_bytes())
    require(plan == signed_plan(plan['release'], args.index_dir, args.keyring), 'plan differs from signed archive closure')
    packages = archives(plan, args.plan.parent / 'debs')
    root = Path('/tmp/legacy')
    root.mkdir()
    for archive, _ in packages.values():
        subprocess.run(['/usr/bin/dpkg-deb', '-x', archive, root], check=True)
    prefix = root / 'opt/ros' / plan['release']
    programs = {role: prefix / 'lib' / ('examples_rclcpp_minimal_' + role) / (role + '_member_function')
                for role in ('publisher', 'subscriber')}
    stems = ['fastrtps', 'fastcdr', 'rclcpp', 'rmw-fastrtps-shared-cpp', *DEMOS]
    selected = {name: pair for name, pair in packages.items() if name.removeprefix('ros-' + plan['release'] + '-') in stems}
    home = Path('/tmp/ros-home')
    home.mkdir()
    legacy_profile = getattr(args, 'legacy_profile', None)
    if legacy_profile:
        workdir = Path('/tmp/legacy-config')
        workdir.mkdir()
        shutil.copyfile(legacy_profile, workdir / 'DEFAULT_FASTRTPS_PROFILES.xml')
    use_defaults = getattr(args, 'legacy_defaults', False)
    environments = {'legacy': ros_environment(prefix, args.legacy_system, home, None if use_defaults else legacy_profile or args.profile),
                    'jazzy': ros_environment(args.jazzy_prefix, args.jazzy_system, home, args.profile)}
    result = {'format': 'uwvm-legacy-fastrtps-string-v1', 'release': plan['release'], 'suite': plan['suite'],
              'scope': 'Native ROS String and signed historical executables', 'network': network, 'complete': False, 'cases': [],
              'signer': SIGNER, 'planSha256': sha(args.plan), 'indexSha256': plan['indexSha256'],
              'inReleaseSha256': plan['inReleaseSha256'], 'profileSha256': sha(args.profile), 'fixtureSha256': sha(HERE),
              'verificationSha256': sha(HERE.with_name('packages.py')), 'witnessSha256': sha(args.witness),
              'interfaces': interfaces, 'transportSecurity': 'plaintext', 'rosGraphInteroperability': 'unverified',
              'addresses': json.loads(subprocess.check_output(['/usr/sbin/ip', '-j', '-4', 'address', 'show'])),
              'routes': json.loads(subprocess.check_output(['/usr/sbin/ip', '-j', '-4', 'route', 'show'])),
              'legacyVendorDefaults': use_defaults,
              'legacyWorkingDirectoryProfileSha256': sha(legacy_profile) if legacy_profile else None}
    write_json(args.output / 'results.json', result)
    for mode in MODES:
        try:
            case = run_case(mode, args, prefix, programs, environments, selected)
        except (OSError, ValueError, subprocess.SubprocessError) as error:
            case = {'mode': mode, 'success': False, 'observationError': str(error)}
        write_json(args.output / mode / 'case.json', case)
        result['cases'].append(case)
        write_json(args.output / 'results.json', result)
    result['complete'] = True
    result['crossReleaseBothDirections'] = all(row['success'] for row in result['cases'][:2])
    result['nativePairPassed'] = result['cases'][2]['success']
    write_json(args.output / 'results.json', result)
    return 0 if result['crossReleaseBothDirections'] and result['nativePairPassed'] else 1


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('plan', 'index-dir', 'keyring', 'legacy-system', 'jazzy-prefix', 'jazzy-system', 'profile', 'witness', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    profile_mode = parser.add_mutually_exclusive_group()
    profile_mode.add_argument('--legacy-defaults', action='store_true', help='Leave legacy vendor profile selection at its defaults')
    profile_mode.add_argument('--legacy-profile', type=Path, help='Place a vendor profile in the legacy application working directory')
    parser.add_argument('--worker', action='store_true', help=argparse.SUPPRESS)
    args = parser.parse_args()
    verify_gate()
    require(os.sched_getaffinity(0).issubset({4, 5, 6, 7}), 'unexpected qualification CPUs')
    if args.worker:
        return worker(args)
    args.output.mkdir(mode=0o700, parents=True, exist_ok=False)
    forwarded = [arg for name, value in vars(args).items() if isinstance(value, Path)
                 for arg in ['--' + name.replace('_', '-'), value.resolve()]]
    if args.legacy_defaults:
        forwarded.append('--legacy-defaults')
    command = ['/usr/bin/bwrap', '--die-with-parent', '--unshare-net', '--unshare-ipc', '--unshare-pid',
               '--ro-bind', '/', '/', '--bind', args.output.resolve(), args.output.resolve(), '--tmpfs', '/tmp',
               '--proc', '/proc', '--dev', '/dev', '--ro-bind', '/sys', '/sys', sys.executable, '-B', HERE, *forwarded, '--worker']
    with (args.output / 'namespace.log').open('wb') as log:
        return subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=120).returncode


if __name__ == '__main__':
    sys.exit(main())
