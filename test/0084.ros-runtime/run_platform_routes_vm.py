#!/usr/bin/env python3
"""Exercise every platform foundation's restriction on real common driver calls.

The external peer implements only authenticated connect/release for a declared
device.submit contract. No vendor implementation or physical effect is tested.
An enabled child cannot restore a denied parent; a disabled child prevents even
the initial hello and leaves guest result memory unchanged.
"""
import argparse
import json
import os
from pathlib import Path
import re
import socket
import struct
import subprocess
import sys
import time
from run_scene_vm import GATE, verify_gate, sha
from run_module_security_vm import Fixture
from run_driver_attachment_vm import IDS, DIGEST


def peer(fd, mode):
    verify_gate(); expected = int(sys.stdin.readline())
    endpoint = socket.socket(fileno=fd); endpoint.settimeout(15); packets = 0
    while True:
        data, ancillary, flags, _ = endpoint.recvmsg(176, socket.CMSG_SPACE(12))
        if not data:
            break
        assert mode == 'enabled' and len(data) == 176 and not flags
        credentials = [struct.unpack('3i', value) for level, kind, value in ancillary
                       if level == socket.SOL_SOCKET and kind == socket.SCM_CREDENTIALS]
        assert credentials == [(expected, os.getuid(), os.getgid())]
        header = list(struct.unpack('<22Q', data))
        assert header[:2] == [0x44534f52564d5755, 176] and header[3] == 0
        assert data[32:112] == b''.join(IDS) and data[112:144] == DIGEST and not any(header[18:])
        packets += 1
        if packets == 1:
            assert header[2] == 1
            header[2] = 2; assert endpoint.send(struct.pack('<22Q', *header)) == 176
        else:
            assert packets == 2 and header[2] == 5
            break
    endpoint.close()
    assert packets == (2 if mode == 'enabled' else 0)
    print(json.dumps(dict(packets=packets, effects=0)), flush=True)


def guest(width, memory, mode):
    g = Fixture(['driver.connect', 'driver.disconnect'], width, memory)
    capability = g.open('arm')
    expression = f'{capability} i32.const {memory} i64.const 128 i64.const 0 i64.const 16384 i64.const 8 i64.const 1024 call $driver.connect'
    if mode != 'enabled':
        g.write(16384, 'i64.const 17185'); g.write(1024, 'i64.const 4660')
        g.check(expression, 13); g.equal(g.word(16384), 17185); g.equal(g.word(1024), 4660)
    else:
        g.extra_functions.append(f'''(func $connect (result i32) (local $s i32) (local $n i32)
            block $done loop $retry {expression} local.tee $s i32.const 11 i32.ne br_if $done
            local.get $n i32.const 1 i32.add local.tee $n i32.const 10000000 i32.gt_u if unreachable end
            br $retry end end local.get $s)''')
        g.check('call $connect')
        g.request('driver.disconnect', b'', cap=capability, capacity=8)
    return g


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--peer', type=int)
    parser.add_argument('--mode', choices=['enabled', 'disabled', 'parent-denied'])
    args = parser.parse_args()
    if args.peer is not None:
        peer(args.peer, args.mode); return
    verify_gate(); runtime = args.runtime.resolve(); out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    build_path = runtime.parent/'full-build-results.json'; build = json.loads(build_path.read_text())
    assert build['complete'] and build['binary_sha256'] == sha(runtime)
    header = Path(build['source_root'])/'src/uwvm2/imported/ros/scene/platform_module.h'
    names = re.findall(r'"([a-z0-9_-]+)"', re.search(r'foundation_platforms\{(.*?)\};', header.read_text(), re.S)[1])
    assert len(names) == 32 and len(set(names)) == 32
    report = dict(complete=False, scope=__doc__, runtime_sha256=sha(runtime), cases=[],
                  inputs={str(p): sha(p) for p in (Path(__file__), build_path, header)},
                  cgroup=str(GATE), events_before=(GATE/'memory.events').read_text())
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    try:
        guests = {}
        for width, memory in ((32, 0), (32, 1), (64, 0), (64, 1)):
            for mode in ('enabled', 'disabled', 'parent-denied'):
                wasm = out/f'{width}-{memory}-{mode}.wasm'; wat = wasm.with_suffix('.wat')
                wat.write_text(guest(width, memory, mode).render())
                subprocess.run(['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)], check=True)
                guests[width, memory, mode] = wasm
        for name in names:
            module = 'uwvm_ros_v2.platform_extension.'+name+'.v1'
            for (width, memory, mode), wasm in guests.items():
                for engine in ('int', 'aot'):
                    left, right = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
                    for endpoint in (left, right):
                        endpoint.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
                    child = subprocess.Popen([sys.executable, __file__, '--peer', str(right.fileno()), '--mode', mode],
                        pass_fds=(right.fileno(),), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                    right.close(); vm = None
                    try:
                        flags = ['--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', 'main']
                        for action in ('capability.open', 'driver.connect', 'driver.disconnect'):
                            flags += ['--ros-scene-interface', 'robot', action, 'allow']
                        actions = 'driver.connect,driver.disconnect' if mode != 'parent-denied' else 'driver.disconnect'
                        flags += ['--ros-scene-grant', 'robot', 'arm', 'driver', 'joint', 'registered-driver', actions,
                            '--ros-scene-driver-budget', 'robot', '1', '65536', '--ros-scene-driver-kernel-channel', 'robot', 'arm', 'main',
                            str(left.fileno()), str(child.pid), str(os.getuid()), str(os.getgid()), *[v.hex() for v in IDS], DIGEST.hex(),
                            '128', '128', '1000000000', str(time.monotonic_ns()+10000000000),
                            '--ros-scene-driver-schema', 'robot', 'arm', 'device.submit', '64', '64', '8',
                            '--ros-scene', 'robot', 'platform_extension', module, 'disable' if mode == 'disabled' else 'enable',
                            '--ros-scene', 'robot', 'platform_extension.bind', module, 'arm']
                        command = [str(runtime), '--wasm-set-main-module-name', 'main', '--runtime-'+engine,
                            '--runtime-compile-threads', '0', '--runtime-llvm-jit-cache-path', 'disable', *flags, '--run', str(wasm)]
                        vm = subprocess.Popen(command, pass_fds=(left.fileno(),), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                        left.close(); child.stdin.write((str(vm.pid)+'\n').encode()); child.stdin.close(); child.stdin = None
                        _, stderr = vm.communicate(timeout=20); response, errors = child.communicate(timeout=5)
                        label = f'{name}-{width}-{memory}-{engine}-{mode}'
                        (out/(label+'.stderr')).write_bytes(stderr+errors)
                        assert vm.returncode == child.returncode == 0, (label, stderr[-3000:], errors)
                        report['cases'].append(dict(name=label, passed=True, argv=command, guest_sha256=sha(wasm), peer=json.loads(response)))
                    finally:
                        left.close()
                        for process in ([vm] if vm else [])+[child]:
                            if process.poll() is None:
                                process.kill(); process.wait()
            save(); print('PASS '+name, flush=True)
        report['complete'] = all(sha(p) == digest for p, digest in report['inputs'].items())
    finally:
        report['events_after'] = (GATE/'memory.events').read_text(); save()
    assert report['complete']


if __name__ == '__main__':
    main()
