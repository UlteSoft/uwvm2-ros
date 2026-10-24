#!/usr/bin/env python3
"""Exercise the matching native runtime, broker and DAP adapter on Linux.

@details Reuse the maintained genuine native-window witness: 400 instruction
rows, actual instruction stepping, stale references and copied/executed bytes.
This is protocol execution evidence, not a claim that VS Code's UI was tested.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT / "test/0018.debugger"))
import run_dap_current_broker as live


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("uwvm", "wat2wasm", "build-receipt", "source-manifest", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=False)
    report = {"passed": False, "scope": "Linux x86_64 real native broker/DAP; VS Code UI untested"}
    session = None
    try:
        assert sys.platform == "linux" and os.uname().machine == "x86_64"
        cg = Path("/sys/fs/cgroup")
        report["cgroup"] = {name: (cg / name).read_text().strip()
                            for name in ("memory.max", "memory.swap.max", "cgroup.procs")}
        assert report["cgroup"]["memory.max"] == "68719476736"
        assert report["cgroup"]["memory.swap.max"] == "0"
        assert str(os.getpid()) in report["cgroup"]["cgroup.procs"].split()
        build = json.loads(args.build_receipt.read_text())
        assert Path(build["runtime"]["path"]).resolve() == args.uwvm.resolve()
        assert live.sha(args.uwvm) == build["runtime"]["sha256"]
        assert any(row["name"] == "vm-build" and row.get("returncode") == 0
                   for row in build["commands"])
        manifest = json.loads(args.source_manifest.read_text())
        identity = hashlib.sha256(json.dumps(manifest, sort_keys=True, separators=(",", ":")).encode()).hexdigest()
        assert "sha256:" + identity == build["source"]["source_identity"]
        for row in manifest:
            if row["path"].startswith("vm/"):
                file = ROOT / row["path"][3:]
                assert file.stat().st_size == row["bytes"] and live.sha(file) == row["sha256"], row["path"]
        report["source_identity"] = build["source"]["source_identity"]
        report["runtime_sha256"] = live.sha(args.uwvm)
        guest = args.out / "native-loop.wasm"
        command = [str(args.wat2wasm), "--debug-names",
                   str(Path(__file__).with_name("live_disassembly.wat")), "-o", str(guest)]
        report["compiler_argv"] = command
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=60)
        (args.out / "wat2wasm.stdout").write_bytes(result.stdout)
        (args.out / "wat2wasm.stderr").write_bytes(result.stderr)
        assert result.returncode == 0
        report["guest_sha256"] = live.sha(guest)
        vm_args = ["--runtime-aot", "-Rct", "0", "-Rllvm-call-stack", "instruction",
                   "-Rllvm-cache-path", "disable", "--run", str(guest)]
        session = live.BrokerSession(args.uwvm, ROOT / "tools/debug/secure_server.py",
                                     ROOT / "tools/debug/dap_adapter.py", vm_args, args.out)
        report["native_window"] = live.native_case(session, guest)
        report["passed"] = True
    except BaseException as error:
        report["failure"] = repr(error)
        raise
    finally:
        if session is not None:
            try:
                report["cleanup"] = session.close()
            except BaseException as error:
                report["passed"] = False
                report["cleanup_failure"] = repr(error)
        (args.out / "result.json").write_text(json.dumps(report, indent=2) + "\n")
    if not report["passed"]:
        raise RuntimeError("Native disassembly qualification or cleanup failed")


if __name__ == "__main__":
    main()
