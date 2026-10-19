#!/usr/bin/env python3
"""Capture only the selected ROS setup's build environment into owned metadata.

This program is executed by the explicitly selected host Python after a checked
``source setup && exec`` chain. It never reads an ambient shell startup file.
Unknown environment variables are not copied into compiler or native commands.
"""
import json
import os
from pathlib import Path
import sys

sys.dont_write_bytecode = True
from common import atomic_json, require
from profile import ROS1_HISTORICAL

KEYS = ("PATH", "LD_LIBRARY_PATH", "DYLD_LIBRARY_PATH", "PYTHONPATH", "CMAKE_PREFIX_PATH",
        "AMENT_PREFIX_PATH", "COLCON_PREFIX_PATH", "ROS_PACKAGE_PATH", "PKG_CONFIG_PATH",
        "ROS_DISTRO", "ROS_VERSION", "ROS_PYTHON_VERSION")


def classify_setup(profile, environment):
    """Retain absent historical metadata without inventing an SDK identity.

    Pre-Groovy source trees can predate modern ROS_DISTRO hooks. A required
    physical source lock permits that absence. Any nonempty contradictory
    export still fails. This is not an upstream authenticity certificate and
    does not replace any actual feature, link or native behavior requirement.
    """
    distro = environment.get("ROS_DISTRO")
    if profile["distro"] == "legacy":
        scope = "USER_LEGACY_SOURCE_LOCK_LABEL"
    elif profile["distro"] in ROS1_HISTORICAL and not distro:
        require(profile["sdk"]["source_lock"] is not None, "historical setup without metadata requires source lock")
        scope = "HISTORICAL_LOCKED_SOURCE_LABEL_NO_SETUP_DISTRO_EXPORT"
    else:
        require(distro == profile["distro"], "sourced ROS_DISTRO differs from selected profile")
        scope = "SETUP_EXPORTED_EXACT"
    require(environment.get("ROS_VERSION", str(profile["backend"])) == str(profile["backend"]),
            "sourced ROS_VERSION differs from selected backend")
    return {"requested_distro": profile["distro"], "sourced_ros_distro": distro,
            "sourced_ros_version": environment.get("ROS_VERSION"), "identity_scope": scope,
            "upstream_release_authenticity": "NOT_CERTIFIED_BY_THIS_CAPTURE"}


def main():
    require(len(sys.argv) == 3, "environment.py PROFILE OUTPUT")
    profile = json.loads(Path(sys.argv[1]).read_text())
    environment = {key: os.environ[key] for key in KEYS if key in os.environ}
    identity = classify_setup(profile, environment)
    environment.update({"HOME": str(Path(sys.argv[2]).parent / "home"), "LC_ALL": "C",
                        "LANG": "C", "PYTHONDONTWRITEBYTECODE": "1", "MAKEFLAGS": "-j1",
                        "CMAKE_BUILD_PARALLEL_LEVEL": "1"})
    runtime = profile["runtime"]
    if profile["backend"] == 1:
        environment["ROS_MASTER_URI"] = runtime["master_uri"]
        environment["ROS_IP"] = "127.0.0.1"
    else:
        environment["RMW_IMPLEMENTATION"] = runtime["rmw"]
        environment["ROS_DOMAIN_ID"] = str(runtime["domain_id"])
        environment["ROS_LOCALHOST_ONLY"] = "1"
    # A setup cannot silently opt a run into permissive SROS2 behavior. Security
    # acceptance has its own explicit policy and native witness, outside this
    # basic SDK-topic scope; no keystore or security-grant variables are copied.
    atomic_json(Path(sys.argv[2]), environment)
    atomic_json(Path(sys.argv[2]).with_name("setup-environment-identity.json"), identity)


if __name__ == "__main__":
    main()
