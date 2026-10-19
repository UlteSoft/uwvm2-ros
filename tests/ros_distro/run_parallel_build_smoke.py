#!/usr/bin/env python3
"""Validate the real memory-planned CMake build method in an admitted cgroup.

This small integration test compiles four independent generated C translation
units and one main, observes concurrent compiler processes in the same cgroup,
and runs the linked checksum program. It exercises Preparation.build_target
with real process ownership. It does not construct a ROS distribution profile,
qualify a ROS SDK, measure performance, or grant any Guest capability.

Pass a fresh output, an explicit current-cgroup configuration, and actual CMake
and C compiler paths. Run from a byte-pinned source package in the original
64-GiB/swap-zero leaf. All generated files stay below the selected output.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import sys
import time

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/ros_distro"))
import common
import probe

MIB = 1024**2
GIB = 1024**3


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ("out", "cgroup-config", "cmake", "cc"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    config, config_pin = common.document(args.cgroup_config.resolve(strict=True))
    before = common.cgroup_state(config)
    out = args.out.resolve()
    common.require(not out.exists(), "fresh smoke output required")
    out.mkdir(mode=0o700)
    source, build = out / "source", out / "build"
    source.mkdir(mode=0o700)
    tools = {"cmake": args.cmake.resolve(strict=True), "cc": args.cc.resolve(strict=True)}
    tool_pins = {name: common.opened(path) for name, path in tools.items()}
    iterations = 12000
    expected = []
    files = []
    for index in range(4):
        value = index + 1
        for _ in range(iterations):
            value = (value * 1664525 + 1013904223) & 0xffffffff
        expected.append(value)
        path = source / ("unit%d.c" % index)
        path.write_text("_Static_assert(sizeof(unsigned)==4, \"uint32 required\");\n"
                        "unsigned f%d(unsigned x) {\n" % index +
                        "x = x * 1664525u + 1013904223u;\n" * iterations + "return x; }\n")
        files.append(path)
    main_source = source / "main.c"
    main_source.write_text("\n".join("unsigned f%d(unsigned);" % i for i in range(4)) +
                           "\nint main(void) { return " + " || ".join(
                               "f%d(%du) != %du" % (i, i + 1, value)
                               for i, value in enumerate(expected)) + "; }\n")
    files.append(main_source)
    cmake_source = source / "CMakeLists.txt"
    cmake_source.write_text("cmake_minimum_required(VERSION 3.20)\n"
                            "project(uwvm_parallel_smoke C)\n"
                            "add_executable(parallel-smoke " +
                            " ".join(path.name for path in files) + ")\n")
    pins = [common.opened(path) for path in [*files, cmake_source]]
    source_root = Path(__file__).resolve().parents[2]
    implementation_pins = [common.opened(source_root / relative) for relative in (
        "tools/ros_distro/common.py", "tools/ros_distro/profile.py",
        "tools/ros_distro/probe.py", "tools/ros_distro/build_parallelism.py",
        "tests/ros_cgroup/probe.py", "tests/ros_distro/run_parallel_build_smoke.py")]
    owned_sources = {os.fsencode(str(path)) for path in files[:4]}
    samples = []
    started = time.monotonic()
    membership = Path("/proc/self/cgroup").read_text()
    leaf = Path(config["expected_leaf"])

    def guard():
        common.require(time.monotonic() - started < 300, "finite smoke deadline")
        common.cgroup_preserved(before, common.cgroup_state(config))
        fs = os.statvfs(out)
        common.require(fs.f_bavail * fs.f_frsize >= 8 * GIB, "eight-GiB disk reserve")
        running = []
        for spelling in (leaf / "cgroup.procs").read_text().split():
            directory = Path("/proc") / spelling
            try:
                executable = (directory / "exe").resolve(strict=True)
                if executable.name not in ("cc1", "clang", "clang-23"):
                    continue
                with (directory / "cmdline").open("rb") as stream:
                    words = stream.read(65537).split(b"\0")
                if owned_sources.intersection(words):
                    common.require((directory / "cgroup").read_text() == membership,
                                   "actual compiler left admitted cgroup")
                    running.append(int(spelling))
            except (FileNotFoundError, ProcessLookupError):
                continue
        if running:
            samples.append({"elapsed_seconds": time.monotonic() - started,
                            "owned_compiler_pids": running})

    limits = {"jobs": 4, "rss_bytes": 512 * MIB,
              "command_seconds": 120, "log_bytes": 8 * MIB}
    runner = common.SerialRunner(out, limits, guard)
    # Only the build-method adapter is under test. A real ROS profile/SDK
    # admission is intentionally not fabricated for this generated C project.
    prep = probe.Preparation.__new__(probe.Preparation)
    prep.out, prep.before, prep.runner = out, before, runner
    prep.profile = {"runtime": config, "limits": limits,
                    "tools": {"cmake": str(tools["cmake"])}}
    prep.environment = {"PATH": "/usr/bin:/bin", "LANG": "C", "LC_ALL": "C",
                        "HOME": str(out), "TMPDIR": str(out)}
    prep.build_environment_selection = {"UWVM_ROS_BUILD_MAX_JOBS": "4",
                                       "UWVM_ROS_BUILD_JOB_RSS_BYTES": str(128 * MIB)}
    prep.state = {"kind": "real-cmake-memory-parallel-build-method-smoke", "status": "RUNNING"}
    error = None
    try:
        runner.run("configure-generated-parallel-project",
                   [str(tools["cmake"]), "-S", str(source), "-B", str(build),
                    "-G", "Unix Makefiles", "-DCMAKE_C_COMPILER=" + str(tools["cc"]),
                    "-DCMAKE_C_FLAGS=-O0 -g0", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"],
                   out, prep.environment)
        prep.build_target("actual-memory-planned-four-job-build", build, "parallel-smoke")
        common.require(prep.state["build_parallelism"][-1]["jobs"] == 4,
                       "expected four jobs within the selected 512-MiB aggregate budget")
        common.require(max((len(row["owned_compiler_pids"]) for row in samples), default=0) >= 2,
                       "actual compiler overlap must be observed, not inferred from -j")
        database = json.loads((build / "compile_commands.json").read_bytes())
        common.require(len(database) == 5 and {row["file"] for row in database} ==
                       {str(path) for path in files}, "five actual translation units required")
        runner.run("execute-linked-checksum", [str(build / "parallel-smoke")], out, prep.environment)
    except BaseException as caught:
        error = repr(caught)
    finally:
        runner.close()
        guard()
        preserved = all(common.opened(Path(pin["path"])) == pin for pin in [*pins, *implementation_pins])
        preserved = preserved and all(common.opened(Path(pin["path"])) == pin for pin in tool_pins.values())
        preserved = preserved and common.opened(Path(config_pin["path"])) == config_pin
        peak = max((len(row["owned_compiler_pids"]) for row in samples), default=0)
        prep.state.update(status="PASS_ACTUAL_PLANNED_PARALLEL_CMAKE_SMOKE_ONLY" if
                          error is None and preserved else "FAIL", error=error,
                          scope="Build-method/containment test only; no ROS, robot or performance qualification",
                          cgroup_before=before, cgroup_after=common.cgroup_state(config),
                          configuration=config_pin, source_pins=pins, implementation_pins=implementation_pins, tool_pins=tool_pins,
                          inputs_preserved=preserved, observed_peak_target_compilers=peak,
                          compiler_overlap_samples=samples, commands=runner.commands, cleanup=runner.cleanup)
        common.atomic_json(out / "report.json", prep.state)
    print(json.dumps({"status": prep.state["status"], "report": common.opened(out / "report.json"),
                      "selected_jobs": prep.state.get("build_parallelism", [{}])[-1].get("jobs"),
                      "observed_peak_target_compilers": peak, "error": error,
                      "cleanup": runner.cleanup}), flush=True)
    return 0 if prep.state["status"].startswith("PASS_") else 1


if __name__ == "__main__":
    raise SystemExit(main())
