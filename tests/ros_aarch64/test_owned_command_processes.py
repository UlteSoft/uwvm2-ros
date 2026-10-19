"""Eleven real Linux ownership regressions; SOURCE ONLY, never author-executed.

Root loads this factory and BOTH candidate modules only through its actual
complete manifest/source review. Root supplies an otherwise child-free test
process, a fresh owned output directory, actual current Python full-seven pin
and its existing original64GiB/swap0/768MiB/watchdog guard. Nothing imports a
project module here. These are pidfd/subreaper tests, not ROS/binfmt/QEMU tests.
"""
from pathlib import Path
import json
import os
import signal
import subprocess
import time
import unittest


ORPHAN = r'''
import os, signal, time
read, write = os.pipe()
intermediate = os.fork()
if intermediate == 0:
    os.close(read)
    if os.fork() != 0:
        os._exit(0)
    if NEW_SESSION:
        os.setsid()
    os.write(write, b'R'); os.close(write)
    if STOP_SELF:
        signal.raise_signal(signal.SIGSTOP)
    time.sleep(60)
    os._exit(0)
os.close(write)
assert os.read(read, 1) == b'R'
os.close(read)
os.waitpid(intermediate, 0)
os._exit(0)
'''


def suite(lease, common, *, out, python, guard):
    """Return eleven real ownership tests; no standalone CLI or ambient imports.

    `python` is an actual complete full-seven physical executable pin, not a
    version string. Real Kernel/Popen/subreaper/pidfd/waitid operations occur
    only when Root later invokes this returned suite. Every case retains raw
    stdout and the actual owner cleanup ledger even when an assertion fails.
    """
    out = Path(out)
    if not out.is_absolute() or out.resolve(strict=True) != out or not out.is_dir() or any(out.iterdir()):
        raise ValueError('fresh explicit physical owned output directory required')
    if set(python) != {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}:
        raise ValueError('closed actual Python executable full7 required')
    guard()
    if common.opened(Path(python['path']), 512*1024**2) != python:
        raise ValueError('actual admitted native Python executable changed')

    class RealOwnedProcesses(unittest.TestCase):
        ordinal = 0

        def make(self, text, *, timeout=5, cleanup_guard=None, popen_kwargs=None):
            guard()
            if common.opened(Path(python['path']), 512*1024**2) != python:
                raise ValueError('admitted test Python changed')
            directory = out/('%02d'%RealOwnedProcesses.ordinal)
            RealOwnedProcesses.ordinal += 1
            directory.mkdir(mode=0o700)
            owner = lease.OwnedCommand(common, directory, timeout, 128*1024**2, guard,
                cleanup_guard=guard if cleanup_guard is None else cleanup_guard)
            self.last_owner = owner
            record = {'scope': 'REAL_LINUX_PIDFD_SUBREAPER_ONLY_NOT_ROS_OR_BINFMT',
                      'argv': [python['path'], '-I', '-B', '-S', '-c', text],
                      'python': python, 'status': 'PENDING'}
            # Register cleanup before Popen/admission can fail. Tests never
            # rely on a destructor or on a later unrelated test to retire it.
            def finish():
                try:
                    record['cleanup'] = owner.close()
                    record['actual_launch'] = owner.launch
                    record['root_identity_after_cleanup'] = None if owner.child is None else owner.runner.roots.get(owner.child.pid)
                    record['stdout'] = common.opened(directory/'stdout.log', 1024**2)
                    record['python_preserved'] = common.opened(Path(python['path']), 512*1024**2) == python
                    record['status'] = 'RETIRED' if not record['cleanup']['remaining'] else 'CLEANUP_INCOMPLETE'
                except BaseException as error:
                    record['status'] = 'CLEANUP_INCOMPLETE';record['error'] = repr(error)
                    raise
                finally:
                    common.atomic_json(directory/'actual-process.json', record)
            self.addCleanup(finish)
            with (directory/'stdout.log').open('xb') as stream:
                child = owner.start(record['argv'], stdout=stream, stderr=stream, stdin=subprocess.DEVNULL, **(popen_kwargs or {}))
            record['root_identity'] = owner.runner.roots[child.pid]
            return owner, record

        def barrier(self, owner):
            cleanup = owner.close()
            self.assertEqual(cleanup['remaining'], [])
            self.assertTrue(cleanup['subreaper_restored'])
            self.assertEqual(owner.direct_children(), set())
            return cleanup

        def test_direct_zero_exit(self):
            owner, row = self.make('print("real-direct-zero")')
            self.assertEqual(owner.wait(), 0)
            self.assertEqual(self.barrier(owner)['status'], 'PASS')

        def test_direct_nonzero_is_actual_status(self):
            owner, row = self.make('raise SystemExit(7)')
            self.assertEqual(owner.wait(), 7)
            self.assertEqual(self.barrier(owner)['status'], 'PASS')

        def orphan(self, session=False, stopped=False):
            text = 'NEW_SESSION='+repr(session)+'\nSTOP_SELF='+repr(stopped)+'\n'+ORPHAN
            owner, row = self.make(text)
            with self.assertRaisesRegex(RuntimeError, 'live owned descendant'):
                owner.wait()
            result = self.barrier(owner)
            self.assertEqual(result['status'], 'PASS')
            self.assertGreaterEqual(len(result['adopted_births']), 1)
            self.assertGreaterEqual(len(result['reaped_adopted_births']), 1)
            self.assertTrue(any(x['method']=='P_PIDFD' for x in result['reaped_adopted_births']))
            return result

        def test_double_fork_orphan_adopted_and_reaped(self):
            self.orphan()

        def test_setsid_orphan_does_not_escape_pidfd_owner(self):
            self.orphan(session=True)

        def test_stopped_orphan_is_continued_then_retired(self):
            result = self.orphan(stopped=True)
            self.assertTrue(any(x['signal'] in (signal.SIGTERM, signal.SIGKILL) for x in result['signals']))

        def test_real_timeout_still_retires_original_birth(self):
            owner, row = self.make('import time; time.sleep(60)', timeout=1)
            with self.assertRaisesRegex(RuntimeError, 'deadline'):
                owner.wait()
            self.assertEqual(self.barrier(owner)['status'], 'PASS')

        def test_existing_direct_child_refuses_a_second_phase(self):
            owner, row = self.make('import time; time.sleep(60)')
            with self.assertRaisesRegex(RuntimeError, 'no pre-existing direct children'):
                lease.OwnedCommand(common, out, 1, 128*1024**2, guard)
            self.assertEqual(self.barrier(owner)['status'], 'PASS')

        def test_cleanup_observation_error_cannot_skip_pidfd_signals(self):
            once = [False]
            def interrupted_guard():
                guard()
                if not once[0]:
                    once[0] = True
                    raise InterruptedError('injected first cleanup observation interruption')
            owner, row = self.make('import time; time.sleep(60)', cleanup_guard=interrupted_guard)
            result = self.barrier(owner)
            self.assertEqual(result['status'], 'CLEANUP_INCOMPLETE')
            self.assertTrue(result['errors'])
            self.assertTrue(result['signals'])

        def test_first_identity_read_error_recovers_retained_direct_popen(self):
            original = common.identity
            once = [False]
            def fail_first(pid):
                if not once[0]:
                    once[0] = True
                    raise OSError('injected first kernel identity observation failure')
                return original(pid)
            common.identity = fail_first
            try:
                with self.assertRaisesRegex(OSError, 'first kernel identity'):
                    self.make('import time; time.sleep(60)')
            finally:
                common.identity = original
            # make() registered cleanup before admission; explicitly require
            # that recovery and terminal barrier now, rather than merely saving
            # an INCOMPLETE row in addCleanup and accidentally passing this case.
            owner = self.last_owner
            self.assertIsNotNone(owner.child)
            self.assertIsNone(owner.root_birth)
            self.assertEqual(self.barrier(owner)['status'], 'PASS')
            self.assertIsNotNone(owner.root_birth)

        def test_child_signal_mask_and_graceful_pidfd_term(self):
            # Exercise a nonempty inherited mask while TERM/INT are genuinely
            # unblocked. Restore the caller's exact original mask on all exits.
            prior = signal.pthread_sigmask(signal.SIG_BLOCK, set())
            expected = (set(prior) | {signal.SIGUSR1}) - {signal.SIGTERM, signal.SIGINT}
            text = ('import json, os, signal, sys, time\n'
                    'print("MASK_READY " + json.dumps({"mask": sorted(map(int, signal.pthread_sigmask(signal.SIG_BLOCK, set()))), '
                    '"pid": os.getpid(), "argv": sys.argv, "cwd": os.getcwd(), '
                    '"marker": os.environ.get("UWVM_ARM_MASK_TEST")}), flush=True)\n'
                    'time.sleep(60)\n')
            environment = dict(os.environ)
            environment['UWVM_ARM_MASK_TEST'] = 'unchanged-original-target-environment'
            signal.pthread_sigmask(signal.SIG_SETMASK, expected)
            try:
                owner, row = self.make(text, popen_kwargs={'env': environment, 'cwd': str(out)})
                deadline = time.monotonic()+3
                observed = None
                while observed is None:
                    guard()
                    if time.monotonic() >= deadline:
                        self.fail('actual target mask readiness timeout')
                    raw = (owner.out/'stdout.log').read_bytes()
                    self.assertLessEqual(len(raw), 8192)
                    lines = [line[len(b'MASK_READY '):] for line in raw.splitlines() if line.startswith(b'MASK_READY ')]
                    if lines:
                        self.assertEqual(len(lines), 1)
                        observed = json.loads(lines[0])
                    else:
                        time.sleep(.01)
                self.assertEqual(observed['mask'], sorted(map(int, expected)))
                self.assertNotIn(int(signal.SIGTERM), observed['mask'])
                self.assertNotIn(int(signal.SIGINT), observed['mask'])
                self.assertEqual(observed['pid'], owner.child.pid)
                self.assertEqual(observed['argv'], ['-c'])
                self.assertEqual(observed['cwd'], str(out))
                self.assertEqual(observed['marker'], environment['UWVM_ARM_MASK_TEST'])
                self.assertEqual(owner.launch['original_signal_mask'], observed['mask'])
                key = owner.root_birth
                retained = owner.runner.owned[key]
                before = common._ARM_ACTOR_SNAPSHOT(common, key, retained)
                self.assertFalse(before['terminal'])
                actual_executable = (Path('/proc')/str(owner.child.pid)/'exe').stat()
                self.assertEqual(tuple(getattr(actual_executable, name) for name in
                    ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')),
                    tuple(python[name] for name in ('device','inode','bytes','mtime_ns','ctime_ns')))
                cmdline = (Path('/proc')/str(owner.child.pid)/'cmdline').read_bytes()
                self.assertEqual(cmdline, b'\0'.join(os.fsencode(value) for value in row['argv'])+b'\0')
                after = common._ARM_ACTOR_SNAPSHOT(common, key, retained)
                self.assertFalse(after['terminal'])
                # This is the retained actual birth, never a numeric PID.
                signal.pidfd_send_signal(retained['fd'], signal.SIGTERM)
                self.assertEqual(owner.wait(), -signal.SIGTERM)
                cleanup = self.barrier(owner)
                self.assertEqual(cleanup['status'], 'PASS')
                self.assertFalse(any(item['signal'] == signal.SIGKILL for item in cleanup['signals']))
                row['actual_original_mask_and_target_semantics'] = observed
                row['actual_graceful_pidfd_TERM'] = {'signal': int(signal.SIGTERM),
                    'returncode': -int(signal.SIGTERM), 'SIGKILL_required': False,
                    'same_retained_birth': owner.runner.roots[owner.child.pid]}
            finally:
                signal.pthread_sigmask(signal.SIG_SETMASK, prior)

        def test_activation_interruption_restores_actual_subreaper(self):
            owner = lease.OwnedCommand(common, out, 1, 128*1024**2, guard)
            original = owner.libc.prctl
            once = [False]
            def interrupted_prctl(option, value, a, b, c):
                result = original(option, value, a, b, c)
                if option == 36 and value == 1 and not once[0]:
                    once[0] = True
                    raise InterruptedError('injected interruption AFTER actual subreaper activation')
                return result
            owner.libc.prctl = interrupted_prctl
            try:
                with self.assertRaisesRegex(InterruptedError, 'AFTER actual'):
                    owner.start([python['path'], '-I', '-B', '-S', '-c', 'raise SystemExit(0)'])
            finally:
                result = owner.close()
                owner.libc.prctl = original
            self.assertIsNone(owner.child)
            self.assertEqual(result['remaining'], [])
            self.assertEqual(result['status'], 'PASS')
            actual = lease.ctypes.c_int()
            self.assertEqual(original(37, lease.ctypes.addressof(actual), 0, 0, 0), 0)
            self.assertEqual(actual.value, owner.previous_subreaper)

    return unittest.defaultTestLoader.loadTestsFromTestCase(RealOwnedProcesses)
