"""Check programmed hardware periods for the ABI-query loop and resolve samples in VTune."""
import argparse
import json
import socket
import subprocess
import sys
from pathlib import Path

from profile_scene_call_audit import event_rows, sampled
from run_scene_vm import sha, verify_gate
from hardware_sampling import audit_samples, reference_frequency


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--benchmark", type=Path, required=True)
    ap.add_argument("--reference-context", type=Path, required=True)
    ap.add_argument("--output", type=Path, required=True)
    ap.add_argument("--vtune", type=Path, default=Path("/opt/intel/oneapi/vtune/latest/bin64/vtune"))
    args = ap.parse_args()
    verify_gate()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    source = json.loads(args.benchmark.read_bytes())
    assert source["timings_complete"]
    hz = reference_frequency(args.reference_context, socket.gethostname())
    expected = {"CPU_CLK_UNHALTED.REF_TSC": 24000, "INST_RETIRED.ANY": 240000}
    config = ("cpu_core/umask=0x3,period=24000,name=CPU_CLK_UNHALTED.REF_TSC/u,"
              "cpu_core/event=0xc0,period=240000,name=INST_RETIRED.ANY/u")
    report = dict(benchmark_sha256=sha(args.benchmark), hardware_reference_hz=hz,
                  expected_periods=expected, commands=[], profiles=[], complete=False)

    def save():
        pending = out / "results.tmp"
        pending.write_text(json.dumps(report, indent=2)+"\n")
        pending.replace(out / "results.json")

    def run(label, command):
        stdout, stderr = out/(label+".out"), out/(label+".err")
        with stdout.open("wb") as data, stderr.open("wb") as errors:
            rc = subprocess.run(command, cwd=out, stdout=data, stderr=errors, timeout=360).returncode
        report["commands"].append(dict(name=label, argv=list(map(str,command)), exit=rc))
        save()
        if rc: raise RuntimeError((label, stderr.read_text(errors="replace")[-1500:]))
        return stdout

    for kind in ("native","wasm"):
        argv = list(source["commands"][kind]["100000"])
        binary = Path(argv[0])
        assert sha(binary) == (source["inputs"]["native"] if kind=="native" else source["runtime_sha256"])
        if kind=="native":
            argv[-1] = "1000000" if "membership" in source.get("native_boundary","") else "200000000"
        pid_file, trace, target = out/(kind+".pid"), out/(kind+".perf"), out/(kind+"-vtune")
        stamp = ("import os,sys; from pathlib import Path; Path(sys.argv[1]).write_text(str(os.getpid())); "
                 "os.execv(sys.argv[2],sys.argv[2:])")
        run(kind+"-record", ["/usr/bin/perf","record","-N","-B","-T","--sample-cpu","-e",config,"-o",trace,
                             "--","/usr/bin/taskset","-c","4",sys.executable,"-c",stamp,pid_file,*argv])
        pid, comm = int(pid_file.read_text()), binary.name[:15]
        attrs = run(kind+"-attributes", ["perf","evlist","-v","-i",trace]).read_text()
        sample_path = run(kind+"-samples", ["perf","script","--ns","-F","comm,pid,tid,time,event,period,ip","-i",trace])
        raw_path = run(kind+"-raw", ["perf","script","-D","-i",trace])
        lines = []
        with sample_path.open() as stream:
            for line in stream:
                fields=line.split(maxsplit=2)
                if len(fields)==3 and fields[0]==comm and fields[1].partition("/")[0]==str(pid):
                    lines.append(line)
        audit = audit_samples(attrs, "".join(lines), raw_path.read_text(errors="replace"), expected, hz, target_comm=comm)
        row=dict(target=kind, client_pid=pid, executable_sha256=sha(binary), audit=audit)
        report["profiles"].append(row)
        save()
        run(kind+"-import", [args.vtune,"-import",trace,"-result-dir",target,"-finalization-mode","full"])
        options=[args.vtune,"-report","hw-events","-result-dir",target,"-filter",f"process-id={pid}",
                 "-filter","module="+binary.name,"-format","csv","-csv-delimiter","tab"]
        source_table=out/(kind+"-lines.tsv")
        run(kind+"-lines", [*options,"-group-by","function,source-file,source-line","-report-output",source_table])
        candidates = ({"main","scene_abi"} if kind=="native" else
                      {"uwvm2::uwvm::imported::scene::abi","uwvm2::uwvm::imported::scene::wrapper::observe_result"})
        relevant=[r for r in event_rows(source_table,"Function")
                  if r["Function"] in candidates and r.get("Source Line","").isdigit() and sampled(r)]
        row["source_rows"]=len(relevant)
        if relevant:
            chosen=max(relevant,key=lambda r:sum(float(x.replace(",","") or 0) for k,x in r.items() if k.startswith("Hardware Event Count:")))
            row.update(function=chosen["Function"], source_line=int(chosen["Source Line"]), source_file=chosen["Source File"])
            asm=out/(kind+"-asm.tsv")
            run(kind+"-assembly", [*options,"-source-object","function="+chosen["Function"],"-group-by","address",
                                   "-column","Assembly,Source Line,Hardware Event Count","-report-output",asm])
            row["assembly_rows"]=sum(r["Address"].startswith("0x") and r.get("Source Line","").isdigit() and sampled(r)
                                     for r in event_rows(asm,"Address"))
        save()
    report["complete"]=all(r["audit"]["ten_us_reference_period_verified"] and r["source_rows"] and r.get("assembly_rows")
                           for r in report["profiles"])
    save()
    return 0 if report["complete"] else 1


if __name__=="__main__":
    sys.exit(main())
