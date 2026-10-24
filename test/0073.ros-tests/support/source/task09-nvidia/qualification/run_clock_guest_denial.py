#!/usr/bin/env python3
"""Run the actual built full Rust clock application under empty ROS authority."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("artifact", "runtime"):
        parser.add_argument("--" + name, type=Path, required=True)
        parser.add_argument("--" + name + "-sha256", required=True)
    args = parser.parse_args()
    assert os.environ["UWVM_RESOURCE_HEAVY_LOCK_HELD"] == "1"
    work = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True) / "work"
    for name in ("artifact", "runtime"):
        assert digest(getattr(args, name)) == getattr(args, name + "_sha256")
    argv = [str(args.runtime), "--runtime-int", "-Rct", "0", "--wasm-set-main-module-name", "main",
            "--ros2-global-enable", "--wasip1-global-disable", "--wasip1-global-noinherit-system-environment",
            "--wasip1-single-create", "main", "--wasip1-single-enable", "main",
            "--wasip1-single-hide-host-api", "main", "--wasip1-single-noinherit-system-environment", "main"]
    for right in ("realtime-clock", "monotonic-clock", "process-clock", "thread-clock", "random", "proc-raise", "sched-yield", "stdio-full-rights"):
        argv += ["--wasip1-single-deny", "main", right]
    qos = "1,1,2,1,1,0,0,0,0,0,0,0"
    actual_args = ["clock-program", "clock.call", "clock.observe", "clock.available", "1", "64", qos, qos]
    argv += ["--wasip1-single-force-args", "main", str(len(actual_args)), *actual_args, "--run", str(args.artifact)]
    with (work / "guest.stdout").open("xb") as stdout, (work / "guest.stderr").open("xb") as stderr:
        process = subprocess.run(argv, stdout=stdout, stderr=stderr, timeout=45)
    stdout = (work / "guest.stdout").read_text(errors="replace")
    stderr = (work / "guest.stderr").read_text(errors="replace")
    passed = process.returncode == 1 and "clock-program status=13" in stderr and "full_transport_diagnostics=" in stderr
    passed = passed and "actual retained endpoint receipt=" not in stdout and "response status=" not in stdout
    result = {"format": "uwvm-task09-full-clock-real-guest-denial-v1", "status": "PASS" if passed else "FAILED_RETAINED",
              "argv": argv, "guest_exit_code": process.returncode, "expected_guest_exit_code": 1,
              "expected_sdk_status": 13, "artifact_sha256": args.artifact_sha256, "runtime_sha256": args.runtime_sha256,
              "actual_guest_executions": 1, "positive_native_exchanges": 0, "official_program_acceptance": False,
              "ros_resource_grants": [], "default_authority": []}
    for name in ("artifact", "runtime"):
        assert digest(getattr(args, name)) == getattr(args, name + "_sha256")
    (work / "clock-guest-denial.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({key: result[key] for key in ("status", "guest_exit_code", "expected_sdk_status", "actual_guest_executions")}))
    return 0 if passed else 1


if __name__ == "__main__":
    raise SystemExit(main())
