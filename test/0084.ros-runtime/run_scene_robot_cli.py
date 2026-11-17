#!/usr/bin/env python3
"""Reject incomplete robot deployment policy before provider activation.

Reuse the exact successful static configurations from the real Wasm receipts.
No guest is run: every invocation ends in --ros-scene-check. Missing worker
reservations, unknown restriction names and substituted profile digests must
produce ordinary configuration errors, never a crash or successful activation.
"""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time

from run_scene_vm import GATE, ROOT, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--local', type=Path, required=True)
    parser.add_argument('--process', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    assert os.sched_getaffinity(0).issubset({4, 5, 6, 7})
    out = args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    local_path = args.local.resolve()
    process_path = args.process.resolve()
    local = json.loads(local_path.read_text())
    process = json.loads(process_path.read_text())
    assert local['complete'] and process['complete']
    runtime = Path(local['runtime'])
    assert runtime == Path(process['runtime'])
    assert local['runtime_sha256'] == process['runtime_sha256'] == sha(runtime)
    build = runtime.parent/'full-build-results.json'
    assert local['build_sha256'] == process['build_sha256'] == sha(build)
    pins = {str(path): sha(path) for path in (local_path, process_path, Path(__file__).resolve(), build, runtime)}
    plans = {}
    for cohort in (local, process):
        for case in cohort['cases']:
            if case['name'].endswith('-static'):
                assert case['passed'] and case['exit'] == 0
                argv = case['argv']
                assert argv[0] == str(runtime) and argv[-1] == '--ros-scene-check'
                assert '--run' not in argv
                plans[case['name'].removesuffix('-static')] = argv
    assert set(plans) == {'media', 'device', 'bridge', 'component'}
    report = dict(complete=False, scope=__doc__, runtime_sha256=sha(runtime), build_sha256=sha(build),
        cgroup=str(GATE), inode=GATE.stat().st_ino, memory_max=68719476736, swap_max=0,
        events_before=(GATE/'memory.events').read_text(), inputs=pins, cases=[])

    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    def run(name, argv, success=False):
        verify_gate()
        assert argv[-1] == '--ros-scene-check' and '--run' not in argv
        start = time.monotonic()
        result = subprocess.run(argv, capture_output=True, text=True, timeout=30)
        (out/(name+'.stdout')).write_text(result.stdout)
        (out/(name+'.stderr')).write_text(result.stderr)
        passed = result.returncode == 0 if success else result.returncode > 0 and '[crash]' not in result.stderr
        report['cases'].append(dict(name=name, argv=argv, exit=result.returncode,
            passed=passed, seconds=time.monotonic()-start))
        save()
        assert passed, (name, result.returncode, result.stderr[-3000:])
        print('PASS '+name, flush=True)

    for family, argv in plans.items():
        run('valid-'+family, argv, True)
    for family in ('component-worker', 'bridge'):
        original = plans['component' if family == 'component-worker' else family]
        option = '--ros-scene-'+family+'-limit'
        indices = [index for index, token in enumerate(original) if token == option]
        assert len(indices) == (13 if family == 'component-worker' else 3)
        for index in indices:
            bad = original.copy()
            field = bad[index+3]
            del bad[index:index+5]
            run('missing-'+family+'-'+field, bad)
    for family in ('device', 'control', 'media', 'bridge', 'component', 'component-worker'):
        original = plans['device' if family == 'control' else 'component' if family == 'component-worker' else family]
        bad = original.copy()
        index = bad.index('--ros-scene-'+family+'-limit')
        bad[index+3] = 'unrecognized.restriction'
        run('unknown-'+family+'-restriction', bad)
    for family in ('device', 'control', 'media', 'bridge', 'component'):
        bad = plans['device' if family == 'control' else family].copy()
        index = bad.index('--ros-scene-'+family+'-profile')
        assert bad[index+4] != '0'*64
        bad[index+4] = '0'*64
        run('changed-'+family+'-digest', bad)
    for name, cpu, numa, nodes in [('invalid-cpu', '18446744073709551616', 'none', '-'),
                                   ('invalid-numa', '4', 'unrecognized', '0')]:
        bad = plans['component'].copy()
        bad[-1:-1] = ['--ros-scene-component-worker-placement', 'robot', 'component', cpu, numa, nodes]
        run(name, bad)
    assert len(report['cases']) == 33
    report['inputs_unchanged'] = all(sha(Path(path)) == value for path, value in pins.items())
    assert report['inputs_unchanged']
    report['events_after'] = (GATE/'memory.events').read_text()
    report['complete'] = True
    save()


if __name__ == '__main__':
    main()
