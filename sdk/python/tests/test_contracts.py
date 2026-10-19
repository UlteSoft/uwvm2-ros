"""Local SDK logic tests, explicitly not Runtime/Gateway/native ROS acceptance.

Run from a /tmp copy with PYTHONDONTWRITEBYTECODE=1. The deterministic backend
below exercises caller-side wire, ownership and rollback failures. Separate real
Wasm default-denied and granted lessons are required for ROS qualification.
"""
import struct
import unittest
from unittest.mock import patch

from roswasm import Backend, Result, RosError, Session, Node, Executor, codecs, events
from roswasm._registry import FUNCTIONS


class LocalBackend(Backend):
    """Local test double; every call is recorded and no external I/O exists."""
    def __init__(self, version=2):
        self.version, self.functions = version, FUNCTIONS[version]
        self.calls, self.replies, self.next_handle = [], {}, 1

    def invoke(self, name, handle=0, payload=b'', capacity=0, other=0, correlation=0):
        self.calls.append((name, handle, payload, capacity, other, correlation))
        queued = self.replies.get(name, [])
        if queued:
            value = queued.pop(0)
            if isinstance(value, BaseException):
                raise value
            return value
        if name == 'abi_version':
            return Result(0, value=1)
        if name == 'capability_open':
            handle = self.next_handle; self.next_handle += 1
            return Result(0, value=handle)
        return Result(0)

    def retire(self, name, handle, witness):
        # This historical deterministic model treats its invoke entry as the
        # effect boundary. It cannot qualify the real C bridge; the separate
        # retirement fault fixture covers that source path.
        if type(witness) is not list or len(witness) != 1 or witness[0] is not False:
            raise ValueError('unattempted witness required')
        witness[0] = True
        return self.invoke(name, handle)


def event(sequence=1, source=1, *, kind=1, correlation=0, body=b'', flags=0,
          error=0, backend=2, instance=9, session=7):
    """Independent struct fixture for the published64-byte envelope contract."""
    return struct.pack('<IHHBBHIQQQQQQ', 0x45525755, 1, kind, backend, flags,
                       0, error, source, correlation, sequence, instance, session, len(body)) + body


class Codecs(unittest.TestCase):
    def test_unicode_exact(self):
        for value in ('', 'a\0b', '\ufeffrobot', '机器人🙂'):
            with self.subTest(value=value):
                wire = codecs.String.encode(value)
                self.assertEqual(int.from_bytes(wire[:4], 'little'), len(value.encode()))
                self.assertEqual(codecs.String.decode(wire), value)

    def test_bad_strings(self):
        for wire in (b'', b'\1\0\0\0', b'\1\0\0\0\xff', b'\0\0\0\0x', b'\3\0\0\0\xed\xa0\x80'):
            with self.subTest(wire=wire), self.assertRaises((ValueError, UnicodeError)):
                codecs.String.decode(wire)
        with self.assertRaises(UnicodeError):
            codecs.String.encode('\ud800')

    def test_finite_twist_and_signed_zero(self):
        value = (0.0, -0.0, 2.5, -3.0, 1e-20, 1e20)
        self.assertEqual(codecs.Twist.encode(value), struct.pack('<6d', *value))
        self.assertEqual(codecs.Twist.decode(codecs.Twist.encode(value)), value)
        for number in (float('nan'), float('inf'), -float('inf')):
            with self.subTest(number=number), self.assertRaises(ValueError):
                codecs.Twist.decode(struct.pack('<6d', number, 0, 0, 0, 0, 0))

    def test_bool_exact(self):
        for value in (False, True):
            self.assertIs(codecs.Bool.decode(codecs.Bool.encode(value)), value)
        for value in (0, 1, None, 'true'):
            with self.subTest(value=value), self.assertRaises(ValueError):
                codecs.Bool.encode(value)
        for wire in (b'', b'\2', b'\1\0'):
            with self.subTest(wire=wire), self.assertRaises(ValueError):
                codecs.Bool.decode(wire)

    def test_integer_array_boundaries(self):
        codec = codecs.array_codec(codecs.Int64, 3)
        values = (-(1 << 63), 0, (1 << 63) - 1)
        self.assertEqual(codec.decode(codec.encode(values)), values)
        for wire in (b'\4\0\0\0', codec.encode(values) + b'\0', b'\1\0\0\0'):
            with self.subTest(wire=wire), self.assertRaises(ValueError):
                codec.decode(wire)
        with self.assertRaises(OverflowError):
            codecs.Int64.encode(1 << 63)

    def test_owned_u64_all_bits(self):
        for value in (1, 1 << 63, (1 << 64) - 1):
            self.assertEqual(codecs.goal_id(value), struct.pack('<Q', value))
        for value in (0, -1, True, 1 << 64):
            with self.subTest(value=value), self.assertRaises(ValueError):
                codecs.goal_id(value)

    def test_service_response_exact(self):
        value = True, '成功\0'
        self.assertEqual(codecs.ServiceResponse.decode(codecs.ServiceResponse.encode(value)), value)
        with self.assertRaises(ValueError):
            codecs.ServiceResponse.decode(b'\2\0\0\0\0')

    def test_generated_record_float32_and_bounds(self):
        wire = codecs.Writer(8).integer(-32768, 2, True).integer(65535, 2).f32(-0.0).finish()
        self.assertEqual(wire, struct.pack('<hHf', -32768, 65535, -0.0))
        reader = codecs.Reader(wire)
        self.assertEqual(reader.integer(2, True), -32768)
        self.assertEqual(reader.integer(2), 65535)
        self.assertEqual(struct.pack('<f', reader.f32()), struct.pack('<f', -0.0)); reader.end()
        with self.assertRaises((OverflowError, struct.error)):
            codecs.Writer(4).f32(1e39)
        with self.assertRaises(ValueError):
            codecs.Reader(struct.pack('<f', float('inf'))).f32()
        with self.assertRaises(ValueError):
            codecs.Writer(3).f32(1.0)


class Ownership(unittest.TestCase):
    def setUp(self):
        self.backend = LocalBackend()
        self.session = Session(self.backend, 16)
        self.node = Node(self.session)

    def test_default_permission_failure_is_not_a_handle(self):
        self.backend.replies['capability_open'] = [Result(13)]
        with self.assertRaises(RosError) as error:
            self.session.open('denied')
        self.assertEqual(error.exception.error, 13)
        self.assertEqual(self.session.owners, [])
        self.assertEqual([c[0] for c in self.backend.calls], ['capability_open'])

    def test_full_unsigned_handle(self):
        self.backend.replies['capability_open'] = [Result(0, value=(1 << 64)-1)]
        owner = self.session.open('allbits')
        owner.close().checked()
        self.assertEqual(self.backend.calls[-1][1], (1 << 64)-1)

    def test_close_once_after_timeout(self):
        owner = self.session.open('one')
        self.backend.replies['capability_close'] = [Result(110)]
        self.assertEqual(owner.close().error, 110)
        self.assertEqual(owner.close().error, 9)
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_close']), 1)

    def test_all_cleanup_in_reverse_order(self):
        owners = [self.session.open(name) for name in ('a', 'b', 'c')]
        self.backend.replies['capability_close'] = [Result(107), MemoryError('close'), Result(125)]
        report = self.session.close('application error')
        self.assertEqual(report.primary, 'application error')
        self.assertEqual(len(report.failures), 3)
        self.assertEqual([c[1] for c in self.backend.calls if c[0] == 'capability_close'], [3, 2, 1])
        self.assertTrue(all(owner.handle == 0 for owner in owners))

    def test_poll_lease_blocks_group_close_and_revoke(self):
        sibling = self.session.open('topic')
        observed = []
        def callback(value):
            observed.extend((sibling.revoke().error, self.session.close().outcomes[0].error))
            self.assertEqual(value, 'hello')
        subscription = self.node.create_subscription(codecs.String, 'topic', callback)
        self.backend.replies['event_poll'] = [Result(0, codecs.String.encode('hello'))]
        subscription.poll_once().checked()
        self.assertEqual(observed, [17, 17])
        self.assertNotEqual(subscription.owner.handle, 0)
        self.assertTrue(self.session.active)

    def test_callback_exception_releases_lease(self):
        def callback(value):
            raise LookupError(value)
        sub = self.node.create_subscription(codecs.String, 'sub', callback)
        self.backend.replies['event_poll'] = [Result(0, codecs.String.encode('value'))]
        with self.assertRaises(LookupError):
            sub.poll_once()
        sub.close().checked()

    def test_factory_failure_closes_all_acquisitions(self):
        self.backend.replies['capability_open'] = [Result(0, value=19), Result(13)]
        self.backend.replies['capability_close'] = [Result(107)]
        with self.assertRaises(RosError) as error:
            self.node.create_service(codecs.Trigger, codecs.ServiceResponse, 'ad', 'reply', lambda _: (True, ''))
        self.assertEqual(error.exception.error, 13)
        self.assertEqual(error.exception.roswasm_cleanup[0].error, 107)
        self.assertEqual([c[1] for c in self.backend.calls if c[0] == 'capability_close'], [19])

    def test_wrapper_allocation_failure_precedes_native_open(self):
        with patch('roswasm.client.Publisher', side_effect=MemoryError('wrapper')):
            with self.assertRaises(MemoryError):
                self.node.create_publisher(codecs.String, 'pub')
        self.assertEqual(self.backend.calls, [])
        self.assertTrue(self.session.last_factory_cleanup.completed)

    def test_capacity_failure_is_never_replayed(self):
        subscription = self.node.create_subscription(codecs.String, 'topic', lambda _: self.fail('callback on error'))
        self.backend.replies['event_poll'] = [Result(90, required=None)]
        self.assertIsNone(subscription.poll_once().required)
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'event_poll']), 1)

    def test_round_robin_and_reentrant_executor(self):
        executor = Executor(2); delivered = []
        first = self.node.create_subscription(codecs.String, 'first', lambda value: delivered.append(value))
        def second_callback(value):
            delivered.append(value)
            with self.assertRaises(RosError) as error:
                executor.spin_once()
            self.assertEqual(error.exception.error, 17)
        second = self.node.create_subscription(codecs.String, 'second', second_callback)
        executor.add(first); executor.add(second)
        self.backend.replies['event_poll'] = [Result(0, codecs.String.encode('first')), Result(0, codecs.String.encode('second'))]
        executor.spin_once().checked(); executor.spin_once().checked()
        self.assertEqual(delivered, ['first', 'second'])

    def test_observer_interleaved_source_sequence(self):
        first, second = self.session.open('first'), self.session.open('second')
        observer = self.node.create_observer('observer')
        observer.watch(first).checked(); observer.watch(second).checked()
        self.backend.replies['event_poll_v1'] = [Result(0, event(1, first.handle)), Result(0, event(2, second.handle))]
        self.assertEqual(observer.poll_once()[1].sequence, 1)
        self.assertEqual(observer.poll_once()[1].sequence, 2)

    def test_observer_failed_binding_does_not_commit(self):
        observer = self.node.create_observer('observer')
        first = self.session.open('source'); observer.watch(first).checked()
        self.backend.replies['event_poll_v1'] = [Result(0, event(2, first.handle)), Result(0, event(1, first.handle))]
        with self.assertRaises(ValueError):
            observer.poll_once()
        self.assertEqual(observer.sequence, 0)
        self.assertEqual(observer.poll_once()[1].sequence, 1)

    def test_event_malformed_cases(self):
        for wire in (event()[:-1], event()+b'x', event(source=0), event(sequence=0),
                     event(flags=1), event(kind=4, correlation=9, body=struct.pack('<Q', 8)),
                     event(kind=7), event(kind=6, error=0)):
            with self.subTest(wire=wire), self.assertRaises(ValueError):
                events.decode(wire)

    def test_action_cancel_buffer_and_terminal_ownership(self):
        from roswasm.client import ActionClient
        send, feedback, result, cancel = [self.session.open(name) for name in ('send', 'feedback', 'result', 'cancel')]
        client = ActionClient(send, feedback, result, cancel)
        self.backend.replies['action_send_goal'] = [Result(0, struct.pack('<Q', 1 << 63))]
        identity = client.send_goal(codecs.fibonacci_goal(5))
        client.cancel_goal(identity).checked()
        self.assertEqual(self.backend.calls[-1][3], 1)
        self.backend.replies['action_receive_result'] = [Result(0, struct.pack('<QB', identity, 2))]
        client.receive(identity, 32, terminal=True).checked()
        self.assertEqual(client.cancel_goal(identity).error, 9)


    def test_factory_primary_refuses_attributes_all_closes_still_run(self):
        class Primary(MemoryError):
            def __setattr__(self, name, value):
                if name.startswith('roswasm_'):
                    raise MemoryError('diagnostic allocation')
                super().__setattr__(name, value)
        primary = Primary('activation')
        self.backend.replies['service_advertise'] = [primary]
        first_cleanup = MemoryError('first cleanup')
        self.backend.replies['capability_close'] = [first_cleanup, Result(107)]
        with self.assertRaises(Primary) as caught:
            self.node.create_service(codecs.Trigger, codecs.ServiceResponse, 'ad', 'response', lambda _: (True, ''))
        self.assertIs(caught.exception, primary)
        frame = self.session.last_factory_cleanup
        self.assertIs(frame.primary, primary)
        self.assertTrue(frame.completed)
        self.assertEqual(frame.attempted, 2)
        self.assertIs(frame.outcomes[0], first_cleanup)
        self.assertEqual(frame.outcomes[1].error, 107)
        self.assertEqual([c[1] for c in self.backend.calls if c[0] == 'capability_close'], [2, 1])
        self.assertTrue(all(owner.handle == 0 for owner in self.session.owners))

    def test_pending_factory_owner_registered_before_backend_returns(self):
        original = self.backend.invoke
        observed = []
        def invoke(name, *args, **kwargs):
            if name == 'capability_open':
                frame = self.session.last_factory_cleanup
                observed.append(frame.owners[0] is self.session.owners[0] and frame.owners[0].handle == 0)
            return original(name, *args, **kwargs)
        self.backend.invoke = invoke
        self.node.create_publisher(codecs.String, 'pub')
        self.assertEqual(observed, [True])

    def test_owner_list_allocation_failure_has_no_native_effect(self):
        class RefuseAppend(list):
            def append(self, value):
                raise MemoryError('owner storage')
        self.session.owners = RefuseAppend()
        with self.assertRaises(MemoryError):
            self.node.create_publisher(codecs.String, 'pub')
        self.assertEqual(self.backend.calls, [])
        self.assertEqual(self.session.last_factory_cleanup.attempted, 0)

    def test_postopen_wrapper_assignment_failure_closes_once(self):
        primary = MemoryError('owner assignment')
        class Wrapper:
            def __init__(self, owner, codec):
                self.owner, self.codec = owner, codec
            def __setattr__(self, key, value):
                if key == 'owner' and value is not None:
                    raise primary
                object.__setattr__(self, key, value)
        self.backend.replies['capability_close'] = [Result(107)]
        with patch('roswasm.client.Publisher', Wrapper), self.assertRaises(MemoryError) as caught:
            self.node.create_publisher(codecs.String, 'pub')
        self.assertIs(caught.exception, primary)
        self.assertEqual(self.session.last_factory_cleanup.outcomes[0].error, 107)
        self.assertEqual(len([c for c in self.backend.calls if c[0] == 'capability_close']), 1)
        self.assertEqual(self.session.owners[0].handle, 0)

    def test_executor_advances_fairness_after_source_failure(self):
        executor = Executor(2); values = []
        first = self.node.create_subscription(codecs.String, 'first', values.append)
        second = self.node.create_subscription(codecs.String, 'second', values.append)
        executor.add(first); executor.add(second)
        self.backend.replies['event_poll'] = [Result(110), Result(0, codecs.String.encode('second'))]
        self.assertEqual(executor.spin_once().error, 110)
        executor.spin_once().checked()
        self.assertEqual(values, ['second'])
        self.assertFalse(first.owner.busy or second.owner.busy)


if __name__ == '__main__':
    unittest.main(verbosity=2)
