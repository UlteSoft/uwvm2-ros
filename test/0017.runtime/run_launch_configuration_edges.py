#!/usr/bin/env python3
"""Finite real-CLI regression: expected launch refusal is not a guest crash.

No terminal `unreachable` is used as an access oracle. Successful Wasm cases
must return 0; refused configuration must return 126 AND its specific diagnostic.
The optional inherited-FD probes require a Linux LLVM-full product.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource
import shlex
import socket
import subprocess


def u(value):
    result = bytearray()
    while True:
        byte = value & 127
        value >>= 7
        result.append(byte | (128 if value else 0))
        if not value:
            return bytes(result)


def section(number, data):
    return bytes([number]) + u(len(data)) + data


def module(memories=1, code=b"", wide=False, minimum=1, maximum=8, params=b""):
    body = b"\0" + code + b"\x0b"
    wasm = b"\0asm\x01\0\0\0" + section(1, b"\x01\x60" + u(len(params)) + params + b"\0")
    wasm += section(3, b"\x01\0")
    if memories:
        wasm += section(5, u(memories) + (u(5 if wide else 1) + u(minimum) + u(maximum)) * memories)
    return wasm + section(7, b"\x01\x06_start\0\0") + section(10, b"\x01" + u(len(body)) + body)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--engine", choices=("int", "aot"), required=True)
    parser.add_argument("--runner", default="", help="e.g. 'qemu-aarch64 -L SYSROOT -cpu max'")
    parser.add_argument("--fd-tests", action="store_true")
    args = parser.parse_args()
    args.vm = args.vm.resolve()
    args.out = args.out.resolve()
    args.out.mkdir(parents=True, exist_ok=False)
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    fixtures = {
        "ok": module(), "none": module(0), "multi": module(2), "mem64": module(wide=True),
        "argument": module(params=b"\x7f", code=b"\x20\0\x41\x2a\x47\x04\x40\0\x0b"),
        "last-byte": module(code=b"\x41\xff\xff\x03\x2d\0\0\x1a"),
        "unaligned": module(code=b"\x41\x01\x28\0\0\x1a"),
        "oob-byte": module(code=b"\x41\x80\x80\x04\x2d\0\0\x1a"),
        "grow-fail": module(maximum=1, code=b"\x41\x01\x40\0\x41\x7f\x47\x04\x40\0\x0b"),
    }
    for name, data in fixtures.items():
        (args.out / (name + ".wasm")).write_bytes(data)
    base = shlex.split(args.runner) + [str(args.vm), "--runtime-" + args.engine,
        "--wasm-feature-enable-multi-memory", "--wasm-feature-enable-memory64",
        "--wasip1-single-create", "main", "--wasip1-single-enable", "main"]
    if args.engine == "aot":
        base += ["--runtime-llvm-jit-cache-path", "disable", "--runtime-compile-threads", "0"]
    rows = []

    def run(name, fixture="ok", flags=(), expected=0, diagnostic="", entry=True, fd=None):
        naming = [] if "-Wname" in flags else ["--wasm-set-main-module-name", "main"]
        command = base + naming + list(flags) + (["-Wstart", "0"] if entry else [])
        command += ["--run", str(args.out / (fixture + ".wasm"))]
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                timeout=90, pass_fds=() if fd is None else (fd,))
        output = result.stderr.decode("utf-8", "replace")
        passed = result.returncode == expected and diagnostic.lower() in output.lower()
        if expected in (120, 121, 122, 123):
            category={120:"unreachable",121:"execution",122:"memory-protection",123:"runtime-internal"}[expected]
            passed = passed and f"uwvm: [crash] {category}\n" in output and f"\n  Exit status: {expected}\n" in output and '\"schema\"' not in output
        for stream, data in (("stdout", result.stdout), ("stderr", result.stderr)):
            (args.out / (name + "." + stream)).write_bytes(data)
        rows.append(dict(name=name, command=command, exit=result.returncode, expected=expected,
                         diagnostic=diagnostic, passed=passed, fixture_sha256=hashlib.sha256(fixtures[fixture]).hexdigest()))

    run("control")
    for option in ("-Wmemlim", "--wasm-set-memory-limit"):
        suffix = "short" if option == "-Wmemlim" else "long"
        run("unknown-module-" + suffix, flags=[option, "missing", "0", "1", "1"], expected=126, diagnostic="unknown or non-runtime module")
        for index in ("all", "0"):
            run("excessive-min-" + index + "-" + suffix, flags=[option, "main", index, "100000", "100000"], expected=126, diagnostic="initial-allocation limit")
        run("memory-index-" + suffix, fixture="multi", flags=[option, "main", "5", "1", "1"], expected=126, diagnostic="out-of-range memory index")
        for index in ("all", "0"):
            run("no-memory-" + index + "-" + suffix, fixture="none", flags=[option, "main", index, "1", "1"], expected=126, diagnostic="module defines none")
        run("valid-memory-index-" + suffix, fixture="multi", flags=[option, "main", "1", "1", "1"])
        run("max-is-cap-" + suffix, flags=[option, "main", "all", "1", "4294967295"])
    run("renamed-module", flags=["-Wname", "renamed", "-Wmemlim", "main", "all", "1", "1"], expected=126, diagnostic="unknown or non-runtime module")
    for quota in ("runtime_modules", "local_defined_memories", "local_defined_functions", "local_defined_codes"):
        run("quota-" + quota, flags=["-Wilim", quota, "0"], expected=126, diagnostic="reserve limit exceeded")
    run("zero-unused-quota", fixture="none", flags=["-Wilim", "local_defined_memories", "0"])
    run("zero-memory-min", flags=["-Wmemlim", "main", "all", "0", "0"])
    # Read the target ELF class, not the Python launcher's native word size.
    image = args.vm.read_bytes()
    if image[:4] == b"\x7fELF":
        min64 = "16777217" if image[4] == 2 else "4097"
        run("memory64-min", fixture="mem64", flags=["-Wmemlim", "main", "all", min64, min64], expected=126, diagnostic="initial-allocation limit")
    for option in ("-Wstart", "--wasm-set-start-func"):
        suffix = "short" if option == "-Wstart" else "long"
        for index in ("1", "4294967295"):
            run("entry-index-" + index + "-" + suffix, flags=[option, index], entry=False, expected=126, diagnostic="--wasm-set-start-func")
        run("entry-arity-" + suffix, flags=[option, "0", "i32", "42"], entry=False, expected=126, diagnostic="argument count mismatch")
        run("entry-bad-argument-" + suffix, fixture="argument", flags=[option, "0", "bogus"], entry=False, expected=126, diagnostic="--wasm-set-start-func")
        run("entry-good-argument-" + suffix, fixture="argument", flags=[option, "0", "42"], entry=False)
    for fixture in ("last-byte", "unaligned", "grow-fail"):
        run("semantics-" + fixture, fixture=fixture)
    run("semantics-oob-byte", fixture="oob-byte", expected=121, diagnostic="memory access out of bounds")
    run("strict-limit-failure", fixture="grow-fail", flags=["-Wmemstrict"])
    if args.fd_tests:
        for endpoint in ("--diagnostic-control-fd", "--debug-jit-control-fd"):
            for permission in ("none", "limited", "read-only", "full"):
                host, vm = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
                with host, vm:
                    run(endpoint[2:] + "-" + permission,
                        flags=["--debug-permissions", permission, endpoint, str(vm.fileno())], fd=vm.fileno(),
                        expected=126 if permission == "none" else 0,
                        diagnostic="denies all" if permission == "none" else "")
    report = dict(binary=str(args.vm), binary_sha256=hashlib.sha256(image).hexdigest(),
                  passed=sum(row["passed"] for row in rows), total=len(rows), cases=rows)
    (args.out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({key: report[key] for key in ("passed", "total", "binary_sha256")}))
    for row in rows:
        if not row["passed"]:
            print("FAIL", row["name"], "exit", row["exit"], "expected", row["expected"])
    return 0 if report["passed"] == report["total"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
