"""Measure native ROS, authenticated IPC, and checked-unwind Wasm on real Linux.

@file run_ros_service_benchmark.py
@brief Compare identical native Trigger exchanges and retain every raw result.
@details The caller owns the global test lease. This driver verifies its actual
64 GiB, zero-swap cgroup and pins the process tree to one physical CPU. Native
requests, response bytes, warm-up counts and peer callback counts must match.
An optional paused VTune collection resumes only after discovery, compilation,
capability binding and warm-up; instrumented durations are explicitly excluded
from the uninstrumented latency cohort. No robot or external ROS graph is used.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import random
import re
import signal
import socket as socket_module
import statistics
import subprocess
import tempfile
import time
from ros_resource_cases import CASES as RESOURCE_CASES, configuration as resource_configuration


def main():
    """Run a bounded, source-selected comparison and preserve failures verbatim."""
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("peer", "gateway", "uwvm", "wasm", "out", "cache"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--count", type=int, default=200)
    parser.add_argument("--call-timeout-ms", type=int, default=3000)
    parser.add_argument("--ros-version", type=int, choices=(1, 2), default=2)
    parser.add_argument("--operation", choices=("trigger", "time-ros", "time-wall", "time-system", "time-steady", *RESOURCE_CASES), default="trigger")
    parser.add_argument("--rounds", type=int, default=9)
    parser.add_argument("--warmup-rounds", type=int, default=2)
    parser.add_argument("--modes", default="native,ipc,wasm")
    parser.add_argument("--vtune", type=Path)
    parser.add_argument("--profile-result", type=Path)
    parser.add_argument("--runtime-loader", type=Path)
    parser.add_argument("--runtime-library-path")
    parser.add_argument("--cache-mode", choices=("signed", "disabled"), default="signed")
    for name in ("peer", "gateway", "uwvm"):
        parser.add_argument("--baseline-" + name, type=Path)
    args = parser.parse_args()
    assert args.operation != "time-wall" or args.ros_version == 1
    assert args.operation not in ("time-system", "time-steady") or args.ros_version == 2
    assert bool(args.runtime_loader) == bool(args.runtime_library_path)
    if args.runtime_loader:
        assert args.cache_mode == "disabled", "an explicit ELF loader does not qualify executable cache identity"
    assert 1 <= args.count <= 1000000 and 1 <= args.rounds <= 30
    assert 1 <= args.call_timeout_ms <= 10000
    assert 0 <= args.warmup_rounds < args.rounds
    modes = args.modes.split(",")
    assert len(set(modes)) == len(modes) and set(modes) <= {"native", "ipc", "wasm", "baseline-ipc", "baseline-wasm"}
    if any(mode.startswith("baseline-") for mode in modes):
        assert all((args.baseline_peer, args.baseline_gateway, args.baseline_uwvm))
        assert args.cache_mode == "disabled", "paired interface timing excludes cache admission and compilation"
    assert not args.profile_result or (args.vtune and len(modes) == 1 and args.rounds == 1)
    assert platform.system() == "Linux" and platform.machine() == "x86_64"
    assert "qemu" not in str(Path("/proc/self/exe").resolve()).lower()
    args.out.mkdir(parents=True, exist_ok=False)
    report = {"status": "PREFLIGHT", "commands": [], "samples": [], "started": time.time(),
              "instrumented": bool(args.profile_result), "count": args.count, "cache_mode": args.cache_mode,
              "warmup_per_launch": 20, "warmup_rounds": args.warmup_rounds, "ros_version": args.ros_version,
              "operation": args.operation}
    children = []
    server = None
    sockets = None

    def persist():
        (args.out / "report.json").write_text(json.dumps(report, indent=2) + "\n")

    def start(label, argv, stdin=None):
        """Start only owned children with separate bounded-lifetime process groups."""
        command = [str(item) for item in argv]
        with (args.out / (label + ".log")).open("wb") as log:
            child = subprocess.Popen(command, env=env, stdin=stdin, stdout=log,
                                     stderr=subprocess.STDOUT, start_new_session=True)
        children.append(child)
        report["commands"].append({"label": label, "argv": command, "pid": child.pid})
        persist()
        return child

    def wait_for(predicate, child, seconds=30):
        """Require positive readiness, preserving child failure and timeout separately."""
        end = time.monotonic() + seconds
        while not predicate():
            if child.poll() is not None:
                raise RuntimeError(f"Readiness failed: pid={child.pid} exit={child.returncode}")
            if time.monotonic() >= end:
                raise TimeoutError(f"Readiness deadline: pid={child.pid}")
            time.sleep(0.005)

    def stop(child):
        if child.poll() is None:
            os.killpg(child.pid, signal.SIGINT if args.ros_version == 1 else signal.SIGTERM)
            os.killpg(child.pid, signal.SIGCONT)
            try:
                child.wait(timeout=5)
            except subprocess.TimeoutExpired:
                os.killpg(child.pid, signal.SIGKILL)
                child.wait(timeout=5)

    def profiler(command):
        if args.profile_result:
            subprocess.run([str(args.vtune), "-command", command, "-result-dir", str(args.profile_result)],
                           check=True, timeout=30, stdout=subprocess.DEVNULL)

    try:
        cg = Path("/sys/fs/cgroup")
        report["cgroup"] = {name: (cg / name).read_text().strip()
                            for name in ("memory.max", "memory.swap.max", "cgroup.procs", "cpu.max", "cpu.stat")}
        assert report["cgroup"]["memory.max"] == "68719476736"
        assert report["cgroup"]["memory.swap.max"] == "0"
        assert str(os.getpid()) in report["cgroup"]["cgroup.procs"].split()
        cpu = min(os.sched_getaffinity(0))
        os.sched_setaffinity(0, {cpu})
        report["cpu_affinity"] = [cpu]
        report["cpuinfo"] = Path("/proc/cpuinfo").read_text()
        report["boot_id"] = Path("/proc/sys/kernel/random/boot_id").read_text().strip()
        report["binaries"] = {str(path): hashlib.sha256(path.read_bytes()).hexdigest()
                              for path in (args.peer, args.gateway, args.uwvm, args.wasm,
                                           args.baseline_peer, args.baseline_gateway, args.baseline_uwvm,
                                           args.runtime_loader) if path is not None}
        report["runtime_loader"] = str(args.runtime_loader) if args.runtime_loader else None
        report["runtime_library_path"] = args.runtime_library_path
        env = dict(os.environ, ROS_DOMAIN_ID="191", ROS_LOCALHOST_ONLY="1",
                   ROS_SECURITY_ENABLE="false", RMW_IMPLEMENTATION="rmw_fastrtps_cpp",
                   ROS_LOG_DIR=str(args.out / "ros-logs"), ROS_HOME=str(args.out / "ros-logs"),
                   ROS_MASTER_URI="http://127.0.0.1:11391", ROS_IP="127.0.0.1")
        # Receipt paths can be arbitrarily deep; AF_UNIX addresses cannot.
        # mkdtemp creates an exclusive private directory for this driver. Keep
        # its lifetime until every owned worker and gateway has been reaped.
        sockets = tempfile.TemporaryDirectory(prefix="uwvm-ros-bench-", dir="/tmp")
        socket_dir = Path(sockets.name)
        report["socket_directory"] = str(socket_dir)
        connection = {"node-name": "benchmark_gateway", "namespace": "/uwvm_perf", "domain-id": "191",
                      "rmw-implementation": "rmw_fastrtps_cpp", "security-mode": "disabled-test",
                      "keystore": "not-applicable", "enclave": "not-applicable"}
        grant = ["client", "service.call", "/uwvm_perf/trigger", "std_srvs/srv/Trigger",
                 "RIHS01_eeff2cd6fa5ad9d27cdf4dec64818317839b62f212a91e6b5304b634b2062c5f"]
        # Explicit bounds cover the long PMU cohort without removing admission,
        # quota, response validation, authentication, or allow-and-deny auditing.
        limits = {"timeout-ms": str(args.call_timeout_ms), "max-inflight": "1", "max-request-bytes": "64",
                  "max-response-bytes": "64", "max-rate-hz": "1000000", "burst": "1000000",
                  "max-bytes-per-second": "1048576", "validator": "none", "audit": "allow-and-deny",
                  "audit-max-rate-hz": "1000000", "audit-burst": "1000000", "audit-max-records": "1048576",
                  "audit-max-bytes": "67108864", "qos.reliability": "reliable", "qos.durability": "volatile",
                  "qos.history": "keep-last", "qos.depth": "10", "qos.deadline": "infinite",
                  "qos.lifespan": "infinite", "qos.liveliness": "automatic", "qos.liveliness-lease-duration": "infinite"}
        if args.ros_version == 1:
            connection = {"node-name": "benchmark_gateway", "namespace": "/uwvm_perf",
                          "master-uri": env["ROS_MASTER_URI"], "advertise-host": "127.0.0.1", "transport": "tcpros"}
            grant[3:] = ["std_srvs/Trigger", "937c9679a518e3a18d831e57125ea522"]
            limits = {key: value for key, value in limits.items() if not key.startswith("qos.")}
            master = start("roscore", ["roscore", "-p", "11391"])

            def master_ready():
                with socket_module.socket() as probe:
                    probe.settimeout(0.1)
                    return probe.connect_ex(("127.0.0.1", 11391)) == 0

            wait_for(master_ready, master)
        if args.operation.startswith("time-"):
            operation = "time.read-" + args.operation[len("time-"):]
            grant = ["client", operation, "/clock", "builtin/time-v1", "builtin:time-nanoseconds-v1"]
            # Clock authority has neither a service request body nor QoS or a
            # discovery timeout. Keep its actual public policy schema intact.
            limits = {key: value for key, value in limits.items()
                      if not key.startswith("qos.") and key not in ("timeout-ms", "max-request-bytes", "validator")}
            limits["max-response-bytes"] = "8"
        if args.operation in RESOURCE_CASES:
            grant, limits = resource_configuration(args.operation, args.ros_version, limits)
            report["resource_case"] = {"number": RESOURCE_CASES[args.operation][0],
                                       "import": RESOURCE_CASES[args.operation][1], "grant": grant, "limits": limits}
        option = f"--ros{args.ros_version}-global-"
        report["policy"] = {"connection": connection, "grant": grant, "limits": limits}
        server = start("server", [args.peer, "server"])
        wait_for(lambda: "BENCH_SERVER_READY=1" in (args.out / "server.log").read_text(), server)
        for round_index in range(args.rounds):
            order = list(modes)
            random.Random(2500 + round_index).shuffle(order)
            for mode in order:
                # ROS1 Noetic ships Python 3.8, before str.removeprefix.
                family = mode[len("baseline-"):] if mode.startswith("baseline-") else mode
                baseline = mode.startswith("baseline-")
                peer_binary = args.baseline_peer if baseline else args.peer
                gateway_binary = args.baseline_gateway if baseline else args.gateway
                runtime_binary = args.baseline_uwvm if baseline else args.uwvm
                label = f"{mode}-{round_index}"
                gateway = None
                policy = args.out / (label + ".gateway.conf")
                socket = socket_dir / (label + ".sock")
                if family == "native":
                    command = [peer_binary, "native", str(args.count)]
                    if args.operation != "trigger":
                        command.append(args.operation)
                elif family == "ipc":
                    command = [peer_binary, "ipc", str(args.count), policy]
                    if args.operation in RESOURCE_CASES:
                        command.append(str(RESOURCE_CASES[args.operation][0]))
                else:
                    command = [runtime_binary, "-Raot", "-Rct", "0", "-Rllvm-policy", "max",
                               "--runtime-llvm-jit-call-stack", "unwind", "--log-verbose",
                               "--runtime-compiler-log", "err",
                               "--wasip1-global-noinherit-system-environment",
                               "--wasip1-global-grant", "monotonic-clock", option + "enable"]
                    if args.cache_mode == "signed":
                        command += ["--runtime-llvm-jit-cache-seal-process", "-Rllvm-cache-path", "path", args.cache]
                    else:
                        command += ["-Rllvm-cache-path", "disable"]
                    worker_limits = {"connect-timeout-ms": "10000", "audit-deny-max-rate-hz": "100",
                                     "audit-deny-burst": "100", "audit-deny-max-records": "10000",
                                     "audit-deny-max-bytes": "1048576"}
                    for key, value in {**connection, **worker_limits, "gateway-socket": str(socket),
                                       "gateway-uid": str(os.getuid())}.items():
                        command += [option + key, value]
                    command += [option + "grant", *grant]
                    for key, value in limits.items():
                        command += [option + "set", "client", key, value]
                    command += ["--run", args.wasm]
                    if args.runtime_loader:
                        command = [args.runtime_loader, "--library-path", args.runtime_library_path, *command]
                if family != "native":
                    command = ["/bin/bash", "-c", 'kill -STOP $$; exec "$@"', "benchmark-worker", *command]
                launched = time.monotonic_ns()
                worker = start(label, command, stdin=subprocess.PIPE)
                if family != "native":
                    wait_for(lambda: "\nState:\tT" in Path(f"/proc/{worker.pid}/status").read_text(), worker)
                    lines = [f"backend ros{args.ros_version}", f"worker-uid {os.getuid()}", f"worker-pid {worker.pid}",
                             "socket " + str(socket), "timeout-ms 10000", "max-sessions 4",
                             "audit-max-records-per-second 1000000", "audit-max-records 1048576",
                             "audit-max-bytes 67108864"]
                    lines += [f"connection {key} {value}" for key, value in connection.items()]
                    lines += ["grant " + " ".join(grant)]
                    lines += [f"set client {key} {value}" for key, value in limits.items()]
                    policy.write_text("\n".join(lines) + "\n")
                    policy.chmod(0o600)
                    gateway = start(label + "-gateway", [gateway_binary, policy])
                    wait_for(socket.is_socket, gateway)
                    os.kill(worker.pid, signal.SIGCONT)
                log_path = args.out / (label + ".log")
                wait_for(lambda: f"BENCH_READY={args.count}" in log_path.read_text(), worker, 60)
                ready = time.monotonic_ns()
                profiler("resume")
                worker.stdin.write(b"R")
                worker.stdin.flush()
                code = worker.wait(timeout=300)
                profiler("pause")
                finished = time.monotonic_ns()
                worker.stdin.close()
                text = re.sub(r"\x1b\[[0-9;]*m", "", log_path.read_text())
                values = {key: int(value) for key, value in re.findall(r"BENCH_(\w+)=(\d+)", text)}
                row = {"mode": mode, "round": round_index, "warmup": round_index < args.warmup_rounds,
                       "returncode": code, "ready_after_ns": ready - launched,
                       "process_with_setup_ns": finished - launched, "values": values,
                       "cache_hits": len(re.findall(r"object-cache-hit [^\n]*signature_verified=1\b", text)),
                       "log": log_path.name}
                report["samples"].append(row)
                persist()
                assert code == 0 and values.get("ROS_COUNT") == args.count and values.get("CHECKSUM") == args.count, row
                assert values.get("BODY_NS", 0) > 0
                if family == "wasm" and args.cache_mode == "signed" and round_index > 0:
                    assert row["cache_hits"] >= 1, "An unchanged launch must use its signed cache"
                if gateway:
                    stop(gateway)
        stop(server)
        observed = re.findall(r"BENCH_PEER_CALLS=(\d+)", (args.out / "server.log").read_text())
        # Clock operations execute in the gateway's native clock implementation;
        # a Trigger callback would indicate an incorrectly routed test fixture.
        expected = len(report["samples"]) * (args.count + 20) if args.operation == "trigger" else 0
        assert observed == [str(expected)], (observed, expected)
        report["peer_callbacks"] = expected
        report["summary"] = []
        for mode in modes:
            samples = [row["values"]["BODY_NS"] / args.count for row in report["samples"]
                       if row["mode"] == mode and not row["warmup"]]
            report["summary"].append({"mode": mode, "n": len(samples), "median_ns_per_exchange": statistics.median(samples),
                                      "min_ns_per_exchange": min(samples), "max_ns_per_exchange": max(samples)})
        report["cgroup_cpu_after"] = (cg / "cpu.stat").read_text()
        report["status"] = "PASS"
    except BaseException as error:
        report.update(status="FAIL", failure=repr(error))
        raise
    finally:
        for child in reversed(children):
            stop(child)
        if sockets is not None:
            sockets.cleanup()
        report["finished"] = time.time()
        persist()
    print(json.dumps(report["summary"], indent=2), flush=True)


if __name__ == "__main__":
    main()
