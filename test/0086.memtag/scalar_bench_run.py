#!/usr/bin/env python3
"""Execute identical scalar Wasm modules before/after a runtime change.

This measures the real VM process (int or aot), including startup and guest
assertions. The entry function is resolved from the actual _start export.
"""
import argparse
import csv
import datetime
import hashlib
import json
import os
from pathlib import Path
import random
import resource
import statistics
import subprocess
import time


def uleb(data, at):
    value = shift = 0
    while True:
        b = data[at]
        at += 1
        value |= (b & 127) << shift
        if not b & 128:
            return value, at
        shift += 7
        assert shift <= 70


def start_export(data):
    assert data[:8] == b"\0asm\x01\0\0\0"
    at = 8
    while at < len(data):
        section = data[at]
        size, pos = uleb(data, at + 1)
        end = pos + size
        if section == 7:
            count, pos = uleb(data, pos)
            for _ in range(count):
                length, pos = uleb(data, pos)
                name = data[pos:pos+length]
                pos += length
                kind = data[pos]
                index, pos = uleb(data, pos + 1)
                if name == b"_start" and kind == 0:
                    return index
        at = end
    raise ValueError("No _start function export")


def command(binary, fixture, engine, mte, candidate_mode=None):
    data = fixture.read_bytes()
    result = [str(binary), "--runtime-int" if engine == "int" else "-Raot"]
    if engine == "aot":
        result += ["--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable"]
    result += ["--wasm-feature-enable-memory64", "--wasm-feature-enable-table64",
        "--wasm-feature-enable-threads", "--wasm-set-main-module-name", "main",
        "--wasip1-single-create", "main", "--wasip1-single-enable", "main",
        "--wasm-set-start-func", str(start_export(data))]
    if mte == "off":
        result += ["--runtime-disable-memtag"]
    elif candidate_mode is not None:
        result += ["--runtime-memtag-mode", candidate_mode]
    return result + ["--", str(fixture)]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--fixtures", type=Path, nargs="+", required=True)
    parser.add_argument("--baseline", type=Path, required=True)
    parser.add_argument("--baseline-wire", choices=["packed", "legacy-fc"], default="packed",
                        help="Explicit input identity for measuring an immutable old binary; does not enable any runtime compatibility.")
    parser.add_argument("--candidate", type=Path)
    parser.add_argument("--candidate-loop-versioning", choices=["disabled", "exclusive-private"], default="disabled",
                        help="Enable the experimental private loop proof only for the candidate. The baseline always has the experiment disabled.")
    parser.add_argument("--candidate-memtag-mode", choices=["sync", "async"],
                        help="Explicit candidate policy for MTE-on samples. Omitted for historical binary comparisons; MTE-off samples never request an asynchronous mode.")
    parser.add_argument("--candidate-fixtures", type=Path, nargs="+",
                        help="Matched semantic fixtures for FC/FF comparison; baseline remains --fixtures.")
    parser.add_argument("--engine", choices=["int", "aot"], default="int")
    parser.add_argument("--cpu", type=int, default=8)
    parser.add_argument("--trials", type=int, default=9)
    parser.add_argument("--warmups", type=int, default=1)
    parser.add_argument("--minimum-ms", type=float, default=0)
    parser.add_argument("--modes", default="on,off")
    parser.add_argument("--select", default="")
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    if "UWVM_MTE_STUDY_IR_DIR" in os.environ:
        parser.error("timing must use an unobserved binary with UWVM_MTE_STUDY_IR_DIR unset")
    if args.candidate_loop_versioning != "disabled" and not args.candidate:
        parser.error("--candidate-loop-versioning requires --candidate")
    if args.candidate_memtag_mode is not None and not args.candidate:
        parser.error("--candidate-memtag-mode requires --candidate")
    if args.candidate_loop_versioning != "disabled" and args.candidate_memtag_mode == "async":
        parser.error("loop versioning is disabled for asynchronous execution; measure these mechanisms separately")
    assert args.trials > 0 and args.warmups >= 0 and args.minimum_ms >= 0
    modes = args.modes.split(",")
    assert set(modes) <= {"on", "off"}
    args.out.mkdir(parents=True, exist_ok=True)
    os.sched_setaffinity(0, {args.cpu})
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    variants = {"baseline": args.baseline.resolve()}
    if args.candidate:
        variants["candidate"] = args.candidate.resolve()
    environments = {}
    for variant in variants:
        environment = dict(os.environ)
        environment.pop("UWVM_STUDY_MEMTAG_LOOP_VERSIONING", None)
        if variant == "candidate" and args.candidate_loop_versioning == "exclusive-private":
            environment["UWVM_STUDY_MEMTAG_LOOP_VERSIONING"] = "exclusive-private"
        environments[variant] = environment
    fixtures = []
    for manifest in args.fixtures:
        for row in json.loads(manifest.read_text()):
            if args.baseline_wire == "packed" and row.get("encoding") not in ("packed", "packed-padded"):
                parser.error("unidentified baseline fixture encoding; use regenerated packed fixtures, or explicitly identify an old benchmark binary and its legacy-fc fixtures")
            if args.baseline_wire == "legacy-fc" and row.get("encoding") not in (None, "legacy-fc"):
                parser.error("baseline fixture manifest disagrees with --baseline-wire legacy-fc")
            if args.select and args.select not in row["name"]:
                continue
            path = Path(row["file"])
            if not path.exists():
                path = manifest.parent/path.name
            path = path.resolve()
            assert hashlib.sha256(path.read_bytes()).hexdigest() == row["sha256"]
            fixtures.append(dict(row, file=str(path), start_function=start_export(path.read_bytes())))
    assert fixtures
    if args.baseline_wire == "legacy-fc" and (not args.candidate or not args.candidate_fixtures):
        parser.error("a historical baseline comparison requires a current candidate and matched packed --candidate-fixtures")
    candidate_fixtures = {}
    if args.candidate_fixtures:
        assert args.candidate, "--candidate-fixtures requires --candidate"
        for manifest in args.candidate_fixtures:
            for row in json.loads(manifest.read_text()):
                if row.get("encoding") not in ("packed", "packed-padded"):
                    parser.error("obsolete/unidentified alternate MTE fixture encoding; regenerate the fixture")
                if args.select and args.select not in row["name"]:
                    continue
                path = Path(row["file"])
                if not path.exists():
                    path = manifest.parent/path.name
                path = path.resolve()
                assert hashlib.sha256(path.read_bytes()).hexdigest() == row["sha256"]
                assert row["name"] not in candidate_fixtures
                candidate_fixtures[row["name"]] = dict(row, file=str(path))
        assert set(candidate_fixtures) == {r["name"] for r in fixtures}
        for row in fixtures:
            other = candidate_fixtures[row["name"]]
            for key in ("bits", "kind", "iterations", "rounds", "expected_checksum", "verified_values", "verified_pointer_entries"):
                assert row[key] == other[key], (row["name"], key)
    metadata = dict(engine=args.engine, baseline_wire=args.baseline_wire, scope="whole actual VM process including parse/compile/initialization/guest assertions; no startup subtraction",
        started_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),
        experiment_env={name: {"UWVM_STUDY_MEMTAG_LOOP_VERSIONING":environment.get("UWVM_STUDY_MEMTAG_LOOP_VERSIONING")}
                        for name, environment in environments.items()},
        candidate_memtag_mode=args.candidate_memtag_mode,
        ir_observer_unset="UWVM_MTE_STUDY_IR_DIR" not in os.environ,
        cpu=args.cpu, affinity=sorted(os.sched_getaffinity(0)), trials=args.trials, warmups=args.warmups,
        minimum_ms=args.minimum_ms, clock=str(time.get_clock_info("perf_counter")),
        binaries={name:dict(file=str(path),sha256=hashlib.sha256(path.read_bytes()).hexdigest()) for name,path in variants.items()},
        fixtures=fixtures, candidate_fixtures=candidate_fixtures,
        source_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest())
    (args.out/"metadata.json").write_text(json.dumps(metadata,indent=2)+"\n")
    fields = ["name","kind","bits","mte","variant","trial","iterations","rounds","wall_ns","user_seconds","system_seconds","involuntary_switches","exit"]
    rows, invocations = [], []
    failed = None
    with (args.out/"raw.csv").open("w",newline="") as stream:
        writer = csv.DictWriter(stream,fieldnames=fields)
        writer.writeheader()
        for fixture in fixtures:
            for mode in modes:
                for trial in range(-args.warmups,args.trials):
                    order = list(variants)
                    if trial & 1:
                        order.reverse()
                    for variant in order:
                        actual_fixture = candidate_fixtures.get(fixture["name"], fixture) if variant == "candidate" else fixture
                        argv = command(variants[variant],Path(actual_fixture["file"]),args.engine,mode,
                                       args.candidate_memtag_mode if variant == "candidate" else None)
                        before = resource.getrusage(resource.RUSAGE_CHILDREN)
                        begin = time.perf_counter_ns()
                        result = subprocess.run(argv,capture_output=True,timeout=180,env=environments[variant])
                        elapsed = time.perf_counter_ns()-begin
                        after = resource.getrusage(resource.RUSAGE_CHILDREN)
                        invocation = dict(name=fixture["name"],mode=mode,variant=variant,trial=trial,args=argv,exit=result.returncode,wall_ns=elapsed)
                        invocations.append(invocation)
                        if result.stdout or result.stderr:
                            log=f"{fixture['name']}-{mode}-{variant}-{trial}.log"
                            (args.out/log).write_bytes(result.stdout+result.stderr)
                            invocation["log"] = log
                        if result.returncode:
                            failed = invocation
                            break
                        if trial >= 0:
                            row=dict(name=fixture["name"],kind=fixture["kind"],bits=fixture["bits"],mte=mode,variant=variant,trial=trial,
                                iterations=fixture["iterations"],rounds=fixture["rounds"],wall_ns=elapsed,
                                user_seconds=after.ru_utime-before.ru_utime,system_seconds=after.ru_stime-before.ru_stime,
                                involuntary_switches=after.ru_nivcsw-before.ru_nivcsw,exit=result.returncode)
                            rows.append(row);writer.writerow(row);stream.flush()
                    if failed:break
                print(fixture["name"],mode,"FAIL" if failed else "PASS",flush=True)
                if failed:break
            if failed:break
    (args.out/"invocations.json").write_text(json.dumps(invocations,indent=2)+"\n")
    metadata["completed_utc"]=datetime.datetime.now(datetime.timezone.utc).isoformat()
    (args.out/"metadata.json").write_text(json.dumps(metadata,indent=2)+"\n")
    if failed:
        (args.out/"receipt.json").write_text(json.dumps(dict(status="FAIL",failed=failed),indent=2)+"\n")
        return 2
    summaries=[]
    for fixture in fixtures:
        for mode in modes:
            subset=[r for r in rows if r["name"]==fixture["name"] and r["mte"]==mode]
            item=dict(name=fixture["name"],kind=fixture["kind"],bits=fixture["bits"],mte=mode,trials=args.trials)
            for variant in variants:
                values=[r["wall_ns"]/1e6 for r in subset if r["variant"]==variant]
                assert len(values)==args.trials
                item[variant+"_median_ms"]=statistics.median(values)
                item[variant+"_min_ms"]=min(values)
                item[variant+"_max_ms"]=max(values)
            if "candidate" in variants:
                paired=[]
                for trial in range(args.trials):
                    pair={r["variant"]:r["wall_ns"] for r in subset if r["trial"]==trial}
                    paired.append(100*(pair["candidate"]/pair["baseline"]-1))
                item["paired_change_percent"]=statistics.median(paired)
                item["wins"]=sum(v<0 for v in paired)
                item["paired_changes"]=paired
                if args.trials>=5:
                    rng=random.Random(127)
                    bootstrap=sorted(statistics.median(rng.choices(paired,k=len(paired))) for _ in range(10000))
                    item["bootstrap95_percent"]=[bootstrap[249],bootstrap[9749]]
            summaries.append(item)
    shortest=min(r["wall_ns"] for r in rows)/1e6
    gate=shortest>=args.minimum_ms
    receipt=dict(status="PASS" if gate else "DURATION_GATE_FAIL",functional_runs=len(invocations),samples=len(rows),shortest_ms=shortest,
        raw_sha256=hashlib.sha256((args.out/"raw.csv").read_bytes()).hexdigest(),all_guest_assertions_passed=True)
    (args.out/"receipt.json").write_text(json.dumps(receipt,indent=2)+"\n")
    (args.out/"summary.json").write_text(json.dumps(summaries,indent=2)+"\n")
    print(json.dumps(receipt,indent=2))
    return 0 if gate else 3


if __name__=="__main__":
    raise SystemExit(main())
