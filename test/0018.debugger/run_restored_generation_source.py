#!/usr/bin/env python3
"""Check original DWARF and effective Wasm documents across a real DBG restore."""

import argparse
import json
from pathlib import Path
import re
import resource
import subprocess
import sys

from run_dap_current_broker import BrokerSession, actual_location, require, sha
from run_debug_source_inline_metadata_cli import function
from run_replacement_snapshot_stop import wait_for


TARGET = """int target(void) {
    return 7;
}
"""
DRIVER = """extern int target(void);
volatile int observed;
void _start(void) {
    observed = target();
    if (observed != 7) __builtin_trap();
    observed = target();
    if (observed != 9) __builtin_trap();
}
"""


def source_breakpoints(client, path, lines):
    return client.request("setBreakpoints", {"source": {"path": str(path)},
        "breakpoints": [{"line": line} for line in lines]})["body"]["breakpoints"]


def wasm_breakpoint(client, target, offset):
    response = client.evaluate(f"breakwasm 0 {target} {offset}")
    match = re.search(rb"breakpoint ([0-9]+) ", response)
    require(match, "Wasm breakpoint", response)
    return int(match[1])


def replacement_document(client, target, offset, target_file):
    location = actual_location(client)
    require((location["function"], location["offset"]) == (target, offset), "actual replaced callee", location)
    source = client.request("uwvm/currentWasmDocument")["body"]["source"]
    require(source["path"].endswith(f"code-{location['epoch']}-2.uwvm-wat"), "effective document identity", source)
    content = client.request("source", {"sourceReference": source["sourceReference"], "source": source})["body"]["content"]
    instructions = re.findall(r"(?m)^;; 0x([0-9a-f]+)\s+([.a-z0-9_]+)([^\n]*)$", content)
    decoded = [(int(offset, 16), op, operand.strip()) for offset, op, operand in instructions]
    require(decoded == [(0, "i32.const", "9"), (2, "local.set", "0"), (4, "local.get", "0"), (6, "end", "")],
            "exact replacement instruction offsets and operands", decoded)
    positions = client.request("uwvm/positions")["body"]
    require(positions["stopId"] == str(location["stop"]) and positions["source"] is None and
            positions["wasm"] == {"module": "0", "function": str(target), "offset": str(offset), "exact": True},
            "current Wasm position with optional native landing", positions)
    denied = client.request("uwvm/codeLocation", {"level": "source"}, success=False)
    require("No source position" in denied["message"], "retired source navigation", denied)
    require(b"local 0 i32=" in client.evaluate(f"locals {location['thread']}"), "actual replacement Wasm local")
    locals_reply = client.request("evaluate", {"context": "repl",
        "expression": f"locals source {location['thread']} {location['stop']} 0"}, success=False)
    require("result" not in locals_reply.get("body", {}), "original source locals unavailable", locals_reply)
    retired = source_breakpoints(client, target_file, [2])
    require(len(retired) == 1 and not retired[0]["verified"], "retired source breakpoint", retired)
    return {"location": location, "document": source, "content": content, "source_breakpoint": retired[0],
            "native_position": positions["asm"]}


def check(client, out, guest, body, target_file, driver_file, report):
    target, _ = function(guest, "target")
    driver, _ = function(guest, "_start")
    original = source_breakpoints(client, target_file, [2])
    require(len(original) == 1 and original[0]["verified"], "original target DWARF line", original)
    client.evaluate("continue")
    wait_for(client, "stopped: breakpoint")
    active = actual_location(client)
    original_source = client.request("uwvm/codeLocation", {"level": "source"})["body"]
    require(active["function"] == target and original_source["source"]["path"] == str(target_file) and
            original_source["line"] == 2 and original_source["exact"], "original target's exact DWARF statement", original_source)
    report["original"] = {"location": active, "source": original_source}
    source_breakpoints(client, target_file, [])
    caller = source_breakpoints(client, driver_file, [6])
    require(len(caller) == 1 and caller[0]["verified"], "caller before second target invocation", caller)
    client.evaluate("continue")
    wait_for(client, "stopped: breakpoint")
    require(actual_location(client)["function"] == driver, "caller parked before update")
    update = client.evaluate(f"replace 0 {target} 1 {body}")
    require(b"function replaced; generation 2" in update, "first publication", update)
    source_breakpoints(client, driver_file, [])
    target_point = wasm_breakpoint(client, target, active["offset"])
    client.evaluate("continue")
    wait_for(client, "stopped: breakpoint")
    report["before"] = replacement_document(client, target, active["offset"], target_file)
    fresh = report["before"]["location"]
    archive = out / "updated.uwsnap"
    require(b"wasm-dump saved" in client.evaluate(f"dump wasm full {fresh['stop']} {archive}"), "save active g2")
    restored = client.request("uwvm/restoreSnapshot", {"path": str(archive),
        "stopId": str(fresh["stop"]), "environment": "current"})["body"]
    for _ in range(10):
        if not restored["pending"]:
            break
        restored = client.request("uwvm/restoreSnapshot", {"continue": True})["body"]
    require(restored["applied"] and not restored["pending"], "full restore applied", restored)
    report["restored"] = replacement_document(client, target, active["offset"], target_file)
    require(report["restored"]["content"] == report["before"]["content"] and
            report["restored"]["location"]["epoch"] > fresh["epoch"], "same bytes across independently bound epochs")
    client.evaluate(f"delete {target_point}")
    source_breakpoints(client, target_file, [])
    caller = source_breakpoints(client, driver_file, [7])
    require(len(caller) == 1 and caller[0]["verified"], "unchanged caller breakpoint", caller)
    client.evaluate("continue")
    wait_for(client, "stopped: breakpoint")
    position = client.request("uwvm/codeLocation", {"level": "source"})["body"]
    require(position["source"]["path"] == str(driver_file) and position["line"] == 7 and position["exact"],
            "restored caller's exact original DWARF statement", position)
    require(actual_location(client)["function"] == driver, "actual unchanged caller")
    report["caller"] = position
    source_breakpoints(client, driver_file, [])
    client.evaluate("continue")
    require(wait_for(client, "guest exited:").strip() == "guest exited: 0", "replacement return value reached caller")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", required=True, type=Path)
    parser.add_argument("--clang", required=True, type=Path)
    parser.add_argument("--source-root", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--out", required=True, type=Path)
    args = parser.parse_args()
    root, out = args.source_root.resolve(strict=True), args.out.resolve()
    require(out.is_relative_to(root / ".tmp"), "receipts belong under project .tmp")
    group = Path("/sys/fs/cgroup") / Path("/proc/self/cgroup").read_text().strip().removeprefix("0::/")
    require((group / "memory.max").read_text().strip() == "68719476736" and
            (group / "memory.swap.max").read_text().strip() == "0", "64 GiB test cgroup with no swap")
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out.mkdir(mode=0o700, parents=True)
    target_file, driver_file, guest, body = (out / name for name in ("target.c", "driver.c", "program.wasm", "body.bin"))
    target_file.write_text(TARGET)
    driver_file.write_text(DRIVER)
    compiler = [str(args.clang.resolve(strict=True)), "--target=wasm32-unknown-unknown", "-O1", "-g",
                "-gdwarf-4", "-nostdlib", "-Wl,--no-entry", "-Wl,--export=_start", "-Wl,--export=target",
                str(target_file), str(driver_file), "-o", str(guest)]
    subprocess.run(compiler, cwd=out, check=True, timeout=30)
    body.write_bytes(bytes.fromhex("01 01 7f 41 09 21 00 20 00 0b"))
    binary = args.binary.resolve(strict=True)
    report = {"passed": False, "binary_sha256": sha(binary), "guest_sha256": sha(guest), "compiler": compiler, "cgroup": str(group)}
    session = BrokerSession(binary, root / "tools/debug/secure_server.py", root / "tools/debug/dap_adapter.py",
                            ["-Raot", "-Rdbg", "-Rct", "0", "-Rllvm-cache-path", "disable", "--run", str(guest)],
                            out, socket_parent=str(root / ".tmp"))
    try:
        check(session.connect("wasm"), out, guest, body, target_file, driver_file, report)
        report["passed"] = True
    except Exception as error:
        report["error"] = repr(error)
    finally:
        try:
            report["retirement"] = session.close()
        finally:
            (out / "result.json").write_text(json.dumps(report, indent=2) + "\n")
    return 0 if report["passed"] else 1


if __name__ == "__main__":
    sys.exit(main())
