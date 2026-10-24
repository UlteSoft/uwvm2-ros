#!/usr/bin/env python3
"""Actual AssemblyScript compile/whole-Wasm validation of the clock SDK app."""
import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import sys
import tarfile


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def main():
    p = argparse.ArgumentParser(description=__doc__)
    for name in ("core-source", "prepared", "node", "asc"):
        p.add_argument("--" + name, type=Path, required=True)
        p.add_argument("--" + name + "-sha256", required=True)
    p.add_argument("--import-sdk-root", type=Path, required=True)
    for name in ("checker", "helper", "service-catalog", "business-catalog"):
        p.add_argument("--import-" + name + "-sha256", required=True)
    args = p.parse_args()
    assert os.environ["UWVM_RESOURCE_HEAVY_LOCK_HELD"] == "1"
    run = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    work = run / "work"
    published = args.import_sdk_root
    assert published.is_absolute() and published.resolve(strict=True) == published
    import_inputs = {}
    for role, relative in (("checker", "tools/check_wasm_imports.py"),
                           ("helper", "tools/rmw_service_contract.py"),
                           ("service_catalog", "share/abi/rmw-service-v1.json"),
                           ("business_catalog", "share/abi/abi-v1.json")):
        path = published / relative
        assert path.resolve(strict=True) == path and not path.is_symlink()
        assert path.is_file() and path.stat().st_size <= 2 << 20
        pin = {"bytes": path.stat().st_size, "sha256": digest(path)}
        assert pin["sha256"] == getattr(args, "import_" + role + "_sha256")
        import_inputs[relative] = pin
    assert args.core_source.is_relative_to(run / "inputs") and digest(args.core_source) == args.core_source_sha256
    receipt = args.prepared / "clock-sdk-preparation.json"
    assert digest(receipt) == args.prepared_sha256
    assert digest(args.node) == args.node_sha256 and digest(args.asc) == args.asc_sha256
    compiler_root = args.asc.resolve(strict=True).parents[2]
    compiler_dependencies = {}
    for path in sorted(compiler_root.rglob("*")):
        if path.is_file():
            assert path.resolve(strict=True).is_relative_to(compiler_root)
            assert path.stat().st_size <= 128 << 20 and len(compiler_dependencies) < 1024
            compiler_dependencies[str(path.relative_to(compiler_root))] = {"bytes": path.stat().st_size, "sha256": digest(path)}
    original = json.loads(receipt.read_bytes())
    sdk = work / "sdk"
    assert not published.is_relative_to(sdk) and not sdk.is_relative_to(published)
    sdk.mkdir()
    for name, pin in original["products"].items():
        if "/as/" not in name:
            continue
        source = args.prepared / "sdk" / name
        assert source.stat().st_size == pin["bytes"] and digest(source) == pin["sha256"]
        dest = sdk / name
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, dest)
    captured = {}
    with tarfile.open(args.core_source, "r:") as archive:
        members = archive.getmembers()
        assert len(members) < 100
        total = 0
        for member in members:
            path = PurePosixPath(member.name)
            assert not path.is_absolute() and ".." not in path.parts and (member.isfile() or member.isdir())
            if member.isdir():
                continue
            prefix = "sdk/origin/assemblyscript/"
            assert member.name.startswith(prefix) and member.size <= 1 << 20
            relative = member.name[len(prefix):]
            assert relative not in captured
            total += member.size
            assert total <= 1 << 20
            raw = archive.extractfile(member).read(member.size + 1)
            assert len(raw) == member.size
            dest = sdk / "include/platform/ros2/as" / relative
            dest.parent.mkdir(parents=True, exist_ok=True)
            with dest.open("xb") as stream:
                stream.write(raw)
            captured[relative] = hashlib.sha256(raw).hexdigest()
    commands = []
    summary = {"format": "uwvm-task09-real-clock-as-v1", "status": "RUNNING", "commands": commands,
               "prepared_receipt_sha256": args.prepared_sha256, "core_source_sha256": args.core_source_sha256,
               "shared_core_sources": captured, "node_sha256": args.node_sha256, "asc_sha256": args.asc_sha256,
               "compiler_dependency_root": str(compiler_root), "compiler_dependencies": compiler_dependencies,
               "published_import_sdk_root": str(published), "published_import_inputs": import_inputs,
               "guest_executions": 0, "native_kit_ros_executions": 0, "xmake_executed": False,
               "whole_installed_sdk_qualified": False, "default_authority": []}
    env = dict(os.environ)
    for name in ("NODE_OPTIONS", "NODE_PATH", "NODE_COMPILE_CACHE"):
        env.pop(name, None)
    def command(name, argv):
        row = {"name": name, "argv": [str(v) for v in argv]}
        commands.append(row)
        with (work / (name + ".stdout")).open("xb") as stdout, (work / (name + ".stderr")).open("xb") as stderr:
            result = subprocess.run(row["argv"], env=env, cwd=sdk, stdout=stdout, stderr=stderr, timeout=120)
        row["exit_code"] = result.returncode
        assert result.returncode == 0, "actual operation failed: " + name
    try:
        command("asc-version", [args.node, "--max-old-space-size=256", args.asc, "--version"])
        assert (work / "asc-version.stdout").read_text().strip() == "Version 0.28.20"
        artifact = work / "clock-program.wasm"
        application = "example/platform/nvidia-isaac-sim/as/clock-program/main.ts"
        command("full-clock-application", [args.node, "--max-old-space-size=256", args.asc, application,
                                          "--outFile", artifact, "--runtime", "incremental", "--optimize", "--debug",
                                          "--use", "abort=include/platform/ros2/as/api/abortGuest"])
        command("whole-wasm-validation", ["/usr/bin/wasm-validate", artifact])
        checked_imports = work / "authoritative-imports.json"
        command("authoritative-service-import-signatures", [sys.executable, "-I", "-S", "-B",
            published / "tools/check_wasm_imports.py", artifact, "--backend", "2", "--ros2-rmw-service",
            "--allow-module", "wasi_snapshot_preview1", "--report", checked_imports])
        summary["authoritative_import_report"] = {"path": str(checked_imports),
            "bytes": checked_imports.stat().st_size, "sha256": digest(checked_imports)}
        command("actual-imports", ["/usr/bin/wasm-objdump", "-x", artifact])
        imports = (work / "actual-imports.stdout").read_text()
        assert 'uwvm_ros2_rmw_srv_v1.' in imports and 'uwvm_ros2_v1.capability_open' in imports
        assert 'wasi_snapshot_preview1.args_get' in imports and 'wasi_snapshot_preview1.fd_write' in imports
        summary["artifact"] = {"path": str(artifact), "bytes": artifact.stat().st_size, "sha256": digest(artifact)}
        summary["status"] = "BUILT_AND_VALIDATED_NOT_GUEST_OR_NATIVE_EXECUTED"
    except Exception as error:
        summary["status"] = "FAILED_RETAINED"
        summary["error"] = repr(error)
    assert digest(args.node) == args.node_sha256 and digest(args.asc) == args.asc_sha256 and digest(receipt) == args.prepared_sha256
    for name, pin in compiler_dependencies.items():
        path = compiler_root / name
        assert path.stat().st_size == pin["bytes"] and digest(path) == pin["sha256"]
    for relative, pin in captured.items():
        assert digest(sdk / "include/platform/ros2/as" / relative) == pin
    for relative, pin in import_inputs.items():
        path = published / relative
        assert path.stat().st_size == pin["bytes"] and digest(path) == pin["sha256"]
    (work / "clock-as.json").write_text(json.dumps(summary, indent=2) + "\n")
    print(json.dumps({key: summary[key] for key in ("status", "guest_executions", "native_kit_ros_executions", "xmake_executed")}))
    return 0 if summary["status"] != "FAILED_RETAINED" else 1


if __name__ == "__main__":
    raise SystemExit(main())
