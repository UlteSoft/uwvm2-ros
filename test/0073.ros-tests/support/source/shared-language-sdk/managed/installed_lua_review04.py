#!/usr/bin/env python3
"""@brief Build and run one Lua backend using an existing read-only installed SDK.

@details The explicit SDK/input closure remains unchanged; all xmake state,
interpreter compilation, products and logs belong to a fresh admitted run.
Standard-EH validation, complete backend imports and one real Guest denial
outcome are independent gates. No host Lua application or positive ROS grant
is used. This recipe requires the original Linux finite owner and common lock.
@see lua_xmake_guest.py
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def _pin(path):
    path = path.resolve(strict=True)
    with path.open("rb") as stream:
        return dict(path=str(path), bytes=path.stat().st_size,
                    sha256=hashlib.file_digest(stream, "sha256").hexdigest())


def _main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("sdk", "inputs", "runtime", "validator", "xmake", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--inputs-sha256", required=True)
    parser.add_argument("--backend", choices=("1", "2"), required=True)
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("the original admitted Linux finite owner is required")
    sdk = args.sdk.resolve(strict=True)
    if any((sdk / name).exists() for name in ("origin", "test", "tests", "sdk", "include/roswasm")):
        raise ValueError("explicit isolated canonical installed SDK required")
    if _pin(args.inputs)["sha256"] != args.inputs_sha256:
        raise ValueError("selected actual offline closure changed")
    manifest = sdk / "share/wasm-profiles.json"
    before = [_pin(sdk / row["path"]) for row in json.loads(manifest.read_bytes())["profiles"][0]["files"]]
    before += [_pin(path) for path in (manifest, args.inputs, args.runtime, args.validator, args.xmake)]
    args.out.mkdir(parents=True, exist_ok=False)
    project = args.out / "project"
    project.mkdir()
    commands = args.out / "commands"
    commands.mkdir()
    report = dict(format="task04-existing-installed-lua-backend-v1", status="STARTED", sdk=str(sdk),
                  backend=int(args.backend), commands=[], source_and_tool_pins=before,
                  built=0, validated=0, guest=0, positive_ros=0, original_upstream_suite=0, default_authority=[])

    def save():
        (args.out / "results.json").write_text(json.dumps(report, indent=2) + "\n")

    def run(label, argv):
        folder = commands / label
        folder.mkdir()
        argv = list(map(str, argv))
        with (folder / "stdout").open("xb") as stdout, (folder / "stderr").open("xb") as stderr:
            completed = subprocess.run(argv, cwd=project, stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr,
                env=dict(os.environ, XMAKE_STATS="false", XMAKE_SKIP_HISTORY="y"), check=False)
        report["commands"].append(dict(label=label, argv=argv, cwd=str(project), exit=completed.returncode,
            stdout=_pin(folder / "stdout"), stderr=_pin(folder / "stderr")))
        save()
        if completed.returncode:
            raise RuntimeError("actual existing installed Lua step failed: " + label)
        return (folder / "stdout").read_text()

    def xmake(task):
        return [args.xmake, task, "-P", project, "-F", sdk / "example/xmake.lua"]

    try:
        run("xmake-version", [args.xmake, "--version"])
        run("validator-version", [args.validator, "--version"])
        run("configure", [*xmake("f"), "-c", "--yes", "--ros-sdk-platforms=ros", "--ros-sdk-languages=lua",
            "--sdk-example-platform=ros", "--sdk-example-language=lua", "--sdk-example=default-denied-ros" + args.backend,
            "--sdk-target=wasm32-wasip1", "--sdk-startup=command", "--sdk-exceptions=y", "--sdk-threads=n",
            "--sdk-offline=y", "--sdk-python=" + str(Path(sys.executable).resolve()), "--sdk-toolchain=/usr",
            "--sdk-profile-manifest=" + str(manifest), "--ros-sdk-lua-runtime=lua54-wasi-eh",
            "--ros-sdk-managed-backend=" + args.backend, "--ros-sdk-managed-inputs=" + str(args.inputs),
            "--ros-sdk-managed-inputs-sha256=" + args.inputs_sha256])
        product = args.out / "example"
        run("build", [*xmake("sdk-example-build"), "--output=" + str(product)])
        receipt_path = product / "build-receipt.json"
        receipt = json.loads(receipt_path.read_bytes())
        if receipt["status"] != "BUILT_NOT_EXECUTED" or len(receipt["artifacts"]) != 1:
            raise ValueError("one actual xmake-built Lua core-Wasm artifact required")
        artifact = Path(receipt["artifacts"][0]["path"])
        report.update(built=1, artifact=_pin(artifact), build_receipt=_pin(receipt_path))
        run("validate", [args.validator, "validate", artifact])
        report["validated"] = 1
        run("imports", [sys.executable, "-I", "-S", "-B", sdk / "tools/check_wasm_imports.py", artifact,
            "--backend", args.backend, "--allow-module", "wasi_snapshot_preview1", "--require-all",
            "--report", args.out / "imports.json"])
        output = run("guest", [*xmake("sdk-example-run"), "--receipt=" + str(receipt_path), "--runtime=" + str(args.runtime)])
        if output.count("PASS_ROS_DEFAULT_DENIED") != 1:
            raise ValueError("one actual default-denial Lua Guest outcome required")
        if any(_pin(Path(row["path"])) != row for row in before):
            raise ValueError("original installed SDK/source/tool input changed")
        report.update(status="ACTUAL_INSTALLED_LUA_BUILD_VALIDATE_GUEST_PASS", guest=1)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        save()


if __name__ == "__main__":
    _main()
