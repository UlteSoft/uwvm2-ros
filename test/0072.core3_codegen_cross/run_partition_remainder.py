#!/usr/bin/env python3
"""Small GC checks for non-divisible parallel native-object partitions.

Run under the caller's resource cgroup. Output must be a fresh scratch directory.
No compiler installation or persistent VM cache is used.
"""
import argparse
import json
from pathlib import Path
import resource
import subprocess

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--uwvm', type=Path, required=True)
parser.add_argument('--wasm-tools', type=Path, required=True)
parser.add_argument('--output', type=Path, required=True)
a = parser.parse_args()
a.output.mkdir(parents=True, exist_ok=False)
resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
rows = []
for helpers in (2, 3, 4, 6):
    wat = ['(module (type $a (array (mut i32)))']
    for index in range(helpers):
        wat.append(f'(func (param $p (ref null $a)) local.get $p i32.const 0 '
                   f'local.get $p i32.const 0 array.get $a i32.const {index + 1} '
                   'i32.add array.set $a)')
    wat.append('(func (export "_start") (local $p (ref null $a)) '
               'i32.const 1 array.new_default $a local.set $p')
    for index in range(helpers):
        wat.append(f'local.get $p call {index}')
    wat.append(f'local.get $p i32.const 0 array.get $a '
               f'i32.const {helpers * (helpers + 1) // 2} i32.ne '
               'if unreachable end))')
    source = a.output / f'gc-{helpers}.wat'
    module = source.with_suffix('.wasm')
    source.write_text('\n'.join(wat))
    subprocess.run([str(a.wasm_tools.resolve()), 'parse', str(source),
                    '-o', str(module)], check=True)
    for workers in (0, 3, 7):
        command = [str(a.uwvm.resolve()), '-Raot', '-Rct', str(workers),
                   '-Rllvm-cache-path', 'disable', '--runtime-disable-memtag',
                   '--debug-fault-action', 'exit', '--runtime-compiler-log', 'err',
                   '-Wstart', str(helpers), '--run', str(module)]
        process = subprocess.run(command, capture_output=True, timeout=30)
        (a.output / f'gc-{helpers}-workers-{workers}.log').write_bytes(
            process.stdout + process.stderr)
        rows.append(dict(helpers=helpers, extra_workers=workers,
                         exit=process.returncode, command=command))
(a.output / 'results.json').write_text(json.dumps(rows, indent=2) + '\n')
if any(row['exit'] != 0 for row in rows):
    raise SystemExit('Parallel partition regression failed; see results.json')
print(f'PASS {len(rows)} cold AOT executions with checked GC results')
