#!/usr/bin/env python3
"""Real Linux VM launches with child-only, strictly additional seccomp denials.

No syscall wrappers are mocked. The real SDK syscall first verifies each denial
in the child before exec. Closing a denied FD is not claimed successful: process
exit is what finally retires it. Signal/core status never proves a readable core.
"""
import argparse
import ctypes
import errno
import hashlib
import json
import os
from pathlib import Path
import platform
import resource
import socket
import subprocess


class Filter(ctypes.Structure):
    _fields_ = [('code', ctypes.c_ushort), ('jt', ctypes.c_ubyte), ('jf', ctypes.c_ubyte), ('k', ctypes.c_uint)]


class Program(ctypes.Structure):
    _fields_ = [('len', ctypes.c_ushort), ('filter', ctypes.POINTER(Filter))]


def constrain(fd, denials):
    # GNU x86_64 native syscall numbers, independently checked as the precondition.
    calls = {'pidfd': (434, (), errno.EMFILE), 'passcred': (54, ((0, fd), (2, socket.SO_PASSCRED)), errno.EPERM),
             'dup': (72, ((1, 1030),), errno.EMFILE), 'shutdown': (48, ((0, fd),), errno.EPERM),
             'close': (3, ((0, fd),), errno.EPERM)}
    cells = [(0x20, 0, 0, 4), (0x15, 1, 0, 0xc000003e), (0x06, 0, 0, 0x80000000),
             (0x20, 0, 0, 0), (0x45, 0, 1, 0x40000000), (0x06, 0, 0, 0x80000000)]
    for name in denials:
        number, conditions, error = calls[name]
        cells += [(0x20, 0, 0, 0), (0x15, 0, len(conditions)*2+1, number)]
        for i, (argument, value) in enumerate(conditions):
            cells += [(0x20, 0, 0, 16+8*argument), (0x15, 0, (len(conditions)-i-1)*2+1, value)]
        cells += [(0x06, 0, 0, 0x50000 | error)]
    cells += [(0x06, 0, 0, 0x7fff0000)]
    array = (Filter*len(cells))(*(Filter(*cell) for cell in cells))
    program = Program(len(cells), array)
    libc = ctypes.CDLL(None, use_errno=True)
    libc.syscall.restype = ctypes.c_long
    parent = os.getpid()

    def child():
        if libc.syscall(157, 38, 1, 0, 0, 0) or libc.syscall(317, 1, 0, ctypes.byref(program)):
            os._exit(200)
        for name in denials:
            number, _, expected = calls[name]
            ctypes.set_errno(0)
            if name == 'pidfd': result = libc.syscall(number, parent, 0)
            elif name == 'passcred':
                value = ctypes.c_int(1)
                result = libc.syscall(number, fd, socket.SOL_SOCKET, socket.SO_PASSCRED, ctypes.byref(value), ctypes.sizeof(value))
            elif name == 'dup': result = libc.syscall(number, fd, 1030, 3)
            elif name == 'shutdown': result = libc.syscall(number, fd, socket.SHUT_RDWR)
            else: result = libc.syscall(number, fd)
            if result != -1 or ctypes.get_errno() != expected: os._exit(201)
    return child


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--vm', required=True, type=Path)
    ap.add_argument('--out', required=True, type=Path)
    args = ap.parse_args()
    if platform.system() != 'Linux' or platform.machine() != 'x86_64':
        raise SystemExit('UNSUPPORTED: actual native Linux x86_64 is required; no pass claimed')
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    args.out.mkdir(parents=True, exist_ok=False)
    wasm = args.out/'empty.wasm'
    wasm.write_bytes(bytes.fromhex('0061736d0100000001040160000003020100070a01065f737461727400000a040102000b'))
    rows = []
    for endpoint in ('--debug-jit-control-fd', '--diagnostic-control-fd'):
        for denied in ((), ('pidfd',), ('passcred',), ('dup',), ('shutdown',), ('close',), ('passcred', 'close'), ('dup', 'close')):
            host, guest = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
            with host, guest:
                name = endpoint[2:]+'-'+('-'.join(denied) or 'healthy')
                command = [str(args.vm.resolve()), '--runtime-aot', '--runtime-compile-threads', '0',
                    '--runtime-llvm-jit-cache-path', 'disable', '--debug-permissions', 'limited',
                    endpoint, str(guest.fileno()), '--wasip1-single-create', 'main', '--wasip1-single-enable', 'main',
                    '--wasm-set-main-module-name', 'main', '-Wstart', '0', '--run', str(wasm.resolve())]
                expected = 126 if set(denied) & {'pidfd', 'passcred', 'dup'} else 0
                done = subprocess.run(command, pass_fds=(guest.fileno(),), preexec_fn=constrain(guest.fileno(), denied),
                    stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30)
                (args.out/(name+'.stderr')).write_bytes(done.stderr)
                row = dict(name=name, command=command, denied=denied, exit=done.returncode, expected=expected,
                    passed=done.returncode==expected, preexec_oracle_verified=done.returncode not in (200,201),
                    core_readability_tested=False)
                rows.append(row)
                print(name, done.returncode, 'PASS' if row['passed'] else 'FAIL', flush=True)
    report = dict(binary=str(args.vm.resolve()), binary_sha256=hashlib.sha256(args.vm.read_bytes()).hexdigest(),
        cases=rows, passed=sum(x['passed'] for x in rows), total=len(rows))
    (args.out/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    return 0 if report['passed']==report['total'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
