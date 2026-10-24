#!/usr/bin/env python3
"""Actual Nim/WASI SDK and command builds, full values and default denial.

Use only as a finite workload of the original common Linux resource owner.
Links the existing unmodified Nim ROS shim/startup and genuine previously built
packet-library objects. This is a private integration receipt, not public SDK
installation, a producer profile, positive native ROS or a whole official port.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import shutil
import subprocess


def pin(path):
    body = path.read_bytes()
    return {"bytes": len(body), "sha256": hashlib.sha256(body).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("unitree", "foundation", "records", "fixture", "nim-root", "wasi", "packet-run", "runtime", "validator", "checker", "memory-profile"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    work = Path.cwd()
    captured = {}
    commands = []

    def observe(path):
        path = path.absolute()
        path.resolve(strict=True)
        captured[str(path)] = pin(path)
        return path

    nim = observe(args.nim_root / "bin/nim")
    pretty = observe(args.nim_root / "bin/nimpretty")
    cc = observe(args.wasi / "bin/clang")
    cxx = observe(args.wasi / "bin/clang++")
    runtime = observe(args.runtime)
    validator = observe(args.validator)
    checker = observe(args.checker)
    observe(checker.parent.parent / "abi/abi-v1.json")
    memory_path=observe(args.memory_profile)
    memory=json.loads(memory_path.read_text())
    expected_memory={"format":"uwvm-unitree-wasm32-command-memory-v1","target":"wasm32-wasip1",
        "memory_index":0,"owned_memories":1,"shared":False,"memory64":False,
        "stack_bytes":1048576,"initial_memory_bytes":16777216,"maximum_memory_bytes":67108864,
        "core_packet_proven_stack_bytes":262144,"default_authority":[]}
    if any(type(memory.get(key)) is not type(value) or memory[key]!=value for key,value in expected_memory.items()):
        raise ValueError("actual reviewed complete-record command memory profile required")
    inspector_spec=importlib.util.spec_from_file_location("task05_nim_actual_inspector",checker)
    inspector=importlib.util.module_from_spec(inspector_spec)
    inspector_spec.loader.exec_module(inspector)
    foundation = args.foundation.resolve(strict=True)
    for path in sorted(foundation.rglob("*")):
        if path.is_file():
            observe(path)
    packet = json.loads(observe(args.packet_run / "work/result.json").read_text())
    if packet["status"] != "PASS_CORE_PACKET_NATIVE_AND_WASM_VALUES_ONLY":
        raise ValueError("genuine successful packet-library build required")
    for path in (cc, cxx):
        if packet["source_and_tool_inputs"][str(path)] != pin(path):
            raise ValueError("packet and Nim C/C++ provider pins differ")
    objects = [observe(args.packet_run / ("work/wasm-" + str(i) + ".o")) for i in range(3)]
    source = work / "source"
    source.mkdir()
    origins = {"unitree_sport.nim": args.unitree / "language/nim/unitree_sport.nim",
               "sport_topic_command.nim": args.unitree / "examples/nim/sport_topic_command.nim",
               "unitree_api_records.nim": args.records,
               "nim_sport_values.nim": args.fixture}
    for name, path in origins.items():
        shutil.copyfile(observe(path), source / name)

    def run(name, argv, data=b"", expected=0):
        out, err = work / (name + ".stdout"), work / (name + ".stderr")
        with out.open("xb") as stdout, err.open("xb") as stderr:
            result = subprocess.run([str(value) for value in argv], input=data,
                                    stdout=stdout, stderr=stderr, check=False)
        commands.append({"name": name, "argv": [str(value) for value in argv],
                         "exit": result.returncode, "expected_exit": expected,
                         "stdin": {"bytes": len(data), "sha256": hashlib.sha256(data).hexdigest()},
                         "stdout": pin(out), "stderr": pin(err)})
        if result.returncode != expected:
            raise ValueError(name + " failed; genuine compiler/runtime logs retained")
        return out.read_bytes()

    files = [source / name for name in origins]
    run("pretty-first", [pretty, "--indent:2", "--maxLineLen:100", *files])
    formatted = {str(path): pin(path) for path in files}
    run("pretty-second", [pretty, "--indent:2", "--maxLineLen:100", *files])
    if any(pin(Path(path)) != value for path, value in formatted.items()):
        raise ValueError("actual Nim formatter not idempotent")
    artifacts = {}
    for name in ("nim_sport_values", "sport_topic_command"):
        entry = work / (name + "_entry.nim")
        entry.write_text('{.compile: ' + json.dumps(str(foundation / "bridge/shim.c")) + '.}\n' +
                         '{.compile: ' + json.dumps(str(foundation / "bridge/main.c")) + '.}\n' +
                         (source / (name + ".nim")).read_text())
        binary = work / (name + ".wasm")
        links = ["--passL:" + str(path) for path in objects] if name == "nim_sport_values" else []
        run("build-" + name, [nim, "c", "--mm:arc", "--threads:off", "--exceptions:goto",
            "--cpu:wasm32", "--os:any", "-d:wasm", "-d:useMalloc", "-d:noSignalHandler",
            "--stackTrace:off", "--lineTrace:off", "--parallelBuild:1", "--cc:clang",
            "--clang.exe:" + str(cc), "--clang.linkerexe:" + str(cxx),
            "--passC:--target=wasm32-wasip1", "--passL:--target=wasm32-wasip1",
            "--passC:--sysroot=" + str(args.wasi / "share/wasi-sysroot"),
            "--passL:--sysroot=" + str(args.wasi / "share/wasi-sysroot"),
            "--passL:-fno-exceptions", "--passL:-fno-rtti",
            "--passL:-Wl,--threads=1",
            "--passC:-DROSWASM_C_FFI_BACKEND=2", "--passC:-Dmain=roswasm_c_ffi_nim_main",
            "--passL:-Wl,-z,stack-size="+str(memory['stack_bytes']),
            "--passL:-Wl,--initial-memory="+str(memory['initial_memory_bytes']),
            "--passL:-Wl,--max-memory="+str(memory['maximum_memory_bytes']),
            "--path:" + str(foundation / "src"),
            "--path:" + str(source), "--nimcache:" + str(work / (name + "-cache")),
            "--out:" + str(binary), *links, entry])
        if binary.read_bytes()[:8] != b"\0asm\1\0\0\0":
            raise ValueError("real Wasm command required")
        run("validate-" + name, [validator, "validate", binary])
        image=binary.read_bytes()
        imported=inspector.inspect(image)
        if any(row['module'].startswith('uwvm_ros') for row in imported):
            run("imports-" + name, ["/usr/bin/python3", "-I", "-S", "-B", checker, binary,
                "--backend", "2", "--allow-module", "wasi_snapshot_preview1",
                "--report", work / (name + "-imports.json")])
        elif name!='nim_sport_values' or any(row['module']!='wasi_snapshot_preview1' or row['kind']!=0 for row in imported):
            raise ValueError("the application must retain real ROS imports; pure values admit only actual WASI functions")
        else:
            (work/(name+'-imports.json')).write_text(json.dumps({
                'status':'PURE_VALUE_IMPORTS_ONLY','ROS_functions_checked':0,'imports':imported,
                'ABI_coverage_qualified':False,'default_authority':[]},indent=2)+'\n')
        if any(row['kind']==2 for row in imported):
            raise ValueError("an imported memory is outside the selected Nim command profile")
        reader=inspector.Reader(image)
        reader.take(8)
        observed=[]
        while reader.position<len(reader.data):
            section=reader.byte()
            part=inspector.Reader(reader.take(reader.uint()))
            if section==5:
                if part.uint()!=1 or part.uint()!=1:
                    raise ValueError("one owned nonshared memory32 with a maximum is required")
                initial,maximum=part.uint(),part.uint()
                if part.position!=len(part.data):raise ValueError("trailing memory section fields")
                observed.append({"initial_pages":initial,"maximum_pages":maximum})
        if observed!=[{"initial_pages":memory['initial_memory_bytes']//65536,
                       "maximum_pages":memory['maximum_memory_bytes']//65536}]:
            raise ValueError("actual Nim Wasm memory bounds differ from the product profile")
        artifacts[name] = {"path": str(binary), **pin(binary)}
        artifacts[name]['owned_memory']=observed[0]
    fixture, app = work / "nim_sport_values.wasm", work / "sport_topic_command.wasm"
    if b"UNITREE_NIM_VALUES_OK operations=60 fixed_slots=16" not in run("sdk-values", [runtime, "--run", fixture]):
        raise ValueError("complete actual SDK values marker missing")
    request = run("emit-original-request", [runtime, "--run", fixture, "emit-request"])
    methods = run("list-methods", [runtime, "--run", fixture, "list"]).decode("ascii").splitlines()
    if len(methods) != 60:
        raise ValueError("all original payload methods required")
    outputs = []
    shape_arguments = {0: [], 1: ["1.25", "-0.0", "0.5"], 2: [str(-(1 << 31))],
                       3: ["1"], 4: ["-0.125"], 6: ["1.25", "-0.0", "0.5"],
                       5: [value for i in range(30) for value in (str(i), "0", "-0.0", "0", "0", "0", "0")]}
    for i, row in enumerate(methods):
        family, name, shape = row.split()
        output = run("prepare-" + str(i), [runtime, "--run", app, "prepare", family, name,
                                          *shape_arguments[int(shape)]], request)
        outputs.append(output)
    if b"UNITREE_NIM_COMMANDS_OK operations=60 complete_records=1" not in run(
            "verify-command-records", [runtime, "--run", fixture, "check-commands"], b"".join(outputs)):
        raise ValueError("actual complete command outputs differ from genuine native library")
    refused = [("prepare", "wrong", "Move"), ("prepare", "go2", "Missing"),
               ("prepare", "go2", "Move", "0", "0"), ("prepare", "go2", "Move", "nan", "0", "0"),
               ("prepare", "b2", "ContinuousGait", "2"), ("prepare", "go2", "SpeedLevel", "2147483648")]
    for i, argv in enumerate(refused):
        run("refuse-arguments-" + str(i), [runtime, "--run", app, *argv], expected=22)
    for family in ("go2", "b2"):
        output = run("deny-ungranted-" + family,
            [runtime, "--run", app, "send", family, "Move", "unitree-pub", "unitree-sub",
             "10", "100", "1", "0", "0", "0"], expected=13)
        if b"DONE primary=13 subscription=0 publisher=0 native_valid=0 native_code=0" not in output or b"READY" in output:
            raise ValueError("ungranted open must refuse before stdin and owner readiness")
    for path, before in captured.items():
        if pin(Path(path)) != before:
            raise ValueError("original Nim/packet/foundation/provider input changed")
    result = {"status": "PASS_PRIVATE_NIM_WASI_VALUES_COMMANDS_AND_DEFAULT_DENIAL",
        "artifacts": artifacts, "source_and_tool_inputs": captured, "commands": commands,
        "formatted": formatted, "payload_methods": 60, "command_preparations": 60,
        "pending_slots": 16, "invalid_argument_refusals": len(refused), "ungranted_pair_refusals": 2,
        "actual_Wasm_Guest_runs": sum(row["name"] in ("sdk-values", "emit-original-request", "list-methods", "verify-command-records") or row["name"].startswith(("prepare-", "refuse-arguments-", "deny-ungranted-")) for row in commands),
        "public_installed_SDK_qualified": False, "native_ROS_runs": 0, "default_authority": [],
        "Wasm_explicit_stack_bytes": memory['stack_bytes'],"wasm_memory_profile":{**pin(memory_path),"path":str(memory_path)}}
    (work / "result.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps({"status": result["status"], "payload_methods": 60, "command_preparations": 60}))


if __name__ == "__main__":
    main()
