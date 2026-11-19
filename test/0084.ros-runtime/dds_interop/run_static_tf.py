"""Qualify late-join and live ROS /tf_static through native C++ and stock Python TF2."""

import argparse
from contextlib import ExitStack
import json
import os
from pathlib import Path
import resource
import secrets
import statistics
import subprocess
import sys
import time

from run_image import COMPILER, HERE, RMW, command, compile_peer, digest, environment, gate, require

CASES = (("jazzy", "transient"), ("humble", "transient"),
         ("jazzy", "volatile"), ("humble", "volatile"),
         ("humble", "both-volatile"), ("humble", "writer-gone"))
LIVE_COUNT = 24


def wait_for(process, path, witness, timeout):
    """Wait for a bounded, flushed process observation with early failure reporting."""
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        if any(line.startswith(witness) for line in path.read_text().splitlines()):
            return
        require(process.poll() is None, f"{path.name} stopped before {witness}")
        time.sleep(0.002)
    raise RuntimeError(f"{path.name} timed out waiting for {witness}")


def run_peers(args, out):
    """Coordinate phases without injecting artificial ROS frames or using a ROS daemon."""
    interfaces = [line.split(":", 1)[0].strip()
                  for line in Path("/proc/net/dev").read_text().splitlines()[2:]]
    require(interfaces == ["lo"], "DDS peers require private loopback")
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    resource.setrlimit(resource.RLIMIT_FSIZE, (8 << 20, 8 << 20))
    release, mode = args.run_peers
    require((release, mode) in CASES, "unexpected peer scenario")
    token = 1 + secrets.randbelow(4095)
    writer_env = environment(getattr(args, release + "_prefix"), getattr(args, release + "_system"), release, out)
    reader_env = environment(args.jazzy_prefix, args.jazzy_system, "jazzy", out)
    py_site = args.jazzy_prefix / "lib/python3.12/site-packages"
    reader_env.update(PYTHONHOME=str(args.python_home), PYTHONNOUSERSITE="1",
                      LD_LIBRARY_PATH=os.pathsep.join([str(out.parent / "python-libs"), reader_env["LD_LIBRARY_PATH"]]),
                      PYTHONPATH=os.pathsep.join(map(str, [py_site, *args.python_site])))
    writer_log, reader_log = out / "writer.stdout", out / "reader.stdout"
    with ExitStack() as processes:
        durability = "volatile" if mode == "both-volatile" else "transient"
        writer = processes.enter_context(subprocess.Popen(
            [out.parent / (release + "-static_tf"), RMW[release], str(token), durability], env=writer_env,
            stdin=subprocess.PIPE, stdout=processes.enter_context(writer_log.open("wb")),
            stderr=processes.enter_context((out / "writer.stderr").open("wb"))))
        processes.callback(stop, writer)
        wait_for(writer, writer_log, "PUBLISHED 0 ", 8)
        if mode == "writer-gone":
            finish(writer)
        reader = processes.enter_context(subprocess.Popen(
            [args.python_home / "bin/python3.12", "-B", HERE / "ros_static_tf.py", RMW["jazzy"], mode, str(token)],
            env=reader_env, stdin=subprocess.DEVNULL,
            stdout=processes.enter_context(reader_log.open("wb")),
            stderr=processes.enter_context((out / "reader.stderr").open("wb"))))
        processes.callback(stop, reader)
        wait_for(reader, reader_log, "HISTORY ", 12)
        if mode != "writer-gone":
            for sequence in range(1, LIVE_COUNT + 1):
                writer.stdin.write(b"p\n")
                writer.stdin.flush()
                wait_for(reader, reader_log, f"RECEIVED {sequence} ", 3)
            require(writer.wait(timeout=5) == 0, "native publisher failed")
        require(reader.wait(timeout=5) == 0, "Python listener failed")
    (out / "processes.json").write_text(json.dumps({"writer": writer.returncode, "reader": reader.returncode,
                                                   "token": token}) + "\n")


def stop(process):
    """Reap a partially started fixture when an observation fails."""
    if process.poll() is None:
        process.kill()
        process.wait(timeout=2)


def finish(writer):
    writer.stdin.write(b"x\n")
    writer.stdin.flush()
    require(writer.wait(timeout=5) == 0, "publisher did not exit cleanly")


def files(lines, prefix, required, vendor):
    """Record only the required libraries and require their selected installation."""
    mapped = {}
    for line in lines:
        if not line.startswith("LIB "):
            continue
        path = Path(line.removeprefix("LIB "))
        name = path.name.split(".so", 1)[0]
        if name not in required | vendor:
            continue
        resolved = path.resolve(strict=True)
        require(resolved.is_relative_to(prefix), f"library loaded outside expected ROS prefix: {path}")
        require(name not in mapped, f"unexpected second library mapping: {name}")
        mapped[name] = {"path": str(resolved), "sha256": digest(resolved)}
    require(required <= mapped.keys() and len(vendor & mapped.keys()) == 1, "missing RMW/type-support mapping")
    return mapped


def observations(lines, label):
    """Parse every observation and reject duplicate sequence or invalid timing."""
    observed = {}
    for line in lines:
        fields = line.split()
        if fields and fields[0] == label:
            seq, ns = map(int, fields[1:])
            require(seq not in observed and ns > 0, f"duplicate or invalid {label}")
            observed[seq] = ns
    return observed


def check_case(args, out, release, mode):
    """Reconcile independent endpoint logs and retain exact timings and executable identities."""
    writer, reader = ((out / (role + ".stdout")).read_text().splitlines() for role in ("writer", "reader"))
    results = json.loads((out / "processes.json").read_bytes())
    require(results["writer"] == results["reader"] == 0, "fixture endpoint did not complete")
    require(writer.count("SUBSCRIBERS_BEFORE_HISTORY 0") == writer.count("PUBLISHER_EXIT") == 1,
            "missing publication or exit witness")
    nlive = 0 if mode == "writer-gone" else LIVE_COUNT
    history, = [int(line.split()[1]) for line in reader if line.startswith("HISTORY ")]
    require(history in (0, 1) and (mode == "volatile" or history == int(mode == "transient")),
            "unexpected retention")
    results["retained_samples"] = history
    results["documented_retained_samples"] = int(mode == "transient")
    results["documented_history_matches"] = history == results["documented_retained_samples"]
    results["writer_durability"] = "volatile" if mode == "both-volatile" else "transient"
    results["reader_durability"] = "volatile" if mode in ("volatile", "both-volatile") else "transient"
    require(writer.count("WRITER_QOS " + results["writer_durability"]) == 1 and
            reader.count("READER_QOS " + results["reader_durability"]) == 1, "actual QoS was not confirmed")
    require(reader.count("MATCHED") == int(nlive > 0), "unexpected publisher match count")
    require(reader.count(f"HISTORY {history}") == reader.count(f"COMPLETE {history + nlive}") == 1,
            "missing listener history or completion witness")
    start, = [int(line.split()[1]) for line in reader if line.startswith("LISTEN_STARTED ")]
    published = observations(writer, "PUBLISHED")
    received = observations(reader, "RECEIVED")
    require(list(published) == list(range(nlive + 1)), "unexpected publication")
    require(list(received) == list(range(1 - history, nlive + 1)), "unexpected TF delivery")
    require(published[0] < start, "historical publish must precede reader construction")
    if history:
        require(start <= received[0] < published[1], "retained sample arrived outside the history phase")
        results["late_join_ns"] = received[0] - start
    raw = [received[index] - published[index] for index in range(1, nlive + 1)]
    require(all(ns > 0 for ns in raw), "publish and receipt clock order is invalid")
    results["live_publish_to_validated_lookup_ns"] = {"raw": raw}
    if raw and not args.sanitized:
        tail = sorted(raw[4:])
        results["live_publish_to_validated_lookup_ns"].update(
            warmup=4, p50=statistics.median(tail), p95=tail[(95 * len(tail) + 99) // 100 - 1])
    stem = "libtf2_msgs__rosidl_typesupport_"
    writer_required = {"librclcpp", "lib" + RMW[release], stem + "cpp",
                       stem + ("fastrtps_cpp" if release == "jazzy" else "introspection_cpp")}
    results["writer_libraries"] = files(writer, getattr(args, release + "_prefix"), writer_required,
                                       {"libfastdds", "libfastrtps"} if release == "jazzy" else {"libddsc"})
    # The installed rclpy and tf2_py extensions must have been mapped by the same process.
    reader_required = {"lib" + RMW["jazzy"], stem + "c", stem + "fastrtps_c",
                       "_rclpy_pybind11.cpython-312-x86_64-linux-gnu", "_tf2_py"}
    results["reader_libraries"] = files(reader, args.jazzy_prefix, reader_required, {"libfastdds", "libfastrtps"})
    modules = [Path(line.removeprefix("MODULE ")).resolve(strict=True)
               for line in reader if line.startswith("MODULE ")]
    require(len(set(modules)) == 5 and all(path.is_relative_to(args.jazzy_prefix) for path in modules),
            "unexpected Python ROS module identity")
    results["reader_modules"] = {str(path): digest(path) for path in modules}
    logs = ("writer.stdout", "writer.stderr", "reader.stdout", "reader.stderr", "processes.json", "sandbox.log")
    results["output_sha256"] = {log: digest(out / log) for log in logs}
    results["diagnostics"] = {}
    for role in ("writer", "reader"):
        diagnostics = (out / (role + ".stderr")).read_text(errors="replace")
        require("XMLPARSER" not in diagnostics and "config:" not in diagnostics, f"{role} rejected the profile")
        results["diagnostics"][role] = {"bytes": len(diagnostics.encode()),
                                        "rcutils_error_state_messages": diagnostics.count("rcutils_set_error_state()")}
    return results


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for flag in ("jazzy-prefix", "jazzy-system", "humble-prefix", "humble-system", "python-home", "output"):
        parser.add_argument("--" + flag, type=lambda path: Path(path).resolve(), required=True)
    parser.add_argument("--python-site", type=lambda path: Path(path).resolve(strict=True), action="append", default=[])
    parser.add_argument("--sanitized", action="store_true")
    parser.add_argument("--run-peers", nargs=2, help=argparse.SUPPRESS)
    args = parser.parse_args()
    member = gate()
    out = args.output
    require(".tmp" in out.parts, "output must be below a project .tmp")
    if args.run_peers:
        run_peers(args, out)
        return
    out.mkdir()
    # Supply the interpreter's matching shared SONAME without replacing host libc.
    python_soname = "libpython3.12.so.1.0"
    python_lib = (args.python_home / "lib/x86_64-linux-gnu" / python_soname).resolve(strict=True)
    python_libs = out / "python-libs"
    python_libs.mkdir()
    (python_libs / python_soname).symlink_to(python_lib)
    inputs = [Path(__file__).resolve(), COMPILER, args.python_home / "bin/python3.12", python_lib,
              *(HERE / filename for filename in ("ros_static_tf.cpp", "ros_static_tf.py", "run.py", "run_image.py",
                                                 "fastdds-image-loopback.xml", "cyclone-image-loopback.xml"))]
    inputs += [getattr(args, release + "_prefix") / "share/tf2_msgs/msg/TFMessage.idl" for release in RMW]
    report = {"complete": False, "format": "uwvm-native-static-tf-v1",
              "scope": "Jazzy Python TF2 with native Jazzy or Humble C++ over private plaintext DDS UDPv4",
              "cgroup": member, "cpu_set": sorted(os.sched_getaffinity(0)), "sanitized_writer": args.sanitized,
              "inputs": {str(path): digest(path) for path in inputs}, "compile": {}, "cases": []}

    def save():
        (out / "results.json").write_text(json.dumps(report, indent=2) + "\n")

    save()
    for release in RMW:
        report["compile"][release] = compile_peer(getattr(args, release + "_prefix"), getattr(args, release + "_system"),
                                                  release, out, args.sanitized, witness="static_tf",
                                                  message_package="tf2_msgs")
        report["compile"][release + "_sha256"] = digest(out / (release + "-static_tf"))
        save()
    for release, mode in CASES:
        target = out / (release + "-" + mode)
        target.mkdir()
        case = {"writer": release + "-rclcpp", "reader": "jazzy-rclpy-tf2", "scenario": mode, "passed": False}
        report["cases"].append(case)
        save()
        chosen = ["jazzy-prefix", "jazzy-system", "humble-prefix", "humble-system", "python-home"]
        options = [entry for flag in chosen for entry in ("--" + flag, getattr(args, flag.replace("-", "_")))]
        sites = [entry for site in args.python_site for entry in ("--python-site", site)]
        case["argv"] = command([
            "/usr/bin/bwrap", "--die-with-parent", "--unshare-net", "--unshare-ipc", "--unshare-pid",
            "--ro-bind", "/", "/", "--bind", target, target, "--proc", "/proc", "--dev", "/dev",
            "--ro-bind", "/sys", "/sys", sys.executable, "-B", Path(__file__).resolve(),
            *options, *sites, "--output", target, "--run-peers", release, mode,
        ], target / "sandbox.log", timeout=35)
        case["observed"] = check_case(args, target, release, mode)
        case["passed"] = True
        save()
    require(all(digest(Path(path)) == sha for path, sha in report["inputs"].items()), "fixture inputs changed")
    report["all_documented_history_matches"] = all(case["observed"]["documented_history_matches"]
                                                   for case in report["cases"])
    report["complete"] = True
    save()


if __name__ == "__main__":
    main()
