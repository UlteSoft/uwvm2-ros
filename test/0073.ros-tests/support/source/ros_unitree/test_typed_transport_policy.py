#!/usr/bin/env python3
"""Check custom Topic policy binding with explicit synthetic codec metadata.

Run only inside the selected Linux cgroup test owner. No compiler, Guest,
Gateway, native SDK, network or namespace is used here. Synthetic RIHS and
codec strings exercise strict policy correspondence; they attest to no actual
native type. Real exporter/registry/full-MD admission and28 native DDS cases
remain separate requirements of the transport owner.
"""
from pathlib import Path
import copy
import hashlib
import importlib.util
import unittest


spec = importlib.util.spec_from_file_location(
    "unitree_typed_policy_model", Path(__file__).with_name("typed_transport_inputs.py"))
policy = importlib.util.module_from_spec(spec)
spec.loader.exec_module(policy)
direct_spec = importlib.util.spec_from_file_location(
    "unitree_direct_policy_model", Path(__file__).with_name("run_direct_physics.py"))
direct = importlib.util.module_from_spec(direct_spec)
direct_spec.loader.exec_module(direct)


def generation(names):
    """Build model-only complete record/root pairs; never a native receipt."""
    rows = [{"type": name, "ros_version": 2,
             "native_identity": "RIHS01_" + hashlib.sha256(("native-model:" + name).encode()).hexdigest(),
             "adapter_codec_identity": "UWVMROS1_" + hashlib.sha256(("codec-model:" + name).encode()).hexdigest()}
            for name in names]
    return {"records": rows, "types": copy.deepcopy(rows)}, {
        row["type"]: row["native_identity"] for row in rows}


class TypedTransportPolicyTests(unittest.TestCase):
    def test_complete_record_and_root_codecs_are_returned_exactly(self):
        data, identities = generation(["unitree_go/msg/LowState", "unitree_go/msg/LowCmd"])
        self.assertEqual(policy.generation_codecs(data, identities), {
            row["type"]: row["adapter_codec_identity"] for row in data["records"]})
        self.assertEqual(data["records"], data["types"])

    def test_missing_duplicate_or_foreign_type_cannot_complete_inventory(self):
        original, identities = generation(["unitree_go/msg/LowState", "unitree_go/msg/LowCmd"])
        for collection in ("records", "types"):
            for mutation in ("missing", "duplicate", "foreign"):
                with self.subTest(collection=collection, mutation=mutation):
                    data = copy.deepcopy(original)
                    if mutation == "missing":
                        data[collection].pop()
                    elif mutation == "duplicate":
                        data[collection][1] = copy.deepcopy(data[collection][0])
                    else:
                        data[collection][1]["type"] = "unitree_hg/msg/LowState"
                    with self.assertRaises(ValueError):
                        policy.generation_codecs(data, identities)

    def test_malformed_or_mismatched_codec_native_identity_and_backend_fail(self):
        original, identities = generation(["unitree_api/msg/Request"])
        for collection in ("records", "types"):
            mutations = [("adapter_codec_identity", value) for value in
                         ("", None, "UWVMROS2_" + "0" * 64,
                          "UWVMROS1_" + "A" * 64, "UWVMROS1_" + "0" * 63)]
            mutations += [("native_identity", "RIHS01_" + "f" * 64), ("ros_version", 1)]
            # A syntactically valid but different root codec must also fail.
            if collection == "types":
                mutations.append(("adapter_codec_identity", "UWVMROS1_" + "0" * 64))
            for key, value in mutations:
                with self.subTest(collection=collection, key=key, value=value):
                    data = copy.deepcopy(original)
                    data[collection][0][key] = value
                    with self.assertRaises(ValueError):
                        policy.generation_codecs(data, identities)

    def test_positive_go_hg_api_grants_have_their_exact_separate_codecs(self):
        for code, names in ((1, ["unitree_go/msg/LowState"]),
                            (2, ["unitree_hg/msg/LowState"]),
                            (3, ["unitree_api/msg/Request", "unitree_api/msg/Response"])):
            with self.subTest(family_code=code):
                data, identities = generation(names)
                codecs = policy.generation_codecs(data, identities)
                connection, grants = policy.policy({"family_code": code, "identities": identities,
                                                   "codec_identities": codecs}, True)
                self.assertEqual(connection["domain-id"], "1")
                self.assertEqual([grant["type"] for grant in grants], names)
                self.assertEqual(len(grants), len(names))
                for grant in grants:
                    self.assertEqual(grant["identity"], identities[grant["type"]])
                    self.assertEqual(grant["limits"]["adapter-codec-identity"], codecs[grant["type"]])
                    self.assertTrue(grant["resource"].startswith("/uwvm_unitree_typed/"))
                    self.assertEqual(grant["limits"]["max-inflight"], "1")

    def test_default_denied_needs_no_type_metadata_and_emits_no_grant(self):
        for code in (1, 2, 3):
            _, grants = policy.policy({"family_code": code}, False)
            self.assertEqual(grants, [])
        data, identities = generation(["unitree_go/msg/LowState"])
        chosen = {"family_code": 1, "identities": identities, "codec_identities": {}}
        with self.assertRaises(KeyError):
            policy.policy(chosen, True)

    def test_direct_control_grants_bind_both_exact_native_record_codecs(self):
        names = ["unitree_go/msg/LowCmd", "unitree_go/msg/LowState"]
        data, identities = generation(names)
        records = {row["type"]: {**row, "maximum_wire_bytes": 1069} for row in data["records"]}
        codecs = policy.generation_codecs(data, identities)
        _, grants = direct.policy({"identities": identities, "codec_identities": codecs}, True, records)
        self.assertEqual([grant["type"] for grant in grants], names)
        self.assertEqual([grant["operation"] for grant in grants], ["topic.publish", "topic.subscribe"])
        for grant in grants:
            self.assertEqual(grant["identity"], identities[grant["type"]])
            self.assertEqual(grant["limits"]["adapter-codec-identity"], codecs[grant["type"]])
            self.assertEqual(grant["limits"]["max-message-bytes"], "1069")

    def test_direct_control_denial_is_empty_and_mismatched_records_fail(self):
        self.assertEqual(direct.policy({}, False, {})[1], [])
        data, identities = generation(["unitree_go/msg/LowCmd", "unitree_go/msg/LowState"])
        chosen = {"identities": identities, "codec_identities": policy.generation_codecs(data, identities)}
        for key, wrong in (("adapter_codec_identity", "UWVMROS1_" + "0" * 64),
                           ("native_identity", "RIHS01_" + "0" * 64)):
            with self.subTest(field=key):
                records = {row["type"]: {**row, "maximum_wire_bytes": 1069} for row in data["records"]}
                records["unitree_go/msg/LowCmd"][key] = wrong
                with self.assertRaises(ValueError):
                    direct.policy(chosen, True, records)


if __name__ == "__main__":
    unittest.main()
