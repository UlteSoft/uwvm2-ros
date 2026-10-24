#!/usr/bin/env python3
"""Root-invoked serial modular/Unitree build preparation, never ROS delivery.

The author has not executed this program. It consumes an actual complete frozen
source and the maintained Unitree Frame/SerialRunner. No SDK, compiler, source
generator, target package, ROS process, simulator or network is discovered or
downloaded. All tool invocation names and physical pins are explicit inputs.
Only the two ordinary native C mathematical contracts execute here. Wasm code
is inspected and fully validated, never instantiated or granted authority.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import select
import shlex
import stat
import sys
import traceback
import types

sys.dont_write_bytecode = True
MIB = 1024**2
CAP = 512 * MIB
MODULAR = "sdk/examples/robotics_modular"
UNITREE = "sdk/robots/unitree"


def need(ok, reason):
    if not ok:
        raise ValueError(reason)


def closed(value, keys, label):
    need(type(value) is dict and set(value) == set(keys), "closed " + label + " required")


def raw_file(path, limit=16*MIB):
    """Bootstrap stable regular bytes; do not import a helper before admission."""
    path = Path(path)
    need(path.is_absolute(), "absolute file required")
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(fd)
        need(stat.S_ISREG(first.st_mode) and 0 <= first.st_size <= limit, "bounded regular file required")
        body = bytearray()
        while True:
            part = os.read(fd, 65536)
            if not part:
                break
            body.extend(part)
            need(len(body) <= limit, "file grew beyond its bound")
        last, named = os.fstat(fd), path.stat(follow_symlinks=False)
        need(len(body) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
             for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")), "opened file changed")
        return bytes(body)
    finally:
        os.close(fd)


def bootstrap(args):
    source = args.source.resolve(strict=True)
    body = raw_file(args.manifest.resolve(strict=True))
    need(hashlib.sha256(body).hexdigest() == args.manifest_sha256, "actual complete MF differs")
    frozen = json.loads(body)
    need(type(frozen) is dict and frozen, "complete frozen MF required")
    own = raw_file(Path(__file__).resolve(), 2*MIB)
    need(hashlib.sha256(own).hexdigest() == args.runner_sha256, "reviewed private runner bytes differ")
    relative = "tests/ros_unitree/common.py"
    path = source / relative
    helper = raw_file(path, MIB)
    need(frozen.get(relative) == {"bytes": len(helper), "sha256": hashlib.sha256(helper).hexdigest()},
         "Unitree Frame helper differs from the explicit MF")
    module = types.ModuleType("uwvm_example_frame")
    module.__file__ = str(path)
    sys.modules[module.__name__] = module
    exec(compile(helper, str(path), "exec"), module.__dict__)
    return module, frozen


def admitted(frame, frozen, relative, name):
    path = frame.source / relative
    body = raw_file(path, MIB)
    need(frozen.get(relative) == {"bytes": len(body), "sha256": hashlib.sha256(body).hexdigest()},
         "selected maintained helper differs from the current MF")
    frame.pin(path)
    module = types.ModuleType(name)
    module.__file__ = str(path)
    sys.modules[name] = module
    exec(compile(body, str(path), "exec"), module.__dict__)
    return module


def alias(path):
    """Preserve multicall invocation spelling independently of physical bytes."""
    path = Path(path)
    need(path.is_absolute() and path.parent.resolve(strict=True) == path.parent, "canonical invocation parent required")
    first = path.lstat()
    target = os.readlink(path) if stat.S_ISLNK(first.st_mode) else None
    need(stat.S_ISREG(first.st_mode) or stat.S_ISLNK(first.st_mode), "regular tool or selected tool alias required")
    resolved = path.resolve(strict=True)
    last = path.lstat()
    need(all(getattr(first, key) == getattr(last, key) for key in
         ("st_dev", "st_ino", "st_mode", "st_size", "st_mtime_ns", "st_ctime_ns")) and
         (target is None or os.readlink(path) == target), "tool invocation alias changed")
    return {"path": str(path), "physical": str(resolved), "target": target,
        "device": first.st_dev, "inode": first.st_ino, "mode": first.st_mode,
        "bytes": first.st_size, "mtime_ns": first.st_mtime_ns, "ctime_ns": first.st_ctime_ns}


def contained(frame, path, owned=False):
    path = Path(path).resolve(strict=True)
    need(path.is_relative_to(frame.source) or any(path.is_relative_to(root) for root in frame.roots) or
         (owned and path.is_relative_to(frame.out)), "input lacks a complete frozen/tool/SDK inventory: " + str(path))
    return path


def select_inputs(frame, plan):
    closed(plan, ("format_version", "kind", "tools", "paths", "generated", "seed", "iterations", "maintained_preloads",
        "native_elf", "host_c_flags", "host_link_flags", "tinygo_wasi_imports", "unitree_wasi_imports"), "build selection")
    need(plan["format_version"] == 1 and plan["kind"] == "unitree-modular-example-build-selection", "selection version/kind differs")
    names = ("PYTHON", "CLANG", "CLANGXX", "WASI_CLANG", "WASI_CLANGXX", "WASM_LD", "RUSTC", "NODE", "ASC_JS",
             "TINYGO", "GO", "HOST_CC", "WASM_TOOLS")
    closed(plan["tools"], names, "explicit tool invocation map")
    values, aliases = {}, []
    for name, selected in plan["tools"].items():
        closed(selected, ("input", "invocation"), "tool selection")
        physical = frame.input(selected["input"])
        item = alias(selected["invocation"])
        need(Path(item["physical"]) == physical, "invocation and actual pinned tool differ: " + name)
        if name != "ASC_JS":
            need(os.access(selected["invocation"], os.X_OK), "selected executable is not executable")
        values[name] = selected["invocation"]
        aliases.append(item)
    need(Path(values["PYTHON"]).resolve(strict=True) == Path(sys.executable).resolve(strict=True),
         "running Python differs from the explicitly pinned interpreter")
    closed(plan["paths"], ("WASI_SYSROOT", "RUST_SYSROOT", "GO_ROOT", "TINYGO_ROOT"), "target package paths")
    for name, value in plan["paths"].items():
        path = Path(value)
        need(path.is_absolute() and path.resolve(strict=True) == path and path.is_dir(), "canonical actual target package directory required")
        contained(frame, path)
        values[name] = str(path)
    need(type(plan["seed"]) is int and 0 <= plan["seed"] <= 0xffffffff and type(plan["iterations"]) is int and
         1 <= plan["iterations"] <= 1000000, "finite exact benchmark seed/count required")
    values.update(SEED=str(plan["seed"]), ITERATIONS=str(plan["iterations"]))
    for group in ("host_c_flags", "host_link_flags"):
        need(type(plan[group]) is list and len(plan[group]) <= 64 and all(type(v) is str and v and
             not any(c in v for c in "\0\r\n{}") and not v.startswith("@") for v in plan[group]), "literal bounded host flags required")
        need(not any(v in ("-o", "-c", "-MMD", "-MP", "-Ofast", "-ffast-math", "-DNDEBUG") for v in plan[group]),
             "host flags cannot replace owned outputs, assertions, full-MD or numerical semantics")
    return values, aliases


def generation(frame, selected, family):
    closed(selected, ("directory", "generation_input"), "generated family selection")
    directory = Path(selected["directory"])
    need(directory.is_absolute() and directory.resolve(strict=True) == directory and directory.is_dir(), "canonical generated directory required")
    contained(frame, directory)
    proof, _ = frame.common.document(frame.input(selected["generation_input"]))
    need(frame.input(selected["generation_input"]) == directory / "generation.json", "selected generation receipt is not beside the actual outputs")
    need(proof.get("format") == "uwvm-ros-custom-bounded-v2" and isinstance(proof.get("outputs"), dict), "actual bounded generation receipt required")
    for field, relative in (("generator_sha256", "sdk/tools/custom_types.py"),
                            ("bounded_generator_sha256", "sdk/custom-types/bounded_profile.py")):
        need(proof.get(field) == frame.pin(frame.source/relative)["sha256"], "generated family is not bound to today's actual frozen generator")
    names = {row.get("type") for row in proof.get("types", []) if row.get("ros_version") == 2}
    required = {"unitree_" + family + "/msg/LowCmd"}
    if family == "go":
        required.add("unitree_go/msg/LowState")
    need(required <= names, "complete selected Unitree command/state generated family missing")
    for name, expected in proof["outputs"].items():
        need(Path(name).name == name and name not in (".", ".."), "flat safe generated output name required")
        actual = frame.pin(directory / name)
        need(actual["bytes"] == expected["bytes"] and actual["sha256"] == expected["sha256"], "generated output differs from its receipt")
    for name in ("bounded_records.h", "bounded_records.hpp"):
        need(name in proof["outputs"], "actual complete C/C++ generated codecs missing")
    return str(directory)


def expand(value, replacements):
    need(type(value) is str and "\0" not in value and "\n" not in value, "literal clean recipe token required")
    for name, replacement in replacements.items():
        value = value.replace("{" + name + "}", replacement)
    need("{" not in value and "}" not in value, "unresolved template token: " + value)
    return value


def signatures(rows):
    need(type(rows) is list and len(rows) <= 64, "explicit bounded WASI signature list required")
    result = {}
    for row in rows:
        closed(row, ("name", "params", "results"), "WASI expected import")
        need(re.fullmatch(r"[a-z][a-z0-9_]*", row["name"]) and row["name"] not in result and
             type(row["params"]) is list and type(row["results"]) is list and len(row["params"]) <= 16 and len(row["results"]) <= 1 and
             all(v in ("i32", "i64") for v in row["params"] + row["results"]), "closed core WASIp1 scalar signature required")
        result[row["name"]] = (row["params"], row["results"])
    return result


def wasm_structure(data, inspector):
    """Reuse the maintained bounded core reader, adding actual export/memory data.

    Full wasm-tools validation remains a separate actual child command. New GC,
    memory64, shared memories or unsupported constant expressions fail closed;
    these scalar examples do not claim those proposals as their own coverage.
    """
    R = inspector.Reader
    source = R(data)
    need(bytes(source.take(8)) == b"\0asm\1\0\0\0", "actual core Wasm binary required")
    types_, functions, globals_, memories, exports, start = [], [], [], [], {}, None
    imported_globals = {}
    seen = set()
    def limits(body):
        flags = body.uint()
        need(flags in (0, 1), "shared/memory64 limits are outside this recipe")
        minimum = body.uint()
        return {"minimum": minimum, "maximum": body.uint() if flags else None}
    def global_type(body):
        value, mutable = body.value(), body.byte()
        need(mutable in (0, 1), "invalid global mutability")
        return {"type": value, "mutable": bool(mutable)}
    def expression(body):
        operation = body.byte()
        if operation in (0x41, 0x42):
            for _ in range(10):
                if not body.byte() & 128:
                    break
            else:
                raise ValueError("unbounded initializer integer")
        elif operation in (0x43, 0x44):
            body.take(4 if operation == 0x43 else 8)
        elif operation in (0x23, 0xd2):
            body.uint()
        elif operation == 0xd0:
            body.value()
        else:
            raise ValueError("unsupported initializer expression in scalar artifact")
        need(body.byte() == 0x0b, "nontrivial initializer outside scalar recipe")
    while source.position < len(source.data):
        section, body = source.byte(), None
        body = R(source.take(source.uint()))
        if section and section in seen:
            raise ValueError("duplicate Wasm section")
        seen.add(section)
        if section == 1:
            for _ in range(body.count()):
                need(body.byte() == 0x60, "ordinary function type required")
                types_.append(([body.value() for _ in range(body.count())], [body.value() for _ in range(body.count())]))
        elif section == 2:
            for _ in range(body.count()):
                imported_module, imported_name, kind = body.name(), body.name(), body.byte()
                if kind == 0:
                    functions.append(body.uint())
                elif kind == 1:
                    body.value(); limits(body)
                elif kind == 2:
                    memories.append(dict(limits(body), imported=True))
                elif kind == 3:
                    value = global_type(body)
                    globals_.append(value)
                    imported_globals[imported_module+"."+imported_name] = value
                else:
                    raise ValueError("tag import outside scalar recipe")
        elif section == 3:
            functions.extend(body.uint() for _ in range(body.count()))
        elif section == 5:
            memories.extend(dict(limits(body), imported=False) for _ in range(body.count()))
        elif section == 6:
            for _ in range(body.count()):
                globals_.append(global_type(body)); expression(body)
        elif section == 7:
            for _ in range(body.count()):
                name, kind, index = body.name(), body.byte(), body.uint()
                need(name not in exports, "duplicate export name")
                value = {"kind": kind, "index": index}
                if kind == 0:
                    need(index < len(functions) and functions[index] < len(types_), "exported function type absent")
                    value.update(params=types_[functions[index]][0], results=types_[functions[index]][1])
                elif kind == 3:
                    need(index < len(globals_), "exported global absent")
                    value.update(globals_[index])
                exports[name] = value
        elif section == 8:
            start = body.uint()
        else:
            continue
        body.end()
    return {"exports": exports, "memories": memories, "start_function": start, "imports": inspector.inspect(data),
            "imported_globals": imported_globals}


def sig(text):
    left, right = text.split("->")
    return ([v for v in left[1:-1].split(",") if v], [] if right == "()" else [right])


def contract(frame, inspector, schema, row, data, plan, recipes):
    actual = wasm_structure(data, inspector)
    name = row["id"]
    required, foreign, backend, wasi, memory_limit = {}, {}, None, {}, 2
    full = lambda key: {tuple(k.split(".", 1)): sig(v) for k, v in recipes["import_export_contracts"][key].get("imports", {}).items()}
    if name in ("callback-c", "callback-cpp"):
        required = recipes["import_export_contracts"]["callbacks"]["exports"]
    elif name in ("motion-c", "motion-cpp"):
        required = recipes["import_export_contracts"]["motion"]["exports"]; foreign = full("motion")
    elif name.startswith("vision-"):
        required = recipes["import_export_contracts"]["vision"]["exports"]
        if name == "vision-tinygo-conditional":
            required = {"_initialize": "()->()", "vision_clearance_mm": "(i32,i32,i32,i32)->i32"}
            wasi = signatures(plan["tinygo_wasi_imports"]); memory_limit = 256
    elif name.startswith("main-"):
        required = {"_start": "()->()"}
        foreign = {tuple(k.split(".", 1)): sig(v) for k, v in recipes["import_export_contracts"]["main"]["required_imports"].items()}
        backend = "1" if "ros1" in name else "2"
        wasi = {"fd_write": (["i32"]*4, ["i32"]), "proc_exit": (["i32"], [])}
        if "publish" in name:
            wasi["fd_read"] = (["i32"]*4, ["i32"])
        if name.startswith("main-cpp"):
            memory_limit = 4
    elif name.startswith("callback-ros"):
        required = recipes["import_export_contracts"]["callbacks"]["exports"]
        backend = "1" if "ros1" in name else "2"
    elif name in ("benchmark-main", "benchmark-session-main"):
        required = {"_start": "()->()"}
        foreign = {tuple(k.split(".", 1)): sig(v) for k, v in recipes["import_export_contracts"]["main"]["required_imports"].items()}
        wasi = {"fd_write": (["i32"]*4, ["i32"]), "proc_exit": (["i32"], [])}
        if name == "benchmark-session-main":
            wasi["fd_read"] = (["i32"]*4, ["i32"])
    elif name == "tinygo_adapter":
        required = recipes["import_export_contracts"]["vision"]["exports"]
        foreign = {("vision_go", "_initialize"): ([], []), ("vision_go", "vision_clearance_mm"): (["i32"]*4, ["i32"])}
    elif name == "funcref_callback":
        foreign = {("callbacks", "on_motion"): (["i32"]*4, ["i32"]), ("callbacks", "callback_count"): (["i32"]*2, ["i32"])}
        reference = actual["exports"].get("callback_ref", {})
        need(reference.get("type") == "funcref" and reference.get("mutable") is False, "actual immutable callback reference export missing")
    elif name == "funcref_main":
        foreign = {("callbacks", "callback_begin"): (["i32"], ["i32"]), ("callbacks", "callback_end"): (["i32"]*2, ["i32"])}
        need(actual["start_function"] is not None, "funcref main has an actual start proof")
    elif name.startswith("unitree-") and name in ("unitree-callback", "unitree-motion"):
        item = row["preload_contract"]
        required = item["exports"]; foreign = {tuple(k.split(".", 1)): sig(v) for k, v in item["imports"].items()}
    else:
        need(name.startswith(("passive-", "direct-", "hold-")), "unknown closed Wasm selection")
        backend, memory_limit = "2", 256
        required = {"_start": "()->()"}; wasi = signatures(plan["unitree_wasi_imports"])
        if row["definitions"].get("UWVM_UNITREE_MODULAR_MOTION"):
            foreign = {("motion", "motion_begin"): (["i32"]*2, ["i32"]), ("motion", "motion_status"): ([], ["i32"]),
                       ("motion", "motion_word"): (["i32"]*3, ["i32"]), ("motion", "motion_end"): (["i32"], ["i32"])}
    found, ros = set(), set()
    expected_ros = {} if backend is None else {function["name"]: ([p["type"] for p in function["params"]], function["results"])
        for function in schema["modules"]["uwvm_ros"+backend+"_v1"]["functions"]}
    for item in actual["imports"]:
        key = item["module"], item["name"]
        need(key not in found, "duplicate actual import"); found.add(key)
        if name == "funcref_main" and key == ("callback_ref", "callback_ref"):
            need(item["kind"] == 3 and actual["imported_globals"].get("callback_ref.callback_ref") == {"type":"funcref", "mutable":False},
                 "function reference is an actual immutable funcref global import")
            continue
        need(item["kind"] == 0, "unexpected non-function import")
        value = item["params"], item["results"]
        if key in foreign:
            need(value == foreign[key], "wrong actual module callback signature")
        elif item["module"] == "wasi_snapshot_preview1":
            need(item["name"] in wasi and value == wasi[item["name"]], "unexpected actual WASI import/signature")
        elif backend is not None and item["module"] == "uwvm_ros"+backend+"_v1":
            need(item["name"] in expected_ros and value == expected_ros[item["name"]], "unknown/wrong actual ROS signature")
            ros.add(item["name"])
        else:
            raise ValueError("unexpected actual import: " + ".".join(key))
    need(set(foreign) <= found, "required real module imports missing")
    if backend is not None:
        need("capability_open" in ros, "actual selected ROS import absent")
    for field, text in required.items():
        item = actual["exports"].get(field)
        need(item is not None and item["kind"] == 0 and (item["params"], item["results"]) == sig(text), "actual exported scalar signature differs: " + field)
    bounded_memory = len(actual["memories"]) <= 1 and all(not item["imported"] and item["maximum"] is not None and
         0 <= item["minimum"] <= item["maximum"] <= memory_limit for item in actual["memories"])
    if name == "vision-tinygo-conditional":
        actual["conditional_preload_admission"] = "NOT_EXECUTED_NOT_AUTHORIZED" if bounded_memory else "UNBOUNDED_MEMORY_REFUSES_PRELOAD_ADMISSION"
    else:
        need(bounded_memory, "actual memory maximum/import contract differs")
    if name in ("unitree-callback", "unitree-motion"):
        need(len(actual["memories"]) == 1 and actual["memories"][0] == {"minimum":2, "maximum":2, "imported":False},
             "Unitree providers require their actual separate fixed two-page memories")
    if name not in ("funcref_main", "vision-tinygo-conditional"):
        need(actual["start_function"] is None, "unexpected implicit Wasm start section")
    actual.update(status="CONDITIONAL_BINARY_INSPECTED" if name == "vision-tinygo-conditional" else "BINARY_CONTRACT_PASS", actual_execution="NOT_EXECUTED", granted_rights=[],
        bounded_memory=bounded_memory,
        zero_authority_startup_qualified=False, scope="Actual binary signatures/memory only; no instance startup, authority or ROS delivery")
    return actual


def full_md(frame, dependency, obj, source, cwd):
    body, _ = frame.common.body_pin(dependency)
    text = body.decode("utf-8").replace("\\\n", " ")
    need(":" in text, "actual complete dependency rule required")
    left, right = text.split(":", 1)
    need(shlex.split(left) == [str(obj)], "dependency target differs from fresh actual object")
    names = shlex.split(right)
    need(names and not any(value.endswith(":") for value in names), "single full-MD rule required")
    result = {}
    for value in names:
        path = Path(value)
        path = contained(frame, path if path.is_absolute() else cwd/path, owned=True)
        pin = frame.pin(path); result[pin["path"]] = pin
    need(str(source.resolve(strict=True)) in result, "actual source absent from compiler dependencies")
    return list(result.values())


def trace_inputs(frame, command, cwd):
    """Retain all existing linker trace inputs; no nonexistent file is invented."""
    text = raw_file(Path(command["log"]["path"]), frame.limits["log_bytes"]).decode("utf-8", "strict")
    result = {}
    for line in text.splitlines():
        value = line.strip()
        candidate = value.split("(", 1)[0] if value.endswith(")") else value
        if not candidate or any(c in candidate for c in "\0\t"):
            continue
        path = Path(candidate)
        if not path.is_absolute():
            path = cwd / path
        if path.is_file():
            path = contained(frame, path, owned=True)
            pin = frame.pin(path); result[pin["path"]] = pin
    return list(result.values())


def run(frame, owner, label, argv, cwd, environment):
    need(Path(argv[0]).is_absolute() and Path(argv[0]).resolve(strict=True).is_file(), "actual explicit command tool required")
    row = owner.run(label, argv, cwd, environment)
    owner.discover()
    retired = []
    for key, item in list(owner.owned.items()):
        need(frame.common.pidfd_terminal(item["fd"]), "command left a live owned descendant; refuse overlapping command")
        retired.append({"first": item["identity"], "after_pidfd": item["identity_after"], "terminal": True})
        os.close(item["fd"]); del owner.owned[key]
    for key, item in owner.unproven.items():
        now = frame.common.identity(item["pid"])
        need(frame.common.birth(now) != key or now["state"] == "Z", "unproven command descendant still live")
    row["terminal_owned_births"] = retired
    frame.pin(row["log"]["path"])
    frame.common.atomic_json(frame.out/"commands.json", owner.commands)
    frame.save()
    return row


def receipt_command(frame, row, dependencies):
    log = frame.pin(row["log"]["path"])
    # SerialRunner really merged the streams; equality is intentional and
    # never presented as two independent captures.
    return {"argv": row["argv"], "returncode": row["returncode"], "stdout": log, "stderr": log,
            "dependencies": dependencies}


def clang(frame, owner, row, environment, values):
    folder = frame.out/row["id"]; folder.mkdir(mode=0o700)
    artifact = Path(row["artifact"])
    argv = list(row["argv"])
    need(not any(value.startswith("--ld-path=") or any(part in ("--start-group", "--end-group") for part in value.split(",")) for value in argv),
         "Wasm jobs cannot use an ignored linker selector or ELF archive groups")
    sources = [Path(value) for value in argv[1:] if value.endswith((".c", ".cpp")) and Path(value).is_file()]
    need(len(sources) == 1 and argv.count("-o") == 1 and argv[argv.index("-o")+1] == str(artifact), "one actual C/C++ source and owned artifact required")
    source = contained(frame, sources[0]); obj, dep = folder/"translation.o", folder/"translation.d"
    cpp = source.suffix == ".cpp"
    driver = [argv[0], "--driver-mode=g++" if cpp else "--driver-mode=gcc"]
    flags, skip = [], False
    for value in argv[1:]:
        if skip:
            skip = False; continue
        if value == "-o":
            skip = True; continue
        if value == str(source) or value.startswith("-Wl,") or value.startswith("-fuse-ld=") or value in ("-nostdlib", "-nostartfiles"):
            continue
        if value.endswith(".a") and Path(value).is_absolute():
            continue
        flags.append(value)
    compile_argv = [*driver, *flags, "-MD", "-MF", str(dep), "-MT", str(obj), "-c", str(source), "-o", str(obj)]
    compiled = run(frame, owner, row["id"]+"-compile", compile_argv, Path(row["cwd"]), environment)
    dependencies = full_md(frame, dep, obj, source, Path(row["cwd"]))
    frame.pin(dep); frame.pin(obj)
    linked_argv = [*driver, *[str(obj) if value == str(source) else value for value in argv[1:]], "-Wl,--trace", "-Wl,--threads=1"]
    if not any(value.startswith("-fuse-ld=") for value in linked_argv):
        linked_argv.insert(2, "-fuse-ld="+values["WASM_LD"])
    need([value for value in linked_argv if value.startswith("-fuse-ld=")] == ["-fuse-ld="+values["WASM_LD"]],
         "one exact selected Wasm linker alias required")
    linked = run(frame, owner, row["id"]+"-link", linked_argv, Path(row["cwd"]), environment)
    link_inputs = trace_inputs(frame, linked, Path(row["cwd"]))
    need(any(pin["path"] == str(obj) for pin in link_inputs), "actual linker trace omitted the fresh object")
    return {"object": frame.pin(obj), "dependency_file": frame.pin(dep), "dependencies": dependencies,
            "link_inputs": link_inputs, "commands": [receipt_command(frame, compiled, dependencies), receipt_command(frame, linked, link_inputs)]}


def unitree_rows(frame, values):
    result = []
    base = [values["WASI_CLANGXX"], "--target=wasm32-wasip1", "--sysroot="+values["WASI_SYSROOT"], "-std=c++20", "-O3", "-g0", "-UNDEBUG",
        "-fno-fast-math", "-ffp-contract=off", "-fno-rtti", "-fno-exceptions", "-I"+str(frame.source/"sdk/include"),
        "-I"+str(frame.source/UNITREE/"include"), "-I"+str(frame.source/MODULAR), "-I"+str(frame.source/"third-parties/fast_io/include"),
        "-I"+str(frame.source/"third-parties/bizwen/include"), "-I"+str(frame.source/"third-parties/boost_unordered/include")]
    def add(name, source, definitions, family, c=False):
        artifact = frame.out/"wasm"/(name+".wasm")
        flags = [values["WASI_CLANG"], *["-std=c11" if value=="-std=c++20" else value for value in base[1:] if value not in ("-fno-rtti", "-fno-exceptions")]] if c else list(base)
        flags.extend("-D"+key+"="+str(value) for key, value in definitions.items())
        flags.extend(["-I"+values["GENERATED_"+family.upper()], str(frame.source/UNITREE/"examples"/source),
            "-Wl,--allow-undefined", "-Wl,--initial-memory=131072", "-Wl,--max-memory=16777216", "-Wl,-z,stack-size=65536", "-o", str(artifact)])
        result.append({"id": name, "argv": flags, "cwd": str(frame.source), "artifact": str(artifact), "definitions": definitions})
    for family, number in (("go", 1), ("hg", 2)):
        for language, filename in (("c", "passive_command.c"), ("cpp", "passive_command.cpp")):
            for enabled in (0, 1):
                add("passive-"+family+"-"+language+"-"+str(enabled), filename,
                    {"UWVM_UNITREE_FAMILY": number, "UWVM_UNITREE_SEND": enabled}, family, language=="c")
    need(len(result) == 8, "closed passive GO/HG x C/C++ x denied/send inventory differs")
    return result


def native_models(frame, owner, values, plan, environment):
    selections = {"x86_64": (62, 64, "little"), "aarch64": (183, 64, "little"), "riscv64": (243, 64, "little"),
                  "loongarch64": (258, 64, "little"), "ppc64le": (21, 64, "little"), "s390x": (22, 64, "big")}
    closed(plan["native_elf"], ("machine", "bits", "endian"), "native architecture")
    need(tuple(plan["native_elf"][key] for key in ("machine", "bits", "endian")) == selections.get(os.uname().machine), "native C models must match the actual host, not an unrecorded emulator")
    results = []
    for name, count in (("crc_contract", 29), ("topic_rpc_contract", 19)):
        folder = frame.out/name; folder.mkdir()
        wrapper = folder/"check_count.c"
        wrapper.write_text("/* Root-owned counter witness, preserving the original model source. */\nint uwvm_model_main(void);\nunsigned uwvm_unitree_completed_checks(void);\nint main(void){int status=uwvm_model_main();if(status)return 252;return uwvm_unitree_completed_checks()=="+str(count)+"u?0:253;}\n")
        frame.pin(wrapper)
        source = frame.source/UNITREE/"tests"/(name+".c")
        objects, dependencies, commands = [], [], []
        for index, translation in enumerate((source, wrapper)):
            obj, dep = folder/("tu%d.o"%index), folder/("tu%d.d"%index)
            argv = [values["HOST_CC"], *plan["host_c_flags"], "-std=c11", "-O1", "-g0", "-UNDEBUG", "-fno-fast-math", "-ffp-contract=off",
                "-I"+str(frame.source/UNITREE/"include"), *(["-Dmain=uwvm_model_main"] if index==0 else []), "-MD", "-MF", str(dep), "-MT", str(obj), "-c", str(translation), "-o", str(obj)]
            command = run(frame, owner, name+"-compile%d"%index, argv, frame.source, environment)
            deps = full_md(frame, dep, obj, translation, frame.source); dependencies.extend(deps)
            frame.pin(dep); objects.append(frame.pin(obj)); commands.append(command)
        output = folder/name
        argv = [values["HOST_CC"], *plan["host_c_flags"], *[pin["path"] for pin in objects], *plan["host_link_flags"], "-Wl,--trace", "-o", str(output)]
        linked = run(frame, owner, name+"-link", argv, frame.source, environment)
        linked_inputs = trace_inputs(frame, linked, frame.source)
        body = raw_file(output, 64*MIB)
        endian = {1:"little", 2:"big"}.get(body[5]) if len(body)>=64 else None
        need(body[:4] == b"\x7fELF" and endian == plan["native_elf"]["endian"] and
             {1:32,2:64}.get(body[4]) == plan["native_elf"]["bits"] and int.from_bytes(body[18:20], endian) == plan["native_elf"]["machine"], "actual mathematical fixture ELF differs from the real host")
        product = frame.pin(output)
        executed = run(frame, owner, name+"-execute", [str(output)], folder, environment)
        results.append({"id": name, "status": "PASS_LOCAL_MATHEMATICAL_CONTRACT", "expected_completed_checks": count,
            "output": product, "commands": [*commands, linked, executed], "dependencies": dependencies,
            "link_inputs": linked_inputs, "scope": "Exact unchanged C model plus actual counter witness; not a native Unitree oracle or ROS test"})
    return results


def maintained_preloads(frame, owner, values, plan, environment, inspector, schema, recipes, preload_recipes):
    """Consume real maintained producer evidence; do not compile providers twice.

    The outer report must itself have completed source/input/cgroup/ownership
    checks. All retained seven-field pins, original full-MD source/object/link
    associations and full validator argv are reopened before fresh inspection.
    This is a current-source product association, never an old-version PASS.
    """
    closed(plan["maintained_preloads"], ("callbacks", "motion"), "two actual maintained providers")
    result = []
    pin_keys = {"path", "sha256", "bytes", "device", "inode", "mtime_ns", "ctime_ns"}
    required_sources = {
        "callbacks": {str(frame.source/MODULAR/"unitree_callback.c"), str(frame.source/MODULAR/"unitree_motion_abi.h")},
        "motion": {str(frame.source/MODULAR/"unitree_motion.c"), str(frame.source/MODULAR/"unitree_motion_abi.h"),
                   str(frame.source/UNITREE/"include/roswasm_unitree/simulation_control.h")}}
    for module_name in ("callbacks", "motion"):
        selected = plan["maintained_preloads"][module_name]
        closed(selected, ("artifact_input", "receipt_input", "preparation_input"), "maintained provider evidence selection")
        outer, _ = frame.common.document(frame.input(selected["preparation_input"]))
        need(outer.get("kind") == "unitree-fixture-preparation" and outer.get("status") == "PASS_ACTUAL_FIXTURE_PREPARATION" and
             outer.get("source") == str(frame.source) and outer.get("source_manifest_sha256") == frame.report["source_manifest_sha256"] and
             outer.get("source_preserved") is True and outer.get("input_pins_preserved") is True and outer.get("whole_roots_preserved") is True,
             "maintained provider preparation is not a successful preserved current-source build")
        need(outer.get("cleanup") and all(item.get("status") == "PASS" and not item.get("remaining") and not item.get("errors")
             for item in outer["cleanup"]), "maintained build cleanup was not successful")
        frame.common.cgroup_preserved(outer["cgroup_before"], frame.before)
        frame.common.cgroup_preserved(outer["cgroup_before"], outer["cgroup_after"])
        original_config, config_pin = frame.common.document(Path(outer["configuration"]["path"]))
        need(config_pin == outer["configuration"] and original_config["limits"]["jobs"] == 1 and
             original_config["limits"]["rss_bytes"] <= CAP and original_config["limits"]["command_seconds"] <= 600,
             "provider build did not use the required serial512MiB/600second budget")
        need(outer.get("commands") and all(item.get("status") == "PASS" and item.get("returncode") == 0 and
             item.get("peak_owned_rss_bytes", CAP+1) <= CAP for item in outer["commands"]), "maintained actual command result/RSS evidence differs")
        proof, _ = frame.common.document(frame.input(selected["receipt_input"]))
        closed(proof, ("format_version", "kind", "status", "source_manifest_sha256", "output", "source_inputs", "definitions",
                       "commands", "full_dependencies_verified"), "maintained fixture build receipt")
        artifact = frame.input(selected["artifact_input"])
        need(proof["format_version"] == 1 and proof["kind"] == "unitree-fixture-build" and proof["status"] == "PASS" and
             proof["source_manifest_sha256"] == frame.report["source_manifest_sha256"] and proof["output"] == frame.inputs[selected["artifact_input"]] and
             proof["definitions"] == {} and proof["full_dependencies_verified"] is True,
             "actual provider identity/source/output/definitions differ")
        # Inspect complete raw documents, not only a caller-selected pin subset.
        pending, visited = [outer, proof], 0
        while pending:
            value = pending.pop(); visited += 1
            need(visited <= 500000, "bounded retained preparation metadata required")
            if type(value) is dict:
                if set(value) == pin_keys:
                    need(frame.pin(value["path"]) == value, "maintained physical compiler/input/log/output pin changed")
                else:
                    pending.extend(value.values())
            elif type(value) is list:
                pending.extend(value)
        sources = {item["path"] for item in proof["source_inputs"]}
        need(required_sources[module_name] <= sources, "actual provider source/kernel/header binding missing")
        compiles, links, validations = [], [], []
        for command in proof["commands"]:
            closed(command, ("argv", "returncode", "stdout", "stderr", "dependencies"), "maintained command")
            argv = command["argv"]
            need(command["returncode"] == 0 and argv and Path(argv[0]).is_absolute(), "actual successful original command required")
            if "-MD" in argv:
                need(all(flag in argv for flag in ("-c", "-MF", "-o", "-O3", "-ffp-contract=off")) and
                     not any(flag in argv for flag in ("-MMD", "-MP", "-ffast-math", "-Ofast", "-DNDEBUG")), "actual bounded provider full-MD/numerical compile flags differ")
                source = frame.source/MODULAR/("unitree_callback.c" if module_name=="callbacks" else "unitree_motion.c")
                obj = Path(argv[argv.index("-o")+1]); dep = Path(argv[argv.index("-MF")+1])
                need(str(source) in argv and obj.is_absolute() and dep.is_absolute(), "actual original source/object/dependency association differs")
                # prepare.py's contract requires absolute retained prereqs.
                deps = full_md(frame, dep, obj, source, frame.source)
                need({item["path"] for item in deps} == {item["path"] for item in command["dependencies"]}, "retained full-MD body and recorded dependencies differ")
                compiles.append(str(obj))
            elif len(argv) == 3 and argv[1:] == ["validate", str(artifact)]:
                need(Path(argv[0]).resolve(strict=True) == Path(values["WASM_TOOLS"]).resolve(strict=True), "maintained validator tool differs from the explicitly selected tool")
                validations.append(command)
            else:
                links.append(command)
        need(len(compiles) == 1 and len(links) == 1 and len(validations) == 1, "closed compile/link/full-validation provider recipe required")
        link_argv = links[0]["argv"]
        need(all(obj in link_argv for obj in compiles) and "-o" in link_argv and link_argv[link_argv.index("-o")+1] == str(artifact) and
             any(flag in link_argv for flag in ("--trace", "-Wl,--trace")), "actual retained link does not consume its fresh object/trace/output")
        name = "unitree-callback" if module_name=="callbacks" else "unitree-motion"
        before = frame.pin(artifact)
        row = {"id":name, "preload_contract":preload_recipes["contracts"][name]}
        inspected = contract(frame, inspector, schema, row, raw_file(artifact,64*MIB), plan, recipes)
        validated = run(frame, owner, name+"-independent-validate", [values["WASM_TOOLS"], "validate", str(artifact)], frame.out, environment)
        need(frame.pin(artifact) == before, "maintained provider changed around fresh independent full validation")
        result.append({"module":module_name, "artifact":before, "build_receipt":frame.inputs[selected["receipt_input"]],
            "preparation_report":frame.inputs[selected["preparation_input"]], "binary_contract":inspected, "validation":validated,
            "status":"ACTUAL_CURRENT_BUILD_BOUND_BINARY_VALIDATED_NOT_EXECUTED", "compilation_in_this_runner":False, "granted_rights":[]})
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ("source", "manifest", "config", "out"):
        parser.add_argument("--"+name, type=Path, required=True)
    for name in ("manifest-sha256", "config-sha256", "runner-sha256", "plan-input"):
        parser.add_argument("--"+name, required=True)
    args = parser.parse_args()
    need(all(getattr(args, name).is_absolute() for name in ("source", "manifest", "config", "out")) and
         all(re.fullmatch(r"[0-9a-f]{64}", getattr(args, name)) for name in ("manifest_sha256", "config_sha256", "runner_sha256")),
         "explicit absolute inputs/output and actual lowercase SHA256 arguments required")
    frame = None
    current = None
    status, error = "FAIL", None
    try:
        module, frozen = bootstrap(args)
        frame = module.Frame(args, "unitree-modular-example-builds")
        need(callable(getattr(frame.common, "pidfd_terminal", None)),
             "frozen common.pidfd_terminal is required before any compiler actor")
        need(frame.limits["rss_bytes"] <= CAP and frame.limits["command_seconds"] <= 600 and frame.limits["jobs"] == 1,
             "every compiler/validator/model remains within512MiB/600seconds/j1")
        need(frame.config["simulation"] is None and frame.config["benchmark"] is None and frame.config["profiler"] is None,
             "this preparation supplies no Runtime/simulator/profiler commands")
        frame.pin(Path(__file__).resolve())
        plan, _ = frame.common.document(frame.input(args.plan_input))
        values, aliases = select_inputs(frame, plan)
        closed(plan["generated"], ("go", "hg"), "complete generated family map")
        for family, selected in plan["generated"].items():
            values["GENERATED_"+family.upper()] = generation(frame, selected, family)
        inspector = admitted(frame, frozen, "sdk/tools/check_wasm_imports.py", "uwvm_example_binary_inspector")
        schema, _ = frame.common.document(frame.source/"sdk/abi/abi-v1.json")
        frame.pin(frame.source/"sdk/abi/abi-v1.json")
        recipes, _ = frame.common.document(frame.source/MODULAR/"recipes.json")
        preloads, _ = frame.common.document(frame.source/MODULAR/"unitree-preload-recipes.json")
        frame.pin(frame.source/MODULAR/"recipes.json"); frame.pin(frame.source/MODULAR/"unitree-preload-recipes.json")
        need(len(recipes["builds"]) == 28 and len(preloads["builds"]) == 2, "closed original28/new2 recipes required")
        for name in ("wasm", "go-cache", "go-path", "go-tmp"):
            (frame.out/name).mkdir()
        values.update(SOURCE=str(frame.source), EXAMPLE=str(frame.source/MODULAR), OUT=str(frame.out/"wasm"))
        # Recipe OUT denotes the artifact/cache namespace; Frame owns it all.
        for name in ("tmp", "go-cache", "go-path", "go-tmp"):
            (frame.out/"wasm"/name).mkdir()
        environment = dict(frame.environment)
        environment.update(CARGO_NET_OFFLINE="true", CARGO_BUILD_JOBS="1", RUSTUP_AUTO_INSTALL="0", RUSTFLAGS="", RUSTC_WRAPPER="",
            RUSTC_WORKSPACE_WRAPPER="", RUSTC_BOOTSTRAP="", GOENV="off", GOWORK="off", GOFLAGS="-mod=readonly", GOMAXPROCS="1", GOMEMLIMIT="192MiB",
            RAYON_NUM_THREADS="1", BINARYEN_CORES="1", MAKEFLAGS="-j1", CMAKE_BUILD_PARALLEL_LEVEL="1",
            GOROOT=values["GO_ROOT"], TINYGOROOT=values["TINYGO_ROOT"], GOPROXY="off", GOSUMDB="off", GOTOOLCHAIN="local")
        path_dirs = list(dict.fromkeys(str(Path(values[name]).parent) for name in ("GO", "TINYGO", "RUSTC", "WASM_LD", "CLANG", "NODE")))
        for directory in environment["PATH"].split(":"):
            need(directory and Path(directory).is_absolute(), "PATH entries must be explicit absolute input directories")
            contained(frame, directory)
        for directory in environment.get("LD_LIBRARY_PATH", "").split(":") if "LD_LIBRARY_PATH" in environment else []:
            need(directory and Path(directory).is_absolute(), "loader entries must be explicit absolute input directories")
            contained(frame, directory)
        environment["PATH"] = ":".join(path_dirs+environment["PATH"].split(":"))
        owner = frame.common.SerialRunner(frame.out, frame.limits, frame.guard)
        frame.actors.append(owner)
        frame.report.update(commands=owner.commands, products=[], scope="Actual fresh compilation/import/export/memory/full-validation plus two local C mathematical contracts only",
            native_ROS="NOT_EXECUTED", simulator="NOT_EXECUTED", native_identity_generation="NOT_EXECUTED", granted_rights=[], reused_products=0)
        frame.report["maintained_preloads"] = maintained_preloads(frame, owner, values, plan, environment,
            inspector, schema, recipes, preloads)
        rows = []
        for original in recipes["builds"]:
            row = {"id": original["id"], "argv": [expand(t, values) for t in original["argv"]], "cwd": expand(original["cwd"], values),
                "artifact": expand(original["artifact"], values), "environment": {k:expand(v, values) for k,v in original["environment"].items()}, "definitions": {}}
            rows.append(row)
        rows.extend(unitree_rows(frame, values))
        need(len(rows) == 36 and len({row["id"] for row in rows}) == 36, "closed original28/passive8 Wasm inventory differs")
        for row in rows:
            product = {"id": row["id"], "status": "RUNNING", "definitions": row["definitions"]}
            current = product
            frame.report["products"].append(product); frame.save()
            env = dict(environment); env.update(row.get("environment", {}))
            artifact = Path(row["artifact"])
            need(artifact.is_relative_to(frame.out) and not artifact.exists(), "fresh owned Wasm product required")
            if any(value.endswith((".c", ".cpp")) and Path(value).is_file() for value in row["argv"][1:]):
                compiled = clang(frame, owner, row, env, values); product["compilation"] = compiled
            else:
                folder = frame.out/row["id"]; folder.mkdir()
                if row["id"] == "vision-rust":
                    dep = folder/"rust.d"
                    row["argv"].extend(["--emit=link,dep-info="+str(dep), "-C", "link-arg=--threads=1", "-C", "link-arg=--trace"])
                command = run(frame, owner, row["id"]+"-build", row["argv"], Path(row["cwd"]), env)
                product["compilation"] = {"command": command, "closure": "Full selected immutable package/sysroot inventories; not an invented Clang MD invocation"}
                if row["id"] == "vision-rust":
                    product["rust_dependency_file"] = frame.pin(dep)
            before = frame.pin(artifact)
            data = raw_file(artifact, 64*MIB)
            need(hashlib.sha256(data).hexdigest() == before["sha256"], "actual artifact changed during binary inspection")
            product["binary_contract"] = contract(frame, inspector, schema, row, data, plan, recipes)
            validated = run(frame, owner, row["id"]+"-validate", [values["WASM_TOOLS"], "validate", str(artifact)], frame.out, env)
            need(frame.pin(artifact) == before, "actual artifact changed around full validation")
            product.update(status="PREPARED_WASM_NOT_EXECUTED", artifact=before, validation=validated, granted_rights=[])
            frame.save()
            current = None
        frame.report["native_contracts"] = native_models(frame, owner, values, plan, environment)
        need(all(alias(item["path"]) == item for item in aliases), "selected multicall tool spelling/alias changed")
        frame.report.update(tool_invocation_aliases=aliases, prepared_wasm_selections=36, modular_original_selections=28,
            maintained_provider_validations=2, passive_unitree_selections=8, native_mathematical_contracts=2,
            wasm_execution="NOT_EXECUTED", conditional_tinygo_startup="NOT_EXECUTED_NOT_AUTHORIZED", all_compilation_serial=True)
        status = "PASS_BUILD_AND_BINARY_PREPARATION_ONLY"
    except BaseException as caught:
        error = repr(caught)
        if frame is not None:
            if current is not None:
                current.update(status="FAIL", error=error)
            frame.report["traceback"] = traceback.format_exc()
    finally:
        if frame is not None:
            result = frame.finish(status, error)
        else:
            result = 1
            print(json.dumps({"status":"INCOMPLETE_BEFORE_FRAME", "error":error}), file=sys.stderr)
    return result


if __name__ == "__main__":
    raise SystemExit(main())
