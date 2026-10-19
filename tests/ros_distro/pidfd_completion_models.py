#!/usr/bin/env python3
"""Actual Linux pidfd contracts for long serial build/test ownership.

Run only in the admitted cgroup after the complete source freeze. High numbered
handles are obtained with F_DUPFD_CLOEXEC, without creating thousands of actors
or consuming significant memory. The terminal case executes one bounded Python
child and observes its real kernel pidfd; no numeric PID is used for signalling.
"""
import fcntl
import importlib.util
import os
from pathlib import Path
import signal
import subprocess
import sys
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('ros_owned_pidfd_common', ROOT / 'tools/ros_distro/common.py')
common = importlib.util.module_from_spec(spec)
spec.loader.exec_module(common)


class PidfdCompletionModels(unittest.TestCase):
    def high(self, fd):
        duplicate = fcntl.fcntl(fd, fcntl.F_DUPFD_CLOEXEC, 2048)
        try:
            self.assertGreaterEqual(duplicate, 2048)
        except BaseException:
            os.close(duplicate)
            raise
        return duplicate

    def test_live_process_high_pidfd_remains_live(self):
        fd = os.pidfd_open(os.getpid())
        high = None
        try:
            high = self.high(fd)
            self.assertFalse(common.pidfd_terminal(high))
        finally:
            if high is not None:
                os.close(high)
            os.close(fd)

    def test_actual_completed_child_high_pidfd_is_terminal(self):
        child = subprocess.Popen([sys.executable, '-B', '-c', 'pass'],
                                 stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL,
                                 stderr=subprocess.DEVNULL)
        fd = high = None
        try:
            fd = os.pidfd_open(child.pid)
            high = self.high(fd)
            self.assertEqual(child.wait(timeout=15), 0)
            self.assertTrue(common.pidfd_terminal(high))
        finally:
            try:
                if child.poll() is None:
                    if fd is not None:
                        try:
                            signal.pidfd_send_signal(fd, signal.SIGKILL)
                        except ProcessLookupError:
                            pass
                    child.wait(timeout=15)
            finally:
                if high is not None:
                    os.close(high)
                if fd is not None:
                    os.close(fd)

    def test_closed_high_descriptor_is_not_promoted_to_terminal(self):
        fd = os.pidfd_open(os.getpid())
        high = None
        try:
            high = self.high(fd)
            closed = high
            os.close(high)
            high = None
            with self.assertRaises(ValueError):
                common.pidfd_terminal(closed)
        finally:
            if high is not None:
                os.close(high)
            os.close(fd)

    def test_serial_runner_finalizes_real_high_numbered_owned_handles(self):
        with tempfile.TemporaryDirectory() as directory:
            runner = common.SerialRunner(Path(directory),
                {'command_seconds': 15, 'rss_bytes': 128 * 1024**2,
                 'log_bytes': 1024**2})
            try:
                runner.run('bounded-child', [sys.executable, '-B', '-c', 'pass'],
                           directory, {'PATH': '/usr/bin:/bin',
                                       'PYTHONDONTWRITEBYTECODE': '1'})
                self.assertTrue(runner.owned)
                for item in runner.owned.values():
                    original = item['fd']
                    item['fd'] = self.high(original)
                    os.close(original)
                self.assertEqual(runner.close(), {'status': 'PASS', 'remaining': []})
            finally:
                if runner.owned:
                    runner.close()


if __name__ == '__main__':
    unittest.main(verbosity=2)
