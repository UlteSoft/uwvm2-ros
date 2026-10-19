#!/usr/bin/env python3
"""Small, bounded primitives shared by the independent ROS distribution lane.

This lane does not change Guest permissions and does not qualify the historical
AArch64 acceptance lane. Every command is an owned, bounded child; a declared
profile, an available archive, and a successful configuration are not tests.
Only regular opened bytes are hashed. Large SDK catalogs are JSON Lines files,
so hashing a multi-gigabyte sysroot does not retain its file bodies in memory.
"""
from __future__ import annotations

import errno
import hashlib
import json
import os
from pathlib import Path
import select
import signal
import stat
import subprocess
import sys
import time

sys.dont_write_bytecode = True
CHUNK = 1024 * 1024
JSON_LIMIT = 16 * CHUNK
FILE_LIMIT = 2 * 1024 * CHUNK
_CGROUP_PROBE = None
EXCLUDED = frozenset({".git", "__pycache__", "node_modules", "target", ".cache"})


def install_interrupts():
    """Turn a first interactive/CI cancellation into checked owned cleanup."""
    state = {"cancelled": False}
    def interrupted(signum, _frame):
        state["cancelled"] = True
        signal.signal(signum, signal.SIG_IGN)  # Do not interrupt the cleanup pass.
        raise InterruptedError("distribution lane cancelled by signal " + str(signum))
    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    return state


def require(condition, message):
    if not condition:
        raise ValueError(message)


def opened(path, limit=FILE_LIMIT):
    """Return a streaming byte pin; reject special files and in-flight changes."""
    path = Path(path)
    require(path.is_absolute(), "absolute file path required: " + str(path))
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= limit,
                "bounded regular file required: " + str(path))
        digest = hashlib.sha256()
        size = 0
        while True:
            data = os.read(fd, CHUNK)
            if not data:
                break
            size += len(data)
            require(size <= limit, "file grew beyond bound")
            digest.update(data)
        after = os.fstat(fd)
        now = path.stat(follow_symlinks=False)
        keys = ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")
        require(size == before.st_size and all(getattr(before, k) == getattr(after, k)
                == getattr(now, k) for k in keys), "opened file changed: " + str(path))
        return {"path": str(path), "sha256": digest.hexdigest(), "bytes": size,
                "device": before.st_dev, "inode": before.st_ino,
                "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
    finally:
        os.close(fd)


def body_pin(path, limit=JSON_LIMIT):
    """Read one small document through the same descriptor used for its digest."""
    path = Path(path)
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and before.st_size <= limit,
                "bounded regular metadata required")
        chunks = []
        count = 0
        while True:
            part = os.read(fd, min(65536, limit + 1 - count))
            if not part:
                break
            chunks.append(part)
            count += len(part)
            require(count <= limit, "metadata limit exceeded")
        data = b"".join(chunks)
        after = os.fstat(fd)
        now = path.stat(follow_symlinks=False)
        keys = ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")
        require(count == before.st_size and all(getattr(before, k) == getattr(after, k)
                == getattr(now, k) for k in keys), "metadata changed while opening")
        pin = {"path": str(path), "sha256": hashlib.sha256(data).hexdigest(),
               "bytes": count, "device": before.st_dev, "inode": before.st_ino,
               "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
        return data, pin
    finally:
        os.close(fd)


def document(path):
    body, pin = body_pin(path)
    def pairs(items):
        result = {}
        for key, value in items:
            require(key not in result, "duplicate JSON key: " + key)
            result[key] = value
        return result
    return json.loads(body, object_pairs_hook=pairs), pin


def atomic_json(path, value):
    """Publish reports atomically, retaining the last complete state on ENOSPC."""
    path = Path(path)
    temporary = path.with_name(path.name + ".pending")
    with temporary.open("x", encoding="utf-8") as output:
        json.dump(value, output, indent=2, sort_keys=True)
        output.write("\n")
        output.flush()
        os.fsync(output.fileno())
    os.replace(temporary, path)


def catalog(root, output, allowed_roots=None):
    """Hash every SDK/tool-root file; retain all relative symlink identities.

    Symlinks are inventoried rather than traversed. Directory targets must stay
    inside an explicitly inventoried root. External regular file targets (for
    example GCC's libstdc++.so link to its sibling system-library directory)
    are separately opened and physically pinned, never silently ignored.
    An unused dangling link records its stable link identity and ENOENT state
    only. It establishes no target bytes and cannot qualify a selected input.
    """
    root = Path(root).resolve(strict=True)
    allowed_roots = [Path(value).resolve(strict=True) for value in (allowed_roots or [root])]
    require(root.is_dir(), "inventory root must be a directory")
    count = size = links = 0
    digest = hashlib.sha256()
    with Path(output).open("xb") as stream:
        for directory, dirs, names in os.walk(root, followlinks=False):
            dirs.sort()
            names.sort()
            for name in sorted(dirs + names):
                path = Path(directory) / name
                if path.is_symlink():
                    before = path.lstat()
                    target = os.readlink(path)
                    try:
                        resolved = path.resolve(strict=True)
                    except OSError as error:
                        if error.errno != errno.ENOENT:
                            raise
                        resolved = None
                    row = {"relative": str(path.relative_to(root)), "kind": "symlink",
                           "target": target, "resolved": str(resolved) if resolved is not None else None,
                           "device": before.st_dev,
                           "inode": before.st_ino, "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
                    if resolved is None:
                        # Whole OS inventories can contain unused dangling
                        # links. Record only their actual link identity; no
                        # target bytes/closure or usable input is established.
                        row["resolution_state"] = "MISSING_TARGET_ENOENT"
                        try:
                            path.resolve(strict=True)
                        except OSError as error:
                            if error.errno != errno.ENOENT:
                                raise
                        else:
                            require(False, "dangling link target appeared during inventory: " + str(path))
                    else:
                        contained = any(resolved == selected or selected in resolved.parents for selected in allowed_roots)
                        require(not resolved.is_dir() or contained,
                                "directory symlink escapes all explicit inventory roots: " + str(path))
                        if not resolved.is_dir():
                            row["target_file"] = opened(resolved)
                    after = path.lstat()
                    require(all(getattr(before, k) == getattr(after, k) for k in
                                ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode")) and
                            os.readlink(path) == target,
                            "symlink changed during inventory")
                    links += 1
                elif path.is_dir():
                    continue
                else:
                    pin = opened(path)
                    row = {"relative": str(path.relative_to(root)), "kind": "file",
                           "sha256": pin["sha256"], "bytes": pin["bytes"],
                           "device": pin["device"], "inode": pin["inode"],
                           "mtime_ns": pin["mtime_ns"], "ctime_ns": pin["ctime_ns"],
                           "executable": bool(path.stat().st_mode & 0o111)}
                    count += 1
                    size += pin["bytes"]
                line = (json.dumps(row, sort_keys=True, separators=(",", ":")) + "\n").encode()
                stream.write(line)
                digest.update(line)
        stream.flush()
        os.fsync(stream.fileno())
    return {"root": str(root), "files": count, "bytes": size, "symlinks": links,
            "content_sha256": digest.hexdigest(), "catalog": opened(Path(output))}


def source_state(source, manifest):
    """Verify the complete frozen manifest, including the new tools via --extra."""
    entries, manifest_pin = document(manifest)
    require(isinstance(entries, dict) and entries, "nonempty frozen file manifest required")
    required = {"tools/ros_distro/common.py", "tools/ros_distro/build.py",
                "tools/ros_distro/probe.py", "tools/ros_distro/profile.py",
                "tests/ros_distro/native_basic.cc", "tests/ros_distro/CMakeLists.txt",
                "tests/ros_cgroup/probe.py"}
    require(required <= entries.keys(), "freeze must include tools/ros_distro and native fixtures")
    names = set()
    for path in Path(source).rglob("*"):
        require(not path.is_symlink(), "frozen source must not contain symlink inputs")
        if path.is_file():
            names.add(str(path.relative_to(source)))
    require(names == set(entries), "frozen source file set differs from its complete manifest")
    for name, expected in sorted(entries.items()):
        relative = Path(name)
        require(not relative.is_absolute() and ".." not in relative.parts,
                "unsafe frozen source entry")
        actual = opened(Path(source) / relative)
        require(actual["sha256"] == expected["sha256"] and actual["bytes"] == expected["bytes"],
                "frozen source changed: " + name)
    return {"root": str(source), "manifest": manifest_pin, "files": len(entries)}


def _procfs_exited(error):
    """Only kernel absence/exit errno values describe a disappearing proc entry.

    A task can exit between directory iteration and a children/status/cgroup
    read, producing either ENOENT or ESRCH. Permission and I/O failures remain
    fatal; this predicate must not surround ownership or identity assertions.
    """
    return error.errno in (errno.ENOENT, errno.ESRCH)


def identity(pid):
    """Linux PID identity: PID/starttime is the birth, PPID is observed ancestry."""
    try:
        body = (Path("/proc") / str(pid) / "stat").read_text()
        tail = body[body.rindex(")") + 2:].split()
        return {"pid": int(pid), "parent_pid": int(tail[1]),
                "starttime": int(tail[19]), "state": tail[0]}
    except OSError as error:
        if not _procfs_exited(error):
            raise
        return None


def birth(item):
    return None if item is None else (item["pid"], item["starttime"])


def pidfd_terminal(fd):
    """Read Linux pidfd completion without select's fixed FD_SETSIZE ceiling.

    A long serial matrix can retain more than1024 terminal ownership handles.
    poll accepts those descriptor numbers while retaining the same kernel
    process-birth binding. An invalid descriptor or poll error is a failure,
    never evidence that the owned process exited.
    """
    poller = select.poll()
    poller.register(fd, select.POLLIN)
    events = poller.poll(0)
    for selected, mask in events:
        require(selected == fd and not mask & (select.POLLNVAL | select.POLLERR),
                "invalid owned pidfd completion observation")
        if mask & (select.POLLIN | select.POLLHUP):
            return True
    return False


def cgroup_state(runtime):
    """Reuse the frozen lane's authoritative actual-leaf/controller-root probe.

    The profile must pin the original supplied leaf, not merely another cgroup
    with the same memory.max. Loading this data-only helper is lazy; the driver
    later binds its bytes through the complete frozen source manifest.
    """
    import importlib.util
    global _CGROUP_PROBE
    if _CGROUP_PROBE is None:
        path = Path(__file__).resolve().parents[2] / "tests/ros_cgroup/probe.py"
        spec = importlib.util.spec_from_file_location("uwvm_distribution_current_cgroup", path)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        _CGROUP_PROBE = module
    result = _CGROUP_PROBE.probe(expected_memory_max=runtime["memory_max"],
        expected_leaf=runtime["expected_leaf"], expected_device=runtime["expected_device"],
        expected_inode=runtime["expected_inode"], expected_boot=runtime["expected_boot_id"])
    require(result.get("status") == "PASS", "original cgroup admission failed: " + str(result.get("reason")))
    incarnation = result["cgroup_incarnation"]
    records = result["cgroup_ancestors"]
    cpus = records[0]["cpuset.cpus.effective"]
    require(cpus == runtime["expected_cpus"], "original cgroup effective CPUs changed")
    current = int(records[0]["memory.current"])
    current_limit = runtime["memory_max"] - 4 * 1024**3
    require(current <= current_limit, "original cgroup cannot retain its explicit four-GiB reserve")
    ancestors = []
    for row in records:
        if "controllerless_hierarchy_root" in row:
            ancestors.append({"path": row["path"], "events": {},
                              "controllerless_hierarchy_root": row["controllerless_hierarchy_root"]})
            continue
        events = dict((key, int(value)) for key, value in
                      (line.split() for line in row["memory.events"].splitlines()))
        ancestors.append({"path": row["path"], "events": events,
                          "memory_max": row["memory.max"], "memory_swap_max": row["memory.swap.max"],
                          "cpu_max": row["cpu.max"], "pids_max": row["pids.max"]})
    return {"boot_id": incarnation["boot_id"], "membership": result["membership"],
            "path": incarnation["path"], "device": incarnation["device"], "inode": incarnation["inode"],
            "memory_max": runtime["memory_max"], "memory_swap_max": 0,
            "memory_current": current, "memory_current_guard": current_limit,
            "cpus": cpus, "ancestors": ancestors}


def cgroup_preserved(before, after):
    varying = {"ancestors", "memory_current"}
    require({k: v for k, v in before.items() if k not in varying} ==
            {k: v for k, v in after.items() if k not in varying}, "cgroup incarnation changed")
    require([r["path"] for r in before["ancestors"]] == [r["path"] for r in after["ancestors"]],
            "cgroup ancestry changed")
    for left, right in zip(before["ancestors"], after["ancestors"]):
        require({k: v for k, v in left.items() if k != "events"} ==
                {k: v for k, v in right.items() if k != "events"}, "ancestor controller identity/limits changed")
        for name in ("oom", "oom_kill", "oom_group_kill"):
            require(left["events"].get(name, 0) == right["events"].get(name, 0),
                    "ancestor OOM counter changed")
    return True


class SerialRunner:
    """Run one owned command at a time with bounded logs, RSS and wall time.

    Every Popen is retained before pidfd admission can fail. Descendants are
    admitted only from an already owned parent and a stable birth. Reparenting
    after that observation does not manufacture a new birth. Signals use only
    retained pidfds. A live process whose ownership cannot be proved produces
    CLEANUP_INCOMPLETE; it is never signalled by a numeric PID or process group.
    """
    def __init__(self, out, limits, guard=lambda: None):
        require(sys.platform == "linux" and hasattr(os, "pidfd_open") and
                hasattr(signal, "pidfd_send_signal"), "Linux pidfd support required")
        self.out = Path(out)
        self.limits = limits
        self.commands = []
        self.owned = {}
        self.children = []
        self.roots = {}
        self.unproven = {}
        self.cleanup = {"status": "NOT_PERFORMED", "remaining": []}
        self.guard = guard

    def acquire(self, pid, parent):
        first = identity(pid)
        if first is None:
            return
        key = birth(first)
        if key in self.owned:
            return
        if first["parent_pid"] != parent:
            self.unproven[key] = first
            return
        try:
            fd = os.pidfd_open(pid)
        except ProcessLookupError:
            return
        try:
            second = identity(pid)
            if second is None:
                return
            require(birth(first) == birth(second), "process birth changed while opening pidfd")
            self.owned[key] = {"identity": first, "identity_after": second, "fd": fd}
            self.unproven.pop(key, None)
            fd = None
            try:
                membership = (Path("/proc") / str(pid) / "cgroup").read_text()
            except OSError as error:
                if not _procfs_exited(error):
                    raise
                # Keep the already registered pidfd for terminal cleanup.
            else:
                # Failure to read the coordinator's own stable membership is
                # not evidence that the owned child exited. Never hide it.
                require(membership == Path("/proc/self/cgroup").read_text(),
                        "owned child escaped the coordinator cgroup")
        finally:
            if fd is not None:
                os.close(fd)

    def discover(self):
        for item in list(self.owned.values()):
            pid = item["identity"]["pid"]
            if birth(identity(pid)) != birth(item["identity"]):
                continue
            try:
                # Materialize only this process's ephemeral task directory so
                # lazy iterdir failures are caught at the directory operation.
                tasks = list((Path("/proc") / str(pid) / "task").iterdir())
            except OSError as error:
                if not _procfs_exited(error):
                    raise
                continue
            for task in tasks:
                try:
                    children = (task / "children").read_text().split()
                except OSError as error:
                    if not _procfs_exited(error):
                        raise
                    continue
                for child in children:
                    # Ownership/cgroup/pidfd failures are outside the procfs
                    # read handler, even if one happens to carry an exit errno.
                    self.acquire(int(child), pid)

    def rss(self):
        total = 0
        for item in self.owned.values():
            original = item["identity"]
            if birth(identity(original["pid"])) != birth(original):
                continue
            try:
                lines = (Path("/proc") / str(original["pid"]) / "status").read_text().splitlines()
            except OSError as error:
                if not _procfs_exited(error):
                    raise
                continue
            for line in lines:
                if line.startswith("VmRSS:"):
                    total += int(line.split()[1]) * 1024
                    break
        return total

    def close(self):
        errors = []
        for child in self.children:
            if child.poll() is not None:
                continue
            original = self.roots.get(child.pid)
            current = identity(child.pid)
            try:
                require(current is not None and current["parent_pid"] == os.getpid() and
                        (original is None or birth(original) == birth(current)),
                        "retained Popen recovery cannot establish its original direct birth")
                self.acquire(child.pid, os.getpid())
            except BaseException as error:
                errors.append(repr(error))
        try:
            self.discover()
        except BaseException as error:
            errors.append(repr(error))
        for sig, duration in ((signal.SIGTERM, 5), (signal.SIGKILL, 5)):
            for item in reversed(list(self.owned.values())):
                try:
                    signal.pidfd_send_signal(item["fd"], sig)
                except ProcessLookupError:
                    pass
                except OSError as error:
                    errors.append(repr(error))
            until = time.monotonic() + duration
            while time.monotonic() < until:
                try:
                    self.discover()
                except BaseException as error:
                    errors.append(repr(error))
                for item in self.owned.values():
                    try:
                        signal.pidfd_send_signal(item["fd"], sig)
                    except ProcessLookupError:
                        pass
                    except OSError as error:
                        errors.append(repr(error))
                for child in self.children:
                    child.poll()
                if all(pidfd_terminal(item["fd"]) for item in self.owned.values()):
                    break
                time.sleep(0.02)
        remaining = []
        for key, item in self.owned.items():
            if not pidfd_terminal(item["fd"]):
                remaining.append(item["identity"])
            os.close(item["fd"])
        self.owned.clear()
        for key, item in self.unproven.items():
            now = identity(item["pid"])
            if birth(now) == key and now["state"] != "Z":
                remaining.append({"unproven": item, "current": now})
        for child in self.children:
            if child.poll() is None:
                remaining.append({"retained_popen": child.pid})
        self.cleanup = {"status": "PASS" if not remaining and not errors else "CLEANUP_INCOMPLETE",
                        "remaining": remaining}
        if errors:
            self.cleanup["errors"] = errors[:64]
        return self.cleanup

    def run(self, label, argv, cwd, environment, expected=0):
        require(all(isinstance(x, str) and "\0" not in x for x in argv), "argv must contain strings")
        require(Path(argv[0]).is_absolute(), "explicit executable required")
        self.guard()
        path = self.out / (label + ".log")
        row = {"label": label, "argv": argv, "cwd": str(cwd), "status": "RUNNING",
               "expected_returncode": expected, "environment": environment,
               "started_monotonic_ns": time.monotonic_ns(), "peak_owned_rss_bytes": 0}
        self.commands.append(row)
        atomic_json(self.out / "commands.json", self.commands)
        try:
            with path.open("xb") as log:
                child = subprocess.Popen(argv, cwd=cwd, env=environment, stdin=subprocess.DEVNULL,
                                         stdout=log, stderr=subprocess.STDOUT)
                self.children.append(child)
                row["pid"] = child.pid
                row["first_identity"] = identity(child.pid)
                self.roots[child.pid] = row["first_identity"]
                self.acquire(child.pid, os.getpid())
                require(any(key[0] == child.pid for key in self.owned) or child.poll() is not None,
                        "live direct child ownership not proven")
                deadline = time.monotonic() + self.limits["command_seconds"]
                while child.poll() is None:
                    self.guard()
                    self.discover()
                    rss = self.rss()
                    row["peak_owned_rss_bytes"] = max(rss, row["peak_owned_rss_bytes"])
                    require(rss <= self.limits["rss_bytes"], "owned command RSS limit exceeded")
                    require(log.tell() <= self.limits["log_bytes"], "command log limit exceeded")
                    require(time.monotonic() < deadline, "command deadline exceeded")
                    time.sleep(0.02)
                row["returncode"] = child.returncode
                self.guard()
                require(log.tell() <= self.limits["log_bytes"], "completed command log limit exceeded")
                require(child.returncode == expected, "command returned unexpected status: " + label)
                row["status"] = "PASS"
        except BaseException as error:
            row["status"] = "FAIL"
            row["error"] = repr(error)
            raise
        finally:
            row["finished_monotonic_ns"] = time.monotonic_ns()
            if path.exists():
                row["log"] = opened(path, self.limits["log_bytes"])
            atomic_json(self.out / "commands.json", self.commands)
        return row
