"""Deterministic DATA races around the real owned-actor snapshot function.

Root calls suite(actual_manifest_bound_unitree_module) inside its current64
policy owner. This file never imports a candidate module, creates a process,
signals a PID or claims a kernel/ROS result. Injected observations exercise
specific interleavings which a high-frequency real-child run cannot promise
will occur. The separate stress entry uses real Linux children and pidfds.
"""
import errno
from pathlib import Path
import types
import unittest


def suite(common):
    unitree = common
    class ProcfsRaces(unittest.TestCase):
        def fixture(self,identities=None,polls=None,membership=None,key=None,own_error=None):
            original={'pid':12345,'parent_pid':100,'starttime':765,'state':'S'}
            identities=list(identities if identities is not None else [original,original])
            polls=list(polls if polls is not None else [False,False])
            membership=Path('/proc/self/cgroup').read_text() if membership is None else membership
            calls={'identity':[],'pidfd':[],'membership':[]}
            def identity(pid):
                calls['identity'].append(pid);self.assertEqual(pid,original['pid'])
                value=identities.pop(0)
                if isinstance(value,BaseException):raise value
                return value
            def terminal(fd):
                calls['pidfd'].append(fd);self.assertEqual(fd,401)
                value=polls.pop(0)
                if isinstance(value,BaseException):raise value
                return value
            def read(pid):
                calls['membership'].append(pid);self.assertEqual(pid,original['pid'])
                if isinstance(membership,BaseException):raise membership
                return membership
            def own():
                if own_error is not None:raise own_error
                return Path('/proc/self/cgroup').read_text()
            common=types.SimpleNamespace(identity=identity,pidfd_terminal=terminal,
                birth=lambda row:None if row is None else(row['pid'],row['starttime']))
            item={'identity':original,'fd':401}
            return lambda:unitree._owned_actor_snapshot(common,key or (12345,765),item,read_membership=read,read_self_membership=own),calls,item
        def changed(self):return {'pid':12345,'parent_pid':900,'starttime':766,'state':'S'}
        def test_live_original_snapshot(self):
            run,calls,item=self.fixture();value=run()
            self.assertFalse(value['terminal']);self.assertEqual(value['identity'],item['identity'])
            self.assertEqual(calls['pidfd'],[401,401])
        def test_initial_terminal_does_not_touch_numeric_procfs(self):
            run,calls,_=self.fixture(identities=[],polls=[True]);value=run()
            self.assertTrue(value['terminal']);self.assertEqual(calls['identity'],[]);self.assertEqual(calls['membership'],[])
        def test_before_identity_enoent_needs_same_terminal_fd(self):
            run,calls,_=self.fixture([FileNotFoundError(errno.ENOENT,'injected vanished stat')],[False,True])
            self.assertTrue(run()['terminal']);self.assertEqual(calls['pidfd'],[401,401])
        def test_before_identity_esrch_needs_same_terminal_fd(self):
            run,_,_=self.fixture([ProcessLookupError(errno.ESRCH,'injected exited task')],[False,True])
            self.assertTrue(run()['terminal'])
        def test_identity_none_terminal_is_old_retirement(self):
            run,calls,_=self.fixture([None],[False,True]);self.assertTrue(run()['terminal'])
            self.assertEqual(calls['membership'],[])
        def test_cgroup_disappears_after_live_identity(self):
            run,calls,_=self.fixture(polls=[False,True],membership=FileNotFoundError(errno.ENOENT,'injected cgroup exit'))
            value=run();self.assertTrue(value['terminal']);self.assertEqual(value['phase'],'CGROUP_PROCFS_ABSENT')
            self.assertEqual(calls['identity'],[12345])
        def test_cgroup_esrch_after_live_identity(self):
            run,_,_=self.fixture(polls=[False,True],membership=ProcessLookupError(errno.ESRCH,'injected cgroup exit'))
            self.assertTrue(run()['terminal'])
        def test_after_cgroup_identity_disappears(self):
            old={'pid':12345,'parent_pid':100,'starttime':765,'state':'S'}
            run,_,_=self.fixture([old,None],[False,True]);self.assertEqual(run()['phase'],'AFTER_CGROUP_BIRTH_UNAVAILABLE_OR_CHANGED')
        def test_after_cgroup_stat_enoent(self):
            old={'pid':12345,'parent_pid':100,'starttime':765,'state':'S'}
            run,_,_=self.fixture([old,FileNotFoundError(errno.ENOENT,'injected trailing stat')],[False,True])
            self.assertEqual(run()['phase'],'AFTER_CGROUP_PROCFS_ABSENT')
        def test_exit_after_all_procfs_reads(self):
            run,_,_=self.fixture(polls=[False,True]);self.assertEqual(run()['phase'],'FINAL_POLL')
        def test_absent_cgroup_with_live_pidfd_keeps_real_error(self):
            run,_,_=self.fixture(polls=[False,False],membership=FileNotFoundError(errno.ENOENT,'injected still-live absence'))
            with self.assertRaises(FileNotFoundError) as found:run()
            self.assertEqual(found.exception.errno,errno.ENOENT)
        def test_absent_identity_with_live_pidfd_refused(self):
            run,_,_=self.fixture([None],[False,False])
            with self.assertRaisesRegex(ValueError,'without terminal retained pidfd'):run()
        def test_reused_birth_before_cgroup_only_retires_old_terminal_fd(self):
            run,calls,item=self.fixture([self.changed()],[False,True]);self.assertTrue(run()['terminal'])
            self.assertEqual(item['identity']['starttime'],765);self.assertEqual(calls['membership'],[])
        def test_reused_birth_before_cgroup_with_live_fd_refused(self):
            run,calls,_=self.fixture([self.changed()],[False,False])
            with self.assertRaisesRegex(ValueError,'without terminal retained pidfd'):run()
            self.assertEqual(calls['membership'],[])
        def test_reused_birth_after_cgroup_only_retires_original_terminal_fd(self):
            old={'pid':12345,'parent_pid':100,'starttime':765,'state':'S'}
            run,_,item=self.fixture([old,self.changed()],[False,True]);self.assertTrue(run()['terminal'])
            self.assertEqual(item['identity'],old)
        def test_reused_birth_after_cgroup_live_fd_refused(self):
            old={'pid':12345,'parent_pid':100,'starttime':765,'state':'S'}
            run,_,_=self.fixture([old,self.changed()],[False,False])
            with self.assertRaisesRegex(ValueError,'without terminal retained pidfd'):run()
        def test_permission_denial_never_becomes_exit(self):
            run,calls,_=self.fixture(polls=[False,True],membership=PermissionError(errno.EACCES,'injected procfs permission'))
            with self.assertRaises(PermissionError):run()
            self.assertEqual(calls['pidfd'],[401])
        def test_unknown_identity_io_error_never_becomes_exit(self):
            run,calls,_=self.fixture([OSError(errno.EIO,'injected stat I/O error')],[False,True])
            with self.assertRaises(OSError) as found:run()
            self.assertEqual(found.exception.errno,errno.EIO);self.assertEqual(calls['pidfd'],[401])
        def test_invalid_initial_pidfd_refused(self):
            run,calls,_=self.fixture(polls=[OSError(errno.EBADF,'injected invalid retained fd')])
            with self.assertRaises(OSError):run()
            self.assertEqual(calls['identity'],[])
        def test_invalid_recheck_pidfd_refused(self):
            run,_,_=self.fixture([None],[False,OSError(errno.EBADF,'injected invalid recheck fd')])
            with self.assertRaises(OSError) as found:run()
            self.assertEqual(found.exception.errno,errno.EBADF)
        def test_live_membership_violation_not_erased_by_later_exit(self):
            run,calls,_=self.fixture(polls=[False,True],membership='0::/different-owned-leaf\n')
            with self.assertRaisesRegex(ValueError,'crossed cgroup boundary'):run()
            self.assertEqual(calls['pidfd'],[401])
        def test_record_key_cannot_adopt_a_new_birth(self):
            run,calls,_=self.fixture(key=(12345,766))
            with self.assertRaisesRegex(ValueError,'original birth'):run()
            self.assertEqual(calls['pidfd'],[]);self.assertEqual(calls['identity'],[])
        def test_mismatched_membership_precedes_trailing_vanished_birth(self):
            old={'pid':12345,'parent_pid':100,'starttime':765,'state':'S'}
            run,calls,_=self.fixture([old,None],[False,True],membership='0::/different-observed-leaf\n')
            with self.assertRaisesRegex(ValueError,'crossed cgroup boundary'):run()
            self.assertEqual(calls['identity'],[12345]);self.assertEqual(calls['pidfd'],[401])
        def test_mismatched_membership_precedes_trailing_reused_birth(self):
            old={'pid':12345,'parent_pid':100,'starttime':765,'state':'S'}
            run,calls,_=self.fixture([old,self.changed()],[False,True],membership='0::/different-observed-leaf\n')
            with self.assertRaisesRegex(ValueError,'crossed cgroup boundary'):run()
            self.assertEqual(calls['identity'],[12345]);self.assertEqual(calls['pidfd'],[401])
        def test_actual_own_cgroup_read_error_never_means_child_exit(self):
            run,calls,_=self.fixture(polls=[False,True],own_error=OSError(errno.EIO,'injected own cgroup I/O error'))
            with self.assertRaises(OSError) as found:run()
            self.assertEqual(found.exception.errno,errno.EIO)
            self.assertEqual(calls['identity'],[12345]);self.assertEqual(calls['pidfd'],[401])
    return unittest.defaultTestLoader.loadTestsFromTestCase(ProcfsRaces)
