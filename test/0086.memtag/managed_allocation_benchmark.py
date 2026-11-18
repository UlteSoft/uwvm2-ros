"""Compare a claim/release pair between two live neighbors using identical owner semantics."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import statistics
import subprocess
import time

from initialization_matrix import HEADER, exports
from vm_matrix import c, op, sec, u


def guest(count):
    # Four i32 locals hold the left, right and transient pointers and loop count.
    def claim(address):
        return c(address) + c(16) + op(0x40)
    def release(index):
        return bytes([0x20,index]) + c(16) + op(0x44) + b"\x1a"
    body = (b"\x01\x04\x7f" + claim(0) + b"\x21\x00" + claim(32) + b"\x21\x01" +
            c(count) + b"\x21\x03\x03\x40" + claim(16) + b"\x21\x02" + release(2) +
            b"\x20\x03" + c(1) + b"\x6b\x22\x03\x0d\x00\x0b" +
            release(0) + release(1) + b"\x0b")
    return (HEADER + sec(1,b"\x01\x60\x00\x00") + sec(3,b"\x01\x00") +
            sec(5,b"\x01\x01\x01\x01") + exports([("_start",0,0)]) + sec(10,u(1)+u(len(body))+body))


def digest(path):
    with path.open("rb") as source:
        return hashlib.file_digest(source,"sha256").hexdigest()


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm",type=Path,required=True)
    parser.add_argument("--native",type=Path,required=True)
    parser.add_argument("--output",type=Path,required=True)
    parser.add_argument("--count",type=int,default=250000)
    args=parser.parse_args()
    if not 1 <= args.count <= (1<<25):
        parser.error("count outside benchmark range")
    group=Path("/sys/fs/cgroup") / Path("/proc/self/cgroup").read_text().strip().split("::",1)[1].lstrip("/")
    if (group/"memory.max").read_text().strip()!="68719476736" or (group/"memory.swap.max").read_text().strip()!="0":
        parser.error("expected the 64 GiB cgroup with zero swap")
    args.output.mkdir(parents=True,exist_ok=True)
    vm=args.vm.resolve()
    native=args.native.resolve()
    report=dict(complete=False,cgroup=str(group),affinity=sorted(os.sched_getaffinity(0)),
                count=args.count,vm=digest(vm),native=digest(native),measurements={})
    resource.setrlimit(resource.RLIMIT_CORE,(0,0))
    for kind in ("native","aot","int"):
        commands=[]
        for count in (args.count,args.count*2):
            if kind=="native":
                commands.append([str(native),str(count)])
            else:
                path=args.output/f"{count}.wasm"
                path.write_bytes(guest(count))
                flags=["--runtime-llvm-jit-cache-path","disable","--runtime-compile-threads","0"] if kind=="aot" else []
                commands.append([str(vm),"--runtime-"+kind,*flags,"--runtime-memtag-backend","software","--runtime-memtag-mode","sync","--wasm-set-start-func","0","--run",str(path)])
        def timed(argv):
            start=time.perf_counter_ns()
            result=subprocess.run(argv,capture_output=True,timeout=90)
            elapsed=time.perf_counter_ns()-start
            if result.returncode:
                raise RuntimeError(result.stdout.decode(errors="replace")+result.stderr.decode(errors="replace"))
            return elapsed
        timed(commands[0])
        pairs=[]
        for repetition in range(9):
            order=(0,1) if repetition%2==0 else (1,0)
            timing=[0,0]
            for index in order:
                timing[index]=timed(commands[index])
            pairs.append(timing)
        costs=[(b-a)/args.count for a,b in pairs]
        report["measurements"][kind]=dict(commands=commands,pairs_ns=pairs,
            median_ns_per_claim_release=statistics.median(costs))
    report["complete"]=True
    (args.output/"results.json").write_text(json.dumps(report,indent=2)+"\n")

if __name__=="__main__":
    main()
