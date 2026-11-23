#!/usr/bin/env python3
"""Host/DAP policy regressions. Modeled replies do not qualify a runtime capture."""
import importlib.util
import io
import json
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
import fault_action
import snapshot
spec = importlib.util.spec_from_file_location("dump_policy_dap", ROOT / "tools/debug/dap_adapter.py")
dap = importlib.util.module_from_spec(spec)
spec.loader.exec_module(dap)


class Broker:
    def __init__(self, reply):
        self.reply, self.commands = reply, []

    def request(self, command):
        self.commands.append(command)
        return self.reply


class Policy(unittest.TestCase):
    def test_launch_host_options_only(self):
        for boundary in ("--run", "-r", "--"):
            self.assertEqual(fault_action.validate_dump_policy([boundary, "guest.wasm", "--debug-dump-force", "none"]), ("max", "full"))
        self.assertEqual(fault_action.validate_dump_policy(["--wasip1-global-force-args", "2", "--debug-dump-force", "none"]), ("max", "full"))

    def test_launch_combinations(self):
        for permission in fault_action.PERMISSIONS:
            for option in ("max", "force"):
                for level in ("none", "diagnostic", "full"):
                    args = ["--debug-permissions", permission, "--debug-dump-" + option, level]
                    permitted = (option == "max" and level != "none") or (option == "force" and
                        (level == "none" or permission != "none" and (level == "diagnostic" or permission in ("full", "read-only"))))
                    with self.subTest(args=args):
                        if permitted:
                            self.assertEqual(fault_action.validate_dump_policy(args), (option, level))
                        else:
                            with self.assertRaises(ValueError): fault_action.validate_dump_policy(args)
        for args in (["--debug-dump-max"], ["--debug-dump-force", "other"],
                     ["--debug-dump-max", "full", "--debug-dump-force", "diagnostic"],
                     ["--debug-dump-force", "none", "--debug-dump-force", "none"]):
            with self.assertRaises(ValueError): fault_action.validate_dump_policy(args)

    def dispatch(self, permission, reply, command="uwvm/dumpPolicy", arguments=None):
        output = io.BytesIO(); adapter = dap.Adapter(output); broker = Broker(reply)
        adapter.broker = broker; adapter.debug_permissions = permission
        adapter.handle({"seq": 1, "command": command, "arguments": arguments or {}})
        output.seek(0); rows = []
        while line := output.readline():
            count = int(line.split(b":")[1]); self.assertEqual(output.readline(), b"\r\n")
            rows.append(json.loads(output.read(count)))
        return next(row for row in rows if row["type"] == "response"), broker

    def test_dap_policy_display_and_disabled_permissions(self):
        for permission in ("full", "read-only", "limited"):
            for kind, level, effective, allowed in (("max", "full", "full", ["diagnostic", "full"]),
                    ("max", "full", "diagnostic", ["diagnostic"]), ("force", "diagnostic", "diagnostic", ["diagnostic"]),
                    ("force", "none", "none", [])):
                response, broker = self.dispatch(permission, f"dump-policy {kind}={level} effective={effective}\n")
                ceiling = "diagnostic" if permission == "limited" else "full"
                expected = level if kind == "force" or ceiling == "full" else "diagnostic" if level != "none" else "none"
                valid = expected == effective and not (permission == "limited" and effective == "full")
                self.assertEqual(response["success"], valid, response)
                if valid: self.assertEqual(response["body"]["allowed"], allowed)
                self.assertEqual(broker.commands, ["dump policy"])
        response, broker = self.dispatch("none", "dump-policy max=full effective=none\n")
        self.assertFalse(response["success"]); self.assertEqual(broker.commands, [])

    def test_dap_failure_is_never_saved(self):
        args = {"path": "/private/robot dump.uwsnap", "stopId": "7", "disclosure": "full"}
        reply = "error: wasm-dump rejected: requested disclosure conflicts with --debug-dump-force\n"
        response, broker = self.dispatch("full", reply, "uwvm/dump", args)
        self.assertFalse(response["success"])
        self.assertEqual(broker.commands, ["dump wasm full 7 /private/robot dump.uwsnap"])
        response, _ = self.dispatch("full", "wasm-dump saved stop=7 disclosure=diagnostic; current stop retained; inspect capture completeness metadata\n", "uwvm/dump",
                                    {**args, "disclosure": "diagnostic"})
        self.assertTrue(response["success"]);self.assertTrue(response["body"]["redacted"])

    def test_dump_acknowledgement_is_bound_to_stop_and_disclosure(self):
        args = {"path": "/private/fault.uwsnap", "stopId": "7", "disclosure": "diagnostic"}
        for permission, reply in (
                ("full", "wasm-dump saved but not really\n"),
                ("full", "wasm-dump saved stop=8 disclosure=diagnostic; current stop retained; inspect capture completeness metadata\n"),
                ("full", "wasm-dump saved stop=7 disclosure=full; current stop retained; inspect capture completeness metadata\n"),
                ("limited", "wasm-dump saved stop=7 disclosure=full; current stop retained; inspect capture completeness metadata\n")):
            response, _ = self.dispatch(permission, reply, "uwvm/dump", args)
            self.assertFalse(response["success"], (permission, reply))

    def test_malformed_dump_policy_is_not_presented_as_authority(self):
        for reply in ("", "dump-policy max=none effective=none\n", "dump-policy force=full effective=full\nextra\n",
                      "dump-policy max=full effective=bad\n", "permissions full\n",
                      "dump-policy max=diagnostic effective=full\n", "dump-policy force=none effective=diagnostic\n",
                      "dump-policy force=diagnostic effective=none\n", "dump-policy force=full effective=diagnostic\n"):
            response, _ = self.dispatch("limited", reply)
            self.assertFalse(response["success"], reply)


if __name__ == "__main__": unittest.main()
