#!/usr/bin/env python3
"""@brief Refuse incomplete or changed debugger source inventories.

@details These metadata tests do not execute a VM or qualify the debugger. They
ensure that both maintained manifest layouts inspect the actual source bytes.
Run on Linux under the same cgroup and test controls as the debugger cohort.
"""

import hashlib
from pathlib import Path
import tempfile
import unittest

from run_live_disassembly import verify_source_inputs


class SourceIdentityTests(unittest.TestCase):
    """@brief Exercise real temporary files and independent manifest alterations."""

    def setUp(self):
        self.directory = tempfile.TemporaryDirectory(prefix="uwvm-debug-source-")
        self.root = Path(self.directory.name)
        self.source = self.root / "main.cc"
        self.source.write_bytes(b"original\n")
        self.row = {"path": "main.cc", "bytes": 9,
                    "sha256": hashlib.sha256(b"original\n").hexdigest()}

    def tearDown(self):
        self.directory.cleanup()

    def test_both_layouts_check_files(self):
        for prefix in ("", "vm/"):
            with self.subTest(prefix=prefix):
                row = dict(self.row, path=prefix + "main.cc")
                self.assertEqual(verify_source_inputs(self.root, [row]), {"files": 1, "bytes": 9})
                row["sha256"] = "0" * 64
                with self.assertRaisesRegex(ValueError, "SHA-256 differs"):
                    verify_source_inputs(self.root, [row])

    def test_same_size_source_change(self):
        self.source.write_bytes(b"modified\n")
        with self.assertRaisesRegex(ValueError, "SHA-256 differs"):
            verify_source_inputs(self.root, [self.row])

    def test_truncated_source(self):
        self.source.write_bytes(b"short\n")
        with self.assertRaisesRegex(ValueError, "size differs"):
            verify_source_inputs(self.root, [self.row])

    def test_empty_inventory(self):
        with self.assertRaisesRegex(ValueError, "must contain"):
            verify_source_inputs(self.root, [])

    def test_duplicate_after_prefix_removal(self):
        duplicate = dict(self.row, path="vm/main.cc")
        with self.assertRaisesRegex(ValueError, "duplicate"):
            verify_source_inputs(self.root, [self.row, duplicate])

    def test_noncanonical_path(self):
        for path in ("../main.cc", "vm/../main.cc", "/main.cc", "vm//main.cc"):
            with self.subTest(path=path), self.assertRaisesRegex(ValueError, "Noncanonical"):
                verify_source_inputs(self.root, [dict(self.row, path=path)])

    def test_missing_file(self):
        with self.assertRaises(FileNotFoundError):
            verify_source_inputs(self.root, [dict(self.row, path="missing.cc")])

    def test_symlink_outside_selected_tree(self):
        selected = self.root / "selected"
        selected.mkdir()
        (selected / "main.cc").symlink_to(self.source)
        with self.assertRaisesRegex(ValueError, "Escaping"):
            verify_source_inputs(selected, [self.row])


if __name__ == "__main__":
    unittest.main()
