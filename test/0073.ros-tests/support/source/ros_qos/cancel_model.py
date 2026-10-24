#!/usr/bin/env python3
"""Check source ordering and independent cancellation models without ROS execution.

This program reads source and runs a small Python state model. It does not parse
C++ semantically, load a ROS library, execute a Guest, compile anything, or
qualify middleware behavior. Always write a new report and retain prior results.
"""
from __future__ import annotations
import argparse
import copy
import hashlib
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[5]
FILES = [
    "src/ros/gateway/ros2/action.h", "src/ros/gateway/ros2/qos.h",
    "src/uwvm2/imported/ros/common/action_qos_contract.h",
    "test/0073.ros-tests/support/source/ros_qos/cancel_matrix.json", "test/0073.ros-tests/support/source/ros_qos/cancel_matrix.h",
    "test/0073.ros-tests/support/source/ros_qos/cancel_contract.cc", "test/0073.ros-tests/support/source/ros_qos/cancel_native_ros2.cc",
    "test/0073.ros-tests/support/source/ros_qos/cancel_model.py", "test/0073.ros-tests/support/source/ros_qos/cancel_README.md",
    "test/0073.ros-tests/support/source/ros_qos/cancel_driver_models.py", "test/0073.ros-tests/support/source/ros_aarch64/run_action_qos.py",
    "test/0073.ros-tests/support/source/ros_aarch64/run_all.py", "test/0073.ros-tests/support/source/ros_aarch64/build_worker.py",
]


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", required=True, type=Path,
                        help="New JSON report path; an existing report is never replaced.")
    args = parser.parse_args()
    if args.out.exists():
        parser.error("report already exists; choose a fresh path")
    checks: list[str] = []

    def check(value: bool, label: str) -> None:
        if not value:
            raise AssertionError(label)
        checks.append(label)

    matrix = json.loads((ROOT / FILES[3]).read_text())["fields"]
    keys = [row["key"] for row in matrix]
    check(len(keys) == len(set(keys)) == 8, "independent cancellation matrix has eight unique fields")
    helper = (ROOT / FILES[2]).read_text()
    array = re.search(r"action_cancel_qos_fields\{(.*?)\};", helper, re.S)
    check(array is not None, "production cancellation field array exists")
    check(re.findall(r'"([^\"]+)"', array.group(1)) == keys, "production fields match the independent matrix")
    fixture = (ROOT / FILES[4]).read_text()
    cpp_rows = re.findall(r'\{"([^"]+)","([^"]+)","([^"]+)",(true|false)\}', fixture)
    check(cpp_rows == [(r["key"], r["baseline"], r["mutation"], str(r["individually_valid"]).lower()) for r in matrix],
          "prepared C++ fixture has the independent JSON expectations")
    action = (ROOT / FILES[0]).read_text()
    cancel = action[action.index("ipc::call_result cancel("):action.index("\n    public:", action.index("ipc::call_result cancel("))]
    preflight = cancel.index("ros::equal_action_cancel_qos_contract")
    check(cancel.index("selected.empty()") < preflight, "ownership selection precedes preflight")
    check(cancel.index('limit(cap,"max-active-goals")') < preflight, "selection quota precedes preflight")
    check(cancel.index("bool accepted{};") > preflight, "full preflight precedes native delivery loop")
    check(cancel.index("client->async_cancel_goal") > preflight, "outgoing native cancellation follows preflight")
    check("goal->active&&!goal->terminal&&!goal->incoming&&" in cancel[:preflight],
          "pending outgoing Goals are checked without requiring a native handle")
    actual = cancel.index("ros2_qos::client(*cancel_client,configured)")
    check(actual < cancel.index("cancel_client->wait_for_service"), "incoming actual QoS qualification precedes discovery")
    check(actual < cancel.index("cancel_client->async_send_request"), "incoming actual QoS qualification precedes native request")
    check('#include "qos.h"' in action, "standalone Action header includes actual-QoS helper")

    baseline = {row["key"]: row["baseline"] for row in matrix}

    def goal(contract=None, *, active=True, terminal=False, incoming=False, handle=True):
        return {"contract": copy.deepcopy(baseline if contract is None else contract), "active": active,
                "terminal": terminal, "incoming": incoming, "handle": handle, "native_calls": 0}

    def compare(left, right):
        return all(k in left and k in right and left[k] == right[k] for k in keys)

    def cancel_model(selected, contract, *, quota=2, expired=False, actual_request=True, actual_response=True):
        # This is an independent specification model, not the C++ implementation.
        if not selected:
            return 9
        if len(selected) > quota:
            return 11
        if expired:
            return 110
        for item in selected:
            if item["active"] and not item["terminal"] and not item["incoming"]:
                if not compare(item["contract"], contract):
                    return 13
        for item in selected:
            if not item["active"] or item["terminal"]:
                continue
            if item["incoming"] and not (actual_request and actual_response):
                return 95
            if not item["handle"]:
                return 9
            item["native_calls"] += 1
        return 0

    for row in matrix:
        key = row["key"]
        changed = dict(baseline, **{key: row["mutation"]})
        first = goal()
        before = copy.deepcopy(first)
        check(cancel_model([first], changed) == 13 and first == before, key + ": own mismatch has no native effects")
        selected = [goal(), goal(changed)]
        before = copy.deepcopy(selected)
        check(cancel_model(selected, baseline) == 13 and selected == before, key + ": later any mismatch has no native effects")
        selected = [goal(), goal(changed, handle=False)]
        before = copy.deepcopy(selected)
        check(cancel_model(selected, baseline) == 13 and selected == before, key + ": pending handle cannot bypass preflight")
        check(compare(changed, changed), key + ": equal explicit fields match")
        missing = dict(baseline)
        missing.pop(key)
        check(not compare(missing, baseline) and not compare(baseline, missing), key + ": missing fields fail closed")
    for selected in [[goal()], [goal(), goal()]]:
        check(cancel_model(selected, baseline) == 0 and all(g["native_calls"] == 1 for g in selected),
              "matching own/any model reaches each selected native operation")
    for state in [{"active": False}, {"terminal": True}]:
        selected = [goal(), goal({}, **state)]
        check(cancel_model(selected, baseline) == 0 and [g["native_calls"] for g in selected] == [1, 0],
              "inactive/terminal mismatch does not prevent a live matching cancellation")
    for request, response in [(False, True), (True, False), (False, False)]:
        selected = [goal(incoming=True)]
        before = copy.deepcopy(selected)
        check(cancel_model(selected, baseline, actual_request=request, actual_response=response) == 95 and selected == before,
              "incoming actual-QoS refusal has no native request")
    for kwargs, expected in [({"quota": 0}, 11), ({"expired": True}, 110)]:
        selected = [goal({})]
        check(cancel_model(selected, baseline, **kwargs) == expected and selected[0]["native_calls"] == 0,
              "quota/deadline priority remains before QoS mismatch")
    check(cancel_model([], baseline) == 9, "unknown selection retains missing ownership result")
    padded = dict(baseline, **{"qos.cancel-service.depth": "08"})
    check(not compare(baseline, padded) and compare(padded, padded), "exact decimal spelling is preserved")
    unrelated = dict(baseline, **{"qos.feedback.depth": "999"})
    check(compare(baseline, unrelated), "unused channel fields do not alter cancellation")

    # Preserve and compare the existing G04 test sources against the prior freeze.
    frozen = Path("/tmp/uwvm-action-qos-20261014-prepared-r3/overlay-manifest.json")
    preserved = {}
    if frozen.is_file():
        manifest = json.loads(frozen.read_text())
        for path, expected in manifest.items():
            if not path.startswith("test/0073.ros-tests/support/source/ros_qos/"):
                continue
            actual_hash = hashlib.sha256((ROOT / path).read_bytes()).hexdigest()
            recorded = expected if isinstance(expected, str) else expected.get("sha256")
            check(actual_hash == recorded, "prior G04 fixture preserved: " + path)
            preserved[path] = actual_hash
    inputs = {path: hashlib.sha256((ROOT / path).read_bytes()).hexdigest() for path in FILES}
    report = {"status": "SOURCE_AND_PYTHON_MODEL_PASS_ONLY", "checks": len(checks), "assertions": checks,
              "inputs_sha256": inputs, "prior_g04_fixtures_sha256": preserved,
              "compiled": False, "native_ros_executed": False, "guest_executed": False,
              "scope": "Source text ordering and independent Python state models; no C++ semantic parsing or runtime qualification.",
              "prepared_native_fault_scope": "Test-only rcl query interposition, not an observed natural RMW downgrade."}
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open("x") as out:
        json.dump(report, out, indent=2)
        out.write("\n")
    print(json.dumps({"status": report["status"], "checks": len(checks), "report": str(args.out)}))


if __name__ == "__main__":
    main()
