#!/usr/bin/env python3
"""Qualify actual CLI input-clear policy and warning ordering in the 64 GiB gate.

This checks configuration only. The frame, hostile-FP and live-DAP suites
separately check byte retention, mandatory delivery cleanup and observation.
"""
import argparse
import json
from pathlib import Path
import subprocess

from run_scene_vm import ROOT, GATE, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    runtime, out = args.runtime.resolve(), args.output.resolve()
    assert any(out.is_relative_to(folder.resolve()) for folder in (ROOT/'.tmp', ROOT.parent/'uwvm2-ros-sdk/.tmp'))
    out.mkdir(parents=True, exist_ok=False)
    build = runtime.parent/'full-build-results.json'
    evidence = json.loads(build.read_text())
    assert evidence['complete'] and evidence['binary_sha256'] == sha(runtime)
    pins = {str(p): sha(p) for p in (runtime, build, Path(__file__).resolve(), Path(__file__).with_name('run_scene_vm.py'))}
    report = dict(complete=False, runtime_sha256=sha(runtime), inputs=pins,
                  cgroup=str(GATE), memory_max=68719476736, swap_max=0,
                  events_before=(GATE/'memory.events').read_text(), cases=[])
    base = ['--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', 'main',
            '--ros-scene-bind', 'robot', 'worker', '--ros-scene-call-budget', 'robot', '65536', '1',
            '--ros-scene-call', 'robot', 'route', 'main', 'worker', 'echo', 'octets', 'octets', '8', '8', '1', '1000000000']
    option = ['--ros-scene-call-clear-input', 'robot', 'route']

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(name, flags, okay=True, warning=False):
        verify_gate()
        command = [str(runtime), '--log-color', 'disable', *flags, '--ros-scene-check']
        result = subprocess.run(command, capture_output=True, timeout=30)
        (out/(name+'.stdout')).write_bytes(result.stdout)
        (out/(name+'.stderr')).write_bytes(result.stderr)
        text = result.stdout+result.stderr
        passed = (result.returncode == 0) == okay and (b'(retained-ros-input)' in text) == warning
        report['cases'].append(dict(name=name, argv=command, returncode=result.returncode, passed=passed))
        save()
        assert passed, (name, result.returncode, text[-3000:])

    try:
        run('default', base)
        run('explicit-clear', [*base, *option, 'true'])
        run('retain-warning', [*base, *option, 'false'], warning=True)
        for value in ('0', '1', 'TRUE', 'False', 'yes', ''):
            run('invalid-boolean-'+(value or 'empty'), [*base, *option, value], okay=False)
        run('missing-route', [*base, '--ros-scene-call-clear-input', 'robot', 'absent', 'true'], okay=False)
        run('missing-environment', [*base, '--ros-scene-call-clear-input', 'absent', 'route', 'true'], okay=False)
        run('before-route', [*option, 'true', *base], okay=False)
        for category in ('retained-ros-input', 'all'):
            run('suppressed-'+category, ['--disable-warning', category, *base, *option, 'false'])
            run('fatal-'+category, ['--warning-as-error', category, *base, *option, 'false'], okay=False, warning=True)
            run('fatal-safe-'+category, ['--warning-as-error', category, *base, *option, 'true'])
            run('fatal-suppressed-'+category, ['--warning-as-error', category, '--disable-warning', category, *base, *option, 'false'])
            run('fatal-reenabled-'+category, ['--warning-as-error', category, '--disable-warning', category,
                                            '--enable-warning', category, *base, *option, 'false'], okay=False, warning=True)
        run('later-promotion', [*base, *option, 'false', '--warning-as-error', 'retained-ros-input'], warning=True)
        run('reenabled-warning', ['--disable-warning', 'all', '--enable-warning', 'retained-ros-input', *base, *option, 'false'], warning=True)
        report['complete'] = True
    finally:
        verify_gate()
        report['inputs_unchanged'] = all(sha(p) == digest for p, digest in pins.items())
        report['complete'] &= report['inputs_unchanged']
        report['events_after'] = (GATE/'memory.events').read_text()
        save()
    print('PASS', len(report['cases']), 'CLI Boolean, route and warning policy cases')


if __name__ == '__main__':
    main()
