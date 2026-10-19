#!/usr/bin/env python3
"""Observe an unchanged frozen event controller without altering its stop proof.

This additive diagnostic launches one explicitly supplied frozen controller in
the original, pinned 64 GiB Linux cgroup. It records owned descendant births,
all host task states, pending signal masks, wait channels and controller pidfd
metadata for the exact Gateway snapshot named by that controller's growing
report. Observation is periodic; it is not an exact SIGSTOP syscall timestamp.
It never resumes a task during normal observation or replaces a failing result.
Timeout/interrupt cleanup uses only acquired pidfds and the existing frozen
ownership implementation. No native/ROS behavior is promoted by this report.

Pass a fresh output, the independent observer/source SHA pins, the controller's
report path, and ``--`` followed by its original private-namespace argv. The
parent remains responsible for the original container and active binfmt lease.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import signal
import subprocess
import sys
import time

sys.dont_write_bytecode = True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--manifest-sha256', required=True)
    parser.add_argument('--observer-sha256', required=True)
    parser.add_argument('--controller-report', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('command', nargs=argparse.REMAINDER)
    args = parser.parse_args()
    command = args.command[1:] if args.command[:1] == ['--'] else args.command
    if sys.platform != 'linux' or not command or args.out.exists():
        parser.error('Linux, an explicit controller command and a fresh output are required')
    sys.path.insert(0, str(args.source / 'tests/ros_aarch64'))
    from runtime import Evidence, digest, read_json_binding
    from run_ros1_search import OwnedProcesses, process_identity
    manifest_path = args.source.parent / 'source-manifest.json'
    manifest, binding = read_json_binding(manifest_path)
    if binding['sha256'] != args.manifest_sha256 or digest(__file__) != args.observer_sha256:
        raise RuntimeError('observer or complete source manifest differs from its explicit pin')
    expected_controller = str(args.source / 'tests/ros_aarch64/run_local_parameter_events.py')
    if (command[:5] != ['unshare', '-m', '/usr/bin/python3', '-B', expected_controller]
            or command.count('--mode') != 1 or command[command.index('--mode')+1] != 'execute'
            or command.count('--source') != 1 or command[command.index('--source')+1] != str(args.source)
            or command.count('--out') != 1 or command[command.index('--out')+1] != str(args.controller_report.parent)
            or args.controller_report.name != 'report.json' or args.controller_report.parent.exists()
            or not args.out.is_absolute() or not args.controller_report.is_absolute()
            or args.out.resolve().is_relative_to(args.source.resolve())):
        raise RuntimeError('exact frozen execute controller and fresh independent report/output are required')
    for name in ('runtime.py', 'run_ros1_search.py', 'run_action_qos.py', 'run_local_parameter_events.py'):
        if digest(args.source / 'tests/ros_aarch64' / name) != manifest['tests/ros_aarch64/' + name]['sha256']:
            raise RuntimeError('frozen observer/controller helper changed: ' + name)
    probe_path = args.source / 'tests/ros_cgroup/probe.py'
    if digest(probe_path) != manifest['tests/ros_cgroup/probe.py']['sha256']:
        raise RuntimeError('frozen cgroup probe changed')
    spec = importlib.util.spec_from_file_location('stop_observer_probe', probe_path)
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
    args.out.mkdir(mode=0o700)
    evidence = Evidence(args.out / 'scope', module.probe)
    expected = {'path': '/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope',
                'device': 29, 'inode': 18927, 'boot_id': '518a7b26-33dd-4044-9456-93e75aac9b63'}
    if evidence.incarnation_before != expected:
        raise RuntimeError('diagnostic is not in the separately verified current incarnation')
    owned = OwnedProcesses(evidence); process = None; last = {}; captured = 0
    report = {'status': 'DIAGNOSTIC_RUNNING', 'scope': 'Periodic kernel observation only; no functional acceptance claim',
              'observer_sha256': args.observer_sha256, 'source_manifest_sha256': binding['sha256'],
              'expected_incarnation': expected, 'command': command, 'behavior_result_promoted': False}

    def interrupted(signum, frame):
        raise RuntimeError('diagnostic interrupted: ' + str(signum))

    signal.signal(signal.SIGINT, interrupted); signal.signal(signal.SIGTERM, interrupted)
    try:
        with (args.out / 'controller.log').open('xb') as log, (args.out / 'observations.jsonl').open('x') as records:
            process = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
            owned.acquire(process.pid, 'frozen-controller', [process.pid])
            deadline = time.monotonic() + 240
            while process.poll() is None:
                owned.sample()
                if time.monotonic() > deadline or (args.out / 'observations.jsonl').stat().st_size > 16*1024**2:
                    raise RuntimeError('diagnostic deadline or bounded observation log exceeded')
                try:
                    progress = json.loads(args.controller_report.read_bytes())
                    target = progress.get('gateway_snapshot', {})
                except (FileNotFoundError, json.JSONDecodeError):
                    target = {}
                for row in list(owned.rows.values()):
                    if not owned.check(row):
                        continue
                    pid = row['identity']['pid']
                    try:
                        before = process_identity(pid)
                        mapping_receipt = evidence.mapping(pid, {'observed-gateway':target}) if target else None
                        selected = mapping_receipt['raw_maps'] if mapping_receipt else []
                        if not selected:
                            continue
                        tasks = []
                        for task in sorted(Path(f'/proc/{pid}/task').iterdir()):
                            try:
                                status = dict(line.split(':', 1) for line in (task / 'status').read_text().splitlines() if ':' in line)
                                tasks.append({'tid': int(task.name), 'stat': (task / 'stat').read_text().strip(),
                                              'wchan': (task / 'wchan').read_text().strip(),
                                              'status': {key: status.get(key, '').strip() for key in
                                                         ('State', 'Tgid', 'Pid', 'PPid', 'TracerPid', 'SigPnd', 'ShdPnd', 'SigBlk', 'SigIgn', 'SigCgt')}})
                            except (FileNotFoundError, ProcessLookupError):
                                continue
                        after = process_identity(pid)
                        if any(before[key] != after[key] for key in ('pid', 'parent_pid', 'starttime_ticks')) or not owned.check(row):
                            continue
                        fact = {'pid': pid, 'identity': after, 'ancestry': row['ancestry'], 'target': target,
                                'membership': Path(f'/proc/{pid}/cgroup').read_text().splitlines(),
                                'exe': os.readlink(f'/proc/{pid}/exe'), 'maps': selected, 'opened_target_mappings':mapping_receipt, 'tasks': tasks,
                                'observer_pidfd': Path(f'/proc/self/fdinfo/{row["pidfd"]}').read_text()}
                        for fdinfo in Path(f'/proc/{process.pid}/fdinfo').iterdir():
                            try:
                                text = fdinfo.read_text()
                                if any(line.split() == ['Pid:', str(pid)] for line in text.splitlines()):
                                    fact.setdefault('controller_pidfds', []).append({'fd': fdinfo.name, 'fdinfo': text})
                            except (FileNotFoundError, ProcessLookupError):
                                continue
                        key = json.dumps({k: v for k, v in fact.items() if k != 'tasks'}, sort_keys=True) + json.dumps(
                            [{k: v for k, v in task.items() if k != 'stat'} for task in tasks], sort_keys=True)
                        now = time.monotonic()
                        if pid not in last or key != last[pid][0] or now - last[pid][1] >= .1:
                            fact['capture_monotonic_ns'] = time.monotonic_ns()
                            records.write(json.dumps(fact) + '\n'); records.flush(); last[pid] = (key, now); captured += 1
                    except (FileNotFoundError, ProcessLookupError):
                        continue
                time.sleep(.005)
            report.update(status='DIAGNOSTIC_CONTROLLER_EXIT_RECORDED', controller_returncode=process.returncode)
    except BaseException as error:
        report.update(status='DIAGNOSTIC_FAILED', error=repr(error))
    finally:
        report['cleanup'] = owned.stop([process] if process is not None else [])
        report['observations'] = captured
        architecture = evidence.finish('PASS' if report['cleanup']['status'] == 'PASS' else 'FAIL')
        report['architecture'] = architecture
        if architecture['status'] != 'PASS':
            report.update(status='DIAGNOSTIC_FAILED', error='diagnostic cgroup/ownership envelope failed')
        elif captured == 0 and report['status'] == 'DIAGNOSTIC_CONTROLLER_EXIT_RECORDED':
            report.update(status='DIAGNOSTIC_NO_GATEWAY_CAPTURE', error='no qualified Gateway mapping was observed')
        (args.out / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({key: report.get(key) for key in ('status', 'controller_returncode', 'observations', 'error')}), flush=True)
    return 0 if report['status'] == 'DIAGNOSTIC_CONTROLLER_EXIT_RECORDED' and report['cleanup']['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
