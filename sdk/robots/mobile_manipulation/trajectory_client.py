"""Typed, bounded FollowJointTrajectory client over actual roswasm owners.

No alias is opened here. All four owners are borrowed from one real Session;
their original owner must close them independently, even after a failed close.
The supplied codecs come from the complete selected vendor-profile catalog.
This module does not invent native identities or remove standard fields.
"""
from dataclasses import dataclass
from roswasm.raw import Result, RosError


class _Scope:
    """One preallocated lexical lease for all four actual borrowed owners."""
    def __init__(self, owners):
        self.owners = owners

    def __enter__(self):
        session = self.owners[0].session
        if session.retiring or any(owner.busy for owner in self.owners):
            raise RosError(Result(17))
        if not session.active:
            raise RosError(Result(125))
        if any(not owner.handle for owner in self.owners):
            raise RosError(Result(9))
        for owner in self.owners:
            owner.busy = True
        return self

    def __exit__(self, kind, value, traceback):
        for owner in self.owners:
            owner.busy = False


def _scoped(method):
    def call(self, *args, **kwargs):
        with self._scope:
            return method(self, *args, **kwargs)
    return call


@dataclass
class Goal:
    """One never-reused local history row; native ID is a separate full u64."""
    index: int
    identity: int = 0
    state: str = 'unused'
    primary: object = None
    value: object = None


class TrajectoryClient:
    def __init__(self, send, feedback, result, cancel, goal_codec,
                 feedback_codec, result_codec, maximum_goals=32):
        if type(maximum_goals) is not int or not 1 <= maximum_goals <= 256:
            raise ValueError('history must be 1..256')
        owners = (send, feedback, result, cancel)
        if any(owner is None or owner.session is not send.session for owner in owners):
            raise ValueError('four explicit owners from the same real Session required')
        if send.session.backend.version != 2:
            raise RosError(Result(95))
        if len({owner.handle for owner in owners}) != 4:
            raise ValueError('independent Action roles required')
        for codec in (goal_codec, feedback_codec, result_codec):
            maximum = codec.MAX_WIRE_BYTES
            if type(maximum) is not int or not 0 <= maximum <= 4193786:
                raise ValueError('actual bounded complete codec required')
            if not callable(codec.encode) or not callable(codec.decode):
                raise ValueError('typed encode/decode required')
        self.owners, self.codecs = owners, (goal_codec, feedback_codec, result_codec)
        self._scope = _Scope(owners)
        self.rows = tuple(Goal(index) for index in range(maximum_goals))
        self.used, self.closed = 0, False

    def _live(self, goal):
        if self.closed:
            raise RosError(Result(125))
        if (not isinstance(goal, Goal) or not 0 <= goal.index < self.used or
                self.rows[goal.index] is not goal or goal.identity == 0):
            raise ValueError('foreign or unsubmitted goal')
        if goal.state not in ('accepted', 'cancel-requested'):
            raise RosError(Result(125))

    @_scoped
    def submit(self, complete_goal):
        if self.closed:
            raise RosError(Result(125))
        if self.used == len(self.rows):
            raise RosError(Result(90))
        # Validate the WHOLE Goal, including multi-DOF and tolerance arrays,
        # before any import. No positions-only projection is used on the wire.
        wire = self.codecs[0].encode(complete_goal)
        if not isinstance(wire, bytes) or len(wire) > self.codecs[0].MAX_WIRE_BYTES:
            raise ValueError('codec exceeded selected canonical bound')
        row = self.rows[self.used]
        self.used += 1
        row.state = 'unknown'
        try:
            reply = self.owners[0].invoke('action_send_goal', wire, 8).checked().data
            if len(reply) != 8:
                raise ValueError('send reply extent')
            identity = int.from_bytes(reply, 'little')
            if not identity or any(old.identity == identity for old in self.rows[:row.index]):
                raise ValueError('zero or reused native goal ID')
            row.identity, row.state = identity, 'accepted'
            return row
        except BaseException as error:
            row.primary = error
            # The reserved row remains. A lost reply must never be replayed.
            raise

    @_scoped
    def cancel(self, goal):
        self._live(goal)
        try:
            response = self.owners[3].invoke(
                'action_cancel_own_goal', goal.identity.to_bytes(8, 'little'), 1).checked().data
            if len(response) != 1 or response[0] not in (0, 1):
                raise ValueError('cancel reply must be one canonical bool')
            # Acceptance is a protocol response, never a physical stop receipt.
            if response[0]:
                goal.state = 'cancel-requested'
            return bool(response[0])
        except BaseException as error:
            goal.primary, goal.state = error, 'unknown'
            raise

    @_scoped
    def receive(self, goal, *, terminal=False):
        self._live(goal)
        offset = 9 if terminal else 8
        codec = self.codecs[2 if terminal else 1]
        owner = self.owners[2 if terminal else 1]
        operation = 'action_receive_result' if terminal else 'action_receive_feedback'
        try:
            # The real invoke may consume a Native record before allocating
            # its returned Result. A raised BaseException is therefore an
            # unknown receive outcome, just like a lost non-idle reply.
            response = owner.invoke(operation, goal.identity.to_bytes(8, 'little'),
                                    codec.MAX_WIRE_BYTES + offset)
            if response.error:
                # Only an actual returned idle11 keeps this row usable. An
                # exception must never be reinterpreted as an idle result.
                if response.error != 11:
                    goal.primary, goal.state = RosError(response), 'unknown'
                return response, None
            data = response.data
            if len(data) < offset or int.from_bytes(data[:8], 'little') != goal.identity:
                raise ValueError('feedback/result correlation mismatch')
            if terminal and data[8] not in (2, 3, 4):
                raise ValueError('nonterminal result status')
            value = codec.decode(data[offset:])
            if terminal:
                goal.state, goal.value = 'terminal', value
            return response, value
        except BaseException as error:
            goal.primary, goal.state = error, 'unknown'
            raise

    @_scoped
    def retire_local(self):
        """Refuse new work. The caller still owns and must close all four owners."""
        self.closed = True


def close_owned_reverse(owners, outcomes):
    """Caller-owned cleanup. Slots must exist before opening any raw owner.

    This never stops a robot or issues cancellation under an ungranted alias.
    It attempts every actually acquired owner independently and keeps results.
    """
    if len(outcomes) != len(owners):
        raise ValueError('preallocated cleanup slots required')
    for index in range(len(owners) - 1, -1, -1):
        if owners[index] is not None:
            try:
                outcomes[index] = owners[index].close()
            except BaseException as error:
                outcomes[index] = error
