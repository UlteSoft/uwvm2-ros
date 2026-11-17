#!/usr/bin/env python3
"""Check every compiled platform foundation through the actual VM CLI.

Foundations are permission restrictions, not vendor drivers. Static validation
must never send a channel packet or create a process. The independent common
driver tests exercise invocation; this suite covers all platform selections.
"""
import argparse
import json
import os
from pathlib import Path
import re
import socket
import subprocess
import time
from run_scene_vm import GATE, verify_gate, sha
from run_driver_attachment_vm import IDS, DIGEST


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args(); verify_gate()
    build = json.loads((args.runtime.parent/'full-build-results.json').read_text())
    assert build['complete'] and build['binary_sha256'] == sha(args.runtime)
    header = Path(build['source_root'])/'src/uwvm2/imported/ros/scene/platform_module.h'
    block = re.search(r'foundation_platforms\{(.*?)\};', header.read_text(), re.S)
    assert block
    names = re.findall(r'"([a-z0-9_-]+)"', block[1])
    assert len(names) == 32 and len(set(names)) == 32
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    report = dict(complete=False, scope=__doc__, runtime_sha256=sha(args.runtime),
                  header_sha256=sha(header), cases=[], cgroup=str(GATE),
                  events_before=(GATE/'memory.events').read_text())
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    try:
        for name in names:
            module = 'uwvm_ros_v2.platform_extension.'+name+'.v1'
            for mode in ('enabled', 'disabled', 'missing-binding', 'wrong-revision'):
                left, right = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
                try:
                    for fd in (left, right): fd.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
                    flags = ['--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', 'main',
                        '--ros-scene-interface', 'robot', 'driver.connect', 'allow',
                        '--ros-scene-grant', 'robot', 'arm', 'driver', 'joint', 'registered-driver', 'driver.connect',
                        '--ros-scene-driver-budget', 'robot', '1', '65536',
                        '--ros-scene-driver-kernel-channel', 'robot', 'arm', 'main', str(left.fileno()), str(os.getpid()),
                        str(os.getuid()), str(os.getgid()), *[value.hex() for value in IDS], DIGEST.hex(),
                        '128', '128', '1000000000', str(time.monotonic_ns()+10000000000),
                        '--ros-scene-driver-schema', 'robot', 'arm', 'device.submit', '64', '64', '8']
                    selected = module if mode != 'wrong-revision' else module[:-1]+'2'
                    flags += ['--ros-scene', 'robot', 'platform_extension', selected, 'disable' if mode == 'disabled' else 'enable']
                    if mode != 'missing-binding':
                        flags += ['--ros-scene', 'robot', 'platform_extension.bind', selected, 'arm']
                    command = [str(args.runtime), *flags, '--ros-scene-check']
                    result = subprocess.run(command, pass_fds=(left.fileno(),), capture_output=True, timeout=20)
                    expected = mode in ('enabled', 'disabled')
                    assert (result.returncode == 0) == expected, (name, mode, result.stderr[-2000:])
                    right.setblocking(False)
                    try:
                        right.recv(1024)
                    except BlockingIOError:
                        pass
                    else:
                        raise AssertionError('static platform configuration produced channel I/O')
                    assert os.kill(os.getpid(), 0) is None
                    (out/(name+'-'+mode+'.stderr')).write_bytes(result.stderr)
                    report['cases'].append(dict(platform=name, mode=mode, argv=command, exit=result.returncode, no_packet=True, passed=True)); save()
                finally:
                    left.close(); right.close()
            print('PASS '+name, flush=True)
        report['complete'] = True
    finally:
        report['events_after'] = (GATE/'memory.events').read_text(); save()


if __name__ == '__main__':
    main()
