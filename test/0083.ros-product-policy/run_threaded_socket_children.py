#!/usr/bin/env python3
"""Real two-child Guest socket authorization and quota regression.

Two distinct admitted Guest children rendezvous before either socket call.
Each owns its iovec, output canary, result slot, and TCP descriptor. The main
checks both actual TIDs, both completion publications, and every API result
before proc_exit(37). This proves overlapping child lifetimes and real network
effects, not simultaneous CPU execution of two providers or a ROS transport.
The fixtures contain no invalid tag event or explicit tag manipulation. Mode
selection qualifies provider/permission/quota composition; it does not by
itself prove asynchronous tag-worker activity or bad-tag interception.
"""

import argparse
import hashlib
import json
from pathlib import Path
import selectors
import socket
import subprocess
import time

from run_permission_composition import check, const, name, section, store, u


CANARY = 0x5A6BC7D8
COMPLETED = 37
PAYLOADS = (b"child-A\n", b"child-B\n")
READY, RELEASE, DONE, CHILD_TIDS, SPAWN_TIDS, RETRY_WORD = 4096, 4100, 4104, 4112, 4128, 4140
STATES = ("hidden", "deny", "missing-rights", "one-fd-only", "unlimited", "quota-reject", "quota-wait")


def local(index):
    return b"\x20" + u(index)


def atomic(opcode):
    return b"\xfe" + u(opcode) + b"\x02\0"


def load(address):
    return const(address) + b"\x28\x02\0"


def notify(address):
    return const(address) + const(2) + atomic(0) + b"\x1a"


def expected_errors(state):
    if state in ("deny", "missing-rights"):
        return (76, 76)
    if state == "one-fd-only":
        return (0, 76)
    if state == "quota-reject":
        return (1, 1)
    return (0, 0)


def guest(state):
    # Imported function indices: proc_exit=0, thread-spawn=1, sock_send=2.
    # Defined functions: main=3, child=4, bounded_spawn=5, wait_equal=6.
    main = b""
    for child, payload in enumerate(PAYLOADS):
        main += store(child * 16, 128 + child * 16) + store(child * 16 + 4, len(payload))
        main += store(child * 16 + 8, CANARY) + store(child * 16 + 12, CANARY)
        for offset in (0, 4):
            main += store(128 + child * 16 + offset, int.from_bytes(payload[offset:offset + 4], "little"))
    for child in (0, 1):
        main += const(SPAWN_TIDS + child * 4) + const(child) + b"\x10\x05\x36\x02\0"
    main += const(READY) + const(2) + b"\x10\x06"
    for child in (0, 1):
        main += check(load(SPAWN_TIDS + child * 4) + const(0) + b"\x4a", 1)
        main += check(load(CHILD_TIDS + child * 4) + load(SPAWN_TIDS + child * 4) + b"\x46", 1)
    main += check(load(CHILD_TIDS) + load(CHILD_TIDS + 4) + b"\x47", 1)
    main += const(RELEASE) + const(1) + atomic(0x17) + notify(RELEASE)
    main += const(DONE) + const(2) + b"\x10\x06"
    for child, error in enumerate(expected_errors(state)):
        main += check(load(child * 16 + 12), error)
        main += check(load(child * 16 + 8), len(PAYLOADS[child]) if error == 0 else CANARY)
    # A distinct completion exit prevents an early normal return or exit(0)
    # from passing denied-call cases that intentionally send no network bytes.
    main += const(COMPLETED) + b"\x10\0\0"

    def child_slot(offset, stride=16):
        return local(1) + const(stride) + b"\x6c" + const(offset) + b"\x6a"

    child = check(local(1) + const(1) + b"\x4b", 0)  # unsigned arg > 1 is invalid
    child += child_slot(CHILD_TIDS, 4) + local(0) + b"\x36\x02\0"
    child += const(READY) + const(1) + atomic(0x1E) + b"\x1a" + notify(READY)
    child += const(RELEASE) + const(1) + b"\x10\x06"
    child += (child_slot(12) + const(3) + local(1) + b"\x6a" + child_slot(0) +
              const(1) + const(0) + child_slot(8) + b"\x10\x02\x36\x02\0")
    child += const(DONE) + const(1) + atomic(0x1E) + b"\x1a" + notify(DONE)

    # Retry only failed admission; never replay a successful child or provider.
    spawn = const(64) + b"\x21\x01\x03\x40"
    spawn += local(0) + b"\x10\x01\x22\x02" + const(0) + b"\x4a\x04\x40" + local(2) + b"\x0f\x0b"
    spawn += local(1) + const(1) + b"\x6b\x22\x01\x45\x04\x40\0\x0b"
    spawn += const(RETRY_WORD) + const(0) + const(1_000_000, "i64") + atomic(1) + b"\x1a\x0c\0\x0b\0"
    # Wait on the value actually observed, so notify-before-wait cannot be lost.
    wait = b"\x02\x40\x03\x40" + local(0) + atomic(0x10) + b"\x22\x02" + local(1) + b"\x46\x0d\x01"
    wait += local(0) + local(2) + const(-1, "i64") + atomic(1) + b"\x1a\x0c\0\x0b\x0b"

    types = [(0, 0), (2, 0), (1, 0), (1, 1), (5, 1)]
    encoded_types = b"".join(b"\x60" + u(p) + b"\x7f" * p + u(r) + b"\x7f" * r for p, r in types)
    imports = (name("env") + name("memory") + b"\x02\x03\x01\x02" +
               name("wasi_snapshot_preview1") + name("proc_exit") + b"\0\x02" +
               name("wasi") + name("thread-spawn") + b"\0\x03" +
               name("wasi_snapshot_preview1") + name("sock_send") + b"\0\x04")
    exports = name("_start") + b"\0\x03" + name("wasi_thread_start") + b"\0\x04" + name("memory") + b"\x02\0"
    bodies = [b"\0" + main, b"\0" + child, b"\x01\x02\x7f" + spawn, b"\x01\x01\x7f" + wait]
    return (b"\0asm\x01\0\0\0" + section(1, u(len(types)) + encoded_types) + section(2, b"\x04" + imports) +
            section(3, b"\x04\0\x01\x03\x01") + section(7, b"\x03" + exports) +
            section(10, u(len(bodies)) + b"".join(u(len(body) + 1) + body + b"\x0b" for body in bodies)))


def receive_while_running(command, listener, stdout_path, stderr_path, timeout):
    records, connections = [], []
    begin = time.monotonic()
    timed_out = False
    with selectors.DefaultSelector() as selector, stdout_path.open("wb") as stdout, stderr_path.open("wb") as stderr:
        listener.setblocking(False)
        selector.register(listener, selectors.EVENT_READ, None)
        with subprocess.Popen(command, stdout=stdout, stderr=stderr) as process:
            exited_at = None
            while True:
                now = time.monotonic()
                if process.poll() is None and now - begin > timeout:
                    timed_out = True
                    process.kill()
                    process.wait()
                if process.poll() is not None:
                    if exited_at is None:
                        exited_at = now
                    if now - exited_at > 0.2:
                        break
                for key, _ in selector.select(0.025):
                    if key.fileobj is listener:
                        connection, _ = listener.accept()
                        connection.setblocking(False)
                        record = {"data_hex": "", "chunks": [], "closed": False}
                        records.append(record)
                        connections.append(connection)
                        selector.register(connection, selectors.EVENT_READ, record)
                    else:
                        try:
                            data = key.fileobj.recv(65536)
                        except ConnectionResetError:
                            data = b""
                            key.data["reset"] = True
                        if data:
                            key.data["data_hex"] += data.hex()
                            key.data["chunks"].append({"seconds": time.monotonic() - begin, "data_hex": data.hex()})
                        else:
                            key.data["closed"] = True
                            selector.unregister(key.fileobj)
                            key.fileobj.close()
            code = process.wait()
    for connection in connections:
        connection.close()
    return "timeout" if timed_out else code, time.monotonic() - begin, records


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--runtime-prefix", type=json.loads, default=[])
    parser.add_argument("--engine", choices=("int", "aot"), required=True)
    parser.add_argument("--memtag-backend", choices=("software", "hardware"), default="software")
    parser.add_argument("--memtag-mode", choices=("sync", "async"), default="sync")
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--states", nargs="+", choices=STATES, default=list(STATES))
    parser.add_argument("--timeout", type=float, default=20.0)
    args = parser.parse_args()
    if not isinstance(args.runtime_prefix, list) or not all(isinstance(x, str) for x in args.runtime_prefix):
        parser.error("--runtime-prefix must be a JSON string array")
    if not 0 < args.timeout < float("inf"):
        parser.error("--timeout must be positive and finite")
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    owner = out / "owner.wasm"
    owner.write_bytes(b"\0asm\x01\0\0\0" + section(5, b"\x01\x03\x01\x02") + section(7, b"\x01\x06memory\x02\0"))
    runtime = args.runtime.resolve()
    report = {"runtime": str(runtime), "runtime_sha256": hashlib.sha256(runtime.read_bytes()).hexdigest(),
              "runtime_prefix": args.runtime_prefix, "engine": args.engine, "completion_exit": COMPLETED,
              "memtag_backend": args.memtag_backend, "memtag_mode": args.memtag_mode, 
              "scope": "two real Guest child lifetimes overlap at an atomic barrier; each has an independent TCP fd and buffer",
              "memory_tag_scope": "no explicit tag manipulation or bad-tag event; this is provider/permission/quota composition, not bad-tag detection or worker activity proof",
              "cases": [], "complete": False}
    for state in args.states:
        raw = guest(state)
        wasm = out / (state + ".wasm")
        wasm.write_bytes(raw)
        flags = ["--runtime-" + args.engine, "--runtime-compile-threads", "0", "-WFE-threads",
                 "--runtime-memtag-backend", args.memtag_backend, "--runtime-memtag-mode", args.memtag_mode, 
                 "--wasi-threads", "3", "3", "1048576", "16777216", "67108864", "131072",
                 "--wasm-set-main-module-name", "main", "--wasip1-single-create", "main", "--wasip1-single-enable", "main",
                 "--wasip1-single-interface", "main", "proc_exit", "allow", "--wasip1-single-grant", "main", "proc-exit",
                 "--wasip1-single-interface", "main", "thread-spawn", "allow", "--wasip1-single-grant", "main", "thread-spawn",
                 "--wasm-preload-library", str(owner), "env"]
        if args.engine == "aot":
            flags += ["--runtime-llvm-jit-cache-path", "disable", "--runtime-llvm-jit-policy", "debug"]
        if state != "hidden":
            flags += ["--wasip1-single-interface", "main", "sock_send", "deny" if state == "deny" else "allow"]
        if state == "quota-reject":
            flags += ["--wasip1-single-limit", "main", "socket-write.request-bytes", "1",
                      "--enable-warning", "all_host_limited_wasip1"]
        if state == "quota-wait":
            flags += ["--wasip1-single-limit", "main", "socket-write.window-calls", "1",
                      "--wasip1-single-limit-action", "main", "socket-write.window-calls", "wait",
                      "--enable-warning", "all_host_limited_wasip1"]
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as listener:
            listener.bind(("127.0.0.1", 0))
            listener.listen(4)
            for child in (0, 1):
                rights = "none" if state == "missing-rights" or (state == "one-fd-only" and child == 1) else "write"
                flags += ["--wasip1-single-socket-rights", "main", str(3 + child), rights, "none",
                          "--wasip1-single-socket-tcp-connect", "main", str(3 + child),
                          "127.0.0.1:" + str(listener.getsockname()[1])]
            command = [*args.runtime_prefix, str(runtime), *flags, "--run", str(wasm)]
            stdout, stderr = out / (state + ".stdout"), out / (state + ".stderr")
            code, elapsed, streams = receive_while_running(command, listener, stdout, stderr, args.timeout)
        output = stdout.read_text(errors="replace") + stderr.read_text(errors="replace")
        wanted = [PAYLOADS[i].hex() for i, error in enumerate(expected_errors(state)) if error == 0] if state != "hidden" else []
        received = [record["data_hex"] for record in streams if record["data_hex"]]
        if state == "hidden":
            passed = code == 127 and "interface exposure permission is hidden" in output
        else:
            passed = code == COMPLETED and len(streams) == 2
        passed = passed and sorted(received) == sorted(wanted)
        if state == "deny":
            passed = passed and output.count("[host_denied_wasip1.sock_send]") == 2
        if state == "quota-reject":
            passed = passed and "quota rejected: socket-write.request-bytes" in output
        if state == "quota-wait":
            # Total wall time includes startup/JIT and is only a coarse guard.
            # The explicit diagnostic proves entry into the quota wait path;
            # per-stream receive times retain the observed network effect.
            # Fixed window boundaries do not promise a full second between
            # the first successful call and the second successful call.
            passed = passed and elapsed >= 0.5 and "synchronously waiting for quota socket-write.window-calls" in output
        row = {"state": state, "command": command, "fixture_sha256": hashlib.sha256(raw).hexdigest(),
               "exit": code, "seconds": elapsed, "streams": streams, "expected_payloads_hex": wanted,
               "guest_expected_errno": expected_errors(state), "stdout": str(stdout), "stderr": str(stderr), "passed": bool(passed)}
        report["cases"].append(row)
        (out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
        print(state, code, round(elapsed, 3), "streams", len(streams), "passed", bool(passed), flush=True)
        if not passed:
            print(output[-6000:], flush=True)
    report["complete"] = all(row["passed"] for row in report["cases"])
    (out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    return 0 if report["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
