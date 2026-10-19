#!/usr/bin/env python3
"""Read the real Source40 partial-build schema without inventing observations.

This module is a metadata predicate, not a compiler or a Runtime test.  The
caller must hash the complete original report before calling ``admit_batch``.
It must separately reopen every selected persistent input, product, depfile,
linker receipt, log and binary.  After a reboot, historical and newly opened
physical pins remain different facts; use ``requalify_after_restart`` for an
explicit device transition rather than changing an old receipt in memory.

The historical producer used ``Frame``.  Its report has ``source`` and
``source_manifest_sha256`` plus full manifest pins in ``input_pins``; it has no
``source_before`` member.  Its RSS-aborted command has no recorded returncode.
Neither missing member can be fabricated to satisfy a newer consumer.
"""
from __future__ import annotations

from pathlib import PurePosixPath
import re


REPORT_SHA256 = "b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747"
REPORT_BYTES = 3911428
SOURCE = "/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r40/source"
MANIFEST_SHA256 = "a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd"
MANIFEST_BYTES = 3404861
FULL_PIN_KEYS = frozenset(("path", "device", "inode", "bytes", "mtime_ns", "ctime_ns", "sha256"))
COMPLETE = "COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED"
COMPLETED_IDS = frozenset(language + "-" + role for language in
    ("rust", "javascript", "assemblyscript") for role in
    ("lowstate-go", "lowstate-hg", "topic-rpc-api"))
MIB = 1024 ** 2


def _need(condition, message):
    if not condition:
        raise ValueError(message)


def _pin(value, label, nonempty=True):
    """Validate an immutable historical pin's shape, never assert it is current."""
    _need(type(value) is dict and set(value) == FULL_PIN_KEYS,
          label + ": exact historical full-seven pin required")
    path = value["path"]
    _need(type(path) is str and path.startswith("/") and
          not any(item in path for item in ("\0", "\n", "\r")) and
          ".." not in PurePosixPath(path).parts, label + ": clean absolute path required")
    _need(all(type(value[key]) is int and value[key] >= 0 for key in
          ("device", "inode", "bytes", "mtime_ns", "ctime_ns")) and value["inode"] > 0,
          label + ": actual integer physical facts required")
    _need((not nonempty or value["bytes"] > 0) and type(value["sha256"]) is str and
          re.fullmatch("[0-9a-f]{64}", value["sha256"]) is not None,
          label + ": complete content identity required")
    return value


def _successful(command, label):
    """Require a genuinely completed original command, including its exit code."""
    _need(type(command) is dict and command.get("status") == "PASS" and
          type(command.get("returncode")) is int and command["returncode"] == 0 and
          type(command.get("argv")) is list and command["argv"] and
          type(command["argv"][0]) is str and command["argv"][0].startswith("/"),
          label + ": original successful command and recorded returncode0 required")


def admit_batch(report, opened_report_pin):
    """Select nine completed command records from the exact failed real batch.

    ``opened_report_pin`` is the caller's independently opened current report
    pin.  Only its actual path, size and complete SHA identify this immutable
    raw report; embedded historical device facts are retained unchanged.
    This predicate cannot qualify original files or promote the failed batch.
    """
    _pin(opened_report_pin, "current raw report")
    expected_report = str(PurePosixPath(SOURCE).parents[1] /
        "acceptance-storage-r1/source40-unitree-command14-build-r1/report.json")
    _need(opened_report_pin["path"] == expected_report and
          opened_report_pin["bytes"] == REPORT_BYTES and
          opened_report_pin["sha256"] == REPORT_SHA256,
          "the complete exact original failed Source40 report is required")
    _need(type(report) is dict and report.get("kind") == "uwvm-unitree-command14-guest-builds-v1" and
          report.get("status") == "FAIL" and report.get("format_version") == 1 and
          report.get("source") == SOURCE and report.get("source_manifest_sha256") == MANIFEST_SHA256 and
          report.get("error") == "ValueError('owned command RSS limit exceeded')" and
          "source_before" not in report and not report.get("finalization_errors") and
          all(report.get(key) is True for key in
              ("source_preserved", "input_pins_preserved", "whole_roots_preserved")),
          "the original preserved RSS-failed Frame report schema is required")
    _need(report.get("granted_rights") == [] and all(report.get(key) == "NOT_EXECUTED" for key in
          ("Wasm_initialization", "default_denial_cases", "positive_native_transport",
           "native_ROS", "simulator", "performance")),
          "retained compilation must not be relabelled as authority or Runtime acceptance")
    cleanup = report.get("cleanup")
    _need(type(cleanup) is list and cleanup and all(type(row) is dict and
          row.get("status") == "PASS" and not row.get("remaining") and not row.get("errors")
          for row in cleanup), "the original producer must have completed owned cleanup")
    pins = report.get("input_pins")
    _need(type(pins) is list and pins, "the original input pin ledger is required")
    manifest_path = str(PurePosixPath(SOURCE).parent / "source-manifest.json")
    manifest_pins = [row for row in pins if type(row) is dict and row.get("path") == manifest_path]
    _need(manifest_pins, "the original complete source manifest pin is required")
    historical_manifest = _pin(manifest_pins[0], "original full manifest")
    _need(all(row == historical_manifest for row in manifest_pins) and
          historical_manifest["bytes"] == MANIFEST_BYTES and
          historical_manifest["sha256"] == MANIFEST_SHA256,
          "all repeated original manifest observations must be identical")
    commands = report.get("commands")
    _need(type(commands) is list and len(commands) == 68,
          "all68 original compiler/helper/validator command records are required")
    failed = [row for row in commands if type(row) is dict and row.get("status") != "PASS"]
    _need(len(failed) == 1, "exactly one original command may have failed")
    failure = failed[0]
    _need(failure.get("label") == "tinygo-lowstate-go-same-main-command" and
          failure.get("status") == "FAIL" and
          failure.get("error") == "ValueError('owned command RSS limit exceeded')" and
          "returncode" not in failure and
          type(failure.get("peak_owned_rss_bytes")) is int and
          512 * MIB < failure["peak_owned_rss_bytes"] <= 768 * MIB,
          "retain the original RSS interruption; no process exit status was recorded")
    for command in commands:
        if command is not failure:
            _successful(command, "original command")
    products = report.get("products")
    _need(type(products) is list and len(products) == 10 and
          all(type(row) is dict for row in products),
          "the original inventory has nine completed products and one incomplete row")
    completed = [row for row in products if row.get("status") == COMPLETE]
    incomplete = [row for row in products if row.get("status") != COMPLETE]
    _need(len(completed) == 9 and {row.get("reactor_id") for row in completed} == COMPLETED_IDS and
          len(incomplete) == 1 and incomplete[0].get("reactor_id") == "tinygo-lowstate-go" and
          incomplete[0].get("status") == "RUNNING" and
          "artifact" not in incomplete[0] and "receipt" not in incomplete[0],
          "only the exact nine complete original products are eligible")
    for product in completed:
        identifier = product["reactor_id"]
        _need(product.get("id") == identifier + "-command" and
              product.get("language") == identifier.split("-", 1)[0] and
              product.get("granted_rights") == [] and
              product.get("future_transport_cases_executed") == 0 and
              product.get("cross_module_guard_modified") is False and
              product.get("preloaded_application") is False,
              "original product and language association differs")
        for field in ("source", "artifact", "receipt"):
            _pin(product.get(field), identifier + " " + field)
        _successful(product.get("validation"), identifier + " full validator")
        _need(type(product.get("commands")) is list and product["commands"],
              identifier + ": original compiler commands are required")
        for command in product["commands"]:
            _successful(command, identifier)
        entry = product.get("entry_compile")
        if entry is not None:
            _need(type(entry) is dict, identifier + ": original entry compile association required")
            _successful(entry.get("command"), identifier + " entry")
    _pin(report.get("configuration"), "original configuration")
    return {"status": "SCHEMA_ONLY_PERSISTENT_INPUTS_AND_BINARIES_REQUIRE_REOPEN",
            "raw_batch_status": "FAIL", "historical_manifest": historical_manifest,
            "configuration": report["configuration"],
            "completed": {row["reactor_id"]: row for row in completed},
            "incomplete_product_consumed": False, "batch_promoted_to_success": False,
            "missing_returncode_interpreted_as_exit_status": False}


def bind_current_source(report_schema, current_source_state):
    """Bind a complete current source_state result to the original manifest bytes.

    The caller must run the real complete ``common.source_state`` check first.
    A post-reboot physical transition must additionally be admitted by the
    separate restart bridge; matching content never asserts old device facts.
    """
    _need(report_schema.get("status") == "SCHEMA_ONLY_PERSISTENT_INPUTS_AND_BINARIES_REQUIRE_REOPEN" and
          type(current_source_state) is dict and current_source_state.get("root") == SOURCE and
          current_source_state.get("files") == 17928,
          "the actual complete current Source40 state is required")
    current = _pin(current_source_state.get("manifest"), "current complete manifest")
    historical = report_schema["historical_manifest"]
    _need(all(current[key] == historical[key] for key in ("path", "bytes", "sha256")),
          "complete current source must match the original manifest content")
    return {"status": "CURRENT_COMPLETE_SOURCE_BYTES_MATCH_ORIGINAL_MANIFEST",
            "historical_manifest": historical, "current_manifest": current,
            "files": current_source_state["files"], "historical_report_rewritten": False,
            "persistent_inputs_requalified": False, "runtime_execution_qualified": False}
