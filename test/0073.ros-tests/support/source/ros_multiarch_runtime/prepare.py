#!/usr/bin/env python3
"""Compile one real target interpreter and an independent native ROS fixture.

No Runtime acceptance is asserted by PREPARED. Each profile builds fresh
products from the same full frozen source, using one compiler job and ordinary
system-inclusive -MD. The Runtime's target is checked independently from the
Gateway/peer target; this never claims a foreign architecture has native DDS.
"""
import argparse
from pathlib import Path
import json
import os
import shlex
import sys
import traceback

sys.dont_write_bytecode = True
from profile import validate, require, ARCHES, available
from engine import Frame, environment, executable, elf, json_input


def dependencies(frame, depfile, cwd):
    """Read a real compiler Make rule; support escaped spaces, never eval it."""
    raw = Path(depfile).read_text();require(len(raw) <= 16*1024**2, "bounded full-MD required")
    text = raw.replace("\\\n", " ");colon = text.find(":")
    require(colon > 0, "compiler dependency object rule missing")
    names, token, escaped = [], [], False
    for char in text[colon+1:]:
        if escaped:
            token.append(char);escaped=False
        elif char == "\\":
            escaped=True
        elif char.isspace():
            if token:
                names.append("".join(token));token=[]
        else:
            token.append(char)
    if token:
        names.append("".join(token))
    require(not escaped and names and len(names) <= 65536, "complete bounded Make dependency list required")
    pins = []
    for name in dict.fromkeys(names):
        path = Path(name);path = path if path.is_absolute() else Path(cwd)/path
        pins.append(frame.pin(path))
    return {"depfile":frame.pin(depfile),"inputs":pins,"scope":"Original full system-inclusive MD, not a dependency replay or link/runtime dependency graph"}


def arguments():
    parser=argparse.ArgumentParser(description=__doc__,allow_abbrev=False)
    for name in ("source","manifest","profile","out"):
        parser.add_argument("--"+name,type=Path,required=True)
    return parser.parse_args()


def main():
    args=arguments();frame=None;status="FAIL";error=None
    try:
        profile,profile_pin=json_input(args.profile);validate(profile)
        available(profile)
        frame=Frame(args.source,args.manifest,args.profile,profile,args.out,profile_pin)
        frame.common.install_interrupts()
        runner=frame.runner_for(frame.out/"compile",compiler=True)
        base=["--driver-mode=g++","-std=c++26","-O1","-g0","-UNDEBUG","-pthread","-Wno-undefined-inline","-fconstexpr-steps=8388608",
              "-DUWVM=2","-DUWVM_USE_UWVM_INT","-DUWVM_DISABLE_JIT","-DUWVM_USE_THREAD_LOCAL",
              "-DUWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64","-DUWVM_ENABLE_WASIP1_SOCKET",
              "-DUWVM_VERSION_X=2","-DUWVM_VERSION_Y=0","-DUWVM_VERSION_Z=3","-DUWVM_VERSION_S=0",
              "-DUWVM_IMPORT_ROS1","-DUWVM_IMPORT_ROS2"]
        for name in ("src","third-parties/fast_io/include","third-parties/bizwen/include","third-parties/boost_unordered/include"):
            base += ["-I",str(frame.source/name)]
        compiler=Path(profile["compiler"]).resolve(strict=True);frame.pin(compiler)
        require(elf(compiler,frame.common)["machine"]==62,"explicit compiler must run on actual host ELF62")
        env=environment();env.update(profile["compiler_environment"])
        runner.owner.run("compiler-version",[str(compiler),"--version"],runner.out,env)
        worker_dir=frame.out/"worker";worker_dir.mkdir()
        objects=[];records=[]
        for name,relative in (("main","src/uwvm2/uwvm/main.default.cpp"),("runtime","src/uwvm2/runtime/lib/uwvm_runtime.default.cpp"),
                              ("host_api","src/uwvm2/uwvm/host_api.default.cpp")):
            obj=worker_dir/(name+".o");dep=worker_dir/(name+".d")
            flags=[*profile["compile_flags"],*base]
            if name=="main":
                flags += ["-include",str(frame.source/"test/0073.ros-tests/support/source/ros_multiarch_runtime/stop.h")]
            command=[str(compiler),*flags,"-MD","-MF",str(dep),"-c",str(frame.source/relative),"-o",str(obj)]
            runner.owner.run("worker-"+name,command,runner.out,env)
            image=elf(obj,frame.common)
            require(image["type"]==1 and (image["machine"],image["bits"],image["endian"])==ARCHES[profile["runtime"]["architecture"]], "compiled object architecture mismatch")
            records.append({"source":relative,"object":frame.pin(obj),"dependencies":dependencies(frame,dep,runner.out)})
            objects.append(str(obj))
        worker=worker_dir/"uwvm2-ros"
        runner.owner.run("worker-link",[str(compiler),"--driver-mode=g++",*profile["link_flags"],"-pthread",*objects,"-ldl","-o",str(worker)],runner.out,env)
        image=frame.image(worker,profile["runtime"])
        frame.report["worker"]={"image":image,"objects":records,"pre_main_barrier":"injected into actual main translation unit only"}
        # During preparation, only the Runtime compiler/linker uses compiler_rss
        # (up to 8 GiB).
        # Restore the independent native 2-GiB ceiling before CMake can start
        # feature probes, the Gateway compiler or either native peer compiler.
        runner.owner.limits["rss_bytes"]=profile["limits"]["native_rss"]
        # The independent native SDK keeps its own architecture, full compiler
        # flags, CMake feature probes and package identity. No cross try_run.
        build=frame.out/"native-build";cmake=Path(profile["cmake"]).resolve(strict=True)
        make=Path(profile["make"]).resolve(strict=True);frame.pin(cmake);frame.pin(make)
        native_env=environment();native_env.update(profile["compiler_environment"])
        native_env.update({k:v for k,v in profile["native"]["execution"]["environment"].items() if k!="LD_LIBRARY_PATH"})
        configure=[str(cmake),"-S",str(frame.source/"test/0073.ros-tests/support/source/ros_multiarch_runtime"),"-B",str(build),"-G","Unix Makefiles",
                   "-DCMAKE_MAKE_PROGRAM="+str(make),"-DUWVM_MULTIARCH_SOURCE="+str(frame.source),
                   "-DUWVM_MULTIARCH_BACKEND="+str(profile["backend"]),"-DCMAKE_CXX_STANDARD=17",
                   "-DCMAKE_BUILD_TYPE=None","-DCMAKE_CXX_FLAGS=-O1 -g0 -UNDEBUG -MD",*profile["native_cmake_flags"]]
        forbidden=("-DUWVM_MULTIARCH_SOURCE=","-DUWVM_MULTIARCH_BACKEND=","-DCMAKE_MAKE_PROGRAM=")
        require(not any(x.startswith(forbidden) for x in profile["native_cmake_flags"]),"native flags cannot replace closed source/backend/make selection")
        runner.owner.run("native-configure",configure,runner.out,native_env)
        target="uwvm-ros"+str(profile["backend"])+"-gateway"
        runner.owner.run("native-build",[str(cmake),"--build",str(build),"--target",target,"uwvm-multiarch-sdk-peer","uwvm-multiarch-events-peer","--","-j1"],runner.out,native_env)
        database=build/"compile_commands.json";frame.pin(database);rows=json.loads(database.read_bytes())
        selected={"gateway":("src/ros/gateway/ros%d/main.cc"%profile["backend"],build/"gateway"/target),
                  "sdk-peer":("test/0073.ros-tests/support/source/ros_sdk_relay/native_ros%d_peer.cc"%profile["backend"],build/"uwvm-multiarch-sdk-peer"),
                  "events-peer":("test/0073.ros-tests/support/source/ros_unified_events/native_events_ros%d.cc"%profile["backend"],build/"uwvm-multiarch-events-peer")}
        frame.report["native_products"]={}
        for role,(relative,binary) in selected.items():
            matches=[r for r in rows if Path(r["file"]).resolve()==frame.source/relative]
            require(len(matches)==1,"exact original native compile command required")
            row=matches[0];command=row.get("arguments") or shlex.split(row["command"])
            require("-MD" in command and "-MMD" not in command and "-c" in command,"actual system-inclusive native MD required")
            require("-UNDEBUG" in command,"native assertions must remain active")
            output=Path(command[command.index("-o")+1]);output=output if output.is_absolute() else Path(row["directory"])/output
            dep=Path(str(output)+".d")
            frame.report["native_products"][role]={"image":frame.image(binary,profile["native"]),"source":relative,
                "compile_command":row,"object":frame.pin(output),"dependencies":dependencies(frame,dep,row["directory"]),
                "pre_main_barrier":"injected" if role=="gateway" else "existing actual native witness pre-init SIGSTOP"}
        frame.report["native_link_inputs"]=[frame.pin(p) for p in build.rglob("link.txt")]
        frame.report["actual_Runtime_or_ROS_behavior"]="NOT_EXECUTED"
        frame.report["compiler_input_closure"]="Full selected package catalogs and actual original system MD; link/runtime dependency graph not claimed"
        frame.report["kind"]="multiarch-runtime-products";status="PREPARED"
    except FileNotFoundError as caught:
        status="BLOCKED";error="Explicit SDK/tool input unavailable: "+str(caught)
    except BaseException as caught:
        error=repr(caught)
        if frame:
            frame.report["traceback"]=traceback.format_exc()
    if frame:
        return frame.finish(status,error)
    if status=="BLOCKED":
        args.out.mkdir(mode=0o700,parents=True,exist_ok=False)
        with (args.out/"report.json").open("x") as stream:
            json.dump({"kind":"multiarch-runtime-products","status":"BLOCKED","error":error,"Runtime_or_ROS_behavior":"NOT_EXECUTED"},stream)
        return 1
    raise RuntimeError(error)


if __name__=="__main__":
    raise SystemExit(main())
