"""Explicit Stretch lift trajectory/independent stop witness using original ROS2."""
import argparse
import math
import time
import rclpy
from rclpy.node import Node
from rclpy.executors import SingleThreadedExecutor
from control_msgs.action import FollowJointTrajectory
from trajectory_msgs.msg import JointTrajectoryPoint
from builtin_interfaces.msg import Duration
from ...native_trajectory_peer import NativeTrajectoryPeer, trigger_once


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--execute', action='store_true')
    parser.add_argument('--model', required=True, choices=('RE1V0', 'RE2V0', 'SE3'))
    parser.add_argument('--operation', required=True, choices=('lift', 'stop', 'trajectory-mode'))
    parser.add_argument('--lift-m', type=float)
    parser.add_argument('--cancel', action='store_true')
    args = parser.parse_args()
    if not args.execute:
        raise PermissionError(13, 'native witness is stopped unless explicitly selected')
    if args.operation == 'lift' and (args.lift_m is None or not math.isfinite(args.lift_m)):
        raise ValueError('explicit finite lift position required; actual robot limits remain mandatory')
    rclpy.init()
    node, executor, peer = None, None, None
    primary, outcomes = None, [None, None, None, None]
    try:
        node = Node('uwvm_stretch_native_witness')
        executor = SingleThreadedExecutor(context=node.context)
        executor.add_node(node)
        if args.operation != 'lift':
            name = '/stop_the_robot' if args.operation == 'stop' else '/switch_to_trajectory_mode'
            response = trigger_once(node, executor, name, time.monotonic_ns() + 3_000_000_000)
            print({'actual_service': name, 'success': response.success, 'message': response.message})
            return
        # Driver must already be in explicitly selected trajectory mode and
        # homed/tool-qualified. Do not implicitly switch mode or home/stow.
        goal = FollowJointTrajectory.Goal()
        goal.trajectory.joint_names = ['joint_lift']
        goal.trajectory.points = [JointTrajectoryPoint(
            positions=[args.lift_m], time_from_start=Duration(sec=2))]
        peer = NativeTrajectoryPeer(node, executor, '/stretch_controller/follow_joint_trajectory')
        peer.submit(goal, time.monotonic_ns() + 5_000_000_000)
        if args.cancel:
            print({'cancel_accepted': peer.cancel(time.monotonic_ns() + 2_000_000_000)})
        result = peer.result(time.monotonic_ns() + 10_000_000_000)
        print({'actual_goal_uuid': bytes(peer.goal_handle.goal_id.uuid).hex(), 'actual_status': result.status,
               'actual_error_code': result.result.error_code, 'actual_error_string': result.result.error_string,
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
