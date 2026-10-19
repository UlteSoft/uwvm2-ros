#!/usr/bin/env python3
"""Own the two explicit Typed40 post-restart phases inside the selected cgroup.

``prepare`` starts only the reviewed zero-actor metadata maker. ``build`` is a
separate invocation that requires the maker's actual complete success receipt
and starts only its exact published compiler command. This owner does not run
Wasm, DDS, ROS, simulation, robot commands or performance probes. Historical
failed reports are content provenance; their cgroup/process facts are never
used as current admission or rewritten to resemble a new execution.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import signal
import stat
import sys
import time
import traceback
import types


MIB, GIB = 1024**2, 1024**3
P = Path("/var/tmp/uwvm-ros-complete-20261015-r1")
M = P / "acceptance-storage-r1"
B = Path("/var/tmp/uwvm-ros-unitree-20261016-r1")
SOURCE = P / "inputs-r40/source"
MANIFEST = SOURCE.parent / "source-manifest.json"
MANIFEST_SHA = "a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd"
CODE = B / "source40-unitree-command14-boot3-controls-r7"
META = M / "source40-unitree-command14-boot3-inputs-r7"
ACTOR = M / "source40-unitree-command14-boot3-build-r7"
OWNER = M / "source40-unitree-command14-boot3-owner-r7"
ORIGINAL = M / "source40-unitree-command14-build-r1/report.json"
ORIGINAL_SHA = "b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747"
# Exact current observations supplied by Root; no debug parent/sibling is an input.
EXACT_CURRENT_ALIASES = [
    {
        "current_pin": {
            "bytes": 2186512,
            "ctime_ns": 1791893890224829563,
            "device": 66312,
            "inode": 12359710,
            "mtime_ns": 1774989310000000000,
            "path": "/root/.debug/usr/lib/x86_64-linux-gnu/libc.so.6/90ebd03ae9d9f42b23b4eb82fdf70352cf744198/elf",
            "sha256": "d763925433ff9b757390549e1b20c085f5e6de27ae700fe89194178d96a8a2b0"
        },
        "historical_anchor": {
            "device": 66307,
            "inode": 12359710
        }
    },
    {
        "current_pin": {
            "bytes": 2735712,
            "ctime_ns": 1791893890225590691,
            "device": 66312,
            "inode": 12322103,
            "mtime_ns": 1774168304000000000,
            "path": "/root/.debug/usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.35/3a7a8c6bd922e6314edb7a710fdc0293265c2c42/elf",
            "sha256": "5bb0d21308f123b6ad46c6f35b42cedfcb8d6d439a53aa3dae04d880aaffdde3"
        },
        "historical_anchor": {
            "device": 66307,
            "inode": 12322103
        }
    }
]
EXACT_CURRENT_MODES = {
    "/root/.debug/usr/lib/x86_64-linux-gnu/libc.so.6/90ebd03ae9d9f42b23b4eb82fdf70352cf744198/elf": 33261,
    "/root/.debug/usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.35/3a7a8c6bd922e6314edb7a710fdc0293265c2c42/elf": 33188
}
CG = {"memory_max": 64 * GIB, "memory_swap_max": 0, "expected_device": 29,
      "expected_inode": 18358, "expected_boot_id": "501c54bd-af24-4d22-900c-8e24de274464",
      "expected_leaf": "/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope",
      "expected_cpus": "0-31"}
OLD_CG = {**CG, "expected_boot_id": "af121220-8c6e-4302-9853-f08bf4c50dbb", "expected_inode": 18151}
NAMES = {"maker": "command14-input-maker.py", "builder": "command14-builder.py",
         "adapter": "typed-command-postboot.py", "restart": "requalify_after_restart.py",
         "union": "requalify_hardlink_union.py",
         "exact_aliases": "requalify_exact_aliases.py",
         "cpp_qualifier": "official-cpp-qualifier.py", "utilities": "command14-utilities.py"}
OUTER = {"self_rss_bytes": 512 * MIB, "tree_rss_bytes": 1536 * MIB,
         "prepare_command_seconds": 3600, "build_command_seconds": 19800,
         "active_seconds": 20400, "whole_seconds": 21600, "cleanup_seconds": 600,
         "reserve_bytes": 8 * GIB}
PIN_KEYS = frozenset(("path", "bytes", "sha256", "device", "inode", "mtime_ns", "ctime_ns"))
LIMITS = {"jobs": 1, "command_seconds": 600, "rss_bytes": 512 * MIB,
          "log_bytes": 16 * MIB, "output_bytes": 4 * GIB}
HISTORICAL_MANIFEST_CONTENT = {
    str(P / "inputs-r35/source-manifest.json"): "eb7fd49e03325deffbe510e24be06d1c0e8274b0207f0a33bd335ea470ad2396",
    str(P / "inputs-r37/source-manifest.json"): "c8a2bde2c12fab767ca31eba68a08c10093dc5844ef76ea2dec8f87ae9738ee7"}


def need(value, reason):
    if not value:
        raise ValueError(reason)


def pairs(items):
    result = {}
    for key, value in items:
        need(key not in result, "duplicate JSON member: " + key)
        result[key] = value
    return result


def read(path, maximum=64 * MIB):
    """Hash bounded metadata through its opened descriptor and stable name."""
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, "canonical ordinary metadata path required")
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum, "bounded regular metadata required")
        body = bytearray()
        while True:
            piece = os.read(descriptor, min(MIB, maximum - len(body) + 1))
            if not piece:
                break
            body.extend(piece)
            need(len(body) <= maximum, "opened metadata exceeded its byte bound")
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(body) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
             for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode")),
             "metadata changed during opened read")
        return bytes(body), {"path": str(path), "bytes": len(body), "sha256": hashlib.sha256(body).hexdigest(),
            "device": before.st_dev, "inode": before.st_ino, "mtime_ns": before.st_mtime_ns,
            "ctime_ns": before.st_ctime_ns}
    finally:
        os.close(descriptor)


def document(path, maximum=64 * MIB):
    raw, pin = read(path, maximum)
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda value: need(False, "nonfinite JSON: " + value)), pin


def thin(pin):
    return {key: pin[key] for key in ("path", "bytes", "sha256")}


def full_pin(pin):
    need(type(pin) is dict and set(pin) == PIN_KEYS and type(pin["path"]) is str and
         Path(pin["path"]).is_absolute() and not any(char in pin["path"] for char in ("\0", "\r", "\n")) and
         all(type(pin[key]) is int and pin[key] >= 0 for key in ("bytes", "device", "mtime_ns", "ctime_ns")) and
         type(pin["inode"]) is int and pin["inode"] > 0 and type(pin["sha256"]) is str and
         re.fullmatch(r"[0-9a-f]{64}", pin["sha256"]), "exact seven-field file observation required")
    return pin


def disjoint(left, right):
    return left != right and left not in right.parents and right not in left.parents


def same_catalog(left, right):
    return {key: value for key, value in left.items() if key != "catalog"} == {
        key: value for key, value in right.items() if key != "catalog"}


def alias(value):
    """Keep the selected invocation spelling and all physical alias identities."""
    path, prefix, links = Path(value), Path("/"), []
    need(path.is_absolute(), "absolute executable selection required")
    for part in path.parts[1:]:
        prefix /= part
        if prefix.is_symlink():
            before, target = prefix.lstat(), os.readlink(prefix)
            resolved, after = prefix.resolve(strict=True), prefix.lstat()
            need(all(getattr(before, key) == getattr(after, key) for key in
                 ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode")) and
                 os.readlink(prefix) == target, "selected executable alias changed")
            links.append({"path": str(prefix), "target": target, "resolved": str(resolved),
                "device": before.st_dev, "inode": before.st_ino, "bytes": before.st_size,
                "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns})
    return {"selection": str(path), "resolved": str(path.resolve(strict=True)), "links": links}


def metadata_success(value, helpers, original_current):
    """Require an actual completed metadata-only maker, never an old r2 report."""
    need(type(value) is dict and value.get("format_version") == 1 and
         value.get("kind") == "typed-command-postboot-input-preparation" and
         value.get("status") == "PASS_TYPED_COMMAND_POSTBOOT_INPUTS_ONLY" and
         value.get("source") == str(SOURCE) and value.get("source_manifest_sha256") == MANIFEST_SHA and
         value.get("maker") == helpers["maker"] and value.get("errors") == [] and
         value.get("commands") == [] and value.get("actors_started") == 0 and
         value.get("compiler_executed") is False and value.get("actual_build_executed") is False and
         value.get("cleanup") == {"status": "PASS_NO_ACTORS", "remaining": []} and
         all(value.get(key) is True for key in ("source_preserved", "input_pins_preserved",
             "whole_roots_preserved", "product_pins_preserved")),
         "actual successful preserved zero-actor postboot metadata receipt required")
    # A producer may additionally record the original historical full-seven
    # report. It cannot replace this independently reopened current selection.
    if "original_batch_current_pin" in value:
        need(value["original_batch_current_pin"] == original_current, "metadata current failed-batch pin differs")


def build_success(value, configuration, original):
    """Check the original compiler-only contract with explicit retained/fresh counts."""
    need(type(value) is dict and value.get("kind") == "uwvm-unitree-command14-continuation-builds-v1" and
         value.get("status") == "PASS_SOURCE40_FOURTEEN_COMMAND_PRODUCTS_NINE_RETAINED_FIVE_FRESH_BINARY_VALIDATED_ONLY" and
         value.get("source") == str(SOURCE) and value.get("source_manifest_sha256") == MANIFEST_SHA and
         value.get("configuration") == configuration and value.get("compiled_command_products") == 14 and
         value.get("compiled_Guest_products") == 14 and value.get("retained_command_products") == 9 and
         value.get("fresh_command_products") == 5 and value.get("separate_official_object_compiles") == 0 and
         value.get("retained_official_object_count") == 1 and value.get("actual_native_definition_records") == 43 and
         value.get("actual_Runtime_cases_executed") == 0 and type(value.get("products")) is list and
         len(value["products"]) == 14 and type(value.get("commands")) is list and len(value["commands"]) == 14 and
         all(row.get("status") == "PASS" and type(row.get("returncode")) is int and row["returncode"] == 0
             for row in value["commands"]) and value.get("cleanup") and
         all(row.get("status") == "PASS" and not row.get("remaining") and not row.get("errors")
             for row in value["cleanup"]) and not value.get("finalization_errors") and
         all(value.get(key) is True for key in ("source_preserved", "input_pins_preserved", "whole_roots_preserved")),
         "actual fourteen compiler-only products with preserved inputs and owned cleanup required")
    qualification = value.get("postboot_retained_input_qualification", {})
    union = qualification.get("historical_hardlink_union", {})
    registered = union.get("registered_complete_roots", [])
    wanted = {item["root"]: item["catalog"] for item in original["inventories_before"]}
    wanted["/usr/lib/cargo/bin/coreutils"] = {
        "path": str(M / "source39-native-gateways-physics-peer-r1/input-after-19.jsonl"),
        "bytes": 27376, "sha256": "c9b899e553c50248a9b82ca898b55691a4100a2d5235174afcb43f7082d1ebc4",
        "device": 81, "inode": 677449, "mtime_ns": 1792118135782019011, "ctime_ns": 1792118135782019011}
    need(len(wanted) == 60 and union.get("policy") == "historical-complete-inventoried-union" and
         union.get("all_selected_roots_admitted") is True and union.get("execution_qualification") == "NOT_PERFORMED" and
         len(registered) == 60 and {item["root"]: item["catalog"] for item in registered} == wanted and
         qualification.get("raw_reports_rewritten") is False and qualification.get("old_execution_promoted_to_current") is False,
         "complete original59/cargo historical hardlink union must finish before any compiler result")
    auxiliary = union.get("current_auxiliary_complete_roots", [])
    anchors = [{"device": 66307, "inode": value} for value in (12327242, 12327243, 12327247,
        12327248, 12327249, 12327250, 12327251, 12327252, 12327255)]
    need(len(auxiliary) == 1 and union.get("current_auxiliary_policy") == "current-complete-root-bound-to-old-anchors" and
         auxiliary[0].get("current_root") == "/usr/lib/arm-none-eabi/bin" and
         auxiliary[0].get("historical_anchor_classes") == anchors and auxiliary[0]["totals"]["files"] == 10 and
         auxiliary[0]["totals"]["entries"] == 10 and auxiliary[0]["totals"]["directories"] == 1 and
         auxiliary[0]["totals"]["symlinks"] == 0 and auxiliary[0].get("historical_auxiliary_names") == "NOT_RECORDED" and
         auxiliary[0].get("historical_auxiliary_nlink") == "NOT_RECORDED" and
         auxiliary[0].get("execution_qualification") == "NOT_PERFORMED",
         "only the complete current ARM ten-name/nine-anchor root may supplement the historical union")
    exact_rows = union.get("exact_current_aliases", [])
    expected_exact = {item["current_pin"]["path"]: item for item in EXACT_CURRENT_ALIASES}
    need(union.get("exact_current_alias_policy") == "current-exact-files-bound-to-old-anchors" and
         union.get("exact_aliases_qualified") is True and len(exact_rows) == 2 and
         {item.get("current_pin", {}).get("path") for item in exact_rows} == set(expected_exact),
         "exact two current elf aliases required without a complete debug directory claim")
    for item in exact_rows:
        selected = expected_exact[item["current_pin"]["path"]]
        need(set(item) == {"policy", "historical_anchor", "current_pin", "current_mode", "current_nlink",
             "historical_auxiliary_names", "historical_auxiliary_nlink", "sibling_directories", "execution_qualification"} and
             item["policy"] == "current-exact-files-bound-to-old-anchors" and
             item["current_pin"] == selected["current_pin"] and item["historical_anchor"] == selected["historical_anchor"] and
             item["current_nlink"] == 2 and item["current_mode"] == EXACT_CURRENT_MODES[item["current_pin"]["path"]] and
             item["historical_auxiliary_names"] == item["historical_auxiliary_nlink"] == "NOT_RECORDED" and
             item["sibling_directories"] == "NOT_SELECTED" and item["execution_qualification"] == "NOT_PERFORMED",
             "exact-file alias sidecar differs from independently selected current physical observations")
    retained, fresh = [], []
    for row in value["products"]:
        need(row.get("status") == "COMPILED_COMMAND_AND_FULL_BINARY_VALIDATED_NOT_EXECUTED" and
             row.get("granted_rights") == [] and row.get("future_transport_cases_executed") == 0 and
             row.get("cross_module_guard_modified") is False and row.get("preloaded_application") is False,
             "binary-validation result cannot supply Runtime acceptance or Guest authority")
        provenance = row.get("compilation_provenance", {})
        mode = provenance.get("mode")
        if mode == "RETAINED_COMPLETED_PRODUCT_FROM_FAILED_BATCH":
            original = full_pin(provenance.get("original_report"))
            need(thin(original) == {"path": str(ORIGINAL), "bytes": 3911428, "sha256": ORIGINAL_SHA},
                 "retained product must name the genuine unchanged failed batch")
            retained.append(row)
        elif mode == "FRESH_IN_CONTINUATION":
            fresh.append(row)
        else:
            need(False, "unknown command compilation provenance")
    need(len(retained) == 9 and len(fresh) == 5 and
         sorted(row["language"] for row in retained) == sorted(["rust"] * 3 + ["javascript"] * 3 + ["assemblyscript"] * 3) and
         sorted(row["language"] for row in fresh) == sorted(["tinygo"] * 3 + ["c", "cpp"]),
         "exact nine completed retained and five fresh product families required")
    return retained, fresh


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument("--phase", choices=("prepare", "build"), required=True)
    parser.add_argument("--control", type=Path, required=True)
    parser.add_argument("--control-sha256", required=True)
    args = parser.parse_args()
    started = time.monotonic()
    need(sys.platform == "linux" and os.geteuid() == 0 and hasattr(os, "pidfd_open"),
         "actual current Linux Root pidfd owner required")
    need(re.fullmatch(r"[0-9a-f]{64}", args.control_sha256), "exact reviewed control SHA256 required")
    control, control_pin = document(args.control, 4 * MIB)
    need(control_pin["sha256"] == args.control_sha256 and type(control) is dict and set(control) == {
        "format_version", "kind", "source", "manifest", "manifest_sha256", "manifest_pin", "source_files", "cgroup",
        "code_root", "metadata_out", "actor_out", "owner_out", "python", "helpers", "helper_selection", "inputs", "outer"} and
        control["format_version"] == 1 and control["kind"] == "typed-command-postboot-root-control-v1",
        "closed current reviewed Root control required")
    need(control["source"] == str(SOURCE) and control["manifest"] == str(MANIFEST) and
         control["manifest_sha256"] == MANIFEST_SHA and control["source_files"] == 17928 and
         control["cgroup"] == CG and control["code_root"] == str(CODE) and control["metadata_out"] == str(META) and
         control["actor_out"] == str(ACTOR) and control["owner_out"] == str(OWNER) and control["outer"] == OUTER,
         "exact original source content, fresh paths and new incarnation budgets required")
    frozen, manifest_pin = document(MANIFEST, 16 * MIB)
    need(full_pin(control["manifest_pin"]) == manifest_pin and manifest_pin["sha256"] == MANIFEST_SHA and
         manifest_pin["bytes"] == 3404861 and type(frozen) is dict and len(frozen) == 17928,
         "actual current complete Source40 manifest required; historical/null pins are not current")
    bootstrap, bodies = [control_pin, manifest_pin], {}
    for relative in ("tools/ros_distro/common.py", "tests/ros_cgroup/probe.py"):
        raw, pin = read(SOURCE / relative, MIB)
        need(frozen.get(relative) == {key: pin[key] for key in ("bytes", "sha256")},
             "full-manifest-bound owner/probe required")
        bodies[relative] = raw
        bootstrap.append(pin)
    common = types.ModuleType("typed40_postboot_root_common")
    common.__file__ = str(SOURCE / "tools/ros_distro/common.py")
    sys.modules[common.__name__] = common
    exec(compile(bodies["tools/ros_distro/common.py"], common.__file__, "exec"), common.__dict__)
    probe = types.ModuleType("typed40_postboot_root_probe")
    probe.__file__ = str(SOURCE / "tests/ros_cgroup/probe.py")
    exec(compile(bodies["tests/ros_cgroup/probe.py"], probe.__file__, "exec"), probe.__dict__)
    common._CGROUP_PROBE = probe
    cancelled = common.install_interrupts()
    initial_cgroup = common.cgroup_state(CG)
    need(all(row["events"].get(key, 0) == 0 for row in initial_cgroup["ancestors"]
             for key in ("oom", "oom_kill", "oom_group_kill")), "current ancestor OOM counters must start at zero")
    need(args.control.resolve(strict=True) == CODE / "root-control.json" and CODE.resolve(strict=True) == CODE,
         "invoke the actual sealed fresh code/control directory")
    need(type(control["helpers"]) is dict and set(control["helpers"]) == set(NAMES) | {"outer"},
         "exact reviewed helper closure required")
    helpers = {}
    for role, name in {**NAMES, "outer": "root-launch.py"}.items():
        selection = control["helpers"][role]
        need(type(selection) is dict and set(selection) == {"path", "bytes", "sha256"} and selection["path"] == str(CODE / name),
             "exact helper role/path required")
        _, pin = read(CODE / name, 2 * MIB)
        need(thin(pin) == selection, "staged reviewed helper differs: " + role)
        helpers[role] = pin
        bootstrap.append(pin)
    need(Path(__file__).resolve(strict=True) == Path(helpers["outer"]["path"]), "actual staged Root owner required")
    helper_selection, selection_pin = document(CODE / "helper-selection.json", MIB)
    need(thin(selection_pin) == control["helper_selection"] and helper_selection == {"format_version": 1,
         "kind": "typed-command-postboot-helper-selection", "helpers": {role: thin(helpers[role]) for role in NAMES}},
         "exact maker helper selection required")
    bootstrap.append(selection_pin)
    need(set(CODE.iterdir()) == {CODE / name for name in [*NAMES.values(), "root-launch.py", "helper-selection.json", "root-control.json"]},
         "sealed fresh code root contains unexpected or missing files")
    need(type(control["inputs"]) is dict and set(control["inputs"]) == {"seed_config", "original_batch"},
         "closed seed configuration and original raw batch selections required")
    selections, values = {}, {}
    for role, selection in control["inputs"].items():
        value, pin = document(Path(selection["path"]))
        need(thin(pin) == selection, "current selected input content differs")
        selections[role], values[role] = pin, value
        bootstrap.append(pin)
    seed, original = values["seed_config"], values["original_batch"]
    need(seed.get("cgroup") == OLD_CG and seed.get("limits") == LIMITS and
         selections["original_batch"]["path"] == str(ORIGINAL) and selections["original_batch"]["bytes"] == 3911428 and
         selections["original_batch"]["sha256"] == ORIGINAL_SHA and original.get("status") == "FAIL" and
         original.get("source") == str(SOURCE) and original.get("source_manifest_sha256") == MANIFEST_SHA,
         "genuine unchanged historical inputs required; old cgroup is provenance only")
    python = full_pin(control["python"])
    need(python["path"] == "/usr/bin/python3.14" and common.opened(Path(python["path"]), 128 * MIB) == python and
         Path(sys.executable).resolve(strict=True) == Path(python["path"]), "actual current selected Python required")
    bootstrap.append(python)
    python_alias = alias(python["path"])
    roots = []
    need(type(seed.get("input_roots")) is list and 1 <= len(seed["input_roots"]) <= 62,
         "original complete seed roots required")
    for value in [*seed["input_roots"], "/usr/lib/cargo/bin/coreutils", str(CODE)]:
        root = Path(value)
        need(root.is_absolute() and root.resolve(strict=True) == root and root.is_dir() and root not in
             (Path("/"), Path("/tmp"), Path("/var/tmp")), "complete canonical current root required")
        if root not in roots:
            roots.append(root)
    if args.phase == "build":
        need(META.resolve(strict=True) == META and META.is_dir(), "actual completed metadata root required")
        roots.append(META)
    need(len(roots) <= 64, "current complete root count exceeds its finite bound")
    out = OWNER / args.phase
    need(all(disjoint(root, path) for root in [SOURCE, *roots] for path in (out, ACTOR)) and
         disjoint(out, META) and disjoint(ACTOR, META), "fresh outputs must not modify source/input roots")
    need(not out.exists() and not out.is_symlink() and not ACTOR.exists() and not ACTOR.is_symlink() and
         OWNER.parent.resolve(strict=True) == OWNER.parent and ACTOR.parent.resolve(strict=True) == ACTOR.parent and
         META.parent.resolve(strict=True) == META.parent, "fresh canonical phase/actor output paths required")
    if args.phase == "prepare":
        need(not META.exists() and not META.is_symlink(), "metadata preparation needs a fresh output")
    if not OWNER.exists():
        OWNER.mkdir(mode=0o700)
    need(OWNER.is_dir() and not OWNER.is_symlink(), "ordinary owned phase parent required")
    out.mkdir(mode=0o700)
    for name in ("home", "tmp"):
        (out / name).mkdir(mode=0o700)
    runner, source_before, before = None, None, initial_cgroup
    catalogs, pins, products, external_union_pins, exact_alias_pins = [], {}, {}, {}, {}
    finalizing = False
    report = {"format_version": 1, "kind": "typed-command-postboot-root-phase-supervisor", "status": "RUNNING",
        "phase": args.phase, "source": str(SOURCE), "source_manifest_sha256": MANIFEST_SHA,
        "control": control_pin, "owner": helpers["outer"], "limits": OUTER, "cgroup_before": before,
        "historical_failed_batch_current_pin": selections["original_batch"], "historical_batch_status": "FAIL",
        "historical_cgroup_continuity_claimed": False, "commands": [], "compiler_phase_started": False,
        "retained_products_requested": 9, "fresh_products_requested": 5, "Runtime_executed": False,
        "ROS_or_simulator_started": False, "Guest_authority": "UNCHANGED_EMPTY"}

    def guard():
        elapsed = time.monotonic() - started
        need(elapsed <= OUTER["whole_seconds"] and (finalizing or elapsed <= OUTER["active_seconds"]),
             "finite Root owner deadline exceeded")
        need(finalizing or not cancelled["cancelled"], "cooperative cancellation requested")
        self_rss = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024
        need(finalizing or self_rss <= OUTER["self_rss_bytes"], "Root metadata RSS bound exceeded")
        current = common.cgroup_state(CG)
        common.cgroup_preserved(before, current)
        need(alias(python["path"]) == python_alias, "selected Python alias changed")
        need(runner is None or runner.rss() + self_rss <= OUTER["tree_rss_bytes"], "aggregate owned Root tree RSS exceeded")
        # Low disk must not prevent owned cleanup or input preservation.
        # Finalization keeps the cgroup, aggregate RSS, alias and time guards.
        if not finalizing:
            for folder in (out, *out.parents, ACTOR.parent, META.parent):
                space = os.statvfs(folder)
                need(space.f_bavail * space.f_frsize >= OUTER["reserve_bytes"], "eight-GiB backing reserve exhausted")
        return current

    def remember(pin):
        full_pin(pin)
        path = Path(pin["path"])
        # Retain the original owner's two exact manifest-only exceptions. No
        # surrounding historical source directory or different manifest is
        # admitted by a matching suffix or an arbitrary historical path.
        exact_historical_manifest = HISTORICAL_MANIFEST_CONTENT.get(pin["path"]) == pin["sha256"]
        need(path == MANIFEST or exact_historical_manifest or path.is_relative_to(SOURCE) or path.is_relative_to(ACTOR) or
             path.is_relative_to(META) or any(path.is_relative_to(root) for root in roots),
             "current pin lacks complete source/input/output coverage")
        need(common.opened(path, max(64 * MIB, pin["bytes"])) == pin, "current input changed")
        need(pin["path"] not in pins or pins[pin["path"]] == pin, "current physical input identity changed")
        pins[pin["path"]] = pin
        return pin

    def product(path, expected=None):
        path = Path(path)
        need(path.is_relative_to(out) or path.is_relative_to(ACTOR), "product outside fresh owned output")
        pin = common.opened(path, max(64 * MIB, expected["bytes"] if expected else 64 * MIB))
        need(expected is None or pin == expected, "current retained product changed")
        need(pin["path"] not in products or products[pin["path"]] == pin, "owned product overwritten")
        products[pin["path"]] = pin
        return pin

    def save():
        report.update(commands=runner.commands if runner else [], input_pins=list(pins.values()),
                      product_pins=list(products.values()),
                      separately_selected_union_metadata_current_pins=list(external_union_pins.values()),
                      separately_selected_exact_alias_current_pins=list(exact_alias_pins.values()))
        common.atomic_json(out / "report.json", report)

    environment = {"PATH": "/usr/bin:/bin", "LANG": "C.UTF-8", "LC_ALL": "C.UTF-8",
        "HOME": str(out / "home"), "TMPDIR": str(out / "tmp"), "PYTHONDONTWRITEBYTECODE": "1",
        "OMP_NUM_THREADS": "1", "OPENBLAS_NUM_THREADS": "1", "MKL_NUM_THREADS": "1", "GOMAXPROCS": "1"}
    save()
    try:
        guard()
        source_before = common.source_state(SOURCE, MANIFEST)
        report["source_before"] = source_before
        for pin in bootstrap:
            remember(pin)
        for index, root in enumerate(roots):
            guard()
            row = common.catalog(root, out / ("input-before-%02d.jsonl" % index), roots)
            product(row["catalog"]["path"])
            catalogs.append(row)
        command = [python["path"], "-B", helpers["maker"]["path"], "--source", str(SOURCE), "--manifest", str(MANIFEST),
            "--manifest-sha256", MANIFEST_SHA, "--seed-config", selections["seed_config"]["path"],
            "--seed-config-sha256", selections["seed_config"]["sha256"], "--original-batch", str(ORIGINAL),
            "--original-batch-sha256", ORIGINAL_SHA, "--out", str(META), "--actor-out", str(ACTOR),
            "--runner-sha256", helpers["maker"]["sha256"], "--builder-sha256", helpers["builder"]["sha256"],
            "--helper-selection", selection_pin["path"], "--helper-selection-sha256", selection_pin["sha256"]]
        configuration = None
        if args.phase == "build":
            admission, admission_pin = document(META / "report.json")
            remember(admission_pin)
            metadata_success(admission, helpers, selections["original_batch"])
            for pin in admission["input_pins"] + admission["product_pins"]:
                remember(pin)
            configuration_body, configuration = document(Path(admission["configuration"]["path"]))
            invocation, invocation_pin = document(Path(admission["invocation"]["path"]))
            need(configuration == admission["configuration"] and invocation_pin == admission["invocation"],
                 "actual metadata configuration/invocation changed")
            remember(configuration)
            remember(invocation_pin)
            controls = META / "controls"
            expected_configuration = {**seed, "cgroup": dict(CG),
                "inputs": {**seed["inputs"], "typed_cpp_qualifier": thin(helpers["cpp_qualifier"])},
                "input_roots": [str(root) for root in roots if root != META] + [str(controls)]}
            need(configuration["path"] == str(controls / "frame-config.json") and
                 configuration_body == expected_configuration and len(configuration_body["input_roots"]) <= 64,
                 "exact seed-derived current configuration and complete original root closure required")
            command = [python["path"], "-B", helpers["builder"]["path"], "--source", str(SOURCE),
                "--manifest", str(MANIFEST), "--manifest-sha256", MANIFEST_SHA, "--config", configuration["path"],
                "--config-sha256", configuration["sha256"], "--out", str(ACTOR), "--utilities",
                helpers["utilities"]["path"], "--runner-sha256", helpers["builder"]["sha256"]]
            need(invocation == {"format_version": 1, "kind": "typed-command-postboot-build-invocation", "argv": command,
                 "executed": False, "retained_products": 9, "fresh_products": 5, "actual_compiler_jobs": 14,
                 "required_outer_rss_bytes": 1536 * MIB}, "only the exact actual published build invocation may run")
            for role, name in NAMES.items():
                current = common.opened(CODE / name, 2 * MIB)
                need(current == helpers[role], "sealed actual code helper changed: " + role)
                remember(current)
            report.update(metadata_report=admission_pin, actual_configuration=configuration, actual_invocation=invocation_pin)
        runner = common.SerialRunner(out, {"rss_bytes": OUTER["tree_rss_bytes"], "command_seconds":
            OUTER["prepare_command_seconds" if args.phase == "prepare" else "build_command_seconds"], "log_bytes": 16 * MIB}, guard)
        report.update(selected_argv=command, compiler_phase_started=args.phase == "build", phase_launch_requested=True)
        save()
        if args.phase == "build":
            # Select only the two genuine current elf files before launching
            # the builder. Their sibling debug/probes names are not inventoried.
            for selected in EXACT_CURRENT_ALIASES:
                pin = selected["current_pin"]
                need(common.opened(Path(pin["path"]), 8 * MIB) == pin,
                     "exact current alias changed before child launch")
                info = Path(pin["path"]).stat(follow_symlinks=False)
                need(info.st_nlink == 2 and info.st_mode == EXACT_CURRENT_MODES[pin["path"]],
                     "exact current alias mode/link count changed before child launch")
                exact_alias_pins[pin["path"]] = pin
            save()
        row = runner.run("typed-command-" + args.phase, command, out, environment)
        runner.discover()
        need(all(common.pidfd_terminal(item["fd"]) for item in runner.owned.values()), "owned phase descendants remain live")
        for key in runner.unproven:
            now = common.identity(key[0])
            need(now is None or common.birth(now) != key or now["state"] in ("Z", "X"), "same-birth unproven actor remains live")
        row["owned_terminal_barrier"] = {"all_observed_pidfds_terminal": True, "birth_ledger_retained": True}
        result, result_pin = document((META if args.phase == "prepare" else ACTOR) / "report.json")
        if args.phase == "prepare":
            metadata_success(result, helpers, selections["original_batch"])
        else:
            retained, fresh = build_success(result, configuration, original)
            exact_bindings = result["postboot_retained_input_qualification"]["historical_hardlink_union"]["exact_alias_helper_bindings"]
            need(exact_bindings == {"base": helpers["restart"], "union": helpers["union"]},
                 "exact alias wrapper must bind these independently sealed current base/union helpers")
            # These two metadata files are outside every complete Frame root.
            # Select their exact old/full content facts independently; do not
            # enlarge Frame.pin or inventory either surrounding directory.
            external = result["postboot_retained_input_qualification"]["separately_selected_cargo_metadata"]
            need(external["raw_report_status_unchanged"] == "FAIL" and
                 external["selection_scope"] == "ONE_EXACT_CATALOG_REFERENCE_FROM_UNMODIFIED_SOURCE39_INPUT_ROOTS_AFTER_19",
                 "original external cargo producer status and finite selection must remain explicit")
            selected_external = [external["current_raw_report"], external["historical_catalog_bridge"]["current_pin"]]
            expected_external = {
                str(M / "source39-native-gateways-physics-peer-r1/report.json"): (7986560,
                    "75248cdddba3ab42f0020758cec81b0388c59d45ebd46d356ad434f6207e04d2", 677463,
                    1792118141739107205, 1792118141748107338),
                str(M / "source39-native-gateways-physics-peer-r1/input-after-19.jsonl"): (27376,
                    "c9b899e553c50248a9b82ca898b55691a4100a2d5235174afcb43f7082d1ebc4", 677449,
                    1792118135782019011, 1792118135782019011)}
            for selected in selected_external:
                full_pin(selected)
                need(selected["path"] in expected_external and selected["device"] == 81 and
                     tuple(selected[key] for key in ("bytes", "sha256", "inode", "mtime_ns", "ctime_ns")) ==
                     expected_external[selected["path"]] and common.opened(Path(selected["path"]), 16 * MIB) == selected,
                     "exact separately selected external union metadata changed")
                external_union_pins[selected["path"]] = selected
            need(set(external_union_pins) == set(expected_external), "both exact external union metadata inputs required")
            for retained_row in retained:
                sidecar = retained_row.get("current_file_qualification")
                need(type(sidecar) is dict and set(sidecar) == {"artifact", "receipt", "source",
                     "original_receipt_unchanged", "Runtime_executed"} and
                     sidecar["original_receipt_unchanged"] is True and sidecar["Runtime_executed"] is False,
                     "retained product needs its separate current physical qualification")
                for field in ("artifact", "receipt", "source"):
                    historical, current = full_pin(retained_row[field]), full_pin(sidecar[field])
                    need(historical["device"] in (81, 66307) and
                         current["device"] == {81: 81, 66307: 66312}[historical["device"]] and
                         all(current[key] == historical[key] for key in PIN_KEYS - {"device"}),
                         "retained file differs beyond its known persistent filesystem renumbering")
                    remember(current)
        for pin in result["input_pins"]:
            remember(pin)
        for pin in result.get("product_pins", []):
            # Metadata products become immutable build inputs; the prepare
            # phase nevertheless owns its fresh sibling META directory.
            if args.phase == "prepare":
                remember(pin)
            else:
                product(pin["path"], pin)
        report["phase_report"] = remember(result_pin) if args.phase == "prepare" else product(result_pin["path"], result_pin)
        report["phase_completed"] = True
        guard()
    except BaseException as error:
        report.update(status="FAIL", error=repr(error), traceback=traceback.format_exc())
    finally:
        finalizing = True
        errors = []

        def finish(label, operation):
            try:
                operation()
            except BaseException as error:
                errors.append({"stage": label, "error": repr(error)})
                report["status"] = "FAIL"

        def cooperative():
            if runner is None:
                return
            for child in runner.children:
                if child.poll() is not None:
                    continue
                first, now = runner.roots.get(child.pid), common.identity(child.pid)
                if now is None and child.poll() is not None:
                    continue
                need(first is not None and now is not None and common.birth(first) == common.birth(now) and
                     now["parent_pid"] == os.getpid(), "only the retained same-birth direct coordinator may receive TERM")
                runner.acquire(child.pid, os.getpid())
                key = common.birth(first)
                if key not in runner.owned:
                    need(child.poll() is not None, "live direct coordinator lacks its owned pidfd")
                    continue
                try:
                    signal.pidfd_send_signal(runner.owned[key]["fd"], signal.SIGTERM)
                except ProcessLookupError:
                    pass
                until = min(time.monotonic() + OUTER["cleanup_seconds"], started + OUTER["whole_seconds"])
                while child.poll() is None and time.monotonic() < until:
                    guard()
                    runner.discover()
                    need(runner.rss() <= OUTER["tree_rss_bytes"], "owned cleanup tree RSS exceeded")
                    for command_row in runner.commands:
                        log = out / (command_row["label"] + ".log")
                        need(not log.exists() or log.stat().st_size <= 16 * MIB, "cleanup raw log exceeded its bound")
                    time.sleep(0.05)

        def cleanup():
            report["cleanup"] = runner.close() if runner else {"status": "PASS", "remaining": [], "scope": "NO_ACTOR_STARTED"}
            need(report["cleanup"]["status"] == "PASS" and not report["cleanup"]["remaining"] and
                 not report["cleanup"].get("errors"), "owned pidfd cleanup incomplete")

        finish("direct-coordinator-600-second-cooperative-cleanup", cooperative)
        finish("child-first-owned-pidfd-fallback-cleanup", cleanup)
        if runner:
            for command_row in runner.commands:
                path = out / (command_row["label"] + ".log")
                if path.exists():
                    finish("stable-final-owned-log", lambda path=path: product(path))
            if (out / "commands.json").exists():
                finish("stable-final-command-ledger", lambda: product(out / "commands.json"))

        def preserve():
            need(source_before is not None and len(catalogs) == len(roots), "complete initial closure was not admitted")
            report["source_after"] = common.source_state(SOURCE, MANIFEST)
            need(report["source_after"] == source_before, "current complete frozen source changed")
            after = []
            for index, root in enumerate(roots):
                guard()
                current = common.catalog(root, out / ("input-after-%02d.jsonl" % index), roots)
                product(current["catalog"]["path"])
                need(same_catalog(catalogs[index], current), "current complete input root changed")
                after.append(current)
            need(all(common.opened(Path(pin["path"]), max(64 * MIB, pin["bytes"])) == pin for pin in pins.values()),
                 "current opened input changed before finalization")
            need(all(common.opened(Path(pin["path"]), 16 * MIB) == pin for pin in external_union_pins.values()),
                 "exact separately selected external union metadata changed before finish")
            need(all(common.opened(Path(pin["path"]), 8 * MIB) == pin and
                     Path(pin["path"]).stat(follow_symlinks=False).st_nlink == 2 and
                     Path(pin["path"]).stat(follow_symlinks=False).st_mode == EXACT_CURRENT_MODES[pin["path"]]
                     for pin in exact_alias_pins.values()),
                 "two exact current alias files changed before terminal preservation")
            need(all(common.opened(Path(pin["path"]), max(64 * MIB, pin["bytes"])) == pin for pin in products.values()),
                 "current retained product changed before finalization")
            report.update(inventories_before=catalogs, inventories_after=after, source_preserved=True,
                inputs_preserved=True, products_preserved=True, selected_python_alias=python_alias, cgroup_after=guard())

        finish("current-source-roots-and-physical-pin-preservation", preserve)
        report["finalization_errors"] = errors
        if report.get("phase_completed") and report["status"] == "RUNNING" and not errors:
            report["status"] = ("PASS_TYPED_COMMAND_POSTBOOT_INPUTS_ONLY" if args.phase == "prepare" else
                                "PASS_TYPED_COMMAND_POSTBOOT_NINE_RETAINED_FIVE_FRESH_COMPILATION_ONLY")
        report["elapsed_seconds"] = time.monotonic() - started
        save()
    return 0 if report["status"].startswith("PASS_TYPED_COMMAND_POSTBOOT_") else 1


if __name__ == "__main__":
    raise SystemExit(main())
