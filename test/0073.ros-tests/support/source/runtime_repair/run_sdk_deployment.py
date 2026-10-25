#!/usr/bin/env python3
"""@brief Check the SDK's public launch arguments against the actual ROS VM.

@details A real Wasm command writes through its explicitly granted standard
stream, observes a denied clock and observes a disabled ROS module. Both ROS
families must run through checked native unwind. No compiler, ROS installation,
gateway, physical endpoint or implicit permission is needed for this probe.
"""

import argparse
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys


def load(path, name):
    """@brief Load the explicitly selected source module without bytecode files."""
    specification = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(specification)
    specification.loader.exec_module(module)
    return module


def guest(codec, backend):
    """@brief Encode independent expected errno and output checks as real Wasm."""
    i32, i64 = 0x7f, 0x7e
    types = [codec.function_type([i32] * 4, [i32]),
             codec.function_type([i32, i64, i32], [i32]),
             codec.function_type([i32, i64, i64, i64], [i32]),
             codec.function_type([], [])]
    imports = [codec.name("wasi_snapshot_preview1") + codec.name("fd_write") + b"\x00\x00",
               codec.name("wasi_snapshot_preview1") + codec.name("clock_time_get") + b"\x00\x01",
               codec.name(f"uwvm_{backend}_v1") + codec.name("capability_open") + b"\x00\x02"]
    # No clock grant: clock_time_get(monotonic, 0, 64) must return NOTCAPABLE.
    instructions = b"\x41\x01\x42\x00\x41\xc0\x00\x10\x01\x41\xcc\x00\x47\x04\x40\x00\x0b"
    # A well-formed, ungranted alias reaches policy denial rather than EINVAL.
    instructions += b"\x41\x00\x42\xc0\x00\x42\x05\x42\x10\x10\x02\x41\x0d\x47\x04\x40\x00\x0b"
    instructions += b"\x41\x01\x41\x00\x41\x01\x41\x10\x10\x00\x04\x40\x00\x0b"
    message = b"SDK_UNWIND_POLICY_OK\n"
    data = (32).to_bytes(4, "little") + len(message).to_bytes(4, "little")
    data = (data.ljust(32, b"\x00") + message).ljust(64, b"\x00") + b"probe"
    result = b"\x00asm\x01\x00\x00\x00"
    result += codec.section(1, codec.vector(types))
    result += codec.section(2, codec.vector(imports))
    result += codec.section(3, b"\x01\x03")
    result += codec.section(5, b"\x01\x01\x01\x01")
    result += codec.section(7, codec.vector([codec.name("_start") + b"\x00\x03",
                                           codec.name("memory") + b"\x02\x00"]))
    result += codec.section(10, codec.vector([codec.body(instructions)]))
    result += codec.section(11, codec.vector([b"\x00\x41\x00\x0b" + codec.uleb(len(data)) + data]))
    return result, message


def main():
    """@brief Require both real backend launches and preserve their diagnostics."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk-root", type=Path, required=True)
    parser.add_argument("--uwvm", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != "linux":
        parser.error("the product launch probes require Linux")
    sys.dont_write_bytecode = True
    deployment = load(args.sdk_root / "tools/platform/ros/deployment.py", "sdk_deployment")
    codec = load(Path(__file__).resolve().parents[3] / "basic_test.py", "basic_codec")
    args.out.mkdir(parents=True, exist_ok=False)
    runtime = str(args.uwvm.resolve(strict=True))
    report = {"passed": False, "checks": []}
    try:
        for backend in ("ros1", "ros2"):
            binary, expected = guest(codec, backend)
            path = (args.out / (backend + ".wasm")).resolve()
            path.write_bytes(binary)
            plan = {"backend": backend, "main_module": "main", "features": [],
                    "preloads": [], "arguments": [], "artifacts": {"main": {"path": str(path)}},
                    "modules": {"main": {"ros_enabled": False, "wasi_grants": ["stdio-full-rights"]}}}
            prefix = [runtime, "--log-verbose", "--runtime-compiler-log", "err", "-Rct", "0",
                      "--runtime-llvm-jit-cache-path", "disable"]
            argv = deployment.command(plan, prefix, {}, os.getuid())
            row = {"backend": backend, "argv": argv}
            report["checks"].append(row)
            result = subprocess.run(argv, stdin=subprocess.DEVNULL, capture_output=True, timeout=60)
            (args.out / (backend + ".stdout")).write_bytes(result.stdout)
            (args.out / (backend + ".stderr")).write_bytes(result.stderr)
            row["exit"] = result.returncode
            row["passed"] = (result.returncode == 0 and result.stdout == expected and
                             b"call_stack=unwind " in result.stderr and b"unwind_check=live" in result.stderr)
            if not row["passed"]:
                raise RuntimeError(f"{backend}: SDK launch or native authority check failed")
        report["passed"] = True
    finally:
        (args.out / "report.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
