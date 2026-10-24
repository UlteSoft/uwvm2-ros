#!/usr/bin/env python3
"""Stage one reviewed Typed40 post-restart source package, without starting it.

Root supplies a SHA256-bound JSON payload containing the exact seven reviewed
source files. The program independently admits the current complete Source40
freeze, current host Python, retained seed configuration and unchanged failed
batch. It fills current seven-field control observations from actual opened
files, writes a fresh sealed nine-file control directory, and emits separate
prepare/build commands as metadata. It starts no child and grants no rights.
"""
from __future__ import annotations

import argparse
import base64
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
P = Path("/var/tmp/uwvm-ros-complete-20261015-r1")
M = P / "acceptance-storage-r1"
B = Path("/var/tmp/uwvm-ros-unitree-20261016-r1")
SOURCE = P / "inputs-r40/source"
MANIFEST = SOURCE.parent / "source-manifest.json"
MANIFEST_SHA = "a1c302c830b9965c73d3a9f1fa775056ff60cb7b252ec2af15c8c73ee0969afd"
CODE = B / "source40-unitree-command14-boot2-controls-r1"
META = M / "source40-unitree-command14-boot2-inputs-r1"
ACTOR = M / "source40-unitree-command14-boot2-build-r1"
OWNER = M / "source40-unitree-command14-boot2-owner-r1"
RECEIPT = B / "source40-unitree-command14-boot2-stage-r1.json"
SEED = M / "source40-unitree-command14-input-admission-r2/controls/frame-config.json"
SEED_SHA = "ed31e25026685a3e18f760c28dbc713468d66555c0984cdb306683ca1d86a8bd"
ORIGINAL = M / "source40-unitree-command14-build-r1/report.json"
ORIGINAL_SHA = "b7c07cc7b83bf5996d85e1e812c7e4bc5221358d6dafcb69c3601d261af86747"
PYTHON = Path("/usr/bin/python3.14")
PYTHON_SHA = "b8d8288faefdd300201f43fcf00f6f539a27218eeed3a3dff5ab10b9c4c99700"
NAMES = {"maker": "command14-input-maker.py", "builder": "command14-builder.py",
         "adapter": "typed-command-postboot.py", "restart": "requalify_after_restart.py",
         "cpp_qualifier": "official-cpp-qualifier.py", "utilities": "command14-utilities.py",
         "outer": "root-launch.py"}
CG = {"memory_max": 64 * GIB, "memory_swap_max": 0, "expected_device": 29,
      "expected_inode": 18315, "expected_boot_id": "ec0d5f0f-3060-4220-bee2-2a7791ed4e72",
      "expected_leaf": "/sys/fs/cgroup/system.slice/docker-4dd58ad4560fde2ab9b539e4ef9cab1fac531654e99ad554aff1fce8ae0eae37.scope",
      "expected_cpus": "0-31"}
OUTER = {"self_rss_bytes": 512 * MIB, "tree_rss_bytes": 1536 * MIB,
         "prepare_command_seconds": 3600, "build_command_seconds": 19800,
         "active_seconds": 20400, "whole_seconds": 21600, "cleanup_seconds": 600,
         "reserve_bytes": 8 * GIB}


def need(value, message):
    if not value:
        raise ValueError(message)


def pairs(items):
    result = {}
    for key, value in items:
        need(key not in result, "duplicate JSON member: " + key)
        result[key] = value
    return result


def read(path, maximum):
    path = Path(path)
    need(path.is_absolute() and path.resolve(strict=True) == path, "canonical ordinary metadata path required")
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        need(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= maximum, "bounded nonempty ordinary input required")
        body = bytearray()
        while True:
            piece = os.read(descriptor, min(MIB, maximum - len(body) + 1))
            if not piece:
                break
            body.extend(piece)
            need(len(body) <= maximum, "opened metadata exceeded its finite bound")
        after, named = os.fstat(descriptor), path.stat(follow_symlinks=False)
        need(len(body) == before.st_size and all(getattr(before, key) == getattr(after, key) == getattr(named, key)
             for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode")),
             "opened input changed")
        return bytes(body), {"path": str(path), "bytes": len(body), "sha256": hashlib.sha256(body).hexdigest(),
            "device": before.st_dev, "inode": before.st_ino, "mtime_ns": before.st_mtime_ns,
            "ctime_ns": before.st_ctime_ns}
    finally:
        os.close(descriptor)


def parse(raw):
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda value: need(False, "nonfinite JSON: " + value))


def thin(pin):
    return {key: pin[key] for key in ("path", "bytes", "sha256")}


def encoded(value):
    return (json.dumps(value, indent=2, sort_keys=True) + "\n").encode()


def main():
    started = time.monotonic()
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument("--payload", type=Path, required=True)
    parser.add_argument("--payload-sha256", required=True)
    args = parser.parse_args()
    need(sys.platform == "linux" and os.geteuid() == 0, "actual current Root Linux lane required")
    need(re.fullmatch(r"[0-9a-f]{64}", args.payload_sha256), "reviewed exact stage payload SHA256 required")
    payload_raw, payload_pin = read(args.payload, 16 * MIB)
    need(payload_pin["sha256"] == args.payload_sha256, "Root's reviewed source payload differs")
    payload = parse(payload_raw)
    need(type(payload) is dict and set(payload) == {"format_version", "kind", "files"} and
         type(payload["format_version"]) is int and payload["format_version"] == 1 and
         payload["kind"] == "typed-command-postboot-reviewed-source-package" and type(payload["files"]) is dict and
         set(payload["files"]) == set(NAMES), "closed seven-file reviewed source package required")
    decoded = {}
    for role, name in NAMES.items():
        row = payload["files"][role]
        need(type(row) is dict and set(row) == {"name", "bytes", "sha256", "base64"} and row["name"] == name and
             type(row["bytes"]) is int and 0 < row["bytes"] <= 2 * MIB and type(row["sha256"]) is str and
             re.fullmatch(r"[0-9a-f]{64}", row["sha256"]) and type(row["base64"]) is str and
             len(row["base64"]) <= 3 * MIB, "bounded exact role source bytes required")
        raw = base64.b64decode(row["base64"], validate=True)
        need(len(raw) == row["bytes"] and hashlib.sha256(raw).hexdigest() == row["sha256"] and
             base64.b64encode(raw).decode("ascii") == row["base64"] and
             raw.startswith(b"#!/usr/bin/env python3\n"), "reviewed full source body differs: " + role)
        decoded[role] = raw
    manifest_raw, manifest_pin = read(MANIFEST, 16 * MIB)
    frozen = parse(manifest_raw)
    need(manifest_pin["sha256"] == MANIFEST_SHA and manifest_pin["bytes"] == 3404861 and
         type(frozen) is dict and len(frozen) == 17928, "actual complete Source40 manifest required")
    bodies, bootstrap = {}, [payload_pin, manifest_pin]
    for relative in ("tools/ros_distro/common.py", "test/0073.ros-tests/support/source/ros_cgroup/probe.py"):
        raw, pin = read(SOURCE / relative, MIB)
        need(frozen.get(relative) == {key: pin[key] for key in ("bytes", "sha256")}, "frozen ownership/probe body differs")
        bodies[relative] = raw
        bootstrap.append(pin)
    common = types.ModuleType("typed40_postboot_stage_common")
    common.__file__ = str(SOURCE / "tools/ros_distro/common.py")
    sys.modules[common.__name__] = common
    exec(compile(bodies["tools/ros_distro/common.py"], common.__file__, "exec"), common.__dict__)
    probe = types.ModuleType("typed40_postboot_stage_probe")
    probe.__file__ = str(SOURCE / "test/0073.ros-tests/support/source/ros_cgroup/probe.py")
    exec(compile(bodies["test/0073.ros-tests/support/source/ros_cgroup/probe.py"], probe.__file__, "exec"), probe.__dict__)
    common._CGROUP_PROBE = probe
    before_cgroup = common.cgroup_state(CG)

    def guard():
        need(time.monotonic() - started <= 900 and
             resource.getrusage(resource.RUSAGE_SELF).ru_maxrss * 1024 <= 512 * MIB,
             "metadata-only stage exceeded its finite 900-second/512-MiB bound")
        current = common.cgroup_state(CG)
        common.cgroup_preserved(before_cgroup, current)
        need(all(row["events"].get(key, 0) == 0 for row in current["ancestors"]
                 for key in ("oom", "oom_kill", "oom_group_kill")), "current OOM counters are not zero")
        for path in (B, M, *M.parents):
            space = os.statvfs(path)
            need(space.f_bavail * space.f_frsize >= 8 * GIB, "eight-GiB backing reserve exhausted")
        return current

    guard()
    source_before = common.source_state(SOURCE, MANIFEST)
    current_inputs = {}
    for role, path, size, digest in (("seed_config", SEED, 18803, SEED_SHA),
                                    ("original_batch", ORIGINAL, 3911428, ORIGINAL_SHA),
                                    ("python", PYTHON, 7481192, PYTHON_SHA)):
        pin = common.opened(path, 64 * MIB)
        need(pin["bytes"] == size and pin["sha256"] == digest, "actual current selected input differs: " + role)
        current_inputs[role] = pin
        bootstrap.append(pin)
    need(Path(sys.executable).resolve(strict=True) == PYTHON and B.resolve(strict=True) == B and
         not CODE.exists() and not CODE.is_symlink() and not RECEIPT.exists() and not RECEIPT.is_symlink() and
         all(not path.exists() and not path.is_symlink() for path in (META, ACTOR, OWNER)),
         "actual selected interpreter and fresh code/phase outputs required")
    report = {"format_version": 1, "kind": "typed-command-postboot-source-stage", "status": "RUNNING",
        "source": str(SOURCE), "source_manifest_sha256": MANIFEST_SHA, "source_before": source_before,
        "cgroup_before": before_cgroup, "source_payload": payload_pin, "current_inputs": current_inputs,
        "commands": [], "actors_started": 0, "compiler_executed": False, "Runtime_executed": False,
        "Guest_authority": "UNCHANGED_EMPTY", "historical_batch_status": "FAIL",
        "historical_reports_rewritten": False, "staged_files": []}
    common.atomic_json(RECEIPT, report)
    try:
        CODE.mkdir(mode=0o700)
        directory = os.open(CODE, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        try:
            def put(name, raw):
                need("/" not in name and name not in (".", ".."), "one confined staged filename required")
                descriptor = os.open(name, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, 0o600, dir_fd=directory)
                try:
                    offset = 0
                    while offset < len(raw):
                        guard()
                        count = os.write(descriptor, raw[offset:])
                        need(count > 0, "short staged source write")
                        offset += count
                    os.fsync(descriptor)
                finally:
                    os.close(descriptor)
                pin = common.opened(CODE / name, 2 * MIB)
                need(pin["bytes"] == len(raw) and pin["sha256"] == hashlib.sha256(raw).hexdigest(), "staged bytes differ")
                report["staged_files"].append(pin)
                return pin

            helpers = {role: thin(put(NAMES[role], decoded[role])) for role in NAMES}
            helper_selection = {"format_version": 1, "kind": "typed-command-postboot-helper-selection",
                                "helpers": {role: value for role, value in helpers.items() if role != "outer"}}
            selection_pin = put("helper-selection.json", encoded(helper_selection))
            control = {"format_version": 1, "kind": "typed-command-postboot-root-control-v1", "source": str(SOURCE),
                "manifest": str(MANIFEST), "manifest_sha256": MANIFEST_SHA, "manifest_pin": manifest_pin,
                "source_files": 17928, "cgroup": CG, "code_root": str(CODE), "metadata_out": str(META),
                "actor_out": str(ACTOR), "owner_out": str(OWNER), "python": current_inputs["python"],
                "helpers": helpers, "helper_selection": thin(selection_pin),
                "inputs": {role: thin(current_inputs[role]) for role in ("seed_config", "original_batch")}, "outer": OUTER}
            control_pin = put("root-control.json", encoded(control))
            os.fsync(directory)
        finally:
            os.close(directory)
        parent = os.open(B, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        try:
            os.fsync(parent)
        finally:
            os.close(parent)
        need(set(CODE.iterdir()) == {CODE / name for name in [*NAMES.values(), "helper-selection.json", "root-control.json"]},
             "sealed source package has unexpected files")
        need(all(common.opened(Path(pin["path"]), max(64 * MIB, pin["bytes"])) == pin for pin in bootstrap) and
             all(common.opened(Path(pin["path"]), 2 * MIB) == pin for pin in report["staged_files"]),
             "source payload/input/staged current pins changed")
        source_after = common.source_state(SOURCE, MANIFEST)
        need(source_after == source_before, "current full frozen source changed during staging")
        report.update(source_after=source_after, source_preserved=True, inputs_preserved=True, staged_files_preserved=True,
                      cgroup_after=guard(), control=control_pin, helper_selection=selection_pin,
                      status="PASS_SOURCE_STAGE_ONLY_NO_PHASE_EXECUTED")
        report["unexecuted_phase_commands"] = {phase: [str(PYTHON), "-B", str(CODE / "root-launch.py"),
            "--phase", phase, "--control", str(CODE / "root-control.json"), "--control-sha256", control_pin["sha256"]]
            for phase in ("prepare", "build")}
    except BaseException as error:
        report.update(status="FAIL_SOURCE_STAGE", error=repr(error))
        common.atomic_json(RECEIPT, report)
        raise
    common.atomic_json(RECEIPT, report)
    print(json.dumps({"status": report["status"], "stage_receipt": common.opened(RECEIPT),
                      "control": report["control"], "phases_executed": False}, sort_keys=True))


if __name__ == "__main__":
    main()
