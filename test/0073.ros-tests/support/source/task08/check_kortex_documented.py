"""Compile the current documented/formatted 17-operation owner against real SDK2.8."""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("common finite owner is required")
    source = Path(sys.argv[1])
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    agents = Path("/home/macromodel/Documents/uwvm-test-261018/agents/08")
    original = agents / "20261021-kortex-acquire-r1/work/kortex-2.8.0"
    lock = json.loads((source / "original-sdk-lock.json").read_text())
    assert sha(original / "original.zip") == lock["archive_sha256"]
    for item in lock["files"]:
        path = original / "original" / item["path"]
        assert not path.is_symlink() and path.stat().st_size == item["bytes"]
        assert sha(path) == item["sha256"]
    prefix = Path("/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm")
    compiler = prefix / "bin/clang++"
    temporary = work / "tmp"
    temporary.mkdir()
    environment = dict(
        os.environ,
        TMPDIR=str(temporary),
        LD_LIBRARY_PATH=(
            str(prefix / "lib/x86_64-unknown-linux-gnu")
            + ":/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu"
        ),
    )
    rows = []

    def run(name, argv):
        with (
            (work / (name + ".stdout")).open("xb") as out,
            (work / (name + ".stderr")).open("xb") as err,
        ):
            value = subprocess.run(
                list(map(str, argv)),
                env=environment,
                cwd=work,
                stdin=subprocess.DEVNULL,
                stdout=out,
                stderr=err,
                timeout=90,
                check=False,
            )
        row = dict(
            name=name,
            argv=list(map(str, argv)),
            exit_code=value.returncode,
            stdout_sha256=sha(work / (name + ".stdout")),
            stderr_sha256=sha(work / (name + ".stderr")),
        )
        rows.append(row)
        print(json.dumps(row), flush=True)
        if value.returncode:
            raise RuntimeError(name + " failed; retained output identifies the cause")

    report = dict(
        schema="uwvm-task08-documented-kortex-current-v1",
        rows=rows,
        input_hashes=[
            dict(path=path.name, bytes=path.stat().st_size, sha256=sha(path))
            for path in sorted(source.iterdir())
            if path.is_file()
        ],
        compiler=dict(path=str(compiler), sha256=sha(compiler)),
        original_sdk_expanded_files_checked=len(lock["files"]),
        original_archive_sha256=lock["archive_sha256"],
        original_operation_bodies=17,
        native_sdk_rpcs=0,
        default_authority=[],
        native_ros_cases=0,
        wasm_guest_cases=0,
    )
    try:
        run("compiler-version", [compiler, "--version"])
        includes = [
            original / "original/include" / part
            for part in ("", "client", "common", "messages", "client_stubs")
        ]
        executable = work / "kortex-cxx11"
        run(
            "current-cxx11-final-link",
            [
                compiler,
                "-std=c++11",
                "-D_OS_UNIX",
                "-O1",
                "-UNDEBUG",
                "-stdlib=libstdc++",
                "--gcc-toolchain=/usr",
                "-fuse-ld=/usr/bin/ld",
                "-I" + str(source),
                *["-I" + str(path) for path in includes],
                source / "kortex_native_contract.cpp",
                original / "original/lib/release/libKortexApiCpp.a",
                "-pthread",
                "-ldl",
                "-no-pie",
                "-o",
                executable,
            ],
        )
        run("no-rpc-current-contract", [executable])
        report.update(
            status="PASS_CURRENT_DOCUMENTED_FORMATTED_CPP11",
            artifact=dict(
                path=str(executable),
                bytes=executable.stat().st_size,
                sha256=sha(executable),
            ),
        )
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        report["input_files_unchanged"] = all(
            (source / row["path"]).stat().st_size == row["bytes"]
            and sha(source / row["path"]) == row["sha256"]
            for row in report["input_hashes"]
        )
        report["original_inputs_unchanged"] = (
            sha(original / "original.zip") == lock["archive_sha256"]
            and all(
                (original / "original" / row["path"]).stat().st_size == row["bytes"]
                and sha(original / "original" / row["path"]) == row["sha256"]
                for row in lock["files"]
            )
        )
        report["compiler_unchanged"] = sha(compiler) == report["compiler"]["sha256"]
        if not all(report[key] for key in (
            "input_files_unchanged", "original_inputs_unchanged", "compiler_unchanged"
        )):
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        (work / "documented-kortex.json").write_text(
            json.dumps(report, indent=2, sort_keys=True) + "\n"
        )
        if report["status"] == "FAILED_INPUT_IMMUTABILITY_RETAINED":
            raise RuntimeError("current source/original SDK/compiler changed")


if __name__ == "__main__":
    main()
