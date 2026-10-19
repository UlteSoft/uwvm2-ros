"""Whole-context leases for the explicit single-threaded migration scheduler.

Allocate the complete release work before changing a flag or polling a native
source. Only that source's own owners are exempt: its real SDK poller sets their
busy flags itself. All sibling owners are borrowed for the entire poll/decode/
callback pass. This is not a native wait-set or a new Host permission.
"""
from roswasm import Executor, Result, RosError


class DispatchLease:
    def __init__(self, context, exempt=()):
        if context._dispatching:
            raise RosError(Result(17))
        owners = tuple(context.session.owners)
        if any(owner.busy for owner in owners):
            raise RosError(Result(17))
        self.context = context
        self.work = tuple(owner for owner in owners if all(owner is not item for item in exempt))
        self.restore = iter(self.work)
        self.active = False

    def __enter__(self):
        if self.context._dispatching:
            raise RosError(Result(17))
        try:
            self.context._dispatching = self.active = True
            for owner in self.work:
                owner.busy = True
        except BaseException:
            self.__exit__(None, None, None)
            raise
        return self

    def __exit__(self, kind, error, trace):
        if self.active:
            for owner in self.restore:
                owner.busy = False
            self.context._dispatching = self.active = False
        return False


class ContextExecutor(Executor):
    """Fixed bounded round-robin polling with a lease shared by every Node."""
    def __init__(self, context, maximum_sources):
        super().__init__(maximum_sources)
        self.context = context

    def spin_once(self):
        if self.spinning or self.context._dispatching:
            raise RosError(Result(17))
        try:
            self.spinning = True
            for _ in range(len(self.sources)):
                source = self.sources[self.position]
                self.position = (self.position + 1) % len(self.sources)
                own = getattr(source, '_dispatch_owners', None)
                # Existing Subscription and Timer have one actual owner/clock.
                if own is None:
                    owner = getattr(source, 'owner', None)
                    if owner is None:
                        owner = None  # Timer reads its Clock without borrowing that capability; lease it too.
                    own = () if owner is None else (owner,)
                with DispatchLease(self.context, own):
                    result = source.poll_once()
                if result.error != 11:
                    return result
            return Result(11)
        finally:
            self.spinning = False
