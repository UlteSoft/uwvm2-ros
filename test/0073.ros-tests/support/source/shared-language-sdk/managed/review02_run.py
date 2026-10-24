#!/usr/bin/env python3
"""Run two source-only suites under one already admitted finite Linux owner."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--xmake', type=Path)
    parser.add_argument('--dotnet', type=Path)
    parser.add_argument('--control', action='store_true')
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('shared admitted Linux owner required')
    args.out.mkdir(parents=True, exist_ok=False)
    source = args.source.resolve(strict=True)
    files = []
    for path in sorted(source.rglob('*')):
        if not path.is_file():
            continue
        files.append(dict(path=str(path.relative_to(source)), bytes=path.stat().st_size,
            sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
    report = dict(format='uwvm-managed-review02-source-run-v1', inputs=files, commands=[],
        compiler_cases=0, guest_cases=0, native_ros_cases=0, default_authority=[])
    adapter_suite = source / 'test/0073.ros-tests/support/source/shared-language-sdk/managed/adapters/check_sources.py'
    if not adapter_suite.is_file():
        adapter_suite = source / 'sdk/origin/language/managed/test/check_sources.py'
    suites = [('consumers', source / 'test/0073.ros-tests/support/source/shared-language-sdk/managed/review02_acceptance.py'),
        ('adapters', adapter_suite)]
    try:
        for name, script in suites:
            argv = [sys.executable, '-I', '-S', '-B', str(script), '--source', str(source), '--out', str(args.out / name)]
            with (args.out / (name + '.stdout')).open('xb') as out, (args.out / (name + '.stderr')).open('xb') as err:
                result = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=out, stderr=err, check=False)
            report['commands'].append(dict(name=name, argv=argv, exit=result.returncode))
        if args.xmake:
            argv = [str(args.xmake), 'l', str(Path(__file__).with_name('profile_review02.lua')),
                str(source), str(args.out / 'xmake-local.json')]
            with (args.out / 'xmake.stdout').open('xb') as out, (args.out / 'xmake.stderr').open('xb') as err:
                result = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=out, stderr=err, check=False)
            report['commands'].append(dict(name='xmake-local', argv=argv, exit=result.returncode))
        if args.control:
            argv = [sys.executable, '-I', '-S', '-B', str(Path(__file__).with_name('control_source_review02.py')),
                '--source', str(source), '--out', str(args.out / 'control')]
            if args.dotnet:
                argv += ['--dotnet', str(args.dotnet)]
            with (args.out / 'control.stdout').open('xb') as out, (args.out / 'control.stderr').open('xb') as err:
                result = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=out, stderr=err, check=False)
            report['commands'].append(dict(name='control-source', argv=argv, exit=result.returncode))
        report['status'] = 'SOURCE_SUITES_PASS' if all(r['exit'] == 0 for r in report['commands']) else 'FAILED_RETAINED'
        return 0 if report['status'] == 'SOURCE_SUITES_PASS' else 1
    finally:
        (args.out / 'run.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    raise SystemExit(main())
