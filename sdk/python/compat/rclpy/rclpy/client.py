# Copyright 2016 Open Source Robotics Foundation, Inc.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy at http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND. See the License for the specific language governing
# permissions and limitations under the License.
# Adapted public spellings from ros2/rclpy rclpy/rclpy/client.py at immutable
# 4e607c09d624debc39a4ae5405680c06d87fac35. Original whole bodies/license are
# retained under upstream/service_future/rclpy. Changed source: bounded Guest
# scheduling/storage, actual SDK capabilities and no native/JNI pointers.
"""Genuine observer-driven async Service client over real service_submit.

A successful admission returns a request ID, not a response. Every complete
terminal event must match the same client source/observer/backend/instance/
session/sequence and an admitted request. Detachment/cancel affects only local
Future delivery; native work may remain admitted and is never replayed.
"""
from roswasm import Result, RosError
from .task import Future
from ._dispatch import DispatchLease
from roswasm.client import _FactoryCleanup, _factory_failure

_OK = Result(0)
_IDLE = Result(11)


class ServiceEventError(RuntimeError):
    """Preallocated terminal diagnostic filled with actual numeric event facts."""
    def __init__(self, code=0):
        super().__init__('service event failed; inspect code/flags/request_id/event')
        self.code, self.flags, self.request_id, self.event = code, 0, 0, None
        self.origin, self.cause = 'native-event', None


class _Pending:
    def __init__(self, context, client):
        self.future = Future(_context=context, _client=client)
        self.error = ServiceEventError()
        self.request_id = 0
        self.attached = True


class Client:
    def __init__(self, node, srv_type, name, profile, maximum_pending=32):
        self.node, self.context = node, node.context
        self.srv_type, self.srv_name, self.qos_profile = srv_type, name, profile
        self._resource = self._observer = None
        self._dispatch_owners = [None, None]
        self._retired = False
        self._pending_requests = [None] * maximum_pending
        self._indices = tuple(range(maximum_pending))
        self._closed_work = iter(self._indices)
        self.last_admission = None
        self.last_cleanup = None
        self.last_event = None
        self._last_request_id = 0
        self._source_handle = 0

    @property
    def service_name(self):
        return self.srv_name

    @property
    def handle(self):
        raise NotImplementedError('a native rcl client pointer is not a Wasm capability')

    @property
    def logger_name(self):
        return self.node.get_fully_qualified_name().strip('/').replace('/', '.')

    def _live(self):
        self.node._live()
        if self._retired:
            raise RosError(Result(125))
        if (self._resource.owner.handle != self._source_handle or
                not self._observer.owner.handle):
            raise RosError(Result(9))

    def call_async(self, request):
        self._live()
        if self.context._dispatching:
            raise RosError(Result(17))
        if type(request) is not self.srv_type.Request:
            raise TypeError('complete selected Request required')
        slot = next((index for index, value in enumerate(self._pending_requests) if value is None), None)
        if slot is None:
            raise RosError(Result(90))
        # Future/callback/diagnostic storage exists before admission. Capture the
        # canonical request before publishing a reservation or invoking submit.
        record = _Pending(self.context, self)
        payload = self.srv_type.Request._encode(request)
        failure_cleanup = _FactoryCleanup(self.context.session, 2)
        failure_cleanup.seed(self._dispatch_owners)
        failure_lease = DispatchLease(self.context)
        failure_work = iter(self._indices)
        try:
            self._pending_requests[slot] = record
            result = self._resource.owner.invoke('service_submit', payload, other=self._observer.owner)
            self.last_admission = result
            result.checked()
            if type(result.value) is not int or not 0 < result.value < 1 << 64:
                raise ValueError('successful service_submit did not return a nonzero request ID')
            # The real per-backend Host counter is strictly increasing and
            # refuses exhaustion. Compare without allocating a post-submit
            # pending-set/iterator; no old ID may be resurrected after detach.
            if result.value <= self._last_request_id:
                raise ValueError('duplicate/backward actual request ID')
            record.request_id = self._last_request_id = result.value
            record.future.request_id = result.value
            return record.future
        except BaseException as primary:
            # The once-captured outcome may represent uncertain admission, even
            # when result delivery failed. Retire this client pair once using
            # work/results/closers reserved before submit; never replay it.
            self._retired = True
            self.last_cleanup = failure_cleanup
            try:
                with failure_lease:
                    for index in failure_work:
                        pending = self._pending_requests[index]
                        if pending is not None:
                            if pending is record:
                                self._complete(index, pending, None, primary)
                            else:
                                # An admission failure of THIS new request is
                                # not a native110 completion of an older one.
                                self._local_closed(index, pending, primary)
            except BaseException as diagnostic:
                failure_cleanup.diagnostic_error = diagnostic
            finally:
                _factory_failure(failure_cleanup, primary)
            raise

    def get_pending_request(self, sequence_number):
        for record in self._pending_requests:
            if record is not None and record.attached and record.request_id == sequence_number:
                return record.future
        raise KeyError(sequence_number)

    def remove_pending_request(self, future):
        for record in self._pending_requests:
            if record is not None and record.future is future:
                # Retain an internal tombstone until the actual completion is
                # drained; caller detachment cannot release native inflight.
                record.attached = False
                return

    def _complete(self, index, record, event, error=None):
        self._pending_requests[index] = None
        future = record.future
        future.last_event = event
        if not record.attached or not future._pending():
            # Local detachment suppresses delivery, not complete wire checks.
            # A malformed consumed response must still fail the source closed.
            if error is None and event is not None and event.kind == 3:
                self.srv_type.Response._decode(event.body)
            return
        if error is not None:
            future.set_exception(error)
        elif event.kind == 3:
            try:
                response = self.srv_type.Response._decode(event.body)
            except BaseException as invalid:
                future.set_exception(invalid)
            else:
                future.set_result(response)
        else:
            record.error.code, record.error.flags = event.error or 125, event.flags
            record.error.request_id, record.error.event = record.request_id, event
            record.error.origin = 'native-resource-closed' if event.kind == 7 else 'native-event'
            future.set_exception(record.error)

    def _local_closed(self, index, record, cause=None):
        record.error.code, record.error.flags = 125, 0
        record.error.request_id, record.error.event = record.request_id, None
        record.error.origin, record.error.cause = 'local-resource-retirement', cause
        self._complete(index, record, None, record.error)

    def _finish_closed(self):
        for index in self._closed_work:
            record = self._pending_requests[index]
            if record is not None:
                self._local_closed(index, record)

    def poll_once(self):
        if self.context._dispatching:
            return self._poll_inside()
        with DispatchLease(self.context, self._dispatch_owners):
            return self._poll_inside()

    def _poll_inside(self):
        first, second = self._dispatch_owners
        if first.busy or second.busy:
            return Result(17)
        try:
            first.busy = second.busy = True
            return self._consume_event()
        finally:
            first.busy = second.busy = False

    def _consume_event(self):
        if self._retired:
            return Result(125)
        self.node._live()
        terminal_work = iter(self._indices)
        unmatched_work = iter(self._indices)
        try:
            result, event = self._observer.poll_once()
        except BaseException as invalid:
            # A consumed malformed/unbound envelope cannot be replayed or
            # accepted as a completion. Fail every live local Future closed.
            self._retired = True
            for index in terminal_work:
                record = self._pending_requests[index]
                if record is not None:
                    self._complete(index, record, None, invalid)
            raise
        if result.error:
            return result
        self.last_event = event
        if event.source != self._source_handle or event.kind not in (3, 6, 7):
            invalid = ValueError('unexpected service source/event kind')
            self._retired = True
            for index in terminal_work:
                record = self._pending_requests[index]
                if record is not None:
                    self._complete(index, record, event, invalid)
            raise invalid
        if event.kind == 7 or (event.kind == 6 and (event.correlation == 0 or event.flags & 1)):
            self._retired = True
            for index in terminal_work:
                record = self._pending_requests[index]
                if record is not None:
                    self._complete(index, record, event)
            return _OK
        for index in terminal_work:
            record = self._pending_requests[index]
            if record is not None and record.request_id == event.correlation:
                try:
                    self._complete(index, record, event)
                except BaseException as invalid:
                    self._retired = True
                    for remaining in unmatched_work:
                        pending = self._pending_requests[remaining]
                        if pending is not None:
                            self._complete(remaining, pending, event, invalid)
                    raise
                return _OK
        invalid = ValueError('service completion does not match an admitted request')
        self._retired = True
        # Matching scan consumed the iterator. This unknown-correlation path
        # needs a SECOND release work iterator reserved before the native poll.
        for index in unmatched_work:
            record = self._pending_requests[index]
            if record is not None:
                self._complete(index, record, event, invalid)
        raise invalid

    def call(self, request, timeout_sec=None):
        raise NotImplementedError('synchronous native wait/thread semantics are not implemented; use real call_async and explicit bounded spin')

    def service_is_ready(self):
        raise NotImplementedError('ABI v1 has no native service availability predicate; graph listings are not a substitute')

    def wait_for_service(self, timeout_sec=None):
        raise NotImplementedError('native service availability/wait is required before unchanged official client mains can run')

    def destroy(self):
        from ._service_support import close_resource
        close_resource(self)

    def configure_introspection(self, *args, **kwargs):
        raise NotImplementedError('native service introspection configuration is not implemented')
