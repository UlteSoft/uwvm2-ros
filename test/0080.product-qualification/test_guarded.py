"""Admission failures must retain a block before any compiler/test actor starts."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

SCRIPT = Path(__file__).with_name("guarded.py")


@unittest.skipUnless(sys.platform == "linux", "Linux admission contract")
class GuardedContracts(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.marker = self.root / "actor-ran"
        self.report = self.root / "resources.json"

    def invoke(self, admission):
        return subprocess.run([sys.executable, "-B", str(SCRIPT), "--source", str(self.root),
            "--admission", str(admission), "--out", str(self.report), "--", sys.executable, "-B", "-c",
            "from pathlib import Path; Path(" + repr(str(self.marker)) + ").write_text('ran')"],
            stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=10)

    def test_missing_admission_is_retained_and_starts_no_actor(self):
        self.assertEqual(self.invoke(self.root / "missing.json").returncode, 77)
        self.assertEqual(json.loads(self.report.read_text())["status"], "BLOCKED")
        self.assertFalse(self.marker.exists())

    def test_changed_provisioning_is_retained_and_starts_no_actor(self):
        lock = self.root / "lease"
        lock.touch(mode=0o600)
        state = lock.stat()
        provision = self.root / "provision.json"
        provision.write_text("{}\n")
        admission = self.root / "admission.json"
        admission.write_text(json.dumps({"schema_version": 1, "leaf": "/not-an-admitted-leaf", "device": 0, "inode": 0,
            "boot_id": "not-admitted", "machine": os.uname().machine, "global_lock": str(lock),
            "lock_device": state.st_dev, "lock_inode": state.st_ino, "provision_manifest": str(provision),
            "provision_sha256": "0" * 64}))
        self.assertEqual(self.invoke(admission).returncode, 77)
        report = json.loads(self.report.read_text())
        self.assertEqual(report["status"], "BLOCKED")
        self.assertIn("provisioning manifest differs", report["reason"])
        self.assertFalse(self.marker.exists())


if __name__ == "__main__":
    unittest.main()
