#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
"""@file run_core3_kernel_performance.py
@brief Time the same standard Wasm kernels after compilation in each engine.
"""
import argparse
import json
import os
from pathlib import Path
import re
import resource
import statistics
import subprocess
import sys
import time

from make_debug_core3_feature_workloads import author, definitions
from run_core3_engine_comparison import digest, verify_gate


CLOCK_AND_OUTPUT = [
    "--wasm-set-main-module-name", "main",
    "--wasip1-single-create", "main", "--wasip1-single-enable", "main",
    "--wasip1-single-interface", "main", "clock_time_get", "allow",
    "--wasip1-single-interface", "main", "fd_write", "allow",
    "--wasip1-single-grant", "main", "monotonic-clock",
    "--wasip1-single-grant", "main", "stdout.write",
]


def run(command, expected, log):
    """@brief Read one bounded guest timing with its independently computed checksum."""
    begin = time.monotonic_ns()
    result = subprocess.run(command, capture_output=True, timeout=60)
    wall = time.monotonic_ns() - begin
    log.write_bytes(result.stdout + result.stderr)
    match = re.fullmatch(rb"Kernel ns: ([0-9]+)\nChecksum: ([0-9]+)\n", result.stdout)
    if result.returncode != 0 or not match:
        raise RuntimeError(f"failed benchmark execution {log}")
    elapsed, checksum = map(int, match.groups())
    if not 0 < elapsed <= wall or checksum != expected:
        raise ValueError(f"unexpected timing or checksum in {log}")
    return {"kernel_ns": elapsed, "wall_ns": wall, "checksum": checksum}


def main():
    """@brief Record warmed repetitions inside identical per-process Wasm kernels."""
    p = argparse.ArgumentParser(description=__doc__)
    for name in ("runtime", "baseline", "wasmtime", "wasm-tools", "output"):
        p.add_argument("--" + name, type=Path, required=True)
    cases = [case[0] for case in definitions() if case[0] != "deterministic_profile"]
    p.add_argument("--case", action="append", choices=cases)
    p.add_argument("--iterations", type=int, default=100000)
    p.add_argument("--kernel-repeats", type=int, default=1)
    p.add_argument("--rounds", type=int, default=7)
    a = p.parse_args()
    if not (1 <= a.iterations <= 1000000 and 1 <= a.kernel_repeats <= 64 and 1 <= a.rounds <= 20):
        p.error("benchmark workload is out of range")
    verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    if len(os.sched_getaffinity(0)) != 1:
        p.error("pin the guarded test to a single CPU")
    a.output.mkdir(mode=0o700)
    fixtures = a.output / "inputs"
    inputs = [a.runtime, a.baseline, a.wasmtime, a.wasm_tools, Path(__file__),
              Path(__file__).with_name("run_core3_engine_comparison.py"),
              Path(__file__).with_name("make_debug_core3_feature_workloads.py")]
    pins = {str(path.resolve()): digest(path) for path in inputs}
    rows = author(fixtures, a.iterations, a.kernel_repeats)
    selected = set(a.case or cases)
    flags = ["-WF3", "-Raot", "--runtime-llvm-jit-policy", "max", "-Rct", "0",
             "-Rllvm-cache-path", "disable", "--wasm-feature-disable-memtag-extension", *CLOCK_AND_OUTPUT]
    engines = {
        "uwvm-llvm": [a.runtime, *flags, "--run"],
        "uwvm-baseline-llvm": [a.baseline, *flags, "--run"],
        "wasmtime-cranelift": [a.wasmtime, "run", "-C", "compiler=cranelift", "-C", "cache=n",
                              "-O", "opt-level=2", "-W", "all-proposals=y", "-W", "component-model=n"],
    }
    report = {"complete": False, "scope": "Guest monotonic kernel interval with startup and output excluded",
              "affinity": sorted(os.sched_getaffinity(0)), "cgroup": Path("/proc/self/cgroup").read_text().strip(),
              "inputs": pins, "rounds": a.rounds, "cases": {}}
    result = a.output / "results.json"

    def save():
        temp = result.with_suffix(".tmp")
        temp.write_text(json.dumps(report, indent=2) + "\n")
        temp.replace(result)

    save()
    for row in rows:
        name = row["name"]
        if name not in selected:
            continue
        wat = fixtures / (name + ".wat")
        wasm = wat.with_suffix(".wasm")
        subprocess.run([a.wasm_tools, "parse", wat, "-o", wasm], check=True, timeout=30)
        subprocess.run([a.wasm_tools, "validate", "--features", "all", wasm], check=True, timeout=30)
        positions = list(engines)
        timing = {engine: [] for engine in engines}
        report["cases"][name] = dict(fixture=row, wasm_sha256=digest(wasm), timing=timing)
        commands = {engine: [*prefix, wasm] for engine, prefix in engines.items()}
        report["cases"][name]["commands"] = {engine: list(map(str, cmd)) for engine, cmd in commands.items()}
        for repetition in range(-1, a.rounds):
            for engine in positions if repetition % 2 == 0 else reversed(positions):
                log = a.output / f"{name}-{engine}-{repetition}.log"
                sample = run(commands[engine], row["expected_checksum"], log)
                if repetition >= 0:
                    timing[engine].append(sample)
        median = {engine: statistics.median(s["kernel_ns"] for s in samples) for engine, samples in timing.items()}
        report["cases"][name]["median_kernel_ns"] = median
        save()
    report["complete"] = len(report["cases"]) == len(selected) and all(digest(Path(p)) == sha for p, sha in pins.items())
    save()
    return 0 if report["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
