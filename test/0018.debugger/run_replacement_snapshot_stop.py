#!/usr/bin/env python3
"""Real DBG replacement and snapshot restore under a bounded test cgroup."""

import argparse
import hashlib
import json
from pathlib import Path
import resource
import subprocess
import sys

from run_dap_current_broker import BrokerSession, actual_location, require


def wait_for(client, expected):
    for _ in range(32):
        status = client.evaluate("wait").decode()
        if status.startswith(expected):
            return status
        require(not status.startswith("guest exited:"), "unexpected guest exit", status)
    raise AssertionError((expected, status))


def check(client, folder, body, remove_breakpoint):
    require(b"breakpoint 1 " in client.evaluate("breakwasm 0 1 0"), "entry breakpoint")
    client.evaluate("continue")
    wait_for(client, "stopped: breakpoint")
    paused = actual_location(client)
    require(paused["function"] == 1 and paused["offset"] == 0, "actual stop", paused)
    if remove_breakpoint:
        client.evaluate("delete 1")

    stack = client.request("stackTrace", {"threadId": paused["thread"]})
    old_frame = stack["body"]["stackFrames"][0]["id"]
    update, stale = client.queue([
        ("evaluate", {"expression": f"replace 0 0 1 {body}", "context": "repl"}),
        ("scopes", {"frameId": old_frame}),
    ])
    result = client.response(update)
    require(result["success"] and "generation 2" in result["body"]["result"], "replacement", result)
    require(not client.response(stale)["success"], "old debugger frame survived code update")
    stopped = actual_location(client)
    require(stopped["stop"] > paused["stop"] and
            (stopped["thread"], stopped["function"], stopped["offset"]) ==
            (paused["thread"], paused["function"], paused["offset"]), "same parked worker", stopped)

    require(b"breakpoint 2 " in client.evaluate("breakwasm 0 0 4"), "replacement breakpoint")
    client.evaluate("continue")
    wait_for(client, "stopped: breakpoint")
    updated_frame = actual_location(client)
    require(updated_frame["function"] == 0 and updated_frame["offset"] == 4,
            "actual replacement instruction", updated_frame)
    frames = client.request("stackTrace", {"threadId": updated_frame["thread"]})
    require(bool(frames["body"]["stackFrames"]), "new body's debugger frame", frames)
    document = client.request("uwvm/currentWasmDocument", {})
    require(document["success"], "new body's Wasm instruction positions", document)

    positions = []
    reloaded = updated_frame
    for index in range(2):
        require(b"local 0 i32=9" in client.evaluate(f"locals {reloaded['thread']}"),
                "replacement local layout and value", reloaded)
        archive = folder / f"updated-{index}.uwsnap"
        require(b"wasm-dump saved" in client.evaluate(f"dump wasm full {reloaded['stop']} {archive}"),
                "export full stop")
        restored = client.request("uwvm/restoreSnapshot", {
            "path": str(archive), "stopId": str(reloaded["stop"]), "environment": "current",
        })["body"]
        for _ in range(10):
            if not restored["pending"]:
                break
            restored = client.request("uwvm/restoreSnapshot", {"continue": True})["body"]
        require(restored["applied"] and not restored["pending"], "restore after replacement", restored)
        fresh = actual_location(client)
        require(fresh["stop"] > reloaded["stop"] and
                (fresh["function"], fresh["offset"]) ==
                (updated_frame["function"], updated_frame["offset"]), "restored Wasm location", fresh)
        positions.append(fresh)
        reloaded = fresh
    client.evaluate("delete 2")
    if not remove_breakpoint:
        client.evaluate("delete 1")
    client.evaluate("continue")
    require(wait_for(client, "guest exited:").strip() == "guest exited: 0",
            "restored program must execute the replacement body")
    return {"initial": paused, "updated": stopped, "replacement_entry": updated_frame,
            "restores": positions}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--source-root", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--wat2wasm", default="wat2wasm")
    args = parser.parse_args()
    root = args.source_root.resolve()
    out = args.out.resolve()
    require(out.is_relative_to(root / ".tmp"), "output must be in project .tmp")
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    group = Path("/sys/fs/cgroup") / Path("/proc/self/cgroup").read_text().strip().removeprefix("0::/")
    require((group / "memory.max").read_text().strip() == "68719476736" and
            (group / "memory.swap.max").read_text().strip() == "0", "64 GiB cgroup with no swap")
    out.mkdir(mode=0o700, parents=True)
    source, wasm, body = (out / name for name in ("update.wat", "update.wasm", "body.bin"))
    source.write_text('''(module
  (memory 1)
  (func (result i32) i32.const 7)
  (func (export "_start") call 0 i32.const 9 i32.ne if unreachable end))
''')
    subprocess.run([args.wat2wasm, "--debug-names", str(source), "-o", str(wasm)], check=True)
    body.write_bytes(bytes.fromhex("01 01 7f 41 09 21 00 20 00 0b"))
    executable = args.binary.resolve()
    records = {}
    for name in ("removed", "present"):
        folder = out / name
        folder.mkdir()
        session = BrokerSession(executable, root / "tools/debug/secure_server.py",
                                root / "tools/debug/dap_adapter.py",
                                ["-Raot", "-Rdbg", "-Rct", "0", "-Rllvm-cache-path", "disable",
                                 "--run", str(wasm)], folder, socket_parent=str(root / ".tmp"))
        try:
            records[name] = check(session.connect("wasm"), folder, body, name == "removed")
        except Exception as error:
            records[name] = {"error": repr(error)}
        finally:
            try:
                session.close()
            except Exception as error:
                records[name]["cleanup_error"] = repr(error)
    with executable.open("rb") as stream:
        sha256 = hashlib.file_digest(stream, "sha256").hexdigest()
    (out / "results.json").write_text(json.dumps({
        "cgroup": str(group), "binary_sha256": sha256, "rows": records,
    }, indent=2) + "\n")
    return 1 if any("error" in row or "cleanup_error" in row for row in records.values()) else 0


if __name__ == "__main__":
    sys.exit(main())
