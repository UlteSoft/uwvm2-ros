#!/usr/bin/env python3
"""Qualify current generated C exports with one-family scratch and pinned inputs."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


ARCHIVE = Path("/home/macromodel/Documents/uwvm-test-261018/agents/02/c-matrix-r6/resource-archives/evidence.tar")
ARCHIVE_SHA256 = "9c4844cac8f47b3d98b0629f51d8fd72d3d85bab294c7d2353486c144d6acbab"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, required=True,
                        help="explicit verified current owned C source tree")
    parser.add_argument("--extra-root", type=Path,
                        help="explicit read-only captured extra-header root; all235 input hashes still checked")
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs = run / "inputs"
    sdk = args.sdk.resolve(strict=True)
    extra_root = args.extra_root.resolve(strict=True) if args.extra_root else inputs / "extra"
    selected = {}
    for line in (inputs / "generated-headers-current.sha256").read_text().splitlines():
        sha256, spelling = line.split("  ", 1)
        relative_path = Path(spelling)
        if relative_path.is_absolute() or ".." in relative_path.parts:
            raise RuntimeError("confined captured-header identity required")
        relative = relative_path.as_posix()
        extra = extra_root / relative
        selected[relative] = {"sha256": sha256}
        if extra.is_file():
            selected[relative]["path"] = str(extra)
        else:
            selected[relative]["archive_member"] = relative
    if len(selected) != 235:
        raise RuntimeError("current captured235-header denominator changed")
    mapping = run / "work/generated-input-map.json"
    mapping.write_text(json.dumps(selected, indent=2) + "\n")
    command = [sys.executable, "-I", "-S", "-B", inputs / "generated_portability.py",
               "--sdk", sdk, "--test-sources", inputs, "--output", run / "work/generated",
               "--stream", "--adapter", inputs / "c_profiles.py",
               "--portability-header", sdk / "include/platform/ros/c/roswasm/detail/common/c_portability.h",
               "--generated-input-map", mapping, "--generated-archive", ARCHIVE,
               "--generated-archive-sha256", ARCHIVE_SHA256]
    with (run / "work/generated.stdout").open("wb") as stdout, (run / "work/generated.stderr").open("wb") as stderr:
        result = subprocess.run([str(value) for value in command], stdin=subprocess.DEVNULL,
                                stdout=stdout, stderr=stderr, timeout=600, check=False)
    pins = {str(path): hashlib.sha256(path.read_bytes()).hexdigest()
            for path in (inputs / "c_profiles.py", inputs / "c_api_documentation.py",
                         inputs / "generated_portability.py", inputs / "portable_generated_input.h",
                         inputs / "portable_generated_main.c")}
    dependencies = {path.relative_to(sdk).as_posix(): hashlib.sha256(path.read_bytes()).hexdigest()
                    for directory in (sdk / "include/platform/ros/c", sdk / "include/platform/ros1/c", sdk / "include/platform/ros2/c")
                    for path in directory.rglob("*.h")}
    report = {"format": "task02-current-generated-stream-replay-v1", "command": [str(x) for x in command],
              "exit_code": result.returncode, "input_sha256": pins, "C_dependencies_sha256": dependencies,
              "immutable_source_archive_sha256": ARCHIVE_SHA256, "current_source_denominator": len(selected),
              "default_authority": [], "public_sdk_published": False}
    (run / "work/replay.json").write_text(json.dumps(report, indent=2) + "\n")
    return result.returncode


if __name__ == "__main__":
    sys.exit(main())
