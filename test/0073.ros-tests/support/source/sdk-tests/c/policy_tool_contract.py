#!/usr/bin/env python3
"""Check explicit policy preparation in source, installed and split SDK layouts.

Only local JSON/configuration generation is exercised. No emitted command is
executed and no Gateway, ROS process or capability grant is admitted.
"""

import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs = run / "inputs"
    work = run / "work"
    layouts = {}
    for name, script, schema, helper in (
        ("source", "example/platform/ros/c/c/advanced_policy.py", "abi/abi-v1.json", "tools/example_policy.py"),
        ("installed", "example/platform/ros/c/c/advanced_policy.py", "share/abi/abi-v1.json", "tools/platform/ros/example_policy.py"),
        ("split", "advanced_policy.py", None, None),
        ("missing", "advanced_policy.py", None, None),
    ):
        root = work / name
        target = root / script
        target.parent.mkdir(parents=True)
        shutil.copyfile(inputs / "advanced_policy.py", target)
        for relative, source in ((schema, "abi-v1.json"), (helper, "example_policy.py")):
            if relative:
                path = root / relative
                path.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(inputs / source, path)
        layouts[name] = target
    rows = []

    def execute(name, layout, backend, aliases, expect=0, extra=()):
        output = work / ("output-" + name)
        argv = [sys.executable, "-I", "-S", "-B", str(layouts[layout]),
                "--backend", str(backend), "--socket", "/tmp/task02-policy.sock",
                "--worker-uid", "1000", "--worker-pid", "12345", "--gateway-uid", "1000",
                "--node-name", "task02_policy", "--namespace", "/task02", "--out", str(output)]
        argv += (["--master-uri", "http://localhost:11311", "--advertise-host", "localhost"]
                 if backend == 1 else ["--domain-id", "42", "--disabled-security-tutorial"])
        for alias in aliases:
            argv += ["--alias", alias]
        argv += list(extra)
        result = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, timeout=20, check=False)
        (work / (name + ".stdout")).write_bytes(result.stdout)
        (work / (name + ".stderr")).write_bytes(result.stderr)
        passed = result.returncode == expect
        if expect == 0 and passed:
            policy = json.loads((output / "policy.json").read_text())
            passed = (len(policy["grants"]) == len(aliases)
                      and [grant["alias"] for grant in policy["grants"]] == aliases
                      and policy["implicit_rights"] == []
                      and policy["native_acceptance"] == "NOT_PERFORMED"
                      and policy["status"] == "PREPARED_NOT_VALIDATED_NOT_EXECUTED"
                      and policy["Worker_policy_check_arguments"][-1] == f"--ros{backend}-check-policy"
                      and "gateway-socket" not in policy["Gateway_connection"]
                      and "gateway-uid" not in policy["Gateway_connection"])
            passed = passed and all(
                hashlib.sha256(Path(pin["path"]).read_bytes()).hexdigest() == pin["sha256"]
                for pin in policy["sdk_support"].values())
        elif expect != 0:
            passed = passed and not output.exists()
        rows.append({"name": name, "argv": argv, "expected_exit": expect,
                     "actual_exit": result.returncode, "status": "PASS" if passed else "FAIL"})

    aliases = ["pub", "sub", "trigger-call", "trigger-advertise", "trigger-respond",
               "set-bool-call", "set-bool-advertise", "set-bool-respond", "log-info"]
    for backend in (1, 2):
        for layout in ("source", "installed", "split"):
            extra = (["--abi-schema", str(inputs / "abi-v1.json"), "--policy-support", str(inputs / "example_policy.py")]
                     if layout == "split" else [])
            execute(f"{layout}-ros{backend}", layout, backend, aliases, extra=extra)
    execute("missing-support", "missing", 1, ["pub"], expect=2)
    execute("unsupported-ros1-lifecycle", "installed", 1, ["lifecycle-get"], expect=2)
    execute("unknown-alias", "installed", 2, ["unknown"], expect=2)
    execute("duplicate-alias", "installed", 2, ["pub", "pub"], expect=2)
    execute("explicit-missing-schema", "split", 1, ["pub"], expect=2,
            extra=["--abi-schema", str(work / "absent.json"), "--policy-support", str(inputs / "example_policy.py")])
    failures = sum(row["status"] == "FAIL" for row in rows)
    report = {"format": "task02-advanced-policy-layout-contract-v1",
              "status": "FAIL" if failures else "PASS_PREPARATION_SCOPE",
              "counts": {"PASS": len(rows) - failures, "FAIL": failures}, "rows": rows,
              "inputs_sha256": {name: hashlib.sha256((inputs / name).read_bytes()).hexdigest()
                                for name in ("advanced_policy.py", "example_policy.py", "abi-v1.json")},
              "runtime_policy_checks": 0, "gateway_runs": 0, "native_ros_runs": 0,
              "physical_motion_runs": 0, "default_authority": [], "public_sdk_published": False}
    (work / "policy-tool.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({key: report[key] for key in ("status", "counts")}), flush=True)
    return bool(failures)


if __name__ == "__main__":
    sys.exit(main())
