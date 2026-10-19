"""Unexecuted migration-profile regression SOURCE; no native ROS claim.

Run only in the agreed SSH Linux cgroup, after overlaying the reviewed SDK in
an isolated directory. LocalBackend is the existing explicit contract double.
These tests exercise actual rclpy lifecycle code, not a parallel manager model.
"""
import unittest
from unittest.mock import patch

import rclpy
from rclpy.context import Context
from test_contracts import LocalBackend


class DefaultInitContext(unittest.TestCase):
    def setUp(self):
        self.previous = rclpy._default_context
        rclpy._default_context = None
        self.backend = LocalBackend()
        self.backend_patch = patch('rclpy.context.Backend', return_value=self.backend)
        self.backend_patch.start()

    def tearDown(self):
        try:
            selected = rclpy._default_context
            if selected is not None:
                selected.try_shutdown()
        finally:
            rclpy._default_context = self.previous
            self.backend_patch.stop()

    def test_two_default_with_blocks_create_distinct_contexts(self):
        seen = []
        for _ in range(2):
            with rclpy.init() as manager:
                seen.append(manager.context)
                self.assertIs(rclpy.get_default_context(), manager.context)
                self.assertTrue(manager.context.ok())
            self.assertFalse(manager.context.ok())
            self.assertIsNone(rclpy._default_context)
        self.assertIsNot(seen[0], seen[1])
        self.assertEqual([call[0] for call in self.backend.calls],
                         ['abi_version', 'abi_version'])

    def test_explicit_context_does_not_create_or_replace_default(self):
        explicit = Context()
        with rclpy.init(context=explicit):
            self.assertTrue(explicit.ok())
            self.assertIsNone(rclpy._default_context)
        self.assertFalse(explicit.ok())
        self.assertIsNone(rclpy._default_context)
        with self.assertRaisesRegex(RuntimeError, 'only once'):
            rclpy.init(context=explicit)

    def test_explicit_context_preserves_an_unrelated_default(self):
        default = rclpy.get_default_context()
        explicit = Context()
        with rclpy.init(context=explicit):
            self.assertIs(rclpy._default_context, default)
        self.assertIs(rclpy._default_context, default)
        self.assertIsNot(default, explicit)

    def test_primary_exception_and_shutdown_diagnostic_survive_default_reset(self):
        primary = ValueError('application primary')
        callback_error = RuntimeError('shutdown callback')

        def fail_callback():
            raise callback_error

        with self.assertRaises(ValueError) as caught:
            with rclpy.init() as manager:
                manager.context.on_shutdown(fail_callback)
                raise primary
        self.assertIs(caught.exception, primary)
        cleanup = manager.context.last_shutdown_cleanup
        self.assertIs(primary.roswasm_cleanup, cleanup)
        self.assertIn(callback_error, tuple(cleanup.roswasm_failures))
        self.assertFalse(manager.context.ok())
        self.assertIsNone(rclpy._default_context)

    def test_pre_effect_shutdown_failure_keeps_the_live_default(self):
        manager = rclpy.init()
        selected = manager.context
        failure = MemoryError('before shutdown effects')
        with patch.object(selected, 'try_shutdown', side_effect=failure):
            with self.assertRaises(MemoryError) as caught:
                manager.__exit__(None, None, None)
        self.assertIs(caught.exception, failure)
        self.assertIs(rclpy._default_context, selected)
        self.assertTrue(selected.ok())
        rclpy.try_shutdown(context=selected)
        self.assertFalse(selected.ok())
        self.assertIsNone(rclpy._default_context)


if __name__ == '__main__':
    unittest.main()
