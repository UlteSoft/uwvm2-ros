#!/usr/bin/env python3
"""Exercise real configuration rejection in an isolated project under .tmp."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[2]
    out = args.out.resolve()
    if not out.is_relative_to(root / ".tmp"):
        raise SystemExit("configuration artifacts must stay under uwvm2-ros/.tmp")
    relative = Path("/proc/self/cgroup").read_text().strip().split("0::", 1)[1]
    group = Path("/sys/fs/cgroup") / relative.lstrip("/")
    if ((group / "memory.max").read_text().strip() != str(64 * 1024**3)
            or (group / "memory.swap.max").read_text().strip() != "0"
            or 0 in os.sched_getaffinity(0)):
        raise SystemExit("64 GiB, zero-swap cgroup and affinity excluding CPU 0 required")
    source = out / "source"
    source.mkdir(parents=True, exist_ok=True)
    for item in root.iterdir():
        if item.name in {".git", ".tmp", ".xmake", "build", "xmake", "xmake.lua"}:
            continue
        destination = source / item.name
        if not destination.exists():
            destination.symlink_to(item, target_is_directory=item.is_dir())
    shutil.copy2(root / "xmake.lua", source / "xmake.lua")
    shutil.copytree(root / "xmake", source / "xmake", dirs_exist_ok=True)
    cases = [
        ("--import-ros1=n", "--import-ros1=false"),
        ("--import-ros2=n", "--import-ros2=false"),
        ("--disable-local-imported-wasip1=y", "--disable-local-imported-wasip1"),
        ("--enable-local-imported-wasip1-wasm64=n", "--enable-local-imported-wasip1-wasm64=false"),
        ("--enable-local-imported-wasip1-socket=none", "--enable-local-imported-wasip1-socket"),
        ("--enable-local-imported-wasip1-socket=wasix", "--enable-local-imported-wasip1-socket"),
    ]
    results = []
    for flag, expected in cases:
        command = ["xmake", "f", "-P", str(source), "-c", "--yes", flag]
        completed = subprocess.run(command, cwd=source, stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT, text=True, timeout=90)
        name = flag.lstrip("-").replace("=", "-")
        log = out / (name + ".log")
        log.write_text(completed.stdout)
        passed = (completed.returncode != 0
                  and "uwvm2-ros configuration rejected" in completed.stdout
                  and expected in completed.stdout)
        results.append(dict(command=command, exit=completed.returncode,
                            passed=passed, log=str(log)))
        print(flag, "PASS" if passed else "FAIL", flush=True)
    report = dict(results=results, source_policy_sha256=hashlib.sha256(
        (root / "xmake/utility/ros_product.lua").read_bytes()).hexdigest())
    (out / "report.json").write_text(json.dumps(report, indent=2))
    raise SystemExit(not all(result["passed"] for result in results))

if __name__ == "__main__":
    main()
