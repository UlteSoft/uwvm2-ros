#!/usr/bin/env python3
"""Check owned Python syntax and actual local CMake OFF/migration gates.

Only the shared finite owner may execute this source capsule. Compiler probes
inside CMake configuration do not count as SDK application builds or Guest runs.
"""

import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import subprocess


def fact(path):
    with path.open("rb") as stream:
        checksum = hashlib.file_digest(stream, "sha256").hexdigest()
    return {"bytes": path.stat().st_size, "sha256": checksum}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    args = parser.parse_args()
    run = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    assert os.environ["UWVM_RESOURCE_HEAVY_LOCK_HELD"] == "1"
    source = args.source.resolve(strict=True)
    out = run / "work/validation"
    out.mkdir()
    report = {"status": "RUNNING", "syntax": [], "commands": [],
              "disabled_project_builds": 0, "SDK_application_builds": 0,
              "Guest_runs": 0, "native_ROS_runs": 0, "SDK_tests_created": False,
              "inputs": []}

    def command(label, argv, expected=0):
        row = {"label": label, "argv": list(map(str, argv))}
        report["commands"].append(row)
        child = subprocess.run(row["argv"], stdin=subprocess.DEVNULL, cwd=out,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=90)
        row["exit"] = child.returncode
        (out / (label + ".stdout")).write_bytes(child.stdout)
        (out / (label + ".stderr")).write_bytes(child.stderr)
        assert child.returncode == expected, (label, child.stderr[-2000:])
        return child.stdout + child.stderr

    try:
        for path in sorted(source.rglob("*.py")):
            ast.parse(path.read_text(), filename=str(path))
            report["syntax"].append({"path": path.relative_to(source).as_posix(), **fact(path)})
        assert report["syntax"]
        assert not (source / "sdk/test").exists()
        for name in ("/usr/bin/cmake", "/usr/bin/clang-21", "/usr/bin/clang++-21"):
            path = Path(name).resolve(strict=True)
            report["inputs"].append({"path": str(path), **fact(path)})
        flags = ["-DCMAKE_C_COMPILER=/usr/bin/clang-21",
                 "-DCMAKE_CXX_COMPILER=/usr/bin/clang++-21", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"]
        projects = {"unitree": "sdk/origin/robots/unitree",
                    "deep": "sdk/origin/robots/platforms/deep_robotics"}
        for platform, relative in projects.items():
            project = source / relative
            report["inputs"].append({"path": relative + "/CMakeLists.txt", **fact(project / "CMakeLists.txt")})
            build = out / (platform + "-off")
            command(platform + "-configure", ["/usr/bin/cmake", "-S", project, "-B", build, *flags])
            command(platform + "-build", ["/usr/bin/cmake", "--build", build, "-j1"])
            commands = build / "compile_commands.json"
            assert not commands.exists() or json.loads(commands.read_bytes()) == []
            report["disabled_project_builds"] += 1
        refused = command("explicit-missing-test-artifact", ["/usr/bin/cmake", "-S",
            source / projects["unitree"], "-B", out / "missing-tests", *flags,
            "-DUWVM_UNITREE_BUILD_SPORT=ON", "-DUWVM_UNITREE_BUILD_SPORT_CONTRACTS=ON",
            "-DUWVM_UNITREE_TEST_ROOT=" + str(out / "not-present")], 1)
        assert b"separate canonical Unitree test artifact" in refused
        assert not (source / "sdk/test").exists()
        report.update(status="PASS_PYTHON_SYNTAX_CMAKE_OFF_AND_MIGRATION_GATE_ONLY",
                      migration_refusals=1)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        (out / "result.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
