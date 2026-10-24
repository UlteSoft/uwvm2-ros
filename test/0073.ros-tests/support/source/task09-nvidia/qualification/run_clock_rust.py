#!/usr/bin/env python3
"""Compile the real clock SDK/application with retained genuine Rust/WASI tools."""
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
    p.add_argument("--core-source", type=Path, required=True)
    p.add_argument("--core-source-sha256", required=True)
    p.add_argument("--prepared", type=Path, required=True)
    p.add_argument("--prepared-sha256", required=True)
    p.add_argument("--rust-root", type=Path, required=True)
    p.add_argument("--rustc-sha256", required=True)
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
    original = json.loads(receipt.read_bytes())
    rustc = args.rust_root.resolve(strict=True) / "bin/rustc"
    assert digest(rustc) == args.rustc_sha256
    compiler_dependencies = {}
    selected_tools = list((args.rust_root / "lib/rustlib/wasm32-wasip1/lib").glob("*"))
    selected_tools += list((args.rust_root / "lib").glob("*.so*"))
    selected_tools += list((args.rust_root / "lib/rustlib/x86_64-unknown-linux-gnu/bin").glob("*"))
    for path in sorted(selected_tools):
        if path.is_file():
            assert path.resolve(strict=True).is_relative_to(args.rust_root.resolve(strict=True))
            assert len(compiler_dependencies) < 512
            compiler_dependencies[str(path.relative_to(args.rust_root))] = {"bytes": path.stat().st_size, "sha256": digest(path)}
    sdk = work / "sdk"
    assert not published.is_relative_to(sdk) and not sdk.is_relative_to(published)
    sdk.mkdir()
    for name, pin in original["products"].items():
        if "/rs/" not in name:
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
            prefix = "sdk/origin/rust/"
            assert member.name.startswith(prefix) and member.size <= 1 << 20
            relative = member.name[len(prefix):]
            assert relative not in captured
            total += member.size
            assert total <= 1 << 20
            raw = archive.extractfile(member).read(member.size + 1)
            assert len(raw) == member.size
            dest = sdk / "include/platform/ros2/rs" / relative
            dest.parent.mkdir(parents=True, exist_ok=True)
            with dest.open("xb") as stream:
                stream.write(raw)
            captured[relative] = hashlib.sha256(raw).hexdigest()
    commands = []
    summary = {"format": "uwvm-task09-real-clock-rust-v1", "status": "RUNNING", "commands": commands,
               "prepared_receipt_sha256": args.prepared_sha256, "core_source_sha256": args.core_source_sha256,
               "shared_core_sources": captured, "rustc_sha256": args.rustc_sha256,
               "selected_compiler_target_dependencies": compiler_dependencies,
               "published_import_sdk_root": str(published), "published_import_inputs": import_inputs,
               "guest_executions": 0, "native_kit_ros_executions": 0, "xmake_executed": False,
               "whole_installed_sdk_qualified": False, "default_authority": []}
    env = dict(os.environ, CARGO_BUILD_JOBS="1", CARGO_NET_OFFLINE="true", RUSTFLAGS="", RUSTC_WRAPPER="", RUSTC_WORKSPACE_WRAPPER="")
    env["LD_LIBRARY_PATH"] = str(args.rust_root / "lib")
    output = work / "objects"
    output.mkdir()
    base = sdk / "include/platform/ros2/rs"
    def command(name, argv):
        row = {"name": name, "argv": [str(v) for v in argv]}
        commands.append(row)
        with (work / (name + ".stdout")).open("xb") as stdout, (work / (name + ".stderr")).open("xb") as stderr:
            result = subprocess.run(row["argv"], env=env, cwd=sdk, stdout=stdout, stderr=stderr, timeout=120)
        row["exit_code"] = result.returncode
        assert result.returncode == 0, "actual operation failed: " + name
    try:
        command("rust-version", [rustc, "--version", "--verbose"])
        assert "rustc 1.85.0 " in (work / "rust-version.stdout").read_text()
        common = [rustc, "--edition=2021", "--target=wasm32-wasip1", "-C", "panic=abort", "-C", "opt-level=s",
                  "-L", "dependency=" + str(output)]
        core = output / "libroswasm.rlib"
        owned = output / "libroswasm_owned_service.rlib"
        clock = output / "libroswasm_isaac_sim_clock.rlib"
        command("shared-core", [*common, "--crate-name", "roswasm", "--crate-type", "rlib", "--cfg", 'feature="alloc"',
                                "--cfg", 'feature="ros2"', base / "src/lib.rs", "-o", core])
        command("shared-retained-service", [*common, "--crate-name", "roswasm_owned_service", "--crate-type", "rlib",
                                           "--extern", "roswasm=" + str(core), base / "owned_service/src/lib.rs", "-o", owned])
        library = sdk / "include/platform/nvidia-isaac-sim/rs/clock-control/lib.rs"
        command("platform-sdk", [*common, "--crate-name", "roswasm_isaac_sim_clock", "--crate-type", "rlib",
                                 "--extern", "roswasm=" + str(core), "--extern", "roswasm_owned_service=" + str(owned), library, "-o", clock])
        artifact = work / "clock-program.wasm"
        application = sdk / "example/platform/nvidia-isaac-sim/rs/clock-program/main.rs"
        command("full-clock-application", [*common, "--crate-name", "clock_program", "--crate-type", "bin",
                                          "--extern", "roswasm=" + str(core), "--extern", "roswasm_isaac_sim_clock=" + str(clock),
                                          application, "-o", artifact])
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
        summary["artifact"] = {"path": str(artifact), "bytes": artifact.stat().st_size, "sha256": digest(artifact)}
        summary["status"] = "BUILT_AND_VALIDATED_NOT_GUEST_OR_NATIVE_EXECUTED"
    except Exception as error:
        summary["status"] = "FAILED_RETAINED"
        summary["error"] = repr(error)
    assert digest(rustc) == args.rustc_sha256 and digest(receipt) == args.prepared_sha256
    for name, pin in compiler_dependencies.items():
        path = args.rust_root / name
        assert path.stat().st_size == pin["bytes"] and digest(path) == pin["sha256"]
    for relative, pin in captured.items():
        assert digest(base / relative) == pin
    for relative, pin in import_inputs.items():
        path = published / relative
        assert path.stat().st_size == pin["bytes"] and digest(path) == pin["sha256"]
    (work / "clock-rust.json").write_text(json.dumps(summary, indent=2) + "\n")
    print(json.dumps({key: summary[key] for key in ("status", "guest_executions", "native_kit_ros_executions", "xmake_executed")}))
    return 0 if summary["status"] != "FAILED_RETAINED" else 1


if __name__ == "__main__":
    raise SystemExit(main())
