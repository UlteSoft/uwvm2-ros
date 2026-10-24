#!/usr/bin/env python3
"""@brief Build and run one actual AS entry from an existing installed SDK.

@details The installed SDK stays an explicit read-only source input. Xmake's
project/cache and every new product live in this admitted run. Actual Node/AS,
validator and Runtime bytes are pinned; no repository checkout or host-language
application substitutes for the resulting Wasm. The external tool tree must
retain Node's genuine node_modules layout.
@see ../../../../../../sdk/origin/language/managed/EXAMPLES.md
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def _pin(path):
    path = path.resolve(strict=True)
    with path.open("rb") as stream:
        return {"path": str(path), "bytes": path.stat().st_size,
                "sha256": hashlib.file_digest(stream, "sha256").hexdigest()}


def _main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("sdk", "node-modules", "node", "validator", "runtime", "xmake", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--owned-fixes", type=Path,
                        help="Exact owned integration capsule; create a freshly pinned installed candidate")
    parser.add_argument("--backend", choices=("1", "2"), required=True)
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("the original admitted Linux owner is required")
    sdk = args.sdk.resolve(strict=True)
    if any((sdk / name).exists() for name in ("origin", "test", "tests", "sdk")):
        raise ValueError("an installed SDK without repository sources/tests is required")
    if args.node_modules.name != "node_modules":
        raise ValueError("the real Node dependency tree must retain its node_modules directory name")
    args.out.mkdir(parents=True, exist_ok=False)
    original_sdk = sdk
    fixes = []
    if args.owned_fixes is not None:
        selected = ("tools/shared_managed04/profile.py", "tools/xmake/examples/managed_backend.py",
                    "tools/xmake/examples/managed.lua")
        capsule = args.owned_fixes.resolve(strict=True)
        if any(p.is_symlink() for p in capsule.rglob("*")):
            raise ValueError("ordinary exact owned integration capsule required")
        actual = {p.relative_to(capsule).as_posix() for p in capsule.rglob("*") if p.is_file()}
        if actual == set(selected) | {"tools/xmake/managed.lua"}:
            selected = (*selected, "tools/xmake/managed.lua")
        elif actual != set(selected):
            raise ValueError("exact three-file legacy or four-file managed integration capsule required")
        original_manifest = json.loads((sdk / "share/wasm-profiles.json").read_bytes())
        if len(original_manifest["profiles"]) != 1:
            raise ValueError("one explicit private AS source provider required")
        candidate = args.out / "SDK installed with Unicode-λ"
        candidate.mkdir()
        for row in original_manifest["profiles"][0]["files"]:
            relative = Path(row["path"])
            if relative.is_absolute() or ".." in relative.parts:
                raise ValueError("confined original installed source pin required")
            source = sdk / relative
            if {k: _pin(source)[k] for k in ("bytes", "sha256")} != {k: row[k] for k in ("bytes", "sha256")}:
                raise ValueError("the original installed SDK source pin differs")
            target = candidate / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            replacement = capsule / relative if row["path"] in selected else source
            shutil.copyfile(replacement, target)
            if row["path"] in selected:
                fixes.append(dict(original=_pin(source), replacement=_pin(replacement)))
            actual = _pin(target)
            row.update(bytes=actual["bytes"], sha256=actual["sha256"])
        if len(fixes) != len(selected):
            raise ValueError("every exact owned fix must already be an exported provider input")
        (candidate / "share/wasm-profiles.json").write_text(json.dumps(original_manifest, indent=2) + "\n")
        sdk = candidate.resolve(strict=True)
    project = args.out / "project"
    project.mkdir()
    commands = args.out / "commands"
    commands.mkdir()
    manifest_path = sdk / "share/wasm-profiles.json"
    manifest = json.loads(manifest_path.read_bytes())
    before = [_pin(sdk / row["path"]) for row in manifest["profiles"][0]["files"]]
    template = json.loads((sdk / "share/as-inputs.json").read_bytes())
    tools = [_pin(path) for path in (args.node, args.validator, args.runtime, args.xmake)]
    inputs = {str(path.resolve()): _pin(path) for path in args.node_modules.rglob("*") if path.is_file()}
    for path in (args.node, args.validator):
        inputs[str(path.resolve())] = _pin(path)
    template.update(paths={"node": str(args.node), "asc": str(args.node_modules / "assemblyscript/bin/asc.js"),
        "validator": str(args.validator)}, directories=[str(args.node_modules)], files=list(inputs.values()))
    input_path = args.out / "actual-offline-inputs.json"
    input_path.write_text(json.dumps(template, indent=2) + "\n")
    report = {"format": "task04-installed-as-example-v1", "status": "STARTED", "commands": [],
        "sdk": str(sdk), "profile_manifest": _pin(manifest_path), "source_pins": before,
        "original_installed_sdk": str(original_sdk), "owned_fix_inputs": fixes,
        "tools": tools, "selected_inputs": _pin(input_path), "backend": int(args.backend),
        "built": 0, "validated": 0, "guest": 0, "native_ros": 0, "default_authority": []}

    def save():
        (args.out / "results.json").write_text(json.dumps(report, indent=2) + "\n")

    def run(name, arguments):
        folder = commands / name
        folder.mkdir()
        with (folder / "stdout").open("xb") as stdout, (folder / "stderr").open("xb") as stderr:
            completed = subprocess.run(list(map(str, arguments)), cwd=project, stdin=subprocess.DEVNULL,
                stdout=stdout, stderr=stderr, env=dict(os.environ, XMAKE_STATS="false", XMAKE_SKIP_HISTORY="y"), check=False)
        report["commands"].append({"name": name, "argv": list(map(str, arguments)), "cwd": str(project),
            "exit": completed.returncode, "stdout": _pin(folder / "stdout"), "stderr": _pin(folder / "stderr")})
        save()
        if completed.returncode:
            raise RuntimeError("actual installed example step failed: " + name)
        return (folder / "stdout").read_text()

    try:
        def xmake(task):
            # Xmake selects its task before parsing that task's options.
            return [args.xmake, task, "-P", project, "-F", sdk / "example/xmake.lua"]
        run("node-version", [args.node, "--version"])
        run("xmake-version", [args.xmake, "--version"])
        run("validator-version", [args.validator, "--version"])
        run("asc-version", [args.node, "--max-old-space-size=256",
            args.node_modules / "assemblyscript/bin/asc.js", "--version"])
        run("configure", [*xmake("f"), "-c", "--yes", "--ros-sdk-platforms=ros", "--ros-sdk-languages=as",
            "--sdk-example-platform=ros", "--sdk-example-language=as", "--sdk-example=telemetry-ros" + args.backend,
            "--sdk-target=wasm32-wasip1", "--sdk-startup=command", "--sdk-exceptions=n", "--sdk-threads=n",
            "--sdk-offline=y", "--sdk-python=" + str(Path(sys.executable).resolve()), "--sdk-toolchain=/usr",
            "--sdk-profile-manifest=" + str(manifest_path), "--ros-sdk-as-runtime=assemblyscript",
            "--ros-sdk-as-runtime-version=0.28.20", "--ros-sdk-managed-backend=" + args.backend,
            "--ros-sdk-managed-inputs=" + str(input_path), "--ros-sdk-managed-inputs-sha256=" + _pin(input_path)["sha256"]])
        output = args.out / "example"
        run("build", [*xmake("sdk-example-build"), "--output=" + str(output)])
        receipt_path = output / "build-receipt.json"
        receipt = json.loads(receipt_path.read_bytes())
        if receipt["status"] != "BUILT_NOT_EXECUTED" or len(receipt["artifacts"]) != 1:
            raise ValueError("one genuine core-Wasm build artifact is required")
        artifact = Path(receipt["artifacts"][0]["path"])
        report.update(built=1, artifact=_pin(artifact), build_receipt=_pin(receipt_path))
        run("validate", [args.validator, "validate", artifact])
        report["validated"] = 1
        run("imports", [sys.executable, "-I", "-S", "-B", sdk / "tools/check_wasm_imports.py", artifact,
            "--backend", args.backend, "--allow-module", "wasi_snapshot_preview1", "--report", args.out / "imports.json"])
        text = run("guest", [*xmake("sdk-example-run"), "--receipt=" + str(receipt_path), "--runtime=" + str(args.runtime)])
        if text.count("PASS_ROS_DEFAULT_DENIED") != 1:
            raise ValueError("one actual self-checked Guest default-denial result is required")
        if any(_pin(Path(row["path"])) != row for row in [*before, *tools, *inputs.values()]):
            raise ValueError("actual installed source or tool dependency changed")
        report.update(status="ACTUAL_INSTALLED_AS_BUILD_VALIDATE_GUEST_PASS", guest=1)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        save()


if __name__ == "__main__":
    _main()
