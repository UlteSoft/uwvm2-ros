#!/usr/bin/env python3
"""Probe one pinned codec under explicit Runtime feature and memory limits."""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tarfile


ARCHIVE_SHA256 = "1a25a0088b4891e58712631888267472bc7b86a0f7bc82c8a7b0b4881fcc2b40"
ARTIFACT_SHA256 = "56f06b9e913489870f9163964b9259d49756a60381bc820a79474af53c08fce8"


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
    original = run.parent / "c-focused-r1/work"
    runtime = original / "uwvm2-ros"
    archive_path = run.parent / "c-focused-r1/resource-archives/evidence.tar"
    if digest(archive_path) != ARCHIVE_SHA256:
        raise RuntimeError("pinned frozen codec archive changed")
    artifact = run / "work/wasm-clang-c89-codec.wasm"
    with tarfile.open(archive_path) as archive:
        member = archive.getmember("work/matrix64/wasm-clang-c89-codec.wasm")
        if not member.isfile():
            raise RuntimeError("ordinary archived codec required")
        artifact.write_bytes(archive.extractfile(member).read())
    if digest(artifact) != ARTIFACT_SHA256:
        raise RuntimeError("frozen codec bytes changed")
    rows = []
    base = [str(runtime), "--runtime-int", "-Rct", "0", "--wasm-set-main-module-name", "main",
            "--wasip1-global-disable", "--wasip1-global-noinherit-system-environment",
            "--wasip1-single-create", "main", "--wasip1-single-enable", "main",
            "--wasip1-single-hide-host-api", "main",
            "--wasip1-single-noinherit-system-environment", "main"]
    for right in ("realtime-clock", "monotonic-clock", "process-clock", "thread-clock", "random",
                  "proc-raise", "sched-yield", "stdio-full-rights"):
        base += ["--wasip1-single-deny", "main", right]
    attempts = [
        ("declarations", ["/usr/bin/wasm-objdump", "-x", str(artifact)]),
        ("features-and-bounded-pages", base + ["--wasm-feature-enable-memory64",
            "--wasm-feature-enable-table64", "--wasm-set-memory-limit", "main", "all", "2", "256",
            "--run", str(artifact)]),
    ]
    for label, command in attempts:
        result = subprocess.run(command, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, timeout=40, check=False)
        (run / ("work/" + label + ".stdout")).write_bytes(result.stdout)
        (run / ("work/" + label + ".stderr")).write_bytes(result.stderr)
        rows.append({"label": label, "command": command, "exit_code": result.returncode,
                     "stdout_sha256": hashlib.sha256(result.stdout).hexdigest(),
                     "stderr_sha256": hashlib.sha256(result.stderr).hexdigest()})
    report = {"format": "task02-memory64-single-codec-probe-v1", "rows": rows,
              "runtime": str(runtime), "runtime_sha256": hashlib.sha256(runtime.read_bytes()).hexdigest(),
              "artifact": str(artifact), "artifact_sha256": hashlib.sha256(artifact.read_bytes()).hexdigest(),
              "source_archive": str(archive_path), "source_archive_sha256": ARCHIVE_SHA256,
              "default_authority": [], "finite_address_mib": 49152,
              "physical_memory_limit_bytes": 68719476736,
              "status": "PASS_CODEC_SCOPE" if rows[-1]["exit_code"] == 0 else "UNQUALIFIED_RUNTIME_COMBINATION"}
    (run / "work/probe.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({"status": report["status"], "exit_code": rows[-1]["exit_code"]}), flush=True)


if __name__ == "__main__":
    main()
