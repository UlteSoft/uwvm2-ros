#!/usr/bin/env python3
"""Real twice-idempotent Go source formatting plus exact Python syntax inventory.

Finite Linux-owner workload only. No language SDK compiler, Guest or native ROS
run follows. Preserve original inputs and provide exact source-only candidates.
"""
from pathlib import Path
import argparse
import ast
import hashlib
import json
import subprocess
import sys
import tarfile

GO=("sdk/origin/robots/unitree/language/go/unitree.go",
    "sdk/origin/robots/unitree/examples/go_sport/main.go")
PYTHON=("sdk/origin/robots/unitree/stage_language_sdk.py",
    "sdk/origin/robots/unitree/emit_language_records.py",
    "test/0073.ros-tests/support/source/ros_platform05/unitree/language_overlay_contract.py",
    "test/0073.ros-tests/support/source/ros_unitree/task05/language_sources.py",
    "test/0073.ros-tests/support/source/ros_unitree/task05/format_language_sources.py")

def pin(body):
    return {"bytes":len(body),"sha256":hashlib.sha256(body).hexdigest()}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--snapshot",type=Path,required=True)
    parser.add_argument("--go-root",type=Path,required=True)
    args=parser.parse_args()
    root=args.snapshot.resolve(strict=True)
    go_root=args.go_root.resolve(strict=True)
    formatter=go_root/"bin/gofmt"
    work=Path.cwd()
    captured={name:(root/name).read_bytes() for name in GO+PYTHON}
    python=[]
    for name in PYTHON:
        tree=ast.parse(captured[name].decode("utf-8"),filename=name)
        python.append({"path":name,"source":pin(captured[name]),"syntax":"PASS","nodes":sum(1 for _ in ast.walk(tree))})
    candidate=work/"candidate"
    rows=[]
    for name in GO:
        path=candidate/name;path.parent.mkdir(parents=True,exist_ok=True)
        with path.open("xb") as stream:stream.write(captured[name])
        subprocess.run([str(formatter),"-w",str(path)],check=True)
        once=path.read_bytes()
        subprocess.run([str(formatter),"-w",str(path)],check=True)
        if path.read_bytes()!=once:
            raise ValueError("genuine gofmt is not byte-idempotent")
        rows.append({"path":name,"before":pin(captured[name]),"after":pin(once),"idempotent":True})
    for name,body in captured.items():
        if (root/name).read_bytes()!=body:
            raise ValueError("original formatter/syntax input changed")
    with tarfile.open(work/"source-candidate.tar","w") as archive:
        for name in GO:archive.add(candidate/name,arcname=name,recursive=False)
    result={"status":"PASS_FORMAT_AND_PYTHON_SYNTAX_ONLY","formatter":{
        "path":str(formatter),**pin(formatter.read_bytes()),"go_version_file":(go_root/"VERSION").read_text()},
        "go":rows,"python":python,"python_runtime":sys.version,
        "compiler_runs":0,"Guest_runs":0,"native_ROS_runs":0,"default_authority":[]}
    (work/"result.json").write_text(json.dumps(result,indent=2)+"\n")
    print(json.dumps({"status":result["status"],"real_gofmt_files":len(rows),"python_AST":len(python)},sort_keys=True))

if __name__=="__main__":main()
