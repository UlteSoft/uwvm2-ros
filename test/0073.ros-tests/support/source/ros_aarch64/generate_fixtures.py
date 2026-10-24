#!/usr/bin/env python3
"""Build two ROS multi-memory witnesses in a fresh output directory."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess


def wat(version):
    return r'''(module
  (import "uwvm_rosVERSION_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_rosVERSION_v1" "capability_close" (func $close (param i64) (result i32)))
  (import "uwvm_rosVERSION_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))
  (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
  (import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32) (result i32)))
  (memory (export "memory") 1) (memory 1)
  (data (memory 0) (i32.const 1) "bad")
  (data (memory 1) (i32.const 1) "pub")
  (data (memory 1) (i32.const 33) "\05\00\00\00hello")
  (data (memory 0) (i32.const 128) "WASM_PUBLISHER_OPEN\0a")
  (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
  (func $expect22 (param $e i32)
    (if (i32.ne (local.get $e) (i32.const 22)) (then (call $exit (i32.const 221)))))
  (func $delay (local $start i64)
    (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64)))
    (local.set $start (i64.load (i32.const 64)))
    (loop $wait
      (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
      (br_if $wait (i64.lt_u (i64.sub (i64.load (i32.const 72)) (local.get $start))
                           (i64.const 1000000000)))))
  (func (export "_start") (local $handle i64)
    ;; The nonzero memory has the only valid alias. All outputs are deliberately
    ;; unaligned to catch assumptions hidden by x86 hosts.
    (i64.store 1 (i32.const 9) (i64.const -1))
    (call $expect22 (call $open (i32.const 2) (i64.const 1) (i64.const 3) (i64.const 9)))
    (call $expect22 (call $open (i32.const 1) (i64.const -1) (i64.const 3) (i64.const 9)))
    (call $expect22 (call $open (i32.const 1) (i64.const 65535) (i64.const 3) (i64.const 9)))
    (call $expect22 (call $open (i32.const 1) (i64.const 1) (i64.const 3) (i64.const 65535)))
    (if (i64.ne (i64.load 1 (i32.const 9)) (i64.const -1))
      (then (call $exit (i32.const 222))))
    (call $check (call $open (i32.const 1) (i64.const 1) (i64.const 3) (i64.const 9)))
    (local.set $handle (i64.load 1 (i32.const 9)))
    (call $expect22 (call $publish (local.get $handle) (i32.const 2) (i64.const 33) (i64.const 9)))
    (call $expect22 (call $publish (local.get $handle) (i32.const 1) (i64.const -1) (i64.const 9)))
    (call $expect22 (call $publish (local.get $handle) (i32.const 1) (i64.const 65535) (i64.const 9)))
    ;; The first publish creates the native endpoint. A volatile first sample
    ;; can precede discovery; the second waits for a real native writer match.
    (call $check (call $publish (local.get $handle) (i32.const 1) (i64.const 33) (i64.const 9)))
    (call $delay)
    ;; Pause on the existing WASI stdin right until the subscriber proves a match.
    (i32.store (i32.const 200) (i32.const 128))
    (i32.store (i32.const 204) (i32.const 20))
    (call $check (call $write (i32.const 1) (i32.const 200) (i32.const 1) (i32.const 208)))
    (i32.store (i32.const 216) (i32.const 232))
    (i32.store (i32.const 220) (i32.const 1))
    (call $check (call $read (i32.const 0) (i32.const 216) (i32.const 1) (i32.const 224)))
    (if (i32.or (i32.ne (i32.load (i32.const 224)) (i32.const 1))
                (i32.ne (i32.load8_u (i32.const 232)) (i32.const 71)))
      (then (call $exit (i32.const 223))))
    ;; Exactly two accepted publishes total; native verifies exact content.
    (call $check (call $publish (local.get $handle) (i32.const 1) (i64.const 33) (i64.const 9)))
    (call $delay)
    (call $check (call $close (local.get $handle)))))
'''.replace('VERSION', str(version))


def generate(out, compiler='wat2wasm'):
    out = Path(out)
    out.mkdir(parents=True, exist_ok=False)
    rows = []
    for version in (1, 2):
        name = f'ros{version}-multi-memory-publish'
        source, binary = out/(name+'.wat'), out/(name+'.wasm')
        source.write_text(wat(version))
        command = [compiler, '--enable-all', str(source), '-o', str(binary)]
        result = subprocess.run(command, capture_output=True, timeout=30)
        (out/(name+'.compile.log')).write_bytes(result.stdout+result.stderr)
        row = {'version': version, 'name': name, 'command': command,
               'returncode': result.returncode, 'wat_sha256': hashlib.sha256(source.read_bytes()).hexdigest()}
        if result.returncode:
            raise RuntimeError('FAIL multi-memory fixture compilation: '+result.stderr.decode(errors='replace'))
        row['wasm_sha256'] = hashlib.sha256(binary.read_bytes()).hexdigest()
        rows.append(row)
    (out/'manifest.json').write_text(json.dumps({'status': 'PASS', 'scope': 'fixture compilation only', 'fixtures': rows}, indent=2)+'\n')
    return rows


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--wat2wasm', default='wat2wasm')
    args = parser.parse_args()
    generate(args.out, args.wat2wasm)
