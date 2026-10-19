"""Real CPython/WASI JointState Guest; SOURCE_ONLY_UNEXECUTED.

Freeze actual bounded_records.py and one exact mode source as joint_state_mode.
No host Python backend, generated-code replacement, grant, timer or retry.
"""
import sys
import bounded_records as records
from joint_state_mode import MODE
from _roswasm_native import backend as selected_backend
from roswasm import Backend, Session, Node
from roswasm.raw import RosError


# Independently retained diagnostics; a failed import/close is never replayed.
last_primary = None
last_cleanup = None
last_factory_cleanup = None
last_session = None
received_count = 0
denied_verified = False


def observe(value):
    global received_count
    header = value.header
    if selected_backend == 1:
        header_ok = (header.seq == 19 and header.stamp.sec == 42
                     and header.stamp.nsec == 300000000)
    else:
        header_ok = (header.stamp.sec == -2
                     and header.stamp.nanosec == 300000000)
    if not (header_ok and header.frame_id == 'base'
            and len(value.name) == 2
            and value.name[0] == 'left' and value.name[1] == 'right'
            and len(value.position) == 2
            and value.position[0] == 1.25 and value.position[1] == -2.5
            and len(value.velocity) == 0
            and len(value.effort) == 2
            and value.effort[0] == 0.5 and value.effort[1] == -0.25):
        raise ValueError('complete JointState differs from observer-fixture.json')
    received_count += 1


def main():
    global last_primary, last_cleanup, last_factory_cleanup, last_session, denied_verified
    # One command invocation per fresh interpreter/Instance is mandatory.
    if MODE not in (0, 1) or type(MODE) is not int or selected_backend not in (1, 2):
        raise ValueError('explicit compile/freeze mode and actual native backend')
    symbol = ('Ros1Sensor_msgsJointState' if selected_backend == 1
              else 'Ros2Sensor_msgsMsgJointState')
    codec = getattr(records, symbol)  # actual complete generated class, not a stub
    if (codec.ROS_VERSION != selected_backend
            or codec.NATIVE_TYPE != ('sensor_msgs/JointState' if selected_backend == 1
                                     else 'sensor_msgs/msg/JointState')
            or type(codec.MAX_WIRE_BYTES) is not int
            or not 1 <= codec.MAX_WIRE_BYTES <= 32768):
        raise ValueError('actual complete generated codec and bound required')
    session = Session(Backend(selected_backend), maximum_owners=1)
    last_session = session  # retain factory/owner diagnostics before any open
    primary = None
    try:
        session.check_abi()
        if MODE == 0:
            try:
                session.open('joint-state')
            except RosError as error:
                if error.error != 13 or error.result.value is not None:
                    raise
                if any(owner.handle for owner in session.owners):
                    raise ValueError('denied open retained a live owner')
                denied_verified = True
            else:
                raise ValueError('default-denied open unexpectedly succeeded')
        else:
            subscription = Node(session).create_subscription(
                codec, 'joint-state', observe, codec.MAX_WIRE_BYTES)
            for _ in range(64):
                # Every actual poll consumes one caller-owned binary R credit.
                if sys.stdin.buffer.read(1) != b'R':
                    raise ValueError('one actual R byte per bounded poll required')
                result = subscription.poll_once()
                if result.error not in (0, 11):
                    result.checked()
                if received_count:
                    break
            if received_count != 1:
                raise RuntimeError('no one complete value within64 credits')
    except BaseException as error:
        primary = last_primary = error
    finally:
        last_factory_cleanup = session.last_factory_cleanup
        try:
            last_cleanup = session.close(primary)
        except BaseException as cleanup:
            # Preserve primary and the independent cleanup exception.
            last_cleanup = cleanup
            if primary is None:
                primary = cleanup
    if primary is not None:
        raise primary
    failures = last_cleanup.failures
    if failures:
        first = failures[0]
        if isinstance(first, BaseException):
            raise first
        if first is None:
            raise RuntimeError('incomplete cleanup: no success marker')
        raise RosError(first)
    if any(owner.handle for owner in session.owners):
        raise RuntimeError('known unattempted owner remains; no automatic retry')
    if (MODE == 0 and (not denied_verified or received_count != 0)):
        raise ValueError('default denial oracle')
    # Cleanup returned successfully before any success marker is written.
    sys.stdout.write('PASS_JOINT_STATE_DEFAULT_DENIED\n' if MODE == 0
                     else 'PASS_JOINT_STATE_FULL_VALUE\n')
    sys.stdout.flush()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
