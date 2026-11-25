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
TRANSFER_ITERATIONS = 100_000
TRANSFER_LOW = 11 * 1024 * 1024


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


def transfer_source():
    """@brief Issue the source tokens once and throw them as exception payloads."""
    return f"""(module
      (tag $inner (export "inner") (param i32))
      (tag $envelope (export "envelope") (param exnref))
      (table $held {TRANSFER_ITERATIONS} exnref)
      (func (export "prepare") (local $index i32)
        (loop $again
          local.get $index
          (block $captured (result (ref exn))
            (try_table (catch_all_ref $captured) local.get $index throw $inner)
            unreachable)
          table.set $held
          local.get $index i32.const 1 i32.add local.tee $index
          i32.const {TRANSFER_ITERATIONS} i32.lt_u br_if $again))
      (func (export "send") (param $index i32)
        local.get $index table.get $held throw $envelope))
    """


def transfer_receiver(retain):
    """@brief Compare a typed payload transfer with a catch that discards it."""
    if retain:
        receive = """
          (block $received (result exnref)
            (try_table (catch $envelope $received) local.get $index call $send)
            unreachable)
          local.set $reference
          (block $checked (result i32)
            (try_table (catch $inner $checked) local.get $reference throw_ref)
            unreachable)
          local.get $index i32.ne if unreachable end
        """
    else:
        receive = """
          (block $discarded
            (try_table (catch_all $discarded) local.get $index call $send)
            unreachable)
        """
    return f"""(module
      (import "source" "inner" (tag $inner (param i32)))
      (import "source" "envelope" (tag $envelope (param exnref)))
      (import "source" "prepare" (func $prepare))
      (import "source" "send" (func $send (param i32)))
      (func (export "_start") (local $index i32) (local $reference exnref)
        call $prepare
        (loop $again
          {receive}
          local.get $index i32.const 1 i32.add local.tee $index
          i32.const {TRANSFER_ITERATIONS} i32.lt_u br_if $again)))
    """


def main():
    """@brief Compare classified exhaustion with a successful execution of the same binary."""
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("runtime", "wasm-tools", "wasmtime", "output"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--baseline", type=Path, help="Optional Linux x86-64 baseline with the legacy payload fault")
    args = parser.parse_args()
    verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    args.output.mkdir(mode=0o700)
    inputs = [args.runtime, args.wasm_tools, args.wasmtime, Path(__file__)]
    if args.baseline:
        inputs.append(args.baseline)
    results = {"complete": False, "iterations": ITERATIONS, "auxiliary_live_limits": [LOW, HIGH],
               "transfer_iterations": TRANSFER_ITERATIONS, "transfer_auxiliary_live_limits": [TRANSFER_LOW, HIGH],
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

    def encode(name, contents):
        source = args.output / f"{name}.wat"
        wasm = source.with_suffix(".wasm")
        source.write_text(contents)
        run(f"{name}-encode", [args.wasm_tools, "parse", source, "-o", wasm], 0)
        results["pins"][str(wasm)] = digest(wasm)
        return wasm

    save()
    engines = (("aot", AOT), ("interpreter", ("-Rint",)))
    for route in ("loop", "acyclic", "cross-frame"):
        wasm = encode(route, fixture(route))
        run(f"{route}-oracle", [args.wasmtime, "run", "-W", "exceptions=y", "-W", "gc=y", wasm], 0)
        for engine, options in engines:
            flags = ["--wasm-policy-global", "parser.baseline", "wasm3", *options, "--debug-fault-action", "exit",
                     "--wasm-set-resource-limit", "runtime", "auxiliary-live-bytes"]
            stem = f"{route}-{engine}"
            run(stem + "-within-budget", [args.runtime, *flags, HIGH, "--run", wasm], 0)
            run(stem + "-over-budget", [args.runtime, *flags, LOW, "--run", wasm], 121, REASON)
            if args.baseline:
                run(stem + "-baseline", [args.baseline, *flags, LOW, "--run", wasm], 121, REASON)
    source = encode("transfer-source", transfer_source())
    retain = encode("transfer-retain", transfer_receiver(retain=True))
    discard = encode("transfer-discard", transfer_receiver(retain=False))
    for name, wasm in (("retain", retain), ("discard", discard)):
        run(f"transfer-{name}-oracle",
            [args.wasmtime, "run", "-W", "exceptions=y", "-W", "gc=y", "--preload", f"source={source}", wasm], 0)
    for engine, options in engines:
        flags = ["--wasm-policy-global", "parser.baseline", "wasm3", *options, "--debug-fault-action", "exit", "--wasm-preload-library", source, "source",
                 "--wasm-set-resource-limit", "runtime", "auxiliary-live-bytes"]
        stem = f"transfer-{engine}"
        run(stem + "-discard", [args.runtime, *flags, TRANSFER_LOW, "--run", discard], 0)
        run(stem + "-within-budget", [args.runtime, *flags, HIGH, "--run", retain], 0)
        run(stem + "-over-budget", [args.runtime, *flags, TRANSFER_LOW, "--run", retain], 121, REASON)
        if args.baseline:
            run(stem + "-baseline-discard", [args.baseline, *flags, TRANSFER_LOW, "--run", discard], 0)
            run(stem + "-baseline", [args.baseline, *flags, TRANSFER_LOW, "--run", retain], -signal.SIGILL)
    results["complete"] = True
    save()


if __name__ == "__main__":
    main()
