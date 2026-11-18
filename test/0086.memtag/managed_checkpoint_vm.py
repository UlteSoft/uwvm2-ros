#!/usr/bin/env python3
"""Check compatibility of authenticated worker origins with tag-only history."""
import argparse
import hashlib
import json
import resource
import subprocess
from pathlib import Path

from threaded_matrix import shared_module
from vm_matrix import c, module, op, sec, u


def static_section(length):
    name = b"uwvm.memtag.linear.globals"
    return sec(0, u(len(name)) + name + b"\x02\x01\x00\x00" + u(length))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--legacy-vm", type=Path, help="VM with tag-only recording before source classification")
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    vm = args.vm.resolve()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)

    # The preloaded env is the physical owner. The main module imports this
    # memory and has the only _start. Both source files form the journal origin.
    owner_bytes = b"\0asm\x01\0\0\0" + sec(5, b"\x01\x03\x01\x02") + sec(7, b"\x01\x06memory\x02\x00")
    cases = [
        ("ordinary", b"", b"", False, True),
        ("raw-tag", op(0x20) + b"\x1a", b"", False, True),
        ("instruction-word-in-constant", b"\x42" + op(0x40)[:-1] + b"\x1a", b"", False, True),
        ("empty-owner-descriptor", b"", static_section(0), False, True),
        ("permanent-preload", b"", static_section(16), False, False),
        ("permanent-disabled", b"", static_section(16), True, True),
    ]
    for code, name in enumerate(("claim", "claimz", "reserve", "activate", "release"), 0x40):
        # Refusal must happen even before a function can establish an entry, or
        # when this instruction is unreachable. No execution-state heuristic.
        cases.append((name, b"\x0f" + c(0) + c(16) + op(code) + b"\x1a", b"", False, False))
    cases.append(("managed-disabled", b"\x0f" + c(0) + c(16) + op(0x40) + b"\x1a", b"", True, True))

    with vm.open("rb") as source:
        report = {"vm_sha256": hashlib.file_digest(source, "sha256").hexdigest(), "cases": []}

    if args.legacy_vm:
        with args.legacy_vm.open("rb") as source:
            report["legacy_vm_sha256"] = hashlib.file_digest(source, "sha256").hexdigest()
    for name, code, custom, disabled, accepts in cases:
        owner = out / (name + "-env.wasm")
        guest = out / (name + ".wasm")
        owner.write_bytes(owner_bytes + custom)
        guest.write_bytes(shared_module(module(code)))
        for engine in ("int", "aot"):
            archive = out / (name + "-" + engine + ".uwsnap")
            common = [str(vm), "--debug-permissions", "full", "--debug-fault-action", "exit", "--runtime-" + engine,
                      "--runtime-compile-threads", "0", "--wasm-feature-enable-threads", "--wasi-threads",
                      "3", "3", "1048576", "16777216", "67108864", "131072", "--wasm-set-main-module-name", "main",
                      "--wasip1-single-create", "main", "--wasip1-single-enable", "main", "--wasip1-single-interface",
                      "main", "proc_exit", "allow", "--wasip1-single-grant", "main", "proc-exit",
                      "--wasm-preload-library", str(owner), "env"]
            if engine == "aot":
                common += ["--runtime-llvm-jit-cache-path", "disable", "--runtime-llvm-jit-policy", "debug"]
            if disabled:
                common += ["--runtime-disable-memtag"]
            for mode in ("record", "replay") if accepts else ("record",):
                command = common + ["--debug-thread-history", mode, str(archive), "4194304", "--run", str(guest)]
                completed = subprocess.run(command, capture_output=True, timeout=40)
                log = (completed.stdout + completed.stderr).decode(errors="replace")
                (out / (name + "-" + engine + "-" + mode + ".log")).write_text(log)
                passed = ((completed.returncode == 0 and archive.is_file() and archive.stat().st_size > 0)
                          if accepts else (completed.returncode == 125 and not archive.exists()
                                           and "Thread history origin does not match" in log))
                report["cases"].append({"name": name, "engine": engine, "mode": mode,
                                        "accepts": accepts, "exit": completed.returncode,
                                        "archive_exists": archive.exists(), "passed": passed, "command": command})
                (out / "results.json").write_text(json.dumps(report, indent=2) + "\n")
            if args.legacy_vm and not accepts:
                legacy = out / (name + "-" + engine + "-legacy.uwsnap")
                for mode in ("record", "replay"):
                    selected = [str(args.legacy_vm.resolve()) if mode == "record" else str(vm), *common[1:]]
                    command = selected + ["--debug-thread-history", mode, str(legacy), "4194304", "--run", str(guest)]
                    completed = subprocess.run(command, capture_output=True, timeout=40)
                    log = (completed.stdout + completed.stderr).decode(errors="replace")
                    (out / (name + "-" + engine + "-legacy-" + mode + ".log")).write_text(log)
                    passed = (legacy.is_file() and legacy.stat().st_size > 0
                              and (completed.returncode == 0 if mode == "record" else
                                   completed.returncode == 125 and "Thread history origin does not match" in log))
                    report["cases"].append({"name": name, "engine": engine, "mode": "legacy-" + mode,
                                            "passed": passed, "exit": completed.returncode, "command": command})
    report["complete"] = all(row["passed"] for row in report["cases"])
    (out / "results.json").write_text(json.dumps(report, indent=2) + "\n")
    return 0 if report["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
