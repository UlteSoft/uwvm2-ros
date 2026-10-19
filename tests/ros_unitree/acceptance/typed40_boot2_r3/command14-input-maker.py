#!/usr/bin/env python3
"""Prepare current controls for nine retained and five fresh typed commands.

This metadata-only producer derives the current configuration from the exact
preserved Source40 continuation configuration. Historical configurations and
reports are read without modification. Every new physical pin is opened in the
current cgroup; actual retained-file qualification occurs separately in the
builder before any compiler starts. No process or compiler is launched here.
"""
from __future__ import annotations

import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import sys
import time
import traceback
import types


MIB = 1024**2
SOURCE = Path("/var/tmp/uwvm-ros-complete-20261015-r1/inputs-r40/source")
MF = "a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd"
SEED = "ed31e25026685a3e18f760c28dbc713468d66555c0984cdb306683ca1d86a8bd"
BATCH = "b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747"
CG = {"memory_max": 68719476736, "memory_swap_max": 0, "expected_device": 29,
    "expected_inode": 18358, "expected_boot_id": "501c54bd-af24-4d22-900c-8e24de274464",
    "expected_leaf": "/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope",
    "expected_cpus": "0-31"}
OLD_CG = {**CG, "expected_inode": 18151,
          "expected_boot_id": "af121220-8c6e-4302-9853-f08bf4c50dbb"}
ROLE_FILES = {"maker": "command14-input-maker.py", "builder": "command14-builder.py",
    "adapter": "typed-command-postboot.py", "restart": "requalify_after_restart.py",
    "union": "requalify_hardlink_union.py",
    "cpp_qualifier": "official-cpp-qualifier.py", "utilities": "command14-utilities.py"}


def need(condition, reason):
    if not condition:
        raise ValueError(reason)


def pairs(rows):
    result = {}
    for key, value in rows:
        need(key not in result, "duplicate JSON member")
        result[key] = value
    return result


def decode(raw):
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda _: need(False, "nonfinite JSON"))


def opened(path, maximum=64 * MIB):
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, "canonical physical input required")
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        import stat
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum, "bounded regular file required")
        data = bytearray()
        while True:
            block = os.read(descriptor, min(MIB, maximum + 1 - len(data)))
            if not block:
                break
            data.extend(block)
            need(len(data) <= maximum, "input grew beyond bound")
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(data) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
             for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")), "input changed while opened")
        return bytes(data), {"path": str(path), "bytes": len(data), "sha256": hashlib.sha256(data).hexdigest(),
            "device": before.st_dev, "inode": before.st_ino, "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
    finally:
        os.close(descriptor)


def thin(pin):
    return {key: pin[key] for key in ("path", "bytes", "sha256")}


def disjoint(left, right):
    return left != right and left not in right.parents and right not in left.parents


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    for name in ("source", "manifest", "seed-config", "original-batch", "out", "actor-out", "helper-selection"):
        parser.add_argument("--" + name, type=Path, required=True)
    for name in ("manifest-sha256", "seed-config-sha256", "original-batch-sha256", "runner-sha256",
                 "builder-sha256", "helper-selection-sha256"):
        parser.add_argument("--" + name, required=True)
    args = parser.parse_args()
    need(sys.platform == "linux" and os.geteuid() == 0, "Root's actual current Linux cgroup required")
    need(args.source == SOURCE and args.manifest == SOURCE.parent / "source-manifest.json" and
         args.manifest_sha256 == MF and args.seed_config_sha256 == SEED and args.original_batch_sha256 == BATCH,
         "exact original Source40 manifest, continuation configuration and failed batch required")
    initial = {}
    def document(path, wanted):
        body, pin = opened(path)
        need(pin["sha256"] == wanted, "actual metadata SHA256 differs: " + str(path))
        initial[pin["path"]] = pin
        return decode(body), pin
    frozen, mf_pin = document(args.manifest, MF)
    need(type(frozen) is dict and len(frozen) == 17928, "complete original Source40 manifest required")
    helper, hp = opened(SOURCE / "tools/ros_distro/common.py", MIB)
    need(frozen.get("tools/ros_distro/common.py") == {key: hp[key] for key in ("bytes", "sha256")},
         "shared helper differs from the complete actual manifest")
    initial[hp["path"]] = hp
    common = types.ModuleType("typed_command_postboot_metadata_common")
    common.__file__ = hp["path"]
    sys.modules[common.__name__] = common
    exec(compile(helper, hp["path"], "exec"), common.__dict__)
    probe_raw, probe_pin = opened(SOURCE / "tests/ros_cgroup/probe.py", MIB)
    need(frozen.get("tests/ros_cgroup/probe.py") == {key: probe_pin[key] for key in ("bytes", "sha256")},
         "cgroup probe differs from the complete manifest")
    initial[probe_pin["path"]] = probe_pin
    seed, seed_pin = document(args.seed_config, SEED)
    batch, batch_pin = document(args.original_batch, BATCH)
    selection, selection_pin = document(args.helper_selection, args.helper_selection_sha256)
    code = Path(__file__).resolve().parent
    own_raw, own_pin = opened(Path(__file__).resolve(), 2 * MIB)
    need(own_pin["sha256"] == args.runner_sha256 and selection.get("format_version") == 1 and
         selection.get("kind") == "typed-command-postboot-helper-selection" and
         set(selection) == {"format_version", "kind", "helpers"} and set(selection["helpers"]) == set(ROLE_FILES),
         "exact reviewed current helper selection required")
    initial[own_pin["path"]] = own_pin
    helpers = {}
    for role, name in ROLE_FILES.items():
        path = code / name
        body, pin = opened(path, 2 * MIB)
        need(thin(pin) == selection["helpers"][role] and pin["path"] == str(path), "reviewed helper changed: " + role)
        initial[pin["path"]], helpers[role] = pin, pin
    need(helpers["builder"]["sha256"] == args.builder_sha256 and helpers["maker"] == own_pin,
         "exact current maker and builder required")
    need(seed["cgroup"] == OLD_CG and seed["limits"] == {"jobs": 1, "command_seconds": 600,
         "rss_bytes": 512 * MIB, "log_bytes": 16 * MIB, "output_bytes": 4 * 1024**3} and
         seed["upstreams"] == {} and all(seed[key] is None for key in ("simulation", "benchmark", "profiler")),
         "unchanged original compiler limits and historical cgroup selection required")
    need(batch.get("status") == "FAIL" and batch.get("source") == str(SOURCE) and
         batch.get("source_manifest_sha256") == MF and batch.get("source_preserved") is True,
         "original failed build is historical evidence only")
    roots = [Path(value) for value in seed["input_roots"]]
    cargo = Path("/usr/lib/cargo/bin/coreutils")
    need(cargo not in roots, "one explicitly new complete historical cargo alias root required")
    roots.append(cargo)
    if code not in roots:
        roots.append(code)
    need(len(roots) <= 63 and all(root.is_absolute() and root.resolve(strict=True) == root and root.is_dir() and
         root not in (Path("/"), Path("/tmp"), Path("/var/tmp")) for root in roots), "complete original input roots required")
    out, actor = args.out, args.actor_out
    need(all(path.is_absolute() and path.resolve() == path and not path.exists() and not path.is_symlink()
         and path.parent.resolve(strict=True) == path.parent for path in (out, actor)) and disjoint(out, actor) and
         all(disjoint(path, root) for path in (out, actor) for root in [SOURCE, *roots]), "fresh nonoverlapping output roots required")
    before = common.cgroup_state(CG)
    need(all(row["events"].get(key, 0) == 0 for row in before["ancestors"]
         for key in ("oom", "oom_kill", "oom_group_kill")), "current admitted cgroup OOM counters must be zero")
    cancelled = common.install_interrupts()
    out.mkdir(mode=0o700)
    controls = out / "controls"
    controls.mkdir(mode=0o700)
    pins, products, catalogs, errors = dict(initial), {}, [], []
    report = {"format_version": 1, "kind": "typed-command-postboot-input-preparation", "status": "RUNNING",
        "source": str(SOURCE), "source_manifest_sha256": MF, "configuration_seed": seed_pin,
        "maker": own_pin, "actors_started": 0, "actual_build_executed": False,
        "original_batch_current_pin": batch_pin,
        "raw_original_failed_batch": batch_pin, "historical_cgroup": OLD_CG, "cgroup_before": before,
        "commands": [], "compiler_executed": False, "Runtime_executed": False,
        "planned_retained_products": 9, "planned_fresh_products": 5, "Guest_rights": []}
    started, finalizing, source_before = time.monotonic(), False, None
    def guard():
        need(finalizing or not cancelled["cancelled"], "metadata preparation interrupted")
        need(finalizing or time.monotonic() - started <= 3600, "metadata deadline exhausted")
        need(resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= 512 * MIB, "metadata512MiB bound exceeded")
        common.cgroup_preserved(before, common.cgroup_state(CG))
        for ancestor in (out, *out.parents):
            fs = os.statvfs(ancestor)
            need(fs.f_bavail * fs.f_frsize >= 8 * 1024**3, "eightGiB backing reserve required")
        total = 0
        for folder, directories, files in os.walk(out, followlinks=False):
            for name in (*directories, *files):
                path = Path(folder) / name
                need(not path.is_symlink(), "metadata output aliases are forbidden")
                if path.is_file():
                    total += path.stat().st_size
        need(total <= 512 * MIB, "complete metadata output exceeds512MiB")
    def remember(path, expected=None):
        pin = common.opened(Path(path))
        physical = Path(pin["path"])
        need(physical == args.manifest or physical.is_relative_to(SOURCE) or
             physical.is_relative_to(out) or any(physical.is_relative_to(root) for root in roots),
             "current selected input lacks a complete explicit source/control/package root")
        need(expected is None or pin == expected, "current physical input changed")
        need(pin["path"] not in pins or pins[pin["path"]] == pin, "current opened identity changed")
        pins[pin["path"]] = pin
        return pin
    def write(path, value):
        with path.open("x", encoding="utf-8") as stream:
            json.dump(value, stream, indent=2)
            stream.write("\n")
        pin = common.opened(path)
        products[pin["path"]] = pin
        return pin
    try:
        guard()
        source_before = common.source_state(SOURCE, args.manifest)
        report["source_before"] = source_before
        catalogs = [common.catalog(root, out / ("input-before-%02d.jsonl" % index), roots)
                    for index, root in enumerate(roots)]
        for label, value in seed["inputs"].items():
            actual = remember(value["path"])
            need(thin(actual) == value, "actual selected compiler/source bytes changed: " + label)
        config = copy.deepcopy(seed)
        config["cgroup"] = dict(CG)
        config["inputs"]["typed_cpp_qualifier"] = thin(helpers["cpp_qualifier"])
        config["input_roots"] = [str(root) for root in [*roots, controls]]
        config_pin = write(controls / "frame-config.json", config)
        python = config["inputs"]["python"]
        need(python["path"] == "/usr/bin/python3.14" and Path(sys.executable).resolve(strict=True) == Path(python["path"]),
             "the exact selected current Python must run this metadata producer")
        argv = [python["path"], "-B", helpers["builder"]["path"], "--source", str(SOURCE),
            "--manifest", str(args.manifest), "--manifest-sha256", MF, "--config", config_pin["path"],
            "--config-sha256", config_pin["sha256"], "--out", str(actor), "--utilities", helpers["utilities"]["path"],
            "--runner-sha256", helpers["builder"]["sha256"]]
        invocation = write(controls / "root-build-invocation.json", {"format_version": 1,
            "kind": "typed-command-postboot-build-invocation", "argv": argv, "executed": False,
            "retained_products": 9, "fresh_products": 5, "actual_compiler_jobs": 14,
            "required_outer_rss_bytes": 1610612736})
        report.update(configuration=config_pin, invocation=invocation, current_helpers=helpers)
        guard()
    except BaseException as error:
        errors.append({"error": repr(error), "traceback": traceback.format_exc()})
    finally:
        finalizing = True
        for key, operation in (("source_preserved", lambda: source_before is not None and
                common.source_state(SOURCE, args.manifest) == source_before),
            ("input_pins_preserved", lambda: all(common.opened(Path(pin["path"])) == pin for pin in pins.values())),
            ("product_pins_preserved", lambda: all(common.opened(Path(pin["path"])) == pin for pin in products.values()))):
            try:
                report[key] = operation()
                need(report[key], key + " failed")
            except BaseException as error:
                errors.append({key: repr(error)})
        try:
            after = [common.catalog(root, out / ("input-after-%02d.jsonl" % index), roots)
                     for index, root in enumerate(roots)]
            need(len(catalogs) == len(after) and all({key: value for key, value in left.items() if key != "catalog"} ==
                 {key: value for key, value in right.items() if key != "catalog"} for left, right in zip(catalogs, after)),
                 "complete current input roots changed")
            report.update(inventories_before=catalogs, inventories_after=after, whole_roots_preserved=True)
            guard()
            report["cgroup_after"] = common.cgroup_state(CG)
        except BaseException as error:
            errors.append({"final_catalogs_and_guard": repr(error)})
        report.update(input_pins=list(pins.values()), product_pins=list(products.values()), errors=errors,
            cleanup={"status": "PASS_NO_ACTORS", "remaining": []},
            status="PASS_TYPED_COMMAND_POSTBOOT_INPUTS_ONLY" if not errors and report.get("configuration") else "FAIL")
        common.atomic_json(out / "report.json", report)
        print(json.dumps({"status": report["status"], "report": common.opened(out / "report.json"),
                          "configuration": report.get("configuration"), "invocation": report.get("invocation"), "errors": errors}), flush=True)
    return 0 if report["status"].startswith("PASS_") else 1


if __name__ == "__main__":
    raise SystemExit(main())
