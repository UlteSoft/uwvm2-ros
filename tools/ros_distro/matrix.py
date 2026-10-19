#!/usr/bin/env python3
"""Execute explicit native ROS distribution profiles strictly one at a time.

No image is pulled and no package is installed. OCI/rootfs descriptors are
configuration-only and automatic daemon launch is explicitly refused until
original-cgroup containment can be proved. Use a host profile inside an already
admitted user container, or a host cross SDK/sysroot. Reports preserve unavailable,
failed, build-covered and narrowly validated cells without turning skips into
passes. No Worker or Guest permission flag is inferred from this matrix.
"""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import platform
import re
import sys
import uuid

sys.dont_write_bytecode = True
from common import (SerialRunner, atomic_json, body_pin, cgroup_preserved, cgroup_state,
                    document, install_interrupts, opened, require, source_state)
from profile import load
from probe import initial_environment

CATALOG = Path(__file__).with_name("distributions.json")


def command(profile, source, manifest, profile_path, out, token):
    """Launch only inside the already admitted caller cgroup.

    A Docker/Podman daemon can place a container outside that cgroup even when
    its memory.max happens to be identical. Until a separate stopped-init,
    cgroup-parent and host-pidfd admission protocol exists, automatic OCI/rootfs
    launch is refused before any engine process is started. Users can run this
    same host profile inside their explicitly provisioned and admitted container.
    """
    require(profile["execution"]["kind"] == "host",
            "UNSUPPORTED_UNPROVEN_CGROUP_CONTAINMENT: enter an explicitly admitted container and use a host profile")
    return [profile["execution"]["python"], "-B", str(source / "tools/ros_distro/build.py"),
            "--profile", str(profile_path), "--source", str(source),
            "--source-manifest", str(manifest), "--out", str(out / "cell")]


def main():
    cancellation = install_interrupts()
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--profiles", required=True, type=Path,
                        help="Closed JSON {schema_version:1,profiles:[absolute profile paths]}")
    parser.add_argument("--source", required=True, type=Path)
    parser.add_argument("--source-manifest", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    parser.add_argument("--require-behavior", action="store_true",
                        help="Fail unless every selected cell actually validates its native SDK topic")
    parser.add_argument("--minimum-distributions", type=int, default=1)
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    require(Path(__file__).resolve() == source / "tools/ros_distro/matrix.py",
            "invoke the matrix tool from the selected frozen source")
    manifest = args.source_manifest.resolve(strict=True)
    specification, spec_pin = document(args.profiles.resolve(strict=True))
    require(set(specification) == {"schema_version", "profiles"} and specification["schema_version"] == 1 and
            isinstance(specification["profiles"], list) and 0 < len(specification["profiles"]) <= 128,
            "bounded closed profile-selection document required")
    require(1 <= args.minimum_distributions <= 128, "invalid minimum-distributions")
    selections = []
    for value in specification["profiles"]:
        require(isinstance(value, str) and Path(value).is_absolute(), "absolute profile path required")
        path = Path(value).resolve(strict=True)
        profile, pin = load(path)
        selections.append((path, profile, pin))
    require(len({p["id"] for _, p, _ in selections}) == len(selections), "duplicate profile id")
    require(len({(p["backend"], p["distro"]) for _, p, _ in selections}) >= args.minimum_distributions,
            "insufficient explicitly selected distributions")
    out = args.out.resolve()
    require(not out.exists() and source not in out.parents and out not in source.parents,
            "fresh matrix output outside source required")
    out.mkdir(parents=True, mode=0o700)
    index, catalog_pin = document(CATALOG.resolve(strict=True))
    before_source = source_state(source, manifest)
    report = {"format_version": 1, "kind": "ros-distribution-matrix", "status": "RUNNING",
              "scope": "NATIVE_GATEWAY_BUILDS_AND_OPTIONAL_BASIC_SDK_TOPICS",
              "guest_authority": "EMPTY_UNCHANGED", "source": before_source,
              "selection": spec_pin, "catalog": catalog_pin, "cells": [],
              "targets": [{**row, "status": "UNVERIFIED_NOT_SELECTED"} for row in index["distributions"]]}
    atomic_json(out / "report.json", report)
    for path, profile, profile_pin in selections:
        cell_out = out / profile["id"]
        cell_out.mkdir()
        # The inner builder receives exactly the opened configuration bytes in
        # a new owned copy; source paths embedded by the user remain explicit.
        data, same_pin = body_pin(path)
        require(same_pin == profile_pin, "profile changed after selection")
        physical_profile = cell_out / "profile.json"
        with physical_profile.open("xb") as stream:
            stream.write(data)
        token = uuid.uuid4().hex
        cell = {"id": profile["id"], "backend": profile["backend"], "distro": profile["distro"],
                "target": profile["target"], "status": "RUNNING", "profile": profile_pin,
                "actual_build": "NOT_PERFORMED", "actual_behavior": "NOT_PERFORMED"}
        report["cells"].append(cell)
        atomic_json(out / "report.json", report)
        runner = None
        environment = initial_environment(profile, cell_out)
        try:
            require(profile["execution"]["kind"] == "host",
                    "UNSUPPORTED_UNPROVEN_CGROUP_CONTAINMENT: automatic OCI/rootfs actors are not admitted")
            host_arch = {"x86_64": "amd64", "aarch64": "arm64", "armv7l": "arm", "i386": "386", "i686": "386"}.get(platform.machine(), platform.machine())
            require(profile["execution"]["platform"].split("/")[1] == host_arch,
                    "tool container/host platform differs from actual coordinator architecture; no implicit binfmt")
            guard_before = cgroup_state(profile["runtime"])
            # This child coordinates up to nine serial commands plus complete
            # SDK/source streaming checks. Its outer deadline is a derived
            # finite sum, not the single compiler's shorter deadline. The
            # inner command limit is unchanged; 64 MiB accounts for its Python
            # coordinator in the outer process-tree RSS ceiling.
            outer_limits = dict(profile["limits"])
            outer_limits["command_seconds"] = 10 * profile["limits"]["command_seconds"] + 600
            outer_limits["rss_bytes"] += 64 * 1024**2
            cell["outer_coordinator_limits"] = outer_limits
            runner = SerialRunner(cell_out, outer_limits,
                                  lambda: cgroup_preserved(guard_before, cgroup_state(profile["runtime"])))
            runner.run("profile-build", command(profile, source, manifest, physical_profile, cell_out, token),
                       cell_out, environment)
            actual, actual_pin = document(cell_out / "cell/report.json")
            require(actual.get("status") == "PASS" and actual.get("profile") == profile and
                    actual.get("source_manifest", {}).get("sha256") == before_source["manifest"]["sha256"] and
                    actual.get("cleanup") == {"status": "PASS", "remaining": []} and actual.get("inputs_preserved") is True,
                    "actual profile report did not preserve source/SDK/cleanup")
            cell.update({"report": actual_pin, "actual_build": actual["build_status"],
                         "actual_behavior": actual["behavior_status"], "status": "BUILD_COVERED"})
            if actual["behavior_status"] == "VALIDATED":
                cell["status"] = "VALIDATED_NATIVE_SDK_TOPIC_ONLY"
            require(not args.require_behavior or actual["behavior_status"] == "VALIDATED",
                    "required actual native behavior was not performed")
        except BaseException as error:
            cell["status"] = "FAIL"
            cell["error"] = repr(error)
            if profile["execution"]["kind"] != "host":
                cell["compatibility"] = "UNSUPPORTED_UNPROVEN_CGROUP_CONTAINMENT"
            if (cell_out / "cell/report.json").is_file():
                actual, cell["report"] = document(cell_out / "cell/report.json")
                cell["actual_build"] = actual.get("build_status", "NOT_PERFORMED")
                cell["actual_behavior"] = actual.get("behavior_status", "NOT_PERFORMED")
                if actual.get("feature_status") == "UNSUPPORTED":
                    cell["compatibility"] = "UNSUPPORTED_REQUIRED_FEATURES"
        finally:
            try:
                if runner:
                    cell["owned_process_cleanup"] = runner.close()
                    cell["outer_commands"] = runner.commands
                    require(cell["owned_process_cleanup"]["status"] == "PASS",
                            "owned process cleanup incomplete")
                    cgroup_preserved(guard_before, cgroup_state(profile["runtime"]))
                require(opened(path) == profile_pin, "selected profile changed")
            except BaseException as error:
                cell["status"] = "FAIL"
                cell["cleanup_error"] = repr(error)
            atomic_json(out / "report.json", report)
        if cancellation["cancelled"]:
            report["cancelled"] = True
            break
    for target in report["targets"]:
        selected = [row["id"] for row in report["cells"] if row["backend"] == target["backend"] and
                    row["distro"] == target["distro"]]
        if selected:
            target["status"] = "SELECTED_SEE_ACTUAL_CELLS"
            target["selected_profiles"] = selected
    try:
        require(source_state(source, manifest) == before_source, "frozen source changed")
        require(opened(Path(spec_pin["path"])) == spec_pin, "selection changed")
        report["inputs_preserved"] = True
    except BaseException as error:
        report["inputs_preserved"] = False
        report["preservation_error"] = repr(error)
    report["status"] = "PASS" if report["inputs_preserved"] and all(
        row["status"] in ("BUILD_COVERED", "VALIDATED_NATIVE_SDK_TOPIC_ONLY") for row in report["cells"]) else "FAIL"
    report["counts"] = {"selected": len(report["cells"]), "failed": sum(row["status"] == "FAIL" for row in report["cells"]),
                        "native_sdk_topic_validated": sum(row["status"] == "VALIDATED_NATIVE_SDK_TOPIC_ONLY" for row in report["cells"])}
    atomic_json(out / "report.json", report)
    print(json.dumps({"status": report["status"], "counts": report["counts"], "report": str(out / "report.json")}))
    return 0 if report["status"] == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
