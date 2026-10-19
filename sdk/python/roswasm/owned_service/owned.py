"""Caller-driven retained ROS2 Service owners, using the real optional builtin.

Host policy remains authoritative. These local identity/codec checks do not
certify a Native SDK, infer a grant, or replace the current Runtime caller.
All public methods are synchronous; application handlers run after take/ACK.
"""
from dataclasses import dataclass
import re
from ..raw import Backend
from ..client import Session, Capability
from .._registry import FUNCTIONS
from _roswasm_native import invoke as _legacy_invoke
from .bridge import Fact, attempted, returned, success, version_one
from .wire import (CLIENT, SERVICE, MAX_PACKET, PREFIX, MAX_WAIT, WireError,
                   uint, full_handle, expected_qos, receipt, same_receipt,
                   delivery, number)

FREE, RESERVED, ADMITTED, DELIVERED, DONE, ABANDONED, DISCARDED = range(7)
_close = Capability.close
_identity = re.compile(r'UWVMROS1_[0-9a-f]{64}\Z')


@dataclass(frozen=True)
class ServiceDefinition:
    """Original Service root facts and two distinct complete component codecs."""
    native_type: str
    codec_identity: str
    native_identity: object
    request: object
    response: object

    def __post_init__(self):
        if (type(self.native_type) is not str or not re.fullmatch(r'[A-Za-z_][A-Za-z_0-9]*/srv/[A-Za-z_][A-Za-z_0-9]*', self.native_type)
                or type(self.codec_identity) is not str or not _identity.fullmatch(self.codec_identity)
                or (self.native_identity is not None and (type(self.native_identity) is not str
                    or not 0 < len(self.native_identity) <= 256 or '\0' in self.native_identity))):
            raise ValueError('exact original Service identity facts required')
        for codec in (self.request, self.response):
            if (not callable(codec.encode) or not callable(codec.decode)
                    or not _identity.fullmatch(codec.codec_identity)
                    or type(codec.maximum) is not int or not 0 <= codec.maximum <= MAX_PACKET - PREFIX):
                raise ValueError('complete bounded canonical codec required')


class _Owner:
    __slots__ = ('cap', 'session', 'handle', 'cleanup', 'error', 'attempted',
                 'finished', 'retained', 'external', 'closer')

    def __init__(self, cap):
        if type(cap) is not Capability or type(cap.session) is not Session or type(cap.session.backend) is not Backend:
            raise TypeError('genuine exact SDK Capability/Session/Backend required')
        session, backend = cap.session, cap.session.backend
        if (backend.version != 2 or backend.functions is not FUNCTIONS[2] or backend._invoke is not _legacy_invoke
                or not any(item is cap for item in session.owners) or not full_handle(cap.handle)):
            raise ValueError('registered live ROS2 owner from the actual builtin required')
        self.cap, self.session, self.handle = cap, session, cap.handle
        self.cleanup = self.error = None
        self.attempted = self.finished = self.external = False
        self.retained = True
        self.closer = _close.__get__(cap, Capability)

    def current(self):
        if (self.cap.session is not self.session or self.cap.handle != self.handle
                or not self.session.active or self.session.retiring or self.cap.busy):
            raise WireError('owner/session is no longer current or is busy', 17)


class _Row:
    __slots__ = ('state', 'ticket', 'native_sequence', 'writer', 'submit', 'abandon', 'respond')

    def __init__(self):
        self.state, self.ticket, self.native_sequence = FREE, 0, 0
        self.writer = b''
        self.submit, self.abandon, self.respond = Fact(), Fact(), Fact()


@dataclass
class Outcome:
    """Allocated before effects/ACK; retained on endpoint if later Python fails."""
    status: int = 0
    ticket: object = None
    delivery: object = None
    available: object = None
    discarded: bool = False
    error: object = None


@dataclass(frozen=True)
class Delivery:
    ticket: int
    sequence: int
    kind: int
    control: bool
    control_flags: int
    error: int
    information: object
    value: object


class Endpoint:
    """Retain BEFORE activate. No finalizer invokes ROS or retries cleanup."""
    def __init__(self, kind, source, peer, definition, *, expected_profiles,
                 history_entries, ownership, availability=None):
        if type(definition) is not ServiceDefinition or kind not in (CLIENT, SERVICE):
            raise TypeError('actual selected Service definition required')
        if type(history_entries) is not int or not 1 <= history_entries <= 65536 or ownership not in ('owned', 'borrowed'):
            raise ValueError('explicit bounded history and ownership required')
        first, second = _Owner(source), _Owner(peer)
        query = None if availability is None else _Owner(availability)
        owners = (first, second) if query is None else (first, second, query)
        if (any(row.session is not first.session for row in owners)
                or len({id(row.cap) for row in owners}) != len(owners)
                or len({row.handle for row in owners}) != len(owners)
                or (query is not None and kind != CLIENT)):
            raise ValueError('independent distinct same-Session owners required')
        self.kind, self.source, self.peer, self.query = kind, first, second, query
        self.owners, self.own_capabilities = owners, ownership == 'owned'
        self.reverse_owners = tuple(reversed(owners))
        self.definition = definition
        # Delegates/bounds captured once, before effect or application getters.
        self.request_encode, self.request_decode = definition.request.encode, definition.request.decode
        self.response_encode, self.response_decode = definition.response.encode, definition.response.decode
        self.request_bound, self.response_bound = definition.request.maximum, definition.response.maximum
        self.expected_profiles = expected_qos(expected_profiles)
        self.history = tuple(_Row() for _ in range(history_entries))
        self.indices = tuple(range(history_entries + 1))
        self.used = 0
        self.calls = {name: Fact() for name in ('abi', 'activate', 'inspect', 'peek', 'commit', 'cancel', 'available', 'retire')}
        self.receipt = self.last_outcome = self.primary_error = None
        self.primary = 0
        self.activation_scheduled = self.retirement_scheduled = False
        self.active = self.closed = self.failed = self.busy = False
        self.last_committed = self.last_reservation = 0
        self.pending_borrow = self.pending_delivery = None
        self.cancel_borrow = None

    def _enter(self, active=True):
        if self.busy:
            return 17
        if self.closed or (active and (not self.active or self.failed)):
            return self.primary or 9
        for owner in self.owners:
            owner.current()
        self.busy = True
        for owner in self.owners:
            owner.cap.busy = True
        return 0

    def _leave(self):
        for owner in self.owners:
            owner.cap.busy = False
        self.busy = False

    def _prepare(self, fact, index, a=0, b=0, data=b'', capacity=0, incarnation=None):
        inc = (0 if self.receipt is None else self.receipt.incarnation) if incarnation is None else incarnation
        return fact.prepare(index, self.source.handle if index != 0 else 0, inc, a, b, data, capacity)

    def _code(self, fact):
        # May allocate. The opaque call record is already retained in fact.
        return fact.code()

    def _terminal(self, code, error=None):
        self.active, self.failed = False, True
        if not self.primary:
            self.primary = code
        if self.primary_error is None:
            self.primary_error = error
        self._retire()
        return code

    def _retire(self):
        if self.retirement_scheduled or self.receipt is None:
            return
        fact = self.calls['retire']
        try:
            token = fact.token if fact.token is not None else self._prepare(fact, 11)
        except BaseException as error:
            # Local preparation did not enter. Keep the diagnostic; independent
            # raw owner closes still run and do not invent Native retirement.
            fact.error = error
            return
        self.retirement_scheduled = True
        try:
            fact.enter(token)
        except BaseException as error:
            fact.error = error

    def _inspect(self):
        fact = self.calls['inspect']
        token = self._prepare(fact, 3, capacity=256)
        if not fact.enter(token):
            return self._terminal(self._code(fact))
        inspected = receipt(fact.bytes(), self.source.handle, self.kind, 2, self.expected_profiles)
        if not same_receipt(self.receipt, inspected):
            return self._terminal(13)
        return 0

    def _find(self, ticket):
        for row in self.history[:self.used]:
            if row.ticket == ticket and row.ticket != 0:
                return row
        return None

    def activate(self):
        code = self._enter(False)
        if code:
            return code
        try:
            if self.activation_scheduled:
                return 17
            abi = self.calls['abi']
            token = abi.prepare(0)
            abi.enter(token)
            if not version_one(token):
                return self._terminal(95)
            fact = self.calls['activate']
            token = self._prepare(fact, 1 if self.kind == CLIENT else 2, self.peer.handle, capacity=256, incarnation=0)
            self.activation_scheduled = True
            if not fact.enter(token):
                return self._terminal(self._code(fact))
            self.receipt = receipt(fact.bytes(), self.source.handle, self.kind, 1, self.expected_profiles)
            # Reserve Native retirement before admitting any user operation.
            # A successful endpoint need not allocate its C output/context
            # during close or while handling a later Python allocation failure.
            self._prepare(self.calls['retire'], 11)
            self.active = True
            return 0
        except BaseException as error:
            self._terminal(error.code if type(error) is WireError else 22, error)
            raise
        finally:
            self._leave()

    def _cancel(self, borrow, code):
        if self.cancel_borrow is borrow:
            return self._terminal(code)
        self.cancel_borrow = borrow
        fact = self.calls['cancel']
        token = self._prepare(fact, 9, borrow.sequence, borrow.reservation)
        # A cancellation outcome is never retried; the private borrow remains
        # retained if an interpreter interruption occurs around the C return.
        self.active = False
        try:
            if fact.enter(token):
                self.pending_borrow = self.pending_delivery = None
                self.active = not self.failed
            else:
                self._terminal(code)
        except BaseException as error:
            self._terminal(code, error)
            raise
        return code

    def take(self):
        result = Outcome()
        self.last_outcome = result
        code = self._enter()
        if code:
            result.status = code
            return result
        borrow = commit = None
        try:
            if self.kind == SERVICE and self.used == len(self.history):
                result.status = 90
                return result
            result.status = self._inspect()
            if result.status:
                return result
            fact = self.calls['peek']
            bound = self.response_bound if self.kind == CLIENT else self.request_bound
            token = self._prepare(fact, 7, 16, capacity=PREFIX + bound)
            if not fact.enter(token):
                result.status = self._code(fact)
                if result.status not in (11, 90, 95):
                    self._terminal(result.status)
                return result
            try:
                packet = delivery(fact.bytes(), self.receipt, self.peer.handle, bound, self.last_committed, self.last_reservation)
            except WireError as error:
                if error.borrow is not None:
                    borrow = self.pending_borrow = error.borrow
                    self.last_reservation = borrow.reservation
                    result.status = self._cancel(borrow, error.code)
                else:
                    result.status = self._terminal(error.code, error)
                return result
            borrow = self.pending_borrow = packet.borrow
            self.last_reservation = borrow.reservation
            row, value, discard, next_used = self._find(borrow.ticket), None, False, self.used
            if not packet.control:
                if self.kind == CLIENT:
                    if row is None or row.state not in (ADMITTED, ABANDONED):
                        result.status = self._cancel(borrow, 13)
                        self._terminal(13)
                        return result
                    discard = row.state == ABANDONED
                else:
                    if row is not None or any(old.native_sequence == packet.information.native_sequence
                            and old.writer == packet.information.writer for old in self.history[:self.used]):
                        result.status = self._cancel(borrow, 13)
                        self._terminal(13)
                        return result
                    row = self.history[self.used]
                    next_used = self.indices[self.used + 1]
                    row.state, row.ticket = RESERVED, borrow.ticket
                    row.native_sequence, row.writer = packet.information.native_sequence, packet.information.writer
                # ABANDONED responses are still decoded in full before disposal.
                value = (self.response_decode if self.kind == CLIENT else self.request_decode)(packet.body)
            complete = Delivery(borrow.ticket, borrow.sequence, packet.kind, packet.control,
                                packet.control_flags, packet.error, packet.information, value)
            self.pending_delivery = complete
            final_row_state = DISCARDED if discard else DONE if self.kind == CLIENT else DELIVERED
            terminal_code = packet.error or 125
            fact = self.calls['commit']
            commit = self._prepare(fact, 8, borrow.sequence, borrow.reservation)
            # Fail-closed BEFORE crossing ACK; success resumes using only rooted
            # values/scalar assignments. Unknown ACK cannot enter cancel/retry.
            self.active = False
            if not fact.enter(commit):
                result.status = self._terminal(self._code(fact))
                return result
            self.last_committed = borrow.sequence
            if not packet.control:
                row.state = final_row_state
                self.used = next_used
            elif row is not None and row.state == ADMITTED:
                row.state = DONE
            result.delivery = None if discard else complete
            result.discarded = discard
            self.pending_borrow = self.pending_delivery = None
            if packet.control:
                self.failed = True
                if not self.primary:
                    self.primary = terminal_code
            else:
                self.active = True
            return result
        except BaseException as error:
            result.error = error
            if commit is not None and attempted(commit):
                # Includes a known C return followed by a Python interruption.
                # Its exact outcome remains in calls['commit']; never cancel it.
                self._terminal(22, error)
            elif borrow is not None and self.cancel_borrow is not borrow:
                self._cancel(borrow, 90 if isinstance(error, MemoryError) else 22)
            else:
                self._terminal(22, error)
            raise
        finally:
            self._leave()

    def close(self):
        if self.busy:
            return 17
        # Endpoint reentry exclusion covers Native retirement AND all raw closes.
        # Capability.busy covers only the Native phase, and is released before
        # genuine Capability.close (which correctly refuses a busy owner).
        self.busy = True
        try:
            if not self.closed:
                if any(owner.cap.busy or owner.session.retiring for owner in self.owners):
                    return 17
                for owner in self.owners:
                    owner.cap.busy = True
                try:
                    self.active, self.failed, self.closed = False, True, True
                    self._retire()
                finally:
                    for owner in self.owners:
                        owner.cap.busy = False
            if self.own_capabilities:
                for owner in self.reverse_owners:
                    if owner.finished:
                        continue
                    try:
                        if not owner.cap.handle:
                            owner.external, owner.finished, owner.retained = True, True, False
                            continue
                        if owner.cap.session is not owner.session or owner.cap.handle != owner.handle:
                            raise ValueError('raw owner identity changed before cleanup')
                        owner.cleanup = owner.closer()
                    except BaseException as error:
                        owner.error = error
                    finally:
                        owner.attempted = not owner.external and (owner.cap._close_attempt[0] or owner.cap._revoke_attempt[0])
                        owner.retained = bool(owner.cap.handle)
                        owner.finished = not owner.retained
            fact = self.calls['retire']
            if fact.error is not None:
                return 22
            if fact.token is not None and not success(fact.token):
                return fact.code() if returned(fact.token) else 22
            for owner in self.reverse_owners:
                if self.own_capabilities:
                    if owner.error is not None:
                        return 22
                    if owner.retained:
                        return 17
                    if owner.cleanup is not None and owner.cleanup.error:
                        return owner.cleanup.error
            activation = self.calls['activate'].token
            if activation is not None and attempted(activation) and self.receipt is None:
                # Even successful raw owner cleanup cannot invent a validated
                # Native-incarnation retirement observation after bad/unknown
                # activation. Borrowed raw owners especially remain explicit.
                return 22
            return 0
        finally:
            self.busy = False

    def facts(self):
        """Observation may allocate; no private ticket/writer buffer is mutable."""
        return {'kind': self.kind, 'native_type': self.definition.native_type,
                'codec_identity': self.definition.codec_identity, 'primary': self.primary,
                'primary_error': self.primary_error, 'active': self.active, 'closed': self.closed,
                'activation_scheduled': self.activation_scheduled,
                'retirement_scheduled': self.retirement_scheduled,
                'capability_ownership': 'owned' if self.own_capabilities else 'borrowed',
                'borrowed_owner_cleanup_required': not self.own_capabilities,
                'history_used': self.used, 'last_committed': self.last_committed,
                'last_reservation': self.last_reservation, 'last_outcome': self.last_outcome,
                'pending_borrow': self.pending_borrow, 'pending_delivery': self.pending_delivery,
                'calls': {name: fact.snapshot() for name, fact in self.calls.items()},
                'owners': tuple({'handle': row.handle, 'attempted': row.attempted,
                                 'retained': row.retained, 'external': row.external,
                                 'finished': row.finished, 'result': row.cleanup,
                                 'error': row.error} for row in self.owners),
                'history': tuple({'state': row.state, 'ticket': row.ticket,
                                  'native_sequence': row.native_sequence, 'writer': row.writer,
                                  'submit': row.submit.snapshot(), 'abandon': row.abandon.snapshot(),
                                  'respond': row.respond.snapshot()} for row in self.history[:self.used])}


class OwnedClient(Endpoint):
    def __init__(self, call, observer, definition, **options):
        super().__init__(CLIENT, call, observer, definition, **options)

    def submit(self, request):
        result = Outcome()
        self.last_outcome = result
        code = self._enter()
        if code:
            result.status = code
            return result
        try:
            if self.used == len(self.history):
                result.status = 90
                return result
            data = self.request_encode(request)
            if type(data) is not bytes or len(data) > self.request_bound:
                raise WireError('complete bounded immutable request required', 90)
            result.status = self._inspect()
            if result.status:
                return result
            row = self.history[self.used]
            next_used = self.indices[self.used + 1]
            token = self._prepare(row.submit, 4, data=data, capacity=8)
            row.state, self.used = RESERVED, next_used
            self.active = False
            if not row.submit.enter(token):
                result.status = row.submit.code()
                if result.status == 11:
                    self.active = True
                else:
                    self._terminal(result.status)
                return result
            ticket = number(row.submit.bytes(), 0)
            if ticket == 0 or self._find(ticket) is not None:
                result.status = self._terminal(13)
                return result
            row.ticket, row.state, result.ticket = ticket, ADMITTED, ticket
            self.active = True
            return result
        except BaseException as error:
            result.error = error
            self._terminal(22, error)
            raise
        finally:
            self._leave()

    def abandon(self, ticket):
        uint(ticket)
        code = self._enter()
        if code:
            return code
        try:
            row = self._find(ticket)
            # Actual Native transport accepts ADMITTED only. A repeated local
            # abandon never re-enters raw5, including a prior nonzero/throw.
            if row is None or row.state != ADMITTED:
                return 9
            code = self._inspect()
            if code:
                return code
            token = self._prepare(row.abandon, 5, ticket)
            row.state, self.active = ABANDONED, False
            if not row.abandon.enter(token):
                return self._terminal(row.abandon.code())
            self.active = True
            return 0
        except BaseException as error:
            self._terminal(22, error)
            raise
        finally:
            self._leave()

    def available(self, wait_nanoseconds=0):
        uint(wait_nanoseconds, MAX_WAIT)
        result = Outcome()
        self.last_outcome = result
        code = self._enter()
        if code:
            result.status = code
            return result
        try:
            if self.query is None:
                result.status = 13
                return result
            result.status = self._inspect()
            if result.status:
                return result
            fact = self.calls['available']
            token = self._prepare(fact, 10, self.query.handle, wait_nanoseconds, capacity=1)
            if not fact.enter(token):
                result.status = fact.code()
                return result
            data = fact.bytes()
            if data not in (b'\0', b'\1'):
                result.status = self._terminal(22)
            else:
                result.available = data == b'\1'
            return result
        except BaseException as error:
            result.error = error
            self._terminal(22, error)
            raise
        finally:
            self._leave()


class OwnedServer(Endpoint):
    def __init__(self, advertise, responder, definition, **options):
        super().__init__(SERVICE, advertise, responder, definition, **options)

    def respond(self, ticket, response):
        uint(ticket)
        code = self._enter()
        if code:
            return code
        try:
            row = self._find(ticket)
            if row is None or row.state != DELIVERED:
                return 9
            data = self.response_encode(response)
            if type(data) is not bytes or len(data) > self.response_bound:
                raise WireError('complete bounded immutable response required', 90)
            code = self._inspect()
            if code:
                return code
            token = self._prepare(row.respond, 6, self.peer.handle, ticket, data=data)
            row.state, self.active = DONE, False
            if not row.respond.enter(token):
                return self._terminal(row.respond.code())
            self.active = True
            return 0
        except BaseException as error:
            self._terminal(22, error)
            raise
        finally:
            self._leave()
