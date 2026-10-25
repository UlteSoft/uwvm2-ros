#!/usr/bin/env python3
"""@brief Check the SDK's public launch arguments against the actual ROS VM.

@details A real Wasm command writes through its explicitly granted standard
stream, observes a denied clock and observes a disabled ROS module. Both ROS
families must run through checked native unwind. No compiler, ROS installation,
gateway, physical endpoint or implicit permission is needed for this probe.
"""

import argparse
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys


def load(path, name):
    """@brief Load the explicitly selected source module without bytecode files."""
    specification = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(specification)
    specification.loader.exec_module(module)
    return module


def guest(codec, backend):
    """@brief Encode independent expected errno and output checks as real Wasm."""
    i32, i64 = 0x7f, 0x7e
    types = [codec.function_type([i32] * 4, [i32]),
             codec.function_type([i32, i64, i32], [i32]),
             codec.function_type([i32, i64, i64, i64], [i32]),
             codec.function_type([], [])]
    imports = [codec.name("wasi_snapshot_preview1") + codec.name("fd_write") + b"\x00\x00",
               codec.name("wasi_snapshot_preview1") + codec.name("clock_time_get") + b"\x00\x01",
               codec.name(f"uwvm_{backend}_v1") + codec.name("capability_open") + b"\x00\x02"]
    # No clock grant: clock_time_get(monotonic, 0, 64) must return NOTCAPABLE.
    instructions = b"\x41\x01\x42\x00\x41\xc0\x00\x10\x01\x41\xcc\x00\x47\x04\x40\x00\x0b"
    # A well-formed, ungranted alias reaches policy denial rather than EINVAL.
    instructions += b"\x41\x00\x42\xc0\x00\x42\x05\x42\x10\x10\x02\x41\x0d\x47\x04\x40\x00\x0b"
    instructions += b"\x41\x01\x41\x00\x41\x01\x41\x10\x10\x00\x04\x40\x00\x0b"
    message = b"SDK_UNWIND_POLICY_OK\n"
    data = (32).to_bytes(4, "little") + len(message).to_bytes(4, "little")
    data = (data.ljust(32, b"\x00") + message).ljust(64, b"\x00") + b"probe"
    result = b"\x00asm\x01\x00\x00\x00"
    result += codec.section(1, codec.vector(types))
    result += codec.section(2, codec.vector(imports))
    result += codec.section(3, b"\x01\x03")
    result += codec.section(5, b"\x01\x01\x01\x01")
    result += codec.section(7, codec.vector([codec.name("_start") + b"\x00\x03",
                                           codec.name("memory") + b"\x02\x00"]))
    result += codec.section(10, codec.vector([codec.body(instructions)]))
    result += codec.section(11, codec.vector([b"\x00\x41\x00\x0b" + codec.uleb(len(data)) + data]))
    return result, message


def main():
    """@brief Require both real backend launches and preserve their diagnostics."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdk-root", type=Path, required=True)
    parser.add_argument("--uwvm", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != "linux":
        parser.error("the product launch probes require Linux")
    sys.dont_write_bytecode = True
    sys.path.insert(0, str(args.sdk_root / "tools/platform/ros"))
    deployment = load(args.sdk_root / "tools/platform/ros/deployment.py", "sdk_deployment")
    runner = load(args.sdk_root / "tools/platform/ros/run.py", "sdk_runner")
    codec = load(Path(__file__).resolve().parents[3] / "basic_test.py", "basic_codec")
    args.out.mkdir(parents=True, exist_ok=False)
    runtime = str(args.uwvm.resolve(strict=True))
    report = {"passed": False, "checks": []}
    try:
        for backend in ("ros1", "ros2"):
            binary, expected = guest(codec, backend)
            path = (args.out / (backend + ".wasm")).resolve()
            path.write_bytes(binary)
            plan = {"backend": backend, "main_module": "main", "features": [],
                    "preloads": [], "arguments": [], "artifacts": {"main": {"path": str(path)}},
                    "modules": {"main": {"ros_enabled": False, "wasi_grants": ["stdio-full-rights"]}}}
            prefix = [runtime, "--log-verbose", "--runtime-compiler-log", "err", "-Rct", "0",
                      "--runtime-llvm-jit-cache-path", "disable"]
            argv = deployment.command(plan, prefix, {}, os.getuid())
            row = {"backend": backend, "argv": argv}
            report["checks"].append(row)
            result = subprocess.run(argv, stdin=subprocess.DEVNULL, capture_output=True, timeout=60)
            (args.out / (backend + ".stdout")).write_bytes(result.stdout)
            (args.out / (backend + ".stderr")).write_bytes(result.stderr)
            row["exit"] = result.returncode
            row["passed"] = (result.returncode == 0 and result.stdout == expected and
                             b"call_stack=unwind " in result.stderr and b"unwind_check=live" in result.stderr)
            if not row["passed"]:
                raise RuntimeError(f"{backend}: SDK launch or native authority check failed")
        # Exercise the complete public runner, including executable discovery,
        # the stopped-worker handshake and evidence output. Disabled ROS must
        # not require an unused gateway or the user's sourced ROS distribution.
        previous_distribution = os.environ.get("ROS_DISTRO")
        try:
            for backend, distribution in (("ros1", "noetic"), ("ros2", "jazzy")):
                connection = (
                    {"node-name": "sdk_probe", "namespace": "/",
                     "master-uri": "http://127.0.0.1:11311",
                     "advertise-host": "127.0.0.1", "transport": "tcpros"}
                    if backend == "ros1" else
                    {"node-name": "sdk_probe", "namespace": "/", "domain-id": "219",
                     "rmw-implementation": "rmw_fastrtps_cpp", "security-mode": "disabled-test",
                     "keystore": "not-applicable", "enclave": "not-applicable"}
                )
                scenario = {"arguments": [], "main_module": "main", "preloads": [],
                            "modules": {"main": {"ros_enabled": False, "grants": [],
                                                "wasi_grants": ["stdio-full-rights"]}}}
                config = {"format": deployment.FORMAT, "runtime": runtime,
                          "backend": backend, "distribution": distribution,
                          "connection": connection, "examples": {"probe": scenario}}
                wasm = (args.out / (backend + ".wasm")).resolve()
                for supplied in (False, True):
                    label = backend + ("-unused-gateway" if supplied else "-no-gateway")
                    if supplied:
                        config["gateway"] = str(args.out / "deliberately-absent-gateway")
                    else:
                        config.pop("gateway", None)
                    configuration = args.out / (label + ".json")
                    configuration.write_text(json.dumps(config) + "\n")
                    destination = args.out / label
                    os.environ["ROS_DISTRO"] = "unrelated-sourced-distribution"
                    code = runner.run(configuration, "probe", wasm, destination,
                                      "wasm32-wasip1", backend, distribution)
                    records = list(destination.glob("*/execution.json"))
                    assert code == 0 and len(records) == 1, label
                    record = json.loads(records[0].read_text())
                    assert record["gateway"] is None and record["gateway_sha256"] is None
                    assert record["gateway_pids"] == {} and record["status"] == "PASS"
                    assert "--runtime-aot" in record["argv"] and "unwind" in record["argv"]
                    assert (records[0].parent / "worker.stdout").read_bytes() == expected
                    report["checks"].append({"case": label, "passed": True,
                                              "execution": str(records[0])})
                # Any enabled preload requires a gateway even if main has no
                # ROS authority. This refusal must precede process creation.
                config.pop("gateway", None)
                scenario["preloads"] = [{"module": "observer", "path": str(wasm), "attribute": "none"}]
                scenario["modules"]["observer"] = {"ros_enabled": True, "grants": [], "wasi_grants": []}
                configuration = args.out / (backend + "-enabled-preload.json")
                configuration.write_text(json.dumps(config) + "\n")
                destination = args.out / (backend + "-enabled-preload")
                os.environ["ROS_DISTRO"] = distribution
                try:
                    runner.run(configuration, "probe", wasm, destination,
                               "wasm32-wasip1", backend, distribution)
                except ValueError as error:
                    assert str(error) == "A ROS-enabled module requires a gateway executable"
                else:
                    raise AssertionError("An enabled preload bypassed gateway admission")
                assert not destination.exists(), "A refused deployment started a worker"
                report["checks"].append({"case": backend + "-enabled-preload-refused", "passed": True})
        finally:
            if previous_distribution is None:
                os.environ.pop("ROS_DISTRO", None)
            else:
                os.environ["ROS_DISTRO"] = previous_distribution
        report["passed"] = True
    finally:
        (args.out / "report.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
