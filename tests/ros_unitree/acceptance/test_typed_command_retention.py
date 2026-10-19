#!/usr/bin/env python3
"""Regress the genuine failed-batch schema inside Root's current Linux cgroup.

Invoke this source only after Root has pinned this file and both sibling
helpers in its current owned control closure.  Supply the unmodified actual
Source40 report and explicit current admission selections; no report fixture,
exit status, old PID or filesystem identity is manufactured.  Negative cases
mutate in-memory copies only.  These are metadata-predicate regressions, never
retained-file, compiler, Wasm, native ROS, physics or performance acceptance.
No work starts when this module is imported.
"""
from __future__ import annotations

import argparse
import copy
import json
import os
from pathlib import Path
import sys
import unittest

from requalify_after_restart import CurrentGuard, _document
from typed_command_retention import (COMPLETED_IDS, REPORT_SHA256, SOURCE,
                                     admit_batch, bind_current_source)


class ActualFailedBatchSchema(unittest.TestCase):
    """A genuine raw receipt plus explicit contradictory-metadata regressions."""
    report = None
    pin = None
    guard = None

    def setUp(self):
        self.guard()
        self.value = copy.deepcopy(self.report)

    def admit(self):
        return admit_batch(self.value, self.pin)

    def test_actual_missing_source_before_is_the_original_schema(self):
        self.assertNotIn("source_before", self.value)
        self.assertEqual(set(self.admit()["completed"]), COMPLETED_IDS)

    def test_actual_rss_interruption_has_no_recorded_exit_status(self):
        failed = next(row for row in self.value["commands"] if row["status"] == "FAIL")
        self.assertNotIn("returncode", failed)
        self.assertFalse(self.admit()["missing_returncode_interpreted_as_exit_status"])

    def test_an_invented_source_before_observation_is_rejected(self):
        self.value["source_before"] = {}
        with self.assertRaises(ValueError):
            self.admit()

    def test_an_invented_null_exit_status_is_rejected(self):
        next(row for row in self.value["commands"] if row["status"] == "FAIL")["returncode"] = None
        with self.assertRaises(ValueError):
            self.admit()

    def test_batch_status_cannot_be_promoted(self):
        self.value["status"] = "PASS"
        with self.assertRaises(ValueError):
            self.admit()

    def test_incomplete_tinygo_cannot_be_counted_as_a_product(self):
        self.value["products"][-1]["status"] = self.value["products"][0]["status"]
        with self.assertRaises(ValueError):
            self.admit()

    def test_another_failed_command_is_rejected(self):
        self.value["commands"][0]["status"] = "FAIL"
        with self.assertRaises(ValueError):
            self.admit()

    def test_success_without_recorded_returncode_is_rejected(self):
        del self.value["commands"][0]["returncode"]
        with self.assertRaises(ValueError):
            self.admit()

    def test_missing_original_manifest_pin_is_rejected(self):
        self.value["input_pins"] = [row for row in self.value["input_pins"]
            if row["path"] != str(Path(SOURCE).parent / "source-manifest.json")]
        with self.assertRaises(ValueError):
            self.admit()

    def test_conflicting_repeated_manifest_observation_is_rejected(self):
        row = copy.deepcopy(next(row for row in self.value["input_pins"]
            if row["path"] == str(Path(SOURCE).parent / "source-manifest.json")))
        row["inode"] += 1
        self.value["input_pins"].append(row)
        with self.assertRaises(ValueError):
            self.admit()

    def test_failed_cleanup_is_rejected(self):
        self.value["cleanup"][0]["status"] = "FAIL"
        with self.assertRaises(ValueError):
            self.admit()

    def test_build_receipt_cannot_supply_runtime_authority(self):
        self.value["products"][0]["granted_rights"] = ["topic.publish"]
        with self.assertRaises(ValueError):
            self.admit()

    def test_current_report_hash_is_mandatory(self):
        pin = {**self.pin, "sha256": "0" * 64}
        with self.assertRaises(ValueError):
            admit_batch(self.value, pin)

    def test_source_binding_requires_the_complete_actual_state(self):
        with self.assertRaises(ValueError):
            bind_current_source(self.admit(), {"root": SOURCE})


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ("source", "manifest", "cgroup-selection", "report"):
        parser.add_argument("--" + name, type=Path, required=True)
    for name in ("manifest-sha256", "cgroup-selection-sha256"):
        parser.add_argument("--" + name, required=True)
    args = parser.parse_args()
    if sys.platform != "linux" or os.geteuid() != 0:
        raise ValueError("Root's actual current Linux cgroup is required")
    frozen, _ = _document(str(args.manifest), args.manifest_sha256, lambda: None)
    selection, _ = _document(str(args.cgroup_selection), args.cgroup_selection_sha256, lambda: None)
    guard = CurrentGuard(str(args.source), frozen, selection)
    report, pin = _document(str(args.report), REPORT_SHA256, guard)
    ActualFailedBatchSchema.report, ActualFailedBatchSchema.pin = report, pin
    ActualFailedBatchSchema.guard = staticmethod(guard)
    result = unittest.TextTestRunner(verbosity=2).run(
        unittest.defaultTestLoader.loadTestsFromTestCase(ActualFailedBatchSchema))
    guard.force()
    print(json.dumps({"kind": "typed-command-failed-batch-schema-regression",
        "status": "PASS_METADATA_PREDICATES_ONLY" if result.wasSuccessful() else "FAIL",
        "tests": result.testsRun, "source40_report": pin,
        "compilers_started": False, "Runtime_executed": False,
        "persistent_products_requalified": False, "historical_receipts_changed": False}))
    return 0 if result.wasSuccessful() else 1


if __name__ == "__main__":
    raise SystemExit(main())
