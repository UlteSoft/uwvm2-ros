"""Real caller-driven TypeQuery and parameter applications; no scheduler/relay.

Retain the application BEFORE open(). Its fixed owner/endpoint/error slots keep
partial setup and every cleanup outcome observable even when open raises.
Deployment supplies full generated components, exact aliases and both QoS rows.
"""
from roswasm.client import Session, _factory_failure
from roswasm.owned_service import OwnedClient, OwnedServer


class CommandFailure:
    """Reserved sticky driver failure, independent of cleanup observations.

    The first failed command ends this driver lifetime; it is never retried.
    A take11 with no delivery/error is explicitly an expected empty poll. All
    other nonzero statuses, actual controls and raised objects remain failures.
    Retained objects are references, not a claim that diagnostics never allocate.
    """
    __slots__ = ('failed', 'operation', 'service', 'status', 'result', 'exception',
                 'cleanup_status', 'cleanup_error', 'shutdown_error')

    def __init__(self):
        self.failed = False
        self.operation = self.service = self.result = self.exception = None
        self.status = self.cleanup_status = 0
        self.cleanup_error = self.shutdown_error = None

    def record(self, operation, service, status, result):
        error = getattr(result, 'error', None)
        item = getattr(result, 'delivery', None)
        control = operation == 'take' and item is not None and item.control
        if control:
            status = item.error if item.error != 0 else 125
        idle = operation == 'take' and status == 11 and item is None and error is None
        if not self.failed and (error is not None or control or status != 0 and not idle):
            self.failed = True
            self.operation, self.service, self.status = operation, service, status
            self.result, self.exception = result, error


class Application:
    """Explicit once-open lifetime, bounded by the selected endpoint count."""
    def __init__(self, session, plans, *, server=False, history_entries=64):
        if type(session) is not Session or type(plans) is not tuple or not plans:
            raise TypeError('real Session and explicit finite endpoint plans required')
        self.session, self.plans, self.server = session, plans, server
        self.history_entries = history_entries
        self.endpoints = [None] * len(plans)
        self.close_outcomes = [None] * len(plans)
        self.close_errors = [None] * len(plans)
        self.closed = self.open_scheduled = self.busy = False
        self.primary = self.factory = None
        self.failure_cleanup = None
        self.command_failure = CommandFailure()
        self.reverse_indices = tuple(range(len(plans) - 1, -1, -1))

    def open(self):
        if self.open_scheduled or self.closed or self.busy:
            raise RuntimeError('application lifetime is once-open and nonreentrant')
        # The actual Session factory reserves all pending raw owner slots before
        # the FIRST open, including owners whose endpoint allocation later fails.
        frame, owns = self.session._begin_factory(sum(2 + (plan.get('availability') is not None) for plan in self.plans))
        self.factory, self.open_scheduled, self.busy = frame, True, True
        try:
            for index, plan in enumerate(self.plans):
                source = self.session.open(plan['source'], owned_capture=True)
                peer = self.session.open(plan['peer'], owned_capture=True)
                query = None if plan.get('availability') is None else self.session.open(plan['availability'], owned_capture=True)
                endpoint = (OwnedServer if self.server else OwnedClient)(source, peer, plan['definition'],
                    expected_profiles=plan['qos'], history_entries=self.history_entries,
                    ownership='owned', availability=query)
                self.endpoints[index] = endpoint
                code = endpoint.activate()
                if code:
                    raise RuntimeError('activation refused with retained endpoint status', code)
            frame.committed = True
            return self
        except BaseException as error:
            self.primary = error
            # Native retirement precedes all actual raw cleanup. Endpoint slots
            # were installed before activation; pending opens remain in frame.
            self._cleanup_endpoints()
            _factory_failure(frame, error)
            self.failure_cleanup = frame.outcomes
            raise
        finally:
            self.session._end_factory(frame, owns)
            self.busy = False

    def _cleanup_endpoints(self):
        for index in self.reverse_indices:
            endpoint = self.endpoints[index]
            if endpoint is not None:
                try:
                    self.close_outcomes[index] = endpoint.close()
                except BaseException as error:
                    self.close_errors[index] = error

    def close(self):
        if self.busy:
            return 17
        self.busy = True
        try:
            self._cleanup_endpoints()
            self.closed = True
            for error in self.close_errors:
                if error is not None:
                    return 22
            for code in self.close_outcomes:
                if code is not None and code != 0:
                    return code
            if self.factory is not None and self.factory.failed():
                return 22
            if self.factory is not None:
                for owner in self.factory.owners:
                    capture = None if owner is None else getattr(owner, '_open_capture', None)
                    if capture is not None and (capture.cleanup_failed() or capture.pending()):
                        return 22
            return 0
        finally:
            self.busy = False

    def facts(self):
        return {'primary': self.primary, 'factory': self.factory,
                'failure_cleanup': self.failure_cleanup, 'closed': self.closed,
                'command_failure': self.command_failure,
                'open_captures': () if self.factory is None else tuple(
                    None if owner is None else getattr(owner, '_open_capture', None)
                    for owner in self.factory.owners),
                'close_outcomes': tuple(self.close_outcomes), 'close_errors': tuple(self.close_errors),
                'endpoints': tuple(None if ep is None else ep.facts() for ep in self.endpoints)}


class TypeQueryApplication(Application):
    """One original GetTypeDescription service, including all returned sources."""
    def query(self, full_request):
        if self.server:
            raise RuntimeError('client role required')
        return self.endpoints[0].submit(full_request)

    def step(self):
        return self.endpoints[0].take()

    def respond(self, ticket, full_response):
        if not self.server:
            raise RuntimeError('server role required')
        return self.endpoints[0].respond(ticket, full_response)


class ParametersApplication(Application):
    """Six separately granted original parameter services; no legacy RPC path.

    plans are in Describe/GetTypes/Get/List/Set/SetAtomically order. Each request
    and response is its complete generated dataclass, not a subset dictionary.
    """
    def __init__(self, session, plans, **options):
        if len(plans) != 6:
            raise ValueError('all six original parameter service plans required')
        super().__init__(session, plans, **options)

    def submit(self, operation, full_request):
        if self.server or type(operation) is not int or not 0 <= operation < 6:
            raise ValueError('client operation index0..5 required')
        return self.endpoints[operation].submit(full_request)

    def step(self, operation):
        if type(operation) is not int or not 0 <= operation < 6:
            raise ValueError('operation index0..5 required')
        return self.endpoints[operation].take()

    def respond(self, operation, ticket, full_response):
        if not self.server or type(operation) is not int or not 0 <= operation < 6:
            raise ValueError('server operation index0..5 required')
        return self.endpoints[operation].respond(ticket, full_response)
