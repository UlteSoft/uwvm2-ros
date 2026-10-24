#!/usr/bin/env python3
"""Nine pure adversarial models of the actual bounded delivery controller.

The root runs this source after freezing it, with Python -B. Only the named
controller/marker function ASTs are loaded; the launch module and its imports
are never executed. Actors, time and logs below are explicit synthetic values.
These cases qualify causal gate/refusal logic, never DDS or native delivery.
"""
from __future__ import annotations
import ast
from pathlib import Path
import unittest

HERE = Path(__file__).resolve().parent


def require(condition, reason):
    if not condition:
        raise ValueError(reason)


def functions(path, names, namespace):
    """Select exact top-level pure function bodies without module import."""
    tree = ast.parse(path.read_text(encoding="utf-8"), filename=str(path))
    selected = [node for node in tree.body
                if isinstance(node, ast.FunctionDef) and node.name in names]
    require({node.name for node in selected} == set(names), "closed function inventory")
    exec(compile(ast.Module(body=selected, type_ignores=[]), str(path), "exec"), namespace)


class Clock:
    def __init__(self): self.now = 0.0
    def monotonic(self): return self.now
    def sleep(self, seconds): self.now += seconds


class Actor:
    def __init__(self, code=None): self.code = code
    def poll(self): return self.code


class LogLane:
    def __init__(self, clock, fragment):
        self.clock, self.fragment, self.text, self.releases = clock, fragment, "", []
    def check(self): self.clock.now += .01
    def read(self, row): return self.text
    def send(self, actor, row, byte, reason):
        require(byte == b"R", "model grants only explicit pass R")
        self.releases.append(byte)
        self.text += self.fragment


class FeedbackProtocolModels(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.clock = Clock()
        cls.namespace = {"require": require, "time": cls.clock}
        functions(HERE.parents[1] / "ros_acceptance/protocol.py",
                  ["marker_lines"], cls.namespace)
        functions(HERE / "run_custom_bounded.py", ["advance", "native_markers"], cls.namespace)

    def drive(self, fragment, hook=None, gateway=None, worker=None, child=None):
        self.clock.now = 0
        lane = LogLane(self.clock, fragment)
        action = None if hook is None else ("FEEDBACK", lambda: hook(lane))
        self.namespace["advance"](lane, child or Actor(), {}, "COMPLETE", "AGAIN",
                                  gateway or Actor(), worker or Actor(), action)
        return lane

    def test_client_terminal_is_released_only_after_actual_feedback(self):
        calls = []
        def release(lane):
            self.assertEqual(lane.text, "FEEDBACK\n")
            calls.append("native E then Guest E")
            lane.text += "COMPLETE\n"
        lane = self.drive("FEEDBACK\n", release)
        self.assertEqual(calls, ["native E then Guest E"])
        self.assertEqual(lane.releases, [b"R"])

    def test_server_native_callback_must_precede_guest_terminal(self):
        proof = {"native_feedback": False}
        def release(lane):
            proof["native_feedback"] = True  # Explicit independent oracle input.
            self.assertTrue(proof["native_feedback"])
            lane.text += "COMPLETE\n"
        self.drive("FEEDBACK\n", release)

    def test_completion_without_feedback_is_refused(self):
        with self.assertRaisesRegex(ValueError, "preceded required feedback"):
            self.drive("COMPLETE\n", lambda lane: self.fail("hook must not run"))

    def test_duplicate_feedback_marker_is_refused_before_hook(self):
        with self.assertRaisesRegex(ValueError, "repeated"):
            self.drive("FEEDBACK\nFEEDBACK\n", lambda lane: self.fail("hook must not run"))

    def test_pass_budget_remains_exactly_two_hundred(self):
        self.clock.now = 0
        lane = LogLane(self.clock, "AGAIN\n")
        with self.assertRaisesRegex(TimeoutError, "200-pass budget"):
            self.namespace["advance"](lane, Actor(), {}, "COMPLETE", "AGAIN", Actor(), Actor())
        self.assertEqual(len(lane.releases), 200)

    def test_retired_guest_before_completion_is_refused(self):
        with self.assertRaisesRegex(ValueError, "Guest retired"):
            self.drive("", child=Actor(0))

    def test_gateway_zero_requires_actual_worker_zero(self):
        with self.assertRaisesRegex(ValueError, "Gateway exited"):
            self.drive("COMPLETE\n", gateway=Actor(0))
        self.drive("COMPLETE\n", gateway=Actor(0), worker=Actor(0))

    def markers(self, mode, middle):
        common = " backend=2 pid=123 mode=" + mode + "\n"
        non_topic = 1 if mode in ("client", "server") else 0
        topic = 1 if mode == "topics" else 0
        return ("READY_BOUNDED_NATIVE" + common + "".join(name + common for name in middle)
                + "PASS_BOUNDED_NATIVE backend=2 pid=123 mode=" + mode
                + f" services={non_topic} empty={int(mode == 'server')} goals={non_topic}"
                + f" feedback={non_topic} result={non_topic} mismatch=0 topic_received={topic}"
                + f" topic_sent={topic} quiet_samples=30 quiet_ns=2000000000\n")

    def test_closed_native_marker_inventory_accepts_each_mode(self):
        middle = {"server": ["FEEDBACK_SENT_BOUNDED_NATIVE", "RESULT_SENT_BOUNDED_NATIVE"],
                  "client": ["FEEDBACK_DELIVERED_BOUNDED_NATIVE", "DELIVERED_BOUNDED_NATIVE"],
                  "topics": ["MATCHED_BOUNDED_NATIVE"], "absent": []}
        for mode, names in middle.items():
            self.namespace["native_markers"](self.markers(mode, names), 2, mode, 123)

    def test_missing_reordered_or_excess_native_feedback_is_refused(self):
        for names in ([], ["RESULT_SENT_BOUNDED_NATIVE", "FEEDBACK_SENT_BOUNDED_NATIVE"],
                      ["FEEDBACK_SENT_BOUNDED_NATIVE", "RESULT_SENT_BOUNDED_NATIVE", "RESULT_SENT_BOUNDED_NATIVE"]):
            with self.assertRaises(ValueError):
                self.namespace["native_markers"](self.markers("server", names), 2, "server", 123)
        text = self.markers("server", ["FEEDBACK_SENT_BOUNDED_NATIVE", "RESULT_SENT_BOUNDED_NATIVE"])
        with self.assertRaises(ValueError):
            self.namespace["native_markers"](text.replace("feedback=1", "feedback=2"), 2, "server", 123)


if __name__ == "__main__":
    unittest.main()
