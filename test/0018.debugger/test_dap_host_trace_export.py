"""Lossless, streaming and stop-bound host trace export, including failure atomicity."""
import io
import json
import os
from pathlib import Path
import re
import stat
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import export_host_trace as export
import dap_adapter as dap


def quote(data):
    return '"' + "".join(chr(x) if 32 <= x < 127 and x not in (34, 92) else f"\\x{x:02x}" for x in data) + '"'


class Journal:
    def __init__(self, count=19, label=b'module\n"\x00\xff\\\xe4\xb8\xad', data=b'text\n"\x00\xff\\\xe4\xb8\xad'):
        self.count, self.label, self.data = count, label, data
        self.commands, self.mutate, self.steps = [], None, 0
        self.header = (f"host-trace enabled=7 epoch=2 cleared-through=0 newest={count}"
                       " allocation-failures=0 invalid-ranges=0 sequence-failures=0\n")

    def request(self, text):
        self.commands.append(text)
        assert text.endswith(" if-stop=7"), text
        words = dict(x.split("=", 1) for x in text.split()[3:])
        if text.startswith("trace host read "):
            after, count = int(words["after"]), int(words["count"])
            selected = [x for x in range(after+1, self.count+1)
                        if words["family"] in ("all", export.FAMILIES[1 << ((x-1) % 3)])]
            ids = selected[:count]
            lines = []
            for i in ids:
                lines.append(f'host-call id={i} family={1 << ((i-1)%3)} namespace="test" function="function"'
                    f' module-name={quote(self.label[:255])} module-name-bytes={len(self.label)}'
                    f' module-name-truncated={int(len(self.label)>255)} module=0 participant=1 generation=1'
                    f' environment=2 args=32:4294967295,64:18446744073709551615 returned={int(i!=3)} has-result=1'
                    f' result=18446744073709551615 timed=1 duration-ns=9007199254740993'
                    f' segments={int(i==1)} incomplete={int(i==2)}')
            reply = self.header + "\n".join(lines) + ("\n" if lines else "")
            reply += f"host-calls next={ids[-1] if ids else after} more={int(len(selected)>count)}\n"
        else:
            call, segment, offset, count = (int(words[k]) for k in ("call", "segment", "offset", "count"))
            if segment == export.U64:
                chunk = self.label[offset:offset+count]
                reply = self.header + f"host-label call={call} size={len(self.label)} offset={offset} next={offset+len(chunk)} value={quote(chunk)}\n"
            else:
                chunk = self.data[offset:offset+count]
                reply = self.header + f"host-bytes call={call} segment={segment} memory=0 guest-offset=64 direction=input observation=transfer size={len(self.data)} offset={offset} next={offset+len(chunk)} hex={chunk.hex()}\n"
        return self.mutate(text, reply) if self.mutate else reply

    def checkpoint(self):
        self.steps += 1
        return ("cohort", 1, 7)


class Tests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="host-export-")
        self.root = Path(self.temp.name)
        self.path = self.root / "calls.json"

    def tearDown(self):
        self.temp.cleanup()

    def save(self, journal, **kwargs):
        return export.save_host_trace(journal.request, journal.checkpoint, str(self.path), **kwargs)

    def read(self):
        return json.loads(self.path.read_text())

    def assert_no_output(self):
        self.assertEqual(list(self.root.iterdir()), [])

    def test_lossless_large_buffers_labels_and_pagination(self):
        label = b'A"\\\n\x00\xff' * 12000
        data = bytes(range(256)) * 300
        j = Journal(label=label, data=data)
        summary = self.save(j)
        result = self.read()
        self.assertEqual(summary["calls"], 19)
        self.assertEqual(summary["payloadBytes"], len(data))
        self.assertEqual(result["summary"]["incompleteCalls"], "1")
        self.assertEqual(result["summary"]["unreturnedCalls"], "1")
        for call in result["calls"]:
            self.assertEqual(b"".join(bytes.fromhex(p["hex"]) for p in call["moduleName"]["chunks"]), label)
            self.assertEqual(call["resultUnsigned"], "18446744073709551615")
            self.assertEqual(call["arguments"][1]["unsigned"], "18446744073709551615")
            self.assertEqual(call["durationNs"], "9007199254740993")
        chunks = result["calls"][0]["segments"][0]["chunks"]
        self.assertEqual(b"".join(bytes.fromhex(p["hex"]) for p in chunks), data)
        self.assertTrue(all(p["utf8IfValid"] is None for p in chunks))
        self.assertEqual(stat.S_IMODE(self.path.stat().st_mode), 0o600)
        self.assertTrue(all(c.startswith(("trace host read ", "trace host bytes ")) for c in j.commands))
        self.assertEqual(j.steps, 2)

    def test_more_than_old_history_limits(self):
        self.save(Journal(count=17000, label=b"x", data=b""))
        self.assertEqual(len(self.read()["calls"]), 17000)

    def test_family_empty_and_readable_text(self):
        self.save(Journal(data="你好\n".encode()), family="wasip1")
        result = self.read()
        self.assertEqual(len(result["calls"]), 7)
        self.assertTrue(all(c["family"] == "wasip1" for c in result["calls"]))
        self.assertEqual(result["calls"][0]["segments"][0]["chunks"][0]["utf8IfValid"], "你好\n")
        self.assertIn("你好", self.path.read_text())
        self.path.unlink()
        self.save(Journal(count=0))
        self.assertEqual(self.read()["calls"], [])
        self.path.unlink()
        self.save(Journal(count=1), family="ros2")
        self.assertEqual(self.read()["calls"], [])

    def test_loss_counters_are_not_hidden(self):
        j = Journal()
        j.header = j.header.replace("allocation-failures=0", "allocation-failures=5")
        self.save(j)
        self.assertTrue(self.read()["summary"]["observationLossReported"])
        self.assertEqual(self.read()["journal"]["allocation-failures"], "5")

    def test_reject_retired_epoch_or_stop(self):
        j = Journal()
        j.mutate = lambda c, r: r.replace("epoch=2", "epoch=3") if c.startswith("trace host bytes") else r
        with self.assertRaises(ValueError):
            self.save(j)
        self.assert_no_output()
        j = Journal()
        checkpoints = iter([("same", 1, 7), ("changed", 1, 8)])
        j.checkpoint = lambda: next(checkpoints)
        with self.assertRaises(ValueError):
            self.save(j)
        self.assert_no_output()

    def test_malformed_and_incomplete_packets_do_not_publish(self):
        transforms = [
            lambda r: r.replace("family=1 ", "family=8 "),
            lambda r: r.replace("returned=1 ", "returned=2 "),
            lambda r: r.replace("next=8 more=1", "next=7 more=1"),
            lambda r: r.replace("next=8 more=1", "next=8 more=2"),
            lambda r: r.replace("id=2 ", "id=1 "),
            lambda r: r.replace("generation=1 ", "generation=18446744073709551616 "),
            lambda r: r.replace("hex=", "hex=ff"),
            lambda r: r.replace("memory=0 ", "memory=4294967296 "),
            lambda r: r.replace("observation=transfer", "observation=unknown"),
            lambda r: r.replace("module-name-bytes=15", "module-name-bytes=16"),
            lambda r: r.replace("args=32:", "args=8:"),
            lambda r: r.replace("incomplete=0", "incomplete=0 incomplete=0"),
            lambda r: r.rstrip("\n"),
        ]
        # A generated label has an independently known encoded length.
        transforms[9] = lambda r: r.replace("module-name-bytes=14", "module-name-bytes=13")
        for transform in transforms:
            j = Journal(label=b"12345678901234")
            j.mutate = lambda c, r: transform(r)
            with self.subTest(transform=transforms.index(transform)):
                with self.assertRaises(ValueError):
                    self.save(j)
                self.assert_no_output()

    def test_existing_files_symlinks_and_write_failure_are_safe(self):
        self.path.write_text("keep")
        with self.assertRaises(FileExistsError):
            self.save(Journal())
        self.assertEqual(self.path.read_text(), "keep")
        self.path.unlink()
        target = self.root / "target"
        target.write_text("keep")
        self.path.symlink_to(target)
        with self.assertRaises(FileExistsError):
            self.save(Journal())
        self.assertEqual(target.read_text(), "keep")
        self.path.unlink(); target.unlink()
        with patch.object(export.os, "fsync", side_effect=OSError("disk full")):
            with self.assertRaises(OSError):
                self.save(Journal())
        self.assert_no_output()

    def test_invalid_export_parameters_do_not_query(self):
        j = Journal()
        for path, family in [("", "all"), (str(self.path), "host"), (None, "all"), ("\0", "all")]:
            with self.assertRaises(ValueError):
                export.save_host_trace(j.request, j.checkpoint, path, family)
        self.assertEqual(j.commands, [])
        self.assert_no_output()

    def test_adapter_custom_request_and_console(self):
        for mode in ("custom", "console"):
            output, j = io.BytesIO(), Journal(count=2)
            adapter = dap.Adapter(output)
            adapter.command, adapter.cooperative_memory_stop = j.request, j.checkpoint
            request = ({"command": "uwvm/exportHostTrace", "arguments": {"path": str(self.path), "family": "ros2"}}
                if mode == "custom" else {"command": "evaluate", "arguments": {
                    "expression": "trace host save " + repr(str(self.path)), "context": "repl"}})
            adapter.handle({"seq": 1, "type": "request", **request})
            response = json.loads(output.getvalue().split(b"\r\n\r\n", 1)[1])
            self.assertTrue(response["success"], response)
            self.assertEqual(len(self.read()["calls"]), 0 if mode == "custom" else 2)
            self.path.unlink()


if __name__ == "__main__":
    unittest.main()
