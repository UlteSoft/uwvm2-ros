#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
"""@file run_llvm_eh_resource_limits.py
@brief Verify exception-reference publication under the auxiliary allocation budget.
"""
import argparse
import json
from pathlib import Path
import resource
import signal
import subprocess

from run_core3_engine_comparison import digest, verify_gate


ITERATIONS = 200_000
LOW = 8 * 1024 * 1024
HIGH = 64 * 1024 * 1024
REASON = b"GC allocation failed: memory or size limit"
AOT = ("-Raot", "-Rct", "0", "-Rllvm-cache-path", "disable",
       "-Rllvm-exception-dispatch", "native-unwind")


def fixture(route):
    """@brief Retain an observable reference and verify its last payload after the loop."""
    declarations = """
    (type $throw_type (func (param i32)))
    (table $calls 1 funcref)
    (elem (table $calls) (i32.const 0) func $throw)
    (func $throw (type $throw_type) local.get 0 throw $event)
    """ if route == "cross-frame" else ""
    producer = ("local.get $input i32.const 0 call_indirect $calls (type $throw_type)"
                if route == "cross-frame" else "local.get $input throw $event")
    capture = f"""
        i32.const 0
        (block $caught (result (ref exn))
          (try_table (catch_all_ref $caught)
            {producer})
          unreachable)
        table.set $retained
    """
    if route == "acyclic":
        declarations = f"(func $capture (param $input i32) {capture})"
        capture = "local.get $input call $capture"
    return f"""(module
      (tag $event (param i32))
      (table $retained 1 exnref)
      {declarations}
      (func (export "_start") (local $input i32)
        (loop $again
          {capture}
          local.get $input i32.const 1 i32.add local.tee $input
          i32.const {ITERATIONS} i32.lt_u br_if $again)
        (block $payload (result i32)
          (try_table (catch $event $payload)
            i32.const 0 table.get $retained throw_ref)
          unreachable)
        i32.const {ITERATIONS - 1} i32.ne if unreachable end))
    """


def main():
    """@brief Compare classified exhaustion with a successful execution of the same binary."""
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("runtime", "wasm-tools", "wasmtime", "output"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--baseline", type=Path, help="Optional Linux x86-64 baseline with the legacy SIGILL")
    args = parser.parse_args()
    verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    args.output.mkdir(mode=0o700)
    inputs = [args.runtime, args.wasm_tools, args.wasmtime, Path(__file__)]
    if args.baseline:
        inputs.append(args.baseline)
    results = {"complete": False, "iterations": ITERATIONS, "auxiliary_live_limits": [LOW, HIGH],
               "cgroup": Path("/proc/self/cgroup").read_text().strip(),
               "pins": {str(path): digest(path) for path in inputs}, "cases": {}}

    def save():
        pending = args.output / "pending.json"
        pending.write_text(json.dumps(results, indent=2) + "\n")
        pending.replace(args.output / "results.json")

    def run(name, command, expected_exit, expected_reason=None):
        command = list(map(str, command))
        result = subprocess.run(command, capture_output=True, timeout=30)
        log = args.output / (name + ".log")
        output = result.stdout + result.stderr
        log.write_bytes(output)
        passed = result.returncode == expected_exit
        if expected_reason is not None:
            passed = passed and expected_reason in output
        results["cases"][name] = {"argv": command, "exit": result.returncode,
                                  "passed": passed, "log": str(log), "log_sha256": digest(log)}
        save()
        print(f"{name}: {result.returncode}", flush=True)
        if not passed:
            raise RuntimeError(f"Unexpected outcome in {log}")

    save()
    for route in ("loop", "acyclic", "cross-frame"):
        source = args.output / f"{route}.wat"
        wasm = source.with_suffix(".wasm")
        source.write_text(fixture(route))
        run(f"{route}-encode", [args.wasm_tools, "parse", source, "-o", wasm], 0)
        results["pins"][str(wasm)] = digest(wasm)
        run(f"{route}-oracle", [args.wasmtime, "run", "-W", "exceptions=y", "-W", "gc=y", wasm], 0)
        for engine, options in (("aot", AOT), ("interpreter", ("-Rint",))):
            flags = ["-WF3", *options, "--debug-fault-action", "exit",
                     "--wasm-set-resource-limit", "runtime", "auxiliary-live-bytes"]
            stem = f"{route}-{engine}"
            run(stem + "-within-budget", [args.runtime, *flags, HIGH, "--run", wasm], 0)
            run(stem + "-over-budget", [args.runtime, *flags, LOW, "--run", wasm], 121, REASON)
            if args.baseline:
                run(stem + "-baseline", [args.baseline, *flags, LOW, "--run", wasm], -signal.SIGILL)
    results["complete"] = True
    save()


if __name__ == "__main__":
    main()
