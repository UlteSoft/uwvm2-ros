#!/usr/bin/env python3
"""Replay the canonical current C adapter contract with the real xmake VM."""
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys

if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
    raise RuntimeError("shared admitted Linux finite owner required")
run = Path(os.environ["UWVM_RESOURCE_RUN"])
inputs = run / "inputs"
work = run / "work"
xmake = Path("/home/macromodel/.local/bin/xmake")
commands = []
count = None
for label, argv in (
    ("version", [xmake, "--version"]),
    ("contract", [xmake, "lua", inputs / "xmake_contract.lua", inputs / "c.lua"]),
):
    result = subprocess.run([str(value) for value in argv], cwd=work,
                            stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, timeout=20, check=False)
    (work / (label + ".stdout")).write_bytes(result.stdout)
    (work / (label + ".stderr")).write_bytes(result.stderr)
    commands.append({"argv": [str(value) for value in argv], "exit_code": result.returncode})
    if result.returncode:
        raise RuntimeError("actual xmake failed " + label)
    if label == "contract":
        found = re.search(rb"C_XMAKE_CONTRACT_CHECKS=(\d+)", result.stdout)
        count = int(found[1]) if found else None
        if count != 23:
            raise RuntimeError("exact canonical23-check contract denominator required")
report = {"format": "task02-current-c-lua-contract-v1", "status": "PASS_CURRENT_ADAPTER_SCOPE",
          "checks": count, "commands": commands,
          "input_sha256": {name: hashlib.sha256((inputs / name).read_bytes()).hexdigest()
                           for name in ("c.lua", "xmake_contract.lua")},
          "xmake_sha256": hashlib.sha256(xmake.read_bytes()).hexdigest(),
          "genuine_lua_formatter_credit": False, "compiler_runs": 0, "guest_runs": 0,
          "default_authority": [], "public_sdk_published": False}
(work / "lua-contract.json").write_text(json.dumps(report, indent=2) + "\n")
print(json.dumps({"status": report["status"], "checks": count}), flush=True)
