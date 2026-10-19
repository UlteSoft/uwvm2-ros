#!/usr/bin/env python3
"""Validate explicit distribution/toolchain selections without running actors.

The catalog is a compatibility target inventory, not a list of passed tests.
Private source overlays use ``distro=legacy`` with an explicit description;
five official pre-Groovy names have distinct locked-source targets. No
distribution, architecture, or profile enables a ROS
capability: Guest authority still starts empty and is granted only by the CLI.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
from common import document, require

ARCHES = {"x86_64": (62, 64, "little"), "i386": (3, 32, "little"), "aarch64": (183, 64, "little"),
          "i686": (3, 32, "little"), "armel": (40, 32, "little"),
          "armhf": (40, 32, "little"), "armeb": (40, 32, "big"),
          "armebhf": (40, 32, "big"), "aarch64_be": (183, 64, "big"),
          "aarch64be": (183, 64, "big"),
          "arm": (40, 32, "little"), "riscv64": (243, 64, "little"),
          "loongarch64": (258, 64, "little"), "ppc64le": (21, 64, "little"),
          "s390x": (22, 64, "big"), "mips": (8, 32, "big"),
          "mipsel": (8, 32, "little"), "mips64": (8, 64, "big"),
          "mips64el": (8, 64, "little")}
ROS1_HISTORICAL = ("boxturtle", "cturtle", "diamondback", "electric", "fuerte")
ROS1_INDEXED = ("groovy", "hydro", "indigo", "jade", "kinetic", "lunar", "melodic", "noetic")
ROS1 = ROS1_HISTORICAL + ROS1_INDEXED
ROS2 = ("ardent", "bouncy", "crystal", "dashing", "eloquent", "foxy", "galactic",
        "humble", "iron", "jazzy", "kilted", "lyrical", "rolling")
FIELDS = {"schema_version", "id", "backend", "distro", "legacy_description", "target",
          "execution", "sdk", "tools", "cxx", "runtime", "limits", "custom", "features", "cmake_options"}


def exact(value, keys, label):
    require(isinstance(value, dict) and set(value) == set(keys), "closed " + label + " fields required")


def absolute(value, label, nullable=False):
    if nullable and value is None:
        return
    require(isinstance(value, str) and Path(value).is_absolute() and "\0" not in value and
            "\n" not in value and "\r" not in value, "explicit absolute " + label + " required")


def validate(value):
    """Validate configuration only; existence and content are checked on execution."""
    exact(value, FIELDS, "profile")
    require(value["schema_version"] == 1, "unsupported profile schema")
    require(isinstance(value["id"], str) and re.fullmatch(r"[a-z0-9][a-z0-9_.-]{0,79}", value["id"]),
            "safe unique profile id required")
    require(type(value["backend"]) is int and value["backend"] in (1, 2), "backend must be exactly 1 or 2")
    require(value["distro"] in (ROS1 if value["backend"] == 1 else ROS2) or
            value["distro"] == "legacy", "unknown distro: use explicit legacy profile")
    require(isinstance(value["legacy_description"], str) and
            (bool(value["legacy_description"].strip()) if value["distro"] == "legacy" else
             value["legacy_description"] == ""), "legacy source provenance must be explicit")
    target = value["target"]
    exact(target, {"architecture", "triple", "elf_machine", "bits", "endian"}, "target")
    require(isinstance(target["triple"], str) and re.fullmatch(r"[A-Za-z0-9_.+-]+", target["triple"]),
            "explicit compiler target triple required")
    if target["architecture"] in ARCHES:
        require((target["elf_machine"], target["bits"], target["endian"]) == ARCHES[target["architecture"]],
                "architecture label differs from ELF identity")
    else:
        require(re.fullmatch(r"[a-z0-9_]+", target["architecture"]) and
                isinstance(target["elf_machine"], int) and 0 < target["elf_machine"] < 65536 and
                target["bits"] in (32, 64) and target["endian"] in ("little", "big"),
                "extension architecture needs explicit ELF identity")
    execution = value["execution"]
    exact(execution, {"kind", "engine", "image", "rootfs", "python", "network", "user", "mounts", "platform"}, "execution")
    require(execution["kind"] in ("host", "oci", "rootfs"), "unknown execution kind")
    absolute(execution["python"], "execution Python")
    require(execution["network"] in ("none", "host"), "network must be explicit none or host")
    require(isinstance(execution["platform"], str) and re.fullmatch(r"linux/[a-z0-9_]+(?:/v[0-9]+)?", execution["platform"]),
            "explicit OCI/tool-host platform required")
    require(isinstance(execution["user"], str) and re.fullmatch(r"[0-9]+:[0-9]+", execution["user"]),
            "explicit numeric container uid:gid required")
    require(isinstance(execution["mounts"], list) and len(execution["mounts"]) <= 16,
            "bounded explicit read-only SDK mounts required")
    for mount in execution["mounts"]:
        exact(mount, {"source", "target"}, "SDK mount")
        absolute(mount["source"], "SDK mount source")
        absolute(mount["target"], "SDK mount target")
    if execution["kind"] == "host":
        require(all(execution[k] is None for k in ("engine", "image", "rootfs")), "host cannot select OCI inputs")
        require(not execution["mounts"], "host cannot silently remap SDK roots")
    else:
        absolute(execution["engine"], "OCI engine")
        if execution["kind"] == "oci":
            require(isinstance(execution["image"], str) and re.fullmatch(
                    r"[^\s@]+@sha256:[0-9a-f]{64}", execution["image"]), "digest-pinned OCI image required")
            require(execution["rootfs"] is None, "OCI and rootfs are independent selections")
        else:
            absolute(execution["rootfs"], "user rootfs")
            require(execution["image"] is None, "rootfs selection does not invent an image digest")
    sdk = value["sdk"]
    exact(sdk, {"roots", "ros_prefix", "setup", "distribution_index", "source_lock"}, "SDK")
    require(isinstance(sdk["roots"], list) and sdk["roots"] and len(sdk["roots"]) <= 16 and
            len(set(sdk["roots"])) == len(sdk["roots"]), "explicit distinct SDK roots required")
    for path in sdk["roots"]:
        absolute(path, "SDK root")
    for key in ("ros_prefix", "setup", "distribution_index"):
        absolute(sdk[key], key)
    absolute(sdk["source_lock"], "source/archive lock", nullable=True)
    require(value["distro"] not in ("legacy",) + ROS1_HISTORICAL or sdk["source_lock"] is not None,
            "legacy/pre-Groovy named profile requires immutable source-lock metadata")
    tools = value["tools"]
    exact(tools, {"cxx", "cmake", "make", "bash", "host_python", "generator_python",
                  "toolchain_file", "sysroot", "roots"}, "tools")
    for key in ("cxx", "cmake", "make", "bash", "host_python", "generator_python"):
        absolute(tools[key], key)
    for key in ("toolchain_file", "sysroot"):
        absolute(tools[key], key, nullable=True)
    require(isinstance(tools["roots"], list) and 0 < len(tools["roots"]) <= 16 and
            len(set(tools["roots"])) == len(tools["roots"]), "whole toolchain roots required")
    for path in tools["roots"]:
        absolute(path, "toolchain root")
    cxx = value["cxx"]
    exact(cxx, {"standard", "flags", "link_flags", "stdlib_abi"}, "C++")
    require(cxx["standard"] in (17, 20, 23), "native SDK compiler needs explicitly selected C++17/20/23")
    require(isinstance(cxx["stdlib_abi"], str) and cxx["stdlib_abi"].strip(), "stdlib ABI description required")
    for key in ("flags", "link_flags"):
        require(isinstance(cxx[key], list) and len(cxx[key]) <= 32 and all(
                isinstance(x, str) and x and not any(c in x for c in ("\0", "\n", ";")) for x in cxx[key]),
                "bounded explicit compiler flag tokens required")
    runtime = value["runtime"]
    exact(runtime, {"mode", "qemu", "loader", "rmw", "master_uri", "domain_id",
                    "memory_max", "memory_swap_max", "behavior", "expected_boot_id",
                    "expected_leaf", "expected_device", "expected_inode", "expected_cpus"}, "runtime")
    require(runtime["mode"] in ("native", "qemu", "build-only"), "explicit runtime mode required")
    absolute(runtime["qemu"], "QEMU", nullable=True)
    absolute(runtime["loader"], "target dynamic loader", nullable=True)
    if runtime["mode"] == "qemu":
        require(runtime["qemu"] is not None and runtime["loader"] is not None and
                tools["sysroot"] is not None, "user-QEMU requires its own sysroot, loader and executable")
    else:
        require(runtime["qemu"] is None, "native/build-only must not silently use QEMU")
    if runtime["mode"] == "native":
        names = {"amd64": "x86_64", "arm64": "aarch64", "386": "i386", "ppc64le": "ppc64le"}
        platform_arch = execution["platform"].split("/")[1]
        require(names.get(platform_arch, platform_arch) == target["architecture"],
                "native mode requires the actual tool-host architecture; use explicit QEMU otherwise")
    require(runtime["memory_max"] == 68719476736 and runtime["memory_swap_max"] == 0,
            "current independent matrix requires explicit 64 GiB and zero swap")
    require(runtime["behavior"] in ("native-sdk-topic", "none"), "unknown behavior scope")
    require(isinstance(runtime["domain_id"], int) and 0 <= runtime["domain_id"] <= 232,
            "explicit isolated ROS2 domain required")
    require(isinstance(runtime["rmw"], str) and (value["backend"] == 1 and runtime["rmw"] == "" or
            value["backend"] == 2 and re.fullmatch(r"[a-z][a-z0-9_]*", runtime["rmw"])), "explicit RMW required for ROS2")
    require(isinstance(runtime["master_uri"], str) and (value["backend"] == 2 and runtime["master_uri"] == "" or
            value["backend"] == 1 and re.fullmatch(r"http://127\.0\.0\.1:[0-9]{1,5}", runtime["master_uri"])),
            "ROS1 master must be an explicit loopback endpoint; run it separately")
    require(isinstance(runtime["expected_boot_id"], str) and re.fullmatch(
            r"[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}", runtime["expected_boot_id"]), "invalid expected boot id")
    absolute(runtime["expected_leaf"], "original expected cgroup leaf")
    require(type(runtime["expected_device"]) is int and runtime["expected_device"] >= 0 and
            type(runtime["expected_inode"]) is int and runtime["expected_inode"] > 0 and
            isinstance(runtime["expected_cpus"], str) and re.fullmatch(r"[0-9,-]+", runtime["expected_cpus"]),
            "complete original cgroup device/inode/CPU pins required")
    limits = value["limits"]
    exact(limits, {"jobs", "rss_bytes", "command_seconds", "log_bytes"}, "limits")
    require(type(limits["jobs"]) is int and 1 <= limits["jobs"] <= 64 and
            64 * 1024**2 <= limits["rss_bytes"] <= 4 * 1024**3 and
            1 <= limits["command_seconds"] <= 7200 and 65536 <= limits["log_bytes"] <= 64 * 1024**2,
            "memory-bounded compiler policy required")
    custom = value["custom"]
    exact(custom, {"registry", "native", "packages"}, "custom provider")
    for key in ("registry", "native"):
        absolute(custom[key], "custom " + key, nullable=True)
    require(isinstance(custom["packages"], list) and len(set(custom["packages"])) == len(custom["packages"]) and
            all(re.fullmatch(r"[a-z][a-z0-9_]*", x) for x in custom["packages"]), "invalid custom package list")
    require((custom["registry"] is None and custom["native"] is None and not custom["packages"]) or
            (custom["registry"] is not None and custom["native"] is not None and bool(custom["packages"])),
            "custom provider requires both reviewed headers and explicit packages")
    require(isinstance(value["features"], list) and len(set(value["features"])) == len(value["features"]) and
            all(re.fullmatch(r"[a-z][a-z0-9_]*", x) for x in value["features"]), "closed required feature identifiers")
    options = value["cmake_options"]
    allowed = {"UWVM_ROS1_DISCOVERY", "UWVM_ROS1_INCLUDE_DIRS", "UWVM_ROS1_LIBRARIES",
               "UWVM_ROS1_CXX11_ABI", "UWVM_ROS1_GENERATOR_PYTHON", "UWVM_ROS1_GENCPP_SCRIPT",
               "UWVM_ROS1_GENCPP_TEMPLATE_DIR"}
    require(isinstance(options, dict) and set(options) <= allowed, "unsupported native CMake option")
    for key, choice in options.items():
        require(value["backend"] == 1, "ROS1 native backport options cannot select ROS2 behavior")
        if key == "UWVM_ROS1_DISCOVERY":
            require(choice in ("CATKIN", "EXPLICIT"), "invalid ROS1 discovery mode")
        elif key == "UWVM_ROS1_CXX11_ABI":
            require(choice in ("auto", "0", "1"), "explicit libstdc++ ABI selection required")
        elif key in ("UWVM_ROS1_INCLUDE_DIRS", "UWVM_ROS1_LIBRARIES"):
            require(isinstance(choice, list) and choice, "explicit nonempty native input list required")
            for path in choice:
                absolute(path, key)
                require(";" not in path, "CMake list paths cannot contain semicolons")
        else:
            absolute(choice, key)
    return value


def load(path):
    value, pin = document(Path(path).resolve(strict=True))
    return validate(value), pin


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("profile", type=Path)
    args = parser.parse_args()
    value, pin = load(args.profile)
    print(json.dumps({"status": "CONFIGURATION_ONLY", "profile": value["id"], "input": pin,
                      "build": "NOT_PERFORMED", "behavior": "NOT_PERFORMED", "guest_authority": "EMPTY"}, indent=2))


if __name__ == "__main__":
    main()
