"""Genuine ROS2 two-point Stretch example loopback, with no hardware driver.

Requires the actual selected rclpy/control_msgs/sensor_msgs/trajectory_msgs
packages and localhost-only middleware. This is a native ROS application test;
it never represents a Wasm/Runtime/Gateway or physical Stretch qualification.
"""

import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import sys
import time
import uuid


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    if os.environ.get("ROS_LOCALHOST_ONLY") != "1" or not os.environ.get(
        "ROS_DOMAIN_ID"
    ):
        raise RuntimeError("explicit localhost-only genuine ROS2 domain required")
    sys.path.insert(0, str(args.source / "sdk/origin/robots"))
    import rclpy
    from rclpy.action import ActionServer, GoalResponse
    from rclpy.context import Context
    from rclpy.executors import SingleThreadedExecutor
    from rclpy.node import Node
    from rclpy.qos import qos_profile_sensor_data
    from rclpy.serialization import serialize_message
    from control_msgs.action import FollowJointTrajectory
    from sensor_msgs.msg import JointState
    from std_msgs.msg import Bool, String
    from mobile_manipulation.stretch.examples.native_support.observation import (
        complete_joint_state,
        observe_current,
        lift_goal,
    )
    from mobile_manipulation.stretch.examples.native_support.native_trajectory_peer import (
        NativeTrajectoryPeer,
    )

    frame_root = args.output.parent / "stretch-frames"
    frame_root.mkdir()
    rows, frames, accepted = [], [], []

    def capture(name, value):
        body = serialize_message(value)
        file = frame_root / (name + ".cdr")
        with file.open("xb") as stream:
            stream.write(body)
        frames.append(
            dict(
                name=name,
                path=str(file),
                bytes=len(body),
                sha256=hashlib.sha256(body).hexdigest(),
            )
        )
        return body

    def refused(name, kind, call):
        before = len(accepted)
        try:
            call()
        except kind:
            pass
        else:
            raise AssertionError(name + " unexpectedly returned")
        assert len(accepted) == before
        rows.append(dict(name=name, status="PASS", native_goals=0))

    context = Context()
    rclpy.init(context=context)
    client_node = server_node = executor = peer = server = timer = None
    primary = None
    cleanup = []
    try:
        namespace = "/uwvm_task08_" + uuid.uuid4().hex
        client_node = Node(
            "client", namespace=namespace, context=context, enable_rosout=False
        )
        server_node = Node(
            "test_peer", namespace=namespace, context=context, enable_rosout=False
        )
        executor = SingleThreadedExecutor(context=context)
        executor.add_node(client_node)
        executor.add_node(server_node)
        values = [
            JointState(),
            Bool(data=True),
            Bool(data=False),
            String(data="trajectory"),
            Bool(data=False),
            String(data="stretch_gripper"),
        ]
        joints = values[0]
        joints.header.frame_id = "test-only-base"
        joints.name = ["joint_lift", "joint_head_pan"]
        joints.position = [0.5, 0.2]
        joints.velocity = [0.0, 0.0]
        joints.effort = [1.25, 2.5]
        topics = (
            "joint_states",
            "is_homed",
            "is_runstopped",
            "mode",
            "is_streaming_position",
            "tool",
        )
        publishers = [
            server_node.create_publisher(type(value), topic, qos_profile_sensor_data)
            for topic, value in zip(topics, values)
        ]

        def publish():
            joints.header.stamp = server_node.get_clock().now().to_msg()
            for publisher, value in zip(publishers, values):
                publisher.publish(value)

        timer = server_node.create_timer(0.02, publish)
        observation = observe_current(
            client_node, executor, time.monotonic_ns() + 5_000_000_000
        )
        for name, value in zip(
            topics,
            (
                observation.joints,
                observation.homed,
                observation.runstopped,
                observation.mode,
                observation.streaming,
                observation.tool,
            ),
        ):
            capture("observation-" + name, value)
        assert observation.joints.name == joints.name
        assert list(observation.joints.position) == list(joints.position)
        assert list(observation.joints.velocity) == list(joints.velocity)
        assert list(observation.joints.effort) == list(joints.effort)
        rows.append(
            dict(
                name="six actual full driver-topic observations with monotonic freshness",
                status="PASS",
                native_goals=0,
            )
        )

        settings = dict(
            expected_tool="stretch_gripper",
            target_m=0.6,
            lower_m=0.0,
            upper_m=1.3,
            maximum_speed_mps=0.1,
            duration_ns=2_000_000_000,
            now_ns=time.monotonic_ns(),
        )
        duplicate = copy.deepcopy(observation.joints)
        duplicate.name = ["joint_lift", "joint_lift"]
        refused(
            "duplicate original JointState names",
            ValueError,
            lambda: complete_joint_state(duplicate),
        )
        missing = copy.deepcopy(observation.joints)
        missing.velocity = [1.0]
        refused(
            "partial original JointState vector",
            ValueError,
            lambda: complete_joint_state(missing),
        )
        refused(
            "wrong actual tool",
            OSError,
            lambda: lift_goal(observation, **dict(settings, expected_tool="other")),
        )
        refused(
            "stale original observation",
            ValueError,
            lambda: lift_goal(
                observation, **dict(settings, now_ns=observation.fresh_until_ns() + 1)
            ),
        )
        refused(
            "target outside reviewed limits",
            ValueError,
            lambda: lift_goal(observation, **dict(settings, target_m=1.4)),
        )
        refused(
            "speed exceeds reviewed lift bound",
            ValueError,
            lambda: lift_goal(observation, **dict(settings, duration_ns=1_000_000)),
        )
        goal = lift_goal(observation, **settings)
        assert len(goal.trajectory.points) == 2
        assert list(goal.trajectory.points[0].positions) == [0.5]
        assert list(goal.trajectory.points[1].positions) == [0.6]
        assert goal.trajectory.points[0].time_from_start.sec == 0
        assert goal.trajectory.points[1].time_from_start.sec == 2
        assert not goal.multi_dof_trajectory.points
        expected = capture("goal-sent", goal)
        rows.append(
            dict(
                name="complete original two-point Goal from observed start and explicit target",
                status="PASS",
                native_goals=0,
            )
        )

        def accept(value):
            assert serialize_message(value) == expected
            accepted.append(capture("goal-native-peer", value))
            return GoalResponse.ACCEPT

        def execute(handle):
            feedback = FollowJointTrajectory.Feedback()
            feedback.header.frame_id = "test-only-base"
            feedback.header.stamp = server_node.get_clock().now().to_msg()
            feedback.joint_names = ["joint_lift"]
            for point, position in (
                (feedback.desired, 0.6),
                (feedback.actual, 0.59),
                (feedback.error, 0.01),
            ):
                point.positions = [position]
                point.velocities = [0.0]
                point.accelerations = [0.0]
                point.effort = [1.25]
                point.time_from_start.sec = 2
            capture("feedback-native-peer", feedback)
            handle.publish_feedback(feedback)
            handle.succeed()
            result = FollowJointTrajectory.Result()
            result.error_code = FollowJointTrajectory.Result.SUCCESSFUL
            result.error_string = "bounded native loopback result: \u03b2"
            capture("result-native-peer", result)
            return result

        server = ActionServer(
            server_node,
            FollowJointTrajectory,
            "follow_joint_trajectory",
            execute_callback=execute,
            goal_callback=accept,
        )
        peer = NativeTrajectoryPeer(
            client_node, executor, "follow_joint_trajectory", maximum_feedback=4
        )
        # Discover before the fresh observation used for actual submission.
        peer.ready(time.monotonic_ns() + 5_000_000_000)
        observation = observe_current(
            client_node, executor, time.monotonic_ns() + 3_000_000_000
        )
        goal = lift_goal(observation, **dict(settings, now_ns=time.monotonic_ns()))
        assert serialize_message(goal) == expected
        native_uuid = peer.submit(goal, observation.fresh_until_ns())
        result = peer.result(time.monotonic_ns() + 5_000_000_000)
        assert (
            result.status == 4
            and result.result.error_code == FollowJointTrajectory.Result.SUCCESSFUL
        )
        assert result.result.error_string == "bounded native loopback result: \u03b2"
        capture("result-complete-client-wrapper", result)
        deadline = time.monotonic_ns() + 1_000_000_000
        while not peer.feedback_used and time.monotonic_ns() < deadline:
            executor.spin_once(timeout_sec=0.02)
        assert peer.feedback_used == 1 and peer.feedback_error == 0
        feedback = peer.feedback[0]
        assert (
            bytes(feedback.goal_id.uuid)
            == bytes(native_uuid.uuid)
            == bytes(peer.goal_handle.goal_id.uuid)
        )
        capture("feedback-complete-client-wrapper", feedback)
        assert (
            serialize_message(feedback.feedback)
            == (frame_root / "feedback-native-peer.cdr").read_bytes()
        )
        assert (
            serialize_message(result.result)
            == (frame_root / "result-native-peer.cdr").read_bytes()
        )
        assert len(accepted) == 1 and peer.cancel_future is None
        rows.append(
            dict(
                name="one actual Action goal, complete correlated feedback and terminal result",
                status="PASS",
                native_goals=1,
            )
        )
        refused(
            "one-shot native peer does not resend a Goal",
            OSError,
            lambda: peer.submit(goal, time.monotonic_ns() + 1_000_000_000),
        )
        report = dict(
            schema="uwvm-task08-stretch-native-ros2-loopback-v1",
            cases=rows,
            passed=len(rows),
            denominator=len(rows),
            actual_native_goals=len(accepted),
            actual_native_feedback=peer.feedback_used,
            actual_goal_uuid=bytes(native_uuid.uuid).hex(),
            frames=frames,
            atomic_six_topic_snapshot=False,
            default_authority=[],
            actual_rclpy_module=rclpy.__file__,
            robot_cases=0,
            wasm_guest_cases=0,
            gateway_cases=0,
            physical_driver_loaded=False,
            native_cancel_requests=0,
        )
        args.output.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
        print(
            json.dumps(
                dict(
                    passed=len(rows),
                    denominator=len(rows),
                    actual_native_goals=len(accepted),
                )
            )
        )
    except BaseException as error:
        primary = error
        raise
    finally:
        for closer in (
            peer.close if peer else None,
            server.destroy if server else None,
            (
                (lambda: server_node.destroy_timer(timer))
                if server_node and timer
                else None
            ),
            (lambda: executor.shutdown(timeout_sec=2.0)) if executor else None,
            client_node.destroy_node if client_node else None,
            server_node.destroy_node if server_node else None,
            context.shutdown,
        ):
            if closer:
                try:
                    if closer() is False:
                        raise RuntimeError("native cleanup returned false")
                except BaseException as error:
                    cleanup.append(error)
        if cleanup:
            if primary is not None:
                primary.task08_cleanup = cleanup
            else:
                raise RuntimeError(cleanup)


if __name__ == "__main__":
    main()
