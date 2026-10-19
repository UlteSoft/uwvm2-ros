#!/usr/bin/env python3
"""Emit a finite Root-owned FUTURE qualification plan. Never execute a command.
Missing actual Source/SDK33/runtime tools are refusal95, not a success stub.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys

PTHREAD_SHA = "c71f5fdc85acdd6864d125427cacab94345f5549b5a75a1819ee84334a8916c3"
TOOLCHAIN_SHA = "417c102556d8375b331e0dbbd69856c2d29d2222957ff34dfe266c7abebc7730"
def file(path, executable=False):
    path=path.resolve(strict=True)
    if not path.is_file() or (executable and not path.stat().st_mode&0o111):
        raise ValueError("actual regular executable/source missing: "+str(path))
    raw=path.read_bytes()
    return {"path":str(path),"bytes":len(raw),"sha256":hashlib.sha256(raw).hexdigest()}
def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ("source-root","sdk33-prefix","cross-prefix","work-parent",
                 "cmake","wasm-tools","runtime","runtime-args-json"):
        p.add_argument("--"+name,type=Path,required=True)
    for name in ("shared-initial-bytes","shared-maximum-bytes","main-stack-bytes"):
        p.add_argument("--"+name,type=int,required=True)
    a=p.parse_args()
    repo=a.source_root.resolve(strict=True);sdk=a.sdk33_prefix.resolve(strict=True)
    cross=a.cross_prefix.resolve(strict=True);work_parent=a.work_parent.resolve(strict=True)
    if not all(x.is_dir() for x in (repo,sdk,cross,work_parent)):
        raise ValueError("actual directories required")
    initial,maximum,stack=a.shared_initial_bytes,a.shared_maximum_bytes,a.main_stack_bytes
    if not all(0<x<=2147483648 and x%65536==0 for x in (initial,maximum,stack)) or not stack<initial<=maximum:
        raise ValueError("explicit valid page-aligned real memory/stack budgets required")
    spot=repo/"sdk/robots/mobile_manipulation/spot/protobuf"
    # Public composed Source is required, not a guessed private after directory.
    roles=[repo/"sdk/robots/mobile_manipulation/spot/sdk.h",
           spot/"include/document.h",spot/"include/document.hpp",spot/"src/nanopb_decode_merged.c",
           spot/"bridge-generated/bounded_records.h",spot/"bridge-generated/generation.vendor.json",
           spot/"generated/source-products.json",spot/"CMakeLists.txt",
           spot/"cmake/GeneratedSources.cmake",spot/"tests/document_contract.c",
           spot/"tests/document_contract.cc",repo/"sdk/include/roswasm/ros2.h",
           repo/"sdk/custom-types/native_current/SelectedNative.cmake"]
    observations=[file(x) for x in roles]
    toolchain=repo/"sdk/rmw_wasm/cmake/threaded-eh-toolchain.cmake"
    tc=file(toolchain)
    if tc["sha256"]!=TOOLCHAIN_SHA:raise ValueError("review a changed real toolchain Source first")
    pthread=file(sdk/"share/cmake/wasi-sdk-pthread.cmake")
    if pthread["sha256"]!=PTHREAD_SHA:raise ValueError("installed SDK33 pthread body differs")
    for lane in ("include/wasm32-wasi-threads/eh/c++/v1","lib/wasm32-wasi-threads/eh"):
        if not (sdk/"share/wasi-sysroot"/lane).is_dir():
            raise ValueError("actual SDK33 DUAL/ON EH lane unavailable; SDK24/C++17 names are insufficient")
    tools={name:file(sdk/"bin"/name,True) for name in ("clang","clang++","llvm-ar","llvm-ranlib")}
    tools.update(cmake=file(a.cmake,True),wasm_tools=file(a.wasm_tools,True),runtime=file(a.runtime,True))
    args_source=file(a.runtime_args_json)
    argument_bytes=Path(args_source["path"]).read_bytes()
    if hashlib.sha256(argument_bytes).hexdigest()!=args_source["sha256"]:
        raise ValueError("actual Runtime arguments changed during Source capture")
    runtime_args=json.loads(argument_bytes)
    if not isinstance(runtime_args,list) or len(runtime_args)>128 or not all(
        isinstance(s,str) and 0<len(s)<=4096 and "\0" not in s for s in runtime_args):
        raise ValueError("explicit actual Root runtime argument vector required")
    # This program cannot infer permissions from a compiler or ROS alias.
    # Root must independently admit these actual arguments/CLI producer and the
    # exact produced import census. No positive rights are inserted here.
    build=work_parent/"spot-full-protobuf-sdk33-qualification"
    if build.exists():raise ValueError("fresh build required; preserve old failures")
    cmake=tools["cmake"]["path"]
    configure=[cmake,"-S",str(spot),"-B",str(build),
        "--toolchain",str(toolchain),
        "-DUWVM_WASI_SDK33_PREFIX="+str(sdk),"-DUWVM_CROSS_PREFIX="+str(cross),
        "-DUWVM_SHARED_INITIAL_BYTES="+str(initial),"-DUWVM_SHARED_MAXIMUM_BYTES="+str(maximum),
        "-DUWVM_MAIN_STACK_BYTES="+str(stack),"-DUWVM_ROS_SOURCE_ROOT="+str(repo),
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
        "-DUWVM_SPOT_PROTOBUF_C=ON","-DUWVM_SPOT_PROTOBUF_CPP=ON",
        "-DUWVM_SPOT_PROTOBUF_CONTRACTS=ON","-DUWVM_SPOT_PROTOBUF_ROS_GUESTS=ON"]
    targets=["spot_proto_c_contract","spot_proto_cpp_contract",
             "spot_full_state_c_guest","spot_full_state_cpp_guest"]
    plan={"status":"PLAN_ONLY_NOT_EXECUTED_NOT_QUALIFICATION",
          "source":observations,"actual_tools":tools,"actual_toolchain":tc,"actual_pthread":pthread,
          "explicit_runtime_arguments":runtime_args,"runtime_arguments_source":args_source,
          "permissions_inferred_or_added":[],"real_sdk_compiler_version_and_producer_md_still_required":True,
          "before_every_step":"Root must re-open/hash every selected Source/tool/input condition; a changed file requires a new retained plan, not reuse of this observation",
          "steps":[
            {"run":[tools["clang"]["path"],"--version"],"capture":"actual-clang-version"},
            {"run":[tools["clang++"]["path"],"--version"],"capture":"actual-clangxx-version"},
            {"write_file":str(build/".cmake/api/v1/query/codemodel-v2"),"bytes":"",
             "note":"Root creates only this fresh owned query directory before configure"},
            {"run":configure,"capture":"configure-complete-stdout-stderr-exit-and-inputs"},
            {"run":[cmake,"--build",str(build),"--parallel","1","--verbose","--target"]+targets,
             "capture":"serial-final-executable-link+whole-compiler-MD"},
            {"resolve_actual_cmake_file_api_artifacts":{"build":str(build),"targets":targets,
              "require":"one real Wasm artifact per exact target, all paths/hashes/extents captured; no guessed suffix"}},
            {"for_each_actual_target":targets,
             "run":[tools["wasm_tools"]["path"],"validate","ACTUAL_FILE_API_ARTIFACT"],
             "capture":"actual-Wasm-validation"},
            {"for_each_actual_target":targets,
             "required_gate":"Root independently checks actual imports/shared-memory/TLS/EH/limits and current runtime producer admission; validator alone grants no leaf"},
            {"for_each_actual_target":targets[:2],
             "run":[tools["runtime"]["path"]]+runtime_args+["ACTUAL_FILE_API_ARTIFACT"],
             "capture":"real-C-and-CPP-wholevalue/merge/NUL/UTF8/unknown/depth/storage/sentinel results; exact physical join"},
            {"for_each_actual_target":targets[2:],
             "deferred_until":"Actual new ROS schemas+full Native IDs+paired Gateway metadata proof+operator Native allowlist+per-instance alias permission are independently qualified",
             "require":"telemetry-only controlled robot run; explicit token/nonce/budget input; genuine transport/provider/unknown/cleanup and full original-field reference comparison"}],
          "executed_by_this_planner":0}
    print(json.dumps(plan,indent=2))
if __name__=="__main__":
    try:main()
    except (OSError,ValueError,KeyError) as e:
        print(json.dumps({"status":"REFUSED95_NO_COMMAND_EXECUTED","reason":str(e)}),file=sys.stderr)
        raise SystemExit(95)
