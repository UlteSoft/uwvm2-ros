#!/usr/bin/env python3
"""Generate bounded array WAT/profile sources; compile only with --compile."""
import argparse
import json
from pathlib import Path
import subprocess
import array_parameters as arrays
import run_rpc as rpc


def literal(value):
    return ''.join('\\'+format(byte, '02x') for byte in value)


def guest(version, description):
    kind, scenario = description['kind'], description['scenario']
    grants = arrays.grants(version, kind, scenario, rpc.grant)
    aliases = {cap['alias']: (8192+index*32, index*8) for index, cap in enumerate(grants)}
    operations = {'get', 'set'}
    if scenario.startswith(('local_', 'undeclare_')):
        operations |= {'declare', 'undeclare'}
    source = ['(module',
              f' (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))']
    source += [f' (import "uwvm_ros{version}_v1" "parameter_{op}" (func ${op} (param i64 i32 i64 i64 i64 i64 i64) (result i32)))'
               for op in sorted(operations)]
    source += [' (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))',
               ' (memory (export "memory") 1)']
    source += [f' (data (i32.const {offset}) "{alias}")' for alias, (offset, _) in aliases.items()]
    source += [
        ' (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))',
        ' (func $sentinels (local $i i32)',
        '  (i64.store (i32.const 64) (i64.const 1234605616436508552))',
        '  (block $done (loop $fill (br_if $done (i32.ge_u (local.get $i) (i32.const 4096)))',
        '   (i32.store (i32.add (i32.const 256) (local.get $i)) (i32.const 1515870810))',
        '   (local.set $i (i32.add (local.get $i) (i32.const 4))) (br $fill))))',
        ' (func $buffer_unchanged (local $i i32)',
        '  (block $done (loop $verify (br_if $done (i32.ge_u (local.get $i) (i32.const 4096)))',
        '   (if (i32.ne (i32.load8_u (i32.add (i32.const 256) (local.get $i))) (i32.const 90)) (then (call $exit (i32.const 201))))',
        '   (local.set $i (i32.add (local.get $i) (i32.const 1))) (br $verify))))',
        ' (func $unchanged',
        '  (if (i64.ne (i64.load (i32.const 64)) (i64.const 1234605616436508552)) (then (call $exit (i32.const 202))))',
        '  (call $buffer_unchanged))',
        ' (func $response (param $expected i32) (param $length i32) (local $i i32)',
        '  (if (i64.ne (i64.load (i32.const 64)) (i64.extend_i32_u (local.get $length))) (then (call $exit (i32.const 203))))',
        '  (block $done (loop $compare (br_if $done (i32.ge_u (local.get $i) (local.get $length)))',
        '   (if (i32.ne (i32.load8_u (i32.add (i32.const 256) (local.get $i)))',
        '                (i32.load8_u (i32.add (local.get $expected) (local.get $i)))) (then (call $exit (i32.const 204))))',
        '   (local.set $i (i32.add (local.get $i) (i32.const 1))) (br $compare)))',
        '  (block $done (loop $tail (br_if $done (i32.ge_u (local.get $i) (i32.const 4096)))',
        '   (if (i32.ne (i32.load8_u (i32.add (i32.const 256) (local.get $i))) (i32.const 90)) (then (call $exit (i32.const 205))))',
        '   (local.set $i (i32.add (local.get $i) (i32.const 1))) (br $tail))))',
    ]
    body, data, assertions = [], [], []
    next_offset = 16384

    def store(payload):
        nonlocal next_offset
        offset = next_offset
        next_offset += max(16, (len(payload)+15)//16*16)
        if next_offset > 65536:
            raise ValueError('array fixture data exceeds bounded memory')
        data.append((offset, payload))
        return offset

    def invoke(operation, alias, request=b'', error=0, response=b'', label=None):
        offset = store(request)
        body.append('  (call $sentinels)')
        call = f'(call ${operation} (i64.load (i32.const {aliases[alias][1]})) (i32.const 0) (i64.const {offset}) (i64.const {len(request)}) (i64.const 256) (i64.const 4096) (i64.const 64))'
        if error:
            body.extend([f'  (if (i32.ne {call} (i32.const {error})) (then (call $exit (i32.const 206))))',
                         '  (call $unchanged)'])
        else:
            body.append(f'  (call $check {call})')
            body.append(f'  (call $response (i32.const {store(response)}) (i32.const {len(response)}))')
        assertions.append({'operation': 'param.'+operation, 'alias': alias, 'request_hex': request.hex(),
                           'expected_error': error, 'expected_response_hex': None if error else response.hex(),
                           'rejects_preserve_output_bytes': 4096 if error else 0,
                           'rejects_preserve_length': bool(error), 'whole_response_compared': not bool(error),
                           'successful_copy_preserves_remaining_bytes': True, 'label': label})

    empty = arrays.encode(kind, [])
    multi = arrays.encode(kind, arrays.VALUES[kind])
    if scenario == 'roundtrip':
        invoke('get', 'get', response=empty);invoke('set', 'set', multi);invoke('get', 'get', response=multi)
    elif scenario == 'empty_roundtrip':
        invoke('get', 'get', response=multi);invoke('set', 'set', empty);invoke('get', 'get', response=empty)
    elif scenario == 'create':
        invoke('set', 'set', multi);invoke('get', 'get', response=multi)
    elif scenario in ('malformed', 'range'):
        rows = arrays.malformed(kind, version) if scenario == 'malformed' else arrays.outside(kind)
        for payload, error, label in rows:
            invoke('set', 'set', payload, error, label=label)
        invoke('get', 'get', response=multi)
    elif scenario in ('count_quota', 'byte_quota'):
        invoke('set', 'set', arrays.encode(kind, arrays.four(kind)) if scenario == 'count_quota' else multi, 90)
        invoke('get', 'get', response=multi)
    elif scenario in ('get_bounds', 'get_count_quota', 'get_byte_quota', 'get_mixed_native', 'get_nonfinite'):
        invoke('get', 'get', error={'get_bounds': 13, 'get_mixed_native': 95, 'get_nonfinite': 22}.get(scenario, 90))
    elif scenario == 'wrong_kind':
        invoke('get', 'get', error=95);invoke('set', 'set', multi, 95)
    elif scenario in ('operation_denied', 'dynamic_denied'):
        invoke('set', 'get' if scenario == 'operation_denied' else 'set', multi,
               13 if scenario == 'operation_denied' else 95)
        invoke('get', 'get', response=multi)
    elif scenario == 'absent_denied':
        invoke('set', 'set', multi, 95);invoke('get', 'get', error=2)
    elif scenario.startswith(('local_', 'undeclare_')):
        old = empty if scenario == 'local_roundtrip' else multi
        invoke('declare', 'declare', old);invoke('get', 'get', response=old)
        if scenario == 'local_roundtrip':
            invoke('set', 'set', multi);invoke('get', 'get', response=multi);invoke('undeclare', 'undeclare')
        else:
            invoke('undeclare', 'undeclare', error=95 if scenario == 'undeclare_wrong_kind' else 13)
            invoke('get', 'get', response=multi)
            if scenario == 'undeclare_bounds':
                allowed = arrays.encode(kind, [1])
                invoke('set', 'set', allowed);invoke('get', 'get', response=allowed);invoke('undeclare', 'undeclare')
            else:
                invoke('undeclare', 'cleanup')
        invoke('get', 'get', error=2)
    else:
        raise ValueError(scenario)
    source += [f' (data (i32.const {offset}) "{literal(payload)}")' for offset, payload in data]
    source += [' (func (export "_start")']
    source += [f'  (call $check (call $open (i32.const 0) (i64.const {offset}) (i64.const {len(alias)}) (i64.const {handle})))'
               for alias, (offset, handle) in aliases.items()]
    source += body+[' ))', '']
    return '\n'.join(source), grants, assertions


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[5])
    parser.add_argument('--compile', action='store_true')
    parser.add_argument('--wat2wasm', default='wat2wasm')
    args = parser.parse_args()
    for version in (1, 2):
        profiles = []
        for description in arrays.cases(version):
            contents, grants, assertions = guest(version, description)
            path = args.source/f'examples/ros{version}'/(description['name'].removeprefix(f'ros{version}-')+'.wat')
            path.write_text(contents)
            profiles.append({**description, 'grants': grants, 'guest_assertions': assertions})
            if args.compile:
                subprocess.run([args.wat2wasm, '--enable-all', str(path), '-o', str(path.with_suffix('.wasm'))], check=True, timeout=30)
        (args.source/f'examples/ros{version}/array_parameter_profiles.json').write_text(json.dumps(profiles, indent=2)+'\n')
        print(f'WROTE ROS{version} array WAT/profiles: {len(profiles)}; compiled={args.compile}', flush=True)


if __name__ == '__main__':
    main()
