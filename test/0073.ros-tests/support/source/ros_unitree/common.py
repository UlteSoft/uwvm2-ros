#!/usr/bin/env python3
"""Bounded, source-pinned Linux primitives for the Unitree integration lane.

Importing this module launches no process. Root supplies every executable,
upstream tree, and package. The coordinator must already inhabit the selected
64 GiB cgroup; this code never creates, moves to, or weakens a cgroup. Guest
capabilities are properties of the actual Runtime command, never this harness.
"""
from __future__ import annotations

import argparse
import errno
import hashlib
import json
import math
import os
from pathlib import Path
import re
import select
import signal
import stat
import subprocess
import sys
import time
import types

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[5]
MAX_JSON = 16 * 1024**2
MAX_LINE = 1024**2


def require(ok, reason):
    if not ok:
        raise ValueError(reason)


def exact(value, keys, label):
    require(type(value) is dict and set(value) == set(keys), "closed " + label + " required")


def current_loopback(expected_namespace):
    """Prove interfaces in the caller's actual Linux network namespace.

    A sysfs mount inherited before unshare may still show the parent's network
    devices. Kernel socket enumeration and a read-only SIOCGIFFLAGS ioctl use
    the socket's CURRENT namespace instead. This trusted native test primitive
    sends no packet and changes no interface. Its short-lived descriptor is
    closed before returning; it grants no raw-network permission to any Guest.
    """
    import fcntl
    import socket
    import struct
    require(sys.platform == "linux" and type(expected_namespace) is str and
            re.fullmatch(r"net:\[[1-9][0-9]*\]", expected_namespace) and
            os.readlink("/proc/self/ns/net") == expected_namespace,
            "exact live selected Linux network namespace required")
    interfaces = socket.if_nameindex()
    require(len(interfaces) == 1 and interfaces[0][1] == "lo" and interfaces[0][0] > 0,
            "actual current namespace must contain only loopback")
    # Linux ifreq has16 name bytes and at most24 union bytes on the admitted
    # ordinary architectures. Flags are the native unsigned short at offset16;
    # the complete40-byte request avoids a host-width-dependent short buffer.
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as descriptor:
        request = struct.pack("16s24x", b"lo")
        actual = fcntl.ioctl(descriptor.fileno(), 0x8913, request)  # SIOCGIFFLAGS, read only.
        require(len(actual) == len(request), "complete actual Linux ifreq required")
        flags = struct.unpack_from("H", actual, 16)[0]
    require(flags & 0x9 == 0x9 and socket.if_nameindex() == interfaces and
            os.readlink("/proc/self/ns/net") == expected_namespace,
            "actual loopback must remain UP/LOOPBACK in the same selected namespace")
    return {"network_namespace": expected_namespace, "interfaces": [{"index": interfaces[0][0],
        "name": "lo", "flags": flags}], "method": "CURRENT_NETNS_IF_NAMEINDEX_AND_READONLY_SIOCGIFFLAGS",
        "packets_sent": 0, "interface_settings_changed": False, "Guest_network_rights_granted": False}


def pairs(items):
    result = {}
    for key, value in items:
        require(key not in result, "duplicate JSON member: " + key)
        result[key] = value
    return result


def decode(raw):
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda value: require(False, "nonfinite JSON: " + value))


def read_json(path):
    """Read stable bounded regular bytes before any helper is imported."""
    path = Path(path)
    require(path.is_absolute(), "absolute metadata path required")
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= MAX_JSON,
                "bounded nonempty regular JSON required")
        body = bytearray()
        while True:
            part = os.read(fd, 65536)
            if not part:
                break
            body.extend(part)
            require(len(body) <= MAX_JSON, "metadata grew beyond bound")
        after = os.fstat(fd)
        named = path.stat(follow_symlinks=False)
        require(all(getattr(before, key) == getattr(after, key) == getattr(named, key)
                    for key in ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")),
                "metadata changed while opened")
        return decode(body), {"path": str(path), "sha256": hashlib.sha256(body).hexdigest(),
            "bytes": len(body), "device": before.st_dev, "inode": before.st_ino,
            "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
    finally:
        os.close(fd)


def safe_path(value):
    require(type(value) is str and Path(value).is_absolute() and
            not any(c in value for c in ("\0", "\n", "\r")), "absolute clean path required")
    return Path(value)


def load_common(source, frozen):
    """Execute only the reviewed helper bytes admitted by this full freeze."""
    # cgroup_state lazily executes the shared probe. Bind it before any call
    # can reach that loader, including the coordinator's initial admission.
    probe = source / "test/0073.ros-tests/support/source/ros_cgroup/probe.py"
    probe_raw = probe.read_bytes()
    require(0 < len(probe_raw) <= 1024**2 and frozen.get("test/0073.ros-tests/support/source/ros_cgroup/probe.py") ==
            {"bytes": len(probe_raw), "sha256": hashlib.sha256(probe_raw).hexdigest()},
            "cgroup probe absent from complete freeze")
    path = source / "tools/ros_distro/common.py"
    raw = path.read_bytes()
    require(0 < len(raw) <= 1024**2 and frozen.get("tools/ros_distro/common.py") ==
            {"bytes": len(raw), "sha256": hashlib.sha256(raw).hexdigest()}, "shared helper absent from freeze")
    module = types.ModuleType("uwvm_unitree_owned_primitives")
    module.__file__ = str(path)
    sys.modules[module.__name__] = module
    exec(compile(raw, str(path), "exec"), module.__dict__)
    return module


def arguments(description):
    parser = argparse.ArgumentParser(description=description)
    for name in ("source", "manifest", "config", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    for name in ("manifest-sha256", "config-sha256"):
        parser.add_argument("--" + name, required=True)
    return parser


def finite(value, low, high, label):
    require(type(value) in (int, float) and math.isfinite(value) and low <= value <= high,
            "finite bounded " + label + " required")
    return float(value)


class Frame:
    """One fresh output, one physical input closure, one actual cgroup identity.

    Input roots are read-only by contract, inventoried before/after. Child
    processes receive an explicit environment. No PATH search, shell command,
    package download, service manager, container daemon, or global sysctl is
    used by this lane. Its source must be the invoked complete frozen source.
    """
    def __init__(self, args, kind):
        require(sys.platform == "linux", "actual Linux lane required")
        self.source = args.source.resolve(strict=True)
        require(self.source == ROOT, "invoke the actual frozen Unitree entrypoint")
        self.manifest = args.manifest.resolve(strict=True)
        frozen, manifest_pin = read_json(self.manifest)
        require(manifest_pin["sha256"] == args.manifest_sha256,
                "explicit current source manifest hash differs")
        self.common = load_common(self.source, frozen)
        self.cancelled = self.common.install_interrupts()
        self.config, config_pin = read_json(args.config.resolve(strict=True))
        require(config_pin["sha256"] == args.config_sha256, "explicit configuration hash differs")
        exact(self.config, ("format_version", "cgroup", "input_roots", "inputs", "environment",
              "limits", "upstreams", "simulation", "benchmark", "profiler"), "Unitree configuration")
        require(type(self.config["format_version"]) is int and self.config["format_version"] == 1,
                "Unitree configuration version 1 required")
        cg = self.config["cgroup"]
        exact(cg, ("memory_max", "memory_swap_max", "expected_boot_id", "expected_leaf", "expected_device",
                   "expected_inode", "expected_cpus"), "original cgroup selection")
        require(cg["memory_max"] == 68719476736 and cg["memory_swap_max"] == 0,
                "exact 64 GiB/swap0 required")
        self.before = self.common.cgroup_state(cg)
        for row in self.before["ancestors"]:
            require(all(row["events"].get(name, 0) == 0 for name in ("oom", "oom_kill", "oom_group_kill")),
                    "all admitted ancestor OOM counters must start at zero")
        self.out = args.out.absolute()
        require(self.out.resolve() == self.out, "output path must not contain symlink parents or dot traversal")
        require(not self.out.exists() and not self.out.is_symlink(), "fresh owned output required")
        require(self.out != self.source and self.source not in self.out.parents and self.out not in self.source.parents,
                "output cannot overlap the complete frozen source")
        self.out.mkdir(mode=0o700, parents=True, exist_ok=False)
        self.pins = [manifest_pin, config_pin]
        self.roots = [safe_path(value).resolve(strict=True) for value in self.config["input_roots"]]
        require(1 <= len(self.roots) <= 64 and len(set(self.roots)) == len(self.roots),
                "distinct complete SDK/tool/upstream roots required")
        require(all(root.is_dir() and root != Path("/") and root != self.out and root not in self.out.parents and
                    self.out not in root.parents for root in self.roots), "output must not overlap any input root")
        self.inputs = {}
        require(type(self.config["inputs"]) is dict and self.config["inputs"], "explicit pinned inputs required")
        for name, value in self.config["inputs"].items():
            require(re.fullmatch(r"[a-z][a-z0-9_-]{0,63}", name), "safe unique input label required")
            exact(value, ("path", "sha256", "bytes"), "input pin")
            pin = self.pin(safe_path(value["path"]).resolve(strict=True))
            require(pin["sha256"] == value["sha256"] and pin["bytes"] == value["bytes"], "input bytes differ: " + name)
            require(any(Path(pin["path"]).is_relative_to(root) for root in self.roots) or
                    Path(pin["path"]).is_relative_to(self.source), "selected input outside complete inventories")
            self.inputs[name] = pin
        self.limits = self.config["limits"]
        exact(self.limits, ("jobs", "command_seconds", "rss_bytes", "log_bytes", "output_bytes"), "limits")
        require(self.limits["jobs"] == 1 and all(type(value) is int and value > 0 for value in self.limits.values()),
                "positive finite serial limits required")
        require(self.limits["command_seconds"] <= 3600 and self.limits["rss_bytes"] <= 8 * 1024**3 and
                self.limits["log_bytes"] <= 16 * 1024**2 and self.limits["output_bytes"] <= 4 * 1024**3,
                "lane limits exceed maintained low-memory bounds")
        self.environment = {"PATH": "/usr/bin:/bin", "LANG": "C.UTF-8", "LC_ALL": "C.UTF-8",
            "PYTHONDONTWRITEBYTECODE": "1", "OMP_NUM_THREADS": "1", "OPENBLAS_NUM_THREADS": "1",
            "MKL_NUM_THREADS": "1", "GOMAXPROCS": "1"}
        allowed = {"PATH", "LD_LIBRARY_PATH", "PYTHONPATH", "AMENT_PREFIX_PATH", "CMAKE_PREFIX_PATH",
                   "ROS_PACKAGE_PATH", "RMW_IMPLEMENTATION", "CYCLONEDDS_URI", "ROS_DOMAIN_ID", "ROS_MASTER_URI",
                   "ROS_LOCALHOST_ONLY", "MUJOCO_GL", "ROS_SECURITY_ENABLE", "ROS_SECURITY_STRATEGY",
                   "ROS_SECURITY_KEYSTORE", "ROS_SECURITY_ENCLAVE_OVERRIDE"}
        require(type(self.config["environment"]) is dict and set(self.config["environment"]) <= allowed,
                "only selected runtime/SDK environment allowed")
        for key, value in self.config["environment"].items():
            require(type(value) is str and "\0" not in value and "\n" not in value, "clean explicit environment required")
            self.environment[key] = value
        if "ROS_SECURITY_ENABLE" in self.environment:
            require(self.environment["ROS_SECURITY_ENABLE"] in ("true", "false"), "explicit ROS security boolean required")
        if self.environment.get("ROS_SECURITY_ENABLE") == "true":
            require(self.environment.get("ROS_SECURITY_STRATEGY") == "Enforce" and "ROS_SECURITY_KEYSTORE" in self.environment,
                    "selected security must remain Enforce with an explicit owned keystore")
        if "ROS_SECURITY_KEYSTORE" in self.environment:
            keystore = safe_path(self.environment["ROS_SECURITY_KEYSTORE"]).resolve(strict=True)
            require(any(keystore == root or keystore.is_relative_to(root) for root in self.roots),
                    "security keystore must belong to a fully inventoried input root")
        for name in ("tmp", "cache", "config", "home"):
            (self.out / name).mkdir(mode=0o700)
        self.environment.update(TMPDIR=str(self.out / "tmp"), XDG_CACHE_HOME=str(self.out / "cache"),
                                XDG_CONFIG_HOME=str(self.out / "config"), HOME=str(self.out / "home"))
        self.guard()
        self.validate_upstreams()
        self.source_before = self.common.source_state(self.source, self.manifest)
        self.catalogs = [self.common.catalog(root, self.out / ("input-before-%02d.jsonl" % index), self.roots)
                         for index, root in enumerate(self.roots)]
        self.report = {"format_version": 1, "kind": kind, "status": "RUNNING", "source": str(self.source),
            "source_manifest_sha256": args.manifest_sha256, "configuration": config_pin,
            "cgroup_before": self.before, "commands": [], "observations": [],
            "guest_authority": "No rights supplied by harness; exact positive/negative Runtime argv retained",
            "native_security_selection": {key: value for key, value in self.environment.items() if key.startswith("ROS_SECURITY_")},
            "scope": "Actual simulation/performance observations only; no physical robot qualification"}
        self.actors = []
        self._next_actor_ledger = 0
        self.save()

    def validate_upstreams(self):
        """Admit immutable official source selections, never infer a Git revision."""
        require(type(self.config["upstreams"]) is dict, "upstream lock map required")
        for name, value in self.config["upstreams"].items():
            exact(value, ("root", "repository", "commit", "lock_input"), "upstream selection")
            root = safe_path(value["root"]).resolve(strict=True)
            require(root in self.roots and re.fullmatch(r"[0-9a-f]{40}", value["commit"]),
                    "complete root and exact upstream Git commit required")
            repository = value["repository"].removeprefix("https://github.com/").removesuffix(".git")
            require(repository.startswith("unitreerobotics/") or repository == "google-deepmind/mujoco",
                    "official upstream repository required")
            lock, _ = self.common.document(self.input(value["lock_input"]))
            if name in lock and "status" not in lock:
                lock = lock[name]  # Root may select the actual combined six-proof metadata copy.
            require(lock.get("repository") == repository and lock.get("commit") == value["commit"] and
                    lock.get("root") == str(root) and lock.get("status") == "OFFICIAL_SNAPSHOT_GIT_TREE_AND_ALL_BLOBS_VERIFIED" and
                    type(lock.get("files")) is list and lock["files"] and lock.get("code_executed") is False,
                    "actual official Git-tree/all-blob acquisition proof required")
            # A caller-provided commit string is insufficient: reopen every
            # file admitted by its producer and require the complete file set.
            names = {str(path.relative_to(root)) for path in root.rglob("*")
                     if (path.is_file() or path.is_symlink()) and ".git" not in path.relative_to(root).parts}
            expected_names = [row["path"] for row in lock["files"]]
            require(len(set(expected_names)) == len(expected_names) and names == set(expected_names),
                    "upstream tree differs from complete acquisition proof")
            for expected in lock["files"]:
                relative = expected["path"]
                path = Path(relative)
                require(not path.is_absolute() and ".." not in path.parts, "unsafe upstream lock name")
                selected = root / path
                if expected["mode"] == "120000":
                    require(selected.is_symlink() and os.readlink(selected) == expected["target"],
                            "upstream symlink identity differs")
                else:
                    require(expected["mode"] in ("100644", "100755") and not selected.is_symlink(),
                            "regular upstream Git blob required")
                    pin = self.pin(selected)
                    require(pin["sha256"] == expected["sha256"] and pin["bytes"] == expected["bytes"],
                            "upstream locked file differs: " + relative)
            for missing in lock["unmaterialized_gitlinks"]:
                relative = Path(missing["path"])
                require(not relative.is_absolute() and ".." not in relative.parts and
                        not (root / relative).exists(), "unmaterialized upstream gitlink must remain explicitly absent")

    def input(self, name):
        require(name in self.inputs, "missing selected input: " + str(name))
        return Path(self.inputs[name]["path"])

    def pin(self, path):
        value = self.common.opened(Path(path).resolve(strict=True))
        self.pins.append(value)
        return value

    def save(self):
        self.common.atomic_json(self.out / "report.json", self.report)

    def guard(self):
        self.common.cgroup_preserved(self.before, self.common.cgroup_state(self.config["cgroup"]))
        # Sparse-image free space is not backing-filesystem free space. Check
        # every enclosing path so the Btrfs artifact mount cannot conceal an
        # exhausted outer /var/tmp filesystem. This changes no global cache.
        for path in (self.out, *self.out.parents):
            filesystem = os.statvfs(path)
            require(filesystem.f_bavail * filesystem.f_frsize >= 8 * 1024**3,
                    "owned output/backing ancestor cannot retain eight-GiB disk reserve")
        # File hashing calls this guard for every catalog row and every streamed
        # chunk. Rewalking an unchanged output tree for each call makes input
        # qualification proportional to input rows multiplied by output files.
        # Keep cgroup and backing-reserve admission above on EVERY call; only
        # the advisory owned-output walk uses a bounded half-second interval.
        # Child RSS/log/deadline limits remain independently enforced by the
        # process owner. finish() invalidates this interval before its terminal
        # check, so the final byte/alias decision never uses an earlier scan.
        now = time.monotonic()
        if now < getattr(self, "_next_output_budget_scan", 0.0):
            return
        size = 0
        for folder, dirs, files in os.walk(self.out, followlinks=False):
            for name in [*dirs, *files]:
                path = Path(folder) / name
                if path.is_symlink():
                    target = path.resolve(strict=True)
                    require(target.is_relative_to(self.out) or any(target == root or target.is_relative_to(root) for root in self.roots),
                            "output alias escapes owned output and inventoried inputs")
                    size += path.lstat().st_size
                elif path.is_file():
                    size += path.stat().st_size
        require(size <= self.limits["output_bytes"], "complete owned output budget exhausted")
        self._next_output_budget_scan = time.monotonic() + 0.5

    def command(self, job, replacements=None):
        """Expand an argv array only; no shell/parser or executable PATH lookup."""
        exact(job, ("executable", "arguments"), "command")
        executable = self.input(job["executable"])
        require(os.access(executable, os.X_OK), "selected executable lacks execute permission")
        values = {"source": str(self.source), "out": str(self.out), **(replacements or {})}
        require(type(job["arguments"]) is list and len(job["arguments"]) <= 128, "bounded actual argv required")
        args = []
        for value in job["arguments"]:
            require(type(value) is str and "\0" not in value and "\n" not in value, "clean argument required")
            for key, replacement in values.items():
                value = value.replace("{" + key + "}", replacement)
            for key in self.inputs:
                value = value.replace("{input:" + key + "}", str(self.input(key)))
            require("{" not in value and "}" not in value, "unknown argv placeholder")
            args.append(value)
        return [str(executable), *args]

    def finish(self, status, error=None):
        """Attempt every independent cleanup/preservation check, even on failure."""
        self.report.update(status=status, error=error)
        errors = []
        for actor in reversed(self.actors):
            try:
                cleanup = actor.close()
                self.report.setdefault("cleanup", []).append(cleanup)
                require(cleanup["status"] == "PASS", "owned cleanup incomplete")
            except BaseException as caught:
                errors.append(repr(caught))
        for field, operation in (("input_pins_preserved", lambda: all(self.common.opened(Path(pin["path"])) == pin
                    for pin in self.pins)), ("source_preserved", lambda: self.common.source_state(self.source, self.manifest) == self.source_before)):
            try:
                self.report[field] = operation()
                require(self.report[field], field + " failed")
            except BaseException as caught:
                errors.append(repr(caught))
        try:
            after = [self.common.catalog(root, self.out / ("input-after-%02d.jsonl" % index), self.roots)
                     for index, root in enumerate(self.roots)]
            self.report["whole_roots_preserved"] = all({k: v for k, v in left.items() if k != "catalog"} ==
                {k: v for k, v in right.items() if k != "catalog"} for left, right in zip(self.catalogs, after))
            require(self.report["whole_roots_preserved"], "input roots changed")
            self.report["inventories_before"] = self.catalogs
            self.report["inventories_after"] = after
        except BaseException as caught:
            errors.append(repr(caught))
        try:
            self._next_output_budget_scan = 0.0
            self.guard()
            self.report["cgroup_after"] = self.common.cgroup_state(self.config["cgroup"])
        except BaseException as caught:
            errors.append(repr(caught))
        if errors:
            self.report.update(status="FAIL", finalization_errors=errors)
        self.report["input_pins"] = self.pins
        self.save()
        return 0 if self.report["status"].startswith("PASS_") else 1


def _owned_actor_snapshot(common, key, item, *, read_membership=None, read_self_membership=None):
    """Bracket procfs observations with the SAME retained kernel pidfd.

    Procfs absence is not process termination. Only ENOENT/ESRCH may trigger a
    second poll of this already owned pidfd. A reused numerical PID/birth is
    never adopted, tracked or signalled: it can only cause retirement of the
    ORIGINAL record if that original pidfd is terminal, otherwise refusal.
    Unknown errno/invalid pidfd/cgroup violations remain fatal. The injected
    reader exists solely for deterministic DATA race regressions; production
    passes none and reads the actual original PID's cgroup file.
    """
    original, descriptor = item["identity"], item["fd"]
    require(common.birth(original) == key, "owned record key differs from original birth")
    def terminal(phase):
        if common.pidfd_terminal(descriptor):
            return {"terminal": True, "phase": phase,
                    "evidence": "SAME_RETAINED_PIDFD_CONFIRMED_TERMINAL"}
        return None
    def identity(phase):
        try:
            current = common.identity(original["pid"])
        except OSError as error:
            if error.errno not in (errno.ENOENT, errno.ESRCH):
                raise
            retired = terminal(phase + "_PROCFS_ABSENT")
            if retired is not None: return None, retired
            raise
        if current is None or common.birth(current) != key:
            retired = terminal(phase + "_BIRTH_UNAVAILABLE_OR_CHANGED")
            if retired is not None: return None, retired
            require(False, "owned birth disappeared or changed without terminal retained pidfd")
        return current, None
    retired = terminal("INITIAL_POLL")
    if retired is not None: return retired
    first, retired = identity("BEFORE_CGROUP")
    if retired is not None: return retired
    try:
        membership = (read_membership(original["pid"]) if read_membership is not None else
                      Path("/proc", str(original["pid"]), "cgroup").read_text())
    except OSError as error:
        if error.errno not in (errno.ENOENT, errno.ESRCH):
            raise
        retired = terminal("CGROUP_PROCFS_ABSENT")
        if retired is not None: return retired
        raise
    # A successfully observed mismatch is fatal immediately. Neither a
    # trailing vanished/reused birth nor a terminal poll may erase it. Read
    # errors for the coordinator itself also remain fatal, never task exit.
    own_membership = (read_self_membership() if read_self_membership is not None else
                      Path("/proc/self/cgroup").read_text())
    require(membership == own_membership, "owned process crossed cgroup boundary")
    last, retired = identity("AFTER_CGROUP")
    if retired is not None: return retired
    retired = terminal("FINAL_POLL")
    if retired is not None: return retired
    return {"terminal": False, "identity": last, "membership": membership}


class Actors:
    """A bounded cooperating process group using the common pidfd owner.

    Several actors are necessary for one real DDS case. Cases, compilers,
    benchmark repetitions, and profiler collections still execute serially.
    No numeric PID or process-group signal is issued by this adapter.
    """
    def __init__(self, frame):
        self.frame = frame
        self.owner = frame.common.SerialRunner(frame.out, frame.limits, frame.guard)
        self.rows = []
        self.deadlines = {}
        self.buffers = {}
        self.lines = {}
        self.logs = {}
        self.read_offsets = {}
        # Closed actors may be removed from the active cleanup list between
        # cases. Ledger names belong to the entire Frame lifetime, so active
        # list length cannot choose a fresh exclusive-create filename.
        ordinal = frame._next_actor_ledger
        frame._next_actor_ledger += 1
        self.retired = frame.out / ("retired-%d.jsonl" % ordinal)
        self.retirement = self.retired.open("x", encoding="utf-8")
        frame.actors.append(self)

    def start(self, name, command, environment=None, data=None, json_output=True, credentials=None):
        require(re.fullmatch(r"[a-zA-Z0-9_-]{1,96}", name) and name not in [r[1]["name"] for r in self.rows],
                "unique bounded actor name required")
        self.frame.guard()
        out, err = self.frame.out / (name + ".stdout"), self.frame.out / (name + ".stderr")
        row = {"name": name, "argv": command, "environment": environment or self.frame.environment,
               "stdout": str(out), "stderr": str(err), "status": "RUNNING", "peak_owned_rss_bytes": 0,
               "stdout_protocol": "json-lines" if json_output else "binary", "started_monotonic_ns": time.monotonic_ns()}
        require(command and Path(command[0]).is_absolute(), "absolute explicit executable required")
        # Retain Popen before any procfs/pidfd operation can raise.
        with err.open("xb") as errors:
            options = {}
            if credentials is not None:
                require(type(credentials) is dict and set(credentials) == {"uid", "gid"} and
                        all(type(value) is int and value >= 1000 for value in credentials.values()),
                        "explicit unprivileged installer credentials required")
                options = {"user": credentials["uid"], "group": credentials["gid"], "extra_groups": ()}
                row["credentials"] = credentials
            child = subprocess.Popen(command, cwd=self.frame.out, env=row["environment"],
                stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=errors, bufsize=0, **options)
        self.owner.children.append(child)
        self.rows.append((child, row))
        self.frame.report["commands"].append(row)
        row["pid"] = child.pid
        first = self.frame.common.identity(child.pid)
        self.owner.roots[child.pid] = first
        row["first_identity"] = first
        self.owner.acquire(child.pid, os.getpid())
        require(any(key[0] == child.pid for key in self.owner.owned) or child.poll() is not None,
                "live actor ownership unproven")
        self.deadlines[child.pid] = time.monotonic() + self.frame.limits["command_seconds"]
        self.buffers[child.pid] = bytearray()
        self.lines[child.pid] = []
        self.read_offsets[child.pid] = 0
        self.logs[child.pid] = out.open("xb")
        os.set_blocking(child.stdout.fileno(), False)
        os.set_blocking(child.stdin.fileno(), False)
        self.save()
        if data is not None:
            self.write(child, data)
            child.stdin.close()
        return child, row

    def save(self):
        self.frame.save()

    def pump(self):
        for child, row in self.rows:
            if child.stdout.closed:
                continue
            while True:
                try:
                    data = os.read(child.stdout.fileno(), 65536)
                except BlockingIOError:
                    break
                if not data:
                    break
                self.logs[child.pid].write(data)
                self.logs[child.pid].flush()
                if row["stdout_protocol"] == "binary":
                    continue
                buffer = self.buffers[child.pid]
                buffer.extend(data)
                require(len(buffer) <= MAX_LINE, "actor frame exceeds one MiB")
                while b"\n" in buffer:
                    line, _, rest = buffer.partition(b"\n")
                    self.lines[child.pid].append(bytes(line))
                    buffer[:] = rest
                    require(len(self.lines[child.pid]) <= 4096, "unconsumed actor records exceed bound")

    def check(self):
        self.frame.guard()
        self.owner.discover()
        self.pump()
        rss = self.owner.rss()
        require(rss <= self.frame.limits["rss_bytes"], "combined owned actor RSS exhausted")
        now = time.monotonic()
        for key, item in list(self.owner.owned.items()):
            snapshot = _owned_actor_snapshot(self.frame.common, key, item)
            if snapshot["terminal"]:
                self.retirement.write(json.dumps({"first": item["identity"], "after_pidfd": item["identity_after"],
                    "terminal_check": snapshot}) + "\n")
                self.retirement.flush()
                os.close(item["fd"])
                del self.owner.owned[key]
                continue
            item.setdefault("admitted_monotonic", now)
            require(now - item["admitted_monotonic"] <= min(600, self.frame.limits["command_seconds"]),
                    "owned descendant lifetime exceeded")
        for child, row in self.rows:
            row["peak_owned_rss_bytes"] = max(row["peak_owned_rss_bytes"], rss)
            require(child.poll() is not None or now < self.deadlines[child.pid], "actor deadline exhausted")
            require(all(Path(row[key]).stat().st_size <= self.frame.limits["log_bytes"]
                        for key in ("stdout", "stderr")), "actor log budget exhausted")

    def write(self, child, data):
        require(type(data) is bytes and len(data) <= MAX_LINE, "bounded actor input required")
        offset = 0
        while offset < len(data):
            self.check()
            require(child.poll() is None, "actor exited before complete input")
            try:
                count = os.write(child.stdin.fileno(), data[offset:])
            except BlockingIOError:
                select.select([], [child.stdin], [], .01)
                continue
            require(count > 0, "zero actor input transfer")
            offset += count

    def send(self, child, value):
        self.write(child, (json.dumps(value, allow_nan=False, separators=(",", ":")) + "\n").encode())

    def receive(self, child):
        while True:
            self.check()
            if self.lines[child.pid]:
                return decode(self.lines[child.pid].pop(0))
            require(child.poll() is None, "actor exited before complete JSON record")
            select.select([child.stdout], [], [], .01)

    def receive_line(self, child):
        """Read one exact UTF-8 marker line from an actual Guest, without JSON."""
        while True:
            self.check()
            if self.lines[child.pid]:
                return self.lines[child.pid].pop(0).decode("utf-8", "strict")
            require(child.poll() is None, "actor exited before complete marker line")
            select.select([child.stdout], [], [], .01)

    def receive_bytes(self, child, count):
        """Read the next exact binary frame after logging the actual pipe bytes."""
        require(type(count) is int and 0 < count <= MAX_LINE, "bounded exact binary frame required")
        row = next(row for actor, row in self.rows if actor is child)
        require(row["stdout_protocol"] == "binary", "binary actor required")
        offset = self.read_offsets[child.pid]
        while True:
            self.check()
            if Path(row["stdout"]).stat().st_size >= offset + count:
                with Path(row["stdout"]).open("rb") as stream:
                    stream.seek(offset)
                    value = stream.read(count)
                require(len(value) == count, "complete logged binary frame disappeared")
                self.read_offsets[child.pid] += count
                return value
            require(child.poll() is None, "actor retired before complete binary frame")
            select.select([child.stdout], [], [], .01)

    def finish(self, child):
        while child.poll() is None:
            self.check()
            time.sleep(.005)
        self.check()
        row = next(row for actor, row in self.rows if actor is child)
        require(not self.buffers[child.pid], "actor left a truncated output record")
        row.update(returncode=child.returncode, status="PASS" if child.returncode == 0 else "FAIL")
        row["finished_monotonic_ns"] = time.monotonic_ns()
        require(child.returncode == 0, "actor returned failure: " + row["name"])
        self.save()
        return row

    def close(self):
        result = self.owner.close()
        errors = []
        try:
            self.pump()
        except BaseException as error:
            errors.append(repr(error))
        for child, row in self.rows:
            try:
                # Drain after all owned writers have retired; avoid losing a
                # final diagnostic line when the normal command failed.
                self.logs[child.pid].close()
                row["final_returncode"] = child.poll()
                for key in ("stdout", "stderr"):
                    row[key + "_pin"] = self.frame.pin(row[key])
                child.stdout.close()
                if not child.stdin.closed:
                    child.stdin.close()
            except BaseException as error:
                errors.append(repr(error))
        self.retirement.close()
        result["retired_processes"] = self.frame.pin(self.retired)
        if errors:
            result.update(status="CLEANUP_INCOMPLETE", errors=errors)
        self.rows.clear()
        return result


def child_arguments(parser):
    """Internal actors are explicit consumers of the same current Root lock."""
    parser.add_argument("--control-config", type=Path, required=True)
    parser.add_argument("--control-config-sha256", required=True)
    parser.add_argument("--control-manifest", type=Path, required=True)
    parser.add_argument("--control-manifest-sha256", required=True)


def child_guard(args):
    """Check admission before importing native MuJoCo/DDS/ROS packages."""
    config, pin = read_json(args.control_config)
    frozen, manifest = read_json(args.control_manifest)
    require(pin["sha256"] == args.control_config_sha256 and
            manifest["sha256"] == args.control_manifest_sha256, "child control input changed")
    common = load_common(ROOT, frozen)
    for name in ("test/0073.ros-tests/support/source/ros_cgroup/probe.py", "test/0073.ros-tests/support/source/ros_unitree/common.py"):
        actual = common.opened(ROOT / name)
        require(frozen.get(name) == {"bytes": actual["bytes"], "sha256": actual["sha256"]},
                "child helper absent from complete freeze")
    require(config["cgroup"]["memory_max"] == 68719476736 and config["cgroup"]["memory_swap_max"] == 0,
            "child requires exact 64 GiB/swap0")
    before = common.cgroup_state(config["cgroup"])
    def guard():
        current = common.cgroup_state(config["cgroup"])
        common.cgroup_preserved(before, current)
        require(all(row["events"].get(key, 0) == 0 for row in current["ancestors"]
                    for key in ("oom", "oom_kill", "oom_group_kill")), "ancestor OOM counter nonzero")
    guard()
    return config, guard


def actor_control_arguments(frame):
    return ["--control-config", frame.report["configuration"]["path"],
        "--control-config-sha256", frame.report["configuration"]["sha256"],
        "--control-manifest", str(frame.manifest), "--control-manifest-sha256", frame.report["source_manifest_sha256"]]


def write_record(value):
    """Emit one complete bounded JSON record, with no implicit success marker."""
    raw = (json.dumps(value, allow_nan=False, separators=(",", ":")) + "\n").encode()
    require(len(raw) <= MAX_LINE, "output record exceeds bound")
    sys.stdout.buffer.write(raw)
    sys.stdout.buffer.flush()


def read_record():
    raw = sys.stdin.buffer.readline(MAX_LINE + 1)
    require(raw and len(raw) <= MAX_LINE and raw.endswith(b"\n"), "complete bounded input record required")
    return decode(raw)
