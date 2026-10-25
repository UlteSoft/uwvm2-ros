#!/usr/bin/env python3
"""@brief Measure matched old/new runtime paths on an actual native Linux CPU.

@details QEMU is rejected for timing. The finite guests check a deterministic
LCG checksum and exact ROS errno after every boundary. These probes separate
function calls, ROS import admission and process startup. They do not measure
DDS transport, gateway discovery, robot motion or GPU execution.
"""
import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import platform
import re
import statistics
import struct
import subprocess
import time

ANSI = re.compile(rb"\x1b\[[0-9;]*m")


def checksum(count):
    """Compose the exact unsigned 32-bit LCG in logarithmic time."""
    multiplier, increment, result, shift = 1664525, 1013904223, 1, 0
    while count:
        if count & 1:
            result, shift = result * multiplier & 0xffffffff, (shift * multiplier + increment) & 0xffffffff
        increment = increment * (multiplier + 1) & 0xffffffff
        multiplier = multiplier * multiplier & 0xffffffff
        count >>= 1
    value = (result * 17 + shift) & 0xffffffff
    return value if value < 0x80000000 else value - 0x100000000


def fixture(version, iterations):
    """Keep the arithmetic identical; add one authenticated boundary per loop."""
    imported = ""
    boundary = ""
    if version:
        imported = f'(import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))'
        boundary = '(if (i32.ne (call $open (i32.const 0) (i64.const 0) (i64.const 0) (i64.const 16)) (i32.const 22)) (then unreachable))'
    return f'''(module
  {imported}
  (memory (export "memory") 1 1)
  (func $leaf (param $value i32) (result i32)
    (i32.add (i32.mul (local.get $value) (i32.const 1664525)) (i32.const 1013904223)))
  (func $middle (param $value i32) (result i32) (call $leaf (call $leaf (local.get $value))))
  (func (export "_start") (local $count i32) (local $value i32)
    (local.set $value (i32.const 17))
    (loop $again
      (local.set $value (call $middle (local.get $value)))
      {boundary}
      (local.set $count (i32.add (local.get $count) (i32.const 1)))
      (br_if $again (i32.lt_u (local.get $count) (i32.const {iterations}))))
    (if (i32.ne (local.get $value) (i32.const {checksum(2 * iterations)})) (then unreachable))))
'''


def statistics_of(values):
    ordered = sorted(values)
    return {"count": len(values), "median_seconds": statistics.median(values),
            "p95_seconds": ordered[math.ceil(.95 * len(values)) - 1],
            "minimum_seconds": ordered[0], "maximum_seconds": ordered[-1]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("current", "baseline", "wat2wasm", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--samples", type=int, default=15)
    args = parser.parse_args()
    assert platform.system() == "Linux" and platform.machine() in ("x86_64", "aarch64")
    assert 5 <= args.samples <= 100
    assert "qemu" not in Path('/proc/self/exe').resolve().name.lower()
    args.out.mkdir(parents=True, exist_ok=False)
    cg = Path('/sys/fs/cgroup')
    cgroup = {name: (cg / name).read_text().strip() for name in ('memory.max', 'memory.swap.max', 'cgroup.procs')}
    assert cgroup['memory.max'] == '68719476736' and cgroup['memory.swap.max'] == '0'
    assert str(os.getpid()) in cgroup['cgroup.procs'].split()
    report = {'passed': False, 'scope': __doc__, 'cgroup': cgroup, 'machine': platform.machine(),
              'cpuinfo': Path('/proc/cpuinfo').read_text(), 'rows': [], 'products': {}}
    # A single allowed CPU removes migration between different core classes.
    cpu = min(os.sched_getaffinity(0))
    os.sched_setaffinity(0, {cpu})
    report['cpu_affinity'] = list(os.sched_getaffinity(0))
    for label in ('current', 'baseline'):
        path = getattr(args, label).resolve(strict=True)
        header = path.read_bytes()[:64]
        assert header[:6] == b'\x7fELF\x02\x01'
        assert struct.unpack_from('<H', header, 18)[0] == {'x86_64': 62, 'aarch64': 183}[platform.machine()]
        report['products'][label] = {'path': str(path), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}
    try:
        cases = []
        for version, iterations in ((0, 10000000), (1, 100000), (2, 100000)):
            name = 'calls' if version == 0 else f'ros{version}-admission'
            wat, wasm = args.out / (name + '.wat'), args.out / (name + '.wasm')
            wat.write_text(fixture(version, iterations))
            command = [str(args.wat2wasm), '--debug-names', str(wat), '-o', str(wasm)]
            subprocess.run(command, check=True, capture_output=True, timeout=30)
            modes = [('baseline', 'instruction'), ('current', 'unwind')]
            if version == 0:
                modes.append(('baseline', 'unwind'))
            for label, policy in modes:
                cases.append((name, iterations, wasm, label, policy))
        for repetition in range(args.samples + 2):
            # Rotate order to avoid making one variant always follow warmup.
            ordered = cases[repetition % len(cases):] + cases[:repetition % len(cases)]
            for name, iterations, wasm, label, policy in ordered:
                command = [str(getattr(args, label)), '-Raot', '-Rct', '0', '-Rllvm-policy', 'max',
                           '-Rllvm-call-stack', policy, '-Rllvm-cache-path', 'disable', '--log-verbose',
                           '--wasip1-global-noinherit-system-environment', '--run', str(wasm)]
                start = time.perf_counter_ns()
                child = subprocess.run(command, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30)
                wall = (time.perf_counter_ns() - start) / 1e9
                raw = ANSI.sub(b'', child.stderr)
                log = f'{name}-{label}-{policy}-{repetition}.log'
                (args.out / log).write_bytes(child.stderr)
                assert child.returncode == 0, (log, child.returncode)
                matched = re.findall(rb'Total WASM execution time: ([0-9.]+)s', raw)
                assert len(matched) == 1, log
                report['rows'].append({'case': name, 'runtime': label, 'policy': policy, 'repetition': repetition,
                    'warmup': repetition < 2, 'iterations': iterations, 'argv': command,
                    'execution_seconds': float(matched[0]), 'process_seconds': wall, 'raw_log': log})
        report['summary'] = {}
        for name, _, _, label, policy in cases:
            rows = [row for row in report['rows'] if not row['warmup'] and (row['case'], row['runtime'], row['policy']) == (name, label, policy)]
            report['summary'][name + '/' + label + '/' + policy] = {
                kind: statistics_of([row[kind] for row in rows]) for kind in ('execution_seconds', 'process_seconds')}
        report['passed'] = True
    finally:
        (args.out / 'result.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
