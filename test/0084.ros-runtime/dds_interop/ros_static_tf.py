"""Read native /tf_static through the installed Jazzy Python TF2 frame buffer."""

import math
import os
from pathlib import Path
import sys
import time

import rclpy
import tf2_py
import tf2_ros
from rclpy.parameter import Parameter
from rclpy.qos import DurabilityPolicy, QoSProfile
from rclpy.time import Time
from rclpy.utilities import get_rmw_implementation_identifier


def require(ok, message):
    if not ok:
        raise RuntimeError(message)


def vector(values, expected):
    """Validate all transform components and reject non-finite coordinates."""
    require(all(math.isfinite(a) and math.isclose(a, b, rel_tol=0, abs_tol=1e-9)
                for a, b in zip(values, expected, strict=True)), "wrong transform")


def fields(transform):
    shift, turn = transform.translation, transform.rotation
    return shift.x, shift.y, shift.z, turn.x, turn.y, turn.z, turn.w


def mapped_libraries():
    """Emit original Python module and executable dependency paths."""
    static = [rclpy.__file__, tf2_ros.__file__, tf2_py.__file__]
    static.extend(sys.modules[name].__file__ for name in ("tf2_ros.transform_listener", "tf2_ros.buffer"))
    for path in static:
        print("MODULE", path)
    mappings = set()
    for line in Path("/proc/self/maps").read_text().splitlines():
        row = line.split(maxsplit=5)
        if len(row) == 6 and "x" in row[1] and row[5].startswith("/"):
            mappings.add(row[5])
    for path in sorted(mappings):
        print("LIB", path)


def main(expected_rmw, scenario, token):
    """Validate retained and live samples independently in the official listener."""
    rclpy.init(args=[])
    require(get_rmw_implementation_identifier() == expected_rmw, "active RMW changed")
    overrides = [Parameter("start_type_description_service", value=False)] if (
        os.getenv("ROS_SECURITY_ENABLE") == "true") else []
    node = rclpy.create_node("wros_static_tf_sink", enable_rosout=False, start_parameter_services=False,
                             parameter_overrides=overrides)
    tree = tf2_ros.Buffer()
    seen = []
    q = math.sqrt(0.5)
    live = False

    class Witness(tf2_ros.TransformListener):
        """Validate the two edge transforms before passing them to the stock listener."""

        def static_callback(self, message):
            require(scenario != "writer-gone" and len(message.transforms) == 2, "unexpected transform batch")
            mount, optical = message.transforms
            index = mount.header.stamp.nanosec
            index_expected = (seen[-1] + 1 if seen else 1) if live else 0
            require(index == index_expected and index <= 24, "duplicate, stale or reordered frame")
            require(live or not seen, "duplicate historical frame")
            for transform in message.transforms:
                require(transform.header.stamp.sec == token and transform.header.stamp.nanosec == index,
                        "frame freshness token mismatch")
            require((mount.header.frame_id, mount.child_frame_id, optical.header.frame_id, optical.child_frame_id) ==
                    ("wros_base", "wros_camera_mount", "wros_camera_mount", "wros_camera_optical"),
                    "incorrect frame tree")
            x = token / 4096 + index
            vector(fields(mount.transform), (1, 2, 3, 0, 0, q, q))
            vector(fields(optical.transform), (x, 4, 5, 0, 0, 0, 1))

            super().static_callback(message)
            # A nonzero timestamp checks static lookup semantics across both edges.
            when = Time(seconds=1337)
            forward = tree.lookup_transform("wros_base", "wros_camera_optical", when)
            inverse = tree.lookup_transform("wros_camera_optical", "wros_base", when)
            vector(fields(forward.transform), (-3, 2 + x, 8, 0, 0, q, q))
            vector(fields(inverse.transform), (-(2 + x), -3, -8, 0, 0, -q, q))
            now = time.monotonic_ns()
            seen.append(index)
            print("RECEIVED", index, now, flush=True)

    volatile = scenario in {"volatile", "both-volatile"}
    qos = QoSProfile(depth=100, durability=DurabilityPolicy.VOLATILE) if volatile else None
    before_listen = time.monotonic_ns()
    listener = Witness(tree, node, static_qos=qos)
    print("LISTEN_STARTED", before_listen, flush=True)
    try:
        # Jazzy rclpy exposes the advertised RMW profile in its graph API.
        until = time.monotonic() + 2
        while True:
            reported = node.get_subscriptions_info_by_topic("/tf_static")
            if reported:
                break
            require(time.monotonic() < until, "listener absent from RMW graph")
            rclpy.spin_once(node, timeout_sec=0.01)
        subscription, = reported
        require(subscription.node_name == node.get_name(), "unexpected listener in RMW graph")
        actual = subscription.qos_profile.durability
        expected = DurabilityPolicy.VOLATILE if volatile else DurabilityPolicy.TRANSIENT_LOCAL
        require(actual == expected, "listener durability changed")
        print("READER_QOS", "volatile" if volatile else "transient", flush=True)
        if scenario == "writer-gone":
            until = time.monotonic() + 1
            while time.monotonic() < until:
                rclpy.spin_once(node, timeout_sec=0.01)
                require(not node.count_publishers("/tf_static"), "a writer is unexpectedly present")
        else:
            until = time.monotonic() + 8
            while listener.tf_static_sub.get_publisher_count() != 1:
                require(time.monotonic() < until, "no matched writer")
                rclpy.spin_once(node, timeout_sec=0.01)
            print("MATCHED", flush=True)
            if scenario == "transient":
                while not seen:
                    require(time.monotonic() < until, "cached transform was not delivered")
                    rclpy.spin_once(node, timeout_sec=0.01)
            else:
                until = time.monotonic() + 0.75
                while time.monotonic() < until:
                    rclpy.spin_once(node, timeout_sec=0.01)
        if scenario == "transient":
            require(seen == [0], "missing retained transform")
        elif scenario != "volatile":
            require(not seen, "unexpected retained transform")
        mapped_libraries()
        live = True
        print("HISTORY", len(seen), flush=True)

        if scenario != "writer-gone":
            until = time.monotonic() + 20
            while not seen or seen[-1] < 24:
                require(time.monotonic() < until, "incomplete live samples")
                rclpy.spin_once(node, timeout_sec=0.01)
            until = time.monotonic() + 0.15
            while time.monotonic() < until:
                rclpy.spin_once(node, timeout_sec=0.01)
        print("COMPLETE", len(seen), flush=True)
    finally:
        listener.unregister()
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2], int(sys.argv[3]))
