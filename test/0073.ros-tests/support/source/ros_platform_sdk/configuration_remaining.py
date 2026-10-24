#!/usr/bin/env python3
"""Run actual build selectors and ABI/size/union refusal contracts on Linux.

The real production Lua executes through xmake using explicitly empty registry
fixtures. Those files are configuration inputs, never native type identities.
No compiler, Guest, controller, driver or ROS process is launched by this lane.
The independent outer owner supplies finite original-cgroup containment.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    source = Path(json.loads((root / 'inputs.json').read_bytes())['source_root'])
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    commands = [
        ('delivery', [sys.executable, '-I', '-S', '-B', source / 'test/0073.ros-tests/support/source/ros_build_options/test_platform_delivery_tools.py',
                      '--source', source, '--out', out / 'delivery']),
        ('target-elf', [sys.executable, '-I', '-S', '-B', source / 'test/0073.ros-tests/support/source/ros_build_options/test_target_elf.py',
                       '--source-root', source, '--out', out / 'target-elf']),
        ('actual-xmake', [sys.executable, '-I', '-S', '-B', source / 'test/0073.ros-tests/support/source/ros_build_options/test_xmake_vendors.py',
                         '--source-root', source, '--xmake', '/home/macromodel/.local/bin/xmake',
                         '--physical-storage', root.parent, '--out', out / 'actual-xmake'])]
    report = dict(status='RUNNING', commands=[], default_authority=[], original_official_cases_run=0,
                  scope='Actual production xmake selector plus authored target/configuration/ELF/union boundaries only')
    try:
        for label, argv in commands:
            with (out / (label + '.stdout')).open('xb') as stdout, (out / (label + '.stderr')).open('xb') as stderr:
                result = subprocess.run(list(map(str, argv)), stdout=stdout, stderr=stderr,
                                        stdin=subprocess.DEVNULL, check=False, timeout=900)
            report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=result.returncode,
                stdout_sha256=hashlib.sha256((out / (label + '.stdout')).read_bytes()).hexdigest(),
                stderr_sha256=hashlib.sha256((out / (label + '.stderr')).read_bytes()).hexdigest()))
            assert result.returncode == 0, (label, (out / (label + '.stderr')).read_text()[-2400:])
        report['status'] = 'CURRENT_ACTUAL_XMAKE_ABI_ELF_AND_SOURCE_UNION_BOUNDARIES_PASS'
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (out / 'configuration.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
