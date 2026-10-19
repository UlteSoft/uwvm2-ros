#!/usr/bin/env python3
"""Authored Linux OS/Guest contract harness; never run by its source author.

The Root-supplied plan pins the complete actual binary/Wasm roles and argv.
This is not a provider, simulator, import admission or ROS/native proof.
An independent outer cgroup/process owner must still bound the whole harness.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import selectors
import signal
import subprocess
import sys
import time


def ordinary_file(path, expected):
    path = Path(path)
    if not path.is_absolute() or path.is_symlink():
        raise ValueError('An absolute ordinary role is required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        import stat
        if not stat.S_ISREG(first.st_mode):
            raise ValueError('Role is not an ordinary file')
        digest = hashlib.sha256()
        while True:
            block = os.read(fd, 131072)
            if not block:
                break
            digest.update(block)
        last = os.fstat(fd)
        if (first.st_dev, first.st_ino, first.st_size, first.st_mtime_ns, first.st_ctime_ns) != (last.st_dev, last.st_ino, last.st_size, last.st_mtime_ns, last.st_ctime_ns):
            raise ValueError('Actual role changed during admission')
        if digest.hexdigest() != expected:
            raise ValueError('Actual role hash differs from Root selection')
        return {'path': str(path), 'dev': first.st_dev, 'ino': first.st_ino,
                'size': first.st_size, 'sha256': digest.hexdigest()}
    finally:
        os.close(fd)


def main():
    if sys.platform != 'linux':
        raise RuntimeError('Real Linux POSIX process/signal semantics are required')
    parser = argparse.ArgumentParser()
    parser.add_argument('actual_plan')
    parser.add_argument('new_report')
    args = parser.parse_args()
    plan = json.loads(Path(args.actual_plan).read_text())
    if set(plan) != {'argv', 'roles', 'event', 'ready_timeout_ms', 'finish_timeout_ms', 'expected_exit', 'expected_line'}:
        raise ValueError('Use the exact finite Root plan schema')
    argv = plan['argv']
    if not isinstance(argv, list) or not argv or not all(type(v) is str and v for v in argv):
        raise ValueError('Explicit complete actual argv is required')
    if plan['event'] not in ('SIGINT', 'SIGTERM'):
        raise ValueError('Only the two explicit notifications are admitted')
    for field in ('ready_timeout_ms', 'finish_timeout_ms'):
        if type(plan[field]) is not int or not 1 <= plan[field] <= 180000:
            raise ValueError('Independent harness deadlines must be finite')
    if type(plan['expected_exit']) is not int or not 0 <= plan['expected_exit'] <= 255:
        raise ValueError('An actual ordinary exit code is required')
    if plan['expected_line'] not in ('CLI_SIGNAL_INT\n', 'CLI_SIGNAL_TERM\n', ''):
        raise ValueError('Unexpected Guest observation')
    if not isinstance(plan['roles'], list) or not 2 <= len(plan['roles']) <= 32:
        raise ValueError('Pin the executable and actual shared/main Wasm roles')
    admitted = []
    seen = set()
    for role in plan['roles']:
        if set(role) != {'path', 'sha256'} or role['path'] in seen:
            raise ValueError('Closed unique ordinary roles are required')
        seen.add(role['path'])
        admitted.append(ordinary_file(role['path'], role['sha256']))
    if argv[0] not in seen or '--wasi-thread-signals' not in argv or '--wasi-threads' not in argv:
        raise ValueError('Select the actual new opt-in executable invocation')
    if argv.count('--wasi-thread-signals') != 1:
        raise ValueError('Exactly one complete signal policy is required')
    at = argv.index('--wasi-thread-signals')
    values = argv[at+1:at+7]
    if len(values) != 6 or any(not v.isascii() or not v.isdecimal() or int(v) <= 0 for v in values):
        raise ValueError('All six complete explicit positive policy fields are required')
    read_fd, cookie, frames, pipe_bytes, grace_ms, retire_ms = map(int, values)
    if not 3 <= read_fd <= 2147483647 or not 0 < cookie <= 18446744073709551615 or not 0 < frames <= 1048576 or not 32 <= pipe_bytes <= 1048576 or not 0 < grace_ms <= 60000 or not 0 < retire_ms <= 60000:
        raise ValueError('Policy is outside the production finite bounds')
    if read_fd != 3 or cookie != 261:
        raise ValueError('These actual source fixtures explicitly use fd3/cookie261')
    if signal.getsignal(signal.SIGINT) is not signal.default_int_handler or signal.getsignal(signal.SIGTERM) is not signal.SIG_DFL:
        raise ValueError('Run this harness in its ordinary Root-owned process')
    # Popen restores only a small predefined set; explicitly restore INT/TERM
    # in this SINGLE-threaded source harness's child before the real exec.
    def child_defaults():
        signal.signal(signal.SIGINT, signal.SIG_DFL)
        signal.signal(signal.SIGTERM, signal.SIG_DFL)
    out_r, out_w = os.pipe2(os.O_NONBLOCK | os.O_CLOEXEC)
    err_r, err_w = os.pipe2(os.O_NONBLOCK | os.O_CLOEXEC)
    child = None
    selector = selectors.DefaultSelector()
    streams = {'stdout': bytearray(), 'stderr': bytearray()}
    report = {'kind': 'actual-cli-os-signal-contract', 'status': 'FAIL',
              'actual_roles': admitted, 'argv': argv, 'event': plan['event'],
              'signal_sent': False, 'harness_killed': False,
              'qualification': 'CLI/Guest only; no original rclcpp/ROS/SDK33/native-credit'}
    try:
        child = subprocess.Popen(argv, stdin=subprocess.DEVNULL, stdout=out_w, stderr=err_w,
                                 close_fds=True, start_new_session=True, preexec_fn=child_defaults)
        report['pid'] = child.pid
        os.close(out_w); out_w = -1
        os.close(err_w); err_w = -1
        selector.register(out_r, selectors.EVENT_READ, 'stdout')
        selector.register(err_r, selectors.EVENT_READ, 'stderr')
        ready_until = time.monotonic_ns() + plan['ready_timeout_ms'] * 1000000
        finish_until = None
        while True:
            now = time.monotonic_ns()
            until = finish_until if finish_until is not None else ready_until
            if now >= until:
                raise TimeoutError('Actual readiness/retirement deadline expired')
            for key, _ in selector.select(min((until - now) / 1e9, .1)):
                try:
                    block = os.read(key.fd, 16384)
                except BlockingIOError:
                    continue
                if not block:
                    selector.unregister(key.fd)
                    continue
                target = streams[key.data]
                if len(target) + len(block) > 65536:
                    raise RuntimeError('Raw output quota exceeded')
                target.extend(block)
            if not report['signal_sent'] and b'CLI_SIGNAL_READY\n' in streams['stdout'].splitlines(keepends=True):
                if child.poll() is not None:
                    raise RuntimeError('Guest exited before the actual external signal')
                os.kill(child.pid, getattr(signal, plan['event']))
                report['signal_sent'] = True
                report['sent_monotonic_ns'] = time.monotonic_ns()
                finish_until = report['sent_monotonic_ns'] + plan['finish_timeout_ms'] * 1000000
            result = child.poll()
            if result is not None and not selector.get_map():
                report['returncode'] = result
                break
        if not report['signal_sent'] or report['returncode'] != plan['expected_exit']:
            raise RuntimeError('Actual signal/exit result differs from the selected case')
        expected = plan['expected_line'].encode()
        if expected and expected not in streams['stdout'].splitlines(keepends=True):
            raise RuntimeError('The real Guest did not report the expected event')
        report['status'] = 'PASS_CLI_GUEST_ONLY'
    except BaseException as error:
        report['error'] = repr(error)
    finally:
        # Real outer emergency containment, not a passing graceful/physical join.
        if child is not None and child.poll() is None:
            report['harness_killed'] = True
            os.killpg(child.pid, signal.SIGKILL)
            try:
                child.wait(timeout=2)
            except subprocess.TimeoutExpired:
                report['outer_wait_pending'] = True
        for fd in (out_r, out_w, err_r, err_w):
            if fd >= 0:
                try:
                    os.close(fd)
                except OSError:
                    pass
        selector.close()
        report['raw_stdout_hex'] = bytes(streams['stdout']).hex()
        report['raw_stderr_hex'] = bytes(streams['stderr']).hex()
        report['source_or_native_physical_join_claim'] = False
        destination = Path(args.new_report)
        with destination.open('x') as output:
            json.dump(report, output, indent=2)
            output.write('\n')
    return 0 if report['status'] == 'PASS_CLI_GUEST_ONLY' else 1


if __name__ == '__main__':
    raise SystemExit(main())
