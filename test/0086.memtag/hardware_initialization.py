#!/usr/bin/env python3
"""Run preloaded starts, deferred data and checked guest accesses on a chosen backend."""
import argparse
import hashlib
import json
import shlex
import subprocess
from pathlib import Path

import vm_matrix as m


def build(code, wide, adm_bits, *, owner=False, alias=False, globals_tag=False):
    body = b"\0" + code + b"\x0b"
    result = b"\0asm\x01\0\0\0" + m.sec(1, b"\x01\x60\0\0")
    limits = m.memory_limits(wide, maximum=1, memory32_adm=adm_bits, memory64_adm=adm_bits)
    if alias:
        result += m.sec(2, b"\x01\x05owner\x06memory\x02" + limits)
    result += m.sec(3, b"\x01\0")
    if not alias:
        result += m.sec(5, b"\x01" + limits)
    result += m.sec(7, b"\x01" + (b"\x06memory\x02\0" if owner else b"\x06_start\0\0"))
    if owner:
        result += m.sec(8, b"\0")
    result += m.sec(10, b"\x01" + m.u(len(body)) + body)
    if not owner:
        payload = b"deferred active data spans tags!"
        assert len(payload) == 32
        result += m.sec(11, b"\x01\0" + m.c(80, 64 if wide else 32) + b"\x0b" + m.u(len(payload)) + payload)
    if globals_tag:
        name = b"uwvm.memtag.linear.globals"
        result += m.sec(0, m.u(len(name)) + name + m.u(2) + m.u(1) + m.u(0) + m.u(80) + m.u(32))
    return result


def guest(wide, adm_bits, physical, alias, checked):
    code = m.Program(wide)
    bits = code.bits
    start = bits - (8 if physical and adm_bits >= 8 else 4)
    code.eq(m.op(0x20), 3 if physical else 2, bits)
    code.eq(m.op(0x23), 4, bits)
    code.eq(m.op(0x24), start, bits)
    ignored = ((1 << adm_bits) - 1) << (bits - adm_bits)
    metadata = ignored & ~(15 << start)
    # The permanent globals at [80, 112) receive a random nonzero tag.
    # Keep the fixed tag outside their four-granule exclusion neighborhood.
    probe = 1024
    pointer = metadata | (probe + 3)
    code.eq(code.n(metadata | probe) + m.mem(0x2d), 0)

    # Exercise ordinary guest stores and loads with a literal nonzero logical tag.
    # Each other ADM bit is set, including the high guest nibble in the direct layout.
    raw = pointer | (9 << start)
    code.eq(code.tagged(pointer, 9), raw, bits)
    code.eq(code.n(raw) + m.op(0x21), 9, bits)
    code.eq(code.n(raw) + m.op(0x28), pointer, bits)
    code.add(code.tags(probe, 16, 9))
    code.add(code.n(raw) + m.c(0x5a) + m.mem(0x3a))
    code.eq(code.n(raw) + m.mem(0x2d), 0x5a)
    if not checked:
        code.eq(code.n(pointer) + m.mem(0x2d), 0x5a)
    # The preloaded start tagged the alias. A global descriptor tagged the
    # independent owner before the first preload start activated all owners.
    for pos, value in enumerate(b"deferred active data spans tags!"):
        code.eq(code.n(metadata | (80 + pos)) + m.op(0x27) + m.mem(0x2d), value)
    if checked:
        code.add(code.n(80) + m.op(0x27) + m.op(0x21) + bytes([0x50 if wide else 0x45]) + b'\x04\x40\x00\x0b')
        if alias:
            code.eq(code.n(80) + m.op(0x27) + m.op(0x21), 7, bits)
    return build(code.code, wide, adm_bits, alias=alias, globals_tag=not alias)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vm', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--engines', default='int,aot')
    parser.add_argument('--backend', choices=('default', 'auto', 'software', 'arm-mte'), default='default')
    parser.add_argument('--hardware-supported', action='store_true',
                        help='expect an OS-usable Arm MTE mapping when exercising automatic selection')
    parser.add_argument('--adm-bits', type=int, nargs='+', choices=range(4, 17), default=(4, 7, 8, 16))
    parser.add_argument('--runner', default='', help='optional native VM runner prefix')
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    rows = []
    hardware = args.backend == 'arm-mte' or (args.backend in ('default', 'auto') and args.hardware_supported)
    for wide in (False, True):
        bits = 64 if wide else 32
        for k in args.adm_bits:
            owner_code = m.Program(wide)
            owner_code.add(owner_code.tags(80, 32, 7))
            owner = args.out / f'owner-{bits}-adm{k}.wasm'
            owner.write_bytes(build(owner_code.code, wide, k, owner=True))
            for alias in (False, True):
                for checked in (False, True):
                    raw = guest(wide, k, hardware and checked, alias, checked)
                    name = f'{"alias" if alias else "independent"}-{bits}-adm{k}-{int(checked)}'
                    file = args.out / (name + '.wasm')
                    file.write_bytes(raw)
                    for engine in args.engines.split(','):
                        command = [*shlex.split(args.runner), str(args.vm), '-R' + engine, '--runtime-compile-threads', '0',
                                   '--wasm-set-main-module-name', 'main']
                        if args.backend != 'default':
                            command += ['--wasm-policy-global', 'runtime.memtag-backend', args.backend]
                        if wide:
                            command += ['--wasm-policy-global', 'parser.memory64', 'on']
                        command += ['--wasm-preload-library', str(owner), 'owner']
                        if engine == 'aot':
                            command += ['--runtime-llvm-jit-cache-path', 'disable']
                        if not checked:
                            command += ['--wasm-policy-single', 'main', 'runtime.memtag-checks', 'off']
                        command += ['--run', str(file)]
                        done = subprocess.run(command, capture_output=True, timeout=90)
                        log = (done.stdout + done.stderr).decode('utf8', 'replace')
                        (args.out / (name + '-' + engine + '.log')).write_text(log)
                        expected = 127 if args.backend == 'arm-mte' and not checked else 0
                        passed = done.returncode == expected and (expected == 0 or 'unchecked caller' in log)
                        rows.append(dict(name=name, engine=engine, backend=args.backend, adm_bits=k, passed=passed,
                                         exit=done.returncode, expected=expected, command=command, sha256=hashlib.sha256(raw).hexdigest()))
                        (args.out / 'report.json').write_text(json.dumps(rows, indent=2))
                        print(name, engine, done.returncode, passed, flush=True)
    raise SystemExit(any(not row['passed'] for row in rows))


if __name__ == '__main__':
    main()
