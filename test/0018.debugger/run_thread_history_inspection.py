#!/usr/bin/env python3
"""Inspect real thread histories from an ordinary debugger, with no replay.

The VM under inspection is deliberately unrelated to the saved program. No
provider is contacted and no recorded resource is imported into that VM.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path
import re
import subprocess
import sys
from run_dump_launch_policy import VM, require

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
import dap_adapter as dap
import snapshot


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--history", type=Path, action="append", required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--repeat", type=int, default=1, help="repeat selection and paged reads to stress ownership")
    args = parser.parse_args()
    args.out = args.out.resolve(); args.out.mkdir(parents=True, exist_ok=False)
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "resource cgroup required")
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    wat = args.out / "ordinary.wat"; wasm = wat.with_suffix(".wasm")
    wat.write_text('(module (memory 1) (func (export "_start") unreachable))\n')
    subprocess.run(["/usr/bin/wat2wasm", str(wat), "-o", str(wasm)], check=True)
    rows = []
    for permission in ("full", "read-only", "limited"):
        folder = args.out / permission; folder.mkdir(); vm = None
        row = {"permission": permission, "passed": False, "samples": 0, "environments": 0}
        try:
            vm = VM([str(args.binary.resolve()), "--debug-permissions", permission,
                     "--debug-fault-action", "pause", "-Rct", "0", "-Rllvm-cache-path", "disable",
                     "--run", str(wasm)], folder, prefix)
            output = io.BytesIO(); adapter = dap.Adapter(output)
            adapter.broker = vm; adapter.debug_permissions = permission
            sequence = 0

            def request(arguments, success=True):
                nonlocal sequence
                sequence += 1; output.seek(0); output.truncate()
                adapter.handle({"seq": sequence, "command": "uwvm/snapshotHistory", "arguments": arguments})
                output.seek(0); messages = []
                while line := output.readline():
                    length = int(line.split(b":")[1]); require(output.readline() == b"\r\n", "DAP framing")
                    messages.append(json.loads(output.read(length)))
                reply = next(m for m in messages if m["type"] == "response")
                require(reply["success"] == success, reply)
                return reply.get("body", {})

            # This is a real ordinary driver, not the private history driver.
            require("threaded-profile" not in vm.request("status"), "wrong execution driver")
            require(args.repeat > 0, "positive repeat count required")
            for path in args.history * args.repeat:
                image = snapshot.decode(path.read_bytes()); info = image.describe()["threadHistory"]
                if permission == "limited":
                    request({"path": str(path.resolve())}, False)
                    require(vm.request("dump history open 0 " + str(path.resolve())).startswith("error:"),
                            "VM accepted full history at limited permission")
                    continue
                for index, expected in enumerate(info["environmentSamples"]):
                    opened = request({"path": str(path.resolve()), "event": str(index)})
                    require(opened["historical"] and not opened["restoreAuthority"], opened)
                    require(int(opened["event"]) == index, opened)
                    row["samples"] += 1
                    selected = vm.request("dump history")
                    serial = re.search(r"serial=(\d+)", selected)[1]
                    for env in opened["environments"]:
                        value = request({"kind": env["kind"], "id": env["id"]})
                        require(value["environment"] and value["historical"], value)
                        data = value["environment"]
                        original = next(e["data"] for e in expected["environments"]
                                        if e["kind"] == env["kind"] and e["id"] == env["id"])
                        require(data == original, "transferred historical data differs from the saved typed environment")
                        if env["kind"] == "wasip1":
                            require(str(data["module"]) == env["id"], data)
                        else:
                            require(data["backend"] == {"ros1": 1, "ros2": 2}[env["kind"]], data)
                        row["environments"] += 1
                        kind = {"wasip1": 6, "ros1": 7, "ros2": 8}[env["kind"]]
                        require(vm.request(f"dump history {int(serial)+1} {kind} {env['id']} 0 1").startswith("error:"),
                                "stale transfer accepted")
                    corrupt = folder / "truncated.uwsnap"; corrupt.write_bytes(image.data[:-1])
                    request({"path": str(corrupt), "event": "0"}, False)
                    require(vm.request("dump history") == selected, "failed open replaced historical selection")
                    request({"path": str(path.resolve()), "event": str(len(info["environmentSamples"]))}, False)
                    require(vm.request("dump history") == selected, "invalid sample changed historical selection")
            row["passed"] = True
        except Exception as error:
            row["error"] = repr(error)
        finally:
            if vm: row["commands"] = vm.commands; vm.close()
            rows.append(row)
            (args.out / "results.json").write_text(json.dumps(rows, indent=2))
            print(json.dumps({k: v for k, v in row.items() if k != "commands"}), flush=True)
    (args.out / "binary.sha256").write_text(hashlib.sha256(args.binary.read_bytes()).hexdigest() + "\n")
    return int(any(not r["passed"] for r in rows))


if __name__ == "__main__":
    raise SystemExit(main())
