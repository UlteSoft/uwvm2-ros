#!/usr/bin/env python3
"""Actual fault exits and explicitly requested Linux whole-process pauses.

Ordinary INT/AOT (also via --runner) covers sync/software-async memory faults,
bounds faults and unreachable. Optional signed-update cases use the maintained
active-data/f32 fixture. Async delivery stays noexcept: after a real SIGSTOP,
authorized SIGCONT continues terminal processing to122, never managed recovery.
The separate signed-module-state runner tests the synchronous retained manager.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import signal
import subprocess
import sys
import time

sys.dont_write_bytecode = True
from run_crash_delivery import module
from run_signed_module_state import Harness, Instance, state_module


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def wait_event(child, timeout):
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        pid, status = os.waitpid(child.pid, os.WNOHANG | os.WUNTRACED)
        if pid:
            stopped = os.WIFSTOPPED(status)
            if not stopped:
                child.returncode = os.waitstatus_to_exitcode(status)
            return dict(pid=pid, status=status, stopped=stopped,
                        signal=os.WSTOPSIG(status) if stopped else None,
                        exit=child.returncode)
        time.sleep(.002)
    return dict(pid=0, status=None, stopped=False, signal=None, exit=None, timed_out=True)


def report_matches(text, expected, category, pause, mode, memtag):
    fields = [f'[crash] {category}', f'Exit status: {expected}', 'Reason: ',
              'Fault action: ' + ('pause for the authorized host' if pause else 'terminate execution')]
    if memtag and mode == 'software-async':
        fields += ['Reason: asynchronous memory tag mismatch', 'Delivery: asynchronous']
    elif memtag and mode == 'hardware-async':
        fields += ['Reason: asynchronous hardware memory tag mismatch', 'Delivery: asynchronous']
    return all(field in text for field in fields) and '"schema"' not in text


def flags(mode):
    return ['--runtime-memtag-backend', 'hardware' if mode.startswith('hardware-') else 'software', '--runtime-memtag-mode', 'async' if mode.endswith('async') else 'sync']


def ordinary(h, modes):
    fixtures = [('memtag', state_module(fault=True), 122, 'memory-protection'),
                ('bounds', module(b'\x41\x80\x80\x04\x2d\0\0\x1a'), 121, 'execution'),
                ('unreachable', module(b'\0'), 120, 'unreachable')]
    for mode in modes:
        for kind, image, expected, category in fixtures:
            for pause in (False, True):
                name = f'ordinary-{mode}-{kind}-' + ('pause' if pause else 'default')
                h.current = name
                path = h.write_image(name, image)
                command = h.command() + flags(mode) + ['--debug-permissions', 'none']
                if pause:
                    command += ['--debug-fault-action', 'pause']
                command += ['-Wstart', '0', '--run', str(path.resolve())]
                record = dict(name=name, command=command, image_sha256=hashlib.sha256(image).hexdigest(),
                              expected=expected, explicit_pause=pause, mode=mode, cleanup=None)
                error_path = h.args.out / (name + '.stderr')
                output_path = h.args.out / (name + '.stdout')
                child = None
                okay = False
                try:
                    with error_path.open('wb') as error, output_path.open('wb') as output:
                        child = subprocess.Popen(command, stdout=output, stderr=error)
                        event = wait_event(child, h.args.timeout)
                        record['kernel_wait'] = event
                        stopped = event['stopped'] and event['signal'] == signal.SIGSTOP
                        if stopped and pause:
                            record['stderr_before_continuation'] = error_path.read_text(errors='replace')
                            record['explicit_supervisor_signal'] = 'SIGCONT'
                            os.kill(child.pid, signal.SIGCONT)
                            child.wait(timeout=h.args.timeout)
                        elif event['stopped'] or event.get('timed_out'):
                            record['cleanup'] = 'supervisor killed unexpected stop or unresponsive fixture'
                            child.kill()
                            child.wait(timeout=5)
                        text = error_path.read_text(errors='replace')
                        record.update(exit=child.returncode, stderr=text)
                        okay = (record['cleanup'] is None and stopped == pause and child.returncode == expected
                                and report_matches(text, expected, category, pause, mode, kind == 'memtag'))
                        if pause:
                            okay = okay and report_matches(record.get('stderr_before_continuation', ''),
                                expected, category, True, mode, kind == 'memtag')
                except (OSError, ValueError, subprocess.TimeoutExpired) as error:
                    record['error'] = repr(error)
                finally:
                    if child is not None and child.poll() is None:
                        record['cleanup'] = 'supervisor killed owned timed-out fixture'
                        child.kill()
                        child.wait(timeout=5)
                    if child is not None:
                        record['exit'] = child.returncode
                    h.processes.append(record)
                h.rows.append(dict(scenario=name, name='actual pause then terminal exit' if pause else 'actual default terminal exit',
                                   passed=okay, detail=record))
                print(name, 'PASS' if okay else 'FAIL', flush=True)
                if okay:
                    h.completed.append(name)


def update(h):
    # No dynamic recovery/keeper oracle is imposed on the noexcept callback.
    # Existing synchronous fault-pause tests separately require inactive epoch9.
    for pause in (False, True):
        name = 'update-software-async-' + ('pause' if pause else 'default')
        h.current = name
        try:
            with Instance(h, name, state_module(fault=True), pause=pause, features=flags('software-async')) as vm:
                h.launch(vm)
                event = wait_event(vm.child, h.args.timeout)
                vm.fault_probe = dict(mode='software-async', internal_worker_count=1, kernel_wait=event,
                                      explicit_pause=pause, owner_disconnected=False)
                if pause:
                    h.check('actual SIGSTOP precedes any continuation', event['stopped'] and event['signal'] == signal.SIGSTOP, event)
                    text = vm.error_path.read_text(errors='replace')
                    h.check('async fault report exists before actual stop',
                            report_matches(text, 122, 'memory-protection', True, 'software-async', True), text)
                    vm.fault_probe['explicit_supervisor_signal'] = 'SIGCONT'
                    os.kill(vm.child.pid, signal.SIGCONT)
                    vm.child.wait(timeout=h.args.timeout)
                elif event['stopped'] or event.get('timed_out'):
                    h.check('default is an actual exit without supervisor action', False, event)
                text = vm.error_path.read_text(errors='replace')
                vm.fault_probe['exit_without_owner_disconnect'] = vm.child.returncode
                h.check('async terminal process exits122 without owner disconnect', vm.child.returncode == 122
                        and report_matches(text, 122, 'memory-protection', pause, 'software-async', True), vm.fault_probe)
            h.completed.append(name)
        except (AssertionError, OSError, ValueError, EOFError, subprocess.TimeoutExpired) as error:
            if not isinstance(error, AssertionError):
                h.rows.append(dict(scenario=name, name='actual transport/termination', passed=False, detail=repr(error)))
                print(name, 'FAIL', repr(error), flush=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vm', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path)
    parser.add_argument('--engine', required=True, choices=('int', 'aot'))
    parser.add_argument('--runner', default='')
    parser.add_argument('--profile', default='ordinary', choices=('ordinary', 'update', 'all'))
    parser.add_argument('--mode', action='append', choices=('sync', 'software-async', 'hardware-sync', 'hardware-async'))
    parser.add_argument('--timeout', type=float, default=15)
    args = parser.parse_args()
    if args.profile != 'ordinary' and args.engine != 'aot':
        parser.error('Signed-update profile requires the actual LLVM-full backend; use ordinary for INT targets.')
    args.out.mkdir(parents=True, exist_ok=False)
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    before = digest(args.vm)
    sources = [Path(__file__), Path(__file__).with_name('run_crash_delivery.py'),
               Path(__file__).with_name('run_signed_module_state.py'), Path(__file__).with_name('run_signed_update_boundaries.py')]
    hashes = {str(path.resolve()): digest(path) for path in sources}
    h = Harness(args)
    modes = args.mode or ['sync', 'software-async']
    if args.profile != 'update':
        ordinary(h, modes)
    if args.profile != 'ordinary':
        update(h)
    expected = (0 if args.profile == 'update' else len(modes) * 6) + (0 if args.profile == 'ordinary' else 2)
    result = dict(binary=str(args.vm.resolve()), binary_sha256=before, engine=args.engine, runner=h.runner,
                  test_source_sha256=hashes, profile=args.profile, ordinary_modes=modes,
                  cpu_affinity=sorted(os.sched_getaffinity(0)), expected_processes=expected,
                  completed_scenarios=h.completed, processes=h.processes, cases=h.rows,
                  passed=sum(row['passed'] for row in h.rows), total=len(h.rows),
                  complete=len(h.completed) == expected and all(row['passed'] for row in h.rows),
                  scope='Actual Guest execution and kernel stop/exit status. Default never receives SIGCONT. Only an explicit pause permits supervisor SIGCONT after observed SIGSTOP. Async signed-update continuation must terminate122; synchronous retained-manager semantics have a separate maintained test. QEMU is an execution prefix, not native hardware qualification.')
    assert digest(args.vm) == before
    assert all(digest(path) == hashes[str(path.resolve())] for path in sources)
    (args.out / 'report.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({key: result[key] for key in ('binary_sha256', 'passed', 'total', 'complete')}), flush=True)
    return 0 if result['complete'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
