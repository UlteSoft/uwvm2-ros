#!/usr/bin/env python3
"""Qualify actual AOT cache hits and WASI imports using fresh VM processes.

@details Run on Linux inside the task's resource cgroup. Compile the authored
guests with a real Wasm compiler. Capture pipes rather than exposing log files
as guest standard descriptors. A directory, successful exit, or short duration
alone never counts as a cache hit. Preserve every command and failure.
"""

import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--uwvm", required=True, type=Path)
    parser.add_argument("--clang", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--cache-root", type=Path,
                        help="Fresh owner-controlled cache parent, separate from receipt storage")
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=False)
    cache_root = args.cache_root or args.out
    if args.cache_root is not None:
        cache_root.mkdir(parents=True, exist_ok=False, mode=0o700)
    source = Path(__file__).resolve().parent
    report = {"passed": False, "commands": [], "warm_hits": 0,
              "cache_root": str(cache_root),
              "runtime_sha256": hashlib.sha256(args.uwvm.read_bytes()).hexdigest()}

    def run(name, argv):
        started = time.monotonic()
        result = subprocess.run([str(item) for item in argv], stdin=subprocess.DEVNULL,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=180)
        row = {"name": name, "argv": [str(item) for item in argv],
               "returncode": result.returncode, "seconds": time.monotonic() - started}
        report["commands"].append(row)
        for stream in ("stdout", "stderr"):
            content = getattr(result, stream)
            (args.out / f"{name}.{stream}").write_bytes(content)
            row[stream + "_sha256"] = hashlib.sha256(content).hexdigest()
        return result

    try:
        guests = {}
        for name in ("pure", "wasi"):
            guests[name] = args.out / (name + ".wasm")
            result = run("compile-" + name, [args.clang, "--target=wasm32-unknown-unknown",
                         "-O1", "-g", "-nostdlib", "-Wl,--allow-undefined", "-Wl,--export=_start",
                         "-Wl,--entry=_start", source / (name + "_guest.c"), "-o", guests[name]])
            assert result.returncode == 0, "real Wasm guest compilation failed"
            assert guests[name].read_bytes()[:8] == b"\0asm\1\0\0\0"

        base = [args.uwvm, "--runtime-aot", "-Rct", "0", "-Rllvm-policy", "max",
                "-Rllvm-call-stack", "unwind", "--log-verbose", "--runtime-compiler-log", "err"]
        def cache_case():
            unsealed_dir = cache_root / "unsealed-cache"
            unsealed = run("unsealed", [*base, "-Rllvm-cache-path", "path", unsealed_dir,
                                        "--run", guests["pure"]])
            assert unsealed.returncode == 0, "unsealed cache refusal must not fail the guest"
            assert b"status=process-not-sealed" in unsealed.stderr, "missing explicit cache refusal"
            assert b"object-cache-hit " not in unsealed.stderr

            cache_rounds("sealed", guests["pure"])

        def cache_rounds(prefix, guest, flags=(), expected_output=None):
            """Require completed cold storage and verified hits in fresh processes."""
            cache_dir = cache_root / (prefix + "-cache")
            warm = [*base, "--runtime-llvm-jit-cache-seal-process", "-Rllvm-cache-path",
                    "path", cache_dir, *flags, "--run", guest]
            cold = run(prefix + "-cold", warm)
            assert cold.returncode == 0 and (expected_output is None or cold.stdout == expected_output)
            assert re.search(rb"object-cache-store-complete [^\n]*status=ok\b", cold.stderr), "cold cache store not completed"
            assert any(path.is_file() for path in cache_dir.rglob("*")), "cache directory contains no stored object"
            report.setdefault("stored_files", {})[prefix] = [
                {"path": str(path.relative_to(cache_dir)), "bytes": path.stat().st_size,
                 "sha256": hashlib.sha256(path.read_bytes()).hexdigest()}
                for path in sorted(cache_dir.rglob("*")) if path.is_file()
            ]
            for number in range(2):
                result = run(f"{prefix}-warm-{number}", warm)
                hits = re.findall(rb"object-cache-hit [^\n]*signature_verified=1\b", result.stderr)
                assert result.returncode == 0 and hits and (expected_output is None or result.stdout == expected_output), "fresh process did not verify a real cache hit"
                assert b"object-cache-store-enqueue " not in result.stderr, "warm process recompiled an object"
                report["warm_hits"] += len(hits)
            return cache_dir


        def wasi_case():
            wasi_base = [*base, "-Rllvm-cache-path", "disable",
                         "--wasip1-global-noinherit-system-environment"]
            wasi = run("aot-wasi-granted", [*wasi_base, "--wasip1-global-grant", "monotonic-clock",
                                           "--run", guests["wasi"]])
            assert wasi.returncode == 0 and wasi.stdout == b"AOT-WASI-OK\n", "AOT did not execute actual WASI imports"
            denied = run("aot-wasi-clock-denied", [*wasi_base, "--run", guests["wasi"]])
            assert denied.returncode == 0 and denied.stdout == b"AOT-WASI-DENIED:76\n", "missing actual WASI capability refusal"

        def wasi_cache_case():
            flags = ("--wasip1-global-noinherit-system-environment", "--wasip1-global-grant", "monotonic-clock")
            cache_dir = cache_rounds("wasi-sealed", guests["wasi"], flags, b"AOT-WASI-OK\n")
            # A cached object never grants a subsequent process the first
            # process's clock permission. A changed policy may miss the cache.
            denied = run("aot-wasi-cached-clock-denied", [*base, "--runtime-llvm-jit-cache-seal-process",
                         "-Rllvm-cache-path", "path", cache_dir,
                         "--wasip1-global-noinherit-system-environment", "--run", guests["wasi"]])
            assert denied.returncode == 0 and denied.stdout == b"AOT-WASI-DENIED:76\n", "cached code retained a prior clock grant"

        def ros_import_case():
            namespaces = ("uwvm_ros1_v1", "uwvm_ros1_client_v1", "uwvm_ros2_v1",
                          "uwvm_ros2_client_v1", "uwvm_ros2_rmw_v1",
                          "uwvm_ros2_rmw_sub_v1", "uwvm_ros2_rmw_srv_v1")
            flags = [args.clang, "--target=wasm32-unknown-unknown", "-O1", "-nostdlib",
                     "-Wl,--allow-undefined", "-Wl,--export=_start", "-Wl,--entry=_start"]
            for namespace in namespaces:
                guest = args.out / (namespace + ".wasm")
                result = run("compile-" + namespace, [*flags, '-DTEST_MODULE="' + namespace + '"',
                             source / "ros_import_guest.c", "-o", guest])
                assert result.returncode == 0
                result = run("import-" + namespace, [*base, "-Rllvm-cache-path", "disable", "--run", guest])
                assert result.returncode == 0, "referenced ROS namespace did not load"
            alias = args.out / "alias.wasm"
            result = run("compile-alias", [*flags, '-DTEST_MODULE="test-alias"',
                         source / "ros_import_guest.c", "-o", alias])
            assert result.returncode == 0
            rewrite = ["--wasm-reset-import", "alias.wasm", "test-alias", "abi_version", "uwvm_ros2_v1", "abi_version"]
            result = run("import-rewrite", [*base, "-Rllvm-cache-path", "disable", *rewrite, "--run", alias])
            assert result.returncode == 0, "rewritten ROS namespace did not load"
            caller = args.out / "preload-caller.wasm"
            result = run("compile-preload-caller", [*flags, source / "ros_preload_caller.c", "-o", caller])
            assert result.returncode == 0
            result = run("import-preload", [*base, "-Rllvm-cache-path", "disable",
                         "--wasm-preload-library", args.out / "uwvm_ros2_v1.wasm", "ros-library", "--run", caller])
            assert result.returncode == 0, "ROS namespace required only by preload did not load"

        report["cases"] = {}
        for name, case in (("cache", cache_case), ("wasi", wasi_case), ("wasi-cache", wasi_cache_case), ("ros-import", ros_import_case)):
            try:
                case()
                report["cases"][name] = {"passed": True}
            except BaseException as error:
                report["cases"][name] = {"passed": False, "failure": repr(error)}
        assert all(row["passed"] for row in report["cases"].values()), "One or more runtime cases failed"
        report["passed"] = True
    except BaseException as error:
        report["failure"] = repr(error)
        raise
    finally:
        (args.out / "result.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
