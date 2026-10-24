#!/usr/bin/env python3
"""Run the unchanged original native regressions in one bounded ARM/QEMU lane.

Required interface: --source --manifest --config --sdk-manifest
--compiler-resource-root --out. Execute this script from the complete frozen
source, inside the already authorized 64-GiB, zero-swap Linux cgroup. It neither
creates a cgroup nor starts ROS, a Gateway, a Guest, a network witness, or xmake.
The supplied compiler configuration has the same compiler/compile_flags/
link_flags/qemu_argv/environment fields as run_unit.py.
An explicit environment.LD_LIBRARY_PATH is a HOST_COMPILER-only input. Its
canonical directories must remain inside the selected physical compiler's
prefix/lib tree, whose complete regular files and contained symlink aliases
are pinned before/after each chunk and replayed by aggregation. The closed
native environment never receives this host loader setting. This is an input
closure, not a claim to have resolved the compiler's DT_NEEDED dependency graph.

Inventory is closed: 94 original WASIp1 function sources, 11 hybrid sources,
five local-imported sources, and all 314 interpreter sources. The two explicit
LLVM-only wasm3 harnesses are recorded as EXCLUDED_BACKEND, never test PASS.
The 241 executable strict sources run all twelve combine/delay configurations
from the original strict script; its five validator sources run heavy/heavy.
The other portable interpreter sources run none/none. This is interpreter-only
regression coverage, not the independent LLVM, sanitizer, timing or cross-CPU
matrix. An intentional compile-time concept test is identified separately.

Run one portable chunk with --groups wasip1-functions wasip1-hybrid
local-imported uwvm-int-portable uwvm-int-validate (181 configurations), and
twelve chunks with --groups uwvm-int-strict --strict-config COMBINE:DELAY
(241 configurations each). Optional --strict-shard INDEX/COUNT partitions only
the strict source names, with COUNT <= 32. Give all completed results.json files
to a fresh invocation using repeatable --aggregate-input. It performs read-only
qualification of the exact 3073 keys and original target bytes, logs and map
proofs; it neither recompiles nor executes the already tested products.

Eight original wasm3 harnesses need external fixture argv. The configuration
must name original_regression_fixtures_report, an opened/pinned PREPARED report
produced from this same frozen source by prepare_original_fixtures.py. Missing
fixtures produce REQUIRED_FIXTURE and an INCOMPLETE verdict with exit 1. No
empty argv invocation, disabled stub or omitted file can satisfy this gate.

Each original translation unit is unchanged. A separate linker --wrap=main
witness prints one exact marker only after its real main returns zero. A
constructor stops the actual ARM image before main; a retained birth/pidfd,
live cgroup membership, ARM map and pinned x86 QEMU image are required before
CONT. Clang's filtered AST also proves that the compiled main is not an empty
platform/feature stub. Compilers and actors run serially with a 2-GiB observed
aggregate RSS limit, bounded logs/deadlines and exact-owned cleanup.
"""
from __future__ import annotations

import argparse
import importlib.util
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import signal
import stat
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
from runtime import Evidence, EXPECTED_MEMORY_MAX, cgroup_incarnation, cgroup_oom_deltas, digest, read_json_binding, require_arm
from gateway_qualification import read_file_binding, pins_preserved
from run_unit import BARRIER, worker_test_configuration, sdk_root_from_flags, verify_sdk_inventory, resource_inventory, process_tree_rss
from run_ros1_search import OwnedProcesses
from run_action_qos import process_identity

FIXTURE_NAMES = (
    "dead_codegen_model", "gc_two_phase_policy", "gc_two_phase_policy_declarations",
    "memory_layout_probe", "memory_performance", "memory_store_atomicity",
    "ref_branch_control_flow", "ref_join_type_preservation",
)
FIXTURE_RUN_COUNTS = {"memory_performance": 24, "memory_layout_probe": 1, "memory_store_atomicity": 104,
    "dead_codegen_model": 1, "ref_join_type_preservation": 1, "ref_branch_control_flow": 1,
    "gc_two_phase_policy": 1, "gc_two_phase_policy_declarations": 2}
BACKEND_EXCLUSIONS = {
    "test/0013.uwvm_int/wasm3/memory_performance_jit.cc":
        "Original source explicitly defines UWVM2TEST_RUNNER_USE_LLVM_JIT and calls llvm_jit_call_raw_host_api; requires a separate LLVM runtime and timing fixtures.",
    "test/0013.uwvm_int/wasm3/memory_store_atomicity_jit.cc":
        "Original source explicitly defines UWVM2TEST_RUNNER_USE_LLVM_JIT and calls llvm_jit_call_raw_host_api; requires a separate LLVM runtime and store fixtures.",
}
RUNTIME_SOURCES = (
    "src/uwvm2/runtime/lib/uwvm_runtime.default.cpp",
    "src/uwvm2/uwvm/host_api.default.cpp",
)
RUNTIME_TESTS = {
    "test/0013.uwvm_int/uwvm_int_fp_environment.cc",
    "test/0013.uwvm_int/uwvm_int_fp_bit_environment.cc",
}
COMBINE_MODES = ("none", "soft", "heavy", "extra")
DELAY_MODES = ("none", "soft", "heavy")
RSS_LIMIT = 2 * 1024**3
LOG_LIMIT = 16 * 1024**2
COMPILE_SECONDS = 600
RUN_SECONDS = 180
FIXTURE_RUN_SECONDS = {"memory_layout_probe": 3600}
CHUNK_GROUPS = ("wasip1-functions", "wasip1-hybrid", "local-imported", "uwvm-int-portable", "uwvm-int-strict", "uwvm-int-validate")
STRICT_CONFIGS = tuple(combine+":"+delay for combine in COMBINE_MODES for delay in DELAY_MODES)


class ResourceGuardError(RuntimeError):
    """A changed cgroup/disk budget aborts the chunk, not thousands of retries."""


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def atomic_json(path, value):
    temporary = path.with_name(path.name + ".new")
    with temporary.open("x", encoding="utf-8") as stream:
        json.dump(value, stream, indent=2); stream.write("\n"); stream.flush(); os.fsync(stream.fileno())
    os.replace(temporary, path)


def compiler_loader_selection(config):
    """Select only the explicit compiler loader path; absence selects nothing.

    The ordinary environment policy still refuses every other LD_, DYLD_, ROS_,
    RMW_, QEMU_ and UWVM2TEST_ override. No ambient environment is consulted, and
    neither the compiler's RUNPATH nor a system library directory is inferred.
    """
    overrides = config.get("environment", {})
    require(type(overrides) is dict and all(type(key) is str and type(value) is str
            and "\x00" not in key+value and "=" not in key and key
            for key, value in overrides.items()) and not any(
            key.startswith(("ROS_", "RMW_", "QEMU_", "LD_", "DYLD_", "UWVM2TEST_"))
            and key != "LD_LIBRARY_PATH" for key in overrides),
            "configuration may not inherit ROS/loader/backend test policy")
    selected = overrides.get("LD_LIBRARY_PATH")
    if selected is None:
        return {"status": "NOT_SELECTED", "environment_role": "HOST_COMPILER",
                "explicit_ld_library_path": None, "compiler": None,
                "library_root": None, "selected_directories": []}
    require(selected and "\n" not in selected and "\r" not in selected,
            "explicit compiler LD_LIBRARY_PATH must be a nonempty directory list")
    compiler = Path(config["compiler"][0]).resolve(strict=True)
    require(compiler.is_file(), "selected physical compiler must be a regular file")
    root = compiler.parent.parent/"lib"
    require(root.is_dir() and not root.is_symlink() and root.resolve(strict=True) == root,
            "compiler loader root must be the canonical selected compiler prefix/lib")
    directories = selected.split(":")
    require(all(directories) and len(directories) == len(set(directories)),
            "compiler loader directories may not be empty or duplicated")
    for directory in directories:
        path = Path(directory)
        require(path.is_absolute() and str(path) == directory and path.is_dir()
                and path.resolve(strict=True) == path and path.is_relative_to(root),
                "compiler loader directory is not canonical within selected compiler prefix/lib")
    return {"status": "SELECTED", "environment_role": "HOST_COMPILER",
            "explicit_ld_library_path": selected, "compiler": str(compiler),
            "library_root": str(root), "selected_directories": directories}


def compiler_loader_inventory(selection):
    """Stream the entire immutable selected lib tree, not only discovered .so files.

    Opened file bindings retain descriptor identity, size and SHA-256. Symlinks
    are never traversed during the walk, must resolve inside this same tree,
    and retain their literal link text plus identity. Directory mtimes are not
    identities; path/device/inode/mode are. No payload bytes are held in memory.
    """
    result = {"format_version": 1, "kind": "host-compiler-loader-input-closure",
              "selection": selection, "directories": {}, "files": {}, "aliases": {},
              "regular_file_count": 0, "regular_file_bytes": 0,
              "scope": "complete selected compiler prefix/lib input tree; no DT_NEEDED graph claim"}
    if selection["status"] == "NOT_SELECTED":
        return result
    root = Path(selection["library_root"])
    require(compiler_loader_selection({"compiler": [selection["compiler"]],
            "environment": {"LD_LIBRARY_PATH": selection["explicit_ld_library_path"]}}) == selection,
            "selected compiler loader paths changed")
    def failed_walk(error):
        raise error
    for directory, names, files in os.walk(root, followlinks=False, onerror=failed_walk):
        names.sort(); files.sort()
        parent = Path(directory)
        require(parent.resolve(strict=True) == parent and parent.is_relative_to(root),
                "compiler library directory escaped its canonical root")
        info = parent.lstat()
        require(stat.S_ISDIR(info.st_mode), "compiler library directory changed type")
        result["directories"][str(parent.relative_to(root))] = {
            "path": str(parent), "device": info.st_dev, "inode": info.st_ino,
            "mode": stat.S_IMODE(info.st_mode)}
        for name in sorted([*names, *files]):
            path = parent/name; info = path.lstat(); relative = str(path.relative_to(root))
            if stat.S_ISLNK(info.st_mode):
                target = os.readlink(path); resolved = path.resolve(strict=True)
                literal = Path(target) if Path(target).is_absolute() else path.parent/target
                require(Path(os.path.normpath(literal)).is_relative_to(root)
                        and resolved.is_relative_to(root) and (resolved.is_file() or resolved.is_dir()),
                        "compiler library alias escapes root or names an unsupported object")
                result["aliases"][relative] = {"path": str(path), "device": info.st_dev,
                    "inode": info.st_ino, "mtime_ns": info.st_mtime_ns, "ctime_ns": info.st_ctime_ns,
                    "target": target, "resolved_relative": str(resolved.relative_to(root))}
                last = path.lstat()
                require(all(getattr(last, key) == getattr(info, key) for key in
                        ("st_dev", "st_ino", "st_mode", "st_mtime_ns", "st_ctime_ns"))
                        and os.readlink(path) == target,
                        "compiler library alias changed during inventory")
            elif stat.S_ISREG(info.st_mode):
                require(path.resolve(strict=True) == path, "compiler library file alias changed")
                pin = read_file_binding(path)
                require(pin["device"] == info.st_dev and pin["inode"] == info.st_ino,
                        "compiler library file changed before descriptor binding")
                result["files"][relative] = pin
                result["regular_file_bytes"] += pin["bytes"]
            else:
                require(stat.S_ISDIR(info.st_mode) and name in names,
                        "compiler library tree contains an unsupported special object")
            require(len(result["files"])+len(result["aliases"])+len(result["directories"]) <= 16384
                    and result["regular_file_bytes"] <= 4*1024**3,
                    "compiler library input tree exceeds its explicit streamed inventory bound")
    result["regular_file_count"] = len(result["files"])
    require(result["files"], "selected compiler library root has no regular files")
    for identity in result["directories"].values():
        path = Path(identity["path"]); info = path.lstat()
        require(stat.S_ISDIR(info.st_mode) and path.resolve(strict=True) == path and identity == {
            "path": str(path), "device": info.st_dev, "inode": info.st_ino,
            "mode": stat.S_IMODE(info.st_mode)}, "compiler library directory identity changed during inventory")
    return result


def compiler_command_environment(config, selection, argv, native_environment, target):
    """Apply host loader bytes only to an exact configured compiler argv prefix."""
    prefix = config["compiler"]
    compiler = len(argv) >= len(prefix) and argv[:len(prefix)] == prefix
    require("LD_LIBRARY_PATH" not in native_environment, "native environment contains host loader input")
    environment = dict(native_environment)
    if compiler:
        require(target is None, "a native target cannot be classified as host compiler")
        if selection["explicit_ld_library_path"] is not None:
            environment["LD_LIBRARY_PATH"] = selection["explicit_ld_library_path"]
    else:
        require(target is not None and argv[:len(config["qemu_argv"])] == config["qemu_argv"],
                "noncompiler command must use the unchanged explicit native QEMU argv")
    return environment, {"environment_role": "HOST_COMPILER" if compiler else "NATIVE_TARGET",
        "host_compiler_ld_library_path": selection["explicit_ld_library_path"] if compiler else None,
        "native_host_loader_environment_cleared": not compiler}


def qualify_compiler_loader(value, selection, current_inventory, inputs):
    """Reopen both chunk snapshots and compare their full catalogs with current files."""
    closure = value.get("host_compiler_loader")
    require(type(closure) is dict and set(closure) == {"selection", "before", "after", "inputs_preserved"}
            and closure["selection"] == selection and closure["inputs_preserved"] is True,
            "chunk explicit compiler loader selection/preservation differs")
    for field in ("before", "after"):
        old = closure[field]; snapshot, pin = read_json_binding(old["path"], limit=16*1024**2)
        require(pin == old and snapshot == current_inventory,
                "chunk complete compiler library files/aliases/current root differ")
        inputs.append(pin)
    inputs.extend(current_inventory["files"].values())


def inventory(source):
    """Fail closed if the established inventory changes; never shrink by glob."""
    groups = {
        "wasip1-functions": sorted((source/"test/0008.imported/wasi/wasip1/func").glob("*.cc")),
        "wasip1-hybrid": sorted((source/"test/0008.imported/wasi/wasip1/hybrid").glob("*.cc")),
        "local-imported": sorted((source/"test/0010.local_imported").glob("*.cc")),
        "uwvm-int": sorted((source/"test/0013.uwvm_int").rglob("*.cc")),
    }
    require({key: len(value) for key, value in groups.items()} ==
            {"wasip1-functions": 94, "wasip1-hybrid": 11, "local-imported": 5, "uwvm-int": 314},
            "original inventory differs from the reviewed 94/11/5/314 source set")
    require(sum("/strict/" in str(path) for path in groups["uwvm-int"]) == 246,
            "original strict inventory must contain exactly246 sources")
    require(sum("/strict/validate/" in str(path) for path in groups["uwvm-int"]) == 5,
            "original strict validator inventory must contain exactly5 sources")
    rows = []
    for group, paths in groups.items():
        for path in paths:
            relative = str(path.relative_to(source))
            require(not path.is_symlink() and path.is_file(), "original test must be a frozen regular file")
            if relative in BACKEND_EXCLUSIONS:
                text = path.read_text(encoding="utf-8-sig")
                require("#define UWVM2TEST_RUNNER_USE_LLVM_JIT 1" in text and
                        "llvm_jit_call_raw_host_api" in text, "backend exclusion no longer matches original source")
                rows.append({"group": group, "original_source": relative, "status": "EXCLUDED_BACKEND",
                             "reason": BACKEND_EXCLUSIONS[relative]})
                continue
            modes = [("heavy", "heavy")] if "/strict/validate/" in relative else (
                [(combine, delay) for combine in COMBINE_MODES for delay in DELAY_MODES]
                if "/strict/" in relative else [("none", "none")])
            for combine, delay in modes:
                name = group + "-" + path.stem + "-" + combine + "-" + delay
                require(re.fullmatch(r"[a-zA-Z0-9_-]+", name), "original witness name must be a fixed safe literal")
                rows.append({"suite": name, "group": group, "original_source": relative,
                             "combine": combine, "delay": delay, "status": "NOT_PERFORMED",
                             "fixture_required": path.stem in FIXTURE_NAMES,
                             "compile_time_contract": relative == "test/0010.local_imported/concept.cc"})
    require(len(rows) == 3075 and len({row.get("suite", row["original_source"]) for row in rows}) == len(rows),
            "closed original regression configuration inventory differs")
    return rows


def mode_flags(combine, delay):
    flags = []
    if combine != "none": flags.append("-DUWVM_ENABLE_UWVM_INT_COMBINE_OPS")
    if combine in ("heavy", "extra"): flags.append("-DUWVM_ENABLE_UWVM_INT_HEAVY_COMBINE_OPS")
    if combine == "extra": flags.append("-DUWVM_ENABLE_UWVM_INT_EXTRA_HEAVY_COMBINE_OPS")
    if delay != "none": flags.append("-DUWVM_ENABLE_UWVM_INT_DELAY_LOCAL_SOFT")
    if delay == "heavy": flags.append("-DUWVM_ENABLE_UWVM_INT_DELAY_LOCAL_HEAVY")
    return flags


def chunk_group(row):
    if row["group"] != "uwvm-int": return row["group"]
    if "/strict/validate/" in row["original_source"]: return "uwvm-int-validate"
    if "/strict/" in row["original_source"]: return "uwvm-int-strict"
    return "uwvm-int-portable"


def selected_rows(rows, groups, strict_config, strict_shard=None):
    """A chunk owns exactly its declared keys; unselected keys are not PASS."""
    result = []
    original_strict = sorted({row["original_source"] for row in rows if chunk_group(row) == "uwvm-int-strict"})
    shard_index, shard_count = strict_shard if strict_shard is not None else (0, 1)
    assigned = {name for index, name in enumerate(original_strict) if index % shard_count == shard_index}
    for row in rows:
        if row["status"] == "EXCLUDED_BACKEND":
            if "uwvm-int-portable" in groups: result.append(row)
            continue
        if chunk_group(row) not in groups: continue
        if (chunk_group(row) == "uwvm-int-strict" and strict_config is not None and
                row["combine"]+":"+row["delay"] != strict_config): continue
        if chunk_group(row) == "uwvm-int-strict" and row["original_source"] not in assigned: continue
        result.append(row)
    require(result, "empty original regression chunk is not a test")
    return result


def strict_shard(value):
    require(type(value) is str and re.fullmatch(r"[1-9][0-9]?/[1-9][0-9]?", value), "strict shard must be INDEX/COUNT")
    index, count = map(int, value.split("/"))
    require(1 <= index <= count <= 32, "strict shard index/count must lie in1..32")
    return (index-1, count)


def guard_header(group, wide, socket, strict):
    """Reject target/feature choices which would turn original main into a stub.

    The header restores the project's feature macros before the unchanged
    original source sees them. Existing macro-leak assertions remain effective.
    """
    text = """#include <signal.h>
#if !defined(__linux__) || !defined(__aarch64__) || defined(__APPLE__)
#error "Original regression lane requires Linux AArch64, not a platform stub."
#endif
#if defined(NDEBUG) || !defined(UWVM_USE_UWVM_INT) || !defined(UWVM_DISABLE_JIT)
#error "Assertions and the non-JIT interpreter must remain enabled."
#endif
#if defined(UWVM_MODULE) || defined(UWVM_RUNTIME_LLVM_JIT) || defined(UWVM2TEST_RUNNER_USE_LLVM_JIT)
#error "This lane does not qualify modules or LLVM backend replacements."
#endif
"""
    if strict:
        text += """#if defined(UWVM2TEST_STRICT_NO_INTERPRETER)
#error "Strict interpreter main may not be replaced by a disabled stub."
#endif
#if !defined(__cpp_exceptions)
#error "Original strict validator catch/assertion contracts require C++ exceptions."
#endif
"""
    if group.startswith("wasip1-"):
        text += "#include <uwvm2/imported/wasi/wasip1/feature/feature_push_macro.h>\n"
        text += "#if !defined(UWVM_IMPORT_WASI_WASIP1)\n#error \"WASIp1 original test may not be disabled.\"\n#endif\n"
        if wide:
            text += "#if !defined(UWVM_IMPORT_WASI_WASIP1_WASM64)\n#error \"Wasm64 original test may not be disabled.\"\n#endif\n"
        if socket:
            text += "#if !defined(UWVM_IMPORT_WASI_WASIP1_SUPPORT_SOCKET)\n#error \"Socket original test may not be disabled.\"\n#endif\n"
        text += "#include <uwvm2/imported/wasi/wasip1/feature/feature_pop_macro.h>\n"
    text += BARRIER
    return text


def return_witness(name, has_arguments):
    declaration = "int, char**" if has_arguments else ""
    call = "argc, argv" if has_arguments else ""
    return """// Generated linker witness; no original test source/assertion is edited.
#include <fast_io.h>
#include <cstdio>
extern "C" int __real_main(""" + declaration + """);
extern "C" int __wrap_main(int argc, char** argv) {
    int const result{__real_main(""" + call + """)};
    if(result != 0) { return result; }
    // Original fixtures use stdio too. Preserve their completed diagnostics
    // before the fast_io successful-return marker; do not rewrite their I/O.
    if(::std::fflush(nullptr) != 0) { return 125; }
    ::fast_io::io::println("\\nPASS_ORIGINAL_REGRESSION case=""" + name + """ return=0");
    return 0;
}
"""


def main_ast(path, compile_time_contract):
    """Inspect the actual filtered Clang AST, not an unprocessed source guess.

    Clang may print several independent JSON values for a substring filter.
    Only the global FunctionDecl named main and its real CompoundStmt count.
    A concept test's intentional return-zero main is allowed only when the
    original source's compile-time assertions are separately retained below.
    """
    require(path.stat().st_size <= LOG_LIMIT, "filtered main AST exceeds bounded log size")
    raw = path.read_bytes().decode("utf-8", errors="strict")
    decoder, position, mains = json.JSONDecoder(), 0, []
    while position < len(raw):
        while position < len(raw) and raw[position].isspace(): position += 1
        if position == len(raw): break
        value, position = decoder.raw_decode(raw, position)
        if value.get("kind") == "FunctionDecl" and value.get("name") == "main":
            bodies = [item for item in value.get("inner", []) if item.get("kind") == "CompoundStmt"]
            if bodies: mains.append((value, bodies))
    require(len(mains) == 1 and len(mains[0][1]) == 1, "exact one actual compiled main body required")
    declaration, bodies = mains[0]; statements = bodies[0].get("inner", [])
    only_zero = len(statements) == 1 and statements[0].get("kind") == "ReturnStmt" and all(
        item.get("kind") == "IntegerLiteral" and item.get("value") == "0"
        for item in statements[0].get("inner", []))
    require(bool(statements) and (not only_zero or compile_time_contract),
            "compiled original main is an empty/return-zero feature or platform stub")
    parameters = [item for item in declaration.get("inner", []) if item.get("kind") == "ParmVarDecl"]
    require(len(parameters) in (0, 2), "original main has an unsupported ABI signature")
    return {"main_count": 1, "statement_count": len(statements), "parameter_count": len(parameters),
            "intentional_compile_time_contract": compile_time_contract,
            "compiled_stub_refused": not compile_time_contract, "ast": read_file_binding(path)}


def validate_fixture_arguments(name, arguments, paths):
    """Validate each original harness's closed argv shape before invocation."""
    require(type(arguments) is list and arguments and all(type(value) is str and value and
            "\x00" not in value for value in arguments), "original fixture arguments must be nonempty strings")
    require(type(paths) is list and paths and len(set(paths)) == len(paths) and
            all(type(value) is str and Path(value).is_absolute() and value in arguments for value in paths),
            "actual fixture paths must be explicit absolute argv inputs")
    if name in ("dead_codegen_model", "ref_join_type_preservation"):
        require(len(arguments) % 2 == 0 and all(arguments[i] in ("valid", "invalid") and
                arguments[i+1] in paths for i in range(0, len(arguments), 2)), "invalid original valid/invalid pairs")
    elif name in ("gc_two_phase_policy", "gc_two_phase_policy_declarations"):
        start = 0
        if name == "gc_two_phase_policy_declarations" and arguments[0] == "--provider":
            require(len(arguments) >= 5 and arguments[1] in paths, "explicit declarations provider bytes missing")
            start = 2
        require((len(arguments)-start) % 3 == 0 and len(arguments) > start and all(arguments[i] in ("accept", "reject") and
                arguments[i+1] in ("type-gc", "type-no-gc") and arguments[i+2] in paths
                for i in range(start, len(arguments), 3)), "invalid original GC policy triples")
    elif name == "ref_branch_control_flow":
        require(set(arguments) == set(paths), "reference control flow requires only actual Wasm paths")
    elif name == "memory_layout_probe":
        require(len(arguments) == 1 and arguments[0] in paths, "memory layout probe requires one actual Wasm")
    elif name == "memory_performance":
        require(len(arguments) == 3 and arguments[0] in paths and arguments[1].isascii() and
                arguments[1].isdecimal() and 1 <= int(arguments[1]) <= 100000000 and
                arguments[2].isascii() and arguments[2].isdecimal() and 1 <= int(arguments[2]) <= 100,
                "memory performance requires Wasm, bounded iterations and samples")
    elif name == "memory_store_atomicity":
        require(2 <= len(arguments) <= 4 and arguments[0] in paths and arguments[1] in ("2", "4", "8", "16")
                and len(arguments[2:]) == len(set(arguments[2:])) and
                set(arguments[2:]) <= {"growth", "memory64"}, "invalid original store atomicity argv")
    else:
        raise RuntimeError("unknown original fixture harness")
    used = {value for value in arguments if value in paths}
    require(used == set(paths), "fixture argv contains an unused or missing path")


def aggregate(args, source, frozen, config, manifest_pin, config_pin, sdk_pin, report, checked):
    """Qualify immutable completed chunks without running any old test again.

    All 3073 configuration keys are required exactly once. Target byte/FD
    identity, every actual command/log/depfile, successful-return marker, the
    pre-main stopped mapping, complete source/SDK/tool closure, and current
    boot/leaf are replayed. The aggregate architecture file copies *recorded*
    target samples with per-chunk report pins. It never labels aggregation as
    fresh target execution or infers mappings from an ELF pathname.
    """
    expected = {row["suite"]: row for row in inventory(source) if row["status"] != "EXCLUDED_BACKEND"}
    accepted, targets, samples, launches, inputs, chunks, architectures = {}, {}, [], [], [], [], []
    seen_invocation_births = set()
    loader_selection = compiler_loader_selection(config)
    loader_before = compiler_loader_inventory(loader_selection)
    loader_before_path = args.out.resolve()/"host-compiler-loader-before.json"
    atomic_json(loader_before_path, loader_before)
    report["host_compiler_loader"] = {"selection": loader_selection,
        "before": read_file_binding(loader_before_path), "after": None, "inputs_preserved": False}
    inputs.append(report["host_compiler_loader"]["before"])
    preparation_path = config.get("original_regression_fixtures_report")
    require(type(preparation_path) is str and Path(preparation_path).is_absolute(),
            "complete aggregate requires the explicit original fixture preparation report")
    preparation, preparation_pin = read_json_binding(preparation_path, limit=64*1024**2)
    inputs.append(preparation_pin)
    require(preparation.get("kind") == "original-regression-fixtures" and preparation.get("status") == "PREPARED"
            and preparation.get("source") == str(source) and preparation.get("source_manifest_sha256") == manifest_pin["sha256"]
            and preparation.get("source_preserved") is True and preparation.get("inputs_preserved") is True,
            "aggregate original fixture preparation source/status differs")
    prepared_cases = preparation.get("cases")
    require(type(prepared_cases) is dict and set(prepared_cases) == set(FIXTURE_NAMES),
            "aggregate requires all eight prepared original fixture sources")
    for fixture_name, case in prepared_cases.items():
        require(type(case) is dict and type(case.get("runs")) is list and
                len(case["runs"]) == FIXTURE_RUN_COUNTS[fixture_name],
                "aggregate prepared original fixture invocation inventory differs")
        expected_vectors = set()
        for parameters in case["runs"]:
            require(type(parameters) is dict and set(parameters) == {"arguments", "fixture_paths"},
                    "aggregate prepared fixture invocation has unknown fields")
            validate_fixture_arguments(fixture_name, parameters["arguments"], parameters["fixture_paths"])
            vector = (tuple(parameters["arguments"]), tuple(parameters["fixture_paths"]))
            require(vector not in expected_vectors, "prepared original fixture invocation is duplicated")
            expected_vectors.add(vector)
    require(sum(len(case["runs"]) for case in prepared_cases.values()) == 135,
            "aggregate requires all135 distinct per-source prepared invocation vectors")
    require(type(preparation.get("source_inputs")) is dict and
            "test/0073.ros-tests/support/source/ros_aarch64/prepare_original_fixtures.py" in preparation["source_inputs"],
            "aggregate fixture producer source closure missing")
    for relative, facts in preparation["source_inputs"].items():
        require(checked(relative) == facts, "aggregate prepared fixture source input changed")
    require(type(preparation.get("input_pins")) is list and preparation["input_pins"] and
            pins_preserved(preparation["input_pins"]), "aggregate prepared fixture/log/tool bytes changed")
    inputs.extend(preparation["input_pins"])
    probe_spec = importlib.util.spec_from_file_location("original_aggregate_cgroup", source/"test/0073.ros-tests/support/source/ros_cgroup/probe.py")
    probe = importlib.util.module_from_spec(probe_spec); probe_spec.loader.exec_module(probe)
    baseline = probe.probe()
    require(baseline.get("status") == "PASS" and baseline["effective_memory_max"] == EXPECTED_MEMORY_MAX,
            "current original48 cgroup is required to qualify completed chunks")
    leaf = Path(baseline["cgroup_ancestors"][0]["path"]); incarnation = cgroup_incarnation(leaf)
    require((leaf/"memory.swap.max").read_text().strip() == "0", "aggregate current64 must have zero swap")
    for path in args.aggregate_input:
        value, pin = read_json_binding(path, limit=64*1024**2); inputs.append(pin)
        require(value.get("kind") == "original-native-regressions" and value.get("mode") == "chunk"
                and value.get("status") == "PASS" and value.get("source") == str(source)
                and value.get("source_manifest_sha256") == manifest_pin["sha256"]
                and value.get("configuration") == config_pin and value.get("sdk_manifest") == sdk_pin
                and value.get("cgroup_incarnation") == incarnation and value.get("same_cgroup_incarnation") is True
                and value.get("source_inputs_preserved") is True and value.get("sdk_inputs_preserved") is True
                and value.get("tool_inputs_preserved") is True, "completed chunk source/config/current-incarnation qualification differs")
        require(type(value.get("input_pins")) is list and pins_preserved(value["input_pins"]), "chunk input/tool identity changed")
        inputs.extend(value["input_pins"])
        qualify_compiler_loader(value, loader_selection, loader_before, inputs)
        require(value.get("fixture_preparation", {}).get("report") == preparation_pin and
                value["fixture_preparation"].get("cases") == prepared_cases,
                "chunk did not qualify this exact complete original fixture preparation")
        old_arch = value.get("architecture_evidence", {})
        architecture, arch_pin = read_json_binding(old_arch["path"], limit=128*1024**2); inputs.append(arch_pin)
        require(old_arch.get("sha256") == arch_pin["sha256"] and architecture.get("status") == "PASS"
                and architecture.get("same_cgroup_incarnation") is True
                and architecture.get("cgroup_incarnation_before") == incarnation
                and architecture.get("cgroup_incarnation_after") == incarnation
                and not architecture.get("remaining_owned_processes") and not architecture.get("fatal_errors"),
                "chunk actual ARM/current64/OOM/cleanup architecture evidence differs")
        require(type(value.get("suites")) is list and value["suites"] and type(value.get("commands")) is list,
                "chunk executed inventory and terminal commands missing")
        for command in value["commands"]:
            require(command.get("status") == "PASS" and type(command.get("returncode")) is int and
                    command["returncode"] == 0 and command.get("owned_cleanup", {}).get("status") == "PASS",
                    "chunk contains an unended/failed command or cleanup")
            argv = command.get("argv")
            require(type(argv) is list and argv and all(type(item) is str for item in argv),
                    "chunk actual command argv is missing")
            compiler = argv[:len(config["compiler"])] == config["compiler"]
            require(compiler or argv[:len(config["qemu_argv"])] == config["qemu_argv"],
                    "chunk command has neither exact compiler nor native QEMU prefix")
            expected_environment = {"environment_role": "HOST_COMPILER" if compiler else "NATIVE_TARGET",
                "host_compiler_ld_library_path": loader_selection["explicit_ld_library_path"] if compiler else None,
                "native_host_loader_environment_cleared": not compiler}
            require(all(type(command.get(key)) is type(expected_value) and command[key] == expected_value
                        for key, expected_value in expected_environment.items()),
                    "chunk compiler-only loader role/path differs or leaked into native environment")
            for field in ("stdout_pin", "stderr_pin"):
                old = command[field]; require(read_file_binding(old["path"]) == old, "chunk actual command log changed")
                inputs.append(old)
        expected_support = RUNTIME_SOURCES if any(item["original_source"] in RUNTIME_TESTS for item in value["suites"]) else ()
        require([item["original_source"] for item in value.get("runtime_support", [])] == list(expected_support),
                "completed chunk runtime-backed FP support object inventory differs")
        for support in value.get("runtime_support", []):
            dep = support["dependencies"]
            require(read_file_binding(dep["file"]["path"]) == dep["file"] and
                    require_arm(dep["object"]["path"]) == dep["object"], "runtime support actual-MD/object changed")
            inputs.extend([dep["file"], read_file_binding(dep["object"]["path"])])
            require(support["original_source"] in dep["project_inputs"], "runtime support compiler dependencies omit original source")
            for relative, facts in dep["project_inputs"].items(): require(checked(relative) == facts, "runtime support source input changed")
            for system_path, facts in dep["system_inputs"].items():
                require({"sha256": digest(system_path), "bytes": Path(system_path).stat().st_size} == facts,
                        "runtime support qualified compiler system input changed")
        shard = value["selection"].get("strict_shard")
        if shard is not None:
            require(type(shard) is list and len(shard) == 2 and all(type(item) is int for item in shard)
                    and 0 <= shard[0] < shard[1] <= 32, "aggregate strict shard selection differs")
        selected = selected_rows(inventory(source), value["selection"]["groups"], value["selection"]["strict_config"], shard)
        require([(row.get("suite"), row["original_source"]) for row in value["suites"]] ==
                [(row.get("suite"), row["original_source"]) for row in selected], "chunk selected original keys were omitted/added/reordered")
        for row in value["suites"]:
            if row["status"] == "EXCLUDED_BACKEND":
                require(row["original_source"] in BACKEND_EXCLUSIONS, "unknown original source exclusion")
                continue
            name = row.get("suite")
            require(name in expected and name not in accepted and row["status"] == "PASS" and
                    all(row[key] == expected[name][key] for key in ("original_source", "group", "combine", "delay")),
                    "aggregate duplicated/unknown/failed original configuration")
            receipt, receipt_pin = read_json_binding(row["row_receipt"]["path"], limit=64*1024**2)
            require(receipt_pin == row["row_receipt"] and all(receipt[key] == row[key] for key in
                    ("suite", "group", "original_source", "combine", "delay", "status", "artifact")),
                    "compact row does not match its retained complete original receipt")
            inputs.append(receipt_pin); row = receipt
            actual = require_arm(row["artifact"]["path"])
            require(actual == row["artifact"] and architecture["arm_targets"].get(name) == actual,
                    "completed chunk original ARM image changed")
            inputs.append(read_file_binding(actual["path"])); targets[name] = actual
            for dep_field in ("dependencies", "witness_dependencies"):
                dependency = row[dep_field]
                require(read_file_binding(dependency["file"]["path"]) == dependency["file"] and
                        require_arm(dependency["object"]["path"]) == dependency["object"], "chunk actual-MD/object identity changed")
                inputs.extend([dependency["file"], read_file_binding(dependency["object"]["path"])])
                for relative, facts in dependency["project_inputs"].items(): require(checked(relative) == facts, "chunk frozen compiler input changed")
                for generated in dependency["generated_inputs"].values():
                    require(read_file_binding(generated["path"]) == generated, "chunk generated compiler input changed"); inputs.append(generated)
                for system_path, facts in dependency["system_inputs"].items():
                    require({"sha256": digest(system_path), "bytes": Path(system_path).stat().st_size} == facts,
                            "chunk qualified system dependency changed")
            compiled = row["compiled_main"]
            require(read_file_binding(compiled["ast"]["path"]) == compiled["ast"], "chunk real compiled-main AST changed")
            require(main_ast(Path(compiled["ast"]["path"]), row["compile_time_contract"]) == compiled,
                    "chunk compiled-main stub/ABI proof differs")
            fixture_name = Path(row["original_source"]).stem
            parameters = (prepared_cases[fixture_name]["runs"] if fixture_name in FIXTURE_NAMES else
                          [{"arguments": [], "fixture_paths": []}])
            require(type(row.get("runs")) is list and len(row["runs"]) == len(parameters),
                    "chunk omitted or duplicated an original native invocation")
            compact_runs = []
            for invocation_index, (invocation, parameters_row) in enumerate(zip(row["runs"], parameters)):
                # Ordered identity is independent of a chunk's claimed PASS:
                # every real launch must use its original prepared argv once.
                # Ordinary sources have exactly the one empty-argument entry.
                require(invocation.get("argv") == [*config["qemu_argv"], actual["path"], *parameters_row["arguments"]]
                        and invocation.get("name") == name+"-run-"+str(invocation_index)
                        and invocation.get("cwd") == str(Path(actual["path"]).parent),
                        "chunk original invocation argv/order/owned working directory differs")
                require(type(invocation.get("returncode")) is int and invocation["returncode"] == 0 and
                        invocation.get("status") == "PASS" and invocation.get("live_arm_mapping_before_resume"),
                        "completed original main lacks actual successful mapped execution")
                text = Path(invocation["stdout_pin"]["path"]).read_bytes().decode("utf-8", errors="strict")
                marker = "PASS_ORIGINAL_REGRESSION case="+name+" return=0"
                require("\r" not in text and text.endswith("\n") and text.split("\n").count(marker) == 1,
                        "completed original main successful-return marker changed")
                proof = invocation["live_arm_mapping_before_resume"]
                birth = (invocation["pid"], proof["identity"]["starttime_ticks"])
                require(birth not in seen_invocation_births, "same actual original native birth was counted twice")
                seen_invocation_births.add(birth)
                matched = [sample for sample in architecture["actual_process_samples"] if
                    sample["pid"] == invocation["pid"] and sample["starttime_ticks"] == proof["identity"]["starttime_ticks"]
                    and name in sample["mapped_arm_targets"] and sample.get("live_cgroup_procs") is True and
                    sample["membership"] == architecture["cgroup_before"]["membership"] and
                    sample["execution_model"] == "QEMU user host process" and
                    sample["kernel_exe_elf"]["sha256"] == digest(config["qemu_argv"][0])]
                require(matched and proof["sample"] in matched and proof.get("cooperative_constructor_stop") is True,
                        "completed original main stopped birth/ARM/QEMU proof differs")
                samples.append(proof["sample"]); launches.append({"pid": invocation["pid"], "required": [name],
                    "starttime_ticks": proof["identity"]["starttime_ticks"], "chunk_report": pin})
                compact_runs.append({key: invocation[key] for key in ("pid", "returncode", "semantic_marker", "stdout_pin", "stderr_pin")})
            accepted[name] = {key: row[key] for key in ("suite", "group", "original_source", "combine", "delay", "status", "artifact")}
            accepted[name].update(runs=compact_runs, chunk_report=pin, row_receipt=receipt_pin)
        architectures.append(architecture); chunks.append({"report": pin, "architecture": arch_pin,
                                                           "selection": value["selection"], "commands": len(value["commands"])})
    require(set(accepted) == set(expected), "original aggregate is missing one or more of3073 required configuration keys")
    require(len(launches) == len(seen_invocation_births) == 3200,
            "complete original aggregate must contain exactly3200 actual distinct native invocations")
    earliest = min(architectures, key=lambda item: min((sample["capture_monotonic_ns"] for sample in item["actual_process_samples"]), default=0))
    final = probe.probe(); require(cgroup_incarnation(leaf) == incarnation and final.get("status") == "PASS",
                                 "current aggregate leaf/boot/budget changed")
    valid, deltas = cgroup_oom_deltas(earliest["cgroup_before"], final, True)
    require(valid, "aggregate complete same-incarnation interval has nonzero/unavailable ancestor OOM counters")
    architecture = {**earliest, "status": "PASS", "scope": "data-only qualification of previously executed exact original chunks; no target launched by aggregation",
        "cgroup_after": final, "cgroup_incarnation_after_before_probe": incarnation, "cgroup_incarnation_after": incarnation,
        "same_cgroup_incarnation": True, "cgroup_ancestor_event_deltas": deltas, "arm_targets": targets,
        "actual_process_samples": samples, "remaining_owned_processes": [], "fatal_errors": [], "commands": [],
        "component_receipts": chunks, "unavailable_samples": [], "aggregation_current_probe": baseline}
    architecture_dir = args.out.resolve()/"architecture"; architecture_dir.mkdir(mode=0o700)
    architecture_path = architecture_dir/"architecture-evidence.json"; atomic_json(architecture_path, architecture)
    require(architecture_path.stat().st_size <= LOG_LIMIT,
            "compact aggregate architecture exceeds the existing16-MiB qualification boundary")
    for relative in frozen: checked(relative)
    sdk_root = sdk_root_from_flags(config["compile_flags"])
    sdk_value, _ = read_json_binding(args.sdk_manifest, limit=64*1024**2); verify_sdk_inventory(sdk_root, sdk_value)
    resource_rows, _ = resource_inventory(args.compiler_resource_root.resolve(strict=True))
    for component in args.aggregate_input:
        value, _ = read_json_binding(component, limit=64*1024**2)
        require(value["source_compiler_closure"]["compiler_resource_snapshot"] == resource_rows,
                "aggregate Clang resource input closure differs")
    require(compiler_loader_selection(config) == loader_selection, "aggregate compiler loader selection changed")
    loader_after = compiler_loader_inventory(loader_selection)
    loader_after_path = args.out.resolve()/"host-compiler-loader-after.json"; atomic_json(loader_after_path, loader_after)
    report["host_compiler_loader"]["after"] = read_file_binding(loader_after_path)
    inputs.append(report["host_compiler_loader"]["after"])
    require(loader_after == loader_before, "aggregate complete compiler library closure changed")
    report["host_compiler_loader"]["inputs_preserved"] = True
    require(pins_preserved(inputs), "aggregate report/log/tool/artifact input identity changed")
    report.update(status="PASS", mode="aggregate", source_inputs_preserved=True, sdk_inputs_preserved=True,
        tool_inputs_preserved=True, same_cgroup_incarnation=True, cgroup_incarnation=incarnation,
        cgroup_ancestor_event_deltas=deltas, chunk_reports=chunks, suites=list(accepted.values()), launches=launches,
        actual_native_invocations=3200, prepared_fixture_invocations=135,
        ordered_original_invocation_inventory_verified=True, fixture_preparation_report=preparation_pin,
        excluded_backend_sources=[{"original_source": name, "status": "EXCLUDED_BACKEND", "reason": reason}
            for name, reason in BACKEND_EXCLUSIONS.items()],
        architecture_evidence={"path": str(architecture_path), "sha256": digest(architecture_path), "status": "PASS"})
    report["input_pins"] = list({pin["path"]: pin for pin in inputs}.values())
    for group in report["source_counts"]:
        rows = [row for row in report["suites"] if row["group"] == group]
        report["groups"][group] = {"status": "PASS", "inventory_sources": report["source_counts"][group],
            "required_configurations": len(rows), "passed_configurations": len(rows),
            "excluded_backend_sources": 2 if group == "uwvm-int" else 0,
            "actual_native_invocations": sum(len(row["runs"]) for row in rows),
            "launches": [launch for launch in launches if launch["required"][0] in {row["suite"] for row in rows}]}
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("source", "manifest", "config", "sdk-manifest", "compiler-resource-root", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--groups", nargs="+", choices=CHUNK_GROUPS, default=list(CHUNK_GROUPS),
                        help="Explicit chunk groups. Final aggregate must cover all original keys exactly once.")
    parser.add_argument("--strict-config", choices=STRICT_CONFIGS,
                        help="One original combine:delay configuration; use twelve independent strict chunks.")
    parser.add_argument("--strict-shard", type=strict_shard,
                        help="Explicit1-based INDEX/COUNT partition (COUNT<=32) of the241 strict source names.")
    parser.add_argument("--aggregate-input", type=Path, action="append", default=[],
                        help="Read-only qualification of completed chunk results.json; never reruns their actors.")
    args = parser.parse_args()
    require(sys.platform == "linux", "BLOCKED original regression actors require Linux")
    require(not args.source.is_symlink(), "source root symlink is not a frozen checkout")
    source = args.source.resolve(strict=True)
    require(Path(__file__).resolve() == source/"test/0073.ros-tests/support/source/ros_aarch64/run_original_regressions.py",
            "runner and helpers must execute from the selected complete frozen source")
    out = args.out.resolve(); out.mkdir(mode=0o700, parents=True, exist_ok=False)
    frozen, manifest_pin = read_json_binding(args.manifest, limit=64*1024**2)
    config, config_pin = read_json_binding(args.config)
    sdk_inventory, sdk_pin = read_json_binding(args.sdk_manifest, limit=64*1024**2)
    pins = [manifest_pin, config_pin, sdk_pin]
    report = {"format_version": 1, "kind": "original-native-regressions", "status": "RUNNING",
              "source": str(source), "source_manifest_sha256": manifest_pin["sha256"],
              "configuration": config_pin, "sdk_manifest": sdk_pin,
              "scope": "unchanged original Linux AArch64 CPP26 non-JIT interpreter/native WASIp1/local-imported tests; no ROS network/Guest/LLVM/sanitizer/cross-CPU claim",
              "compiler_parallelism": 1, "compiler_aggregate_rss_budget_bytes": RSS_LIMIT,
              "actor_aggregate_rss_budget_bytes": RSS_LIMIT, "compile_timeout_seconds": COMPILE_SECONDS,
              "run_timeout_seconds": RUN_SECONDS, "fixture_run_timeout_seconds": FIXTURE_RUN_SECONDS,
              "source_counts": {"wasip1-functions": 94,
                  "wasip1-hybrid": 11, "local-imported": 5, "uwvm-int": 314},
              "required_portable_sources": 422, "required_configurations": 3073,
              "strict_matrix": {"combine": list(COMBINE_MODES), "delay": list(DELAY_MODES),
                  "executable_sources": 241, "validator_sources": 5, "validator_configuration": ["heavy", "heavy"],
                  "abi_modes": "byref,tail-min,tail-sysv,tail-aapcs64", "matrix_level": "default"},
              "mode": "aggregate" if args.aggregate_input else "chunk",
              "selection": {"groups": args.groups, "strict_config": args.strict_config,
                            "strict_shard": list(args.strict_shard) if args.strict_shard is not None else None},
              "suites": [], "commands": [], "runtime_support": [], "groups": {}, "missing_fixtures": [], "launches": []}
    evidence = None; active = None; owner = None

    def save(): atomic_json(out/"results.json", report)

    def finish_compiler_loader():
        """Retain an after catalog even when an actor failed; never turn FAIL into PASS."""
        closure = report.get("host_compiler_loader")
        if closure is None or closure["after"] is not None:
            return
        require(compiler_loader_selection(config) == loader_selection, "compiler loader selection changed")
        after = compiler_loader_inventory(loader_selection)
        path = out/"host-compiler-loader-after.json"; atomic_json(path, after)
        closure["after"] = read_file_binding(path); pins.append(closure["after"])
        closure["inputs_preserved"] = after == loader_before
        require(closure["inputs_preserved"], "complete compiler library files/aliases/root changed")

    def checked(relative):
        require(type(relative) is str and relative in frozen and not Path(relative).is_absolute()
                and ".." not in Path(relative).parts, "compiler source absent from full frozen view")
        path = source/relative; expected = frozen[relative]
        require(path.is_file() and not path.is_symlink() and path.resolve().is_relative_to(source),
                "frozen source is not a contained regular file: " + relative)
        pin = read_file_binding(path)
        require(pin["sha256"] == (expected["sha256"] if isinstance(expected, dict) else expected) and
                (not isinstance(expected, dict) or pin["bytes"] == expected["bytes"]), "frozen source changed: " + relative)
        return {"sha256": pin["sha256"], "bytes": pin["bytes"]}

    def current():
        if not (cgroup_incarnation(evidence.leaf) == incarnation and
                (evidence.leaf/"memory.max").read_text().strip() == str(EXPECTED_MEMORY_MAX) and
                (evidence.leaf/"memory.swap.max").read_text().strip() == "0"):
            raise ResourceGuardError("current64 cgroup incarnation/limits changed")
        if int((evidence.leaf/"memory.current").read_text()) >= 60*1024**3:
            raise ResourceGuardError("original48 cgroup cannot retain its4-GiB reserve")
        if shutil.disk_usage(out).free < 1024**3:
            raise ResourceGuardError("retained original regression evidence requires1-GiB free output capacity; no evidence is erased")
        if owner is not None: owner.sample()

    def run(argv, label, work, seconds, target=None):
        nonlocal active, owner
        current(); owner = OwnedProcesses(evidence); began = time.monotonic()
        stdout, stderr = out/(label+".stdout"), out/(label+".stderr")
        row = {"name": label, "argv": list(map(str, argv)), "cwd": str(work), "status": "RUNNING",
               "timeout_seconds": seconds, "rss_limit_bytes": RSS_LIMIT,
               "stdout": str(stdout), "stderr": str(stderr)}
        command_environment, environment_record = compiler_command_environment(
            config, loader_selection, row["argv"], environment, target)
        row.update(environment_record)
        report["commands"].append(row); save()
        try:
            with stdout.open("xb") as output, stderr.open("xb") as error:
                active = subprocess.Popen(row["argv"], cwd=work, env=command_environment, stdin=subprocess.DEVNULL,
                    stdout=output, stderr=error, start_new_session=True)
            owner.acquire(active.pid, label, [active.pid]); evidence.start(active, label, row["argv"])
            row.update(pid=active.pid, birth=process_identity(active.pid),
                       membership=Path(f"/proc/{active.pid}/cgroup").read_text().splitlines())
            require(row["membership"] == evidence.before["membership"], "command left the inherited cgroup")
            save()
            if target is not None:
                deadline = began + min(20, seconds)
                while active.poll() is None and time.monotonic() < deadline:
                    current(); identity = process_identity(active.pid)
                    if identity["state"] in ("T", "t"):
                        evidence.observe(active.pid, label); evidence.check([target], label)
                        samples = [sample for sample in evidence.samples.values() if sample["name"] == label and
                            sample["pid"] == active.pid and sample["starttime_ticks"] == identity["starttime_ticks"] and
                            target in sample["mapped_arm_targets"] and sample["execution_model"] == "QEMU user host process"]
                        require(samples and process_identity(active.pid) == identity, "stopped target/QEMU/birth proof differs")
                        retained = next(value for value in owner.rows.values() if value["identity"]["pid"] == active.pid)
                        require(owner.check(retained), "stopped ARM target is not in the live owned cgroup")
                        row["live_arm_mapping_before_resume"] = {"identity": identity, "sample": samples[-1],
                            "target": target, "cooperative_constructor_stop": True}
                        owner.send(retained, signal.SIGCONT); save(); break
                    time.sleep(.002)
                require("live_arm_mapping_before_resume" in row, "original target exited/missed its pre-main barrier")
            peak = 0
            while active.poll() is None:
                current(); peak = max(peak, owner.rss(label), process_tree_rss(active.pid))
                if peak > RSS_LIMIT: raise ResourceGuardError("serial command exceeded2-GiB aggregate observed RSS")
                require(time.monotonic()-began <= seconds, "serial command exceeded its deadline")
                require(stdout.stat().st_size <= LOG_LIMIT and stderr.stat().st_size <= LOG_LIMIT,
                        "command exceeded bounded log capacity")
                time.sleep(.01)
            row.update(returncode=active.returncode, peak_observed_RSS=peak)
            require(active.returncode == 0, "original command returned a failure")
            if target is not None: evidence.check([target], label)
            row["status"] = "PASS"
        except BaseException as error:
            row.update(status="FAIL", error=str(error)); raise
        finally:
            if active is not None:
                row["returncode_before_cleanup"] = active.poll()
                row["owned_cleanup"] = owner.stop([active])
                row["returncode"] = active.poll()
                if row["owned_cleanup"]["status"] != "PASS": row["status"] = "FAIL"
            for key, path in (("stdout_pin", stdout), ("stderr_pin", stderr)):
                if path.exists(): row[key] = read_file_binding(path)
            row["seconds"] = round(time.monotonic()-began, 3)
            active = None; owner = None; save()
        require(row["status"] == "PASS", "owned command cleanup failed")
        return row

    def dependencies(path, object_path, original, generated):
        raw = path.read_bytes().decode("utf-8", errors="strict").replace("\\\n", " ")
        target, colon, remainder = raw.partition(":")
        require(colon and shlex.split(target) == [str(object_path)], "depfile object rule does not match its actual output")
        project, temporary, system = {}, {}, {}
        for token in shlex.split(remainder):
            item = Path(token).resolve(strict=True); name = str(item)
            if name in generated:
                require(read_file_binding(item) == generated[name], "generated compilation input changed")
                temporary[name] = generated[name]; continue
            if item.is_relative_to(source): project[str(item.relative_to(source))] = checked(str(item.relative_to(source))); continue
            require(name in system_inputs, "unqualified compiler input outside source/SDK/resource roots: " + name)
            actual = {"sha256": digest(item), "bytes": item.stat().st_size}
            require(actual == system_inputs[name], "qualified compiler system input changed")
            system[name] = actual
        require((original is None or original in project) and set(temporary) == set(generated),
                "full-MD dependencies omit original or explicit generated inputs")
        return {"file": read_file_binding(path), "object": require_arm(object_path),
                "project_inputs": project, "generated_inputs": temporary, "system_inputs": system,
                "scope": "actual full-MD depfile with exact object rule; frozen source+SDK+Clang resource inputs"}

    def fixture_report():
        selected = config.get("original_regression_fixtures_report")
        if selected is None: return {}
        require(type(selected) is str and Path(selected).is_absolute(), "fixture report path must be explicit absolute input")
        value, pin = read_json_binding(selected, limit=64*1024**2); pins.append(pin)
        require(value.get("kind") == "original-regression-fixtures" and value.get("status") == "PREPARED"
                and value.get("source") == str(source) and value.get("source_manifest_sha256") == manifest_pin["sha256"],
                "fixture preparation must match the same complete frozen source")
        require(value.get("source_preserved") is True and value.get("inputs_preserved") is True and
                value.get("process_cleanup", {}).get("status") == "PASS" and not value.get("cleanup_errors") and
                not value.get("remaining_owned_mounts"), "fixture preparation preservation/cleanup failed")
        fixture_architecture, fixture_arch_pin = read_json_binding(value["architecture_evidence"]["path"], limit=16*1024**2)
        require(fixture_arch_pin == value["architecture_evidence"] and fixture_architecture.get("status") == "PASS"
                and fixture_architecture.get("same_cgroup_incarnation") is True and
                fixture_architecture.get("cgroup_incarnation_before") == incarnation and
                fixture_architecture.get("cgroup_incarnation_after") == incarnation and
                not fixture_architecture.get("remaining_owned_processes") and not fixture_architecture.get("fatal_errors"),
                "fixture preparation must retain actual current64/OOM/owned-cleanup evidence")
        pins.append(fixture_arch_pin)
        inputs, fixture_pins = value.get("source_inputs"), value.get("input_pins")
        require(type(inputs) is dict and inputs and type(fixture_pins) is list and fixture_pins,
                "fixture preparation source/tool/generated/log closure is missing")
        for relative, expected in inputs.items(): require(checked(relative) == expected, "fixture source input changed")
        require("test/0073.ros-tests/support/source/ros_aarch64/prepare_original_fixtures.py" in inputs, "selected fixture producer is not source-bound")
        require(pins_preserved(fixture_pins), "fixture opened input/tool/generated/log identity changed")
        pins.extend(fixture_pins); by_path = {item["path"]: item for item in fixture_pins}
        require(all(item == by_path[item["path"]] for item in fixture_pins),
                "fixture input pins contain inconsistent duplicate opened paths")
        commands = value.get("commands")
        require(type(commands) is list and len(commands) == 230 and
                len({item.get("name") for item in commands}) == 230 and
                sum(item.get("role") == "fixture-assembly" for item in commands) == 115 and
                sum(item.get("role") == "fixture-validation" for item in commands) == 115 and
                sum(item.get("status") == "EXPECTED_REJECTION" for item in commands) == 5,
                "full115 assembly/115 validation/five expected rejection command evidence missing")
        for command in commands:
            expected = command.get("expected_returncode", 0)
            require(type(expected) is int and expected in (0, 1) and type(command.get("returncode")) is int
                    and command["returncode"] == expected and command.get("status") ==
                    ("PASS" if expected == 0 else "EXPECTED_REJECTION"), "fixture tool command failed unexpectedly")
            require(type(command.get("argv")) is list and command["argv"] and
                    str(Path(command["argv"][0]).resolve()) in by_path, "fixture tool executable is not pinned")
            for field in ("stdout_pin", "stderr_pin"):
                require(type(command.get(field)) is dict and command[field].get("path") in by_path and
                        command[field] == by_path[command[field]["path"]] and command[field]["bytes"] <= LOG_LIMIT,
                        "fixture raw command log is not an exact preserved input")
            if expected:
                require(command.get("role") == "fixture-validation" and "validate" in command["argv"]
                        and type(command.get("diagnostic_contains")) is list and command["diagnostic_contains"],
                        "expected fixture rejection lacks an explicit validation predicate")
                text = b"".join(Path(command[field]["path"]).read_bytes() for field in ("stdout_pin", "stderr_pin"))
                require(text and len(text) <= 2*LOG_LIMIT and all(type(item) is str and item and
                        item.encode("utf-8") in text for item in command["diagnostic_contains"]),
                        "expected validator rejection diagnostic is absent")
        cases = value.get("cases")
        require(type(cases) is dict and set(cases) == set(FIXTURE_NAMES), "exact eight original fixture harnesses required")
        for name, item in cases.items():
            relative = "test/0013.uwvm_int/wasm3/" + name + ".cc"
            require(relative in inputs and type(item) is dict and type(item.get("runs")) is list and
                    len(item["runs"]) == FIXTURE_RUN_COUNTS[name],
                    "fixture harness has no source-bound actual argv runs")
            for run_row in item["runs"]:
                require(type(run_row) is dict and set(run_row) == {"arguments", "fixture_paths"}, "unknown fixture run fields")
                validate_fixture_arguments(name, run_row["arguments"], run_row["fixture_paths"])
                for path in run_row["fixture_paths"]:
                    require(path in by_path and by_path[path]["bytes"] >= 8 and by_path[path]["bytes"] <= LOG_LIMIT,
                            "fixture actual Wasm bytes are not in the bounded immutable input closure")
                    with Path(path).open("rb") as stream: require(stream.read(8) == b"\0asm\1\0\0\0", "fixture is not a Core Wasm binary")
        products = value.get("fixtures")
        require(type(products) is list and len(products) == 115 and
                len({item["wasm"]["path"] for item in products}) == 115 and
                all(item["wasm"] == by_path.get(item["wasm"]["path"]) and item["wat"] == by_path.get(item["wat"]["path"])
                    for item in products), "full115 original binary/WAT product pins missing")
        selected_paths = {path for item in cases.values() for parameters in item["runs"] for path in parameters["fixture_paths"]}
        require(selected_paths == {item["wasm"]["path"] for item in products}, "fixture original argv omits/adds prepared Wasm products")
        report["fixture_preparation"] = {"report": pin, "source_inputs": inputs, "input_pins": fixture_pins,
                                         "commands": commands, "cases": cases}
        return cases

    def terminate(signum, frame):
        # The current command's finally block owns the single pidfd retirement.
        # Do not close that owner's descriptors twice from a signal callback.
        raise RuntimeError("runner interrupted by signal " + str(signum))

    signal.signal(signal.SIGTERM, terminate); signal.signal(signal.SIGINT, terminate)
    save()
    try:
        for relative in frozen: checked(relative)
        require(len(args.groups) == len(set(args.groups)), "duplicate chunk groups are not distinct coverage")
        require(args.strict_shard is None or "uwvm-int-strict" in args.groups,
                "a strict shard must select the strict group")
        require(args.strict_config is None or "uwvm-int-strict" in args.groups,
                "a strict configuration must select the strict group")
        for name in ("compiler", "qemu_argv", "compile_flags", "link_flags"):
            require(type(config.get(name)) is list and (bool(config[name]) or name.endswith("flags")) and
                    all(type(item) is str and item and "\x00" not in item for item in config[name]), "invalid explicit compiler configuration argv")
        require(not any(token.startswith(("-DUWVM_MODULE", "-DUWVM_RUNTIME_LLVM_JIT", "-DUWVM2TEST_RUNNER_USE_LLVM_JIT",
                "-DUWVM2TEST_STRICT_NO_INTERPRETER", "-DUWVM2TEST_SIMD_CROSS_ADAPTER", "-fno-exceptions"))
                for token in config["compile_flags"]), "compiler configuration disables required original coverage")
        require(not any(token.startswith(("-DUWVM_ENABLE_UWVM_INT_COMBINE_OPS", "-DUWVM_ENABLE_UWVM_INT_HEAVY_COMBINE_OPS",
                "-DUWVM_ENABLE_UWVM_INT_EXTRA_HEAVY_COMBINE_OPS", "-DUWVM_ENABLE_UWVM_INT_DELAY_LOCAL_SOFT",
                "-DUWVM_ENABLE_UWVM_INT_DELAY_LOCAL_HEAVY", "-ffast-math")) for token in config["compile_flags"]),
                "compiler configuration may not replace the exact original combine/delay/precise-FP selection")
        loader_selection = compiler_loader_selection(config)
        overrides = {key: value for key, value in config.get("environment", {}).items() if key != "LD_LIBRARY_PATH"}
        if args.aggregate_input:
            require(len(args.aggregate_input) == len({str(path.resolve()) for path in args.aggregate_input}),
                    "aggregate chunk report paths may not be duplicated")
            report = aggregate(args, source, frozen, config, manifest_pin, config_pin, sdk_pin, report, checked)
            save(); print(report["status"], out/"results.json", flush=True); return 0
        environment = {"PATH": "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
            "HOME": str(out), "TMPDIR": str(out), "LANG": "C.UTF-8", "LC_ALL": "C.UTF-8", "PYTHONDONTWRITEBYTECODE": "1",
            **overrides, "UWVM2TEST_ABI_MODES": report["strict_matrix"]["abi_modes"], "UWVM2TEST_MATRIX_LEVEL": "default"}
        probe_spec = importlib.util.spec_from_file_location("original_regression_cgroup", source/"test/0073.ros-tests/support/source/ros_cgroup/probe.py")
        probe = importlib.util.module_from_spec(probe_spec); probe_spec.loader.exec_module(probe)
        evidence = Evidence(out/"architecture", probe.probe, interval=.01); incarnation = cgroup_incarnation(evidence.leaf)
        require((evidence.leaf/"memory.swap.max").read_text().strip() == "0", "existing64GiB cgroup must retain zero swap")
        report["cgroup_incarnation"] = incarnation; report["emulator"] = evidence.emulator(config["qemu_argv"][0])
        for executable in (config["compiler"][0], config["qemu_argv"][0]): pins.append(read_file_binding(executable))
        loader_before = compiler_loader_inventory(loader_selection)
        loader_before_path = out/"host-compiler-loader-before.json"; atomic_json(loader_before_path, loader_before)
        pins.extend(loader_before["files"].values())
        report["host_compiler_loader"] = {"selection": loader_selection,
            "before": read_file_binding(loader_before_path), "after": None, "inputs_preserved": False}
        pins.append(report["host_compiler_loader"]["before"]); save()
        builder = source/"test/0073.ros-tests/support/source/ros_aarch64/build_worker.py"; base, directories = worker_test_configuration(builder)
        sdk_root = sdk_root_from_flags(config["compile_flags"])
        system_inputs = verify_sdk_inventory(sdk_root, sdk_inventory)
        resource_root = args.compiler_resource_root.resolve(strict=True)
        resource_rows, resource_files = resource_inventory(resource_root); system_inputs.update(resource_files)
        flags = [*config["compile_flags"], *base, "-std=c++26", "-UNDEBUG", "-ffp-model=precise", "-DUWVM_TEST=2", "-I", str(source)]
        for directory in directories: flags += ["-I", str(source/directory)]
        report["source_compiler_closure"] = {"worker_builder": read_file_binding(builder), "base_flags": base,
            "include_directories": directories, "sdk_root": str(sdk_root), "sdk_manifest": sdk_pin,
            "compiler_resource_root": str(resource_root), "compiler_resource_snapshot": resource_rows,
            "compile_flags": flags, "link_flags": config["link_flags"], "dependency_mode": "-MD with exact -MF/object rule"}
        report["suites"] = selected_rows(inventory(source), args.groups, args.strict_config, args.strict_shard); fixtures = fixture_report()
        report["selected_required_configurations"] = sum(row["status"] != "EXCLUDED_BACKEND" for row in report["suites"])
        report["complete_inventory_covered_by_this_chunk"] = report["selected_required_configurations"] == 3073
        report["missing_fixtures"] = sorted(set(FIXTURE_NAMES)-set(fixtures))
        runtime_objects = []
        support = out/"runtime-support"; support.mkdir(mode=0o700)
        support_sources = RUNTIME_SOURCES if any(row["original_source"] in RUNTIME_TESTS for row in report["suites"]) else ()
        for index, relative in enumerate(support_sources):
            object_path, dep = support/(str(index)+".o"), support/(str(index)+".d")
            command = [*config["compiler"], *flags, "-MD", "-MF", str(dep), "-c", str(source/relative), "-o", str(object_path)]
            record = run(command, "runtime-support-"+str(index), support, COMPILE_SECONDS)
            report["runtime_support"].append({"original_source": relative, "command": record,
                "dependencies": dependencies(dep, object_path, relative, {})})
            runtime_objects.append(str(object_path))
        for index, row in enumerate(report["suites"]):
            if row["status"] == "EXCLUDED_BACKEND": continue
            relative, name = row["original_source"], row["suite"]
            if row["fixture_required"] and Path(relative).stem not in fixtures:
                row.update(status="REQUIRED_FIXTURE", reason="original nonempty native argv/Wasm preparation report missing")
                save(); continue
            try:
                current(); work = out/("case-"+str(index)); work.mkdir(mode=0o700)
                text = (source/relative).read_text(encoding="utf-8-sig")
                if row["compile_time_contract"]:
                    row["original_static_assertions"] = len(re.findall(r"\bstatic_assert\s*\(", text))
                    require(row["original_static_assertions"] >= 20, "intentional concept compile-time assertions missing")
                guard = work/"evidence-guard.hpp"
                guard.write_text(guard_header(row["group"], "_wasm64" in relative or "/hybrid/" in relative,
                    Path(relative).name.startswith("sock_"), "/strict/" in relative), encoding="utf-8")
                guard_pin = read_file_binding(guard); original_flags = [*flags, *mode_flags(row["combine"], row["delay"]), "-include", str(guard)]
                ast_path = out/(name+"-main-ast.stdout")
                row["main_ast_command"] = run([*config["compiler"], *original_flags, "-fsyntax-only",
                    "-Xclang", "-ast-dump=json", "-Xclang", "-ast-dump-filter=main", str(source/relative)],
                    name+"-main-ast", work, COMPILE_SECONDS)
                row["compiled_main"] = main_ast(ast_path, row["compile_time_contract"])
                original_object, dep = work/"original.o", work/"original.d"
                row["build"] = run([*config["compiler"], *original_flags, "-MD", "-MF", str(dep), "-c",
                    str(source/relative), "-o", str(original_object)], name+"-build", work, COMPILE_SECONDS)
                row["dependencies"] = dependencies(dep, original_object, relative, {str(guard): guard_pin})
                witness = work/"return-witness.cc"; witness.write_text(return_witness(name,
                    row["compiled_main"]["parameter_count"] == 2), encoding="utf-8")
                witness_pin = read_file_binding(witness); witness_object, witness_dep = work/"witness.o", work/"witness.d"
                row["witness_build"] = run([*config["compiler"], *flags, "-MD", "-MF", str(witness_dep), "-c",
                    str(witness), "-o", str(witness_object)], name+"-witness-build", work, COMPILE_SECONDS)
                row["witness_dependencies"] = dependencies(witness_dep, witness_object, None, {str(witness): witness_pin})
                binary = work/"original-test"
                objects = [str(original_object), str(witness_object), *(runtime_objects if relative in RUNTIME_TESTS else [])]
                row["link"] = run([*config["compiler"], *config["link_flags"], "-pthread", *objects,
                    "-Wl,--wrap=main", "-ldl", "-o", str(binary)], name+"-link", work, COMPILE_SECONDS)
                row["artifact"] = evidence.target(name, binary)
                arguments = fixtures[Path(relative).stem]["runs"] if row["fixture_required"] else [{"arguments": [], "fixture_paths": []}]
                row["runs"] = []
                for run_index, parameters in enumerate(arguments):
                    label = name+"-run-"+str(run_index)
                    # This unchanged diagnostic checks336 million-iteration
                    # relocated-stream samples. Give only it a finite hour;
                    # preserve every original iteration, the2-GiB RSS bound,
                    # target/birth/current64 proof, and all normal180s limits.
                    deadline = FIXTURE_RUN_SECONDS.get(Path(relative).stem, RUN_SECONDS)
                    record = run([*config["qemu_argv"], str(binary), *parameters["arguments"]], label, work, deadline, name)
                    log = Path(record["stdout"]).read_bytes().decode("utf-8", errors="strict")
                    marker = "PASS_ORIGINAL_REGRESSION case=" + name + " return=0"
                    require("\r" not in log and log.endswith("\n") and log.split("\n").count(marker) == 1,
                            "original real main did not return zero with its exact witness marker")
                    record["semantic_marker"] = marker; row["runs"].append(record)
                    report["launches"].append({"pid": record["pid"], "required": [name],
                        "starttime_ticks": record["live_arm_mapping_before_resume"]["identity"]["starttime_ticks"]})
                row["status"] = "PASS"
                row_receipt = work/"row-receipt.json"; atomic_json(row_receipt, row)
                compact = {key: row[key] for key in ("suite", "group", "original_source", "combine", "delay", "status", "artifact")}
                compact["runs"] = [{key: invocation[key] for key in ("pid", "returncode", "semantic_marker", "stdout_pin", "stderr_pin")}
                                   for invocation in row["runs"]]
                compact["row_receipt"] = read_file_binding(row_receipt)
                report["suites"][index] = compact
            except ResourceGuardError:
                row.update(status="FAIL", error=traceback.format_exc()); save(); raise
            except Exception:
                row.update(status="FAIL", error=traceback.format_exc())
            save(); print(row["status"], name, flush=True)
        for relative in frozen: checked(relative)
        require(pins_preserved(pins), "configuration/compiler/QEMU/fixture inputs changed")
        verify_sdk_inventory(sdk_root, sdk_inventory)
        require(resource_inventory(resource_root)[0] == resource_rows, "Clang resource closure changed")
        finish_compiler_loader()
        current(); report.update(source_inputs_preserved=True, sdk_inputs_preserved=True, tool_inputs_preserved=True)
        required = [row for row in report["suites"] if row["status"] != "EXCLUDED_BACKEND"]
        for group in report["source_counts"]:
            rows = [row for row in report["suites"] if row["group"] == group]
            selected = [row for row in rows if row["status"] != "EXCLUDED_BACKEND"]
            report["groups"][group] = {"status": "PASS" if all(row["status"] == "PASS" for row in selected) else "FAIL",
                "inventory_sources": report["source_counts"][group], "excluded_backend_sources": len(rows)-len(selected),
                "required_configurations": len(selected), "passed_configurations": sum(row["status"] == "PASS" for row in selected),
                "actual_native_invocations": sum(len(row.get("runs", [])) for row in selected),
                "launches": [launch for launch in report["launches"] if launch["required"][0] in {row["suite"] for row in selected}]}
        report["input_pins"] = pins
        report["status"] = "PASS" if required and all(row["status"] == "PASS" for row in required) else (
            "INCOMPLETE" if report["missing_fixtures"] and not any(row["status"] == "FAIL" for row in required) else "FAIL")
    except BaseException:
        report.update(status="FAIL", error=traceback.format_exc())
    finally:
        if owner is not None and active is not None: report["final_owned_cleanup"] = owner.stop([active])
        if not args.aggregate_input:
            try:
                finish_compiler_loader()
            except BaseException:
                report.update(status="FAIL", host_compiler_loader_finalization_error=traceback.format_exc())
            report["input_pins"] = pins
        if evidence is not None:
            facts = evidence.finish("PASS" if report["status"] == "PASS" else "FAIL", {
                "scope": report["scope"], "driver_sha256": digest(__file__)})
            report["architecture_evidence"] = {"path": str(evidence.out/"architecture-evidence.json"),
                "sha256": digest(evidence.out/"architecture-evidence.json"), "status": facts["status"]}
            report["same_cgroup_incarnation"] = facts["same_cgroup_incarnation"]
            report["cgroup_ancestor_event_deltas"] = facts["cgroup_ancestor_event_deltas"]
            if report["status"] == "PASS" and facts["status"] != "PASS": report["status"] = "FAIL"
        save()
    print(report["status"], out/"results.json", flush=True)
    return 0 if report["status"] == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
