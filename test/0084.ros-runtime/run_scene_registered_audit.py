"""Compare every registered ROSv2 import with the operator audit on its actual VM return.

Each guest calls every denied operation with the documented signature and verifies
memory canaries after each call. Comparing the audit sequence with the import list
also detects a dispatcher that calls a different, equally denied host function.
"""
import argparse
import json
import subprocess
from pathlib import Path

from run_scene_all_import_denials import denied_guest, exposure
from run_scene_call_audit import audit
from run_scene_vm import ROOT, sha, verify_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    verify_gate()
    vm = args.runtime.resolve()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)

    catalogue = ROOT / "tools/ros_scene/interface-catalogue.json"
    rows = [row for row in json.loads(catalogue.read_bytes())["operations"]
            if row["implementation"] == "registered_local"]
    names = [row["name"] for row in rows]
    configured = subprocess.run([vm, "--ros-scene-dump-effective-policy"], capture_output=True,
                                timeout=30, check=True)
    assert sorted(names) == json.loads(configured.stdout)["registeredOperations"]
    expected = [name for name in names for _ in range(1 if name == "abi" else 2)]
    limit = len(expected)
    policy = exposure(rows, "deny", "main")
    flags = ["--wasm-set-main-module-name", "main", "--runtime-compile-threads", "0",
             "--runtime-llvm-jit-cache-path", "disable", *policy,
             *audit("single", "main", "denied", limit, limit * 1024, limit, limit)]
    receipt = dict(complete=False, vm_sha256=sha(vm), catalogue_sha256=sha(catalogue),
                   operations=names, cases=[])
    result = output / "results.json"
    result.write_text(json.dumps(receipt, indent=2))
    for width, index in [(32, 0), (64, 0), (32, 1), (64, 1)]:
        wat = output / f"w{width}-m{index}.wat"
        wasm = wat.with_suffix(".wasm")
        wat.write_text(denied_guest(rows, width, index, "_start"))
        subprocess.run(["wat2wasm", "--enable-memory64", "--enable-multi-memory", wat, "-o", wasm],
                       check=True, timeout=60)
        for engine in ["int", "aot"]:
            proc = subprocess.run([vm, "--runtime-" + engine, *flags, "--run", wasm],
                                  capture_output=True, timeout=120)
            log = output / f"w{width}-m{index}-{engine}.stderr"
            log.write_bytes(proc.stderr)
            events = [json.loads(line) for line in proc.stderr.splitlines()
                      if line.startswith(b'{"ros":"uwvm_ros_v2",')]
            passed = (proc.returncode == 0 and [row["function"] for row in events] == expected
                      and all(row["result"] == 13 and row["phase"] == "return" and row["generation"] > 0
                              and row["caller_hex"] == b"main".hex() and row["group_hex"] == b"denied-group".hex()
                              and row["sequence"] == i for i, row in enumerate(events, 1)))
            receipt["cases"].append(dict(width=width, memory=index, engine=engine, exit=proc.returncode,
                                         calls=len(events), wasm_sha256=sha(wasm), audit_sha256=sha(log), passed=passed))
            result.write_text(json.dumps(receipt, indent=2))
            if not passed:
                raise RuntimeError(f"unexpected registered call result or audit sequence; see {log}")
    receipt["complete"] = True
    result.write_text(json.dumps(receipt, indent=2) + "\n")


if __name__ == "__main__":
    main()
