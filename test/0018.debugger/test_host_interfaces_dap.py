#!/usr/bin/env python3
"""Build availability, native inventory framing and debugger disclosure gates."""
from pathlib import Path
import io
import json
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import dap_adapter as dap


def inventory(total=0, first=0, preload=True, limit=32):
    count = min(limit, max(0, total - first))
    text = (f"host-interfaces schema=1 first={first} total={total} count={count} more={int(first+count<total)}\n"
            "interface u2rosv1-ros1 built=0\ninterface u2rosv1-ros2 built=1\n"
            f"interface u2rosv2 built=1\ninterface preloaddl built={int(preload)}\n")
    for index in range(first, first+count):
        text += (f"library index={index} functions=3 signature-verified=0 name-hex=647269766572 name-truncated=0 "
                 "path-hex=2f6f70742f6472697665722e736f path-truncated=0\n")
    return text


class HostInterfaces(unittest.TestCase):
    def test_builds_are_independent_of_ros_backend_number(self):
        reply = dap.parse_host_interfaces(inventory())
        self.assertFalse(reply["builds"]["u2rosv1-ros1"])
        self.assertTrue(reply["builds"]["u2rosv1-ros2"])
        self.assertTrue(reply["builds"]["u2rosv2"])
        self.assertEqual(reply["libraries"], [])

    def test_pages_keep_exact_metadata_without_native_addresses(self):
        for first in (0, 32, 34, 100):
            row = dap.parse_host_interfaces(inventory(35, first), first)
            self.assertEqual(row["more"], first < 32)
            for library in row["libraries"]:
                self.assertEqual(library["name"]["text"], "driver")
                self.assertEqual(library["path"]["text"], "/opt/driver.so")
                self.assertFalse(library["signatureVerified"])
                self.assertFalse(any("address" in key.lower() for key in library))

    def test_malformed_metadata_is_rejected(self):
        valid = inventory(1)
        bad = ["", valid.replace("schema=1", "schema=2"), valid.replace("first=0", "first=1"),
               valid.replace("count=1", "count=33"), valid.replace("total=1", "total="+str(1<<64)),
               valid.replace("more=0", "more=1"), valid.replace("index=0", "index=1"),
               valid.replace("name-hex=647269766572", "name-hex=6"), valid+"native-pc=0x1234\n",
               valid.replace("interface preloaddl built=1", "interface preloaddl built=0"),
               valid.replace("interface u2rosv1-ros2", "interface u2rosv1-ros1")]
        bad += [inventory(35, limit=0), " " * 65537]
        for text in bad:
            with self.subTest(text=text), self.assertRaises(ValueError):
                dap.parse_host_interfaces(text)

    def test_byte_bounded_pages_still_advance(self):
        for first in range(0, 35, 3):
            reply = dap.parse_host_interfaces(inventory(35, first, limit=3), first)
            self.assertEqual(int(reply["next"]), min(first + 3, 35))
            self.assertEqual(reply["more"], first + 3 < 35)

    def test_limited_and_none_never_reach_transport(self):
        class NoTransport:
            def request(self, command):
                raise AssertionError("native inventory request reached denied transport")
        for permission in ("none", "limited"):
            output = io.BytesIO()
            adapter = dap.Adapter(output)
            adapter.broker = NoTransport()
            adapter.debug_permissions = permission
            adapter.handle({"seq": 1, "command": "uwvm/hostInterfaces", "arguments": {}})
            self.assertFalse(json.loads(output.getvalue().split(b"\r\n\r\n", 1)[1])["success"])

    def test_read_only_has_inventory_but_cannot_modify_execution(self):
        self.assertTrue(dap.dap_permission_allowed("read-only", "uwvm/hostInterfaces"))
        self.assertFalse(dap.dap_permission_allowed("read-only", "uwvm/restoreSnapshot"))


if __name__ == "__main__":
    unittest.main()
