#!/usr/bin/env python3
"""Independently verify retained integration data without executing artifacts.

Only this checked-in verifier runs. No archived SDK/controller is imported, and
no compiler, Wasm, native ROS process, subprocess or network request is started.
The selected external native SDK/toolchain stays a separately admitted input.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import stat

ORIGINAL = Path('/home/macromodel/Documents/uwvm-test-261018/build/ros-language-integration-20261019-r46')


class Reader:
    def __init__(self, data):
        self.data, self.at = data, 0

    def take(self, count):
        if count < 0 or self.at+count > len(self.data):
            raise ValueError('truncated binary')
        result = self.data[self.at:self.at+count]
        self.at += count
        return result

    def byte(self):
        return self.take(1)[0]

    def uint(self):
        value = 0
        for shift in range(0, 35, 7):
            byte = self.byte()
            if shift == 28 and byte & 0xf0:
                raise ValueError('u32 LEB overflow')
            value |= (byte & 127) << shift
            if byte < 128:
                return value
        raise ValueError('unbounded LEB')

    def name(self):
        return self.take(self.uint()).decode('utf-8', errors='strict')


def numeric_imports(body):
    """Separate small Core parser for the four inspection-only numeric probes."""
    reader = Reader(body)
    if reader.take(8) != b'\0asm\x01\0\0\0':
        raise ValueError('Core Wasm v1 required')
    types, imports = [], []
    names = {0x7f:'i32', 0x7e:'i64', 0x7d:'f32', 0x7c:'f64',
             0x7b:'v128', 0x70:'funcref', 0x6f:'externref'}
    while reader.at < len(body):
        section = reader.byte()
        item = Reader(reader.take(reader.uint()))
        if section == 1:
            for _ in range(item.uint()):
                if item.byte() != 0x60:
                    raise ValueError('probe uses an unexpected non-function type')
                params = [names[item.byte()] for _ in range(item.uint())]
                results = [names[item.byte()] for _ in range(item.uint())]
                types.append((params, results))
            assert item.at == len(item.data)
        elif section == 2:
            for _ in range(item.uint()):
                module, name, kind = item.name(), item.name(), item.byte()
                if kind != 0:
                    raise ValueError('numeric probe imported a non-function object')
                params, results = types[item.uint()]
                imports.append((module, name, params, results))
            assert item.at == len(item.data)
    return imports


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--work-root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root = args.work_root.resolve(strict=True)
    def relative(path):
        value = Path(path)
        return root/value.relative_to(ORIGINAL) if value.is_absolute() else root/value
    def read(path, maximum=64 << 20):
        target = relative(path)
        if not target.resolve(strict=True).is_relative_to(root):
            raise ValueError('data path escapes the owned root')
        info = target.lstat()
        if not stat.S_ISREG(info.st_mode) or info.st_size > maximum:
            raise ValueError('bounded ordinary data file required')
        return target.read_bytes()
    def data(path):
        return json.loads(read(path))
    def sha(body):
        return hashlib.sha256(body).hexdigest()
    def actor(row):
        label = row['label']
        assert re.fullmatch('[a-z0-9-]+', label)
        folder = 'out/commands/'+label+'/'
        return read(folder+'stdout'), read(folder+'stderr')
    matrix = data('out/all-language-results.json')
    assert matrix['passed'] and len(matrix['artifacts']) == 30
    products = {(a['language'], a['backend']):a for a in matrix['artifacts']}
    assert len(products) == 30 and len({lang for lang, _ in products}) == 15
    for artifact in products.values():
        body = read(artifact['restored_path'])
        assert len(body) == artifact['bytes'] and sha(body) == artifact['sha256']
    for row in matrix['outcomes']:
        lang = row['language']
        expected_status = 13 if lang in ('c','cpp','rust','tinygo','assemblyscript') else 0
        if lang == 'javascript':
            expected = (13).to_bytes(4, 'little')+bytes(4)
        elif expected_status == 13:
            expected = b''
        elif lang in ('java','csharp','python','moonbit'):
            expected = b'PASS_JOINT_STATE_DEFAULT_DENIED\n'
        else:
            expected = b'PASS_ROS_DEFAULT_DENIED\n'
        stdout, stderr = actor(row)
        assert row['exit'] == expected_status and row['guard_failure'] is None
        assert stdout == expected and stderr == b''
        actual = row['argv'][row['argv'].index('--run')+1]
        assert actual == products[(lang, row['backend'])]['restored_path']
    assert len(matrix['outcomes']) == 42
    for row in matrix['invalid_argument_outcomes']:
        stdout, stderr = actor(row)
        assert row['exit'] == 2 and row['guard_failure'] is None
        assert stdout == b'' and stderr == b''
    assert len(matrix['invalid_argument_outcomes']) == 20
    failed = matrix['original_failed_argument_actor']
    assert failed['exit'] == -4 and 'kotlin' in failed['label']
    inventory = data('out/inventory-policy-results.json')
    assert inventory['passed'] and len(inventory['artifacts']) == 4
    catalog = data('source/sdk/origin/abi/abi-v1.json')
    signatures = 0
    for artifact in inventory['artifacts']:
        assert artifact['executed'] is False
        body = read(artifact['path'])
        assert sha(body) == artifact['sha256']
        module = 'uwvm_ros'+str(artifact['backend'])+'_v1'
        expected = {f['name']:([p['type'] for p in f['params']], f['results'])
                    for f in catalog['modules'][module]['functions']}
        actual = {}
        for mod, name, params, results in numeric_imports(body):
            assert mod in (module, 'wasi_snapshot_preview1')
            if mod == module:
                assert name not in actual
                actual[name] = (params, results)
        assert actual == expected
        signatures += len(actual)
    assert signatures == 162
    native_reports = [data('out/native-results-r4.json'), data('out/native-results-kotlin-final.json'),
                      data('out/native-results-go-synced.json')]
    native_cases = {}
    for report in native_reports:
        for row in report['commands']:
            assert row['exit'] == 0 and row['forced_signal'] is None
        for case in report['cases']:
            lang = case['language']
            rows = {r['label']:r for r in report['commands']}
            worker_labels = [name for name in rows if name.startswith(lang+'-') and name.endswith('-native-worker')]
            assert len(worker_labels) == 1
            prefix = worker_labels[0][:-len('worker')]
            worker, stderr = actor(rows[prefix+'worker'])
            peer, peer_error = actor(rows[prefix+'peer'])
            assert worker == b'READY_ROS_PUBLISH\nPUBLISHED_ROS_VALUE\nPASS_ROS_PUBLISH\n'
            assert peer_error == b''
            audits = [json.loads(line) for line in stderr.splitlines()]
            denies = [row for row in audits if row.get('event') == 'unresolved-denial']
            assert len(denies) == 1 and denies[0]['error'] == 13
            assert all(row.get('error') in (0,13) for row in audits)
            assert case['complete_value'] == 'bounded WasmROS telemetry'
            lines = peer.decode().splitlines()
            assert len(lines) == 4
            values = []
            for phase, line in zip(('READY','MATCHED','DELIVERED','PASS'), lines):
                match = re.fullmatch(phase+r'_NATIVE_TELEMETRY namespace=([^ ]+) gateway=roswasm_language_gateway gid=([0-9a-f]{48}) warmup=([01]) delivered=([01])', line)
                assert match
                values.append(match.groups())
            assert values[0][1] == '0'*48 and values[1][1] != '0'*48
            assert len({x[0] for x in values}) == 1 and len({x[1] for x in values[1:]}) == 1
            assert [x[3] for x in values] == ['0','0','1','1']
            assert case['quiet_seconds'] == 2 and case['publisher_count_after_retirement'] == 0
            native_cases[lang] = case
    assert set(native_cases) == {'go','zig','fsharp','vbnet','kotlin'}
    for lang, case in native_cases.items():
        assert case['wasm']['sha256'] == products[(lang,2)]['sha256']
    result = {'passed':True, 'scope':'Independent inert data verification; no retained program executed',
              'languages':15, 'selected_wasms':30, 'selected_default_denials':42,
              'selected_normal_argument_refusals':20, 'full_numeric_signatures':162,
              'selected_native_ros2_languages':sorted(native_cases),
              'original_kotlin_signal_failure_retained':True,
              'external_native_sdk_and_toolchains_reexecuted':False}
    args.out.write_text(json.dumps(result, indent=2)+'\n')
    print(json.dumps(result))


if __name__ == '__main__':
    main()
