#!/usr/bin/env python3
"""Generate exact typed ParameterEvent WAT/profiles; compile only by explicit opt-in."""
import argparse
import json
from pathlib import Path
import subprocess
import run_rpc as rpc
import typed_parameter_events as events


def literal(value):
    return ''.join('\\'+format(byte, '02x') for byte in value)


def guest(description):
    kind, scenario = description['kind'], description['scenario']
    grants = events.grants(description, rpc.scalar_grant)
    expected = [] if scenario in ('permission-denied', 'no-capability') else events.expected_events(kind, scenario)
    source = ['(module',
              ' (import "uwvm_ros2_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))',
              ' (import "uwvm_ros2_v1" "parameter_subscribe_events" (func $subscribe (param i64) (result i32)))',
              ' (import "uwvm_ros2_v1" "event_poll" (func $poll (param i64 i32 i64 i64 i64) (result i32)))',
              ' (import "uwvm_ros2_v1" "capability_revoke" (func $revoke (param i64) (result i32)))',
              ' (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))',
              ' (import "wasi_snapshot_preview1" "clock_time_get" (func $clock (param i32 i64 i32) (result i32)))',
              ' (memory (export "memory") 1)', ' (global $start (mut i64) (i64.const 0))',
              ' (data (i32.const 8192) "cap")', ' (data (i32.const 8208) "marker")']
    source += [f' (data (i32.const {16384+index*256}) "{literal(value)}")'
               for index, value in enumerate(expected)]
    source += [
        ' (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))',
        ' (func $sentinels (local $i i32)',
        '  (i64.store (i32.const 64) (i64.const 1234605616436508552))',
        '  (block $done (loop $fill (br_if $done (i32.ge_u (local.get $i) (i32.const 4096)))',
        '   (i32.store (i32.add (i32.const 256) (local.get $i)) (i32.const 1515870810))',
        '   (local.set $i (i32.add (local.get $i) (i32.const 4))) (br $fill))))',
        ' (func $unchanged (local $i i32)',
        '  (if (i64.ne (i64.load (i32.const 64)) (i64.const 1234605616436508552)) (then (call $exit (i32.const 201))))',
        '  (block $done (loop $verify (br_if $done (i32.ge_u (local.get $i) (i32.const 4096)))',
        '   (if (i32.ne (i32.load8_u (i32.add (i32.const 256) (local.get $i))) (i32.const 90)) (then (call $exit (i32.const 202))))',
        '   (local.set $i (i32.add (local.get $i) (i32.const 1))) (br $verify))))',
        ' (func $begin',
        '  (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 128)))',
        '  (global.set $start (i64.load (i32.const 128))))',
        ' (func $elapsed (result i64)',
        '  (call $check (call $clock (i32.const 1) (i64.const 1) (i32.const 136)))',
        '  (i64.sub (i64.load (i32.const 136)) (global.get $start)))',
        ' (func $next (param $h i64) (result i32) (local $e i32)',
        '  (call $sentinels)',
        '  (local.set $e (call $poll (local.get $h) (i32.const 0) (i64.const 256) (i64.const 4096) (i64.const 64)))',
        '  (if (i32.eq (local.get $e) (i32.const 11)) (then (call $unchanged))) (local.get $e))',
        ' (func $reject_phase (param $h i64) (local $tries i32)',
        '  (call $begin)',
        '  (block $done (loop $wait',
        '   (if (i32.ne (call $next (local.get $h)) (i32.const 11)) (then (call $exit (i32.const 203))))',
        '   (local.set $tries (i32.add (local.get $tries) (i32.const 1)))',
        '   (if (i32.ge_u (local.get $tries) (i32.const 100000)) (then (call $exit (i32.const 110))))',
        '   (br_if $done (i64.ge_u (call $elapsed) (i64.const 1000000000))) (br $wait))))',
        ' (func $await (param $h i64) (param $expected i32) (param $length i32)',
        '  (local $e i32) (local $i i32) (local $tries i32) (call $begin)',
        '  (block $received (loop $wait',
        '   (local.set $e (call $next (local.get $h)))',
        '   (br_if $received (i32.eqz (local.get $e)))',
        '   (if (i32.ne (local.get $e) (i32.const 11)) (then (call $exit (local.get $e))))',
        '   (local.set $tries (i32.add (local.get $tries) (i32.const 1)))',
        '   (if (i32.ge_u (local.get $tries) (i32.const 100000)) (then (call $exit (i32.const 110))))',
        '   (if (i64.gt_u (call $elapsed) (i64.const 10000000000)) (then (call $exit (i32.const 110)))) (br $wait)))',
        '  (if (i64.ne (i64.load (i32.const 64)) (i64.extend_i32_u (local.get $length))) (then (call $exit (i32.const 204))))',
        '  (block $done (loop $compare (br_if $done (i32.ge_u (local.get $i) (local.get $length)))',
        '   (if (i32.ne (i32.load8_u (i32.add (i32.const 256) (local.get $i)))',
        '                (i32.load8_u (i32.add (local.get $expected) (local.get $i)))) (then (call $exit (i32.const 205))))',
        '   (local.set $i (i32.add (local.get $i) (i32.const 1))) (br $compare))))',
        ' (func (export "_start")']
    if scenario == 'no-capability':
        source += ['  (i64.store (i32.const 0) (i64.const 1234605616436508552))',
                   '  (if (i32.ne (call $open (i32.const 0) (i64.const 8192) (i64.const 3) (i64.const 0)) (i32.const 13)) (then (call $exit (i32.const 206))))',
                   '  (if (i64.ne (i64.load (i32.const 0)) (i64.const 1234605616436508552)) (then (call $exit (i32.const 207))))']
    else:
        source += ['  (call $check (call $open (i32.const 0) (i64.const 8192) (i64.const 3) (i64.const 0)))']
        if scenario == 'permission-denied':
            source += ['  (if (i32.ne (call $subscribe (i64.load (i32.const 0))) (i32.const 13)) (then (call $exit (i32.const 208))))',
                       '  (call $sentinels)',
                       '  (if (i32.ne (call $poll (i64.load (i32.const 0)) (i32.const 0) (i64.const 256) (i64.const 4096) (i64.const 64)) (i32.const 13)) (then (call $exit (i32.const 209))))',
                       '  (call $unchanged)']
        else:
            source += ['  (call $check (call $subscribe (i64.load (i32.const 0))))']
            handle = 0
            if scenario == 'bytes':
                source += ['  (call $check (call $open (i32.const 0) (i64.const 8208) (i64.const 6) (i64.const 8)))',
                           '  (call $check (call $subscribe (i64.load (i32.const 8))))']
                handle = 8
            if scenario != 'roundtrip':
                source += ['  (call $reject_phase (i64.load (i32.const 0)))']
            source += [f'  (call $await (i64.load (i32.const {handle})) (i32.const {16384+index*256}) (i32.const {len(value)}))'
                       for index, value in enumerate(expected)]
            if scenario == 'bytes':
                source += ['  (if (i32.ne (call $next (i64.load (i32.const 0))) (i32.const 11)) (then (call $exit (i32.const 210))))']
            source += [f'  (call $check (call $revoke (i64.load (i32.const {handle}))))',
                       '  (call $sentinels)',
                       f'  (if (i32.ne (call $poll (i64.load (i32.const {handle})) (i32.const 0) (i64.const 256) (i64.const 4096) (i64.const 64)) (i32.const 9)) (then (call $exit (i32.const 211))))',
                       '  (call $unchanged)']
    source += [' ))', '']
    assertions = {'all_event_bytes_compared': True, 'expected_event_hex': [value.hex() for value in expected],
                  'poll11_preserves_all_output_bytes': 4096, 'poll11_preserves_length': True,
                  'max_poll_attempts_per_phase': 100000, 'positive_deadline_seconds': 10,
                  'rejected_phase_seconds': 0 if scenario == 'roundtrip' else 1,
                  'native_kind_checked_before_typed_delete': True}
    return '\n'.join(source), grants, assertions


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument('--compile', action='store_true')
    parser.add_argument('--wat2wasm', default='wat2wasm')
    args = parser.parse_args()
    profiles = []
    for description in events.cases():
        contents, grants, assertions = guest(description)
        path = args.source/'examples/ros2'/(description['name'].removeprefix('ros2-')+'.wat')
        path.write_text(contents)
        profiles.append({**description, 'grants': grants, 'guest_assertions': assertions})
        if args.compile:
            subprocess.run([args.wat2wasm, '--enable-all', str(path), '-o', str(path.with_suffix('.wasm'))], check=True, timeout=30)
    (args.source/'examples/ros2/typed_parameter_event_profiles.json').write_text(json.dumps(profiles, indent=2)+'\n')
    print(f'WROTE typed ParameterEvent WAT/profiles: {len(profiles)}; compiled={args.compile}', flush=True)


if __name__ == '__main__':
    main()
