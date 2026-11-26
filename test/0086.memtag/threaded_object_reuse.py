"""Check Linux x86-64 group-object reuse with a live shared Wasm64 tag shadow."""
import argparse
import re
import subprocess
from pathlib import Path

import vm_matrix as wasm
from threaded_matrix import spawn_and_wait, worker_module
from vm_diagnostics import fault_matches


def guest(limits, tagged, stale, reference):
    main = wasm.Program(wide=True)
    child = wasm.Program(wide=True)
    global_get, global_set = b'\x23\0', b'\x24\0'
    def pointer(tag):
        return main.tagged(128, tag) if tagged else main.n((tag << 60) | 128)

    main.add(wasm.c(17) + global_set)
    if tagged:
        main.add(main.tags(128, 16, 3))
    main.add(pointer(3) + wasm.c(41) + wasm.mem(0x36))
    spawn_and_wait(main)
    if tagged:
        main.eq(main.n(128) + wasm.op(0x27) + wasm.op(0x21), 5, 64)
    main.add(main.n(4096) + wasm.c(0) + b'\xfe\x17\x02\0')
    spawn_and_wait(main, argument=1)
    main.eq(pointer(5) + wasm.mem(0x28), 73)
    main.eq(global_get, 17)

    child.eq(global_get, 7)
    child.add(wasm.c(31) + global_set)
    child.add(b'\x20\x01\x45\x04\x40')  # if spawn argument == 0
    child.eq(pointer(3) + wasm.mem(0x28), 41)
    if tagged:
        child.add(child.tags(128, 16, 5))
    child.add(b'\x05')
    if tagged:
        child.eq(child.n(128) + wasm.op(0x27) + wasm.op(0x21), 5, 64)
    child.add(pointer(3 if stale else 5) + wasm.c(73) + wasm.mem(0x36))
    child.add(b'\x0b')
    if reference:
        child.add(b'\xd0\x70\xd1\x1a')  # ref.is_null(ref.null func), then drop
    return worker_module(main, child, limits, initial_global=7)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vm', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    for label, bits, checks, stale, reference in (
        ('adm-only', 16, False, False, False), ('checked', 16, True, False, False), ('checked-stale', 16, True, True, False),
        ('unchecked-stale', 16, False, True, False), ('exact-ir', 16, True, False, True),
        ('other-geometry', 8, True, False, False),
    ):
        tagged = label != 'adm-only'
        limits = wasm.memory_limits(wide=True, shared=True, memory64_adm=bits) if tagged else bytes([0x17, bits, 1, 2])
        owner, module = out / f'owner-{label}.wasm', out / f'{label}.wasm'
        owner.write_bytes(b'\0asm\x01\0\0\0' + wasm.sec(5, b'\x01' + limits) +
                          wasm.sec(7, b'\x01\x06memory\x02\0'))
        module.write_bytes(guest(limits, tagged, stale, reference))
        command = [args.vm.resolve(), '-Raot', '-Rct', '0', '-Rllvm-cache-path', 'disable',
                   '--debug-fault-action', 'exit', '--log-color', 'disable', '--log-verbose']
        for policy, value in [('parser.baseline', 'wasm3'), ('parser.threads', 'on'), ('parser.memory64', 'on'),
                              ('parser.adm', 'on'), ('parser.memtag', 'on'), ('runtime.llvm.policy', 'balanced'),
                              ('runtime.memtag-backend', 'software'), ('runtime.memtag-checks', 'on' if checks else 'off')]:
            command += ['--wasm-policy-global', policy, value]
        command += ['--wasi-threads', '3', '4', '1048576', '16777216', '67108864', '131072',
                    '--wasm-set-main-module-name', 'main', '--wasip1-single-create', 'main', '--wasip1-single-enable', 'main',
                    '--wasip1-single-interface', 'main', 'proc_exit', 'allow', '--wasip1-single-grant', 'main', 'proc-exit',
                    '--wasithread-single-create', 'main', '--wasithread-single-enable', 'main',
                    '--wasithread-single-interface', 'main', 'thread-spawn', 'allow',
                    '--wasithread-single-limit', 'main', 'max-extra-threads', '3',
                    '--wasithread-single-limit', 'main', 'max-spawns', '4', '--wasm-preload-library', owner, 'env', '--run', module]
        result = subprocess.run(command, capture_output=True, text=True, timeout=45)
        log = out / f'{label}.stderr'
        log.write_text(result.stderr)
        events = re.findall(r'WASI Threads LLVM group object (hit|miss|fallback)\b([^\n]*)', result.stderr)
        expected = ['miss', 'hit', 'hit'] if bits == 16 else []
        passed = (fault_matches(result.returncode, result.stderr, 122, 'memory tag mismatch')
                  if checks and stale else result.returncode == 0)
        if (not passed or result.stdout or [kind for kind, _ in events] != expected or
                any(('instance-template' in detail) != (not reference) for kind, detail in events if kind == 'hit')):
            raise RuntimeError(f'{label} returned {result.returncode}, cache events {events!r}, see {log}')
        print(label, result.returncode, *[kind for kind, _ in events])


if __name__ == '__main__':
    main()
