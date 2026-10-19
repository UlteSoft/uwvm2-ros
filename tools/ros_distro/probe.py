#!/usr/bin/env python3
"""Configure the selected Gateway and retain its real SDK feature probes.

Successful try_compile/try_link results are build observations, not ROS delivery
or SROS2 qualification. A missing feature manifest is an explicit preparation
failure; a distribution name never supplies a replacement feature result.
"""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import re
import shlex
import sys
import xml.etree.ElementTree as ET

sys.dont_write_bytecode = True
from common import (SerialRunner, atomic_json, body_pin, catalog, cgroup_preserved, cgroup_state,
                    document, install_interrupts, opened, require, source_state)
from profile import ROS1_HISTORICAL, load
from build_parallelism import (current_affinity, current_host_memory_available,
                               environment_selection, plan as build_plan)


def initial_environment(profile, out):
    tools = profile["tools"]
    directories = list(dict.fromkeys(str(Path(tools[k]).parent) for k in
                       ("cxx", "cmake", "make", "bash", "host_python", "generator_python")))
    return {"PATH": ":".join(directories + ["/usr/bin", "/bin"]), "HOME": str(out / "home"),
            "LANG": "C", "LC_ALL": "C", "PYTHONDONTWRITEBYTECODE": "1",
            "MAKEFLAGS": "-j1", "CMAKE_BUILD_PARALLEL_LEVEL": "1",
            **environment_selection(os.environ)}


def cmake_arguments(profile, source, build):
    """Use argv tokens, an explicit toolchain, and the actual selected ABI flags."""
    tools = profile["tools"]
    standard = profile["cxx"]["standard"]
    args = [tools["cmake"], "-S", str(source / ("gateways/ros%d" % profile["backend"])),
            "-B", str(build), "-G", "Unix Makefiles", "-DCMAKE_BUILD_TYPE=Release",
            "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON", "-DCMAKE_CXX_COMPILER=" + tools["cxx"],
            "-DCMAKE_MAKE_PROGRAM=" + tools["make"], "-DCMAKE_CXX_STANDARD=" + str(standard),
            "-DCMAKE_CXX_FLAGS=" + shlex.join(profile["cxx"]["flags"] + ["-O0", "-g0", "-MD"]),
            "-DCMAKE_EXE_LINKER_FLAGS=" + shlex.join(profile["cxx"]["link_flags"]),
            "-DPYTHON_EXECUTABLE=" + tools["generator_python"],
            "-DPython3_EXECUTABLE=" + tools["host_python"],
            "-DUWVM_ROS%d_CXX_STANDARD=%d" % (profile["backend"], standard)]
    if tools["sysroot"]:
        args.append("-DCMAKE_SYSROOT=" + tools["sysroot"])
    if tools["toolchain_file"]:
        args.append("-DCMAKE_TOOLCHAIN_FILE=" + tools["toolchain_file"])
    custom = profile["custom"]
    for key, value in sorted(profile["cmake_options"].items()):
        args.append("-D" + key + "=" + (";".join(value) if isinstance(value, list) else value))
    if custom["registry"]:
        args.extend(["-DUWVM_ROS_CUSTOM_REGISTRY_HEADER=" + custom["registry"],
                     "-DUWVM_ROS_CUSTOM_ROS%d_HEADER=%s" % (profile["backend"], custom["native"]),
                     "-DUWVM_ROS_CUSTOM_ROS%d_PACKAGES=%s" % (profile["backend"], ";".join(custom["packages"]))])
    return args


def validate_ros2_action_features(features, custom):
    """Admit three observed Action flags without deriving native availability.

    The unchanged ``action`` ID is the actual Fibonacci lifecycle probe.
    ``action_core`` is independent of tutorial types. ``custom_action`` means
    the complete explicitly paired native catalog passed every typed lifecycle
    probe. This policy only rejects inconsistent observations; it never turns
    any observed false result into true or supplies a missing compile result.
    """
    names = ("action", "action_core", "custom_action")
    require(type(features) is dict and all(type(features.get(name)) is bool for name in names),
            "three explicit observed ROS2 Action feature flags required")
    require(type(custom) is dict and set(custom) == {"registry", "native", "packages"},
            "closed custom provider selection required")
    require(not (features["action"] or features["custom_action"]) or features["action_core"],
            "a concrete Action lifecycle requires the public Action core")
    require(not features["custom_action"] or
            (bool(custom["registry"]) and bool(custom["native"]) and bool(custom["packages"])),
            "a custom Action probe cannot pass without the paired selected provider and packages")


def package_versions(prefix):
    """Read physical official package metadata; a package name is not an ABI proof."""
    packages = []
    metadata = set()
    for name in ("package.xml", "manifest.xml", "stack.xml"):
        metadata.update(Path(prefix).rglob(name))
    require(len(metadata) <= 16384, "bounded installed/source SDK package metadata inventory required")
    for path in sorted(metadata):
        physical = path.resolve(strict=True)
        pin = opened(physical, 1024 * 1024)
        root = ET.fromstring(physical.read_bytes())
        packages.append({"name": root.findtext("name") or path.parent.name,
                         "version": root.findtext("version") or "NOT_RECORDED_IN_LEGACY_MANIFEST",
                         "metadata_format": path.name, "input": pin})
    require(packages, "no package.xml/manifest.xml/stack.xml metadata under explicit ROS prefix")
    return packages


def distribution_entry(path, profile):
    """Read the small index-v4 distribution mapping without executing YAML tags.

    Only the official index's two scalar classification fields are consumed.
    Other YAML is retained by its complete file pin. Unknown indentation or a
    duplicate selected entry fails rather than applying a distro-name table.
    """
    text, pin = body_pin(Path(path))
    text = text.decode()
    require(re.search(r"(?m)^version: 4\s*$", text), "official index-v4 input required")
    if profile["distro"] == "legacy":
        return {"distro": "legacy", "status": "USER_SOURCE_UNVERIFIED", "input": pin}
    if profile["distro"] in ROS1_HISTORICAL:
        require(profile["backend"] == 1 and profile["sdk"]["source_lock"] is not None,
                "named pre-Groovy SDK requires its explicit source lock")
        return {"distro": profile["distro"], "status": "end-of-life", "input": pin,
                "index_v4_presence": "NOT_LISTED",
                "basis": "https://raw.githubusercontent.com/ros-infrastructure/rep/master/rep-0003.rst",
                "source_lock": opened(Path(profile["sdk"]["source_lock"]).resolve(strict=True)),
                "scope": "NAMED_HISTORICAL_TARGET_ONLY_NO_STOCK_CATKIN_OR_UPSTREAM_BINARY_CLAIM"}
    blocks = list(re.finditer(r"(?m)^  ([a-z][a-z0-9_]*):\s*$", text))
    matches = []
    for index, match in enumerate(blocks):
        if match.group(1) == profile["distro"]:
            body = text[match.end():blocks[index + 1].start() if index + 1 < len(blocks) else len(text)]
            types = re.findall(r"(?m)^    distribution_type: (ros1|ros2)\s*$", body)
            statuses = re.findall(r"(?m)^    distribution_status: ([a-z-]+)\s*$", body)
            require(types == ["ros%d" % profile["backend"]] and len(statuses) == 1,
                    "selected backend differs from physical distribution index")
            matches.append({"distro": profile["distro"], "status": statuses[0], "input": pin})
    require(len(matches) == 1, "selected distribution absent or duplicated in index-v4")
    return matches[0]


class Preparation:
    """A fresh physical source/profile/SDK/tool inventory, shared by probe/build."""
    def __init__(self, profile_path, source, manifest, out):
        self.profile, profile_pin = load(profile_path)
        # Capture build controls before the ROS setup runs. Unknown setup
        # exports cannot raise concurrency or change a memory reservation.
        self.build_environment_selection = environment_selection(os.environ)
        require(self.profile["execution"]["kind"] == "host",
                "UNSUPPORTED_UNPROVEN_CGROUP_CONTAINMENT: run inside an admitted container with a host profile")
        self.profile_path = Path(profile_path).resolve(strict=True)
        self.source = Path(source).resolve(strict=True)
        require(Path(__file__).resolve() == self.source / "tools/ros_distro/probe.py",
                "invoke the actual tool from the selected frozen source")
        self.manifest = Path(manifest).resolve(strict=True)
        self.out = Path(out).resolve()
        require(not self.out.exists() and self.source not in self.out.parents and
                self.out not in self.source.parents, "fresh output outside frozen source required")
        input_roots = self.profile["sdk"]["roots"] + self.profile["tools"]["roots"]
        if self.profile["tools"]["sysroot"]:
            input_roots.append(self.profile["tools"]["sysroot"])
        require(all(Path(root).resolve(strict=True) != self.out and Path(root).resolve(strict=True) not in self.out.parents
                    for root in input_roots), "output must not mutate a selected SDK/toolchain root")
        self.out.mkdir(parents=True, mode=0o700)
        (self.out / "home").mkdir()
        self.state = {"format_version": 1, "kind": "ros-distribution-build", "status": "RUNNING",
                      "profile": self.profile, "profile_input": profile_pin,
                      "source": str(self.source), "source_manifest": opened(self.manifest),
                      "guest_authority": "EMPTY_UNCHANGED", "feature_status": "NOT_PERFORMED",
                      "build_status": "NOT_PERFORMED", "behavior_status": "NOT_PERFORMED",
                      "security_qualification": "NOT_ESTABLISHED",
                      "runtime_dependency_closure": "UNQUALIFIED",
                      "compiler_input_closure": "RECORDED_DEPENDENCIES_ONLY",
                      "cleanup": {"status": "NOT_PERFORMED", "remaining": []}}
        self.runner = None
        self.before = None
        self.inventories = []
        atomic_json(self.out / "report.json", self.state)

    def save(self):
        if self.runner:
            self.state["commands"] = self.runner.commands
        atomic_json(self.out / "report.json", self.state)

    def build_target(self, label, build, target):
        """Admit one build tree with a newly recorded memory-bounded job cap.

        Matrix profiles and top-level commands remain sequential. Only make's
        compilation/link dependency graph may use multiple jobs. Configuration
        probes and native behavior actors retain their original serial setup.
        The command owner still checks the same aggregate RSS ceiling and
        original cgroup throughout execution; planning never raises either.
        """
        current = cgroup_state(self.profile["runtime"])
        cgroup_preserved(self.before, current)
        selection = {"label": label, "status": "PLANNING", "cgroup_snapshot": current,
                     "environment_selection": self.build_environment_selection,
                     "aggregate_owned_rss_limit_bytes": self.profile["limits"]["rss_bytes"]}
        self.state.setdefault("build_parallelism", []).append(selection)
        self.save()
        try:
            selection.update(build_plan(current, self.profile["limits"], self.build_environment_selection,
                                        current_affinity(), current_host_memory_available()))
            selection["status"] = "ADMITTED"
        except BaseException as error:
            selection.update({"status": "FAIL", "error": repr(error)})
            self.save()
            raise
        self.save()
        jobs = str(selection["jobs"])
        environment = dict(self.environment)
        environment.update({"MAKEFLAGS": "-j" + jobs, "CMAKE_BUILD_PARALLEL_LEVEL": jobs})
        return self.runner.run(label, [self.profile["tools"]["cmake"], "--build", str(build),
                               "--target", target, "--", "-j" + jobs, "VERBOSE=1"],
                               self.out, environment)

    def begin(self):
        profile = self.profile
        self.before = cgroup_state(profile["runtime"])
        self.state["cgroup_before"] = self.before
        self.state["source_before"] = source_state(self.source, self.manifest)
        tools = profile["tools"]
        inputs = [self.profile_path, Path(profile["sdk"]["setup"]),
                  Path(profile["sdk"]["distribution_index"]), Path(profile["execution"]["python"])]
        require(Path(sys.executable).resolve(strict=True) == Path(profile["execution"]["python"]).resolve(strict=True),
                "builder Python differs from the explicit execution tool")
        inputs += [Path(tools[k]) for k in ("cxx", "cmake", "make", "bash", "host_python", "generator_python")]
        inputs += [Path(x) for x in (tools["toolchain_file"], profile["sdk"]["source_lock"],
                   profile["custom"]["registry"], profile["custom"]["native"],
                   profile["runtime"]["qemu"], profile["runtime"]["loader"]) if x]
        option_roots = []
        for key, value in profile["cmake_options"].items():
            if key in ("UWVM_ROS1_DISCOVERY", "UWVM_ROS1_CXX11_ABI"):
                continue
            for name in value if isinstance(value, list) else [value]:
                path = Path(name)
                if path.is_dir():
                    option_roots.append(str(path))
                else:
                    inputs.append(path)
        self.state["input_pins"] = [opened(path.resolve(strict=True)) for path in dict.fromkeys(inputs)]
        self.state["configured_file_aliases"] = [{"configured": str(path), "resolved": str(path.resolve(strict=True))}
                                                for path in dict.fromkeys(inputs)]
        self.state["distribution_entry"] = distribution_entry(profile["sdk"]["distribution_index"], profile)
        roots = list(dict.fromkeys(profile["sdk"]["roots"] + tools["roots"] + option_roots))
        prefix = Path(profile["sdk"]["ros_prefix"]).resolve(strict=True)
        require(any(Path(root).resolve(strict=True) == prefix or Path(root).resolve(strict=True) in prefix.parents
                    for root in profile["sdk"]["roots"]), "ROS prefix must belong to a selected whole SDK inventory")
        if tools["sysroot"] and tools["sysroot"] not in roots:
            roots.append(tools["sysroot"])
        self.state["configured_directory_aliases"] = [{"configured": root,
            "resolved": str(Path(root).resolve(strict=True))} for root in roots]
        for index, root in enumerate(roots):
            resolved = Path(root).resolve(strict=True)
            require(resolved != Path("/") and resolved not in self.out.parents,
                    "inventory root cannot include proc/sys/output or entire host filesystem")
            row = catalog(resolved, self.out / ("inventory-before-%02d.jsonl" % index), roots)
            self.inventories.append(row)
        self.state["inventories_before"] = self.inventories
        self.runner = SerialRunner(self.out, profile["limits"],
                                   lambda: cgroup_preserved(self.before, cgroup_state(profile["runtime"])))
        environment = initial_environment(profile, self.out)
        capture = self.out / "setup-environment.json"
        argv = [tools["bash"], "--noprofile", "--norc", "-c", 'source "$1" && shift && exec "$@"',
                "uwvm-ros-setup", profile["sdk"]["setup"], tools["host_python"], "-B",
                str(self.source / "tools/ros_distro/environment.py"), str(self.profile_path), str(capture)]
        self.runner.run("setup-environment", argv, self.out, environment)
        self.environment, environment_pin = document(capture)
        self.state["setup_environment"] = environment_pin
        identity, identity_pin = document(capture.with_name("setup-environment-identity.json"))
        self.state["setup_identity"] = {"value": identity, "input": identity_pin}
        self.state["package_versions"] = package_versions(profile["sdk"]["ros_prefix"])
        self.runner.run("compiler-version", [tools["cxx"], "--version"], self.out, self.environment)
        self.runner.run("compiler-triple", [tools["cxx"]] + profile["cxx"]["flags"] + ["-dumpmachine"], self.out, self.environment)
        triple = (self.out / "compiler-triple.log").read_text().strip()
        require(triple == profile["target"]["triple"], "compiler reports a different target triple")
        self.state["actual_compiler_triple"] = triple
        self.runner.run("cmake-version", [tools["cmake"], "--version"], self.out, self.environment)
        self.save()

    def configure(self):
        self.build = self.out / "gateway-build"
        self.runner.run("gateway-configure", cmake_arguments(self.profile, self.source, self.build),
                        self.out, self.environment)
        path = self.build / ("uwvm_ros%d_features.json" % self.profile["backend"])
        features, pin = document(path)
        keys = {"format_version", "backend", "cxx_standard", "features"}
        require(set(features) == keys and
                features.get("format_version") == 1 and features.get("backend") ==
                "ros%d" % self.profile["backend"] and isinstance(features.get("features"), dict) and
                features.get("cxx_standard") == self.profile["cxx"]["standard"] and
                all(isinstance(k, str) and type(v) is bool for k, v in features["features"].items()),
                "Gateway must emit its actual closed feature-probe manifest")
        if self.profile["backend"] == 2:
            validate_ros2_action_features(features["features"], self.profile["custom"])
        missing = [name for name in self.profile["features"] if not features["features"].get(name, False)]
        self.state.update({"feature_manifest": pin, "actual_features": features,
                           "missing_required_features": missing,
                           "feature_status": "UNSUPPORTED" if missing else "PROBED_BUILD_ONLY"})
        self.save()
        require(not missing, "required SDK features are unavailable: " + ", ".join(missing))

    def finish(self, error=None):
        cleanup_error = None
        try:
            if self.runner:
                self.state["cleanup"] = self.runner.close()
        except BaseException as caught:
            cleanup_error = repr(caught)
            self.state["cleanup"] = {"status": "CLEANUP_INCOMPLETE", "remaining": [], "error": cleanup_error}
        try:
            after = []
            roots = [row["root"] for row in self.inventories]
            for index, old in enumerate(self.inventories):
                current = catalog(old["root"], self.out / ("inventory-after-%02d.jsonl" % index), roots)
                after.append(current)
                require(all(old[k] == current[k] for k in ("root", "files", "bytes", "symlinks", "content_sha256")),
                        "SDK/toolchain inventory changed")
            self.state["inventories_after"] = after
            for pin in self.state.get("input_pins", []):
                require(opened(Path(pin["path"])) == pin, "physical input changed")
            for alias in self.state.get("configured_file_aliases", []) + self.state.get("configured_directory_aliases", []):
                require(str(Path(alias["configured"]).resolve(strict=True)) == alias["resolved"],
                        "configured executable/setup/input alias changed")
            def preserve(value):
                if isinstance(value, dict):
                    fields = {"path", "bytes", "sha256", "device", "inode", "mtime_ns", "ctime_ns"}
                    if set(value) == fields:
                        require(opened(Path(value["path"])) == value, "recorded byte input/product changed")
                    else:
                        for child in value.values():
                            preserve(child)
                elif isinstance(value, list):
                    for child in value:
                        preserve(child)
            preserve(self.state)
            self.state["source_after"] = source_state(self.source, self.manifest)
            self.state["cgroup_after"] = cgroup_state(self.profile["runtime"])
            cgroup_preserved(self.before, self.state["cgroup_after"])
            self.state["inputs_preserved"] = True
        except BaseException as caught:
            self.state["inputs_preserved"] = False
            self.state["preservation_error"] = repr(caught)
        self.state["status"] = "PASS" if error is None and not cleanup_error and self.state.get(
            "inputs_preserved") and self.state["cleanup"]["status"] == "PASS" else "FAIL"
        if error is not None:
            self.state["error"] = repr(error)
        self.save()
        return self.state["status"]


def arguments(description):
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("--profile", required=True, type=Path)
    parser.add_argument("--source", required=True, type=Path)
    parser.add_argument("--source-manifest", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    return parser.parse_args()


def main():
    install_interrupts()
    args = arguments(__doc__)
    prep = Preparation(args.profile, args.source, args.source_manifest, args.out)
    error = None
    try:
        prep.begin()
        prep.configure()
        prep.state["scope"] = "SDK_FEATURE_CONFIGURE_ONLY_NO_GATEWAY_LINK_NO_ROS_BEHAVIOR"
    except BaseException as caught:
        error = caught
    return 0 if prep.finish(error) == "PASS" else 1


if __name__ == "__main__":
    raise SystemExit(main())
