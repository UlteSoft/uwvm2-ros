#!/usr/bin/env python3
"""Actual native driver stable WASI table gates; unsigned trusted local DSO.

No mock runtime or host ABI. Caller-memory canaries and a final native witness
also detect early proc_exit(0). Run on the full zero-default product, optionally
with --ros none for a baseline before ROS native-driver composition was fixed.
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
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--runtime-prefix", type=json.loads, default=[], help="JSON argv prefix for the actual VM, e.g. QEMU -cpu/-L")
    parser.add_argument("--cxx", required=True, type=Path)
    parser.add_argument("--cxx-flags-json", type=json.loads, default=[], help="Explicit target/sysroot/linker/ISA flags for the driver DSO")
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--engines", nargs="+", default=["int", "aot"])
    parser.add_argument("--build-only", action="store_true", help="build the real DSO fixture without running any qualification case")
    parser.add_argument("--ros", nargs="+", default=["none", "ros1", "ros2"], choices=["none", "ros1", "ros2"])
    args = parser.parse_args()
    for name, value in (("runtime-prefix", args.runtime_prefix), ("cxx-flags-json", args.cxx_flags_json)):
        if not isinstance(value, list) or not all(isinstance(v, str) for v in value):
            parser.error("--" + name + " must be a JSON array of argument strings")
    source = Path(__file__).resolve().parent
    repo = source.parents[1]
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    spec = importlib.util.spec_from_file_location("wasm", source / "run_permission_composition.py")
    wasm = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(wasm)
    api_rows = [r for r in json.loads((source / "host-interface-signatures.json").read_text()) if r["ns"] == "wasi_snapshot_preview1"]
    assert len(api_rows) == 92 and len({r["name"] for r in api_rows}) == 92
    (out / "driver_denied_calls.inc").write_text("\n".join("CHECK(" + row["name"] + ");" for row in api_rows) + "\n")
    library = out / "gate.so"
    compile_command = [str(args.cxx), *args.cxx_flags_json, "-std=c++26", "-O2", "-shared", "-fPIC", "-DUWVM_ROS_ZERO_HOST_DEFAULTS=1",
                       "-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1", "-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64", "-DUWVM_ENABLE_WASIP1_SOCKET",
                       "-I" + str(repo / "src"), "-I" + str(repo / "third-parties/fast_io/include"),
                       "-I" + str(repo / "third-parties/bizwen/include"), "-I" + str(repo / "third-parties/boost_unordered/include"), "-I" + str(out),
                       str(source / "native_driver_gate_fixture.cc"), "-o", str(library)]
    built = subprocess.run(compile_command, capture_output=True, env=dict(os.environ, TMPDIR=str(out)))
    (out / "compile.log").write_bytes(built.stdout + built.stderr)
    report = {"runtime": str(args.runtime.resolve()), "runtime_sha256": hashlib.sha256(args.runtime.read_bytes()).hexdigest(),
              "runtime_prefix": args.runtime_prefix, "selected_ros": args.ros,
              "compile_command": compile_command, "compile_exit": built.returncode, "cases": [], "complete": False}
    def save(): (out / "report.json").write_text(json.dumps(report, indent=2))
    save()
    assert built.returncode == 0, built.stderr.decode(errors="replace")[-3000:]
    if args.build_only:
        report["fixture_built_only"] = True
        save()
        print(json.dumps({"fixture": str(library), "qualified_cases": 0}))
        return
    canary = 123456789
    def fixture(label, calls):
        imports = []
        body = wasm.store(0, canary) + wasm.store(4, canary) + wasm.store(8, canary) + wasm.store(16, canary)
        for target, function, expected, changed in calls:
            imports.append({"ns": target, "name": function, "params": [], "results": ["i32"]})
            body += wasm.check(b"\x10" + wasm.u(len(imports) - 1), expected)
            if changed == "32":
                body += wasm.check(wasm.const(0) + b"\x28\x02\0", 2) + wasm.check(wasm.const(4) + b"\x28\x02\0", 13)
            elif changed == "64":
                body += wasm.check(wasm.const(8) + b"\x29\x03\0", 2, "i64") + wasm.check(wasm.const(16) + b"\x29\x03\0", 13, "i64")
            else:
                for offset in (0, 4, 8, 16): body += wasm.check(wasm.const(offset) + b"\x28\x02\0", canary)
            body += wasm.store(0, canary) + wasm.store(4, canary) + wasm.store(8, canary) + wasm.store(16, canary)
        imports.append({"ns": calls[-1][0], "name": "complete", "params": [], "results": ["i32"]})
        body += wasm.check(b"\x10" + wasm.u(len(imports) - 1), 0)
        path = out / (label + ".wasm")
        path.write_bytes(wasm.module(imports, body))
        return path
    def target(name, *, expose=True, enabled=True, states=(), resource=True):
        flags = ["--wasip1-single-create", name,
                 "--wasip1-single-expose-host-api" if expose else "--wasip1-single-hide-host-api", name]
        if enabled: flags += ["--wasip1-single-enable", name]
        for function, state in states: flags += ["--wasip1-single-interface", name, function, state]
        if resource: flags += ["--wasip1-single-grant", name, "args"]
        flags += ["--wasip1-single-force-args", name, "2", "first", "second"]
        return flags
    allow = [("args_sizes_get", "allow"), ("args_sizes_get_wasm64", "allow")]
    probes = [
        ("default-table-hidden", [], [("driver", "api_present", 0, None)], ["driver"]),
        ("table-visible-namespace-disabled", target("driver", enabled=False, states=allow),
         [("driver", "api_present", 1, None), ("driver", "all_denied", 0, None)], ["driver"]),
        ("table-visible-interfaces-hidden", target("driver"),
         [("driver", "api_present", 1, None), ("driver", "all_denied", 0, None)], ["driver"]),
        ("table-visible-92-denied", target("driver", states=[(r["name"], "deny") for r in api_rows]),
         [("driver", "api_present", 1, None), ("driver", "all_denied", 0, None)], ["driver"]),
        ("allowed-api-resource-denied", target("driver", states=allow, resource=False),
         [("driver", "args_probe", 76, None), ("driver", "args64_probe", 76, None)], ["driver"]),
        ("allowed-api-and-resource", target("driver", states=allow),
         [("driver", "args_probe", 0, "32"), ("driver", "args64_probe", 0, "64")], ["driver"]),
        ("exit-api-allowed-resource-denied", target("driver", states=allow + [("proc_exit", "allow"), ("proc_exit_wasm64", "allow")]),
         [("driver", "denied_exits", 0, None), ("driver", "args_probe", 0, "32")], ["driver"]),
        ("post-registration-expose", target("driver", states=allow),
         [("driver", "api_present", 1, None), ("driver", "args_probe", 0, "32")], ["driver"]),
        ("post-registration-hide", target("driver", expose=False, states=allow),
         [("driver", "api_present", 0, None)], ["driver"]),
    ]
    for exposure in (False, True):
        states = [(r["name"], "deny") for r in api_rows] if exposure else allow
        flags = target("permitted", states=allow) + target("restricted", expose=exposure, states=states)
        for order in (["permitted", "restricted"], ["restricted", "permitted"]):
            label = "alias-" + ("interface" if exposure else "exposure") + "-" + order[0] + "-first"
            calls = [("permitted", "args_probe", 0, "32"), ("restricted", "all_denied", 0, None),
                     ("permitted", "args64_probe", 0, "64")]
            probes.append((label, flags, calls, order))
            probes.append(("post-registration-" + label, flags, calls, order))
    for engine in args.engines:
        for ros in args.ros:
            for label, policy, calls, order in probes:
                late_policy = label.startswith("post-registration-")
                label = "-".join([label, ros, engine])
                module = fixture(label, calls)
                marker = out / (label + ".marker")
                marker.unlink(missing_ok=True)
                trace = out / (label + ".trace")
                trace.unlink(missing_ok=True)
                flags = ["--runtime-" + engine, "--wasm-set-main-module-name", "main"]
                if engine == "aot": flags += ["--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable"]
                if ros != "none": flags += ["--" + ros + "-single-create", "main", "--" + ros + "-single-enable", "main"]
                if not late_policy: flags += policy
                for name in order: flags += ["--wasm-register-dl", str(library), name]
                if late_policy: flags += policy
                argv = [*args.runtime_prefix, str(args.runtime), *flags, "--run", str(module)]
                log = out / (label + ".stderr")
                with log.open("wb") as errors:
                    value = subprocess.run(argv, stdout=subprocess.PIPE, stderr=errors, timeout=40,
                                           env=dict(os.environ, UWVM_DRIVER_GATE_MARKER=str(marker), UWVM_DRIVER_GATE_TRACE=str(trace)))
                observed = marker.read_text() if marker.exists() else ""
                row = {"name": label, "argv": argv, "exit": value.returncode, "stderr": log.read_text(errors="replace"),
                       "completion_witness": observed, "driver_results": trace.read_text() if trace.exists() else "",
                       "pass": value.returncode == 0 and observed == "1"}
                report["cases"].append(row)
                save()
                assert row["pass"], row
    report["complete"] = True
    save()
    print(json.dumps({"passed": len(report["cases"]), "report": str(out / "report.json")}))


if __name__ == "__main__":
    main()
