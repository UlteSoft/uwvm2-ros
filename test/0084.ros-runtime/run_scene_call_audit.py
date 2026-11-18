"""Check production result audit scopes, quotas and redaction using real core Wasm calls."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
from run_scene_vm import verify_gate

WAT = '''(module
  (import "uwvm_ros_v2" "abi" (func $abi (result i32)))
  (import "uwvm_ros_v2" "capability.open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros_v2" "capability.close" (func $close (param i64) (result i32)))
  (memory 1)
  (data (i32.const 32) "secret_audit_sentinel")
  (func (export "_start")
    (if (i32.ne (call $abi) (i32.const 2)) (then unreachable))
    (if (i32.ne (call $open (i32.const 0) (i64.const 32) (i64.const 21) (i64.const 128))
                (i32.const 13)) (then unreachable))
    (if (i32.ne (call $close (i64.const 789)) (i32.const 9)) (then unreachable))
    (drop (call $abi))))'''


def audit(scope, name, mode, max_count=10, max_bytes=65536, rate=1000, burst=10):
    return ["--ros-scene-call-audit", scope, name, mode, *map(str, (max_count, max_bytes, rate, burst))]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    wasm = out / "robot.wasm"
    source = out / "robot.wat"
    source.write_text(WAT)
    subprocess.run(["wat2wasm", str(source), "-o", str(wasm)], check=True)
    interfaces = ["--ros-scene-interface","robot","abi","allow","--ros-scene-interface","robot","capability.open","allow",
                  "--ros-scene-interface","robot","capability.close","allow"]
    setup = ["--wasm-set-main-module-name","main","--ros-scene-create","robot","--ros-scene-bind","robot","main",*interfaces]
    all_calls=[("abi",2),("capability.open",13),("capability.close",9),("abi",2)]
    with args.runtime.open("rb") as stream:
        vm_digest=hashlib.file_digest(stream,"sha256").hexdigest()
    report=dict(vm_sha256=vm_digest,
                wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest(),setup=setup,cases=[],complete=False)
    result_file=out/"results.json"
    for engine in ("int","aot"):
        for label,flags,expected in (
            ("default",[],[]),
            ("global",audit("global","*","all"),all_calls),
            ("single",audit("single","main","denied"),all_calls[1:3]),
            ("group",audit("group","robot","all"),all_calls),
            ("other",audit("single","peer","all"),[]),
            ("caps",audit("global","*","all",1)+audit("single","main","all"),all_calls[:1]),
            ("intersection",audit("group","robot","all")+audit("single","main","denied"),all_calls[1:3]),
            ("zero-bytes",audit("group","robot","all",10,0),[]),
            ("zero-rate",audit("global","*","all",rate=0),[]),
            ("zero-burst",audit("global","*","all",burst=0),[]),
        ):
            cmd=[str(args.runtime),"--runtime-"+engine,"--runtime-compile-threads","0","--runtime-llvm-jit-cache-path","disable",
                 *setup,*flags,"--run",str(wasm)]
            run=subprocess.run(cmd,capture_output=True,timeout=60)
            data=run.stderr.decode(errors="replace")
            rows=[json.loads(s) for s in data.splitlines() if s.startswith('{"ros":"uwvm_ros_v2",')]
            passed=(run.returncode==0 and [(r["function"],r["result"]) for r in rows]==expected
                    and [r["sequence"] for r in rows]==list(range(1,len(rows)+1))
                    and all(r["group_hex"]=="robot".encode().hex() and r["caller_hex"]=="main".encode().hex()
                            and r["generation"]>0 and r["steady_ns"]>0 and r["phase"]=="return" for r in rows)
                    and "secret_audit_sentinel" not in data)
            report["cases"].append(dict(engine=engine,case=label,flags=flags,exit=run.returncode,rows=rows,passed=passed))
            result_file.write_text(json.dumps(report,indent=2)+"\n")
            if not passed:raise RuntimeError((label,expected,data[-3000:]))
    for label,flags in [("missing-group",audit("group","missing","all")),("invalid-mode",audit("global","*","off")),
                        ("ambiguous",audit("single","main","all")*2),("unbounded",audit("global","*","all","unlimited"))]:
        run=subprocess.run([args.runtime,*setup,*flags,"--ros-scene-check"],capture_output=True,timeout=30)
        if run.returncode==0:raise RuntimeError(label)
        report["cases"].append(dict(case=label,exit=run.returncode,passed=True))
    report["complete"]=True
    result_file.write_text(json.dumps(report,indent=2)+"\n")
    print("passed",len(report["cases"]))


if __name__=="__main__":
    main()
