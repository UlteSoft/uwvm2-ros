"""Compare a real ROSv2 ABI query with a separately compiled native function.

Collect timings outside the profiler. Subtract a short run from a long one to
reduce the contribution from process startup and loading. The native side
makes a direct call across two translation units without LTO. A hardware
profile describes its own workload and cannot establish a call's duration.
"""
import argparse
import csv
import json
import os
import select
import statistics
import subprocess
import sys
import time
from pathlib import Path

from run_scene_vm import GATE, sha, verify_gate

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/ros_scene"))
from hardware_sampling import vtune_periods

LOW, HIGH = 100000, 8100000
WAT = """(module
 (import "uwvm_ros_v2" "abi" (func $abi (result i32)))
 (func (export "_start") (local $left i32) (local $sum i32)
  (local.set $left (i32.const N))
  (loop $again
   (local.set $sum (i32.add (local.get $sum) (call $abi)))
   (br_if $again (local.tee $left (i32.sub (local.get $left) (i32.const 1)))))
  (if (i32.ne (local.get $sum) (i32.const SUM)) (then unreachable))))"""


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--vtune", type=Path, default=Path("/opt/intel/oneapi/vtune/latest/bin64/vtune"))
    args = parser.parse_args()
    verify_gate()
    vm, out = args.runtime.resolve(), args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    report = dict(runtime=str(vm), runtime_sha256=sha(vm), operations=[LOW, HIGH],
                  cgroup=str(GATE), audit="disabled", samples={}, profiles=[], complete=False)

    def save():
        pending = out / "results.tmp"
        pending.write_text(json.dumps(report, indent=2) + "\n")
        pending.replace(out / "results.json")

    def run(name, command, *, optional=False, limit=180):
        before = time.monotonic_ns()
        with (out / (name + ".log")).open("wb") as log:
            with subprocess.Popen(list(map(str, command)), cwd=out, stdout=log, stderr=subprocess.STDOUT) as result:
                fd = os.pidfd_open(result.pid)
                try:
                    deadline = select.poll()
                    deadline.register(fd, select.POLLIN)
                    if not deadline.poll(limit*1000):
                        result.kill()
                        result.wait()
                        raise TimeoutError(name)
                    elapsed = time.monotonic_ns()-before
                    result.wait()
                finally:
                    os.close(fd)
        if result.returncode and not optional:
            raise RuntimeError(f"{name} failed, see {out / (name+'.log')}")
        return result.returncode, elapsed

    native = out / "native"
    (out / "abi.c").write_text("#include <stdint.h>\nuint32_t scene_abi(void) { return 2; }\n")
    (out / "loop.c").write_text("""#include <stdint.h>
#include <stdlib.h>
uint32_t scene_abi(void);
int main(int argc, char **argv) {
    if (argc != 2) return 2;
    uint32_t n = (uint32_t)strtoul(argv[1], 0, 10), sum = 0;
    for (uint32_t i = 0; i < n; ++i) sum += scene_abi();
    return sum != n * 2;
}
""")
    run("native-build", ["cc", "-O3", "-g", "-fno-lto", "loop.c", "abi.c", "-o", native])
    commands = {"native": {}, "wasm": {}}
    for n in (LOW, HIGH):
        wat, wasm = out / f"calls-{n}.wat", out / f"calls-{n}.wasm"
        wat.write_text(WAT.replace("SUM", str(n*2)).replace("N", str(n)))
        run(f"assemble-{n}", ["wat2wasm", "--debug-names", wat, "-o", wasm])
        commands["native"][n] = [native, str(n)]
        commands["wasm"][n] = [vm, "--runtime-aot", "--runtime-compile-threads", "0",
            "--runtime-llvm-jit-cache-path", "disable", "--wasm-set-main-module-name", "main",
            "--ros-scene-create", "robot", "--ros-scene-bind", "robot", "main",
            "--ros-scene-interface", "robot", "abi", "allow", "--run", wasm]
    report["inputs"] = {p.name: sha(p) for p in [native, out / "abi.c", out / "loop.c", *out.glob("*.wasm")]}
    report["commands"] = {k: {n: list(map(str, v)) for n, v in jobs.items()} for k, jobs in commands.items()}
    for kind in commands:
        run(kind+"-warm", commands[kind][LOW])
        report["samples"][kind] = []
    for trial in range(7):
        for kind in (("native","wasm") if trial%2==0 else ("wasm","native")):
            elapsed = {}
            for n in ((LOW,HIGH) if trial%2==0 else (HIGH,LOW)):
                _, elapsed[str(n)] = run(f"{kind}-{n}-{trial}", commands[kind][n])
            elapsed["differential_ns_per_call"] = (elapsed[str(HIGH)]-elapsed[str(LOW)])/(HIGH-LOW)
            report["samples"][kind].append(elapsed)
            save()
    report["median_ns_per_call"] = {k: statistics.median(x["differential_ns_per_call"] for x in samples)
                                    for k, samples in report["samples"].items()}
    report["relative"] = report["median_ns_per_call"]["wasm"]/report["median_ns_per_call"]["native"]
    report["timings_complete"] = True
    save()
    for kind, jobs in commands.items():
        for analysis in ("hotspots", "uarch-exploration"):
            name = kind+"-"+analysis
            target = out/name
            knobs = (["-knob", "sampling-mode=hw", "-knob", "enable-stack-collection=false"]
                     if analysis=="hotspots" else ["-knob", "pmu-collection-mode=detailed", "-knob", "dram-bandwidth-limits=false"])
            task = [args.vtune, "-collect", analysis, *knobs, "-knob", "sampling-interval=0.01",
                    "-no-analyze-system", "-no-summary", "-cpu-mask", "4", "-data-limit=16",
                    "-return-app-exitcode", "-result-dir", target, "-source-search-dir", ROOT,
                    "--", "/usr/bin/taskset", "-c", "4", *jobs[HIGH]]
            code, _ = run(name+"-collect", task, optional=True, limit=300)
            profile = {"kind":kind,"analysis":analysis,"requested_ms":0.01,"collection_exit":code,
                       "source_rows":0,"assembly_rows":0,"programmed":vtune_periods(target)}
            report["profiles"].append(profile)
            save()
            if code: continue
            binary = native.name if kind=="native" else vm.name
            options = [args.vtune, "-report", "hw-events", "-result-dir", target, "-filter", "module="+binary,
                       "-format", "csv", "-csv-delimiter", "tab"]
            table = out/(name+"-source.tsv")
            code, _ = run(name+"-source", [*options, "-group-by", "function,source-file,source-line",
                                           "-report-output", table], optional=True)
            if code: continue
            def sampled(row):
                return any(float(v.replace(",","") or 0)>0 for k,v in row.items() if k and k.startswith("Hardware Event Count:"))
            with table.open() as stream:
                rows = [row for row in csv.DictReader(stream, delimiter="\t") if row.get("Source Line","").isdigit() and sampled(row)]
            relevant = [row for row in rows if (kind=="native" and Path(row.get("Source File","")).name in {"abi.c","loop.c"}) or
                        (kind=="wasm" and "uwvm2::uwvm::imported::scene" in row.get("Function",""))]
            profile["source_rows"] = len(relevant)
            if relevant:
                func = relevant[0]["Function"]
                assembly = out/(name+"-asm.tsv")
                code, _ = run(name+"-asm", [*options, "-source-object", "function="+func, "-group-by", "address",
                              "-column", "Assembly,Source Line,Hardware Event Count", "-report-output", assembly], optional=True)
                if not code:
                    with assembly.open() as stream:
                        profile["function"] = func
                        profile["assembly_rows"] = sum(1 for row in csv.DictReader(stream,delimiter="\t")
                            if row.get("Address","").startswith("0x") and row.get("Source Line","").isdigit() and sampled(row))
            save()
    report["complete"] = all(p["source_rows"] and p["assembly_rows"] for p in report["profiles"])
    save()
    print(json.dumps({k:report[k] for k in ("median_ns_per_call","relative","complete")}))
    return 0 if report["complete"] else 1


if __name__ == "__main__":
    sys.exit(main())
