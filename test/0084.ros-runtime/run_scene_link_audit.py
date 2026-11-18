"""Verify visibility refusals from the VM loader before any guest entry point runs."""
import argparse
import json
from pathlib import Path
import subprocess

from run_scene_call_audit import audit
from run_scene_vm import sha, verify_gate


def guest(out, label, name):
    source = out / (label + ".wat")
    binary = source.with_suffix(".wasm")
    escaped = "".join(f"\\{byte:02x}" for byte in name)
    source.write_text(f'(module (import "uwvm_ros_v2" "{escaped}" (func $f (result i32)))'
                      ' (func (export "_start") call $f drop))')
    subprocess.run(["wat2wasm", source, "-o", binary], check=True, timeout=30)
    return binary


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    verify_gate()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    vm = args.runtime.resolve()
    global_rule = audit("global", "*", "denied")
    group_rule = audit("group", "robot", "all")
    bind = ["--ros-scene-bind", "robot", "main"]
    alias = ["--wasm-reset-import", "main", "uwvm_ros_v2", "wasm_alias", "uwvm_ros_v2", "abi"]
    names = {"known": b"abi", "unknown": b"unknown.op", "unicode": 'a"\n机器'.encode(),
             "long": b"f" * 256, "empty": b"", "rewritten": b"wasm_alias"}
    binaries = {key: guest(out, key, value) for key, value in names.items()}
    cases = [
        ("disabled", "known", bind, [], False),
        ("denied-only", "known", bind, global_rule, True),
        ("global-unbound", "known", [], global_rule, True),
        ("wrong-group", "known", [], group_rule, False),
        ("single-hit", "unknown", bind, audit("single", "main", "all"), True),
        ("single-miss", "known", bind, audit("single", "peer", "all"), False),
        ("zero-count", "known", bind, audit("global", "*", "all", 0), False),
        ("zero-bytes", "unknown", bind, audit("global", "*", "all", max_bytes=0), False),
        ("unicode-name", "unicode", bind, group_rule, True),
        ("long-name", "long", bind, group_rule, True),
        ("empty-name", "empty", bind, group_rule, True),
        ("effective-name", "rewritten", bind, global_rule + alias, True),
    ]
    report = dict(vm_sha256=sha(vm), guests={k: sha(v) for k,v in binaries.items()}, cases=[], complete=False)
    target = out / "results.json"
    for engine in ("int", "aot"):
        for label, key, membership, flags, admitted in cases:
            cmd = [vm, "--runtime-" + engine, "--runtime-compile-threads", "0",
                   "--runtime-llvm-jit-cache-path", "disable", "--wasm-set-main-module-name", "main",
                   "--ros-scene-create", "robot", *membership, *flags, "--run", binaries[key]]
            run = subprocess.run(cmd, capture_output=True, timeout=60)
            lines = run.stderr.splitlines()
            rows = [json.loads(line) for line in lines if line.startswith(b'{"ros":"uwvm_ros_v2",')]
            ok = run.returncode > 0 and b"ROS scene interface is hidden" in run.stderr and len(rows) == int(admitted)
            if rows:
                row = rows[0]
                name = b"abi" if key == "rewritten" else names[key]
                ok &= (row["phase"] == "link" and row["reason"] == "hidden" and row["result"] is None and
                       row["generation"] == 0 and row["sequence"] == 1 and row["steady_ns"] > 0 and
                       row["caller_hex"] == b"main".hex() and not row["caller_truncated"] and
                       row["group_hex"] == (b"robot" if membership else b"").hex())
                if key in ("unicode", "long", "empty"):
                    ok &= (row["function"] is None and row["function_bytes"] == len(name) and
                           row["function_truncated"] == (len(name) > 128) and
                           row["function_hex"] == name[:128].hex())
                else:
                    ok &= (row["function"] == name.decode("ascii") and "function_hex" not in row)
            report["cases"].append(dict(engine=engine, case=label, exit=run.returncode, rows=rows, passed=ok))
            target.write_text(json.dumps(report, indent=2) + "\n")
            if not ok:
                raise RuntimeError((engine, label, run.returncode, run.stderr[-1600:]))
    # An ordinary Wasm library may be linked and called. Its own exposed
    # import still checks the immediate runtime ancestry before performing work.
    helper_wat = out / "helper.wat"
    helper_wat.write_text('(module (import "uwvm_ros_v2" "abi" (func $abi (result i32)))'
                          ' (func (export "check") (result i32) call $abi))')
    caller_wat = out / "application.wat"
    caller_wat.write_text('(module (import "helper" "check" (func $check (result i32)))'
                          ' (func (export "_start") call $check i32.const 13 i32.ne if unreachable end))')
    for source in (helper_wat, caller_wat):
        subprocess.run(["wat2wasm", source, "-o", source.with_suffix(".wasm")], check=True, timeout=30)
    report["guests"].update({p.stem:sha(p.with_suffix(".wasm")) for p in (helper_wat, caller_wat)})
    for engine in ("int", "aot"):
        for principal in ("helper", "main"):
            cmd = [vm, "--runtime-"+engine, "--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable",
                   "--wasm-set-main-module-name", "main", "--wasm-preload-library", helper_wat.with_suffix(".wasm"), "helper",
                   "--ros-scene-create", "library", "--ros-scene-bind", "library", "helper",
                   "--ros-scene-interface", "library", "abi", "allow", *audit("single",principal,"denied"),
                   "--run", caller_wat.with_suffix(".wasm")]
            run = subprocess.run(cmd, capture_output=True, timeout=60)
            rows = [json.loads(line) for line in run.stderr.splitlines() if line.startswith(b'{"ros":"uwvm_ros_v2",')]
            ok = run.returncode==0 and len(rows)==int(principal=="helper")
            if rows:
                row=rows[0]
                ok &= (row["phase"]=="return" and row["function"]=="abi" and row["result"]==13 and
                       row["generation"]>0 and row["group_hex"]==b"library".hex() and row["caller_hex"]==b"helper".hex())
            report["cases"].append(dict(engine=engine, case="library-authority", scope=principal,
                                        exit=run.returncode, rows=rows, passed=ok))
            target.write_text(json.dumps(report, indent=2) + "\n")
            if not ok: raise RuntimeError((engine, principal, run.returncode, run.stderr[-1600:]))
    report["complete"] = True
    target.write_text(json.dumps(report, indent=2) + "\n")
    print("passed", len(report["cases"]))


if __name__ == "__main__":
    main()
