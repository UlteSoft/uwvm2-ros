# Copyright 2016 Open Source Robotics Foundation, Inc.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy at http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software distributed
# under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND. See the License for the specific language governing
# permissions and limitations under the License.
# Adapted public spellings from ros2/rclpy rclpy/rclpy/service.py at immutable
# 4e607c09d624debc39a4ae5405680c06d87fac35. Original whole bodies/license are
# retained under upstream/service_future/rclpy. Changed source: bounded Guest
# scheduling/storage, actual SDK capabilities and no native/JNI pointers.
"""Complete synchronous callback shape for selected pinned Jazzy Service APIs."""
from roswasm import Result, RosError
from ._dispatch import DispatchLease


class Service:
    def __init__(self, node, srv_type, name, callback, profile):
        # Node reserves this whole wrapper and all list positions before opens.
        self.node, self.srv_type, self.srv_name = node, srv_type, name
        self.callback, self.qos_profile = callback, profile
        self._resource = None
        self._dispatch_owners = [None, None]
        self._retired = False
        self.last_cleanup = None

    @property
    def service_name(self):
        return self.srv_name

    @property
    def handle(self):
        raise NotImplementedError('a native rcl service pointer is not a Wasm capability')

    @property
    def logger_name(self):
        return self.node.get_fully_qualified_name().strip('/').replace('/', '.')

    def _respond(self, request):
        response = self.srv_type.Response()
        returned = self.callback(request, response)
        if type(returned) is not self.srv_type.Response:
            raise TypeError('callback must return the complete selected Response')
        return returned

    def poll_once(self):
        if self._retired:
            return Result(125)
        self.node._live()
        # Real ServiceServer validates the nonzero prefix, owns both roles over
        # decode/callback and responds exactly once with that unchanged ID.
        if self.node.context._dispatching:
            return self._resource.poll_once()
        with DispatchLease(self.node.context, self._dispatch_owners):
            return self._resource.poll_once()

    def _finish_closed(self):
        pass  # No fake error response is sent for an application exception.

    def destroy(self):
        from ._service_support import close_resource
        close_resource(self)

    def configure_introspection(self, *args, **kwargs):
        raise NotImplementedError('native service introspection configuration requires explicit protocol support')

    def send_response(self, response, header):
        raise NotImplementedError('native rmw headers are not request handles; automatic owned-ID callback response is the selected API')
