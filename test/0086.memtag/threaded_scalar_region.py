"""Exercise a real ADM16 reader, a concurrent tag writer/grower, and a checked import."""
import argparse
import json
import subprocess
from pathlib import Path

import vm_matrix as wasm
from threaded_matrix import spawn_and_wait
from vm_diagnostics import fault_matches

HEADER = b'\0asm\x01\0\0\0'
LIMITS = wasm.memory_limits(wide=True, shared=True)
READY, FINISHED, POINTER = 512, 520, 528
OBJECT = 320


def body(code, locals_=b'\0'):
    encoded = locals_ + code + b'\x0b'
    return wasm.u(len(encoded)) + encoded


def reader():
    module = HEADER + wasm.sec(1, bytes.fromhex('01 60 01 7e 01 7f'))
    module += wasm.sec(2, b'\x01\x03env\x06memory\x02' + LIMITS)
    module += wasm.sec(3, b'\x01\0') + wasm.sec(7, b'\x01\x04read\0\0')
    return module + wasm.sec(10, b'\x01' + body(b'\x20\0' + wasm.mem(0x28)))


def spin_on(position):
    # The worker must be able to publish through a real exclusive claimz
    # while its peer loops on ordinary scalar reads from the same owner.
    return b'\x03\x40' + wasm.c(position, 64) + wasm.mem(0x28) + b'\x45\x0d\0\x0b'


def guest(negative):
    main = wasm.Program(wide=True)
    child = wasm.Program(wide=True)
    main.add(main.n(OBJECT) + wasm.c(0x12345678) + wasm.mem(0x36))
    spawn_and_wait(main, while_running=main.n(READY) + wasm.c(1) + wasm.mem(0x36) + spin_on(FINISHED))
    tagged = main.n(POINTER) + wasm.mem(0x29)
    main.eq(tagged + b'\x10\x02', 0)  # claimz zeroed the prior nonzero word.
    main.eq(main.n(65536) + wasm.mem(0x28), 77)
    main.add(tagged + wasm.c(91) + wasm.mem(0x36))
    main.eq(tagged + b'\x10\x02', 91)
    if negative == 'stale':
        main.eq(main.n(OBJECT) + b'\x10\x02', 91)
    if negative == 'bounds':
        main.add(main.n(131061) + wasm.c(0x56a7a1cbd09e3211, 64) + wasm.mem(0x37))
        main.add(main.n(131068) + wasm.c(-1, 64) + wasm.mem(0x37))
    if negative == 'second-tag':
        main.add(tagged + main.n(8) + b'\x7c' + wasm.c(0x5ab2731469dec8f1, 64) + wasm.mem(0x37))
        main.add(tagged + main.n(13) + b'\x7c' + wasm.c(-1, 64) + wasm.mem(0x37))

    child.add(spin_on(READY))
    child.add(child.n(POINTER) + child.n(OBJECT) + child.n(16) + wasm.op(0x41) + wasm.mem(0x37))
    child.eq(child.n(1) + b'\x40\0', 1, 64)
    child.add(child.n(65536) + wasm.c(77) + wasm.mem(0x36))
    child.add(child.n(FINISHED) + wasm.c(1) + wasm.mem(0x36))
    child.add(child.n(4096) + wasm.c(1) + b'\xfe\x17\x02\0')
    child.add(child.n(4096) + wasm.c(1) + b'\xfe\0\x02\0\x1a')

    signatures = bytes.fromhex('05 60 00 00 60 02 7f 7f 00 60 01 7f 00 60 01 7f 01 7f 60 01 7e 01 7f')
    module = HEADER + wasm.sec(1, signatures) + wasm.sec(2, b'\x04\x03env\x06memory\x02' + LIMITS +
        b'\x16wasi_snapshot_preview1\x09proc_exit\0\x02\x04wasi\x0cthread-spawn\0\x03\x04peer\x04read\0\x04')
    module += wasm.sec(3, b'\x02\0\x01') + wasm.sec(7, b'\x03\x06_start\0\x03\x11wasi_thread_start\0\x04\x06memory\x02\0')
    return module + wasm.sec(10, b'\x02' + body(main.code, b'\x01\x01\x7f') + body(child.code))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vm', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    owner, peer = out / 'owner.wasm', out / 'peer.wasm'
    owner.write_bytes(HEADER + wasm.sec(5, b'\x01' + LIMITS) + wasm.sec(7, b'\x01\x06memory\x02\0'))
    peer.write_bytes(reader())
    for case, caller_checks, peer_checks, expected in (
        ('good', 'off', 'on', 0), ('stale', 'off', 'off', 0), ('stale', 'off', 'on', 122),
        ('bounds', 'off', 'on', 121), ('second-tag', 'on', 'off', 122),
    ):
        label = f'{case}-{caller_checks}-{peer_checks}'
        module = out / (case + '.wasm')
        module.write_bytes(guest(case))
        cmd = [str(args.vm.resolve()), '-Raot', '-Rct', '0', '-Rllvm-cache-path', 'disable',
               '--log-verbose', '--log-color', 'disable', '--debug-fault-action', 'exit']
        for key, value in [('parser.baseline', 'wasm3'), ('parser.threads', 'on'), ('parser.memory64', 'on'),
                           ('parser.adm', 'on'), ('parser.memtag', 'on'), ('runtime.llvm.policy', 'balanced'),
                           ('runtime.memtag-backend', 'software'), ('runtime.memtag-checks', caller_checks)]:
            cmd.extend(['--wasm-policy-global', key, value])
        cmd += ['--wasm-policy-single', 'peer', 'runtime.memtag-checks', peer_checks,
                '--wasi-threads', '3', '4', '1048576', '16777216', '67108864', '131072',
                '--wasm-set-main-module-name', 'main', '--wasip1-single-create', 'main', '--wasip1-single-enable', 'main',
                '--wasip1-single-interface', 'main', 'proc_exit', 'allow', '--wasip1-single-grant', 'main', 'proc-exit',
                '--wasithread-single-create', 'main', '--wasithread-single-enable', 'main',
                '--wasithread-single-interface', 'main', 'thread-spawn', 'allow',
                '--wasithread-single-limit', 'main', 'max-extra-threads', '3',
                '--wasithread-single-limit', 'main', 'max-spawns', '4',
                '--wasm-preload-library', str(owner), 'env', '--wasm-preload-library', str(peer), 'peer',
                '--run', str(module)]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        (out / (label + '.stderr')).write_text(result.stderr)
        (out / (label + '.json')).write_text(json.dumps(dict(argv=cmd, exit=result.returncode), indent=2))
        ok = result.returncode == 0 if not expected else fault_matches(result.returncode, result.stderr, expected,
            'out of bounds' if expected == 121 else 'memory tag mismatch')
        if result.stdout or not ok:
            raise RuntimeError(f'{label} exited {result.returncode}. See {out / (label + ".stderr")}')
        print(label, result.returncode, flush=True)


if __name__ == '__main__':
    main()
