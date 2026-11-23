#!/usr/bin/env python3
"""Detached history transport, disclosure admission and stale-page regression."""
from pathlib import Path
import io
import json
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import dap_adapter as dap
from test_snapshot_wasip1_reader import fixture

class History(unittest.TestCase):
    def setUp(self):
        self.data = fixture()
        self.label = (b"x" * 16).hex()
        self.header = (f"snapshot-history serial=7 position=checkpoint event=12 instruction=8 recording={self.label}; historical data, no live resource access\n"
                       f"environment kind=6 id=2 bytes={len(self.data)}\n")
        self.commands = []

    def send(self, command):
        self.commands.append(command)
        if command == "dump history" or command.startswith("dump history open "):
            return self.header
        _, _, serial, kind, identifier, offset, count = command.split()
        self.assertEqual((serial, kind, identifier), ("7", "6", "2"))
        offset, count = int(offset), int(count)
        return (f"snapshot-history-bytes serial=7 kind=6 id=2 offset={offset} count={count} hex="
                f"{self.data[offset:offset+count].hex()}\n")

    def test_large_arguments_are_complete_and_rights_are_data(self):
        row = dap.read_snapshot_history(self.send, {"kind": "wasip1", "id": "2"})
        env = row["environment"]
        self.assertEqual(env["arguments"][1]["text"], "a" * 8193)
        self.assertEqual(env["variables"][0]["text"], "MODE=restore")
        self.assertEqual(env["bindings"][0]["baseRights"], "0x40")
        self.assertFalse(row["restoreAuthority"])
        self.assertEqual(len(self.commands), 5)  # index, three pages, recheck

    def test_bad_requests_never_use_transport(self):
        invalid = [None, [], {"id": "2"}, {"kind": "wasip1"}, {"kind": "ros3", "id": 0},
                   {"kind": "wasip1", "id": True}, {"kind": "wasip1", "id": -1},
                   {"kind": "wasip1", "id": str(1 << 64)}, {"path": "relative"},
                   {"path": "/x\ncontinue"}, {"path": "/x\x7f"},
                   {"path": "/x", "event": True}, {"path": "/x", "event": str(1 << 64)},
                   {"path": "/x", "kind": "wasip1"}]
        for args in invalid:
            with self.subTest(args=args), self.assertRaises(ValueError):
                dap.read_snapshot_history(self.send, args)
        self.assertEqual(self.commands, [])

    def test_remote_paths_are_sent_to_vm_host(self):
        for path in ("/host/archive with spaces.uwsnap", "C:/host/archive.uwsnap", "C:\\host\\archive.uwsnap"):
            row = dap.read_snapshot_history(self.send, {"path": path, "event": "12"})
            self.assertEqual(self.commands[-1], f"dump history open 12 {path}")
            self.assertNotIn("environment", row)

    def test_malformed_pages_fail_without_live_fallback(self):
        for defect in ("serial", "offset", "label", "duplicate", "tail", "refuse"):
            def broken(command):
                text = self.send(command)
                if command == "dump history":
                    return text + text.splitlines()[-1] + "\n" if defect == "duplicate" else text
                if defect == "serial": return text.replace("serial=7", "serial=8")
                if defect == "offset": return text.replace("offset=0", "offset=1")
                if defect == "label": return text.replace(self.label, (b"y" * 16).hex())
                if defect == "tail": return text + "\n"
                if defect == "refuse": return "error: permission denied\n"
                return text
            with self.subTest(defect=defect), self.assertRaises(ValueError):
                dap.read_snapshot_history(broken, {"kind": "wasip1", "id": "2"})
        self.assertTrue(all(text.startswith("dump history") for text in self.commands))

    def test_replacement_after_last_page_is_rejected(self):
        def changed(command):
            text = self.send(command)
            return text.replace("serial=7", "serial=8") if command == "dump history" and len(self.commands) > 1 else text
        with self.assertRaisesRegex(ValueError, "changed during transfer"):
            dap.read_snapshot_history(changed, {"kind": "wasip1", "id": "2"})

    def test_limited_and_none_decline_before_transport(self):
        class NoTransport:
            def request(self, command):
                raise AssertionError("denied history request reached transport")
        for permission in ("none", "limited"):
            output = io.BytesIO()
            adapter = dap.Adapter(output); adapter.broker = NoTransport(); adapter.debug_permissions = permission
            adapter.handle({"seq": 1, "command": "uwvm/snapshotHistory", "arguments": {}})
            response = json.loads(output.getvalue().split(b"\r\n\r\n", 1)[1])
            self.assertFalse(response["success"])

if __name__ == "__main__": unittest.main()
