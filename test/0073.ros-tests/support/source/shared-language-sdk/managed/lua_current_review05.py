#!/usr/bin/env python3
"""Qualify current real Lua record generation, native parsing and installed Guests.

All compilers/generators/Guests are descendants of the original admitted owner.
Native Lua/luac is compiler tooling only. SDK applications execute only in Wasm.
No original upstream source, shared policy or other owner's source is modified.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tarfile


def pin(path):
    path = path.resolve(strict=True)
    with path.open("rb") as stream:
        return dict(path=str(path), bytes=path.stat().st_size,
                    sha256=hashlib.file_digest(stream, "sha256").hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("source", "archive", "wasi-sdk", "toolchain-receipt", "validator", "runtime", "xmake", "clang-format", "format-config", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    for name in ("toolchain-receipt", "clang-format", "format-config"):
        parser.add_argument("--" + name + "-sha256", required=True)
    parser.add_argument("--native-only", action="store_true")
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("original admitted Linux finite owner required")
    args.out.mkdir(parents=True, exist_ok=False)
    source = args.source.resolve(strict=True)
    support = source / "test/0073.ros-tests/support/source/shared-language-sdk/managed"
    report = dict(format="task04-current-lua-record-xmake-v1", status="STARTED", commands=[],
                  native_ros=0, upstream_suite=0, default_authority=[])

    def save():
        (args.out / "results.json").write_text(json.dumps(report, indent=2) + "\n")

    def run(label, argv, cwd=None):
        folder = args.out / "commands" / label
        folder.mkdir(parents=True)
        with (folder / "stdout").open("xb") as stdout, (folder / "stderr").open("xb") as stderr:
            result = subprocess.run(list(map(str, argv)), cwd=cwd or args.out,
                stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, check=False)
        report["commands"].append(dict(label=label, argv=list(map(str, argv)), exit=result.returncode,
            stdout=pin(folder / "stdout"), stderr=pin(folder / "stderr")))
        save()
        if result.returncode:
            raise RuntimeError("actual current-source Lua step failed: " + label)

    try:
        archive = pin(args.archive)
        if archive["sha256"] != "4f18ddae154e793e46eeab727c59ef1c0c0c2b744e7b94219710d76f530629ae":
            raise ValueError("exact unchanged Lua5.4.8 official archive required")
        report["upstream_archive"] = archive
        native = args.out / "native-parser"
        native.mkdir()
        with tarfile.open(args.archive) as package:
            if sum(max(0, row.size) for row in package.getmembers()) > 8 << 20:
                raise ValueError("finite actual Lua source bound required")
            package.extractall(native, filter="data")
        lua = native / "lua-5.4.8"
        run("native-cc-version", ["/usr/bin/cc", "--version"])
        run("build-native-lua-parser", ["/usr/bin/make", "-j1", "generic", "MYCFLAGS=-O0"], lua)
        luac = lua / "src/luac"
        run("native-luac-version", [luac, "-v"])
        report["native_lua_compiler"] = pin(luac)
        package = args.out / "generated-records"
        catalog = source / "test/0073.ros-tests/support/source/ros_custom_types/managed-records/fixture/catalog.preparation.json"
        run("generate-complete-lua-records", [sys.executable, "-I", "-S", "-B",
            source / "sdk/origin/lua/tools/records.py", catalog, "--out", package])
        proof = json.loads((package / "generation.lua.json").read_bytes())
        report["generation"] = pin(package / "generation.lua.json")
        report["generated_record_count"] = len(proof["records"])
        files = [source / "sdk/origin/lua/include/roswasm/init.lua", source / "sdk/origin/lua/include/roswasm/wire.lua",
                 source / "sdk/origin/examples/lua/owned_session.lua", source / "sdk/origin/examples/lua/record_values.lua",
                 package / "bounded_records.lua", package / "roswasm/wire.lua"]
        for index, file in enumerate(files):
            run("native-lua-parse-" + str(index), [luac, "-p", file])
        report["native_lua_syntax"] = [pin(p) for p in files]
        if args.native_only:
            report.update(status="ACTUAL_LUA_RECORD_GENERATION_NATIVE_PARSE_PASS", actual_guests=0,
                native_lua_application_executions=0, lua_formatter_qualified=False,
                complete_public_api_documentation_qualified=False)
            return
        selected = [word for name in ("wasi_sdk", "toolchain_receipt", "validator", "runtime", "xmake", "archive", "clang_format", "format_config")
                    for word in ("--" + name.replace("_", "-"), getattr(args, name))]
        selected += [word for name in ("toolchain_receipt", "clang_format", "format_config")
                     for word in ("--" + name.replace("_", "-") + "-sha256", getattr(args, name + "_sha256"))]
        for backend in ("2", "1"):
            out = args.out / ("installed-ros" + backend)
            run("installed-xmake-records-ros" + backend, [sys.executable, "-I", "-S", "-B", support / "lua_xmake_guest.py",
                "--source", source, *selected, "--backend", backend, "--example", "record-values", "--records-package", package, "--out", out])
            result = json.loads((out / "results.json").read_bytes())
            if result["status"] != "ACTUAL_INSTALLED_LUA_XMAKE_VALIDATE_GUEST_PASS" or result["actual_guest_record_inventory"]["records"] != len(proof["records"]):
                raise ValueError("actual relocated record build/Guest result is incomplete")
            report.setdefault("actual_installed_results", []).append(pin(out / "results.json"))
        report.update(status="ACTUAL_CURRENT_LUA_RECORD_GENERATION_NATIVE_PARSE_INSTALLED_XMAKE_GUEST_PASS",
            actual_guests=2, native_lua_application_executions=0, lua_formatter_qualified=False,
            complete_public_api_documentation_qualified=False)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        save()


if __name__ == "__main__":
    main()
