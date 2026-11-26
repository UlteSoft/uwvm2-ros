#!/usr/bin/env python3
"""Observe the real shared owner at the failed, crossing i64 store.

Run threaded_matrix.py for spawn-plain-32-cross and spawn-plain-64-cross
with --engines aot first. The Linux VM needs its native function symbols.
The report supplies the original owner, guest and CLI arguments.
"""
import json
import os
from pathlib import Path


def observe(record):
    import gdb
    gdb.Breakpoint('uwvm2::runtime::lib::ThreadedLlvmChild::memory_trap',
                   internal=True, temporary=True)
    gdb.execute('run', to_string=True)
    frame = gdb.newest_frame()
    if not frame or 'ThreadedLlvmChild::memory_trap' not in (frame.name() or ''):
        raise RuntimeError('the native bounds callback was not reached')

    page = 65536
    length = 3 * page
    inferior = gdb.selected_inferior()
    owners = set()
    for line in Path(f'/proc/{inferior.pid}/maps').read_text().splitlines():
        region, perms, *_ = line.split()
        low, high = (int(x, 16) for x in region.split('-'))
        if not perms.startswith('rw') or high-low < length:
            continue
        # The mmap is committed up to the next guard. Identify the owner by
        # the initial prefix marker and published index, independently of the
        # bytes the failing store could have changed.
        for base in {low, high-length}:
            def read(offset, count):
                return inferior.read_memory(base+offset, count).tobytes()
            if (read(2*page-8, 8) == bytes.fromhex('9887766554436281')
                    and int.from_bytes(read(8, 8), 'little') == length-10):
                owners.add(base)
    if len(owners) != 1:
        raise RuntimeError(f'expected one shared guest mapping, found {len(owners)}')
    base, = owners
    tail = inferior.read_memory(base+length-9, 9).tobytes()
    if tail != bytes.fromhex('b2a1908ffeeddccb00'):
        raise RuntimeError(f'crossing store modified the committed tail {tail.hex()}')

    # Read the detailed trap ABI where all six fields are in registers.
    args = None
    arch = frame.architecture().name()
    registers = ('rdi', 'rsi', 'rdx', 'rcx', 'r8', 'r9') if arch == 'i386:x86-64' else (
        ('x0', 'x1', 'x2', 'x3', 'x4', 'x5') if arch == 'aarch64' else ())
    if registers:
        args = [int(frame.read_register(reg)) for reg in registers]
        args[3] &= 0xffffffff
        if args != [0, 3, length-7, 0, length, 8]:
            raise RuntimeError(f'incorrect bounds callback operands {args}')
    Path(record).write_text(json.dumps(dict(tail=tail.hex(), arguments=args, arch=arch)))


def main():
    import argparse
    import subprocess
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--gdb', default='gdb')
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=False)
    rows = json.loads(args.report.read_text())
    for bits in (32, 64):
        name = f'spawn-plain-{bits}-cross'
        row, = (r for r in rows if r['name'] == name and r['engine'] == 'aot')
        if not row['passed'] or row['exit'] != 121:
            raise RuntimeError(f'{name} did not report its original bounds trap')
        record = (args.out/(name+'.json')).resolve()
        command = [args.gdb, '-nx', '-nh', '-q', '-batch', '-iex', 'set auto-load off',
                   '-iex', 'set debuginfod enabled off', '-ex', 'set disable-randomization off',
                   '-x', str(Path(__file__).resolve()), '--args', *row['command']]
        with (args.out/(name+'.log')).open('w') as log:
            subprocess.run(command, env=dict(os.environ, UWVM_TEST_STORE_RECORD=str(record)),
                           stdout=log, stderr=subprocess.STDOUT, timeout=90, check=True)
        # GDB may exit zero after a Python command error. Only the successful
        # callback inspection above writes the record in this fresh directory.
        print(name, json.loads(record.read_text()), flush=True)


if os.environ.get('UWVM_TEST_STORE_RECORD'):
    observe(os.environ['UWVM_TEST_STORE_RECORD'])
elif __name__ == '__main__':
    main()
