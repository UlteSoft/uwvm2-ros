#!/usr/bin/env python3
"""Two genuine LLVM guest workers, stable ROS-v1 imports and repeated replay.

Requires existing ROS gateways and an isolated network namespace. A real native
peer audits publish/bind operations; the fixture checks both replay endpoints.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import signal
import socket
import subprocess
import sys
import tempfile
import time


def require(value, message):
    if not value:
        raise RuntimeError(message)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--native-root", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--repository", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--memory64", action="store_true")
    parser.add_argument("--backend", choices=("software", "hardware"), default="software")
    parser.add_argument("--delivery", choices=("sync", "async"), default=os.environ.get("UWVM_TEST_MEMTAG_DELIVERY", "sync"))
    parser.add_argument("--report", choices=("crash", "log"), default="crash")
    parser.add_argument("--host-tag-mismatch", action="store_true")
    parser.add_argument("--both-publish", action="store_true", help="Both workers contend on the same real ROS backend")
    parser.add_argument("--memtag", action="store_true", help="Tag the published payload and use its tagged pointer")
    args = parser.parse_args()
    require(not args.host_tag_mismatch or (args.memtag and args.report == "log"), "actual tagged payload and log policy required")
    address_type = "i64" if args.memory64 else "i32"
    extension = "i64.extend_i32_u" if args.memory64 else ""
    binary = args.binary.resolve()
    root = args.native_root.resolve()
    out = args.out.resolve()
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "resource cgroup required")
    require([name for _, name in socket.if_nameindex()] == ["lo"], "isolated network namespace required")
    out.mkdir(parents=True, exist_ok=False)
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    results = []
    for version in (1, 2):
        folder = out / f"ros{version}"
        folder.mkdir()
        receipt = json.loads((root / f"native-ros{version}/receipt.json").read_text())
        gateway = root / f"native-ros{version}/build/gateway/uwvm-ros{version}-gateway"
        require(hashlib.sha256(gateway.read_bytes()).hexdigest() == receipt["binaries"][str(gateway)], "gateway identity")
        env = {**os.environ, **receipt["env"], "PATH": str(root / "tools") + ":" + os.environ["PATH"],
               "ROS_DOMAIN_ID": "193", "ROS_LOCALHOST_ONLY": "1", "RMW_IMPLEMENTATION": "rmw_fastrtps_cpp",
               "ROS_MASTER_URI": "http://127.0.0.1:11393", "ROS_IP": "127.0.0.1",
               "ROS_HOME": str(folder / "ros-home"), "ROS_LOG_DIR": str(folder / "ros-logs")}
        contract = folder / "gateway.conf"
        env.update(UWVM_TEST_ROS_CONTRACT=str(contract), UWVM_TEST_ROS_AUDIT=str(folder / "gateway.log"))
        wat = folder / "concurrent.wat"
        wasm = wat.with_suffix(".wasm")
        selected = "i32.const 1" if args.both_publish else "local.get $id i32.eqz"
        tag = 19 if args.host_tag_mismatch and args.memory64 else 2 if args.host_tag_mismatch else 3
        payload = (tag << (56 if args.memory64 else 28)) | 128 if args.memtag else 128
        env.update(UWVM_TEST_MEMTAG_BACKEND=args.backend, UWVM_TEST_MEMTAG_DELIVERY=args.delivery,
                   UWVM_TEST_MEMTAG_REPORT=args.report)
        if args.host_tag_mismatch:
            env["UWVM_TEST_REQUIRE_REPORTS"] = "1"
        wat.write_text(f'''(module
          (import "uwvm_ros{version}_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
          (import "uwvm_ros{version}_v1" "capability_close" (func $close (param i64) (result i32)))
          (import "uwvm_ros{version}_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))
          (memory {"i64 " if args.memory64 else ""}1 1 shared)
          (data ({address_type}.const 64) "talk") (data ({address_type}.const 128) "\\08\\00\\00\\00restored")
          (func $initialize i32.const 0 i64.const 64 i64.const 4 i64.const 16 call $open if unreachable end)
          (func $worker (result i32) (local $id i32) (local $temporary i32)
            {address_type}.const 0 i32.const 1 i32.atomic.rmw.add local.set $id
            local.get $id i32.const 8 i32.mul i32.const 256 i32.add local.set $temporary
            loop $work
              {selected} if
                i32.const 0 i64.const 64 i64.const 4 local.get $temporary i64.extend_i32_u call $open if unreachable end
                local.get $temporary {extension} i64.load call $close if unreachable end
                {address_type}.const 16 i64.load i32.const 0 i64.const {payload} i64.const 12 call $publish if unreachable end
              else
                {address_type}.const 32 i32.const 1 i32.atomic.rmw.add drop
              end
              br $work
            end i32.const 0))
''')
        subprocess.run(["/usr/bin/wat2wasm", "--enable-threads", "--enable-memory64", "--debug-names", str(wat), "-o", str(wasm)], check=True)
        if args.memtag:
            sys.path.insert(0, str(args.repository.resolve() / "test/0086.memtag"))
            import vm_matrix
            original = wasm.read_bytes()

            def read_u(data, at):
                value = shift = 0
                while True:
                    byte = data[at]; at += 1
                    value |= (byte & 127) << shift
                    if not byte & 128:
                        return value, at
                    shift += 7

            encoded = bytearray(original[:8]); at = 8
            while at < len(original):
                section = original[at]; length, start = read_u(original, at + 1)
                body = original[start:start + length]; at = start + length
                if section == 10:
                    count, cursor = read_u(body, 0)
                    size, first = read_u(body, cursor)
                    code = body[first:first + size]
                    require(code[0] == 0, "initializer has no local declarations")
                    code = code[:1] + vm_matrix.Program(args.memory64).tags(128, 16, 3) + code[1:]
                    body = vm_matrix.u(count) + vm_matrix.u(len(code)) + code + body[first + size:]
                encoded += bytes([section]) + vm_matrix.u(len(body)) + body
            wasm.write_bytes(encoded)
            env["UWVM_TEST_REQUIRE_MTE"] = "1"
        actors = []
        logs = []
        short = tempfile.TemporaryDirectory(prefix="cr-", dir=args.repository.resolve() / ".tmp")
        address = Path(short.name) / "g.sock"

        def launch(name, command):
            log = (folder / (name + ".log")).open("wb")
            logs.append(log)
            child = subprocess.Popen(command, cwd=folder, env=env, stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
            actors.append(child)
            return child

        try:
            if version == 1:
                master = launch("master", [str(root / "tools/roscore"), "-p", "11393"])
                until = time.monotonic() + 30
                while True:
                    with socket.socket() as probe:
                        if probe.connect_ex(("127.0.0.1", 11393)) == 0:
                            break
                    require(master.poll() is None and time.monotonic() < until, "ROS master startup")
                    time.sleep(.01)
            worker = launch("worker", ["/bin/bash", "-c", 'kill -STOP $$; exec "$@"', "replay-worker",
                                       *prefix, str(binary), str(wasm), "3000"])
            until = time.monotonic() + 20
            while "\nState:\tT" not in Path(f"/proc/{worker.pid}/status").read_text():
                require(worker.poll() is None and time.monotonic() < until, "worker gate")
                time.sleep(.005)
            fields = {"node-name": "checkpoint", "namespace": "/"}
            if version == 1:
                fields.update({"master-uri": env["ROS_MASTER_URI"], "advertise-host": "127.0.0.1", "transport": "tcpros"})
                datatype, identity = "std_msgs/String", "992ce8a1687cec8c8bd883ec73ca41d1"
                settings = {"queue-depth": "4", "transport": "tcpros", "latch": "false"}
            else:
                fields.update({"domain-id": "193", "rmw-implementation": "rmw_fastrtps_cpp", "security-mode": "disabled-test",
                               "keystore": "not-applicable", "enclave": "not-applicable"})
                datatype, identity = "std_msgs/msg/String", "RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18"
                settings = {"qos.reliability": "reliable", "qos.durability": "volatile", "qos.history": "keep-last", "qos.depth": "4",
                            "qos.deadline": "infinite", "qos.lifespan": "infinite", "qos.liveliness": "automatic",
                            "qos.liveliness-lease-duration": "infinite"}
            settings.update({"overflow-policy": "reject", "validator": "none", "audit": "allow-and-deny"})
            lines = [f"backend ros{version}", f"worker-uid {os.getuid()}", f"worker-pid {worker.pid}", f"socket {address}"]
            lines += [f"connection {k} {v}" for k, v in fields.items()]
            lines += [f"grant talk topic.publish /test/data {datatype} {identity}"]
            lines += [f"set talk {k} {v}" for k, v in settings.items()]
            contract.write_text("\n".join(lines) + "\n")
            contract.chmod(0o600)
            peer = launch("gateway", [str(gateway), str(contract)])
            while not address.is_socket():
                require(peer.poll() is None and time.monotonic() < until, "gateway startup")
                time.sleep(.005)
            os.kill(worker.pid, signal.SIGCONT)
            code = worker.wait(timeout=900 if prefix else 240)
            text = (folder / "worker.log").read_text(errors="replace")
            require(code == 0 and "ROS phases began=" in text and text.count("memory_equal=1") == 2 and
                    "two-worker shared-memory instruction replay PASS" in text,
                    f"ROS{version} concurrent replay exit={code}: {text[-5000:]}")
            if args.host_tag_mismatch:
                require("MTE journaled host violations=" in text and text.count("MTE replay round=") == 2 and
                        "MTE crash-policy replay refusal preserved original stop and generation" in text,
                        "all replayed native-import MTE diagnostics and crash-policy refusal are required")
            results.append({"backend": version, "abi": "v1", "passed": True,
                            "memory_bits": 64 if args.memory64 else 32, "mte_backend": args.backend,
                            "mte_delivery": args.delivery, "mte_report": args.report, "host_tag_mismatch": args.host_tag_mismatch,
                            "binary_sha256": hashlib.sha256(binary.read_bytes()).hexdigest(),
                            "wasm_sha256": hashlib.sha256(wasm.read_bytes()).hexdigest()})
            (out / "results.json").write_text(json.dumps(results, indent=2))
            print(f"ROS{version} concurrent replay PASS", flush=True)
        finally:
            for child in reversed(actors):
                if child.poll() is None:
                    os.killpg(child.pid, signal.SIGKILL)
                    child.wait(timeout=15)
            for log in logs:
                log.close()
            short.cleanup()


if __name__ == "__main__":
    main()
