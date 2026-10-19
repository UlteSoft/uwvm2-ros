"""Real ROS2 SDK + optional generated JointState, requiring trusted bindings.

Generate the actual ROS2 Header/JointState preparation/bound catalog into a
fresh revision, include bounded_records in the selected interpreter's frozen
modules, and give two explicit aliases to this separately owned command. A
source-generation receipt alone does not qualify native identity or delivery.
"""
import sys
from roswasm import Backend, Session, Node
from bounded_records import Ros2Sensor_msgsMsgJointState as JointState


def main():
    if len(sys.argv) != 3:
        raise ValueError('publish-alias and subscribe-alias required')
    seen = []
    def receive(value):
        if (value.header.stamp.sec != 1 or value.header.stamp.nanosec != 2 or
                value.header.frame_id != 'managed' or value.name != ['joint1'] or
                value.position != [1.25] or value.velocity != [0.5] or value.effort != [-0.75]):
            raise ValueError('complete native/Guest value differs')
        seen.append(value)
    with Session(Backend(2), maximum_owners=2) as session:
        node = Node(session)
        subscription = node.create_subscription(JointState, sys.argv[2], receive, JointState.MAX_WIRE_BYTES)
        publisher = node.create_publisher(JointState, sys.argv[1])
        value = JointState()
        value.header.stamp.sec = 1
        value.header.stamp.nanosec = 2
        value.header.frame_id = 'managed'
        value.name, value.position, value.velocity, value.effort = ['joint1'], [1.25], [0.5], [-0.75]
        publisher.publish(value).checked()
        for _ in range(256):
            credit = sys.stdin.buffer.read(1)
            if credit == b'R':
                result = subscription.poll_once()
                if result.error not in (0, 11): result.checked()
                if seen: break
            elif credit == b'Q': break
            else: raise ValueError('bounded explicit R/Q stdin schedule required')
        if len(seen) != 1:
            raise RuntimeError('no complete independently witnessed value; no replay')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
