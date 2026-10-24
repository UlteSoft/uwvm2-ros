#!/usr/bin/env python3
"""Run explicit selected SDK lanes serially, each under its independent owner.

This operator-side controller changes only the next test's vendor selection.
It refuses live owners, existing attempt paths, changed tool/source pins and a
different cgroup. It never imports a vendor SDK or grants a Guest capability.
Every child owner supplies its own finite process-tree, time and memory bounds.
The batch stops on the first failure and retains all original child evidence.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource
import subprocess
import sys
import time

VENDORS = ('deep_robotics', 'pal', 'trossen', 'agibot', 'ubtech', 'fourier')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--phase', choices=('values', 'managed', 'cross', 'finish', 'endian'), required=True)
    parser.add_argument('--vendor', action='append', choices=VENDORS, required=True)
    parser.add_argument('--attempt', choices=('r1', 'r2', 'r3'), default='r1')
    args = parser.parse_args()
    assert len(args.vendor) == len(set(args.vendor)) <= len(VENDORS)
    root = args.root.resolve(strict=True)
    resource.setrlimit(resource.RLIMIT_AS, (256 << 20, 256 << 20))
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    inputs = json.loads((root / 'inputs.json').read_bytes())
    group = Path(inputs['cgroup']['path'])
    owner = Path(inputs['source_root']) / 'test/0073.ros-tests/support/source/ros_platform_sdk/owner_remaining.py'
    identity = hashlib.sha256(owner.read_bytes()).hexdigest()
    report_path = root / 'control' / ('batch-' + args.phase + '-' + args.attempt + '.json')
    assert not report_path.exists()
    report = dict(status='RUNNING', phase=args.phase, vendors=args.vendor,
                  commands=[], default_authority=[], started_monotonic_ns=time.monotonic_ns())

    def guard():
        assert Path('/proc/self/cgroup').read_text().strip() == '0::' + inputs['cgroup']['membership']
        assert Path('/proc/sys/kernel/random/boot_id').read_text().strip() == inputs['boot_id']
        assert [group.stat().st_dev, group.stat().st_ino] == inputs['cgroup']['identity']
        assert (group / 'memory.max').read_text().strip() == str(64 << 30)
        assert (group / 'memory.swap.max').read_text().strip() == '0'
        assert hashlib.sha256(owner.read_bytes()).hexdigest() == identity
        for path in (root / 'out').glob('remaining-*.json'):
            row = json.loads(path.read_bytes())
            assert row['status'] != 'RUNNING'
            assert all(command['exit'] is not None for command in row['commands'])

    def save():
        report_path.write_text(json.dumps(report, indent=2) + '\n')

    save()
    try:
        for vendor in args.vendor:
            guard()
            attempt = vendor.replace('_', '-') + '-' + args.attempt
            assert not (root / 'out' / ('remaining-' + args.phase + '-' + attempt + '.json')).exists()
            assert not (root / 'out' / (args.phase + '-' + attempt)).exists()
            direct_name = 'deep' if vendor == 'deep_robotics' else vendor.replace('_', '-')
            direct = root / 'out' / ('qualify-' + direct_name +
                                     ('-r2' if vendor == 'agibot' else '-r1'))
            qualification = json.loads((direct / 'qualification.json').read_bytes())
            assert qualification['status'] == 'SELECTED_VENDOR_C_CPP_COMPLETE_DATA_AND_DEFAULT_REFUSAL_PASS'
            assert qualification['vendor'] == vendor
            current = json.loads((root / 'inputs.json').read_bytes())
            current.update(selected_vendor=vendor, selected_direct_result=str(direct))
            (root / 'inputs.json').write_text(json.dumps(current, indent=2) + '\n')
            argv = [sys.executable, '-I', '-S', '-B', str(owner), '--root', str(root),
                    '--phase', args.phase, '--attempt', attempt]
            result = subprocess.run(argv, stdin=subprocess.DEVNULL, check=False, timeout=3700)
            report['commands'].append(dict(vendor=vendor, argv=argv, exit=result.returncode))
            save()
            assert result.returncode == 0, vendor
            guard()
        report['status'] = 'EXPLICIT_SELECTED_OWNER_LANES_SERIAL_PASS'
        print(report['status'], args.phase, flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        report['closed_monotonic_ns'] = time.monotonic_ns()
        save()


if __name__ == '__main__':
    main()
