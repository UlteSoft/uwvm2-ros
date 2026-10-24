#!/usr/bin/env python3
"""Independently check closed original Doosan interface-package evidence.

This checks actual original source/tool/product bytes and registered CTest
results. It executes no native driver or Guest and never turns an empty test
inventory into an official acceptance count. Run beneath owner_doosan.py.
"""
import argparse
import hashlib
import json
from pathlib import Path
import xml.etree.ElementTree as ET


def read(path):
    return json.loads(path.read_bytes())


def pin(row):
    path = Path(row['path'])
    assert path.is_file() and path.stat().st_size == row['bytes']
    with path.open('rb') as stream:
        assert hashlib.file_digest(stream, 'sha256').hexdigest() == row['sha256']


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root = args.root.resolve(strict=True)
    inputs = read(root / 'inputs.json')
    attempts = inputs['native_attempts']
    assert set(attempts) == {'ros1_noetic_devel', 'ros2_humble'}
    assert args.out.parent == root / 'out' and not args.out.exists()
    args.out.mkdir(mode=0o700)
    rows = []
    for profile, attempt in attempts.items():
        owner = read(root / 'out' / ('doosan-native-' + attempt + '.json'))
        assert owner['status'] == 'PASS' and owner['all_admitted_source_and_tool_pins_preserved']
        assert owner['inputs']['native_profile'] == profile
        assert all(r['exit'] == 0 and r['forced_signal'] is None for r in owner['commands'])
        events = dict(line.split() for line in owner['events_after'].splitlines())
        assert all(events[k] == '0' for k in ('oom', 'oom_kill', 'oom_group_kill'))
        result_root = root / 'out' / ('native-' + attempt)
        result = read(result_root / 'native.json')
        assert result['source_variant'] == 'UNCHANGED_ORIGINAL_PACKAGE'
        assert result['status'] in (
            'ORIGINAL_NATIVE_INTERFACE_BUILD_AND_PACKAGE_TESTS_PASS',
            'ORIGINAL_NATIVE_INTERFACE_BUILD_PASS_UPSTREAM_TEST_FAILURES_RETAINED')
        for name in ('original_files', 'captured_original_files', 'sdk_tools',
                     'sdk_identity_files', 'installed_products'):
            assert result[name]
            for row in result[name]:
                pin(row)
        assert result['original_files'] == result['captured_original_files']
        for command in result['commands']:
            pin(command['stdout'])
            pin(command['stderr'])
            if command['expected_exit'] is not None:
                assert command['exit'] == command['expected_exit']
        discovered = read(result_root / 'commands/list-original-tests/stdout')['tests']
        assert len(discovered) == result['original_test_cases_discovered']
        junit = result_root / 'upstream-junit.xml'
        if discovered:
            cases = ET.fromstring(junit.read_bytes()).findall('.//testcase')
            failures = sum(c.find('failure') is not None or c.find('error') is not None for c in cases)
            skips = sum(c.find('skipped') is not None for c in cases)
            assert len(cases) == result['original_cases_run'] == len(discovered)
            assert failures == result['original_case_failures']
            assert skips == result['original_case_skips']
        else:
            assert result['original_cases_run'] == result['original_case_failures'] == result['original_case_skips'] == 0
        rows.append(dict(profile=profile, status=result['status'],
                         installed_regular_products=len(result['installed_products']),
                         original_cases_discovered=len(discovered),
                         original_cases_run=result['original_cases_run'],
                         original_case_failures=result['original_case_failures'],
                         original_case_skips=result['original_case_skips'],
                         sampled_owned_rss=max(r['max_sampled_owned_rss'] for r in owner['commands'])))
    report = dict(status='INDEPENDENT_DOOSAN_SELECTED_ORIGINAL_PACKAGE_BUILDS_VERIFIED',
                  profiles=rows, default_authority=[], native_driver_qualified=False,
                  scope='Selected original native interface-package build/install/registered tests only; no full driver/controller/robot/Runtime or hardware-performance acceptance')
    (args.out / 'native-verification.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
