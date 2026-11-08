#!/usr/bin/env python3
"""Record native AArch64/Linux MTE provider prerequisites; never installs a provider."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess

def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", type=Path, required=True)
    parser.add_argument("--cflag", action="append", default=[])
    parser.add_argument("--qemu", type=Path)
    parser.add_argument("--runtime-prefix", type=Path)
    parser.add_argument("--cpu", default="max")
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[3]
    out = args.out.resolve()
    if not out.is_relative_to(root / ".tmp"):
        parser.error("all build and execution artifacts must be below uwvm2-ros/.tmp")
    if args.qemu and not args.runtime_prefix:
        parser.error("--qemu requires --runtime-prefix")
    group_path = Path("/proc/self/cgroup").read_text().split("0::", 1)[1].strip()
    group = Path("/sys/fs/cgroup") / group_path.lstrip("/")
    if ((group/"memory.max").read_text().strip() != str(64 * 1024**3)
            or (group/"memory.swap.max").read_text().strip() != "0"
            or 0 in os.sched_getaffinity(0)):
        parser.error("run inside the 64 GiB zero-swap cgroup, excluding CPU 0")
    out.mkdir(parents=True, exist_ok=True)
    source = Path(__file__).resolve().parent
    records = []
    def command(name, argv, *, env=None):
        result = subprocess.run(argv, text=True, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, timeout=60, env=env)
        (out/(name+".log")).write_text(result.stdout)
        record = dict(name=name, argv=argv, exit=result.returncode, output=result.stdout)
        records.append(record)
        return record
    compiler = str(args.compiler.resolve())
    command("compiler-version", [compiler, "--version"])
    base = [compiler, *args.cflag, "-march=armv8.5-a+memtag", "-O2", "-g",
            "-fuse-ld=lld", "-pthread", "-Wl,--fatal-warnings"]
    control = out/"control.o"
    control_result = command("control-compile",
                             base+["-c", str(source/"control.c"), "-o", str(control)])
    if control_result["exit"]:
        (out/"report.json").write_text(json.dumps(dict(records=records,
            provider_prerequisites_pass=False, full_vm_qualified=False), indent=2)+"\n")
        return 1
    cases = [(kind, action) for kind in ["global", "stack", "heap", "thread-stack",
             "thread-heap", "explicit-thread"] for action in ["good", "bad-read", "bad-write"]]
    cases += [("heap", "uaf"), ("heap", "double-free"), ("lifecycle", "good")]
    builds = {}
    executions = {}
    for profile in ["none", "memtag-stack", "memtag-heap", "memtag-globals", "memtag"]:
        flags = [] if profile == "none" else [
            "-fsanitize="+profile, "-fsanitize-memtag-mode=sync"]
        obj, binary = out/(profile+".o"), out/profile
        compiled = command(profile+"-compile",
                           base+flags+["-c", str(source/"payload.c"), "-o", str(obj)])
        linked = None
        if compiled["exit"] == 0:
            linked = command(profile+"-link",
                             base+flags+[str(control), str(obj), "-o", str(binary)])
        builds[profile] = dict(compile=compiled["exit"],
                              link=linked["exit"] if linked else None)
        if linked is None or linked["exit"]:
            continue
        command(profile+"-dynamic", ["readelf", "-d", str(binary)])
        for tunable in [0, 3]:
            for kind, action in cases:
                name = f"{profile}-t{tunable}-{kind}-{action}"
                env = os.environ.copy()
                env.pop("LD_PRELOAD", None)
                env.pop("LD_LIBRARY_PATH", None)
                if args.qemu:
                    argv = [str(args.qemu.resolve()), "-cpu", args.cpu, "-L",
                            str(args.runtime_prefix.resolve()), "-E",
                            "GLIBC_TUNABLES=glibc.mem.tagging="+str(tunable), str(binary),
                            kind, action]
                    env.pop("GLIBC_TUNABLES", None)
                else:
                    argv = [str(binary), kind, action]
                    env["GLIBC_TUNABLES"] = "glibc.mem.tagging="+str(tunable)
                executions[name] = command(name, argv, env=env)
    def positive(name, marker=None):
        item = executions.get(name, {})
        return item.get("exit") == 0 and (not marker or marker in item.get("output", ""))
    def blocked(name, marker):
        item = executions.get(name, {})
        output = item.get("output", "")
        return item.get("exit") == 109 and marker in output and "_NOT_BLOCKED" not in output
    prerequisites = {}
    for tunable in [0, 3]:
        prefix = f"memtag-t{tunable}-"
        checks = {}
        for kind in ["global", "stack", "thread-stack", "heap", "thread-heap"]:
            checks[kind+"_valid_lifetime"] = positive(prefix+kind+"-good",
                "FREE_OK" if "heap" in kind else None)
            for action in ["bad-read", "bad-write"]:
                checks[kind+"_"+action] = blocked(prefix+kind+"-"+action, "BAD_ACCESS_BEGIN")
        checks["heap_uaf"] = blocked(prefix+"heap-uaf", "UAF_BEGIN")
        checks["heap_double_free"] = blocked(prefix+"heap-double-free", "DOUBLE_FREE_BEGIN")
        checks["heap_lifecycle"] = positive(prefix+"lifecycle-good", "LIFECYCLE_OK")
        valid_modes = [int(mode) for name,item in executions.items()
                       if name.startswith(prefix) for mode in
                       re.findall(r"(?:START HWCAP2=\d+ MODE|THREAD_MODE)=(\d+)", item["output"])]
        checks["strict_sync_main_and_workers"] = bool(valid_modes) and all(
            (mode & 7) == 3 for mode in valid_modes)
        prerequisites[str(tunable)] = checks
    report = dict(schema=1, records=records, builds=builds, provider_prerequisites=prerequisites,
        provider_prerequisites_pass=any(all(items.values()) for items in prerequisites.values()),
        full_vm_qualified=False,
        unqualified_coverage=["complete VM executable", "LLVM/libc++/libc++abi/libunwind",
            "ROS native dependencies and shared libraries", "TLS objects", "alternate stacks",
            "hot-update and unwind lifecycle", "native hardware performance"],
        execution="QEMU correctness only" if args.qemu else "native execution",
        source_sha256={str(p.relative_to(root)):digest(p) for p in
                       [source/"control.c", source/"payload.c", Path(__file__)]},
        compiler_sha256=digest(args.compiler))
    (out/"report.json").write_text(json.dumps(report, indent=2)+"\n")
    print(json.dumps({key: report[key] for key in ["builds", "provider_prerequisites_pass",
                                                 "full_vm_qualified"]}))
    # Observation collection succeeds even when qualification fails. Consumers
    # must inspect both explicit qualification fields, never just this exit code.
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
