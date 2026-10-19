"""Unexecuted configuration/ELF parser regressions, never Runtime/ROS evidence.

The temporary ELF below is explicitly mathematical parser input, not a fake
Runtime admission. Actual run.py additionally requires successful current
compilation, kernel birth/pidfd, cgroup and stopped executable maps.
"""
import copy
import hashlib
import json
from pathlib import Path
import struct
import tempfile
import unittest

import profile as selected
import engine


class Contracts(unittest.TestCase):
    def setUp(self):
        self.value=json.loads(Path(__file__).with_name("example-profile.json").read_bytes())

    def refused(self, change):
        value=copy.deepcopy(self.value);change(value)
        with self.assertRaises(ValueError):
            selected.validate(value)

    def test_all_eight_runtime_targets_keep_independent_native_architecture(self):
        for architecture in selected.ARCHES:
            value=copy.deepcopy(self.value);value["runtime"]["architecture"]=architecture
            self.assertIs(selected.validate(value),value)
            self.assertEqual(value["native"]["architecture"],"aarch64")

    def test_both_backends_need_explicit_master_only_for_ros1(self):
        value=copy.deepcopy(self.value);value["backend"]=1
        with self.assertRaises(ValueError):
            selected.validate(value)
        value["master"]={"python":"/reviewed/python3","script":"/reviewed/roscore"}
        self.assertIs(selected.validate(value),value)

    def test_native_mode_does_not_claim_loongarch_host(self):
        self.refused(lambda p:p["runtime"]["execution"].update(mode="native",qemu=None))

    def test_qemu_selection_requires_target_loader(self):
        self.refused(lambda p:p["runtime"]["execution"].update(loader=None))

    def test_unknown_or_relabelled_native_architecture_refuses(self):
        self.refused(lambda p:p["runtime"].update(architecture="invented"))
        self.refused(lambda p:p["native"].update(architecture="loongarch64"))

    def test_environment_cannot_inherit_ros_or_grant_authority(self):
        self.refused(lambda p:p["runtime"]["execution"]["environment"].update(ROS_SECURITY_ENABLE="false"))
        self.refused(lambda p:p["compiler_environment"].update(UWVM2TEST="1"))

    def test_all_six_language_positive_and_empty_pairs_are_mandatory(self):
        for case in selected.SDK_CASES:
            self.refused(lambda p,case=case:p["cases"].remove(case))

    def test_event_family_cannot_be_silently_skipped(self):
        self.refused(lambda p:p["cases"].remove("seven-events"))
        self.refused(lambda p:p["cases"].append("parameters-native-only"))

    def test_duplicate_case_cannot_inflate_actual_count(self):
        self.refused(lambda p:p["cases"].append(p["cases"][0]))

    def test_current64_swap_and_leaf_bounds_do_not_relax(self):
        self.refused(lambda p:p["cgroup"].update(memory_max=51539607552))
        self.refused(lambda p:p["cgroup"].update(memory_swap_max=1))
        self.refused(lambda p:p["limits"].update(jobs=2))
        value=copy.deepcopy(self.value);value["limits"]["compiler_rss"]=8589934592
        self.assertIs(selected.validate(value),value)
        self.refused(lambda p:p["limits"].update(compiler_rss=8589934593))
        self.refused(lambda p:p["limits"].update(native_rss=2147483649))

    def test_no_assertion_disable_or_user_only_dependency_rule(self):
        self.refused(lambda p:p["compile_flags"].append("-DNDEBUG"))
        self.refused(lambda p:p["compile_flags"].append("-MMD"))

    def test_unknown_fields_and_empty_loader_path_component_refuse(self):
        self.refused(lambda p:p.update(implicit_rights=["random"]))
        self.refused(lambda p:p["compiler_environment"].update(LD_LIBRARY_PATH="/safe:"))

    def test_parser_reads_big_endian_machine_without_executing_elf(self):
        class Opened:
            @staticmethod
            def opened(path):
                raw=Path(path).read_bytes();return {"path":str(path),"bytes":len(raw),"sha256":hashlib.sha256(raw).hexdigest()}
        header=bytearray(64);header[:7]=b"\x7fELF\x02\x02\x01"
        struct.pack_into(">HH",header,16,2,22)
        with tempfile.TemporaryDirectory() as directory:
            path=Path(directory)/"mathematical-header-only";path.write_bytes(header)
            value=engine.elf(path,Opened)
            self.assertEqual((value["machine"],value["bits"],value["endian"]),(22,64,"big"))
            struct.pack_into(">Q",header,32,64);struct.pack_into(">HH",header,54,56,1);path.write_bytes(header)
            with self.assertRaises(ValueError):
                engine.elf(path,Opened)


if __name__=="__main__":
    unittest.main()
