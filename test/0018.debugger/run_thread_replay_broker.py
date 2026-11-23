#!/usr/bin/env python3
"""Normal DAP wire/host broker startup and replay, with an actual VM process."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import select
import shlex
import subprocess
import sys
import tempfile
import time

ROOT = Path(__file__).resolve().parents[2]


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--binary", type=Path, required=True)
    p.add_argument("--argv-json", type=Path, required=True)
    p.add_argument("--runner-json", type=Path)
    p.add_argument("--out", type=Path, required=True)
    args = p.parse_args(); args.out = args.out.resolve(); args.out.mkdir(parents=True, exist_ok=False)
    assert "/docker-" in Path("/proc/self/cgroup").read_text()
    binary = args.binary.resolve(); argv = json.loads(args.argv_json.read_text()); argv = argv[argv.index("--") + 1:]
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    executable = args.out / "existing-vm"
    executable.write_text("#!/bin/sh\nexec " + shlex.join([*prefix, str(binary)]) + ' "$@"\n'); executable.chmod(0o700)
    # Keep the Unix path short; both this socket and every artifact stay .tmp.
    private = tempfile.TemporaryDirectory(prefix="dap-th-", dir=ROOT / ".tmp")
    server = adapter = None; logs = []; result = {"passed": False, "messages": [], "binarySHA256": hashlib.sha256(binary.read_bytes()).hexdigest()}
    buffer = bytearray(); sequence = 0
    def require(value, detail):
        if not value: raise AssertionError(detail)
    try:
        log = (args.out / "vm.log").open("wb"); logs.append(log)
        command = [sys.executable, "-B", str(ROOT / "tools/debug/secure_server.py"), "serve", "--uwvm", str(executable), "--socket-dir", private.name, "--", *argv]
        (args.out / "argv.json").write_text(json.dumps(command, indent=2))
        server = subprocess.Popen(command, stdout=log, stderr=subprocess.STDOUT)
        deadline = time.monotonic() + 120
        while not (Path(private.name) / "control.sock").is_socket():
            require(server.poll() is None and time.monotonic() < deadline, "broker startup failed"); time.sleep(.01)
        err = (args.out / "adapter.log").open("wb"); logs.append(err)
        adapter = subprocess.Popen([sys.executable, "-B", str(ROOT / "tools/debug/dap_adapter.py")], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=err)

        def read_message():
            deadline = time.monotonic() + 120
            while True:
                pos = buffer.find(b"\r\n\r\n")
                if pos >= 0:
                    size = int(buffer[:pos].split(b":")[1]); end = pos + 4 + size
                    if len(buffer) >= end:
                        message = json.loads(buffer[pos + 4:end]); del buffer[:end]
                        result["messages"].append(message)
                        require(message.get("event") != "terminated", "normal IDE lost its live replay session")
                        return message
                require(time.monotonic() < deadline, "DAP response timed out")
                ready, _, _ = select.select([adapter.stdout], [], [], min(1, max(0, deadline - time.monotonic())))
                if ready:
                    data = os.read(adapter.stdout.fileno(), 65536); require(data, "DAP process closed"); buffer.extend(data)

        def request(command, arguments=None):
            nonlocal sequence
            sequence += 1; message = {"seq": sequence, "type": "request", "command": command, "arguments": arguments or {}}
            data = json.dumps(message).encode(); adapter.stdin.write(f"Content-Length: {len(data)}\r\n\r\n".encode() + data); adapter.stdin.flush()
            while True:
                value = read_message()
                if value.get("type") == "response" and value.get("request_seq") == sequence:
                    require(value["success"], value); return value.get("body", {})

        request("initialize", {"adapterID": "uwvm", "supportsVariableType": True})
        request("attach", {"socketDir": private.name, "debugPermissions": "full", "stopOnEntry": False, "stepLevel": "wasm"})
        request("configurationDone")
        def joined():
            deadline = time.monotonic() + 120
            while True:
                value = request("uwvm/diagnostics")
                require(value.get("threaded"), "private profile identification missing")
                if value["details"]["State"] == "exited": return
                require(time.monotonic() < deadline, value); time.sleep(.02)
        joined()
        path = args.out / "recording.uwsnap"
        saved = request("uwvm/recordSnapshot", {"action": "save", "path": str(path), "stopId": "0"})
        require(saved["saved"] and int(saved["samples"]) >= 2, saved)
        index = request("uwvm/snapshotHistory", {"path": str(path)})
        for row in index["environments"]:
            value = request("uwvm/snapshotHistory", {"kind": row["kind"], "id": row["id"]})
            require(value.get("environment"), value)
        writes = (args.out / "vm.log").read_bytes().count(b"THREAD-ONCE"); require(writes == 2, writes)
        for _ in range(2):
            state = request("uwvm/threadReplay", {"action": "replay", "path": str(path), "stopId": "0", "cursor": "2"})
            require(state["execution"] == "stopped", state)
            threads = request("threads")["threads"]; require(threads, "missing restored participants")
            frames = request("stackTrace", {"threadId": threads[0]["id"]})
            require(frames["stackFrames"], "missing actual restored instruction")
            request("continue", {"threadId": threads[0]["id"]}); joined()
            require((args.out / "vm.log").read_bytes().count(b"THREAD-ONCE") == writes, "external write repeated")
        result["passed"] = True
    except Exception as error:
        result["error"] = repr(error)
    finally:
        if adapter:
            adapter.stdin.close()
            try: adapter.wait(timeout=10)
            except subprocess.TimeoutExpired: adapter.kill(); adapter.wait()
        if server:
            server.terminate()
            try: server.wait(timeout=10)
            except subprocess.TimeoutExpired: server.kill(); server.wait()
        for log in logs: log.close()
        private.cleanup()
        (args.out / "results.json").write_text(json.dumps(result, indent=2))
    print(json.dumps({k: result[k] for k in ("passed", "error") if k in result}), flush=True)
    return int(not result["passed"])


if __name__ == "__main__":
    raise SystemExit(main())
