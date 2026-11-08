#!/usr/bin/env python3
"""Dynamic syntax DATA and opaque-stop routing, without guest read authority.

Broker responses below are protocol fixtures. Native source evaluation and actual
stop identity require the separate authenticated VM qualification.
"""
import importlib.util
from pathlib import Path
import sys
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location("dynamic_source_fixture", Path(__file__).with_name("test_dap_source_frames.py"))
fixture = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(fixture)
dap = fixture.dap


class DynamicSyntax(unittest.TestCase):
    def test_deep_groups_and_unary_do_not_change_the_python_recursion_limit(self):
        limit = sys.getrecursionlimit()
        depth = 131072
        selectors = ("(" * depth + "p" + ")" * depth, "*" * depth + "p")
        scalars = ("(" * depth + "1" + ")" * depth, "!" * depth + "1",
                   "sizeof " * depth + "1")
        for expression in selectors:
            self.assertEqual(dap.validate_source_expression(expression), expression)
        for expression in scalars:
            self.assertEqual(dap.validate_source_evaluation_expression(expression), expression)
        self.assertEqual(sys.getrecursionlimit(), limit)

    def test_long_selectors_nodes_digits_and_spaces_have_no_syntax_budget(self):
        selectors = ("p" + ".x" * 131073, "len(" + "p" + ".x" * 131073 + ")")
        scalars = ("1+" * 32768 + "1", "0" * 65537 + "1", "0x" + "0" * 65537 + "1",
                   "0b" + "0" * 65537 + "1", "0x" + "0" * 65537 + "1u64",
                   "0" * 65537 + "1u64", "value" + " " * 131072 + "+1",
                   "1 and " * 131072 + "1", "0 or " * 131072 + "1")
        for expression in selectors:
            self.assertEqual(dap.validate_source_expression(expression), expression)
        for expression in scalars:
            self.assertEqual(dap.validate_source_evaluation_expression(expression), expression)

    def test_deep_malformed_and_unsafe_arms_still_fail(self):
        depth = 131072
        cases = ("(" * depth + "1" + ")" * (depth - 1), "!" * depth,
                 "0 && (" + "(" * 4096 + "call()" + ")" * 4096 + ")",
                 "1 ? 1 : (" + "(" * 4096 + "(value=2)" + ")" * 4096 + ")",
                 "p[" + "9" * 65537 + "]", "p[-9223372036854775809]", "p[9223372036854775808]",
                 "0x" + "f" * 65537, "1" + "0" * 65537, "0x" + "f" * 65537 + "u64")
        for expression in cases:
            with self.assertRaises(ValueError):
                dap.validate_source_evaluation_expression(expression)
        for expression in ("p[-9223372036854775808]", "p[9223372036854775807]"):
            self.assertEqual(dap.validate_source_evaluation_expression(expression), expression)
        self.assertEqual(dap.validate_source_evaluation_expression("1+2"), "1+2")

    def test_failed_continuations_close_deepest_first(self):
        closed = []
        def continuation(index):
            try:
                if index == 4096:
                    raise MemoryError("injected child allocation failure")
                yield continuation(index + 1)
            finally:
                closed.append(index)
        with self.assertRaises(MemoryError):
            dap._run_source_syntax(continuation(0))
        self.assertEqual(closed, list(range(4096, -1, -1)))
        self.assertEqual(dap.validate_source_evaluation_expression("1+2"), "1+2")


class DynamicRouting(unittest.TestCase):
    setUp = fixture.SourceFrameTests.setUp
    send = fixture.SourceFrameTests.send
    frames = fixture.SourceFrameTests.frames
    assert_retired = fixture.SourceFrameTests.assert_retired

    def test_large_syntax_within_actual_packet_routes_without_truncation(self):
        expressions = ("0" * 300 + "1", "(" * 100 + "1" + ")" * 100,
                       "1+" * 129 + "1", "p" + ".x" * 40 + "+1")
        for expression in expressions:
            self.setUp()
            frame = self.frames()[0]
            before = len(self.broker.commands)
            reply = self.send("evaluate", frameId=frame["id"], context="watch", expression=expression)
            self.assertTrue(reply["success"], reply)
            self.assertEqual(self.broker.commands[before:], ["status", "print-frame 1 41 0 " + expression, "status"])
            self.assertNotIn("memoryReference", reply["body"])

    def test_controller_packet_exact_boundary_and_explicit_refusal(self):
        prefix = "print-frame 1 41 0 "
        for size in (512, 513, 8448):
            self.setUp()
            frame = self.frames()[0]
            expression = "0" * (size - len(prefix) - 1) + "1"
            before = len(self.broker.commands)
            reply = self.send("evaluate", frameId=frame["id"], context="hover", expression=expression)
            if size == 512:
                self.assertTrue(reply["success"], reply)
                self.assertEqual(self.broker.commands[before:], ["status", prefix + expression, "status"])
            else:
                self.assertFalse(reply["success"], reply)
                self.assertIn("512-byte command packet capacity", reply["message"])
                self.assertEqual(self.broker.commands[before:], [])
                self.assert_retired()
        self.assertEqual(dap.MAX_COMMAND, 8448)
        self.assertEqual(dap.breakpoint_condition_suffix({"condition": "a" * 256}), " if " + "a" * 256)
        with self.assertRaises(ValueError):
            dap.breakpoint_condition_suffix({"condition": "a" * 257})

    def test_dynamic_expression_rejects_stale_before_read_and_after_copy(self):
        expression = "(" * 100 + "1+2" + ")" * 100
        for phase in ("before", "after"):
            for field, value in (("stop", 42), ("epoch", 8), ("state", "running"), ("native", True)):
                self.setUp()
                frame = self.frames()[1]
                if phase == "before":
                    setattr(self.broker, field, value)
                else:
                    self.broker.after_copy = lambda field=field, value=value: setattr(self.broker, field, value)
                before = len(self.broker.commands)
                reply = self.send("evaluate", frameId=frame["id"], context="watch", expression=expression)
                self.assertFalse(reply["success"], reply)
                self.assertNotIn("body", reply)
                commands = self.broker.commands[before:]
                self.assertEqual(commands, ["status"] if phase == "before" else ["status", "print-frame 1 41 1 " + expression, "status"])
                self.assert_retired()

    def test_unsafe_unselected_arms_and_allocation_failure_do_not_reach_broker(self):
        for expression in ("1 ? 7 : call()", "0 && (value=2)", "0 && *(value+1)",
                           "0 && (void*)value", "1 || ++value", "value\x00status",
                           "value\ncontinue", "value+\u00e9"):
            self.setUp()
            frame = self.frames()[0]
            before = len(self.broker.commands)
            reply = self.send("evaluate", frameId=frame["id"], context="variables", expression=expression)
            self.assertFalse(reply["success"], reply)
            self.assertEqual(self.broker.commands[before:], [])
            self.assert_retired()
        self.setUp()
        frame = self.frames()[0]
        before = len(self.broker.commands)
        with patch.object(dap, "_parse_source_selector", side_effect=MemoryError("injected syntax allocation failure")):
            reply = self.send("evaluate", frameId=frame["id"], context="watch", expression="(" * 100 + "1" + ")" * 100)
        self.assertFalse(reply["success"], reply)
        self.assertIn("debug adapter allocation failed", reply["message"])
        self.assertNotIn("body", reply)
        self.assertEqual(self.broker.commands[before:], [])
        self.assert_retired()


if __name__ == "__main__":
    unittest.main()
