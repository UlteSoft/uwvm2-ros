"""Current Patrol C source with actual WASI compiler, validator and Guest."""

import argparse, hashlib, json, os, struct, subprocess, time
from pathlib import Path

if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
    raise RuntimeError("common finite owner required")
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--source", type=Path, required=True)
args = parser.parse_args()
source = args.source.resolve(strict=True)
agents = Path("/home/macromodel/Documents/uwvm-test-261018/agents/08")
kit = agents / "20261021-contracts-r1/inputs/wasi-sdk"
validator = agents / "20261021-contracts-r1/inputs/wasm-tools"
runtime = agents / "20261021-contracts-r6/inputs/uwvm2-ros"
work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
environment = dict(
    os.environ,
    LD_LIBRARY_PATH="/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm/lib/x86_64-unknown-linux-gnu:/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu",
)
rows = []


def invoke(name, layer, argv, expected=0, output=None):
    prefix = work / (name + "-" + layer)
    with (
        prefix.with_suffix(".stdout").open("xb") as out,
        prefix.with_suffix(".stderr").open("xb") as err,
    ):
        result = subprocess.run(
            list(map(str, argv)),
            env=environment,
            stdin=subprocess.DEVNULL,
            stdout=out,
            stderr=err,
            timeout=45,
            check=False,
        )
    actual = prefix.with_suffix(".stdout").read_bytes()
    matched = result.returncode == expected and (output is None or actual == output)
    row = dict(
        name=name,
        layer=layer,
        argv=list(map(str, argv)),
        exit_code=result.returncode,
        expected_exit=expected,
        matched=matched,
        stdout_bytes=len(actual),
        stdout_sha256=hashlib.sha256(actual).hexdigest(),
    )
    rows.append(row)
    print(json.dumps(row), flush=True)
    return matched


assert (
    hashlib.sha256(runtime.read_bytes()).hexdigest()
    == "719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19"
)
invoke("wasi-clang", "version", [kit / "bin/clang", "--version"])
invoke("wasm-tools", "version", [validator, "--version"])
for selection, profile in ((2, "humble"), (3, "jazzy"), (4, "main")):
    family = "robotis-turtlebot3-" + profile + "-complete"
    artifact = work / ("patrol-" + profile + ".wasm")
    common = [
        "-std=c11",
        "-O1",
        "-UNDEBUG",
        "-DUWVM_TURTLEBOT3_PROFILE=" + str(selection),
    ]
    for folder in (
        "sdk/include",
        "sdk/include/platform/ros/c",
        "sdk/include/platform/ros1/c",
        "sdk/include/platform/ros2/c",
        "sdk/include/platform/robotis/c",
        "sdk/include/platform/robotis/c/ros2/" + family,
    ):
        common += ["-I" + str(source / folder)]
    command = [
        kit / "bin/clang",
        *common,
        source / "sdk/example/platform/robotis/c/patrol_client.c",
        "-lm",
        "-Wl,--threads=1",
        "-Wl,-z,stack-size=8388608",
        "-Wl,--max-memory=67108864",
        "-o",
        artifact,
    ]
    if not invoke("patrol-" + profile, "actual-wasi-compile", command):
        continue
    if not invoke(
        "patrol-" + profile, "actual-wasm-validate", [validator, "validate", artifact]
    ):
        continue
    guest = [
        runtime,
        "-Rint",
        "--wasip1-global-noinherit-system-environment",
        "--run",
        artifact,
    ]
    cases = [
        (
            "square",
            ["encode", "s", "1.25", "3"],
            0,
            struct.pack("<ddd", 1.0, 1.25, 3.0),
        ),
        (
            "triangle-negative",
            ["encode", "t", "-2.5", "-7"],
            0,
            struct.pack("<ddd", 2.0, -2.5, -7.0),
        ),
        (
            "integer-negative-zero",
            ["encode", "s", "1", "-0"],
            0,
            struct.pack("<ddd", 1.0, 1.0, 0.0),
        ),
        ("invalid-mode", ["encode", "q", "1", "1"], 22, b""),
        ("nonfinite-distance", ["encode", "s", "nan", "1"], 22, b""),
        ("overflow-distance", ["encode", "s", "1e999", "1"], 22, b""),
        ("fractional-count", ["encode", "s", "1", "1.5"], 22, b""),
        (
            "quit-opens-nothing",
            ["send", "cancel", "feedback", "result", "x", "1", "1", "1"],
            0,
            b"",
        ),
        (
            "invalid-poll-budget",
            ["send", "cancel", "feedback", "result", "s", "1", "1", "0"],
            22,
            b"",
        ),
        (
            "too-large-poll-budget",
            ["send", "cancel", "feedback", "result", "s", "1", "1", "4097"],
            22,
            b"",
        ),
        (
            "duplicate-role-alias",
            ["same", "same", "feedback", "result", "s", "1", "1", "1"],
            22,
            b"",
        ),
        (
            "default-empty-authority",
            ["send", "cancel", "feedback", "result", "s", "1", "1", "1"],
            13,
            b"",
        ),
    ]
    for label, argv, expected, output in cases:
        invoke(profile + "-" + label, "actual-guest", guest + argv, expected, output)
pins = []
for file in sorted(source.rglob("*")):
    if file.is_file():
        body = file.read_bytes()
        pins.append(
            dict(
                path=str(file.relative_to(source)),
                bytes=len(body),
                sha256=hashlib.sha256(body).hexdigest(),
            )
        )
report = dict(
    schema="uwvm-task08-patrol-current-v1",
    rows=rows,
    input_hashes=pins,
    compiler=str(kit / "bin/clang"),
    runtime=dict(
        path=str(runtime), sha256=hashlib.sha256(runtime.read_bytes()).hexdigest()
    ),
    actual_compiler_cases=sum(
        row["layer"] == "actual-wasi-compile" and row["matched"] for row in rows
    ),
    actual_guest_cases=sum(
        row["layer"] == "actual-guest" and row["matched"] for row in rows
    ),
    native_ros_cases=0,
    gateway_cases=0,
    official_suite_cases=0,
    robot_cases=0,
    xmake_compiler_cases=0,
    default_authority=[],
    source_domain="finite ASCII argv and selected complete bounded original Patrol types",
)
(work / "patrol-current.json").write_text(
    json.dumps(report, indent=2, sort_keys=True) + "\n"
)
if (
    any(not row["matched"] for row in rows)
    or report["actual_compiler_cases"] != 3
    or report["actual_guest_cases"] != 36
):
    raise SystemExit(1)
