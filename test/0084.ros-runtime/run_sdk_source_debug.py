#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
"""Verify an SDK-produced guest at two original source lines through live DAP."""
import argparse
import json
from pathlib import Path
import sys

from run_scene_vm import ROOT, sha, verify_gate

sys.path.insert(0, str(ROOT / "test/0018.debugger"))
from run_dap_current_broker import BrokerSession, actual_location


def wait_for_stop(client, reason):
    """@brief Wait for a new stop of the original test process."""
    for _ in range(30):
        reply = client.evaluate("wait")
        if reason in reply:
            return
        if b"guest exited:" in reply:
            raise ValueError(reply.decode())
    raise RuntimeError("The guest did not reach the requested debugger stop")


def exact_frame(client, current, source, line):
    """@brief Resolve a precise original-file position in the actual stack."""
    frames = client.request("stackTrace", {"threadId": current["thread"]})["body"]["stackFrames"]
    for frame in frames:
        if frame.get("source", {}).get("path") == str(source) and frame.get("line") == line:
            if frame.get("uwvmSourceExact") is not True:
                raise ValueError("The source position is approximate")
            return frame
    raise ValueError(f"Missing exact frame for {source}:{line}: {frames}")


def main():
    """@brief Check a source breakpoint, next, copied values and stale-reference refusal."""
    p = argparse.ArgumentParser(description=__doc__)
    for name in ("runtime", "guest", "source", "output", "socket-parent"):
        p.add_argument("--" + name, required=True, type=Path)
    p.add_argument("--break-line", required=True, type=int)
    p.add_argument("--next-line", required=True, type=int)
    p.add_argument("--value", action="append", default=[], metavar="NAME=VALUE")
    p.add_argument("--vm-option", action="append", default=[], help="repeat as --vm-option=ARG in declared order")
    p.add_argument("--source-map", type=Path, help="DAP source root mapping in JSON")
    a = p.parse_args()
    verify_gate()
    source = a.source.resolve(strict=True)
    line_count = len(source.read_text().splitlines())
    if not (1 <= a.break_line <= line_count and 1 <= a.next_line <= line_count) or a.break_line == a.next_line:
        p.error("select two distinct source lines")
    requested_values = {}
    for item in a.value:
        name, separator, value = item.partition("=")
        if not separator or not name or name in requested_values:
            p.error("--value requires a unique NAME=VALUE")
        requested_values[name] = value
    a.output.mkdir(mode=0o700)
    inputs = (source, a.guest.resolve(strict=True), a.runtime.resolve(strict=True), Path(__file__).resolve(),
              ROOT / "tools/debug/secure_server.py", ROOT / "tools/debug/dap_adapter.py")
    pins = {str(path): sha(path) for path in inputs}
    mapping = json.loads(a.source_map.read_bytes()) if a.source_map else None
    if a.source_map:
        pins[str(a.source_map.resolve())] = sha(a.source_map)
    report = {"complete": False, "inputs": pins, "source_map": mapping,
              "cgroup": Path("/proc/self/cgroup").read_text().strip()}
    server = None
    try:
        flags = ["--runtime-aot", "--runtime-debug", "--runtime-compile-threads", "0",
                 "--runtime-llvm-jit-cache-path", "disable", "--debug-permissions", "full",
                 *a.vm_option, "--run", str(a.guest.resolve())]
        server = BrokerSession(a.runtime.resolve(), inputs[4], inputs[5], flags, a.output,
                               socket_parent=str(a.socket_parent.resolve(strict=True)))
        client = server.connect("source", source_map=mapping)
        assert b"prepared; no Wasm instruction executed" in client.evaluate("status")
        spec = {"source": {"path": str(source)}}
        breakpoint = client.request("setBreakpoints", {**spec, "breakpoints": [{"line": a.break_line}]})
        assert breakpoint["body"]["breakpoints"][0]["verified"], breakpoint
        assert b"error:" not in client.evaluate("continue")
        wait_for_stop(client, b"stopped: breakpoint")
        stopped = actual_location(client)
        frame = exact_frame(client, stopped, source, a.break_line)
        report["breakpoint"] = frame
        scopes = client.request("scopes", {"frameId": frame["id"]})["body"]["scopes"]
        for entry in scopes:
            if entry["name"] == "Source variables":
                reference = entry["variablesReference"]
                report["variables"] = client.request("variables", {"variablesReference": reference})["body"]["variables"]
        actual = {row["name"]: row["value"] for row in report.get("variables", [])}
        for name, value in requested_values.items():
            assert actual.get(name) == value, (name, value, actual)
        client.request("setBreakpoints", {**spec, "breakpoints": []})
        client.request("next", {"threadId": stopped["thread"]})
        wait_for_stop(client, b"stopped:")
        moved = actual_location(client)
        assert moved["stop"] > stopped["stop"]
        report["next"] = exact_frame(client, moved, source, a.next_line)
        client.request("scopes", {"frameId": frame["id"]}, success=False)
        client.request("continue", {"threadId": moved["thread"]})
        wait_for_stop(client, b"guest exited: 0")
        assert all(sha(Path(path)) == value for path, value in pins.items())
        report["complete"] = True
    except Exception as error:
        report["failure"] = str(error)
        raise
    finally:
        try:
            if server:
                report["retirement"] = server.close()
        except Exception as error:
            report["complete"] = False
            report["retirement_failure"] = str(error)
            if "failure" not in report:
                raise
        finally:
            (a.output / "results.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
