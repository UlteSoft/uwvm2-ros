#!/usr/bin/env python3
"""@brief Qualify one real Lua command through a relocated installed xmake SDK.

@details Uses the current shared distribution export map implementation for
ordinary SDK files, explicit external compiler inputs and the actual production
Lua builder. The fresh SDK contains no origin/tests or checkout-relative source
dependency. Results distinguish xmake build, standard validation, Guest default
denial and positive ROS integration. Only ordinary stdio is permitted.
@see ../../../../../../sdk/origin/language/managed/EXAMPLES.md
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def _pin(path, root=None):
    path = path.resolve(strict=True)
    with path.open("rb") as stream:
        return dict(path=path.relative_to(root).as_posix() if root else str(path), bytes=path.stat().st_size,
                    sha256=hashlib.file_digest(stream, "sha256").hexdigest())


def _save(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2) + "\n")


def _main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("source", "wasi-sdk", "toolchain-receipt", "validator", "runtime", "xmake", "archive",
                 "clang-format", "format-config", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    for name in ("toolchain-receipt", "clang-format", "format-config"):
        parser.add_argument("--" + name + "-sha256", required=True)
    parser.add_argument("--backend", choices=("1", "2"), required=True)
    parser.add_argument("--example", choices=("default-denied", "record-values"), default="default-denied")
    parser.add_argument("--records-package", type=Path)
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("original admitted Linux finite owner and common lock required")
    source = args.source.resolve(strict=True)
    origin = source / "sdk/origin"
    args.out.mkdir(parents=True, exist_ok=False)
    commands = args.out / "commands"
    commands.mkdir()
    report = dict(format="task04-actual-installed-lua-xmake-v1", status="STARTED", commands=[],
                  built=0, validated=0, guest=0, native_ros=0, original_upstream_suite=0, default_authority=[])

    def save():
        _save(args.out / "results.json", report)

    def run(label, argv, cwd=None):
        folder = commands / label
        folder.mkdir()
        argv = list(map(str, argv))
        with (folder / "stdout").open("xb") as stdout, (folder / "stderr").open("xb") as stderr:
            completed = subprocess.run(argv, cwd=cwd or args.out, stdin=subprocess.DEVNULL,
                stdout=stdout, stderr=stderr, env=dict(os.environ, XMAKE_STATS="false", XMAKE_SKIP_HISTORY="y"), check=False)
        report["commands"].append(dict(label=label, argv=argv, cwd=str(cwd or args.out), exit=completed.returncode,
            stdout=_pin(folder / "stdout"), stderr=_pin(folder / "stderr")))
        save()
        if completed.returncode:
            raise RuntimeError("actual relocated Lua step failed: " + label)
        return (folder / "stdout").read_text()

    try:
        for name in ("toolchain_receipt", "clang_format", "format_config"):
            if _pin(getattr(args, name))["sha256"] != getattr(args, name + "_sha256"):
                raise ValueError("exact selected tool/configuration pin differs: " + name)
        report["tools"] = [_pin(p) for p in (args.validator, args.runtime, args.xmake, args.clang_format)]
        sys.path.insert(0, str(source / "sdk/tools"))
        spec = importlib.util.spec_from_file_location("_lua_actual_distribution", source / "sdk/tools/prepare_distribution.py")
        distribution = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(distribution)
        original = args.out / "sdk-original"
        original.mkdir()
        delivery = distribution.Distribution(origin, original)
        for path in sorted((origin / "lua/include").rglob("*.lua")):
            delivery.add(path, "include/platform/ros/lua/" + path.relative_to(origin / "lua/include").as_posix())
        for name in ("build.py", "bridge.py", "stdlib_profile.py"):
            delivery.add(origin / "lua/tools" / name, "tools/platform/ros/lua/" + name)
        for name, relative in (("lib/native.c.in", "native.c.in"), ("embedding/guest_config.h", "guest_config.h")):
            delivery.add(origin / "lua" / name, "lib/platform/ros/lua/source/" + relative)
        for backend in ("1", "2"):
            delivery.add(origin / ("include/roswasm/abi/ros" + backend + ".h"),
                         "include/platform/ros" + backend + "/c/roswasm/abi/ros" + backend + ".h")
            delivery.add(origin / ("ros" + backend + "/abi.h"),
                         "include/platform/ros" + backend + "/c/roswasm/detail/ros" + backend + "/abi.h")
        for name in ("abi.h", "c_portability.h"):
            delivery.add(origin / "common" / name, "include/platform/ros/c/roswasm/detail/common/" + name)
        for path in sorted((origin / "tools/xmake").rglob("*")):
            if path.suffix in (".lua", ".py"):
                delivery.add(path, "tools/xmake/" + path.relative_to(origin / "tools/xmake").as_posix())
        for name in ("check_wasm_imports.py", "wasm_profiles.py", "shared_managed04/profile.py"):
            delivery.add(origin / "tools" / name, "tools/" + name)
        delivery.add(origin / "abi/abi-v1.json", "share/abi/abi-v1.json")
        delivery.add(origin / "examples/lua/owned_session.lua", "example/platform/ros/lua/lua/owned_session.lua")
        delivery.add(origin / "examples/lua/record_values.lua", "example/platform/ros/lua/lua/record_values.lua")
        delivery.add(origin / "tools/xmake/examples/managed_rules.lua", "example/platform/ros/lua/xmake.lua")
        delivery.add(origin / "lua/README.md", "share/platform/ros/lua/README.md")
        delivery.copy_sources()
        record_proof = None
        if args.example == "record-values":
            if args.records_package is None:
                raise ValueError("a real generated complete package is required for record-values")
            package = args.records_package.resolve(strict=True)
            record_proof = json.loads((package / "generation.lua.json").read_bytes())
            if record_proof["kind"] != "complete-lua-record-sources-v1" or record_proof["default_authority"] != []:
                raise ValueError("the explicit complete package has an incompatible receipt")
            if {p.relative_to(package).as_posix() for p in package.rglob("*") if p.is_file()} != set(record_proof["outputs"]) | {"generation.lua.json"}:
                raise ValueError("the explicit complete package contains unrecorded files")
            if any(p.is_symlink() for p in package.rglob("*")):
                raise ValueError("ordinary generated package products required")
            for name, pin in record_proof["outputs"].items():
                relative = Path(name)
                if relative.is_absolute() or ".." in relative.parts:
                    raise ValueError("confined generated package product required")
                actual = _pin(package / relative)
                if any(actual[key] != pin[key] for key in ("bytes", "sha256")):
                    raise ValueError("actual generated complete package product changed")
            if (package / "roswasm/wire.lua").read_bytes() != (origin / "lua/include/roswasm/wire.lua").read_bytes():
                raise ValueError("the generated package must use the exact current Lua wire implementation")
            shutil.copytree(package, original / "lib/platform/ros/lua/records")
        elif args.records_package is not None:
            raise ValueError("the selected example does not consume a record package")
        shutil.copy2(source / "sdk/xmake.lua", original / "xmake.lua")
        shutil.copy2(source / "sdk/example/xmake.lua", original / "example/xmake.lua")
        configuration = original / "share/formatting/clang-format.yaml"
        configuration.parent.mkdir()
        shutil.copy2(args.format_config, configuration)
        source_formatting = []
        # The anchors are valid C comments. Format and verify owned templates
        # before freezing provider pins; never touch C-provider or upstream files.
        for name in ("native.c.in", "guest_config.h"):
            path = original / "lib/platform/ros/lua/source" / name
            before = _pin(origin / "lua" / ("lib/native.c.in" if name == "native.c.in" else "embedding/guest_config.h"))
            argv = [args.clang_format, "--style=file:" + str(configuration), "--assume-filename=" + name.removesuffix(".in"), path]
            run("format-source-" + name, argv)
            body = (commands / ("format-source-" + name) / "stdout").read_bytes()
            path.write_bytes(body)
            run("format-source-again-" + name, argv)
            if (commands / ("format-source-again-" + name) / "stdout").read_bytes() != body:
                raise ValueError("owned C template/config source formatting is not byte-idempotent")
            source_formatting.append(dict(before=before, after=_pin(path)))
        sdk = args.out / "SDK Lua relocated with Unicode-λ"
        original.rename(sdk)
        for row in source_formatting:
            row["after"]["path"] = str(sdk / Path(row["after"]["path"]).relative_to(original))
        if any((sdk / name).exists() for name in ("origin", "test", "tests", "sdk", "include/roswasm")):
            raise ValueError("independent installed SDK with canonical include paths required")
        report["sdk"] = str(sdk)
        report["assembly"] = list(delivery.files.values())
        report["formatted_owned_sources"] = source_formatting
        runtime = dict(name="Lua", version="5.4.8", route="lua-wasi", mode="static-built-in-ros-bridge",
                       exceptions=True, exception_encoding="exnref-try-table", required_features=["exceptions"],
                       setjmp_strategy="wasi-libsetjmp-llvm-sjlj", experimental=True, language="lua", address_bits=32,
                       target="wasm32-wasip1", startup="command", default_authority=[], source_ready=True,
                       compiled=False, runtime_tested=False)
        contract = dict(target="wasm32-wasip1", address_bits=32, wasi="wasip1", exceptions=True,
                        threads=False, startup="command", core_baseline="3.0")
        default = dict(contract, exceptions=False)
        compiler = Path(sys.executable).resolve(strict=True)
        selected = {str(p.resolve(strict=True)): _pin(p) for p in
                    (args.archive, args.validator, args.toolchain_receipt, args.clang_format, sdk / "share/formatting/clang-format.yaml")
                    if not p.resolve(strict=True).is_relative_to(args.wasi_sdk.resolve(strict=True))}
        # Files inside the complete staged compiler root are already expanded
        # from its receipt. Re-listing clang-format would be a duplicate pin.
        inputs = dict(format="uwvm-managed-example-inputs-v1", language="lua", runtime=runtime,
                      paths=dict(wasi_sdk=str(args.wasi_sdk), archive=str(args.archive), validator=str(args.validator),
                          toolchain_receipt=str(args.toolchain_receipt), clang_format=str(args.clang_format),
                          format_config=str(sdk / "share/formatting/clang-format.yaml")),
                      receipt_sha256=args.toolchain_receipt_sha256, directories=[str(args.wasi_sdk)],
                      files=list(selected.values()), file_receipts=[dict(path=str(args.toolchain_receipt),
                          sha256=args.toolchain_receipt_sha256, root=str(args.wasi_sdk))])
        if record_proof is not None:
            package = sdk / "lib/platform/ros/lua/records"
            inputs["paths"]["records_package"] = str(package)
            inputs["directories"].append(str(package))
            inputs["files"].extend(_pin(p) for p in sorted(package.rglob("*")) if p.is_file())
        inputs_path = args.out / "actual-lua-inputs.json"
        _save(inputs_path, inputs)
        profile = dict(id="task04-lua548-standard-eh-source-provider", profile=contract,
            toolchain_id="actual-lua548-sdk33-offline", sysroot="include/platform/ros/lua", resource_dir=None,
            files=[_pin(p, sdk) for p in sorted(sdk.rglob("*")) if p.is_file()], runtime_requirements=[],
            library_abi=dict(address_bits=32, wasi_pointer_abi="wasip1-memory32", exceptions=True, threads=False,
                exception_encoding="exnref-try-table", thread_abi="none", shared_memory=False,
                required_files=["tools/platform/ros/lua/build.py", "lib/platform/ros/lua/source/native.c.in",
                    "include/platform/ros/lua/roswasm/init.lua"]),
            languages={"lua": dict(toolchain_id="actual-lua548-sdk33-offline", version="5.4.8",
                compiler=_pin(compiler, compiler.parent.parent), compiler_argv=[], linker_argv=[],
                library_modes=["runtime:lua-wasi"], source_ready=True, compiled=False, runtime_tested=False,
                runtime_identity={k: runtime[k] for k in ("route", "name", "version", "mode", "language", "target",
                    "address_bits", "startup", "exceptions", "exception_encoding")}, driver="uwvm-managed-sdk-consumer-v1",
                supports_clang_config=False, host_runtime_dependency_names=["admitted Linux CPython build driver/loader TCB; no host Lua application"])})
        manifest = sdk / "share/wasm-profiles.json"
        _save(manifest, dict(format="uwvm-sdk-wasm-profiles-v1", defaults=default, profiles=[profile], default_authority=[]))
        report["manifest"] = _pin(manifest)
        project = args.out / "project"
        project.mkdir()

        def xmake(task):
            return [args.xmake, task, "-P", project, "-F", sdk / "example/xmake.lua"]

        run("xmake-version", [args.xmake, "--version"])
        run("configure", [*xmake("f"), "-c", "--yes", "--ros-sdk-platforms=ros", "--ros-sdk-languages=lua",
            "--sdk-example-platform=ros", "--sdk-example-language=lua", "--sdk-example=" + args.example + "-ros" + args.backend,
            "--sdk-target=wasm32-wasip1", "--sdk-startup=command", "--sdk-exceptions=y", "--sdk-threads=n",
            "--sdk-offline=y", "--sdk-python=" + str(compiler), "--sdk-toolchain=" + str(compiler.parent.parent),
            "--sdk-profile-manifest=" + str(manifest), "--ros-sdk-lua-runtime=lua54-wasi-eh",
            "--ros-sdk-managed-backend=" + args.backend, "--ros-sdk-managed-inputs=" + str(inputs_path),
            "--ros-sdk-managed-inputs-sha256=" + _pin(inputs_path)["sha256"]], project)
        product = args.out / "example"
        run("build", [*xmake("sdk-example-build"), "--output=" + str(product)], project)
        receipt = product / "build-receipt.json"
        built = json.loads(receipt.read_bytes())
        if built["status"] != "BUILT_NOT_EXECUTED" or len(built["artifacts"]) != 1:
            raise ValueError("one genuine xmake-built Lua Wasm artifact required")
        artifact = Path(built["artifacts"][0]["path"])
        report.update(built=1, artifact=_pin(artifact), build_receipt=_pin(receipt))
        run("validate", [args.validator, "validate", artifact])
        report["validated"] = 1
        run("imports", [sys.executable, "-I", "-S", "-B", sdk / "tools/check_wasm_imports.py", artifact,
            "--backend", args.backend, "--allow-module", "wasi_snapshot_preview1", "--require-all",
            "--report", args.out / "imports.json"])
        output = run("guest", [*xmake("sdk-example-run"), "--receipt=" + str(receipt), "--runtime=" + str(args.runtime)], project)
        if output.count("PASS_ROS_DEFAULT_DENIED") != 1:
            raise ValueError("actual Lua Guest must confirm one default-denial application outcome")
        if record_proof is not None:
            expected = {row["symbol"]: {field["name"] for field in row["fields"]} for row in record_proof["records"]}
            observed, members = {}, {}
            for line in output.splitlines():
                if line.startswith("LUA_RECORD_FIELD\t"):
                    _, symbol, field, kind = line.split("\t")
                    if field in observed.setdefault(symbol, {}):
                        raise ValueError("duplicate actual Guest field binding")
                    observed[symbol][field] = kind
                elif line.startswith("LUA_RECORD_MEMBER\t"):
                    _, symbol, name, kind = line.split("\t")
                    observed.setdefault(symbol, {})
                    if name in members.setdefault(symbol, {}):
                        raise ValueError("duplicate actual Guest class member binding")
                    members[symbol][name] = kind
            if {name: set(fields) for name, fields in observed.items()} != expected or set(members) != set(expected):
                raise ValueError("actual Guest complete-field/class inventory differs from actual generated IR")
            for symbol, actual in members.items():
                if any(actual.get(name) != "function" for name in ("new", "encode", "decode", "_read")):
                    raise ValueError("actual generated class is missing a complete codec callable")
            if output.count("PASS_ROS_RECORD_VALUES\t" + str(len(expected))) != 1:
                raise ValueError("the real Guest did not confirm every complete record round trip")
            report["actual_guest_record_inventory"] = dict(fields=observed, members=members,
                records=len(expected), complete_field_names_match_admitted_ir=True,
                evidence="actual embedded Lua Guest tables/functions and complete encode/decode; no host Lua application",
                documentation_semantic_review="PENDING; dynamic binding does not establish full API documentation")
        for row in profile["files"]:
            actual = _pin(sdk / row["path"], sdk)
            if actual != row:
                raise ValueError("installed SDK input changed during its real xmake build/run")
        report.update(status="ACTUAL_INSTALLED_LUA_XMAKE_VALIDATE_GUEST_PASS", guest=1)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        save()


if __name__ == "__main__":
    _main()
