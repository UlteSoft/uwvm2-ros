"""rclpy context lifecycle over an explicitly installed guest binding profile.

Binding a symbolic endpoint to an alias is guest-local configuration. It does
not create a Runtime grant or modify host policy, debug settings or updates.
An ordinary legacy application can retain its imports/API when a deployment
bootstrap installs this profile before importing its main module.
"""
from dataclasses import dataclass
from roswasm import Backend, Session, Result, RosError
from .qos import checked


@dataclass(frozen=True)
class AliasBinding:
    """Exact operation/resource/type/QoS mapping to one independently granted alias.

    The dictionary key is (operation, fully_qualified_resource, message_type).
    For clock/logger use the node's fully qualified name and empty type.
    capacity is a guest allocation bound; it cannot raise any host quota.
    """
    alias: str
    qos: object = None
    capacity: int = 65536


class _ShutdownFailures:
    """Failure-only view over already-reserved owner/callback diagnostic slots."""
    __slots__ = ('slots',)

    def __init__(self, slots):
        self.slots = slots

    def __iter__(self):
        for value in self.slots:
            if value is not None:
                yield value

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


class Context:
    """One ROS2 guest context; no ambient ROS domain or native node is created."""
    def __init__(self):
        self.session = None
        self.bindings = {}
        self.idle_wait = None
        self.nodes = []
        self.callbacks = []
        self._initialized = False
        self._dispatching = False
        self._remaining_credits = 4096
        self.prepare_callback_logging = False
        self.last_shutdown_cleanup = None

    def configure(self, bindings, *, idle_wait=None, maximum_owners=256,
                  maximum_credits=4096, prepare_callback_logging=False):
        """Install local aliases before init; all host grants remain independent.

        idle_wait is an explicit deployment driver for blocking spin, e.g. a
        bounded supervisor credit read from existing stdin. It must not be
        confused with an implicit ROS wait-set or native executor equivalence.
        """
        if self._initialized:
            raise RuntimeError('configure before rclpy.init')
        for key, binding in bindings.items():
            if (not isinstance(key, tuple) or len(key) != 3 or
                    not all(isinstance(part, str) for part in key) or
                    not isinstance(binding, AliasBinding) or
                    type(binding.capacity) is not int or not 0 <= binding.capacity <= 4194304):
                raise ValueError('exact binding key and bounded AliasBinding required')
        if idle_wait is not None and not callable(idle_wait):
            raise TypeError('idle_wait must be callable')
        if type(maximum_owners) is not int or not 0 <= maximum_owners <= 65536:
            raise ValueError('owner bound')
        if type(maximum_credits) is not int or not 0 <= maximum_credits <= 65536:
            raise ValueError('finite deployment credit bound required')
        if type(prepare_callback_logging) is not bool:
            raise TypeError('explicit prepare_callback_logging bool required')
        self.prepare_callback_logging = prepare_callback_logging
        self._remaining_credits = maximum_credits
        self.bindings = dict(bindings)
        self.idle_wait, self.maximum_owners = idle_wait, maximum_owners

    def init(self, args=None, *, initialize_logging=True, domain_id=None):
        """Initialize locally and check actual imports; grant no ROS operation."""
        if self._initialized:
            raise RuntimeError('Context.init may be called only once')
        if args not in (None, [], ()) or domain_id is not None:
            raise NotImplementedError('ROS remapping/domain selection must be represented in the deployment binding profile')
        if type(initialize_logging) is not bool:
            raise TypeError('initialize_logging')
        pending = Session(Backend(2), getattr(self, 'maximum_owners', 256))
        pending.check_abi()
        self.session = pending
        self._initialized = True

    def ok(self):
        return self.session is not None and self.session.active

    def next_credit(self):
        """Explicit finite scheduling credit; no availability/time inference."""
        if not self.ok():
            raise RuntimeError('context is not live')
        if self._dispatching:
            raise RosError(Result(17))
        if self.idle_wait is None:
            raise NotImplementedError('explicit finite deployment driver required')
        if not self._remaining_credits:
            return False
        self._remaining_credits -= 1
        return self.idle_wait() is not False

    def shutdown(self):
        """Reserve all failure/callback storage first, then attempt every callback."""
        if not self.ok():
            raise RuntimeError('context is not initialized or already shut down')
        if self._dispatching:
            raise RosError(Result(17))
        callbacks = tuple(self.callbacks)
        owner_count = len(self.session.owners)
        clients = tuple(client for node in self.nodes for client in node.clients)
        client_work = iter(tuple((client, client._finish_closed, owner_count + index)
                                 for index, client in enumerate(clients)))
        owner_indices = iter(tuple(range(owner_count)))
        callback_work = iter(tuple((callback, owner_count + len(clients) + index)
                                   for index, callback in enumerate(callbacks)))
        failures = [None] * (owner_count + len(clients) + len(callbacks))
        error = RuntimeError('rclpy shutdown had cleanup/callback failures')
        error.roswasm_failures = _ShutdownFailures(failures)
        # Retain recovery ownership even if close fails before native entry and
        # the default Context is reset. Allocate this diagnostic before effects.
        error.roswasm_owner_ledger = self.session.owners
        # Publish already-reserved diagnostic storage before effects; no list
        # append/tuple conversion is needed after a close/callback failure.
        self.last_shutdown_cleanup = error
        report = self.session.close()
        if (len(report.outcomes) == 1 and getattr(report.outcomes[0], 'error', None) == 17
                and self.session.active):
            raise RosError(report.outcomes[0])
        # Native/session owners are now retired. Every pending local Future
        # is marked125 using its preexisting storage; this claims no native
        # cancellation and cannot grant callbacks a new live Context.
        bad = False
        for client, finish, index in client_work:
            try:
                client._retired = True
                finish()
            except BaseException as failure:
                failures[index] = failure
                bad = True
        for index in owner_indices:
            value = report.outcomes[index]
            if value is None or isinstance(value, BaseException) or value.error:
                failures[index] = value if value is not None else error
                bad = True
        for callback, index in callback_work:
            try:
                callback()
            except BaseException as failure:
                failures[index] = failure
                bad = True
        if bad:
            raise error

    def try_shutdown(self):
        if self.ok():
            self.shutdown()

    def on_shutdown(self, callback):
        if not callable(callback):
            raise TypeError('callback required')
        if self._initialized and not self.ok():
            callback()
        else:
            self.callbacks.append(callback)

    def binding(self, operation, resource, message_type='', qos=None):
        """Require an exact configured binding; absence fails closed with13."""
        binding = self.bindings.get((operation, resource, message_type))
        if binding is None:
            raise RosError(Result(13))
        if qos is not None and (binding.qos is None or checked(binding.qos).key() != checked(qos).key()):
            raise RosError(Result(17))
        return binding
