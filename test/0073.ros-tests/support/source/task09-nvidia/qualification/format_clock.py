#!/usr/bin/env python3
"""Genuine format/parser checks for the bounded twelve-file clock source delta."""
import argparse
import ast
import fcntl
import hashlib
import inspect
import io
import json
import os
from pathlib import Path, PurePosixPath
import re
import subprocess
import sys
import tarfile
import zipfile


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def normalized(raw):
    tree = ast.parse(raw)
    for node in ast.walk(tree):
        if isinstance(node, (ast.Module, ast.ClassDef, ast.FunctionDef, ast.AsyncFunctionDef)):
            if node.body and isinstance(node.body[0], ast.Expr) and isinstance(node.body[0].value, ast.Constant):
                if isinstance(node.body[0].value.value, str):
                    node.body[0].value.value = inspect.cleandoc(node.body[0].value.value)
    return ast.dump(tree, include_attributes=False)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-sha256", required=True)
    args = parser.parse_args()
    assert os.environ["UWVM_RESOURCE_HEAVY_LOCK_HELD"] == "1"
    run = Path(os.environ["UWVM_RESOURCE_RUN"]).resolve(strict=True)
    work = run / "work"
    archive_path = run / "inputs/source.tar"
    assert digest(archive_path) == args.source_sha256
    source = work / "candidate"
    source.mkdir()
    before, original, commands = {}, {}, []
    with tarfile.open(archive_path, "r:") as archive:
        members = archive.getmembers()
        assert len(members) == 12
        total = 0
        for member in members:
            name = PurePosixPath(member.name)
            assert member.isfile() and not name.is_absolute() and ".." not in name.parts
            assert member.name.startswith("sdk/origin/robots/nvidia/isaac_sim/")
            assert member.name not in before and member.size <= 65536
            total += member.size
            assert total <= 196608
            raw = archive.extractfile(member).read(member.size + 1)
            assert len(raw) == member.size
            path = source / member.name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(raw)
            original[member.name] = raw
            before[member.name] = hashlib.sha256(raw).hexdigest()
    rustfmt = Path("/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-round03-rustfmt-r1/work/tools/bin/rustfmt")
    gofmt = Path("/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-toolchains-r1/work/toolchains/go-input/go/bin/gofmt")
    node = Path("/usr/bin/node")
    prettier = Path("/home/macromodel/Documents/uwvm-test-261018/agents/07/20261022-round04-prettier-r1/work/prettier/bin/prettier.cjs")
    pins = {rustfmt: "5d90e4b57cf7ae89a83c35428a245c0de9fa050fd13c5ad7f19b2ee4b561ae6f",
            gofmt: "acb1f5c293c3f0e2032676b72a288f7f18022653aedb465e3c1bd0ea78eab6e5",
            node: "893391109bf5d2b34dd50143bfab997b3fcd123dd59cb156b67f01b7a1db02bf",
            prettier: "ac5523cd57e7e9d8eac71caef7e022a8a8489bcdc19ca8a778b7e728ec103b93"}
    as_parser = Path("/home/macromodel/Documents/uwvm-test-261018/agents/04/round03-as-xmake-r1/staged/node_modules/assemblyscript/dist/assemblyscript.js")
    pins[as_parser] = "b0ea444edfff355fe2981451358c5b8b24996f5e9190e02bceae60c94adeea19"
    for path, expected in pins.items():
        assert digest(path) == expected
    prettier_root = prettier.parent.parent
    prettier_pins = {str(path.relative_to(prettier_root)): digest(path)
                     for path in sorted(prettier_root.rglob("*")) if path.is_file()}
    assert len(prettier_pins) <= 512
    as_root = as_parser.parents[2]
    as_pins = {str(path.relative_to(as_root)): digest(path)
               for path in sorted(as_root.rglob("*")) if path.is_file()}
    assert len(as_pins) <= 512
    package_archive = Path("/home/macromodel/Documents/uwvm-test-261018/agents/09/format-r18/resource-archives/evidence.tar")
    assert digest(package_archive) == "ede0921ab080fcbe526958994c5eed34dfa1e2f2489f8e5407d83f2faa143784"
    with tarfile.open(package_archive, "r:") as archive:
        metadata = archive.extractfile("work/packages/ruff-metadata.json").read()
        choices = [row for row in json.loads(metadata)["urls"] if row["filename"].endswith(".whl") and
                   "py3-none" in row["filename"] and "x86_64" in row["filename"] and
                   "manylinux" in row["filename"] and "musllinux" not in row["filename"]]
        assert len(choices) == 1
        package = choices[0]
        wheel = archive.extractfile("work/packages/" + package["filename"]).read()
    assert len(wheel) == package["size"] and hashlib.sha256(wheel).hexdigest() == package["digests"]["sha256"]
    with zipfile.ZipFile(io.BytesIO(wheel)) as archive:
        names = [name for name in archive.namelist() if name.endswith("/ruff")]
        assert len(names) == 1
        elf = archive.read(names[0])
    descriptor = os.memfd_create("task09-clock-ruff", os.MFD_CLOEXEC | os.MFD_ALLOW_SEALING | getattr(os, "MFD_EXEC", 0))
    try:
        with os.fdopen(os.dup(descriptor), "wb") as stream:
            stream.write(elf)
        os.fchmod(descriptor, 0o500)
        fcntl.fcntl(descriptor, fcntl.F_ADD_SEALS, fcntl.F_SEAL_WRITE | fcntl.F_SEAL_GROW | fcntl.F_SEAL_SHRINK | fcntl.F_SEAL_SEAL)
        ruff = Path("/proc/self/fd") / str(descriptor)
        env = dict(os.environ, UV_THREADPOOL_SIZE="1")
        for name in ("NODE_OPTIONS", "NODE_PATH", "NODE_COMPILE_CACHE"):
            env.pop(name, None)
        def command(label, argv, extra=None, input_bytes=None):
            execution_env = dict(env)
            execution_env.update(extra or {})
            input_arguments = {"stdin": subprocess.DEVNULL} if input_bytes is None else {"input": input_bytes}
            result = subprocess.run([str(v) for v in argv], cwd=source, env=execution_env,
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                    pass_fds=(descriptor,), timeout=45, **input_arguments)
            assert len(result.stdout) + len(result.stderr) <= 131072
            commands.append({"name": label, "argv": [str(v) for v in argv], "exit_code": result.returncode,
                             "stdout_sha256": hashlib.sha256(result.stdout).hexdigest(),
                             "stdout_bytes": len(result.stdout), "stdout": result.stdout[:512].decode(errors="replace"),
                             "stderr": result.stderr.decode(errors="replace")})
            (work / "clock-format-commands.json").write_text(json.dumps(commands, indent=2) + "\n")
            assert result.returncode == 0, label + ": " + result.stderr.decode(errors="replace")
            return result.stdout
        py = [source / name for name in before if name.endswith(".py")]
        rs = [source / name for name in before if name.endswith(".rs")]
        go = [source / name for name in before if name.endswith(".go")]
        ts = [source / name for name in before if name.endswith(".ts")]
        js = [source / name for name in before if name.endswith(".mjs")]
        assert (len(py), len(rs), len(go), len(ts), len(js)) == (3, 2, 2, 3, 2)
        formatted_ts = [path for path in ts if path.name != "wasi.ts"]
        assert len(formatted_ts) == 2
        rust_env = {"LD_LIBRARY_PATH": "/home/macromodel/Documents/uwvm-test-261018/agents/03/20261021-toolchains-r1/work/toolchains/rust/lib"}
        assert command("ruff-version", [ruff, "--version"]).strip() == b"ruff 0.14.14"
        command("rustfmt-version", [rustfmt, "--version"], rust_env)
        assert command("prettier-version", [node, prettier, "--version"]).strip() == b"3.6.2"
        command("ruff-format", [ruff, "format", "--isolated", "--target-version", "py312", *py])
        for number, path in enumerate(rs):
            path.write_bytes(command("rustfmt-" + str(number), [rustfmt, "--edition", "2021", "--emit", "stdout"], rust_env, path.read_bytes()))
        command("gofmt", [gofmt, "-w", *go])
        command("prettier-typescript", [node, prettier, "--parser", "typescript", "--write", *formatted_ts])
        command("prettier-javascript", [node, prettier, "--parser", "babel", "--write", *js])
        formatted = {name: digest(source / name) for name in before}
        for name, raw in original.items():
            if name.endswith(".py"):
                assert normalized(raw) == normalized((source / name).read_bytes())
        command("ruff-idempotence", [ruff, "format", "--isolated", "--target-version", "py312", "--check", *py])
        for number, path in enumerate(rs):
            assert command("rustfmt-idempotence-" + str(number), [rustfmt, "--edition", "2021", "--emit", "stdout"], rust_env, path.read_bytes()) == path.read_bytes()
        assert command("gofmt-idempotence", [gofmt, "-l", *go]) == b""
        command("prettier-ts-idempotence", [node, prettier, "--parser", "typescript", "--check", *formatted_ts])
        command("prettier-js-idempotence", [node, prettier, "--parser", "babel", "--check", *js])
        for number, path in enumerate(js):
            command("javascript-parse-" + str(number), [node, "--check", path])
        parser_script = work / "actual-as-parser.mjs"
        parser_script.write_text("import fs from 'node:fs';\n"
            + "import * as compiler from " + json.dumps(as_parser.as_uri()) + ";\n"
            + "const parser=new compiler.Parser();\n"
            + "for(const file of process.argv.slice(2))parser.parseFile(fs.readFileSync(file,'utf8'),file,true);\n"
            + "const diagnostics=parser.diagnostics.map(d=>({category:d.category,code:d.code,text:compiler.formatDiagnostic(d,false,false)}));\n"
            + "console.log(JSON.stringify({actualParser:'AssemblyScript0.28.20',sources:parser.sources.length,diagnostics,codegen:false}));\n"
            + "if(parser.sources.length!==3||diagnostics.some(d=>d.category===3))process.exit(1);\n")
        assert json.loads((as_parser.parent.parent / "package.json").read_bytes())["version"] == "0.28.20"
        parsed = command("actual-assemblyscript-parser", [node, "--max-old-space-size=256", parser_script, *ts])
        (work / "actual-as-parser.json").write_bytes(parsed)
        assert formatted == {name: digest(source / name) for name in before}
        api = []
        for path in py[:2]:
            def visit(nodes, prefix=""):
                for item in nodes:
                    if not isinstance(item, (ast.FunctionDef, ast.ClassDef)) or (item.name.startswith("_") and item.name != "__init__"):
                        continue
                    doc = ast.get_docstring(item) or ""
                    assert all(tag in doc for tag in ("@brief", "@details", "@see")), prefix + item.name
                    if isinstance(item, ast.FunctionDef):
                        for arg in item.args.args + item.args.kwonlyargs:
                            if arg.arg not in ("self", "cls"):
                                assert re.search(r"@param\[[^\]]+\]\s+" + re.escape(arg.arg) + r"\b", doc)
                    api.append(str(path.relative_to(source)) + ":" + prefix + item.name)
                    if isinstance(item, ast.ClassDef):
                        visit(item.body, prefix + item.name + ".")
            visit(ast.parse(path.read_bytes()).body)
        assert all(digest(path) == pin for path, pin in pins.items())
        assert all(digest(prettier_root / name) == pin for name, pin in prettier_pins.items())
        assert all(digest(as_root / name) == pin for name, pin in as_pins.items())
        result = {"format": "uwvm-task09-current-clock-format-v1", "status": "PASS",
                  "inputs": before, "outputs": formatted, "commands": commands,
                  "formatter_pins": {str(path): pin for path, pin in pins.items()}, "prettier_dependency_pins": prettier_pins,
                  "assemblyscript_parser_dependency_pins": as_pins,
                  "ruff_package_sha256": package["digests"]["sha256"], "ruff_elf_sha256": hashlib.sha256(elf).hexdigest(),
                  "ruff_metadata_sha256": hashlib.sha256(metadata).hexdigest(), "selected_native_public_apis": api,
                  "source_files": 12, "formatted_source_files": 11, "python_ast_equivalence": True, "idempotent": True,
                  "unformatted_source_files": [{"source": "sdk/example/platform/nvidia-isaac-sim/as/clock-program/wasi.ts",
                      "reason": "Actual AssemblyScript @external function decorators are outside the selected TypeScript formatter grammar; genuine AS parser checked syntax"}],
                  "rust_compiler_executions": 0, "assemblyscript_frontend_parses": 3, "assemblyscript_codegen_link_executions": 0,
                  "quickjs_executions": 0, "guest_executions": 0, "native_sdk_executions": 0,
                  "lua_formatting": "NOT_RUN", "default_authority": []}
        (work / "clock-format.json").write_text(json.dumps(result, indent=2) + "\n")
        with tarfile.open(work / "formatted-clock.tar", "x:") as archive:
            for name in before:
                archive.add(source / name, arcname=name, recursive=False)
        print(json.dumps({key: result[key] for key in ("status", "source_files", "idempotent", "guest_executions")}))
    finally:
        os.close(descriptor)


if __name__ == "__main__":
    main()
