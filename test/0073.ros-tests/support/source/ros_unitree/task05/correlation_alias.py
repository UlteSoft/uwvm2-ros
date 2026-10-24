#!/usr/bin/env python3
"""Compile and run actual C/C++ correlation contracts on native and Wasm.

This validates a private installed header subset, not a complete SDK install,
official program, xmake example, native ROS peer or robot operation.
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess


def fact(path):
    path = Path(path).resolve(strict=True)
    with path.open("rb") as stream:
        checksum = hashlib.file_digest(stream, "sha256").hexdigest()
    return {"path": str(path), "bytes": path.stat().st_size, "sha256": checksum}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("formatted", "cpp-test", "baseline-header", "wasi", "runtime", "validator", "checker"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--case", choices=("alias", "lifetime-c", "lifetime-cpp"),
        help="Explicit single-case small-scratch run; omit for the three-case matrix")
    args = parser.parse_args()
    run = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    assert os.environ["UWVM_RESOURCE_HEAVY_LOCK_HELD"] == "1"
    out = run / "work/correlation"
    out.mkdir()
    report = {"status": "RUNNING", "commands": [], "inputs": [], "products": [],
              "native_value_runs": 0, "actual_Wasm_Guests": 0, "Wasm_validations": 0,
              "ROS_imports_checked": 0, "native_ROS_runs": 0, "official_program_ports": 0,
              "public_xmake_example_builds": 0, "full_public_SDK_qualified": False,
              "default_authority": []}
    pins = {}

    def pin(path):
        path = Path(path).resolve(strict=True)
        if path not in pins:
            pins[path] = fact(path)
            report["inputs"].append(pins[path])
        return path

    def save():
        (out / "result.json").write_text(json.dumps(report, indent=2) + "\n")

    def command(label, argv, expected=0):
        row = {"label": label, "argv": list(map(str, argv))}
        report["commands"].append(row)
        child = subprocess.run(row["argv"], cwd=out, stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=90)
        row["exit"] = child.returncode
        (out / (label + ".stdout")).write_bytes(child.stdout)
        (out / (label + ".stderr")).write_bytes(child.stderr)
        save()
        assert child.returncode == expected, (label, child.returncode, child.stderr[-3000:])

    try:
        pin(__file__)
        formatted = args.formatted.resolve(strict=True)
        sdk = out / "sdk"
        prefix = Path("sdk/origin/robots/unitree/include/roswasm_unitree")
        for name, language in (("topic_rpc.h", "c"), ("topic_rpc.hpp", "cpp")):
            target = sdk / "include/platform/unitree" / language / "roswasm_unitree" / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(pin(formatted / prefix / name), target)
        baseline = out / "baseline/include/platform/unitree/c/roswasm_unitree/topic_rpc.h"
        baseline.parent.mkdir(parents=True)
        shutil.copyfile(pin(args.baseline_header), baseline)
        tests = formatted / "test/0073.ros-tests/support/source/ros_platform05/unitree"
        cases = [("alias", pin(tests / "topic_rpc_alias_contract.c"), False),
                 ("lifetime-c", pin(tests / "topic_rpc_lifetime_contract.c"), False),
                 ("lifetime-cpp", pin(args.cpp_test), True)]
        baseline_source = cases[0][1]
        if args.case:
            cases = [row for row in cases if row[0] == args.case]
        report["selected_cases"] = [row[0] for row in cases]
        for tool in (Path("/usr/bin/clang-21"), Path("/usr/bin/clang++-21"),
                     args.wasi / "bin/clang", args.wasi / "bin/clang++",
                     args.wasi / "bin/wasm-ld", args.runtime, args.validator, args.checker):
            pin(tool)
        command("native-version", ["/usr/bin/clang-21", "--version"])
        command("WASI-version", [args.wasi / "bin/clang", "--version"])
        command("validator-version", [args.validator, "--version"])
        for directory in (args.wasi / "share/wasi-sysroot/lib", args.wasi / "lib/clang"):
            for path in sorted(directory.rglob("*")):
                if path.is_file() and path.suffix in (".a", ".o"):
                    pin(path)
        old = out / "original-alias"
        command("build-original-regression", ["/usr/bin/clang-21", "-std=c11", "-O1", "-g0",
            "-I" + str(out / "baseline/include"), baseline_source, "-o", old])
        # The exact original function returns0 for the live-slot output alias;
        # the independent case therefore stops at its specific return2 gate.
        command("original-regression-rejected", [old], 2)
        report["original_expected_failures"] = 1
        spec = importlib.util.spec_from_file_location("actual_import_checker", args.checker)
        checker = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(checker)
        for kind in ("native", "wasm"):
            for name, source, cpp in cases:
                product = out / (kind + "-" + name + (".wasm" if kind == "wasm" else ""))
                dep = product.with_suffix(".d")
                compiler = (Path("/usr/bin/clang++-21" if cpp else "/usr/bin/clang-21")
                            if kind == "native" else args.wasi / ("bin/clang++" if cpp else "bin/clang"))
                argv = [compiler, "-std=c++20" if cpp else "-std=c11", "-O1", "-g0", "-Wall", "-Wextra", "-Werror",
                        "-I" + str(sdk / "include"), "-MD", "-MF", dep]
                if cpp:
                    argv += ["-fno-exceptions", "-fno-rtti"]
                if kind == "wasm":
                    argv += ["--target=wasm32-wasip1", "--sysroot=" + str(args.wasi / "share/wasi-sysroot"),
                        "-Wl,--threads=1,-z,stack-size=1048576,--initial-memory=16777216,--max-memory=67108864"]
                command("build-" + kind + "-" + name, [*argv, source, "-o", product])
                for dependency in shlex.split(dep.read_text().replace("\\\n", " ").partition(":")[2]):
                    dependency = Path(dependency).resolve(strict=True)
                    if not dependency.is_relative_to(run):
                        pin(dependency)
                report["products"].append({"kind": kind, "case": name, "artifact": fact(product)})
                if kind == "native":
                    command("run-" + kind + "-" + name, [product])
                    report["native_value_runs"] += 1
                else:
                    command("validate-" + name, [args.validator, "validate", product])
                    imports = checker.inspect(product.read_bytes())
                    assert all(row["module"] == "wasi_snapshot_preview1" and row["kind"] == 0 for row in imports)
                    (out / (name + "-imports.json")).write_text(json.dumps(imports, indent=2) + "\n")
                    command("run-" + kind + "-" + name, [args.runtime, "-Rint",
                        "--wasip1-global-noinherit-system-environment", "--run", product])
                    report["Wasm_validations"] += 1
                    report["actual_Wasm_Guests"] += 1
        assert all(fact(path) == row for path, row in pins.items())
        report.update(status="PASS_LOCAL_CORRELATION_ONLY", input_pins_preserved=True)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        save()


if __name__ == "__main__":
    main()
