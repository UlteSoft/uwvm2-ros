#!/usr/bin/env python3
"""Real authenticated VM channel and DAP restore; run inside the resource cgroup.

No synthetic replies or snapshot authority. Leaves all evidence under --out.
The VM's prior source image, current policy and genuine stopped workers remain
the independent authorities; an archive only supplies candidate state.
"""
import argparse
import hashlib
import io
import json
from pathlib import Path
import re
import subprocess
import struct
import sys
import time
from run_dump_launch_policy import VM, require

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
import dap_adapter as dap


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--native-inspection", action="store_true")
    parser.add_argument("--restore-from", type=Path)
    args = parser.parse_args()
    args.binary = args.binary.resolve()
    args.out = args.out.resolve()
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "resource cgroup required")
    args.out.mkdir(parents=True, exist_ok=False)
    wat = args.out / "restore.wat"
    wasm = wat.with_suffix(".wasm")
    wat.write_text('''(module (memory 1) (global $counter (mut i32) (i32.const 0))
      (func (export "_start") (local $x i32)
        (loop $again
          global.get $counter i32.const 1 i32.add global.set $counter
          i32.const 0 global.get $counter i32.store
          global.get $counter local.set $x
          br $again)))\n''')
    subprocess.run(["/usr/bin/wat2wasm", "--debug-names", str(wat), "-o", str(wasm)], check=True)
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    result = {"passed": False, "restores": [], "dap": []}
    vm = None
    try:
        vm = VM([str(args.binary), "-Raot", "-Rdbg", "-Rct", "0", "-Rllvm-cache-path", "disable",
                 "--run", str(wasm)], args.out, prefix)
        require(vm.request("dump policy") == "dump-policy max=full effective=full\n", "default disclosure")
        initial = vm.request("status")
        require(initial.startswith("prepared; no Wasm instruction executed\n") or "stopped:" in initial, initial)
        require(not vm.request("breakwasm 0 0 0").startswith("error:"), "entry breakpoint")
        vm.request("continue")
        def stopped():
            until = time.monotonic()+120
            while True:
                reply = vm.request("status")
                if re.search(r"^thread [1-9]", reply, re.M): return reply
                require(time.monotonic() < until, reply)
                time.sleep(.01)
        status = stopped()
        stop = re.search(r"stop-id ([1-9][0-9]*)", status)[1]
        full = args.out / "full.uwsnap"
        diagnostic = args.out / "diagnostic.uwsnap"
        require("wasm-dump saved" in vm.request(f"dump wasm full {stop} {full}"), "full export")
        require("wasm-dump saved" in vm.request(f"dump wasm diagnostic {stop} {diagnostic}"), "diagnostic export")
        if args.restore_from:
            full = args.restore_from.resolve()
            result["restoreFrom"] = str(full)
        output = io.BytesIO()
        adapter = dap.Adapter(output)
        adapter.broker = vm
        adapter.debug_permissions = "full"
        adapter.configuration_complete = True
        adapter.observe(status, notify=False)
        seq = 0
        def request(command, arguments):
            nonlocal seq
            seq += 1
            output.seek(0); output.truncate()
            adapter.handle({"seq": seq, "command": command, "arguments": arguments})
            output.seek(0); messages = []
            while line := output.readline():
                length = int(line.split(b":")[1]); require(output.readline() == b"\r\n", "DAP framing")
                messages.append(json.loads(output.read(length)))
            result["dap"].append({"command": command, "messages": messages})
            return next(item for item in messages if item["type"] == "response")
        refused = request("uwvm/restoreSnapshot", {"path": str(full), "stopId": str(int(stop)+1), "environment": "current"})
        require(not refused["success"] and not adapter.snapshot_restore_pending,"stale stop must decline without locking DAP")
        require(f"stop-id {stop}\n" in adapter.command("status"),"stale-stop rejection must permit fresh observation")
        refused = request("uwvm/restoreSnapshot", {"path": str(diagnostic), "stopId": stop, "environment": "current"})
        require(not refused["success"] and not adapter.snapshot_restore_pending, "diagnostic restore must decline before retirement")
        require(f"stop-id {stop}\n" in vm.request("status"), "refusal must preserve original stop")
        # A valid outer checksum must not permit unimplemented environment or
        # replay records to vanish silently while only Wasm state is restored.
        for kind in (7, 8, 9):
            candidate = bytearray(full.read_bytes()[:-48])
            count = struct.unpack_from("<Q", candidate, 56)[0]
            struct.pack_into("<Q", candidate, 56, count+1)
            candidate += struct.pack("<HB5xQQQ", kind, 0, 0, 1, 0)
            candidate += b"UWSEND1\0" + struct.pack("<Q", len(candidate)+48)
            candidate += hashlib.sha256(candidate).digest()
            unsupported = args.out / f"unsupported-{kind}.uwsnap"
            unsupported.write_bytes(candidate)
            refused = request("uwvm/restoreSnapshot", {"path": str(unsupported), "stopId": stop, "environment": "current"})
            require(not refused["success"] and not adapter.snapshot_restore_pending, "unadapted state must decline before retirement")
            require("phase=4 detail=8" in refused.get("message", ""), refused)
            require(f"stop-id {stop}\n" in vm.request("status"), "unsupported state must preserve original stop")
        # A fully valid, genuinely collected archive with different original
        # Wasm reaches native preparation (not just the outer decoder). Its
        # rejection must not leave DAP permanently stuck in "restoring".
        foreign_dir=args.out/"different-source";foreign_dir.mkdir()
        foreign_wat=foreign_dir/"different.wat";foreign_wasm=foreign_wat.with_suffix(".wasm")
        foreign_wat.write_text(wat.read_text().replace("i32.const 1 i32.add","i32.const 2 i32.add"))
        subprocess.run(["/usr/bin/wat2wasm","--debug-names",str(foreign_wat),"-o",str(foreign_wasm)],check=True)
        foreign=None
        try:
            foreign=VM([str(args.binary),"-Raot","-Rdbg","-Rct","0","-Rllvm-cache-path","disable",
                "--run",str(foreign_wasm)],foreign_dir,prefix)
            require(not foreign.request("breakwasm 0 0 0").startswith("error:"),"different-source entry breakpoint")
            foreign.request("continue");until=time.monotonic()+120
            while True:
                foreign_status=foreign.request("status")
                if re.search(r"^thread [1-9]",foreign_status,re.M):break
                require(time.monotonic()<until,foreign_status);time.sleep(.01)
            foreign_stop=re.search(r"stop-id ([1-9][0-9]*)",foreign_status)[1]
            foreign_dump=foreign_dir/"full.uwsnap"
            require("wasm-dump saved" in foreign.request(f"dump wasm full {foreign_stop} {foreign_dump}"),"different-source snapshot")
        finally:
            if foreign:
                result["differentSourceCommands"]=foreign.commands;foreign.close()
        refused=request("uwvm/restoreSnapshot",{"path":str(foreign_dump),"stopId":stop,"environment":"current"})
        require(not refused["success"] and not adapter.snapshot_restore_pending,"native preflight refusal must release DAP pending state")
        require("phase=6 detail=11" in refused.get("message","") and "retained=1" in refused["message"],refused)
        refreshed=adapter.command("status")
        require(f"stop-id {stop}\n" in refreshed,"native preflight refusal must retain the original stop")
        adapter.observe(refreshed,notify=False)
        for _ in range(2):
            thread = adapter.threads[0]["id"] if adapter.threads else int(re.search(r"thread ([1-9][0-9]*)", vm.request("status"))[1])
            frames = request("stackTrace", {"threadId": thread})
            require(frames["success"] and frames["body"]["stackFrames"], frames)
            stale_frame = frames["body"]["stackFrames"][0]["id"]
            restored = request("uwvm/restoreSnapshot", {"path": str(full), "stopId": stop, "environment": "current"})
            for _ in range(10):
                if not restored.get("body", {}).get("pending"): break
                restored = request("uwvm/restoreSnapshot", {"continue": True})
            require(restored["success"] and restored["body"]["applied"] and not restored["body"]["pending"], restored)
            require(int(restored["body"]["stopId"]) > int(stop), "fresh stopped identity")
            stop = restored["body"]["stopId"]
            require(not request("scopes", {"frameId": stale_frame})["success"], "retired frame accepted")
            thread = adapter.threads[0]["id"]
            bt = vm.request(f"bt {thread}")
            require(re.search(r"#0 module=0 function=0", bt), bt)
            frames = request("stackTrace", {"threadId": thread})
            require(frames["success"] and frames["body"]["stackFrames"], frames)
            scopes = request("scopes", {"frameId": frames["body"]["stackFrames"][0]["id"]})
            require(scopes["success"], scopes)
            result["restores"].append({"stop": stop, "backtrace": bt, "scopes": scopes})
        repeat = args.out / "restored.uwsnap"
        require("wasm-dump saved" in vm.request(f"dump wasm full {stop} {repeat}"), "restored export")
        wasm_document = request("uwvm/currentWasmDocument", {})
        require(wasm_document["success"], wasm_document)
        if args.native_inspection:
            preview = vm.request(f"disassemble {thread} {stop} 2")
            require("native-disassembly stop=" in preview, preview)
            captured = request("uwvm/nativeEntry", {"ifStop": stop})
            require(captured["success"] and captured["body"]["captured"], captured)
            stop = captured["body"]["stopId"]
            registers = vm.request(f"info registers {thread} {stop}")
            require("native-registers end" in registers and not registers.startswith("error:"), registers)
            result["nativeInspection"] = {"preview": preview, "registers": registers}
        result["passed"] = True
    except Exception as error:
        result["error"] = repr(error)
    finally:
        if vm:
            result["commands"] = vm.commands
            vm.close()
        (args.out / "results.json").write_text(json.dumps(result, indent=2))
    print(json.dumps({key: value for key, value in result.items() if key not in ("commands", "dap", "restores")}), flush=True)
    require(result["passed"], "real VM restoration failed; inspect results.json")


if __name__ == "__main__": main()
