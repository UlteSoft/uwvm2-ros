#!/usr/bin/env python3
"""Formatter contract regressions; these tests grant no native/product PASS."""
from __future__ import annotations

import base64
import io
from pathlib import Path
import sys
import unittest

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "test/0018.debugger"))
import test_dap_wasm_memory_pages as legacy

dap = legacy.dap


def dump(module, memory, address, data):
    result = f"Wasm memory module={module} index={memory} offset={address} bytes={len(data)}\n"
    for first in range(0, len(data), 16):
        page = data[first:first + 16]
        result += (f"0x{address + first:016x}: " + "".join(f"{value:02x} " for value in page)
                   + "   " * (16 - len(page)) + " |"
                   + "".join(chr(value) if 32 <= value < 127 else "." for value in page) + "|\n")
    return result


class CurrentBroker(legacy.Broker):
    def request(self, command):
        if not command.startswith("memory "):
            return super().request(command)
        self.commands.append(command)
        self.pages += 1
        _, module, memory, address, count = command.split()
        module, memory, address, count = map(int, (module, memory, address, count))
        data = bytes((module * 17 + memory * 31 + address + index) & 255 for index in range(count))
        reply = dump(module, memory, address, data)
        if self.pages == self.fail_page and self.fault:
            if self.fault == "selector":
                reply = reply.replace(f"offset={address}", f"offset={address + 1}", 1)
            elif self.fault == "short":
                reply = reply[:-2]
        if self.after_page:
            self.after_page()
        return reply


class MemoryDump(unittest.TestCase):
    def test_current_console_rows_all_byte_values_and_partial_rows(self):
        for count in (0, 1, 15, 16, 17, 31, 128, 255, 256):
            data = bytes(range(256))[:count]
            for module, memory, address in ((0, 0, 0), ((1 << 64) - 1, (1 << 32) - 1, (1 << 63) + 17)):
                with self.subTest(count=count, address=address):
                    self.assertEqual(dap.parse_wasm_memory_page(dump(module, memory, address, data), module, memory, address, count), data)

    def test_selected_guest_header_is_exact(self):
        good = dump(7, 3, 64, b"data")
        for wrong in (good.replace("module=7", "module=8"), good.replace("index=3", "index=4"),
                      good.replace("offset=64", "offset=65"), good.replace("bytes=4", "bytes=3"),
                      good.replace("module=7", "module=07"), "host address\n" + good):
            with self.subTest(reply=wrong), self.assertRaises(ValueError):
                dap.parse_wasm_memory_page(wrong, 7, 3, 64, 4)

    def test_rows_must_be_complete_contiguous_canonical_and_ascii_consistent(self):
        good = dump(0, 0, 64, bytes(range(32, 65)))
        rows = good.splitlines(keepends=True)
        bad = [good[:-1], good + "\n", good.replace("\n", "\r\n"),
               good.replace("0x0000000000000050:", "0x0000000000000060:"),
               good.replace("20 21", "100 21", 1), good.replace("20 21", "zz 21", 1),
               good.replace("20 21", "20\t21", 1), good.replace("| !", "|. !", 1),
               "".join((rows[0], rows[2], rows[1], rows[3])), "".join(rows[:-1]),
               good + rows[1], good.replace("40 " + "   " * 15, "40 00 " + "   " * 14, 1)]
        for wrong in bad:
            self.assertNotEqual(wrong, good, "each negative fixture must actually differ")
            with self.subTest(reply=wrong), self.assertRaises(ValueError):
                dap.parse_wasm_memory_page(wrong, 0, 0, 64, 33)

    def test_empty_page_checks_header_and_rejects_data_or_error(self):
        self.assertEqual(dap.parse_wasm_memory_page(dump(0, 1, 17, b""), 0, 1, 17, 0), b"")
        for wrong in ("", "Wasm memory module=0 index=1 offset=17 bytes=0", "error: unavailable\n",
                      dump(0, 1, 17, b"") + "0x0000000000000011: 00\n"):
            with self.subTest(reply=wrong), self.assertRaises(ValueError):
                dap.parse_wasm_memory_page(wrong, 0, 1, 17, 0)

    def test_parser_bounds_and_legacy_broker_compatibility(self):
        self.assertEqual(dap.parse_wasm_memory_page("memory: 00 7f FF\n", 0, 0, 1, 3), b"\0\x7f\xff")
        self.assertEqual(dap.parse_wasm_memory_page("memory: <empty>\n", 0, 0, 1, 0), b"")
        for coordinates in ((False, 0, 0, 0), (0, True, 0, 0), (0, 0, False, 0), (0, 0, 0, True),
                            (1 << 64, 0, 0, 0), (0, 1 << 32, 0, 0), (0, 0, -1, 0),
                            (0, 0, (1 << 64) - 1, 1), (0, 0, 0, 257)):
            with self.subTest(coordinates=coordinates), self.assertRaises(ValueError):
                dap.parse_wasm_memory_page("memory: <empty>\n", *coordinates)


class CurrentMemoryDispatch(unittest.TestCase):
    send = legacy.WasmMemoryPages.send

    def setUp(self):
        self.output = io.BytesIO()
        self.adapter = dap.Adapter(self.output)
        self.broker = CurrentBroker()
        self.adapter.broker = self.broker
        self.sequence = 0

    def read(self, count=513):
        return self.send("readMemory", memoryReference="wasm-memory:2:1:64", offset=-47, count=count)

    def test_dispatch_paginates_current_console_with_unchanged_cohort(self):
        for count in (0, 1, 16, 17, 256, 257, 513, 65536):
            with self.subTest(count=count):
                self.setUp()
                reply = self.read(count)
                self.assertTrue(reply["success"], reply)
                data = bytes((2 * 17 + 31 + 17 + index) & 255 for index in range(count))
                self.assertEqual(reply["body"], dict(address="17", data=base64.b64encode(data).decode(), unreadableBytes=0))
                self.assertEqual(self.broker.pages, max(1, (count + 255) // 256))

    def test_malformed_later_page_withholds_successful_prefix(self):
        for fault in ("short", "selector"):
            with self.subTest(fault=fault):
                self.setUp()
                self.broker.fault = fault
                reply = self.read()
                self.assertFalse(reply["success"], reply)
                self.assertNotIn("body", reply)
                self.assertIsNone(self.adapter.stop_key)
                self.assertEqual(self.broker.pages, 2)

    def test_retired_stop_while_copying_current_dump_withholds_all_bytes(self):
        def retire():
            if self.broker.pages == 2:
                self.broker.stop += 1
        self.broker.after_page = retire
        reply = self.read()
        self.assertFalse(reply["success"], reply)
        self.assertNotIn("body", reply)
        self.assertEqual(self.broker.pages, 2)


if __name__ == "__main__":
    from qualification_support import admission
    admission(ROOT)
    unittest.main()
