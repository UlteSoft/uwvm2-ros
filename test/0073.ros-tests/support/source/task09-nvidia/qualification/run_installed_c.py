"""Build the real installed NVIDIA CPU image example through xmake.

Only the selected C toolchain closure is copied. C++/EH providers are excluded
because this run selects C99, no EH, no threads, and command startup explicitly.
The installed SDK contains no origin tree, repository tests or root ROS aliases.
All execution is owned by the unchanged shared Linux finite resource owner.
"""

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


def pin(root, path):
    return {
        "path": path.relative_to(root).as_posix(),
        "bytes": path.stat().st_size,
        "sha256": digest(path),
    }


def main():
    assert os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") == "1"
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    work = run / "work"
    source = work / "inputs-expanded"
    source.mkdir()
    with tarfile.open(run / "inputs/source.tar") as archive:
        members = [
            item
            for item in archive
            if not any(part.startswith("._") for part in Path(item.name).parts)
        ]
        archive.extractall(source, members=members, filter="data")
    sdk = work / "relocated-sdk"
    sdk.mkdir()
    origin = source / "sdk/origin"
    nvidia = origin / "robots/nvidia"
    shutil.copyfile(source / "sdk/xmake.lua", sdk / "xmake.lua")
    shutil.copytree(origin / "tools/xmake", sdk / "tools/xmake")
    for name in ("wasm_profiles.py", "resource_contract.py"):
        shutil.copyfile(origin / "tools" / name, sdk / "tools" / name)
    for platform in ("ros", "ros2"):
        shutil.copytree(
            source / "sdk/include/platform" / platform / "c",
            sdk / "include/platform" / platform / "c",
        )
    public = sdk / "include/platform/nvidia-isaac-ros/c"
    public.mkdir(parents=True)
    shutil.copyfile(
        nvidia / "isaac_ros/include/cpu_sensor_contract.h",
        public / "cpu_sensor_contract.h",
    )
    examples = sdk / "example"
    examples.mkdir()
    shutil.copyfile(source / "sdk/example/xmake.lua", examples / "xmake.lua")
    app = examples / "platform/nvidia-isaac-ros/c"
    shutil.copytree(nvidia / "isaac_ros/c_examples", app)
    shutil.copyfile(nvidia / "examples/build_backend.lua", app / "build_backend.lua")
    shutil.copyfile(nvidia / "examples/README.md", app / "README.xmake.md")
    for name in (
        "API-DOCUMENTATION.md",
        "OFFICIAL-PROGRAMS.md",
        "PROGRAM-LANGUAGE-LEDGER.md",
    ):
        shutil.copyfile(nvidia / name, app / name)
    source_pins = [pin(sdk, path) for path in sorted(sdk.rglob("*")) if path.is_file()]
    # Only inert source.tar remains in inputs. Compiler/run arguments cannot
    # accidentally fall back to the expanded maintenance source or its tests.
    shutil.rmtree(source)
    assert not any((sdk / name).exists() for name in ("origin", "test", "tests"))
    assert not (sdk / "include/roswasm").exists()
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
    sysroot = sdk / "toolchains/wasi33/sysroot"
    resource = sdk / "toolchains/wasi33/resource"
    shutil.copytree(
        wasi / "share/wasi-sysroot/include/wasm32-wasip1",
        sysroot / "include/wasm32-wasip1",
        ignore=shutil.ignore_patterns("eh", "noeh", "c++"),
    )
    shutil.copytree(wasi / "lib/clang/22/include", resource / "include")
    for name in ("libc.a", "libm.a", "crt1.o", "crt1-command.o"):
        path = sysroot / "lib/wasm32-wasip1" / name
        path.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(wasi / "share/wasi-sysroot/lib/wasm32-wasip1" / name, path)
    builtins = resource / "lib/wasm32-unknown-wasip1/libclang_rt.builtins.a"
    builtins.parent.mkdir(parents=True)
    shutil.copyfile(
        wasi / "lib/clang/22/lib/wasm32-unknown-wasip1/libclang_rt.builtins.a", builtins
    )
    files = [
        pin(sdk, path)
        for root in (sysroot, resource)
        for path in sorted(root.rglob("*"))
        if path.is_file()
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
    provider = {
        "toolchain_id": "actual-wasi-sdk33-task10-staged-c",
        "source_ready": True,
        "compiled": False,
        "runtime_tested": False,
        # This small workload pins the real archives and validates/runs its
        # final artifact; it does not disassemble every archive member.
        "exception_library_closure_verified": False,
        "driver": "clang",
        "supports_clang_config": True,
        "compiler": pin(wasi, wasi / "bin/clang"),
        "linker": pin(wasi, wasi / "bin/wasm-ld"),
        "compiler_argv": [
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
        ],
        "linker_argv": ["--ld-path={linker}"],
        "library_modes": ["libc"],
        "library_standards": ["c99", "c11", "c17", "c23"],
    }
    manifest = {
        "format": "uwvm-sdk-wasm-profiles-v1",
        "defaults": profile,
        "profiles": [
            {
                "id": "task09-real-wasi33-c-noeh-single-command",
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
                        item["path"]
                        for item in files
                        if item["path"].endswith((".a", ".o"))
                    ],
                },
                "runtime_requirements": [],
                "languages": {"c": provider},
            }
        ],
    }
    (sdk / "share").mkdir()
    (sdk / "share/wasm-profiles.json").write_text(json.dumps(manifest, indent=2) + "\n")
    xmake = Path("/home/macromodel/.local/bin/xmake")
    environment = dict(
        os.environ, XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake"
    )
    rows = []
    result = {
        "schema": "task09-installed-c-xmake-v1",
        "cases": rows,
        "source_pins": source_pins,
        "copied_toolchain_pins": files,
        "source_archive_sha256": digest(run / "inputs/source.tar"),
        "installed_sdk": str(sdk),
        "default_authority": [],
        "public_distribution_accepted": False,
        "native_ros_exchanges": 0,
        "official_program_ports": 0,
        "successful_guest_executions": 0,
        "runtime": pin(runtime.parent, runtime),
        "xmake": pin(xmake.parent, xmake),
    }

    def save():
        (work / "installed-c-result.json").write_text(
            json.dumps(result, indent=2) + "\n"
        )

    def command(name, argv, cwd=work):
        argv = [str(value) for value in argv]
        with (
            (work / (name + ".stdout")).open("wb") as out,
            (work / (name + ".stderr")).open("wb") as err,
        ):
            completed = subprocess.run(
                argv,
                cwd=cwd,
                env=environment,
                stdin=subprocess.DEVNULL,
                stdout=out,
                stderr=err,
                timeout=120,
            )
        row = {
            "case": name,
            "argv": argv,
            "cwd": str(cwd),
            "exit_code": completed.returncode,
            "stdout_sha256": digest(work / (name + ".stdout")),
            "stderr_sha256": digest(work / (name + ".stderr")),
            "status": "PASS" if completed.returncode == 0 else "FAIL",
        }
        rows.append(row)
        save()
        print(name + ": " + row["status"], flush=True)
        return completed.returncode == 0

    try:
        command("xmake-version", [xmake, "--version"])
        command("clang-version", [wasi / "bin/clang", "--version"])
        command("runtime-version", [runtime, "--version"])
        output = work / "cpu-image-build"
        arguments = [
            xmake,
            "f",
            "--ros-sdk-platforms=nvidia-isaac-ros",
            "--ros-sdk-languages=c",
            "--sdk-example-platform=nvidia-isaac-ros",
            "--sdk-example-language=c",
            "--sdk-example=cpu-image-values",
            "--ros-sdk-c-standard=99",
            "--sdk-target=wasm32-wasip1",
            "--sdk-startup=command",
            "--sdk-exceptions=n",
            "--sdk-threads=n",
            "--sdk-offline=y",
            "--sdk-toolchain=" + str(wasi),
            "--sdk-python=/usr/bin/python3",
        ]
        if command("configure", arguments, examples) and command(
            "build", [xmake, "sdk-example-build", "--output=" + str(output)], examples
        ):
            artifact = output / "cpu-image-values.wasm"
            result["artifact"] = pin(output, artifact)
            command("wasm-validation", ["/usr/bin/wasm-validate", artifact])
            command("wasm-imports", ["/usr/bin/wasm-objdump", "-x", artifact])
            if command(
                "guest",
                [
                    xmake,
                    "sdk-example-run",
                    "--receipt=" + str(output / "build-receipt.json"),
                    "--runtime=" + str(runtime),
                ],
                examples,
            ):
                result["successful_guest_executions"] = 1
                text = (work / "guest.stdout").read_text()
                expected = [
                    255,
                    0,
                    0,
                    0,
                    255,
                    0,
                    0,
                    0,
                    255,
                    91,
                    92,
                    93,
                    255,
                    255,
                    255,
                    127,
                    127,
                    127,
                    0,
                    0,
                    0,
                    94,
                    95,
                    96,
                ]
                result["complete_output_verified"] = (
                    "sec=-1 nanosec=999999999 frame_id=cpu_optical height=2 width=3 encoding=rgb8 bigendian=0 step=12 bytes=24\n"
                    in text
                    and all(
                        str(index) + ":" + str(value) + "\n" in text
                        for index, value in enumerate(expected)
                    )
                )
                assert result["complete_output_verified"]
        result["status"] = (
            "PASS"
            if rows and all(row["status"] == "PASS" for row in rows)
            else "FAILED_RETAINED"
        )
        save()
    finally:
        # Exact sources, artifacts, inventories, command logs and manifest remain.
        # Provider inputs are immutable in the original staged toolchain.
        shutil.rmtree(sdk / "toolchains")
    return 0 if result.get("status") == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
