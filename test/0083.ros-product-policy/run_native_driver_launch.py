#!/usr/bin/env python3
"""Actual target-architecture driver launch admission, including QEMU profiles.

This does not claim CMS cryptography or sealed dlopen qualification. Those are
separate in run_trusted_driver.py. No target OpenSSL toolchain is required here.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import os
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--runtime-prefix", type=json.loads, default=[], help="JSON argv prefix for the actual VM, e.g. QEMU -cpu/-L")
    parser.add_argument("--cxx", type=Path, required=True)
    parser.add_argument("--cxx-flags-json", type=json.loads, default=[], help="Explicit target/sysroot/linker/ISA flags for the driver DSO")
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--engines", nargs="+", choices=["int", "aot"], default=["int"])
    parser.add_argument("--expect-verifier-unavailable", action="store_true",
                        help="Require the actual product to diagnose its absent OpenSSL backend; never mark it as verified")
    parser.add_argument("--ros-execution-unsupported", action="store_true",
                        help="Negative oracle only for a product known to reject ROS execution; do not infer this from INT selection")
    parser.add_argument("--ros-api-probe", action="store_true",
                        help="Actually call ROS1/ROS2 abi_version with deny and allow, requiring 13 and 1 respectively")
    args = parser.parse_args()
    if args.ros_execution_unsupported and args.ros_api_probe:
        parser.error("select either the unsupported-backend oracle or actual ROS API probes")
    for name, value in (("runtime-prefix", args.runtime_prefix), ("cxx-flags-json", args.cxx_flags_json)):
        if not isinstance(value, list) or not all(isinstance(v, str) for v in value):
            parser.error("--" + name + " must be a JSON array of argument strings")
    source = Path(__file__).resolve().parent
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    spec = importlib.util.spec_from_file_location("driver_fixture", source / "run_trusted_driver.py")
    fixture = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(fixture)
    library = out / "driver.so"
    compile_command = [str(args.cxx), *args.cxx_flags_json, "-x", "c", "-std=c17", "-D_GNU_SOURCE", "-O2", "-fPIC", "-shared",
                       str(source / "trusted_driver_fixture.c"), "-o", str(library)]
    with (out / "compile.log").open("wb") as log:
        compiled = subprocess.run(compile_command, stdout=log, stderr=subprocess.STDOUT,
                                  env=dict(os.environ, TMPDIR=str(out)), timeout=120)
    report = {"scope": "actual target driver launch only; CMS cryptography and sealed loads unqualified",
              "runtime": str(args.runtime.resolve()), "runtime_sha256": hashlib.sha256(args.runtime.read_bytes()).hexdigest(),
              "runtime_prefix": args.runtime_prefix, "compile_command": compile_command, "compile_exit": compiled.returncode,
              "expect_verifier_unavailable": args.expect_verifier_unavailable,
              "ros_execution_unsupported": args.ros_execution_unsupported, "ros_api_probe": args.ros_api_probe,
              "cases": [], "complete": False}
    def save(): (out / "report.json").write_text(json.dumps(report, indent=2))
    save()
    assert compiled.returncode == 0, (out / "compile.log").read_text(errors="replace")[-4000:]
    report["library_sha256"] = hashlib.sha256(library.read_bytes()).hexdigest()
    guest, api_guest = out / "driver.wasm", out / "driver-api.wasm"
    guest.write_bytes(fixture.guest())
    api_guest.write_bytes(fixture.guest(api=1))
    certificate = out / "untrusted.pem"
    certificate.write_text("intentionally not a trusted certificate\n")
    load = ["--wasm-register-dl", library, "driver"]
    refusal = "OpenSSL driver verifier unavailable" if args.expect_verifier_unavailable else "driver verification"
    probes = [
        ("no-implicit-driver", ["--run", guest], False, "", "Missing module dependency"),
        ("default-off-api-hidden", [*load, "--run", guest], True, "A", ""),
        ("warning-as-error-all", ["--warning-as-error", "all", *load, "--run", guest], False, "", "Convert warnings to fatal errors"),
        ("warning-as-error-untrusted-dl", ["--warning-as-error", "untrusted-dl", *load, "--run", guest], False, "", "Convert warnings to fatal errors"),
        ("explicit-api-exposure", ["--wasip1-single-create", "driver", "--wasip1-single-expose-host-api", "driver",
                                  *load, "--run", api_guest], True, "A", ""),
        ("guest-tail-is-not-host-policy", [*load, "--run", guest, "--wasm-driver-verify", certificate], True, "A", ""),
        ("verification-before-constructor", ["--wasm-driver-verify", certificate, *load, "--run", guest], False, "", refusal),
        ("late-verification-before-constructor", [*load, "--wasm-driver-verify", certificate, "--run", guest], False, "", refusal),
        ("default-wsea-off", ["--secure", "status"], True, "", "WSEA: disabled"),
        ("selected-wsea-refusal-before-constructor", [*load, "--secure", "module", guest, out / "missing.policy", "--run", guest],
         False, "", "DENIED"),
    ]
    if args.ros_execution_unsupported:
        for ros in ("ros1", "ros2"):
            probes.append((ros + "-execution-backend-refused",
                           ["--" + ros + "-single-create", "main", "--" + ros + "-single-enable", "main", *load, "--run", guest],
                           False, "A", "ROS requires LLVM full compilation"))
    if args.ros_api_probe:
        spec = importlib.util.spec_from_file_location("permission_fixture", source / "run_permission_composition.py")
        wasm = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(wasm)
        for ros in ("ros1", "ros2"):
            namespace = "uwvm_" + ros + "_v1"
            for state, expected in (("deny", 13), ("allow", 1)):
                label = ros + "-actual-abi-" + state
                module = out / (label + ".wasm")
                imports = [{"ns": "driver", "name": "value", "params": [], "results": ["i32"]},
                           {"ns": namespace, "name": "abi_version", "params": [], "results": ["i32"]}]
                module.write_bytes(wasm.module(imports, wasm.check(b"\x10\0", 42) + wasm.check(b"\x10\x01", expected)))
                flags = ["--" + ros + "-single-create", "main", "--" + ros + "-single-enable", "main",
                         "--" + ros + "-single-interface", "main", namespace + ".abi_version", state, *load, "--run", module]
                probes.append((label, flags, True, "A", ros + "." + namespace + ".abi_version denied" if state == "deny" else ""))
    for engine in args.engines:
        engine_flags = ["--runtime-" + engine]
        if engine == "aot": engine_flags += ["--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable"]
        for label, flags, success, constructor, diagnostic in probes:
            label += "-" + engine
            witness, log, output = [out / (label + suffix) for suffix in (".marker", ".stderr", ".stdout")]
            witness.unlink(missing_ok=True)
            command = list(map(str, [*args.runtime_prefix, args.runtime, *engine_flags,
                                    "--wasm-set-main-module-name", "main", *flags]))
            with output.open("wb") as stdout, log.open("wb") as stderr:
                result = subprocess.run(command, stdout=stdout, stderr=stderr, timeout=60,
                                        env=dict(os.environ, UWVM_DRIVER_MARKER=str(witness)))
            observed = witness.read_text() if witness.exists() else ""
            text = log.read_text(errors="replace")
            row = {"name": label, "command": command, "exit": result.returncode, "stderr": text,
                   "constructor_marker": observed, "pass": (result.returncode == 0) == success
                   and observed == constructor and diagnostic in text
                   and (not label.startswith("warning-as-error-") or result.returncode == 126)}
            report["cases"].append(row)
            save()
            assert row["pass"], row
    report["complete"] = True
    save()
    print(json.dumps({"passed": len(report["cases"]), "report": str(out / "report.json")}))


if __name__ == "__main__":
    main()
