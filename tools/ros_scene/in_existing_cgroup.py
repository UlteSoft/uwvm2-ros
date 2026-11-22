#!/usr/bin/env python3
"""Run developers' commands under an existing 64 GiB, zero-swap cgroup v2 budget."""
import json
import os
from pathlib import Path
import subprocess
import sys
import time


CGROUPS = Path("/sys/fs/cgroup")
PROJECT = Path(__file__).resolve().parents[2]
BUDGET = "68719476736"
SLICE = "wrosvendor.slice"
CONTAINER = "uwvm-sdk-resource-gate-20261024"
IDENTITY = "af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913"


def membership():
    rows = Path("/proc/self/cgroup").read_text().splitlines()
    path, = (row[4:] for row in rows if row.startswith("0::/"))
    return below_cgroup_root(path)


def below_cgroup_root(relative):
    path = Path(relative)
    if path.is_absolute() or ".." in path.parts:
        raise ValueError("invalid unified cgroup path")
    return CGROUPS / path


def bounded(group):
    try:
        return ((group / "memory.max").read_text().strip() == BUDGET
                and (group / "memory.swap.max").read_text().strip() == "0")
    except FileNotFoundError:
        return False


def execute(group, command):
    if membership() != group or not bounded(group):
        raise SystemExit("64 GiB, zero-swap cgroup membership required")
    project_tmp = PROJECT / ".tmp"
    if project_tmp.is_symlink():
        raise SystemExit("project scratch directory must be on the project disk")
    scratch = project_tmp / "ros-scene-command-tmp"
    if scratch.is_symlink():
        raise SystemExit("invalid scratch directory")
    anchor = next(path for path in (scratch, project_tmp, PROJECT) if path.exists())
    filesystem = subprocess.check_output(
        ["findmnt", "--noheadings", "--output", "FSTYPE", "--target", anchor], text=True
    ).strip()
    if not filesystem or filesystem in {"tmpfs", "ramfs", "devtmpfs", "hugetlbfs"}:
        raise SystemExit("project scratch directory requires a disk filesystem")
    scratch.mkdir(mode=0o700, parents=True, exist_ok=True)
    if scratch.resolve() != scratch:
        raise SystemExit("invalid project scratch location")
    for key in ("TMPDIR", "TMP", "TEMP"):
        os.environ[key] = str(scratch)
    os.environ["PYTHONDONTWRITEBYTECODE"] = "1"
    print(f"EXISTING_CGROUP {group} memory.max={BUDGET} swap.max=0 tmpdir={scratch}",
          flush=True)
    os.execvp(command[0], command)


def legacy_gate(command):
    """Retain the pinned Docker keeper for installations which use it."""
    info = json.loads(subprocess.check_output(["docker", "inspect", CONTAINER], text=True))[0]
    group = CGROUPS / "system.slice" / ("docker-" + IDENTITY + ".scope")
    if info["Id"] != IDENTITY or not info["State"]["Running"] or not bounded(group):
        raise SystemExit("existing Docker test gate unavailable")
    child = subprocess.Popen([sys.executable, __file__, "--wait-enrollment",
                              str(group), str(group.stat().st_ino), *command])
    try:
        subprocess.run([
            "docker", "run", "--rm", "--network=none", "--read-only", "--pid=host",
            "--cgroupns=host", "--cap-drop=ALL", "--cap-add=DAC_OVERRIDE", "--mount",
            "type=bind,src=" + str(group / "cgroup.procs") + ",dst=/enroll", info["Config"]["Image"],
            "/bin/sh", "-c", 'printf "%s\\n" "$1" > /enroll', "enroll", str(child.pid),
        ], check=True, timeout=15)
        return child.wait()
    finally:
        if child.poll() is None:
            child.terminate()
            child.wait()


def main(command):
    if not command:
        raise SystemExit("Usage: in_existing_cgroup.py COMMAND [ARG ...]")
    current = membership()
    if command[0] == "--checked-scope":
        parent = Path(command[1])
        if current.parent != parent or parent.name != SLICE or not bounded(parent):
            raise SystemExit("scope is outside the shared test budget")
        execute(current, command[2:])
    if command[0] == "--wait-enrollment":
        group = Path(command[1])
        deadline = time.monotonic() + 15
        while membership() != group:
            if time.monotonic() >= deadline:
                raise SystemExit("timed out waiting for the test gate")
            time.sleep(0.02)
        if group.stat().st_ino != int(command[2]):
            raise SystemExit("test gate was replaced")
        execute(group, command[3:])
    if bounded(current):
        execute(current, command)
    try:
        control = subprocess.check_output(
            ["systemctl", "--user", "show", SLICE, "--property=ControlGroup", "--value"],
            text=True, stderr=subprocess.DEVNULL,
        ).strip()
    except (OSError, subprocess.CalledProcessError):
        control = ""
    if control.startswith("/") and Path(control).name == SLICE:
        parent = below_cgroup_root(control[1:])
        if bounded(parent):
            os.execvp("systemd-run", ["systemd-run", "--user", "--scope", "--quiet",
                      "--slice=" + SLICE, "-p", "MemoryMax=64G", "-p", "MemorySwapMax=0",
                      "--", sys.executable, __file__, "--checked-scope", str(parent), *command])
    return legacy_gate(command)


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
