# Copyright 2018 Open Source Robotics Foundation, Inc.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy at http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND. See the License for the specific language governing
# permissions and limitations under the License.
# Adapted public spellings from ros2/rclpy rclpy/rclpy/task.py at immutable
# 4e607c09d624debc39a4ae5405680c06d87fac35. Original whole bodies/license are
# retained under upstream/service_future/rclpy. Changed source: bounded Guest
# scheduling/storage, actual SDK capabilities and no native/JNI pointers.
"""Pinned Jazzy Future spellings over a finite single-threaded callback profile.

The upstream source is preserved under upstream/service_future/rclpy. A Future
is a local result, not a native service or cancellation handle. Client Futures
complete only after the real observer validates the admitted request's terminal
event. No call_async path invokes synchronous service_call. Native cancellation,
thread-safe executors and Task/coroutine scheduling remain required work.
"""
from enum import Enum
from roswasm import Result, RosError


class FutureState(Enum):
    PENDING = 'PENDING'
    CANCELLED = 'CANCELLED'
    FINISHED = 'FINISHED'


class Future:
    def __init__(self, *, executor=None, _context=None, _client=None):
        if executor is not None:
            raise NotImplementedError('native rclpy Executor task scheduling is not implemented')
        self._state = FutureState.PENDING
        self._result = self._exception = None
        self._exception_fetched = False
        self._context, self._client = _context, _client
        # A bounded lifetime callback count, including immediate late additions.
        self._callbacks = [None] * 32
        self.callback_errors = [None] * 32
        self._callback_count = 0
        self._work = iter(tuple(range(32)))
        self._notified = False
        self.last_event = None
        self.request_id = 0

    def _pending(self):
        return self._state == FutureState.PENDING

    def __await__(self):
        if self._pending():
            yield self
        if self._pending():
            raise RuntimeError('Future awaited again before completion')
        return self.result()

    def cancelled(self):
        return self._state == FutureState.CANCELLED

    def done(self):
        # Jazzy distinguishes CANCELLED from FINISHED.
        return self._state == FutureState.FINISHED

    def result(self):
        if self._exception is not None:
            raise self.exception()
        return self._result

    def exception(self):
        self._exception_fetched = True
        return self._exception

    def _invoke_one(self, callback):
        context = self._context
        if context is None or context._dispatching or not context.ok():
            callback(self)
        else:
            from ._dispatch import DispatchLease
            with DispatchLease(context):
                callback(self)

    def _notify(self):
        if self._notified:
            return
        self._notified = True
        # Fixed existing slots: one failing application callback cannot skip the
        # remaining callbacks or replace the recorded terminal service outcome.
        for index in self._work:
            callback = self._callbacks[index]
            self._callbacks[index] = None
            if callback is not None:
                try:
                    self._invoke_one(callback)
                except BaseException as error:
                    self.callback_errors[index] = error

    def cancel(self):
        if not self._pending():
            return
        if self._client is not None:
            self._client.remove_pending_request(self)
        self._state = FutureState.CANCELLED
        self._notify()

    def set_result(self, value):
        self._result = value
        self._state = FutureState.FINISHED
        self._notify()

    def set_exception(self, exception):
        if not isinstance(exception, BaseException):
            raise TypeError('exception required')
        self._exception = exception
        self._exception_fetched = False
        self._state = FutureState.FINISHED
        self._notify()

    def add_done_callback(self, callback):
        if not callable(callback):
            raise TypeError('done callback required')
        if self._callback_count == len(self._callbacks):
            raise RosError(Result(90))
        index = self._callback_count
        self._callback_count += 1
        if self._pending():
            self._callbacks[index] = callback
        else:
            # Like the pinned no-executor Future, a late callback is immediate
            # and its own exception propagates to this caller.
            self._invoke_one(callback)

    def remove_done_callback(self, callback):
        if not self._pending():
            return False
        for index in range(self._callback_count):
            if self._callbacks[index] is callback:
                self._callbacks[index] = None
                return True
        return False


class Task:
    def __init__(self, *args, **kwargs):
        raise NotImplementedError('native rclpy Task/coroutine scheduling is not implemented')
