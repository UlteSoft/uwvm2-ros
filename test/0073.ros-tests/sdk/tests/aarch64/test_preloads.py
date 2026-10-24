#!/usr/bin/env python3
"""@brief Check immutable multi-module admission before any real VM invocation.
@details Synthetic modules only exercise metadata refusal. They never qualify
Wasm linking, native transport, module permissions or runtime execution.
"""

from copy import deepcopy
from pathlib import Path
import sys
import tempfile
import unittest

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/aarch64"))
from accept import admit_preloads
from identity import sha256, wasm_memories


class PreloadAdmissionTests(unittest.TestCase):
    """@brief Retain explicit names/bytes without granting or simulating bindings."""

    def setUp(self):
        """@brief Create owned nonexecuted Wasm metadata for one test."""
        self.folder = tempfile.TemporaryDirectory()
        self.path = Path(self.folder.name) / "algorithm.wasm"
        self.path.write_bytes(b"\0asm\x01\0\0\0\x05\x03\x01\x00\x01")
        self.entry = {"main_module_name":"main", "preload_modules":[{
            "name":"algorithm", "wasm":str(self.path), "sha256":sha256(self.path), "memory_bits":32}]}
        self.command = ["runtime", "--wasm-preload-library", str(self.path), "algorithm",
                        "--wasm-set-main-module-name", "main", "--run", "main.wasm"]

    def tearDown(self):
        """@brief Remove only the current test's synthetic owned metadata."""
        self.folder.cleanup()

    def admit(self, entry=None, command=None):
        """@brief Inspect exact test inputs without executing them."""
        return admit_preloads(self.entry if entry is None else entry,
                              self.command if command is None else command, lambda value:value)

    def test_original_sole_command_has_no_preload(self):
        """@brief Existing standard native cases retain their empty preload list."""
        self.assertEqual(self.admit({}, ["runtime", "--run", "main.wasm"]), [])

    def test_exact_identity_and_actual_memory(self):
        """@brief Preserve the actual file hash, plain rename and memory width."""
        row = self.admit()[0]
        self.assertEqual(row["name"], "algorithm")
        self.assertEqual(row["sha256"], sha256(self.path))
        self.assertEqual(row["memories"][0]["bits"], 32)

    def test_refuse_changed_bytes_or_wrong_width(self):
        """@brief An unchanged path cannot conceal a new module or width."""
        for key, value in (("sha256", "0" * 64), ("memory_bits", 64)):
            entry = deepcopy(self.entry)
            entry["preload_modules"][0][key] = value
            with self.assertRaises(ValueError):
                self.admit(entry)

    def test_refuse_undeclared_or_misnamed_preload(self):
        """@brief A different rename changes the runtime identity being tested."""
        with self.assertRaises(ValueError):
            self.admit({})
        for name in ("(algorithm)", "another"):
            command = self.command.copy()
            command[3] = name
            with self.assertRaises(ValueError):
                self.admit(command=command)

    def test_memoryless_library_proves_no_pointer_width(self):
        """@brief A pure function module is explicit and cannot qualify a command."""
        self.path.write_bytes(b"\0asm\x01\0\0\0")
        item = self.entry["preload_modules"][0]
        item.update(sha256=sha256(self.path), memory_bits=None)
        self.assertEqual(self.admit()[0]["memories"], [])
        with self.assertRaises(ValueError):
            wasm_memories(self.path)
        item["memory_bits"] = 32
        with self.assertRaises(ValueError):
            self.admit()

    def test_duplicate_identity_is_left_for_real_vm_refusal(self):
        """@brief Metadata admission never replaces actual collision execution."""
        self.entry["preload_modules"] *= 2
        self.command[4:4] = ["--wasm-preload-library", str(self.path), "algorithm"]
        self.assertEqual(len(self.admit()), 2)


if __name__ == "__main__":
    unittest.main()
