#!/usr/bin/env python3
"""Exercise bounded output monitoring without running a Guest or ROS process.

Run this small policy suite only through the selected Linux cgroup test owner.
TMPDIR must name that owner's writable work directory. All real files used by
the tests live below TemporaryDirectory; source and SDK trees are never changed.
The fake cgroup/free-space providers isolate the monitoring policy. Their values
are not cgroup, filesystem-capacity, ROS, or Runtime qualification evidence.
"""
from pathlib import Path
import importlib.util
import tempfile
import types
import unittest
from unittest import mock


spec = importlib.util.spec_from_file_location(
    "unitree_output_budget_policy", Path(__file__).with_name("common.py"))
common = importlib.util.module_from_spec(spec)
spec.loader.exec_module(common)


class OutputBudgetGuardTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="unitree-budget-policy-")
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name).resolve()
        self.out = self.root / "out"
        self.out.mkdir()
        self.input_root = self.root / "input"
        self.input_root.mkdir()
        self.current_cgroup_valid = True
        self.available = 16 * 1024**3
        self.cgroup_calls = 0

        def cgroup_state(_):
            self.cgroup_calls += 1
            return self.current_cgroup_valid

        def cgroup_preserved(before, after):
            common.require(before is True and after is True, "test cgroup changed")

        # This narrow fake supplies only the boundary/document operations that
        # Frame.guard/finish need. Output bytes and aliases are actual files.
        provider = types.SimpleNamespace(
            cgroup_state=cgroup_state, cgroup_preserved=cgroup_preserved,
            source_state=lambda *_: {"same_source": True},
            catalog=lambda *_: {"unchanged_input": True},
            atomic_json=lambda path, value: path.write_text(common.json.dumps(value)))
        self.frame = object.__new__(common.Frame)
        self.frame.common = provider
        self.frame.before = True
        self.frame.config = {"cgroup": {}}
        self.frame.out, self.frame.roots = self.out, [self.input_root]
        self.frame.limits = {"output_bytes": 32}
        self.frame.actors, self.frame.pins = [], []
        self.frame.source, self.frame.manifest = self.input_root, self.input_root / "manifest"
        self.frame.source_before = {"same_source": True}
        self.frame.catalogs = [{"unchanged_input": True}]
        self.frame.report = {}
        self.clock = mock.patch.object(common.time, "monotonic", return_value=100.0)
        self.clock.start()
        self.addCleanup(self.clock.stop)
        self.space = mock.patch.object(common.os, "statvfs", side_effect=lambda _: types.SimpleNamespace(
            f_bavail=self.available, f_frsize=1))
        self.statvfs_mock = self.space.start()
        self.addCleanup(self.space.stop)

    def test_dense_catalog_guards_share_only_the_output_walk(self):
        """Thousands of streamed rows must not trigger thousands of tree walks."""
        with mock.patch.object(common.os, "walk", wraps=common.os.walk) as walk:
            for _ in range(2048):
                self.frame.guard()
            self.assertEqual(walk.call_count, 1)
        self.assertEqual(self.cgroup_calls, 2048)
        self.assertEqual(self.statvfs_mock.call_count, 2048 * (1 + len(self.out.parents)))

    def test_cgroup_and_backing_fail_during_cached_output_interval(self):
        self.frame.guard()
        self.current_cgroup_valid = False
        with self.assertRaisesRegex(ValueError, "cgroup changed"):
            self.frame.guard()
        self.current_cgroup_valid = True
        self.available = 8 * 1024**3 - 1
        with self.assertRaisesRegex(ValueError, "eight-GiB"):
            self.frame.guard()

    def test_expired_interval_reopens_actual_output_bytes(self):
        self.frame.guard()
        (self.out / "late-output").write_bytes(b"x" * 33)
        with mock.patch.object(common.time, "monotonic", return_value=100.5):
            with self.assertRaisesRegex(ValueError, "output budget"):
                self.frame.guard()

    def test_finish_cannot_reuse_earlier_under_budget_result(self):
        self.frame.guard()
        (self.out / "late-output").write_bytes(b"x" * 33)
        self.assertEqual(self.frame.finish("PASS_POLICY_TEST_ONLY"), 1)
        self.assertEqual(self.frame.report["status"], "FAIL")
        self.assertTrue(any("output budget" in message
                            for message in self.frame.report["finalization_errors"]))

    def test_terminal_scan_rejects_new_unselected_alias(self):
        self.frame.guard()
        outside = self.root / "unselected"
        outside.write_bytes(b"x")
        (self.out / "late-alias").symlink_to(outside)
        self.assertEqual(self.frame.finish("PASS_POLICY_TEST_ONLY"), 1)
        self.assertTrue(any("alias escapes" in message
                            for message in self.frame.report["finalization_errors"]))


if __name__ == "__main__":
    unittest.main()
