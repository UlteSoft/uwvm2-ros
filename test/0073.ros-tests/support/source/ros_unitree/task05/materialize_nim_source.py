#!/usr/bin/env python3
"""Materialize one genuine Nim source overlay through the actual export CLI.

Source-only delivery candidate; not public assembly, compilation, an xmake
receipt, a native type registration or permission. Uses actual complete emitted
records, real original C references and the reviewed Linux formatter postimages.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import shutil
import subprocess


def pin(path):
    body = path.read_bytes()
    return {"bytes": len(body), "sha256": hashlib.sha256(body).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("unitree", "nim-frontend-run", "reference-api", "emission"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    work = Path.cwd()
    captured = {}

    def observe(path):
        path = path.absolute()
        captured[str(path)] = pin(path)
        return path

    original = args.unitree.resolve(strict=True)
    source = work / "source/unitree"
    source.mkdir(parents=True)
    leaves = ["stage_language_sdk.py", "sport.cpp", "b2_sport.cpp", "packet_bridge.c",
              "packet_json.cpp", "NOTICE.md", "language/README.md",
              "recipes/wasm32-command-memory.json"]
    leaves += ["include/roswasm_unitree/" + p.name for p in sorted((original / "include/roswasm_unitree").iterdir())
               if p.suffix in (".h", ".hpp", ".inc")]
    for leaf in leaves:
        output = source / leaf
        output.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(observe(original / leaf), output)
    frontend = json.loads(observe(args.nim_frontend_run / "work/result.json").read_text())
    if frontend["status"] != "PASS_FORMAT_AND_FRONTEND_ONLY":
        raise ValueError("actual successful Nim formatting/frontend receipt required")
    for name, leaf in (("unitree_sport.nim", "language/nim/unitree_sport.nim"),
                       ("sport_topic_command.nim", "examples/nim/sport_topic_command.nim")):
        input_path = observe(args.nim_frontend_run / "work/source" / name)
        if frontend["formatted"][str(input_path)] != pin(input_path):
            raise ValueError("actual Nim formatter postimage changed")
        output = source / leaf
        output.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(input_path, output)
    for root in (args.reference_api, args.emission):
        for path in sorted(root.rglob("*")):
            if path.is_file():
                observe(path)
    hook = source / "stage_language_sdk.py"
    target = work / "SDK source with spaces-路径"
    command = ["/usr/bin/python3", "-I", "-S", "-B", str(hook), "--unitree", str(source),
               "--reference-api", str(args.reference_api), "--language", "nim",
               "--emission", str(args.emission), "--out", str(target)]
    with (work / "export.stdout").open("xb") as out, (work / "export.stderr").open("xb") as err:
        result = subprocess.run(command, stdout=out, stderr=err, check=False)
    if result.returncode:
        raise ValueError("actual source export CLI failed; original diagnostics retained")
    spec = importlib.util.spec_from_file_location("task05_actual_nim_export", hook)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    expected, receipt = module.plan(source, args.reference_api, "nim", args.emission)
    actual_receipt = json.loads((target / "source-overlay.json").read_text())
    if actual_receipt != receipt or set(p.relative_to(target).as_posix() for p in target.rglob("*") if p.is_file()) != set(expected) | {"source-overlay.json"}:
        raise ValueError("actual source CLI membership differs from the real export plan")
    products = {}
    for leaf, body in expected.items():
        path = target / leaf
        if path.is_symlink() or path.read_bytes() != body:
            raise ValueError("actual source CLI output differs: " + leaf)
        products[leaf] = pin(path)
    relocated = work / "Relocated source SDK-搬移"
    target.rename(relocated)
    if any(pin(relocated / leaf) != value for leaf, value in products.items()):
        raise ValueError("actual materialized source bytes changed after relocation")
    for path, before in captured.items():
        if pin(Path(path)) != before:
            raise ValueError("original source, frontend or emission input changed")
    report = {"status": "PASS_ACTUAL_NIM_SOURCE_CLI_AND_SOURCE_RELOCATION_ONLY",
              "source_and_tool_inputs": captured, "export_argv": command,
              "actual_exported_files": len(products), "products": products,
              "relocated_source_root": str(relocated), "source_overlay": pin(relocated / "source-overlay.json"),
              "compiler_runs": 0, "Guest_runs": 0, "native_ROS_runs": 0,
              "public_SDK_assembly_qualified": False, "xmake_runs": 0, "default_authority": []}
    (work / "result.json").write_text(json.dumps(report, indent=2, ensure_ascii=False)+"\n")
    print(json.dumps({"status": report["status"], "actual_exported_files": len(products)}))


if __name__ == "__main__":
    main()
