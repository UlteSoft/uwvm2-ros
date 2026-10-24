#!/usr/bin/env python3
"""@brief Exercise the real expanded offline-tool closure and its refusal cases.

@details These local integrity contracts execute the actual managed backend
validator. They do not run a compiler, application or Guest and confer no
runtime/language acceptance. Requires the original admitted Linux finite owner.
@see ../../../../../../sdk/origin/language/managed/EXAMPLES.md
"""

import argparse
import ast
import copy
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import sys


def _main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--backend", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--parse-source", type=Path, action="append", default=[])
    args = parser.parse_args()
    if os.uname().sysname != "Linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("the original admitted Linux finite owner is required")
    spec = importlib.util.spec_from_file_location("_actual_managed_backend", args.backend.resolve(strict=True))
    backend = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(backend)
    args.out.mkdir(parents=True, exist_ok=False)
    root = args.out / "explicit-tool-input"
    root.mkdir()
    tool = root / "data.bin"
    # Deliberately inert data; no executable/compiler credit is claimed.
    tool.write_bytes(b"real offline file-pinning fixture\n")
    receipt = args.out / "staged-files.json"
    original = dict(kind="immutable-input-byte-copy-v1", inputs=[dict(output=str(root), files=[
        dict(path="data.bin", bytes=tool.stat().st_size, sha256=backend.pin(tool)["sha256"])])])

    def selection(document):
        receipt.write_text(json.dumps(document) + "\n")
        return dict(files=[backend.pin(receipt)], directories=[str(root)], file_receipts=[dict(
            path=str(receipt), sha256=backend.pin(receipt)["sha256"], root=str(root))])

    results = []

    def check(name, inputs, refused=False):
        try:
            pins, roots = backend.validate_closure(inputs)
        except ValueError:
            if not refused:
                raise
            results.append(dict(name=name, outcome="EXPECTED_REFUSAL"))
            return
        if refused:
            raise AssertionError("invalid real offline closure was accepted: " + name)
        if set(pins) != {str(tool), str(receipt)} or roots != [root]:
            raise AssertionError("the actual complete closure lost a tool or receipt pin")
        results.append(dict(name=name, outcome="PASS"))

    check("complete-receipt-expanded-and-individually-pinned", selection(original))
    explicit = selection(original)
    explicit["files"].append(backend.pin(tool))
    explicit.pop("file_receipts")
    check("existing-explicit-file-closure-preserved", explicit)
    changed = selection(original)
    changed["file_receipts"][0]["sha256"] = "0" * 64
    check("independent-receipt-hash-drift-refused", changed, True)
    changed = selection(original)
    tool.write_bytes(b"changed bytes\n")
    check("actual-tool-byte-drift-refused", changed, True)
    tool.write_bytes(b"real offline file-pinning fixture\n")
    changed = selection(original)
    extra = root / "unlisted.bin"
    extra.write_bytes(b"not in the complete receipt\n")
    check("unlisted-directory-file-refused", changed, True)
    extra.unlink()
    changed = selection(original)
    changed["files"].append(backend.pin(tool))
    check("duplicate-explicit-and-expanded-pin-refused", changed, True)
    escaping = copy.deepcopy(original)
    escaping["inputs"][0]["files"][0]["path"] = "../outside.bin"
    check("escaping-receipt-relative-path-refused", selection(escaping), True)
    duplicated = copy.deepcopy(original)
    duplicated["inputs"].append(copy.deepcopy(duplicated["inputs"][0]))
    check("ambiguous-selected-receipt-root-refused", selection(duplicated), True)
    changed = selection(original)
    changed["files"] = [backend.pin(tool)]
    check("unlisted-receipt-self-pin-refused", changed, True)
    changed = selection(original)
    changed["directories"] = []
    check("implicit-input-root-refused", changed, True)
    parsed = []
    for path in args.parse_source:
        raw = path.read_bytes()
        tree = ast.parse(raw, filename=str(path))
        definitions = []
        for node in ast.walk(tree):
            if isinstance(node, (ast.FunctionDef, ast.AsyncFunctionDef, ast.ClassDef)):
                definitions.append(dict(name=node.name, kind=type(node).__name__, line=node.lineno,
                    end_line=node.end_lineno, documentation=ast.get_docstring(node),
                    classification="internal-build-tool-source; not a public SDK language API inventory"))
        parsed.append(dict(source=backend.pin(path), definitions=definitions))
    result = dict(format="task04-real-managed-offline-closure-contract-v1", status="PASS", cases=results,
                  backend=backend.pin(args.backend), checks=len(results), compiler_credit=0, guest_credit=0,
                  python_source_parser=sys.version, parsed_tool_sources=parsed,
                  default_authority=[])
    (args.out / "results.json").write_text(json.dumps(result, indent=2) + "\n")


if __name__ == "__main__":
    _main()
