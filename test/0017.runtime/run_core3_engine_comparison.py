#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
"""@file run_core3_engine_comparison.py
@brief Compare self-checking Core 3.0 modules across VM, Wasmtime and WABT.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import random
import resource
import statistics
import subprocess
import sys
import time

from make_debug_core3_feature_workloads import definitions, expected

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "test/0084.ros-runtime"))
from run_scene_vm import verify_gate


def digest(path):
    """@brief Hash the exact input without rewriting it."""
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def emit(case, values, trap=False):
    """@brief Add a pure entry point that checks the existing scalar oracle."""
    _, _, _, _, oracle, _, declarations, kernel = case
    checks = [f"i32.const {n} call $kernel i64.const {expected(oracle, n)} i64.ne if unreachable end"
              for n in values]
    if trap:
        checks.append("unreachable")
    return "\n".join(("(module", "(memory $io 2)", declarations, kernel,
                      '(func (export "_start")', *checks, "))", ""))


def measure(engine, command, goal):
    """@brief Require actual execution and distinguish traps from launch errors."""
    start = time.monotonic_ns()
    process = subprocess.run(command, capture_output=True, timeout=30)
    elapsed = time.monotonic_ns() - start
    data = process.stdout + process.stderr
    if goal == "trap":
        matched = b"unreachable" in data.lower() and (
            process.returncode != 0 or b"_start() => error:" in process.stdout.lower())
    else:
        matched = process.returncode == 0 and (
            engine != "wabt" or b"_start() =>\n" in process.stdout)
    return {"engine": engine, "exit": process.returncode, "wall_ns": elapsed,
            "matched": matched, **({"diagnostic": data.decode(errors="replace")[-2400:]} if not matched else {})}


def main():
    """@brief Compare fixed workloads or run bounded reproducible random trials."""
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("runtime", "wasmtime", "wasm-tools", "wabt", "wabt-validator", "output"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--baseline", type=Path)
    parser.add_argument("--rounds", type=int, default=7)
    parser.add_argument("--iterations", type=int, default=100000)
    parser.add_argument("--seconds", type=int, default=0)
    parser.add_argument("--seed", type=int, default=1935627)
    args = parser.parse_args()
    if not (1 <= args.rounds <= 25 and 1 <= args.iterations <= 1000000 and 0 <= args.seconds <= 3600):
        parser.error("rounds, iterations or random test duration are out of range")
    verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    if len(os.sched_getaffinity(0)) != 1:
        parser.error("pin the guarded test to a single CPU")
    args.output.mkdir(mode=0o700)
    executables = (args.runtime, args.wasmtime, args.wasm_tools, args.wabt, args.wabt_validator)
    inputs = [*executables, Path(__file__), Path(__file__).with_name("make_debug_core3_feature_workloads.py")]
    if args.baseline:
        inputs.append(args.baseline)
    report = {"complete": False, "pins": {str(path.resolve()): digest(path) for path in inputs},
              "cgroup": Path("/proc/self/cgroup").read_text().strip(), "affinity": sorted(os.sched_getaffinity(0)),
              "cases": 0, "mismatches": 0, "wabt_executed": 0, "wabt_rejections": {},
              "random_seconds": args.seconds, "seed": args.seed,
              "timing": "wall time including startup, validation, compilation, execution and exit"}
    report["versions"] = {str(tool): subprocess.check_output([tool, "--version"], timeout=15, text=True).strip()
                          for tool in executables[1:]}
    cases = [case for case in definitions() if case[0] != "deterministic_profile"]
    source = args.output / "current.wat"
    wasm = source.with_suffix(".wasm")
    log = args.output / "trials.jsonl"

    def save():
        pending = args.output / "results.tmp"
        pending.write_text(json.dumps(report, indent=2) + "\n")
        pending.replace(args.output / "results.json")

    def exercise(case, values, rounds, goal="ok"):
        source.write_text(emit(case, values, trap=goal == "trap"))
        subprocess.run([args.wasm_tools, "parse", source, "-o", wasm], check=True, timeout=30)
        subprocess.run([args.wasm_tools, "validate", "--features", "all", wasm], check=True, timeout=30)
        row = {"case": case[0], "iterations": values, "expected": goal, "sha256": digest(wasm), "trials": []}
        check = subprocess.run([args.wabt_validator, "--enable-all", wasm], capture_output=True, timeout=30)
        row["wabt_validator_exit"] = check.returncode
        strict = ["--wasm-policy-global", "parser.memtag", "off", "--debug-fault-action", "exit", "--run", wasm]
        aot = ["--wasm-policy-global", "parser.baseline", "wasm3", "-Raot", "-Rct", "0", "-Rllvm-cache-path", "disable", *strict]
        engines = [
            ("uwvm-int", [args.runtime, "--wasm-policy-global", "parser.baseline", "wasm3", "-Rint", *strict]),
            ("uwvm-aot", [args.runtime, *aot]),
            ("wasmtime", [args.wasmtime, "run", "-C", "cache=n", "-O", "opt-level=2",
                          "-W", "all-proposals=y", "-W", "component-model=n", wasm]),
        ]
        if args.baseline:
            engines.append(("uwvm-baseline-aot", [args.baseline, *aot]))
        if check.returncode:
            report["wabt_rejections"].setdefault(case[0], check.stderr.decode(errors="replace")[-1000:])
        else:
            report["wabt_executed"] += 1
            engines.append(("wabt", [args.wabt, "--enable-all", "--run-all-exports", wasm]))
        for repeat in range(rounds):
            for label, command in engines if repeat % 2 == 0 else reversed(engines):
                row["trials"].append(measure(label, command, goal))
        differences = sum(not trial["matched"] for trial in row["trials"])
        report["cases"] += 1
        report["mismatches"] += differences
        row["median_ns"] = {name: statistics.median(t["wall_ns"] for t in row["trials"] if t["engine"] == name)
                            for name, _ in engines}
        with log.open("a") as stream:
            stream.write(json.dumps(row, separators=(",", ":")) + "\n")
        if differences and report["mismatches"] == differences:
            (args.output / "first-mismatch.wat").write_bytes(source.read_bytes())
        if differences or report["cases"] % 25 == 0:
            save()

    start = time.monotonic()
    save()
    if args.seconds:
        rng = random.Random(args.seed)
        while time.monotonic() - start < args.seconds:
            exercise(rng.choice(cases), [rng.randint(1, args.iterations) for _ in range(8)], 1)
    else:
        for case in cases:
            exercise(case, [args.iterations], args.rounds)
        exercise(next(case for case in cases if case[0] == "tail_direct"), [16], 1, "trap")
    report["elapsed_seconds"] = time.monotonic() - start
    report["inputs_unchanged"] = all(digest(Path(path)) == identity for path, identity in report["pins"].items())
    report["complete"] = not report["mismatches"] and report["inputs_unchanged"]
    save()
    print(json.dumps({"cases": report["cases"], "mismatches": report["mismatches"], "complete": report["complete"]}))
    return 0 if report["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
