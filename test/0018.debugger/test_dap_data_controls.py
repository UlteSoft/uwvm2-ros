"""Protocol boundary checks. Real commits/watch hits are tested by the live runners."""
import io
import unittest
import test_dap_wasm_state as state

dap = state.dap

class Broker(state.Broker):
    def __init__(self):
        super().__init__()
        self.value = 17
        self.ack = lambda text: text
        self.points = {}
        self.next_point = 1

    def request(self, command):
        if command.startswith("locals wasm 1 0 "):
            self.commands.append(command)
            return (f"wasm-stop {self.stop}\nWasm state thread=1 module=0 epoch=7 first=0 total=1\n"
                    f"local 0 i32 = {self.value}\n")
        if command.startswith("set wasm local "):
            self.commands.append(command)
            words = command.split()
            assert words[:7] == ["set", "wasm", "local", "1", "0", "bits", "i32"]
            bits = int(words[7], 16)
            self.value = bits if bits < 1 << 31 else bits - (1 << 32)
            self.stop += 1
            return self.ack(f"wasm-stop {self.stop}\nWasm mutation v=3 status=available applied=1 reason=none "
                            "runtime=7 module=0 target=local index=0 element=0 bytes=0 address-bytes=0\n")
        if command == "watch locals wasm 1 0 0 1":
            self.commands.append(command)
            identifier = self.next_point
            self.next_point += 1
            self.points[identifier] = True
            return f"watchpoint {identifier} registered\n"
        if command.startswith("watchpoints delete "):
            self.commands.append(command)
            del self.points[int(command.split()[-1])]
            return "watchpoint policy updated\n"
        if command.startswith("watchpoints "):
            self.commands.append(command)
            return "".join(f"watchpoint {i} enabled={'yes' if on else 'no'} hits=0 local\n"
                           for i, on in self.points.items()) + f"watchpoint-next={len(self.points)} total={len(self.points)}\n"
        return super().request(command)

class DataControls(unittest.TestCase):
    send = state.StateProtocol.send
    def setUp(self):
        self.output = io.BytesIO()
        self.adapter = dap.Adapter(self.output)
        self.adapter.step_level = "wasm"
        self.adapter.broker = self.broker = Broker()
        self.sequence = 0

    def scope(self):
        frame = self.send("stackTrace", threadId=1)["body"]["stackFrames"][0]
        scopes = self.send("scopes", frameId=frame["id"])["body"]["scopes"]
        return next(row["variablesReference"] for row in scopes if row["name"] == "Typed Wasm locals")

    def selector(self):
        return self.send("dataBreakpointInfo", variablesReference=self.scope(), name="local 0")["body"]["dataId"]

    def writes(self):
        return [c for c in self.broker.commands if c.startswith("set wasm ")]

    def test_capabilities_match_current_typed_controls(self):
        result = self.send("initialize")["body"]
        self.assertFalse(result["supportsSetVariable"])
        self.assertFalse(result["supportsDataBreakpoints"])
        self.assertFalse(result["supportsLogPoints"])

    def test_actual_slot_name_width_and_stale_scope_before_write(self):
        for name, value in [("x", "1"), ("local 0", "4294967296"), ("local 0", "-2147483649"),
                            ("local 0", "bits 100000000"), ("local 0", "system()")]:
            self.setUp()
            result = self.send("setVariable", variablesReference=self.scope(), name=name, value=value)
            self.assertFalse(result["success"], result)
            self.assertEqual(self.writes(), [])
        self.setUp()
        reference = self.scope()
        self.broker.stop += 1
        self.assertFalse(self.send("setVariable", variablesReference=reference, name="local 0", value="9")["success"])
        self.assertEqual(self.writes(), [])

    def test_signed_bits_commit_once_and_invalidate_previous_handles(self):
        reference = self.scope()
        result = self.send("setVariable", variablesReference=reference, name="local 0", value="-1")
        self.assertTrue(result["success"], result)
        self.assertEqual(result["body"]["value"], "-1")
        self.assertEqual(self.writes(), ["set wasm local 1 0 bits i32 ffffffff"])
        self.assertFalse(self.send("variables", variablesReference=reference)["success"])

    def test_ambiguous_or_foreign_ack_never_retries_a_commit(self):
        for transform in [lambda x: x + "extra\n", lambda x: x.replace("module=0", "module=1"),
                          lambda x: x.replace("wasm-stop 42", "wasm-stop 41"),
                          lambda x: x.replace("index=0", "index=1"),
                          lambda x: x.replace("target=local", "target=global")]:
            self.setUp()
            reference = self.scope()
            self.broker.ack = transform
            result = self.send("setVariable", variablesReference=reference, name="local 0", value="9")
            self.assertFalse(result["success"], result)
            self.assertIn("unconfirmed", result["message"])
            self.assertEqual(len(self.writes()), 1)
            self.assertEqual(self.broker.value, 9)
            self.assertFalse(self.send("variables", variablesReference=reference)["success"])

    def test_data_selector_cannot_rebind_to_later_stop(self):
        identifier = self.selector()
        self.broker.stop += 1
        result = self.send("setDataBreakpoints", breakpoints=[{"dataId": identifier}])
        self.assertFalse(result["body"]["breakpoints"][0]["verified"])
        self.assertEqual(self.broker.points, {})

    def test_retained_watch_must_still_be_enabled_in_runtime(self):
        identifier = self.selector()
        request = [{"dataId": identifier}]
        result = self.send("setDataBreakpoints", breakpoints=request)
        self.assertTrue(result["body"]["breakpoints"][0]["verified"])
        result = self.send("setDataBreakpoints", breakpoints=request)
        self.assertTrue(result["body"]["breakpoints"][0]["verified"])
        self.assertEqual(self.broker.next_point, 2)
        self.broker.points[1] = False
        result = self.send("setDataBreakpoints", breakpoints=request)
        self.assertFalse(result["body"]["breakpoints"][0]["verified"])
        self.assertEqual(self.broker.points, {})

    def test_duplicate_data_ids_share_one_actual_watchpoint(self):
        identifier = self.selector()
        result = self.send("setDataBreakpoints", breakpoints=[{"dataId": identifier}] * 2)
        points = result["body"]["breakpoints"]
        self.assertTrue(all(point["verified"] for point in points), result)
        self.assertEqual([point["id"] for point in points], [1, 1])
        self.assertEqual(self.broker.points, {1: True})
        self.assertEqual(self.broker.next_point, 2)
        self.assertTrue(self.send("setDataBreakpoints", breakpoints=[])["success"])
        self.assertEqual(self.broker.points, {})

    def test_console_deleted_watch_is_not_retained_or_reinstalled(self):
        identifier = self.selector()
        result = self.send("setDataBreakpoints", breakpoints=[{"dataId": identifier}])
        self.assertTrue(result["body"]["breakpoints"][0]["verified"])
        self.broker.points.clear()
        result = self.send("setDataBreakpoints", breakpoints=[{"dataId": identifier}])
        self.assertTrue(result["success"], result)
        self.assertFalse(result["body"]["breakpoints"][0]["verified"])
        self.assertEqual(self.adapter.data_breakpoints, {})
        self.assertEqual(self.broker.next_point, 2)
        self.assertTrue(self.send("setDataBreakpoints", breakpoints=[])["success"])

    def test_reads_and_non_integer_scope_ids_are_not_advertised(self):
        identifier = self.selector()
        for access in ("read", "readWrite"):
            result = self.send("setDataBreakpoints", breakpoints=[{"dataId": identifier, "accessType": access}])
            self.assertFalse(result["body"]["breakpoints"][0]["verified"])
            self.assertEqual(self.broker.points, {})
        for reference in (True, False, None, -1, "0"):
            result = self.send("dataBreakpointInfo", variablesReference=reference, name="wasm-memory:0:0:0:4")
            self.assertFalse(result["success"], result)

if __name__ == "__main__":
    unittest.main()
