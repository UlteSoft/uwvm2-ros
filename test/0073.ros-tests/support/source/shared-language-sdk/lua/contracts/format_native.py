#!/usr/bin/env python3
"""@brief Format and parser-bind the owned Lua C template/configuration only.

@details Actual pinned clang-format formats both inputs twice, then real Clang
parses the C11 Wasm template against explicit Lua/ABI/sysroot headers. Full AST
bytes are retained losslessly. Public Lua registrations bind to documented C
definitions; this does not qualify a linked Wasm artifact or Lua/PHP API coverage.
All execution requires the original admitted Linux finite owner and common lock.
@see ../../../../../../../sdk/origin/lua/lib/native.c.in
"""

import argparse
import gzip
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


def _walk(node):
    yield node
    for child in node.get("inner", []):
        yield from _walk(child)


def _main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("template", "guest-config", "style", "formatter", "wasi-sdk", "lua-headers", "abi-root", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--formatter-sha256", required=True)
    parser.add_argument("--style-sha256", required=True)
    parser.add_argument("--source-kind", choices=("template", "generated"), default="template")
    args = parser.parse_args()
    if os.uname().sysname != "Linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("the original admitted Linux finite owner is required")
    if _pin(args.formatter)["sha256"] != args.formatter_sha256 or _pin(args.style)["sha256"] != args.style_sha256:
        raise ValueError("selected exact formatter/configuration bytes differ")
    args.out.mkdir(parents=True, exist_ok=False)
    report = dict(format="task04-lua-owned-c-format-parser-v1", status="STARTED", commands=[], formatted=[],
                  default_authority=[], compiler_artifact_credit=0, guest_credit=0,
                  source_inputs=[_pin(p) for p in (args.template, args.guest_config, args.style)])

    def save():
        (args.out / "results.json").write_text(json.dumps(report, indent=2) + "\n")

    def run(label, argv):
        argv = list(map(str, argv))
        with (args.out / (label + ".stdout")).open("xb") as stdout, (args.out / (label + ".stderr")).open("xb") as stderr:
            completed = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, check=False)
        report["commands"].append(dict(label=label, argv=argv, exit=completed.returncode,
            stdout=_pin(args.out / (label + ".stdout")), stderr=_pin(args.out / (label + ".stderr"))))
        save()
        if completed.returncode:
            raise RuntimeError("actual C format/parser step failed: " + label)
        return (args.out / (label + ".stdout")).read_bytes()

    try:
        report["formatter"] = _pin(args.formatter)
        report["compiler"] = _pin(args.wasi_sdk / "bin/clang")
        run("formatter-version", [args.formatter, "--version"])
        run("compiler-version", [args.wasi_sdk / "bin/clang", "--version"])
        for source, name in ((args.template, "native.c"), (args.guest_config, "guest_config.h")):
            argv = [args.formatter, "--style=file:" + str(args.style), "--assume-filename=" + name]
            body = run("format-" + name, [*argv, source])
            path = args.out / name
            path.write_bytes(body)
            again = run("format-again-" + name, [*argv, path])
            if again != body:
                raise ValueError("actual owned C formatting is not byte-idempotent")
            report["formatted"].append(dict(source=_pin(source), product=_pin(path), idempotent=True))
        source = args.out / "native.c"
        raw_source = source.read_bytes()
        expected_anchors = 1 if args.source_kind == "template" else 0
        if any(raw_source.count(anchor) != expected_anchors for anchor in (b"/* @SELECT@ */", b"/* @DISPATCH@ */")):
            raise ValueError("actual formatter changed a required source emitter anchor")
        argv = list(map(str, [args.wasi_sdk / "bin/clang", "--target=wasm32-wasip1",
            "--sysroot=" + str(args.wasi_sdk / "share/wasi-sysroot"), "-std=c11", "-DROSWASM_BACKEND=2",
            "-I" + str(args.lua_headers), "-I" + str(args.abi_root), "-include", args.out / "guest_config.h",
            "-fsyntax-only", "-fparse-all-comments", "-Xclang", "-ast-dump=json", "-x", "c", source]))
        errors = args.out / "clang-ast.stderr"
        with errors.open("xb") as stderr:
            with subprocess.Popen(argv, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=stderr) as child:
                raw_ast = child.stdout.read((32 << 20) + 1)
                if len(raw_ast) > 32 << 20:
                    child.kill()
                    raise ValueError("actual Clang AST exceeds its explicit byte bound")
                code = child.wait()
        ast_file = args.out / "clang-ast.json.gz"
        ast_file.write_bytes(gzip.compress(raw_ast, mtime=0))
        report["commands"].append(dict(label="actual-clang-ast", argv=argv, exit=code,
            stdout=_pin(ast_file), stdout_encoding="gzip", stdout_uncompressed_bytes=len(raw_ast),
            stdout_uncompressed_sha256=hashlib.sha256(raw_ast).hexdigest(), stderr=_pin(errors)))
        if code:
            raise RuntimeError("actual formatted C template/configuration syntax check failed")
        tree = json.loads(raw_ast)
        expected = dict(prepare="prepare", invoke="invoke", retire="retire", error="error_code", valid="valid",
                        data="data", value="value_word", required="required", owns="owns")
        definitions = {}
        for node in tree["inner"]:
            if node.get("kind") != "FunctionDecl" or node.get("name") not in {*expected.values(), "luaopen_roswasm_native"}:
                continue
            if not any(child.get("kind") == "CompoundStmt" for child in node.get("inner", [])):
                continue
            comments = [child for child in node.get("inner", []) if child.get("kind") == "FullComment"]
            if len(comments) != 1:
                raise ValueError("one parser-bound public C API comment required: " + node["name"])
            comment = comments[0]
            commands = {child["name"] for child in _walk(comment) if child.get("kind") == "BlockCommandComment"}
            if not {"brief", "details", "see"} <= commands:
                raise ValueError("actual public C declaration is missing required documentation commands")
            definitions[node["name"]] = dict(name=node["name"], location=node["loc"], range=node["range"],
                comment=comment, documentation_commands=sorted(commands),
                semantics="manually reviewed in API-DOC-SCOPE.md; not inferred from tag counts")
        if set(definitions) != {*expected.values(), "luaopen_roswasm_native"}:
            raise ValueError("actual public C definition inventory is incomplete")
        bindings = {}
        loader = next(node for node in tree["inner"] if node.get("name") == "luaopen_roswasm_native" and
                      any(child.get("kind") == "CompoundStmt" for child in node.get("inner", [])))
        for node in _walk(loader):
            if node.get("kind") != "VarDecl" or node.get("name") not in ("methods", "functions"):
                continue
            for initializer in node.get("inner", []):
                if initializer.get("kind") != "InitListExpr":
                    continue
                for entry in initializer.get("inner", []):
                    strings = [child["value"] for child in _walk(entry) if child.get("kind") == "StringLiteral"]
                    functions = [child["referencedDecl"]["name"] for child in _walk(entry)
                                 if child.get("kind") == "DeclRefExpr" and child.get("referencedDecl", {}).get("kind") == "FunctionDecl"]
                    if strings:
                        if len(strings) != 1 or len(functions) != 1:
                            raise ValueError("one exact parsed Lua registration/function binding required")
                        bindings[json.loads(strings[0])] = functions[0]
        if bindings != expected:
            raise ValueError("actual parsed luaL_Reg bindings differ from the reviewed public API inventory")
        report.update(status="OWNED_C_FORMAT_IDEMPOTENCE_AND_PARSER_BINDINGS_PASS", public_c_definitions=definitions,
                      lua_registration_bindings=bindings, public_c_definition_count=len(definitions),
                      lua_callable_bindings=len(bindings), source_kind=args.source_kind,
                      syntax="C11 wasm32-wasip1 owned glue/config only")
    except BaseException as error:
        report.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        save()


if __name__ == "__main__":
    _main()
