#!/usr/bin/env python3
"""Write reviewed WAT test sources; never compile or execute WebAssembly.

Run from this directory's complete checkout. Actual wat2wasm compilation and
Guest/ROS execution are separate Linux-cgroup controller steps. Canonical bytes
come from the independent Python struct specification, not generated codecs.
"""
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parent
PAYLOADS = {
    1: struct.pack('<iBd4B3h', -12345, 1, -1.25, 0, 127, 128, 255, -32768, 0, 32767),
    2: struct.pack('<IBf3d8B2h', 0x12345678, 1, 23.5, -1.25, 0.0, 99.5,
                   0, 1, 127, 128, 200, 253, 254, 255, -32768, 32767),
}


def source(version, payload):
    literal = ''.join('\\'+format(byte, '02x') for byte in payload)
    return f'''(module
  ;; This Guest receives only explicitly granted pub/sub aliases and a monotonic
  ;; WASI clock. The compiled provider does not create these capabilities.
  (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros{version}_v1" "capability_close" (func $close (param i64) (result i32)))
  (import "uwvm_ros{version}_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))
  (import "uwvm_ros{version}_v1" "topic_subscribe" (func $subscribe (param i64) (result i32)))
  (import "uwvm_ros{version}_v1" "event_poll" (func $poll (param i64 i32 i64 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))
  (import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32) (result i32)))
  (memory (export "memory") 1)
  (data (i32.const 0) "pubsub")
  ;; Expected canonical scalar bits. No native TCPROS/CDR framing or C padding.
  (data (i32.const 8192) "{literal}")
  (func $check (param $e i32)
    (if (local.get $e) (then (call $exit (local.get $e)))))
  (func $delay (param $duration i64) (local $start i64)
    (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64)))
    (local.set $start (i64.load (i32.const 64)))
    (loop $wait
      (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
      (br_if $wait (i64.lt_u (i64.sub (i64.load (i32.const 72)) (local.get $start)) (local.get $duration)))))
  (func (export "_start") (local $error i32) (local $start i64) (local $index i32)
    ;; The controller proves the live ARM Worker mapping before releasing one
    ;; byte through standard input. fd 0 is the baseline stdin capability; this
    ;; gate adds no filesystem, socket or other WASI permission.
    (i32.store (i32.const 48) (i32.const 80))
    (i32.store (i32.const 52) (i32.const 1))
    (call $check (call $read (i32.const 0) (i32.const 48) (i32.const 1) (i32.const 56)))
    (if (i32.or (i32.ne (i32.load (i32.const 56)) (i32.const 1))
                 (i32.ne (i32.load8_u (i32.const 80)) (i32.const 71)))
      (then (call $exit (i32.const 205))))
    ;; Each memory index/pointer/length is explicit in the versioned raw ABI.
    (call $check (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)))
    (call $check (call $open (i32.const 0) (i64.const 3) (i64.const 3) (i64.const 16)))
    (call $check (call $subscribe (i64.load (i32.const 16))))
    ;; A noncanonical bool must be denied before native publication, even when
    ;; the explicitly selected validator is "none".
    (i32.store8 (i32.const 8196) (i32.const 2))
    (if (i32.ne (call $publish (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const {len(payload)})) (i32.const 22))
      (then (call $exit (i32.const 203))))
    (i32.store8 (i32.const 8196) (i32.const 1))
    (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 64)))
    (local.set $start (i64.load (i32.const 64)))
    (block $received (loop $exchange
      (call $check (call $publish (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const {len(payload)})))
      (call $delay (i64.const 100000000))
      (local.set $error (call $poll (i64.load (i32.const 16)) (i32.const 0) (i64.const 128) (i64.const 4096) (i64.const 24)))
      (br_if $received (i32.eqz (local.get $error)))
      (if (i32.ne (local.get $error) (i32.const 11)) (then (call $exit (local.get $error))))
      (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 72)))
      (if (i64.gt_u (i64.sub (i64.load (i32.const 72)) (local.get $start)) (i64.const 10000000000))
        (then (call $exit (i32.const 110))))
      (br $exchange)))
    ;; Compare every received byte; a successful polling status alone is not
    ;; evidence that the native adapter preserved the custom fixed record.
    (if (i64.ne (i64.load (i32.const 24)) (i64.const {len(payload)})) (then (call $exit (i32.const 201))))
    (block $equal (loop $compare
      (br_if $equal (i32.ge_u (local.get $index) (i32.const {len(payload)})))
      (if (i32.ne (i32.load8_u (i32.add (i32.const 128) (local.get $index)))
                   (i32.load8_u (i32.add (i32.const 8192) (local.get $index))))
        (then (call $exit (i32.const 202))))
      (local.set $index (i32.add (local.get $index) (i32.const 1)))
      (br $compare)))
    ;; Closing one alias releases its native resource; a stale publication
    ;; handle must remain invalid rather than reusing another grant.
    (call $check (call $close (i64.load (i32.const 8))))
    (if (i32.ne (call $publish (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const {len(payload)})) (i32.const 9))
      (then (call $exit (i32.const 204))))
    (call $check (call $close (i64.load (i32.const 16))))
    (call $exit (i32.const 0))))
'''


if __name__ == '__main__':
    for version, payload in PAYLOADS.items():
        (ROOT/f'loopback_ros{version}.wat').write_text(source(version, payload))
        print('PREPARED WAT SOURCE', version, len(payload), payload.hex())
