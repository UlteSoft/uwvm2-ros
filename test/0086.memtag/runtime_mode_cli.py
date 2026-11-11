#!/usr/bin/env python3
"""Real product launch-policy errors versus offline Wasm validation."""
import argparse
import hashlib
import json
from pathlib import Path
import shlex
import subprocess

import vm_matrix as m


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", required=True, type=Path)
    parser.add_argument("--engine", choices=("int", "aot"), required=True)
    parser.add_argument("--runner", default="")
    parser.add_argument("--hardware-supported", action="store_true",
                        help="Explicit expectation for a genuinely OS-enabled AArch64 MTE test profile")
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=False)
    fixture = args.out / "valid.wasm"
    fixture.write_bytes(m.scalars(False))
    base = shlex.split(args.runner) + [str(args.vm.resolve()), "--runtime-" + args.engine,
                                      "--runtime-compile-threads", "0"]
    if args.engine == "aot":
        base += ["--runtime-llvm-jit-cache-path", "disable"]
    cases = [
        ("default-software-sync-crash", [], 0),
        ("explicit-software-sync-crash", ["--runtime-memtag-backend","software","--runtime-memtag-mode","sync","--runtime-memtag-action","crash"],0),
        ("software-sync-silent", ["--runtime-memtag-action","silent-log","err"],0),
        ("software-async-crash", ["--runtime-memtag-mode","async"],0),
        ("software-async-silent", ["--runtime-memtag-mode","async","--runtime-memtag-action","silent-log","err"],0),
        ("removed-silent",["--runtime-memtag-action","silent"],126),
        ("removed-client-typo",["--runtime-memtag-action","client"],126),
        ("removed-slient-log-typo",["--runtime-memtag-action","slient-log","err"],126),
        ("silent-log-missing-sink",["--runtime-memtag-action","silent-log"],126),
        ("silent-log-missing-path",["--runtime-memtag-action","silent-log","path"],126),
        ("silent-log-invalid-target",["--runtime-memtag-action","silent-log","file","bad"],126),
        ("silent-log-empty-path",["--runtime-memtag-action","silent-log","path",""],126),
        ("silent-log-out",["--runtime-memtag-action","silent-log","out"],0),
        ("silent-log-path",["--runtime-memtag-action","silent-log","path",str(args.out/"reports.jsonl")],0),
        ("silent-log-invalid-path",["--runtime-memtag-action","silent-log","path",str(args.out/"absent"/"report.jsonl")],126),
        ("invalid-action",["--runtime-memtag-action","log"],126),
        ("missing-action",["--runtime-memtag-action"],126),
        ("invalid-backend",["--runtime-memtag-backend","unknown"],126),
        ("missing-backend",["--runtime-memtag-backend"],126),
        ("invalid-mode",["--runtime-memtag-mode","unknown"],126),
        ("missing-mode",["--runtime-memtag-mode"],126),
        ("removed-software-async",["--runtime-memtag-mode","software-async"],126),
        ("removed-hardware-async",["--runtime-memtag-mode","hardware-async"],126),
        ("removed-hardware-sync",["--runtime-memtag-mode","hardware-sync"],126),
        ("removed-workers",["--runtime-memtag-workers","4"],126),
        ("removed-report",["--runtime-memtag-report","log"],126),
        ("removed-report-output",["--runtime-memtag-report-output","err"],126),
        ("disabled-async",["--runtime-memtag-mode","async","--runtime-disable-memtag"],126),
        ("disabled-async-reordered",["--runtime-disable-memtag","--runtime-memtag-mode","async"],126),
        ("disabled-hardware",["--runtime-disable-memtag","--runtime-memtag-backend","hardware"],126),
        ("offline-hardware-validation",["--mode","validation","--runtime-memtag-backend","hardware"],0),
        ("invalid-log-path",["--log-output","file",str(args.out/"absent"/"report.jsonl")],126),
    ]
    for mode in ("sync","async"):
        for action in ("crash","silent-log"):
            expected=0 if args.hardware_supported else 126
            cases.append(("hardware-"+mode+"-"+action,["--runtime-memtag-backend","hardware","--runtime-memtag-mode",mode,"--runtime-memtag-action",action]+(["err"] if action=="silent-log" else []),expected))
            cases.append(("hardware-order-"+mode+"-"+action,["--runtime-memtag-action",action]+(["err"] if action=="silent-log" else [])+["--runtime-memtag-mode",mode,"--runtime-memtag-backend","hardware"],expected))
    rows = []
    for name, flags, expected in cases:
        command = base + flags + ["--", str(fixture.resolve())]
        done = subprocess.run(command, capture_output=True, timeout=30)
        output = (done.stdout + done.stderr).decode(errors="replace")
        (args.out / (name + ".log")).write_text(output)
        rows.append(dict(name=name, command=command, exit=done.returncode, expected=expected,
                         passed=done.returncode == expected and (expected == 0 or "[error]" in output)))
    import re
    help_result=subprocess.run(shlex.split(args.runner)+[str(args.vm.resolve()),"--help","all"],capture_output=True,timeout=30)
    help_text=(help_result.stdout+help_result.stderr).decode(errors="replace")
    controls=set(re.findall(r"--(?:runtime-[a-z-]*memtag[a-z-]*|wasm-feature-disable-memtag-extension)",help_text))
    wanted={"--wasm-feature-disable-memtag-extension","--runtime-disable-memtag","--runtime-memtag-mode","--runtime-memtag-action","--runtime-memtag-backend"}
    rows.append(dict(name="exactly-five-help-controls",passed=help_result.returncode==0 and controls==wanted,controls=sorted(controls)))
    report = dict(binary_sha256=hashlib.sha256(args.vm.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),
                  passed=sum(row["passed"] for row in rows), total=len(rows), rows=rows)
    (args.out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({k: report[k] for k in ("passed", "total", "binary_sha256")}))
    for row in rows:
        if not row["passed"]:
            print("FAIL", row["name"], row.get("exit"), "expected", row.get("expected"))
    return int(report["passed"] != report["total"])


if __name__ == "__main__":
    raise SystemExit(main())
