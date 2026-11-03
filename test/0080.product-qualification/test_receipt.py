"""Meaningful failure, scope and source-binding checks for the evidence writer."""
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

SCRIPT = Path(__file__).with_name("receipt.py")


class ReceiptContracts(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        self.source = self.root / "source"
        self.source.mkdir()
        subprocess.run(["git", "init", "-q", str(self.source)], check=True)
        (self.source / "input.cc").write_text("original source\n")
        subprocess.run(["git", "add", "input.cc"], cwd=self.source, check=True)
        subprocess.run(["git", "-c", "user.name=Fixture", "-c", "user.email=fixture@example.invalid",
                        "commit", "-qm", "fixture"], cwd=self.source, check=True)
        self.out = self.root / "evidence"

    def invoke(self, *args):
        return subprocess.run([sys.executable, "-B", str(SCRIPT), *map(str, args)],
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=20)

    def begin(self, *required):
        args = ["init", "--source", self.source, "--out", self.out, "--lane", "fixture"]
        for name in required:
            args += ["--require", name]
        self.assertEqual(self.invoke(*args).returncode, 0)

    def command(self, name, code, *extra):
        return self.invoke("command", "--out", self.out, "--id", name, "--timeout", 5,
                           *extra, "--", sys.executable, "-B", "-c", code)

    def result(self):
        return json.loads((self.out / "receipt.json").read_text())

    def test_unrun_required_command_is_blocked(self):
        self.begin("native-peer")
        self.assertEqual(self.invoke("seal", "--out", self.out, "--job-status", "success").returncode, 77)
        self.assertEqual(self.result()["missing_required"], ["native-peer"])
        self.assertEqual(self.result()["dimensions"]["native_ros"]["status"], "UNTESTED")

    def test_failure_is_retained_despite_other_success(self):
        self.begin("failed", "worked")
        self.assertEqual(self.command("failed", "raise SystemExit(3)").returncode, 1)
        self.assertEqual(self.command("worked", "print('okay')").returncode, 0)
        self.assertEqual(self.invoke("seal", "--out", self.out, "--job-status", "success").returncode, 1)
        self.assertEqual(self.result()["status"], "FAIL")
        self.assertEqual(self.result()["commands"][0]["returncode"], 3)

    def test_blocked_is_never_a_pass(self):
        self.begin("prerequisite")
        self.assertEqual(self.command("prerequisite", "raise SystemExit(77)").returncode, 77)
        self.assertEqual(self.invoke("seal", "--out", self.out, "--job-status", "success").returncode, 77)
        self.assertEqual(self.result()["status"], "BLOCKED")

    def test_source_change_invalidates_success(self):
        self.begin("worked")
        self.assertEqual(self.command("worked", "pass").returncode, 0)
        (self.source / "input.cc").write_text("changed source\n")
        self.assertEqual(self.invoke("seal", "--out", self.out, "--job-status", "success").returncode, 1)
        self.assertFalse(self.result()["source_unchanged"])

    def test_execution_does_not_certify_build_or_native_ros(self):
        self.begin("worked")
        self.assertEqual(self.command("worked", "pass", "--dimension", "execution").returncode, 0)
        self.assertEqual(self.invoke("seal", "--out", self.out, "--job-status", "success").returncode, 0)
        dimensions = self.result()["dimensions"]
        self.assertEqual(dimensions["execution"]["status"], "PASS")
        self.assertEqual(dimensions["build"]["status"], "UNTESTED")
        self.assertEqual(dimensions["native_ros"]["status"], "UNTESTED")

    def test_nested_orchestration_keeps_subcommands(self):
        self.begin("outer", "inner")
        argv = [sys.executable, "-B", str(SCRIPT), "command", "--out", str(self.out), "--id", "inner",
                "--timeout", "5", "--", sys.executable, "-B", "-c", "pass"]
        code = "import subprocess; raise SystemExit(subprocess.run(" + repr(argv) + ").returncode)"
        self.assertEqual(self.command("outer", code).returncode, 0)
        self.assertEqual(self.invoke("seal", "--out", self.out, "--job-status", "success").returncode, 0)
        self.assertEqual([row["id"] for row in self.result()["commands"]], ["outer", "inner"])

    def test_timeout_is_a_failure(self):
        self.begin("slow")
        result = self.invoke("command", "--out", self.out, "--id", "slow", "--timeout", "0.1", "--",
                             sys.executable, "-B", "-c", "import time; time.sleep(30)")
        self.assertEqual(result.returncode, 1)
        self.assertEqual(self.result()["commands"][0]["reason"], "bounded command timeout")


if __name__ == "__main__":
    unittest.main()
