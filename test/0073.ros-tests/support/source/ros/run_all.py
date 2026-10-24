#!/usr/bin/env python3
"""Bounded acceptance entrypoint; omitted/missing network prerequisites never count as PASS."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import signal
import sys
import time


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--uwvm', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--workspace', type=Path, required=True,
                   help='Prepared task workspace containing gateway/noetic-rootfs and gateway/jazzy-rootfs')
    p.add_argument('--llvm-libs', default=os.environ.get('LD_LIBRARY_PATH', ''))
    p.add_argument('--wat2wasm', default='wat2wasm')
    a = p.parse_args()
    root = Path(__file__).resolve().parents[5]
    out = a.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    rows = []
    start = time.monotonic()
    child_environment = os.environ.copy()
    child_environment['LD_LIBRARY_PATH'] = a.llvm_libs
    commands = [
        ('cgroup', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros_cgroup/probe.py'), '--output', str(out/'cgroup-before.json')], 15),
        ('unit', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_unit.py'), '--out', str(out/'unit')], 420),
        ('cli', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros_cli/run.py'), '--uwvm', str(a.uwvm.resolve()), '--output', str(out/'cli.json'), '--wat2wasm', a.wat2wasm], 90),
        ('regression', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/regression.py'), '--uwvm', str(a.uwvm.resolve()), '--out', str(out/'regression'), '--wat2wasm', a.wat2wasm], 300),
        ('native', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_native.py'), '--workspace', str(a.workspace.resolve()), '--suite', 'all', '--out', str(out/'native')], 600),
        ('resources', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_rpc.py'), '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--llvm-libs', a.llvm_libs, '--wat2wasm', a.wat2wasm, '--mode', 'all'], 1800),
        ('network', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_network.py'), '--mode', 'all', '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--llvm-libs', a.llvm_libs], 1500),
        ('five-module', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros_multimodule/run.py'), '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--expected-product-sha256', hashlib.sha256(a.uwvm.read_bytes()).hexdigest(), '--llvm-libs', a.llvm_libs], 180),
        ('additional-security', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_additional_security.py'), '--mode', 'all', '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--llvm-libs', a.llvm_libs], 300),
        ('ingress-trap', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_ingress.py'), '--mode', 'all', '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--llvm-libs', a.llvm_libs], 300),
        ('revoke-race', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_revoke_race.py'), '--mode', 'all', '--version', 'both', '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--llvm-libs', a.llvm_libs], 180),
        ('flood', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_flood.py'), '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--llvm-libs', a.llvm_libs, '--wat2wasm', a.wat2wasm], 180),
        ('payload-security', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros/run_payload_security.py'), '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--llvm-libs', a.llvm_libs, '--wat2wasm', a.wat2wasm], 240),
        ('performance', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros_perf/run.py'), '--workspace', str(a.workspace.resolve()), '--source', str(root), '--uwvm', str(a.uwvm.resolve()), '--llvm-libs', a.llvm_libs, '--wat2wasm', a.wat2wasm], 240),
        ('cgroup-after', [sys.executable, str(root/'test/0073.ros-tests/support/source/ros_cgroup/probe.py'), '--output', str(out/'cgroup-after.json')], 15),
    ]
    active = None
    def evidence(name):
        fixed = {'cgroup':out/'cgroup-before.json', 'cgroup-after':out/'cgroup-after.json',
                 'unit':out/'unit/results.json', 'cli':out/'cli.json',
                 'regression':out/'regression/results.json', 'native':out/'native/report.json'}
        candidates = {fixed[name]} if name in fixed else set()
        log = out/(name+'.log')
        if log.exists():
            content = log.read_text(errors='replace')
            for value in re.findall(r'(/[^\s]+\.json)', content):
                path = Path(value)
                if path.is_relative_to(a.workspace.resolve()):candidates.add(path)
            for value in re.findall(re.escape(str(a.workspace.resolve()))+r'/[^\s]+', content):
                path = Path(value)
                if path.is_dir() and (path/'report.json').is_file():candidates.add(path/'report.json')
        result = []
        for path in sorted(candidates):
            if path.is_file():
                data = path.read_bytes()
                try:document = json.loads(data)
                except json.JSONDecodeError:
                    result.append({'path':str(path), 'sha256':hashlib.sha256(data).hexdigest(),
                                   'status':'FAIL', 'reason':'invalid evidence JSON'})
                    continue
                item = {'path':str(path), 'sha256':hashlib.sha256(data).hexdigest(),
                        'status':document.get('status')}
                for key in ('results', 'suites', 'checks', 'cases', 'tests'):
                    if isinstance(document.get(key),list):item[key+'_count']=len(document[key])
                result.append(item)
        return result
    def terminated(signum, frame):
        if active is not None and active.poll() is None:
            active.terminate()
            try:active.wait(timeout=15)
            except subprocess.TimeoutExpired:
                active.kill();active.wait(timeout=5)
        raise SystemExit(128+signum)
    signal.signal(signal.SIGTERM, terminated)
    for name, command, timeout in commands:
        if name != 'cgroup' and rows and rows[0]['status'] != 'PASS':
            rows.append({'suite':name, 'status':'BLOCKED', 'reason':'provided 64 GiB cgroup membership not proven'})
            continue
        try:
            with (out/(name+'.log')).open('wb') as log:
                process = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, env=child_environment)
                active = process
                try:
                    returncode = process.wait(timeout=timeout)
                except subprocess.TimeoutExpired:
                    # Network runner handles SIGTERM by cleaning its owned process groups/mounts.
                    process.terminate()
                    try:
                        process.wait(timeout=15)
                    except subprocess.TimeoutExpired:
                        process.kill()
                        process.wait(timeout=5)
                    raise
            status = 'PASS' if returncode == 0 else 'BLOCKED' if returncode in (2, 77) else 'FAIL'
            active = None
            references = evidence(name)
            if status == 'PASS' and (not references or any(item.get('status') != 'PASS' for item in references)):
                status = 'FAIL'
            rows.append({'suite':name, 'status':status, 'returncode':returncode,
                         'command':command, 'evidence':references})
        except FileNotFoundError as error:
            rows.append({'suite':name, 'status':'BLOCKED', 'reason':str(error), 'command':command})
        except subprocess.TimeoutExpired:
            rows.append({'suite':name, 'status':'FAIL', 'reason':'bounded acceptance timeout', 'command':command})
        print(rows[-1]['status'], name, flush=True)
    if (out/'cgroup-before.json').exists() and (out/'cgroup-after.json').exists():
        before = json.loads((out/'cgroup-before.json').read_text())
        after = json.loads((out/'cgroup-after.json').read_text())
        if before['status'] == after['status'] == 'PASS':
            def events(snapshot):
                return dict(line.split() for line in snapshot['cgroup_ancestors'][0]['memory.events'].splitlines())
            first, last = events(before), events(after)
            if any(first.get(key) != last.get(key) for key in ('oom', 'oom_kill')):
                rows.append({'suite':'memory-events', 'status':'FAIL', 'reason':'OOM counter increased in shared target cgroup'})
    status = 'FAIL' if any(r['status']=='FAIL' for r in rows) else 'BLOCKED' if any(r['status']=='BLOCKED' for r in rows) else 'PASS'
    (out/'report.json').write_text(json.dumps({'status':status,
        'product_sha256':hashlib.sha256(a.uwvm.read_bytes()).hexdigest(),
        'seconds':round(time.monotonic()-start,3), 'suites':rows}, indent=2)+'\n')
    print(status, 'acceptance; report:', out/'report.json')
    return 0 if status == 'PASS' else 77 if status == 'BLOCKED' else 1


if __name__ == '__main__':
    sys.exit(main())
