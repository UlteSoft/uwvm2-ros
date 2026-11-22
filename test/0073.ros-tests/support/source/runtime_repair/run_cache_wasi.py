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
    args.out.mkdir(parents=True, exist_ok=False, mode=0o700)
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

        base = [args.uwvm, "--runtime-aot", "-Rct", "0", "--wasm-set-main-module-name", "main",
                "--log-verbose", "--runtime-compiler-log", "err"]
        wasi_flags = ["--wasip1-global-noinherit-system-environment", "--wasip1-global-enable",
                      "--wasip1-global-interface", "fd_write", "allow",
                      "--wasip1-global-interface", "clock_time_get", "allow",
                      "--wasip1-global-grant", "stdout.write"]
        clock_flags = [*wasi_flags, "--wasip1-global-grant", "monotonic-clock"]

        def cache_command(guest, cache_dir, flags=(), policy="max"):
            return [*base, "-Rllvm-policy", policy, "--runtime-llvm-jit-cache-seal-process",
                    "-Rllvm-cache-path", "path", cache_dir, *flags, "--run", guest]

        def verify_hit(name, command, expected_output):
            result = run(name, command)
            hits = re.findall(rb"object-cache-hit [^\n]*signature_verified=1\b", result.stderr)
            assert result.returncode == 0 and hits and result.stdout == expected_output, name
            assert b"object-cache-store-enqueue " not in result.stderr, "warm process recompiled an object"
            report["warm_hits"] += len(hits)

        def cache_case():
            unsealed_dir = cache_root / "unsealed-cache"
            unsealed = run("unsealed", [*base, "-Rllvm-cache-path", "path", unsealed_dir,
                                        "--run", guests["pure"]])
            assert unsealed.returncode == 0, "unsealed cache refusal must not fail the guest"
            assert b"status=process-not-sealed" in unsealed.stderr, "missing explicit cache refusal"
            assert b"object-cache-hit " not in unsealed.stderr

            cache_rounds("sealed", guests["pure"])

        def cache_rounds(prefix, guest, flags=(), expected_output=b"", *, cache_dir=None, policy="max"):
            """Require completed cold storage and verified hits in fresh processes."""
            if cache_dir is None:
                cache_dir = cache_root / (prefix + "-cache")
            warm = cache_command(guest, cache_dir, flags, policy)
            cold = run(prefix + "-cold", warm)
            assert cold.returncode == 0 and cold.stdout == expected_output
            assert b"object-cache-hit " not in cold.stderr, "changed fixture unexpectedly reused cached code"
            assert re.search(rb"object-cache-store-complete [^\n]*status=ok\b", cold.stderr), "cold cache store not completed"
            objects = sorted(cache_dir.rglob("*.uwvm-ljc"))
            assert objects, "cache directory contains no stored object"
            report.setdefault("stored_files", {})[prefix] = [
                {"path": str(path.relative_to(cache_dir)), "bytes": path.stat().st_size,
                 "sha256": hashlib.sha256(path.read_bytes()).hexdigest()}
                for path in objects
            ]
            for number in range(2):
                verify_hit(f"{prefix}-warm-{number}", warm, expected_output)
            return cache_dir


        def wasi_case():
            wasi_base = [*base, "-Rllvm-cache-path", "disable", *wasi_flags]
            wasi = run("aot-wasi-granted", [*wasi_base, "--wasip1-global-grant", "monotonic-clock",
                                           "--run", guests["wasi"]])
            assert wasi.returncode == 0 and wasi.stdout == b"AOT-WASI-OK\n", "AOT did not execute actual WASI imports"
            denied = run("aot-wasi-clock-denied", [*wasi_base, "--run", guests["wasi"]])
            assert denied.returncode == 0 and denied.stdout == b"AOT-WASI-DENIED:76\n", "missing actual WASI capability refusal"

        def wasi_cache_case():
            guest = guests["wasi"]
            output = b"AOT-WASI-OK\n"
            cache_dir = cache_rounds("wasi-sealed", guest, clock_flags, output)
            original = cache_command(guest, cache_dir, clock_flags)
            verify_hit("aot-wasi-cached-clock-denied", cache_command(guest, cache_dir, wasi_flags),
                       b"AOT-WASI-DENIED:76\n")
            hidden = run("aot-wasi-cached-hidden", cache_command(guest, cache_dir))
            assert hidden.returncode == 127 and not hidden.stdout and b"namespace permission is disabled" in hidden.stderr
            assert b"object-cache-hit " not in hidden.stderr and b"object-cache-store-" not in hidden.stderr
            cache_rounds("wasi-fast", guest, clock_flags, output, cache_dir=cache_dir, policy="fast-compile")
            verify_hit("wasi-original-policy", original, output)
            changed = args.out / "wasi-double-output.wasm"
            result = run("compile-wasi-double-output", [args.clang, "--target=wasm32-unknown-unknown",
                         "-O1", "-g", "-nostdlib", "-DTEST_OUTPUT_REPETITIONS=2", "-Wl,--allow-undefined",
                         "-Wl,--export=_start", "-Wl,--entry=_start", source / "wasi_guest.c", "-o", changed])
            assert result.returncode == 0
            cache_rounds("wasi-changed-code", changed, clock_flags, output * 2, cache_dir=cache_dir)
            verify_hit("wasi-original-code", original, output)

        def ros_import_case():
            help_result = run("product-help", [args.uwvm, "--help", "all"])
            assert help_result.returncode == 0
            help_text = help_result.stdout + help_result.stderr
            namespaces = []
            if b"--ros1-single-interface" in help_text:
                namespaces.extend(["uwvm_ros1_v1", "uwvm_ros1_client_v1"])
            if b"--ros2-single-interface" in help_text:
                namespaces.extend(["uwvm_ros2_v1", "uwvm_ros2_client_v1", "uwvm_ros2_rmw_v1",
                                   "uwvm_ros2_rmw_sub_v1", "uwvm_ros2_rmw_srv_v1"])
            if b"--ros-scene-interface" in help_text:
                namespaces.append("uwvm_ros_v2")
            assert namespaces, "the product has no ROS provider to exercise"
            report["ros_namespaces"] = namespaces
            flags = [args.clang, "--target=wasm32-unknown-unknown", "-O1", "-nostdlib",
                     "-Wl,--allow-undefined", "-Wl,--export=_start", "-Wl,--entry=_start"]

            def exposure(namespace, module):
                if namespace == "uwvm_ros_v2":
                    return ["--ros-scene-create", "robot", "--ros-scene-bind", "robot", module,
                            "--ros-scene-interface", "robot", "abi", "allow"]
                ros = "--ros1" if namespace.startswith("uwvm_ros1_") else "--ros2"
                return [ros + "-single-create", module, ros + "-single-enable", module,
                        ros + "-single-interface", module, namespace + ".abi_version", "allow"]

            def compile_abi(name, namespace, output, *, scene=False, nondelegated=False):
                defines = ['-DTEST_MODULE="' + namespace + '"']
                if scene:
                    defines.append("-DTEST_SCENE_ABI")
                if nondelegated:
                    defines.append("-DTEST_NONDELEGATED_SCENE_CALL")
                result = run("compile-" + name, [*flags, *defines, source / "ros_import_guest.c", "-o", output])
                assert result.returncode == 0

            ros_base = [*base, "-Rllvm-cache-path", "disable"]
            for namespace in namespaces:
                guest = args.out / (namespace + ".wasm")
                compile_abi(namespace, namespace, guest, scene=namespace == "uwvm_ros_v2")
                result = run("import-" + namespace, [*ros_base, *exposure(namespace, "main"), "--run", guest])
                assert result.returncode == 0, "referenced ROS namespace did not load"
            # The scene provider is the default build. Legacy-only builds use
            # a retained namespace selected from their actual registered flags.
            selected = namespaces[-1]
            scene = selected == "uwvm_ros_v2"
            interface = "abi" if scene else "abi_version"
            alias = args.out / "alias.wasm"
            compile_abi("alias", "test-alias", alias, scene=scene)
            rewrite = ["--wasm-reset-import", "main", "test-alias", interface, selected, interface]
            result = run("import-rewrite", [*ros_base, *exposure(selected, "main"), *rewrite, "--run", alias])
            assert result.returncode == 0, "rewritten ROS namespace did not load"
            caller = args.out / "preload-caller.wasm"
            library = args.out / (selected + ".wasm")
            if scene:
                # The linked library owns the exposed import. An ordinary
                # cross-module call does not delegate a scene principal.
                library = args.out / "preload-scene.wasm"
                compile_abi("preload-scene", selected, library, scene=True, nondelegated=True)
            result = run("compile-preload-caller", [*flags, source / "ros_preload_caller.c", "-o", caller])
            assert result.returncode == 0
            result = run("import-preload", [*ros_base, *exposure(selected, "ros-library"),
                         "--wasm-preload-library", library, "ros-library", "--run", caller])
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
