"""The planning ledger must cover public imports without granting evidence."""
from pathlib import Path
import unittest

from inventory import generate
from receipt import DIMENSIONS


class InventoryContracts(unittest.TestCase):
    def test_public_imports_are_unique_and_untested(self):
        rows = generate(Path(__file__).resolve().parents[2])
        self.assertEqual(len({row["id"] for row in rows}), len(rows))
        ros = [row for row in rows if row["category"] == "ros-import"]
        wasi = [row for row in rows if row["category"] == "wasi-import"]
        self.assertEqual(len({row["namespace"] for row in ros}), 7)
        self.assertGreater(len(ros), 100)
        self.assertTrue(any(row["export"] == "fd_write" for row in wasi))
        self.assertTrue(any(row["export"] == "fd_write_wasm64" for row in wasi))
        for row in rows:
            self.assertEqual(set(row["dimensions"]), set(DIMENSIONS))
            self.assertTrue(all(value == {"status": "UNTESTED", "evidence": []} for value in row["dimensions"].values()))


if __name__ == "__main__":
    unittest.main()
