#!/usr/bin/env python3
"""Run real dynamic worker reconstruction using an already built embedding fixture.

The caller must enter the required memory/swap cgroup. All binaries and output
paths are explicit; this runner neither builds nor installs a toolchain.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import subprocess
import time

CASES = ("shared-memories", "peer-memories", "peer-memories64", "private-mte", "private-memories", "private-memory64-bounds", "exception-storage", "dynamic-tables", "peer-tables", "table64-bounds","finite", "wasi", "pending", "proc-exit", "grow", "mte", "mte-wait",
         "mte-fault", "mte-log", "trap", "bounds", "provider-limit", "nested", "capacity", "gc-eh", "gc-global", "gc-references", "gc-cast-fault", "gc-exception-references", "active-data", "start-expressions", "peer-start", "peer-memoryless", "peer-globals", "peer-tags", "typed-functions", "peer-functions", "gc-global-initial", "peer-host-reference")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--fixtures", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path, help="existing QEMU/loader argv array")
    parser.add_argument("--engine", choices=("int", "llvm"), action="append")
    parser.add_argument("--case", choices=CASES, action="append")
    parser.add_argument("--memtag-backend", choices=("software", "hardware"), default="software")
    parser.add_argument("--memtag-mode", choices=("sync", "async"), default="sync")
    parser.add_argument("--memtag-action", choices=("crash", "silent-log"), default="crash")
    parser.add_argument("--prefix-at", type=int, action="append", help="additional exact journal prefix to park, inspect and continue")
    args = parser.parse_args()
    binary = args.binary.resolve(strict=True)
    fixtures = args.fixtures.resolve(strict=True)
    output = args.output.resolve(); output.mkdir(parents=True, exist_ok=False)
    runner = json.loads(args.runner_json.read_text()) if args.runner_json else []
    if not isinstance(runner, list) or any(not isinstance(x, str) or not x for x in runner):
        raise ValueError("runner must be an explicit argv array")
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    results = []
    (output / "provenance.json").write_text(json.dumps({
        "binary": str(binary), "sha256": hashlib.sha256(binary.read_bytes()).hexdigest(),
        "cgroup": Path("/proc/self/cgroup").read_text(),
        "fixtures": {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
                     for p in sorted(fixtures.glob("*.wasm"))},
    }, indent=2))
    for case in args.case or CASES:
        for engine in args.engine or ("int", "llvm"):
            if case.startswith("mte") and bytes.fromhex("ff af 80 84 08 00") not in (fixtures / (case + ".wasm")).read_bytes():
                raise ValueError("MTE fixture has no tag instruction; generate it with make_fixtures.py")
            grows = case in ("grow", "mte", "mte-wait", "mte-fault", "mte-log")
            command = runner + [str(binary), "control", engine, "--",
                "-Rint" if engine == "int" else "-Raot", "-Rct", "0",
                "-Rllvm-cache-path", "disable", "--runtime-llvm-jit-policy", "debug",
                "--debug-permissions", "full", "--debug-fault-action", "exit",
                "--wasi-threads", "2" if case == "capacity" else "3", "10", "4194304" if case in ("dynamic-tables", "peer-tables", "exception-storage", "private-memories", "private-memory64-bounds", "private-mte", "peer-memories", "peer-memories64", "shared-memories") else "1048576",
                "16777216", "67108864", "720896" if case == "shared-memories" else "262144" if grows else "65536",
                "--wasm-set-main-module-name", "main", "--wasithread-single-create", "main",
                "--wasithread-single-enable", "main", "--wasithread-single-interface",
                "main", "thread-spawn", "allow", "--wasm-preload-library",
                str(fixtures / ("owner-multi.wasm" if case == "shared-memories" else "owner-grow.wasm" if grows else "owner.wasm")), "env"]
            if case == "shared-memories":
                command += ["--wasm-preload-library", str(fixtures / "owner-wide.wasm"), "other"]
            if case in ("wasi", "pending", "proc-exit", "typed-functions"):
                command += ["--wasip1-single-create", "main", "--wasip1-single-enable", "main"]
                grants = [("random_get", "random")] if case == "typed-functions" else [("proc_exit", "proc-exit")] if case == "proc-exit" else [
                    ("fd_write", "stdout.write"),
                    ("fd_read", "stdin.read") if case == "pending" else ("random_get", "random")]
                for function, permission in grants:
                    command += ["--wasip1-single-interface", "main", function, "allow",
                                "--wasip1-single-grant", "main", permission]
            if case in ("peer-start", "peer-memoryless"):
                command += ["--wasm-preload-library", str(fixtures / ("start-peer.wasm" if case == "peer-start" else "memoryless-peer.wasm")), "peer"]
            if case == "peer-globals":
                command += ["--wasm-preload-library", str(fixtures / "global-peer.wasm"), "peer"]
            if case == "peer-host-reference":
                command += ["--wasm-preload-library", str(fixtures / "host-reference-peer.wasm"), "peer",
                            "--wasip1-single-create", "peer", "--wasip1-single-enable", "peer",
                            "--wasip1-single-interface", "peer", "random_get", "allow",
                            "--wasip1-single-grant", "peer", "random"]
            if case in ("peer-memories", "peer-memories64"):
                command += ["--wasm-preload-library", str(fixtures / ("memory64-peer.wasm" if case == "peer-memories64" else "memory-peer.wasm")), "peer",
                            "--wasip1-single-create", "peer", "--wasip1-single-enable", "peer",
                            "--wasip1-single-interface", "peer", "random_get_wasm64" if case == "peer-memories64" else "random_get", "allow",
                            "--wasip1-single-grant", "peer", "random"]
            if case == "peer-functions":
                command += ["--wasm-preload-library", str(fixtures / "function-peer.wasm"), "peer"]
            if case == "peer-tables":
                command += ["--wasm-preload-library", str(fixtures / "table-peer.wasm"), "peer"]
            if case == "peer-tags":
                command += ["--wasm-preload-library", str(fixtures / "tag-peer.wasm"), "peer"]
            command += ["--runtime-memtag-backend", args.memtag_backend,
                        "--runtime-memtag-mode", args.memtag_mode,
                        "--runtime-memtag-action", args.memtag_action]
            if args.memtag_action == "silent-log": command += ["err"]
            command += ["--run", str(fixtures / (case + ".wasm"))]
            modes = ["record", "replay", "prefix"] + ["prefix-at-" + str(i) for i in args.prefix_at or ()]
            for mode in modes:
                label = case + "-" + engine + "-" + mode
                environment = {k: v for k, v in os.environ.items() if not k.startswith("UWVM_TEST_")}
                environment["UWVM_TEST_HISTORY"] = str(output / (case + "-" + engine + ".history"))
                if mode == "record": environment["UWVM_TEST_SAME_PROCESS"] = "1"
                else: environment["UWVM_TEST_REPLAY"] = "1"
                if mode.startswith("prefix"): environment["UWVM_TEST_PREFIX"] = "1"
                if mode.startswith("prefix-at-"): environment["UWVM_TEST_PREFIX_AT"] = mode[len("prefix-at-"):]
                if case == "pending" and mode == "record": environment["UWVM_TEST_PENDING"] = "1"
                if case == "proc-exit": environment["UWVM_TEST_EXIT_CODE"] = "73"
                if case in ("trap", "bounds", "gc-cast-fault", "table64-bounds", "private-memory64-bounds") or (case in ("mte-fault", "mte-log") and args.memtag_action == "crash"):
                    environment["UWVM_TEST_TRAP"] = "1"
                (output / (label + ".argv.json")).write_text(json.dumps(command, indent=2))
                start = time.monotonic()
                try:
                    process = subprocess.run(command, env=environment, stdout=subprocess.PIPE,
                                             stderr=subprocess.STDOUT, timeout=90)
                    code, text = process.returncode, process.stdout
                except subprocess.TimeoutExpired as error:
                    code, text = 124, error.output or b""
                (output / (label + ".log")).write_bytes(text)
                expected_writes = 2 if case in ("wasi", "pending") and mode == "record" else 0
                passed = code == 0 and text.count(b"THREAD-ONCE") == expected_writes
                row = dict(case=case, engine=engine, mode=mode, code=code, passed=passed,
                           seconds=time.monotonic() - start, external_writes=text.count(b"THREAD-ONCE"))
                results.append(row); print(row, flush=True)
                (output / "results.json").write_text(json.dumps(results, indent=2))
                if not passed:
                    print(text[-4000:].decode(errors="replace"), flush=True)
                    break
    return int(any(not row["passed"] for row in results))


if __name__ == "__main__":
    raise SystemExit(main())
