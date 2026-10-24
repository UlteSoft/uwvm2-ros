#!/usr/bin/env python3
"""Replay frozen memory64 artifacts under explicit features and page bounds.

This establishes Runtime behavior for the pinned earlier compiler/source
snapshot. It is not a current-source build, default-provider qualification or
positive native ROS result. One active composition is materialized at a time;
all original artifacts remain in both immutable archive copies.
"""

import base64
import gzip
import hashlib
import json
import importlib.util
import os
from pathlib import Path
import subprocess
import sys
import tarfile


ARCHIVE_SHA256 = "1a25a0088b4891e58712631888267472bc7b86a0f7bc82c8a7b0b4881fcc2b40"
RUNTIME_SHA256 = "719e1cb683abeed7710d54ae705edc3e3ddd0048daf91cdbfe5a1ae6a9c26c19"
INSPECTOR_ARCHIVE_SHA256 = "f19d4b7ef3d40ce522e1e40dfaafebb89e54b95591bce2f062495b7a53b4fb0c"
PROFILES = {"c89": 89, "c90": 90, "iso9899:199409": 95, "c99": 99,
            "c11": 11, "c17": 17, "c23": 23, "c2x": 23, "c2y": 29}


def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        while block := stream.read(1 << 20):
            value.update(block)
    return value.hexdigest()


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    original = run.parent / "c-focused-r1"
    archive_path = original / "resource-archives/evidence.tar"
    runtime = original / "work/uwvm2-ros"
    if digest(archive_path) != ARCHIVE_SHA256 or digest(runtime) != RUNTIME_SHA256:
        raise RuntimeError("original artifact archive or actual Runtime changed")
    inspector_archive = run.parent / "c-xmake-r11/resource-archives/evidence.tar"
    if digest(inspector_archive) != INSPECTOR_ARCHIVE_SHA256:
        raise RuntimeError("pinned installed inspector archive changed")
    inspector_path = run / "work/check_wasm_imports.py"
    with tarfile.open(inspector_archive) as support:
        member = support.getmember("work/relocated-sdk/tools/check_wasm_imports.py")
        if not member.isfile():
            raise RuntimeError("ordinary archived inspector required")
        inspector_path.write_bytes(support.extractfile(member).read())
    spec = importlib.util.spec_from_file_location("delivery_binary_inspector", inspector_path)
    inspector = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(inspector)
    active = run / "work/active"
    active.mkdir()
    common = [str(runtime), "--runtime-int", "-Rct", "0", "--wasm-set-main-module-name", "main",
              "--wasip1-global-disable", "--wasip1-global-noinherit-system-environment",
              "--wasip1-single-create", "main", "--wasip1-single-enable", "main",
              "--wasip1-single-hide-host-api", "main",
              "--wasip1-single-noinherit-system-environment", "main"]
    for right in ("realtime-clock", "monotonic-clock", "process-clock", "thread-clock", "random",
                  "proc-raise", "sched-yield", "stdio-full-rights"):
        common += ["--wasip1-single-deny", "main", right]
    features = ["--wasm-feature-enable-table64", "--wasm-feature-enable-memory64"]
    rows, artifacts = [], {}
    rebuilt = []
    blocker = None
    with tarfile.open(archive_path) as archive:
        matrix_bytes = archive.extractfile("work/matrix64/matrix.json").read()
        matrix = json.loads(matrix_bytes)
        cases = [(index, row) for index, row in enumerate(matrix["rows"])
                 if row["layer"] == "actual-wasm-runtime"]
        if len(cases) != 405:
            raise RuntimeError("frozen405-case denominator changed")

        def materialize(old_path, role):
            path = Path(old_path)
            if path.parent != original / "work/matrix64" or path.suffix != ".wasm":
                raise RuntimeError("exact archived Wasm composition path required")
            pin = matrix["artifact_sizes"][path.name]
            member = "work/matrix64/" + path.name
            data = archive.extractfile(member).read()
            if len(data) != pin["bytes"] or hashlib.sha256(data).hexdigest() != pin["sha256"]:
                raise RuntimeError("archived binary pin differs: " + member)
            target = active / (role + ".wasm")
            target.write_bytes(data)
            artifacts[member] = {"sha256": pin["sha256"], "bytes": pin["bytes"]}
            return str(target), member

        def rebuild_delivery(row):
            toolchain = run.parent.parent / "10/staged-wasi-r1/staged/wasi-sdk"
            compiler = toolchain / "bin/clang"
            root = run.parent / "c-format-r8/work/source"
            source = run / "inputs/source/06_runtime_delivery.c"
            target = active / "delivery.wasm"
            argv = [str(compiler), "--target=wasm64-wasip1", "-std=" + row["mode"],
                    "-DROSWASM_C_STANDARD=" + str(PROFILES[row["mode"]]),
                    "-DROSWASM_C_ADDRESS_BITS=64", "-DROSWASM_C_EXCEPTIONS=0", "-DROSWASM_C_THREADS=0",
                    "-DROSWASM_EXAMPLE_ROS_VERSION=" + str(row["backend"]),
                    "-DROSWASM_EXAMPLE_DEFAULT_DENIED=" + str(row["definitions"]["ROSWASM_EXAMPLE_DEFAULT_DENIED"]),
                    "-pedantic-errors", "-Werror=implicit-function-declaration", "-O2", "-g0",
                    "-ffreestanding", "-nostdlib", "-fuse-ld=" + str(toolchain / "bin/wasm-ld"),
                    "--sysroot=" + str(original / "work/reference/wasm64-wasip1"),
                    "-I", str(root / "include"), str(source), "-o", str(target)]
            result = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE, timeout=60, check=False)
            record = {"mode": row["mode"], "backend": row["backend"], "definitions": row["definitions"],
                      "compiler_argv": argv, "compiler_exit": result.returncode,
                      "compiler_stdout_base64": base64.b64encode(result.stdout).decode(),
                      "compiler_stderr_base64": base64.b64encode(result.stderr).decode()}
            rebuilt.append(record)
            if result.returncode:
                return None, None
            data = target.read_bytes()
            identifier = "current-delivery/" + row["mode"] + "/ros" + str(row["backend"]) + "/denied" + str(row["definitions"]["ROSWASM_EXAMPLE_DEFAULT_DENIED"])
            compressed = gzip.compress(data, mtime=0)
            if gzip.decompress(compressed) != data:
                raise RuntimeError("lossless current binary capture failed")
            artifacts[identifier] = {"sha256": hashlib.sha256(data).hexdigest(), "bytes": len(data),
                                     "reconstructed_by": len(rebuilt)-1,
                                     "gzip_base64": base64.b64encode(compressed).decode()}
            imported = inspector.inspect(data)
            record["actual_imports"] = imported
            expected = {"fd_read_wasm64": (["i32", "i64", "i64", "i64"], ["i32"]),
                        "fd_write_wasm64": (["i32", "i64", "i64", "i64"], ["i32"]),
                        "proc_exit_wasm64": (["i32"], [])}
            correct = all(item["name"] in expected and (item["params"], item["results"]) == expected[item["name"]]
                          for item in imported if item["module"] == "wasi_snapshot_preview1")
            validation = subprocess.run(["/usr/bin/wasm-validate", "--enable-memory64", str(target)],
                stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=30, check=False)
            record["validator_exit"] = validation.returncode
            record["validator_stderr_base64"] = base64.b64encode(validation.stderr).decode()
            if not correct or validation.returncode:
                return None, None
            record["wasip1_memory64_imports_match"] = True
            return str(target), identifier

        for index, row in cases:
            if blocker is not None:
                rows.append({"original_row": index, "mode": row["mode"], "status": "NOT_RUN",
                             "reason": "prior unexpected Runtime outcome", "blocker_row": blocker})
                continue
            old_command = row["command"]["command"]
            if old_command[:len(common)] != common:
                raise RuntimeError("original empty-authority launch contract differs")
            remaining = old_command[len(common):]
            if remaining[:2] != features:
                raise RuntimeError("original explicit feature selection differs")
            remaining = remaining[2:]
            suffix = ["--wasm-set-memory-limit", "main", "all", "2", "256"]
            members = []
            while remaining and remaining[0] == "--wasm-preload-library":
                if len(remaining) < 3:
                    raise RuntimeError("truncated original composition")
                name = remaining[2]
                if name not in ("callbacks", "codec", "client"):
                    raise RuntimeError("unknown frozen preload role")
                path, member = materialize(remaining[1], name)
                members.append(member)
                suffix += ["--wasm-set-memory-limit", name, "all", "2", "256",
                           "--wasm-preload-library", path, name]
                remaining = remaining[3:]
            if len(remaining) != 2 or remaining[0] != "--run":
                raise RuntimeError("unknown original Runtime arguments")
            if row.get("lesson") == "06_runtime_delivery" and (run / "inputs/source/06_runtime_delivery.c").is_file():
                path, member = rebuild_delivery(row)
                if path is None:
                    rows.append({"original_row": index, "mode": row["mode"], "status": "FAIL",
                                 "reason": "current delivery build/import/validation failed", "rebuild": len(rebuilt)-1})
                    blocker = index
                    continue
            else:
                path, member = materialize(remaining[1], "main")
            members.append(member)
            suffix += ["--run", path]
            command = common + features + suffix
            expected = row["command"]["expected_exit"]
            try:
                result = subprocess.run(command, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                        stderr=subprocess.PIPE, timeout=30, check=False)
                code, stdout, stderr = result.returncode, result.stdout, result.stderr
            except subprocess.TimeoutExpired as error:
                code, stdout, stderr = None, error.stdout or b"", error.stderr or b""
            expected_stdout = None
            if row.get("hosted"):
                expected_stdout = b"C stdio profile\n"
            if row.get("lesson") == "06_runtime_delivery" and expected == 0:
                expected_stdout = b"PASS_SDK_DENIED\n"
            passed = code == expected and (expected_stdout is None or stdout == expected_stdout)
            rows.append({"original_row": index, "mode": row["mode"], "expected_exit": expected,
                         "exit_code": code, "status": "PASS" if passed else "FAIL",
                         "argv_suffix": suffix, "artifact_members": members,
                         "stdout_base64": base64.b64encode(stdout).decode(),
                         "stderr_base64": base64.b64encode(stderr).decode(),
                         "stdout_sha256": hashlib.sha256(stdout).hexdigest(),
                         "stderr_sha256": hashlib.sha256(stderr).hexdigest(),
                         "expected_stdout_checked": expected_stdout is not None})
            if not passed:
                blocker = index
            if len(rows) % 45 == 0:
                print(json.dumps({"completed_rows": len(rows), "last_mode": row["mode"],
                                  "unexpected_result": blocker}), flush=True)
    report = {"format": "task02-frozen-memory64-runtime-replay-v1", "rows": rows,
              "denominator": 405, "common_argv": common + features,
              "command_reconstruction": "common_argv + executed_row.argv_suffix",
              "source_evidence": {"archive": str(archive_path), "archive_sha256": ARCHIVE_SHA256,
                                  "matrix_member": "work/matrix64/matrix.json",
                                  "matrix_sha256": hashlib.sha256(matrix_bytes).hexdigest()},
              "runtime_sha256": RUNTIME_SHA256, "artifacts": artifacts,
              "current_delivery_rebuilds": rebuilt,
              "current_delivery_source_sha256": {p.name: digest(p) for p in (run / "inputs/source").glob('*') if p.is_file()},
              "current_delivery_dependencies_sha256": {p.relative_to(run.parent / "c-format-r8/work/source").as_posix(): digest(p)
                  for directory in ("include", "common", "ros1", "ros2")
                  for p in (run.parent / "c-format-r8/work/source" / directory).rglob('*.h')},
              "current_delivery_inspector_sha256": digest(inspector_path),
              "current_delivery_inspector_archive_sha256": INSPECTOR_ARCHIVE_SHA256,
              "current_delivery_tool_sha256": {name: digest(run.parent.parent / "10/staged-wasi-r1/staged/wasi-sdk/bin" / name)
                  for name in ("clang", "wasm-ld")} if rebuilt else {},
              "counts": {status: sum(row["status"] == status for row in rows)
                         for status in ("PASS", "FAIL", "NOT_RUN")},
              "default_authority": [], "virtual_address_mib": 49152,
              "memory_limits": "each actual module:2..256 standard64KiB pages",
              "current_source_build_qualified": False, "default_memory64_provider_qualified": False,
              "compiler_rt64_qualified": False, "native_ros_runs": 0, "physical_motion_runs": 0,
              "status": "PASS_FROZEN_ARTIFACT_RUNTIME_SCOPE" if blocker is None else "FAILED_RETAINED"}
    (run / "work/memory64-runtime.json").write_text(json.dumps(report, separators=(",", ":")) + "\n")
    print(json.dumps({"status": report["status"], "counts": report["counts"]}), flush=True)
    return int(blocker is not None)


if __name__ == "__main__":
    sys.exit(main())
