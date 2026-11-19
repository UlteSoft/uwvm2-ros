"""Qualify bounded native Image round trips and controls on private loopback."""
import argparse
import json
import os
from pathlib import Path
import resource
import secrets
import statistics
import subprocess
import sys

from run import GROUP, cyclone_version_header, digest

HERE = Path(__file__).resolve().parent
COMPILER = Path("/usr/bin/clang++-22")
RMW = {"jazzy": "rmw_fastrtps_cpp", "humble": "rmw_cyclonedds_cpp"}
CASES = (
    ("jazzy", "jazzy", "reliable"),
    ("humble", "humble", "reliable"),
    ("jazzy", "humble", "reliable"),
    ("humble", "jazzy", "reliable"),
    ("jazzy", "humble", "incompatible"),
    ("jazzy", "humble", "wrong-step"),
)
SIZES = (5760, 921600)
FRAMES_PER_SIZE = 36
WARMUP = 4


def require(ok, message):
    if not ok:
        raise RuntimeError(message)


def gate():
    """Check the shared budget before compilation and again inside namespaces."""
    member = Path("/proc/self/cgroup").read_text().strip()
    require(member == "0::/" + str(GROUP.relative_to("/sys/fs/cgroup")) and
            (GROUP / "memory.max").read_text().strip() == "68719476736" and
            (GROUP / "memory.swap.max").read_text().strip() == "0" and
            os.sched_getaffinity(0) <= {4, 5, 6, 7}, "expected the configured test gate")
    return member


def environment(prefix, system, release, directory):
    """Keep middleware, home, discovery and security settings explicit."""
    lib = prefix / "lib"
    libs = [lib]
    if release == "humble":
        libs += [next(lib.glob("*/libddsc.so")).parent]
    return {
        "PATH": "/usr/bin:/bin",
        "HOME": str(directory),
        "ROS_LOG_DIR": str(directory / "ros-log"),
        "ROS_SECURITY_ENABLE": "false",
        "LD_LIBRARY_PATH": os.pathsep.join(map(str, [*libs, system])),
        "AMENT_PREFIX_PATH": str(prefix),
        "RMW_IMPLEMENTATION": RMW[release],
        "ROS_DOMAIN_ID": "61",
        "ROS_AUTOMATIC_DISCOVERY_RANGE": "SYSTEM_DEFAULT",
        "FASTDDS_DEFAULT_PROFILES_FILE": str(HERE / "fastdds-image-loopback.xml"),
        "FASTRTPS_DEFAULT_PROFILES_FILE": str(HERE / "fastdds-image-loopback.xml"),
        "RMW_FASTRTPS_USE_QOS_FROM_XML": "1",
        "CYCLONEDDS_URI": (HERE / "cyclone-image-loopback.xml").as_uri(),
    }


def run_peers(args, out):
    """Launch exactly two native processes in one isolated network."""
    interfaces = [line.split(":", 1)[0].strip()
                  for line in Path("/proc/net/dev").read_text().splitlines()[2:]]
    require(interfaces == ["lo"], "native peers require private loopback")
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    resource.setrlimit(resource.RLIMIT_FSIZE, (8 << 20, 8 << 20))
    request, echo, case = args.run_peers
    require((request, echo, case) in CASES, "unknown peer scenario")
    token = 1 + secrets.randbelow(2**29)
    runs = {}
    for release, role in [(request, "request"), (echo, "echo")]:
        home = out / role
        home.mkdir()
        env = environment(getattr(args, release + "_prefix"), getattr(args, release + "_system"), release, home)
        runs[role] = ([out.parent / (release + "-image"), role, RMW[release], case, str(token)], env)
    with (out / "echo.stdout").open("wb") as log, (out / "echo.stderr").open("wb") as diagnostics:
        cmd, env = runs["echo"]
        peer = subprocess.Popen(cmd, env=env, stdout=log, stderr=diagnostics)
        try:
            cmd, env = runs["request"]
            with (out / "request.stdout").open("wb") as request_log, (out / "request.stderr").open("wb") as request_errors:
                client = subprocess.run(cmd, env=env, stdout=request_log, stderr=request_errors, timeout=25)
            server = peer.wait(timeout=10)
        finally:
            if peer.poll() is None:
                peer.kill()
                peer.wait()
    (out / "processes.json").write_text(json.dumps({"token": token, "request": client.returncode,
                                                   "echo": server}) + "\n")
    require(client.returncode == server == 0, "peer exited unsuccessfully")


def compile_peer(prefix, system, release, out, sanitized, *, witness="image", message_package="sensor_msgs"):
    """Compile a native witness against the installation's generated type support."""
    include = prefix / "include"
    includes = [include, *(p for p in sorted(include.iterdir()) if (p / p.name).is_dir())]
    vendor = prefix / "lib"
    optional = ("libstatistics_collector", "tracetools", "rcl_interfaces__rosidl_typesupport_cpp",
                "rcpputils", "statistics_msgs__rosidl_typesupport_cpp", "rosgraph_msgs__rosidl_typesupport_cpp",
                "builtin_interfaces__rosidl_typesupport_cpp")
    args = [COMPILER, "-std=c++20", "-O3", "-g", "-Wall", "-Wextra", "-Werror",
            *[item for p in includes for item in ("-isystem", p)], HERE / ("ros_" + witness + ".cpp"),
            "-L" + str(vendor), "-L" + str(system), "-Wl,-rpath-link," + str(vendor),
            "-Wl,-rpath-link," + str(system), "-lrclcpp", "-lrcl", "-lrcutils", "-lrmw", "-lrmw_implementation",
            "-l" + message_package + "__rosidl_typesupport_cpp", "-ldl",
            *["-l" + name for name in optional if (vendor / ("lib" + name + ".so")).is_file()],
            *(["-fsanitize=address,undefined", "-fno-sanitize-recover=all"] if sanitized else []),
            "-o", out / (release + "-" + witness)]
    return command(args, out / ("compile-" + release + ".log"), timeout=60)


def command(argv, log, timeout):
    """Record a bounded compiler or isolation-launch command."""
    args = list(map(str, argv))
    env = {"PATH": "/usr/bin:/bin"}
    with log.open("wb") as stream:
        result = subprocess.run(args, timeout=timeout, env=env, stdout=stream, stderr=subprocess.STDOUT)
    require(result.returncode == 0, f"exit {result.returncode} in {log}")
    return args


def mapped_libraries(lines, prefix, release):
    """Bind loaded RMW, DDS and type-support libraries to the selected prefix."""
    required = {"librclcpp", "lib" + RMW[release], "libsensor_msgs__rosidl_typesupport_cpp",
                "libsensor_msgs__rosidl_typesupport_" + ("fastrtps_cpp" if release == "jazzy" else "introspection_cpp")}
    mappings = {}
    for line in lines:
        if not line.startswith("LIB "):
            continue
        path = Path(line.removeprefix("LIB ")).resolve(strict=True)
        stem = path.name.split(".so", 1)[0]
        if stem in required or stem in {"libfastdds", "libfastrtps", "libddsc"}:
            require(path.is_relative_to(prefix), f"loaded {stem} outside {release} installation")
            require(stem not in mappings, f"duplicate {stem} mapping")
            mappings[stem] = {"path": str(path), "sha256": digest(path)}
    vendor = {"libfastdds", "libfastrtps"} if release == "jazzy" else {"libddsc"}
    require(required <= mappings.keys() and len(vendor & mappings.keys()) == 1, "missing expected vendor mappings")
    return mappings


def check_case(case, result, args):
    """Require the witness for the declared outcome and preserve raw timings."""
    request, echo, mode = case
    receipt = json.loads((result / "processes.json").read_bytes())
    receipt["identities"] = {}
    lines = {}
    logs = [result / name for name in ("request.stdout", "request.stderr", "echo.stdout", "echo.stderr",
                                      "processes.json", "sandbox.log")]
    receipt["output_sha256"] = {log.name: digest(log) for log in logs}
    receipt["diagnostics"] = {}
    frames = FRAMES_PER_SIZE * len(SIZES) if mode == "reliable" else 0
    for release, role in [(request, "request"), (echo, "echo")]:
        lines[role] = (result / (role + ".stdout")).read_text().splitlines()
        diagnostics = (result / (role + ".stderr")).read_text(errors="replace")
        require("XMLPARSER" not in diagnostics and "config:" not in diagnostics, f"{role} rejected the transport profile")
        receipt["diagnostics"][role] = {
            "bytes": len(diagnostics.encode()),
            "rcutils_error_state_messages": diagnostics.count("rcutils_set_error_state()"),
        }
        require(lines[role].count(f"COMPLETE {role} {mode} {frames}") == 1, f"missing {role} completion witness")
        receipt["identities"][role] = mapped_libraries(lines[role], getattr(args, release + "_prefix"), release)
    timings = [tuple(map(int, line.split()[1:])) for line in lines["request"] if line.startswith("FRAME ")]
    if frames:
        expected = [(i + 1, SIZES[i // FRAMES_PER_SIZE]) for i in range(frames)]
        require([(sequence, size) for sequence, size, _ in timings] == expected and
                all(latency > 0 for _, _, latency in timings), "missing or disordered Image observations")
        receipt["round_trip_ns"] = {}
        for size in SIZES:
            raw = [latency for _, n, latency in timings if n == size]
            samples = sorted(raw[WARMUP:])
            receipt["round_trip_ns"][str(size)] = {
                "raw": raw, "warmup": WARMUP, "p50": statistics.median(samples),
                "p95": samples[(95 * len(samples) + 99) // 100 - 1],
            }
    else:
        require(not timings and lines["request"].count("PUBLISHED 1") == 1, "unexpected control delivery")
        witness = "RELIABILITY_MISMATCH" if mode == "incompatible" else "WRONG_STEP"
        require(lines["echo"].count(witness) == 1, "missing independent rejection witness")
    return receipt


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("jazzy-prefix", "jazzy-system", "humble-prefix", "humble-system", "output"):
        parser.add_argument("--" + name, type=lambda value: Path(value).resolve(), required=True)
    parser.add_argument("--sanitized", action="store_true", help="Enable sanitizer witnesses; exclude their timings")
    parser.add_argument("--run-peers", nargs=3, help=argparse.SUPPRESS)
    args = parser.parse_args()
    member = gate()
    out = args.output
    require(".tmp" in out.parts, "output must be below a project .tmp")
    if args.run_peers:
        run_peers(args, out)
        return
    out.mkdir()
    inputs = [Path(__file__).resolve(), HERE / "ros_image.cpp", HERE / "fastdds-image-loopback.xml",
              HERE / "cyclone-image-loopback.xml", HERE / "run.py", COMPILER]
    inputs += [getattr(args, release + "_prefix") / "share/sensor_msgs/msg/Image.idl" for release in RMW]
    inputs.append(cyclone_version_header(args.humble_prefix))
    report = {"complete": False, "format": "uwvm-native-image-roundtrip-v1",
              "scope": "Native rclcpp sensor_msgs/Image over private plaintext UDPv4 loopback",
              "cgroup": member, "cpu_set": sorted(os.sched_getaffinity(0)), "sanitized": args.sanitized,
              "inputs": {str(path): digest(path) for path in inputs}, "compile": {}, "cases": []}
    path = out / "results.json"

    def save():
        path.write_text(json.dumps(report, indent=2) + "\n")

    save()
    for release in RMW:
        report["compile"][release] = compile_peer(getattr(args, release + "_prefix"),
                                                  getattr(args, release + "_system"), release, out, args.sanitized)
        report["compile"][release + "_sha256"] = digest(out / (release + "-image"))
        save()
    for case in CASES:
        result = out / "-".join(case)
        result.mkdir()
        row = {"request": case[0], "echo": case[1], "scenario": case[2], "passed": False}
        report["cases"].append(row)
        save()
        row["argv"] = command([
            "/usr/bin/bwrap", "--die-with-parent", "--unshare-net", "--unshare-ipc", "--unshare-pid",
            "--ro-bind", "/", "/", "--bind", result, result, "--proc", "/proc", "--dev", "/dev",
            "--ro-bind", "/sys", "/sys", sys.executable, "-B", Path(__file__).resolve(),
            "--jazzy-prefix", args.jazzy_prefix, "--jazzy-system", args.jazzy_system,
            "--humble-prefix", args.humble_prefix, "--humble-system", args.humble_system,
            "--output", result, "--run-peers", *case,
        ], result / "sandbox.log", timeout=42)
        row["observed"] = check_case(case, result, args)
        row["passed"] = True
        save()
    require(all(digest(Path(path)) == sha for path, sha in report["inputs"].items()), "fixture inputs changed")
    report["complete"] = True
    save()


if __name__ == "__main__":
    main()
