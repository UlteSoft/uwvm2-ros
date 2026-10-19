#!/usr/bin/env python3
"""Independent actual SDK2 DDS or ROS2 native motor peer for simulation.

The peer owns real native LowCmd/LowState endpoints and uses official SDK2
message objects and CRC code. It never fakes a ROS service or substitutes a
stdout publication marker for actual receipt. SDK2 rt/lowcmd and rt/lowstate
are distinct topic spellings from ROS2 /lowcmd and /lowstate. The fixture
preserves their selected middleware mapping through actual delivery.

Legacy ROS1 messages are deliberately not accepted as SDK2 Go: their fields,
CRC layout and licensed native bridge differ. A separate verified legacy
bridge may be a Root-selected external job; it must supply its own evidence.
"""
import argparse
import importlib
import math
import threading
import time

from common import child_arguments, child_guard, read_record, require, write_record
from messages import command, copy_fields, sdk


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    child_arguments(parser)
    parser.add_argument("--transport", choices=("sdk2", "ros2"), required=True)
    parser.add_argument("--observer-only", action="store_true")
    args = parser.parse_args()
    config, guard = child_guard(args)
    spec = config["simulation"]
    require(spec["interface"] == "lo" and 1 <= spec["domain"] <= 232, "loopback nonphysical test domain required")
    selected = sdk(spec["family"])
    received, errors, mutex = [], [], threading.Lock()
    node = publisher = subscriber = None
    rclpy = None
    def receive(value):
        try:
            mirror = value if args.transport == "sdk2" else copy_fields(selected["state"](), value)
            require(mirror.crc == selected["crc"].Crc(mirror), "actual native LowState CRC mismatch")
            snapshot = {"kind": "received", "tick": int(mirror.tick), "state_crc": int(mirror.crc),
                "received_monotonic_ns": time.monotonic_ns(), "motors": [
                    {"q": float(item.q), "dq": float(item.dq), "tau": float(item.tau_est)}
                    for item in mirror.motor_state]}
            require(all(math.isfinite(value) for motor in snapshot["motors"] for value in motor.values()),
                    "actual native LowState contains nonfinite motor values")
            with mutex:
                require(not received, "native peer received duplicated/unconsumed state")
                received.append(snapshot)
        except BaseException as error:
            with mutex:
                if not errors:
                    errors.append(repr(error))
    if args.transport == "sdk2":
        from unitree_sdk2py.core.channel import ChannelFactoryInitialize, ChannelPublisher, ChannelSubscriber
        ChannelFactoryInitialize(spec["domain"], "lo")
        subscriber = ChannelSubscriber("rt/lowstate", selected["LowState"])
        subscriber.Init(receive, 0)
        if not args.observer_only:
            publisher = ChannelPublisher("rt/lowcmd", selected["LowCmd"])
            publisher.Init()
    else:
        import rclpy
        from rclpy.qos import QoSProfile, HistoryPolicy, ReliabilityPolicy, DurabilityPolicy
        types = importlib.import_module("unitree_" + spec["family"] + ".msg")
        require(config["environment"].get("ROS_DOMAIN_ID") == str(spec["domain"]) and
                config["environment"].get("RMW_IMPLEMENTATION") == "rmw_cyclonedds_cpp",
                "explicit matching ROS2 Cyclone domain required")
        require("lo" in config["environment"].get("CYCLONEDDS_URI", ""), "explicit loopback DDS profile required")
        rclpy.init(args=[])
        # Fail on a distro which cannot honor quiet construction; do not add
        # hidden rosout or parameter endpoint authority to make a test start.
        node = rclpy.create_node("uwvm_unitree_native_peer", enable_rosout=False,
                                 start_parameter_services=False)
        qos = QoSProfile(history=HistoryPolicy.KEEP_LAST, depth=1,
                         reliability=ReliabilityPolicy.RELIABLE, durability=DurabilityPolicy.VOLATILE)
        if not args.observer_only:
            publisher = node.create_publisher(types.LowCmd, "/lowcmd", qos)
        subscriber = node.create_subscription(types.LowState, "/lowstate", receive, qos)
    def spin():
        guard()
        if node:
            rclpy.spin_once(node, timeout_sec=.001)
        else:
            time.sleep(.001)
        with mutex:
            require(not errors, "native peer callback failed: " + repr(errors))
    write_record({"kind": "ready", "protocol": "unitree-native-peer-v1", "transport": args.transport,
                  "family": spec["family"], "domain": spec["domain"], "interface": "lo"})
    published = observed = 0
    try:
        while True:
            packet = read_record()
            guard()
            operation = packet.get("op")
            if operation == "finish":
                with mutex:
                    require(not received and not errors, "unconsumed or failed native state at finish")
                write_record({"kind": "complete", "published": published, "observed": observed})
                break
            if operation == "publish":
                require(not args.observer_only, "observation-only native witness cannot publish commands")
                step = packet["step"]
                require(type(step) is int and step == published + 1, "strict native publish step required")
                value = command(selected, spec["family"], packet["motors"], spec["mode_machine"])
                if node:
                    until = time.monotonic() + spec["step_timeout_seconds"]
                    while publisher.get_subscription_count() < 1:
                        spin()
                        require(time.monotonic() < until, "actual DDS command subscriber not matched")
                    output = copy_fields(types.LowCmd(), value)
                    publisher.publish(output)
                else:
                    require(publisher.Write(value, timeout=spec["step_timeout_seconds"]), "actual SDK2 publish failed")
                published += 1
                write_record({"kind": "published", "step": step, "command_crc": int(value.crc),
                              "published_monotonic_ns": time.monotonic_ns()})
            elif operation == "receive":
                step = packet["step"]
                require(step == observed + 1, "strict native observation step required")
                until = time.monotonic() + spec["step_timeout_seconds"]
                while True:
                    spin()
                    with mutex:
                        if received:
                            value = received.pop()
                            break
                    require(time.monotonic() < until, "actual LowState receipt deadline exhausted")
                require(value["tick"] == step, "native LowState tick differs from current physics step")
                observed += 1
                write_record(value)
            elif operation == "quiet":
                duration = packet["seconds"]
                require(type(duration) in (int, float) and .01 <= duration <= 2, "bounded native quiet window required")
                began, samples = time.monotonic_ns(), 0
                until = time.monotonic() + duration
                while time.monotonic() < until:
                    spin()
                    with mutex:
                        require(not received, "unexpected native state during denied/quiet observation")
                    samples += 1
                write_record({"kind": "quiet", "samples": samples, "elapsed_ns": time.monotonic_ns() - began,
                              "published": published, "observed": observed})
            elif operation == "match":
                require(node is not None, "direct Guest discovery requires an actual ROS2 graph observer")
                until = time.monotonic() + spec["step_timeout_seconds"]
                while True:
                    spin()
                    subscriptions = node.count_subscribers("/lowstate")
                    publications = node.count_publishers("/lowcmd")
                    if subscriptions == packet["state_subscribers"] and publications == packet["command_publishers"]:
                        break
                    require(time.monotonic() < until, "actual direct ROS2 endpoints did not match selected graph counts")
                write_record({"kind": "matched", "state_subscribers": subscriptions, "command_publishers": publications})
            else:
                raise ValueError("unknown native peer operation")
    finally:
        if node:
            node.destroy_node()
            rclpy.shutdown()
        else:
            if subscriber:
                subscriber.Close()
            if publisher:
                publisher.Close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
