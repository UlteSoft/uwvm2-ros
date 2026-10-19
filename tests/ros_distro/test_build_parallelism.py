#!/usr/bin/env python3
"""Exercise the resource boundaries that decide native build concurrency.

Run this suite inside the admitted Linux cgroup after publishing a byte-pinned
source snapshot. It deliberately supplies resource snapshots to the pure
planner: these are policy tests, not measurements of host RAM or ROS behavior.
A separate real CMake build verifies the selected argv and process ownership.
"""
from __future__ import annotations

from copy import deepcopy
from pathlib import Path
import sys
import unittest
from unittest.mock import mock_open, patch

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/ros_distro"))
import build_parallelism as policy

GIB = 1024**3
MIB = 1024**2


class BuildParallelismPolicy(unittest.TestCase):
    """Each limit independently restricts a requested eight-worker build."""

    def setUp(self):
        self.snapshot = {
            "memory_max": 64 * GIB, "memory_swap_max": 0,
            "memory_current_guard": 60 * GIB, "memory_current": 20 * GIB,
            "cpus": "0-31", "ancestors": [
                {"path": "/admitted/leaf", "cpu_max": "max 100000"},
                {"path": "/admitted", "cpu_max": "max 100000"},
                {"controllerless_hierarchy_root": {"path": "/"}},
            ],
        }
        self.limits = {"jobs": 8, "rss_bytes": 4 * GIB}
        self.affinity = set(range(32))

    def choose(self, environment=None, host=80 * GIB):
        return policy.plan(self.snapshot, self.limits, environment or {}, self.affinity, host)

    def test_aggregate_budget_prevents_cpu_only_fanout(self):
        result = self.choose()
        self.assertEqual(result["jobs"], 2)
        self.assertEqual(result["aggregate_owned_rss_limit_bytes"], 4 * GIB)
        self.assertEqual(result["per_job_reservation_bytes"], 2 * GIB)

    def test_cgroup_headroom_reduces_to_one(self):
        self.snapshot["memory_current"] = 57 * GIB
        self.assertEqual(self.choose()["jobs"], 1)

    def test_host_reserve_is_an_additional_cap(self):
        self.assertEqual(self.choose(host=10 * GIB)["jobs"], 1)

    def test_no_job_is_launched_without_a_reservation(self):
        for current, host in [(59 * GIB, 80 * GIB), (20 * GIB, 8 * GIB)]:
            with self.subTest(current=current, host=host):
                self.snapshot["memory_current"] = current
                with self.assertRaises(ValueError):
                    self.choose(host=host)

    def test_environment_cannot_raise_profile_cap(self):
        self.limits["jobs"] = 1
        self.assertEqual(self.choose({"UWVM_ROS_BUILD_MAX_JOBS": "64"})["jobs"], 1)
        self.limits["jobs"] = 8
        self.assertEqual(self.choose({"UWVM_ROS_BUILD_MAX_JOBS": "1"})["jobs"], 1)

    def test_heavier_translation_unit_reduces_concurrency(self):
        result = self.choose({"UWVM_ROS_BUILD_JOB_RSS_BYTES": str(3 * GIB)})
        self.assertEqual(result["jobs"], 1)
        with self.assertRaises(ValueError):
            self.choose({"UWVM_ROS_BUILD_JOB_RSS_BYTES": str(5 * GIB)})

    def test_fractional_ancestor_cpu_quota_rounds_down(self):
        self.snapshot["ancestors"][1]["cpu_max"] = "150000 100000"
        self.assertEqual(self.choose()["jobs"], 1)

    def test_actual_affinity_restricts_pinned_cpuset(self):
        self.affinity = {7}
        self.assertEqual(self.choose()["jobs"], 1)
        self.affinity = {40}
        with self.assertRaises(ValueError):
            self.choose()

    def test_default_one_job_profiles_stay_serial(self):
        self.limits["jobs"] = 1
        self.assertEqual(self.choose()["jobs"], 1)

    def test_malformed_or_unbounded_controls_are_refused(self):
        for value in ["0", "-1", "+2", "2 ", "1e2", "65", "01"]:
            with self.subTest(value=value), self.assertRaises(ValueError):
                self.choose({"UWVM_ROS_BUILD_MAX_JOBS": value})
        for value in [str(32 * MIB), str(61 * GIB)]:
            with self.subTest(value=value), self.assertRaises(ValueError):
                self.choose({"UWVM_ROS_BUILD_JOB_RSS_BYTES": value})

    def test_changed_hard_limits_or_working_guard_are_refused(self):
        original = deepcopy(self.snapshot)
        for key, value in [("memory_max", 63 * GIB), ("memory_swap_max", GIB),
                           ("memory_current_guard", 64 * GIB)]:
            with self.subTest(key=key):
                self.snapshot = {**original, key: value}
                with self.assertRaises(ValueError):
                    self.choose()

    def test_boolean_job_count_is_not_an_integer_budget(self):
        self.limits["jobs"] = True
        with self.assertRaises(ValueError):
            self.choose()

    def test_memavailable_requires_one_actual_unambiguous_field(self):
        for raw in [b"MemTotal: 100 kB\n", b"MemAvailable: 10 kB\nMemAvailable: 20 kB\n"]:
            with self.subTest(raw=raw), patch.object(Path, "open", mock_open(read_data=raw)):
                with self.assertRaises(ValueError):
                    policy.current_host_memory_available()
        with patch.object(Path, "open", mock_open(read_data=b"MemAvailable: 12345 kB\n")):
            self.assertEqual(policy.current_host_memory_available(), 12345 * 1024)

    def test_selected_four_jobs_preserve_all_input_budgets(self):
        before = deepcopy(self.snapshot), deepcopy(self.limits)
        result = self.choose({"UWVM_ROS_BUILD_MAX_JOBS": "4",
                              "UWVM_ROS_BUILD_JOB_RSS_BYTES": str(GIB)})
        self.assertEqual(result["jobs"], 4)
        self.assertEqual((self.snapshot, self.limits), before)


if __name__ == "__main__":
    unittest.main(verbosity=2)
