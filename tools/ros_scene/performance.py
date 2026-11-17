#!/usr/bin/env python3
"""Build and observe scene/native clock and guard workloads within an existing gate.

This tool refuses to compile or execute workloads unless its own process is
already in the selected, running Docker container's exact 64 GiB/zero-swap
cgroup. It never creates a group, relaxes a limit or changes sysctl. Analysis
targets only the launched process tree; detailed uarch collection can still
use VTune's driverless system-wide PMU collector. Native means the installed ROS rcl implementation, not
a surrogate. Scene measurements cover C++ broker dispatch, not Wasm execution.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time

ROOT = Path(__file__).resolve().parents[2]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("mode", choices=["prepare", "baseline", "profile"])
parser.add_argument("--container", default="uwvm-sdk-resource-gate-20261024")
parser.add_argument("--output", type=Path, default=ROOT.parent / "uwvm2-ros-sdk/.tmp/ros-scene-security/performance")
parser.add_argument("--cpu", type=int, default=2)
parser.add_argument("--vtune", default="/opt/intel/oneapi/vtune/latest/bin64/vtune")
args = parser.parse_args()
out = args.output.resolve()
info = json.loads(subprocess.check_output(["docker", "inspect", args.container]))[0]
group = Path("/sys/fs/cgroup/system.slice") / ("docker-" + info["Id"] + ".scope")
membership = "0::/" + str(group.relative_to("/sys/fs/cgroup"))


def verify_gate():
    if (not info["State"]["Running"] or Path("/proc/self/cgroup").read_text().strip() != membership
            or (group / "memory.max").read_text().strip() != "68719476736"
            or (group / "memory.swap.max").read_text().strip() != "0"):
        raise SystemExit("Refusing workload outside the existing exact 64 GiB, zero-swap cgroup")


verify_gate()
out.mkdir(parents=True, exist_ok=True)
record = {"mode": args.mode, "cgroup": membership, "cgroup_inode": group.stat().st_ino,
          "memory_max": 68719476736, "swap_max": 0, "before_events": (group / "memory.events").read_text(),
          "loadavg_start": Path("/proc/loadavg").read_text(), "cpu": args.cpu,
          "uname": list(os.uname()), "runs": [], "wasm_executed": False}
for block in Path('/proc/cpuinfo').read_text().split('\n\n'):
    fields = dict(line.split(':', 1) for line in block.splitlines() if ':' in line)
    fields = {key.strip(): value.strip() for key, value in fields.items()}
    if fields.get('processor') == str(args.cpu):
        record['cpu_info'] = {key: fields[key] for key in ['vendor_id', 'cpu family', 'model', 'model name', 'stepping', 'microcode', 'core id'] if key in fields}
        break
record['cpu_policy'] = {name: (group/name).read_text().strip() for name in ['cpu.max', 'cpuset.cpus.effective'] if (group/name).exists()}


def save():
    (out / (args.mode + ".json")).write_text(json.dumps(record, indent=2) + "\n")


def run(label, command, *, env=None, input=None, timeout=240):
    verify_gate()
    started = time.monotonic()
    result = subprocess.run(command, input=input, capture_output=True, env=env, timeout=timeout, cwd=ROOT)
    (out / (label + ".stdout")).write_bytes(result.stdout)
    (out / (label + ".stderr")).write_bytes(result.stderr)
    record["runs"].append({"label": label, "argv": command, "exit": result.returncode,
                           "seconds": time.monotonic() - started})
    save()
    print(label, result.returncode, flush=True)
    if result.returncode:
        print(result.stderr.decode(errors="replace")[-8000:], flush=True)
        raise SystemExit(result.returncode)
    return result.stdout


def container_python(label, script, *extra, input=None):
    return run(label, ["docker", "exec", "-i", args.container, "python3", "-c", script, *extra], input=input)


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


if args.cpu not in os.sched_getaffinity(0):
    raise SystemExit("Selected benchmark CPU is not in the inherited allowed CPU set")
if args.mode != "prepare":
    prepared = json.loads((out / "prepare.json").read_text())
    record["prepare_sha256"] = digest(out / "prepare.json")
    record["binary_sha256"] = {provider: digest(out / provider) for provider in ["scene", "native"]}
    if record["binary_sha256"] != prepared["binary_sha256"] or not prepared.get("source_unchanged"):
        raise SystemExit("Benchmark binaries or preparation sources differ from their recorded build")
    record["source_sha256"] = prepared["source_sha256"]
    record["native_libraries_sha256"] = {path.name: digest(path) for path in sorted((out / "native-libs").iterdir())}
    if record["native_libraries_sha256"] != {Path(name).name: value for name, value in prepared["native_libraries"].items()}:
        raise SystemExit("Native library closure differs from the recorded build")


if args.mode == "prepare":
    inputs = ["test/0084.ros-runtime/performance.cc", "src/uwvm2/imported/ros/scene/authority.h",
              "src/uwvm2/imported/ros/scene/broker.h", "src/uwvm2/imported/ros/runtime/resources.h",
              "src/uwvm2/imported/ros/runtime/contract.h"]
    record["source_sha256"] = {name: digest(ROOT / name) for name in inputs}
    for name in inputs:
        snapshot = out / "sources" / name
        snapshot.parent.mkdir(parents=True, exist_ok=True)
        snapshot.write_bytes((ROOT / name).read_bytes())
    container_root = "/dev/uwvm-ros-performance"
    for index, name in enumerate(inputs):
        container_python("source-copy-" + str(index),
                         "import sys,pathlib;p=pathlib.Path(sys.argv[1]);p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(sys.stdin.buffer.read())",
                         container_root + "/" + name, input=(ROOT / name).read_bytes())
    build = '''import json,os,pathlib,subprocess,sys
root=pathlib.Path(sys.argv[1]);source=root/'test/0084.ros-runtime/performance.cc'
temp=pathlib.Path('/dev/shm/uwvm-ros-performance');temp.mkdir(exist_ok=True)
env=dict(os.environ,TMPDIR=str(temp),LD_LIBRARY_PATH='/opt/ros/jazzy/lib')
commands=[]
for provider in ['scene','native']:
 command=['g++','-std=c++23','-O3','-g','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-pthread',
          '-fdebug-prefix-map='+str(root)+'='+sys.argv[2],'-I'+str(root/'src'),str(source),'-o',str(root/provider)]
 if provider=='native':
  command+=['-DUWVM_SCENE_BENCH_NATIVE']
  command+=['-I'+str(p) for p in pathlib.Path('/opt/ros/jazzy/include').iterdir() if p.is_dir()]
  command+=['-L/opt/ros/jazzy/lib','-lrcl','-lrcutils','-lrmw']
 print(json.dumps(command),flush=True);subprocess.run(command,check=True,env=env);commands.append(command)
 print(subprocess.check_output([str(root/provider),'mixed','1000','3'],env=env,text=True),flush=True)
print(subprocess.check_output(['g++','--version'],text=True),flush=True)
'''
    container_python("build-and-container-smoke", build, container_root, str(out / "sources"))
    for provider in ["scene", "native"]:
        destination = out / provider
        destination.write_bytes(run("binary-copy-" + provider, ["docker", "exec", args.container, "cat", container_root + "/" + provider]))
        destination.chmod(0o755)
    closure = container_python("native-library-closure", '''import json,os,re,subprocess
env=dict(os.environ,LD_LIBRARY_PATH='/opt/ros/jazzy/lib');paths=set()
roots=['/dev/uwvm-ros-performance/native','/opt/ros/jazzy/lib/librmw_fastrtps_cpp.so','/opt/ros/jazzy/lib/librcl_logging_spdlog.so']
for root in roots:
 paths.add(root) if root.endswith('.so') else None
 output=subprocess.check_output(['ldd',root],env=env,text=True)
 if 'not found' in output:raise SystemExit(output)
 for line in output.splitlines():
  match=re.search(r'=> (/[^ ]+)',line)
  if match:paths.add(match.group(1))
print(json.dumps(sorted(paths)))
''')
    paths = json.loads(closure)
    lib = out / "native-libs"
    lib.mkdir(exist_ok=True)
    # Both binaries use the same host C/C++ runtime. ROS and its other native
    # dependencies are copied from the installed container and hash-recorded.
    excluded = {"libc.so.6", "libm.so.6", "libstdc++.so.6", "libgcc_s.so.1", "libpthread.so.0", "libdl.so.2", "librt.so.1"}
    record["native_libraries"] = {}
    for index, path in enumerate(paths):
        destination = lib / Path(path).name
        if destination.name in excluded:
            continue
        destination.write_bytes(run("library-copy-" + str(index), ["docker", "exec", args.container, "cat", path]))
        record["native_libraries"][path] = digest(destination)
    record["binary_sha256"] = {provider: digest(out / provider) for provider in ["scene", "native"]}
    record["source_sha256_after"] = {name: digest(ROOT / name) for name in inputs}
    record["source_unchanged"] = record["source_sha256"] == record["source_sha256_after"]

environment = dict(os.environ, LD_LIBRARY_PATH=str(out / "native-libs"), RMW_IMPLEMENTATION="rmw_fastrtps_cpp")
if args.mode in ("prepare", "baseline"):
    record["observations"] = []
    iterations, batches = (1000, 3) if args.mode == "prepare" else (200000, 101)
    for round_number in range(1 if args.mode == "prepare" else 3):
        for workload in ["clock", "guard", "mixed"]:
            providers = ["scene", "native"] if round_number % 2 == 0 else ["native", "scene"]
            for provider in providers:
                command = ["taskset", "-c", str(args.cpu), str(out / provider), workload, str(iterations), str(batches)]
                data = run("host-" + provider + "-" + workload + "-" + str(round_number), command, env=environment)
                observation = json.loads(data)
                observation["round"] = round_number
                record["observations"].append(observation)

if args.mode == "profile":
    run("vtune-version", [args.vtune, "-version"])
    for provider in ["scene", "native"]:
        for analysis in ["hotspots", "uarch-exploration"]:
            result = out / ("vtune-" + provider + "-" + analysis)
            if result.exists():
                raise SystemExit("Refusing to overwrite existing VTune results: " + str(result))
            knobs = (["-knob", "sampling-mode=hw", "-knob", "enable-stack-collection=true"] if analysis == "hotspots"
                     else ["-knob", "pmu-collection-mode=detailed", "-knob", "dram-bandwidth-limits=false"])
            command = [args.vtune, "-collect", analysis, *knobs, "-knob", "sampling-interval=1",
                       "-no-analyze-system", "-data-limit=200", "-return-app-exitcode", "-result-dir", str(result),
                       "-source-search-dir", str(out / "sources"), "--", "taskset", "-c", str(args.cpu), str(out / provider),
                       "mixed", "300000", "101"]
            run("collect-" + provider + "-" + analysis, command, env=environment, timeout=300)
            for report_type in ["summary", "hotspots"]:
                run("report-" + provider + "-" + analysis + "-" + report_type,
                    [args.vtune, "-report", report_type, "-result-dir", str(result), "-format", "csv"], timeout=120)
            if analysis == "hotspots":
                functions = (["uwvm2::imported::ros::scene::authority::admit", "uwvm2::imported::ros::scene::broker::invoke",
                              "uwvm2::imported::ros::resources::registry::wait"] if provider == "scene"
                             else ["benchmark::environment::clock_step", "benchmark::environment::guard_step"])
                for function in functions:
                    run("source-" + provider + "-" + function.rsplit("::", 1)[-1],
                        [args.vtune, "-report", "hotspots", "-result-dir", str(result), "-source-object", "function=" + function,
                         "-source-search-dir", str(out / "sources"), "-format", "csv"], timeout=120)

verify_gate()
record["after_events"] = (group / "memory.events").read_text()
record["loadavg_finish"] = Path("/proc/loadavg").read_text()
if args.mode != "prepare":
    record["binary_sha256_after"] = {provider: digest(out / provider) for provider in ["scene", "native"]}
    if record["binary_sha256_after"] != record["binary_sha256"]:
        raise SystemExit("Benchmark executable changed during measurement")
record["complete"] = True
save()
