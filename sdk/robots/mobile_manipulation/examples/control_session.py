"""Callable Guest lesson, using real SDK owners and actual generated codecs.

An operator-selected binding supplies complete Goal/Feedback/Result/JointState
codecs and a complete typed goal. No generated product or native identity is
embedded in this source. Source-only until the new vendor IR product is frozen.
"""
from roswasm import Node
from ..trajectory_client import close_owned_reverse
from ..kinova.sdk import Gen3Client
from ..stretch.sdk import StretchClient


def run(session, binding, read_credit):
    """Telemetry -> four independently opened Action roles -> explicit command.

    Every data import needs its own actual caller credit (T/S/F/R/C). Credits
    bound work; they are not nanoseconds, a clock, native readiness or a grant.
    No credit or alias is inferred from model selection. Source quota/history,
    native full graph and adapter proof remain the Host/Gateway authority.
    """
    required = ('telemetry', 'send', 'feedback', 'result', 'cancel')
    if (set(binding.aliases) != set(required) or
            len(set(binding.aliases.values())) != len(required)):
        raise ValueError('five distinct explicit aliases required')
    owners, outcomes = [None] * 4, [None] * 4
    telemetry, client, primary = None, None, None
    observation = [None]
    local_retirement = None
    try:
        telemetry = Node(session).create_subscription(
            binding.joint_state_codec, binding.aliases['telemetry'],
            lambda value: observation.__setitem__(0, value),
            binding.joint_state_codec.MAX_WIRE_BYTES)
        if read_credit() != b'T':
            raise ValueError('one real telemetry credit required')
        telemetry.poll_once().checked()
        if observation[0] is None:
            raise RuntimeError('no actual complete JointState; do not command')
        # This is the Runtime owner lease, distinct from a native Spot body
        # lease or the native Kortex driver session. Do not fabricate either.
        for index, role in enumerate(required[1:]):
            owners[index] = session.open(binding.aliases[role])
        arguments = (*owners, binding.goal_codec, binding.feedback_codec, binding.result_codec)
        if binding.vendor == 'kinova':
            client = Gen3Client(*arguments, model=binding.model, maximum_goals=1)
        elif binding.vendor == 'stretch':
            client = StretchClient(*arguments, model=binding.model, mode=binding.mode, maximum_goals=1)
        else:
            raise ValueError('explicit selected vendor required')
        if read_credit() != b'S':
            raise ValueError('one explicit send credit required')
        goal = client.submit(binding.complete_goal)
        for _ in range(64):
            operation = read_credit()
            if operation == b'C':
                client.cancel(goal)
            elif operation in (b'F', b'R'):
                response, value = client.receive(goal, terminal=operation == b'R')
                if response.error not in (0, 11):
                    response.checked()
                if value is not None and operation == b'R':
                    return value
            else:
                raise ValueError('only explicit feedback/result/cancel credits admitted')
        raise RuntimeError('finite credit history exhausted; outcome may remain unknown')
    except BaseException as error:
        primary = error
        raise
    finally:
        if client is not None:
            try:
                client.retire_local()
            except BaseException as error:
                local_retirement = error
        close_owned_reverse(owners, outcomes)
        telemetry_cleanup = None
        if telemetry is not None:
            try:
                telemetry_cleanup = telemetry.close()
            except BaseException as error:
                telemetry_cleanup = error
        failures = [value for value in (*outcomes, telemetry_cleanup, local_retirement)
                    if isinstance(value, BaseException) or (value is not None and value.error)]
        if primary is not None:
            primary.mobile_manipulation_cleanup = (outcomes, telemetry_cleanup, local_retirement)
        elif failures:
            raise RuntimeError(outcomes, telemetry_cleanup, local_retirement)
