#!/usr/bin/env python3
"""Matched actual native math versus modular Wasm call/callback measurements.

The optional session protocol measures a warmed live instance through basic
stdin/stdout gates, with no Guest clock grant. The one-shot protocol separately
measures process/load/instantiate/batch/exit. Neither result is ROS/DDS latency
or physical robot performance. Each repetition checks the exact shared-math
checksum and retains actual process provenance and before/after input pins.
"""
import hashlib
import json
import math
from pathlib import Path
import platform
import resource
import struct
import time

from common import Actors, Frame, arguments, exact, require


def image(frame, name):
    """Check actual ELF bytes against the physical machine, never a label."""
    path = frame.input(name)
    with path.open("rb") as stream:
        header = stream.read(64)
    require(len(header) == 64 and header[:4] == b"\x7fELF" and header[4:6] == b"\x02\x01",
            "native benchmark/Runtime must be little-endian ELF64")
    machine = struct.unpack_from("<H", header, 18)[0]
    expected = {"x86_64": 62, "aarch64": 183}.get(platform.machine())
    require(expected is not None and machine == expected, "actual native architecture differs from physical host")
    return {"machine": machine, "bits": 64, "endian": "little", "physical_host": platform.machine()}


def receipt(frame, label, output_label, required_source, definitions):
    """Consume a closed actual fixture build receipt, not an availability flag.

    Root's producer retains full compiler/link commands and opened dependency
    pins. This check reopens those physical facts and binds the selected source,
    output, definitions, and current complete freeze to the measured artifacts.
    """
    direct_qualifier = getattr(frame, "direct_build_receipt", None)
    if direct_qualifier is not None:
        value = direct_qualifier(label, output_label, required_source, definitions)
        if value is not None:
            return value
    body, _ = frame.common.document(frame.input(label))
    exact(body, ("format_version", "kind", "status", "source_manifest_sha256", "output", "source_inputs",
                 "definitions", "commands", "full_dependencies_verified"), "fixture build receipt")
    require(body["format_version"] == 1 and body["kind"] == "unitree-fixture-build" and body["status"] == "PASS" and
            body["source_manifest_sha256"] == frame.report["source_manifest_sha256"] and
            body["full_dependencies_verified"] is True and body["definitions"] == definitions,
            "actual matching source/definition/dependency build proof required")
    def bind(pin):
        require(frame.pin(pin["path"]) == pin, "actual compiler input/log/output changed")
    bind(body["output"])
    require(body["output"] == frame.inputs[output_label], "selected output differs from its actual build")
    require(body["commands"] and body["source_inputs"], "complete build commands/source inputs required")
    names = set()
    for pin in body["source_inputs"]:
        bind(pin)
        names.add(str(Path(pin["path"]).relative_to(frame.source)))
    require(set(required_source) <= names, "shared actual math/kernel source absent from build proof")
    for row in body["commands"]:
        exact(row, ("argv", "returncode", "stdout", "stderr", "dependencies"), "actual build command")
        require(row["returncode"] == 0 and row["argv"] and Path(row["argv"][0]).is_absolute() and
                not any(value in ("-ffast-math", "-Ofast", "-DNDEBUG", "--fast-math") for value in row["argv"]),
                "successful checked actual build command required")
        for pin in (row["stdout"], row["stderr"], *row["dependencies"]):
            bind(pin)
    compile_rows = [row for row in body["commands"] if "-MD" in row["argv"] and "-MF" in row["argv"]]
    require(compile_rows and all("-O3" in row["argv"] and "-ffp-contract=off" in row["argv"] and
            all("-D" + name + "=" + str(value) in row["argv"] for name, value in definitions.items())
            for row in compile_rows),
            "matched O3/full-MD/contraction-off actual compile commands required")
    dependency_names = {pin["path"] for row in compile_rows for pin in row["dependencies"]}
    require(all(str(frame.source / name) in dependency_names for name in required_source),
            "required shared source absent from actual complete compiler dependencies")
    with frame.input(output_label).open("rb") as stream:
        wasm = stream.read(8) == b"\0asm\x01\0\0\0"
    if wasm:
        require(any(row["argv"][1:] == ["validate", str(frame.input(output_label))] and row["returncode"] == 0
                    for row in body["commands"]), "actual selected Wasm output lacks retained validator execution")
    return body


def percentiles(values):
    require(values and all(type(value) is int and value > 0 for value in values), "positive actual timing samples required")
    ordered = sorted(values)
    def rank(fraction):
        return ordered[max(0, math.ceil(fraction * len(ordered)) - 1)]
    return {"count": len(values), "p50_ns": rank(.50), "p95_ns": rank(.95), "p99_ns": rank(.99),
            "min_ns": ordered[0], "max_ns": ordered[-1], "mean_ns": sum(values) / len(values)}


def children_usage():
    """Read kernel accounting; valid as a delta only for serial benchmark roles."""
    value = resource.getrusage(resource.RUSAGE_CHILDREN)
    return {"user_cpu_seconds": value.ru_utime, "system_cpu_seconds": value.ru_stime,
            "voluntary_context_switches": value.ru_nvcsw, "involuntary_context_switches": value.ru_nivcsw,
            "maximum_reaped_child_rss_kib_cumulative": value.ru_maxrss}


def usage_delta(before):
    value = children_usage()
    return {key: value[key] - before[key] for key in before if not key.endswith("cumulative")} | {
        "maximum_reaped_child_rss_kib_cumulative": value["maximum_reaped_child_rss_kib_cumulative"],
        "scope": "Kernel-reaped child accounting for the sole serial workload role, including its warmup/startup/exit; maximum RSS is cumulative, not a per-batch delta"}


def main():
    args = arguments(__doc__).parse_args()
    frame = None
    try:
        frame = Frame(args, "unitree-native-wasm-modular-performance")
        spec = frame.config["benchmark"]
        exact(spec, ("native", "wasm", "native_binary_input", "runtime_input", "wasm_binary_input",
                     "native_build_input", "wasm_build_input", "seed", "iterations", "warmup", "samples",
                     "protocol", "wasm_backend", "slowdown_trigger"), "benchmark selection")
        require(type(spec["seed"]) is int and 0 <= spec["seed"] <= 0xFFFFFFFF and
                type(spec["iterations"]) is int and 1 <= spec["iterations"] <= 1000000 and
                type(spec["warmup"]) is int and 1 <= spec["warmup"] <= 20 and
                type(spec["samples"]) is int and 30 <= spec["samples"] <= 1000, "bounded matched seed/count/repetitions required")
        require(spec["protocol"] in ("one-shot-le64", "session-le64") and spec["wasm_backend"] in
                ("llvm-full", "interp", "u8", "u16", "u32"), "explicit actual engine and wire protocol required")
        require(spec["protocol"] != "session-le64" or spec["warmup"] + spec["samples"] + 1 <= 256,
                "session warmup/samples/quit exceed the actual Guest's 256-command bound")
        require(type(spec["slowdown_trigger"]) in (float, int) and 1 <= spec["slowdown_trigger"] <= 10,
                "predeclared finite slowdown trigger required")
        physical = image(frame, spec["native_binary_input"])
        require(image(frame, spec["runtime_input"]) == physical, "native and Runtime physical architecture differ")
        require(spec["native"]["executable"] == spec["native_binary_input"] and
                spec["wasm"]["executable"] == spec["runtime_input"], "direct physical native/Runtime executables required; no QEMU wrapper")
        definitions = {"ROBOT_BENCHMARK_SEED": spec["seed"], "ROBOT_BENCHMARK_ITERATIONS": spec["iterations"],
                       "ROBOT_BENCHMARK_SESSION": int(spec["protocol"] == "session-le64")}
        required = ["sdk/examples/robotics_modular/robotics_math.h"]
        receipt(frame, spec["native_build_input"], spec["native_binary_input"], [*required, "tests/ros_unitree/native_math.c"], definitions)
        receipt(frame, spec["wasm_build_input"], spec["wasm_binary_input"], required, definitions)
        frame.report.update(physical_architecture=physical, wasm_backend=spec["wasm_backend"],
            timing_scope="warmed same-instance batch plus matched pipe/scheduling overhead" if spec["protocol"] == "session-le64"
            else "process/load/instantiate/batch/exit; warm-up creates separate processes, not a warmed instance",
            excludes=["ROS", "DDS", "MuJoCo", "Unitree actuation", "physical target microarchitecture"],
            shared_math="Actual identical portable header; native fused baseline versus Wasm modular/callback dispatch")
        owner = Actors(frame)
        expected = None
        samples = {"native": [], "wasm": []}
        usage = {"native": [], "wasm": []}
        if spec["protocol"] == "session-le64":
            # Keep only one workload instance live at a time. Alternating
            # sample order is implemented by separate matched runs below for
            # one-shot; sessions record the different grouping explicitly.
            frame.report["execution_order"] = "native session followed by Wasm session; no overlapping workload instances"
            for role in ("native", "wasm"):
                accounted = children_usage()
                child, row = owner.start(role + "-session", frame.command(spec[role]), json_output=False)
                require(owner.receive_bytes(child, 6) == b"READY\n", "actual session readiness differs")
                for index in range(spec["warmup"] + spec["samples"]):
                    measured = index >= spec["warmup"]
                    began = time.monotonic_ns()
                    owner.write(child, b"P" if measured else b"W")
                    value = owner.receive_bytes(child, 8)
                    elapsed = time.monotonic_ns() - began
                    require(value != b"\xff" * 8, "shared math returned invalid-bound sentinel")
                    if expected is None:
                        expected = value
                    require(value == expected, "native/Wasm shared math checksum differs")
                    if measured:
                        samples[role].append(elapsed)
                owner.write(child, b"Q")
                child.stdin.close()
                owner.finish(child)
                require(Path(row["stdout"]).stat().st_size == 6 + 8 * (spec["warmup"] + spec["samples"]),
                        "session emitted unexpected or duplicate checksum bytes")
                usage[role].append(usage_delta(accounted))
        else:
            frame.report["execution_order"] = "alternating native/Wasm, then Wasm/native for adjacent repetitions"
            for index in range(spec["warmup"] + spec["samples"]):
                for role in (("native", "wasm") if index % 2 == 0 else ("wasm", "native")):
                    accounted = children_usage()
                    began = time.monotonic_ns()
                    child, row = owner.start("%s-%04d" % (role, index), frame.command(spec[role]), data=b"", json_output=False)
                    owner.finish(child)
                    elapsed = time.monotonic_ns() - began
                    value = Path(row["stdout"]).read_bytes()
                    require(len(value) == 8 and value != b"\xff" * 8, "one exact actual LE64 checksum required")
                    if expected is None:
                        expected = value
                    require(value == expected, "native/Wasm shared math checksum differs")
                    usage[role].append({"warmup": index < spec["warmup"], **usage_delta(accounted)})
                    if index >= spec["warmup"]:
                        samples[role].append(elapsed)
        frame.report["raw_timing_ns"] = samples
        frame.report["actual_serial_child_accounting"] = usage
        frame.report["statistics"] = {role: percentiles(values) for role, values in samples.items()}
        ratio = frame.report["statistics"]["wasm"]["p50_ns"] / frame.report["statistics"]["native"]["p50_ns"]
        frame.report.update(checksum_le64=expected.hex(), slowdown_p50=ratio,
            profiling_required=ratio > spec["slowdown_trigger"], slowdown_trigger=spec["slowdown_trigger"])
        # A correct but slow result is visible as such. Passing this comparison
        # does not promise native speed or silently erase an optimization task.
        return frame.finish("PASS_ACTUAL_MATCHED_MODULAR_MEASUREMENTS")
    except BaseException as error:
        if frame:
            return frame.finish("FAIL", repr(error))
        raise


if __name__ == "__main__":
    raise SystemExit(main())
