"""Guest-local ownership, typed resources and bounded synchronous dispatch.

The host remains the authority for type, QoS, deadline, quotas and permissions.
Every factory below opens an explicit alias. A Node is a local organizer, not
an implicitly authorized native node. There are no background threads, sockets,
filesystem lookups, ambient ROS discovery, clocks, or automatic retries.
"""
from dataclasses import dataclass
from .raw import Backend, Result, RosError
from .codecs import Reader, Writer, goal_id
from .events import Binding


@dataclass(frozen=True)
class Cleanup:
    """All close outcomes in reverse acquisition order and the primary error."""
    primary: object
    outcomes: object

    @property
    def failures(self):
        """Retain ABI failures and bridge exceptions without conflating them."""
        return tuple(item for item in self.outcomes
                     if item is None or isinstance(item, BaseException) or item.error)


class _FactoryOutcomes:
    """Preallocated sequence view; observation may allocate, rollback never does."""
    __slots__ = ('frame',)

    def __init__(self, frame):
        self.frame = frame

    def __len__(self):
        return self.frame.attempted

    def __iter__(self):
        for index, attempted in enumerate(self.frame.attempted_flags):
            if attempted:
                yield self.frame.results[index]

    def __getitem__(self, wanted):
        if type(wanted) is not int:
            raise TypeError('cleanup outcome index')
        if wanted < 0:
            wanted += len(self)
        for index, attempted in enumerate(self.frame.attempted_flags):
            if attempted:
                if wanted == 0:
                    return self.frame.results[index]
                wanted -= 1
        raise IndexError(wanted)


class _FactoryFailures:
    """Precreated failure-only diagnostic view; full results stay in outcomes."""
    __slots__ = ('frame',)

    def __init__(self, frame):
        self.frame = frame

    def __iter__(self):
        for value in self.frame.outcomes:
            if value is None or isinstance(value, BaseException) or value.error:
                yield value
        if self.frame.diagnostic_error is not None:
            yield self.frame.diagnostic_error

    def __len__(self):
        return sum(1 for _ in self)

    def __getitem__(self, wanted):
        if type(wanted) is not int or wanted < 0:
            raise IndexError(wanted)
        for value in self:
            if wanted == 0:
                return value
            wanted -= 1
        raise IndexError(wanted)


class _FactoryCleanup:
    """One bounded transaction's owner slots and rollback data, reserved first.

    All owner/result/work/count/iterator storage and diagnostic slots exist
    before any open. Session.open registers a pending owner here BEFORE its raw
    effect; a returned owner never depends on a later append for rollback.
    The fixed result slots retain every cleanup error/exception independently
    from primary. Auxiliary exception attributes are best-effort observations.
    This is ordinary executable Python/OOM recovery, not terminal-trap immunity.
    """
    __slots__ = ('session', 'owners', 'closers', 'results', 'attempted_flags', 'work',
                 'primary', 'attempted', 'native_attempted_flags', 'completed', 'committed',
                 'diagnostic_error', 'outcomes', 'failures', '_owner_indices',
                 '_work_iterator', '_count_iterator', '_rolled_back', '_failed')

    def __init__(self, session, capacity):
        if type(capacity) is not int or not 0 <= capacity <= 65536:
            raise ValueError('bounded factory owner slots required')
        self.session = session
        self.owners = [None] * capacity
        self.closers = [None] * capacity
        self.results = [None] * capacity
        self.attempted_flags = [False] * capacity
        self.native_attempted_flags = [False] * capacity
        self.work = tuple((owner, output) for output, owner in enumerate(range(capacity - 1, -1, -1)))
        self.primary = self.diagnostic_error = None
        self.attempted = 0
        self.completed = self.committed = self._rolled_back = self._failed = False
        self.outcomes = _FactoryOutcomes(self)
        self.failures = _FactoryFailures(self)
        self._owner_indices = iter(tuple(range(capacity)))
        self._work_iterator = iter(self.work)
        self._count_iterator = iter(tuple(range(1, capacity + 1)))

    def reserve(self, owner):
        # Called before this owner's native effect, never after successful open.
        index = next(self._owner_indices)
        self.owners[index] = owner
        self.closers[index] = owner.close

    def seed(self, owners):
        # Existing-owner cleanup preflight: all assignments precede first close.
        for owner in owners:
            self.reserve(owner)

    def rollback(self, primary):
        if self._rolled_back:
            return
        self.primary = primary
        self._rolled_back = True
        for owner_index, output in self._work_iterator:
            owner = self.owners[owner_index]
            if owner is not None and owner.handle:
                self.attempted_flags[output] = True
                self.attempted = next(self._count_iterator)
                try:
                    value = self.closers[owner_index]()
                    self.results[output] = value
                    if value is None or value.error:
                        self._failed = True
                except BaseException as error:
                    self.results[output] = error
                    self._failed = True
                finally:
                    # attempted_flags counts local calls, not host effects.
                    self.native_attempted_flags[output] = (
                        owner._close_attempt[0] or owner._revoke_attempt[0])
        self.completed = True

    def failed(self):
        return self._failed or self.diagnostic_error is not None


def _factory_failure(frame, primary):
    """Preserve the setup primary even if optional error attributes cannot grow."""
    try:
        frame.rollback(primary)
    except BaseException as error:
        # Known rollback steps use reserved slots. Retain an unexpected language
        # interruption without replacing primary or claiming cleanup completed.
        frame.diagnostic_error = error
    try:
        primary.roswasm_cleanup = frame.outcomes
    except BaseException:
        pass


class Capability:
    """One backend/instance-owned handle. Explicit close retires it locally once."""
    def __init__(self, session, handle, alias):
        if type(handle) is not int or not 0 < handle < 1 << 64:
            raise ValueError('successful open did not return a nonzero handle')
        # All witness/storage allocation precedes acquisition in Session.open.
        self.session, self._raw_handle, self.alias = session, handle, alias
        self._close_attempt = [False]
        self._revoke_attempt = [False]
        self.busy = False

    @property
    def handle(self):
        # The native bridge flips these exact-list cells immediately before the
        # real import. Result allocation failure cannot make a retired ID live.
        if self._close_attempt[0] or self._revoke_attempt[0]:
            return 0
        return self._raw_handle

    def invoke(self, operation, payload=b'', capacity=0, other=None, correlation=0):
        """Use this owner only; a separate owner is required for other rights."""
        if not self.handle:
            return Result(9)
        if not self.session.active:
            return Result(125)
        if other is not None and (other.session is not self.session or not other.handle):
            return Result(9)
        if operation == 'capability_open':
            raise ValueError('use Session.open')
        if operation in ('capability_close', 'capability_revoke'):
            if (payload != b'' or type(capacity) is not int or capacity != 0
                    or other is not None or type(correlation) is not int or correlation != 0):
                raise ValueError('empty retirement form required')
            return self.close() if operation == 'capability_close' else self.revoke()
        return self.session.backend.invoke(operation, self.handle, payload, capacity,
                                           0 if other is None else other.handle, correlation)

    def close(self):
        """Attempt cleanup once. A timeout cannot cause a close replay."""
        if self.busy or self.session.retiring:
            return Result(17)
        return self._close()

    def _close(self):
        if not self.handle:
            return Result(9)
        handle = self.handle
        return self.session.backend.retire('capability_close', handle, self._close_attempt)

    def revoke(self):
        """Explicitly revoke this alias and retire all matching local siblings."""
        if self.busy or self.session.retiring or any(
                owner.busy for owner in self.session.owners if owner.alias == self.alias):
            return Result(17)
        if not self.handle:
            return Result(9)
        handle = self.handle
        # Installation may fail before entry, while every witness is still
        # False. Close has its own cell: a failed revoke preparation must not
        # make a later close of one sibling retire the others.
        witness = self._revoke_attempt
        for owner in self.session.owners:
            if owner.alias == self.alias and owner.handle:
                owner._revoke_attempt = witness
        return self.session.backend.retire('capability_revoke', handle, witness)


class Session:
    """Explicit backend and bounded stable owners; construction opens nothing."""
    def __init__(self, backend, maximum_owners=64):
        if not isinstance(backend, Backend):
            raise TypeError('a real CPython/WASI Backend is required')
        if type(maximum_owners) is not int or not 0 <= maximum_owners <= 65536:
            raise ValueError('invalid local owner bound')
        self.backend, self.maximum = backend, maximum_owners
        self.owners = []
        self.active, self.retiring = True, False
        self.cleanup = None
        self.last_factory_cleanup = self._factory_current = None

    def check_abi(self):
        """Check version1; this is not proof of any grant or ROS availability."""
        if self.backend.abi_version() != 1:
            raise RosError(Result(95))

    def _begin_factory(self, capacity):
        """Internal same-session synchronous transaction; nesting reuses slots."""
        if self._factory_current is not None:
            return self._factory_current, False
        frame = _FactoryCleanup(self, capacity)
        # Construct the return tuple before publishing either state change.
        result = frame, True
        self.last_factory_cleanup = frame
        self._factory_current = frame
        return result

    def _end_factory(self, frame, owns):
        if owns and self._factory_current is frame:
            self._factory_current = None

    def open(self, alias, *, owned_capture=False):
        """Open once; register pending factory ownership before its raw effect."""
        if type(owned_capture) is not bool:
            raise TypeError('owned capture selection must be an exact bool')
        if owned_capture and self._factory_current is None:
            raise ValueError('owned capture requires the retained application factory')
        if not self.active:
            raise RosError(Result(125))
        if self.retiring:
            raise RosError(Result(17))
        if len(self.owners) >= self.maximum:
            raise RosError(Result(90))
        # Allocate/register pending local storage before acquiring a real handle.
        owner = object.__new__(Capability)
        owner.session, owner._raw_handle, owner.alias, owner.busy = self, 0, alias, False
        owner._close_attempt = [False]
        owner._revoke_attempt = [False]
        capture = self.backend.prepare_owned_open(alias, owner._close_attempt) if owned_capture else None
        if capture is not None:
            owner._open_capture = capture
        if self._factory_current is not None:
            self._factory_current.reserve(owner)
        self.owners.append(owner)
        try:
            result = (self.backend.open(alias, capture=capture) if capture is not None else self.backend.open(alias)).checked()
            if type(result.value) is not int or not 0 < result.value < 1 << 64:
                raise ValueError('successful open did not return a nonzero handle')
            # Existing raw field only: do not add a post-open property setter.
            owner._raw_handle = result.value
            if capture is not None:
                capture.adopt()
            return owner
        except BaseException as primary:
            if capture is None:
                try:
                    self.owners.remove(owner)
                except BaseException:
                    pass  # Original untracked pending-owner behavior.
                raise
            if capture is not None:
                if capture.primary is None:
                    capture.primary = primary
                try:
                    capture.cleanup()
                except BaseException as cleanup:
                    capture.cleanup_error = cleanup
                # Actual C cleanup itself updates owner._close_attempt before
                # raw entry. No allocating Python observer can authorize replay.
            if not owner.handle:
                try:
                    self.owners.remove(owner)
                except BaseException:
                    pass  # Frame still retains its real C capture/owner.
            # An adopted handle survives any later language interruption in the
            # actual Session/frame so ordinary Capability cleanup can retire it.
            raise

    def shutdown(self):
        """Stop local scheduling. Existing owners still require explicit close."""
        self.active = False

    def close(self, primary=None):
        """Close all live owners even after a failure; refuse busy groups first."""
        if self.retiring or any(owner.busy for owner in self.owners):
            return Cleanup(primary, (Result(17),))
        # Reserve the entire work list/result storage before any close effect.
        # List iteration/unpacking and indexed writes reuse existing objects.
        work = tuple(zip(reversed(self.owners), range(len(self.owners))))
        skipped = Result(0)
        outcomes = [None] * len(work)
        report = Cleanup(primary, outcomes)
        self.cleanup = report
        self.retiring = True
        try:
            for owner, index in work:
                if owner.handle:
                    try:
                        outcomes[index] = owner._close()
                    except BaseException as error:
                        outcomes[index] = error
                else:
                    outcomes[index] = skipped
            # Keep the stable ledger, including any NotAttempted IDs after a
            # pre-entry bridge failure. Ordinary use stops, but explicit close
            # remains possible. Attempted/unknown IDs stay permanently hidden.
            self.active = False
            return report
        finally:
            self.retiring = False

    def __enter__(self):
        self.check_abi(); return self

    def __exit__(self, kind, error, trace):
        report = self.close(error)
        if error is None and report.failures:
            first = report.failures[0]
            if first is None:
                raise RuntimeError('cleanup was interrupted before every owner was attempted')
            if isinstance(first, BaseException):
                raise first
            raise RosError(first)
        return False


class Publisher:
    """Typed topic publisher over one separately granted publish alias."""
    def __init__(self, owner, codec):
        self.owner, self.codec = owner, codec

    def publish(self, value):
        """Encode before the one host effect; return its original ABI result."""
        return self.owner.invoke('topic_publish', self.codec.encode(value))

    def close(self):
        """Expose cleanup status; do not rely on Python finalizers."""
        return self.owner.close()


class Subscription:
    """Guest-driven subscription with an explicit canonical output bound."""
    def __init__(self, owner, codec, capacity, callback):
        if type(capacity) is not int or not 0 <= capacity <= 4194304:
            raise ValueError('invalid receive capacity')
        if not callable(callback):
            raise TypeError('callback required')
        self.owner, self.codec, self.capacity, self.callback = owner, codec, capacity, callback

    def poll_once(self):
        """Deliver at most one value. Error11 is idle; error90 is never replayed."""
        if self.owner.busy:
            return Result(17)
        self.owner.busy = True
        try:
            result = self.owner.invoke('event_poll', capacity=self.capacity)
            if result.error == 0:
                value = self.codec.decode(result.data)
                self.callback(value)
            return result
        finally:
            self.owner.busy = False

    def close(self):
        """Refuse close during decode/callback; otherwise retire once."""
        return self.owner.close()


class ServiceClient:
    """Typed synchronous calls or explicitly observed asynchronous admission."""
    def __init__(self, owner, request_codec, response_codec, capacity):
        self.owner, self.request_codec, self.response_codec, self.capacity = owner, request_codec, response_codec, capacity

    def call(self, request):
        """Return a decoded response; exception retains timeout/capacity status."""
        result = self.owner.invoke('service_call', self.request_codec.encode(request), self.capacity).checked()
        return self.response_codec.decode(result.data)

    def submit(self, request, observer):
        """Admit once; a returned ID is not a completed response."""
        return self.owner.invoke('service_submit', self.request_codec.encode(request), other=observer.owner)

    def close(self):
        return self.owner.close()


class ServiceServer:
    """Poll requests and respond with their exact owned IDs using another alias."""
    def __init__(self, advertise, respond, request_codec, response_codec, capacity, callback):
        if advertise.session is not respond.session or not callable(callback):
            raise ValueError('same-session separate owners and callback required')
        self.advertise, self.respond = advertise, respond
        self.request_codec, self.response_codec = request_codec, response_codec
        self.capacity, self.callback = capacity, callback

    def poll_once(self):
        """Retain both owners through decode, callback and the single response."""
        if self.advertise.busy or self.respond.busy:
            return Result(17)
        self.advertise.busy = self.respond.busy = True
        try:
            result = self.advertise.invoke('event_poll', capacity=self.capacity)
            if result.error:
                return result
            reader = Reader(result.data); request_id = reader.integer(8)
            if not request_id:
                raise ValueError('zero service request ID')
            request = self.request_codec.decode(reader.take(len(result.data) - 8)); reader.end()
            response = self.callback(request)
            payload = Writer().integer(request_id, 8).raw(self.response_codec.encode(response)).finish()
            return self.respond.invoke('service_respond', payload)
        finally:
            self.advertise.busy = self.respond.busy = False


class Observer:
    """Unified event observer. Data masks and rights come from its host grant."""
    def __init__(self, owner, capacity):
        if type(capacity) is not int or not 64 <= capacity <= 4194304:
            raise ValueError('complete bounded event capacity required')
        self.owner, self.capacity, self.bindings = owner, capacity, {}
        self.sequence = 0
        self.instance = self.session = None

    def watch(self, source, correlation=0):
        """Register an exact owned source and commit local binding on success."""
        if source.session is not self.owner.session or not source.handle:
            return Result(9)
        Binding(self.owner.session.backend.version, source.handle, correlation)
        result = self.owner.invoke('event_watch', other=source, correlation=correlation)
        if not result.error:
            self.bindings[(source.handle, correlation)] = True
        return result

    def poll_once(self):
        """Read one complete envelope; require a registered source binding."""
        from .events import decode
        result = self.owner.invoke('event_poll_v1', capacity=self.capacity)
        if result.error:
            return result, None
        event = decode(result.data)
        if ((event.source, event.correlation) not in self.bindings and
                (event.source, 0) not in self.bindings):
            raise ValueError('event has no local source binding')
        # Sequence belongs to the observer queue, including interleaved sources.
        if (event.backend != self.owner.session.backend.version or event.sequence != self.sequence + 1 or
                (self.instance is not None and (event.instance != self.instance or event.session != self.session))):
            raise ValueError('observer backend/session/sequence mismatch')
        self.instance, self.session, self.sequence = event.instance, event.session, event.sequence
        return result, event

    def readiness(self):
        """Return four exact u64 counters; these reserve no credit."""
        data = self.owner.invoke('event_ready', capacity=32).checked().data
        r = Reader(data); value = tuple(r.integer(8) for _ in range(4)); r.end(); return value


class ActionClient:
    """Explicit Action protocol capabilities and local outgoing-goal ownership."""
    def __init__(self, send, feedback=None, result=None, cancel=None, maximum_goals=32):
        if type(maximum_goals) is not int or not 0 < maximum_goals <= 65536:
            raise ValueError('invalid goal bound')
        owners = [item for item in (send, feedback, result, cancel) if item is not None]
        if any(item.session is not send.session for item in owners):
            raise ValueError('all Action owners must share a session')
        self.send, self.feedback, self.result, self.cancel = send, feedback, result, cancel
        self.maximum, self.goals = maximum_goals, set()

    def send_goal(self, canonical_goal):
        """Send once and retain the nonzero returned goal ID. Never replay110."""
        if len(self.goals) >= self.maximum:
            raise RosError(Result(90))
        value = self.send.invoke('action_send_goal', canonical_goal, 8).checked().data
        r = Reader(value); identity = r.integer(8); r.end(); goal_id(identity)
        if identity in self.goals:
            raise ValueError('duplicate live goal ID')
        self.goals.add(identity); return identity

    def cancel_goal(self, identity):
        """Cancel only an owned goal with an independently granted cancel alias."""
        if identity not in self.goals or self.cancel is None:
            return Result(9 if identity not in self.goals else 95)
        return self.cancel.invoke('action_cancel_own_goal', goal_id(identity), 1)

    def receive(self, identity, capacity, terminal=False):
        """Read a selected goal, validating its ID before terminal retirement."""
        if identity not in self.goals:
            return Result(9)
        owner = self.result if terminal else self.feedback
        if owner is None:
            return Result(95)
        operation = 'action_receive_result' if terminal else 'action_receive_feedback'
        result = owner.invoke(operation, goal_id(identity), capacity)
        if not result.error:
            if len(result.data) < (9 if terminal else 8) or int.from_bytes(result.data[:8], 'little') != identity:
                raise ValueError('Action response does not match the owned goal')
            if terminal:
                if result.data[8] not in (2, 3, 4):
                    raise ValueError('nonterminal Action result')
                self.goals.remove(identity)
        return result


class Node:
    """Local typed factories over an explicit Session and explicit alias names."""
    def __init__(self, session):
        if not isinstance(session, Session):
            raise TypeError('Session required')
        self.session = session

    def create_publisher(self, codec, alias):
        frame, owns = self.session._begin_factory(1)
        try:
            resource = Publisher(None, codec)
            resource.owner = self.session.open(alias)
            if owns:
                frame.committed = True
            return resource
        except BaseException as error:
            _factory_failure(frame, error)
            raise
        finally:
            self.session._end_factory(frame, owns)

    def create_subscription(self, codec, alias, callback, capacity=65536):
        if not callable(callback) or type(capacity) is not int or not 0 <= capacity <= 4194304:
            raise ValueError('callback and bounded capacity required')
        frame, owns = self.session._begin_factory(1)
        try:
            resource = Subscription(None, codec, capacity, callback)
            resource.owner = self.session.open(alias)
            resource.owner.invoke('topic_subscribe').checked()
            if owns:
                frame.committed = True
            return resource
        except BaseException as error:
            _factory_failure(frame, error)
            raise
        finally:
            self.session._end_factory(frame, owns)

    def create_client(self, request_codec, response_codec, alias, capacity=65536):
        if type(capacity) is not int or not 0 <= capacity <= 4194304:
            raise ValueError('bounded response capacity required')
        frame, owns = self.session._begin_factory(1)
        try:
            resource = ServiceClient(None, request_codec, response_codec, capacity)
            resource.owner = self.session.open(alias)
            if owns:
                frame.committed = True
            return resource
        except BaseException as error:
            _factory_failure(frame, error)
            raise
        finally:
            self.session._end_factory(frame, owns)

    def create_service(self, request_codec, response_codec, advertise_alias, respond_alias, callback, capacity=65536):
        if (not callable(callback) or advertise_alias == respond_alias or
                type(capacity) is not int or not 8 <= capacity <= 4194304):
            raise ValueError('callback, distinct aliases and bounded request capacity required')
        frame, owns = self.session._begin_factory(2)
        try:
            # Prepopulate every wrapper key before any acquisition; subsequent
            # owner assignments replace existing keys, never append tracking.
            resource = object.__new__(ServiceServer)
            resource.advertise = resource.respond = None
            resource.request_codec, resource.response_codec = request_codec, response_codec
            resource.capacity, resource.callback = capacity, callback
            resource.advertise = self.session.open(advertise_alias)
            resource.respond = self.session.open(respond_alias)
            resource.advertise.invoke('service_advertise').checked()
            if owns:
                frame.committed = True
            return resource
        except BaseException as error:
            _factory_failure(frame, error)
            raise
        finally:
            self.session._end_factory(frame, owns)

    def create_observer(self, alias, capacity=65536):
        if type(capacity) is not int or not 64 <= capacity <= 4194304:
            raise ValueError('complete bounded event capacity required')
        frame, owns = self.session._begin_factory(1)
        try:
            resource = Observer(None, capacity)
            resource.owner = self.session.open(alias)
            if owns:
                frame.committed = True
            return resource
        except BaseException as error:
            _factory_failure(frame, error)
            raise
        finally:
            self.session._end_factory(frame, owns)

    def open_operation(self, alias):
        """Open a parameter/TF/graph/lifecycle/time/log alias for explicit invoke.

        Complete raw operations remain available. The selected alias fixes its
        resource/type/transition/frames; empty payload never means global scope.
        """
        return self.session.open(alias)


class Executor:
    """Round-robin, caller-driven callback scheduling without ambient time."""
    def __init__(self, maximum_sources=64):
        if type(maximum_sources) is not int or not 0 <= maximum_sources <= 65536:
            raise ValueError('invalid source bound')
        self.maximum, self.sources, self.position, self.spinning = maximum_sources, [], 0, False

    def add(self, source):
        """Add one poll_once resource; mutating an active pass is refused."""
        if self.spinning or source in self.sources or len(self.sources) >= self.maximum:
            raise RosError(Result(17 if self.spinning or source in self.sources else 90))
        if not callable(getattr(source, 'poll_once', None)):
            raise TypeError('poll_once resource required')
        self.sources.append(source)

    def remove(self, source):
        """Remove local scheduling only; owner cleanup remains explicit."""
        if self.spinning:
            raise RosError(Result(17))
        self.sources.remove(source)
        self.position %= max(1, len(self.sources))

    def spin_once(self):
        """Poll each source at most once and deliver at most one ready callback."""
        if self.spinning:
            raise RosError(Result(17))
        self.spinning = True
        try:
            for _ in range(len(self.sources)):
                source = self.sources[self.position]
                self.position = (self.position + 1) % len(self.sources)
                result = source.poll_once()
                if result.error != 11:
                    return result
            return Result(11)
        finally:
            self.spinning = False
