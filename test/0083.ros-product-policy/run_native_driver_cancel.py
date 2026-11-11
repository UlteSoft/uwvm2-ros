#!/usr/bin/env python3
"""Link a native-driver cancellation fixture against already built product TUs.

The build directory must contain its full official receipt and actual runtime
and host_api objects. This compiles one fixture, never a substitute VM runtime.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import subprocess
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", required=True, type=Path)
    parser.add_argument("--receipt", type=Path,
                        help="Explicit completed build receipt, e.g. an immutable resume-receipt.json")
    parser.add_argument("--library", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    args = parser.parse_args()
    source = Path(__file__).resolve().parent
    repo = source.parents[1]
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    receipt_path = args.receipt if args.receipt is not None else args.build_dir / "receipt.json"
    receipt = json.loads(receipt_path.read_text())
    assert receipt["status"].lower() == "pass", receipt["status"]
    host = next(stage for stage in receipt["stages"] if stage["name"] == "compile-host_api.default")
    main_stage = next(stage for stage in receipt["stages"] if stage["name"] == "compile-main.default")
    link = next(stage for stage in receipt["stages"] if stage["name"] == "link")
    target, obj = out / "native-driver-cancel", out / "native-driver-cancel.o"
    compile_command = list(host["command"])
    compile_command[compile_command.index("-o") + 1] = str(obj)
    compile_command[-1] = str(source / "native_driver_cancel.cc")
    compile_command = ["-O1" if flag == "-O3" else flag for flag in compile_command]
    compile_command += ["-DUWVM2TEST_RUNNER_USE_LLVM_JIT=1", "-DUWVM2TEST_STRICT_NO_INTERPRETER=1"]
    main_object = main_stage["command"][main_stage["command"].index("-o") + 1]
    assert link["command"].count(main_object) == 1, "receipt must link exactly its actual separate product main TU"
    link_command = [str(obj) if value == main_object else value for value in link["command"]]
    link_command[link_command.index("-o") + 1] = str(target)
    report = {"receipt": str(receipt_path.resolve()), "receipt_sha256": hashlib.sha256(receipt_path.read_bytes()).hexdigest(),
              "library": str(args.library.resolve()), "library_sha256": hashlib.sha256(args.library.read_bytes()).hexdigest(),
              "replaced_main_object": main_object, "product_objects": [], "steps": [], "complete": False}
    for stage in receipt["stages"]:
        if stage["name"] not in ("compile-host_api.default", "compile-uwvm_runtime.default"):
            continue
        path = Path(stage["command"][stage["command"].index("-o") + 1])
        if not path.is_absolute():
            path = Path(stage.get("cwd", repo)) / path
        report["product_objects"].append({"path": str(path), "sha256": hashlib.sha256(path.read_bytes()).hexdigest()})
    def save(): (out / "report.json").write_text(json.dumps(report, indent=2))
    for label, command, stage in (("compile", compile_command, host), ("link", link_command, link)):
        log = out / (label + ".log")
        start = time.monotonic()
        cwd = Path(stage.get("cwd", repo))
        with log.open("wb") as output:
            value = subprocess.run(command, cwd=cwd, stdout=output, stderr=subprocess.STDOUT,
                                   env=dict(os.environ, TMPDIR=str(out)), timeout=300)
        report["steps"].append({"name": label, "command": command, "cwd": str(cwd), "exit": value.returncode,
                                "seconds": time.monotonic() - start})
        save()
        assert value.returncode == 0, log.read_text(errors="replace")[-5000:]
    effect, trace = out / "effect", out / "trace"
    for path in (effect, trace): path.unlink(missing_ok=True)
    command = [str(target), str(args.library.resolve())]
    with (out / "stdout").open("wb") as output, (out / "stderr").open("wb") as errors:
        value = subprocess.run(command, cwd=repo, stdout=output, stderr=errors, timeout=40,
                               env=dict(os.environ, UWVM_DRIVER_EFFECT_MARKER=str(effect), UWVM_DRIVER_GATE_TRACE=str(trace)))
    observed = {"exit": value.returncode, "command": command,
                "effect": effect.read_text() if effect.exists() else "", "trace": trace.read_text() if trace.exists() else "",
                "stdout_hex": (out / "stdout").read_bytes().hex(), "stderr": (out / "stderr").read_text(errors="replace")}
    passed = (value.returncode == 0 and observed["effect"] == "BE" and observed["stdout_hex"] == b"safe".hex()
              and "PASS real DSO stable-WASI quota cancellation" in observed["stderr"])
    report.update(result=observed, complete=passed, binary_sha256=hashlib.sha256(target.read_bytes()).hexdigest())
    save()
    assert passed, observed
    print(json.dumps({"pass": True, "report": str(out / "report.json")}))


if __name__ == "__main__":
    main()
