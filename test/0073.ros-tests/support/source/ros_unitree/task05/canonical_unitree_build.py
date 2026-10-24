#!/usr/bin/env python3
"""Compile the one canonical Unitree C/C++ SDK with actual native/WASI tools.

Run only inside the original finite Linux owner and common lock. This builds
the actual CMake targets from an explicit installed-layout input snapshot. It
does not generate schemas, supply JSON/native ROS providers, grant authority,
or qualify the other language ports. Input snapshots are temporary build data.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import os
import subprocess


def fact(path):
    with path.open("rb") as stream:
        digest = hashlib.file_digest(stream, "sha256").hexdigest()
    return {"bytes": path.stat().st_size, "sha256": digest}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("sdk", "tests", "fast-io", "wasi", "runtime", "validator"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("the original finite owner/common lock is required")
    work = Path.cwd()
    sdk = args.sdk.resolve(strict=True)
    tests = args.tests.resolve(strict=True)
    source = sdk / "src/platform/unitree"
    captured = {}
    commands = []
    products = []
    result = {"format": "task05-canonical-unitree-cmake-v1", "status": "RUNNING",
              "input_sources": captured, "commands": commands, "products": products,
              "native_value_runs": 0, "Wasm_value_Guest_runs": 0,
              "actual_xmake_builds": 0, "native_ROS_runs": 0,
              "query_JSON_provider_built": False, "other_languages_qualified": 0,
              "default_authority": []}

    def observe(path):
        path = path.absolute()
        if not path.is_file():
            raise ValueError("actual ordinary input required: " + str(path))
        captured[str(path)] = fact(path)
        return path

    def run(name, argv):
        out = work / (name + ".stdout")
        err = work / (name + ".stderr")
        with out.open("xb") as stdout, err.open("xb") as stderr:
            process = subprocess.run(list(map(str, argv)), stdout=stdout, stderr=stderr,
                                     check=False, cwd=work)
        commands.append({"name": name, "argv": list(map(str, argv)),
                         "exit": process.returncode, "stdout": fact(out), "stderr": fact(err)})
        if process.returncode:
            raise ValueError(name + " failed; actual diagnostics retained")
        return out.read_bytes()

    try:
        for base in (sdk, tests):
            for path in sorted(base.rglob("*")):
                if path.is_symlink():
                    raise ValueError("canonical input snapshot cannot contain symlinks")
                if path.is_file():
                    observe(path)
        for path in (Path("/usr/bin/cmake"), Path("/usr/bin/ninja"),
                     Path("/usr/bin/clang-21"), Path("/usr/bin/clang++-21"),
                     args.wasi / "bin/clang", args.wasi / "bin/clang++",
                     args.wasi / "share/cmake/wasi-sdk.cmake", args.runtime, args.validator,
                     args.fast_io / "fast_io.h", args.fast_io / "fast_io_unit/floating/impl.h"):
            observe(path)
        checker = sdk / "tools/check_wasm_imports.py"
        spec = importlib.util.spec_from_file_location("task05_canonical_actual_inspector", checker)
        inspector = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(inspector)
        targets = ("unitree_sport_values_c", "unitree_sport_values_cpp",
                   "unitree_b2_sport_values_c", "unitree_b2_sport_values_cpp",
                   "unitree_packet_contract")
        applications = ("unitree_sport_operations", "unitree_sport_roundtrip",
                        "unitree_b2_sport_command")
        for mode in ("native", "wasm"):
            build = work / mode
            options = ["-DUWVM_UNITREE_BUILD_SPORT=ON", "-DUWVM_UNITREE_BUILD_B2_SPORT=ON",
                       "-DUWVM_UNITREE_BUILD_PACKET_BRIDGE=ON",
                       "-DUWVM_UNITREE_BUILD_SPORT_CONTRACTS=ON",
                       "-DUWVM_UNITREE_BUILD_PACKET_JSON=OFF",
                       "-DUWVM_UNITREE_TEST_ROOT=" + str(tests),
                       "-DUWVM_UNITREE_FAST_IO=" + str(args.fast_io),
                       "-DCMAKE_BUILD_TYPE=Release", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"]
            if mode == "native":
                options += ["-DCMAKE_C_COMPILER=/usr/bin/clang-21",
                            "-DCMAKE_CXX_COMPILER=/usr/bin/clang++-21"]
            else:
                options += ["-DCMAKE_TOOLCHAIN_FILE=" + str(args.wasi / "share/cmake/wasi-sdk.cmake"),
                            "-DWASI_SDK_PREFIX=" + str(args.wasi),
                            "-DUWVM_UNITREE_BUILD_SPORT_EXAMPLE=ON",
                            "-DUWVM_UNITREE_BUILD_SPORT_ROUNDTRIP=ON",
                            "-DUWVM_UNITREE_BUILD_B2_SPORT_EXAMPLE=ON"]
            run(mode + "-configure", ["/usr/bin/cmake", "-S", source, "-B", build,
                                      "-G", "Ninja", *options])
            run(mode + "-build", ["/usr/bin/cmake", "--build", build, "--parallel", "1"])
            compilation = json.loads((build / "compile_commands.json").read_bytes())
            if any("sdk/origin/" in row["command"] or "/canonical-include/" in row["command"]
                   or "sdk/lib/" in row["command"] for row in compilation):
                raise ValueError("actual compilation selected a retired SDK source path")
            for name in (*targets, *(applications if mode == "wasm" else ())):
                binary = build / (name + (".wasm" if mode == "wasm" else ""))
                row = {"mode": mode, "target": name, "artifact": str(binary), **fact(binary)}
                products.append(row)
                if mode == "wasm":
                    run(name + "-validate", [args.validator, "validate", binary])
                    body = binary.read_bytes()
                    imports = inspector.inspect(body)
                    if any(item["kind"] == 2 for item in imports):
                        raise ValueError("the command profile cannot import memory")
                    reader = inspector.Reader(body)
                    reader.take(8)
                    memories = []
                    while reader.position < len(reader.data):
                        kind = reader.byte()
                        section = inspector.Reader(reader.take(reader.uint()))
                        if kind == 5:
                            if section.uint() != 1 or section.uint() != 1:
                                raise ValueError("one owned nonshared bounded memory32 is required")
                            memories.append({"initial_pages": section.uint(),
                                             "maximum_pages": section.uint()})
                            if section.position != len(section.data):
                                raise ValueError("trailing memory bytes")
                    if memories != [{"initial_pages": 256, "maximum_pages": 1024}]:
                        raise ValueError("actual CMake product memory differs from its SDK profile")
                    row.update(owned_memory=memories[0], imports=imports,
                               stack_bytes_from_executed_CMake_profile=1048576,
                               stack_size_binary_proof=False)
                    if name in targets:
                        output = run(name + "-Guest", [args.runtime, "--run", binary])
                        if name == "unitree_packet_contract" and \
                           b"UNITREE_PACKET_LOCAL_OK operations=60 fixed_slots=16" not in output:
                            raise ValueError("actual complete packet assertions did not run")
                        result["Wasm_value_Guest_runs"] += 1
                else:
                    run(name + "-native", [binary])
                    result["native_value_runs"] += 1
        for path, before in captured.items():
            if fact(Path(path)) != before:
                raise ValueError("canonical input/provider changed: " + path)
        result["status"] = "PASS_CANONICAL_CMAKE_NATIVE_AND_WASM_LOCAL_VALUES"
    except BaseException as error:
        result.update(status="FAILED_RETAINED", error=repr(error))
        raise
    finally:
        (work / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    print(result["status"])


if __name__ == "__main__":
    main()
