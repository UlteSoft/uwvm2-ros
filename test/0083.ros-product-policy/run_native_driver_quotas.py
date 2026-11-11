#!/usr/bin/env python3
"""Real stable native WASI operation waits, exact effects and wasm32/64 ABI.

Use the gate.so built by run_native_driver_permissions.py. The native callback
performs two host operations and writes a separate BE witness exactly once.
No retry may repeat the whole driver, nor emit bytes before quota admission.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import os
import subprocess
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--runtime-prefix", type=json.loads, default=[], help="JSON argv prefix for the actual VM, e.g. QEMU -cpu/-L")
    parser.add_argument("--library", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--engines", nargs="+", default=["int", "aot"], choices=["int", "aot"])
    parser.add_argument("--ros", nargs="+", default=["none", "ros1", "ros2"], choices=["none", "ros1", "ros2"])
    args = parser.parse_args()
    if not isinstance(args.runtime_prefix, list) or not all(isinstance(v, str) for v in args.runtime_prefix):
        parser.error("--runtime-prefix must be a JSON array of argument strings")
    source = Path(__file__).resolve().parent
    spec = importlib.util.spec_from_file_location("wasm", source / "run_permission_composition.py")
    wasm = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(wasm)
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    report = {"runtime": str(args.runtime.resolve()), "runtime_sha256": hashlib.sha256(args.runtime.read_bytes()).hexdigest(),
              "runtime_prefix": args.runtime_prefix, "selected_ros": args.ros,
              "library": str(args.library.resolve()), "library_sha256": hashlib.sha256(args.library.read_bytes()).hexdigest(),
              "cases": [], "complete": False}
    def save(): (out / "report.json").write_text(json.dumps(report, indent=2))
    save()
    sentinel = 123456789
    # first and second provider errno, independent output counts, expected bytes
    states = [
        ("unlimited", None, None, None, (0, 0), (4, 4), b"safesafe"),
        ("request-reject", "request-bytes", "1", "reject", (1, 1), (sentinel, sentinel), b""),
        ("window-reject", "window-calls", "1", "reject", (0, 6), (4, sentinel), b"safe"),
        ("window-wait", "window-calls", "1", "wait", (0, 0), (4, 4), b"safesafe"),
        ("zero-window-wait", "window-calls", "0", "wait", (6, 6), (sentinel, sentinel), b""),
        ("total-reject", "total-calls", "1", "reject", (0, 6), (4, sentinel), b"safe"),
        ("total-wait-policy-rejected", "total-calls", "1", "wait", (0, 6), (4, sentinel), b""),
        ("interface-denied", None, None, None, (76, 76), (sentinel, sentinel), b""),
        ("resource-denied", None, None, None, (76, 76), (sentinel, sentinel), b""),
    ]
    for engine in args.engines:
        for ros in args.ros:
            for wide in (False, True):
                for state, field, amount, action, errors, counts, expected_bytes in states:
                    label = f"{state}-{'wide' if wide else 'narrow'}-{ros}-{engine}"
                    function = "write_pair64" if wide else "write_pair"
                    imports = [{"ns": "driver", "name": function, "params": [], "results": ["i32"]},
                               {"ns": "driver", "name": "complete", "params": [], "results": ["i32"]}]
                    def address(value): return wasm.const(value, "i64" if wide else "i32")
                    def store32(at, value): return address(at) + wasm.const(value) + b"\x36\x02\0"
                    body = store32(64, 160) + store32(68, 4) + store32(160, int.from_bytes(b"safe", "little"))
                    # Wide cases use an actual memory64 module and ciovec64;
                    # narrow cases use memory32. The driver table ABI is real.
                    for offset, value in ((96, 160), (104, 4), (112, sentinel), (120, sentinel)):
                        body += address(offset) + wasm.const(value, "i64") + b"\x37\x03\0"
                    for offset in (0, 4, 8, 16, 80, 84): body += store32(offset, sentinel)
                    body += wasm.check(b"\x10\0", errors[0] * 256 + errors[1])
                    for offset, count in zip((112, 120) if wide else (80, 84), counts):
                        body += wasm.check(address(offset) + (b"\x29\x03\0" if wide else b"\x28\x02\0"),
                                           count, "i64" if wide else "i32")
                    for offset in (0, 4, 8, 16): body += wasm.check(address(offset) + b"\x28\x02\0", sentinel)
                    body += wasm.check(b"\x10\x01", 0)
                    module = out / (label + ".wasm")
                    module.write_bytes(wasm.module(imports, body, memory64=wide))
                    flags = ["--runtime-" + engine, "--wasm-set-main-module-name", "main"]
                    if engine == "aot": flags += ["--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable"]
                    if wide: flags += ["--wasm-feature-enable-memory64"]
                    if ros != "none": flags += ["--" + ros + "-single-create", "main", "--" + ros + "-single-enable", "main"]
                    flags += ["--wasip1-single-create", "driver", "--wasip1-single-enable", "driver",
                              "--wasip1-single-expose-host-api", "driver", "--wasip1-single-interface", "driver",
                              "fd_write_wasm64" if wide else "fd_write", "deny" if state == "interface-denied" else "allow"]
                    if state != "resource-denied": flags += ["--wasip1-single-grant", "driver", "stdout.write"]
                    if field:
                        flags += ["--wasip1-single-limit", "driver", "stdout." + field, amount,
                                  "--wasip1-single-limit-action", "driver", "stdout." + field, action,
                                  "--enable-warning", "all_host_limited_wasip1"]
                    flags += ["--wasm-register-dl", str(args.library.resolve()), "driver", "--run", str(module)]
                    argv = [*args.runtime_prefix, str(args.runtime.resolve()), *flags]
                    marker, trace, effect = [out / (label + suffix) for suffix in (".marker", ".trace", ".effect")]
                    for path in (marker, trace, effect): path.unlink(missing_ok=True)
                    output, log = out / (label + ".stdout"), out / (label + ".stderr")
                    start = time.monotonic()
                    with output.open("wb") as stdout, log.open("wb") as stderr:
                        value = subprocess.run(argv, stdout=stdout, stderr=stderr, timeout=40,
                            env=dict(os.environ, UWVM_DRIVER_GATE_MARKER=str(marker), UWVM_DRIVER_GATE_TRACE=str(trace),
                                     UWVM_DRIVER_EFFECT_MARKER=str(effect)))
                    elapsed = time.monotonic() - start
                    diagnostic = log.read_text(errors="replace")
                    observed = {"completion": marker.read_text() if marker.exists() else "",
                                "driver_effect": effect.read_text() if effect.exists() else "",
                                "driver_results": trace.read_text() if trace.exists() else ""}
                    passed = (value.returncode == 0 and observed["completion"] == "1" and observed["driver_effect"] == "BE"
                              and output.read_bytes() == expected_bytes)
                    if state == "window-wait":
                        passed &= elapsed >= 0.5 and "synchronously waiting for quota stdout.window-calls" in diagnostic
                    if state == "total-wait-policy-rejected":
                        passed = (value.returncode != 0 and observed["completion"] == "" and observed["driver_effect"] == ""
                                  and output.read_bytes() == b""
                                  and "request and lifetime ceilings only support reject" in diagnostic)
                    row = {"name": label, "argv": argv, "exit": value.returncode, "seconds": elapsed,
                           "stderr": diagnostic, "stdout_hex": output.read_bytes().hex(), **observed, "pass": passed}
                    report["cases"].append(row)
                    save()
                    assert passed, row
    report["complete"] = True
    save()
    print(json.dumps({"passed": len(report["cases"]), "report": str(out / "report.json")}))


if __name__ == "__main__":
    main()
