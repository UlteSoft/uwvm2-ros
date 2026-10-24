#!/usr/bin/env python3
"""Rebuild current C modes serially and retain lossless per-mode capsules.

This canonical test scheduler borrows a verified owned formatting output and
real, explicitly selected Linux tools. It restores only the pinned inspector
and schema from the earlier installed SDK. It neither publishes that SDK nor
substitutes a provider profile for the explicit toolchain/CRT probes.
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


MODES = ("c89", "c90", "iso9899:199409", "c99", "c11", "c17", "c23", "c2x", "c2y")
BASE_ARCHIVE_SHA256 = "f19d4b7ef3d40ce522e1e40dfaafebb89e54b95591bce2f062495b7a53b4fb0c"


def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        while block := stream.read(1 << 20):
            value.update(block)
    return value.hexdigest()


def capsule(directory, destination):
    """Verify every ordinary member before retiring this run's mode scratch."""
    files = {}
    for path in directory.rglob("*"):
        if path.is_symlink():
            raise RuntimeError("ordinary test artifacts required")
        if path.is_file():
            files[path.relative_to(directory).as_posix()] = digest(path)
    with tarfile.open(destination, "w:gz") as archive:
        archive.add(directory, arcname="mode")
    with tarfile.open(destination) as archive:
        restored = {
            Path(member.name).relative_to("mode").as_posix():
                hashlib.sha256(archive.extractfile(member).read()).hexdigest()
            for member in archive if member.isfile()
        }
    if files != restored:
        raise RuntimeError("mode capsule changed a retained file")
    receipt = {"path": str(destination), "sha256": digest(destination),
               "bytes": destination.stat().st_size, "members_sha256": files,
               "all_member_bytes_verified_before_retirement": True}
    shutil.rmtree(directory)
    return receipt


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--formatted-source", type=Path, required=True)
    parser.add_argument("--target", choices=("both", "wasm32", "wasm64"), default="both")
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        parser.error("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs = run / "inputs"
    work = run / "work"
    formatted = args.formatted_source.resolve(strict=True)
    # Sources are immutable borrowed inputs for this run; the matrix never
    # writes them. Only its two support tools and mode output use live scratch.
    source = formatted
    support = work / "support"
    support.mkdir()
    baseline = run.parent / "c-xmake-r11/resource-archives/evidence.tar"
    if digest(baseline) != BASE_ARCHIVE_SHA256:
        raise RuntimeError("pinned installed SDK archive changed")
    restored = {}
    with tarfile.open(baseline) as archive:
        for relative, destination in (
            ("tools/check_wasm_imports.py", "tools/check_wasm_imports.py"),
            ("share/abi/abi-v1.json", "abi/abi-v1.json"),
        ):
            member = archive.getmember("work/relocated-sdk/" + relative)
            if not member.isfile():
                raise RuntimeError("ordinary pinned inspector/schema required")
            target = support / destination
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(archive.extractfile(member).read())
            restored[destination] = digest(target)
    expected = (inputs / "c-format-files.txt").read_text().splitlines()
    if len(expected) != 71:
        raise RuntimeError("explicit owned C source denominator changed")
    source_pins = {relative: digest(source / relative) for relative in expected}
    for relative, sha256 in source_pins.items():
        if digest(formatted / relative) != sha256:
            raise RuntimeError("current C input changed during staging")
    sdk = run.parent.parent / "10/staged-wasi-r1/staged/wasi-sdk"
    reference64 = run.parent / "c-focused-r1/work/reference/wasm64-wasip1"
    runtime = run.parent / "c-focused-r1/work/uwvm2-ros"
    report = {
        "format": "task02-current-c-serial-matrix-v1", "status": "RUNNING",
        "source_sha256": source_pins, "restored_support_sha256": restored,
        "support_archive_sha256": BASE_ARCHIVE_SHA256,
        "runtime_sha256": digest(runtime), "default_authority": [],
        "current_source_build_qualified": False, "compiler_rt64_qualified": False,
        "public_profile_resolver_qualified": False, "eh_thread_runtime_qualified": False,
        "native_ros_runs": 0, "gateway_runs": 0, "physical_motion_runs": 0,
        "upstream_official_suite_credit": 0, "qemu_runs": 0,
        "modes": [], "counts": {"PASS": 0, "FAIL": 0, "NOT_RUN": 0, "NOT_SUPPORTED": 0},
        "tool_inputs_sha256": {str(path): digest(path) for path in (
            Path(sys.executable), Path("/usr/bin/gcc-15"), Path("/usr/bin/clang-22"),
            Path("/usr/bin/wasm-validate"), sdk / "bin/clang", sdk / "bin/llvm-ar",
            sdk / "bin/wasm-ld", sdk / "share/wasi-sysroot/lib/wasm32-wasip1/libc.a",
            reference64 / "lib/wasm64-wasip1/libc.a",
            reference64 / "lib/wasm64-wasip1/crt1-command.o",
        )},
    }

    def checkpoint():
        (work / "current-matrix.json").write_text(json.dumps(report, indent=2) + "\n")

    checkpoint()
    targets = ("wasm32", "wasm64") if args.target == "both" else (args.target,)
    report["selected_target_scope"] = list(targets)
    guests = {}
    for target in targets:
        guests[target] = 0
        for mode in MODES:
            label = target + "-" + mode.replace(":", "-")
            output = work / label
            argv = [sys.executable, "-I", "-S", "-B", inputs / "tests/standard_matrix.py",
                    "--sdk", source, "--test-sources", inputs / "tests", "--output", output,
                    "--import-checker", support / "tools/check_wasm_imports.py",
                    "--abi-schema", support / "abi/abi-v1.json",
                    "--mode", mode, "--wasm-cc", sdk / "bin/clang",
                    "--wasm-ar", sdk / "bin/llvm-ar", "--wasm-ld", sdk / "bin/wasm-ld",
                    "--wasm-target", target + "-wasip1", "--runtime", runtime,
                    "--validator", "/usr/bin/wasm-validate"]
            if target == "wasm32":
                sysroot = sdk / "share/wasi-sysroot"
                argv += ["--native-cc", "/usr/bin/gcc-15", "--native-cc", "/usr/bin/clang-22",
                         "--wasm-sysroot", sysroot,
                         "--wasm-libc", sysroot / "lib/wasm32-wasip1/libc.a"]
            else:
                argv += ["--skip-native", "--wasm-sysroot", reference64,
                         "--wasm-libc", reference64 / "lib/wasm64-wasip1/libc.a",
                         "--wasm-crt", reference64 / "lib/wasm64-wasip1/crt1-command.o",
                         "--runtime-extra=--wasm-feature-enable-memory64",
                         "--runtime-extra=--wasm-feature-enable-table64"]
            result = subprocess.run([str(item) for item in argv], stdin=subprocess.DEVNULL,
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                    timeout=240, check=False)
            (work / (label + ".stdout")).write_bytes(result.stdout)
            (work / (label + ".stderr")).write_bytes(result.stderr)
            matrix = json.loads((output / "matrix.json").read_bytes())
            counts = {status: sum(row["status"] == status for row in matrix["rows"])
                      for status in report["counts"]}
            runtime_rows = [row for row in matrix["rows"] if row["layer"] == "actual-wasm-runtime"]
            guests[target] += sum(row["status"] == "PASS" for row in runtime_rows)
            for status, count in counts.items():
                report["counts"][status] += count
            record = {"target": target, "mode": mode, "argv": [str(item) for item in argv],
                      "exit_code": result.returncode, "counts": counts,
                      "guest_passes": sum(row["status"] == "PASS" for row in runtime_rows),
                      "matrix_sha256": digest(output / "matrix.json"),
                      "capsule": capsule(output, work / (label + ".tar.gz"))}
            report["modes"].append(record)
            checkpoint()
            if result.returncode or any(counts[status] for status in ("FAIL", "NOT_RUN", "NOT_SUPPORTED")):
                report["status"] = "FAIL_STOPPED_AFTER_FIRST_UNEXPECTED_MODE"
                checkpoint()
                return 1
    report["guest_passes_by_target"] = guests
    report["expected_guest_denominator_per_target"] = 405
    complete = all(count == 405 for count in guests.values())
    report["current_source_build_qualified"] = complete
    report["status"] = "PASS_EXECUTED_CURRENT_SOURCE_SCOPE" if complete else "FAIL_GUEST_DENOMINATOR"
    checkpoint()
    print(json.dumps({"status": report["status"], "counts": report["counts"],
                      "guest_passes_by_target": guests}), flush=True)
    return int(not complete)


if __name__ == "__main__":
    sys.exit(main())
