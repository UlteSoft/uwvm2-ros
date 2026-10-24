"""Small current-source codec, environment, scope and real Guest acceptance."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inputs", type=Path, required=True)
    parser.add_argument("--generated", type=Path, required=True)
    parser.add_argument("--wasi-sdk", type=Path, required=True)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--cpu-wasm", type=Path, required=True)
    parser.add_argument("--skip-source-capture", action="store_true")
    parser.add_argument("--skip-raw", action="store_true")
    args = parser.parse_args()
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("shared finite Linux owner required")
    root = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work" / "followup"
    root.mkdir()
    rows = []
    receipt = {
        "format": "uwvm-task09-followup-acceptance-v1",
        "cases": rows,
        "default_authority": [],
        "native_ros_exchanges": 0,
        "model_calls": 0,
        "simulator_runs": 0,
        "official_suite_credit": 0,
        "guest_launch_attempts": 0,
        "successful_guest_executions": 0,
        "runtime_sha256": hashlib.sha256(args.runtime.read_bytes()).hexdigest(),
    }
    if (
        receipt["runtime_sha256"]
        != "0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556"
    ):
        raise ValueError("independently retained actual Runtime changed")
    if (
        not args.cpu_wasm.is_file()
        or hashlib.sha256(args.cpu_wasm.read_bytes()).hexdigest()
        != "ce270ea63b507034dc57d2761e95e00c482da9b1132990e1b56dcf700531dfc1"
    ):
        raise ValueError("actual retained CPU layout Wasm path/hash differs")
    receipt["source_capture_repeated"] = not args.skip_source_capture
    receipt["raw_denial_repeated"] = not args.skip_raw

    def save():
        (root / "receipt.json").write_text(
            json.dumps(receipt, indent=2, sort_keys=True) + "\n"
        )

    def run(name, argv, expected=0, timeout=90):
        argv = [str(value) for value in argv]
        result = subprocess.run(
            argv, capture_output=True, timeout=timeout, stdin=subprocess.DEVNULL
        )
        (root / (name + ".stdout")).write_bytes(result.stdout)
        (root / (name + ".stderr")).write_bytes(result.stderr)
        passed = result.returncode == expected
        rows.append(
            {
                "case": name,
                "argv": argv,
                "expected": expected,
                "exit_code": result.returncode,
                "status": "PASS" if passed else "FAIL",
                "stdout_bytes": len(result.stdout),
                "stdout_sha256": hashlib.sha256(result.stdout).hexdigest(),
                "stderr_sha256": hashlib.sha256(result.stderr).hexdigest(),
            }
        )
        save()
        print(name + ": " + rows[-1]["status"], flush=True)
        return passed

    run(
        "corrected-environment",
        [sys.executable, "-B", args.inputs / "inspect_environment.py"],
    )
    # Executes the corrected current native-prefix membership check, then a
    # genuine bounded official source capture. Historical captures stay intact.
    if not args.skip_source_capture:
        run(
            "corrected-source-owner-scope",
            [
                sys.executable,
                "-B",
                args.inputs / "capture_public_sources.py",
                "--enable-source-download",
                "--lane",
                "isaac-ros-apriltag",
                "--scope",
                "tree",
                "--transport",
                "official-api",
                "--out",
                root / "source-scope-check",
            ],
            timeout=120,
        )
    codec = run(
        "staged-codec",
        [
            sys.executable,
            "-B",
            args.inputs / "run_staged_codec.py",
            "--test-source",
            args.inputs / "staged_codec_contract.c",
            "--generated",
            args.generated,
            "--wasi-sdk",
            args.wasi_sdk,
        ],
        timeout=120,
    )
    raw = root / "raw-default-denial.wasm"
    built = False
    if not args.skip_raw:
        built = run(
            "raw-denial-build",
            [
                args.wasi_sdk / "bin/clang++",
                "--target=wasm32-wasip1",
                "--sysroot=" + str(args.wasi_sdk / "share/wasi-sysroot"),
                "-std=c++20",
                "-O2",
                "-fno-exceptions",
                "-fno-rtti",
                "-I",
                args.inputs / "include",
                args.inputs / "raw_denied.cpp",
                "-o",
                raw,
            ],
        )

    def guest(name, wasm):
        # Ordinary WASIp1 stdio only. ROS and all additional rights stay empty.
        command = [
            args.runtime,
            "--runtime-int",
            "-Rct",
            "0",
            "--wasm-set-main-module-name",
            "main",
            "--wasip1-global-disable",
            "--wasip1-global-noinherit-system-environment",
            "--wasip1-single-create",
            "main",
            "--wasip1-single-enable",
            "main",
            "--wasip1-single-hide-host-api",
            "main",
            "--wasip1-single-noinherit-system-environment",
            "main",
        ]
        for right in (
            "realtime-clock",
            "monotonic-clock",
            "process-clock",
            "thread-clock",
            "random",
            "proc-raise",
            "sched-yield",
            "stdio-full-rights",
        ):
            command += ["--wasip1-single-deny", "main", right]
        command += ["--run", wasm]
        receipt["guest_launch_attempts"] += 1
        passed = run(name, command, timeout=45)
        if passed:
            receipt["successful_guest_executions"] += 1
        save()
        return passed

    if codec:
        guest(
            "staged-codec-actual-guest",
            Path(os.environ["UWVM_RESOURCE_RUN"])
            / "work/staged-codec/staged-codec.wasm",
        )
    if built:
        run("raw-denial-wasm-validation", ["/usr/bin/wasm-validate", raw])
        guest("raw-denial-actual-guest", raw)
    guest("complete-cpu-values-actual-guest", args.cpu_wasm)
    receipt["passed"] = sum(row["status"] == "PASS" for row in rows)
    receipt["denominator"] = len(rows)
    receipt["status"] = (
        "PASS" if receipt["passed"] == receipt["denominator"] else "FAILED_RETAINED"
    )
    save()
    print(json.dumps(receipt, sort_keys=True))
    return 0 if receipt["status"] == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
