#!/usr/bin/env python3
"""Reopen retained artifact *content* after a host/cgroup incarnation change.

This program is a read-only input admission step, not a compiler or Runtime
test. Historical seven-field pins remain historical. Only newly opened file
descriptors produce current pins, and no historical execution status is
changed. The coordinator must invoke this program inside its explicitly
selected current 64 GiB, zero-swap cgroup and retain its usual outer deadline
and aggregate RSS supervision. Importing this module starts no work.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import stat
import sys
import time
import types


CHUNK = 1024**2
JSON_LIMIT = 16 * CHUNK
PIN_KEYS = frozenset(("path", "bytes", "sha256", "device", "inode", "mtime_ns", "ctime_ns"))
STAT_KEYS = ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns", "st_mode", "st_nlink")
CGROUP_KEYS = frozenset(("memory_max", "memory_swap_max", "expected_boot_id", "expected_leaf",
                         "expected_device", "expected_inode", "expected_cpus"))


def require(condition, message):
    if not condition:
        raise ValueError(message)


def exact(value, keys, label):
    require(type(value) is dict and set(value) == set(keys), "closed " + label + " required")


def clean_absolute(value):
    """Accept a literal absolute spelling; never resolve through input symlinks."""
    require(type(value) is str and value.startswith("/") and len(value) <= 4096 and
            not any(char in value for char in ("\0", "\n", "\r")), "clean absolute path required")
    path = PurePosixPath(value)
    require(str(path) == value and ".." not in path.parts and value != "/",
            "normalized non-root absolute path required: " + value)
    return value


def relative_name(value):
    require(type(value) is str and 0 < len(value) <= 4096 and
            not any(char in value for char in ("\0", "\n", "\r")), "bounded relative name required")
    path = PurePosixPath(value)
    require(not path.is_absolute() and str(path) == value and
            all(part not in (".", "..") for part in path.parts), "normalized relative name required")
    return value


def historical_pin(value):
    """Validate old observations without interpreting them as current identity."""
    exact(value, PIN_KEYS, "historical seven-field pin")
    clean_absolute(value["path"])
    require(type(value["sha256"]) is str and re.fullmatch(r"[0-9a-f]{64}", value["sha256"]) is not None,
            "lowercase SHA256 required")
    require(all(type(value[key]) is int and value[key] >= 0
                for key in ("bytes", "device", "mtime_ns", "ctime_ns")) and
            type(value["inode"]) is int and value["inode"] > 0, "integer historical pin fields required")
    return dict(value)


def _same_stat(left, right):
    return all(getattr(left, key) == getattr(right, key) for key in STAT_KEYS)


def _directory(path):
    """Open every absolute directory component with O_NOFOLLOW.

    Rejecting only a final file symlink is insufficient: a changed parent link
    could redirect an SDK/source lookup outside the selected root. Descriptor
    relative traversal also keeps already admitted parents bound while read.
    """
    clean_absolute(path)
    descriptor = os.open("/", os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
    try:
        for name in PurePosixPath(path).parts[1:]:
            child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=descriptor)
            os.close(descriptor)
            descriptor = child
        return descriptor
    except BaseException:
        os.close(descriptor)
        raise


def _parent(path):
    clean_absolute(path)
    parent = str(PurePosixPath(path).parent)
    # The directory opener intentionally disallows selecting '/' as an input
    # root, but a directly named file below '/' still has a legitimate parent.
    descriptor = (os.open("/", os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
                  if parent == "/" else _directory(parent))
    return descriptor, PurePosixPath(path).name


def _verify_named_file(path, parent, name, observed):
    """Bracket the absolute parent spelling as well as the opened file name."""
    current_parent, current_name = _parent(path)
    try:
        old, current = os.fstat(parent), os.fstat(current_parent)
        require(name == current_name and (old.st_dev, old.st_ino) == (current.st_dev, current.st_ino) and
                _same_stat(observed, os.stat(current_name, dir_fd=current_parent, follow_symlinks=False)),
                "absolute parent path or opened file changed during read: " + path)
    finally:
        os.close(current_parent)


def _opened_at(parent, name, displayed_path, maximum, guard, *, collect=False, no_hardlinks=False):
    descriptor = os.open(name, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK, dir_fd=parent)
    try:
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= maximum,
                "bounded ordinary file required: " + displayed_path)
        require(not no_hardlinks or before.st_nlink == 1,
                "whole-root admission rejects ambiguous regular hardlinks: " + displayed_path)
        digest, count = hashlib.sha256(), 0
        body = bytearray() if collect else None
        while True:
            guard()
            block = os.read(descriptor, min(CHUNK, maximum - count + 1))
            if not block:
                break
            count += len(block)
            require(count <= maximum, "file grew beyond its explicit bound: " + displayed_path)
            digest.update(block)
            if collect:
                body.extend(block)
        after = os.fstat(descriptor)
        named = os.stat(name, dir_fd=parent, follow_symlinks=False)
        require(count == before.st_size and _same_stat(before, after) and _same_stat(before, named),
                "opened file or its name changed during read: " + displayed_path)
        pin = {"path": displayed_path, "bytes": count, "sha256": digest.hexdigest(),
               "device": before.st_dev, "inode": before.st_ino,
               "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
        return pin, body, before
    finally:
        os.close(descriptor)


def opened_file(path, maximum, guard, *, collect=False):
    """Stream stable regular bytes into a fresh actual pin using at most 1 MiB.

    ``collect`` is reserved for explicitly bounded metadata; file bodies are
    otherwise never retained. This function never substitutes historical
    device/inode/timestamps, including when content matches exactly.
    """
    require(type(maximum) is int and 0 <= maximum <= 16 * 1024**3 and type(collect) is bool and
            (not collect or maximum <= JSON_LIMIT), "finite streamed file or small collected metadata bound required")
    parent, name = _parent(path)
    try:
        pin, body, info = _opened_at(parent, name, path, maximum, guard, collect=collect)
        _verify_named_file(path, parent, name, info)
        return (pin, body) if collect else pin
    finally:
        os.close(parent)


def requalify_file(previous, current_path, guard):
    """Return both identities and a content-only bridge, never an execution PASS."""
    previous = historical_pin(previous)
    current = opened_file(clean_absolute(current_path), previous["bytes"], guard)
    require((current["bytes"], current["sha256"]) == (previous["bytes"], previous["sha256"]),
            "retained file content differs: " + current_path)
    return {"historical_pin": previous, "current_pin": current,
            "content_relation": "SAME_BYTES_FULL_STREAM_SHA256", "execution_qualification": "NOT_PERFORMED"}


def requalify_persistent_pin(previous, current_path, guard, *, historical_device, current_device,
                            observed_current=None):
    """Admit only a known restart device renumbering for one persistent file.

    A consumer of retained build metadata can require this stricter bridge:
    path, inode, timestamps and complete bytes must survive unchanged, while
    the device transition must be the caller's explicit, finite root mapping.
    ``observed_current`` binds the result to a current owner/Frame observation
    when supplied. The returned current pin always contains the actual new
    device; this function never constructs a historical-looking physical pin.
    The caller must separately constrain which persistent paths may use the
    mapping, and must retain this bridge beside its actual current pin ledger.
    """
    require(type(historical_device) is int and historical_device >= 0 and
            type(current_device) is int and current_device >= 0 and historical_device != current_device,
            "explicit distinct historical/current filesystem devices required")
    bridge = requalify_file(previous, current_path, guard)
    old, current = bridge["historical_pin"], bridge["current_pin"]
    require(old["device"] == historical_device and current["device"] == current_device and
            all(old[key] == current[key] for key in PIN_KEYS - {"device"}),
            "persistent file changed beyond its explicitly selected restart device renumbering")
    if observed_current is not None:
        historical_pin(observed_current)  # Validate shape without treating it as an old observation.
        require(current == observed_current, "persistent bridge differs from the current owner's opened pin")
    bridge["content_relation"] = "PERSISTENT_FILE_SAME_BYTES_INODE_TIMES_EXPLICIT_DEVICE_REBIND"
    bridge["permitted_identity_changes"] = ["device"]
    bridge["device_transition"] = {"historical": historical_device, "current": current_device}
    return bridge


class PersistentPinSet:
    """An explicit accessor for a finite retained producer input closure.

    This is deliberately not a recursive report rewriter or a replacement for
    ``Frame.pin``. Callers select the historical pins they intend to consume,
    keep raw reports untouched, and use ``admit`` only at the corresponding
    historical/current comparison. The returned pin is an actual current pin;
    ``evidence`` keeps the old observations separately for the new receipt.
    """
    def __init__(self, expected, roots, guard):
        require(type(expected) is list and 0 < len(expected) <= 16384 and
                type(roots) is list and 0 < len(roots) <= 64, "finite explicit retained pin/root sets required")
        self.expected, self.roots, self.guard, self._evidence = {}, [], guard, {}
        for row in expected:
            pin = historical_pin(row)
            require(pin["path"] not in self.expected, "one historical identity per retained path required")
            self.expected[pin["path"]] = pin
        for row in roots:
            exact(row, ("root", "historical_device", "current_device"), "persistent root/device mapping")
            root = clean_absolute(row["root"])
            require(root not in {item["root"] for item in self.roots} and
                    type(row["historical_device"]) is int and row["historical_device"] >= 0 and
                    type(row["current_device"]) is int and row["current_device"] >= 0 and
                    row["historical_device"] != row["current_device"], "distinct explicit root/device mapping required")
            self.roots.append(dict(row))
        # A deliberately more specific root can select a separate retained
        # filesystem below a parent path. Longest component-prefix wins; the
        # actual opened file still has to match that root's exact new device.
        self.roots.sort(key=lambda item: len(PurePosixPath(item["root"]).parts), reverse=True)
        for pin in self.expected.values():
            self._mapping(pin)

    def _mapping(self, pin):
        matches = [row for row in self.roots if pin["path"] == row["root"] or
                   pin["path"].startswith(row["root"] + "/")]
        require(matches and pin["device"] == matches[0]["historical_device"],
                "historical file is outside its explicit persistent root/device mapping")
        return matches[0]

    def admit(self, previous, *, observed_current=None):
        """Return an actual current pin and retain its separate bridge.

        The first admission always hashes the complete file. A repeated
        comparison may reuse that receipt only when the caller supplies an
        exactly matching current full-seven observation; ``preserve`` still
        independently reopens every consumed file at the end. Supplying no
        current observation always requests a fresh complete read.
        """
        previous = historical_pin(previous)
        require(self.expected.get(previous["path"]) == previous,
                "historical pin is outside the preselected finite producer closure")
        if previous["path"] in self._evidence and observed_current is not None:
            historical_pin(observed_current)
            current = self._evidence[previous["path"]]["current_pin"]
            require(current == observed_current, "memoized bridge differs from the current owner's observation")
            self.guard()
            return dict(current)
        mapping = self._mapping(previous)
        bridge = requalify_persistent_pin(previous, previous["path"], self.guard,
            historical_device=mapping["historical_device"], current_device=mapping["current_device"],
            observed_current=observed_current)
        if previous["path"] in self._evidence:
            require(self._evidence[previous["path"]] == bridge,
                    "persistent current identity changed between admissions")
        self._evidence[previous["path"]] = bridge
        return dict(bridge["current_pin"])

    @property
    def evidence(self):
        """Separate raw historical/current facts for inclusion in a new receipt."""
        return [{"root_mapping": dict(self._mapping(self.expected[path])), **self._evidence[path]}
                for path in sorted(self._evidence)]

    def preserve(self, *, require_complete=False):
        """Reopen every consumed current pin; optionally require the full selection."""
        require(type(require_complete) is bool, "explicit complete-selection boolean required")
        require(not require_complete or set(self._evidence) == set(self.expected),
                "not every selected historical pin was consumed")
        for path in sorted(self._evidence):
            current = self._evidence[path]["current_pin"]
            require(opened_file(path, current["bytes"], self.guard) == current,
                    "persistent consumed file changed before terminal admission")


def _pairs(items):
    result = {}
    for key, value in items:
        require(key not in result, "duplicate JSON member: " + key)
        result[key] = value
    return result


def _decode(raw):
    return json.loads(raw, object_pairs_hook=_pairs,
                      parse_constant=lambda value: require(False, "nonfinite JSON: " + value))


def _document(path, expected_sha256, guard):
    pin, raw = opened_file(path, JSON_LIMIT, guard, collect=True)
    require(pin["sha256"] == expected_sha256, "explicit metadata SHA256 differs: " + path)
    return _decode(raw), pin


class CurrentGuard:
    """Keep hashing in the explicitly selected current cgroup incarnation.

    The data-only cgroup probe is read from the exact old source manifest and
    individually hash-bound before it is loaded. This admits that helper's
    code bytes, not any old execution result or a matching limit elsewhere.
    """
    def __init__(self, source, frozen, selection):
        exact(selection, CGROUP_KEYS, "current cgroup selection")
        require(type(selection["memory_max"]) is int and selection["memory_max"] == 64 * 1024**3 and
                type(selection["memory_swap_max"]) is int and selection["memory_swap_max"] == 0,
                "current exact 64 GiB and zero swap required")
        require(type(selection["expected_device"]) is int and selection["expected_device"] >= 0 and
                type(selection["expected_inode"]) is int and selection["expected_inode"] > 0 and
                type(selection["expected_cpus"]) is str and selection["expected_cpus"] and
                re.fullmatch(r"[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}",
                             selection["expected_boot_id"]) is not None,
                "complete current boot/device/inode/CPU selection required")
        clean_absolute(selection["expected_leaf"])
        name = "test/0073.ros-tests/support/source/ros_cgroup/probe.py"
        require(name in frozen, "historical complete source manifest must bind the cgroup probe")
        pin, body = opened_file(source + "/" + name, CHUNK, lambda: None, collect=True)
        require(frozen[name] == {"bytes": pin["bytes"], "sha256": pin["sha256"]},
                "selected cgroup probe differs from the historical source manifest")
        self.probe_pin = pin
        module = types.ModuleType("uwvm_restart_content_cgroup_probe")
        module.__file__ = pin["path"]
        exec(compile(bytes(body), pin["path"], "exec"), module.__dict__)
        self.module, self.selection = module, dict(selection)
        self.started, self.next_probe, self.maximum_seconds = time.monotonic(), 0.0, 3600
        self.before = self.force()

    def force(self):
        require(time.monotonic() - self.started <= self.maximum_seconds, "content admission deadline exceeded")
        row = self.module.probe(expected_memory_max=self.selection["memory_max"],
            expected_leaf=self.selection["expected_leaf"], expected_device=self.selection["expected_device"],
            expected_inode=self.selection["expected_inode"], expected_boot=self.selection["expected_boot_id"])
        require(row.get("status") == "PASS", "current cgroup admission failed: " + str(row.get("reason")))
        ancestors = row["cgroup_ancestors"]
        require(ancestors and ancestors[0]["cpuset.cpus.effective"] == self.selection["expected_cpus"] and
                int(ancestors[0]["memory.current"]) <= 60 * 1024**3,
                "current CPU selection or four-GiB memory reserve differs")
        for ancestor in ancestors:
            if "controllerless_hierarchy_root" in ancestor:
                continue
            events = dict((key, int(value)) for key, value in
                          (line.split() for line in ancestor["memory.events"].splitlines()))
            require(all(events.get(key, 0) == 0 for key in ("oom", "oom_kill", "oom_group_kill")),
                    "current cgroup ancestry has an OOM event")
        if hasattr(self, "before"):
            previous = self.before
            require(row["cgroup_incarnation"] == previous["cgroup_incarnation"] and
                    row["membership"] == previous["membership"] and
                    len(ancestors) == len(previous["cgroup_ancestors"]), "current cgroup ancestry changed")
            stable = ("path", "memory.max", "memory.swap.max", "cpu.max", "pids.max", "cpuset.cpus.effective",
                      "controllerless_hierarchy_root")
            require(all(all(left.get(key) == right.get(key) for key in stable)
                        for left, right in zip(previous["cgroup_ancestors"], ancestors)),
                    "current ancestor limits or controller-root identity changed")
        self.next_probe = time.monotonic() + 0.5
        self.latest = row
        return row

    def __call__(self):
        if time.monotonic() >= self.next_probe:
            self.force()


def _directory_state(descriptor):
    info = os.fstat(descriptor)
    require(stat.S_ISDIR(info.st_mode), "ordinary directory descriptor required")
    return info


def _walk(root, guard, limits, visitor, *, file_identity=None):
    """Walk anchored directory FDs; never follow a link or cross a mount device.

    All directory names are checked, including empty directories, but historical
    catalogs made by tools/ros_distro/common.py did not record empty directories.
    The compatibility claim is therefore the exact regular-file/symlink set.
    """
    descriptor = _directory(root)
    top = _directory_state(descriptor)
    directories = hashlib.sha256()
    totals = {"entries": 0, "files": 0, "bytes": 0, "symlinks": 0, "directories": 1}

    def descend(parent, prefix, depth):
        require(depth <= 128, "whole-root directory depth exceeds the finite bound")
        before = _directory_state(parent)
        directories.update((json.dumps({"relative": prefix[:-1], "device": before.st_dev,
            "inode": before.st_ino, "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns,
            "mode": before.st_mode, "nlink": before.st_nlink}, sort_keys=True,
            separators=(",", ":")) + "\n").encode())
        guard()
        with os.scandir(parent) as directory:
            names = []
            for item in directory:
                require(len(names) < limits["catalog_entries"], "directory entry count exceeds bound")
                names.append(item.name)
        for name in sorted(names):
            relative = relative_name(prefix + name)
            totals["entries"] += 1
            require(totals["entries"] <= limits["catalog_entries"], "whole-root entry count exceeds bound")
            initial = os.stat(name, dir_fd=parent, follow_symlinks=False)
            require(initial.st_dev == top.st_dev, "whole-root input crosses an unselected filesystem device")
            displayed = root + "/" + relative
            if stat.S_ISDIR(initial.st_mode):
                child = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=parent)
                try:
                    require(_same_stat(initial, os.fstat(child)), "directory changed before descent")
                    totals["directories"] += 1
                    descend(child, relative + "/", depth + 1)
                finally:
                    os.close(child)
                require(_same_stat(initial, os.stat(name, dir_fd=parent, follow_symlinks=False)),
                        "directory name changed during descent")
            elif stat.S_ISREG(initial.st_mode):
                pin, _, info = _opened_at(parent, name, displayed, limits["file_bytes"], guard,
                                          no_hardlinks=file_identity is None)
                require(_same_stat(initial, info), "file changed between enumeration and opened read")
                if file_identity is not None:
                    file_identity(relative, pin, info)
                totals["files"] += 1
                totals["bytes"] += pin["bytes"]
                require(totals["bytes"] <= limits["total_bytes"], "whole-root byte count exceeds bound")
                row = {"relative": relative, "kind": "file", "bytes": pin["bytes"],
                       "sha256": pin["sha256"], "executable": bool(info.st_mode & 0o111), "current_pin": pin}
                if file_identity is not None:
                    row["current_nlink"] = info.st_nlink
                visitor(row)
            elif stat.S_ISLNK(initial.st_mode):
                target = os.readlink(name, dir_fd=parent)
                require(type(target) is str and 0 < len(target) <= 4096 and
                        not any(char in target for char in ("\0", "\n", "\r")), "bounded symlink text required")
                final = os.stat(name, dir_fd=parent, follow_symlinks=False)
                require(_same_stat(initial, final) and os.readlink(name, dir_fd=parent) == target,
                        "symlink identity or text changed during inventory")
                totals["symlinks"] += 1
                visitor({"relative": relative, "kind": "symlink", "target": target,
                         "current_link": {"path": displayed, "target": target, "device": initial.st_dev,
                                          "inode": initial.st_ino, "mtime_ns": initial.st_mtime_ns,
                                          "ctime_ns": initial.st_ctime_ns}})
            else:
                require(False, "special file in whole-root content selection: " + displayed)
        require(_same_stat(before, _directory_state(parent)), "directory changed during whole-root inventory")

    try:
        descend(descriptor, "", 0)
        named = _directory(root)
        try:
            require(_same_stat(top, os.fstat(named)), "whole-root path changed during inventory")
        finally:
            os.close(named)
    finally:
        os.close(descriptor)
    totals["current_directory_identity_sha256"] = directories.hexdigest()
    return totals


def _old_catalog(previous, guard, limits, *, hardlink_groups=None):
    """Normalize only the content fields of a retained common.py JSONL catalog.

    Old resolved/target_file observations stay in the historical catalog. They
    are never current target evidence: this helper deliberately does not follow
    links, including absolute links or dangling ones.
    """
    previous = historical_pin(previous)
    require(previous["bytes"] <= limits["catalog_bytes"], "historical catalog exceeds bound")
    parent, name = _parent(previous["path"])
    descriptor = None
    rows, digest, count = {}, hashlib.sha256(), 0
    try:
        descriptor = os.open(name, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK, dir_fd=parent)
        before = os.fstat(descriptor)
        require(stat.S_ISREG(before.st_mode) and before.st_size == previous["bytes"],
                "bounded ordinary historical catalog required")
        with os.fdopen(descriptor, "rb", closefd=False) as stream:
            while True:
                guard()
                line = stream.readline(65537)
                if not line:
                    break
                require(len(line) <= 65536 and line.endswith(b"\n"), "bounded terminated JSONL row required")
                digest.update(line)
                count += len(line)
                require(count <= limits["catalog_bytes"] and len(rows) < limits["catalog_entries"],
                        "historical catalog exceeds finite bounds")
                row = _decode(line)
                require(type(row) is dict, "catalog row object required")
                relative = relative_name(row.get("relative"))
                require(relative not in rows, "duplicate historical catalog name")
                if row.get("kind") == "file":
                    required = {"relative", "kind", "bytes", "sha256", "executable",
                                "device", "inode", "mtime_ns", "ctime_ns"}
                    exact(row, required, "historical regular-file catalog row")
                    historical_pin({key: (previous["path"] if key == "path" else row[key]) for key in PIN_KEYS})
                    require(type(row["executable"]) is bool and row["bytes"] <= limits["file_bytes"],
                            "bounded historical regular-file content required")
                    rows[relative] = {key: row[key] for key in ("relative", "kind", "bytes", "sha256", "executable")}
                    if hardlink_groups is not None:
                        identity = (row["device"], row["inode"])
                        metadata = {key: row[key] for key in
                                    ("bytes", "sha256", "executable", "mtime_ns", "ctime_ns")}
                        group = hardlink_groups.setdefault(identity, {"members": [], "metadata": metadata})
                        require(group["metadata"] == metadata,
                                "historical aliases of one inode disagree on content or metadata")
                        group["members"].append(relative)
                elif row.get("kind") == "symlink":
                    required = {"relative", "kind", "target", "resolved", "device", "inode", "mtime_ns", "ctime_ns"}
                    require(set(row) in (required, required | {"resolution_state"}, required | {"target_file"}),
                            "known historical symlink catalog shape required")
                    require(type(row["target"]) is str and 0 < len(row["target"]) <= 4096 and
                            not any(char in row["target"] for char in ("\0", "\n", "\r")),
                            "bounded historical symlink text required")
                    rows[relative] = {key: row[key] for key in ("relative", "kind", "target")}
                else:
                    require(False, "only historical regular-file and symlink rows are supported")
        after = os.fstat(descriptor)
        require(count == before.st_size and _same_stat(before, after) and
                _same_stat(before, os.stat(name, dir_fd=parent, follow_symlinks=False)),
                "historical catalog changed while opened")
        _verify_named_file(previous["path"], parent, name, before)
        require(digest.hexdigest() == previous["sha256"], "historical catalog content hash differs")
        current = {"path": previous["path"], "bytes": count, "sha256": digest.hexdigest(),
                   "device": before.st_dev, "inode": before.st_ino,
                   "mtime_ns": before.st_mtime_ns, "ctime_ns": before.st_ctime_ns}
        return rows, {"historical_pin": previous, "current_pin": current,
                      "content_relation": "SAME_BYTES_FULL_STREAM_SHA256"}
    finally:
        if descriptor is not None:
            os.close(descriptor)
        os.close(parent)


HARDLINK_POLICIES = frozenset(("reject", "historical-complete-intra-root"))


def _hardlink_policy(value):
    require(type(value) is str and value in HARDLINK_POLICIES,
            "explicit known whole-root hardlink policy required")
    return value


class _HistoricalHardlinks:
    """Admit only complete current classes already named by an old catalog.

    An old common.py catalog recorded device/inode aliases, but not st_nlink.
    Therefore it can identify historical members without proving that the old
    inode had no aliases elsewhere. The opt-in policy requires the *current*
    link count to equal exactly the catalog's complete in-root member count.
    No outside alias, newly merged class, split class or inferred old nlink is
    admitted. Every member is fully hashed and actual current topology is
    checked again before this observation is returned.
    """
    def __init__(self, groups, root, guard, limits):
        self.groups, self.root, self.guard, self.limits = groups, root, guard, limits
        self.by_name, self.current, self.members, self.owners = {}, {}, {}, {}
        for identity, group in groups.items():
            group["members"].sort()
            for relative in group["members"]:
                require(relative not in self.by_name, "one historical hardlink class per relative name required")
                self.by_name[relative] = identity

    @staticmethod
    def _metadata(pin, info):
        return {**{key: pin[key] for key in
                   ("bytes", "sha256", "device", "inode", "mtime_ns", "ctime_ns")},
                "mode": info.st_mode, "nlink": info.st_nlink}

    def observe(self, relative, pin, info):
        require(relative in self.by_name, "current file has no historical inode class")
        identity = self.by_name[relative]
        group = self.groups[identity]
        require(info.st_nlink == len(group["members"]),
                "current hardlink count differs from the complete historical in-root name class")
        if len(group["members"]) == 1:
            return
        metadata = self._metadata(pin, info)
        current_identity = (info.st_dev, info.st_ino)
        require(current_identity not in self.owners or self.owners[current_identity] == identity,
                "distinct historical hardlink classes merged into one current inode")
        self.owners[current_identity] = identity
        require(identity not in self.current or self.current[identity] == metadata,
                "historical hardlink members no longer share one stable current inode")
        self.current[identity] = metadata
        selected = self.members.setdefault(identity, {})
        require(relative not in selected, "current hardlink member repeated")
        selected[relative] = dict(pin)

    def finish(self):
        """Fully reopen all multi-name members to close the first walk's race."""
        for identity, group in self.groups.items():
            if len(group["members"]) == 1:
                continue
            require(set(self.members.get(identity, {})) == set(group["members"]),
                    "current hardlink class is missing a historical member")
            for relative in group["members"]:
                path = self.root + "/" + relative
                parent, name = _parent(path)
                try:
                    pin, _, info = _opened_at(parent, name, path, self.limits["file_bytes"], self.guard)
                finally:
                    os.close(parent)
                require(pin == self.members[identity][relative] and
                        self._metadata(pin, info) == self.current[identity],
                        "current hardlink identity, link count or bytes changed after the first walk")

    @property
    def evidence(self):
        return [{"historical_device": identity[0], "historical_inode": identity[1],
                 "historical_members": list(group["members"]),
                 "historical_recorded_metadata": dict(group["metadata"]),
                 "historical_nlink": "NOT_RECORDED",
                 "current_identity": dict(self.current[identity]),
                 "current_complete_in_root_member_count": len(group["members"]),
                 "external_current_hardlinks": "NONE_BY_EXACT_CURRENT_NLINK"}
                for identity, group in sorted(self.groups.items()) if len(group["members"]) > 1]


def requalify_tree(previous_catalog, current_root, output_catalog, guard, limits, *, hardlink_policy="reject"):
    """Compare an exact file/link set, with strict hardlink rejection by default.

    The explicit ``historical-complete-intra-root`` policy may admit only
    catalog-known multi-name inode classes whose entire current link count is
    accounted for inside this exact root. Current nlink/topology is recorded
    separately; the historical catalog never acquires an invented link count.
    """
    policy = _hardlink_policy(hardlink_policy)
    groups = {} if policy == "historical-complete-intra-root" else None
    expected, bridge = _old_catalog(previous_catalog, guard, limits, hardlink_groups=groups)
    current_root = clean_absolute(current_root)
    topology = _HistoricalHardlinks(groups, current_root, guard, limits) if groups is not None else None
    seen = set()
    with open(output_catalog, "xb") as output:
        def visit(row):
            relative = row["relative"]
            content = {key: value for key, value in row.items()
                       if key not in ("current_pin", "current_link", "current_nlink")}
            require(relative in expected and relative not in seen and expected[relative] == content,
                    "current whole-root content differs: " + current_root + "/" + relative)
            seen.add(relative)
            output.write((json.dumps(row, sort_keys=True, separators=(",", ":")) + "\n").encode())
        totals = _walk(current_root, guard, limits, visit,
                       file_identity=topology.observe if topology is not None else None)
        require(seen == set(expected), "historical whole-root files or symlinks are missing")
        if topology is not None:
            topology.finish()
        output.flush()
        os.fsync(output.fileno())
    return {"historical_catalog": bridge, "current_root": current_root, "totals": totals,
            "current_catalog": opened_file(str(output_catalog), limits["catalog_bytes"], guard),
            "content_relation": "EXACT_REGULAR_FILE_AND_SYMLINK_CONTENT_SET",
            "symlink_target_qualification": "NOT_PERFORMED", "execution_qualification": "NOT_PERFORMED",
            "hardlink_policy": policy, "hardlink_topology": topology.evidence if topology is not None else []}


def _source_state(root, manifest, guard, limits):
    require(type(manifest) is dict and 0 < len(manifest) <= limits["catalog_entries"],
            "bounded nonempty full source manifest required")
    expected = {}
    for name, value in manifest.items():
        relative_name(name)
        exact(value, ("bytes", "sha256"), "full source manifest entry")
        require(type(value["bytes"]) is int and 0 <= value["bytes"] <= limits["file_bytes"] and
                type(value["sha256"]) is str and re.fullmatch(r"[0-9a-f]{64}", value["sha256"]) is not None,
                "bounded full source entry required")
        expected[name] = value
    digest, seen = hashlib.sha256(), set()
    def visit(row):
        name = row["relative"]
        require(row["kind"] == "file" and name in expected and name not in seen,
                "source file set differs from the full historical manifest")
        require(expected[name] == {"bytes": row["bytes"], "sha256": row["sha256"]},
                "source bytes differ from the full historical manifest: " + name)
        seen.add(name)
        digest.update((json.dumps(row, sort_keys=True, separators=(",", ":")) + "\n").encode())
    totals = _walk(root, guard, limits, visit)
    require(seen == set(expected), "source is missing full-manifest files")
    return {"root": root, "totals": totals, "current_identity_catalog_sha256": digest.hexdigest(),
            "content_relation": "EXACT_FULL_HISTORICAL_SOURCE_MANIFEST", "execution_qualification": "NOT_PERFORMED"}


def _preserve_tree(initial, guard, limits):
    """Rehash current files and current link identities; do not reuse old pins."""
    policy = _hardlink_policy(initial.get("hardlink_policy", "reject"))
    topology = None
    if policy == "historical-complete-intra-root":
        groups = {}
        _old_catalog(initial["historical_catalog"]["historical_pin"], guard, limits, hardlink_groups=groups)
        topology = _HistoricalHardlinks(groups, initial["current_root"], guard, limits)
    digest, count = hashlib.sha256(), 0
    def visit(row):
        nonlocal count
        line = (json.dumps(row, sort_keys=True, separators=(",", ":")) + "\n").encode()
        digest.update(line)
        count += len(line)
    totals = _walk(initial["current_root"], guard, limits, visit,
                   file_identity=topology.observe if topology is not None else None)
    if topology is not None:
        topology.finish()
        require(topology.evidence == initial["hardlink_topology"],
                "current whole-root hardlink topology changed before terminal admission")
    require(totals == initial["totals"] and count == initial["current_catalog"]["bytes"] and
            digest.hexdigest() == initial["current_catalog"]["sha256"],
            "current whole-root identities/content changed during admission")
    require(opened_file(initial["current_catalog"]["path"], limits["catalog_bytes"], guard) ==
            initial["current_catalog"], "fresh current catalog changed during admission")


def _write_report(output, report):
    pending = output / "report.json.pending"
    descriptor = os.open(pending, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, 0o600)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", closefd=False) as stream:
            json.dump(report, stream, indent=2, sort_keys=True)
            stream.write("\n")
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(pending, output / "report.json")
    finally:
        os.close(descriptor)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--historical-source-root", required=True)
    parser.add_argument("--current-source-root", required=True)
    parser.add_argument("--historical-manifest", required=True)
    parser.add_argument("--historical-manifest-sha256", required=True)
    parser.add_argument("--config", required=True)
    parser.add_argument("--config-sha256", required=True)
    parser.add_argument("--out", required=True)
    args = parser.parse_args()
    require(sys.platform == "linux", "actual Linux requalification lane required")
    old_source = clean_absolute(args.historical_source_root)
    source = clean_absolute(args.current_source_root)
    config_path, manifest_path = clean_absolute(args.config), clean_absolute(args.historical_manifest)
    config, config_pin = _document(config_path, args.config_sha256, lambda: None)
    manifest, manifest_pin = _document(manifest_path, args.historical_manifest_sha256, lambda: None)
    exact(config, ("format_version", "kind", "historical_boot_id", "cgroup", "limits", "files", "trees"),
          "restart content selection")
    require(type(config["format_version"]) is int and config["format_version"] == 1 and
            config["kind"] == "uwvm-ros-restart-content-selection" and
            type(config["historical_boot_id"]) is str and
            re.fullmatch(r"[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}", config["historical_boot_id"]) and
            config["historical_boot_id"] != config["cgroup"]["expected_boot_id"],
            "explicit distinct historical and current boot selections required")
    limits = config["limits"]
    exact(limits, ("file_bytes", "catalog_bytes", "catalog_entries", "total_bytes", "seconds"),
          "content admission limits")
    maxima = {"file_bytes": 16 * 1024**3, "catalog_bytes": 64 * CHUNK, "catalog_entries": 100000,
              "total_bytes": 256 * 1024**3, "seconds": 3600}
    require(all(type(limits[key]) is int and 1 <= limits[key] <= maximum for key, maximum in maxima.items()),
            "finite low-memory content admission limits required")
    require(type(config["files"]) is list and len(config["files"]) <= 4096 and
            type(config["trees"]) is list and len(config["trees"]) <= 64 and
            (config["files"] or config["trees"]), "explicit bounded file/tree selections required")
    guard = CurrentGuard(source, manifest, config["cgroup"])
    guard.maximum_seconds = limits["seconds"]
    # The current source may retain its original physical path after a reboot;
    # keeping both names explicit prevents accidental source-version rebasing.
    roots = [source]
    labels = set()
    selected_file_bytes = 0
    for selected in config["files"]:
        exact(selected, ("label", "historical_pin", "current_path"), "selected file")
        previous = historical_pin(selected["historical_pin"])
        require(previous["bytes"] <= limits["file_bytes"], "selected file exceeds its configured byte bound")
        selected_file_bytes += previous["bytes"]
        require(selected_file_bytes <= limits["total_bytes"], "selected file byte sum exceeds configured bound")
        clean_absolute(selected["current_path"])
    for selected in config["trees"]:
        required = {"label", "historical_root", "historical_catalog_pin", "current_root"}
        require(type(selected) is dict and set(selected) in (required, required | {"hardlink_policy"}),
                "closed selected tree with an optional explicit hardlink policy required")
        _hardlink_policy(selected.get("hardlink_policy", "reject"))
        clean_absolute(selected["historical_root"])
        historical_pin(selected["historical_catalog_pin"])
        roots.append(clean_absolute(selected["current_root"]))
    for selected in config["files"] + config["trees"]:
        require(type(selected["label"]) is str and
                re.fullmatch(r"[a-z][a-z0-9_-]{0,63}", selected["label"]) and selected["label"] not in labels,
                "safe unique selection label required")
        labels.add(selected["label"])
    output = Path(clean_absolute(args.out))
    inputs = [Path(value) for value in roots + [config_path, manifest_path] +
              [row["current_path"] for row in config["files"]] +
              [row["historical_catalog_pin"]["path"] for row in config["trees"]]]
    require(all(output != path and output not in path.parents and path not in output.parents for path in inputs),
            "fresh output must not overlap any selected source/input path")
    parent, name = _parent(str(output))
    try:
        os.mkdir(name, 0o700, dir_fd=parent)
    finally:
        os.close(parent)
    report = {"format_version": 1, "kind": "uwvm-ros-restart-content-requalification",
        "status": "CONTENT_REQUALIFICATION_RUNNING", "execution_qualification": "NOT_PERFORMED",
        "historical_boot_id": config["historical_boot_id"], "historical_source_root": old_source,
        "current_source_root": source, "configuration_current_pin": config_pin,
        "historical_manifest_content_sha256": args.historical_manifest_sha256,
        "manifest_current_pin": manifest_pin, "cgroup_before": guard.before,
        "historical_execution_status_policy": "UNCHANGED_IN_RAW_REPORTS_NO_STATUS_PROMOTION",
        "files": [], "trees": [], "commands": [], "guest_authority": "NO_RIGHTS_GRANTED"}
    _write_report(output, report)
    try:
        before_source = _source_state(source, manifest, guard, limits)
        report["source_before"] = before_source
        for selected in config["files"]:
            bridge = requalify_file(selected["historical_pin"], selected["current_path"], guard)
            report["files"].append({"label": selected["label"], **bridge})
        for selected in config["trees"]:
            result = requalify_tree(selected["historical_catalog_pin"], selected["current_root"],
                output / (selected["label"] + "-current.jsonl"), guard, limits,
                hardlink_policy=selected.get("hardlink_policy", "reject"))
            report["trees"].append({"label": selected["label"], "historical_root": selected["historical_root"], **result})
        for selected in report["files"]:
            initial = selected["current_pin"]
            require(opened_file(initial["path"], initial["bytes"], guard) == initial,
                    "current selected file changed during admission")
        for selected in report["trees"]:
            _preserve_tree(selected, guard, limits)
            catalog = selected["historical_catalog"]["current_pin"]
            require(opened_file(catalog["path"], limits["catalog_bytes"], guard) == catalog,
                    "retained historical catalog changed during admission")
        report["source_after"] = _source_state(source, manifest, guard, limits)
        require(report["source_after"] == before_source, "current full source identity/content changed")
        require(opened_file(config_path, JSON_LIMIT, guard) == config_pin and
                opened_file(manifest_path, JSON_LIMIT, guard) == manifest_pin and
                opened_file(guard.probe_pin["path"], CHUNK, guard) == guard.probe_pin,
                "current controls or individually bound cgroup probe changed")
        report["cgroup_after"] = guard.force()
        report["status"] = "CONTENT_REQUALIFIED"
        report["current_inputs_preserved"] = True
    except BaseException as error:
        report["status"] = "CONTENT_REQUALIFICATION_FAILED"
        report["error"] = repr(error)
        _write_report(output, report)
        raise
    _write_report(output, report)
    print(json.dumps({"status": report["status"], "report": str(output / "report.json"),
                      "execution_qualification": "NOT_PERFORMED"}, sort_keys=True))


if __name__ == "__main__":
    main()
