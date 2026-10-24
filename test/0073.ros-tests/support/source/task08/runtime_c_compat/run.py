"""Retain tiny controls for the real C program's reproducible Runtime SIGILL.

Genuine WASI C11 links and validators precede all six actual Runtime runs.
No ROS import is called by these controls. The job keeps every nonzero exit as
a failure; a diagnostic reproduction never qualifies a program or platform.
"""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
WASI = BASE / "10/staged-wasi-r1/staged/wasi-sdk"
SDK = BASE / "08/20261022-obstacle-c-frontend-r5/work/source/sdk/include"
RUNTIMES = {
    "runtime10": (BASE / "10/runtime-denial-r2/work/uwvm-dual", "0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556"),
    "runtime04": (BASE / "04/lua-guest-ros2-r2/input/runtime/uwvm2-ros", "719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19"),
}


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def pins(root):
    rows = []
    for path in sorted(root.rglob("*")):
        assert not path.is_symlink()
        if path.is_file():
            rows.append(dict(path=str(path.relative_to(root)), bytes=path.stat().st_size, sha256=sha(path)))
    return rows


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original Linux finite common owner required")
    source = Path(__file__).parent
    source_before, sdk_before = pins(source), pins(SDK)
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    tools = [WASI / "bin/clang", WASI / "bin/wasm-ld", Path("/usr/bin/wasm-validate"), Path("/usr/bin/wasm-objdump")]
    for path, expected in RUNTIMES.values():
        assert sha(path) == expected
        tools.append(path)
    tool_pins = [dict(path=str(path), sha256=sha(path)) for path in tools]
    commands, products, outcomes = [], [], []
    report = dict(schema="uwvm-task08-C-Runtime-compatibility-controls-v1", status="STARTED",
        source_pins_before=source_before, SDK_pins_before=sdk_before, tools=tool_pins,
        commands=commands, artifacts=products, Runtime_outcomes=outcomes, ROS_import_calls=0,
        positive_ROS_runs=0, native_ROS_runs=0, installed_builds=0, official_parity_accepted=False, default_authority=[])

    def save():
        (work / "runtime-C-compatibility.json").write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")

    def command(name, argv, must_succeed):
        argv = list(map(str, argv))
        value = subprocess.run(argv, cwd=work, stdin=subprocess.DEVNULL, capture_output=True, timeout=30)
        assert len(value.stdout) + len(value.stderr) < 256 << 10
        for kind in ("stdout", "stderr"):
            (work / (name + "." + kind)).write_bytes(getattr(value, kind))
        commands.append(dict(name=name, argv=argv, exit=value.returncode,
            stdout_sha256=sha(work / (name + ".stdout")), stderr_sha256=sha(work / (name + ".stderr"))))
        save()
        if must_succeed:
            assert value.returncode == 0, name
        return value

    try:
        for name in ("argc", "defined_table", "import_table"):
            artifact = work / (name + ".wasm")
            command(name + "-actual-C11-link", [tools[0], "--target=wasm32-wasip1",
                "--sysroot=" + str(WASI / "share/wasi-sysroot"), "-std=c11", "-O1", "-UNDEBUG", "-I" + str(SDK),
                source / (name + ".c"), "-lm", "-Wl,--threads=1", "-Wl,-z,stack-size=8388608",
                "-Wl,--max-memory=67108864", "-o", artifact], True)
            products.append(dict(name=name, path=str(artifact), bytes=artifact.stat().st_size, sha256=sha(artifact)))
            command(name + "-actual-Wasm-validation", [tools[2], artifact], True)
            imported = command(name + "-actual-imports", [tools[3], "-x", artifact], True)
            if name == "import_table":
                assert b"uwvm_ros2_v1.abi_version" in imported.stdout
            for runtime_name, (runtime, expected) in RUNTIMES.items():
                result = command(name + "-" + runtime_name, [runtime, "-Rint",
                    "--wasip1-global-noinherit-system-environment", "--run", artifact, "quit"], False)
                outcomes.append(dict(consumer=name, runtime=runtime_name, runtime_sha256=expected,
                    exit=result.returncode, expected_exit=0, passed=result.returncode == 0))
        report["status"] = "PASS_SCOPED_CONTROLS_NO_ROS_EFFECT" if all(row["passed"] for row in outcomes) else "FAILED_RUNTIME_COMPATIBILITY_RETAINED"
        if report["status"] != "PASS_SCOPED_CONTROLS_NO_ROS_EFFECT":
            raise RuntimeError("genuine Runtime control failures retained without acceptance credit")
    except BaseException as error:
        report.setdefault("error", repr(error))
        if report["status"] == "STARTED":
            report["status"] = "FAILED_RETAINED"
        raise
    finally:
        report.update(source_unchanged=pins(source) == source_before, SDK_unchanged=pins(SDK) == sdk_before,
            tools_unchanged=all(sha(Path(row["path"])) == row["sha256"] for row in tool_pins))
        if not all(report[key] for key in ("source_unchanged", "SDK_unchanged", "tools_unchanged")):
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        save()


if __name__ == "__main__":
    main()
