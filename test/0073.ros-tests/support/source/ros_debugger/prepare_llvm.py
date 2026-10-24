#!/usr/bin/env python3
"""Prepare the current patched static LLVM package for real ROS debugger builds.

No installed LLVM library, old object, cache directory, network fetch, or
metadata-only executable can substitute. This producer compiles the genuine
dependency target in a fresh output. Root alone invokes it after source review.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import select
import shlex
import stat
import sys
import time
import types

sys.dont_write_bytecode = True


def need(ok, message):
    if not ok:
        raise ValueError(message)


EXTRA_BACKENDS = ("AArch64", "ARM", "RISCV", "LoongArch", "PowerPC", "SystemZ")


def backend_targets(config):
    """Explicit host-built code generators, never a foreign Runtime SDK claim.

    X86 remains the required native backend of this host-only preparer. Version1
    keeps its original closed schema/recipe. Version2 selects finite additional
    backend archives rather than accepting LLVM's ambient/all target expansion.
    """
    version = config.get("format_version")
    need(type(version) is int and version in (1, 2), "unsupported preparation configuration")
    if version == 1:
        need("additional_llvm_targets" not in config, "version1 has no additional backend selection")
        return ["X86"]
    selected = config.get("additional_llvm_targets")
    need(type(selected) is list and 0 < len(selected) <= len(EXTRA_BACKENDS) and
         all(type(value) is str and value in EXTRA_BACKENDS for value in selected) and
         len(selected) == len(set(selected)), "explicit unique additional LLVM backend list required")
    return ["X86", *selected]


def additional_backend_evidence(builder, common, build, target, backends, products, guard):
    """Follow real file-API target edges and verify every selected built archive.

    These are native ELF62 libraries implementing other code generators. They
    are kept outside the strict native consumer's response/certificate archive
    list. Nothing here turns them into executable foreign-host LLVM libraries.
    """
    reply = build / ".cmake/api/v1/reply"
    guard()
    indexes = sorted(reply.glob("index-*.json"));need(indexes, "actual backend file-API index missing")
    def document(path):
        guard();value, pin = common.document(path);products[pin["path"]] = pin;guard();return value
    index = document(indexes[-1]);entry = index.get("reply", {}).get("codemodel-v2")
    need(type(entry) is dict and Path(entry["jsonFile"]).name == entry["jsonFile"], "closed backend model filename required")
    model = document(reply / entry["jsonFile"])
    need(len(model["configurations"]) == 1 and model["configurations"][0]["name"] == "Release", "one actual backend Release model required")
    rows = model["configurations"][0]["targets"];by_id = {};by_name = {}
    for row in rows:
        need(row["id"] not in by_id and row["name"] not in by_name and Path(row["jsonFile"]).name == row["jsonFile"],
             "unambiguous backend model target required")
        by_id[row["id"]] = row;by_name[row["name"]] = row
    wanted = ["LLVM"+backend+suffix for backend in backends for suffix in ("CodeGen", "AsmParser", "Disassembler")]
    need(all(name in by_name for name in wanted), "selected backend CodeGen/parser/disassembler target absent from actual SDK")
    pending = [by_name[name]["id"] for name in wanted];seen = set();archives = [];executables = [];targets = []
    while pending:
        guard()
        key = pending.pop()
        if key in seen:continue
        need(key in by_id and len(seen) < 4096, "bounded complete actual backend target dependency graph required")
        seen.add(key);row = by_id[key];detail = document(reply / row["jsonFile"])
        need(detail["id"] == key and detail["name"] == row["name"] and detail["type"] in
             ("STATIC_LIBRARY", "EXECUTABLE", "UTILITY", "OBJECT_LIBRARY"), "unexpected actual backend target kind")
        pending.extend(item["id"] for item in detail.get("dependencies", []))
        artifacts = []
        for item in detail.get("artifacts", []):
            path = Path(item["path"]);path = (path if path.is_absolute() else build / path).resolve(strict=True)
            need(path.is_relative_to(build), "backend artifact escapes actual fresh build")
            if detail["type"] == "STATIC_LIBRARY":
                pin, count = builder.archive(common, path, target)
                need(count > 0, "empty selected backend archive")
                archives.append({"target": row["name"], "input": pin, "members": count})
            elif detail["type"] in ("EXECUTABLE", "OBJECT_LIBRARY"):
                pin = builder.elf(common, path, target, (2, 3) if detail["type"] == "EXECUTABLE" else (1,))
                if detail["type"] == "EXECUTABLE":executables.append({"target": row["name"], "input": pin})
            else:
                pin = common.opened(path)
            products[pin["path"]] = pin;artifacts.append(pin)
            guard()
        if detail["type"] in ("STATIC_LIBRARY", "EXECUTABLE"):
            need(len(artifacts) == 1, "one actual static backend/tool artifact required")
        targets.append({"id": key, "name": row["name"], "type": detail["type"], "artifacts": artifacts})
    need(set(wanted) <= {row["target"] for row in archives}, "every explicitly selected backend archive must have actual member proof")
    definitions = {}
    for name, macro in (("Targets.def", "LLVM_TARGET"), ("AsmParsers.def", "LLVM_ASM_PARSER"),
                        ("Disassemblers.def", "LLVM_DISASSEMBLER")):
        path = build / "include/llvm/Config" / name;raw, pin = common.body_pin(path)
        actual = builder.re.findall(rb'^'+macro.encode()+rb'\(([A-Za-z0-9]+)\)$', raw, builder.re.M)
        values = [value.decode("ascii") for value in actual]
        need(len(values) == len(set(values)) and set(values) == {"X86", *backends},
             "actual generated backend target/parser/disassembler inventory differs")
        products[pin["path"]] = pin;definitions[name] = pin
    cache, _ = common.body_pin(build / "CMakeCache.txt")
    lines = [line.split(b"=", 1)[1].decode() for line in cache.splitlines() if line.startswith(b"LLVM_TARGETS_TO_BUILD:STRING=")]
    need(lines == [";".join(["X86", *backends])], "actual CMake backend selection differs from explicit list")
    guard()
    return {"selected_backends": list(backends), "native_host_elf": {key:target[key] for key in ("machine", "bits", "endian")},
            "explicit_targets": wanted, "targets": sorted(targets, key=lambda row:row["id"]),
            "archives": archives, "host_tools": executables, "generated_definitions": definitions,
            "foreign_host_SDK_qualified": False, "Runtime_or_Guest_executed": False}


def bootstrap(source, manifest):
    """Open and MF-admit the producer/helper source before executing imports."""
    def body(path, maximum):
        fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
        try:
            first = os.fstat(fd)
            need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= maximum, "bounded regular bootstrap input required")
            parts, count = [], 0
            while True:
                piece = os.read(fd, min(65536, maximum + 1 - count))
                if not piece:
                    break
                parts.append(piece); count += len(piece)
                need(count <= maximum, "bootstrap input exceeded bound")
            last = os.fstat(fd); current = Path(path).stat(follow_symlinks=False)
            need(count == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(current, key)
                 for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")), "bootstrap input changed")
            return b"".join(parts)
        finally:
            os.close(fd)
    def pairs(values):
        result = {}
        for key, value in values:
            need(key not in result, "duplicate frozen manifest key")
            result[key] = value
        return result
    entries = json.loads(body(manifest, 16 * 1024**2), object_pairs_hook=pairs)
    candidate = source / "test/0073.ros-tests/support/source/ros_debugger/build_runtime.py"
    loaded = body(candidate, 256 * 1024)
    for name, data in (("test/0073.ros-tests/support/source/ros_debugger/build_runtime.py", loaded),
                       ("test/0073.ros-tests/support/source/ros_debugger/prepare_llvm.py", body(Path(__file__).resolve(), 256 * 1024))):
        entry = entries.get(name)
        need(type(entry) is dict and entry.get("sha256") == hashlib.sha256(data).hexdigest() and
             entry.get("bytes") == len(data), "preparation implementation differs from frozen manifest")
    module = types.ModuleType("uwvm_debugger_frozen_runtime_builder")
    module.__file__ = str(candidate)
    exec(compile(loaded, str(candidate), "exec"), module.__dict__)
    common, helper_sha = module.load_common(source, manifest)
    return module, common, helper_sha


def source_sdk(builder, common, source, sdk):
    """Verify all real upstream bodies without retaining their contents."""
    lock = source / "third-parties/llvm/sources.sha256"
    frozen = common.opened(lock)
    need(common.opened(sdk / "sources.sha256")["sha256"] == frozen["sha256"], "full LLVM SDK lock differs")
    names, size = set(), 0
    with lock.open(encoding="utf-8") as stream:
        for line in stream:
            digest, separator, relative = line.rstrip("\n").partition("  ")
            need(separator and len(digest) == 64 and all(char in "0123456789abcdef" for char in digest), "invalid source lock")
            item = Path(relative)
            need(not item.is_absolute() and ".." not in item.parts and relative not in names, "unsafe source-lock entry")
            names.add(relative)
            physical = (sdk / item).resolve(strict=True)
            need(builder.inside(physical, [sdk]), "LLVM source input escapes selected SDK")
            pin = common.opened(physical)
            need(pin["sha256"] == digest, "patched LLVM source mismatch: " + relative)
            size += pin["bytes"]
    actual = {str(item.relative_to(sdk)) for subtree in ("cmake", "libc", "llvm", "third-party")
              for item in (sdk / subtree).rglob("*") if item.is_file()}
    need(names and actual <= names, "unrecorded upstream source SDK input")
    return {"lock": frozen, "files": len(names), "bytes": size, "root": str(sdk)}


def response_tokens(common, build):
    """Use the same actual file-API link order as bundled_llvm.lua/utility.lua."""
    reply = build / ".cmake/api/v1/reply"
    indexes = sorted(reply.glob("index-*.json"))
    need(indexes, "CMake did not publish its file-API reply")
    index, _ = common.document(indexes[-1])
    entry = index.get("reply", {}).get("codemodel-v2")
    need(type(entry) is dict and Path(entry["jsonFile"]).name == entry["jsonFile"], "closed codemodel reply required")
    model, _ = common.document(reply / entry["jsonFile"])
    need(len(model["configurations"]) == 1 and model["configurations"][0]["name"] == "Release", "single actual Release model required")
    rows = [item for item in model["configurations"][0]["targets"] if item["name"] == "uwvm_ros_llvm_contract"]
    need(len(rows) == 1 and Path(rows[0]["jsonFile"]).name == rows[0]["jsonFile"], "actual consumer target required")
    target, _ = common.document(reply / rows[0]["jsonFile"])
    need(target.get("link", {}).get("language") == "CXX", "actual consumer CXX link contract missing")
    result = []
    for item in target["link"]["commandFragments"]:
        if item.get("role") == "flags" and "backtrace" not in item:
            continue
        for token in shlex.split(item["fragment"]):
            need(not token.startswith(("@", "-lLLVM", "-l:libLLVM")), "nested/search LLVM substitution refused")
            if not token.startswith("-") and (token.endswith((".a", ".so", ".lib")) or ".so." in token):
                value = Path(token)
                token = str((value if value.is_absolute() else build / value).resolve(strict=True))
            result.append(token)
    need(result, "empty actual static consumer link contract")
    return result


def full_dependencies(builder, common, build, source, roots, raw_log, output, remember, products):
    """Bind retained depfiles to the original verbose Ninja compiler invocations.

    CMake's compile_commands database may omit implicit -MD/-MF options. The
    original verbose log is therefore the recipe authority. Relative Ninja
    target/source spellings are interpreted in its recorded build directory.
    """
    recipes = {}
    log_pin = common.opened(raw_log)
    with Path(raw_log).open(encoding="utf-8") as stream:
        for line in stream:
            match = builder.re.match(r"^\[[0-9]+/[0-9]+\] (.+)\n?$", line)
            if not match:
                continue
            argv = shlex.split(match[1]); cwd = build
            if len(argv) >= 3 and argv[0] == "cd" and argv[2] == "&&":
                cwd = Path(argv[1]).resolve(strict=True); argv = argv[3:]
            if "-c" not in argv or "-MD" not in argv or "-MF" not in argv or "-MT" not in argv or "-o" not in argv:
                continue
            need(not any(token in ("&&", ";", "||", "|") for token in argv), "compiler recipe contains an unqualified shell chain")
            def selected(flag):
                need(argv.count(flag) == 1 and argv.index(flag) + 1 < len(argv), "unique original compiler option required")
                item = Path(argv[argv.index(flag) + 1])
                return (item if item.is_absolute() else cwd / item).resolve(strict=True)
            dep = selected("-MF"); obj = selected("-o"); unit = selected("-c")
            need(selected("-MT") == obj, "original compiler -MT differs from its actual object")
            need(builder.inside(dep, [build]) and builder.inside(obj, [build]), "compiler outputs escape owned build")
            need(str(dep) not in recipes, "duplicate original dependency-output recipe")
            recipes[str(dep)] = {"argv": argv, "cwd": str(cwd), "object": obj, "source": unit,
                                 "raw_log": log_pin}
    need(common.opened(raw_log) == log_pin, "original verbose Ninja log changed")
    count = 0
    with output.open("x", encoding="utf-8") as stream:
        for name, recipe in sorted(recipes.items()):
            dep = Path(name)
            data, dep_pin = common.body_pin(dep)
            rule = data.decode().replace("\\\n", "")
            need(":" in rule, "original dependency rule missing")
            target, tail = rule.split(":", 1)
            targets = shlex.split(target)
            need(len(targets) == 1, "one original full-MD output rule required")
            target_path = Path(targets[0]); cwd = Path(recipe["cwd"])
            need((target_path if target_path.is_absolute() else cwd / target_path).resolve(strict=True) == recipe["object"],
                 "original -MD target differs from exact actual object")
            dependencies = []
            for name in dict.fromkeys(shlex.split(tail)):
                item = Path(name); item = (item if item.is_absolute() else cwd / item).resolve(strict=True)
                need(builder.inside(item, [source, build] + roots), "full-MD dependency escapes all qualified roots")
                dependencies.append(remember(item)["path"])
            need(str(recipe["source"]) in dependencies, "full-MD rule omits its actual source")
            object_pin = builder.elf(common, recipe["object"], {"machine":62,"bits":64,"endian":"little"}, (1,))
            products[dep_pin["path"]] = dep_pin; products[object_pin["path"]] = object_pin
            stream.write(json.dumps({"argv": recipe["argv"], "cwd": recipe["cwd"], "raw_log": log_pin,
                "source": remember(recipe["source"]), "dependency_file": dep_pin,
                "object": object_pin, "dependencies": dependencies}, sort_keys=True) + "\n")
            count += 1
        stream.flush(); os.fsync(stream.fileno())
    need(count and count == len(recipes), "every actual full-MD compiler recipe must retain its exact dependency output")
    index_pin = common.opened(output); products[index_pin["path"]] = index_pin
    return {"records": count, "catalog": index_pin, "raw_verbose_log": log_pin}


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--config", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    source = args.source.resolve(strict=True); manifest = args.manifest.resolve(strict=True)
    need(sys.platform == "linux" and platform.machine().lower() in ("x86_64", "amd64") and
         Path(__file__).resolve() == source / "test/0073.ros-tests/support/source/ros_debugger/prepare_llvm.py", "frozen native Linux x86_64 preparer required")
    builder, common, helper_sha = bootstrap(source, manifest)
    common.install_interrupts()
    config_path = args.config.resolve(strict=True)
    config, config_pin = common.document(config_path)
    selected_backends = backend_targets(config)
    config_keys = ("format_version", "source_root", "tools", "target", "stdlib", "cflags", "cxxflags",
                   "link_flags", "sysroot", "input_roots", "environment", "cgroup", "build_seconds")
    builder.closed(config, config_keys + (("additional_llvm_targets",) if config["format_version"] == 2 else ()), "LLVM preparation config")
    builder.closed(config["tools"], ("cc", "cxx", "cmake", "ninja", "ar", "ranlib", "linker", "python"), "explicit tools")
    tools = {}
    for name, value in config["tools"].items():
        physical = builder.path(value)
        # Preserve llvm-ranlib/ld.lld/clang++ multicall invocation basenames.
        tools[name] = str(Path(value).parent.resolve(strict=True) / Path(value).name)
        need(os.access(physical, os.X_OK), "tool is not executable")
    roots = [builder.path(item, True) for item in config["input_roots"]]
    need(roots and len(roots) == len(set(roots)), "unique complete existing input roots required")
    need(all(builder.inside(builder.path(item), roots) for item in tools.values()), "tool outside complete selected roots")
    sdk = builder.path(config["source_root"], True)
    need(builder.inside(sdk, roots), "complete patched source SDK root must be inventoried")
    target = config["target"]
    builder.closed(target, ("triple", "machine", "bits", "endian", "host_machine"), "target")
    need(target["host_machine"] == platform.machine().lower() and
         (target["machine"], target["bits"], target["endian"]) == (62, 64, "little") and
         type(target["triple"]) is str and target["triple"].startswith("x86_64-") and
         all(char.isalnum() or char in "_.+-" for char in target["triple"]), "native X86 target/triple required")
    for name in ("cflags", "cxxflags", "link_flags"):
        builder.strings(config[name], name); builder.safe_fp(config[name])
        need(not any(item.startswith(("@", "-o", "-DUWVM", "-UUWVM", "--target", "--sysroot", "-fuse-ld")) or
                     item in ("-D", "-U", "-c", "-E", "-S", "-fsyntax-only") for item in config[name]), "owned compilation settings cannot be overridden")
    builder.no_extra_llvm(config["link_flags"], "LLVM preparation generic link_flags")
    need(config["stdlib"] in ("libc++", "libstdc++") and all(
        [item for item in config[name] if item.startswith("-stdlib=")] == ["-stdlib=" + config["stdlib"]]
        for name in ("cxxflags", "link_flags")), "one explicit matching standard-library provider required")
    environment = config["environment"]
    need(type(environment) is dict and set(environment) <= {"PATH", "LD_LIBRARY_PATH", "LANG", "LC_ALL"} and "PATH" in environment,
         "closed explicit compiler environment required")
    for key, value in environment.items():
        need(type(value) is str and "\0" not in value, "invalid explicit environment")
        if key in ("PATH", "LD_LIBRARY_PATH"):
            need(value and all(part and builder.inside(builder.path(part, True), roots) for part in value.split(":")), "nonempty inventoried environment directories required")
    if config["sysroot"] is not None:
        need(builder.inside(builder.path(config["sysroot"], True), roots), "sysroot outside selected complete roots")
    builder.closed(config["cgroup"], ("memory_max", "memory_swap_max", "expected_leaf", "expected_device",
                                     "expected_inode", "expected_boot_id", "expected_cpus"), "original cgroup")
    need(config["cgroup"]["memory_max"] == builder.MEMORY_MAX and config["cgroup"]["memory_swap_max"] == 0,
         "current original64GiB/swap0 identity required")
    need(type(config["build_seconds"]) is int and 600 <= config["build_seconds"] <= 86400, "finite explicit overall LLVM build deadline required")
    out = args.out.resolve()
    need(not builder.inside(out, roots + [source]), "fresh dependency outputs must not mutate selected input roots")
    out.mkdir(mode=0o700, parents=True, exist_ok=False)
    build = out / "build"; build.mkdir(mode=0o700)
    query = build / ".cmake/api/v1/query/codemodel-v2"; query.parent.mkdir(parents=True); query.write_bytes(b"")
    state = {"kind": "ros-debugger-LLVM-static-preparation", "format_version": config["format_version"], "status": "FAIL",
             "source": str(source), "config": config_pin, "parallelism": 1, "no_reuse": True,
             "reused_products": 0, "runtime_integration_executed": False, "commands": [],
             "cleanup": {"status": "NOT_PERFORMED"}, "limits": dict(builder.LIMITS),
             "overall_build_seconds": config["build_seconds"], "noncoordinator_actor_seconds": 600}
    before = None; runner = None; snapshots = []; pins = {}; outputs = {}; selected_llvm = None
    ledger = (out / "actor-births.jsonl").open("x", encoding="utf-8")
    seen_births = set()

    def remember(item):
        item = Path(item)
        old = pins.get(str(item))
        if old is not None:
            info = item.stat(follow_symlinks=False)
            need(stat.S_ISREG(info.st_mode) and
                 (info.st_dev,info.st_ino,info.st_size,info.st_mtime_ns,info.st_ctime_ns) ==
                 (old["device"],old["inode"],old["bytes"],old["mtime_ns"],old["ctime_ns"]), "physical dependency changed")
            return old  # Finalization independently rehashes every unique body.
        pin = common.opened(item, builder.CONSUMER_FILE_MAX)
        pins[pin["path"]] = pin
        return pin

    def guard():
        current = common.cgroup_state(config["cgroup"])
        if before is not None:
            common.cgroup_preserved(before, current)
        if runner is not None:
            uptime = float(Path("/proc/uptime").read_text().split()[0])
            hz = os.sysconf("SC_CLK_TCK")
            for key, item in list(runner.owned.items()):
                if common.pidfd_terminal(item["fd"]):
                    continue
                is_ninja = (runner.commands and runner.commands[-1]["label"] == "llvm-static-build" and
                            runner.commands[-1].get("pid") == key[0])
                need(is_ninja or uptime - key[1] / hz <= 600, "actual owned compiler/generator/archive child exceeded600seconds")
        return current

    class RetiringRunner(common.SerialRunner):
        """Stream retired birth facts and close pidfds during a long serial build.

        Thousands of completed compilers must not exhaust RLIMIT_NOFILE. A
        pidfd is retired only after terminal readability, after discovering
        children; active roots/descendants keep the original ownership checks.
        """
        def acquire(self, pid, parent):
            super().acquire(pid, parent)
            for key, row in list(self.owned.items()):
                if key[0] != pid or key in seen_births:
                    continue
                seen_births.add(key)
                argv = []
                try:
                    fd = os.open("/proc/" + str(pid) + "/cmdline", os.O_RDONLY | os.O_NONBLOCK)
                    try:
                        raw = os.read(fd, 65537)
                        need(len(raw) <= 65536, "owned actor cmdline exceeded bound")
                        argv = [os.fsdecode(value) for value in raw.rstrip(b"\0").split(b"\0")] if raw else []
                    finally:
                        os.close(fd)
                except (FileNotFoundError, ProcessLookupError):
                    pass
                ledger.write(json.dumps({"event": "ACQUIRED", "identity": row["identity"],
                    "identity_after": row["identity_after"], "argv_observed": argv,
                    "monotonic_ns": time.monotonic_ns()}, sort_keys=True) + "\n"); ledger.flush()

        def discover(self):
            super().discover()
            roots = {child.pid for child in self.children if child.poll() is None}
            for key, row in list(self.owned.items()):
                if key[0] in roots or not common.pidfd_terminal(row["fd"]):
                    continue
                ledger.write(json.dumps({"event": "RETIRED_TERMINAL_PIDFD", "identity": row["identity"],
                    "monotonic_ns": time.monotonic_ns()}, sort_keys=True) + "\n"); ledger.flush()
                os.close(row["fd"]); del self.owned[key]

    try:
        before = guard(); state["cgroup_before"] = before
        state["source_before"] = common.source_state(source, manifest)
        state["source_manifest_sha256"] = state["source_before"]["manifest"]["sha256"]
        need(remember(source / "tools/ros_distro/common.py")["sha256"] == helper_sha, "executed helper differs")
        for item in (Path(__file__).resolve(), source / "test/0073.ros-tests/support/source/ros_debugger/build_runtime.py", manifest, config_path):
            remember(item)
        for index, root in enumerate(roots):
            guard(); snapshots.append(common.catalog(root, out / ("inputs-before-%03d.jsonl" % index), roots))
        state["input_roots_before"] = snapshots
        state["llvm_source_before"] = source_sdk(builder, common, source, sdk)
        if config["format_version"] == 2:
            state["selected_llvm_targets"] = selected_backends
            for backend in selected_backends:
                directory = sdk / "llvm/lib/Target" / backend
                need(directory.resolve(strict=True) == directory and directory.is_dir(), "selected source SDK backend is absent")
                remember(directory / "CMakeLists.txt")
        for tool in tools.values():
            remember(builder.path(tool)); builder.elf(common, builder.path(tool), target)
        runner = RetiringRunner(out, dict(builder.LIMITS), guard)
        contract = source / "xmake/llvm/contract.cmake"
        contract_bytes, _ = common.body_pin(contract)
        versions = builder.re.findall(rb'if\(NOT PACKAGE_VERSION STREQUAL "([^"\n]+)"\)', contract_bytes)
        need(len(versions) == 1, "one current patched contract version required")
        version = versions[0].decode(); need("-uwvm-" in version, "explicit patched suffix required")
        suffix = "-uwvm-" + version.split("-uwvm-", 1)[1]
        cxxflags = [*config["cxxflags"]]
        linkflags = [*config["link_flags"], "-fuse-ld=" + tools["linker"], "-Wl,--threads=1"]
        argv = [tools["cmake"], "-S", str(sdk / "llvm"), "-B", str(build), "-G", "Ninja",
                "-DCMAKE_MAKE_PROGRAM=" + tools["ninja"], "-DCMAKE_BUILD_TYPE=Release",
                "-DCMAKE_C_COMPILER=" + tools["cc"], "-DCMAKE_CXX_COMPILER=" + tools["cxx"],
                "-DCMAKE_C_COMPILER_ARG1=--driver-mode=gcc", "-DCMAKE_CXX_COMPILER_ARG1=--driver-mode=g++",
                "-DCMAKE_C_COMPILER_TARGET=" + target["triple"], "-DCMAKE_CXX_COMPILER_TARGET=" + target["triple"],
                "-DCMAKE_ASM_COMPILER=" + tools["cc"], "-DCMAKE_ASM_COMPILER_TARGET=" + target["triple"],
                "-DCMAKE_AR=" + tools["ar"], "-DCMAKE_RANLIB=" + tools["ranlib"], "-DCMAKE_LINKER=" + tools["linker"],
                "-DCMAKE_C_FLAGS=" + shlex.join(config["cflags"]), "-DCMAKE_CXX_FLAGS=" + shlex.join(cxxflags),
                "-DCMAKE_EXE_LINKER_FLAGS=" + shlex.join(linkflags), "-DCMAKE_SHARED_LINKER_FLAGS=" + shlex.join(linkflags),
                "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON", "-DCMAKE_POSITION_INDEPENDENT_CODE=ON",
                "-DCMAKE_PROJECT_LLVM_INCLUDE=" + str(contract), "-DUWVM_ROS_REQUIRE_EXPLICIT_HOST_TRIPLE=ON",
                "-DLLVM_HOST_TRIPLE=" + target["triple"], "-DLLVM_DEFAULT_TARGET_TRIPLE=" + target["triple"],
                "-DLLVM_TARGETS_TO_BUILD=" + ";".join(selected_backends), "-DLLVM_VERSION_SUFFIX=" + suffix, "-DLLVM_APPEND_VC_REV=OFF",
                "-DLLVM_ENABLE_PROJECTS=", "-DLLVM_ENABLE_RUNTIMES=", "-DLLVM_INCLUDE_TESTS=OFF",
                "-DLLVM_INCLUDE_BENCHMARKS=OFF", "-DLLVM_INCLUDE_EXAMPLES=OFF", "-DLLVM_INCLUDE_DOCS=OFF",
                "-DLLVM_ENABLE_BINDINGS=OFF", "-DLLVM_BUILD_TOOLS=OFF", "-DLLVM_TOOL_LLVM_CONFIG_BUILD=OFF",
                "-DLLVM_BUILD_UTILS=OFF", "-DLLVM_BUILD_LLVM_DYLIB=OFF", "-DLLVM_LINK_LLVM_DYLIB=OFF",
                "-DBUILD_SHARED_LIBS=OFF", "-DLLVM_ENABLE_ZLIB=OFF", "-DLLVM_ENABLE_ZSTD=OFF",
                "-DLLVM_ENABLE_LIBXML2=OFF", "-DLLVM_ENABLE_CURL=OFF", "-DLLVM_ENABLE_LIBEDIT=OFF",
                "-DLLVM_ENABLE_ASSERTIONS=OFF", "-DLLVM_ENABLE_EH=OFF", "-DLLVM_ENABLE_RTTI=OFF",
                "-DCMAKE_DISABLE_PRECOMPILE_HEADERS=ON", "-DLLVM_ENABLE_LTO=OFF",
                "-DLLVM_PARALLEL_LINK_JOBS=1", "-DLLVM_PARALLEL_COMPILE_JOBS=",
                "-DLLVM_CCACHE_BUILD=OFF",
                "-DPython3_EXECUTABLE=" + tools["python"]]
        if config["sysroot"]:
            argv.append("-DCMAKE_SYSROOT=" + str(builder.path(config["sysroot"], True)))
        runner.run("llvm-configure", argv, out, environment)
        # The genuine dependency target includes only CMake's required static
        # closure. The metadata executable itself is never built or executed.
        runner.limits = dict(builder.LIMITS, command_seconds=config["build_seconds"])
        extra_targets = ["LLVM"+backend+suffix for backend in selected_backends[1:] for suffix in ("CodeGen", "AsmParser", "Disassembler")]
        runner.run("llvm-static-build", [tools["ninja"], "-C", str(build), "-j1", "-v", "-d", "keepdepfile",
                   "-d", "keeprsp", "uwvm_ros_llvm_contract_dependencies", *extra_targets], out, environment)
        runner.limits = dict(builder.LIMITS)
        if config["format_version"] == 2:
            state["additional_backend_evidence"] = additional_backend_evidence(builder, common, build, target,
                selected_backends[1:], outputs, guard)
        tokens = response_tokens(common, build)
        response = out / "consumer-link.rsp"
        response.write_text(shlex.join(tokens) + "\n", encoding="utf-8")
        archive_rows = []
        for token in dict.fromkeys(tokens):
            if not token.startswith("-") and token.endswith(".a"):
                pin, _ = builder.archive(common, builder.path(token), target)
                archive_rows.append({"path": pin["path"], "size": pin["bytes"], "sha256": pin["sha256"]})
        need(archive_rows, "actual built static archive closure is empty")
        certificate = {"kind": "ros-debugger-LLVM-static-package", "version": version, "stdlib": config["stdlib"],
            "source_manifest_sha256": common.opened(source / "third-parties/llvm/sources.sha256")["sha256"],
            "contract_sha256": remember(contract)["sha256"],
            "generated_version_header_sha256": common.opened(build / "include/llvm/Config/llvm-config.h")["sha256"],
            "consumer_link_rsp_sha256": common.opened(response)["sha256"], "archives": archive_rows}
        if config["format_version"] == 2:
            extra_receipt = out / "additional-backends.json"
            common.atomic_json(extra_receipt, state["additional_backend_evidence"])
            extra_pin = common.opened(extra_receipt);outputs[extra_pin["path"]] = extra_pin
            certificate["additional_backends"] = extra_pin
        candidate = out / "package-candidate.json"; common.atomic_json(candidate, certificate)
        selected_llvm = {"mode": "static-response", "package_manifest": str(candidate),
            "package_sha256": common.opened(candidate)["sha256"], "source_lock": str(source / "third-parties/llvm/sources.sha256"),
            "contract": str(contract), "source_root": str(sdk), "build_root": str(build),
            "generated_include": str(build / "include"), "public_include": str(source / "third-parties/llvm/llvm/include"),
            "configuration": [str(build / "include/llvm/Config/llvm-config.h")],
            "consumer_response_file": str(response), "shared_library": None}
        # Reuse the exact frozen Runtime producer's full package qualification,
        # including STATIC_LIBRARY target artifacts, member ELF and stdlib ABI.
        probe_config = {"llvm": selected_llvm, "target": target, "stdlib": config["stdlib"],
                        "compiler": [str(builder.path(tools["cxx"]))], "compile_flags": cxxflags}
        closure, _, package_info, _ = builder.llvm_inputs(common, probe_config, source, roots + [out], remember)
        state["verified_llvm_closure"] = closure; state["package_info"] = package_info
        dep_index = out / "full-dependencies.jsonl"
        # Keep original full-MD files and object pins, streaming deduplicated
        # physical dependencies through the shared input dictionary.
        state["full_dependency_records"] = full_dependencies(builder, common, build, source, roots,
            out / "llvm-static-build.log", dep_index, remember, outputs)
        for item in (response, candidate, build / "compile_commands.json", build / "CMakeCache.txt",
                     build / "build.ninja", build / ".ninja_log", build / ".ninja_deps"):
            pin = common.opened(item); outputs[pin["path"]] = pin
        state["build_executed"] = True
    except BaseException as error:
        state["error"] = repr(error)
    finally:
        try:
            state["cleanup"] = runner.close() if runner else {"status": "PASS", "remaining": [], "scope": "NO_ACTOR_STARTED"}
        except BaseException as error:
            state["cleanup"] = {"status": "CLEANUP_INCOMPLETE", "remaining": [], "error": repr(error)}
        ledger.flush(); os.fsync(ledger.fileno()); ledger.close()
        state["commands"] = runner.commands if runner else []
        try:
            need(state["cleanup"]["status"] == "PASS" and not state["cleanup"]["remaining"], "owned LLVM build cleanup incomplete")
            state["source_after"] = common.source_state(source, manifest)
            need(state["source_after"] == state["source_before"], "complete frozen implementation changed")
            state["llvm_source_after"] = source_sdk(builder, common, source, sdk)
            need(state["llvm_source_after"] == state["llvm_source_before"], "full LLVM source SDK changed")
            after = []
            for index, old in enumerate(snapshots):
                guard(); value = common.catalog(roots[index], out / ("inputs-after-%03d.jsonl" % index), roots)
                need({key: val for key, val in old.items() if key != "catalog"} ==
                     {key: val for key, val in value.items() if key != "catalog"}, "complete SDK/tool root changed")
                after.append(value)
            state["input_roots_after"] = after
            for snapshot in snapshots + after:
                need(common.opened(snapshot["catalog"]["path"]) == snapshot["catalog"], "input root catalog changed")
            for row in state["commands"]:
                if "log" in row:
                    need(common.opened(row["log"]["path"]) == row["log"], "original command raw log changed")
                    outputs[row["log"]["path"]] = row["log"]
            for pin in list(pins.values()) + list(outputs.values()):
                need(common.opened(pin["path"]) == pin, "physical input/output changed")
            state["cgroup_after"] = guard()
            state["source_preserved"] = state["inputs_preserved"] = state["products_preserved"] = True
            if state.get("build_executed") is True and "error" not in state:
                state["status"] = "PASS"
        except BaseException as error:
            state["finalization_error"] = repr(error)
        state["input_pins"] = list(pins.values()); state["products"] = list(outputs.values())
        state["actor_ledger"] = common.opened(out / "actor-births.jsonl")
        state["scope"] = "FRESH_PATCHED_STATIC_LLVM_DEPENDENCY_ONLY_NO_WORKER_OR_GUEST_OR_ROS_EXECUTION"
        common.atomic_json(out / "prepare-evidence.json", state)
        if state["status"] == "PASS":
            certificate["preparation_evidence"] = common.opened(out / "prepare-evidence.json")
            common.atomic_json(out / "package-manifest.json", certificate)
            selected_llvm.update(package_manifest=str(out / "package-manifest.json"),
                                 package_sha256=common.opened(out / "package-manifest.json")["sha256"])
            selected_llvm["configuration"].append(str(out / "prepare-evidence.json"))
            common.atomic_json(out / "runtime-llvm-selection.json", {"llvm": selected_llvm})
    print(json.dumps({"status": state["status"], "evidence": str(out / "prepare-evidence.json"),
                      "package": str(out / "package-manifest.json") if state["status"] == "PASS" else None}), flush=True)
    return 0 if state["status"] == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
