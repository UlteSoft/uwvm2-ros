"""Authored ELF parser negatives; synthetic headers are NOT ARM execution."""

import struct
import unittest

from package_contract import aarch64_elf, main


def minimal(interpreter=b"/lib/ld-linux-aarch64.so.1\0"):
    raw = bytearray(120 + len(interpreter))
    raw[:7] = b"\x7fELF\x02\x01\x01"
    struct.pack_into("<HHI", raw, 16, 3, 183, 1)
    struct.pack_into("<Q", raw, 32, 64)
    struct.pack_into("<HHH", raw, 52, 64, 56, 1)
    struct.pack_into(
        "<IIQQQQQQ", raw, 64, 3, 0, 120, 0, 0, len(interpreter), len(interpreter), 1
    )
    raw[120:] = interpreter
    return raw


class Contracts(unittest.TestCase):
    def test_exact_fields_and_interpreter(self):
        result = aarch64_elf(minimal(), "/lib/ld-linux-aarch64.so.1")
        self.assertEqual(result["machine"], 183)
        self.assertEqual(result["endian"], "little")

    def test_wrong_architecture_class_and_endian_refuse(self):
        for offset, value in ((4, 1), (5, 2), (18, 62)):
            raw = minimal()
            raw[offset] = value
            with self.subTest(offset=offset), self.assertRaises(ValueError):
                aarch64_elf(raw, "/lib/ld-linux-aarch64.so.1")

    def test_truncation_and_program_table_overflow_refuse(self):
        for size in (0, 63, 119, 128):
            with self.subTest(size=size), self.assertRaises(ValueError):
                aarch64_elf(minimal()[:size], "/lib/ld-linux-aarch64.so.1")
        raw = minimal()
        struct.pack_into("<Q", raw, 32, 2**64 - 1)
        with self.assertRaises(ValueError):
            aarch64_elf(raw, "/lib/ld-linux-aarch64.so.1")

    def test_interpreter_embedded_nul_and_mismatch_refuse(self):
        with self.assertRaises(ValueError):
            aarch64_elf(minimal(b"/lib/a\0b\0"), "/lib/a")
        with self.assertRaises(ValueError):
            aarch64_elf(minimal(), "/lib/ld-musl-aarch64.so.1")

    def test_excessive_program_count_refuses(self):
        raw = minimal()
        struct.pack_into("<H", raw, 56, 65535)
        with self.assertRaises(ValueError):
            aarch64_elf(raw, "/lib/ld-linux-aarch64.so.1")

    def test_default_denial_precedes_manifest_open(self):
        self.assertEqual(
            main(
                [
                    "--root",
                    "/absent",
                    "--manifest",
                    "/absent",
                    "--manifest-sha256",
                    "0" * 64,
                ]
            ),
            13,
        )


if __name__ == "__main__":
    unittest.main()
