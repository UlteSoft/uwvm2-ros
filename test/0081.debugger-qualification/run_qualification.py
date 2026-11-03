#!/usr/bin/env python3
"""Serial source/Wasm/ASM debugger qualification against an original fresh product.

The SSH keeper owns the outer exclusive lock/cgroup and process-tree deadline.
Selected stages may pass; full requested debugger restoration remains explicitly
unqualified while the recorded module/concurrency/producer gaps remain open.
"""
from __future__ import annotations

import argparse
import itertools
import json
import os
from pathlib import Path
import resource
import subprocess
import sys

sys.dont_write_bytecode = True
from qualification_support import admission, build_proof, packet_pins, require, sha

STAGES = ("protocol", "endpoint", "broker", "source-step", "source-values", "frame-scope", "cross-module", "blocked-wait")
DEFAULT = STAGES[:-1]
LANGUAGES = ("c", "cpp", "rust")
BROKER_CASES = ("source-into", "source-next", "source-finish", "wasm-native", "replacement", "replacement-script", "replacement-gc-funcref", "replacement-gc-tag")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("source-root", "uwvm", "build-receipt", "wasm-tools", "out"):
        parser.add_argument("--" + name, required=True, type=Path)
    for name in ("wasm-clang", "wasm-ld", "llvm-dwarfdump", "rustc"):
        parser.add_argument("--" + name, type=Path)
    parser.add_argument("--ros", action="store_true")
    parser.add_argument("--profile", choices=("smoke", "full"), default="smoke")
    parser.add_argument("--stage", choices=STAGES, action="append")
    args = parser.parse_args()
    require(sys.platform == "linux" and os.uname().machine == "x86_64", "Linux x86-64 keeper only")
    root = args.source_root.resolve(strict=True)
    packet = Path(__file__).resolve().parent
    require(packet == root / "test/0081.debugger-qualification", "packet must be copied whole into admitted source tree")
    before_admission = admission(root)
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    binary, receipt = args.uwvm.resolve(strict=True), args.build_receipt.resolve(strict=True)
    proof = build_proof(root, binary, receipt)
    stages = args.stage or DEFAULT
    require(len(stages) == len(set(stages)), "duplicate stages would overwrite evidence")
    for name in (("wasm_clang", "wasm_ld", "llvm_dwarfdump") if "broker" in stages else ()):
        require(getattr(args, name) is not None, "selected broker stage requires official tool", name)
    if any(stage in stages for stage in ("source-step", "source-values")):
        for name in ("wasm_clang", "wasm_ld", "llvm_dwarfdump", "rustc"):
            require(getattr(args, name) is not None, "selected source stage requires official producer tool", name)
    out = args.out.absolute()
    out.mkdir(parents=True, exist_ok=False)
    pins = packet_pins(packet)
    for directory in (root / "test/0017.runtime", root / "test/0018.debugger", root / "tools/debug"):
        for path in directory.glob("*.py"):
            pins[str(path)] = sha(path)
    for path in (binary, receipt, *(Path(value) for value in proof["fingerprint_paths"]),
                 *(value.absolute() for name, value in vars(args).items()
                   if name in ("wasm_tools", "wasm_clang", "wasm_ld", "llvm_dwarfdump", "rustc") and value is not None)):
        path.resolve(strict=True)
        pins[str(path)] = sha(path)
    result = dict(passed=False, profile=args.profile, product="uwvm2-ros" if args.ros else "uwvm2",
                  scope="selected actual product and separately identified formatter regressions",
                  proof={key: value for key, value in proof.items() if key != "production"},
                  inputs_before=pins, admission_before=before_admission, stages=[], commands=[],
                  full_requested_objective_qualified=False,
                  unresolved_gaps=json.loads((packet / "scope.json").read_text())["unresolved_gaps"])

    def save():
        (out / "summary.json").write_text(json.dumps(result, indent=2) + "\n")

    # No hidden recovery kills: the outer keeper owns the complete process tree.
    # A runner failure stops this serial packet immediately and preserves output.
    def run(label, argv):
        command = [str(value) for value in argv]
        log = out / (label + ".driver.log")
        record = dict(label=label, argv=command, cwd=str(root), log=str(log), returncode=None)
        result["commands"].append(record)
        save()
        with log.open("wb") as stream:
            completed = subprocess.run(command, cwd=root, stdout=stream, stderr=subprocess.STDOUT,
                                       env=dict(os.environ, PYTHONDONTWRITEBYTECODE="1", TERM="dumb", NO_COLOR="1"))
        record.update(returncode=completed.returncode, log_sha256=sha(log))
        save()
        require(completed.returncode == 0 and log.stat().st_size <= 32 * 1024 * 1024,
                "selected debugger stage failed or exceeded log budget", record)

    def checked(path, rows=None, fields=()):
        value = json.loads(path.read_text())
        require(value.get("passed") is True, "stage did not establish its actual requested behavior", path)
        if rows is not None:
            observed = value[rows]
            require(observed and all(row.get("passed") is True for row in observed), "stage includes failed/unavailable cases", path)
            if fields:
                tuples = [tuple(row[name] for name in fields) for row in observed]
                require(len(tuples) == len(set(tuples)), "duplicate case receipts", path)
        return value

    try:
        harness = root
        policies = ("instruction", "unwind")
        if args.ros:
            from prepare_ros_harness import prepare
            result["ros_harness_derivation"] = prepare(root, out / "ros-harness")
            harness = out / "ros-harness"
            policies = ("mandatory-native",)
        product = ["--uwvm", binary, "--build-receipt", receipt]
        tools = [value for name in ("wasm-clang", "wasm-ld", "rustc", "wasm-tools", "llvm-dwarfdump")
                 if getattr(args, name.replace("-", "_")) is not None
                 for value in ("--" + name, getattr(args, name.replace("-", "_")).absolute())]
        ros = ["--ros"] if args.ros else []
        versions = (5,) if args.profile == "smoke" else (4, 5)
        optimizations = (1,) if args.profile == "smoke" else (0, 1)
        step_cases = ("into", "next", "finish") if args.profile == "smoke" else ("into", "next", "finish", "inline-out", "repeated", "recursive")
        value_cases = ("aggregate", "lexical") if args.profile == "smoke" else ("aggregate", "lexical", "inline-vars", "next", "finish", "frame-selection", "inline-frame-selection", "scalar-constants")
        for stage in stages:
            row = dict(stage=stage, passed=False)
            result["stages"].append(row)
            destination = out / stage
            if stage == "protocol":
                run(stage + "-current-dump", [sys.executable, packet / "test_dap_memory_dump.py"])
                for name in ("test_dap_wasm_memory_pages", "test_dap_wasm_memory_writes", "test_dap_replacement_lifetime", "test_dap_frame_scope_lifetime"):
                    run(stage + "-" + name, [sys.executable, root / "test/0018.debugger" / (name + ".py")])
                row["scope"] = "formatter/protocol regressions only; no real VM qualification credit"
            elif stage == "endpoint":
                run(stage, [sys.executable, packet / "run_endpoint_lifecycle.py", "--source-root", root,
                            *product, "--wasm-tools", args.wasm_tools.absolute(), *ros, "--out", destination])
                detail = checked(destination / "summary.json", "rows", ("case", "policy"))
                expected = set(itertools.product(("quit-wasm", "eof-wasm", "quit-native", "eof-native", "foreign-peer"), policies))
                require({(item["case"], item["policy"]) for item in detail["rows"]} == expected, "endpoint matrix incomplete")
            elif stage == "broker":
                broker_tools = [value for name in ("wasm-clang", "wasm-ld", "wasm-tools", "llvm-dwarfdump")
                                for value in ("--" + name, getattr(args, name.replace("-", "_")).absolute())]
                run(stage, [sys.executable, harness / "test/0018.debugger/run_dap_current_broker.py", "--source-root", harness,
                            *product, *broker_tools, *ros, "--out", destination])
                detail = checked(destination / "summary.json", "cases", ("case", "policy"))
                require({(item["case"], item["policy"]) for item in detail["cases"]} == set(itertools.product(BROKER_CASES, policies)),
                        "actual DAP matrix incomplete")
            elif stage in ("source-step", "source-values"):
                script = "run_debug_source_step_cli.py" if stage == "source-step" else "run_debug_language_experience_cli.py"
                cases = step_cases if stage == "source-step" else value_cases
                options = [value for language in LANGUAGES for value in ("--language", language)]
                options += [value for version in versions for value in ("--dwarf-version", str(version))]
                options += [value for kind in cases for value in ("--case", kind)]
                if stage == "source-values":
                    options += [value for level in optimizations for value in ("--optimization", str(level))]
                run(stage, [sys.executable, harness / "test/0017.runtime" / script, "--source-root", harness,
                            *product, *tools, *ros, *options, "--out", destination])
                fields = ("language", "dwarf_version", "diagnostic_policy", "case") + (("optimization",) if stage == "source-values" else ())
                detail = checked(destination / "summary.json", "cases", fields)
                expected = set(itertools.product(LANGUAGES, versions, policies, cases, optimizations)) if stage == "source-values" else set(itertools.product(LANGUAGES, versions, policies, cases))
                require({tuple(item[name] for name in fields) for item in detail["cases"]} == expected, "actual language matrix incomplete")
            elif stage == "frame-scope":
                destination.mkdir()
                wasm = destination / "frame-scope.wasm"
                run(stage + "-parse", [args.wasm_tools.absolute(), "parse", harness / "test/0018.debugger/fixtures/debug_frame_scope_lifetime_dap.wat", "-o", wasm])
                run(stage + "-validate", [args.wasm_tools.absolute(), "validate", wasm])
                for policy in policies:
                    folder = destination / policy
                    run(stage + "-" + policy, [sys.executable, harness / "test/0018.debugger/run_dap_frame_scope_lifetime.py",
                                               "--uwvm", binary, "--wasm", wasm, "--policy", policy, *ros, "--out", folder])
                    checked(folder / "frame-scope-lifetime.json")
            elif stage == "cross-module":
                run(stage, [sys.executable, harness / "test/0017.runtime/run_debug_sealed_source_binding_pty.py",
                            "--uwvm", binary, "--wasm-tools", args.wasm_tools.absolute(), "--out", destination])
                detail = checked(destination / "result.json")
                require(len(detail["runs"]) == 3 and all(item["correct_leaf_and_suspended_callers"] and item["normal_exit"] for item in detail["runs"]),
                        "cross-module real frame/local ownership incomplete")
                row["scope"] = "three Wasm modules and exact caller locals; no cross-module DWARF filename claim"
            else:
                run(stage, [sys.executable, harness / "test/0018.debugger/run_wasm_blocked_wait_cli.py", "--source-root", harness,
                            "--binary", binary, "--wasm-tools", args.wasm_tools.absolute(), "--scenario", "resume", "--out", destination])
                rows = json.loads((destination / "results.json").read_text())
                require(len(rows) == 4 * len(policies) and all(item.get("passed") for item in rows), "actual blocked-wait matrix incomplete")
                row["scope"] = "real in-flight wait pause/mutation/resume; no multiworker hot-replacement credit"
            row["passed"] = True
            if destination.exists():
                row["artifacts"] = [{"path": str(path.relative_to(out)), "sha256": sha(path)}
                                    for path in sorted(destination.rglob("*.json")) if path.is_file()]
            save()
        result["admission_after"] = admission(root)
        require(result["admission_after"] == before_admission, "boot or cgroup changed during qualification")
        require(build_proof(root, binary, receipt) == proof, "original product/source/dependency closure changed")
        if args.ros:
            for path, digest in result["ros_harness_derivation"]["derived_test_inputs"].items():
                require(sha(path) == digest, "derived shared harness changed during execution", path)
        result["passed"] = len(result["stages"]) == len(stages) and all(row["passed"] for row in result["stages"])
    except BaseException as error:
        result["error"] = repr(error)
        raise
    finally:
        result["inputs_after"] = {path: sha(path) for path in pins}
        if result["inputs_after"] != pins:
            result.update(passed=False, inputs_changed=True)
        save()
    require(result["passed"], "selected debugger qualification failed")
    print("PASS selected debugger stages:", len(result["stages"]), "; full requested objective remains unqualified;", out / "summary.json")


if __name__ == "__main__":
    main()
