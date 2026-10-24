"""Small genuine Buffer storage, xmake catalog and dependency-refusal checks.

No Wasm compiler/Guest or native ROS exchange credit follows from this run.
The larger selected real build recipe remains independently pending.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tarfile


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    assert os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") == "1"
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    work = run / "work"
    source = work / "source"
    source.mkdir()
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--source-archive", type=Path, default=run / "inputs/source.tar"
    )
    parser.add_argument("--skip-accepted-r12", action="store_true")
    args = parser.parse_args()
    assert args.source_archive.is_absolute() and args.source_archive.is_file()
    source_digest = digest(args.source_archive)
    with tarfile.open(args.source_archive) as archive:
        members = [
            m
            for m in archive
            if not any(part.startswith("._") for part in Path(m.name).parts)
        ]
        archive.extractall(source, members=members, filter="data")
    origin = source / "sdk/origin"
    nvidia = origin / "robots/nvidia"
    sdk = work / "installed-sdk"
    sdk.mkdir()
    tests = source / "test/0073.ros-tests/support/source/task09-nvidia"
    rows = []
    result = {
        "format": "uwvm-task09-light-example-qualification-v1",
        "cases": rows,
        "guest_executions": 0,
        "native_ros_exchanges": 0,
        "model_calls": 0,
        "simulator_runs": 0,
        "official_suite_credit": 0,
        "default_authority": [],
        "input_archive_sha256": source_digest,
    }

    def save():
        (work / "light-result.json").write_text(
            json.dumps(result, indent=2, sort_keys=True) + "\n"
        )

    def command(name, argv, cwd=work, env=None, refusal=False, timeout=180):
        argv = [str(x) for x in argv]
        out = work / (name + ".stdout")
        err = work / (name + ".stderr")
        with out.open("wb") as stdout, err.open("wb") as stderr:
            completed = subprocess.run(
                argv,
                cwd=cwd,
                env=env,
                stdin=subprocess.DEVNULL,
                stdout=stdout,
                stderr=stderr,
                timeout=timeout,
            )
        good = completed.returncode != 0 if refusal else completed.returncode == 0
        row = {
            "case": name,
            "argv": argv,
            "cwd": str(cwd),
            "exit_code": completed.returncode,
            "expected": "nonzero explicit refusal" if refusal else 0,
            "status": "PASS" if good else "FAIL",
            "stdout_sha256": digest(out),
            "stderr_sha256": digest(err),
        }
        rows.append(row)
        save()
        print(name + ": " + row["status"], flush=True)
        return good

    skip_accepted = args.skip_accepted_r12
    result["accepted_storage_and_catalogs_repeated"] = not skip_accepted
    captured = work / "api-source"
    if skip_accepted:
        headers = Path(
            "/home/macromodel/Documents/uwvm-test-261018/agents/09/examples-light-r12/work/api-source/rosidl-buffer/files/rosidl_buffer/include"
        )
        assert headers.is_dir()
        command(
            "retained-buffer-header-readback",
            [
                sys.executable,
                "-I",
                "-S",
                "-B",
                nvidia / "isaac_ros/native_cpu/verify_api.py",
                "--rosidl-include",
                headers,
                "--output",
                work / "api-readback.json",
            ],
        )
    elif command(
        "genuine-buffer-source",
        [
            sys.executable,
            "-B",
            nvidia / "capture_public_sources.py",
            "--enable-source-download",
            "--lane",
            "rosidl-buffer",
            "--scope",
            "core",
            "--transport",
            "official-api",
            "--out",
            captured,
        ],
    ):
        headers = captured / "rosidl-buffer/files/rosidl_buffer/include"
        verified = command(
            "buffer-header-readback",
            [
                sys.executable,
                "-I",
                "-S",
                "-B",
                nvidia / "isaac_ros/native_cpu/verify_api.py",
                "--rosidl-include",
                headers,
                "--output",
                work / "api-readback.json",
            ],
        )
        if verified and command(
            "genuine-buffer-storage-build",
            [
                "/usr/bin/clang-21",
                "--driver-mode=g++",
                "-std=c++20",
                "-O2",
                "-Wall",
                "-Wextra",
                "-Werror",
                "-pedantic-errors",
                "-I",
                headers,
                tests / "isaac_ros/qualification/buffer_storage_contract.cpp",
                "-o",
                work / "buffer-storage",
            ],
        ):
            command("genuine-buffer-storage-run", [work / "buffer-storage"])
    else:
        headers = None
    shutil.copytree(source / "sdk/include", sdk / "include")
    shutil.copyfile(source / "sdk/xmake.lua", sdk / "xmake.lua")
    shutil.copytree(origin / "tools/xmake", sdk / "tools/xmake")
    for name in ("wasm_profiles.py", "resource_contract.py"):
        shutil.copyfile(origin / "tools" / name, sdk / "tools" / name)
    (sdk / "example").mkdir()
    shutil.copyfile(source / "sdk/example/xmake.lua", sdk / "example/xmake.lua")
    definition = json.loads((nvidia / "examples/delivery.json").read_bytes())

    def deliver(selected, platform, language, name):
        target = sdk / "example/platform" / platform / language / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(origin / selected, target)

    for item in definition["common"]:
        for platform, language in item["recipients"]:
            deliver(item["source"], platform, language, item["name"])
    for item in definition["files"]:
        deliver(item["source"], item["platform"], item["language"], item["name"])
    for item in definition["trees"]:
        base = origin / item["source"]
        for selected in sorted(base.rglob("*")):
            if selected.is_file():
                deliver(
                    selected.relative_to(origin),
                    item["platform"],
                    item["language"],
                    Path(item["name"]) / selected.relative_to(base),
                )
    xmake = Path("/home/macromodel/.local/bin/xmake")
    env = dict(os.environ, XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake")
    result["xmake"] = {"path": str(xmake), "sha256": digest(xmake)}
    selected = [
        ("nvidia-isaac-ros", "c", "cpu-image-values"),
        ("nvidia-isaac-ros", "cpp", "raw-default-denial"),
        ("nvidia-isaac-sim", "cpp", "telemetry-default-denial"),
        ("nvidia-isaac-sim", "py", "native-sensor-owner"),
        ("nvidia-groot", "py", "native-inference-owner"),
        ("nvidia-groot", "tinygo", "default-denial"),
    ]

    def configure(platform, language, example, label):
        return command(
            label,
            [
                xmake,
                "f",
                "--sdk-example-platform=" + platform,
                "--sdk-example-language=" + language,
                "--sdk-example=" + example,
                "--sdk-python=/usr/bin/python3",
            ],
            cwd=sdk / "example",
            env=env,
        )

    catalogs = []
    for index, (platform, language, example) in enumerate(
        [] if skip_accepted else selected
    ):
        label = "catalog-" + str(index)
        if configure(platform, language, example, label + "-configure"):
            if command(
                label + "-list",
                [xmake, "sdk-example-list"],
                cwd=sdk / "example",
                env=env,
            ):
                lines = (work / (label + "-list.stdout")).read_text().splitlines()
                catalogs.append(
                    {"platform": platform, "language": language, "lines": lines}
                )
    result["loaded_catalogs"] = catalogs
    # Selected source syntax is checked without importing ROS/Kit/model modules.
    python_sources = []
    for selected_source in sorted(nvidia.rglob("*.py")):
        compile(selected_source.read_bytes(), str(selected_source), "exec")
        python_sources.append(
            {
                "path": str(selected_source.relative_to(nvidia)),
                "sha256": digest(selected_source),
            }
        )
    result["host_syntax_only"] = {
        "python": sys.version,
        "files": python_sources,
        "native_python312_credit": 0,
    }
    python = Path(sys.executable)
    config = work / "wrong-python.json"
    config.write_text(
        json.dumps(
            {
                "format": "uwvm-nvidia-example-config-v1",
                "python": {"path": str(python), "sha256": digest(python)},
            }
        )
        + "\n"
    )
    env["UWVM_NVIDIA_EXAMPLE_CONFIG"] = str(config)
    if configure(
        "nvidia-groot", "py", "native-inference-owner", "python-refusal-configure"
    ):
        refused = command(
            "python312-substitution-refused",
            [
                xmake,
                "sdk-example-build",
                "--output=" + str(work / "wrong-python-build"),
            ],
            cwd=sdk / "example",
            env=env,
            refusal=True,
        )
        original = work / "wrong-python-build/backend/python-compile.stderr"
        result["actual_wrong_version_backend_reached"] = bool(
            refused and original.is_file() and "CPython 3.12" in original.read_text()
        )
    if headers:
        cmake = Path("/usr/bin/cmake")
        command(
            "actual-native-dependency-probe",
            [
                cmake,
                "-S",
                nvidia / "isaac_ros/native_cpu",
                "-B",
                work / "native-probe",
                "-DUWVM_CPU_SENSOR_INCLUDE=" + str(nvidia / "isaac_ros/include"),
                "-DUWVM_ROSIDL_BUFFER_INCLUDE=" + str(headers),
                "-DPython3_EXECUTABLE=" + str(python),
            ],
            refusal=True,
        )
        diagnostic = (work / "actual-native-dependency-probe.stderr").read_text()
        result["real_native_dependency_diagnostic"] = (
            "ament_cmake" if "ament_cmake" in diagnostic else diagnostic[-2048:]
        )
    result["native_exchange_status"] = (
        "NOT_RUN: captured Buffer headers are not an actual installed ROS native type/DSO/RMW closure"
    )
    result["full_xmake_guest_build_status"] = (
        "NOT_RUN: examples-r11 refused finite disk admission before launch; exact larger recipe retained"
    )
    result["passed"] = sum(row["status"] == "PASS" for row in rows)
    result["denominator"] = len(rows)
    result["status"] = (
        "PASS"
        if result["passed"] == result["denominator"]
        and result.get("actual_wrong_version_backend_reached")
        else "FAILED_RETAINED"
    )
    assert digest(args.source_archive) == source_digest
    save()
    return 0 if result["status"] == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
