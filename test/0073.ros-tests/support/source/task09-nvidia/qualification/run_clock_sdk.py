#!/usr/bin/env python3
"""Finite actual clock-SDK preparation; compiler/Guest stages remain explicit."""
import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import subprocess
import sys
import tarfile


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", required=True, type=Path)
    parser.add_argument("--source-sha256", required=True)
    parser.add_argument("--with-kotlin", action="store_true")
    for name in ("parent", "bounded", "adapter", "managed"):
        parser.add_argument("--" + name + "-sha256", required=True)
    args = parser.parse_args()
    assert os.environ["UWVM_RESOURCE_HEAVY_LOCK_HELD"] == "1"
    run = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    assert args.source.is_relative_to(run / "inputs") and digest(args.source) == args.source_sha256
    work = run / "work"
    root = work / "source"
    root.mkdir(mode=0o700)
    captured = {}
    total = 0
    with tarfile.open(args.source, "r:") as archive:
        members = archive.getmembers()
        assert len(members) <= 256
        for member in members:
            name = PurePosixPath(member.name)
            assert not name.is_absolute() and all(part not in ("", ".", "..") for part in name.parts)
            assert member.isfile() or member.isdir()
            if member.isdir():
                continue
            assert member.name not in captured and member.size <= 1 << 20
            total += member.size
            assert total <= 4 << 20
            stream = archive.extractfile(member)
            raw = stream.read(member.size + 1)
            assert len(raw) == member.size
            destination = root / str(name)
            destination.parent.mkdir(parents=True, exist_ok=True)
            with destination.open("xb") as output:
                output.write(raw)
            captured[member.name] = {"bytes": len(raw), "sha256": hashlib.sha256(raw).hexdigest()}
    driver = root / "sdk/tools/platform/nvidia-isaac-sim/clock/generate_bindings.py"
    argv = [sys.executable, "-I", "-S", "-B", str(driver), "--source-root", str(root), "--out", str(work / "prepared")]
    for role in ("parent", "bounded", "adapter", "managed"):
        argv += ["--" + role + "-sha256", getattr(args, role + "_sha256")]
    if args.with_kotlin:
        argv += ["--with-kotlin"]
    with (work / "preparation.stdout").open("xb") as stdout, (work / "preparation.stderr").open("xb") as stderr:
        completed = subprocess.run(argv, stdout=stdout, stderr=stderr, timeout=120)
    result = {"format": "uwvm-task09-clock-sdk-source-run-v1", "source_sha256": args.source_sha256,
              "captured_sources": captured, "preparation_exit_code": completed.returncode,
              "actual_language_compilations": 0, "guest_executions": 0,
              "native_kit_ros_executions": 0, "default_authority": []}
    if completed.returncode == 0:
        receipt = work / "prepared/clock-sdk-preparation.json"
        result["preparation_receipt_sha256"] = digest(receipt)
        result["status"] = "SOURCE_PREPARATION_PASS_NOT_COMPILER_ACCEPTANCE"
    else:
        result["status"] = "FAILED_RETAINED"
    for name, pin in captured.items():
        assert digest(root / name) == pin["sha256"]
    (work / "clock-sdk-run.json").write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
    print(json.dumps({key: result[key] for key in ("status", "preparation_exit_code", "actual_language_compilations")}))
    return completed.returncode


if __name__ == "__main__":
    raise SystemExit(main())
