#!/usr/bin/env python3
"""Exercise the Neovim commands with genuine nvim-dap and a paused, owned VM."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

from run_scene_dap_live import ROOT, WAT, BrokerSession, gate


def digest(path):
    with path.open("rb") as source:
        return hashlib.file_digest(source, "sha256").hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--nvim-dap", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--audit", action="store_true", help="also require the process-wide audit query")
    parser.add_argument("--policy", action="store_true", help="require two pages of actual import rules")
    args = parser.parse_args()
    gate()
    out, plugin, runtime = args.output.resolve(), args.nvim_dap.resolve(), args.runtime.resolve()
    upstream = subprocess.check_output(["git", "-C", plugin, "rev-parse", "HEAD"], text=True).strip()
    if upstream != "04ce47fc5a6ef2b717f33c320fc003091cebac40" or subprocess.check_output(
        ["git", "-C", plugin, "status", "--porcelain", "--", "lua"]):
        raise SystemExit("Use a clean Lua checkout from upstream nvim-dap 0.9.0")
    out.mkdir(parents=True, exist_ok=False)
    socket_parent = next(p for p in out.parents if p.name == ".tmp")
    wat, wasm = out / "scene.wat", out / "scene.wasm"
    wat.write_text(WAT)
    subprocess.run(["wat2wasm", "--debug-names", str(wat), "-o", str(wasm)], check=True)
    flags = ["--runtime-debug", "--debug-permissions", "full", "--runtime-compile-threads", "0",
             "--runtime-llvm-jit-cache-path", "disable", "--wasm-set-main-module-name", "main",
             "--ros-scene-create", "robot", "--ros-scene-bind", "robot", "main"]
    for operation in ["capability.open", "guard.create", "guard.trigger"]:
        flags += ["--ros-scene-interface", "robot", operation, "allow"]
    flags += ["--ros-scene-grant", "robot", "signal", "guard", "signal", "local",
              "guard.create,guard.trigger,resource.observe", "--run", str(wasm)]
    script = Path(__file__).with_name("scene_neovim_live.lua")
    paths = [runtime, ROOT/"tools/debug/dap_adapter.py", ROOT/"tools/debug/neovim/uwvm.lua",
             ROOT/"tools/debug/secure_server.py", script, wasm]
    receipt = {"complete": False, "nvim_dap_commit": upstream,
               "cgroup": Path("/proc/self/cgroup").read_text().strip(),
               "sha256": {str(p): digest(p) for p in paths}}
    broker = BrokerSession(runtime, paths[3], paths[1], flags, out, socket_parent=socket_parent)
    try:
        setup = broker.connect("wasm")
        assert b"breakpoint" in setup.evaluate("breakwasm 0 3 0")
        setup.evaluate("continue")
        assert b"stopped: breakpoint" in setup.evaluate("wait")
        setup.close()
        env = {**os.environ, "UWVM_NVIM_ROOT": str(ROOT), "UWVM_NVIM_DAP": str(plugin),
               "UWVM_NVIM_SOCKET": str(broker.directory), "UWVM_NVIM_CAPTURE": str(out/"queries.json"),
               "UWVM_NVIM_AUDIT": "1" if args.audit else "0", "UWVM_NVIM_POLICY": "1" if args.policy else "0"}
        for key in ("CONFIG", "CACHE", "STATE", "DATA"):
            env["XDG_"+key+"_HOME"] = str(out / key.lower())
        with (out/"neovim.log").open("wb") as log:
            subprocess.run(["nvim", "--headless", "-u", "NONE", "-l", str(script)], cwd=ROOT,
                           env=env, stdout=log, stderr=subprocess.STDOUT, check=True, timeout=75)
        rows = json.loads((out/"queries.json").read_text())
        expected = {"authority", "resources"}
        if args.audit:
            expected.add("audit")
        if args.policy:
            expected.update({"policy", "policyNext"})
        assert set(rows) == expected
        assert len({r["stopId"] for r in rows.values()}) == 1
        receipt["pages"] = rows
    finally:
        receipt["cleanup"] = broker.close()
        (out/"results.json").write_text(json.dumps(receipt, indent=2)+"\n")
    receipt["complete"] = True
    (out/"results.json").write_text(json.dumps(receipt, indent=2)+"\n")


if __name__ == "__main__":
    main()
