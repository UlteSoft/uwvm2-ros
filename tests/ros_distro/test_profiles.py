#!/usr/bin/env python3
"""Pure configuration/argv contracts for the optional distribution lane.

These tests do not spawn a compiler, container, ROS node, QEMU or Guest. They
exercise real validation/argv construction code, preserving the distinction
between a declared target and a tested SDK. Native behavior is tested by the
separately built native_basic.cc executable, never by these models.
"""
import copy
import json
from pathlib import Path
import sys
import unittest

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/ros_distro"))
import profile as profiles
import matrix
from environment import classify_setup


class ProfileContracts(unittest.TestCase):
    def setUp(self):
        self.value = json.loads((ROOT / "tools/ros_distro/example-profile.json").read_text())

    def rejected(self, change):
        value = copy.deepcopy(self.value)
        change(value)
        with self.assertRaises((ValueError, TypeError, KeyError)):
            profiles.validate(value)

    def test_example_is_configuration_only(self):
        self.assertIs(profiles.validate(self.value), self.value)

    def test_all_twenty_six_named_targets(self):
        catalog = json.loads((ROOT / "tools/ros_distro/distributions.json").read_text())
        actual = {(row["backend"], row["distro"]) for row in catalog["distributions"]}
        expected = {(1, name) for name in profiles.ROS1} | {(2, name) for name in profiles.ROS2}
        self.assertEqual(actual, expected)
        self.assertEqual(len(catalog["distributions"]), 26)
        self.assertTrue(all(row["sdk_availability"] == "UNVERIFIED" for row in catalog["distributions"]))

    def test_all_indexed_names_remain_configuration_selections(self):
        for backend, names in ((1, profiles.ROS1), (2, profiles.ROS2)):
            for name in names:
                with self.subTest(backend=backend, distro=name):
                    value = copy.deepcopy(self.value)
                    value.update(backend=backend, distro=name)
                    value["runtime"].update(rmw="" if backend == 1 else "rmw_fastrtps_cpp",
                                            master_uri="http://127.0.0.1:11384" if backend == 1 else "")
                    if name in profiles.ROS1_HISTORICAL:
                        value["sdk"]["source_lock"] = "/reviewed/historical-sdk-source-lock.json"
                    self.assertIs(profiles.validate(value), value)

    def test_all_seven_required_architectures_and_mips_extensions(self):
        for arch, (machine, bits, endian) in profiles.ARCHES.items():
            value = copy.deepcopy(self.value)
            value["target"].update(architecture=arch, elf_machine=machine, bits=bits, endian=endian)
            value["runtime"].update(mode="qemu", qemu="/explicit/qemu-" + arch,
                                    loader="/explicit/sysroot/lib/loader.so")
            value["tools"]["sysroot"] = "/explicit/sysroot"
            with self.subTest(arch=arch):
                self.assertIs(profiles.validate(value), value)

    def test_backend_mismatch_is_not_a_fallback(self):
        self.rejected(lambda p: p.update(backend=1))

    def test_unknown_distro_needs_legacy_lock(self):
        self.rejected(lambda p: p.update(distro="not-indexed"))
        self.rejected(lambda p: p.update(distro="legacy", legacy_description="explicit source port"))
        self.value.update(distro="legacy", legacy_description="explicit source port")
        self.value["sdk"]["source_lock"] = "/reviewed/source-lock.json"
        self.assertIs(profiles.validate(self.value), self.value)

    def test_named_historical_targets_need_locked_source(self):
        for name in profiles.ROS1_HISTORICAL:
            value = copy.deepcopy(self.value)
            value.update(backend=1, distro=name)
            value["runtime"].update(rmw="", master_uri="http://127.0.0.1:11384")
            with self.subTest(distro=name), self.assertRaises(ValueError):
                profiles.validate(value)

    def test_historical_absence_is_recorded_and_mismatches_still_refuse(self):
        value = copy.deepcopy(self.value)
        value.update(backend=1, distro="boxturtle")
        value["sdk"]["source_lock"] = "/reviewed/historical-source-lock.json"
        identity = classify_setup(value, {})
        self.assertIsNone(identity["sourced_ros_distro"])
        self.assertEqual(identity["identity_scope"], "HISTORICAL_LOCKED_SOURCE_LABEL_NO_SETUP_DISTRO_EXPORT")
        with self.assertRaises(ValueError):
            classify_setup(value, {"ROS_DISTRO": "noetic"})
        with self.assertRaises(ValueError):
            classify_setup(self.value, {})

    def test_architecture_cannot_be_a_label_only(self):
        self.rejected(lambda p: p["target"].update(architecture="loongarch64"))
        self.rejected(lambda p: p["target"].update(endian="big"))

    def test_qemu_requires_own_loader_and_sysroot(self):
        self.rejected(lambda p: p["runtime"].update(mode="qemu", qemu="/bin/qemu-aarch64"))
        self.rejected(lambda p: p["runtime"].update(qemu="/bin/qemu-x86_64"))

    def test_cross_target_cannot_use_native_mode(self):
        self.rejected(lambda p: p["target"].update(architecture="aarch64", elf_machine=183))

    def test_old_budget_and_swap_are_refused(self):
        self.rejected(lambda p: p["runtime"].update(memory_max=51539607552))
        self.rejected(lambda p: p["runtime"].update(memory_swap_max=1))

    def test_original_cgroup_identity_is_mandatory(self):
        self.rejected(lambda p: p["runtime"].pop("expected_leaf"))
        self.rejected(lambda p: p["runtime"].update(expected_boot_id=None))
        self.rejected(lambda p: p["runtime"].update(expected_inode=0))
        self.rejected(lambda p: p["runtime"].update(expected_cpus=""))

    def test_serial_and_log_bounds(self):
        self.rejected(lambda p: p["limits"].update(jobs=2))
        self.rejected(lambda p: p["limits"].update(log_bytes=2**30))

    def test_partial_custom_registry_never_enables_types(self):
        self.rejected(lambda p: p["custom"].update(registry="/tmp/registry.hpp"))
        self.rejected(lambda p: p["custom"].update(registry="/tmp/r", native="/tmp/n", packages=["x", "x"]))

    def test_oci_digest_is_required(self):
        self.value["execution"].update(kind="oci", engine="/usr/bin/docker", image="ros:jazzy")
        with self.assertRaises(ValueError):
            profiles.validate(self.value)
        self.value["execution"]["image"] = "local/ros@sha256:" + "a" * 64
        self.assertIs(profiles.validate(self.value), self.value)

    def test_schema_rejects_hidden_permissions_and_unknown_options(self):
        self.rejected(lambda p: p.update(grant_all=True))
        self.rejected(lambda p: p["cmake_options"].update(UWVM_GRANT_ALL="ON"))

    def test_plain_host_argv_never_grants_wasi(self):
        command = matrix.command(self.value, Path("/frozen/source"), Path("/frozen/manifest.json"),
                                 Path("/profiles/p.json"), Path("/owned/out"), "token")
        self.assertEqual(command[0], "/usr/bin/python3")
        self.assertNotIn("--wasip1", " ".join(command))
        self.assertEqual(command[-2:], ["--out", "/owned/out/cell"])

    def test_automatic_oci_launch_is_refused_before_actor_creation(self):
        self.value["execution"].update(kind="oci", engine="/usr/bin/docker",
                                       image="local/ros@sha256:" + "b" * 64)
        with self.assertRaisesRegex(ValueError, "UNSUPPORTED_UNPROVEN_CGROUP_CONTAINMENT"):
            matrix.command(self.value, Path("/frozen/source"), Path("/frozen/manifest.json"),
                           Path("/profiles/p.json"), Path("/owned/out"), "owner-token")


if __name__ == "__main__":
    unittest.main()
