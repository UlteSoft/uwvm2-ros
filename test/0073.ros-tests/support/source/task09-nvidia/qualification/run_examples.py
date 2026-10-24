"""Controlled selected xmake/backend qualification; no hardware/simulator/model.

Inputs contain exact current owned sources and current shared adapters. No
private shared-code candidate is overlaid onto an otherwise current SDK.
Reproducible copied SDK dependencies are removed after their read-back inventory;
original historical inputs, new sources, logs, artifacts and receipts remain.
"""

import gzip
import hashlib
import json
import os
from pathlib import Path
import re
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
    with tarfile.open(run / "inputs/source.tar") as archive:
        members = [
            m
            for m in archive
            if not any(part.startswith("._") for part in Path(m.name).parts)
        ]
        archive.extractall(source, members=members, filter="data")
    origin = source / "sdk/origin"
    nvidia = origin / "robots/nvidia"
    tests = source / "test/0073.ros-tests/support/source/task09-nvidia"
    sdk = work / "installed-sdk"
    sdk.mkdir()
    prior = Path(
        "/home/macromodel/Documents/uwvm-test-261018/agents/01/cpp-matrix-r9/work/matrix/sdk/include"
    )
    wasi = Path(
        "/home/macromodel/Documents/uwvm-test-261018/agents/10/staged-wasi-r1/staged/wasi-sdk"
    )
    runtime = Path(
        "/home/macromodel/Documents/uwvm-test-261018/agents/10/runtime-denial-r2/work/uwvm-dual"
    )
    assert (
        digest(runtime)
        == "0b8b7e3ce2d833675a92e6d857008e9a146615c8c067b9277827dabee83e6556"
    )
    rows = []
    tools = {}
    summary = {
        "format": "uwvm-task09-xmake-example-acceptance-v1",
        "cases": rows,
        "scope": "Selected installed source assembly, exact current NVIDIA sources and shared adapters",
        "public_distribution_accepted": False,
        "native_ros_exchanges": 0,
        "model_calls": 0,
        "simulator_runs": 0,
        "guest_successes": 0,
        "default_authority": [],
        "runtime_sha256": digest(runtime),
    }
    result_file = work / "examples-result.json"

    def save():
        result_file.write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n")

    def command(name, argv, cwd=work, expected=0, env=None, timeout=180):
        argv = [str(value) for value in argv]
        out = work / (name + ".stdout")
        err = work / (name + ".stderr")
        with out.open("wb") as stdout, err.open("wb") as stderr:
            result = subprocess.run(
                argv,
                cwd=cwd,
                env=env,
                stdin=subprocess.DEVNULL,
                stdout=stdout,
                stderr=stderr,
                timeout=timeout,
            )
        good = (
            result.returncode == expected
            if expected is not None
            else result.returncode != 0
        )
        row = {
            "case": name,
            "argv": argv,
            "cwd": str(cwd),
            "exit_code": result.returncode,
            "expected": expected
            if expected is not None
            else "nonzero explicit refusal",
            "status": "PASS" if good else "FAIL",
            "stdout": out.name,
            "stderr": err.name,
            "stdout_sha256": digest(out),
            "stderr_sha256": digest(err),
        }
        rows.append(row)
        save()
        print(name + ": " + row["status"], flush=True)
        return good

    try:
        # Original admitted inputs remain read-only; only this owned copy changes.
        if prior.is_dir():
            shutil.copytree(prior, sdk / "include")
        else:
            # Task10 may remove an archived reproducible work copy to restore
            # space. Restore its exact regular-file inputs inside our own run;
            # never rewrite another task's historical work or archive.
            archive_root = prior.parents[3]
            retained = archive_root / "resource-archives/evidence.tar"
            verification = json.loads(
                (archive_root / "resource-archives/verified-copies.json").read_bytes()
            )
            assert digest(retained) == verification["sha256"]
            count = 0
            with tarfile.open(retained) as archive:
                for member in archive:
                    prefix = "work/matrix/sdk/include/"
                    if not member.isfile() or not member.name.startswith(prefix):
                        continue
                    relative = Path(member.name[len(prefix) :])
                    assert not relative.is_absolute() and ".." not in relative.parts
                    target = sdk / "include" / relative
                    target.parent.mkdir(parents=True, exist_ok=True)
                    with (
                        archive.extractfile(member) as captured,
                        target.open("xb") as output,
                    ):
                        shutil.copyfileobj(captured, output)
                    count += 1
            assert count > 0 and digest(retained) == verification["sha256"]
            summary["reference_sdk_recovered_from_verified_archive"] = str(retained)
        copied = {
            str(p.relative_to(sdk)): {"bytes": p.stat().st_size, "sha256": digest(p)}
            for p in sorted((sdk / "include").rglob("*"))
            if p.is_file()
        }
        (work / "copied-dependency-inventory.json").write_text(
            json.dumps(
                {
                    "original_root": str(prior),
                    "files": copied,
                    "originals_modified": False,
                },
                indent=2,
                sort_keys=True,
            )
            + "\n"
        )
        # Historical provider/fast_io inputs remain immutable. Only the private
        # installed candidate is rebound to current canonical authored headers.
        foundation = sdk / "include/wasm/language/c++/include"
        foundation.mkdir(parents=True, exist_ok=True)
        provider_relocations = []
        for selected in sorted((sdk / "include").glob("fast_io*")):
            target = foundation / selected.name
            assert not target.exists(), "Duplicate fast_io provider roots refused"
            provider_relocations.append({"before": str(selected), "after": str(target)})
            selected.rename(target)
        # Only this private historical copy is retired. The current selected
        # libc++ headers below come from the same genuine SDK33 no-EH provider
        # as its libraries; immutable reference/archive/provider bytes stay put.
        if (sdk / "include/c++").exists():
            shutil.rmtree(sdk / "include/c++")
        if (foundation / "c++").exists():
            shutil.rmtree(foundation / "c++")
        (work / "provider-path-relocations.json").write_text(
            json.dumps(provider_relocations, indent=2) + "\n"
        )
        for alias in ("roswasm", "roswasm_unitree"):
            selected = sdk / "include" / alias
            if selected.is_symlink():
                selected.unlink()
            elif selected.exists():
                shutil.rmtree(selected)
        for platform in ("ros", "ros1", "ros2"):
            for language in ("c", "cpp"):
                selected = source / "sdk/include/platform" / platform / language
                assert selected.is_dir(), "Fresh canonical shared SDK headers required"
                destination = sdk / "include/platform" / platform / language
                if destination.exists():
                    shutil.rmtree(destination)
                shutil.copytree(selected, destination)
        shutil.copyfile(source / "sdk/xmake.lua", sdk / "xmake.lua")
        shutil.copytree(origin / "tools/xmake", sdk / "tools/xmake")
        for name in ("wasm_profiles.py", "resource_contract.py"):
            shutil.copyfile(origin / "tools" / name, sdk / "tools" / name)
        (sdk / "example").mkdir()
        shutil.copyfile(source / "sdk/example/xmake.lua", sdk / "example/xmake.lua")
        # Existing application source semantics, plus exact new local recipes.
        for platform, lane, files in (
            (
                "nvidia-isaac-ros",
                "isaac_ros",
                ("telemetry.cpp", "raw_denied.cpp", "full_values.hpp", "basic_io.hpp"),
            ),
            (
                "nvidia-isaac-sim",
                "isaac_sim",
                ("telemetry.cpp", "complete_joint_command.cpp", "basic_io.hpp"),
            ),
        ):
            destination = sdk / "example/platform" / platform / "cpp"
            destination.mkdir(parents=True)
            for name in files:
                shutil.copyfile(nvidia / lane / "examples" / name, destination / name)
        definition = json.loads((nvidia / "examples/delivery.json").read_bytes())

        def deliver(source_path, platform, language, name):
            destination = sdk / "example/platform" / platform / language / name
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(origin / source_path, destination)

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
        for platform, family in (
            ("nvidia-isaac-ros", "isaac-ros-complete"),
            ("nvidia-isaac-sim", "isaac-sim-complete"),
        ):
            for language in ("c", "cpp"):
                selected = (
                    source
                    / "sdk/include/platform"
                    / platform
                    / language
                    / "ros2"
                    / family
                )
                destination = (
                    sdk / "include/platform" / platform / language / "ros2" / family
                )
                shutil.copytree(selected, destination, dirs_exist_ok=True)
            destination = sdk / "share/platform" / platform / "ros2" / family
            destination.mkdir(parents=True)
            shutil.copyfile(
                source
                / "sdk/share/platform"
                / platform
                / "ros2"
                / family
                / "generation.vendor.json",
                destination / "generation.vendor.json",
            )
        for language in ("c", "cpp"):
            selected = sdk / "include/platform/nvidia-isaac-ros" / language
            selected.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(
                nvidia / "isaac_ros/include/cpu_sensor_contract.h",
                selected / "cpu_sensor_contract.h",
            )
        # Real fixed WASI SDK closure, copied only into this run's SDK.
        sysroot = sdk / "toolchains/wasi33/sysroot"
        resource = sdk / "toolchains/wasi33/resource"
        shutil.copytree(
            wasi / "share/wasi-sysroot/include/wasm32-wasip1",
            sysroot / "include/wasm32-wasip1",
            ignore=shutil.ignore_patterns("eh", "noeh", "c++"),
        )
        shutil.copytree(
            wasi / "share/wasi-sysroot/include/wasm32-wasip1/noeh/c++/v1",
            foundation / "c++/v1",
        )
        assert not (sdk / "include/fast_io.h").exists()
        assert not (sdk / "include/c++").exists()
        assert (foundation / "fast_io.h").is_file()
        assert (foundation / "c++/v1/__config_site").is_file()
        shutil.copytree(wasi / "lib/clang/22/include", resource / "include")
        for name in ("libc.a", "libm.a", "crt1.o", "crt1-command.o", "crt1-reactor.o"):
            target = sysroot / "lib/wasm32-wasip1" / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(
                wasi / "share/wasi-sysroot/lib/wasm32-wasip1" / name, target
            )
        noeh = sysroot / "lib/wasm32-wasip1/noeh"
        noeh.mkdir()
        for name in ("libc++.a", "libc++abi.a"):
            shutil.copyfile(
                wasi / "share/wasi-sysroot/lib/wasm32-wasip1/noeh" / name, noeh / name
            )
        builtins = resource / "lib/wasm32-unknown-wasip1/libclang_rt.builtins.a"
        builtins.parent.mkdir(parents=True)
        shutil.copyfile(
            wasi / "lib/clang/22/lib/wasm32-unknown-wasip1/libclang_rt.builtins.a",
            builtins,
        )
        # Inspect the whole selected library/startup closure, including libm
        # and every copied CRT object. A four-archive subset cannot establish
        # the provider's complete no-EH library/startup qualification flag.
        archives = sorted(
            path
            for root in (sysroot, resource)
            for path in root.rglob("*")
            if path.is_file() and path.suffix in {".a", ".o"}
        )
        assert archives
        inspections = []
        for index, archive in enumerate(archives):
            argv = [
                str(wasi / "bin/llvm-objdump"),
                "-d",
                "--no-show-raw-insn",
                "--no-leading-addr",
                str(archive),
            ]
            h = hashlib.sha256()
            forbidden = []
            size = 0
            with (
                (work / ("closure-" + str(index) + ".stderr")).open("wb") as stderr,
                gzip.open(
                    work / ("closure-" + str(index) + ".txt.gz"), "wb"
                ) as captured,
            ):
                process = subprocess.Popen(argv, stdout=subprocess.PIPE, stderr=stderr)
                for line in process.stdout:
                    h.update(line)
                    captured.write(line)
                    size += len(line)
                    if re.match(
                        rb"^\s*(?:[0-9a-f]+:\s*)?(try|try_table|catch|catch_all|throw|throw_ref|rethrow|delegate)(?:\s|$)",
                        line,
                    ):
                        forbidden.append(line.decode().strip())
                code = process.wait(timeout=90)
            inspections.append(
                {
                    "argv": argv,
                    "input_bytes": archive.stat().st_size,
                    "input_sha256": digest(archive),
                    "exit_code": code,
                    "output_bytes": size,
                    "output_sha256": h.hexdigest(),
                    "forbidden_eh": forbidden,
                }
            )
        assert all(
            row["exit_code"] == 0 and not row["forbidden_eh"] for row in inspections
        )
        (work / "closure-inspection.json").write_text(
            json.dumps(inspections, indent=2) + "\n"
        )

        def pin(root, p):
            return {
                "path": p.relative_to(root).as_posix(),
                "bytes": p.stat().st_size,
                "sha256": digest(p),
            }

        files = [
            pin(sdk, p)
            for directory in (sysroot, resource, foundation)
            for p in sorted(directory.rglob("*"))
            if p.is_file()
        ]
        profile = {
            "target": "wasm32-wasip1",
            "address_bits": 32,
            "wasi": "wasip1",
            "exceptions": False,
            "threads": False,
            "startup": "command",
            "core_baseline": "3.0",
        }
        compile_flags = [
            "--no-default-config",
            "--target={target}",
            "--sysroot={sysroot}",
            "-resource-dir={resource_dir}",
            "-fno-exceptions",
            "-mthread-model",
            "single",
            "-nostdinc",
            "-isystem{resource_dir}/include",
            "-isystem{sysroot}/include/wasm32-wasip1",
        ]
        provider = {
            "toolchain_id": "actual-wasi-sdk33-task10-r1",
            "source_ready": True,
            "compiled": False,
            "runtime_tested": False,
            "exception_library_closure_verified": True,
            "driver": "clang",
            "supports_clang_config": True,
            "compiler": pin(wasi, wasi / "bin/clang++"),
            "linker": pin(wasi, wasi / "bin/wasm-ld"),
            "compiler_argv": compile_flags
            + [
                "-fno-rtti",
                "-nostdinc++",
                # These SDK-local paths resolve from the selected C sysroot
                # back to the single canonical public C++ foundation. The
                # resolver's documented substitutions do not include sdk_root.
                "-isystem{sysroot}/../../../include/wasm/language/c++/include/c++/v1",
                "-I{sysroot}/../../../include/wasm/language/c++/include",
            ],
            "linker_argv": [
                "--ld-path={linker}",
                "-nostdlib++",
                "-L{sysroot}/lib/wasm32-wasip1/noeh",
                "-lc++",
                "-lc++abi",
            ],
            "library_modes": ["libc++"],
            "library_standards": ["c++20", "c++23", "c++2c"],
        }
        cprovider = dict(
            provider,
            compiler=pin(wasi, wasi / "bin/clang"),
            compiler_argv=compile_flags,
            linker_argv=["--ld-path={linker}"],
            library_modes=["libc"],
            library_standards=["c99", "c11", "c17", "c23"],
        )
        manifest = {
            "format": "uwvm-sdk-wasm-profiles-v1",
            "defaults": profile,
            "profiles": [
                {
                    "id": "task09-selected-real-wasi33-noeh-single-command",
                    "profile": profile,
                    "toolchain_id": provider["toolchain_id"],
                    "sysroot": sysroot.relative_to(sdk).as_posix(),
                    "resource_dir": resource.relative_to(sdk).as_posix(),
                    "files": files,
                    "library_abi": {
                        "address_bits": 32,
                        "wasi_pointer_abi": "wasip1-memory32",
                        "exceptions": False,
                        "exception_encoding": "none",
                        "threads": False,
                        "thread_abi": "none",
                        "shared_memory": False,
                        "required_files": [
                            p["path"] for p in files if p["path"].endswith((".a", ".o"))
                        ],
                    },
                    "runtime_requirements": [],
                    "languages": {"cpp": provider, "c": cprovider},
                }
            ],
        }
        (sdk / "share").mkdir(exist_ok=True)
        (sdk / "share/wasm-profiles.json").write_text(
            json.dumps(manifest, indent=2) + "\n"
        )
        cmake = Path("/usr/bin/cmake")
        configuration = work / "cmake.json"
        configuration.write_text(
            json.dumps(
                {
                    "format": "uwvm-nvidia-example-config-v1",
                    "cmake": {"path": str(cmake), "sha256": digest(cmake)},
                }
            )
            + "\n"
        )
        xmake = Path("/home/macromodel/.local/bin/xmake")
        env = dict(
            os.environ,
            XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake",
            UWVM_NVIDIA_EXAMPLE_CONFIG=str(configuration),
        )
        tools.update(
            xmake={"path": str(xmake), "sha256": digest(xmake)},
            cmake={"path": str(cmake), "sha256": digest(cmake)},
        )
        summary["tool_pins"] = tools
        # Genuine upstream CPU storage is useful independently of missing ROS.
        captured = work / "buffer-api-source"
        if command(
            "buffer-source-capture",
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
                "buffer-api-readback",
                [
                    sys.executable,
                    "-I",
                    "-S",
                    "-B",
                    nvidia / "isaac_ros/native_cpu/verify_api.py",
                    "--rosidl-include",
                    headers,
                    "--output",
                    work / "buffer-api-readback.json",
                ],
            )
            if verified and command(
                "buffer-storage-build",
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
                command("buffer-storage-run", [work / "buffer-storage"])
            # A real find_package attempt, never generated replacement ROS types.
            absent = command(
                "native-buffer-dependency-probe",
                [
                    cmake,
                    "-S",
                    nvidia / "isaac_ros/native_cpu",
                    "-B",
                    work / "native-buffer-probe",
                    "-DUWVM_CPU_SENSOR_INCLUDE="
                    + str(sdk / "include/platform/nvidia-isaac-ros/cpp"),
                    "-DUWVM_ROSIDL_BUFFER_INCLUDE=" + str(headers),
                    "-DPython3_EXECUTABLE=/usr/bin/python3",
                ],
                expected=None,
            )
            summary["native_dependency_probe_refused"] = absent
            summary["captured_api_is_installed_native_sdk"] = False
        command("xmake-version", [xmake, "--version"], env=env)

        def configure(platform, language, example, label):
            return command(
                label,
                [
                    xmake,
                    "f",
                    "--ros-sdk-platforms=" + platform,
                    "--ros-sdk-languages=" + language,
                    "--sdk-example-platform=" + platform,
                    "--sdk-example-language=" + language,
                    "--sdk-example=" + example,
                    "--ros-sdk-cpp-standard=20",
                    "--ros-sdk-c-standard=99",
                    "--sdk-target=wasm32-wasip1",
                    "--sdk-startup=command",
                    "--sdk-exceptions=n",
                    "--sdk-threads=n",
                    "--sdk-offline=y",
                    "--sdk-toolchain=" + str(wasi),
                    "--sdk-python=/usr/bin/python3",
                ],
                cwd=sdk / "example",
                env=env,
            )

        for index, (platform, language, example) in enumerate(
            [
                ("nvidia-isaac-ros", "cpp", "raw-default-denial"),
                ("nvidia-isaac-ros", "cpp", "telemetry-default-denial"),
                ("nvidia-isaac-sim", "cpp", "telemetry-default-denial"),
                ("nvidia-isaac-sim", "cpp", "joint-command-default-denial"),
                ("nvidia-isaac-ros", "c", "cpu-image-values"),
            ]
        ):
            label = "example-" + str(index)
            out = work / label
            if not configure(platform, language, example, label + "-configure"):
                continue
            command(
                label + "-list",
                [xmake, "sdk-example-list"],
                cwd=sdk / "example",
                env=env,
            )
            if command(
                label + "-build",
                [xmake, "sdk-example-build", "--output=" + str(out)],
                cwd=sdk / "example",
                env=env,
            ):
                actual = json.loads((out / "build-receipt.json").read_bytes())
                summary.setdefault("artifacts", []).append(actual)
                for number, artifact in enumerate(actual["artifacts"]):
                    path = Path(artifact["path"])
                    assert path.stat().st_size == artifact["bytes"]
                    assert digest(path) == artifact["sha256"]
                    command(
                        label + "-validate-" + str(number),
                        ["/usr/bin/wasm-validate", path],
                    )
                    command(
                        label + "-imports-" + str(number),
                        ["/usr/bin/wasm-objdump", "-x", path],
                    )
                good = command(
                    label + "-guest",
                    [
                        xmake,
                        "sdk-example-run",
                        "--receipt=" + str(out / "build-receipt.json"),
                        "--runtime=" + str(runtime),
                    ],
                    cwd=sdk / "example",
                    env=env,
                    timeout=45,
                )
                if good:
                    summary["guest_successes"] += 1
        # No native simulator/model substitution. Actual absence stays explicit.
        summary["native_owner_dependencies"] = (
            "NOT_RUN: actual Lyrical Buffer ROS install, CPython3.12/Kit/GR00T environment, licensed weights and suitable inference GPU not established"
        )
        summary["status"] = (
            "PASS"
            if rows and all(row["status"] == "PASS" for row in rows)
            else "FAILED_RETAINED"
        )
        save()
    finally:
        # These are reproducible copies of retained admitted originals and the
        # exact source.tar input, not historical evidence or test outputs.
        for directory in (sdk / "include", sdk / "toolchains"):
            if directory.exists():
                shutil.rmtree(directory)
        summary["reproducible_dependency_copies_removed"] = True
        save()
    return 0 if summary.get("status") == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
