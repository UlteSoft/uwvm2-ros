"""Repair application is explicit, version-bound, and exactly reversible."""
import importlib.util
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

spec = importlib.util.spec_from_file_location(
    "workbench_compat", Path(__file__).resolve().parents[2] / "tools/debug/vscode_workbench_compat.py")
compat = importlib.util.module_from_spec(spec)
spec.loader.exec_module(compat)


class WorkbenchRepair(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)

    def test_unknown_source_refused(self):
        with self.assertRaisesRegex(ValueError, "Unrecognized workbench"):
            compat.repaired(b"different build")

    def test_replace_roundtrip_and_mode(self):
        path = self.root / "editor.js"
        path.write_bytes(b"original")
        path.chmod(0o640)
        compat.replace_checked(path, b"original", b"repaired")
        self.assertEqual(path.read_bytes(), b"repaired")
        if os.name != "nt":
            self.assertEqual(path.stat().st_mode & 0o777, 0o640)
        compat.replace_checked(path, b"repaired", b"original")
        self.assertEqual(path.read_bytes(), b"original")
        self.assertEqual(list(self.root.glob(".uwvm-workbench-*")), [])

    def test_unexpected_file_not_overwritten(self):
        path = self.root / "editor.js"
        path.write_bytes(b"updated by user")
        with self.assertRaises(ValueError):
            compat.replace_checked(path, b"original", b"repaired")
        self.assertEqual(path.read_bytes(), b"updated by user")

    def test_symlink_not_overwritten(self):
        if os.name == "nt":
            self.skipTest("Windows symlink creation requires separate host qualification")
        path = self.root / "original.js"
        path.write_bytes(b"original")
        link = self.root / "editor.js"
        link.symlink_to(path)
        with self.assertRaises(ValueError):
            compat.replace_checked(link, b"original", b"repaired")
        self.assertTrue(link.is_symlink())
        self.assertEqual(path.read_bytes(), b"original")

    def test_replacement_race_refused(self):
        path = self.root / "editor.js"
        path.write_bytes(b"original")
        real_mkstemp = compat.tempfile.mkstemp
        def concurrent_update(*args, **kwargs):
            path.write_bytes(b"concurrent update")
            return real_mkstemp(*args, **kwargs)
        with patch.object(compat.tempfile, "mkstemp", concurrent_update):
            with self.assertRaisesRegex(ValueError, "changed during preparation"):
                compat.replace_checked(path, b"original", b"repaired")
        self.assertEqual(path.read_bytes(), b"concurrent update")
        self.assertEqual(list(self.root.glob(".uwvm-workbench-*")), [])

    def test_unknown_editor_version_refused(self):
        (self.root / "package.json").write_text(json.dumps(dict(version="0.0.0")))
        (self.root / "product.json").write_text(json.dumps(dict(commit=compat.COMMIT)))
        with self.assertRaisesRegex(ValueError, "has been verified"):
            compat.target(self.root)

    def test_verified_build_bundle_apply_restore(self):
        source = os.environ.get("UWVM_TEST_VSCODE_APP")
        if not source:
            self.skipTest("Set UWVM_TEST_VSCODE_APP for exact installed-build qualification")
        app = self.root / "app"
        app.mkdir()
        for name in ["package.json", "product.json"]:
            (app / name).write_bytes((Path(source) / name).read_bytes())
        target = app / compat.WORKBENCH
        target.parent.mkdir(parents=True)
        original = (Path(source) / compat.WORKBENCH).read_bytes()
        target.write_bytes(original)
        bundle = self.root / "repair"
        result = compat.prepare(app, bundle)
        self.assertFalse(result["applicationChanged"])
        self.assertEqual(target.read_bytes(), original)
        with self.assertRaises(FileExistsError):
            compat.prepare(app, bundle)
        product = (app / "product.json").read_bytes()
        result = compat.change(app, bundle)
        self.assertEqual(result["sha256"], compat.REPAIRED_SHA256)
        self.assertEqual((app / "product.json").read_bytes(), product)
        with self.assertRaises(ValueError):
            compat.change(app, bundle)
        result = compat.change(app, bundle, restore=True)
        self.assertEqual(target.read_bytes(), original)
        with self.assertRaises(ValueError):
            compat.prepare(app, app / "unsafe-bundle")
        (bundle / "repaired.js").write_bytes(b"modified")
        with self.assertRaisesRegex(ValueError, "bundle was modified"):
            compat.change(app, bundle)
        self.assertEqual(target.read_bytes(), original)


if __name__ == "__main__":
    unittest.main()
