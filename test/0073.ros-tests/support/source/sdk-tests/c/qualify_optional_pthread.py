#!/usr/bin/env python3
"""Build real wasm32 pthread C libraries/consumers and require genuine bounded Guest runs.

This is a private, positive optional-closure candidate. It edits no shared
provider, resolver or SDK manifest. An absent compiler/runtime feature never
becomes a successful delivery or a substituted default profile.
"""

import argparse
import collections
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import stat
import subprocess
import sys


SDK = Path("/home/macromodel/Documents/uwvm-test-261018/agents/10/staged-wasi-r1/staged/wasi-sdk")
MANIFEST = SDK.parents[1] / "resource-evidence/staged-inputs.json"
MANIFEST_SHA256 = "7c9feeb25719ab24a6c7a11e9d5ec050fe8d12be8b261b11f969d7601e576a5f"
MODES = (("89", "c89"), ("90", "c90"), ("95", "iso9899:199409"), ("99", "c99"),
         ("11", "c11"), ("17", "c17"), ("23", "c23"), ("29", "c2y"))


def sha(filename):
    with filename.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def pin(filename):
    return {"path": str(filename.resolve(strict=True)), "bytes": filename.stat().st_size, "sha256": sha(filename)}


def load(name, filename):
    spec = importlib.util.spec_from_file_location(name, filename)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk", type=Path, required=True)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--runtime-sha256", required=True)
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        parser.error("original admitted finite Linux owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs, work = run / "inputs", run / "work"
    runtime = args.runtime.resolve(strict=True)
    runtime_pin = pin(runtime)
    if runtime_pin["sha256"] != args.runtime_sha256:
        raise RuntimeError("explicit exact Runtime closure required")
    if sha(MANIFEST) != MANIFEST_SHA256:
        raise RuntimeError("published complete SDK staging manifest changed")
    manifest = json.loads(MANIFEST.read_bytes())
    selected = [entry for entry in manifest["inputs"] if Path(entry["output"]) == SDK]
    if len(selected) != 1:
        raise RuntimeError("one exact materialized WASI SDK closure required")
    records = selected[0]["files"]
    if not records:
        raise RuntimeError("nonempty complete toolchain member pins required")
    c_sources = []
    for line in (inputs / "current-c-source.sha256").read_text().splitlines():
        digest, filename = line.split("  ", 1)
        relative = Path(filename).relative_to("sdk")
        if relative.is_absolute() or ".." in relative.parts:
            raise RuntimeError("confined current C source pin required")
        c_sources.append((args.sdk / relative, digest))
    if len(c_sources) != 71:
        raise RuntimeError("complete accepted71-source C snapshot required")

    def check_c_sources():
        for filename, expected in c_sources:
            if not stat.S_ISREG(filename.lstat().st_mode) or sha(filename) != expected:
                raise RuntimeError("accepted read-only C source changed: " + str(filename))

    def check_sdk():
        if sha(MANIFEST) != MANIFEST_SHA256:
            raise RuntimeError("immutable SDK staging receipt changed")
        for row in records:
            relative = Path(row["path"])
            if relative.is_absolute() or ".." in relative.parts:
                raise RuntimeError("confined toolchain member required")
            filename = SDK / relative
            if not stat.S_ISREG(filename.lstat().st_mode) or filename.stat().st_size != row["bytes"] or sha(filename) != row["sha256"]:
                raise RuntimeError("materialized toolchain member changed: " + str(relative))

    check_sdk()
    check_c_sources()
    for filename, expected in ((inputs / "reference_archive_members.py", "6ae16528f7f1957e611a968b4a4c8a0822319e79c4c6cdebf76f8bbf27ec9b79"),
                               (inputs / "check_wasm_imports.py", "11da0056e7f25766e017e516450b9a425f266ff079e6d75faa83f0b6fc12601b")):
        if sha(filename) != expected:
            raise RuntimeError("exact shared archive/Wasm reader source required")
    archive_reader = load("optional_archive_reader", inputs / "reference_archive_members.py")
    wasm = load("optional_wasm_reader", inputs / "check_wasm_imports.py")
    sysroot = SDK / "share/wasi-sysroot"
    library = sysroot / "lib/wasm32-wasip1-threads"
    builtins = SDK / "lib/clang/22/lib/wasm32-unknown-wasip1-threads/libclang_rt.builtins.a"
    closure = [library / "crt1-command.o", library / "libc.a", builtins]
    for filename in closure:
        if not filename.is_file():
            raise RuntimeError("actual optional CRT/libc/builtins closure missing")
    census = []
    for filename in closure:
        payloads = list(archive_reader.members(filename.read_bytes())) if filename.suffix == ".a" else [(filename.name, filename.read_bytes())]
        if not payloads:
            raise RuntimeError("empty required optional implementation archive")
        features = collections.Counter()
        imported = collections.Counter()
        for name, body in payloads:
            if body[:8] != b"\0asm\1\0\0\0":
                raise RuntimeError("uninspected optional implementation member: " + name)
            reader = wasm.Reader(body)
            reader.take(8)
            while reader.position < len(reader.data):
                section = reader.byte()
                part = wasm.Reader(reader.take(reader.uint()))
                if section == 13 and part.count():
                    raise RuntimeError("EH tag in a no-EH C closure")
                if section == 0 and part.name() == "target_features":
                    for unused in range(part.count()):
                        feature = chr(part.byte()) + part.name()
                        if feature == "+exception-handling":
                            raise RuntimeError("EH target feature in a no-EH C closure")
                        features[feature] += 1
                    part.end()
            for row in wasm.inspect(body):
                if row["kind"] == 4:
                    raise RuntimeError("EH tag import in a no-EH C closure")
                if row["kind"] == 0:
                    imported[(row["module"], row["name"], tuple(row["params"]), tuple(row["results"]))] += 1
        census.append({"file": pin(filename), "wasm_members": len(payloads), "target_features": dict(features),
                       "imports": [{"module": key[0], "name": key[1], "params": list(key[2]), "results": list(key[3]), "members": count} for key, count in sorted(imported.items())]})
    if not any("+atomics" in row["target_features"] for row in census):
        raise RuntimeError("genuine atomic thread library objects required")
    report = {"format": "task02-positive-optional-c-pthread-v1", "status": "RUNNING",
              "runtime": runtime_pin, "toolchain_manifest": pin(MANIFEST), "toolchain_files_rechecked_before": len(records),
              "actual_library_closure": census, "commands": [], "modes": [], "default_ros_authority": [],
              "shared_provider_or_manifest_modified": False, "public_sdk_published": False,
              "other_optional_profiles_qualified": False, "native_ros_runs": 0, "gateway_runs": 0}

    def save():
        (work / "optional-pthread.json").write_text(json.dumps(report, indent=2) + "\n")

    def execute(label, argv, expected=None):
        result = subprocess.run([str(value) for value in argv], stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=90, check=False)
        (work / (label + ".stdout")).write_bytes(result.stdout)
        (work / (label + ".stderr")).write_bytes(result.stderr)
        report["commands"].append({"label": label, "argv": [str(value) for value in argv], "exit_code": result.returncode})
        save()
        if result.returncode or (expected is not None and result.stdout != expected):
            raise RuntimeError("positive optional proof failed: " + label)
        return result.stdout

    save()
    clang, ar, nm = (SDK / "bin" / name for name in ("clang", "llvm-ar", "llvm-nm"))
    execute("compiler-version", [clang, "--version"])
    help_text = execute("runtime-help", [runtime, "--help"]) + (work / "runtime-help.stderr").read_bytes()
    if b"--wasi-threads" not in help_text:
        raise RuntimeError("explicit genuine WASI Threads Runtime entry required")
    common = ["--no-default-config", "--target=wasm32-unknown-wasip1-threads", "--sysroot=" + str(sysroot),
              "-resource-dir=" + str(SDK / "lib/clang/22"), "-fuse-ld=" + str(SDK / "bin/wasm-ld"),
              "-pthread", "-fno-exceptions", "-O2", "-g0", "-I", str(args.sdk / "include"),
              "-DROSWASM_C_ADDRESS_BITS=32", "-DROSWASM_C_EXCEPTIONS=0", "-DROSWASM_C_THREADS=1"]
    for selector, mode in MODES:
        tag = "c" + selector
        obj, archive, artifact = (work / (tag + suffix) for suffix in (".o", ".a", ".wasm"))
        flags = common + ["-std=" + mode, "-pedantic-errors", "-DROSWASM_C_STANDARD=" + selector]
        execute(tag + "-library", [clang, *flags, "-c", args.sdk / "src/platform/ros/c/library_profile.c", "-o", obj])
        execute(tag + "-archive", [ar, "rcs", archive, obj])
        symbols = execute(tag + "-symbols", [nm, "--defined-only", archive])
        canonical = "90" if selector == "89" else selector
        if ("roswasmc_library_c_profile_" + canonical + "_p32_eh0_t1").encode() not in symbols:
            raise RuntimeError("actual optional C library ABI symbol missing")
        execute(tag + "-link", [clang, *flags, inputs / "optional_pthread.c", archive,
                               "-Wl,--threads=1", "-Wl,--shared-memory", "-Wl,--import-memory",
                               "-Wl,--export=wasi_thread_start", "-Wl,--initial-memory=262144", "-Wl,--max-memory=16777216",
                               "-Wl,-z,stack-size=65536", "-Wl,-Map=" + str(work / (tag + ".map")), "-o", artifact])
        execute(tag + "-validate", ["/usr/bin/wasm-validate", "--enable-threads", artifact])
        imports = wasm.inspect(artifact.read_bytes())
        spawn = [row for row in imports if row["kind"] == 0 and row["module"] == "wasi" and row["name"] == "thread-spawn"]
        if len(spawn) != 1 or spawn[0]["params"] != ["i32"] or spawn[0]["results"] != ["i32"]:
            raise RuntimeError("actual wasm32 WASI thread-spawn ABI required")
        if any(row["kind"] == 0 and row["module"].startswith(("ros1", "ros2")) for row in imports):
            raise RuntimeError("pure pthread proof must not import ROS")
        text = execute(tag + "-inspect", ["/usr/bin/wasm-objdump", "-x", artifact]).decode()
        if not re.search(r"memory\[0\].*max=256.*shared", text) or '-> "wasi_thread_start"' not in text:
            raise RuntimeError("bounded shared memory and real WASI thread entry required")
        guest = [runtime, "--wasm-feature-enable-threads", "--wasi-threads", "1", "1", "1048576", "16777216", "8388608", "16777216",
                 "--wasip1-global-noinherit-system-environment", "--run", artifact]
        execute(tag + "-actual-pthread-guest", guest, b"c-pthread=42\n")
        report["modes"].append({"selector": selector, "actual_mode": mode, "actual_library": pin(archive),
                                "actual_guest": pin(artifact), "thread_spawn_import": spawn[0], "positive_thread_guest": True})
        save()
    check_sdk()
    check_c_sources()
    if pin(runtime) != runtime_pin or len(report["modes"]) != 8:
        raise RuntimeError("unchanged Runtime and eight positive mode outcomes required")
    report.update(status="PASS_PRIVATE_WASM32_NOEH_THREAD_COMMAND_EIGHT_C_MODES", actual_positive_thread_guests=8,
                  toolchain_files_rechecked_after=len(records), accepted_c_source_files_rechecked_before_and_after=71,
                  global_profile_acceptance=False)
    save()
    print(json.dumps({"status": report["status"], "actual_positive_thread_guests": 8}))


if __name__ == "__main__":
    try:
        main()
    except BaseException as error:
        retained = Path(os.environ.get("UWVM_RESOURCE_RUN", "/nonexistent")) / "work/optional-pthread.json"
        if retained.is_file():
            partial = json.loads(retained.read_bytes())
            partial.update(status="FAILED_RETAINED", error=repr(error))
            retained.write_text(json.dumps(partial, indent=2) + "\n")
        raise
