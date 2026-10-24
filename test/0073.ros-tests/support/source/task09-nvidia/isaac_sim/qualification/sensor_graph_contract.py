"""Authored graph-input contracts; schema checks do not start Isaac Sim."""

import argparse
import json
from pathlib import Path
import sys
import unittest

from sensor_graph import decode, plan

SCHEMAS = None


def row(kind, **extra):
    value = {"kind": kind, "topic": "/test/" + kind, "qos": "{}"}
    if kind not in {"clock", "tf"}:
        value.update(prim="/World/Sensor", frame_id="sensor")
    if kind.startswith(("camera_", "lidar_")):
        value.update(width=224, height=224)
    if kind == "tf":
        value["prims"] = ["/World/Robot"]
    value.update(extra)
    return value


def config(*rows):
    return {"format": "uwvm-isaac-sim-native-sensors-v1", "sensors": list(rows)}


class Contracts(unittest.TestCase):
    def test_actual_fixed_node_and_port_membership(self):
        schemas = {}
        for path in SCHEMAS.rglob("*.ogn"):
            for name, body in json.loads(path.read_bytes()).items():
                if "." not in name:
                    name = "isaacsim.core.nodes." + name
                schemas[name] = body
        result = plan(
            config(
                *(
                    row(name)
                    for name in (
                        "clock",
                        "tf",
                        "imu",
                        "camera_rgb",
                        "camera_depth",
                        "camera_depth_pointcloud",
                        "lidar_scan",
                        "lidar_pointcloud",
                    )
                )
            )
        )
        nodes = dict(result["nodes"])
        nodes["Context"] = "isaacsim.ros2.bridge.ROS2Context"
        nodes["Time"] = "isaacsim.core.nodes.IsaacReadSimulationTime"
        for _name, identity in result["nodes"]:
            self.assertIn(identity, schemas)
        for address, _value in result["values"] + result["targets"]:
            name, port = address.split(".", 1)
            direction, member = port.split(":", 1)
            self.assertIn(member, schemas[nodes[name]][direction], address)
        for source, destination in result["links"]:
            for address in (source, destination):
                name, port = address.split(".", 1)
                if name == "Tick":
                    continue  # Core OmniGraph node, outside selected NVIDIA schemas.
                direction, member = port.split(":", 1)
                self.assertIn(member, schemas[nodes[name]][direction], address)
        self.assertFalse(result["simulator_started"])

    def test_clock_and_actuator_authority_are_separate(self):
        result = plan(config(row("clock")))
        self.assertEqual(
            [identity for _, identity in result["nodes"]],
            ["isaacsim.ros2.bridge.ROS2PublishClock"],
        )
        self.assertEqual(result["default_authority"], [])

    def test_camera_and_lidar_preserve_different_backends(self):
        result = plan(config(row("camera_depth_pointcloud"), row("lidar_pointcloud")))
        values = dict(result["values"])
        self.assertEqual(values["Sensor0Publish.inputs:type"], "depth_pcl")
        self.assertEqual(values["Sensor1Publish.inputs:type"], "point_cloud")
        self.assertFalse(
            any("frameSkipCount" in key or "fullScan" in key for key in values)
        )

    def test_imu_reads_actual_sensor_time_and_values(self):
        result = plan(config(row("imu")))
        self.assertIn(
            ("Sensor0Read.outputs:sensorTime", "Sensor0Publish.inputs:timeStamp"),
            result["links"],
        )
        self.assertIn(
            ("Sensor0Read.outputs:orientation", "Sensor0Publish.inputs:orientation"),
            result["links"],
        )
        self.assertIn(
            ("Sensor0Read.inputs:imuPrim", ["/World/Sensor"]), result["targets"]
        )

    def test_duplicate_topics_refuse(self):
        with self.assertRaises(ValueError):
            plan(config(row("clock", topic="/same"), row("tf", topic="/same")))

    def test_extra_partial_or_unbounded_fields_refuse(self):
        for invalid in (
            row("camera_rgb", width=0),
            row("camera_rgb", width=True),
            row("imu", prim="/World/../Robot"),
            row("clock", arbitrary="port"),
            row("clock", topic="/test;exec"),
            row("unknown"),
        ):
            with self.subTest(invalid=invalid), self.assertRaises(ValueError):
                plan(config(invalid))

    def test_json_duplicate_and_nonfinite_refuse(self):
        for value in ('{"a":1,"a":2}', '{"a":NaN}', '{"a":Infinity}'):
            with self.subTest(value=value), self.assertRaises(ValueError):
                decode(value)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--schemas", type=Path, required=True)
    args, rest = parser.parse_known_args()
    SCHEMAS = args.schemas
    unittest.main(argv=[sys.argv[0]] + rest)
