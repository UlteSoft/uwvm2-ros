#!/usr/bin/env python3
"""Guest service clients verify the exact native Trigger/SetBool response wire."""
from pathlib import Path
ROOT=Path(__file__).resolve().parents[5]
for version in (1,2):
 for kind in ('trigger','setbool'):
  request_length=1 if kind=='setbool' else 0
  guest=f'''(module
 (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros{version}_v1" "service_call" (func $call (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1) (data (i32.const 0) "client") (data (i32.const 8192) "\\01")
 (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
 (func (export "_start")
  (call $check (call $open (i32.const 0) (i64.const 0) (i64.const 6) (i64.const 8)))
  (call $check (call $call (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const {request_length}) (i64.const 128) (i64.const 4096) (i64.const 16)))
  (if (i64.ne (i64.load (i32.const 16)) (i64.const 10)) (then (call $exit (i32.const 217))))
  (if (i32.ne (i32.load8_u (i32.const 128)) (i32.const 1)) (then (call $exit (i32.const 218))))
  (if (i32.ne (i32.load (i32.const 129)) (i32.const 5)) (then (call $exit (i32.const 219))))
  (if (i32.ne (i32.load (i32.const 133)) (i32.const 1819043176)) (then (call $exit (i32.const 220))))
  (if (i32.ne (i32.load8_u (i32.const 137)) (i32.const 111)) (then (call $exit (i32.const 221))))))
'''
  (ROOT/f'examples/ros{version}/service_client_{kind}.wat').write_text(guest)
