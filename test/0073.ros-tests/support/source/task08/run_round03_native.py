"""Qualify owned formatting and relocated native xmake examples under owner v2.

This runner is a unified-test consumer, never an installed SDK dependency.
It starts no ROS node, Kortex client, robot connection, or Wasm Guest.
"""

import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import urllib.parse
import urllib.request


AGENTS = Path("/home/macromodel/Documents/uwvm-test-261018/agents")
TOOLS = Path("/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm")
FORMAT_PACKAGES = {
    "black": "25.1.0",
    "click": "8.1.8",
    "mypy-extensions": "1.0.0",
    "packaging": "24.2",
    "pathspec": "0.12.1",
    "platformdirs": "4.3.7",
}


def digest(path):
    """Hash one retained ordinary file without loading large inputs into RAM."""
    with Path(path).open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def files_below(root):
    """Record the exact ordinary, confined source closure of a staged tree."""
    result = []
    for path in sorted(root.rglob("*")):
        if path.is_symlink():
            raise ValueError(f"input symlink is not materialized: {path}")
        if path.is_file():
            result.append(
                dict(
                    path=str(path.relative_to(root)),
                    bytes=path.stat().st_size,
                    sha256=digest(path),
                )
            )
    return result


def main():
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("the common finite resource owner is required")
    source = Path(sys.argv[1]).resolve(strict=True)
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    temporary = work / "tmp"
    temporary.mkdir()
    environment = dict(
        os.environ,
        TMPDIR=str(temporary),
        PIP_DISABLE_PIP_VERSION_CHECK="1",
        XMAKE_STATS="false",
        XMAKE_SKIP_HISTORY="y",
        XMAKE_PROGRAM_DIR="/home/macromodel/.local/share/xmake",
        OPENBLAS_NUM_THREADS="1",
        OMP_NUM_THREADS="1",
        LD_LIBRARY_PATH=(
            str(TOOLS / "lib/x86_64-unknown-linux-gnu")
            + ":/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu"
        ),
    )
    commands = []

    def run(name, arguments, cwd=work, *, env=None, required=True):
        directory = work / "logs" / name
        directory.mkdir(parents=True)
        argv = list(map(str, arguments))
        with (
            (directory / "stdout").open("xb") as out,
            (directory / "stderr").open("xb") as err,
        ):
            result = subprocess.run(
                argv,
                cwd=cwd,
                env=env or environment,
                stdin=subprocess.DEVNULL,
                stdout=out,
                stderr=err,
                timeout=120,
                check=False,
            )
        row = dict(
            name=name,
            argv=argv,
            cwd=str(cwd),
            exit_code=result.returncode,
            stdout_sha256=digest(directory / "stdout"),
            stderr_sha256=digest(directory / "stderr"),
        )
        commands.append(row)
        print(json.dumps(row), flush=True)
        if required and result.returncode:
            raise RuntimeError(
                f"{name} failed; retained stdout/stderr identify the cause"
            )
        return directory, result.returncode

    report = dict(
        schema="uwvm-task08-round03-native-v1",
        default_authority=[],
        input_hashes=files_below(source),
        commands=commands,
        builds=[],
        formats=[],
        native_sdk_rpcs=0,
        native_ros_cases=0,
        robot_cases=0,
        wasm_guest_cases=0,
        whole_sdk_assembly_qualified=False,
        status="STARTED",
    )
    try:
        # Read-only input copying is separated from the actual installed build.
        # This capsule contains the real public xmake engine/base headers and
        # installed examples, not an origin/test tree or a second checkout.
        sdk = work / "relocated-sdk"
        shutil.copytree(source / "sdk", sdk, ignore=shutil.ignore_patterns("origin"))
        assert not (sdk / "origin").exists() and not (sdk / "test").exists()
        report["capsule_input_hashes"] = files_below(sdk)

        formatter = TOOLS / "bin/clang-format"
        run("clang-format-version", [formatter, "--version"])
        manifest = json.loads(Path(sys.argv[2]).read_text())
        for index, relative in enumerate(manifest["cpp"]):
            original = source / relative
            output = work / "formatted" / relative
            output.parent.mkdir(parents=True, exist_ok=True)
            command = [formatter, "--style=" + manifest["clang_format_style"], original]
            directory, _ = run(f"clang-format-{index:03}", command)
            shutil.copyfile(directory / "stdout", output)
            second, _ = run(
                f"clang-format-check-{index:03}",
                [formatter, "--style=" + manifest["clang_format_style"], output],
            )
            if (second / "stdout").read_bytes() != output.read_bytes():
                raise AssertionError(f"formatter is not idempotent: {relative}")
            report["formats"].append(
                dict(
                    language="cpp",
                    source=relative,
                    before_sha256=digest(original),
                    after_sha256=digest(output),
                    idempotent=True,
                    formatter=str(formatter),
                    formatter_sha256=digest(formatter),
                )
            )

        # Download exact pure-Python formatter wheels; the PyPI file digest and
        # response metadata are retained before installation without dependencies.
        wheels = work / "formatter-wheels"
        wheels.mkdir()
        downloaded = 0
        packages = []
        for name, version in FORMAT_PACKAGES.items():
            url = f"https://pypi.org/pypi/{name}/{version}/json"
            with urllib.request.urlopen(url, timeout=20) as response:
                metadata_body = response.read(1_048_577)
            if len(metadata_body) > 1_048_576:
                raise ValueError("formatter package metadata exceeds bound")
            metadata = json.loads(metadata_body)
            matches = [
                row
                for row in metadata["urls"]
                if row["filename"].endswith("-py3-none-any.whl")
            ]
            if len(matches) != 1:
                raise ValueError(f"exactly one pure-Python wheel required: {name}")
            item = matches[0]
            if urllib.parse.urlparse(item["url"]).hostname != "files.pythonhosted.org":
                raise ValueError("formatter wheel is outside the pinned provider")
            target = wheels / item["filename"]
            with urllib.request.urlopen(item["url"], timeout=20) as response:
                body = response.read(4_194_305)
            downloaded += len(body)
            if len(body) != item["size"] or downloaded > 12_582_912:
                raise ValueError("formatter wheel size/aggregate bound mismatch")
            target.write_bytes(body)
            if digest(target) != item["digests"]["sha256"]:
                raise ValueError("formatter wheel digest mismatch")
            (wheels / (name + ".json")).write_bytes(metadata_body)
            packages.append(
                dict(
                    name=name,
                    version=version,
                    filename=item["filename"],
                    bytes=len(body),
                    sha256=digest(target),
                    url=item["url"],
                )
            )
        python_tools = work / "formatter-python"
        run(
            "install-pinned-black",
            [
                sys.executable,
                "-I",
                "-B",
                "-m",
                "pip",
                "install",
                "--no-index",
                "--no-deps",
                "--no-compile",
                "--target",
                python_tools,
                *sorted(wheels.glob("*.whl")),
            ],
        )
        report["formatter_packages"] = packages
        black = [
            sys.executable,
            "-I",
            "-B",
            "-c",
            "import runpy,sys; sys.path.insert(0,sys.argv.pop(1)); "
            "sys.argv[0]='black'; runpy.run_module('black',run_name='__main__')",
            python_tools,
        ]
        run("black-version", [*black, "--version"])
        for index, relative in enumerate(manifest["python"]):
            original = source / relative
            output = work / "formatted" / relative
            output.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(original, output)
            run(
                f"black-{index:03}",
                [*black, "--target-version", "py310", "--line-length", "88", output],
            )
            first_hash = digest(output)
            run(
                f"black-check-{index:03}",
                [
                    *black,
                    "--target-version",
                    "py310",
                    "--line-length",
                    "88",
                    "--check",
                    output,
                ],
            )
            assert digest(output) == first_hash
            compile(output.read_bytes(), str(output), "exec")
            report["formats"].append(
                dict(
                    language="py",
                    source=relative,
                    before_sha256=digest(original),
                    after_sha256=first_hash,
                    idempotent=True,
                )
            )

        # Explicit publication candidates are applied only inside this isolated
        # SDK. Task10 retains ownership of the serialized final public cutover.
        for item in manifest["candidate_installed_mirrors"]:
            shutil.copyfile(
                work / "formatted" / item["source"], sdk / item["destination"]
            )
        report["qualified_capsule_hashes"] = files_below(sdk)
        xmake = Path("/home/macromodel/.local/bin/xmake")
        compiler = TOOLS / "bin/clang++"
        cmake = Path("/usr/bin/cmake")
        original = AGENTS / "08/20261021-kortex-acquire-r1/work/kortex-2.8.0"
        environment.update(
            UWVM_PLATFORM08_PYTHON=sys.executable,
            UWVM_PLATFORM08_PYTHON_SHA256=digest(sys.executable),
            UWVM_PLATFORM08_CMAKE=str(cmake),
            UWVM_PLATFORM08_CMAKE_SHA256=digest(cmake),
            UWVM_PLATFORM08_CXX=str(compiler),
            UWVM_PLATFORM08_CXX_SHA256=digest(compiler),
            UWVM_KORTEX_PREFIX=str(original / "original"),
            UWVM_KORTEX_ARCHIVE=str(original / "original.zip"),
            UWVM_FAST_IO_INCLUDE=str(sdk / "include"),
        )
        for name, program in [
            ("xmake", xmake),
            ("cmake", cmake),
            ("clang", compiler),
            ("python", Path(sys.executable)),
        ]:
            run(name + "-version", [program, "--version"])
        for platform, language, entry in [
            ("kinova", "py", "native-peer"),
            ("hello_robot", "py", "native-peer"),
            ("kinova", "cpp", "original-descriptors"),
        ]:
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
                sdk / "example",
            )
            output = work / (name + "-product")
            run(
                name + "-build",
                [xmake, "sdk-example-build", "--output=" + str(output)],
                sdk / "example",
            )
            receipt = json.loads((output / "build-receipt.json").read_text())
            assert receipt["status"] == "BUILT_NOT_EXECUTED"
            assert receipt["default_authority"] == []
            for artifact in receipt["artifacts"]:
                assert artifact["kind"] == "native-component"
                assert digest(artifact["path"]) == artifact["sha256"]
            row = dict(
                platform=platform,
                language=language,
                entry=entry,
                receipt=receipt,
                native_ros_runtime_imported=False,
            )
            if language == "cpp":
                descriptors = work / "original-descriptors.pb"
                run(
                    "original-descriptors-execute",
                    [receipt["artifacts"][0]["path"], descriptors],
                )
                assert descriptors.stat().st_size == 101775
                row["descriptors"] = dict(
                    bytes=descriptors.stat().st_size, sha256=digest(descriptors)
                )
            else:
                support_root = sdk / "example/platform" / platform / "py/native_support"
                for file in sorted(support_root.rglob("*.py")):
                    copied = output / "native_support" / file.relative_to(support_root)
                    assert copied.read_bytes() == file.read_bytes()
                    compile(copied.read_bytes(), str(copied), "exec")
                compile(
                    (output / "native-example.py").read_bytes(),
                    str(output / "native-example.py"),
                    "exec",
                )
                row["native_component_hashes"] = files_below(output)
            report["builds"].append(row)

        # Current documentation/path/format changes are also compiled against
        # the genuine original C++11 ABI, retaining all 17 RPC member bodies.
        contract = (
            source
            / "test/0073.ros-tests/support/source/task08/platform/kinova/tests/cpp/kortex_native_contract.cpp"
        )
        native_header = sdk / "include/platform/kinova/cpp/native"
        executable = work / "kortex-current-contract"
        includes = [
            original / "original/include" / item
            for item in ("", "client", "common", "messages", "client_stubs")
        ]
        run(
            "kortex-current-contract-build",
            [
                compiler,
                "-std=c++11",
                "-D_OS_UNIX",
                "-O1",
                "-UNDEBUG",
                "-stdlib=libstdc++",
                "--gcc-toolchain=/usr",
                "-fuse-ld=/usr/bin/ld",
                "-I" + str(native_header),
                *["-I" + str(path) for path in includes],
                contract,
                original / "original/lib/release/libKortexApiCpp.a",
                "-pthread",
                "-ldl",
                "-no-pie",
                "-o",
                executable,
            ],
        )
        run("kortex-current-contract-execute", [executable])
        report["native_contract"] = dict(
            sha256=digest(executable), linked_operation_bodies=17, rpc_calls=0
        )
        report["status"] = "PASS_SCOPED_NATIVE_AND_FORMAT"
    except BaseException as error:
        report["status"] = "FAILED_RETAINED"
        report["error"] = repr(error)
        raise
    finally:
        (work / "round03-native.json").write_text(
            json.dumps(report, indent=2, sort_keys=True) + "\n"
        )


if __name__ == "__main__":
    main()
