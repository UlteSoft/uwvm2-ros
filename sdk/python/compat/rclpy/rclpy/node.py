"""rclpy Node source migration for explicitly bound topic/timer/logger flows.

This is an incremental compatibility implementation, not a claim that every
rclpy distribution/API is already supported. Unsupported keyword features fail
before resource creation. Complete native conformance remains an acceptance
requirement; see sdk/compatibility/rcl/coverage.json.
"""
import re
import math
from roswasm.client import _factory_failure
from roswasm import Node as CapabilityNode, Result, RosError, Executor
from roswasm import codecs
from . import qos
from .clock import Clock, ClockType
from .timer import Timer
from ._dispatch import ContextExecutor, DispatchLease


class _MessageCodec:
    def __init__(self, message_type):
        if not all(hasattr(message_type, name) for name in ('_encode', '_decode', '_ROSWASM_TYPE')):
            raise TypeError('registered complete ROS message codec required')
        self.message_type = message_type

    def encode(self, message):
        return self.message_type._encode(message)

    def decode(self, data):
        return self.message_type._decode(data)


class _Publisher:
    def __init__(self, resource, topic, message_type, profile):
        self._resource, self.topic, self.msg_type, self.qos_profile = resource, topic, message_type, profile

    def publish(self, message):
        self._resource.publish(message).checked()

    def destroy(self):
        self._resource.close().checked()


class _Logger:
    def __init__(self, node):
        self.node = node

    def info(self, message, **unsupported):
        self.node._live()
        if unsupported:
            raise NotImplementedError('logger filters/once/throttle require compatibility support')
        payload = codecs.String.encode(message)
        if self.node._logger_owner is None:
            # Existing logger callbacks remain usable; only a new lazy resource
            # needs the same factory/reentry/lifetime admission as other opens.
            self.node._open_logger()
        self.node._logger_owner.invoke('log_write', payload).checked()

    def get_effective_level(self):
        raise NotImplementedError('native logger severity introspection is not exposed by ABI v1')


class _Registration:
    """Reserve all ordinary Node/executor list positions BEFORE host effects.

    None is an internal pending position, never polled: factories run on the
    single guest thread and reject active dispatch first. Each final indexed
    replacement can fail and still leaves the real owner in its factory frame.
    Rollback deletes only the exact positions this object reserved, using the
    real builtin list implementation, not user-overridden cleanup hooks.
    """
    def __init__(self, collections):
        if any(not isinstance(value, list) for value in collections):
            raise TypeError('internal registration lists required')
        collections = tuple(collections)
        # A paired Service/Client reserves two positions in the SAME owner list.
        # Count previous identity-equal lists before any append/effect; ordinary
        # topic registrations keep their original position and rollback shape.
        self.rows = tuple((value, list.__len__(value) +
                           sum(1 for previous in collections[:index] if previous is value))
                          for index, value in enumerate(collections))
        self.reverse = tuple(reversed(self.rows))
        self._rollback_iterator = iter(self.reverse)
        try:
            for value, _ in self.rows:
                value.append(None)
        except BaseException:
            self.rollback()
            raise

    def set(self, index, value):
        collection, position = self.rows[index]
        collection[position] = value

    def rollback(self):
        for collection, position in self._rollback_iterator:
            if list.__len__(collection) > position:
                list.__delitem__(collection, position)


class Node:
    def __init__(self, node_name, *, context=None, namespace=None, cli_args=None,
                 use_global_arguments=True, enable_rosout=True,
                 start_parameter_services=True, parameter_overrides=None,
                 allow_undeclared_parameters=False,
                 automatically_declare_parameters_from_overrides=False, **unsupported):
        from . import get_default_context
        if not isinstance(node_name, str) or not re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]*', node_name):
            raise ValueError('invalid node name')
        namespace = '/' if namespace in (None, '') else namespace
        if not isinstance(namespace, str) or not re.fullmatch(r'/(?:[A-Za-z_][A-Za-z_0-9]*(?:/[A-Za-z_][A-Za-z_0-9]*)*)?', namespace):
            raise ValueError('invalid namespace')
        if (unsupported or cli_args not in (None, [], ()) or parameter_overrides or
                allow_undeclared_parameters or automatically_declare_parameters_from_overrides):
            raise NotImplementedError('requested Node features are not yet source compatible')
        # These native facilities need distinct explicit grants. Their flags
        # never cause automatic servers/logger opens during local construction.
        if type(enable_rosout) is not bool or type(start_parameter_services) is not bool or type(use_global_arguments) is not bool:
            raise TypeError('Node flags must be bool')
        self.context = get_default_context() if context is None else context
        if not self.context.ok():
            raise RuntimeError('call rclpy.init first')
        self._name, self._namespace = node_name, namespace
        self._node = CapabilityNode(self.context.session)
        self._sources, self._owners, self.publishers, self.subscriptions, self.timers = [], [], [], [], []
        self._executor = ContextExecutor(self.context, 256)
        self.services, self.clients = [], []
        self._clock, self._logger_owner, self._destroyed = None, None, False
        self._destroy_cleanup = None
        self._destroy_future_cleanup = None
        self._logger = _Logger(self)
        self.context.nodes.append(self)

    def get_name(self):
        return self._name

    def get_namespace(self):
        return self._namespace

    def get_fully_qualified_name(self):
        return self._namespace.rstrip('/') + '/' + self._name

    def resolve_topic_name(self, name, *, only_expand=False):
        if not isinstance(name, str):
            raise TypeError('topic name')
        if name.startswith('~/'):
            name = self.get_fully_qualified_name() + name[1:]
        elif not name.startswith('/'):
            name = self._namespace.rstrip('/') + '/' + name
        if not re.fullmatch(r'/[A-Za-z_][A-Za-z_0-9]*(?:/[A-Za-z_][A-Za-z_0-9]*)*', name):
            raise ValueError('invalid topic name')
        return name

    def _live(self):
        if self._destroyed or not self.context.ok():
            raise RuntimeError('node/context is not live')

    def _factory(self):
        self._live()
        if (self._executor.spinning or self.context._dispatching
                or self.context.session._factory_current is not None):
            raise RosError(Result(17))

    def _capacity(self, *, source=False, publisher=False):
        if ((source and len(self._executor.sources) >= self._executor.maximum) or
                (publisher and len(self.publishers) >= 256)):
            raise RosError(Result(90))

    def _failure(self, frame, primary, registration):
        _factory_failure(frame, primary)
        if registration is not None:
            try:
                registration.rollback()
            except BaseException as error:
                frame.diagnostic_error = error
        # Never let optional diagnostic growth replace the active exception.

    def create_publisher(self, msg_type, topic, qos_profile, **unsupported):
        self._factory(); self._capacity(publisher=True)
        if unsupported:
            raise NotImplementedError('publisher options: ' + ','.join(unsupported))
        codec = _MessageCodec(msg_type); profile = qos.checked(qos_profile)
        topic = self.resolve_topic_name(topic)
        binding = self.context.binding('topic_publish', topic, msg_type._ROSWASM_TYPE, profile)
        frame, owns = self.context.session._begin_factory(1)
        registration = None
        try:
            publisher = _Publisher(None, topic, msg_type, profile)
            registration = _Registration((self._owners, self.publishers))
            resource = self._node.create_publisher(codec, binding.alias)
            publisher._resource = resource
            registration.set(0, resource.owner); registration.set(1, publisher)
            frame.committed = True
            return publisher
        except BaseException as error:
            self._failure(frame, error, registration)
            raise
        finally:
            self.context.session._end_factory(frame, owns)

    def create_subscription(self, msg_type, topic, callback, qos_profile, **unsupported):
        self._factory(); self._capacity(source=True)
        if unsupported:
            raise NotImplementedError('subscription options: ' + ','.join(unsupported))
        codec = _MessageCodec(msg_type); profile = qos.checked(qos_profile)
        topic = self.resolve_topic_name(topic)
        binding = self.context.binding('topic_subscribe', topic, msg_type._ROSWASM_TYPE, profile)
        frame, owns = self.context.session._begin_factory(1)
        registration = None
        try:
            registration = _Registration((self._owners, self._sources,
                                          self.subscriptions, self._executor.sources))
            subscription = self._node.create_subscription(codec, binding.alias, callback, binding.capacity)
            subscription.topic, subscription.msg_type, subscription.qos_profile = topic, msg_type, profile
            registration.set(0, subscription.owner); registration.set(1, subscription)
            registration.set(2, subscription); registration.set(3, subscription)
            frame.committed = True
            return subscription
        except BaseException as error:
            self._failure(frame, error, registration)
            raise
        finally:
            self.context.session._end_factory(frame, owns)

    def create_service(self, srv_type, srv_name, callback, *,
                       qos_profile=qos.qos_profile_services_default, callback_group=None):
        """Select exact advertise/respond roles before opening either owner."""
        import inspect
        from .service import Service
        from ._service_support import codecs_for, RecordCodec, distinct
        self._factory(); self._capacity(source=True)
        request, response = codecs_for(srv_type)
        if callback_group is not None or inspect.iscoroutinefunction(callback):
            raise NotImplementedError('native callback groups/coroutine Tasks are not implemented')
        if not callable(callback):
            raise TypeError('service callback required')
        profile = qos.checked(qos_profile)
        name = self.resolve_topic_name(srv_name)
        advertise = self.context.binding('service_advertise', name, srv_type._ROSWASM_TYPE, profile)
        respond = self.context.binding('service_respond', name, srv_type._ROSWASM_TYPE, profile)
        prepare_logger = self.context.prepare_callback_logging
        new_logger = prepare_logger and self._logger_owner is None
        logger = (self.context.binding('log_write', self.get_fully_qualified_name())
                  if prepare_logger else None)
        if prepare_logger:
            distinct(advertise, respond, logger)
            if not new_logger and self._logger_owner.alias != logger.alias:
                raise RosError(Result(17))
        else:
            distinct(advertise, respond)
        if advertise.capacity < 8 + request._MIN_WIRE_SIZE:
            raise ValueError('complete request ID and payload capacity required')
        required = 3 if new_logger else 2
        if len(self.context.session.owners) > self.context.session.maximum - required:
            raise RosError(Result(90))
        frame, owns = self.context.session._begin_factory(required)
        registration = None
        try:
            service = Service(self, srv_type, name, callback, profile)
            request_codec, response_codec = RecordCodec(request), RecordCodec(response)
            collections = (self._owners,) * required + (self._sources,
                           self.services, self._executor.sources)
            registration = _Registration(collections)
            logger_owner = self.context.session.open(logger.alias) if new_logger else None
            resource = self._node.create_service(request_codec, response_codec,
                advertise.alias, respond.alias, service._respond, advertise.capacity)
            service._resource = resource
            service._dispatch_owners[0] = resource.advertise
            service._dispatch_owners[1] = resource.respond
            owner_start = 1 if new_logger else 0
            registration.set(owner_start, resource.advertise)
            registration.set(owner_start + 1, resource.respond)
            registration.set(required, service)
            registration.set(required + 1, service); registration.set(required + 2, service)
            if new_logger:
                registration.set(0, logger_owner)
                self._logger_owner = logger_owner
            frame.committed = True
            return service
        except BaseException as error:
            if new_logger:
                self._logger_owner = None
            self._failure(frame, error, registration)
            raise
        finally:
            self.context.session._end_factory(frame, owns)

    def create_client(self, srv_type, srv_name, *,
                      qos_profile=qos.qos_profile_services_default, callback_group=None):
        """Create one service source plus its distinct real completion observer."""
        from .client import Client
        from ._service_support import codecs_for, RecordCodec, distinct
        self._factory(); self._capacity(source=True)
        request, response = codecs_for(srv_type)
        if callback_group is not None:
            raise NotImplementedError('native callback groups are not implemented')
        profile = qos.checked(qos_profile)
        name = self.resolve_topic_name(srv_name)
        call = self.context.binding('service_call', name, srv_type._ROSWASM_TYPE, profile)
        observe = self.context.binding('event_observe', name, srv_type._ROSWASM_TYPE, profile)
        prepare_logger = self.context.prepare_callback_logging
        new_logger = prepare_logger and self._logger_owner is None
        logger = (self.context.binding('log_write', self.get_fully_qualified_name())
                  if prepare_logger else None)
        if prepare_logger:
            distinct(call, observe, logger)
            if not new_logger and self._logger_owner.alias != logger.alias:
                raise RosError(Result(17))
        else:
            distinct(call, observe)
        if call.capacity < response._MIN_WIRE_SIZE or observe.capacity < 64 + response._MIN_WIRE_SIZE:
            raise ValueError('complete bounded response/event capacity required')
        required = 3 if new_logger else 2
        if len(self.context.session.owners) > self.context.session.maximum - required:
            raise RosError(Result(90))
        frame, owns = self.context.session._begin_factory(required)
        registration = None
        try:
            client = Client(self, srv_type, name, profile)
            request_codec, response_codec = RecordCodec(request), RecordCodec(response)
            collections = (self._owners,) * required + (self._sources,
                           self.clients, self._executor.sources)
            registration = _Registration(collections)
            logger_owner = self.context.session.open(logger.alias) if new_logger else None
            resource = self._node.create_client(request_codec, response_codec,
                                                 call.alias, call.capacity)
            observer = self._node.create_observer(observe.alias, observe.capacity)
            # The actual Host requires an event_watch of this exact service.call
            # source before submit; successful acknowledgement is not readiness.
            observer.watch(resource.owner).checked()
            client._resource, client._observer = resource, observer
            client._source_handle = resource.owner.handle
            client._dispatch_owners[0] = resource.owner
            client._dispatch_owners[1] = observer.owner
            owner_start = 1 if new_logger else 0
            registration.set(owner_start, resource.owner)
            registration.set(owner_start + 1, observer.owner)
            registration.set(required, client)
            registration.set(required + 1, client); registration.set(required + 2, client)
            if new_logger:
                registration.set(0, logger_owner)
                self._logger_owner = logger_owner
            frame.committed = True
            return client
        except BaseException as error:
            if new_logger:
                self._logger_owner = None
            self._failure(frame, error, registration)
            raise
        finally:
            self.context.session._end_factory(frame, owns)

    def _destroy_service_resource(self, resource, collection):
        if resource not in collection:
            return False
        self._factory()
        try:
            resource.destroy()
        finally:
            # Destruction may report a real close failure after retiring every
            # owner. Remove scheduling only for that committed local retirement.
            if resource._retired:
                if resource in self._executor.sources:
                    self._executor.remove(resource)
                if resource in self._sources:
                    self._sources.remove(resource)
                collection.remove(resource)
        return True

    def destroy_service(self, service):
        return self._destroy_service_resource(service, self.services)

    def destroy_client(self, client):
        return self._destroy_service_resource(client, self.clients)

    def get_logger(self):
        return self._logger

    def _open_logger(self):
        self._factory()
        binding = self.context.binding('log_write', self.get_fully_qualified_name())
        frame, owns = self.context.session._begin_factory(1)
        registration = None
        try:
            registration = _Registration((self._owners,))
            owner = self.context.session.open(binding.alias)
            registration.set(0, owner)
            self._logger_owner = owner
            frame.committed = True
        except BaseException as error:
            self._failure(frame, error, registration)
            raise
        finally:
            self.context.session._end_factory(frame, owns)

    def get_clock(self):
        self._live()
        if self._clock is None:
            self._factory()
            frame, owns = self.context.session._begin_factory(1)
            registration = None
            try:
                registration = _Registration((self._owners,))
                clock = Clock(clock_type=ClockType.ROS_TIME, context=self.context,
                              node_name=self.get_fully_qualified_name())
                registration.set(0, clock.owner)
                self._clock = clock
                frame.committed = True
            except BaseException as error:
                self._failure(frame, error, registration)
                raise
            finally:
                self.context.session._end_factory(frame, owns)
        return self._clock

    def create_timer(self, timer_period_sec, callback, callback_group=None, clock=None, autostart=True):
        self._factory(); self._capacity(source=True)
        if callback_group is not None:
            raise NotImplementedError('callback groups are not yet source compatible')
        # Match Timer's local period/callback validation before lazy clock open.
        if (type(timer_period_sec) not in (int, float) or not math.isfinite(timer_period_sec)
                or timer_period_sec <= 0 or not callable(callback)):
            raise ValueError('positive finite period and callback required')
        period = int(timer_period_sec * 1000000000)
        if not 0 < period < 1 << 63:
            raise ValueError('timer nanosecond range')
        new_clock = clock is None and self._clock is None
        frame, owns = self.context.session._begin_factory(1 if new_clock else 0)
        registration = None
        try:
            collections = (self._sources, self.timers, self._executor.sources)
            if new_clock:
                collections = collections + (self._owners,)
            registration = _Registration(collections)
            selected = clock if clock is not None else self._clock
            if new_clock:
                selected = Clock(clock_type=ClockType.ROS_TIME, context=self.context,
                                 node_name=self.get_fully_qualified_name())
            timer = Timer(timer_period_sec, callback, selected, autostart=autostart)
            registration.set(0, timer); registration.set(1, timer); registration.set(2, timer)
            if new_clock:
                registration.set(3, selected.owner)
                self._clock = selected
            frame.committed = True
            return timer
        except BaseException as error:
            self._failure(frame, error, registration)
            raise
        finally:
            self.context.session._end_factory(frame, owns)

    def destroy_timer(self, timer):
        if timer not in self.timers:
            return False
        self._executor.remove(timer)
        timer.destroy(); self.timers.remove(timer); self._sources.remove(timer); return True

    def destroy_publisher(self, publisher):
        if publisher not in self.publishers:
            return False
        publisher.destroy(); self.publishers.remove(publisher); return True

    def destroy_subscription(self, subscription):
        if subscription not in self.subscriptions:
            return False
        if self._executor.spinning:
            raise RosError(Result(17))
        # Reserve diagnostic/work storage before the close effect.
        cleanup = [None, None, None]
        subscription._destroy_cleanup = cleanup
        work = iter(((self._executor.remove, 0),
                     (self.subscriptions.remove, 1), (self._sources.remove, 2)))
        primary = cleanup_error = None
        try:
            subscription.close().checked()
        except BaseException as error:
            primary = error
            raise
        finally:
            try:
                # Actual attempts retire the ID even on errno/unknown outcomes.
                # A NotAttempted pre-entry failure remains scheduled/owned.
                if not subscription.owner.handle:
                    for remove, index in work:
                        try:
                            remove(subscription)
                        except BaseException as failure:
                            cleanup[index] = failure
                            if cleanup_error is None:
                                cleanup_error = failure
            except BaseException as failure:
                if cleanup_error is None:
                    cleanup_error = failure
            if cleanup_error is not None:
                if primary is None:
                    raise cleanup_error
                try:
                    primary.roswasm_destroy_cleanup = cleanup_error
                except BaseException:
                    pass  # Reserved subscription diagnostic remains available.
        return True

    def destroy_node(self):
        if self._destroyed:
            return True
        if (self._executor.spinning or self.context._dispatching
                or self.context.session._factory_current is not None
                or any(owner.busy for owner in self._owners)):
            raise RosError(Result(17))
        frame, owns = self.context.session._begin_factory(len(self._owners))
        try:
            frame.seed(self._owners)
            timer_work = iter(tuple((index, timer.destroy) for index, timer in enumerate(self.timers)))
            timer_failures = [None] * len(self.timers)
            client_rows = tuple((client, client._finish_closed, index)
                                for index, client in enumerate(self.clients))
            client_work = iter(client_rows)
            client_failures = [None] * len(client_rows)
            service_work = iter(tuple(self.services))
            future_lease = DispatchLease(self.context)
            error = RuntimeError('node cleanup failed')
            error.roswasm_failures = frame.failures
            error.roswasm_cleanup = frame.outcomes
            error.roswasm_timer_failures = timer_failures
            error.roswasm_future_failures = client_failures
            self._destroy_cleanup = frame
            self._destroy_future_cleanup = client_failures
            self._destroyed = True
            try:
                with future_lease:
                    for client, finish, index in client_work:
                        try:
                            client._retired = True
                            finish()
                        except BaseException as failure:
                            client_failures[index] = failure
                            frame.diagnostic_error = failure
                    for service in service_work:
                        service._retired = True
            except BaseException as failure:
                # The reserved actual-owner cleanup below must still run even
                # if auxiliary Future notification or lease setup is interrupted.
                frame.diagnostic_error = failure
            timer_bad = False
            for index, destroy in timer_work:
                try:
                    destroy()
                except BaseException as failure:
                    timer_failures[index] = failure
                    timer_bad = True
            frame.rollback(None)
            list.clear(self._sources); list.clear(self._executor.sources)
            list.clear(self.clients); list.clear(self.services)
            try:
                if self in self.context.nodes:
                    self.context.nodes.remove(self)
            except BaseException as failure:
                frame.diagnostic_error = failure
            if timer_bad or frame.failed() or not frame.completed:
                raise error
            return True
        finally:
            self.context.session._end_factory(frame, owns)
