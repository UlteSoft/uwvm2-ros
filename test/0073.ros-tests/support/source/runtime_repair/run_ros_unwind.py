#!/usr/bin/env python3
"""Verify ROS admission and mandatory checked native AOT stack unwinding.

@details Each guest calls the real ROS capability API, checks an ungranted
operation leaves its output untouched, then returns or traps. Native traces
must contain exactly the physical Wasm activations, including recursion; ROS
provenance passed in the private native ABI must not add shadow-stack frames.
Optional signed-cache qualification requires a completed cold write and fresh
process hits. A cache refusal is a failure, never a warm-start observation.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import subprocess
import time


POLICIES = ("unwind", "default")
REMOVED_POLICIES = ("instruction", "unwind-uncheck", "none", "auto")
ANSI = re.compile(rb"\x1b\[[0-9;]*m")


def fixture(version, recursive):
    """Return a real ROS import with a parameter-selected, nonconstant trap."""
    middle = """(func $middle (param $n i32) (param $kind i32)
      (if (i32.eqz (local.get $n))
        (then (call $leaf (local.get $kind)))
        (else (call $middle (i32.sub (local.get $n) (i32.const 1)) (local.get $kind)))))"""
    if not recursive:
        middle = "(func $middle (param $kind i32) (call $leaf (local.get $kind)))"
    call = "(call $middle (i32.const 3) (local.get $kind))" if recursive else "(call $middle (local.get $kind))"
    return f'''(module
      (import "uwvm_ros{version}_v1" "capability_open"
        (func $open (param i32 i64 i64 i64) (result i32)))
      (memory (export "memory") 1 1)
      (data (i32.const 0) "x")
      (func $leaf (param $kind i32)
        ;; An authenticated owner reaches argument validation and returns EINVAL.
        (if (i32.ne (call $open (i32.const 0) (i64.const 0) (i64.const 0) (i64.const 16))
                    (i32.const 22)) (then unreachable))
        ;; Enabling a backend alone grants no endpoint and must not alter output.
        (i64.store (i32.const 16) (i64.const -1))
        (if (i32.ne (call $open (i32.const 0) (i64.const 0) (i64.const 1) (i64.const 16))
                    (i32.const 13)) (then unreachable))
        (if (i64.ne (i64.load (i32.const 16)) (i64.const -1)) (then unreachable))
        (if (i32.eq (local.get $kind) (i32.const 1)) (then unreachable))
        (if (i32.eq (local.get $kind) (i32.const 2))
          (then (drop (i32.div_s (i32.const 7) (i32.const 0)))))
        (if (i32.eq (local.get $kind) (i32.const 3))
          (then (drop (i32.trunc_f32_s (f32.const nan)))))
        (if (i32.eq (local.get $kind) (i32.const 4))
          (then (drop (i32.load (i32.const -1)))))
        (if (i32.eq (local.get $kind) (i32.const 5))
          (then (i64.store (i32.const -1) (i64.const 7)))))
      {middle}
      (func $entry (export "_start") (param $kind i32) {call}))'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("uwvm", "wat2wasm", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--cache-root", type=Path,
                        help="Require real signed cache hits in a fresh owner-controlled directory")
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    args.out.mkdir(parents=True, exist_ok=False)
    if args.cache_root:
        args.cache_root.mkdir(parents=True, exist_ok=False, mode=0o700)
    report = {"passed": False, "runtime_sha256": hashlib.sha256(args.uwvm.read_bytes()).hexdigest(),
              "commands": [], "verified_warm_hits": 0, "scope": "ROS ABI admission and diagnostics; no transport peer"}

    def run(name, argv):
        started = time.monotonic()
        result = subprocess.run([str(item) for item in argv], stdin=subprocess.DEVNULL,
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=90)
        (args.out / (name + ".log")).write_bytes(result.stdout)
        report["commands"].append({"name": name, "argv": [str(item) for item in argv],
                                   "returncode": result.returncode, "seconds": time.monotonic() - started,
                                   "log_sha256": hashlib.sha256(result.stdout).hexdigest()})
        return result, ANSI.sub(b"", result.stdout)

    try:
        assert os.uname().sysname == "Linux"
        cg = Path("/sys/fs/cgroup")
        report["cgroup"] = {key: (cg / key).read_text().strip()
                            for key in ("memory.max", "memory.swap.max", "cgroup.procs")}
        assert report["cgroup"]["memory.max"] == "68719476736"
        assert report["cgroup"]["memory.swap.max"] == "0"
        assert str(os.getpid()) in report["cgroup"]["cgroup.procs"].split()
        for version in (1, 2):
            for recursive in (False, True):
                shape = "recursive" if recursive else "chain"
                wat = args.out / f"ros{version}-{shape}.wat"
                wat.write_text(fixture(version, recursive))
                wasm = wat.with_suffix(".wasm")
                compiled, _ = run(wat.stem + "-compile", [args.wat2wasm, "--debug-names", wat, "-o", wasm])
                assert compiled.returncode == 0
                expected = [1, 2, 2, 2, 2, 3] if recursive else [1, 2, 3]
                for policy in POLICIES:
                    prefix = f"ros{version}-{shape}-{policy}"
                    argv = [args.uwvm, "-Raot", "-Rct", "0", "-Rllvm-policy", "max",
                            "--log-verbose", "--runtime-compiler-log", "err",
                            "--wasip1-global-noinherit-system-environment"]
                    if policy != "default":
                        argv += ["-Rllvm-call-stack", policy]
                    if args.cache_root:
                        argv += ["--runtime-llvm-jit-cache-seal-process", "-Rllvm-cache-path", "path", args.cache_root / prefix]
                    else:
                        argv += ["-Rllvm-cache-path", "disable"]
                    seed, text = run(prefix + "-seed", [*argv, "-Wstart", "2", "0", "--run", wasm])
                    assert seed.returncode == 0, prefix + ": ROS admission or normal return failed"
                    assert b"call_stack=unwind " in text and b"unwind_check=live" in text
                    assert b"call_stack_frames=omit" in text and b"unwind_replace_frames=yes" in text
                    if args.cache_root:
                        assert re.search(rb"object-cache-store-complete [^\n]*status=ok\b", text), "cold store failed"
                    kinds = range(1, 6) if not recursive and policy == "unwind" else (1,)
                    for kind in kinds:
                        result, text = run(prefix + f"-trap-{kind}", [*argv, "-Wstart", "2", str(kind), "--run", wasm])
                        assert result.returncode < 0, prefix + ": expected a genuine fatal trap"
                        frames = [int(value) for value in re.findall(rb"func_idx=([0-9]+)", text)]
                        assert frames == expected, (prefix, kind, frames, expected)
                        assert b"native Wasm backtrace unavailable" not in text
                        if args.cache_root:
                            hits = re.findall(rb"object-cache-hit [^\n]*signature_verified=1\b", text)
                            assert hits and b"object-cache-store-enqueue " not in text, "trap process did not reuse a verified object"
                            report["verified_warm_hits"] += len(hits)
        for policy in REMOVED_POLICIES:
            rejected, text = run("removed-" + policy,
                                 [args.uwvm, "-Raot", "-Rllvm-call-stack", policy, "--run", wasm])
            assert rejected.returncode != 0 and b"only checked native unwind" in text
        report["passed"] = True
    except BaseException as error:
        report["failure"] = repr(error)
        raise
    finally:
        (args.out / "result.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
