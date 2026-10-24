#!/usr/bin/env python3
"""Inert independent verification of the portable Wasm/native evidence tree.

No archived SDK/controller is imported. This program opens no network, starts
no subprocess and executes no compiler, Wasm or native binary. It independently
parses Core function imports and checks raw outcomes, products and native data.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

LANGUAGES = {'c', 'cpp', 'rust', 'tinygo', 'assemblyscript'}


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


class Reader:
    def __init__(self, body):
        self.body, self.at = body, 0
    def take(self, count):
        assert count >= 0 and self.at + count <= len(self.body)
        value = self.body[self.at:self.at + count]
        self.at += count
        return value
    def byte(self):
        return self.take(1)[0]
    def uint(self):
        value = 0
        for shift in range(0, 35, 7):
            byte = self.byte()
            assert shift != 28 or byte & 0xf0 == 0
            value |= (byte & 127) << shift
            if byte < 128:
                return value
        raise ValueError('unbounded u32 LEB')
    def name(self):
        return self.take(self.uint()).decode('utf-8', errors='strict')


def imports(body):
    reader = Reader(body)
    assert reader.take(8) == b'\0asm\x01\0\0\0'
    types, imported = [], []
    names = {0x7f: 'i32', 0x7e: 'i64', 0x7d: 'f32', 0x7c: 'f64',
             0x7b: 'v128', 0x70: 'funcref', 0x6f: 'externref'}
    while reader.at < len(body):
        kind = reader.byte()
        part = Reader(reader.take(reader.uint()))
        if kind == 1:
            for _ in range(part.uint()):
                assert part.byte() == 0x60
                params = [names[part.byte()] for _ in range(part.uint())]
                results = [names[part.byte()] for _ in range(part.uint())]
                types.append((params, results))
            assert part.at == len(part.body)
        elif kind == 2:
            for _ in range(part.uint()):
                namespace, name = part.name(), part.name()
                assert part.byte() == 0, 'only function imports admitted for these command products'
                params, results = types[part.uint()]
                imported.append((namespace, name, params, results))
            assert part.at == len(part.body)
    return imported


def verify(root, phase):
    build = json.loads((root / 'out/portable-results.json').read_bytes())
    assert build['status'] == 'ACTUAL_FIVE_PORTABLE_LANGUAGE_TEN_WASM_MATRIX_PASS'
    assert build['source_and_tool_pins_preserved'] is True
    original = Path(build['inputs']['runtime']).parent.parent
    def owned(path):
        return root / Path(path).relative_to(original)
    for pin in build['inputs']['pins']:
        if Path(pin['path']).is_relative_to(original):
            path = owned(pin['path'])
            assert path.stat().st_size == pin['bytes'] and sha(path) == pin['sha256']
    rows = build['artifacts']
    assert len(rows) == 10 and {(row['language'], row['backend']) for row in rows} == {(lang, backend) for lang in LANGUAGES for backend in (1, 2)}
    commands = {row['label']: row for row in build['commands']}
    assert len(commands) == len(build['commands']) == 60
    def command(row):
        assert row['exit'] == row.get('expected_exit', 0) and row['forced_signal'] is None
        for stream in ('stdout', 'stderr'):
            assert sha(root / 'out/commands' / row['label'] / stream) == row[stream + '_sha256']
    for row in commands.values():
        command(row)
    abi = json.loads((root / 'source/sdk/origin/abi/abi-v1.json').read_bytes())['modules']
    products = {}
    for row in rows:
        key = (row['language'], row['backend'])
        path = owned(row['path'])
        assert row['fresh_compiler_product'] is True and path.stat().st_size == row['bytes'] and sha(path) == row['sha256']
        expected_module = 'uwvm_ros' + str(row['backend']) + '_v1'
        functions = {function['name']: ([item['type'] for item in function['params']], function['results']) for function in abi[expected_module]['functions']}
        imported = imports(path.read_bytes())
        ros = []
        for namespace, name, params, results in imported:
            assert namespace in (expected_module, 'wasi_snapshot_preview1')
            if namespace == expected_module:
                assert (params, results) == functions[name]
                ros.append(name)
        assert len(ros) == len(set(ros)) and {'abi_version', 'capability_open', 'capability_close', 'topic_publish'} <= set(ros)
        label = row['language'] + '-ros' + str(row['backend'])
        assert (root / 'out/commands' / (label + '-default-denial') / 'stdout').read_bytes() == b'PASS_ROS_DEFAULT_DENIED\n'
        assert (root / 'out/commands' / (label + '-default-denial') / 'stderr').read_bytes() == b''
        for suffix in ('unknown', 'extra'):
            refused = commands[label + '-argument-' + suffix]
            assert refused['exit'] == 2
            assert (root / 'out/commands' / refused['label'] / 'stdout').read_bytes() == (root / 'out/commands' / refused['label'] / 'stderr').read_bytes() == b''
        products[key] = {'language': row['language'], 'backend': row['backend'], 'sha256': row['sha256'], 'ros_function_signatures': len(ros)}
    result = {'status': 'INDEPENDENT_PORTABLE_WASM_DATA_VERIFICATION_PASS', 'execution_performed': False,
              'wasms': list(products.values()), 'default_denials': 10, 'normal_argument_refusals': 20,
              'build_commands': 60, 'scope': 'Raw outcomes/products/full ROS function signatures; native scope remains separate'}
    if phase == 'wasm':
        return result
    native1 = json.loads((root / 'out/native-ros1-results.json').read_bytes())
    proof1 = json.loads((root / 'out/independent-ros1-verification.json').read_bytes())
    assert native1['status'] == 'ACTUAL_FIVE_LANGUAGE_NATIVE_ROS1_DELIVERY_PASS'
    assert proof1['status'] == 'INDEPENDENT_FIVE_LANGUAGE_ROS1_DATA_VERIFICATION_PASS'
    assert {row['language'] for row in native1['cases']} == {row['language'] for row in proof1['cases']} == LANGUAGES
    for case in native1['cases']:
        assert case['wasm']['sha256'] == products[case['language'], 1]['sha256']
        assert next(row for row in proof1['cases'] if row['language'] == case['language'])['wasm_sha256'] == case['wasm']['sha256']
    native2 = json.loads((root / 'out/native-ros2-results.json').read_bytes())
    assert native2['status'] == 'ACTUAL_FIVE_PORTABLE_LANGUAGE_NATIVE_ROS2_PASS'
    assert native2['input_and_compile_dependencies_preserved'] is True
    assert len(native2['commands']) == 31
    for row in native2['commands']:
        command(row)
    assert len(native2['cases']) == len(native2['zero_grant']) == 5
    assert {row['language'] for row in native2['cases']} == {row['language'] for row in native2['zero_grant']} == LANGUAGES
    expected_phases = ['zero-native-quiet-interval-started', 'peer-ready', 'worker-exec-released',
                       'native-graph-qos-gid-match-checked', 'P-credit-sent',
                       'complete-native-value-and-gid-observed', 'A-credit-sent',
                       'natural-exits-and-quiet-retirement-checked']
    native2_commands = {row['label']: row for row in native2['commands']}
    for case in native2['cases']:
        lang = case['language']
        assert case['wasm']['sha256'] == products[lang, 2]['sha256']
        policy = case['policy']
        assert policy['backend'] == 2 and len(policy['grants']) == 1
        grant = policy['grants'][0]
        assert grant['alias'] == 'pub' and grant['operation'] == 'topic.publish' and grant['type'] == 'std_msgs/msg/String'
        assert grant['identity'] == 'RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18'
        namespace = policy['connection']['namespace']
        assert grant['resource'] == namespace + '/telemetry'
        assert policy['connection']['security-mode'] == 'disabled-test' and policy['connection']['domain-id'] == '209'
        assert case['complete_value'] == 'bounded WasmROS telemetry' and case['quiet_seconds_after_zero_publishers'] == 2 and case['publisher_count_after_retirement'] == 0
        prefix = lang + '-ros2-positive'
        def output(label, stream='stdout'):
            return (root / 'out/commands' / label / stream).read_text()
        assert output(prefix + '-worker') == 'READY_ROS_PUBLISH\nPUBLISHED_ROS_VALUE\nPASS_ROS_PUBLISH\n'
        audit = [json.loads(line) for line in output(prefix + '-worker', 'stderr').splitlines()]
        assert [row for row in audit if row.get('event') == 'unresolved-denial'] == [case['denial']]
        assert case['denial']['error'] == 13 and case['denial']['module'] == 'portable_ros2_' + lang
        lines = output(prefix + '-peer').splitlines()
        assert len(lines) == 4 and output(prefix + '-peer', 'stderr') == ''
        derived = []
        for step, line in zip(('READY', 'MATCHED', 'DELIVERED', 'PASS'), lines):
            match = re.fullmatch(step + '_NATIVE_TELEMETRY namespace=' + re.escape(namespace) + r' gateway=roswasm_language_gateway gid=([0-9a-f]{48}) warmup=([01]) delivered=([01])', line)
            assert match, line
            derived.append({'phase': step, 'gid': match[1], 'warmup': int(match[2]), 'delivered': int(match[3])})
        assert derived == case['protocol'] and derived[0]['gid'] == '0' * 48
        assert derived[1]['gid'] != '0' * 48 and len({row['gid'] for row in derived[1:]}) == 1
        assert [row['delivered'] for row in derived] == [0, 0, 1, 1]
        zero = next(row for row in native2['zero_grant'] if row['language'] == lang)
        assert zero['wasm']['sha256'] == products[lang, 2]['sha256'] and zero['policy']['grants'] == []
        assert zero['native_zero_publishers'] is True and zero['quiet_seconds_after_Q'] == 2
        zero_prefix = lang + '-ros2-zero'
        assert output(zero_prefix + '-worker') == 'PASS_ROS_DEFAULT_DENIED\n'
        zero_ns = zero['policy']['connection']['namespace']
        assert output(zero_prefix + '-peer').splitlines() == [step + '_NATIVE_TELEMETRY namespace=' + zero_ns + ' gateway=roswasm_language_gateway gid=' + '0' * 48 + ' warmup=0 delivered=0' for step in ('READY', 'ABSENT')]
        assert output(zero_prefix + '-peer', 'stderr') == ''
        zero_audit = [json.loads(line) for line in output(zero_prefix + '-worker', 'stderr').splitlines()]
        assert [row for row in zero_audit if row.get('event') == 'unresolved-denial'] == zero['denials']
        assert len(zero['denials']) == 2 and all(row['error'] == 13 for row in zero['denials'])
        journal = [row for row in native2['protocol_events'] if row['language'] == lang]
        assert [row['phase'] for row in journal] == expected_phases
        assert all(a['monotonic_ns'] < b['monotonic_ns'] for a, b in zip(journal, journal[1:]))
        argv = native2_commands[prefix + '-worker']['argv']
        assert argv.count('--ros2-global-grant') == 1 and '--wasip1-global-grant' not in argv
        assert argv[argv.index('--run') + 1:] == [case['wasm']['path'], 'publish']
    result.update(status='INDEPENDENT_PORTABLE_WASM_ROS1_ROS2_DATA_VERIFICATION_PASS',
                  native_ros1_publications=5, native_ros2_publications=5,
                  enabled_zero_resource_cases=10, ros1_native_commands=27, ros2_native_commands=31,
                  scope='Inert independent product/import/output/status/ROS1-proof/ROS2-policy/GID/absence/ordered-retirement correspondence; no new execution or performance credit')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--work-root', type=Path, required=True)
    parser.add_argument('--phase', choices=('wasm', 'all'), default='all')
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    result = verify(args.work_root.resolve(strict=True), args.phase)
    with args.out.open('x') as stream:
        json.dump(result, stream, indent=2)
        stream.write('\n')
    print(result['status'])
