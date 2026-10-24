#!/usr/bin/env python3
"""Validate selected documented Unitree headers and existing value contracts.

The compiler inventory binds declarations to exact source offsets and attached
comments. It does not judge documentation quality by tag counts. Existing CRC
and hold cases run as native C11 and actual Wasm. No official-program, xmake,
native ROS, simulator integration or full installed SDK credit follows.
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess


def fact(path):
    path = Path(path).resolve(strict=True)
    with path.open("rb") as stream:
        checksum = hashlib.file_digest(stream, "sha256").hexdigest()
    return {"path": str(path), "bytes": path.stat().st_size, "sha256": checksum}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("formatted", "tests", "wasi", "runtime", "validator", "checker"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    run = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    assert os.environ["UWVM_RESOURCE_HEAVY_LOCK_HELD"] == "1"
    out = run / "work/header-contracts"
    out.mkdir()
    report = {"status": "RUNNING", "inputs": [], "commands": [], "inventory": [],
              "products": [], "native_value_runs": 0, "actual_Wasm_Guests": 0,
              "Wasm_validations": 0, "native_ROS_runs": 0, "official_program_ports": 0,
              "full_SDK_qualified": False, "all_language_documentation_qualified": False,
              "documentation_semantic_review": "Separate source-content review required",
              "default_authority": []}
    pins = {}

    def pin(path):
        path = Path(path).resolve(strict=True)
        if path not in pins:
            pins[path] = fact(path)
            report["inputs"].append(pins[path])
        return path

    def save():
        (out / "result.json").write_text(json.dumps(report, indent=2) + "\n")

    def command(label, argv, *, retain_stdout=True):
        row = {"label": label, "argv": list(map(str, argv))}
        report["commands"].append(row)
        child = subprocess.run(row["argv"], cwd=out, stdin=subprocess.DEVNULL,
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=90)
        row.update(exit=child.returncode, stdout_bytes=len(child.stdout),
                   stdout_sha256=hashlib.sha256(child.stdout).hexdigest())
        if retain_stdout:
            (out / (label + ".stdout")).write_bytes(child.stdout)
        (out / (label + ".stderr")).write_bytes(child.stderr)
        save()
        assert child.returncode == 0, (label, child.returncode, child.stderr[-3000:])
        return child.stdout

    def dependencies(path):
        for name in shlex.split(path.read_text().replace("\\\n", " ").partition(":")[2]):
            dependency = Path(name).resolve(strict=True)
            if not dependency.is_relative_to(run):
                pin(dependency)

    def inventory(header, raw):
        text = header.read_text()
        decoder = json.JSONDecoder()
        remaining = raw.decode()
        bound = {}
        kinds = {"FunctionDecl", "FunctionTemplateDecl", "RecordDecl", "CXXRecordDecl",
                 "TypedefDecl", "ClassTemplateDecl", "ClassTemplatePartialSpecializationDecl",
                 "VarDecl"}

        def visit(node):
            name = node.get("name", "")
            location = node.get("loc", {})
            offset = location.get("offset")
            if (node.get("kind") in kinds and name and not node.get("isImplicit")
                    and (node["kind"] != "VarDecl" or node.get("constexpr") is True)
                    and isinstance(offset, int) and text[offset:offset + len(name)] == name
                    and (not location.get("file") or Path(location["file"]).resolve() == header)):
                key = (offset, name)
                item = bound.setdefault(key, {"name": name, "source_offset": offset,
                    "source_line": text.count("\n", 0, offset) + 1, "compiler_kinds": [],
                    "type": node.get("type"), "parameters": [], "attached_comments": [],
                    "quality_status": "Content review is independent of parser attachment"})
                if node["kind"] not in item["compiler_kinds"]:
                    item["compiler_kinds"].append(node["kind"])
                if item["type"] is None and node.get("type") is not None:
                    item["type"] = node["type"]
                for child in node.get("inner", []):
                    if child.get("kind") == "ParmVarDecl":
                        parameter = {key: child.get(key) for key in ("name", "type", "loc")}
                        if parameter not in item["parameters"]:
                            item["parameters"].append(parameter)
                    if child.get("kind") == "FullComment" and child not in item["attached_comments"]:
                        item["attached_comments"].append(child)
            for child in node.get("inner", []):
                visit(child)

        while remaining.strip():
            remaining = remaining.lstrip()
            node, end = decoder.raw_decode(remaining)
            visit(node)
            remaining = remaining[end:]
        assert bound, header
        report["inventory"].append({"header": fact(header),
            "declarations": [bound[key] for key in sorted(bound)],
            "scope": "Compiler-bound function/type/constexpr declarations; macro/field review separate"})

    try:
        pin(__file__)
        sdk = out / "sdk"
        headers = []
        prefix = Path("sdk/origin/robots/unitree/include/roswasm_unitree")
        for name in ("crc.h", "crc.hpp", "simulation_hold.h", "simulation_control.h"):
            language = "cpp" if name.endswith(".hpp") else "c"
            target = sdk / "include/platform/unitree" / language / "roswasm_unitree" / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(pin(args.formatted / prefix / name), target)
            headers.append(target)
        for tool in ("/usr/bin/clang-21", "/usr/bin/clang++-21", args.wasi / "bin/clang",
                     args.wasi / "bin/wasm-ld", args.runtime, args.validator, args.checker):
            pin(tool)
        command("native-version", ["/usr/bin/clang-21", "--version"])
        command("WASI-version", [args.wasi / "bin/clang", "--version"])
        command("validator-version", [args.validator, "--version"])
        for header in headers:
            cpp = header.suffix == ".hpp"
            compiler = "/usr/bin/clang++-21" if cpp else "/usr/bin/clang-21"
            dep = out / (header.name + ".d")
            raw = command("inventory-" + header.name, [compiler, "-x", "c++" if cpp else "c",
                "-std=c++17" if cpp else "-std=c11", "-fsyntax-only", "-fparse-all-comments",
                "-I" + str(sdk / "include"), "-MD", "-MF", dep,
                "-Xclang", "-ast-dump=json", "-Xclang",
                "-ast-dump-filter=" + ("roswasm_unitree" if cpp else "uwvm_unitree_"), header],
                retain_stdout=False)
            dependencies(dep)
            inventory(header, raw)
        spec = importlib.util.spec_from_file_location("actual_import_checker", args.checker)
        checker = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(checker)
        for directory in (args.wasi / "share/wasi-sysroot/lib", args.wasi / "lib/clang"):
            for path in sorted(directory.rglob("*")):
                if path.is_file() and path.suffix in (".a", ".o"):
                    pin(path)
        for name in ("crc", "hold"):
            source = pin(args.tests / (name + "_contract.c"))
            for target in ("native", "wasm"):
                product = out / (target + "-" + name + (".wasm" if target == "wasm" else ""))
                dep = product.with_suffix(".d")
                compiler = "/usr/bin/clang-21" if target == "native" else args.wasi / "bin/clang"
                flags = ["-std=c11", "-O1", "-g0", "-Wall", "-Wextra", "-Werror",
                         "-I" + str(sdk / "include"), "-MD", "-MF", dep]
                if target == "wasm":
                    flags += ["--target=wasm32-wasip1", "--sysroot=" + str(args.wasi / "share/wasi-sysroot"),
                        "-Wl,--threads=1,-z,stack-size=1048576,--initial-memory=16777216,--max-memory=67108864"]
                command("build-" + target + "-" + name, [compiler, *flags, source, "-o", product])
                dependencies(dep)
                report["products"].append({"case": name, "target": target, **fact(product)})
                if target == "native":
                    command("run-native-" + name, [product])
                    report["native_value_runs"] += 1
                else:
                    command("validate-" + name, [args.validator, "validate", product])
                    imports = checker.inspect(product.read_bytes())
                    assert all(row["module"] == "wasi_snapshot_preview1" and row["kind"] == 0 for row in imports)
                    (out / (name + "-imports.json")).write_text(json.dumps(imports, indent=2) + "\n")
                    command("run-wasm-" + name, [args.runtime, "-Rint",
                        "--wasip1-global-noinherit-system-environment", "--run", product])
                    report["Wasm_validations"] += 1
                    report["actual_Wasm_Guests"] += 1
        assert all(fact(path) == row for path, row in pins.items())
        report.update(status="PASS_HEADER_FRONTEND_AND_LOCAL_VALUES_ONLY", input_pins_preserved=True)
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        save()


if __name__ == "__main__":
    main()
