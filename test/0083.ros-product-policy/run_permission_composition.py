#!/usr/bin/env python3
"""Actual CLI 0/1/2 gates and ROS/WASI socket composition, without a WAT tool.

This checks admission and guest-visible effects. It does not qualify a native
ROS gateway or claim that the threaded profile supports every WASIp1 function.
"""
from pathlib import Path
import argparse
import hashlib
import json
import socket
import subprocess
import time


def u(n):
    out = bytearray()
    while n >= 128:
        out.append((n & 127) | 128)
        n >>= 7
    return bytes(out + bytes([n]))


def s(n):
    out = bytearray()
    while True:
        value = n & 127
        n >>= 7
        done = (n == 0 and value < 64) or (n == -1 and value >= 64)
        out.append(value if done else value | 128)
        if done: return bytes(out)


def name(value):
    value = value.encode()
    return u(len(value)) + value


def section(code, value):
    return bytes([code]) + u(len(value)) + value


def const(value, kind="i32"):
    return (b"\x41" if kind == "i32" else b"\x42") + s(value)


def check(value, expected, kind="i32"):
    return value + const(expected, kind) + (b"\x47" if kind == "i32" else b"\x52") + b"\x04\x40\0\x0b"


def store(address, value):
    return const(address) + const(value) + b"\x36\x02\0"


def module(imports=(), body=b"", *, threaded=False, memory64=False, export="_start", results=()):
    assert not (threaded and memory64), "this fixture's threaded profile uses memory32"
    types = []
    def index(parameters, outputs):
        key = (tuple(parameters), tuple(outputs))
        if key not in types: types.append(key)
        return types.index(key)
    encoded = []
    for row in imports:
        encoded.append(name(row["ns"]) + name(row["name"]) + b"\0" + u(index(row["params"], row["results"])))
    start_type = index((), results)
    if threaded:
        encoded.append(name("env") + name("memory") + b"\x02\x03\x01\x01")
        worker_type = index(("i32", "i32"), ())
    value_types = {"i32": 0x7f, "i64": 0x7e, "f32": 0x7d, "f64": 0x7c}
    table = b"".join(b"\x60" + u(len(p)) + bytes(value_types[t] for t in p)
                     + u(len(r)) + bytes(value_types[t] for t in r) for p, r in types)
    out = b"\0asm\x01\0\0\0" + section(1, u(len(types)) + table)
    if encoded: out += section(2, u(len(encoded)) + b"".join(encoded))
    out += section(3, u(2 if threaded else 1) + u(start_type) + (u(worker_type) if threaded else b""))
    if not threaded: out += section(5, b"\x01" + (b"\x04" if memory64 else b"\0") + b"\x01")
    exports = [name(export) + b"\0" + u(len(imports)), name("memory") + b"\x02\0"]
    if threaded: exports.append(name("wasi_thread_start") + b"\0" + u(len(imports) + 1))
    out += section(7, u(len(exports)) + b"".join(exports))
    body = b"\0" + body + b"\x0b"
    return out + section(10, u(2 if threaded else 1) + u(len(body)) + body + (b"\x02\0\x0b" if threaded else b""))


def family(row):
    if row["ns"] in ("wasi_snapshot_preview1", "wasi"): return "wasip1"
    if row["ns"].startswith("uwvm_ros1_"): return "ros1"
    return "ros2"


def key(row):
    return row["name"] if family(row) == "wasip1" else row["ns"] + "." + row["name"]


def policy(rows, state, target="main"):
    flags = []
    for group in sorted({family(row) for row in rows}):
        flags += ["--" + group + "-single-create", target, "--" + group + "-single-enable", target]
    for row in rows:
        flags += ["--" + family(row) + "-single-interface", target, key(row), state]
    return flags


def invoke(row, index):
    return b"".join(const(v, t) for v, t in zip(row["args"], row["params"])) + b"\x10" + u(index)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--runtime-prefix", type=json.loads, default=[],
                        help="JSON argv prefix, e.g. QEMU plus -cpu/-L; hashes still identify the actual VM")
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--engines", nargs="+", default=["int", "aot"])
    parser.add_argument("--suites", nargs="+", choices=["exposure", "contracts", "sockets"],
                        default=["exposure", "contracts", "sockets"])
    parser.add_argument("--socket-ros", nargs="+", choices=["none", "ros1", "ros2"], default=["none", "ros1", "ros2"])
    parser.add_argument("--socket-profiles", nargs="+", choices=["ordinary", "threaded"], default=["ordinary", "threaded"])
    parser.add_argument("--families", nargs="+", choices=["wasip1", "ros1", "ros2"], default=["wasip1", "ros1", "ros2"],
                        help="Select exposure/contract families explicitly; unsupported ROS backends are not silently passed")
    args = parser.parse_args()
    if not isinstance(args.runtime_prefix, list) or not all(isinstance(v, str) for v in args.runtime_prefix):
        parser.error("--runtime-prefix must be a JSON array of argument strings")
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    rows = json.loads(Path(__file__).with_name("host-interface-signatures.json").read_text())
    rows = [row for row in rows if family(row) in args.families]
    report = {"runtime": str(args.runtime.resolve()), "runtime_sha256": hashlib.sha256(args.runtime.read_bytes()).hexdigest(),
              "runtime_prefix": args.runtime_prefix, "selected_families": args.families,
              "complete": False, "cases": []}
    def save(): (out / "report.json").write_text(json.dumps(report, indent=2))
    def fixture(label, contents):
        path = out / (label + ".wasm")
        path.write_bytes(contents)
        return path
    def run(label, engine, flags, wasm, *, succeeds=True, contains=(), stdout=None):
        engine_flags = ["--runtime-" + engine]
        if engine == "aot": engine_flags += ["--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable"]
        argv = list(map(str, [*args.runtime_prefix, args.runtime, *engine_flags, "--wasm-set-main-module-name", "main", *flags, "--run", wasm]))
        start = time.monotonic()
        # A regular log sink also works on test hosts without procfs. The safe
        # pipe writer intentionally cannot reopen a blocking pipe there.
        log = out / (label + "-" + engine + ".stderr")
        with log.open("wb") as error_stream:
            result = subprocess.run(argv, stdout=subprocess.PIPE if stdout is None else stdout,
                                    stderr=error_stream, timeout=45)
        stderr = log.read_text(errors="replace")
        row = {"name": label, "engine": engine, "argv": argv, "exit": result.returncode,
               "seconds": time.monotonic() - start, "stderr": stderr,
               "pass": (result.returncode == 0) == succeeds and all(x in stderr for x in contains)}
        report["cases"].append(row)
        save()
        if not row["pass"]: raise AssertionError(row)
        return row
    if "exposure" in args.suites:
        namespaces = set()
        for i, row in enumerate(rows):
            wasm = fixture("interface-" + str(i), module([row]))
            if row["ns"] not in namespaces:
                run("namespace-hidden-" + row["ns"], args.engines[0], [], wasm, succeeds=False,
                    contains=("namespace permission is disabled",))
                namespaces.add(row["ns"])
            group = family(row)
            run("interface-hidden-" + str(i), args.engines[0], ["--" + group + "-single-create", "main",
                "--" + group + "-single-enable", "main"], wasm, succeeds=False,
                contains=("interface exposure permission is hidden",))
            # Validation, including all exact signatures, succeeds at state 1.
            if row["ns"] != "wasi":
                run("interface-deny-validates-" + str(i), args.engines[0], ["--mode", "validation", *policy([row], "deny")], wasm)
    if "contracts" in args.suites:
        returning = [r for r in rows if r["results"] and r["ns"] != "wasi"]
        body = store(0, 123456789)
        for i, row in enumerate(returning):
            body += check(invoke(row, i), 76 if family(row) == "wasip1" else 13, row["results"][0])
        body += check(const(0) + b"\x28\x02\0", 123456789)
        wasm = fixture("all-denied", module(returning, body))
        representative = {"wasip1": "wasip1.random_get denied", "ros1": "ros1.uwvm_ros1_v1.abi_version denied",
                          "ros2": "ros2.uwvm_ros2_v1.abi_version denied"}
        for engine in args.engines:
            denied = run("all-denied", engine, policy(returning, "deny"), wasm,
                         contains=tuple(representative[group] for group in args.families))
            assert denied["stderr"].count("[warning]") == len(returning)
            silent = run("all-denied-silent", engine, policy(returning, "deny") + ["--disable-warning", "all_host_denied"], wasm)
            assert "[warning]" not in silent["stderr"]
            probes = [r for r in rows if r["name"] == "random_get" or
                      (r["ns"] in ("uwvm_ros1_v1", "uwvm_ros2_v1") and r["name"] == "abi_version")]
            for row in probes:
                group = family(row)
                expected = 76 if group == "wasip1" else 13
                worker = fixture(group + "-worker", module([row], invoke(row, 0), export="forward", results=("i32",)))
                imported = {"ns": "worker", "name": "forward", "params": [], "results": ["i32"], "args": []}
                main = fixture(group + "-caller", module([imported], check(b"\x10\0", expected)))
                flags = policy([row], "allow") + policy([row], "deny", "worker")
                if group == "wasip1": flags += ["--wasip1-single-grant", "main", "random"]
                run(group + "-immediate-module-denial", engine, flags + ["--wasm-preload-library", worker, "worker"], main,
                    contains=("denied",))
    if "sockets" in args.suites:
        shared = fixture("shared-memory", b"\0asm\x01\0\0\0" + section(5, b"\x01\x03\x01\x01")
                         + section(7, b"\x01" + name("memory") + b"\x02\0"))
        payload = b"safe"
        for engine in args.engines:
            for ros in args.socket_ros:
                ros_flags = [] if ros == "none" else ["--" + ros + "-single-create", "main", "--" + ros + "-single-enable", "main"]
                for profile in args.socket_profiles:
                    threaded = profile == "threaded"
                    thread_flags = (["-WFE-threads", "--wasi-threads", "3", "3", "1048576", "16777216", "8388608", "65536",
                                     "--wasm-preload-library", shared, "env"] if threaded else [])
                    for transport, call_name in [("preopen", "sock_send"), ("inherited-stdio", "fd_write")]:
                        for state in ["hidden", "deny", "resource-denied", "unlimited", "limited", "wait", "renumber-limited"]:
                            interface = "hidden" if state == "hidden" else "deny" if state == "deny" else "allow"
                            descriptor = 3 if transport == "preopen" else 1
                            call_fd = (1 if transport == "preopen" else 2) if state == "renumber-limited" else descriptor
                            params = [call_fd, 0, 1, 0, 16] if call_name == "sock_send" else [call_fd, 0, 1, 16]
                            imports = [{"ns": "wasi_snapshot_preview1", "name": call_name,
                                        "params": ["i32"] * len(params), "results": ["i32"], "args": params}]
                            expected = 76 if state in ("deny", "resource-denied") else 1 if state in ("limited", "renumber-limited") else 0
                            repetitions = 2 if state == "wait" else 1
                            body = store(0, 128) + store(4, len(payload)) + store(128, int.from_bytes(payload, "little"))
                            if state == "renumber-limited":
                                imports.append({"ns": "wasi_snapshot_preview1", "name": "fd_renumber", "params": ["i32", "i32"], "results": ["i32"], "args": [descriptor, call_fd]})
                                body += check(invoke(imports[1], 1), 0)
                            body += check(invoke(imports[0], 0), expected) * repetitions
                            label = "-".join([ros, profile, transport, state, engine])
                            wasm = fixture(label, module(imports, body, threaded=threaded))
                            flags = policy(imports, interface) + ros_flags + thread_flags
                            rights = "none" if state == "resource-denied" else "write"
                            flags += ["--wasip1-single-socket-rights", "main", str(descriptor), rights, "none"]
                            if state == "renumber-limited":
                                flags += ["--wasip1-single-grant", "main", "stdout.write" if call_fd == 1 else "stderr.write"]
                            if state in ("limited", "renumber-limited"):
                                # The socket budget must apply even after moving
                                # the handle to another stdio slot, whose stdio
                                # budget is unlimited. Destination direction
                                # rights are independently and explicitly granted.
                                flags += ["--wasip1-single-limit", "main", "socket-write.request-bytes", "1",
                                          "--enable-warning", "all_host_limited_wasip1"]
                            if state == "wait":
                                flags += ["--wasip1-single-limit", "main", "socket-write.window-calls", "1",
                                          "--wasip1-single-limit-action", "main", "socket-write.window-calls", "wait",
                                          "--enable-warning", "all_host_limited_wasip1"]
                            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as listener:
                                listener.bind(("127.0.0.1", 0)); listener.listen(); listener.settimeout(0.15)
                                if transport == "preopen":
                                    flags += ["--wasip1-single-socket-tcp-connect", "main", str(descriptor),
                                              "127.0.0.1:" + str(listener.getsockname()[1])]
                                    peer = child = None
                                else:
                                    peer, child = socket.socketpair()
                                    peer.settimeout(0.15)
                                    flags += ["--wasip1-single-grant", "main", "stdout.write"]
                                try:
                                    needed = ("interface exposure permission is hidden",) if state == "hidden" else ()
                                    result = run(label, engine, flags, wasm, succeeds=state != "hidden", contains=needed, stdout=child)
                                    if child is not None: child.close()
                                    if transport == "preopen":
                                        try: peer, _ = listener.accept(); peer.settimeout(0.15)
                                        except TimeoutError: peer = None
                                    received = bytearray()
                                    if peer is not None:
                                        while True:
                                            try: data = peer.recv(65536)
                                            except TimeoutError: break
                                            if not data: break
                                            received += data
                                    wanted = payload * repetitions if state in ("unlimited", "wait") else b""
                                    result["received_hex"] = received.hex()
                                    result["pass"] &= received == wanted
                                    if state == "wait":
                                        result["pass"] &= result["seconds"] >= 0.5 and "synchronously waiting for quota socket-write.window-calls" in result["stderr"]
                                        # An isolated window wait must block on its timer, rather than
                                        # retrying on its own FD-unlock notifications and flooding logs.
                                        result["wait_warning_count"] = result["stderr"].count("synchronously waiting for quota socket-write.window-calls")
                                        result["pass"] &= 1 <= result["wait_warning_count"] <= 8
                                    if state in ("limited", "renumber-limited"):
                                        result["pass"] &= "quota rejected: socket-write.request-bytes" in result["stderr"]
                                    save()
                                    assert result["pass"], result
                                finally:
                                    if child is not None: child.close()
                                    if peer is not None: peer.close()
    report["complete"] = True
    save()
    print(json.dumps({"passed": len(report["cases"]), "report": str(out / "report.json")}))


if __name__ == "__main__":
    main()
