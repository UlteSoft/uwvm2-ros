#!/usr/bin/env python3
"""Build/link the genuine Unitree packet library and run native plus WASI values.

Finite Linux-owner workload only. Uses complete real generated C records and
both actual C++20/fast_io payload libraries. No fake C symbols, ROS bridge,
endpoint, native ROS daemon, robot or extra Guest permission. Optional JSON is
a separate target requiring its actual dependency; this core test omits it.
"""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess

HEADERS=("sport.h","b2_sport.h","topic_rpc.h","packet_bridge.h",
         "sport_operations.inc","b2_sport_operations.inc")

def pin(path):
    body=path.read_bytes();return {"bytes":len(body),"sha256":hashlib.sha256(body).hexdigest()}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ("unitree","records","fixture","native-cc","native-cxx","wasi","fast-io","runtime","validator"):
        parser.add_argument("--"+name,type=Path,required=True)
    args=parser.parse_args();work=Path.cwd();captured={};commands=[]
    def observe(path):
        # Preserve clang++ driver spelling while pinning its actual readable
        # bytes. Resolving that symlink into clang would change linker behavior.
        path=path.absolute();path.resolve(strict=True);captured[str(path)]=pin(path);return path
    unitree=args.unitree.resolve(strict=True);records=args.records.resolve(strict=True)
    fixture=observe(args.fixture);fast=args.fast_io.resolve(strict=True)
    native_cc=observe(args.native_cc);native_cxx=observe(args.native_cxx)
    wasi_cc=observe(args.wasi/"bin/clang");wasi_cxx=observe(args.wasi/"bin/clang++")
    runtime=observe(args.runtime);validator=observe(args.validator)
    generation=json.loads(observe(records/"generation.vendor.json").read_text())
    original=observe(records/"bounded_records.h")
    expected=generation["outputs"]["bounded_records.h"]
    if pin(original)!={k:expected[k] for k in ("bytes","sha256")} or len(generation["records"])!=8:
        raise ValueError("actual whole complete API C records required")
    include=work/"include";public=include/"platform/unitree/c/roswasm_unitree";public.mkdir(parents=True)
    for name in HEADERS:shutil.copyfile(observe(unitree/"include/roswasm_unitree"/name),public/name)
    family=include/"complete-api";family.mkdir();shutil.copyfile(original,family/"bounded_records.h")
    sources=[observe(unitree/name) for name in ("sport.cpp","b2_sport.cpp","packet_bridge.c")]+[fixture]
    for name in ("fast_io.h","fast_io_unit/floating/impl.h"):observe(fast/name)
    def run(name,argv):
        stdout=work/(name+".stdout");stderr=work/(name+".stderr")
        with stdout.open("xb") as out,stderr.open("xb") as err:
            p=subprocess.run([str(a) for a in argv],stdout=out,stderr=err,check=False)
        commands.append({"name":name,"argv":[str(a) for a in argv],"exit":p.returncode,
                         "stdout":pin(stdout),"stderr":pin(stderr)})
        if p.returncode:raise ValueError(name+" failed; original stdout/stderr retained")
        return stdout.read_text()
    artifacts={}
    for target in ("native","wasm"):
        cc,cxx=(native_cc,native_cxx) if target=="native" else (wasi_cc,wasi_cxx)
        objects=[]
        for index,source in enumerate(sources):
            cpp=source.suffix==".cpp";compiler=cxx if cpp else cc
            obj=work/(target+"-"+str(index)+".o");objects.append(obj)
            argv=[compiler,"-std=c++20" if cpp else "-std=c11","-O2","-g0","-UNDEBUG",
                  "-I",include,"-I",family,"-I",fast,"-c",source,"-o",obj]
            if cpp:argv.extend(["-fno-exceptions","-fno-rtti"])
            run(target+"-compile-"+str(index),argv)
        binary=work/("packet-native" if target=="native" else "packet.wasm")
        stack=["-Wl,-z,stack-size=262144"] if target=="wasm" else []
        run(target+"-link",[cxx,*objects,"-fno-exceptions","-fno-rtti",*stack,"-o",binary])
        if target=="wasm":
            if binary.read_bytes()[:8]!=b"\0asm\1\0\0\0":raise ValueError("real Wasm command required")
            run("wasm-validate",[validator,"validate",binary])
            output=run("wasm-value",[runtime,"--run",binary])
        else:output=run("native-value",[binary])
        if "UNITREE_PACKET_LOCAL_OK operations=60 fixed_slots=16" not in output:
            raise ValueError("actual complete packet value marker missing")
        artifacts[target]={"path":str(binary),**pin(binary)}
    for path,before in captured.items():
        if pin(Path(path))!=before:raise ValueError("actual packet dependency changed")
    result={"status":"PASS_CORE_PACKET_NATIVE_AND_WASM_VALUES_ONLY","artifacts":artifacts,
        "source_and_tool_inputs":captured,"commands":commands,"payload_methods":60,"pending_slots":16,
        "native_value_runs":1,"actual_Wasm_Guest_runs":1,"native_ROS_runs":0,
        "optional_JSON_tested":False,"new_language_SDKs_qualified":0,"default_authority":[]}
    result["Wasm_explicit_stack_bytes"]=262144
    (work/"result.json").write_text(json.dumps(result,indent=2)+"\n")
    print(json.dumps({"status":result["status"],"payload_methods":60,"pending_slots":16},sort_keys=True))

if __name__=="__main__":main()
