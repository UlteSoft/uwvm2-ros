#!/usr/bin/env python3
"""Check per-Frame retirement ledger ownership without starting a process.

Run this policy suite only under the selected Linux cgroup test owner, with
TMPDIR below its writable work directory. The ordinary temporary ledger files
are real. A deliberately empty fake SerialRunner isolates filename lifetime
from pidfd/process behavior; passing these tests does not qualify ROS, process
cleanup, the cgroup boundary, or a Runtime execution case.
"""
from pathlib import Path
import hashlib
import importlib.util
import tempfile
import types
import unittest


spec = importlib.util.spec_from_file_location(
    "unitree_actor_ledger_policy", Path(__file__).with_name("common.py"))
common = importlib.util.module_from_spec(spec)
spec.loader.exec_module(common)


class EmptySerialRunner:
    """No child, pidfd, descriptor, or process group is created by this fake."""
    def __init__(self, output, limits, guard):
        self.close_calls = 0

    def close(self):
        self.close_calls += 1
        return {"status": "PASS", "remaining": []}


class ActorRetirementLedgerTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="unitree-actor-ledger-policy-")
        self.addCleanup(self.temporary.cleanup)
        output = Path(self.temporary.name).resolve()

        def pin(path):
            raw = Path(path).read_bytes()
            return {"path": str(path), "bytes": len(raw),
                    "sha256": hashlib.sha256(raw).hexdigest()}

        self.frame = types.SimpleNamespace(
            out=output, limits={}, guard=lambda: None, pin=pin,
            common=types.SimpleNamespace(SerialRunner=EmptySerialRunner),
            actors=[], _next_actor_ledger=0)

    def actor(self):
        actor = common.Actors(self.frame)
        self.addCleanup(lambda: actor.retirement.close()
                        if not actor.retirement.closed else None)
        return actor

    def retire(self, actor):
        result = actor.close()
        self.assertEqual(result["status"], "PASS")
        self.assertEqual(actor.owner.close_calls, 1)
        self.assertTrue(actor.retirement.closed)
        self.frame.actors.remove(actor)
        return result

    def test_serial_removed_owners_keep_distinct_immutable_ledgers(self):
        """The former active-list-length scheme fails on the second case."""
        pins = []
        for index in range(32):
            self.assertEqual(self.frame.actors, [])
            actor = self.actor()
            self.assertEqual(actor.retired.name, "retired-%d.jsonl" % index)
            actor.retirement.write('{"case":%d}\n' % index)
            pins.append(self.retire(actor)["retired_processes"])
        self.assertEqual(self.frame._next_actor_ledger, 32)
        self.assertEqual(len(list(self.frame.out.iterdir())), 32)
        for index, expected in enumerate(pins):
            self.assertEqual(self.frame.pin(self.frame.out / ("retired-%d.jsonl" % index)), expected)

    def test_out_of_order_removal_does_not_reuse_high_water_mark(self):
        first, second = self.actor(), self.actor()
        self.retire(first)
        third = self.actor()
        self.assertEqual([second.retired.name, third.retired.name],
                         ["retired-1.jsonl", "retired-2.jsonl"])
        self.retire(third)
        self.retire(second)
        fourth = self.actor()
        self.assertEqual(fourth.retired.name, "retired-3.jsonl")
        self.retire(fourth)

    def test_exclusive_create_collision_preserves_existing_owner_bytes(self):
        existing = self.frame.out / "retired-0.jsonl"
        existing.write_bytes(b'{"existing_owner":true}\n')
        before = self.frame.pin(existing)
        with self.assertRaises(FileExistsError):
            self.actor()
        self.assertEqual(self.frame.actors, [])
        self.assertEqual(self.frame.pin(existing), before)
        # A failed attempt cannot reserve the active list's filename again.
        following = self.actor()
        self.assertEqual(following.retired.name, "retired-1.jsonl")
        self.retire(following)
        self.assertEqual(self.frame.pin(existing), before)


if __name__ == "__main__":
    unittest.main()
