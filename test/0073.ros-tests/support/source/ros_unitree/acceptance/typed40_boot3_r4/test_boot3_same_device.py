#!/usr/bin/env python3
"""Replay genuine retained family pins when artifact device81 reappeared.

These are metadata tests inside Root's actual current64GiB/swap0 leaf. They
fully reopen the genuine failed b7 report, the three native selection files
and actual profile bytes. They never modify a retained file or start a child,
compiler, Runtime, ROS/DDS participant or simulator. Invalid test candidates
are explicitly altered historical metadata, never asserted physical facts.
"""
from __future__ import annotations
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types
import unittest

MIB = 1024**2
ROOT = Path('/var/tmp/uwvm-ros-complete-20261015-r1')
MOUNT = ROOT / 'acceptance-storage-r1'
CG = {'memory_max': 64*1024**3, 'memory_swap_max': 0, 'expected_device': 29,
    'expected_inode': 18358, 'expected_boot_id': '501c54bd-af24-4d22-900c-8e24de274464',
    'expected_leaf': '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
    'expected_cpus': '0-31'}
BASE = ADAPTER = GUARD = None
CASES, PRESERVED = [], []


def need(value, reason):
    if not value: raise ValueError(reason)


def bounded(path, maximum):
    """Admit size/type before allocation, then retain stable opened full7.

    This bootstrap cannot yet invoke the reviewed restart helper. It still
    refuses aliases/special files/growing metadata and brackets both the open
    descriptor and current named entry before any candidate source executes.
    """
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, 'canonical bounded test input required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= maximum, 'bounded ordinary test input required')
        data = bytearray()
        while True:
            block = os.read(descriptor, min(65536, maximum + 1 - len(data)))
            if not block: break
            data.extend(block); need(len(data) <= maximum, 'test input grew beyond bound')
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(data) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
             for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode', 'st_nlink')),
             'test input changed during bounded bootstrap read')
        return bytes(data), {'path': str(path), 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest(),
            'device': before.st_dev, 'inode': before.st_ino, 'mtime_ns': before.st_mtime_ns, 'ctime_ns': before.st_ctime_ns}
    finally: os.close(descriptor)


def module(path, digest, name):
    data, pin = bounded(path, MIB)
    need(pin['sha256'] == digest, 'exact small reviewed source required')
    result = types.ModuleType(name); result.__file__ = str(path); sys.modules[name] = result
    exec(compile(data, str(path), 'exec'), result.__dict__)
    return result


class SameDeviceReobservations(unittest.TestCase):
    def setUp(self): GUARD()
    def tearDown(self): GUARD()

    def closure(self, case):
        value = ADAPTER.RetainedClosure.__new__(ADAPTER.RetainedClosure)
        value.restart, value.expected = BASE, {case['historical']['path']: case['historical']}
        value.same_device_evidence = {}
        value.frame = types.SimpleNamespace(guard=GUARD,
            pin=lambda path: BASE.opened_file(str(path), 16*MIB, GUARD))
        return value

    def test_three_actual_profiles_fully_reopen_with_all_seven_fields_equal(self):
        for case in CASES:
            with self.subTest(family=case['family']):
                bridge = ADAPTER._same_device_bridge(BASE, case['historical'], GUARD, case['current'])
                self.assertEqual(bridge['historical_pin'], case['historical'])
                self.assertEqual(bridge['current_pin'], case['current'])
                self.assertEqual(bridge['permitted_identity_changes'], [])
                self.assertFalse(bridge['boot_or_process_continuity_qualified'])
                self.assertEqual(bridge['execution_qualification'], 'NOT_PERFORMED')

    def test_same_device_accessor_retains_separate_original_and_current_records(self):
        for case in CASES:
            value = self.closure(case)
            self.assertEqual(value.pin(case['historical']), case['current'])
            self.assertEqual(value.pin(case['historical']), case['current'])
            self.assertEqual(len(value.same_device_evidence), 1)
            self.assertEqual(value.expected[case['historical']['path']], case['historical'])

    def test_unknown_same_name_digest_is_refused_by_the_finite_accessor(self):
        for case in CASES:
            value = self.closure(case)
            with self.assertRaisesRegex(ValueError, 'preselected finite'):
                value.pin({**case['historical'], 'sha256': '0'*64})
            self.assertEqual(value.same_device_evidence, {})

    def test_equal_device_does_not_erase_a_changed_inode(self):
        for case in CASES:
            with self.assertRaises(ValueError):
                ADAPTER._same_device_bridge(BASE, {**case['historical'], 'inode': case['historical']['inode']+1}, GUARD)

    def test_equal_device_does_not_erase_changed_timestamps(self):
        for case in CASES:
            for field in ('mtime_ns', 'ctime_ns'):
                with self.subTest(family=case['family'], field=field), self.assertRaises(ValueError):
                    ADAPTER._same_device_bridge(BASE, {**case['historical'], field: case['historical'][field]+1}, GUARD)

    def test_equal_device_does_not_erase_changed_bytes_or_digest(self):
        for case in CASES:
            for changed in ({'bytes': case['historical']['bytes']+1}, {'sha256': '0'*64}):
                with self.assertRaises(ValueError):
                    ADAPTER._same_device_bridge(BASE, {**case['historical'], **changed}, GUARD)

    def test_only_explicit_artifact81_same_device_rule_is_selected(self):
        for case in CASES:
            with self.assertRaises(ValueError):
                ADAPTER._same_device_bridge(BASE, {**case['historical'], 'device': 68}, GUARD)

    def test_distinct_paths_remain_false_without_any_file_admission(self):
        for case in CASES:
            value = self.closure(case)
            for candidate in case['outputs']:
                if candidate['path'] != case['current']['path']:
                    self.assertFalse(value.same(case['current'], candidate))
                    self.assertFalse(value.same(candidate, case['current']))
            self.assertEqual(value.same_device_evidence, {})

    def test_genuine_three_family_membership_preserves_every_original_row(self):
        for case in CASES:
            value = self.closure(case); before = copy.deepcopy(case['outputs'])
            self.assertTrue(value.member(case['current'], case['outputs']))
            self.assertEqual(case['outputs'], before)
            self.assertTrue(all(pin['device'] == 81 for pin in case['outputs']))


def main():
    global BASE, ADAPTER, GUARD, CASES
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ('helper', 'adapter'):
        parser.add_argument('--'+name, type=Path, required=True)
        parser.add_argument('--'+name+'-sha256', required=True)
    parser.add_argument('--test-sha256', required=True)
    parser.add_argument('--out', type=Path, required=True); args = parser.parse_args()
    need(sys.platform == 'linux', 'actual guarded Linux metadata regression required')
    BASE = module(args.helper, args.helper_sha256, 'typed3_same_device_base')
    source = ROOT / 'inputs-r39/source'
    raw, manifest_pin = bounded(source.parent / 'source-manifest.json', 16*MIB)
    need(hashlib.sha256(raw).hexdigest() == '54d6c79978e36bd13fa37272d8dd5f2cc94a9d2e62ac437b51dfc4aa4f6ae5b9',
         'actual complete retained Source39 probe source required')
    GUARD = BASE.CurrentGuard(str(source), json.loads(raw), CG); GUARD.maximum_seconds = 180
    before = GUARD.force(); ADAPTER = module(args.adapter, args.adapter_sha256, 'typed3_same_device_adapter')
    need(BASE.opened_file(manifest_pin['path'], 16*MIB, GUARD) == manifest_pin,
         'complete source manifest changed after actual live guard admission')
    PRESERVED.append(manifest_pin)
    own = BASE.opened_file(str(Path(__file__).resolve(strict=True)), MIB, GUARD)
    need(own['sha256'] == args.test_sha256, 'actual regression entry differs from the explicit reviewed source')
    PRESERVED.append(own)
    for path in (args.helper, args.adapter): PRESERVED.append(BASE.opened_file(str(path), MIB, GUARD))
    pin, raw = BASE.opened_file(str(MOUNT/'source40-unitree-command14-build-r1/report.json'), 8*MIB, GUARD, collect=True)
    need(pin['bytes'] == 3911428 and pin['sha256'] == 'b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747',
         'genuine complete b7 report required')
    PRESERVED.append(pin); original = json.loads(raw)
    need(original['status'] == 'FAIL', 'the original report must remain FAIL')
    old = {row['path']: row for row in original['input_pins']}
    for family in ('unitree_go', 'unitree_hg', 'unitree_api'):
        directory = MOUNT/'unitree-native-humble-build-export-20261016-r6/work'/family/'selection'
        profile = old[str(directory/'profile.json')]
        receipt = ADAPTER._same_device_bridge(BASE, old[str(directory/'preparation.json')], GUARD)
        current, raw = BASE.opened_file(str(directory/'preparation.json'), MIB, GUARD, collect=True)
        need(current == receipt['current_pin'], 'genuine native selection changed')
        outputs = json.loads(raw)['outputs']
        need(len(outputs) >= 2 and all(type(row) is dict and set(row) == ADAPTER.PIN for row in outputs) and
             [row for row in outputs if row['path'] == profile['path']] == [profile], 'complete actual selection/profile association required')
        current_profile = ADAPTER._same_device_bridge(BASE, profile, GUARD)['current_pin']
        PRESERVED.extend((current, current_profile))
        CASES.append({'family': family, 'historical': profile, 'current': current_profile, 'outputs': outputs})
    need(args.out.is_absolute() and args.out.parent.resolve(strict=True) == args.out.parent and
         not args.out.exists() and not args.out.is_symlink(), 'fresh exact metadata regression output required')
    args.out.mkdir(mode=0o700)
    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(SameDeviceReobservations))
    need(all(BASE.opened_file(pin['path'], pin['bytes'], GUARD) == pin for pin in PRESERVED), 'metadata regression input changed')
    report = {'format_version': 1, 'kind': 'typed-command-boot3-genuine-same-device-metadata-regressions',
        'status': 'PASS' if result.wasSuccessful() and result.testsRun == 9 and not result.skipped else 'FAIL',
        'tests_run': result.testsRun, 'actual_families': [case['family'] for case in CASES], 'input_pins': PRESERVED,
        'source_and_inputs_preserved': True, 'cgroup_before': before, 'cgroup_after': GUARD.force(),
        'scope': 'METADATA_ONLY_NO_COMPILER_RUNTIME_ROS_DDS_PHYSICS_OR_PERFORMANCE_QUALIFICATION',
        'historical_failed_report_promoted': False, 'commands': []}
    BASE._write_report(args.out, report)
    print(json.dumps({'status': report['status'], 'tests_run': report['tests_run'], 'report': str(args.out/'report.json')}), flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__': raise SystemExit(main())
