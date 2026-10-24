#!/usr/bin/env python3
"""Independent inert Java/CSharp/JavaScript/Python/MoonBit product correspondence.

Only the adjacent reviewed data parsers are imported. No archived SDK,
controller, compiler, Wasm or native program is loaded or executed. The program
opens no network and starts no subprocess. External SDKs remain trusted inputs.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import re


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def data_module(name, filename):
    spec = importlib.util.spec_from_file_location(name, Path(__file__).with_name(filename))
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def verify(root, cohort='java'):
    assert cohort in ('java', 'csharp', 'javascript', 'python', 'moonbit')
    core = data_module('reviewed_core_reader', 'verify_portable_telemetry.py')
    ros1 = data_module('reviewed_ros1_reader', 'verify_native_ros1.py')
    build = json.loads((root / ('out/' + cohort + '-producer-results.json')).read_bytes())
    assert build['status'] == 'ACTUAL_' + cohort.upper() + '_TWO_BACKEND_PRODUCER_AND_REFUSAL_PASS'
    assert build['source_and_tool_pins_preserved'] is True and build['default_authority'] == []
    original = Path(build['inputs']['runtime']).parent.parent

    def owned(path):
        return root / Path(path).relative_to(original)

    for row in build['inputs']['pins']:
        if Path(row['path']).is_relative_to(original):
            path = owned(row['path'])
            assert path.stat().st_size == row['bytes'] and sha(path) == row['sha256']

    def check_commands(data, expected_count):
        commands = {row['label']: row for row in data['commands']}
        assert len(commands) == len(data['commands']) == expected_count
        for label, row in commands.items():
            assert row['exit'] == row.get('expected_exit', 0) and row['forced_signal'] is None
            for stream in ('stdout', 'stderr'):
                assert sha(root / 'out/commands' / label / stream) == row[stream + '_sha256']
        return commands

    commands = check_commands(build, 4 if cohort in ('javascript', 'moonbit') else 8)
    products = {row['backend']: row for row in build['artifacts']}
    assert len(build['artifacts']) == 2 and set(products) == {1, 2}
    abi = json.loads((root / 'source/sdk/origin/abi/abi-v1.json').read_bytes())['modules']
    signatures = {}
    for backend, row in products.items():
        path = owned(row['path'])
        assert row['language'] == cohort and row['fresh_compiler_product'] is True
        assert path.stat().st_size == row['bytes'] and sha(path) == row['sha256']
        namespace = 'uwvm_ros' + str(backend) + '_v1'
        functions = {item['name']: ([value['type'] for value in item['params']], item['results']) for item in abi[namespace]['functions']}
        imports = core.imports(path.read_bytes())
        found = []
        for imported_namespace, name, parameters, results in imports:
            assert imported_namespace in (namespace, 'wasi_snapshot_preview1')
            if imported_namespace == namespace:
                assert (parameters, results) == functions[name]
                found.append(name)
        assert len(found) == len(set(found)) and {'abi_version', 'capability_open', 'capability_close', 'topic_publish'} <= set(found)
        if cohort in ('csharp', 'javascript', 'python', 'moonbit'):
            assert set(found) == set(functions)
        assert ('wasi_snapshot_preview1', 'fd_read', ['i32'] * 4, ['i32']) in imports
        signatures[backend] = len(found)
        label = cohort + '-ros' + str(backend)
        directory = root / 'out' / (label + '-producer')
        producer = json.loads((directory / 'build.json').read_bytes())
        assert producer['status'] == 'BUILT_AND_VALIDATED_NOT_ROS_EXECUTED'
        assert len(producer['commands']) == {'csharp': 6, 'java': 5, 'javascript': 17, 'python': 9, 'moonbit': 6}[cohort]
        assert all(item['exit'] == 0 for item in producer['commands'])
        for item in producer['commands']:
            for stream in ('stdout', 'stderr'):
                assert sha(directory / 'commands' / item['label'] / stream) == item[stream + '_sha256']
        if cohort == 'moonbit':
            assert producer['runtime_model'] == 'Native MoonBit linear Wasm; no Host language runtime'
            assert producer['target'] == 'wasm' and producer['mode'] == 'release-strip'
            assert producer['default_authority'] == [] and producer['source_and_tool_inputs_preserved'] is True
            for pin in [producer['builder'], *producer['sources'], *producer['tools']]:
                if Path(pin['path']).is_relative_to(original):
                    current = owned(pin['path'])
                    assert current.stat().st_size == pin['bytes'] and sha(current) == pin['sha256']
            layout_pin = producer['layout']
            assert sha(owned(layout_pin['path'])) == layout_pin['sha256']
            inspector = data_module('reviewed_moonbit_reservation_reader', 'inspect_moonbit_telemetry.py')
            assert json.loads(json.dumps(inspector.inspect(path))) == json.loads(owned(layout_pin['path']).read_bytes())
            wasi = [entry for entry in imports if entry[0] == 'wasi_snapshot_preview1']
            assert len(wasi) == 3 and set((entry[1], tuple(entry[2]), tuple(entry[3])) for entry in wasi) == {
                ('fd_read', ('i32',) * 4, ('i32',)), ('fd_write', ('i32',) * 4, ('i32',)),
                ('proc_exit', ('i32',), ())}
        if cohort == 'python':
            python_reader = data_module('reviewed_python_data_reader', 'verify_python_telemetry.py')
            python_reader.verify_product(root, original, directory, producer, path, core)
        if cohort == 'javascript':
            assert producer['runtime_model'] == 'QuickJS interpreter inside Wasm; no host JS'
            assert producer['argument_profile'] == 'custom _start does not read argv'
            assert producer['default_authority'] == [] and producer['source_and_tool_inputs_preserved']
            preparation = owned(producer['preparation']['path'])
            assert sha(preparation) == producer['preparation']['sha256']
            prepared = json.loads(preparation.read_bytes())
            manifest = prepared['manifest']
            assert manifest['entry'] == 'sdk/origin/examples/javascript_telemetry/telemetry.mjs'
            assert manifest['mode'] == 'command' and manifest['backend'] == backend
            assert len(manifest['modules']) == 13 and manifest['imports'] == manifest['exports'] == []
            assert manifest['initial_memory'] == 16 << 20 and manifest['maximum_memory'] == 64 << 20
            assert prepared['implicit_permissions'] == [] and prepared['expected_ros_imports'] == len(functions)
            for pin in prepared['inputs'] + producer['compile_dependencies']:
                if Path(pin['path']).is_relative_to(original):
                    path = owned(pin['path'])
                    assert path.stat().st_size == pin['bytes'] and sha(path) == pin['sha256']
            wasi = [entry for entry in imports if entry[0] == 'wasi_snapshot_preview1']
            assert set((entry[1], tuple(entry[2]), tuple(entry[3])) for entry in wasi) == {
                ('fd_read', ('i32',) * 4, ('i32',)), ('fd_write', ('i32',) * 4, ('i32',)),
                ('proc_exit', ('i32',), ())}
            assert len(wasi) == 3
            binary = owned(row['path']).read_bytes()
            reader = core.Reader(binary)
            assert reader.take(8) == b'\0asm\x01\0\0\0'
            memories, exports = [], []
            while reader.at < len(binary):
                kind = reader.byte()
                part = core.Reader(reader.take(reader.uint()))
                assert kind != 8, 'custom command must have no implicit start section'
                if kind == 5:
                    assert part.uint() == 1 and part.byte() == 1, 'one bounded unshared wasm32 memory'
                    memories.append((part.uint(), part.uint()))
                    assert part.at == len(part.body)
                elif kind == 7:
                    for _ in range(part.uint()):
                        name, export_kind, index = part.name(), part.byte(), part.uint()
                        exports.append((name, export_kind, index))
                    assert part.at == len(part.body)
            assert memories == [(256, 1024)]
            assert {name for name, _kind, _index in exports} == {
                'memory', '_start', 'roswasm_js_init', 'roswasm_js_pump',
                'roswasm_js_status', 'roswasm_js_close'}
        if cohort == 'csharp':
            assert producer['runtime_model'] == 'Mono IL interpreter inside Wasm; not AOT'
            assert producer['private_tmp'] is True and producer['default_authority'] == []
            assert producer['tools_unchanged'] and producer['sources_unchanged'] and producer['nuget_inputs_unchanged']
            for pin in producer['tools'] + producer['sources'] + producer['nuget_inputs']:
                pinned = Path(pin['path'])
                if pinned.is_relative_to(original):
                    pinned = owned(pinned)
                    assert pinned.stat().st_size == pin['bytes'] and sha(pinned) == pin['sha256']
            bridge = producer['bridge']
            assert sha(owned(bridge['path'])) == bridge['sha256']
            items = owned(producer['native_items']['path'])
            assert sha(items) == producer['native_items']['sha256']
            assert items.read_text().splitlines().count(bridge['path'] + '|roswasm_bridge|true') == 1
            assert producer['static_pinvoke_tables']
            for table in producer['static_pinvoke_tables']:
                path = owned(table['path'])
                assert path.stat().st_size == table['bytes'] and sha(path) == table['sha256']
            assert any('roswasm_bridge' in owned(table['path']).read_text() for table in producer['static_pinvoke_tables'])
        assert (root / 'out/commands' / (label + '-default-denial') / 'stdout').read_bytes() == b'PASS_ROS_DEFAULT_DENIED\n'
        assert (root / 'out/commands' / (label + '-default-denial') / 'stderr').read_bytes() == b''
        for suffix in (() if cohort in ('javascript', 'moonbit') else ('argument-unknown', 'argument-extra')):
            command = commands[label + '-' + suffix]
            assert command['exit'] == 2
            assert all((root / 'out/commands' / command['label'] / stream).read_bytes() == b'' for stream in ('stdout', 'stderr'))
    proof1 = ros1.verify(root, cohort)
    native1 = json.loads((root / 'out/native-ros1-results.json').read_bytes())
    assert native1['cases'][0]['wasm']['sha256'] == products[1]['sha256']
    native2 = json.loads((root / 'out/native-ros2-results.json').read_bytes())
    assert native2['status'] == 'ACTUAL_' + cohort.upper() + '_NATIVE_ROS2_PASS' and native2['cohort'] == cohort
    assert native2['input_and_compile_dependencies_preserved'] is True
    check_commands(native2, 7)
    assert len(native2['cases']) == len(native2['zero_grant']) == 1
    case, zero = native2['cases'][0], native2['zero_grant'][0]
    assert case['language'] == zero['language'] == cohort
    assert case['wasm']['sha256'] == zero['wasm']['sha256'] == products[2]['sha256']
    assert case['complete_value'] == 'bounded WasmROS telemetry'
    assert case['quiet_seconds_after_zero_publishers'] == 2 and case['publisher_count_after_retirement'] == 0
    policy = case['policy']; namespace = policy['connection']['namespace']
    assert policy['backend'] == 2 and len(policy['grants']) == 1
    grant = policy['grants'][0]
    assert (grant['alias'], grant['operation'], grant['resource'], grant['type'], grant['identity']) == (
        'pub', 'topic.publish', namespace + '/telemetry', 'std_msgs/msg/String',
        'RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18')
    assert policy['connection']['security-mode'] == 'disabled-test' and policy['connection']['domain-id'] == '209'
    for positive in (True, False):
        label = cohort + '-ros2-' + ('positive' if positive else 'zero') + '-worker'
        argv = next(row['argv'] for row in native2['commands'] if row['label'] == label)
        assert '--wasip1-global-grant' not in argv
        assert argv.count('--ros2-global-grant') == int(positive)
        assert argv[argv.index('--run') + 1:] == [products[2]['path']] + ([] if cohort in ('javascript', 'moonbit') else ['publish' if positive else 'deny'])

    def output(label, stream='stdout'):
        return (root / 'out/commands' / label / stream).read_text()

    assert output(cohort + '-ros2-positive-worker') == 'READY_ROS_PUBLISH\nPUBLISHED_ROS_VALUE\nPASS_ROS_PUBLISH\n'
    lines = output(cohort + '-ros2-positive-peer').splitlines()
    derived = []
    assert len(lines) == 4 and output(cohort + '-ros2-positive-peer', 'stderr') == ''
    for phase, line in zip(('READY', 'MATCHED', 'DELIVERED', 'PASS'), lines):
        match = re.fullmatch(phase + '_NATIVE_TELEMETRY namespace=' + re.escape(namespace) + r' gateway=roswasm_language_gateway gid=([0-9a-f]{48}) warmup=([01]) delivered=([01])', line)
        assert match
        derived.append({'phase': phase, 'gid': match[1], 'warmup': int(match[2]), 'delivered': int(match[3])})
    assert derived == case['protocol'] and derived[0]['gid'] == '0' * 48 and derived[1]['gid'] != '0' * 48
    assert len({row['gid'] for row in derived[1:]}) == 1 and [row['delivered'] for row in derived] == [0, 0, 1, 1]
    assert zero['policy']['grants'] == [] and zero['native_zero_publishers'] is True and zero['quiet_seconds_after_Q'] == 2
    zero_namespace = zero['policy']['connection']['namespace']
    assert output(cohort + '-ros2-zero-worker') == 'PASS_ROS_DEFAULT_DENIED\n'
    assert output(cohort + '-ros2-zero-peer').splitlines() == [phase + '_NATIVE_TELEMETRY namespace=' + zero_namespace + ' gateway=roswasm_language_gateway gid=' + '0' * 48 + ' warmup=0 delivered=0' for phase in ('READY', 'ABSENT')]
    assert output(cohort + '-ros2-zero-peer', 'stderr') == ''
    for prefix, denials in [(cohort + '-ros2-positive', [case['denial']]), (cohort + '-ros2-zero', zero['denials'])]:
        audit = [json.loads(line) for line in output(prefix + '-worker', 'stderr').splitlines()]
        assert [row for row in audit if row.get('event') == 'unresolved-denial'] == denials
        assert len(denials) == (2 if prefix.endswith('zero') else 1)
        assert all(row['error'] == 13 and row['module'] == 'portable_ros2_' + cohort and row['ros'] == 'ros2' for row in denials)
        assert all(row.get('error') in (0, 13) for row in audit)
    for mapping in case['actual_executable_maps'].values():
        assert mapping['text'] and mapping['files']
        assert all(len(row['sha256']) == 64 and len(row['identity']) == 5 for row in mapping['files'].values())
    journal = native2['protocol_events']
    assert [row['phase'] for row in journal] == ['zero-native-quiet-interval-started', 'peer-ready',
        'worker-exec-released', 'native-graph-qos-gid-match-checked', 'P-credit-sent',
        'complete-native-value-and-gid-observed', 'A-credit-sent', 'natural-exits-and-quiet-retirement-checked']
    assert all(row['language'] == cohort for row in journal)
    assert all(a['monotonic_ns'] < b['monotonic_ns'] for a, b in zip(journal, journal[1:]))
    for data in (build, native1, native2):
        for key in ('first', 'last', 'highest_admission'):
            memory = data['memory_accounting'][key]
            allowances = []
            for statistics in (memory['first_statistics'], memory['second_statistics']):
                ordinary = max(0, statistics['file'] - statistics['shmem']
                    - statistics['file_dirty'] - statistics['file_writeback'] - statistics['file_mapped'])
                allowances.append(min(statistics['inactive_file'], ordinary))
            assert memory['charged_bytes'] == max(memory['charged_samples'])
            assert memory['cache_credit_bytes'] == min(*allowances, memory['charged_bytes'])
            assert memory['admission_bytes'] == memory['charged_bytes'] - memory['cache_credit_bytes']
            assert memory['hard_limit_bytes'] == 64 << 30 and memory['swap_limit_bytes'] == 0
            assert memory['admission_bytes'] < 63 << 30 and memory['limits_or_cache_modified'] is False
        before = dict(row.split() for row in data['events_before'].splitlines())
        after = dict(row.split() for row in data['events_after'].splitlines())
        assert all(before[key] == after[key] == '0' for key in ('oom', 'oom_kill', 'oom_group_kill'))
    return {'status': 'INDEPENDENT_' + cohort.upper() + '_TWO_BACKEND_NATIVE_DATA_VERIFICATION_PASS',
            'execution_performed': False, 'products': list(products.values()),
            'ros_signatures': signatures, 'default_denials': 2,
            'normal_argument_refusals': 0 if cohort in ('javascript', 'moonbit') else 4,
            'producer_top_level_commands': len(commands), 'native_ros1_publications': 1, 'native_ros2_publications': 1,
            'enabled_zero_resource_cases': 2, 'native_ros1_commands': 7, 'native_ros2_commands': 7,
            'ros1_data_verification': proof1,
            'scope': 'Inert actual product/import/raw-output/native-protocol correspondence only; no new execution, full API/vendor/architecture/security/performance qualification'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--work-root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--cohort', choices=('java', 'csharp', 'javascript', 'python', 'moonbit'), default='java')
    args = parser.parse_args()
    result = verify(args.work_root.resolve(strict=True), args.cohort)
    with args.out.open('x') as stream:
        json.dump(result, stream, indent=2)
        stream.write('\n')
    print(result['status'])
