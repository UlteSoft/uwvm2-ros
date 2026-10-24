"""Build the remaining Jazzy/main Patrol selections from the sealed installed SDK.

Uses the exact read-only private SDK accepted by the Humble run. This checks two
real profile selections, not two additional logical official programs. Pure-goal
and default-denial Guests confer no positive ROS/Gateway/native action credit.
"""

import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
PARENT = BASE / "08/20261022-installed-patrol-nim-r3"
RUNTIME = BASE / "10/runtime-denial-r2/work/uwvm-dual"


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def pins(root):
    result = []
    for path in sorted(root.rglob("*")):
        assert not path.is_symlink()
        if path.is_file():
            result.append(dict(path=str(path.relative_to(root)), bytes=path.stat().st_size, sha256=sha(path)))
    return result


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original Linux common finite owner required")
    parent_path = PARENT / "work/installed-patrol-nim.json"
    parent = json.loads(parent_path.read_bytes())
    owner_path = PARENT / "resource-evidence/result.json"
    copies = json.loads((PARENT / "resource-archives/verified-copies.json").read_bytes())
    assert parent["status"] == "PASS_PRIVATE_INSTALLED_NIM_PATROL_BUILD_VALIDATION_AND_EMPTY_AUTHORITY_GUESTS"
    assert json.loads(owner_path.read_bytes())["status"] == "PASS"
    assert copies["copies"] == 2 and copies["result_sha256"] == sha(owner_path)
    sdk = Path(parent["sdk_root"])
    assert sdk == PARENT / "work/relocated-sdk"
    assert not (sdk / "origin").exists() and not (sdk / "test").exists()
    before = pins(sdk)
    assert before == parent["SDK_pins_before"]
    tools = parent["tools"]
    for row in tools:
        path = Path(row["path"])
        assert path.stat().st_size == row["bytes"] and sha(path) == row["sha256"]
    xmake, validator = Path("/home/macromodel/.local/bin/xmake"), Path("/usr/bin/wasm-validate")
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    environment = dict(os.environ, XMAKE_STATS="false", XMAKE_SKIP_HISTORY="y",
        XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake", OMP_NUM_THREADS="1",
        OPENBLAS_NUM_THREADS="1")
    rows, profiles = [], []
    report = dict(schema="uwvm-task08-installed-patrol-nim-profile-selection-v1", status="STARTED",
        parent_receipt=dict(path=str(parent_path), sha256=sha(parent_path)),
        parent_owner=dict(path=str(owner_path), sha256=sha(owner_path)),
        runner_sha256=sha(Path(__file__)), sdk_root=str(sdk), SDK_pins_before=before,
        tools=tools, commands=rows, profiles=profiles, actual_installed_xmake_builds=0,
        wasm_validations=0, actual_Guest_cases=0, logical_programs=1,
        positive_ROS_Guest_runs=0, Gateway_runs=0, native_ROS_runs=0, physical_robot_runs=0,
        official_parity_accepted=False, whole_sdk_assembly=False, default_authority=[])

    def save():
        (work / "installed-patrol-nim-profiles.json").write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")

    def run(name, argv, project, expected_exit=0, expected_output=None):
        arguments = list(map(str, argv))
        with (work / (name + ".stdout")).open("xb") as stdout, (work / (name + ".stderr")).open("xb") as stderr:
            result = subprocess.run(arguments, env=environment, cwd=project, stdin=subprocess.DEVNULL,
                                    stdout=stdout, stderr=stderr, timeout=120)
        rows.append(dict(name=name, argv=arguments, exit=result.returncode, expected_exit=expected_exit,
            stdout_sha256=sha(work / (name + ".stdout")), stderr_sha256=sha(work / (name + ".stderr"))))
        save()
        assert result.returncode == expected_exit, name
        if expected_output is not None:
            assert (work / (name + ".stdout")).read_bytes() == expected_output, name + " output"

    try:
        for profile in ("jazzy", "main"):
            project = work / (profile + "-project")
            project.mkdir()
            run(profile + "-actual-xmake-configuration", [xmake, "f", "--project=" + str(project),
                "--file=" + str(sdk / "xmake.lua"), "--yes", "--ros-sdk-platforms=ros",
                "--ros-sdk-languages=nim", "--sdk-example-platform=robotis", "--sdk-example-language=nim",
                "--sdk-example=patrol-client." + profile, "--sdk-ros-backend=ros2",
                "--sdk-target=wasm32-wasip1", "--sdk-startup=command", "--sdk-exceptions=n",
                "--sdk-threads=n", "--sdk-offline=y", "--sdk-toolchain=" + str(BASE),
                "--sdk-python=/usr/bin/python3", "--sdk-profile-manifest=" + str(sdk / "share/native_languages-nim-profile.json"),
                "--sdk-native-c-compiler=" + str(BASE / "10/staged-wasi-r1/staged/wasi-sdk/bin/clang")], project)
            product = work / ("patrol-" + profile)
            run(profile + "-actual-installed-xmake-build", [xmake, "sdk-example-build", "--project=" + str(project),
                "--file=" + str(sdk / "xmake.lua"), "--output=" + str(product)], project)
            receipt = json.loads((product / "build-receipt.json").read_bytes())
            assert receipt["status"] == "BUILT_NOT_EXECUTED" and receipt["default_authority"] == []
            assert receipt["selection"] == dict(example="patrol-client." + profile, language="nim", platform="robotis")
            assert len(receipt["artifacts"]) == 1
            item = receipt["artifacts"][0]
            artifact = Path(item["path"])
            assert artifact.is_relative_to(product) and item["kind"] == "wasm"
            assert artifact.stat().st_size == item["bytes"] and sha(artifact) == item["sha256"]
            run(profile + "-standard-Wasm-validator", [validator, artifact], project)
            run(profile + "-exact-ROS2-imports", [sys.executable, "-I", "-S", "-B", sdk / "tools/check_wasm_imports.py",
                artifact, "--backend", "2", "--allow-module", "wasi_snapshot_preview1", "--require-all",
                "--report", work / (profile + "-imports.json")], project)
            guest = [RUNTIME, "-Rint", "--wasip1-global-noinherit-system-environment", "--run", artifact]
            run(profile + "-Guest-complete-original-goal", [*guest, "encode", "t", "-0.5", "3"], project,
                expected_output=struct.pack("<ddd", 2.0, -0.5, 3.0))
            run(profile + "-Guest-empty-authority-denial", [*guest, "patrol.send", "patrol.cancel-own",
                "patrol.feedback", "patrol.result", "s", "1.25", "2", "1"], project, expected_exit=13)
            assert sha(artifact) == item["sha256"]
            profiles.append(dict(profile=profile, build_receipt=receipt, artifact=item,
                                 positive_action=False, pure_goal_Guests=1, denial_Guests=1))
            report["actual_installed_xmake_builds"] += 1
            report["wasm_validations"] += 1
            report["actual_Guest_cases"] += 2
        report["status"] = "PASS_TWO_REMAINING_INSTALLED_NIM_PROFILE_BUILDS_AND_SCOPED_GUESTS"
        report["same_artifact_bytes_as_Humble"] = all(p["artifact"]["sha256"] == parent["artifact"]["sha256"] for p in profiles)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        report["SDK_unchanged_during_build"] = pins(sdk) == before
        report["tools_unchanged"] = all(sha(Path(row["path"])) == row["sha256"] for row in tools)
        if not report["SDK_unchanged_during_build"] or not report["tools_unchanged"]:
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        save()
        if report["status"] == "FAILED_INPUT_IMMUTABILITY_RETAINED":
            raise RuntimeError("installed SDK/tools changed")


if __name__ == "__main__":
    main()
