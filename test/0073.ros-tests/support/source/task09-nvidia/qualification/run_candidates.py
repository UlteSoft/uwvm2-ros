"""Run task-local source/host contracts under the shared Linux finite owner.

Compile the identical complete CPU layout workload in actual available modes.
No ROS/GPU/model/simulator is substituted when a dependency is absent. Generated
protocol records and WASI compilation remain different evidence layers.
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import time


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--source", type=Path, required=True)
    p.add_argument(
        "--tests",
        type=Path,
        default=Path(__file__).resolve().parents[1],
        help="Canonical task09-nvidia test source root",
    )
    p.add_argument("--header", type=Path, required=True)
    p.add_argument("--schemas", type=Path, required=True)
    p.add_argument("--wasi-sdk", type=Path)
    p.add_argument("--generator-root", type=Path)
    a = p.parse_args()
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("shared finite owner required")
    root = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    out = root / "work" / "qualification"
    out.mkdir()
    rows = []
    failed = False
    source = a.source.resolve(strict=True)
    tests = a.tests.resolve(strict=True)
    pins = {
        str(path.relative_to(source)): hashlib.sha256(path.read_bytes()).hexdigest()
        for path in sorted(source.rglob("*"))
        if path.is_file() and "upstream" not in path.relative_to(source).parts
    }

    def command(name, argv, layer, env=None):
        nonlocal failed
        started = time.monotonic_ns()
        result = subprocess.run(
            [str(v) for v in argv], capture_output=True, timeout=120, env=env
        )
        (out / (name + ".stdout")).write_bytes(result.stdout)
        (out / (name + ".stderr")).write_bytes(result.stderr)
        text = (result.stdout + result.stderr).decode("utf-8", "replace")
        match = re.search(r"Ran (\d+) tests?", text)
        row = {
            "case": name,
            "layer": layer,
            "argv": [str(v) for v in argv],
            "exit_code": result.returncode,
            "status": "PASS" if result.returncode == 0 else "FAIL",
            "elapsed_ns": time.monotonic_ns() - started,
            "unittest_cases": int(match.group(1)) if match else None,
        }
        rows.append(row)
        failed |= result.returncode != 0
        print(json.dumps(row, sort_keys=True), flush=True)
        return result.returncode

    for path in sorted(source.rglob("*.py")):
        if "upstream" in path.relative_to(source).parts:
            continue
        compile(path.read_bytes(), str(path), "exec", dont_inherit=True)
    rows.append(
        {
            "case": "python-syntax",
            "layer": "authored_source_compile",
            "status": "PASS",
            "official_suite_credit": 0,
        }
    )
    command(
        "dependency-inventory",
        [sys.executable, "-B", tests / "qualification/inspect_environment.py"],
        "read_only_dependency_observation",
    )
    for name, script, parents, extras in (
        (
            "sim-graph",
            tests / "isaac_sim/qualification/sensor_graph_contract.py",
            ["isaac_sim"],
            ["--schemas", a.schemas],
        ),
        (
            "jetson-elf",
            tests / "jetson/qualification/package_contract_test.py",
            ["jetson"],
            [],
        ),
        (
            "groot-staging",
            tests / "groot/tests/staged_video_contract.py",
            ["groot"],
            [],
        ),
    ):
        if name == "groot-staging" and importlib.util.find_spec("numpy") is None:
            rows.append(
                {
                    "case": name,
                    "layer": "authored_host_protocol",
                    "status": "NOT_RUN",
                    "reason": "actual NumPy missing",
                }
            )
            continue
        env = os.environ.copy()
        env["PYTHONPATH"] = os.pathsep.join(str(source / parent) for parent in parents)
        command(
            name,
            [sys.executable, "-B", script, *extras, "-v"],
            "authored_host_protocol",
            env,
        )

    # Execute the exact source-captured native entry in denied mode. Missing
    # model/SDK paths make an accidental pre-admission read an observable fault.
    denied = [
        sys.executable,
        "-B",
        source / "groot/run_captured.py",
        "--sdk-root",
        "/absent",
        "--sdk-source-inventory",
        "/absent",
        "--model-root",
        "/absent",
        "--embodiment",
        "absent",
        "--device",
        "cuda:0",
        "--service",
        "/infer",
        "--domain",
        "231",
        "--lifetime-ms",
        "1",
        "--call-ms",
        "1",
        "--maximum-requests",
        "1",
    ]
    result = subprocess.run([str(v) for v in denied], capture_output=True, timeout=10)
    (out / "groot-denied.stdout").write_bytes(result.stdout)
    (out / "groot-denied.stderr").write_bytes(result.stderr)
    okay = result.returncode == 13
    rows.append(
        {
            "case": "groot-source-bound-default-denial",
            "layer": "native_entry_denial",
            "status": "PASS" if okay else "FAIL",
            "exit_code": result.returncode,
            "model_calls": 0,
        }
    )
    failed |= not okay
    denials = [
        (
            "sim-default-denial",
            [
                source / "isaac_sim/native_graph.py",
                "--sdk-root",
                "/absent",
                "--usd",
                "/absent",
                "--articulation",
                "/Robot",
                "--domain",
                "231",
                "--state-topic",
                "/state",
                "--command-topic",
                "/command",
                "--state-qos-json",
                "{}",
                "--command-qos-json",
                "{}",
                "--frames",
                "1",
                "--lifetime-ms",
                "1",
            ],
        ),
        (
            "jetson-default-denial",
            [
                source / "jetson/admit_platform.py",
                "--model-file",
                "/absent",
                "--model-sha256",
                "0" * 64,
                "--release-file",
                "/absent",
                "--release-sha256",
                "0" * 64,
                "--carrier-label",
                "absent",
                "--module-label",
                "absent",
                "--jetpack-label",
                "absent",
                "--cuda-label",
                "absent",
            ],
        ),
    ]
    for name, arguments in denials:
        result = subprocess.run(
            [sys.executable, "-B", *[str(v) for v in arguments]],
            capture_output=True,
            timeout=10,
        )
        (out / (name + ".stdout")).write_bytes(result.stdout)
        (out / (name + ".stderr")).write_bytes(result.stderr)
        okay = result.returncode == 13
        rows.append(
            {
                "case": name,
                "layer": "native_entry_denial",
                "status": "PASS" if okay else "FAIL",
                "exit_code": result.returncode,
            }
        )
        failed |= not okay

    test = tests / "isaac_ros/qualification/cpu_sensor_contract.c"
    c = shutil.which("clang") or shutil.which("clang-21") or shutil.which("gcc")
    cpp = shutil.which("clang++") or shutil.which("clang++-21") or shutil.which("g++")
    for language, compiler, modes in (
        ("c", c, ["c99", "c11", "c17", "c23"]),
        (
            "cpp",
            cpp,
            ["c++98", "c++03", "c++11", "c++14", "c++17", "c++20", "c++23", "c++26"],
        ),
    ):
        if compiler:
            command(language + "-version", [compiler, "--version"], "toolchain_version")
        for mode in modes:
            name = language + "-" + mode.replace("+", "p")
            if compiler is None:
                rows.append(
                    {
                        "case": name,
                        "layer": "native_cpu_layout",
                        "status": "NOT_RUN",
                        "reason": "compiler missing",
                    }
                )
                continue
            executable = out / name
            if (
                command(
                    name + "-build",
                    [
                        compiler,
                        "-x",
                        "c" if language == "c" else "c++",
                        "-std=" + mode,
                        "-O2",
                        "-Wall",
                        "-Wextra",
                        "-Werror",
                        "-pedantic-errors",
                        "-I",
                        a.header.parent,
                        test,
                        "-o",
                        executable,
                    ],
                    "native_compile",
                )
                == 0
            ):
                command(name + "-run", [executable], "authored_native_cpu_layout")
    if a.wasi_sdk:
        compiler = a.wasi_sdk / "bin/clang"
        artifact = out / "cpu-layout.wasm"
        if (
            command(
                "wasi-c99-build",
                [
                    compiler,
                    "--sysroot=" + str(a.wasi_sdk / "share/wasi-sysroot"),
                    "-std=c99",
                    "-O2",
                    "-Wall",
                    "-Wextra",
                    "-Werror",
                    "-pedantic-errors",
                    "-I",
                    a.header.parent,
                    test,
                    "-o",
                    artifact,
                ],
                "wasm_compile",
            )
            == 0
        ):
            rows.append(
                {
                    "case": "wasi-artifact",
                    "layer": "wasm_artifact",
                    "status": "PRODUCED_NOT_EXECUTED",
                    "bytes": artifact.stat().st_size,
                    "sha256": hashlib.sha256(artifact.read_bytes()).hexdigest(),
                }
            )
    if a.generator_root:
        generator = a.generator_root.resolve(strict=True)
        roles = {
            "parent": generator / "sdk/origin/tools/custom_types.py",
            "bounded": generator / "sdk/origin/custom-types/bounded_profile.py",
            "adapter": generator / "sdk/origin/robots/industrial/vendor_profile.py",
        }
        argv = [
            sys.executable,
            "-B",
            generator / "sdk/origin/robots/industrial/prepare.py",
            "--mode",
            "prepare",
            "--source-root",
            generator,
            "--catalog",
            source / "groot/source-definitions/ros2-staged/catalog.preparation.json",
            "--out",
            out / "groot-staged-generated",
        ]
        for role, path in roles.items():
            argv += [
                "--" + role + "-sha256",
                hashlib.sha256(path.read_bytes()).hexdigest(),
            ]
        command(
            "groot-staged-source-prepare", argv, "generated_source_not_native_binding"
        )
    summary = {
        "format": "uwvm-nvidia-task-local-qualification-v1",
        "cases": rows,
        "authored_source_sha256": pins,
        "standalone_header_sha256": hashlib.sha256(a.header.read_bytes()).hexdigest(),
        "native_ros_exchanges": 0,
        "actual_gpu_inferences": 0,
        "simulator_started_cases": 0,
        "wasm_guest_executions": 0,
        "gateway_integrations": 0,
        "official_upstream_suites": 0,
        "qemu_cases": 0,
        "vtune_hardware_cases": 0,
        "default_authority": [],
        "complete_platform_support": False,
        "status": "FAIL" if failed else "SOURCE_AND_AVAILABLE_HOST_CONTRACTS_ONLY",
    }
    (out / "summary.json").write_text(
        json.dumps(summary, indent=2, sort_keys=True) + "\n"
    )
    print(
        json.dumps(
            {
                "status": summary["status"],
                "rows": len(rows),
                "actual_gpu_inferences": 0,
                "wasm_guest_executions": 0,
                "official_upstream_suites": 0,
            }
        ),
        flush=True,
    )
    return 1 if failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
