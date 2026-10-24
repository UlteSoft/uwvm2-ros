"""Build three native examples using only a relocated SDK and pinned providers."""

import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def sha(path):
    with Path(path).open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def pins(root):
    return [
        dict(
            path=str(path.relative_to(root)),
            bytes=path.stat().st_size,
            sha256=sha(path),
        )
        for path in sorted(root.rglob("*"))
        if path.is_file()
    ]


def main():
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("common finite owner is required")
    inputs = Path(sys.argv[1]).resolve(strict=True)
    selection = sys.argv[2] if len(sys.argv) == 3 else "all"
    if selection not in ("all", "py", "cpp"):
        raise ValueError("select all, the two Python components, or the C++ exporter")
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    sdk = work / "relocated-sdk"
    shutil.copytree(inputs / "sdk", sdk, ignore=shutil.ignore_patterns("origin"))
    # The serialized whole-SDK producer has not published the current shared
    # example registrar yet. Integrate its genuine source in this private
    # installed candidate, recording exact bytes rather than inventing a stub.
    integration = inputs / "sdk/origin/tools/xmake/examples"
    for name in ("register.lua", "engine.lua"):
        destination = sdk / "tools/xmake/examples" / name
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(integration / name, destination)
    assert not (sdk / "origin").exists() and not (sdk / "test").exists()
    agents = Path("/home/macromodel/Documents/uwvm-test-261018/agents/08")
    # Materialize this genuine third-party dependency in the installed SDK.
    # The build never reads the old source checkout from which it was retained.
    fast_io = (
        agents / "20261021-contracts-r6/inputs/source/third-parties/fast_io/include"
    )
    for item in fast_io.iterdir() if selection != "py" else ():
        destination = sdk / "include" / item.name
        if item.is_dir():
            shutil.copytree(item, destination)
        else:
            shutil.copyfile(item, destination)
    tools = Path("/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm")
    cxx, cmake = tools / "bin/clang++", Path("/usr/bin/cmake")
    xmake = Path("/home/macromodel/.local/bin/xmake")
    environment = dict(
        os.environ,
        XMAKE_STATS="false",
        XMAKE_SKIP_HISTORY="y",
        XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake",
        UWVM_PLATFORM08_PYTHON=sys.executable,
        UWVM_PLATFORM08_PYTHON_SHA256=sha(sys.executable),
        UWVM_PLATFORM08_CMAKE=str(cmake),
        UWVM_PLATFORM08_CMAKE_SHA256=sha(cmake),
        UWVM_PLATFORM08_CXX=str(cxx),
        UWVM_PLATFORM08_CXX_SHA256=sha(cxx),
        UWVM_KORTEX_PREFIX=str(
            agents / "20261021-kortex-acquire-r1/work/kortex-2.8.0/original"
        ),
        UWVM_KORTEX_ARCHIVE=str(
            agents / "20261021-kortex-acquire-r1/work/kortex-2.8.0/original.zip"
        ),
        UWVM_FAST_IO_INCLUDE=str(sdk / "include"),
        LD_LIBRARY_PATH=str(tools / "lib/x86_64-unknown-linux-gnu")
        + ":/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu",
        OPENBLAS_NUM_THREADS="1",
        OMP_NUM_THREADS="1",
        CXXFLAGS="-stdlib=libstdc++ --gcc-toolchain=/usr -fuse-ld=/usr/bin/ld",
    )
    temporary = work / "tmp"
    temporary.mkdir()
    environment["TMPDIR"] = str(temporary)
    rows = []
    report = dict(
        schema="uwvm-task08-round03-installed-native-v1",
        rows=rows,
        input_hashes=pins(inputs),
        sdk_hashes=pins(sdk),
        builds=[],
        default_authority=[],
        native_sdk_rpcs=0,
        native_ros_cases=0,
        wasm_guest_cases=0,
        robot_cases=0,
        whole_sdk_assembly=False,
        assembly="SCOPED_PRIVATE_INSTALLED_CANDIDATE",
        selection=selection,
        native_cxxflags=environment["CXXFLAGS"],
    )

    def run(name, argv, cwd=sdk / "example"):
        directory = work / "logs" / name
        directory.mkdir(parents=True)
        with (
            (directory / "stdout").open("xb") as out,
            (directory / "stderr").open("xb") as err,
        ):
            value = subprocess.run(
                list(map(str, argv)),
                cwd=cwd,
                env=environment,
                stdin=subprocess.DEVNULL,
                stdout=out,
                stderr=err,
                timeout=120,
                check=False,
            )
        row = dict(
            name=name,
            argv=list(map(str, argv)),
            cwd=str(cwd),
            exit_code=value.returncode,
            stdout_sha256=sha(directory / "stdout"),
            stderr_sha256=sha(directory / "stderr"),
        )
        rows.append(row)
        print(json.dumps(row), flush=True)
        if value.returncode:
            raise RuntimeError(f"{name} failed; see retained logs")

    try:
        for name, program in [
            ("xmake", xmake),
            ("cmake", cmake),
            ("cxx", cxx),
            ("python", Path(sys.executable)),
        ]:
            run(name + "-version", [program, "--version"], work)
        for platform, language, entry in [
            ("kinova", "py", "native-peer"),
            ("hello_robot", "py", "native-peer"),
            ("kinova", "cpp", "original-descriptors"),
        ]:
            if selection == "py" and language != "py":
                continue
            if selection == "cpp" and language != "cpp":
                continue
            name = platform + "-" + language
            run(
                name + "-configure",
                [
                    xmake,
                    "f",
                    "-y",
                    "--sdk-example-platform=" + platform,
                    "--sdk-example-language=" + language,
                    "--sdk-example=" + entry,
                ],
            )
            output = work / (name + "-product")
            run(
                name + "-build", [xmake, "sdk-example-build", "--output=" + str(output)]
            )
            receipt = json.loads((output / "build-receipt.json").read_text())
            assert (
                receipt["status"] == "BUILT_NOT_EXECUTED"
                and receipt["default_authority"] == []
            )
            for artifact in receipt["artifacts"]:
                assert (
                    artifact["kind"] == "native-component"
                    and sha(artifact["path"]) == artifact["sha256"]
                )
            item = dict(
                platform=platform, language=language, entry=entry, receipt=receipt
            )
            if language == "cpp":
                descriptor = work / "original-descriptors.pb"
                run(
                    "original-export-execute",
                    [receipt["artifacts"][0]["path"], descriptor],
                    work,
                )
                assert descriptor.stat().st_size == 101775
                item["descriptor"] = dict(
                    bytes=descriptor.stat().st_size, sha256=sha(descriptor)
                )
            else:
                original_support = (
                    sdk / "example/platform" / platform / "py/native_support"
                )
                for source in original_support.rglob("*.py"):
                    target = (
                        output / "native_support" / source.relative_to(original_support)
                    )
                    assert target.read_bytes() == source.read_bytes()
                    compile(target.read_bytes(), str(target), "exec")
                item["native_ros_runtime_imported"] = False
            item["product_hashes"] = pins(output)
            report["builds"].append(item)
        report["status"] = (
            {
                "py": "PASS_TWO_NATIVE_PYTHON_COMPONENT_XMAKE_BUILDS",
                "cpp": "PASS_ACTUAL_CPP_EXPORT_XMAKE_BUILD",
            }[selection]
            if selection != "all"
            else "PASS_THREE_ACTUAL_NATIVE_XMAKE_BUILDS"
        )
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        (work / "installed-native.json").write_text(
            json.dumps(report, indent=2, sort_keys=True) + "\n"
        )


if __name__ == "__main__":
    main()
