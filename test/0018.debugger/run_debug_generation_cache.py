#!/usr/bin/env python3
"""Verify that a live DBG replacement leaves authenticated persistent objects unchanged."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import subprocess

from run_dap_current_broker import BrokerSession

GUEST = """(module
    (import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32) (result i32)))
    (memory 1)
    (data (i32.const 16) "GEN-A\\0aGEN-B\\0a")
    (func $choice (result i32) i32.const 16)
    (func (export "_start")
        i32.const 0 call $choice i32.store
        i32.const 4 i32.const 6 i32.store
        i32.const 1 i32.const 0 i32.const 1 i32.const 8 call $write
        if unreachable end))"""


def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def inventory(cache):
    return {str(p.relative_to(cache)): (sha(p), p.stat().st_mtime_ns)
            for p in sorted(cache.rglob("*.uwvm-ljc"))}


def wait(client, expected):
    for _ in range(16):
        reply = client.evaluate("wait")
        if expected in reply:
            return
        assert b"guest exited:" not in reply, reply
    raise TimeoutError(expected)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", required=True, type=Path)
    parser.add_argument("--wat2wasm", default="wat2wasm")
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--socket-parent", type=Path, default=Path(os.environ.get("TMPDIR", ".")))
    parser.add_argument("--source-root", type=Path, default=Path(__file__).resolve().parents[2])
    args = parser.parse_args()
    binary = args.binary.resolve(strict=True)
    out = args.out.resolve()
    root = args.source_root.resolve(strict=True)
    assert out.is_relative_to(root / ".tmp"), "receipts must live under the project .tmp"
    group = Path("/sys/fs/cgroup") / Path("/proc/self/cgroup").read_text().strip().removeprefix("0::/")
    assert (group / "memory.max").read_text().strip() == "68719476736"
    assert (group / "memory.swap.max").read_text().strip() == "0"
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out.mkdir(parents=True, mode=0o700, exist_ok=False)
    cache = out / "cache"
    cache.mkdir(mode=0o700)
    source, image = out / "guest.wat", out / "guest.wasm"
    source.write_text(GUEST)
    subprocess.run([args.wat2wasm, source, "-o", image], check=True, timeout=20)
    body = out / "choice-b.bin"
    body.write_bytes(bytes.fromhex("00 41 16 0b"))  # No locals, i32.const 22, end.
    flags = ["-Raot", "-Rct", "0", "--wasm-policy-global", "runtime.llvm.policy", "max", "--log-verbose", "--runtime-compiler-log", "err",
             "--runtime-llvm-jit-cache-seal-process", "-Rllvm-cache-path", "path", str(cache),
             "--wasip1-global-enable", "--wasip1-global-interface", "fd_write", "allow",
             "--wasip1-global-grant", "stdout.write", "--run", str(image)]
    report = {"vm_sha256": sha(binary), "guest_sha256": sha(image), "replacement_sha256": sha(body),
              "argv": [str(binary), *flags], "cgroup": str(group), "passed": False}

    def ordinary(name, warm):
        result = subprocess.run([binary, *flags], stdin=subprocess.DEVNULL, capture_output=True, timeout=30)
        (out / (name + ".stdout")).write_bytes(result.stdout)
        (out / (name + ".stderr")).write_bytes(result.stderr)
        assert result.returncode == 0 and result.stdout == b"GEN-A\n", name
        hit = re.search(rb"object-cache-hit [^\n]*signature_verified=1\b", result.stderr)
        store = re.search(rb"object-cache-store-complete [^\n]*status=ok\b", result.stderr)
        assert bool(hit) == warm and bool(store) != warm, name

    try:
        ordinary("cold", False)
        before = inventory(cache)
        assert before
        ordinary("warm", True)
        assert inventory(cache) == before
        session = out / "replacement"
        session.mkdir(mode=0o700)
        tools = root / "tools/debug"
        server = BrokerSession(binary, tools / "secure_server.py", tools / "dap_adapter.py",
                               ["-Rdbg", "--debug-permissions", "full", *flags], session,
                               socket_parent=str(args.socket_parent.resolve(strict=True)))
        try:
            client = server.connect("wasm")
            assert b"breakpoint 1 " in client.evaluate("breakwasm 0 2 0")
            client.evaluate("continue")
            wait(client, b"stopped: breakpoint")
            assert b"function replaced; generation 2" in client.evaluate(f"replace 0 1 1 {body}")
            stale = client.request("evaluate", {"expression": f"replace 0 1 1 {body}", "context": "repl"}, success=False)
            assert "function generation changed" in stale["message"], stale
            client.evaluate("delete 1")
            client.evaluate("continue")
            wait(client, b"guest exited: 0")
        finally:
            report["retirement"] = server.close()
        log = (session / "broker.raw").read_bytes()
        assert re.search(rb"^GEN-B$", log, re.M) and b"GEN-A\n" not in log
        assert b"persistent-cache-disabled reason=debugger-active" in log
        assert b"object-cache-hit " not in log
        assert not re.search(rb"object-cache-store-(?:enqueue|complete) [^\n]*status=ok\b", log)
        assert inventory(cache) == before
        ordinary("original-after-debug", True)
        assert inventory(cache) == before
        report.update(passed=True, cached_objects=before)
    finally:
        (out / "result.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
