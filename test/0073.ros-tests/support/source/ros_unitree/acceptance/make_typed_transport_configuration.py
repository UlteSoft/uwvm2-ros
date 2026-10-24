#!/usr/bin/env python3
"""Derive and admit a real Typed28 configuration without starting any actor.

Root supplies one small SHA256-bound request containing current full-seven
observations, an actual complete source manifest and an already isolated live
network namespace. Products and family identities are obtained from original
producer documents; neither future pins nor Runtime argv tails are accepted.
The selected frozen transport admission module performs the final independent
check. A successful metadata result is never a transport or build result.
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
PIN = frozenset(("path", "bytes", "sha256", "device", "inode", "mtime_ns", "ctime_ns"))
CG = frozenset(("memory_max", "memory_swap_max", "expected_boot_id", "expected_leaf",
                "expected_device", "expected_inode", "expected_cpus"))
FAMILIES = {"unitree_go": (24, "ros2", "go-api"),
            "unitree_hg": (11, "dual", "hg"), "unitree_api": (8, "ros2", "go-api")}
MAP_ROLES = ("peer", "worker-ros2", "worker-dual", "gateway-go-api", "gateway-hg",
             "context-go-api", "context-hg")
FORBIDDEN_ROOTS = frozenset(("/", "/tmp", "/var", "/var/tmp", "/usr", "/opt", "/root", "/home"))
STATUS = "PASS_TYPED28_CONFIGURATION_AND_REAL_INPUT_ADMISSION_ONLY_TRANSPORT_NOT_EXECUTED"


def require(value, reason):
    if not value:
        raise ValueError(reason)


def exact(value, names, reason):
    require(type(value) is dict and set(value) == set(names), reason)


def decode(raw):
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, "duplicate JSON member")
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda _: require(False, "nonfinite JSON constant"))


def physical(value, *, existing=True):
    require(type(value) is str and value.startswith("/") and not any(c in value for c in ("\0", "\n", "\r")),
            "clean absolute physical path required")
    path = Path(value)
    require(str(path) == value and ".." not in path.parts and path.resolve(strict=existing) == path,
            "symlink parents, traversal and noncanonical paths are refused")
    return path


def full_pin(value):
    exact(value, PIN, "exact full-seven physical observation required")
    physical(value["path"])
    require(type(value["sha256"]) is str and re.fullmatch("[0-9a-f]{64}", value["sha256"]) and
            all(type(value[key]) is int and value[key] >= 0 for key in PIN - {"path", "sha256"}) and value["inode"] > 0,
            "bounded integral full-seven fields and lowercase SHA256 required")
    return dict(value)


def opened(path, maximum, guard=lambda: None, *, body=False):
    """Stream a size-first ordinary descriptor, retaining actual current identity."""
    path = physical(str(path))
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum, "bounded ordinary input required")
        digest, size, data = hashlib.sha256(), 0, bytearray() if body else None
        while True:
            guard()
            block = os.read(descriptor, min(MIB, maximum + 1 - size))
            if not block:
                break
            size += len(block)
            require(size <= maximum, "opened input grew beyond its finite bound")
            digest.update(block)
            if body:
                data.extend(block)
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        require(size == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode")),
                "opened input identity changed")
        pin = {"path": str(path), "bytes": size, "sha256": digest.hexdigest(), "device": before.st_dev,
               "inode": before.st_ino, "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
        return (bytes(data), pin) if body else pin
    finally:
        os.close(descriptor)


def document(pin, maximum, guard=lambda: None):
    pin = full_pin(pin)
    raw, actual = opened(pin["path"], maximum, guard, body=True)
    require(actual == pin, "supplied current full-seven document observation changed")
    return decode(raw)


def thin(pin):
    return {key: pin[key] for key in ("path", "bytes", "sha256")}


def write(path, value):
    raw = (json.dumps(value, indent=2, sort_keys=True) + "\n").encode()
    with path.open("xb") as stream:
        stream.write(raw)
        stream.flush()
        os.fsync(stream.fileno())
    return opened(path, max(MIB, len(raw)))


def bootstrap(request, manifest_raw, frozen, maker_pin):
    """Admit live current context before importing any selected project helper."""
    context = request["current_context"]
    exact(context, ("cgroup", "network_namespace"), "closed actual context required")
    cg = context["cgroup"]
    exact(cg, CG, "closed seven-field current cgroup selection required")
    require(sys.platform == "linux" and os.geteuid() == 0 and hasattr(os, "pidfd_open") and
            cg["memory_max"] == 64 * GIB and cg["memory_swap_max"] == 0 and cg["expected_cpus"] == "0-31" and
            type(cg["expected_device"]) is int and type(cg["expected_inode"]) is int and
            re.fullmatch("[0-9a-f-]{36}", cg["expected_boot_id"]), "Root actual hard64GiB/swap0/cpus0-31 lane required")
    leaf = physical(cg["expected_leaf"])
    require(str(leaf).startswith("/sys/fs/cgroup/") and leaf.is_dir() and
            leaf.stat().st_dev == cg["expected_device"] and leaf.stat().st_ino == cg["expected_inode"] and
            Path("/proc/sys/kernel/random/boot_id").read_text().strip() == cg["expected_boot_id"] and
            Path("/proc/self/cgroup").read_text().splitlines() == ["0::" + str(leaf).removeprefix("/sys/fs/cgroup")] and
            (leaf / "memory.max").read_text().strip() == str(64 * GIB) and
            (leaf / "memory.swap.max").read_text().strip() == "0" and
            (leaf / "cpuset.cpus.effective").read_text().strip() == "0-31" and
            os.readlink("/proc/self/ns/net") == context["network_namespace"], "actual current boot/leaf/membership/namespace differs")
    source = physical(request["source"]["root"])
    require(source.is_dir() and len(frozen) <= 25000 and
            hashlib.sha256(manifest_raw).hexdigest() == request["source"]["manifest"]["sha256"],
            "actual complete selected source manifest required")
    bodies, initial = {}, [maker_pin, request["source"]["manifest"]]
    for relative in ("tools/ros_distro/common.py", "test/0073.ros-tests/support/source/ros_cgroup/probe.py", "test/0073.ros-tests/support/source/ros_javascript_guest/run.py"):
        raw, pin = opened(source / relative, MIB, body=True)
        require(frozen.get(relative) == {key: pin[key] for key in ("bytes", "sha256")},
                "selected bootstrap helper differs from complete frozen source")
        bodies[relative] = raw
        initial.append(pin)
    common = types.ModuleType("typed28_configuration_frozen_common")
    common.__file__ = str(source / "tools/ros_distro/common.py")
    sys.modules[common.__name__] = common
    exec(compile(bodies["tools/ros_distro/common.py"], common.__file__, "exec"), common.__dict__)
    probe = types.ModuleType("typed28_configuration_frozen_probe")
    probe.__file__ = str(source / "test/0073.ros-tests/support/source/ros_cgroup/probe.py")
    exec(compile(bodies["test/0073.ros-tests/support/source/ros_cgroup/probe.py"], probe.__file__, "exec"), probe.__dict__)
    common._CGROUP_PROBE = probe
    common.install_interrupts()
    before = common.cgroup_state(cg)
    require(all(row["events"].get(key, 0) == 0 for row in before["ancestors"]
                for key in ("oom", "oom_kill", "oom_group_kill")), "ancestor OOM counters must start at zero")
    started = time.monotonic()
    next_probe, latest = 0.0, before
    def guard():
        nonlocal next_probe, latest
        require(time.monotonic() - started <= 7200, "metadata maker exceeded two-hour bound")
        if time.monotonic() < next_probe:
            return latest
        rss = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024
        require(rss <= 1536 * MIB, "metadata maker RSS exceeded1536MiB")
        current = common.cgroup_state(cg)
        common.cgroup_preserved(before, current)
        require(current["memory_current"] <= 60 * GIB, "current cgroup requires four-GiB headroom")
        latest, next_probe = current, time.monotonic() + 0.5
        return current
    api = types.ModuleType("typed28_configuration_frozen_execution")
    api.__file__ = str(source / "test/0073.ros-tests/support/source/ros_javascript_guest/run.py")
    sys.modules[api.__name__] = api
    exec(compile(bodies["test/0073.ros-tests/support/source/ros_javascript_guest/run.py"], api.__file__, "exec"), api.__dict__)
    unitree = api.frozen_module(source, frozen, "test/0073.ros-tests/support/source/ros_unitree/common.py", "typed28_configuration_unitree")
    require(unitree.current_loopback(context["network_namespace"])["network_namespace"] == context["network_namespace"],
            "actual isolated enabled loopback namespace required")
    return source, common, api, unitree, guard, initial


class Discovery:
    """Open only producer-selected files, retaining separate old/current facts.

    This source-planning accessor supplies no execution-path exception. Every
    resulting current file must later fit a complete derived Frame inventory,
    and the real maintained admission accessor independently checks it again.
    """
    def __init__(self, request, guard):
        self.guard, self.maps, self.pins, self.bridges, self.candidates = guard, [], {}, [], {}
        self.observations = {}
        seen = set()
        require(type(request["restart_mappings"]) is list and len(request["restart_mappings"]) <= 64,
                "finite explicit historical device mappings required")
        for row in request["restart_mappings"]:
            exact(row, ("root", "historical_device", "current_device"), "closed mapping row required")
            root = physical(row["root"])
            key = (row["root"], row["historical_device"])
            require(root.is_dir() and type(row["historical_device"]) is int and type(row["current_device"]) is int and
                    row["historical_device"] != row["current_device"] and root.stat().st_dev == row["current_device"] and
                    key not in seen, "duplicate/nonphysical mapping or same-device bridge refused")
            seen.add(key)
            self.maps.append(dict(row))

    def pin(self, previous):
        require(type(previous) is dict and PIN <= previous.keys(), "producer full-seven observation required")
        old = full_pin({key: previous[key] for key in PIN})
        require(old["bytes"] <= 2 * GIB, "selected producer file exceeds two-GiB bound")
        key = tuple(old[name] for name in sorted(PIN))
        if key in self.observations:
            # Repeated full-MD records share many physical headers. The first
            # observation was fully streamed; only an exactly unchanged actual
            # identity may reuse it. All distinct selected files are streamed
            # again before the metadata result is accepted.
            self.guard()
            current = self.observations[key]
            named = Path(old["path"]).stat(follow_symlinks=False)
            require(stat.S_ISREG(named.st_mode) and all(getattr(named, field) == current[name] for field, name in
                    (("st_dev", "device"), ("st_ino", "inode"), ("st_size", "bytes"),
                     ("st_mtime_ns", "mtime_ns"), ("st_ctime_ns", "ctime_ns"))), "memoized current producer identity changed")
            return current
        actual = opened(old["path"], max(64 * MIB, old["bytes"]), self.guard)
        if actual != old:
            candidates = sorted((row for row in self.maps if Path(old["path"]).is_relative_to(Path(row["root"])) and
                                 old["device"] == row["historical_device"] and actual["device"] == row["current_device"]),
                                key=lambda row: len(Path(row["root"]).parts), reverse=True)
            require(candidates and all(old[key] == actual[key] for key in PIN - {"device"}),
                    "producer file differs beyond an explicit device-only mapping")
            self.bridges.append({"historical_pin": old, "current_pin": actual, "mapping": candidates[0],
                                 "execution_qualification": "NOT_PERFORMED"})
        prior = self.pins.setdefault(actual["path"], actual)
        require(prior == actual and len(self.pins) <= 200000, "conflicting/current selected-pin closure exceeded")
        self.observations[key] = actual
        return actual

    def doc(self, previous, maximum=64 * MIB):
        actual = self.pin(previous)
        raw, again = opened(actual["path"], maximum, self.guard, body=True)
        require(again == actual, "selected producer document changed while decoded")
        return decode(raw)

    def collect(self, value):
        """Collect actual full pins only from the consumer-selected section."""
        if type(value) is dict:
            if PIN <= value.keys():
                self.pin(value)
            if "full_MD_receipt" in value:
                self.collect(self.doc(value["full_MD_receipt"], 32 * MIB))
            for item in value.values():
                self.collect(item)
        elif type(value) is list:
            for item in value:
                self.collect(item)

    def root(self, value, origin):
        root = physical(value)
        require(root.is_dir() and str(root) not in FORBIDDEN_ROOTS, "broad or nonphysical root refused")
        self.candidates.setdefault(str(root), []).append(origin)

    def inventories(self, report, origin):
        for key in ("inventories_before", "inventories_after", "input_roots_before", "input_roots_after",
                    "host_input_roots_after", "execution_view_after"):
            rows = report.get(key, [])
            require(type(rows) is list, "actual producer root list required")
            for row in rows:
                require(type(row) is dict and type(row.get("root")) is str and "catalog" in row,
                        "complete actual producer root/catalog observation required")
                self.root(row["root"], origin + ":" + key)

    def source(self, report):
        source = physical(report["source"])
        observed = report.get("source_before")
        if observed is not None:
            previous = observed["manifest"]
        else:
            selected = {json.dumps(pin, sort_keys=True): pin for pin in report["input_pins"] if
                        type(pin) is dict and PIN <= pin.keys() and pin["path"] == str(source.parent / "source-manifest.json") and
                        pin["sha256"] == report["source_manifest_sha256"]}
            require(len(selected) == 1, "one original actual complete source-manifest pin required")
            previous = next(iter(selected.values()))
        self.pin(previous)
        self.root(str(source.parent), "complete original source plus its exact manifest")


def make(request, discovery, template):
    """Derive associations from producer bodies; accept no guessed role paths."""
    exact(request["reports"], ("commands", "workers", "native_products"), "three current producer report observations required")
    # Request observations are CURRENT full-seven pins. Only references inside
    # unchanged original producer documents may use a historical device bridge.
    reports = {key: document(full_pin(pin), 64 * MIB, discovery.guard) for key, pin in request["reports"].items()}
    commands, workers, retained = reports["commands"], reports["workers"], reports["native_products"]
    require(commands.get("status") == "PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY" and
            commands.get("compiled_command_products") == 14 and commands.get("retained_command_products") == 9 and
            commands.get("fresh_command_products") == 5 and commands.get("actual_Runtime_cases_executed") == 0,
            "actual completed Typed14 build required; failed/partial batches refused")
    require(workers.get("status") == "PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED" and
            retained.get("schema") == "uwvm-native74-retained-products-at-rest-v1" and
            retained.get("status") == "PASS_RETAINED_NATIVE74_BUILD_PRODUCTS_FRESH_AT_REST_CATALOG_NOT_EXECUTED" and
            retained.get("preparation_mode") == "retained74-at-rest" and retained.get("execution_performed") is False,
            "actual preserved Workers and distinct successful Native74 retained receipt required")
    raw = discovery.doc(retained["original_raw_inner"])
    outer = discovery.doc(retained["original_raw_outer"])
    require(raw.get("status") == outer.get("status") == "FAIL", "original Native batch statuses must remain unchanged failures")
    interface_pin = discovery.pin(raw["producer_reports"]["native"])
    interfaces = discovery.doc(interface_pin)
    require(interfaces.get("status") == "PREPARED_NATIVE_INTERFACES_IDENTITIES_AND_SOURCE_PROVIDERS",
            "actual native interface producer required")
    sources = {**reports, "raw-native": raw, "raw-native-owner": outer, "native-interfaces": interfaces}
    for role, report in sources.items():
        discovery.inventories(report, role)
    for report in (commands, workers, raw, interfaces):
        discovery.source(report)
    for pin in [*request["reports"].values(), retained["original_raw_inner"], retained["original_raw_outer"], interface_pin]:
        discovery.root(str(Path(pin["path"]).parent), "exact selected original producer output")
    selection = decode(json.dumps(template))
    labels, input_pins = {}, {}
    def add(label, previous):
        require(type(label) is str and re.fullmatch("[a-z][a-z0-9_-]{0,63}", label), "safe derived input label required")
        actual = discovery.pin(previous)
        require(label not in labels or labels[label] == thin(actual), "derived input-label collision")
        labels[label], input_pins[label] = thin(actual), actual
        return label
    for role, pin in request["reports"].items():
        add(selection["reports"][role], pin)
    add(selection["reports"]["native_interfaces"], interface_pin)
    built = {row["id"]: row for row in commands["products"]}
    require(len(commands["products"]) == 14 and set(built) == set(selection["products"]), "all actual Typed14 IDs required")
    for identifier, row in built.items():
        add(selection["products"][identifier]["binary"], row["artifact"])
        add(selection["products"][identifier]["receipt"], row["receipt"])
        discovery.collect(discovery.doc(row["receipt"]))
    profiles = {}
    for name in ("ros2", "dual"):
        rows = [row for row in workers["profiles"] if row["profile"] == name]
        require(len(rows) == 1, "one actual Worker profile required")
        profiles[name] = rows[0]
        add(selection["workers"][name], rows[0]["binary"])
        discovery.collect(rows[0])
        for record in rows[0]["objects"]:
            discovery.collect(workers["commands"][record["command_index"]])
    add(selection["native_peer"], raw["typed_peer"]["input"])
    add(selection["exec_gate"], raw["exec_gate"])
    discovery.collect(raw["exec_gate_compile_commands"])
    discovery.collect(raw["typed_peer_compile"])
    discovery.collect(retained["retained_compilation"]["roles"])
    discovery.collect(retained["product_pins"])
    discovery.collect(retained["at_rest"]["restored_ordinary_placeholders"])
    discovery.pin(retained["input_pin_catalog"]["pin"])
    discovery.pin(retained["execution_file_catalog"])
    discovery.collect(retained["execution_elf_catalog"])
    exact(request["worker_registries"], ("go-api", "hg"), "two explicit actual compiled Worker registry documents required")
    for profile, pin in request["worker_registries"].items():
        registry = document(full_pin(pin), 64 * MIB, discovery.guard)
        require(registry.get("status") == "GENERATED_SOURCE_ONLY" and registry.get("native_registry_emitted") is True and
                registry.get("implicit_permissions") == [], "actual no-authority Worker registry generation required")
        add("go-api-registry-generation" if profile == "go-api" else "hg-registry-generation", pin)
        discovery.root(str(Path(pin["path"]).parent), "explicit actual complete Worker generated outputs")
        discovery.collect(registry["source_inputs"])
    gateway = {row["profile"]: row for row in raw["gateway_profiles"]}
    for profile in ("go-api", "hg"):
        require(profile in gateway, "actual native Gateway profile required")
        discovery.collect(gateway[profile])
    for name, (count, worker_profile, native_profile) in FAMILIES.items():
        chosen = selection["families"][name]
        family_rows = [row for row in interfaces["families"] if row["family"] == name]
        require(len(family_rows) == 1, "one actual native family section required")
        family = family_rows[0]
        discovery.collect({key: family[key] for key in ("binary", "binding_receipt", "generation", "materialization", "source_selection",
                                                       "export_build", "official_package_build")})
        add(chosen["generation"], family["generation"])
        derived = physical(family["derived_tree"]["root"])
        discovery.root(str(derived), "actual complete native family derived tree")
        export = [pin for pin in interfaces["product_pins"] if Path(pin["path"]).name == "native-metadata.json" and
                  Path(pin["path"]).is_relative_to(derived)]
        require(len(export) == 1, "unique actual native exporter stdout required")
        add(chosen["native_export"], export[0])
        add(chosen["gateway"], gateway[native_profile]["binary"])
        for field, role in (("peer_mappings", "peer"), ("worker_mappings", "worker-" + worker_profile),
                            ("gateway_mappings", "gateway-" + native_profile),
                            ("gateway_context_mappings", "context-" + native_profile)):
            chosen[field] = [add("map-" + role + "-%02d" % i, pin)
                             for i, pin in enumerate(request["mapping_sets"][role])]
    add(selection["cyclonedds_xml"], request["cyclonedds_xml"])
    discovery.root(str(Path(request["cyclonedds_xml"]["path"]).parent), "actual selected Cyclone profile directory")
    selection["restart_mappings"] = request["restart_mappings"]
    selection["worker_catalog_roots"] = request["worker_catalog_roots"]
    selection["expected_network_namespace"] = request["current_context"]["network_namespace"]
    return selection, labels, input_pins, sources, retained


def select_roots(discovery, mandatory, source, controls, outputs):
    """Reduce only explicitly observed complete ancestors; never guess a parent."""
    discovery.root(str(controls), "fresh sealed metadata controls")
    exact_roots = set(mandatory)
    require(all(root in discovery.candidates for root in exact_roots), "mandatory execution roots lack complete producer observations")
    chosen = set(exact_roots) | {str(controls)}
    for name in discovery.pins:
        path = Path(name)
        if path.is_relative_to(source):
            continue
        candidates = sorted((root for root in discovery.candidates if path.is_relative_to(Path(root))),
                            key=lambda root: (len(Path(root).parts), root))
        require(candidates, "no observed complete input root covers selected file: " + name)
        chosen.add(candidates[0])
    # An explicit whole parent already covers a redundant child. Native
    # execution-view and Worker host roots remain exact Frame inventories,
    # because the real consumer requires those complete current root records.
    reduced = {root for root in chosen if root in exact_roots or root == str(controls) or not any(
        root != parent and Path(root).is_relative_to(Path(parent)) for parent in chosen)}
    require(1 <= len(reduced) <= 64, "derived complete input closure exceeds64 roots; never broaden a root to fit")
    for root in reduced:
        require(all(not (Path(root).is_relative_to(path) or path.is_relative_to(Path(root))) for path in outputs),
                "fresh admission/transport output overlaps an actual complete input root")
    return sorted(reduced)


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument("--request", type=Path, required=True)
    parser.add_argument("--request-sha256", required=True)
    parser.add_argument("--maker-sha256", required=True)
    args = parser.parse_args()
    sys.dont_write_bytecode = True
    frame = None
    try:
        require(all(re.fullmatch("[0-9a-f]{64}", value) for value in (args.request_sha256, args.maker_sha256)),
                "actual request and maker SHA256 arguments required")
        raw, request_pin = opened(args.request, 4 * MIB, body=True)
        require(request_pin["sha256"] == args.request_sha256, "Root's selected request bytes differ")
        request = decode(raw)
        exact(request, ("format_version", "kind", "source", "python", "current_context", "reports", "worker_registries", "mapping_sets",
                        "worker_catalog_roots", "restart_mappings", "cyclonedds_xml", "paths", "limits"), "closed maker request required")
        require(request["format_version"] == 1 and request["kind"] == "uwvm-unitree-typed28-configuration-request-v1",
                "explicit maker request version required")
        exact(request["source"], ("root", "manifest"), "closed actual complete source selection required")
        maker_pin = opened(Path(__file__).absolute(), MIB)
        require(maker_pin["sha256"] == args.maker_sha256, "actual metadata maker source differs")
        manifest_pin = full_pin(request["source"]["manifest"])
        manifest_raw, actual = opened(manifest_pin["path"], 16 * MIB, body=True)
        require(actual == manifest_pin, "actual complete manifest observation changed")
        frozen = decode(manifest_raw)
        require(type(frozen) is dict and frozen, "complete source manifest dictionary required")
        source, common, api, unitree, guard, bootstrap_pins = bootstrap(request, manifest_raw, frozen, maker_pin)
        python_pin = full_pin(request["python"])
        require(opened(python_pin["path"], 128 * MIB, guard) == python_pin and
                Path(sys.executable).resolve(strict=True) == Path(python_pin["path"]), "actual current executing Python observation required")
        bootstrap_pins.append(python_pin)
        initial_source = common.source_state(source, Path(manifest_pin["path"]))
        exact(request["paths"], ("controls", "admission_out", "transport_out"), "closed fresh output paths required")
        controls, admission_out, transport_out = (physical(request["paths"][key], existing=False)
                                                 for key in ("controls", "admission_out", "transport_out"))
        require(all(not path.exists() and not path.is_symlink() and not path.is_relative_to(source) and
                    not source.is_relative_to(path) for path in (controls, admission_out, transport_out)) and
                all(left != right and not left.is_relative_to(right) and not right.is_relative_to(left)
                    for i, left in enumerate((controls, admission_out, transport_out))
                    for right in (controls, admission_out, transport_out)[i + 1:]) and
                len(os.fsencode(transport_out / "s27")) < 104, "fresh disjoint outputs and short actual UNIX socket path required")
        for path in (controls, admission_out, transport_out):
            ancestor = path.parent
            require(ancestor.is_dir(), "preexisting physical output parent required")
            space = os.statvfs(ancestor)
            require(space.f_bavail * space.f_frsize >= 8 * GIB, "eight-GiB backing-filesystem reserve required")
        exact(request["limits"], ("jobs", "command_seconds", "rss_bytes", "log_bytes", "output_bytes"), "closed bounded lane limits required")
        limits = request["limits"]
        require(all(type(value) is int and value > 0 for value in limits.values()) and limits["jobs"] == 1 and
                limits["command_seconds"] <= 600 and limits["rss_bytes"] <= 2 * GIB and limits["log_bytes"] <= 16 * MIB and
                limits["output_bytes"] <= 4 * GIB, "serial600second/2GiB bounded transport lane required")
        exact(request["mapping_sets"], MAP_ROLES, "seven explicit actual mapped SDK role sets required")
        for pins in request["mapping_sets"].values():
            require(type(pins) is list and 1 <= len(pins) <= 32, "nonempty finite actual required mapped files required")
            for pin in pins:
                full_pin(pin)
        require(type(request["worker_catalog_roots"]) is list and 1 <= len(request["worker_catalog_roots"]) <= 32 and
                len(set(request["worker_catalog_roots"])) == len(request["worker_catalog_roots"]), "explicit complete Worker host roots required")
        discovery = Discovery(request, guard)
        template_path = source / "test/0073.ros-tests/support/source/ros_unitree/typed_transport.selection.template.json"
        template_raw, template_pin = opened(template_path, MIB, guard, body=True)
        require(frozen.get(str(template_path.relative_to(source))) == {key: template_pin[key] for key in ("bytes", "sha256")},
                "complete frozen selection template required")
        template = decode(template_raw)
        selection, inputs, selected, reports, retained = make(request, discovery, template)
        for role, pins in request["mapping_sets"].items():
            require(all(opened(pin["path"], 2 * GIB, guard) == pin for pin in pins), "mapping role requires current observations: " + role)
        require(opened(request["cyclonedds_xml"]["path"], MIB, guard) == request["cyclonedds_xml"], "Cyclone profile must be current")
        mandatory = {row["root"] for row in retained["execution_view_after"]} | set(request["worker_catalog_roots"])
        controls.mkdir(mode=0o700, parents=False, exist_ok=False)
        roots = select_roots(discovery, mandatory, source, controls, (admission_out, transport_out))
        # Mapping scopes may be broader than inventories, but cannot authorize
        # a new path. Unused roots are removed rather than passed to Evidence.
        mappings = [row for row in request["restart_mappings"] if any(Path(root).is_relative_to(Path(row["root"])) for root in roots)]
        selection["restart_mappings"] = mappings
        selection_pin = write(controls / "typed-selection.json", selection)
        inputs["typed-transport-selection"] = thin(selection_pin)
        config = {"format_version": 1, "cgroup": request["current_context"]["cgroup"], "input_roots": roots,
                  "inputs": inputs, "limits": limits, "upstreams": {}, "simulation": None, "benchmark": None, "profiler": None,
                  "environment": {"ROS_DOMAIN_ID": "1", "RMW_IMPLEMENTATION": "rmw_cyclonedds_cpp", "ROS_SECURITY_ENABLE": "false",
                                  "CYCLONEDDS_URI": "file://" + request["cyclonedds_xml"]["path"]}}
        config_pin = write(controls / "frame-config.json", config)
        invocation = [python_pin["path"], "-B", str(source / "test/0073.ros-tests/support/source/ros_unitree/run_typed_transport.py"),
                      "--source", str(source), "--manifest", manifest_pin["path"], "--manifest-sha256", manifest_pin["sha256"],
                      "--config", config_pin["path"], "--config-sha256", config_pin["sha256"], "--selection-input", "typed-transport-selection",
                      "--out", str(transport_out)]
        invocation_pin = write(controls / "run-invocation.json", {"argv": invocation, "execution_performed": False})
        args_frame = types.SimpleNamespace(source=source, manifest=Path(manifest_pin["path"]), manifest_sha256=manifest_pin["sha256"],
                                          config=Path(config_pin["path"]), config_sha256=config_pin["sha256"], out=admission_out)
        frame = unitree.Frame(args_frame, "unitree-typed28-configuration-admission-only")
        frame.unitree, frame.frozen, frame.native_aliases, frame.workers = unitree, frozen, {}, {}
        admission = api.frozen_module(source, frozen, "test/0073.ros-tests/support/source/ros_unitree/typed_transport_inputs.py", "typed28_configuration_real_admission")
        native = api.frozen_module(source, frozen, "test/0073.ros-tests/support/source/ros_javascript_guest/native.py", "typed28_configuration_native_proofs")
        admitted = admission.admit(frame, api, native, selection)
        require(len(admitted["products"]) == 14 and not frame.actors, "admission must qualify14 products without actors")
        frame.report.update(metadata_maker=maker_pin, metadata_request=request_pin, metadata_python=python_pin,
            original_producer_sources={role: {key: report.get(key) for key in ("source", "source_manifest_sha256")}
                                      for role, report in reports.items()},
            derived_configuration=config_pin, derived_selection=selection_pin, future_invocation=invocation_pin,
            root_selection_origins={root: discovery.candidates[root] for root in roots},
            separately_observed_planning_bridges=discovery.bridges,
            actual_Runtime_cases_executed=0, actors_started=0, compiler_executed=False,
            ROS_DDS_simulator_executed=False, Guest_rights_changed=False,
            transport_output_absent=not transport_out.exists(), controls_sealed_before_admission=True)
        require(common.source_state(source, Path(manifest_pin["path"])) == initial_source, "selected complete maker source changed")
        for pin in [request_pin, *bootstrap_pins, template_pin, config_pin, selection_pin, invocation_pin, *selected.values()]:
            require(opened(pin["path"], max(64 * MIB, pin["bytes"]), guard) == pin, "maker bootstrap/current selected file changed")
        for pin in discovery.pins.values():
            require(opened(pin["path"], max(64 * MIB, pin["bytes"]), guard) == pin,
                    "complete selected planning closure changed before final metadata acceptance")
        require(not transport_out.exists() and not transport_out.is_symlink(), "metadata preparation must not start or create transport output")
        return frame.finish(STATUS)
    except BaseException as error:
        if frame is not None:
            return frame.finish("FAIL", repr(error))
        print(repr(error), file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
