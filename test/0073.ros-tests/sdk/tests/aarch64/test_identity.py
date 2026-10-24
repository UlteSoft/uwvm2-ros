#!/usr/bin/env python3
"""@brief Defensively test architecture and memory metadata admission.
@details Synthetic headers test the inspector only. They are never executed and
do not qualify a compiler, guest, native ROS process or ARM runtime.
"""

from pathlib import Path
import struct
import sys
import tempfile
import unittest

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/aarch64"))
from identity import elf, require_native_elf, wasm_memories


class IdentityTests(unittest.TestCase):
    """@brief Exercise refusal boundaries before any infrastructure execution."""

    def setUp(self):
        """@brief Allocate an owned temporary input directory for one test."""
        self.folder = tempfile.TemporaryDirectory()
        self.path = Path(self.folder.name) / "input"

    def tearDown(self):
        """@brief Remove only this test's nonexecutable metadata fixtures."""
        self.folder.cleanup()

    def test_actual_elf_machine_and_byte_order(self):
        """@brief Parse actual bytes independently of a misleading filename."""
        for order, tag in (("<", 1), (">", 2)):
            header = bytearray(64)
            header[:6] = b"\x7fELF\x02" + bytes([tag])
            struct.pack_into(order + "H", header, 18, 183)
            self.path.write_bytes(header)
            self.assertEqual(elf(self.path)["machine"], 183)
            self.assertEqual(elf(self.path)["bits"], 64)

    def test_refuse_truncated_elf64(self):
        """@brief A 52-byte ELF32 extent cannot be accepted as ELF64."""
        self.path.write_bytes(b"\x7fELF\x02\x01" + bytes(46))
        with self.assertRaises(ValueError):
            elf(self.path)

    def test_native_role_requires_elf64_even_without_optional_machine_field(self):
        """@brief A matching e_machine alone cannot qualify an ELF32 participant."""
        header = bytearray(64)
        header[:6] = b'\x7fELF\x01\x01'
        struct.pack_into('<H', header, 18, 183)
        self.path.write_bytes(header)
        self.assertEqual(elf(self.path)['machine'], 183)
        with self.assertRaises(ValueError):
            require_native_elf(self.path, 183)
        header[4] = 2
        self.path.write_bytes(header)
        self.assertEqual(require_native_elf(self.path, 183)['bits'], 64)
        with self.assertRaises(ValueError):
            require_native_elf(self.path, 62)

    def test_both_real_memory_flags(self):
        """@brief Distinguish memory32 and memory64 by the actual limits flags."""
        for flags, bits in ((0, 32), (4, 64)):
            self.path.write_bytes(b"\0asm\x01\0\0\0\x05\x03\x01" + bytes([flags, 1]))
            self.assertEqual(wasm_memories(self.path)[0]["bits"], bits)

    def test_refuse_imported_memory_even_with_defined_memory(self):
        """@brief An imported memory must not be hidden by another defined one."""
        imported = b"\x01\x01m\x01n\x02\x00\x01"
        self.path.write_bytes(b"\0asm\x01\0\0\0\x02" + bytes([len(imported)]) + imported + b"\x05\x03\x01\x04\x01")
        with self.assertRaises(ValueError):
            wasm_memories(self.path)

    def test_refuse_invalid_limits_and_duplicate_sections(self):
        """@brief Refuse malformed bounds, shared memories without maxima and truncation."""
        cases = (b"\x05\x04\x01\x01\x02\x01", b"\x05\x03\x01\x02\x01",
                 b"\x05\x05\x01\x00\x01", b"\x05\x03\x01\x00\x01\x05\x03\x01\x00\x01")
        for case in cases:
            self.path.write_bytes(b"\0asm\x01\0\0\0" + case)
            with self.assertRaises(ValueError):
                wasm_memories(self.path)


if __name__ == "__main__":
    unittest.main()
