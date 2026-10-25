#!/usr/bin/env python3
"""@brief Run short acceptance probes against the actual built ROS VM.

@details Fixtures are encoded directly using the standard Wasm binary format,
so the user needs neither a second compiler nor downloaded test artifacts.
Every probe executes a fresh VM process. Signed cache reuse, real native
frames, WASI calls, ROS owner admission and the live debugger must all work.
No robot, endpoint, filesystem directory or ROS transport peer is authorized.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import struct
import subprocess
import sys
import tempfile
import time

ROOT = Path(__file__).resolve().parents[2]
ANSI = re.compile(rb"\x1b\[[0-9;]*m")


def uleb(value):
    """Encode an unsigned integer in the Wasm binary format."""
    result = bytearray()
    while value >= 128:
        result.append((value & 127) | 128)
        value >>= 7
    result.append(value)
    return bytes(result)


def vector(items):
    """Encode a length-prefixed vector of already encoded elements."""
    return uleb(len(items)) + b"".join(items)


def name(value):
    """Encode a UTF-8 import, export or debug name."""
    encoded = value.encode()
    return uleb(len(encoded)) + encoded


def section(number, contents):
    """Encode one bounded standard or custom section."""
    return bytes([number]) + uleb(len(contents)) + contents


def function_type(parameters, results):
    """Describe public Wasm parameters; private native ABI values never enter it."""
    return b"\x60" + vector([bytes([item]) for item in parameters]) + vector([bytes([item]) for item in results])


def body(instructions, locals_=b"\x00"):
    """Terminate and length-prefix one function body."""
    encoded = locals_ + instructions + b"\x0b"
    return uleb(len(encoded)) + encoded


def fixtures():
    """Return a real imported guest and a finite-size debugger loop fixture."""
    i32, i64 = 0x7f, 0x7e
    types = [function_type([i32, i64, i32], [i32]), function_type([i32] * 4, [i32]),
             function_type([i32, i64, i64, i64], [i32]), function_type([i32], [])]
    imports = [name("wasi_snapshot_preview1") + name("clock_time_get") + b"\x00\x00",
               name("wasi_snapshot_preview1") + name("fd_write") + b"\x00\x01"]
    for version in (1, 2):
        imports.append(name(f"uwvm_ros{version}_v1") + name("capability_open") + b"\x00\x02")
    # clock_time_get(monotonic, 0, 64), then compare errno with 0 or NOTCAPABLE.
    leaf = b"\x41\x01\x42\x00\x41\xc0\x00\x10\x00\x21\x01"
    leaf += b"\x20\x01\x41\xcc\x00\x41\x00\x20\x00\x41\x02\x46\x1b\x47\x04\x40\x00\x0b"
    for index in (2, 3):
        # Empty capability names reach EINVAL only after owner authentication.
        leaf += b"\x41\x00\x42\x00\x42\x00\x42\x10\x10" + uleb(index)
        leaf += b"\x41\x16\x47\x04\x40\x00\x0b"
    leaf += b"\x41\x01\x41\x00\x41\x01\x41\x10\x10\x01\x04\x40\x00\x0b"
    leaf += b"\x20\x00\x41\x01\x46\x04\x40\x00\x0b"
    message = b"BASIC_WASI_ROS_OK\n"
    data = struct.pack("<II", 32, len(message)).ljust(32, b"\x00") + message
    guest = b"\0asm\1\0\0\0" + section(1, vector(types)) + section(2, vector(imports))
    guest += section(3, vector([b"\x03"] * 3)) + section(5, b"\x01\x01\x01\x01")
    guest += section(7, vector([name("_start") + b"\x00\x06", name("memory") + b"\x02\x00"]))
    guest += section(10, vector([body(leaf, b"\x01\x01\x7f"), body(b"\x20\x00\x10\x04"), body(b"\x20\x00\x10\x05")]))
    guest += section(11, vector([b"\x00\x41\x00\x0b" + uleb(len(data)) + data]))
    # Match the independently decoded numeric prefix used by native_case.
    loop = b"\x41\x11\x21\x00\x03\x40\x20\x00\x41\x29\x73\x21\x00"
    loop += b"\x20\x00\x41\x01\x77\x21\x00\x0c\x00\x0b"
    debug = b"\0asm\1\0\0\0" + section(1, vector([function_type([], [])]))
    debug += section(3, b"\x01\x00") + section(7, vector([name("_start") + b"\x00\x00"]))
    debug += section(10, vector([body(loop, b"\x01\x01\x7f")]))
    debug += section(0, name("name") + section(1, vector([b"\x00" + name("window")])))
    return guest, debug, message


def main():
    """Fail the command unless every mandatory product probe really succeeds."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--uwvm", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != "linux":
        parser.error("the ROS product probes require Linux")
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    args.uwvm = args.uwvm.resolve(strict=True)
    args.out.mkdir(parents=True, exist_ok=True)
    output = Path(tempfile.mkdtemp(prefix="run-", dir=args.out.resolve()))
    report = {"passed": False, "runtime_sha256": hashlib.sha256(args.uwvm.read_bytes()).hexdigest(),
              "machine": os.uname().machine, "commands": [], "checks": {}}

    def run(label, arguments, *, regular_output=False):
        """Run a fresh process, optionally reproducing normal file redirection."""
        command = [str(args.uwvm), *map(str, arguments)]
        start = time.monotonic()
        if regular_output:
            with (output / (label + ".stdout")).open("wb") as stdout, (output / (label + ".stderr")).open("wb") as stderr:
                child = subprocess.run(command, stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, timeout=45)
            result = subprocess.CompletedProcess(command, child.returncode,
                (output / (label + ".stdout")).read_bytes(), (output / (label + ".stderr")).read_bytes())
        else:
            result = subprocess.run(command, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE, timeout=45)
        row = {"name": label, "argv": command, "returncode": result.returncode, "seconds": time.monotonic() - start}
        for stream in ("stdout", "stderr"):
            data = getattr(result, stream)
            (output / (label + "." + stream)).write_bytes(data)
            row[stream + "_sha256"] = hashlib.sha256(data).hexdigest()
        report["commands"].append(row)
        return result, ANSI.sub(b"", result.stderr)

    def require(condition, message):
        if not condition:
            raise RuntimeError(message)

    try:
        guest, debug, message = fixtures()
        wasm = output / "basic.wasm"
        wasm.write_bytes(guest)
        debugger_wasm = output / "debug.wasm"
        debugger_wasm.write_bytes(debug)
        cache = output / "cache"
        base = ["-Raot", "-Rct", "0", "-Rllvm-policy", "max", "--log-verbose", "--runtime-compiler-log", "err",
                "--wasip1-global-noinherit-system-environment",
                "--runtime-llvm-jit-cache-seal-process", "-Rllvm-cache-path", "path", cache]
        granted = [*base, "--wasip1-global-grant", "monotonic-clock", "-Wstart", "2", "0", "--run", wasm]
        cold, text = run("cold", granted)
        require(cold.returncode == 0 and cold.stdout == message, "AOT WASI/ROS execution failed (see cold.stderr)")
        require(b"call_stack=unwind " in text and b"unwind_check=live" in text and b"call_stack_frames=omit" in text,
                "the current product did not use live-checked native unwinding")
        require(re.search(rb"object-cache-store-complete [^\n]*status=ok\b", text),
                "cache storage failed; a refused/disabled cache is not a warm-start pass (see cold.stderr)")
        for number in range(2):
            warm, text = run(f"warm-{number}", granted)
            require(warm.returncode == 0 and warm.stdout == message, "cached WASI/ROS execution failed")
            require(re.search(rb"object-cache-hit [^\n]*signature_verified=1\b", text)
                    and b"object-cache-store-enqueue " not in text, "fresh process did not reuse a verified object")
        report["checks"]["cache_wasi_ros"] = "PASS: cold write and two verified fresh-process hits"
        redirected, text = run("file-warm", granted, regular_output=True)
        require(redirected.returncode == 0 and redirected.stdout == message
                and re.search(rb"object-cache-hit [^\n]*signature_verified=1\b", text)
                and b"object-cache-store-enqueue " not in text,
                "ordinary stdout/stderr redirection disabled signed object reuse")
        report["checks"]["cache_file_output"] = "PASS: verified reuse with ordinary stdout/stderr files"
        trapped, text = run("unwind-trap", [*base, "--wasip1-global-grant", "monotonic-clock", "-Wstart", "2", "1", "--run", wasm])
        require(trapped.returncode < 0 and [int(index) for index in re.findall(rb"func_idx=([0-9]+)", text)] == [4, 5, 6],
                "native trap did not report the actual leaf/middle/entry chain")
        report["checks"]["unwind"] = "PASS: real three-function native trap chain"
        denied, text = run("wasi-revoked", [*base, "-Wstart", "2", "2", "--run", wasm])
        require(denied.returncode == 0 and denied.stdout == message, "cached guest retained a prior WASI clock grant")
        for policy in ("instruction", "none", "auto", "unwind-uncheck"):
            rejected, text = run("removed-" + policy, ["-Rllvm-call-stack", policy, "--run", wasm])
            require(rejected.returncode != 0 and b"only checked native unwind" in text, "removed stack policy was accepted")
        invalid, text = run("incomplete-ros-policy", ["-Raot", "--ros1-global-enable", "--run", wasm])
        require(invalid.returncode != 0 and b"ROS policy:" in text,
                "the owning AOT loader skipped ROS policy validation")
        report["checks"]["policy"] = "PASS: revoked clock and removed stack policies"
        sys.path.insert(0, str(ROOT / "test/0018.debugger"))
        import run_dap_current_broker as live
        debug_output = output / "debugger"
        debug_output.mkdir()
        session = live.BrokerSession(args.uwvm, ROOT / "tools/debug/secure_server.py", ROOT / "tools/debug/dap_adapter.py",
                                     ["-Raot", "-Rct", "0", "-Rllvm-cache-path", "disable", "--run", str(debugger_wasm)], debug_output)
        try:
            report["native_debugger"] = live.native_case(session, debugger_wasm)
        finally:
            report["debugger_cleanup"] = session.close()
        report["checks"]["debugger"] = "PASS: genuine native disassembly, stepping and stale-reference refusal"
        report["passed"] = True
    except Exception as error:
        report["failure"] = str(error)
        raise
    finally:
        (output / "result.json").write_text(json.dumps(report, indent=2) + "\n")
        print("basic_test receipt:", output / "result.json", flush=True)
    for label, result in report["checks"].items():
        print(label + ": " + result)


if __name__ == "__main__":
    main()
