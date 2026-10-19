#!/usr/bin/env python3
"""Prepare the closed24 six-language delivery selections with22 real builds.

Root runs this serially inside the verified64GiB lane after the full source
freeze. It compiles20 non-JavaScript Guests and two retained C relay Guests;
positive/absent JavaScript selections deliberately share the same relay bytes.
No Guest, Node application, native ROS peer or Gateway is executed here.
Artifacts are admitted only after binary import inspection and fresh full
validation. Tool package and standard-library roots are explicit immutable
inputs; no package download or inherited compiler/ROS environment is allowed.
"""
import argparse
import importlib.util
import json
import os
from pathlib import Path
import shutil
import sys
import traceback
from types import SimpleNamespace

sys.dont_write_bytecode = True
from lane import Lane, add_arguments, atomic_json
from protocol import LANGUAGES, require
from runtime import digest


def load(source, relative, name):
    spec = importlib.util.spec_from_file_location(name, source/relative)
    module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module); return module


def closure(root):
    """Record all package inputs, including internal symlink targets, once.

    A root is caller-selected before compilation. An escaping link is refused;
    it cannot silently replace a target standard-library/package dependency.
    These input inventories do not claim that every instruction of every short
    compiler child was sampled or independently mapped as a native executable.
    """
    root = root.resolve(strict=True); require(root.is_dir(), "explicit tool package root required")
    result = {}
    for path in sorted(root.rglob("*")):
        name = str(path.relative_to(root))
        if path.is_symlink():
            target = path.resolve(strict=True); require(target.is_relative_to(root), "tool package link escapes qualified root")
            result[name] = {"link": os.readlink(path), "target": str(target.relative_to(root))}
        elif path.is_file(): result[name] = {"sha256": digest(path), "bytes": path.stat().st_size}
    require(result, "empty tool package root"); return result


def make_recipe(lane, args, language, version, mode, symbols, guards):
    """Keep standalone _start tutorials distinct from CRT main programs."""
    rowdir = lane.out/(language+"-ros"+str(version)+"-"+mode); rowdir.mkdir()
    artifact = rowdir/"guest.wasm"; cwd = lane.source; copied = None; archive = None; rust_flags = None
    env = dict(lane.clean); env.update(HOME=str(lane.out/"home"), XDG_CACHE_HOME=str(lane.out/"cache"),
        CARGO_HOME=str(lane.out/"cargo-home"), CARGO_NET_OFFLINE="true", RUSTUP_AUTO_INSTALL="0", CARGO_BUILD_JOBS="1",
        RUSTC_WRAPPER="", RUSTC_WORKSPACE_WRAPPER="", CARGO_BUILD_RUSTC_WRAPPER="", CARGO_BUILD_RUSTC_WORKSPACE_WRAPPER="",
        GOCACHE=str(lane.out/"go-cache"), GOMODCACHE=str(lane.out/"go-mod-cache"), GOPROXY="off", GOSUMDB="off", GOENV="off", GOTOOLCHAIN="local", GOMAXPROCS="1")
    if language in ("c", "cpp"):
        source = lane.source/("sdk/examples/"+language+"/06_runtime_delivery."+("c" if language=="c" else "cpp"))
        compiler = args.wasi_sdk/("bin/clang" if language=="c" else "bin/clang++")
        argv = [str(compiler), "--target=wasm32-wasip1", "--sysroot="+str(args.wasi_sdk/"share/wasi-sysroot"),
            "-std="+("c11" if language=="c" else "c++20"), "-O0", "-nostartfiles", "-ffreestanding", "-fno-builtin",
            "-Wl,--entry=_start", "-Wl,--export=_start", "-Wl,--export-dynamic", "-Wl,--threads=1",
            "-Wl,--allow-undefined-file="+str(symbols), "-I", str(lane.source/"sdk/include"),
            "-DROSWASM_EXAMPLE_ROS_VERSION="+str(version), "-DROSWASM_EXAMPLE_DEFAULT_DENIED="+str(int(mode=="absent")), str(source), "-o", str(artifact)]
        if language=="cpp": argv[4:4] = ["-stdlib=libc++", "-fno-exceptions", "-fno-rtti"]
    elif language=="javascript":
        source = lane.source/"sdk/javascript/relay.c"
        argv = [str(args.wasi_sdk/"bin/clang"), "--target=wasm32", "-std=c11", "-O0", "-nostdlib", "-ffreestanding", "-fno-builtin",
            "-DROSWASM_BRIDGE_ROS_VERSION="+str(version), "-Wl,--entry=_start", "-Wl,--export=_start", "-Wl,--export-memory",
            "-Wl,--threads=1", "-Wl,--initial-memory=131072", "-Wl,--max-memory=131072", "-Wl,-z,stack-size=65536",
            "-Wl,--allow-undefined-file="+str(symbols), str(source), "-o", str(artifact)]
    elif language=="rust":
        cwd = lane.out/"work"/language/"tests/ros_acceptance/sdk_clients/rust"
        source = cwd/"delivery.rs"; features = "ros"+str(version)+(",denied" if mode=="absent" else "")
        target = rowdir/"cargo-target"; copied = target/"wasm32-wasip1/release/roswasm_native_delivery.wasm"
        rust_flags, archive = guards.rust_wasi_link_inputs(args.wasi_sdk, args.rust_sysroot)
        env.update(RUSTC=str(args.rustc), RUSTFLAGS="", CARGO_ENCODED_RUSTFLAGS="\x1f".join(rust_flags))
        argv = [str(args.cargo), "build", "--offline", "-j1", "--release", "--manifest-path", str(cwd/"Cargo.toml"),
            "--target", "wasm32-wasip1", "--target-dir", str(target), "--no-default-features", "--features", features, "--lib"]
    elif language=="tinygo":
        cwd = lane.out/"work"/language/"tests/ros_acceptance/sdk_clients/tinygo"; source = cwd/"main.go"
        # This bounds only Go-managed memory; native LLVM allocations still
        # count against the unchanged aggregate compiler RSS guard.
        env.update(PATH=str(args.go.parent)+os.pathsep+os.defpath, GOROOT=str(args.go_root), TINYGOROOT=str(args.tinygo.resolve().parent.parent), GOMEMLIMIT="192MiB")
        argv = [str(args.tinygo), "build", "-p", "1", "-opt=0", "-target=wasi", "-scheduler=none", "-tags=ros"+str(version)+(",sdk_denied" if mode=="absent" else ""), "-o", str(artifact), "."]
    else:
        source = lane.source/("tests/ros_acceptance/sdk_clients/assemblyscript/ros"+str(version)+"-"+mode+".ts")
        argv = [str(args.node), str(args.asc_js), str(source), "--runtime", "incremental", "--exportRuntime", "--use",
            "abort=sdk/assemblyscript/api/abortGuest", "--outFile", str(artifact)]
    return {"language": language, "version": version, "mode": mode, "source": str(source), "argv": argv,
            "cwd": str(cwd), "artifact": artifact, "copied": copied, "rowdir": rowdir, "environment": env,
            "rust_target_libc": archive, "explicit_rustc_tokens": rust_flags}


def main():
    parser = argparse.ArgumentParser(description=__doc__); add_arguments(parser, workers=False)
    for name in ("wasi-sdk", "cargo", "rustc", "rust-sysroot", "tinygo", "go", "go-root", "node", "asc-js", "wasm-tools"):
        parser.add_argument("--"+name, type=Path, required=True)
    parser.add_argument("--toolchain-root", type=Path, action="append", required=True,
        help="Repeat explicit full compiler/package/standard-library roots; no external symlinks")
    parser.add_argument("--tinygo-startup-right", choices=("random", "monotonic-clock"), action="append", default=[],
        help="Explicit separately reviewed WASIp1 startup authority; never inferred from a failed run")
    args = parser.parse_args(); lane = None; status = "FAIL"
    try:
        lane = Lane(args, "sdk-native-delivery-products"); lane.report["mode"] = "prepare"
        roots = [path.resolve(strict=True) for path in args.toolchain_root]; require(len(roots)==len(set(roots)), "duplicate package roots")
        packages = {str(path): closure(path) for path in roots}; package_path = lane.out/"toolchain-inputs.json"; atomic_json(package_path, packages)
        lane.report["toolchain_packages"] = lane.pin(package_path)
        lane.report["toolchain_root_selections"]=[str(path) for path in roots]
        for name in ("wasi_sdk", "rust_sysroot", "go_root"):
            path = getattr(args, name).resolve(strict=True); require(any(path.is_relative_to(root) for root in roots), "standard-library root lacks full package inventory")
        tools = [args.wasi_sdk/"bin/clang", args.wasi_sdk/"bin/clang++", args.cargo, args.rustc, args.tinygo, args.go, args.node, args.asc_js, args.wasm_tools]
        for path in tools:
            path = path.resolve(strict=True); require(any(path.is_relative_to(root) for root in roots), "explicit tool/script lacks full selected package inventory"); lane.pin(path)
        compiler = load(lane.source, "sdk/tools/build_cgroup_matrix.py", "delivery_guards")
        inspector = load(lane.source, "sdk/tools/check_wasm_imports.py", "delivery_imports")
        controls = SimpleNamespace(expected_memory_max=68719476736, expected_swap_max="0", expected_leaf=lane.evidence.leaf,
            expected_device=lane.before["device"], expected_inode=lane.before["inode"], expected_boot=lane.before["boot_id"],
            expected_cpus=(lane.evidence.leaf/"cpuset.cpus.effective").read_text().strip(), rss_limit_mib=2048,
            cgroup_current_limit_mib=61440, timeout_seconds=600)
        for language in ("rust", "tinygo"):
            work = lane.out/"work"/language; (work/"tests/ros_acceptance/sdk_clients").mkdir(parents=True)
            shutil.copytree(lane.source/("tests/ros_acceptance/sdk_clients/"+language), work/("tests/ros_acceptance/sdk_clients/"+language))
            shutil.copytree(lane.source/("sdk/"+language), work/("sdk/"+language))
        # All copied input bytes remain pinned; Cargo.lock is the sole permitted
        # newly generated source-adjacent file, confined to disposable output.
        work_inputs = {str(path): lane.pin(path) for path in (lane.out/"work").rglob("*") if path.is_file()}
        schema = json.loads((lane.source/"sdk/abi/abi-v1.json").read_text()); symbols = {}
        for version in (1, 2):
            file = lane.out/("allowed-ros"+str(version)+".txt")
            file.write_text("\n".join("uwvm_ros"+str(version)+"_"+row["name"] for row in schema["modules"]["uwvm_ros"+str(version)+"_v1"]["functions"])+"\n")
            symbols[version] = file; lane.pin(file)
        products = []; lane.report["products"] = products
        for language in LANGUAGES:
            for version in (1, 2):
                for mode in (("positive",) if language=="javascript" else ("positive", "absent")):
                    recipe = make_recipe(lane, args, language, version, mode, symbols[version], compiler); record = {
                        "language": language, "version": version, "mode": mode, "status": "RUNNING", "actual_ROS_delivery": "NOT_EXECUTED"}
                    products.append(record); lane.save()
                    if language=="rust":
                        require(lane.pin(recipe["rust_target_libc"]["path"])==recipe["rust_target_libc"], "selected Rust target libc changed")
                        record.update(rust_target_libc=recipe["rust_target_libc"], explicit_rustc_tokens=recipe["explicit_rustc_tokens"])
                    actual = compiler.guarded_command(recipe["argv"], Path(recipe["cwd"]), recipe["environment"], recipe["rowdir"], "compile", controls)
                    record["compile"] = actual; require(not actual["guard_error"] and actual["returncode"]==0, "actual delivery Guest compilation failed")
                    if recipe["copied"]: shutil.copyfile(recipe["copied"], recipe["artifact"])
                    artifact = recipe["artifact"]; before = lane.pin(artifact); data = artifact.read_bytes()
                    contract = inspector.check(data, str(version), ("wasi_snapshot_preview1",), language=="javascript")
                    if language=="javascript": compiler.check_relay_imports(contract)
                    validation = compiler.guarded_command([str(args.wasm_tools), "validate", str(artifact)], recipe["rowdir"], recipe["environment"], recipe["rowdir"], "validate", controls)
                    require(not validation["guard_error"] and validation["returncode"]==0 and lane.pin(artifact)==before, "fresh full validation/product binding failed")
                    if language=="rust":
                        _, record["rust_target_libc_after"] = compiler.rust_wasi_link_inputs(args.wasi_sdk)
                        require(record["rust_target_libc_after"]==record["rust_target_libc"], "Rust target libc changed around compilation/validation")
                    rights = list(dict.fromkeys(args.tinygo_startup_right)) if language=="tinygo" else []
                    record.update(status="PREPARED", artifact=before, compiler_returncode=0, signature_status="PASS", import_contract=contract,
                        full_validation_returncode=0, validation=validation, source_inputs_preserved=all(lane.pin(path)==pin for path,pin in work_inputs.items()),
                        input_pins=[lane.pin(lane.source/"sdk/abi/abi-v1.json"), *[lane.pin(path.resolve()) for path in tools], lane.report["toolchain_packages"],
                            *([record["rust_target_libc"]] if language=="rust" else [])],
                        explicit_wasi_startup_rights=rights, startup_rights_reviewed=bool(rights), toolchain_sdk_closure="Explicit whole selected package/stdlib inputs pinned; short tool-child map coverage remains sampled")
                    require(record["source_inputs_preserved"], "copied SDK/Guest inputs changed")
                    lane.save()
                    if language=="javascript": products.append({**record, "mode": "absent", "same_relay_bytes": True}); lane.save()
        require(len(products)==24 and all(row["status"]=="PREPARED" for row in products), "closed24 delivery products incomplete")
        added={str(path) for path in (lane.out/"work").rglob("*") if path.is_file()}-set(work_inputs)
        require(added<={str(lane.out/"work/rust/tests/ros_acceptance/sdk_clients/rust/Cargo.lock")},"unexpected disposable source-adjacent file added")
        require({str(path):closure(path) for path in roots}==packages, "tool/standard-library package inputs changed")
        lane.report.update(compiler_invocations=22, prepared_selections=24, all_compilation_serial=True, actual_ROS_delivery="NOT_EXECUTED"); status="PREPARED"
    except BaseException as error:
        if lane is not None: lane.report.update(error=repr(error), traceback=traceback.format_exc())
        else: raise
    finally:
        if lane is not None:
            # Evidence understands PASS/FAIL; the public preparation status is
            # restored only after its complete preservation/OOM/cleanup PASS.
            actual_status = lane.close("PASS" if status=="PREPARED" else "FAIL")
            status = "PREPARED" if status=="PREPARED" and actual_status=="PASS" else "FAIL"
            lane.report["status"] = status; lane.save()
    return 0 if status=="PREPARED" else 1


if __name__=="__main__": raise SystemExit(main())
