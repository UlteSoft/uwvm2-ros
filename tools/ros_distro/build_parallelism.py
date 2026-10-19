#!/usr/bin/env python3
"""Choose bounded build concurrency from an already admitted cgroup snapshot.

This module never creates a cgroup, changes a resource limit, starts an actor,
or treats the host's free memory as build capacity. The caller must obtain the
snapshot with ``common.cgroup_state`` and compare its incarnation with the
original admission before using the result. The existing command owner keeps
enforcing total descendant RSS, deadlines, membership and OOM counters while
the build runs; a reservation is an estimate, not an individual compiler limit.
"""
from __future__ import annotations

import os
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
from common import require

GIB = 1024**3
MIB = 1024**2
MAX_JOBS = 64
HOST_RESERVE_BYTES = 8 * GIB
ENVIRONMENT_KEYS = ("UWVM_ROS_BUILD_MAX_JOBS", "UWVM_ROS_BUILD_JOB_RSS_BYTES")


def environment_selection(environment):
    """Copy only the two explicit build knobs, rejecting malformed values.

    The setup script does not supply these controls: the coordinator captures
    them before sourcing any SDK and passes the same reviewed values to a
    matrix child. A job cap may reduce, but never raise, ``limits.jobs``.
    """
    selected = {}
    for key in ENVIRONMENT_KEYS:
        if key not in environment:
            continue
        value = environment[key]
        require(isinstance(value, str) and re.fullmatch(r"[1-9][0-9]{0,10}", value),
                "positive decimal build control required: " + key)
        number = int(value)
        bound = MAX_JOBS if key == ENVIRONMENT_KEYS[0] else 60 * GIB
        require(1 <= number <= bound and
                (key != ENVIRONMENT_KEYS[1] or number >= 64 * MIB),
                "build control outside its finite range: " + key)
        selected[key] = value
    return selected


def cpu_capacity(snapshot, affinity):
    """Count admitted effective CPUs and honor every visible ancestor quota.

    A fractional CPU quota still permits one compiler. Larger quotas are
    rounded down, so this helper never turns a 1.5-CPU quota into two workers.
    The operating system affinity is intersected with the pinned effective
    cpuset rather than using the host-wide processor count.
    """
    effective = set()
    for token in snapshot["cpus"].split(","):
        require(re.fullmatch(r"[0-9]+(?:-[0-9]+)?", token), "invalid admitted CPU range")
        first, _, last = token.partition("-")
        begin, end = int(first), int(last or first)
        require(0 <= begin <= end < 1048576 and end - begin < 65536,
                "bounded admitted CPU range required")
        effective.update(range(begin, end + 1))
    require(isinstance(affinity, (set, frozenset)) and affinity and all(
            type(cpu) is int and cpu >= 0 for cpu in affinity), "actual CPU affinity required")
    capacity = len(effective.intersection(affinity))
    require(capacity > 0, "no CPU belongs to both admitted cpuset and actual affinity")
    quotas = []
    for ancestor in snapshot["ancestors"]:
        if "controllerless_hierarchy_root" in ancestor:
            continue
        fields = ancestor["cpu_max"].split()
        require(len(fields) == 2 and fields[1].isdigit() and int(fields[1]) > 0 and
                (fields[0] == "max" or fields[0].isdigit() and int(fields[0]) > 0),
                "invalid admitted ancestor CPU quota")
        if fields[0] != "max":
            quota = max(1, int(fields[0]) // int(fields[1]))
            capacity = min(capacity, quota)
            quotas.append({"path": ancestor["path"], "cpu_max": ancestor["cpu_max"],
                           "whole_cpu_capacity": quota})
    return capacity, quotas


def plan(snapshot, limits, environment, affinity, host_memory_available_bytes):
    """Return a fresh launch-time job count and the reasons for every cap.

    ``rss_bytes`` remains the existing *aggregate* command-tree ceiling. The
    per-job reservation includes that job's share of make/compiler/linker
    descendants; it is not multiplied into a larger command limit. Its default
    is a conservative 2 GiB, or the existing smaller aggregate ceiling. A
    measured heavier translation unit needs an explicit larger reservation,
    which naturally lowers concurrency. One job is selected when only one
    reservation fits; no child is launched when even that reservation cannot
    retain the original four-GiB cgroup reserve.
    """
    selected = environment_selection(environment)
    require(snapshot["memory_max"] == 64 * GIB and snapshot["memory_swap_max"] == 0 and
            snapshot["memory_current_guard"] == 60 * GIB and
            type(snapshot["memory_current"]) is int and
            0 <= snapshot["memory_current"] <= snapshot["memory_current_guard"],
            "original 64-GiB/swap-zero cgroup and four-GiB reserve required")
    requested = limits["jobs"]
    aggregate = limits["rss_bytes"]
    require(type(requested) is int and 1 <= requested <= MAX_JOBS and
            type(aggregate) is int and 64 * MIB <= aggregate <= 60 * GIB,
            "finite job and aggregate command RSS bounds required")
    maximum = min(requested, int(selected.get(ENVIRONMENT_KEYS[0], requested)))
    per_job = int(selected.get(ENVIRONMENT_KEYS[1], min(2 * GIB, aggregate)))
    require(per_job <= aggregate, "one build reservation exceeds the unchanged aggregate command RSS limit")
    require(type(host_memory_available_bytes) is int and host_memory_available_bytes >= 0,
            "actual host MemAvailable required as an additional safety cap")
    available = snapshot["memory_current_guard"] - snapshot["memory_current"]
    host_available = max(0, host_memory_available_bytes - HOST_RESERVE_BYTES)
    memory_jobs = available // per_job
    host_jobs = host_available // per_job
    aggregate_jobs = aggregate // per_job
    cpu_jobs, quotas = cpu_capacity(snapshot, affinity)
    jobs = min(maximum, memory_jobs, host_jobs, aggregate_jobs, cpu_jobs)
    require(jobs >= 1, "insufficient admitted cgroup/host headroom for one reserved build job")
    return {"format_version": 1, "jobs": jobs, "profile_max_jobs": requested,
            "selected_max_jobs": maximum, "environment_selection": selected,
            "per_job_reservation_bytes": per_job,
            "aggregate_owned_rss_limit_bytes": aggregate,
            "memory_headroom_bytes": available, "memory_capacity_jobs": memory_jobs,
            "host_memory_available_bytes": host_memory_available_bytes,
            "host_memory_reserve_bytes": HOST_RESERVE_BYTES,
            "host_memory_headroom_bytes": host_available, "host_memory_capacity_jobs": host_jobs,
            "aggregate_rss_capacity_jobs": aggregate_jobs, "cpu_capacity_jobs": cpu_jobs,
            "affinity": sorted(affinity), "finite_ancestor_cpu_quotas": quotas,
            "cgroup_snapshot": snapshot,
            "reservation_scope": "ESTIMATE_WITH_UNCHANGED_AGGREGATE_RSS_AND_CGROUP_GUARDS"}


def current_affinity():
    """Require the actual Linux coordinator affinity; never guess from nproc."""
    require(hasattr(os, "sched_getaffinity"), "Linux CPU affinity required for build planning")
    return os.sched_getaffinity(0)


def current_host_memory_available():
    """Read the real procfs observation without allocating an unbounded file.

    MemAvailable is an additional conservative cap, never permission to exceed
    the original cgroup or its reserve. Fail closed if this kernel/container
    cannot expose an unambiguous reading instead of falling back to host RAM.
    """
    with Path("/proc/meminfo").open("rb") as stream:
        raw = stream.read(16385)
    require(0 < len(raw) <= 16384, "bounded actual /proc/meminfo required")
    matches = re.findall(rb"(?m)^MemAvailable:[ \t]+([0-9]+)[ \t]+kB[ \t]*$", raw)
    require(len(matches) == 1, "one actual host MemAvailable observation required")
    return int(matches[0]) * 1024
