"""Unexecuted source contracts: real SDK/facade, deterministic local Backend.

These tests create no native ROS, Runtime, Gateway, producer or Wasm result.
Run only from an explicit Root-owned copied SDK layout after source review.
"""

# Test sources are separate from the selected production SDK.
import sys as _source_sys
from pathlib import Path as _SourcePath
_CHECKOUT_ROOT = next(p for p in _SourcePath(__file__).resolve().parents if (p / 'sdk/origin').is_dir())
_source_sys.path.insert(0, str(_CHECKOUT_ROOT / 'test/0073.ros-tests/support'))
from source_paths import sdk_path as _sdk_path

import importlib.util
from pathlib import Path
import struct
import unittest
from unittest.mock import patch

import rclpy
from rclpy.context import Context, AliasBinding
from rclpy.node import Node
from rclpy.task import Future
from rclpy.client import ServiceEventError
from example_interfaces.srv import AddTwoInts
from std_srvs.srv import Empty, Trigger, SetBool
from std_msgs.msg import String
from roswasm import Result, RosError
from test_contracts import LocalBackend, event


class Model(LocalBackend):
    def __init__(self):
        super().__init__()
        self.request_id = 101

    def invoke(self, name, handle=0, payload=b'', capacity=0, other=0, correlation=0):
        if self.replies.get(name):
            return super().invoke(name, handle, payload, capacity, other, correlation)
        if name in ('event_poll', 'event_poll_v1'):
            self.calls.append((name, handle, payload, capacity, other, correlation))
            return Result(11)
        if name == 'service_submit':
            self.calls.append((name, handle, payload, capacity, other, correlation))
            value = self.request_id; self.request_id += 1
            return Result(0, value=value)
        return super().invoke(name, handle, payload, capacity, other, correlation)


def bindings():
    return {
        ('service_advertise', '/add', AddTwoInts._ROSWASM_TYPE): AliasBinding('advertise', 10, 24),
        ('service_respond', '/add', AddTwoInts._ROSWASM_TYPE): AliasBinding('respond', 10, 8),
        ('service_call', '/add', AddTwoInts._ROSWASM_TYPE): AliasBinding('call', 10, 8),
        ('event_observe', '/add', AddTwoInts._ROSWASM_TYPE): AliasBinding('observe', 10, 72),
        ('log_write', '/service', ''): AliasBinding('logger'),
        ('topic_publish', '/topic', String._ROSWASM_TYPE): AliasBinding('publisher', 10),
        ('topic_subscribe', '/topic', String._ROSWASM_TYPE): AliasBinding('subscriber', 10),
    }


class ServiceFutureContracts(unittest.TestCase):
    def setUp(self):
        self.model = Model(); self.context = Context()
        self.context.configure(bindings(), idle_wait=lambda: False)
        with patch('rclpy.context.Backend', return_value=self.model):
            self.context.init()
        self.node = Node('service', context=self.context)

    def tearDown(self):
        if self.context.ok():
            try:
                self.context.shutdown()
            except RuntimeError:
                pass  # Intentional close-failure cases inspect the retained reports.

    def calls(self, name):
        return [call for call in self.model.calls if call[0] == name]

    def server(self, callback=None):
        return self.node.create_service(AddTwoInts, '/add', callback or
            (lambda request, response: self.add(request, response)))

    @staticmethod
    def add(request, response):
        response.sum = request.a + request.b
        return response

    def client(self):
        return self.node.create_client(AddTwoInts, '/add')

    def admit(self, client):
        return client.call_async(AddTwoInts.Request(a=41, b=1))

    def queue(self, client, *, sequence=1, request=101, body=None, **kwargs):
        if body is None:
            body = AddTwoInts.Response._encode(AddTwoInts.Response(sum=42))
        kind = kwargs.pop('kind', 3)
        self.model.replies['event_poll_v1'] = [Result(0, event(sequence,
            client._resource.owner.handle, kind=kind, correlation=request, body=body, **kwargs))]

    def test_full_signed64_fields_and_std_services(self):
        value = AddTwoInts.Request(a=-(1 << 63), b=(1 << 63) - 1)
        wire = AddTwoInts.Request._encode(value)
        self.assertEqual(wire, struct.pack('<qq', value.a, value.b))
        copied = AddTwoInts.Request._decode(wire)
        self.assertEqual((copied.a, copied.b), (value.a, value.b))
        self.assertEqual(Empty.Request._encode(Empty.Request()), b'')
        self.assertEqual(Empty.Response._encode(Empty.Response()), b'')
        self.assertEqual(Trigger.Request._encode(Trigger.Request()), b'')
        self.assertEqual(SetBool.Request._encode(SetBool.Request(data=True)), b'\1')
        for typ in (Trigger.Response, SetBool.Response):
            response = typ(success=True, message='full\0BOM\ufeff机器人🙂')
            got = typ._decode(typ._encode(response))
            self.assertEqual((got.success, got.message), (response.success, response.message))
        for bad in (True, -(1 << 63) - 1, 1 << 63):
            with self.assertRaises((TypeError, ValueError)):
                AddTwoInts.Request(a=bad)
        with self.assertRaises(ValueError):
            AddTwoInts.Response._decode(b'\0' * 9)

    def test_missing_binding_and_empty_registry_refuse_before_open(self):
        self.context.bindings.clear()
        before = len(self.model.calls)
        with self.assertRaises(RosError) as error:
            self.server()
        self.assertEqual(error.exception.error, 13)
        self.assertEqual(len(self.model.calls), before)
        with self.assertRaises(NotImplementedError):
            self.node.create_service(Empty, '/empty', lambda request, response: response)
        self.assertEqual(len(self.model.calls), before)

    def test_role_alias_qos_and_capacity_preflight(self):
        self.context.bindings[('service_respond', '/add', AddTwoInts._ROSWASM_TYPE)] = AliasBinding('advertise', 10)
        before = len(self.model.calls)
        with self.assertRaises(ValueError):
            self.server()
        self.assertEqual(len(self.model.calls), before)
        self.context.bindings.update(bindings())
        with self.assertRaises(RosError) as error:
            self.node.create_service(AddTwoInts, '/add', self.add, qos_profile=3)
        self.assertEqual(error.exception.error, 17)
        self.context.session.maximum = 1
        with self.assertRaises(RosError) as error:
            self.server()
        self.assertEqual(error.exception.error, 90)
        self.assertFalse(self.calls('capability_open'))

    def test_server_exact_request_id_response_and_callback_shape(self):
        requests = []
        def callback(request, response):
            requests.append((request.a, request.b, type(response)))
            response.sum = request.a + request.b
            return response
        self.server(callback)
        self.model.replies['event_poll'] = [Result(0, struct.pack('<Qqq', (1 << 63) + 7, 41, 1))]
        rclpy.spin_once(self.node, timeout_sec=0)
        self.assertEqual(requests, [(41, 1, AddTwoInts.Response)])
        self.assertEqual(self.calls('service_respond')[-1][2], struct.pack('<Qq', (1 << 63) + 7, 42))

    def test_zero_id_bad_request_and_bad_response_never_send(self):
        server = self.server(lambda request, response: None)
        self.model.replies['event_poll'] = [Result(0, b'\0' * 24)]
        with self.assertRaises(ValueError):
            rclpy.spin_once(self.node, timeout_sec=0)
        self.model.replies['event_poll'] = [Result(0, struct.pack('<Qqq', 9, 1, 2))]
        with self.assertRaises(TypeError):
            rclpy.spin_once(self.node, timeout_sec=0)
        self.assertFalse(self.calls('service_respond'))
        self.assertFalse(server._resource.advertise.busy)
        self.assertFalse(self.context._dispatching)

    def test_cross_node_context_and_raw_session_close_are_leased(self):
        other = Node('other', context=self.context)
        publisher = other.create_publisher(String, '/topic', 10)
        def callback(request, response):
            with self.assertRaises(RosError) as error:
                self.context.shutdown()
            self.assertEqual(error.exception.error, 17)
            with self.assertRaises(RosError):
                other.create_publisher(String, '/topic', 10)
            with self.assertRaises(RosError):
                other.destroy_node()
            self.assertEqual(self.context.session.close().outcomes[0].error, 17)
            publisher.publish(String(data='existing owner remains usable'))
            response.sum = 42
            return response
        self.server(callback)
        self.model.replies['event_poll'] = [Result(0, struct.pack('<Qqq', 1, 41, 1))]
        rclpy.spin_once(self.node, timeout_sec=0)
        self.assertTrue(self.context.ok()); self.assertFalse(self.calls('capability_close'))

    def test_callback_primary_releases_entire_lease_without_response(self):
        primary = BaseException('application')
        def fail(request, response):
            raise primary
        self.server(fail)
        self.model.replies['event_poll'] = [Result(0, struct.pack('<Qqq', 1, 1, 2))]
        with self.assertRaises(BaseException) as error:
            rclpy.spin_once(self.node, timeout_sec=0)
        self.assertIs(error.exception, primary)
        self.assertFalse(self.context._dispatching)
        self.assertTrue(all(not owner.busy for owner in self.context.session.owners))
        self.assertFalse(self.calls('service_respond'))

    def test_default_logger_remains_lazy_callback_refusal(self):
        self.server(lambda request, response: self.logged_response(response))
        before = len(self.calls('capability_open'))
        self.model.replies['event_poll'] = [Result(0, struct.pack('<Qqq', 1, 1, 2))]
        with self.assertRaises(RosError) as error:
            rclpy.spin_once(self.node, timeout_sec=0)
        self.assertEqual(error.exception.error, 17)
        self.assertEqual(len(self.calls('capability_open')), before)

    def logged_response(self, response):
        self.node.get_logger().info('explicit prepared callback log')
        response.sum = 3
        return response

    def test_opt_in_logger_transaction_prepares_all_roles_before_callback(self):
        self.context.prepare_callback_logging = True  # Model the selected pre-init profile.
        self.server(lambda request, response: self.logged_response(response))
        self.assertEqual([row[2] for row in self.calls('capability_open')], [b'logger', b'advertise', b'respond'])
        self.model.replies['event_poll'] = [Result(0, struct.pack('<Qqq', 1, 1, 2))]
        before = len(self.calls('capability_open'))
        rclpy.spin_once(self.node, timeout_sec=0)
        self.assertEqual(len(self.calls('capability_open')), before)
        self.assertEqual(len(self.calls('log_write')), 1)

    def test_three_role_factory_failure_keeps_primary_all_cleanup(self):
        self.context.prepare_callback_logging = True
        primary = RuntimeError('activation')
        self.model.replies['service_advertise'] = [primary]
        self.model.replies['capability_close'] = [Result(107), RuntimeError('close'), Result(0)]
        with self.assertRaises(RuntimeError) as error:
            self.server()
        self.assertIs(error.exception, primary)
        self.assertEqual([row[1] for row in self.calls('capability_close')], [3, 2, 1])
        self.assertEqual(len(self.context.session.last_factory_cleanup.outcomes), 3)
        self.assertEqual(self.node.services, []); self.assertEqual(self.node._owners, [])
        self.assertIsNone(self.node._logger_owner)

    def test_real_async_admission_is_not_synchronous_completion(self):
        client = self.client(); future = self.admit(client)
        self.assertFalse(future.done()); self.assertIsNone(future.result())
        call = self.calls('service_submit')[-1]
        self.assertEqual(call[2], struct.pack('<qq', 41, 1))
        self.assertEqual(call[4], client._observer.owner.handle)
        self.assertEqual(len(self.calls('event_watch')), 1)
        self.assertFalse(self.calls('service_call'))
        self.queue(client); rclpy.spin_once(self.node, timeout_sec=0)
        self.assertTrue(future.done()); self.assertEqual(future.result().sum, 42)
        self.assertEqual(future.last_event.correlation, 101)
        with self.assertRaises(KeyError):
            client.get_pending_request(101)

    def test_native_error_retains_correlated_external_effect_flag(self):
        client = self.client(); future = self.admit(client)
        self.queue(client, kind=6, error=110, flags=2, body=b'')
        rclpy.spin_once(self.node, timeout_sec=0)
        with self.assertRaises(ServiceEventError) as error:
            future.result()
        self.assertEqual((error.exception.code, error.exception.flags, error.exception.request_id), (110, 2, 101))
        self.assertEqual(len(self.calls('service_submit')), 1)

    def test_resource_closed_finishes_all_original_pending_without_regrant(self):
        client = self.client(); a = self.admit(client); b = self.admit(client)
        self.queue(client, kind=7, request=0, flags=1, body=b'')
        rclpy.spin_once(self.node, timeout_sec=0)
        for future in (a, b):
            self.assertTrue(future.done()); self.assertEqual(future.exception().code, 125)
        before = len(self.calls('service_submit'))
        with self.assertRaises(RosError):
            self.admit(client)
        self.assertEqual(len(self.calls('service_submit')), before)
        self.node.destroy_client(client)
        self.assertEqual(len(self.calls('capability_close')), 2)

    def test_destroy_already_retired_pair_finishes_local_future_without_reclose(self):
        client = self.client(); future = self.admit(client)
        seen = []
        def completed(value):
            seen.append(self.context._dispatching)
            with self.assertRaises(RosError) as refused:
                self.context.shutdown()
            self.assertEqual(refused.exception.error, 17)
        future.add_done_callback(completed)
        for owner in client._dispatch_owners:
            owner.close().checked()
        self.assertEqual(len(self.calls('capability_close')), 2)
        self.assertFalse(future.done())
        self.assertTrue(self.node.destroy_client(client))
        self.assertTrue(future.done())
        failure = future.exception()
        self.assertEqual((failure.code, failure.request_id, failure.origin),
                         (125, 101, 'local-resource-retirement'))
        self.assertEqual(seen, [True])
        self.assertFalse(any(future.callback_errors))
        self.assertFalse(self.context._dispatching)
        self.assertTrue(client._retired)
        self.assertNotIn(client, self.node.clients)
        self.assertNotIn(client, self.node._sources)
        self.assertNotIn(client, self.node._executor.sources)
        self.assertEqual(len(self.calls('capability_close')), 2)
        self.context.shutdown()
        self.assertEqual(len(self.calls('capability_close')), 2)
        self.assertEqual(len(self.calls('service_submit')), 1)
        self.assertIsNone(future.last_event)

    def test_detached_and_cancelled_future_do_not_receive_native_response(self):
        client = self.client(); detached = self.admit(client)
        seen = []; detached.add_done_callback(lambda future: seen.append('detached'))
        client.remove_pending_request(detached)
        self.queue(client); rclpy.spin_once(self.node, timeout_sec=0)
        self.assertFalse(detached.done()); self.assertEqual(seen, [])
        cancelled = self.admit(client); cancelled.cancel()
        self.assertTrue(cancelled.cancelled()); self.assertFalse(cancelled.done())
        self.queue(client, sequence=2, request=102)
        rclpy.spin_once(self.node, timeout_sec=0)
        self.assertTrue(cancelled.cancelled()); self.assertIsNone(cancelled.result())
        malformed = self.admit(client)
        client.remove_pending_request(malformed)
        self.queue(client, sequence=3, request=103, body=b'bad')
        with self.assertRaises(ValueError):
            rclpy.spin_once(self.node, timeout_sec=0)
        self.assertFalse(malformed.done()); self.assertTrue(client._retired)

    def test_callbacks_are_all_attempted_and_keep_context_lease(self):
        client = self.client(); future = self.admit(client); called = []
        def first(f):
            called.append(1)
            with self.assertRaises(RosError):
                self.context.shutdown()
            self.assertEqual(self.context.session.close().outcomes[0].error, 17)
            raise BaseException('first done callback')
        future.add_done_callback(first)
        future.add_done_callback(lambda f: called.append(2))
        self.queue(client); rclpy.spin_once(self.node, timeout_sec=0)
        self.assertEqual(called, [1, 2]); self.assertIsInstance(future.callback_errors[0], BaseException)
        self.assertEqual(future.result().sum, 42)

    def test_actual_envelope_identity_sequence_and_unknown_correlation_reject(self):
        client = self.client(); future = self.admit(client)
        self.queue(client, sequence=2)
        with self.assertRaises(ValueError):
            rclpy.spin_once(self.node, timeout_sec=0)
        self.assertTrue(future.done()); self.assertIsInstance(future.exception(), ValueError)
        self.node.destroy_client(client)
        client = self.client(); future = self.admit(client)
        self.queue(client, request=999)
        with self.assertRaises(ValueError):
            rclpy.spin_once(self.node, timeout_sec=0)
        self.assertTrue(future.done()); self.assertIsInstance(future.exception(), ValueError)

    def test_response_codec_error_is_future_error_without_replay(self):
        client = self.client(); future = self.admit(client)
        self.queue(client, body=b'bad')
        rclpy.spin_once(self.node, timeout_sec=0)
        self.assertTrue(future.done()); self.assertIsInstance(future.exception(), ValueError)
        self.assertEqual(len(self.calls('service_submit')), 1)

    def test_pending_capacity_precedes_native_submit(self):
        client = self.client()
        for _ in range(32):
            self.admit(client)
        before = len(self.calls('service_submit'))
        with self.assertRaises(RosError) as error:
            self.admit(client)
        self.assertEqual(error.exception.error, 90)
        self.assertEqual(len(self.calls('service_submit')), before)

    def test_failed_or_malformed_admission_closes_pair_once(self):
        client = self.client(); primary = Result(110)
        self.model.replies['service_submit'] = [primary]
        self.model.replies['capability_close'] = [Result(107), Result(0)]
        with self.assertRaises(RosError) as error:
            self.admit(client)
        self.assertIs(error.exception.result, primary)
        self.assertEqual(len(self.calls('service_submit')), 1)
        self.assertEqual(len(self.calls('capability_close')), 2)
        self.assertEqual(client.last_cleanup.outcomes[0].error, 107)
        self.node.destroy_client(client)
        self.assertEqual(len(self.calls('capability_close')), 2)

    def test_shutdown_retires_future_without_native_cancel_claim(self):
        client = self.client(); future = self.admit(client)
        self.context.shutdown()
        self.assertTrue(future.done()); self.assertEqual(future.exception().code, 125)
        self.assertFalse(self.context.ok()); self.assertEqual(len(self.calls('capability_close')), 2)

    def test_post_open_registration_failure_rolls_back_all_roles(self):
        primary = MemoryError('registration setitem')
        class RefuseCommit(list):
            def __setitem__(self, index, value):
                if value is not None:
                    raise primary
                return super().__setitem__(index, value)
        self.node.services = RefuseCommit()
        with self.assertRaises(MemoryError) as failure:
            self.server()
        self.assertIs(failure.exception, primary)
        self.assertEqual([row[1] for row in self.calls('capability_close')], [2, 1])
        self.assertEqual(self.node._owners, []); self.assertEqual(self.node.services, [])
        self.assertEqual(self.node._executor.sources, [])

    def test_new_admission_error_does_not_become_older_native_completion(self):
        client = self.client(); older = self.admit(client)
        refusal = Result(110)
        self.model.replies['service_submit'] = [refusal]
        with self.assertRaises(RosError) as primary:
            self.admit(client)
        self.assertTrue(older.done())
        retired = older.exception()
        self.assertEqual(retired.code, 125)
        self.assertEqual(retired.request_id, 101)
        self.assertEqual(retired.origin, 'local-resource-retirement')
        self.assertIs(retired.cause, primary.exception)
        self.assertIsNone(older.last_event)
        self.assertEqual(len(self.calls('capability_close')), 2)

    def test_wait_availability_and_native_options_are_explicitly_unimplemented(self):
        client = self.client(); before = len(self.model.calls)
        for operation in (client.wait_for_service, client.service_is_ready):
            with self.assertRaises(NotImplementedError):
                operation()
        self.assertEqual(len(self.model.calls), before)
        with self.assertRaises(NotImplementedError):
            self.node.create_service(AddTwoInts, '/add', self.add, callback_group=object())
        self.assertFalse(self.calls('graph_list_services'))

    def test_credit_driver_stops_even_if_it_always_returns_true(self):
        self.context.idle_wait = lambda: True
        self.context._remaining_credits = 2
        self.assertTrue(self.context.next_credit()); self.assertTrue(self.context.next_credit())
        self.assertFalse(self.context.next_credit())

    def test_original_official_main_class_handler_run_unchanged(self):
        original = _CHECKOUT_ROOT / 'sdk/origin/python/compat/rclpy/upstream/service_future/examples/files/rclpy/services/minimal_service/examples_rclpy_minimal_service/service_member_function.py'
        spec = importlib.util.spec_from_file_location('pinned_original_service_member', original)
        module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
        configured = bindings()
        configured[('service_advertise', '/add_two_ints', AddTwoInts._ROSWASM_TYPE)] = AliasBinding('advertise', 10, 24)
        configured[('service_respond', '/add_two_ints', AddTwoInts._ROSWASM_TYPE)] = AliasBinding('respond', 10, 8)
        configured[('log_write', '/minimal_service', '')] = AliasBinding('logger')
        context = Context(); context.configure(configured, idle_wait=lambda: False, prepare_callback_logging=True)
        self.model.replies['event_poll'] = [Result(0, struct.pack('<Qqq', 7, 41, 1))]
        with patch('rclpy._default_context', context), patch('rclpy.context.Backend', return_value=self.model):
            module.main()
        self.assertFalse(context.ok())
        self.assertEqual(self.calls('service_respond')[-1][2], struct.pack('<Qq', 7, 42))
        self.assertEqual(len(self.calls('log_write')), 1)


if __name__ == '__main__':
    unittest.main()
