#!/usr/bin/env python3
"""Guarded pure protocol checks; these never count as native DDS acceptance.

The test imports only an explicitly hashed ordinary source helper after checking
the current original64GiB cgroup. It launches no child, compiles nothing, sends
no message and instantiates no Runtime. Root may execute it in its guarded
Linux lane; authoring or AST parsing this file is not a test result.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import sys
import types


def require(value, reason):
    if not value:
        raise ValueError(reason)


def metadata(path, maximum):
    """Read stable bounded ordinary JSON before using a control path."""
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical metadata file required')
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= maximum, 'bounded ordinary metadata required')
        raw = os.read(descriptor, maximum + 1)
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        require(len(raw) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                for key in ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'metadata changed')
        return raw
    finally:
        os.close(descriptor)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--helper', type=Path, required=True)
    parser.add_argument('--helper-sha256', required=True)
    parser.add_argument('--cgroup-control', type=Path, required=True,
                        help='Ordinary JSON with the exact seven Frame cgroup-selection fields.')
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    require(sys.platform == 'linux', 'actual Linux guarded checks required')
    control = json.loads(metadata(args.cgroup_control, 65536))
    require(set(control) == {'memory_max', 'memory_swap_max', 'expected_boot_id', 'expected_leaf',
                            'expected_device', 'expected_inode', 'expected_cpus'}, 'closed exact current cgroup required')
    leaf = Path(control['expected_leaf'])
    def guard():
        require(control['memory_max'] == 68719476736 and control['memory_swap_max'] == 0 and
                Path('/proc/sys/kernel/random/boot_id').read_text().strip() == control['expected_boot_id'],
                'current original64GiB boot identity required')
        facts = leaf.stat()
        require(facts.st_dev == control['expected_device'] and facts.st_ino == control['expected_inode'] and
                int((leaf / 'memory.max').read_text()) == 68719476736 and
                int((leaf / 'memory.swap.max').read_text()) == 0 and
                (leaf / 'cpuset.cpus.effective').read_text().strip() == control['expected_cpus'] and
                str(os.getpid()) in (leaf / 'cgroup.procs').read_text().split() and
                Path('/proc/self/cgroup').read_text().strip() == '0::' + str(leaf).removeprefix('/sys/fs/cgroup'),
                'test process must remain in the selected exact cgroup')
        for parent in (leaf, *[p for p in leaf.parents if p.is_relative_to('/sys/fs/cgroup')]):
            event_path = parent / 'memory.events'
            if event_path.exists():
                events = dict(line.split() for line in event_path.read_text().splitlines())
                require(all(int(events.get(key, '0')) == 0 for key in ('oom', 'oom_kill', 'oom_group_kill')),
                        'ancestor OOM observation invalidates protocol checks')
    guard()
    path = args.helper
    require(path.is_absolute() and path.resolve(strict=True) == path, 'physical ordinary helper required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= 1024**2, 'bounded helper source required')
        raw = os.read(fd, 1024**2 + 1)
        after, named = os.fstat(fd), path.stat(follow_symlinks=False)
        require(len(raw) == before.st_size and hashlib.sha256(raw).hexdigest() == args.helper_sha256 and
                all(getattr(before, key) == getattr(after, key) == getattr(named, key) for key in
                    ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'complete actual helper changed')
    finally:
        os.close(fd)
    guard()
    helper = types.ModuleType('typed_protocol_checks_source')
    helper.__file__ = str(path)
    exec(compile(raw, str(path), 'exec'), helper.__dict__)
    checks = []
    def check(name, operation):
        guard()
        operation()
        checks.append({'name': name, 'status': 'PASS_PURE_PROTOCOL_UNIT_CHECK'})
    def equal(actual, expected):
        require(actual == expected, 'pure expected value differs')
    def refuses(operation):
        try:
            operation()
        except ValueError:
            return
        raise ValueError('invalid protocol input was accepted')
    check('GO_all60_binary32_words', lambda: equal(len(helper.carrier(20)), 60))
    check('HG_all105_binary32_words', lambda: equal(len(helper.carrier(35)), 105))
    check('first_q_dq_tau_exact_binary32', lambda: equal(helper.carrier(20)[:3], [0x3d000000, 0xbc800000, 0x3c000000]))
    check('last_HG_q_dq_tau_exact_binary32', lambda: equal(helper.carrier(35)[-3:], [0x3f8c0000, 0xbf0c0000, 0x3e8c0000]))
    check('unsupported_carrier_refused', lambda: refuses(lambda: helper.carrier(21)))
    check('all_u32_boundaries', lambda: equal(helper.words('X 0 4294967295', 'X', 2), [0, 0xffffffff]))
    for name, line in [('negative', 'X -1'), ('overflow', 'X 4294967296'), ('leading_zero', 'X 01'),
                       ('plus_sign', 'X +1'), ('double_space', 'X  1'), ('tab', 'X\t1'),
                       ('trailing_space', 'X 1 '), ('wrong_marker', 'Y 1'), ('extra_word', 'X 1 2')]:
        check('refuse_' + name, lambda line=line: refuses(lambda: helper.words(line, 'X', 1)))
    check('denied_real_quiet_shape', lambda: equal(helper.quiet_record('NATIVE_TYPED_QUIET 1 30 2000000000 0 0 0', 1, False)['states'], 0))
    check('positive_HG_quiet_shape', lambda: equal(helper.quiet_record('NATIVE_TYPED_QUIET 2 31 2300000000 1 0 0', 2, True)['states'], 1))
    check('positive_API_quiet_shape', lambda: equal(helper.quiet_record('NATIVE_TYPED_QUIET 3 31 2300000000 0 1 1', 3, True)['responses'], 1))
    check('elapsed64_above_u32_supported', lambda: equal(helper.quiet_record('NATIVE_TYPED_QUIET 3 100 9000000000 0 1 1', 3, True)['elapsed_ns'], 9000000000))
    for name, line, family, positive in [
        ('denied_payload', 'NATIVE_TYPED_QUIET 1 30 2000000000 1 0 0', 1, False),
        ('missing_API_response', 'NATIVE_TYPED_QUIET 3 30 2000000000 0 1 0', 3, True),
        ('short_quiet', 'NATIVE_TYPED_QUIET 1 30 1999999999 0 0 0', 1, False),
        ('too_few_samples', 'NATIVE_TYPED_QUIET 1 29 2000000000 0 0 0', 1, False),
        ('unbounded_quiet', 'NATIVE_TYPED_QUIET 1 30 10000000001 0 0 0', 1, False),
        ('wrong_family', 'NATIVE_TYPED_QUIET 2 30 2000000000 0 0 0', 1, False)]:
        check('refuse_' + name, lambda line=line, family=family, positive=positive:
              refuses(lambda: helper.quiet_record(line, family, positive)))
    guard()
    require(args.out.is_absolute() and args.out.resolve() == args.out and args.out.is_relative_to('/tmp') and
            not args.out.exists() and not args.out.is_symlink(), 'fresh protocol-check report under/tmp required')
    report = {'status': 'PASS_PROTOCOL_UNIT_CHECKS_ONLY_TRANSPORT_NOT_EXECUTED', 'helper_sha256': args.helper_sha256,
              'checks': checks, 'check_count': len(checks), 'actual_Runtime_cases_executed': 0,
              'native_DDS_cases_executed': 0, 'cgroup': control}
    with args.out.open('x', encoding='utf-8') as stream:
        stream.write(json.dumps(report, indent=2) + '\n')
    guard()
    print(report['status'], len(checks))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
