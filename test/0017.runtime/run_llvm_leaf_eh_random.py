#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
"""Run seeded Core 3 numeric exception cases with an independent engine."""

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import random
import resource
import subprocess
import time

from run_core3_engine_comparison import digest, verify_gate


TYPES = ("i32", "i64", "f32", "f64", "v128")
ROUTES = ("typed", "catch_all", "nested", "rethrow", "typed_rethrow", "ordinary", "effect",
          "local_capture", "local_capture_all")


def clause(name, types):
    return f"({name} {' '.join(types)})" if types else ""


def value(rng, kind):
    if kind == "v128":
        return f"v128.const i64x2 {rng.getrandbits(64)} {rng.getrandbits(64)}"
    width = int(kind[1:])
    number = rng.getrandbits(width)
    return (f"{kind}.const {number}" if kind.startswith("i") else
            f"i{width}.const {number} {kind}.reinterpret_i{width}")


def check_value(kind, expected):
    if kind == "v128":
        return f"{expected} v128.xor v128.any_true if unreachable end"
    if kind.startswith("i"):
        return f"{expected} {kind}.ne if unreachable end"
    integer = "i" + kind[1:]
    return f"{integer}.reinterpret_{kind} {expected} {integer}.reinterpret_{kind} {integer}.ne if unreachable end"


def make_case(rng, index, route):
    parameters = [rng.choice(TYPES) for _ in range(rng.randrange(9))]
    selected = [rng.randrange(len(parameters)) for _ in range(rng.randrange(9))] if parameters else []
    payload = [parameters[i] for i in selected]
    arguments = [value(rng, kind) for kind in parameters]
    expected = [check_value(parameters[i], arguments[i]) for i in reversed(selected)]
    reorder = " ".join(f"local.get {i}" for i in selected)
    tag, other, callee, caller = (f"${name}{index}" for name in ("tag", "other", "callee", "case"))
    call = " ".join([*arguments, f"call {callee}"])
    result = clause("result", payload)
    effect = "i32.const 1 global.set $effect " if route == "effect" else ""
    leaf = f"{effect}{reorder} throw {tag}" if route != "ordinary" else reorder
    callee_results = result if route == "ordinary" else ""
    caller_locals = ""

    if route == "catch_all":
        # The later same-tag clause can never be selected.
        execution = f"""(block $wrong {result}
            (block $ignore (try_table (catch_all $ignore) (catch {tag} $wrong) {call}) unreachable)
            return)
            unreachable"""
    elif route == "nested":
        execution = f"""(block $caught {result}
            (try_table (catch {tag} $caught)
              (block $wrong {result} (try_table (catch {other} $wrong) {call}) unreachable)
              {'drop ' * len(payload)})
            unreachable)
            {' '.join(expected)}"""
    elif route in {"rethrow", "typed_rethrow"}:
        typed = route == "typed_rethrow"
        captured = clause("result", [*(payload if typed else []), "exnref"])
        clause_kind = f"catch_ref {tag}" if typed else "catch_all_ref"
        execution = f"""(block $caught {result}
            (try_table (catch {tag} $caught)
              (block $reference {captured} (try_table ({clause_kind} $reference) {call}) unreachable)
              throw_ref)
            unreachable)
            {' '.join(expected)}"""
    elif route in {"local_capture", "local_capture_all"}:
        typed = route == "local_capture"
        captured = clause("result", [*(payload if typed else []), "exnref"])
        clause_kind = f"catch_ref {tag}" if typed else "catch_all_ref"
        fields = " ".join(arguments[i] for i in selected)
        check_captured = " ".join(expected) if typed else ""
        execution = f"""(block $caught {result}
            (try_table (catch {tag} $caught)
              (block $reference {captured}
                (try_table ({clause_kind} $reference) {fields} throw {tag})
                unreachable)
              global.set $saved
              {check_captured}
              global.get $saved
              ref.null exn global.set $saved
              throw_ref)
            unreachable)
            {' '.join(expected)}"""
        # Visit one static capture site twice in the same activation.
        caller_locals = "(local $round i32)"
        execution = f"""(loop $again
            {execution}
            local.get $round i32.const 1 i32.add local.tee $round
            i32.const 2 i32.lt_u br_if $again)"""
    else:
        success = "br $caught" if route == "ordinary" else ""
        execution = f"""(block $caught {result}
            (try_table (catch {tag} $caught) {call} {success}) unreachable)
            {' '.join(expected)}"""
        if route == "effect":
            execution += " global.get $effect i32.const 1 i32.ne if unreachable end i32.const 0 global.set $effect"
    return f"""(tag {tag} {clause("param", payload)})
        (tag {other} {clause("param", payload)})
        (func {callee} {clause("param", parameters)} {callee_results} {leaf})
        (func {caller} {caller_locals} {execution})"""


def module(rng, batches, routes):
    picked = rng.sample(list(ROUTES), len(ROUTES))
    picked.extend(rng.choices(ROUTES, k=batches - len(picked)))
    for route in picked:
        routes[route] += 1
    functions = "\n".join(make_case(rng, index, route) for index, route in enumerate(picked))
    calls = " ".join(f"call $case{index}" for index in range(len(picked)))
    return f"""(module (global $effect (mut i32) (i32.const 0))
        (global $saved (mut exnref) (ref.null exn)) {functions} (func (export "_start") {calls}))\n"""


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("uwvm", "wasmtime", "wasm-tools", "output"):
        parser.add_argument("--" + name, required=True, type=Path)
    parser.add_argument("--seconds", type=int, default=900, choices=range(1, 901), metavar="[1..900]")
    parser.add_argument("--seed", type=int, default=715217)
    parser.add_argument("--wasm-cases", type=int, default=24, choices=range(len(ROUTES), 49),
                        metavar=f"[{len(ROUTES)}..48]")
    config = parser.parse_args()
    verify_gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    rng = random.Random(config.seed)
    config.output.mkdir(mode=0o700)
    output = config.output.resolve()
    wat, wasm = output / "last.wat", output / "last.wasm"
    hashed = hashlib.sha256()
    routes = Counter()
    engines = {
        "uwvm": [str(config.uwvm.resolve()), "-WF3", "-Raot", "-Rct", "0", "-Rllvm-cache-path", "disable",
                 "--runtime-llvm-jit-policy", "max", "-Rllvm-exception-dispatch", "native-unwind", "--run", str(wasm)],
        "wasmtime": [str(config.wasmtime.resolve()), "run", "-C", "cache=n", "-W", "all-proposals=y",
                     "-W", "component-model=n", str(wasm)],
    }
    encoder = str(config.wasm_tools.resolve())
    cgroup = Path("/proc/self/cgroup").read_text().strip()
    results = {"schema": 1, "seed": config.seed, "seconds": config.seconds, "cgroup": cgroup, "engines": engines,
               "uwvm_sha256": digest(config.uwvm), "wasmtime_sha256": digest(config.wasmtime),
               "wasm_tools_sha256": digest(config.wasm_tools)}
    began = time.monotonic()
    completed = 0

    def save():
        pending = output / "pending.json"
        pending.write_text(json.dumps(results, indent=2) + "\n")
        pending.replace(output / "results.json")

    while time.monotonic() - began < config.seconds:
        wat.write_text(module(rng, config.wasm_cases, routes))
        commands = {
            "encode": [encoder, "parse", str(wat), "-o", str(wasm)],
            "validate": [encoder, "validate", "--features", "all", str(wasm)],
            **engines,
        }
        for name, command in commands.items():
            process = subprocess.run(command, capture_output=True, timeout=30)
            if process.returncode:
                (output / "failure.log").write_bytes(process.stdout + process.stderr)
                results.update(complete=False, failed_step=name, failure_index=completed, status=process.returncode)
                save()
                return 1
        hashed.update(bytes.fromhex(digest(wasm)))
        completed += 1
        if completed == 1 or completed % 32 == 0:
            results.update(complete=False, modules=completed, routes=routes, ordered_module_sha256=hashed.hexdigest(),
                           elapsed_seconds=time.monotonic() - began)
            save()
            print(f"checked {completed} modules", flush=True)
    results.update(complete=True, modules=completed, routes=routes, ordered_module_sha256=hashed.hexdigest(),
                   elapsed_seconds=time.monotonic() - began)
    save()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
