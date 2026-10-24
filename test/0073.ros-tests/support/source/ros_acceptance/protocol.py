"""Closed, independently checkable evidence for the remaining ROS tests.

Reports are evidence containers rather than permissions. A report is accepted
only after its physical input hashes, same-incarnation cgroup proof, real
process mappings, native observations and cleanup have all been checked. A
preparation report can never satisfy a behavior case. No historical receipt is promoted to a current 64 GiB acceptance result.
"""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import re
import stat

MEMORY_MAX = 64 * 1024**3
MAX_REPORT = 64 * 1024**2
SHA = re.compile(r"[0-9a-f]{64}\Z")
GID = re.compile(r"[0-9a-f]{32}\Z")
STATUSES = frozenset(("PASS", "FAIL", "SKIP", "BLOCKED", "PREPARED", "RUNNING"))
LANGUAGES = ("c", "cpp", "rust", "tinygo", "assemblyscript", "javascript")
SDK_CASES = tuple(f"sdk-{language}-ros{version}-{mode}" for language in LANGUAGES
                  for version in (1, 2) for mode in ("positive", "absent"))
RECOVERY_CASES = ("master-restart", "tcpros-peer-restart", "dds-peer-restart")
MEMORY_CASES = ("memory1-ros1", "memory1-ros2", "memory1-dual")
ATTACK_CASES = ("graph-query-flood-ros1", "graph-query-flood-ros2",
                "wrong-uid-ros1", "wrong-uid-ros2", "raw-frame-ros1", "raw-frame-ros2")
METRIC_CASES = ("queue-reject", "queue-drop-oldest", "isolated-ipc")
CASES = RECOVERY_CASES + MEMORY_CASES + ATTACK_CASES + METRIC_CASES + SDK_CASES


def require(condition, message):
    """Fail closed; callers decide whether unavailable prerequisites are BLOCKED."""
    if not condition:
        raise ValueError(message)


def integer(value, minimum=0, maximum=(1 << 64)-1):
    return type(value) is int and minimum <= value <= maximum


def physical_pin(path, limit=MAX_REPORT):
    """Hash one bounded, unchanged regular descriptor, without following links."""
    path = Path(path)
    require(path.is_absolute(), "absolute evidence path required")
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        require(stat.S_ISREG(before.st_mode) and 0 <= before.st_size <= limit,
                "bounded regular evidence file required")
        digest = hashlib.sha256(); data = bytearray()
        while True:
            part = os.read(fd, min(65536, limit + 1 - len(data)))
            if not part:
                break
            data.extend(part); digest.update(part)
            require(len(data) <= limit, "evidence exceeded bound")
        after = os.fstat(fd)
        fields = ("st_dev", "st_ino", "st_size", "st_mtime_ns", "st_ctime_ns")
        require(all(getattr(before, key) == getattr(after, key) for key in fields)
                and len(data) == before.st_size, "evidence changed while hashing")
        return bytes(data), {"path": str(path), "sha256": digest.hexdigest(),
                             "bytes": len(data), "device": before.st_dev,
                             "inode": before.st_ino}
    finally:
        os.close(fd)


def load_bound(path):
    body, pin = physical_pin(path)
    def no_duplicates(pairs):
        result = {}
        for key, value in pairs:
            require(key not in result, "duplicate JSON evidence key")
            result[key] = value
        return result
    return json.loads(body, object_pairs_hook=no_duplicates), pin


def marker_lines(text, prefix, expected_fields, complete=True):
    """Parse exact ordered ASCII fields; no regex-valid unknown body is echoed.

    A trailing fragment is allowed only while a growing log is being polled.
    The final receipt must contain LF-terminated records. Diagnostic FAIL lines
    are refusals, never successful native observations.
    """
    require(isinstance(text, str) and len(text.encode("utf-8")) <= 1024**2,
            "bounded marker log required")
    require("\r" not in text and "\x00" not in text, "noncanonical marker control byte")
    if complete:
        require(not text or text.endswith("\n"), "unterminated final marker")
    lines = text.split("\n")[:-1]
    rows = []
    for line in lines:
        if not line.startswith(prefix):
            continue
        parts = line.split(" ")
        require(parts[0] in expected_fields and all(parts), "unknown marker")
        fields = {}
        for token in parts[1:]:
            require(token.count("=") == 1, "invalid marker token")
            key, value = token.split("=", 1)
            require(key not in fields and value and value.isascii(), "duplicate/empty marker field")
            fields[key] = value
        require(tuple(fields) == tuple(expected_fields[parts[0]]), "closed marker schema differs")
        rows.append((parts[0], fields))
    return rows


def architecture(row):
    """Verify current budget/incarnation and every measured ancestor's OOM deltas."""
    require(row.get("status") == "PASS" and row.get("same_incarnation") is True,
            "actual current architecture proof required")
    require(row.get("memory_max") == MEMORY_MAX and row.get("swap_max") == 0,
            "current 64GiB/swap0 required")
    before, after = row["before"], row["after"]
    require(before == after and before.get("boot_id") and
            integer(before.get("device"), 1) and integer(before.get("inode"), 1),
            "leaf birth changed")
    require(row.get("readable_ancestors") is True and row.get("zero_oom") is True,
            "complete ancestor OOM proof required")
    require(row.get("fatal_errors") == [] and row.get("remaining_owned_processes") == []
            and row.get("remaining_owned_mounts") == [], "architecture cleanup incomplete")


def mapped_role(row, expected_membership):
    """A command line alone is never proof of executing the selected ARM image."""
    require(integer(row.get("pid"), 1) and integer(row.get("starttime_ticks"), 1),
            "process birth missing")
    require(row.get("membership") == expected_membership and row.get("live_cgroup_procs") is True,
            "actual role leaf membership missing")
    require(row.get("mapped_product_sha256") == row.get("selected_product_sha256")
            and SHA.fullmatch(row.get("mapped_product_sha256", "")) is not None,
            "current selected product was not mapped")
    require(row.get("target_elf_machine") == 183 and row.get("kernel_exe_elf_machine") == 62
            and SHA.fullmatch(row.get("qemu_sha256", "")) is not None
            and row.get("stopped_before_release") is True, "stopped ARM/QEMU proof absent")


def queue_samples(samples, depth_limit, byte_limit):
    """Validate real queue snapshots; configured capacity is not occupancy.

    Each snapshot is generated under the native queue's mutex. Monotonic
    counters and conservation expose a fabricated high-water or dropped count.
    This conservation predicate covers queued topic records before retirement;
    reserved async completion credits have a separate explicit fixture oracle.
    """
    require(samples and integer(depth_limit, 1) and integer(byte_limit, 1), "queue sample prerequisites")
    previous = None
    names = ("depth", "bytes", "pending_depth", "pending_bytes", "reserved_depth", "reserved_bytes",
             "high_water_depth", "high_water_bytes", "enqueued", "dequeued", "dropped_oldest", "dropped_newest",
             "rejected", "backpressured", "admission_refused", "completed", "closed")
    for sample in samples:
        require(all(integer(sample.get(key)) for key in names), "queue counter invalid")
        require(sample["depth"]+sample["pending_depth"] <= sample["high_water_depth"] <= depth_limit
                and sample["bytes"]+sample["pending_bytes"] <= sample["high_water_bytes"] <= byte_limit,
                "queue bounds/high-water differ")
        require(sample.get("conservation_scope") == "queued-topic-before-retirement" and not sample["reserved_depth"]
                and not sample["closed"] and sample["enqueued"] == sample["dequeued"] + sample["dropped_oldest"]
                + sample["depth"]+sample["pending_depth"], "queue record conservation scope differs")
        if previous is not None:
            require(all(sample[key] >= previous[key] for key in names[6:]), "queue counter regressed")
        previous = sample
    require(any(sample["depth"] > 0 for sample in samples), "queue never observed occupied")


def side_effect(row):
    """Every negative must bind errno and independent real-native effect count."""
    require(integer(row.get("expected_errno"), 1) and integer(row.get("actual_errno"),1) and row.get("actual_errno") == row["expected_errno"],
            "attack expected errno differs")
    require(row.get("output_sentinels_unchanged") is True,
            "attack output changed on refusal")
    before,after=row.get("native_effects_before"),row.get("native_effects_after")
    require(isinstance(before,dict) and bool(before) and isinstance(after,dict)
            and all(type(key) is str and 1<=len(key)<=64 and key.isascii() and key.replace("_", "").replace("-", "").isalnum()
                    and integer(value) for key,value in before.items()),"explicit measured native counter inventory required")
    require(row.get("native_observer_independent") is True and before == after
            and all(integer(value) for value in after.values()),
            "denied operation had a native side effect or no independent observation")
    require(row.get("observer_kind") in ("tcpros", "dds", "master-graph-query", "native-graph-query-hook"),
            "native observation scope invalid")
    require(integer(row.get("quiet_samples"), 30) and integer(row.get("quiet_ns"), 2_000_000_000),
            "negative no-effect observation too short")


def completion(row):
    """Reject incomplete source-bound cases; never count SKIP/BLOCKED as PASS."""
    require(row.get("format_version") == 1 and type(row.get("format_version")) is int
            and row.get("kind") == "ros-remaining-acceptance", "unknown report format")
    require(row.get("case") in CASES and row.get("status") == "PASS"
            and row.get("mode") == "execute", "actual completed case required")
    require(SHA.fullmatch(row.get("source_manifest_sha256", "")) is not None,
            "source manifest pin missing")
    for name in ("source_preserved", "sdk_preserved", "inputs_preserved", "products_preserved"):
        require(row.get(name) is True, "immutable input binding failed")
    architecture(row["architecture"])
    roles = row["roles"]
    require(roles and len({(item["pid"], item["starttime_ticks"]) for item in roles}) == len(roles),
            "distinct actual roles required")
    for role in roles:
        mapped_role(role, row["architecture"]["membership"])
    require(row.get("cleanup") == {"processes": [], "mounts": [], "sockets": [], "leases": []},
            "task-owned cleanup incomplete")
    require(row.get("behavior_assertions_complete") is True, "behavior not established")
    return True
