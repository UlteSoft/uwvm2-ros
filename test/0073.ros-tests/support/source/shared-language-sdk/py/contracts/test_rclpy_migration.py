"""Explicit rclpy migration-profile logic tests; no native conformance claim."""
import unittest
from unittest.mock import patch
import rclpy
from rclpy.context import Context, AliasBinding
from rclpy.node import Node
from std_msgs.msg import String
from roswasm import Result, RosError
from test_contracts import LocalBackend


class RclpyMigration(unittest.TestCase):
    def setUp(self):
        self.backend = LocalBackend()
        self.context = Context()
        self.context.configure({
            ('topic_publish', '/topic', String._ROSWASM_TYPE): AliasBinding('publish', 10),
            ('topic_subscribe', '/topic', String._ROSWASM_TYPE): AliasBinding('subscribe', 10),
        })
        with patch('rclpy.context.Backend', return_value=self.backend):
            self.context.init()

    def test_original_publisher_spelling_and_fields(self):
        class MinimalPublisher(Node):
            def __init__(self, context):
                super().__init__('minimal_publisher', context=context)
                self.publisher_ = self.create_publisher(String, 'topic', 10)
            def publish_once(self):
                message = String(); message.data = 'Hello World: 0'; self.publisher_.publish(message)
        node = MinimalPublisher(self.context)
        node.publish_once()
        self.assertEqual(self.backend.calls[-1][0], 'topic_publish')
        self.assertEqual(self.backend.calls[-1][2], b'\x0e\0\0\0Hello World: 0')
        node.destroy_node(); self.context.shutdown()

    def test_subscription_original_callback_message(self):
        values = []
        node = Node('minimal_subscriber', context=self.context)
        node.create_subscription(String, 'topic', lambda message: values.append(message.data), 10)
        self.backend.replies['event_poll'] = [Result(0, String._encode(String(data='robot')))]
        rclpy.spin_once(node, timeout_sec=0)
        self.assertEqual(values, ['robot'])

    def test_missing_binding_has_no_open_effect(self):
        node = Node('minimal_publisher', context=self.context)
        before = len(self.backend.calls)
        with self.assertRaises(RosError) as error:
            node.create_publisher(String, 'other', 10)
        self.assertEqual(error.exception.error, 13)
        self.assertEqual(len(self.backend.calls), before)

    def test_qos_mismatch_has_no_open_effect(self):
        node = Node('minimal_publisher', context=self.context)
        before = len(self.backend.calls)
        with self.assertRaises(RosError) as error:
            node.create_publisher(String, 'topic', 3)
        self.assertEqual(error.exception.error, 17)
        self.assertEqual(len(self.backend.calls), before)

    def test_unsupported_native_features_not_ignored(self):
        node = Node('minimal_publisher', context=self.context)
        with self.assertRaises(NotImplementedError):
            rclpy.spin_once(node)
        with self.assertRaises(NotImplementedError):
            node.create_publisher(String, 'topic', 10, event_callbacks=object())
        with self.assertRaises(NotImplementedError):
            rclpy.spin(node)

    def test_private_namespace_resolution(self):
        node = Node('robot', namespace='/fleet', context=self.context)
        self.assertEqual(node.resolve_topic_name('~/status'), '/fleet/robot/status')
        self.assertEqual(node.resolve_topic_name('status'), '/fleet/status')
        with self.assertRaises(ValueError):
            node.resolve_topic_name('../status')


    def test_failed_abi_init_never_commits_live_context(self):
        context = Context()
        self.backend.replies['abi_version'] = [Result(0, value=2)]
        with patch('rclpy.context.Backend', return_value=self.backend), self.assertRaises(RosError):
            context.init()
        self.assertFalse(context.ok())
        self.assertIsNone(context.session)
        before = len(self.backend.calls)
        with self.assertRaises(RuntimeError):
            Node('not_initialized', context=context)
        self.assertEqual(len(self.backend.calls), before)

    def test_destroyed_node_logger_never_opens(self):
        node = Node('logger', context=self.context)
        self.context.bindings[('log_write', '/logger', '')] = AliasBinding('log')
        logger = node.get_logger(); node.destroy_node()
        before = len(self.backend.calls)
        with self.assertRaises(RuntimeError):
            logger.info('after destruction')
        self.assertEqual(len(self.backend.calls), before)

    def test_first_logger_open_during_callback_refuses_without_effect(self):
        node = Node('logger', context=self.context)
        self.context.bindings[('log_write', '/logger', '')] = AliasBinding('log')
        errors = []
        def callback(_):
            try:
                node.get_logger().info('first callback log')
            except RosError as error:
                errors.append(error.error)
        node.create_subscription(String, 'topic', callback, 10)
        opens = len([c for c in self.backend.calls if c[0] == 'capability_open'])
        self.backend.replies['event_poll'] = [Result(0, String._encode(String(data='one')))]
        rclpy.spin_once(node, timeout_sec=0)
        self.assertEqual(errors, [17])
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_open']), opens)
        self.assertIsNone(node._logger_owner)

    def test_existing_logger_remains_usable_in_callback(self):
        node = Node('logger', context=self.context)
        self.context.bindings[('log_write', '/logger', '')] = AliasBinding('log')
        node.get_logger().info('warm')
        node.create_subscription(String, 'topic', lambda _: node.get_logger().info('callback'), 10)
        opens = len([c for c in self.backend.calls if c[0] == 'capability_open'])
        self.backend.replies['event_poll'] = [Result(0, String._encode(String(data='one')))]
        rclpy.spin_once(node, timeout_sec=0)
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'log_write']), 2)
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_open']), opens)

    def test_publisher_registration_failure_immediately_closes_owner(self):
        class RefuseSet(list):
            def __setitem__(self, index, value):
                raise MemoryError('registration')
        node = Node('pub', context=self.context); node.publishers = RefuseSet()
        self.backend.replies['capability_close'] = [Result(107)]
        with self.assertRaises(MemoryError):
            node.create_publisher(String, 'topic', 10)
        self.assertEqual(node._owners, [])
        self.assertEqual(node.publishers, [])
        frame = self.context.session.last_factory_cleanup
        self.assertEqual(frame.outcomes[0].error, 107)
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_close']), 1)

    def test_subscription_registration_failure_undoes_all_positions(self):
        class RefuseSet(list):
            def __setitem__(self, index, value):
                raise MemoryError('subscription registry')
        node = Node('sub', context=self.context); node.subscriptions = RefuseSet()
        with self.assertRaises(MemoryError):
            node.create_subscription(String, 'topic', lambda _: None, 10)
        self.assertEqual(node._owners, [])
        self.assertEqual(node._sources, [])
        self.assertEqual(node.subscriptions, [])
        self.assertEqual(node._executor.sources, [])
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_close']), 1)
        self.assertEqual(self.context.session.owners[0].handle, 0)

    def test_registration_append_failure_precedes_open(self):
        class RefuseAppend(list):
            def append(self, value):
                raise MemoryError('reservation')
        node = Node('sub', context=self.context); node.subscriptions = RefuseAppend()
        before = len(self.backend.calls)
        with self.assertRaises(MemoryError):
            node.create_subscription(String, 'topic', lambda _: None, 10)
        self.assertEqual(len(self.backend.calls), before)
        self.assertEqual(node._owners, [])
        self.assertEqual(node._sources, [])
        self.assertEqual(node._executor.sources, [])

    def test_executor_257th_source_rejected_before_open_or_clock(self):
        context = Context()
        context.configure(self.context.bindings, maximum_owners=512)
        with patch('rclpy.context.Backend', return_value=self.backend):
            context.init()
        node = Node('bounded', context=context)
        for _ in range(256):
            node.create_subscription(String, 'topic', lambda _: None, 10)
        before = len(self.backend.calls)
        for create in (lambda: node.create_subscription(String, 'topic', lambda _: None, 10),
                       lambda: node.create_timer(0.5, lambda: None)):
            with self.assertRaises(RosError) as caught:
                create()
            self.assertEqual(caught.exception.error, 90)
        self.assertEqual(len(self.backend.calls), before)
        self.assertEqual(len(node._executor.sources), 256)
        node.destroy_node(); context.shutdown()

    def test_257th_publisher_rejected_before_open(self):
        context = Context(); context.configure(self.context.bindings, maximum_owners=512)
        with patch('rclpy.context.Backend', return_value=self.backend):
            context.init()
        node = Node('bounded', context=context)
        for _ in range(256):
            node.create_publisher(String, 'topic', 10)
        before = len(self.backend.calls)
        with self.assertRaises(RosError) as caught:
            node.create_publisher(String, 'topic', 10)
        self.assertEqual(caught.exception.error, 90)
        self.assertEqual(len(self.backend.calls), before)
        node.destroy_node(); context.shutdown()

    def test_lazy_clock_registration_failure_closes_once(self):
        class RefuseSet(list):
            def __setitem__(self, index, value):
                raise MemoryError('clock registry')
        node = Node('clock', context=self.context); node._owners = RefuseSet()
        self.context.bindings[('time_read_ros', '/clock', '')] = AliasBinding('clock')
        with self.assertRaises(MemoryError):
            node.get_clock()
        self.assertIsNone(node._clock)
        self.assertEqual(node._owners, [])
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_close']), 1)

    def test_timer_setup_failure_closes_only_new_lazy_clock(self):
        node = Node('timer', context=self.context)
        self.context.bindings[('time_read_ros', '/timer', '')] = AliasBinding('clock')
        self.backend.replies['time_read_ros'] = [Result(110)]
        with self.assertRaises(RosError) as caught:
            node.create_timer(0.5, lambda: None)
        self.assertEqual(caught.exception.error, 110)
        self.assertIsNone(node._clock)
        self.assertEqual(node._owners, [])
        self.assertEqual(node.timers, [])
        self.assertEqual(node._executor.sources, [])
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_close']), 1)

    def test_destroy_node_attempts_all_owners_after_close_exception(self):
        node = Node('cleanup', context=self.context)
        node.create_publisher(String, 'topic', 10); node.create_publisher(String, 'topic', 10)
        first = MemoryError('close')
        self.backend.replies['capability_close'] = [first, Result(107)]
        with self.assertRaises(RuntimeError):
            node.destroy_node()
        report = node._destroy_cleanup
        self.assertIs(report.outcomes[0], first)
        self.assertEqual(report.outcomes[1].error, 107)
        self.assertTrue(report.completed)
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_close']), 2)
        self.assertTrue(all(owner.handle == 0 for owner in node._owners))

    def test_shutdown_attempts_every_callback_with_reserved_failures(self):
        called = []
        def fail():
            called.append('first'); raise MemoryError('callback')
        self.context.on_shutdown(fail)
        self.context.on_shutdown(lambda: called.append('second'))
        with self.assertRaises(RuntimeError) as caught:
            self.context.shutdown()
        self.assertEqual(called, ['first', 'second'])
        self.assertIs(caught.exception, self.context.last_shutdown_cleanup)
        self.assertIsInstance(caught.exception.roswasm_failures[0], MemoryError)
        self.assertFalse(self.context.ok())

    def test_context_manager_diagnostic_refusal_preserves_application_primary(self):
        class Primary(LookupError):
            def __setattr__(self, name, value):
                if name.startswith('roswasm_'):
                    raise MemoryError('diagnostic')
                object.__setattr__(self, name, value)
        def fail():
            raise MemoryError('shutdown callback')
        self.context.on_shutdown(fail)
        primary = Primary('application')
        with self.assertRaises(Primary) as caught:
            with rclpy.InitContextManager(self.context):
                raise primary
        self.assertIs(caught.exception, primary)
        self.assertIsInstance(self.context.last_shutdown_cleanup.roswasm_failures[0], MemoryError)


    def test_init_context_manager_allocation_precedes_context_commit(self):
        context = Context()
        before = len(self.backend.calls)
        with patch('rclpy.InitContextManager', side_effect=MemoryError('manager')):
            with patch('rclpy.context.Backend', return_value=self.backend):
                with self.assertRaises(MemoryError):
                    rclpy.init(context=context)
        self.assertFalse(context.ok())
        self.assertIsNone(context.session)
        self.assertEqual(len(self.backend.calls), before)


    def test_busy_context_shutdown_refuses_before_callbacks_or_closes(self):
        node = Node('busy', context=self.context)
        callbacks, statuses = [], []
        self.context.on_shutdown(lambda: callbacks.append('shutdown'))
        def on_message(_):
            try:
                self.context.shutdown()
            except RosError as error:
                statuses.append(error.error)
        node.create_subscription(String, 'topic', on_message, 10)
        self.backend.replies['event_poll'] = [Result(0, String._encode(String(data='one')))]
        rclpy.spin_once(node, timeout_sec=0)
        self.assertEqual(statuses, [17])
        self.assertEqual(callbacks, [])
        self.assertTrue(self.context.ok())
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_close']), 0)


if __name__ == '__main__':
    unittest.main(verbosity=2)
