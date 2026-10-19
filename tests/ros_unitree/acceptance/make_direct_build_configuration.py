#!/usr/bin/env python3
"""Prepare and admit a six-Guest Direct build configuration; compile nothing.

This external SHA-bound metadata entry consumes a completed actual Typed14
report, ten current tool/archive observations and four explicit SDK paths. The
selected frozen Direct preparer's prerequisite() independently checks the real
official C++ object and complete GO24 generation. Its build() is never called.
No native product receipt, Worker, DDS namespace or simulator is required here.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import stat
import sys
import time
import types


MIB, GIB = 1024**2, 1024**3
SHARED_SHA256 = "86d87f16cde0ddcdb91d893a2520a73d3851f4ef0cf13aeda6c6409ddf8262fb"
PIN = frozenset(("path", "bytes", "sha256", "device", "inode", "mtime_ns", "ctime_ns"))
CG = frozenset(("memory_max", "memory_swap_max", "expected_boot_id", "expected_leaf",
                "expected_device", "expected_inode", "expected_cpus"))
TOOLS = frozenset(("cc", "cxx", "python", "validator", "libc", "libm", "printscan", "builtins", "libcxx", "libcxxabi"))
STATUS = "PASS_DIRECT_SIX_GUEST_BUILD_CONFIGURATION_AND_REAL_PREREQUISITE_ONLY_COMPILER_NOT_STARTED"


def require(value, reason):
    if not value:
        raise ValueError(reason)


def exact(value, names, reason):
    require(type(value) is dict and set(value) == set(names), reason)


def bounded(path, maximum):
    """Read bounded ordinary bootstrap bytes before loading any helper module."""
    path = Path(path)
    require(path.is_absolute() and path.resolve(strict=True) == path, "canonical physical bootstrap path required")
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(descriptor)
        require(stat.S_ISREG(first.st_mode) and 0 < first.st_size <= maximum, "bounded ordinary bootstrap input required")
        data = bytearray()
        while True:
            block = os.read(descriptor, min(65536, maximum + 1 - len(data)))
            if not block:
                break
            data.extend(block)
            require(len(data) <= maximum, "bootstrap input grew")
        last, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        require(len(data) == first.st_size and all(getattr(first, key) == getattr(last, key) == getattr(named, key)
                for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode")), "bootstrap input changed")
        pin = {"path": str(path), "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest(), "device": first.st_dev,
               "inode": first.st_ino, "mtime_ns": first.st_mtime_ns, "ctime_ns": first.st_ctime_ns}
        return bytes(data), pin
    finally:
        os.close(descriptor)


def decode(raw):
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, "duplicate request member")
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: require(False, "nonfinite request"))


def context(cg):
    """Prove the supplied live hard limit before external/frozen helper loading."""
    exact(cg, CG, "closed current seven-field cgroup selection required")
    require(sys.platform == "linux" and os.geteuid() == 0 and hasattr(os, "pidfd_open") and
            cg["memory_max"] == 64 * GIB and cg["memory_swap_max"] == 0 and cg["expected_cpus"] == "0-31" and
            type(cg["expected_device"]) is int and type(cg["expected_inode"]) is int and
            re.fullmatch("[0-9a-f-]{36}", cg["expected_boot_id"]), "actual Root hard64GiB/swap0/cpus0-31 required")
    leaf = Path(cg["expected_leaf"])
    require(leaf.is_absolute() and leaf.resolve(strict=True) == leaf and str(leaf).startswith("/sys/fs/cgroup/") and
            leaf.stat().st_dev == cg["expected_device"] and leaf.stat().st_ino == cg["expected_inode"] and
            Path("/proc/sys/kernel/random/boot_id").read_text().strip() == cg["expected_boot_id"] and
            Path("/proc/self/cgroup").read_text().splitlines() == ["0::" + str(leaf).removeprefix("/sys/fs/cgroup")] and
            (leaf / "memory.max").read_text().strip() == str(64 * GIB) and
            (leaf / "memory.swap.max").read_text().strip() == "0" and
            (leaf / "cpuset.cpus.effective").read_text().strip() == "0-31", "actual current cgroup incarnation differs")


def load(name, path, raw):
    module = types.ModuleType(name)
    module.__file__ = str(path)
    sys.modules[name] = module
    exec(compile(raw, str(path), "exec"), module.__dict__)
    return module


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument("--request", type=Path, required=True)
    parser.add_argument("--request-sha256", required=True)
    parser.add_argument("--maker-sha256", required=True)
    parser.add_argument("--composition",choices=("standalone","modular"),default="standalone",
                        help="explicit pure perception/motion/callback preload build; default remains standalone")
    args = parser.parse_args()
    sys.dont_write_bytecode = True
    frame = None
    try:
        raw, request_pin = bounded(args.request, 4 * MIB)
        require(request_pin["sha256"] == args.request_sha256 and re.fullmatch("[0-9a-f]{64}", args.maker_sha256),
                "explicit actual request/maker SHA256 required")
        request = decode(raw)
        exact(request, ("format_version", "kind", "source", "cgroup", "shared_helper", "typed_build", "tools", "sdk_paths",
                        "restart_mappings", "paths", "limits"), "closed Direct configuration request required")
        require(type(request["format_version"]) is int and request["format_version"] == 1 and
                request["kind"] == "uwvm-unitree-direct-build-configuration-request-v1", "Direct request version differs")
        context(request["cgroup"])
        maker_raw, maker_pin = bounded(Path(__file__).absolute(), MIB)
        require(maker_pin["sha256"] == args.maker_sha256, "actual external Direct maker bytes differ")
        shared_raw, shared_pin = bounded(Path(request["shared_helper"]["path"]), MIB)
        require(shared_pin == request["shared_helper"] and shared_pin["sha256"] == SHARED_SHA256,
                "exact reviewed shared metadata utility source required")
        shared = load("direct_configuration_shared_metadata", Path(shared_pin["path"]), shared_raw)
        exact(request["source"], ("root", "manifest"), "closed actual complete source selection required")
        source = shared.physical(request["source"]["root"])
        manifest_pin = shared.full_pin(request["source"]["manifest"])
        frozen = shared.document(manifest_pin, 16 * MIB)
        require(type(frozen) is dict and frozen, "actual complete source manifest required")
        bootstrap = [request_pin, maker_pin, shared_pin, manifest_pin]
        bodies = {}
        for relative in ("tools/ros_distro/common.py", "tests/ros_cgroup/probe.py", "tests/ros_javascript_guest/run.py"):
            body, pin = shared.opened(source / relative, MIB, body=True)
            require(frozen.get(relative) == {key: pin[key] for key in ("bytes", "sha256")},
                    "actual first helper differs from complete frozen source")
            bodies[relative] = body
            bootstrap.append(pin)
        common = load("direct_configuration_common", source / "tools/ros_distro/common.py", bodies["tools/ros_distro/common.py"])
        probe = load("direct_configuration_probe", source / "tests/ros_cgroup/probe.py", bodies["tests/ros_cgroup/probe.py"])
        common._CGROUP_PROBE = probe
        common.install_interrupts()
        before = common.cgroup_state(request["cgroup"])
        require(all(row["events"].get(key, 0) == 0 for row in before["ancestors"]
                    for key in ("oom", "oom_kill", "oom_group_kill")), "ancestor OOM counters must start at zero")
        started, next_probe, latest, disk_parents = time.monotonic(), 0.0, before, []
        def guard():
            nonlocal next_probe, latest
            require(time.monotonic() - started <= 7200, "Direct metadata maker exceeded two-hour bound")
            if time.monotonic() < next_probe:
                return latest
            require(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= 1536 * MIB, "Direct metadata RSS exceeded1536MiB")
            current = common.cgroup_state(request["cgroup"])
            common.cgroup_preserved(before, current)
            require(current["memory_current"] <= 60 * GIB, "four-GiB current cgroup headroom required")
            for parent in disk_parents:
                space = os.statvfs(parent)
                require(space.f_bavail * space.f_frsize >= 8 * GIB, "eight-GiB backing reserve exhausted")
            latest, next_probe = current, time.monotonic() + 0.5
            return current
        api = load("direct_configuration_execution", source / "tests/ros_javascript_guest/run.py", bodies["tests/ros_javascript_guest/run.py"])
        unitree = api.frozen_module(source, frozen, "tests/ros_unitree/common.py", "direct_configuration_unitree")
        preparer = api.frozen_module(source, frozen, "tests/ros_unitree/prepare_direct_guests.py", "direct_configuration_actual_prerequisite")
        admission = api.frozen_module(source, frozen, "tests/ros_unitree/typed_transport_inputs.py", "direct_configuration_evidence")
        initial_source = common.source_state(source, Path(manifest_pin["path"]))
        typed_pin = shared.full_pin(request["typed_build"])
        packet = shared.document(typed_pin, 64 * MIB, guard)
        admission.clean_build(packet, admission.BUILD_STATUS, ("source_preserved", "input_pins_preserved", "whole_roots_preserved"))
        require(packet.get("kind") == "uwvm-unitree-command14-continuation-builds-v1" and
                packet.get("retained_command_products") == 9 and packet.get("fresh_command_products") == 5,
                "actual completed Typed14 prerequisite required")
        exact(request["tools"], TOOLS, "ten current tool/archive observations required")
        exact(request["sdk_paths"], ("sysroot", "resource_dir", "wasm_ld_alias", "quickjs"), "four explicit actual SDK paths required")
        tool_pins = {key: shared.full_pin(pin) for key, pin in request["tools"].items()}
        for pin in tool_pins.values():
            require(shared.opened(pin["path"], 2 * GIB, guard) == pin, "current tool/archive observation changed")
        require(Path(sys.executable).resolve(strict=True) == Path(tool_pins["python"]["path"]), "select actual current executing Python")
        sdk = {key: Path(value) for key, value in request["sdk_paths"].items()}
        for key in ("sysroot", "resource_dir", "quickjs"):
            shared.physical(str(sdk[key]))
            require(sdk[key].is_dir(), "complete actual SDK directory required")
        alias = sdk["wasm_ld_alias"]
        require(alias.is_absolute() and str(alias) == request["sdk_paths"]["wasm_ld_alias"] and
                alias.parent.resolve(strict=True) == alias.parent and alias.name.startswith("wasm-ld") and alias.is_file(),
                "actual explicit multicall Wasm-linker spelling required")
        alias_before = (os.readlink(alias) if alias.is_symlink() else None, alias.lstat())
        alias_target = shared.opened(alias.resolve(strict=True), 128 * MIB, guard)
        discovery = shared.Discovery({"restart_mappings": request["restart_mappings"]}, guard)
        discovery.pin(typed_pin)
        discovery.inventories(packet, "completed actual Typed14")
        discovery.source(packet)
        discovery.root(str(Path(typed_pin["path"]).parent), "actual completed Typed14 output")
        for pin in tool_pins.values():
            discovery.pin(pin)
        discovery.pin(alias_target)
        official = packet["official_cpp_object"]["qualification"]
        discovery.collect(official)
        discovery.collect(discovery.doc(official["report"]))
        rows = [row for row in packet["products"] if row["family"] == "unitree_go"]
        require(rows and all(row["complete_family"] == rows[0]["complete_family"] for row in rows), "one complete real GO family required")
        family = rows[0]["complete_family"]
        discovery.collect(family)
        generated = Path(family["generation"]["path"]).parent
        # Definitions/outputs are reparsed by the real frozen prerequisite;
        # they must fit already observed complete roots, never guessed parents.
        for path in [*sdk.values(), generated]:
            require(any(path.resolve(strict=True).is_relative_to(Path(root)) for root in discovery.candidates),
                    "SDK/GO path lacks a recorded complete producer root")
        exact(request["paths"], ("controls", "admission_out", "build_out"), "three fresh output paths required")
        controls, admission_out, build_out = [shared.physical(request["paths"][key], existing=False)
                                              for key in ("controls", "admission_out", "build_out")]
        for i, path in enumerate((controls, admission_out, build_out)):
            require(not path.exists() and not path.is_symlink() and path.parent.is_dir() and not path.is_relative_to(source) and
                    not source.is_relative_to(path) and all(path != other and not path.is_relative_to(other) and not other.is_relative_to(path)
                    for other in (controls, admission_out, build_out)[i + 1:]), "fresh disjoint physical output paths required")
            disk_parents.append(path.parent)
        next_probe = 0.0
        guard()
        exact(request["limits"], ("jobs", "command_seconds", "rss_bytes", "log_bytes", "output_bytes"), "closed bounded serial limits required")
        limits = request["limits"]
        require(all(type(value) is int and value > 0 for value in limits.values()) and limits["jobs"] == 1 and
                limits["command_seconds"] <= 600 and limits["rss_bytes"] <= 2 * GIB and limits["log_bytes"] <= 16 * MIB and
                limits["output_bytes"] <= 4 * GIB, "Direct build limits exceed serial600second/2GiB bounds")
        controls.mkdir(mode=0o700)
        # Whole SDK/QuickJS/generated directories must remain selected even
        # when the prerequisite's sparse current pin ledger touches no file yet.
        mandatory = set()
        for path in [sdk["sysroot"], sdk["resource_dir"], sdk["quickjs"], alias.resolve(strict=True), generated]:
            matches = sorted((root for root in discovery.candidates if path.is_relative_to(Path(root))),
                             key=lambda root: (len(Path(root).parts), root))
            require(matches, "selected compile path lacks complete current root coverage")
            mandatory.add(matches[0])
        roots = shared.select_roots(discovery, mandatory, source, controls, (admission_out, build_out))
        mappings = [row for row in request["restart_mappings"] if any(Path(root).is_relative_to(Path(row["root"])) for root in roots)]
        selection = {"format_version": 1, "kind": ("unitree-direct-go2-modular-build-selection-v1" if args.composition=="modular" else
                              "unitree-direct-go2-standalone-build-selection-v1"),
                     "tools": {key: "direct-" + key for key in sorted(TOOLS)}, "paths": request["sdk_paths"],
                     "typed_build": "typed-command-build", "generated_go": str(generated), "restart_mappings": mappings}
        selected = shared.write(controls / "direct-selection.json", selection)
        inputs = {"direct-" + key: shared.thin(pin) for key, pin in tool_pins.items()}
        inputs.update({"typed-command-build": shared.thin(typed_pin), "direct-build-selection": shared.thin(selected)})
        config = {"format_version": 1, "cgroup": request["cgroup"], "input_roots": roots, "inputs": inputs,
                  "environment": {}, "limits": limits, "upstreams": {}, "simulation": None, "benchmark": None, "profiler": None}
        configuration = shared.write(controls / "frame-config.json", config)
        invocation = [tool_pins["python"]["path"], "-B", str(source / "tests/ros_unitree/prepare_direct_guests.py"),
                      "--source", str(source), "--manifest", manifest_pin["path"], "--manifest-sha256", manifest_pin["sha256"],
                      "--config", configuration["path"], "--config-sha256", configuration["sha256"],
                      "--selection-input", "direct-build-selection", "--out", str(build_out)]
        invocation_pin = shared.write(controls / "build-invocation.json", {"argv": invocation, "execution_performed": False})
        frame_args = types.SimpleNamespace(source=source, manifest=Path(manifest_pin["path"]), manifest_sha256=manifest_pin["sha256"],
            config=Path(configuration["path"]), config_sha256=configuration["sha256"], out=admission_out)
        frame = unitree.Frame(frame_args, "unitree-direct-build-configuration-prerequisite-only")
        frame.frozen = frozen
        obj, actual_generated, evidence, official_proof = preparer.prerequisite(frame, admission, api, selection)
        require(actual_generated == generated and official_proof["compiler"]["physical"] == tool_pins["cxx"]["path"] and
                official_proof["sysroot"] == str(sdk["sysroot"]) and official_proof["Guest_compile_flags"] == ["-UNDEBUG", preparer.TRAP],
                "actual retained C++ qualification compiler/sysroot/flags differ")
        for key, name in (("libc", "libc.a"), ("libm", "libm.a"), ("printscan", "libc-printscan-long-double.a")):
            require(Path(tool_pins[key]["path"]) == (sdk["sysroot"] / "lib/wasm32-wasip1" / name).resolve(strict=True),
                    "actual archive must match the selected WASI sysroot")
        cpp_archives = preparer.selected_cpp_archives(frame, api, sdk["sysroot"],
            {key: selection["tools"][key] for key in ("libcxx", "libcxxabi")})
        require(not frame.actors and not build_out.exists(), "metadata maker must not compile or start a child")
        frame.report.update(metadata_maker=maker_pin, metadata_shared_helper=shared_pin, metadata_request=request_pin,
            selected_tools=tool_pins, derived_selection=selected, derived_configuration=configuration, future_invocation=invocation_pin,
            original_compiler_source=packet["source"], original_compiler_manifest_sha256=packet["source_manifest_sha256"],
            observed_prerequisite_object=obj, planning_device_bridges=discovery.bridges, compiler_started=False,
            observed_selected_cpp_archives=cpp_archives,
            actors_started=0, Runtime_executed=False, ROS_DDS_simulator_executed=False, Guest_rights_changed=False)
        require(common.source_state(source, Path(manifest_pin["path"])) == initial_source, "complete selected frozen source changed")
        for pin in [*bootstrap, *tool_pins.values(), typed_pin, selected, configuration, invocation_pin, *discovery.pins.values()]:
            require(shared.opened(pin["path"], max(64 * MIB, pin["bytes"]), guard) == pin, "selected metadata/tool/planning input changed")
        current_alias = alias.lstat()
        require((os.readlink(alias) if alias.is_symlink() else None) == alias_before[0] and
                all(getattr(current_alias, key) == getattr(alias_before[1], key) for key in
                    ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode")) and
                shared.opened(alias.resolve(strict=True), 128 * MIB, guard) == alias_target, "explicit linker spelling/target changed")
        frame.report["selected_composition"]=args.composition
        return frame.finish("PASS_DIRECT_MODULAR_BUILD_CONFIGURATION_AND_REAL_PREREQUISITE_ONLY_COMPILER_NOT_STARTED"
                            if args.composition=="modular" else STATUS)
    except BaseException as error:
        if frame is not None:
            return frame.finish("FAIL", repr(error))
        print(repr(error), file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
