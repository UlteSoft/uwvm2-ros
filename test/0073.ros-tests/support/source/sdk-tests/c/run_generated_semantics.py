#!/usr/bin/env python3
"""Exercise actual generated-C defaults and protocol boundaries under the finite owner."""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys


RAW = Path("/home/macromodel/Documents/uwvm-test-261018/agents/02/c-export-r2/inputs/extra/custom-types/universal-robots-ros2-trajectory-complete/bounded_records.h")
RAW_SHA256 = "9ba26987a589507666b12e9c093fe752554c171ceea543f46493331867f7c011"
METADATA_SHA256 = "ad437e9f5fad609fc391334f091103bfdb1dcd5b847937a064a8abd004d9618e"


def pin(filename):
    raw = filename.read_bytes()
    return {"path": str(filename.resolve()), "bytes": len(raw), "sha256": hashlib.sha256(raw).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        parser.error("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs, work = run / "inputs", run / "work"
    raw_pin = pin(RAW)
    if raw_pin["sha256"] != RAW_SHA256:
        raise RuntimeError("captured actual generated header changed")
    source = inputs / "generated_semantics.c"
    adapter = inputs / "c_profiles.py"
    spec = importlib.util.spec_from_file_location("c_semantic_export", adapter)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    metadata_input = inputs / "generation.vendor.json"
    if pin(metadata_input)["sha256"] != METADATA_SHA256:
        raise RuntimeError("exact native generation manifest required")
    family = work / "source-family"
    family.mkdir()
    family_header = family / "bounded_records.h"
    family_metadata = family / "generation.vendor.json"
    original_header, original_metadata = RAW.read_bytes(), metadata_input.read_bytes()
    family_header.write_bytes(original_header)
    family_metadata.write_bytes(original_metadata)
    destination = "include/platform/ros/c/semantic/bounded_records.h"
    header, adaptation = module.transform(family_header, destination, original_header)
    context = module.native_reference_context(family_header)
    if len(adaptation["native_reference_bindings"]) != 32:
        raise RuntimeError("all32 reviewed records must match the actual manifest")
    if module.generated_documentation(header.decode(), context).encode() != header:
        raise RuntimeError("current generated documentation is not byte-idempotent")
    binding_checks = ["32 actual raw-output/native-reference manifest bindings"]

    def refused(label):
        try:
            module.transform(family_header, destination, original_header)
        except ValueError:
            binding_checks.append(label)
        else:
            raise RuntimeError("native metadata refusal missing: " + label)

    family_header.write_bytes(original_header + b"\n/* changed raw source */\n")
    refused("changed raw header refused")
    family_header.write_bytes(original_header)
    changed = json.loads(original_metadata)
    changed["outputs"]["bounded_records.h"]["sha256"] = "0" * 64
    family_metadata.write_text(json.dumps(changed))
    refused("wrong raw output pin refused")
    changed = json.loads(original_metadata)
    changed["source_inputs"]["definitions"].append(changed["source_inputs"]["definitions"][0])
    family_metadata.write_text(json.dumps(changed))
    refused("duplicate native source pin refused")
    changed["format"] = "unknown"
    family_metadata.write_text(json.dumps(changed))
    refused("unknown metadata format refused")
    changed = json.loads(original_metadata)
    for row in changed["source_inputs"]["definitions"]:
        if row["file"] == "sensor_msgs/msg/JointState.msg":
            row["sha256"] = "0" * 64
    family_metadata.write_text(json.dumps(changed))
    unused_header, changed_report = module.transform(family_header, destination, original_header)
    if "roswasmc_bounded_2_sensor_msgs_msg_JointState" in changed_report["native_reference_bindings"]:
        raise RuntimeError("unmatched reference was promoted into a native binding")
    binding_checks.append("different native source pin remains reference-only")
    family_metadata.unlink()
    unused_header, no_metadata_report = module.transform(family_header, destination, original_header)
    if no_metadata_report["native_reference_bindings"]:
        raise RuntimeError("header-only capture gained native provenance")
    binding_checks.append("absent sidecar remains reference-only")
    family_metadata.write_bytes(original_metadata)
    if module.native_reference_context(family_header) != context:
        raise RuntimeError("original native source manifest restoration differs")
    binding_checks.append("original exact manifest restored")
    if len(binding_checks) != 8:
        raise RuntimeError("native binding check denominator changed")
    include = work / "include"
    include.mkdir()
    output_header = include / "bounded_records.h"
    output_header.write_bytes(header)
    portable = include / "platform/ros/c/roswasm/detail/common/c_portability.h"
    portable.parent.mkdir(parents=True)
    shutil.copyfile(args.sdk / "include/platform/ros/c/roswasm/detail/common/c_portability.h", portable)
    commands = []
    counts = []
    for compiler in (Path("/usr/bin/gcc-15"), Path("/usr/bin/clang-22")):
        for mode in ("c89", "c23"):
            label = compiler.name + "-" + mode
            exe = work / label
            for operation, argv in (
                ("build", [str(compiler), "-std=" + mode, "-pedantic-errors", "-O2", "-I", str(include), str(source), "-o", str(exe)]),
                ("run", [str(exe)]),
            ):
                result = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=60, check=False)
                stem = label + "-" + operation
                (work / (stem + ".stdout")).write_bytes(result.stdout)
                (work / (stem + ".stderr")).write_bytes(result.stderr)
                commands.append({"label": stem, "argv": argv, "exit_code": result.returncode})
                if result.returncode:
                    raise RuntimeError("actual generated semantic consumer failed: " + stem)
                if operation == "run":
                    found = re.fullmatch(rb"C_GENERATED_SEMANTIC_CHECKS=([0-9]+)\n", result.stdout)
                    if not found or int(found[1]) != 115:
                        raise RuntimeError("actual semantic consumer check denominator missing")
                    counts.append(int(found[1]))
    if len(commands) != 8 or len(counts) != 4 or len(set(counts)) != 1:
        raise RuntimeError("complete native compiler/mode/check denominator required")
    if pin(RAW) != raw_pin:
        raise RuntimeError("borrowed raw header changed during verification")
    report = {"format": "task02-generated-semantic-consumers-v1", "status": "PASS_4_ACTUAL_NATIVE_CONSUMERS",
              "commands": commands, "native_consumer_runs": 4, "checks_per_consumer": counts[0],
              "raw_header": raw_pin, "adapted_header": pin(output_header), "adaptation": adaptation,
              "native_generation_metadata": pin(metadata_input), "native_binding_checks": binding_checks,
              "source": pin(source), "adapter": pin(adapter), "documentation": pin(inputs / "c_api_documentation.py"),
              "portability_header": pin(portable), "compilers": [pin(Path("/usr/bin/gcc-15")), pin(Path("/usr/bin/clang-22"))],
              "scope": ["explicit defaults versus zero aggregate", "finite nonunit orientation is not normalized", "parallel joint-array/name requirements are outside representation validation", "signed native outcome versus codec errno", "declared force-mode defaults and native range domains versus representation checks", "false Service payload versus successful codec result", "empty native request versus C placeholder"],
              "guest_runs": 0, "native_ros_runs": 0, "gateway_runs": 0, "physical_motion_runs": 0,
              "public_sdk_published": False}
    (work / "generated-semantics.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({"status": report["status"], "native_consumer_runs": 4, "checks_per_consumer": counts[0]}))


if __name__ == "__main__":
    main()
