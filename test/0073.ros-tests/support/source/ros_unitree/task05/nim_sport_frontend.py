#!/usr/bin/env python3
"""Genuine Nim SDK/application formatting and wasm32 frontend checks, not a Guest."""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess
import tarfile

def pin(path):
    b=path.read_bytes();return {"bytes":len(b),"sha256":hashlib.sha256(b).hexdigest()}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ("unitree","foundation","records","nim-root"):
        parser.add_argument("--"+name,type=Path,required=True)
    args=parser.parse_args();work=Path.cwd();captured={};commands=[]
    def observe(path):
        path=path.absolute();captured[str(path)]=pin(path);return path
    nim=observe(args.nim_root/"bin/nim");pretty=observe(args.nim_root/"bin/nimpretty")
    foundation=args.foundation.resolve(strict=True)
    for p in sorted((foundation/"src").rglob("*.nim")):observe(p)
    source=work/"source";source.mkdir()
    origins={"unitree_sport.nim":args.unitree/"language/nim/unitree_sport.nim",
             "sport_topic_command.nim":args.unitree/"examples/nim/sport_topic_command.nim",
             "unitree_api_records.nim":args.records}
    for name,path in origins.items():shutil.copyfile(observe(path),source/name)
    def run(name,argv):
        out=work/(name+".stdout");err=work/(name+".stderr")
        with out.open("xb") as stdout,err.open("xb") as stderr:
            p=subprocess.run([str(a) for a in argv],stdout=stdout,stderr=stderr,check=False)
        commands.append({"name":name,"argv":[str(a) for a in argv],"exit":p.returncode,
                         "stdout":pin(out),"stderr":pin(err)})
        return p.returncode
    files=[source/name for name in origins]
    if run("pretty-first",[pretty,"--indent:2","--maxLineLen:100",*files]):
        raise ValueError("actual Nim formatter failed; retained original diagnostics")
    once={str(path):pin(path) for path in files}
    if run("pretty-second",[pretty,"--indent:2","--maxLineLen:100",*files]) or any(pin(Path(p))!=v for p,v in once.items()):
        raise ValueError("actual Nim formatter second pass is not idempotent")
    checks=[]
    for name in origins:
        code=run("semantic-"+name,[nim,"check","--mm:arc","--threads:off","--exceptions:goto",
            "--cpu:wasm32","--os:any","-d:wasm","-d:useMalloc","--path:"+str(foundation/"src"),
            "--path:"+str(source),"--nimcache:"+str(work/"nimcache"),source/name])
        checks.append({"source":name,"exit":code})
    for path,before in captured.items():
        if pin(Path(path))!=before:raise ValueError("original Nim/tool/foundation input changed")
    with tarfile.open(work/"source-candidate.tar","w") as archive:
        for name in ("unitree_sport.nim","sport_topic_command.nim"):
            archive.add(source/name,arcname=name,recursive=False)
    result={"status":"PASS_FORMAT_AND_FRONTEND_ONLY" if all(r["exit"]==0 for r in checks) else "FAILED_FRONTEND_RETAINED",
        "source_and_tool_inputs":captured,"commands":commands,"wasm32_semantics":checks,
        "formatted":once,"native_linked":False,"Wasm_command_built":False,
        "Guest_runs":0,"native_ROS_runs":0,"default_authority":[]}
    (work/"result.json").write_text(json.dumps(result,indent=2)+"\n")
    print(json.dumps({"status":result["status"],"wasm32_semantics":checks},sort_keys=True))
    if any(row["exit"] for row in checks):raise SystemExit(1)

if __name__=="__main__":main()
