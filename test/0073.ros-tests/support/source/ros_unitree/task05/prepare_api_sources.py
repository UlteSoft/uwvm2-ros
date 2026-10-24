#!/usr/bin/env python3
"""Prepare the actual complete API family from retained original locked sources.

Finite Linux-owner workload only. Reuse the existing source-paired materializer
and industrial preparation API. No compiler, Guest, native ROS or original
program runs. Record all actual generator source pins before/after preparation.
"""
from pathlib import Path
import argparse
import hashlib
import json
import subprocess
import sys

def pin(path):
    b=path.read_bytes();return {"bytes":len(b),"sha256":hashlib.sha256(b).hexdigest()}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--snapshot",type=Path,required=True)
    parser.add_argument("--upstream-root",type=Path,required=True)
    args=parser.parse_args()
    source=args.snapshot.resolve(strict=True);upstream=args.upstream_root.resolve(strict=True)
    work=Path.cwd();unitree=source/"sdk/origin/robots/unitree"
    captured={str(p.relative_to(source)):pin(p) for p in sorted(source.rglob("*")) if p.is_file() and not p.is_symlink()}
    pair={"parent":source/"sdk/origin/tools/custom_types.py",
          "bounded":source/"sdk/origin/custom-types/bounded_profile.py",
          "adapter":source/"sdk/origin/robots/industrial/vendor_profile.py"}
    python=[sys.executable,"-I","-S","-B"]
    def run(name,argv):
        with (work/(name+".stdout")).open("xb") as out,(work/(name+".stderr")).open("xb") as err:
            p=subprocess.run([str(a) for a in argv],stdout=out,stderr=err,check=False)
        if p.returncode:raise ValueError(name+" failed; retained original stdout/stderr")
    # The original retained license must match this repository/commit's lock.
    lock=json.loads((unitree/"upstream-lock.json").read_text())
    row=next(r for r in lock["files"] if r["repository"]=="unitreerobotics/unitree_ros2" and
        r["commit"]=="668d1ec5a05d1c38d3306bdca7d59f2ba3581a88" and r["path"]=="LICENSE")
    if pin(upstream/"LICENSE")!={k:row[k] for k in ("bytes","sha256")}:raise ValueError("original license pin differs")
    run("materialize",[*python,unitree/"materialize_catalog.py","--upstream-root",upstream,
        "--profile",unitree/"profiles/unitree_api.json","--lock",unitree/"upstream-lock.json",
        "--catalog-format","uwvm-ros-vendor-complete-bounded-v1","--out",work/"preparation"])
    argv=[*python,source/"sdk/origin/robots/industrial/prepare.py","--source-root",source,
        "--catalog",work/"preparation/catalog.preparation.json","--mode","prepare","--out",work/"complete"]
    for name,path in pair.items():argv.extend(["--"+name+"-sha256",pin(path)["sha256"]])
    run("complete",argv)
    generated=json.loads((work/"complete/generated/generation.vendor.json").read_text())
    reference=json.loads((source/"sdk/origin/custom-types/unitree-api-complete/generation.vendor.json").read_text())
    for key in ("catalog","definitions"):
        if generated["source_inputs"][key]!=reference["source_inputs"][key]:raise ValueError("whole original source association differs")
    expected={r["type"]:r for r in reference["records"]}
    if len(generated["records"])!=8:raise ValueError("whole eight-record family required")
    for row in generated["records"]:
        for key in ("native_identity","codec_identity","minimum_wire_bytes","maximum_wire_bytes"):
            if row[key]!=expected[row["type"]][key]:raise ValueError("original record fact differs")
    for name,fact in captured.items():
        if pin(source/name)!=fact:raise ValueError("source changed during actual preparation")
    result={"status":"PASS_COMPLETE_API_SOURCE_PREPARATION_ONLY","complete_records":8,
        "source_inputs":captured,"original_license":pin(upstream/"LICENSE"),
        "catalog":generated["source_inputs"]["catalog"],"compiler_runs":0,"Guest_runs":0,
        "native_ROS_runs":0,"original_program_runs":0,"default_authority":[]}
    (work/"result.json").write_text(json.dumps(result,indent=2)+"\n")
    print(json.dumps({"status":result["status"],"complete_records":8},sort_keys=True))

if __name__=="__main__":main()
