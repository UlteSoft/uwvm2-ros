"""Format/link/run the unpublished native notification candidate against SDK2.8.

All modes use genuine original BaseClient/protobuf code. Recording and finite
memory-ITransport modes cover their explicit dispatch/lifetime contracts only.
The native-tcp-loopback mode also uses original TransportClientTcp/framing and
RouterClient against a finite localhost fixture. None proves authentication,
native ROS, Gateway, Guest, official peer behavior or a physical robot.
"""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import zipfile


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    if (
        sys.platform != "linux"
        or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1"
    ):
        raise RuntimeError("original admitted Linux common finite owner required")
    packet = Path(sys.argv[1])
    assert len(sys.argv) in (2, 3)
    mode = sys.argv[2] if len(sys.argv) == 3 else "recording"
    entries = {"recording": "kortex_notifications_contract.cpp",
        "original-router": "kortex_notifications_router_contract.cpp",
        "native-tcp-loopback": "kortex_notifications_tcp_contract.cpp"}
    assert mode in entries
    router_mode = mode != "recording"
    tcp_mode = mode == "native-tcp-loopback"
    entry = entries[mode]
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    source = work / "formatted-source"
    source.mkdir()
    with zipfile.ZipFile(packet) as archive:
        assert (
            len(archive.infolist()) == 4
            and sum(row.file_size for row in archive.infolist()) < 1 << 20
        )
        inputs = {row.filename: archive.read(row) for row in archive.infolist()}
    assert set(inputs) == {
        "kortex_native.hpp",
        "kortex_notifications.hpp",
        entry,
        "original-sdk-lock.json",
    }
    original = Path(
        "/home/macromodel/Documents/uwvm-test-261018/agents/08/20261021-kortex-acquire-r1/work/kortex-2.8.0"
    )
    lock = json.loads(inputs["original-sdk-lock.json"])
    assert sha(original / "original.zip") == lock["archive_sha256"]
    for row in lock["files"]:
        path = original / "original" / row["path"]
        assert (
            not path.is_symlink()
            and path.stat().st_size == row["bytes"]
            and sha(path) == row["sha256"]
        )
    prefix = Path("/home/macromodel/Documents/tool-chain/x86_64-linux-gnu-llvm")
    compiler, formatter = prefix / "bin/clang++", prefix / "bin/clang-format"
    temporary = work / "tmp"
    temporary.mkdir()
    env = dict(
        os.environ,
        TMPDIR=str(temporary),
        LD_LIBRARY_PATH=(
            str(prefix / "lib/x86_64-unknown-linux-gnu")
            + ":/usr/lib/x86_64-linux-gnu:/lib/x86_64-linux-gnu"
        ),
    )
    rows, formatted = [], []
    report = dict(
        schema="uwvm-task08-kortex-notification-candidate-contract-v1",
        status="STARTED",
        input_packet_sha256=sha(packet),
        input_pins=[
            dict(path=name, bytes=len(body), sha256=hashlib.sha256(body).hexdigest())
            for name, body in sorted(inputs.items())
        ],
        compiler=dict(path=str(compiler), sha256=sha(compiler)),
        formatter=dict(path=str(formatter), sha256=sha(formatter)),
        original_expanded_files_checked=len(lock["files"]),
        original_archive_sha256=lock["archive_sha256"],
        commands=rows,
        formatted=formatted,
        native_original_BaseClient_recording_router_contract_runs=0,
        original_Router_dispatch_contract_runs=0,
        finite_memory_transport_fixture=router_mode and not tcp_mode,
        finite_native_TCP_loopback_fixture=tcp_mode,
        original_Router_transport_runs=0,
        native_ROS_runs=0,
        Gateway_runs=0,
        Guest_runs=0,
        physical_robot_runs=0,
        official_parity_accepted=False,
        public_SDK_delivery=False,
        default_authority=[],
    )

    def save():
        (work / "notification-candidate.json").write_text(
            json.dumps(report, indent=2, sort_keys=True) + "\n"
        )

    def run(name, argv, input_bytes=None):
        result = subprocess.run(
            list(map(str, argv)),
            input=input_bytes,
            env=env,
            cwd=work,
            capture_output=True,
            timeout=90,
        )
        assert len(result.stdout) + len(result.stderr) < 1 << 20
        (work / (name + ".stdout")).write_bytes(result.stdout)
        (work / (name + ".stderr")).write_bytes(result.stderr)
        rows.append(
            dict(
                name=name,
                argv=list(map(str, argv)),
                exit=result.returncode,
                stdin_sha256=(
                    hashlib.sha256(input_bytes).hexdigest()
                    if input_bytes is not None
                    else None
                ),
                stdout_sha256=sha(work / (name + ".stdout")),
                stderr_sha256=sha(work / (name + ".stderr")),
            )
        )
        save()
        assert result.returncode == 0, name
        return result.stdout

    try:
        run("compiler-version", [compiler, "--version"])
        run("formatter-version", [formatter, "--version"])
        style = "{BasedOnStyle: LLVM, IndentWidth: 4, ColumnLimit: 88, SortIncludes: Never, ReflowComments: false}"
        for name, body in sorted(inputs.items()):
            if name.endswith((".hpp", ".cpp")):
                argv = [formatter, "--style=" + style, "--assume-filename=" + name]
                first = run(name + "-format-first", argv, body)
                second = run(name + "-format-second", argv, first)
                assert first == second
                (source / name).write_bytes(first)
                formatted.append(
                    dict(
                        path=name,
                        before_sha256=hashlib.sha256(body).hexdigest(),
                        after_sha256=sha(source / name),
                        bytes=len(first),
                        idempotent=True,
                    )
                )
            else:
                (source / name).write_bytes(body)
        executable = work / "kortex-notifications-contract"
        include_roots = [
            original / "original/include" / part
            for part in ("", "client", "common", "messages", "client_stubs")
        ]
        run(
            "actual-cxx11-final-link",
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
                *["-I" + str(path) for path in include_roots],
                source / entry,
                original / "original/lib/release/libKortexApiCpp.a",
                "-pthread",
                "-ldl",
                "-no-pie",
                "-o",
                executable,
            ],
        )
        run(
            "genuine-BaseClient-original-Router-native-TCP-loopback-contract" if tcp_mode else
            "genuine-BaseClient-original-Router-memory-transport-contract"
            if router_mode
            else "genuine-BaseClient-recording-router-contract",
            [executable],
        )
        report.update(
            status=(
                "PASS_UNPUBLISHED_ORIGINAL_NATIVE_TCP_LOOPBACK_FIXTURE_CONTRACT_ONLY" if tcp_mode else
                "PASS_UNPUBLISHED_ORIGINAL_ROUTER_DISPATCH_MEMORY_TRANSPORT_CONTRACT_ONLY"
                if router_mode
                else "PASS_UNPUBLISHED_NATIVE_BASECLIENT_CONTRACT_ONLY"
            ),
            native_original_BaseClient_recording_router_contract_runs=0 if router_mode else 1,
            original_Router_dispatch_contract_runs=1 if router_mode else 0,
            original_Router_transport_runs=1 if tcp_mode else 0,
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
        report["original_inputs_unchanged"] = (
            sha(original / "original.zip") == lock["archive_sha256"]
            and all(
                (original / "original" / row["path"]).stat().st_size == row["bytes"]
                and sha(original / "original" / row["path"]) == row["sha256"]
                for row in lock["files"]
            )
        )
        report["compiler_and_formatter_unchanged"] = (
            sha(compiler) == report["compiler"]["sha256"]
            and sha(formatter) == report["formatter"]["sha256"]
        )
        if not report["original_inputs_unchanged"] or not report["compiler_and_formatter_unchanged"]:
            report["status"] = "FAILED_INPUT_IMMUTABILITY_RETAINED"
        save()
        if report["status"] == "FAILED_INPUT_IMMUTABILITY_RETAINED":
            raise RuntimeError("original SDK/compiler/formatter changed during the contract")


if __name__ == "__main__":
    main()
