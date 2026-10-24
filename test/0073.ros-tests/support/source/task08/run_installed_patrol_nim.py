"""Build one exact Patrol command from a physically separate installed Nim SDK.

Requires the original Linux finite owner. Copies/pin-checks the genuine Task03
installed input, overlays exact current owned sources and Nim source modules,
refreshes only those explicit private source pins, and calls real xmake. Target
validator/import checks and empty-authority Guests confer no native action parity.
"""

import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import zipfile


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
PARENT_SDK = BASE / "03/20261021-round03-installed-nim-r1/work/relocated-sdk"
RUNTIME = BASE / "10/runtime-denial-r2/work/uwvm-dual"
WASI = BASE / "10/staged-wasi-r1/staged/wasi-sdk"
PREFIX = "sdk/origin/"
PATROL = (
    PREFIX + "robots/mobile_bases/robotis/turtlebot3/examples/patrol/nim/Patrol.nim"
)


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def pins(root):
    result = []
    for path in sorted(root.rglob("*")):
        assert not path.is_symlink()
        if path.is_file():
            result.append(
                dict(
                    path=str(path.relative_to(root)),
                    bytes=path.stat().st_size,
                    sha256=sha(path),
                )
            )
    return result


def main():
    if (
        os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1"
        or sys.platform != "linux"
    ):
        raise RuntimeError("original Linux finite common owner required")
    packet = Path(sys.argv[1])
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    with zipfile.ZipFile(packet) as archive:
        assert (
            len(archive.infolist()) <= 32
            and sum(row.file_size for row in archive.infolist()) < 1 << 20
        )
        bodies = {row.filename: archive.read(row) for row in archive.infolist()}
    parent = json.loads(bodies["parent-installed-nim-r2.json"])
    assert (
        parent["status"]
        == "PASS_INSTALLED_NIM_XMAKE_TWO_BACKENDS_WITH_PINNED_PRIVATE_OVERLAY"
    )
    assert parent["sdk_root"] == str(PARENT_SDK)
    expected = dict(parent["SDK_pins_before"])
    assert len(expected) == 349
    overlay = parent["private_overlay"]
    # The parent pins were captured after its explicit private native_languages fix.
    assert expected[overlay["path"]] == overlay["after_sha256"]
    assert sha(PARENT_SDK / overlay["path"]) == overlay["after_sha256"]
    actual = {row["path"]: row for row in pins(PARENT_SDK)}
    assert {name: row["sha256"] for name, row in actual.items()} == expected
    sdk = work / "relocated-sdk"
    shutil.copytree(PARENT_SDK, sdk)
    assert pins(sdk) == list(actual.values())
    assert not (sdk / "origin").exists() and not (sdk / "test").exists()
    changes = []

    def replace(destination, body, source):
        target = sdk / destination
        before = sha(target) if target.exists() else None
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(body)
        changes.append(
            dict(
                path=destination,
                source=source,
                before_sha256=before,
                after_sha256=sha(target),
                bytes=len(body),
            )
        )

    rules = PREFIX + "platform/robotis/example-rules/"
    for name in ("backend.lua", "profiles.lua", "patrol-module.lua", "nim/xmake.lua"):
        replace("example/platform/robotis/" + name, bodies[rules + name], rules + name)
    replace(
        "example/platform/robotis/nim/patrol/nim/Patrol.nim", bodies[PATROL], PATROL
    )
    for name in (
        "roswasm.nim",
        "roswasm/raw.nim",
        "roswasm/wire.nim",
        "roswasm/typed.nim",
    ):
        source = PREFIX + "nim/src/" + name
        for destination in (
            "include/platform/ros/nim/src/",
            "lib/platform/ros/nim/source/src/",
        ):
            replace(destination + name, bodies[source], source)
    profile_path = sdk / "share/native_languages-nim-profile.json"
    profile = json.loads(profile_path.read_bytes())
    selected = {row["path"]: row for row in changes}
    refreshed = []
    for row in profile["profiles"][0]["files"]:
        path = sdk / row["path"]
        if row["path"] in selected:
            assert row["sha256"] == selected[row["path"]]["before_sha256"]
            refreshed.append(
                dict(
                    path=row["path"],
                    before_sha256=row["sha256"],
                    after_sha256=sha(path),
                )
            )
            row.update(bytes=path.stat().st_size, sha256=sha(path))
        else:
            assert row["bytes"] == path.stat().st_size and row["sha256"] == sha(path)
    assert len(refreshed) == 8
    profile_path.write_text(json.dumps(profile, indent=2) + "\n")
    before = pins(sdk)
    project = work / "project"
    project.mkdir()
    logs = work / "logs"
    logs.mkdir()
    xmake = Path("/home/macromodel/.local/bin/xmake")
    validator = Path("/usr/bin/wasm-validate")
    assert all(
        path.is_file() for path in (xmake, validator, RUNTIME, WASI / "bin/clang")
    )
    environment = dict(
        os.environ,
        XMAKE_STATS="false",
        XMAKE_SKIP_HISTORY="y",
        XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake",
        OMP_NUM_THREADS="1",
        OPENBLAS_NUM_THREADS="1",
    )
    rows = []
    report = dict(
        schema="uwvm-task08-installed-patrol-nim-v1",
        status="STARTED",
        parent_sdk=str(PARENT_SDK),
        parent_sdk_before=actual,
        parent_receipt_sha256=hashlib.sha256(
            bodies["parent-installed-nim-r2.json"]
        ).hexdigest(),
        input_packet_sha256=sha(packet),
        sdk_root=str(sdk),
        explicit_source_overlays=changes,
        explicit_private_profile_source_pin_refresh=refreshed,
        SDK_pins_before=before,
        commands=rows,
        tools=[
            dict(path=str(p), sha256=sha(p), bytes=p.stat().st_size)
            for p in (xmake, validator, RUNTIME, WASI / "bin/clang")
        ],
        assembly="SCOPED_PRIVATE_INSTALLED_CANDIDATE",
        whole_sdk_assembly=False,
        official_parity_accepted=False,
        positive_ROS_Guest_runs=0,
        Gateway_runs=0,
        native_ROS_runs=0,
        physical_robot_runs=0,
        default_authority=[],
    )

    def save():
        (work / "installed-patrol-nim.json").write_text(
            json.dumps(report, indent=2, sort_keys=True) + "\n"
        )

    def run(name, argv, expected_exit=0, expected_output=None):
        arguments = list(map(str, argv))
        with (
            (logs / (name + ".stdout")).open("xb") as stdout,
            (logs / (name + ".stderr")).open("xb") as stderr,
        ):
            result = subprocess.run(
                arguments,
                env=environment,
                cwd=project,
                stdin=subprocess.DEVNULL,
                stdout=stdout,
                stderr=stderr,
                timeout=180,
            )
        output = (logs / (name + ".stdout")).read_bytes()
        row = dict(
            name=name,
            argv=arguments,
            exit=result.returncode,
            expected_exit=expected_exit,
            stdout_sha256=sha(logs / (name + ".stdout")),
            stderr_sha256=sha(logs / (name + ".stderr")),
        )
        rows.append(row)
        save()
        assert result.returncode == expected_exit, name
        if expected_output is not None:
            assert output == expected_output, name + " output"
        return output

    try:
        run("xmake-version", [xmake, "--version"])
        run(
            "Nim-profile-configuration",
            [
                xmake,
                "f",
                "--project=" + str(project),
                "--file=" + str(sdk / "xmake.lua"),
                "--yes",
                "--ros-sdk-platforms=ros",
                "--ros-sdk-languages=nim",
                "--sdk-example-platform=robotis",
                "--sdk-example-language=nim",
                "--sdk-example=patrol-client.humble",
                "--sdk-ros-backend=ros2",
                "--sdk-target=wasm32-wasip1",
                "--sdk-startup=command",
                "--sdk-exceptions=n",
                "--sdk-threads=n",
                "--sdk-offline=y",
                "--sdk-toolchain=" + str(BASE),
                "--sdk-python=/usr/bin/python3",
                "--sdk-profile-manifest=" + str(profile_path),
                "--sdk-native-c-compiler=" + str(WASI / "bin/clang"),
            ],
        )
        product = work / "patrol-humble"
        run(
            "actual-installed-xmake-build",
            [
                xmake,
                "sdk-example-build",
                "--project=" + str(project),
                "--file=" + str(sdk / "xmake.lua"),
                "--output=" + str(product),
            ],
        )
        receipt = json.loads((product / "build-receipt.json").read_bytes())
        assert (
            receipt["status"] == "BUILT_NOT_EXECUTED"
            and receipt["default_authority"] == []
        )
        assert len(receipt["artifacts"]) == 1
        item = receipt["artifacts"][0]
        artifact = Path(item["path"])
        assert item["kind"] == "wasm" and artifact.is_relative_to(product)
        assert (
            artifact.stat().st_size == item["bytes"] and sha(artifact) == item["sha256"]
        )
        run("actual-wasm-validator", [validator, artifact])
        run(
            "exact-ROS2-import-contract",
            [
                sys.executable,
                "-I",
                "-S",
                "-B",
                sdk / "tools/check_wasm_imports.py",
                artifact,
                "--backend",
                "2",
                "--allow-module",
                "wasi_snapshot_preview1",
                "--require-all",
                "--report",
                work / "imports.json",
            ],
        )
        guest = [
            RUNTIME,
            "-Rint",
            "--wasip1-global-noinherit-system-environment",
            "--run",
            artifact,
        ]
        run(
            "Guest-complete-goal-square",
            [*guest, "encode", "s", "1.25", "-2"],
            expected_output=struct.pack("<ddd", 1.0, 1.25, -2.0),
        )
        run(
            "Guest-complete-goal-triangle",
            [*guest, "encode", "t", "-0.5", "3"],
            expected_output=struct.pack("<ddd", 2.0, -0.5, 3.0),
        )
        run(
            "Guest-count-negative-zero",
            [*guest, "encode", "s", "0", "-0"],
            expected_output=struct.pack("<ddd", 1.0, 0.0, 0.0),
        )
        aliases = [
            "patrol.send",
            "patrol.cancel-own",
            "patrol.feedback",
            "patrol.result",
        ]
        run("Guest-quit-before-open", [*guest, *aliases, "x", "0", "0", "1"])
        run(
            "Guest-empty-authority-denial",
            [*guest, *aliases, "s", "1.25", "2", "1"],
            expected_exit=13,
        )
        for label, arguments in (
            ("fractional-count", ["encode", "s", "1.25", "2.5"]),
            ("nonfinite-distance", ["encode", "s", "nan", "2"]),
            ("poll-budget-zero", [*aliases, "s", "1.25", "2", "0"]),
        ):
            run("Guest-invalid-" + label, [*guest, *arguments], expected_exit=22)
        report.update(
            status="PASS_PRIVATE_INSTALLED_NIM_PATROL_BUILD_VALIDATION_AND_EMPTY_AUTHORITY_GUESTS",
            build_receipt=receipt,
            actual_installed_xmake_builds=1,
            wasm_validations=1,
            actual_Guest_cases=8,
            artifact=dict(item),
        )
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        report["SDK_unchanged_during_build"] = pins(sdk) == before
        report["parent_SDK_unchanged"] = {
            row["path"]: row for row in pins(PARENT_SDK)
        } == actual
        save()


if __name__ == "__main__":
    try:
        main()
    except BaseException as error:
        report = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work/installed-patrol-nim.json"
        if not report.exists():
            report.write_text(
                json.dumps(
                    dict(
                        schema="uwvm-task08-installed-patrol-nim-v1",
                        status="FAILED_SETUP_RETAINED",
                        error=repr(error),
                        runner_sha256=sha(Path(__file__)),
                        input_packet_sha256=sha(Path(sys.argv[1])),
                        actual_installed_xmake_builds=0,
                        default_authority=[],
                    ),
                    indent=2,
                )
                + "\n"
            )
        raise
