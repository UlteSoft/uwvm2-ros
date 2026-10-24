#!/usr/bin/env python3
"""@brief Execute a pinned standard-EH Lua contract with ordinary stdio only.

@details Consumes an existing genuine compiler/validator receipt without
changing or relabeling it. Both the artifact and Runtime must still match that
receipt. The separate result credits Guest behavior only, not an xmake build,
positive ROS delivery or a complete upstream test suite. All work requires the
original Linux finite owner; no host Lua process executes the application.
@see build_guest.py
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess


def _pin(path):
    path = path.resolve(strict=True)
    with path.open("rb") as stream:
        return dict(path=str(path), bytes=path.stat().st_size,
                    sha256=hashlib.file_digest(stream, "sha256").hexdigest())


def _main():
    parser = argparse.ArgumentParser(description=__doc__)
    for key in ("build", "runtime", "out"):
        parser.add_argument("--" + key, type=Path, required=True)
    parser.add_argument("--build-sha256", required=True)
    args = parser.parse_args()
    if os.uname().sysname != "Linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("the original admitted Linux finite owner is required")
    build_pin = _pin(args.build)
    if build_pin["sha256"] != args.build_sha256:
        raise ValueError("selected genuine Lua build receipt changed")
    build = json.loads(args.build.read_bytes())
    if (build["status"] != "BUILT_VALIDATED_NOT_EXECUTED" or build["backend"] not in (1, 2)
            or build["exception_encoding"] != "exnref-try-table" or build["default_authority"] != []
            or build["profile"] != dict(target="wasm32-wasip1", core_baseline="3.0",
                exceptions=True, threads=False, startup="command")):
        raise ValueError("exact standard-EH default-denial Lua contract build required")
    artifact = Path(build["artifact"]["path"])
    runtime_pin = _pin(args.runtime)
    if _pin(artifact) != build["artifact"] or runtime_pin not in build["tools"]:
        raise ValueError("actual Lua artifact or originally selected Runtime changed")
    if any(_pin(Path(row["path"])) != row for row in build["source_inputs"]):
        raise ValueError("retained actual Lua build inputs changed")
    args.out.mkdir(parents=True, exist_ok=False)
    argv = [str(args.runtime), "--runtime-int", "-Rct", "0", "--wasm-feature-enable-exceptions",
            "--wasm-set-main-module-name", "main", "--wasip1-global-disable",
            "--wasip1-global-noinherit-system-environment", "--wasip1-single-create", "main",
            "--wasip1-single-enable", "main", "--wasip1-single-hide-host-api", "main",
            "--wasip1-single-noinherit-system-environment", "main"]
    for right in ("realtime-clock", "monotonic-clock", "process-clock", "thread-clock",
                  "random", "proc-raise", "sched-yield", "stdio-full-rights"):
        argv += ["--wasip1-single-deny", "main", right]
    argv += ["--run", str(artifact)]
    with (args.out / "stdout").open("xb") as stdout, (args.out / "stderr").open("xb") as stderr:
        completed = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, check=False)
    passed = completed.returncode == 0 and (args.out / "stdout").read_bytes() == b"PASS_LUA_REAL_GUEST_CHECKS=24\n"
    result = dict(format="task04-pinned-lua-standard-eh-guest-v1", status="PASS" if passed else "FAILED_RETAINED",
                  build=build_pin, artifact=_pin(artifact), runtime=runtime_pin, argv=argv,
                  exit=completed.returncode, stdout=_pin(args.out / "stdout"), stderr=_pin(args.out / "stderr"),
                  guest_checks=24 if passed else 0, xmake_build_credit=0, positive_ros_credit=0,
                  original_upstream_suite_credit=0, default_authority=[])
    (args.out / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    if not passed:
        raise ValueError("actual default-denial Lua Guest contract failed; original streams retained")


if __name__ == "__main__":
    _main()
