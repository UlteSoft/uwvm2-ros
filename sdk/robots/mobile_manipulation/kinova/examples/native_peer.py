"""Explicit native Gen3 6/7DOF FJT witness. Never runs at module import."""
import argparse
import json
import math
import time
import rclpy
from rclpy.executors import SingleThreadedExecutor
from rclpy.node import Node
from control_msgs.action import FollowJointTrajectory
from trajectory_msgs.msg import JointTrajectoryPoint
from builtin_interfaces.msg import Duration
from ...native_trajectory_peer import NativeTrajectoryPeer
from ..models import GEN3_6DOF, GEN3_7DOF


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--execute', action='store_true')
    parser.add_argument('--model', required=True, choices=('gen3-6dof', 'gen3-7dof'))
    parser.add_argument('--positions-file', required=True)
    parser.add_argument('--action', default='/joint_trajectory_controller/follow_joint_trajectory')
    parser.add_argument('--cancel', action='store_true')
    args = parser.parse_args()
    if not args.execute:
        raise PermissionError(13, 'native witness is stopped unless explicitly selected')
    names = GEN3_6DOF if args.model == 'gen3-6dof' else GEN3_7DOF
    with open(args.positions_file, encoding='utf-8') as stream:
        positions = json.load(stream)
    if (type(positions) is not list or len(positions) != len(names) or
            any(type(x) not in (int, float) or not math.isfinite(x) for x in positions)):
        raise ValueError('complete finite operator-selected joint positions required')
    goal = FollowJointTrajectory.Goal()
    goal.trajectory.joint_names = list(names)
    goal.trajectory.points = [JointTrajectoryPoint(
        positions=[float(x) for x in positions], time_from_start=Duration(sec=2))]
    # Every omitted convenience field remains an actual standard typed empty
    # array/zero duration, not an erased field in the interface definition.
    rclpy.init()
    node, executor, peer = None, None, None
    primary, outcomes = None, [None, None, None, None]
    try:
        node = Node('uwvm_gen3_native_witness')
        executor = SingleThreadedExecutor(context=node.context)
        executor.add_node(node)
        peer = NativeTrajectoryPeer(node, executor, args.action)
        peer.submit(goal, time.monotonic_ns() + 5_000_000_000)
        if args.cancel:
            print({'cancel_accepted': peer.cancel(time.monotonic_ns() + 2_000_000_000)})
        result = peer.result(time.monotonic_ns() + 10_000_000_000)
        print({'actual_goal_uuid': bytes(peer.goal_handle.goal_id.uuid).hex(),
               'actual_status': result.status, 'actual_error_code': result.result.error_code,
               'actual_error_string': result.result.error_string,
               'complete_feedback_count': peer.feedback_used})
    except BaseException as error:
        primary = error
        raise
    finally:
        for index, closer in enumerate((peer.close if peer else None,
                                        (lambda: executor.shutdown(timeout_sec=2.0)) if executor else None,
                                        node.destroy_node if node else None)):
            if closer:
                try:
                    outcomes[index] = closer()
                    if outcomes[index] is False:
                        raise RuntimeError('actual cleanup returned false')
                except BaseException as error:
                    outcomes[index] = error
        try:
            if rclpy.ok():
                rclpy.shutdown()
        except BaseException as error:
            outcomes[3] = error
        if primary is not None:
            primary.mobile_manipulation_cleanup = outcomes
        elif any(isinstance(item, BaseException) for item in outcomes):
            raise RuntimeError(outcomes)


if __name__ == '__main__':
    main()
