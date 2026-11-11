#!/usr/bin/env python3
"""Prove the threaded MTE fixture cannot turn persistent spawn denial into success.

This exercises the actual VM and the same bounded spawn builder as the MTE
matrix. A visible, denied thread-spawn API must produce exactly 64 refusals and
an unreachable trap, never a memory-protection trap or an indefinitely waiting
fixture. No child is authorized or created by this test.
"""

import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--runtime-prefix", type=json.loads, default=[])
    parser.add_argument("--engine", choices=("int", "aot"), required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--timeout", type=float, default=10.0)
    args = parser.parse_args()
    if not isinstance(args.runtime_prefix, list) or not all(isinstance(x, str) for x in args.runtime_prefix):
        parser.error("--runtime-prefix must be a JSON array of argument strings")
    if not 0 < args.timeout < float("inf"):
        parser.error("--timeout must be positive and finite")
    source = Path(__file__).resolve().parents[1] / "0086.memtag"
    sys.path.insert(0, str(source))
    import threaded_matrix as threaded
    from vm_diagnostics import fault_matches, human_fault

    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    runtime = args.runtime.resolve()
    raw = threaded.spawned("good")
    guest = out / "good-but-denied.wasm"
    guest.write_bytes(raw)
    m = threaded.m
    owner = out / "owner.wasm"
    owner.write_bytes(b"\0asm\x01\0\0\0" + m.sec(5, b"\x01\x03\x01\x02") + m.sec(7, b"\x01\x06memory\x02\0"))
    report = {
        "runtime": str(runtime),
        "runtime_sha256": hashlib.sha256(runtime.read_bytes()).hexdigest(),
        "runtime_prefix": args.runtime_prefix,
        "engine": args.engine,
        "fixture_sha256": hashlib.sha256(raw).hexdigest(),
        "fixture_builder_sha256": hashlib.sha256((source / "threaded_matrix.py").read_bytes()).hexdigest(),
        "cases": [],
        "complete": False,
    }
    for mode, workers in (("sync", 1), ("software-async", 1)):
        command = [*args.runtime_prefix, str(runtime), "--runtime-" + args.engine,
                   "--runtime-compile-threads", "0", "-WFE-threads",
                   "--wasm-feature-enable-memory64", "--wasm-feature-enable-multi-memory",
                   *m.mode_flags(mode), 
                   "--wasi-threads", "3", "3", "1048576", "16777216", "67108864", "131072",
                   "--wasm-set-main-module-name", "main", "--wasip1-single-create", "main",
                   "--wasip1-single-enable", "main",
                   "--wasip1-single-interface", "main", "proc_exit", "allow",
                   "--wasip1-single-grant", "main", "proc-exit",
                   "--wasip1-single-interface", "main", "thread-spawn", "deny",
                   "--wasm-preload-library", str(owner), "env"]
        if args.engine == "aot":
            command += ["--runtime-llvm-jit-cache-path", "disable", "--runtime-llvm-jit-policy", "debug"]
        command += ["--run", str(guest)]
        label = mode + "-" + str(workers)
        stdout_path, stderr_path = out / (label + ".stdout"), out / (label + ".stderr")
        begin = time.monotonic()
        # Real files preserve the safe diagnostic sink without depending on a
        # procfs pipe reopen; diagnostics are part of the failure oracle.
        with stdout_path.open("wb") as stdout, stderr_path.open("wb") as stderr:
            try:
                result = subprocess.run(command, stdout=stdout, stderr=stderr, timeout=args.timeout)
                code = result.returncode
            except subprocess.TimeoutExpired:
                code = "timeout"
        elapsed = time.monotonic() - begin
        output = stdout_path.read_text(errors="replace") + stderr_path.read_text(errors="replace")
        refused = output.count("[host_denied_wasip1.thread-spawn]")
        row = {"mode": mode, "workers": workers, "command": command, "exit": code,
               "seconds": elapsed, "timeout_seconds": args.timeout, "exact_denied_attempts": refused,
               "fault": human_fault(output), "stdout": str(stdout_path), "stderr": str(stderr_path),
               "passed": refused == 64 and fault_matches(code, output, 120, "unreachable")}
        report["cases"].append(row)
        (out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
        print(label, "exit", code, "refusals", refused, "passed", row["passed"], flush=True)
    report["complete"] = all(x["passed"] for x in report["cases"])
    (out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    return 0 if report["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
