"""Verify platform and parent denials before TLS driver I/O on each SDK platform."""
import argparse
from contextlib import ExitStack
import json
import os
from pathlib import Path
import socket
import subprocess
import time

from attachment_peer import issue
from run_driver_attachment_vm import DIGEST, IDS
from run_platform_routes_vm import guest
from run_scene_vm import GATE, sha, verify_gate


def run_case(runtime, wasm, platform, engine, parent_allowed):
    module = 'uwvm_ros_v2.platform_extension.' + platform + '.v1'
    mode = 'child-disabled' if parent_allowed else 'parent-denied'
    with ExitStack() as handles:
        client, peer = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        for endpoint in (client, peer):
            handles.enter_context(endpoint)
            endpoint.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        key, _ = issue(handles)
        clock = os.open('/proc/self/ns/time', os.O_RDONLY | os.O_CLOEXEC)
        handles.callback(os.close, clock)
        flags = ['--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', 'main']
        for operation in ('capability.open', 'driver.connect', 'driver.disconnect'):
            flags += ['--ros-scene-interface', 'robot', operation, 'allow']
        operations = 'driver.connect,driver.disconnect' if parent_allowed else 'driver.disconnect'
        flags += [
            '--ros-scene-grant', 'robot', 'arm', 'driver', 'joint', 'registered-driver', operations,
            '--ros-scene-driver-budget', 'robot', '1', '1048576',
            '--ros-scene-driver-channel', 'robot', 'arm', 'main', str(client.fileno()), str(os.getpid()),
            str(os.getuid()), str(os.getgid()), *[identifier.hex() for identifier in IDS], DIGEST.hex(),
            '128', '128', '1000000000', str(time.monotonic_ns() + 30_000_000_000), str(key), str(clock),
            '--ros-scene-driver-schema', 'robot', 'arm', 'device.submit', '64', '64', '8',
            '--ros-scene', 'robot', 'platform_extension', module, 'disable' if parent_allowed else 'enable',
            '--ros-scene', 'robot', 'platform_extension.bind', module, 'arm',
        ]
        args = [str(runtime), '--wasm-set-main-module-name', 'main', '--runtime-'+engine,
                '--runtime-compile-threads', '0', '--runtime-llvm-jit-cache-path', 'disable',
                *flags, '--run', str(wasm)]
        result = subprocess.run(args, pass_fds=(client.fileno(), key, clock), capture_output=True, timeout=20)
        if result.returncode:
            raise RuntimeError(f'{platform}/{mode}/{engine}: {result.stderr[-1600:]!r}')
        peer.setblocking(False)
        try:
            peer.recv(1)
        except BlockingIOError:
            pass
        else:
            raise RuntimeError(f'{platform}/{mode}/{engine}: unexpected peer I/O')
        return {'platform': platform, 'policy': mode, 'engine': engine, 'exit': result.returncode,
                'peerBytes': 0, 'guest': wasm.name}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ('runtime', 'platform-targets', 'output'):
        p.add_argument('--'+name, required=True, type=Path)
    a = p.parse_args()
    verify_gate()
    platforms = sorted(json.loads(a.platform_targets.read_bytes())['platforms'])
    if not platforms:
        p.error('empty SDK target registry')
    a.output.mkdir(parents=True, exist_ok=False)
    results = {'complete': False, 'vm': sha(a.runtime), 'registry': sha(a.platform_targets),
               'cgroup': str(GATE), 'guests': {}, 'cases': []}
    try:
        for width, memory in ((32, 0), (64, 1)):
            wasm = a.output / f'denied-{width}-{memory}.wasm'
            wat = wasm.with_suffix('.wat')
            wat.write_text(guest(width, memory, 'disabled').render())
            subprocess.run(['wat2wasm','--enable-memory64','--enable-multi-memory','--debug-names',
                            str(wat),'-o',str(wasm)], check=True)
            results['guests'][wasm.name] = sha(wasm)
            for platform in platforms:
                for engine in ('int', 'aot'):
                    for allowed in (True, False):
                        results['cases'].append(run_case(a.runtime, wasm, platform, engine, allowed))
        results['complete'] = len(results['cases']) == 8 * len(platforms)
    finally:
        (a.output/'results.json').write_text(json.dumps(results, indent=2)+'\n')


if __name__ == '__main__':
    main()
