#!/usr/bin/env python3
"""Hold the reviewed global lease and monitor the original Linux cgroup."""
from __future__ import annotations

import argparse
import fcntl
import importlib.util
import json
import os
from pathlib import Path
import signal
import shutil
import stat
import subprocess
import sys
import time

from receipt import kill_owned, save, sha


def descendants(parent):
    facts = {}
    for path in Path("/proc").iterdir():
        if not path.name.isdecimal():
            continue
        try:
            fields = (path / "stat").read_text().rsplit(") ", 1)[1].split()
            facts[int(path.name)] = (int(fields[1]), fields[19])
        except (OSError, ValueError, IndexError):
            continue
    selected = {parent}
    while True:
        found = {pid for pid, (ppid, _) in facts.items() if ppid in selected}
        if found <= selected:
            break
        selected |= found
    return {pid: facts[pid][1] for pid in selected if pid in facts}


def events(probe):
    return {row["path"]: row["memory.events"] for row in probe.get("cgroup_ancestors", [])
            if "controllerless_hierarchy_root" not in row}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--admission", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("argv", nargs=argparse.REMAINDER)
    args = parser.parse_args()
    if sys.platform != "linux":
        parser.error("full qualification requires Linux")
    argv = args.argv[1:] if args.argv[:1] == ["--"] else args.argv
    if not argv:
        parser.error("explicit owned command required")
    required = {"schema_version", "leaf", "device", "inode", "boot_id", "global_lock", "lock_device", "lock_inode", "machine",
                "provision_manifest", "provision_sha256"}
    args.out.parent.mkdir(parents=True, exist_ok=True)
    report = {"status": "BLOCKED", "argv": argv,
              "helper_sha256": sha(__file__), "maximum_jobs": 1, "membership_checks": 0}
    process = None
    lease = None
    before = None
    owned = {}
    probe = None

    def stop():
        if process is None:
            return
        if process.poll() is None:
            owned.update(descendants(process.pid))
        kill_owned(process, owned)

    def interrupted(signum, _frame):
        raise InterruptedError("qualification interrupted by signal " + str(signum))

    for sig in (signal.SIGINT, signal.SIGTERM):
        signal.signal(sig, interrupted)
    try:
        config = json.loads(args.admission.read_text())
        report["admission_sha256"] = sha(args.admission)
        if set(config) != required or config["schema_version"] != 1:
            raise ValueError("complete reviewed runner identity required; see runner-admission.example.json")
        if not hasattr(os, "pidfd_open") or not hasattr(signal, "pidfd_send_signal"):
            raise ValueError("owned cleanup requires Linux pidfd support")
        lock = Path(config["global_lock"])
        if not lock.is_absolute():
            raise ValueError("global lease must be an absolute existing regular file")
        lease = os.open(lock, os.O_RDWR | os.O_NOFOLLOW | os.O_NONBLOCK)
        state = os.fstat(lease)
        if (not stat.S_ISREG(state.st_mode) or state.st_uid != os.geteuid() or state.st_mode & 0o022
                or (state.st_dev, state.st_ino) != (config["lock_device"], config["lock_inode"])):
            raise ValueError("global lease identity/ownership/mode differs")
        fcntl.flock(lease, fcntl.LOCK_EX | fcntl.LOCK_NB)
        report["global_lock"] = {"path": str(lock), "device": state.st_dev, "inode": state.st_ino}
        provision_path = Path(config["provision_manifest"])
        if not provision_path.is_absolute() or sha(provision_path) != config["provision_sha256"]:
            raise ValueError("reviewed provisioning manifest differs")
        provision = json.loads(provision_path.read_text())
        if provision.get("schema_version") != 1 or not provision.get("files"):
            raise ValueError("reviewed provisioning inventory is empty or invalid")
        prefix = Path(provision["llvm_prefix"]).resolve(strict=True)
        xmake = Path(provision["xmake"]).resolve(strict=True)
        needed = {str(prefix / "bin" / name) for name in ("clang", "clang++", "ld.lld", "llvm-ar", "llvm-config")} | {str(xmake)}
        if not needed.issubset(provision["files"]):
            raise ValueError("provision inventory omits a required compiler/linker/archive/xmake actor")
        for filename, expected in provision["files"].items():
            if not Path(filename).is_absolute() or sha(filename) != expected:
                raise ValueError("provisioned input changed: " + filename)
        if os.environ.get("LLVM23_PREFIX") and Path(os.environ["LLVM23_PREFIX"]).resolve() != prefix:
            raise ValueError("selected LLVM prefix differs from the reviewed provisioning inventory")
        os.environ["PATH"] = str(xmake.parent) + os.pathsep + str(prefix / "bin") + os.pathsep + os.environ.get("PATH", "")
        actual_xmake = shutil.which("xmake")
        if actual_xmake is None or Path(actual_xmake).resolve() != xmake:
            raise ValueError("actual xmake lookup differs from provisioning inventory")
        os.environ["UWVM_QUALIFIED_LLVM_PREFIX"] = str(prefix)
        report["provision"] = {"path": str(provision_path), "sha256": config["provision_sha256"], "files": len(provision["files"])}
        if os.uname().machine != config["machine"]:
            raise ValueError("native runner architecture differs from its reviewed profile")
        helper = args.source / "test/0073.ros-tests/support/source/ros_cgroup/probe.py"
        spec = importlib.util.spec_from_file_location("qualification_cgroup", helper)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        report["cgroup_probe_sha256"] = sha(helper)
        def probe():
            return module.probe(expected_leaf=config["leaf"], expected_device=config["device"],
                                expected_inode=config["inode"], expected_boot=config["boot_id"])
        before = probe()
        report["before"] = before
        if before["status"] != "PASS":
            raise ValueError("original 64 GiB/zero-swap cgroup not admitted: " + str(before.get("reason")))
        if 68719476736 - int(before["cgroup_ancestors"][0]["memory.current"]) < 16 * 1024**3:
            raise ValueError("less than 16 GiB live headroom for a serial compiler and reserve")
        report["status"] = "RUNNING"
        save(args.out, report)
        process = subprocess.Popen(argv, start_new_session=True, stdin=subprocess.DEVNULL)
        while process.poll() is None:
            owned.update(descendants(process.pid))
            for pid, start in list(owned.items()):
                try:
                    current_start = Path(f"/proc/{pid}/stat").read_text().rsplit(") ", 1)[1].split()[19]
                    membership = Path(f"/proc/{pid}/cgroup").read_text().splitlines()
                except FileNotFoundError:
                    continue
                if current_start == start and membership != before["membership"]:
                    raise RuntimeError("owned process escaped the original cgroup: " + str(pid))
                report["membership_checks"] += 1
            current = probe()
            if current["status"] != "PASS" or events(current) != events(before):
                raise RuntimeError("cgroup identity/limits or ancestor memory events changed")
            if lock.stat(follow_symlinks=False).st_ino != state.st_ino or lock.stat(follow_symlinks=False).st_dev != state.st_dev:
                raise RuntimeError("global lease replaced during qualification")
            time.sleep(1)
        report["returncode"] = process.returncode
        report["status"] = "PASS" if process.returncode == 0 else "BLOCKED" if process.returncode == 77 else "FAIL"
        remaining = []
        for pid, start in owned.items():
            try:
                fields = Path(f"/proc/{pid}/stat").read_text().rsplit(") ", 1)[1].split()
                if fields[19] == start and fields[0] != "Z":
                    remaining.append(pid)
            except FileNotFoundError:
                pass
        if remaining:
            kill_owned(process, owned)
            report.update(status="FAIL", reason="owned actors remained after coordinator exit", cleaned_pids=remaining)
    except (OSError, ValueError, RuntimeError, InterruptedError, KeyError, TypeError) as error:
        report["reason"] = str(error)
        report["status"] = "FAIL" if process is not None else "BLOCKED"
        stop()
    finally:
        if before is not None and before["status"] == "PASS" and probe is not None:
            try:
                after = probe()
                report["after"] = after
                if after["status"] != "PASS" or events(before) != events(after):
                    report.update(status="FAIL", final_reason="cgroup final identity/limits/events differ")
            except (OSError, ValueError) as error:
                report.update(status="FAIL", final_reason=str(error))
        if lease is not None:
            os.close(lease)
        save(args.out, report)
    print("resource guard: " + report["status"], flush=True)
    return 0 if report["status"] == "PASS" else 77 if report["status"] == "BLOCKED" else 1


if __name__ == "__main__":
    raise SystemExit(main())
