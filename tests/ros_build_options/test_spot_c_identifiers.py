#!/usr/bin/env python3
"""Regressions for the original-protobuf to C member naming boundary.

Run this file with an explicitly selected Python interpreter. These tests do
not invoke protoc, change generated products, open a ROS resource or claim
that a source-generation receipt is an executable SDK qualification.
"""
from __future__ import annotations

import importlib.util
from pathlib import Path
import unittest


ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location(
    "spot_source_producer",
    ROOT / "sdk/robots/mobile_manipulation/spot/protobuf/prepare.py",
)
PRODUCER = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PRODUCER)


class OriginalMemberNaming(unittest.TestCase):
    def test_real_spot_keyword_regression(self):
        """All five offending original members remain usable from C/C++."""
        original = ["double", "int", "bool", "switch", "const", "timestamp"]
        self.assertEqual(PRODUCER.c_identifiers(original), {
            "double": "double_", "int": "int_", "bool": "bool_",
            "switch": "switch_", "const": "const_", "timestamp": "timestamp",
        })

    def test_existing_original_identifier_is_never_overwritten(self):
        names = ["double", "double_", "double__", "robot_state"]
        result = PRODUCER.c_identifiers(names)
        self.assertEqual(result["double"], "double___")
        for name in names[1:]:
            self.assertEqual(result[name], name)
        self.assertEqual(len(set(result.values())), len(names))

    def test_generated_discriminator_presence_and_size_helpers(self):
        """A renamed field must not alias a real generated helper member."""
        for helper in ("which_int_", "has_int_", "int__count", "int__size"):
            with self.subTest(helper=helper):
                result = PRODUCER.c_identifiers(["int", helper])
                self.assertEqual(result["int"], "int__")
                self.assertEqual(result[helper], helper)

    def test_source_order_and_repeated_names_do_not_change_the_policy(self):
        names = ["bool", "value", "double_", "double", "has_bool_"]
        expected = PRODUCER.c_identifiers(names)
        self.assertEqual(PRODUCER.c_identifiers(reversed(names)), expected)
        self.assertEqual(PRODUCER.c_identifiers(names + names), expected)

    def test_invalid_or_unbounded_original_names_are_refused(self):
        for name in ("", "9value", "a.b", "a/b", "a\x00b", "a" * 257):
            with self.subTest(name=repr(name)):
                with self.assertRaises(ValueError):
                    PRODUCER.c_identifiers([name])
        with self.assertRaises(ValueError):
            PRODUCER.c_identifiers("field_%d" % i for i in range(16385))


if __name__ == "__main__":
    unittest.main(verbosity=2)
