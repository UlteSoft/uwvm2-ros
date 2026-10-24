#!/usr/bin/env python3
"""Exercise the Unitree source stager with actual full shared language emissions.

Execute only as a finite Linux-owner workload. Real source association and
controlled invalid mutations are checked; this is not a language compiler,
Guest, native transport, published SDK or official-program acceptance suite.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import shutil
import tempfile


def require(value, message):
    if not value:
        raise AssertionError(message)


def refused(action):
    try:
        action()
    except (ValueError, KeyError, FileNotFoundError):
        return
    raise AssertionError("invalid source overlay was accepted")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--unitree", type=Path, required=True)
    parser.add_argument("--reference-api", type=Path, required=True)
    parser.add_argument("--emission", action="append", default=[], metavar="LANGUAGE=ROOT")
    parser.add_argument("--work", type=Path, required=True)
    args = parser.parse_args()
    unitree = args.unitree.resolve(strict=True)
    reference = args.reference_api.resolve(strict=True)
    selected = {}
    for item in args.emission:
        language, value = item.split("=",1)
        require(language not in selected, "duplicate source selection")
        selected[language] = Path(value).resolve(strict=True)
    require(set(selected) == {"nim", "go", "haskell", "d", "swift"}, "all five actual complete shared emissions required")
    args.work.mkdir(parents=True,exist_ok=False)
    script = unitree / "stage_language_sdk.py"
    source_pin = hashlib.sha256(script.read_bytes()).hexdigest()
    spec = importlib.util.spec_from_file_location("unitree_source_overlay",script)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    rows = []
    for language in module.LANGUAGES:
        files, receipt = module.plan(unitree,reference,language,selected.get(language))
        require(receipt["status"] == "SOURCE_ONLY" and receipt["compiled"] is False, "source-only scope required")
        require(receipt["Guest_runs"] == receipt["native_ROS_runs"] == 0, "execution must not be invented")
        require(receipt["default_authority"] == [] and receipt["sdk_publication"] is False, "source staging cannot publish/grant")
        require(files, "real installed sources required")
        for name, body in files.items():
            path = Path(name)
            require(not path.is_absolute() and ".." not in path.parts, "relative installed destination")
            require(not set(path.parts).intersection({"origin", "test", "tests", "target", "build"}), "no origin/tests/build dependencies in SDK source paths")
            require(isinstance(body,bytes) and body, "actual source bytes required")
        if language == "go":
            records = files["include/platform/unitree/go/records/bounded_records.go"].decode()
            require(records.startswith("//go:build wasip1 && wasm && ros2 && !ros1\n"), "official-Go tag required")
            require('ros "uwvm2.local/roswasmgo"' in records and 'ros "uwvm2.local/roswasm"' not in records, "existing official-Go foundation required")
            require(len(receipt["exact_source_rebases"]) == 1, "exact source transformation receipt required")
        if language == "d":
            require("example/platform/unitree/d/sport/main.c" in files, "real WASI C startup required")
        rows.append({"language":language,"files":len(files),"status":"SOURCE_ASSOCIATION_ONLY"})
    # Preserve actual emitted baselines. Mutants are only invalid inputs and
    # never stand in for genuine generator output or positive build evidence.
    with tempfile.TemporaryDirectory(dir=args.work,prefix="invalid-") as temporary:
        mutant = Path(temporary) / "emission"
        shutil.copytree(selected["nim"],mutant)
        manifest_path = mutant / "emission.json"
        original_manifest = manifest_path.read_bytes()
        facts_path = mutant / "native-records.json"
        original_facts = facts_path.read_bytes()
        def set_facts(facts):
            body = json.dumps(facts,indent=2).encode()+b"\n"
            facts_path.write_bytes(body)
            manifest = json.loads(original_manifest)
            for product in manifest["products"]:
                if product["path"] == "native-records.json":
                    product.update(bytes=len(body),sha256=hashlib.sha256(body).hexdigest())
            manifest_path.write_text(json.dumps(manifest,indent=2)+"\n")
        removed = json.loads(original_facts)
        removed["records"].pop()
        set_facts(removed)
        refused(lambda:module.plan(unitree,reference,"nim",mutant))
        changed = json.loads(original_facts)
        changed["records"][0]["codec_identity"] = "unrelated"
        set_facts(changed)
        refused(lambda:module.plan(unitree,reference,"nim",mutant))
        changed = json.loads(original_facts)
        changed["records"][0]["native_identity"] = "fabricated-native-identity"
        set_facts(changed)
        refused(lambda:module.plan(unitree,reference,"nim",mutant))
        changed = json.loads(original_facts)
        changed["records"][0]["members"].pop()
        set_facts(changed)
        refused(lambda:module.plan(unitree,reference,"nim",mutant))
        changed = json.loads(original_facts)
        changed["source_inputs"]["catalog"]["sha256"] = "0"*64
        set_facts(changed)
        refused(lambda:module.plan(unitree,reference,"nim",mutant))
        facts_path.write_bytes(original_facts)
        manifest_path.write_bytes(original_manifest)
        source = mutant / "nim/records.nim"
        original_source = source.read_bytes()
        source.write_bytes(original_source+b"# changed\n")
        refused(lambda:module.plan(unitree,reference,"nim",mutant))
        source.write_bytes(original_source)
        source.rename(source.with_suffix(".saved"))
        source.symlink_to(source.with_suffix(".saved").name)
        refused(lambda:module.plan(unitree,reference,"nim",mutant))
    refused(lambda:module.plan(unitree,reference,"nim"))
    refused(lambda:module.plan(unitree,reference,"zig",selected["nim"]))
    refused(lambda:module.official_go(b'package customtypes\n'))
    require(hashlib.sha256(script.read_bytes()).hexdigest() == source_pin, "stager changed during contract")
    result = {"status":"PASS_SOURCE_CONTRACT_ONLY","source_sha256":source_pin,
              "language_source_plans":rows,"negative_contracts":10,
              "compiler_runs":0,"Guest_runs":0,"native_ROS_runs":0,
              "default_authority":[],"whole_sdk_publication":False}
    (args.work/"result.json").write_text(json.dumps(result,indent=2)+"\n")
    print(json.dumps(result,sort_keys=True))


if __name__ == "__main__":
    main()
