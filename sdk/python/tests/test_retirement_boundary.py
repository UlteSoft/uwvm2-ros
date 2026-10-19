"""UNEXECUTED source: requires retirement_bridge_fixture.c as the built-in bridge.

The counted C callees are contract doubles, not ROS transport qualification.
No production fallback, source execution, compilation or test was performed
while preparing this candidate.
"""
import unittest
from unittest.mock import patch
import _roswasm_native as native
from roswasm import Backend, Result, RosError, Session
from roswasm.client import Capability, _FactoryCleanup
from rclpy.context import Context, AliasBinding
from rclpy.node import Node
from std_msgs.msg import String
import rclpy


class RetirementBoundary(unittest.TestCase):
    def setUp(self):
        native.fixture_arm(0, 0)
        self.backend = Backend(2)
        self.session = Session(self.backend, 16)

    def owner(self, handle=(1 << 64) - 1, alias='a'):
        owner = Capability(self.session, handle, alias)
        self.session.owners.append(owner)
        return owner

    def test_python_pre_entry_failure_keeps_full_id(self):
        owner = self.owner()
        original = self.backend._invoke
        def before_entry(*args):
            raise MemoryError('representative pre-C-entry failure')
        self.backend._invoke = before_entry
        with self.assertRaises(MemoryError):
            owner.close()
        self.assertEqual(owner.handle, (1 << 64) - 1)
        self.assertEqual(native.fixture_stats()[0], 0)
        self.assertFalse(owner._close_attempt[0])
        self.backend._invoke = original
        owner.close().checked()
        self.assertEqual(native.fixture_stats(), (1, (1 << 64) - 1, 1))

    def test_actual_bridge_pre_import_allocation_failure_keeps_id(self):
        owner = self.owner(1 << 63)
        native.fixture_arm(1, 0)
        with self.assertRaises(MemoryError):
            owner.close()
        self.assertEqual(owner.handle, 1 << 63)
        self.assertEqual(native.fixture_stats()[0], 0)
        self.assertFalse(owner._close_attempt[0])
        owner.close().checked()
        self.assertEqual(native.fixture_stats(), (1, 1 << 63, 1))

    def test_post_import_result_failure_never_replays(self):
        owner = self.owner()
        native.fixture_arm(2, 0)
        with self.assertRaises(MemoryError):
            owner.close()
        self.assertTrue(owner._close_attempt[0])
        self.assertEqual(owner.handle, 0)
        self.assertEqual(owner._raw_handle, (1 << 64) - 1)
        self.assertEqual(owner.close().error, 9)
        self.assertEqual(native.fixture_stats(), (1, (1 << 64) - 1, 1))

    def test_errno_unknown_is_retired_and_never_replayed(self):
        for code in (107, 110):
            owner = self.owner()
            native.fixture_arm(0, code)
            self.assertEqual(owner.close().error, code)
            self.assertEqual(owner.handle, 0)
            self.assertEqual(owner.close().error, 9)
            self.assertEqual(native.fixture_stats()[0], 1)

    def test_python_result_allocation_failure_never_replays(self):
        owner = self.owner()
        with patch('roswasm.raw.Result', side_effect=MemoryError('Python Result')):
            with self.assertRaises(MemoryError):
                owner.close()
        self.assertTrue(owner._close_attempt[0])
        self.assertEqual(owner.handle, 0)
        self.assertEqual(owner.close().error, 9)
        self.assertEqual(native.fixture_stats(), (1, (1 << 64) - 1, 1))

    def test_revoke_preparation_failure_does_not_retire_siblings(self):
        first = self.owner(1 << 63)
        second = self.owner((1 << 64) - 1)
        native.fixture_arm(1, 0)
        with self.assertRaises(MemoryError):
            first.revoke()
        self.assertEqual((first.handle, second.handle), (1 << 63, (1 << 64) - 1))
        first.close().checked()
        self.assertEqual(first.handle, 0)
        self.assertEqual(second.handle, (1 << 64) - 1)
        second.revoke().checked()
        self.assertEqual(second.handle, 0)
        self.assertEqual(native.fixture_stats()[0], 2)

    def test_revoke_post_import_failure_retires_only_same_alias(self):
        first = self.owner(1 << 63)
        second = self.owner((1 << 64) - 1)
        other = self.owner(9, 'different')
        native.fixture_arm(2, 0)
        with self.assertRaises(MemoryError):
            first.revoke()
        self.assertEqual((first.handle, second.handle, other.handle), (0, 0, 9))
        self.assertEqual(first.revoke().error, 9)
        self.assertEqual(native.fixture_stats(), (1, 1 << 63, 2))

    def test_managed_generic_invoke_cannot_bypass_retirement_witness(self):
        for operation, kind in (('capability_close', 1), ('capability_revoke', 2)):
            owner = self.owner()
            native.fixture_arm(2, 0)
            with self.assertRaises(MemoryError):
                owner.invoke(operation)
            self.assertEqual(owner.handle, 0)
            self.assertEqual(owner.invoke(operation).error, 9)
            self.assertEqual(native.fixture_stats(), (1, (1 << 64) - 1, kind))

    def test_managed_generic_open_cannot_bypass_owner_ledger(self):
        owner = self.owner()
        self.session.maximum = len(self.session.owners)  # No owner capacity left.
        ledger = tuple(self.session.owners)
        full_id = owner.handle
        original = self.backend._invoke
        calls = []
        def forbidden_effect(*args):
            calls.append(args)
            raise AssertionError('managed generic open reached the native bridge')
        self.backend._invoke = forbidden_effect
        try:
            with self.assertRaisesRegex(ValueError, r'^use Session\.open$'):
                owner.invoke('capability_open', payload=b'new-alias')
            foreign = Capability(Session(self.backend, 1), 7, 'foreign')
            self.assertEqual(owner.invoke('capability_open', other=foreign).error, 9)
        finally:
            self.backend._invoke = original
        self.assertEqual(calls, [])
        self.assertEqual(tuple(self.session.owners), ledger)
        self.assertEqual(owner.handle, full_id)
        self.assertFalse(owner._close_attempt[0])
        self.assertFalse(owner._revoke_attempt[0])

    def test_inactive_generic_refuses_but_explicit_unattempted_close_remains(self):
        owner = self.owner()
        full_id = owner.handle
        ledger = tuple(self.session.owners)
        native.fixture_arm(1, 0)
        report = self.session.close()
        self.assertIsInstance(report.outcomes[0], MemoryError)
        self.assertFalse(self.session.active)
        self.assertEqual(owner.handle, full_id)
        self.assertFalse(owner._close_attempt[0])
        for operation in ('capability_open', 'capability_close', 'capability_revoke'):
            self.assertEqual(owner.invoke(operation).error, 125)
        self.assertEqual(native.fixture_stats()[0], 0)
        self.assertEqual(tuple(self.session.owners), ledger)
        owner.close().checked()  # Explicit known-NotAttempted cleanup only.
        self.assertTrue(owner._close_attempt[0])
        self.assertEqual(owner.handle, 0)
        self.assertEqual(native.fixture_stats(), (1, full_id, 1))

    def test_invalid_c_witness_has_no_effect(self):
        for witness in ([True], [False, False], (False,), None):
            with self.assertRaises(ValueError):
                native.invoke('capability_close', (1 << 64) - 1, b'', 0, 0, 0, witness)
        self.assertEqual(native.fixture_stats()[0], 0)

    def test_session_preserves_not_attempted_ledger_after_shutdown(self):
        first = self.owner(8, 'first')
        second = self.owner(9, 'second')
        native.fixture_arm(1, 0)
        report = self.session.close()
        self.assertIsInstance(report.outcomes[0], MemoryError)
        self.assertFalse(self.session.active)
        self.assertEqual(self.session.owners, [first, second])
        self.assertEqual((first.handle, second.handle), (0, 9))
        second.close().checked()  # Explicit, known NotAttempted recovery only.
        self.assertEqual(native.fixture_stats()[0], 2)

    def test_factory_distinguishes_local_from_native_attempt(self):
        owner = self.owner()
        frame = _FactoryCleanup(self.session, 1)
        frame.seed((owner,))
        native.fixture_arm(1, 0)
        primary = ValueError('original factory failure')
        frame.rollback(primary)
        self.assertIs(frame.primary, primary)
        self.assertEqual(frame.attempted_flags, [True])
        self.assertEqual(frame.native_attempted_flags, [False])
        self.assertEqual(owner.handle, (1 << 64) - 1)
        self.assertIsInstance(frame.results[0], MemoryError)
        frame.rollback(primary)
        self.assertEqual(native.fixture_stats()[0], 0)  # No automatic retry.

    def context_and_subscription(self):
        context = Context()
        context.configure({('topic_subscribe', '/topic', String._ROSWASM_TYPE):
                           AliasBinding('subscribe', 10)})
        with patch('rclpy.context.Backend', return_value=self.backend):
            context.init()
        node = Node('subscriber', context=context)
        sub = node.create_subscription(String, '/topic', lambda value: None, 10)
        return context, node, sub

    def test_destroy_retired_subscription_removes_dead_poll_source(self):
        context, node, sub = self.context_and_subscription()
        native.fixture_arm(0, 107)
        with self.assertRaises(RosError) as caught:
            node.destroy_subscription(sub)
        self.assertEqual(caught.exception.error, 107)
        self.assertEqual(sub.owner.handle, 0)
        for collection in (node.subscriptions, node._sources, node._executor.sources):
            self.assertNotIn(sub, collection)
        self.assertFalse(node.destroy_subscription(sub))
        rclpy.spin_once(node, timeout_sec=0)
        self.assertEqual(native.fixture_stats()[0], 1)
        context.shutdown()

    def test_destroy_pre_import_failure_retains_live_subscription(self):
        context, node, sub = self.context_and_subscription()
        native.fixture_arm(1, 0)
        with self.assertRaises(MemoryError):
            node.destroy_subscription(sub)
        self.assertNotEqual(sub.owner.handle, 0)
        for collection in (node.subscriptions, node._sources, node._executor.sources):
            self.assertIn(sub, collection)
        self.assertEqual(native.fixture_stats()[0], 0)
        self.assertTrue(node.destroy_subscription(sub))
        self.assertEqual(native.fixture_stats()[0], 1)
        context.shutdown()

    def test_destroy_cleanup_does_not_replace_close_primary(self):
        context, node, sub = self.context_and_subscription()
        native.fixture_arm(0, 110)
        with patch.object(node._executor, 'remove', side_effect=MemoryError('scheduler remove')):
            with self.assertRaises(RosError) as caught:
                node.destroy_subscription(sub)
        self.assertEqual(caught.exception.error, 110)
        self.assertIsInstance(caught.exception.roswasm_destroy_cleanup, MemoryError)
        self.assertIsInstance(sub._destroy_cleanup[0], MemoryError)
        self.assertNotIn(sub, node.subscriptions)
        self.assertNotIn(sub, node._sources)
        self.assertEqual(native.fixture_stats()[0], 1)
        context.shutdown()


if __name__ == '__main__':
    unittest.main()
