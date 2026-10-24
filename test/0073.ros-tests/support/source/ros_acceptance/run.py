#!/usr/bin/env python3
"""Run the entire same-freeze acceptance inventory serially, retaining failures.

The launch manifest explicitly selects scripts, exact argv, prerequisites and
current receipt paths. It is not a permission preset. Existing suites and the
new gap cases are both mandatory. No historical result or preparation status
is reused as actual behavior; a missing original50/55/20 mapping keeps the
final report incomplete. Source/SDK/compiler products are frozen externally
before this runner begins; root owns the one actual Linux lane.
"""
import argparse
from pathlib import Path
import sys
import traceback

sys.dont_write_bytecode = True
from lane import Lane, add_arguments
from protocol import CASES, require, physical_pin, load_bound, side_effect
from requirements import inventories
from gateway_qualification import architecture_binding

# Only in-tree actual drivers can be launched. No arbitrary shell/command,
# executable supplied by a guest, remote launcher or mutable report is accepted.
DRIVERS = frozenset((
    "test/0073.ros-tests/support/source/ros_aarch64/build_worker.py", "test/0073.ros-tests/support/source/ros_aarch64/rebuild_affected.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_all.py", "test/0073.ros-tests/support/source/ros_aarch64/run_native.py", "test/0073.ros-tests/support/source/ros_aarch64/run_cli.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_regression.py", "test/0073.ros-tests/support/source/ros_aarch64/run_extended.py", "test/0073.ros-tests/support/source/ros_aarch64/run_unit.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_ros1_search.py", "test/0073.ros-tests/support/source/ros_aarch64/run_resources.py", "test/0073.ros-tests/support/source/ros_aarch64/run_action_qos.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_local_parameter_events.py", "test/0073.ros-tests/support/source/ros_aarch64/run_g09_guest_enforce.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_revoke_reconnect.py", "test/0073.ros-tests/support/source/ros_aarch64/run_custom_topics.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_custom_guest.py", "test/0073.ros-tests/support/source/ros_acceptance/prepare_native.py",
    "test/0073.ros-tests/support/source/ros_acceptance/run_delivery.py", "test/0073.ros-tests/support/source/ros_acceptance/run_recovery.py",
    "test/0073.ros-tests/support/source/ros_acceptance/run_attacks.py", "test/0073.ros-tests/support/source/ros_acceptance/run_metrics.py",
    "test/0073.ros-tests/support/source/ros_acceptance/run_bounded_service.py", "test/0073.ros-tests/support/source/ros_aarch64/run_native_adoption.py",
    "test/0073.ros-tests/support/source/ros_acceptance/build_delivery.py", "test/0073.ros-tests/support/source/ros_acceptance/normalize.py",
    "test/0073.ros-tests/support/source/ros_acceptance/run_events.py",
    "test/0073.ros-tests/support/source/ros_acceptance/run_sdk_matrix.py",
))
BASE_SUITES = frozenset(("worker-four-profiles", "native-original", "mandatory-cli", "original-regressions",
    "original-security", "payload-security", "ingress-trap", "revoke-race", "flood", "five-modules", "dual-network",
    "ros1-search", "ros2-resources", "action-policy", "action-native-qos40", "seven-events",
    "custom-types", "custom-guest", "signed-enforce", "revoke-reconnect", "wasi-regression", "trusted-dl-regression",
    "runtime-regression", "bounded-service", "native-versus-wasm-performance", "unified-event-contracts"))
SUPPLEMENTAL_SCOPES = frozenset(("sdk-complete-matrix",))
SDK_SCOPE = "sdk-compiler-model-supplement"


def launch_contract(value, lane):
    require(value.get("format_version") == 1 and type(value.get("format_version")) is int
            and value.get("kind") == "ros-final-serial-launches"
            and value.get("source_manifest_sha256") == lane.report["source_manifest_sha256"], "same-freeze closed launch manifest required")
    rows = value["launches"]; labels = [row["id"] for row in rows]
    require(rows and len(labels) == len(set(labels)) and all(label and all(c.isalnum() or c == "-" for c in label) for label in labels), "unique launch labels required")
    available = set()
    for row in rows:
        require(row["driver"] in DRIVERS and row["driver"] in lane.frozen, "unapproved/unfrozen test driver")
        require(row["requires"] == sorted(set(row["requires"])) and set(row["requires"]) <= available, "serial dependency order differs")
        require(type(row["timeout_seconds"]) is int and 1 <= row["timeout_seconds"] <= 7200
                and type(row["rss_bytes"]) is int and 1 <= row["rss_bytes"] <= 8*1024**3, "bounded launch controls required")
        argv = row["arguments"]
        require(isinstance(argv, list) and all(isinstance(arg, str) and "\x00" not in arg for arg in argv), "closed argv array required")
        # Every driver gets this exact current source/manifest. Output paths are
        # dedicated to the new row and cannot overwrite earlier failed reports.
        for flag, expected in (("--source", str(lane.source)), ("--manifest", str(lane.args.manifest.resolve())),
                               ("--out", str(lane.out/row["id"]))):
            require(argv.count(flag) == 1 and argv[argv.index(flag)+1] == expected, "driver source/output selection differs")
        report = Path(row["report"])
        require(report.is_absolute() and report.is_relative_to(lane.out/row["id"]), "row report escapes owned output")
        available.add(row["id"])
        if "evidence_scope" in row:
            require(row["evidence_scope"]==SDK_SCOPE and row["driver"]=="test/0073.ros-tests/support/source/ros_acceptance/normalize.py",
                    "only the closed normalized SDK supplement has non-ROS execution scope")
    require(set(value["base_suites"]) == BASE_SUITES and set(value["gap_cases"]) == set(CASES), "complete base plus gap inventory required")
    for name in (*BASE_SUITES, *CASES):
        require(name in value["coverage"] and value["coverage"][name]
                and set(value["coverage"][name]) <= available, "mandatory suite/case mapping missing")
    require(set(value.get("supplemental_scopes",[]))==SUPPLEMENTAL_SCOPES and
        set(value.get("supplemental_coverage",{}))==SUPPLEMENTAL_SCOPES,"mandatory SDK supplement missing")
    for name in SUPPLEMENTAL_SCOPES:
        selected=value["supplemental_coverage"][name]
        require(selected and len(selected)==len(set(selected)) and set(selected)<=available and
            all(next(row for row in rows if row["id"]==label).get("evidence_scope")==SDK_SCOPE for label in selected),
            "SDK supplemental mapping must name explicit host/compiler-model rows")
    return rows


def terminal_receipt(lane, launch):
    """Bind current real execution plus architecture, never an old PASS label."""
    value, pin = lane.json(launch["report"])
    sdk=launch.get("evidence_scope")==SDK_SCOPE
    require(value.get("source_manifest_sha256") == lane.report["source_manifest_sha256"]
            and value.get("status") == "PASS" and value.get("mode") != "prepare"
            and (value.get("actual_ROS_delivery") != "NOT_EXECUTED" if not sdk else
                value.get("evidence_scope")==SDK_SCOPE and value.get("supplemental_scope")=="sdk-complete-matrix"
                and value.get("actual_ROS_delivery")=="NOT_EXECUTED" and value.get("actual_ARM_Runtime_execution")=="NOT_EXECUTED"),
            "current actual terminal receipt or explicit non-ROS SDK supplement required")
    reference = value.get("architecture_evidence")
    require(isinstance(reference, dict), "raw architecture reference required")
    architecture, binding = architecture_binding(reference["path"], lane.pins)
    require(binding["sha256"] == reference["sha256"] and architecture["cgroup_incarnation_before"] == lane.before,
            "row architecture differs from current outer incarnation")
    return value, pin


def original_coverage(launch_manifest, receipts, requirement_sets):
    """Require every original ID, explicit actual probes, and honest statuses.

    Each mapping names a physical report and exact inner row, not a paragraph
    claiming coverage. Security mappings additionally bind an independently
    measured zero-effect record. Example mappings bind all seven material
    fields and actual Guest/independent-native execution. Final criteria may
    need multiple rows; none is inferred from a neighbouring criterion.
    """
    result = {}
    for category, inventory in requirement_sets.items():
        mapping = launch_manifest["original_requirements"][category]
        require(set(mapping) == {str(row["id"]) for row in inventory}, "original requirement mapping incomplete")
        result[category] = []
        for item in inventory:
            evidence = mapping[str(item["id"])]; require(evidence, "empty original requirement evidence")
            resolved = []
            for reference in evidence:
                require(reference["launch"] in receipts, "original requirement missing actual launch")
                report, pin = receipts[reference["launch"]]; rows = report.get("acceptance_records", [])
                selected = [row for row in rows if row.get("id") == reference["record"]]
                require(len(selected) == 1 and selected[0].get("status") in ("PASS","QUALIFIED_SUPPORTED_SCOPE"), "exact original record missing")
                row = selected[0]
                require(row.get("kind")!="sdk-compiler-model","SDK mock/compiler evidence cannot replace original ROS/Runtime/material crosswalks")
                if row["status"]=="QUALIFIED_SUPPORTED_SCOPE":
                    require(reference.get("bounded_unsupported_allowed") is True and row.get("expected_errno")==95
                        and row.get("unsupported_reason") and row.get("actual_execution") is True,
                        "bounded unsupported scope was not explicitly allowed by this original criterion")
                if category == "outputs":
                    require(row.get("materials_verified") is True and row.get("physical_material_pins"),
                            "exact delivered material input pins missing")
                elif reference.get("evidence_role")=="material":
                    require(category=="acceptance" and item["id"] in (46,47,48,49,50)
                        and row.get("materials_verified") is True and row.get("physical_material_pins"),
                        "material evidence cannot replace this original behavior requirement")
                else:
                    require(row.get("actual_execution") is True, "source checks cannot replace actual behavior")
                if category == "security":
                    if "expected_errno" in row:side_effect(row)
                    else:
                        # Original attacks include positive memory growth,
                        # two permitted flood effects, revoke and Trap cleanup.
                        # They are not all zero-effect denial experiments.
                        contract=row.get("security_contract",{})
                        require(contract.get("kind") in ("guest-refusal-and-native-assertions","startup-refusal",
                            "bounded-allowed-counts","retained-memory-growth","owned-lifetime","native-concurrency")
                            and contract.get("actual_assertions_verified") is True and contract.get("assertions")
                            and contract.get("scope") and row.get("actual_execution") is True,
                            "exact actual security experiment contract missing")
                if category == "examples":
                    require(all(row.get(key) for key in ("guest_source", "compile_argv", "runtime_argv", "grants", "limits", "expected_output", "native_test"))
                            and row.get("Guest_exit") == 0 and row.get("independent_native_verified") is True,
                            "example seven-part material/real behavior incomplete")
                resolved.append({"report": pin, "record": row})
            qualified=any(item["record"]["status"]=="QUALIFIED_SUPPORTED_SCOPE" for item in resolved)
            result[category].append({**item, "status": "QUALIFIED_SUPPORTED_SCOPE" if qualified else "PASS", "evidence": resolved})
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__); add_arguments(parser, workers=False)
    parser.add_argument("--launch-manifest", type=Path, required=True); args = parser.parse_args()
    lane = None; status = "FAIL"
    try:
        lane = Lane(args, "ros-final-serial-acceptance"); plan, plan_pin = lane.json(args.launch_manifest)
        rows = launch_contract(plan, lane); lane.report["launch_manifest"] = plan_pin
        lane.report["requirements"] = inventories(lane.source); lane.report["original_task_complete"] = False
        receipts = {}; failures = set()
        for launch in rows:
            child=None;command=None
            record = {"id": launch["id"], "status": "RUNNING"}; lane.report["cases"].append(record); lane.save()
            if set(launch["requires"]) & failures:
                record.update(status="BLOCKED", reason="current prerequisite failed; historical receipt not substituted")
                failures.add(launch["id"]); lane.save(); continue
            try:
                child, command = lane.start(launch["id"], ["/usr/bin/unshare", "-m", "--",sys.executable, "-B", str(lane.source/launch["driver"]), *launch["arguments"]],
                    seconds=launch["timeout_seconds"], rss=launch["rss_bytes"])
                lane.finish(child, command); receipts[launch["id"]] = terminal_receipt(lane, launch)
                record.update(status="PASS", report=receipts[launch["id"]][1])
            except BaseException as error:
                record.update(status="FAIL", error=repr(error)); failures.add(launch["id"])
                # Independent following rows still run. A failed actor must be
                # retired before serial resource leases can be reused.
                if child is not None and child.poll() is None: lane.stop_role(child, command)
            lane.save()
        require(not failures, "one or more current serial acceptance rows failed or blocked")
        lane.report["original_requirements"] = original_coverage(plan, receipts, lane.report["requirements"])
        supplement={}
        for name in SUPPLEMENTAL_SCOPES:
            supplement[name]=[]
            for label in plan["supplemental_coverage"][name]:
                report,pin=receipts[label];records=report.get("acceptance_records",[])
                require(records and all(row.get("kind")=="sdk-compiler-model" and row.get("status")=="PASS" and
                    row.get("actual_host_execution") is True and row.get("actual_execution") is False and
                    row.get("actual_ROS_delivery")=="NOT_EXECUTED" and row.get("actual_ARM_Runtime_execution")=="NOT_EXECUTED"
                    for row in records),"mandatory complete SDK supplement failed or overstated ROS scope")
                supplement[name].append({"report":pin,"records":records,"scope":SDK_SCOPE})
        lane.report["mandatory_supplemental_results"]=supplement
        lane.report["original_task_complete"] = True; lane.report["new_tests_executed"] = True; status = "PASS"
    except BaseException as error:
        if lane is not None: lane.report.update(error=repr(error), traceback=traceback.format_exc())
        else: raise
    finally:
        if lane is not None:
            status = lane.close(status)
            if status != "PASS": lane.report["original_task_complete"] = False; lane.save()
    return 0 if status == "PASS" else 1


if __name__ == "__main__": raise SystemExit(main())
