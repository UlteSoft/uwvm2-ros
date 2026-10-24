"""Build complete obstacle/PTP applications from a private installed C SDK.

Only the genuine sealed Task09 C foundation/toolchain files and explicit owned
overlays are copied. Actual xmake builds, validation/import checks and complete
value/default-denial Guests stay separate from positive ROS/native acceptance.
Requires the original Linux finite owner and an honest32MiB disk allowance.
Both products finish their build/validator/import gates before an optional Guest
phase. No Runtime is selected by default; optional --runtime uses the explicit
retained binary under the qualified execution recipe. The two retained
Runtime binaries need no code repair: their guard reservation requires the
proven finite32768MiB RLIMIT_AS, distinct from physical/RSS memory limits.
"""

import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import resource
import shutil
import struct
import subprocess
import sys
import zipfile


BASE = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
PARENT = BASE / "09/installed-c-current-r36/work/relocated-sdk"
WASI = BASE / "10/staged-wasi-r1/staged/wasi-sdk"
GUEST_ADDRESS_SPACE_MIB = 32768


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


def require_guest_address_space():
    """Inspect the original owner's finite known-good envelope; never change it.

    Task10 traced the retained wasm32 Runtime's8590000192-byte PROT_NONE guard
    reservation failing under the former8589934592-byte RLIMIT_AS. Both exact
    original Runtimes passed this frozen application's vectors at32768MiB.
    This virtual reservation does not allocate that quantity of resident RAM.
    Other programs/Runtimes still need their own accounting and actual execution.
    """
    soft, hard = resource.getrlimit(resource.RLIMIT_AS)
    expected = GUEST_ADDRESS_SPACE_MIB << 20
    if (soft, hard) != (expected, expected):
        raise RuntimeError("this ordinary-C Guest recipe requires the original owner's finite32768MiB RLIMIT_AS; do not repeat the proven-insufficient8GiB envelope or change Runtime/ROS grants")
    return dict(soft_bytes=soft, hard_bytes=hard, address_mib=GUEST_ADDRESS_SPACE_MIB,
                virtual_guard_is_not_resident_RAM=True,
                diagnosis="Task10/RUNTIME-ADDRESS-SPACE.md; original8GiB failures remain failures")


def scan_wire(ranges, intensities=(0.5, 1.25)):
    # Whole original ROS2 Header + seven float32 metadata values and both arrays.
    frame = "task08-full-scan-λ".encode()
    return (struct.pack("<iII", -2, 345678901, len(frame)) + frame
        + struct.pack("<7fI", -3.0, 3.0, 0.01, 0.0001, 0.1, 0.05, 9.0, len(ranges))
        + struct.pack("<" + "f" * len(ranges), *ranges)
        + struct.pack("<I", len(intensities)) + struct.pack("<" + "f" * len(intensities), *intensities))


def original_ptp_goal(index):
    positions = [-0.5, -math.pi / 4, 0.0, -3 * math.pi / 4, 0.0, math.pi / 2, math.pi / 4]
    if index == 20:
        positions[0] = positions[2] = 0.0
    elif index:
        direction = 1 if index % 2 else -1
        positions[0] = direction * 1.0 * index / 10.0
        positions[2] = direction * 0.75 * index / 10.0
    return struct.pack("<I7dI8d", 7, *positions, 7, *([1.0] * 7), 0.01)


def exercise_program(platform, artifact, guest):
    guest(platform + "-quit", artifact, ["quit"], expected_output=b"")
    if platform == "robotis":
        raw = (1.25, -2.5, 3.75, -4.25, 5.5, -6.75)
        for label, count, beam, distance, custom in (
            ("clear-full-six-fields", 360, 0, 1.5, True),
            ("obstacle-preserves-only-original-yaw", 360, 0, 0.25, True),
            ("threshold-equality-clear", 360, 359, 0.5, True),
            ("excluded-middle-beam", 360, 180, 0.125, True),
            ("original-default-forward", 4, 0, 1.5, False),
            ("hardcoded360-last-nonempty-slice", 479, 359, 0.25, True)):
            ranges = [2.0] * count
            ranges[beam] = distance
            scan = scan_wire(ranges)
            body = struct.pack("<I", len(scan)) + scan + bytes((int(custom),)) + (struct.pack("<6d", *raw) if custom else b"")
            # Actual original finite-domain slice/min branch, retaining the360 endpoint.
            selected = ranges[:count // 4] + ranges[3 * count // 4:360]
            minimum = min(selected)
            obstacle = minimum < 0.5
            command = ((0.0,) * 5 + (raw[5] if custom else 0.0,)) if obstacle else (raw if custom else (0.2, 0, 0, 0, 0, 0))
            expected = struct.pack("<dB6d", minimum, int(obstacle), *command)
            guest("obstacle-" + label, artifact, ["step"], body=body, expected_output=expected)
        for count in (0, 3, 480):
            scan = scan_wire([2.0] * count)
            guest("obstacle-empty-original-slice-" + str(count), artifact, ["step"],
                body=struct.pack("<I", len(scan)) + scan + b"\0", expected_exit=22, expected_output=b"")
        guest("obstacle-empty-authority-denial", artifact,
            ["run", "scan", "raw-twist", "publish-twist", "steady-clock", "info-log", "1"], expected_exit=13)
    else:
        for index in range(21):
            guest("ptp-complete-original-target-" + str(index), artifact, ["encode", str(index)], expected_output=original_ptp_goal(index))
        guest("ptp-invalid-target-index", artifact, ["encode", "21"], expected_exit=22, expected_output=b"")
        # Explicit synthetic finite limits are used only for refusal; no native model/hardware acceptance.
        limits = struct.pack("<22d", *([-4.0] * 7 + [4.0] * 7 + [2.0] * 7 + [0.1]))
        args = ["run", "ptp-send", "ptp-cancel-own", "ptp-feedback", "ptp-result", "1", "32"]
        guest("ptp-empty-authority-denial", artifact, args, body=limits, expected_exit=13)
        invalid = struct.pack("<22d", *([-4.0] * 7 + [4.0] * 7 + [0.0] * 7 + [0.1]))
        guest("ptp-invalid-model-before-open", artifact, args, body=invalid, expected_exit=22)


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("original Linux common finite owner required")
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("packet", type=Path)
    parser.add_argument("--runtime", type=Path)
    args = parser.parse_args()
    packet = args.packet
    runtime = args.runtime.resolve(strict=True) if args.runtime is not None else None
    if runtime is not None:
        assert args.runtime.is_absolute() and runtime.is_file()
    guest_limits = require_guest_address_space() if runtime is not None else None
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    with zipfile.ZipFile(packet) as archive:
        members = archive.infolist()
        assert len(members) <= 48 and sum(row.file_size for row in members) <= 2 << 20
        assert len({row.filename for row in members}) == len(members)
        bodies = {}
        for row in members:
            name = Path(row.filename)
            assert not name.is_absolute() and ".." not in name.parts and not row.is_dir()
            bodies[row.filename] = archive.read(row)
    manifest = json.loads(bodies["manifest.json"])
    parent = json.loads(bodies["parent-installed-c-r36.json"])
    assert parent["status"] == "PASS" and parent["installed_sdk"] == str(PARENT)
    parent_before = pins(PARENT)
    complete_parent = {row["path"]: row for row in parent_before}
    for row in parent["source_pins"] + parent["copied_toolchain_pins"]:
        assert complete_parent[row["path"]] == row
    sdk = work / "relocated-sdk"
    sdk.mkdir()
    copied = []
    for row in parent["source_pins"] + parent["copied_toolchain_pins"]:
        if "/nvidia-isaac-ros/" in row["path"]:
            continue
        target = sdk / row["path"]
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(PARENT / row["path"], target)
        assert sha(target) == row["sha256"]
        copied.append(row)
    profile = "share/wasm-profiles.json"
    (sdk / profile).parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(PARENT / profile, sdk / profile)
    profile_before = sha(sdk / profile)
    changes = []
    for row in manifest["overlays"]:
        destination = Path(row["destination"])
        assert not destination.is_absolute() and ".." not in destination.parts
        assert destination.parts[0] in ("include", "example", "tools", "share")
        body = bodies[row["source"]]
        assert hashlib.sha256(body).hexdigest() == row["sha256"]
        target = sdk / destination
        before = sha(target) if target.is_file() else None
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(body)
        changes.append(dict(row, before_sha256=before, bytes=len(body)))
    assert not any((sdk / name).exists() for name in ("origin", "test", "tests"))
    sdk_before = pins(sdk)
    project = work / "project"
    project.mkdir()
    logs = work / "logs"
    logs.mkdir()
    xmake = Path("/home/macromodel/.local/bin/xmake")
    validator = Path("/usr/bin/wasm-validate")
    tools = (xmake, validator, Path("/usr/bin/wasm-objdump"), WASI / "bin/clang", WASI / "bin/wasm-ld")
    if runtime is not None:
        tools += (runtime,)
    tool_pins = [dict(path=str(path), bytes=path.stat().st_size, sha256=sha(path)) for path in tools]
    environment = dict(os.environ, XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake",
        XMAKE_STATS="false", XMAKE_SKIP_HISTORY="y")
    commands = []
    report = dict(schema="uwvm-task08-installed-official-c-programs-v1", status="STARTED",
        input_packet_sha256=sha(packet), parent_sdk=str(PARENT), parent_pins_before=parent_before,
        parent_receipt_sha256=hashlib.sha256(bodies["parent-installed-c-r36.json"]).hexdigest(),
        copied_parent_files=copied, explicit_owned_overlays=changes, sdk_root=str(sdk),
        SDK_pins_before=sdk_before, tools=tool_pins, commands=commands, artifacts=[],
        actual_installed_xmake_builds=0, wasm_validations=0, import_contracts=0, Guest_cases=0,
        positive_ROS_Guest_runs=0, Gateway_runs=0, native_ROS_runs=0, physical_robot_runs=0,
        whole_sdk_assembly=False, official_parity_accepted=False, default_authority=[],
        selected_runtime=(dict(path=str(runtime), sha256=sha(runtime)) if runtime is not None else None),
        Guest_address_envelope=guest_limits,
        Guest_execution=("EXPLICIT_RUNTIME_AFTER_BOTH_BUILD_GATES" if runtime is not None
                         else "NOT_REQUESTED_BUILD_VALIDATION_PHASE_ONLY"))

    def save():
        (work / "installed-official-c-programs.json").write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")

    def run(name, argv, body=None, expected_exit=0, expected_output=None):
        argv = list(map(str, argv))
        kwargs = dict(stdin=subprocess.DEVNULL) if body is None else dict(input=body)
        completed = subprocess.run(argv, cwd=project, env=environment, capture_output=True, timeout=120, **kwargs)
        assert len(completed.stdout) + len(completed.stderr) < 512 << 10
        for kind in ("stdout", "stderr"):
            (logs / (name + "." + kind)).write_bytes(getattr(completed, kind))
        commands.append(dict(name=name, argv=argv, exit=completed.returncode, expected_exit=expected_exit,
            stdin_sha256=hashlib.sha256(body).hexdigest() if body is not None else None,
            stdout_sha256=sha(logs / (name + ".stdout")), stderr_sha256=sha(logs / (name + ".stderr"))))
        save()
        assert completed.returncode == expected_exit, name
        if expected_output is not None:
            assert completed.stdout == expected_output, name + " complete output"
        return completed.stdout

    def guest(name, artifact, arguments, **kwargs):
        assert runtime is not None
        run(name, [runtime, "-Rint", "--wasip1-global-noinherit-system-environment", "--run", artifact, *arguments], **kwargs)
        report["Guest_cases"] += 1

    try:
        run("xmake-version", [xmake, "--version"])
        for platform, example in (("robotis", "obstacle-detection.humble"), ("franka", "ptp-motion-sequence.ros2")):
            run(platform + "-configure", [xmake, "f", "--project=" + str(project), "--file=" + str(sdk / "xmake.lua"),
                "--yes", "--ros-sdk-platforms=" + platform, "--ros-sdk-languages=c",
                "--sdk-example-platform=" + platform, "--sdk-example-language=c", "--sdk-example=" + example,
                "--ros-sdk-c-standard=11", "--sdk-target=wasm32-wasip1", "--sdk-startup=command",
                "--sdk-exceptions=n", "--sdk-threads=n", "--sdk-offline=y", "--sdk-toolchain=" + str(WASI),
                "--sdk-python=/usr/bin/python3"])
            product = work / (platform + "-build")
            run(platform + "-installed-xmake-build", [xmake, "sdk-example-build", "--project=" + str(project),
                "--file=" + str(sdk / "xmake.lua"), "--output=" + str(product)])
            receipt = json.loads((product / "build-receipt.json").read_bytes())
            assert receipt["status"] == "BUILT_NOT_EXECUTED" and receipt["default_authority"] == []
            assert len(receipt["artifacts"]) == 1
            item = receipt["artifacts"][0]
            artifact = Path(item["path"])
            assert artifact.is_relative_to(product) and item["kind"] == "wasm"
            assert artifact.stat().st_size == item["bytes"] and sha(artifact) == item["sha256"]
            report["artifacts"].append(dict(platform=platform, example=example, artifact=item, build_receipt=receipt))
            report["actual_installed_xmake_builds"] += 1
            run(platform + "-wasm-validation", [validator, artifact])
            report["wasm_validations"] += 1
            run(platform + "-complete-import-contract", [sys.executable, "-I", "-S", "-B", sdk / "tools/check_wasm_imports.py",
                artifact, "--backend", "2", "--allow-module", "wasi_snapshot_preview1",
                "--report", work / (platform + "-imports.json")])
            report["import_contracts"] += 1
        if runtime is not None:
            for item in report["artifacts"]:
                exercise_program(item["platform"], Path(item["artifact"]["path"]), guest)
            report["status"] = "PASS_TWO_PRIVATE_INSTALLED_C_PROGRAMS_COMPLETE_VALUE_AND_EMPTY_AUTHORITY_GUESTS"
        else:
            report["status"] = "PASS_TWO_PRIVATE_INSTALLED_C_PROGRAMS_BUILDS_VALIDATORS_ACTUAL_IMPORTS_ONLY"
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        report.update(SDK_pins_after=pins(sdk), SDK_unchanged=pins(sdk) == sdk_before,
            parent_unchanged=pins(PARENT) == parent_before, provider_manifest_unchanged=sha(sdk / profile) == profile_before,
            tools_unchanged=all(sha(Path(row["path"])) == row["sha256"] for row in tool_pins))
        if not all(report[key] for key in ("SDK_unchanged", "parent_unchanged", "provider_manifest_unchanged", "tools_unchanged")):
            report["status"] = "FAILED_IMMUTABILITY_RETAINED"
        save()
        if report["status"] == "FAILED_IMMUTABILITY_RETAINED":
            raise RuntimeError("selected installed/provider/tool inputs changed")


if __name__ == "__main__":
    main()
