#!/usr/bin/env python3
"""Deterministic procfs race regressions for the actual distribution runner.

These tests use the maintained identity/acquire/discover/rss implementations.
Only procfs reads, pidfd_open and descriptor close are mocked; no process,
compiler, ROS, cgroup mutation or signal is launched. The implementation agent
has prepared this source without executing it. Root runs it in the serial lane.
"""
import errno
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import Mock, patch

sys.dont_write_bytecode = True
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/ros_distro"))
import common


EXIT = (errno.ENOENT, errno.ESRCH)
FATAL = (errno.EACCES, errno.EPERM, errno.EIO)
PID, PARENT, START, FD = 123, 44, 789, 88
FIRST = {"pid": PID, "parent_pid": PARENT, "starttime": START, "state": "R"}


def runner(owned=True):
    # Bypass only constructor platform admission: the tested methods execute
    # unchanged, and every OS-facing operation they need is explicitly mocked.
    value = object.__new__(common.SerialRunner)
    value.owned = {(PID, START): {"identity": dict(FIRST), "identity_after": dict(FIRST), "fd": FD}} if owned else {}
    value.unproven = {}
    return value


def failed_iteration(code):
    # The exception is raised on next(), matching Path.iterdir's lazy failure.
    raise OSError(code, "deterministic vanished task directory")
    yield  # pragma: no cover - keep this an intentionally lazy iterator.


class ProcfsExitContracts(unittest.TestCase):
    def test_identity_absence_and_exit_only(self):
        for code in EXIT:
            with self.subTest(errno=code), patch.object(common.Path, "read_text", side_effect=OSError(code, "gone")):
                self.assertIsNone(common.identity(PID))

    def test_identity_permission_io_and_misclassified_exceptions_propagate(self):
        errors = [OSError(code, "fatal") for code in FATAL] + [FileNotFoundError(errno.EACCES, "wrong subclass"), ProcessLookupError()]
        for error in errors:
            with self.subTest(error=repr(error)), patch.object(common.Path, "read_text", side_effect=error):
                with self.assertRaises(OSError) as caught:
                    common.identity(PID)
                self.assertIs(caught.exception, error)

    def test_identity_malformed_stat_is_not_an_exit(self):
        with patch.object(common.Path, "read_text", return_value="malformed stat"):
            with self.assertRaises(ValueError):
                common.identity(PID)

    def test_lazy_task_iteration_exit_is_benign(self):
        for code in EXIT:
            value = runner(); value.acquire = Mock()
            with self.subTest(errno=code), patch.object(common, "identity", return_value=dict(FIRST)), \
                    patch.object(common.Path, "iterdir", return_value=failed_iteration(code)):
                value.discover()
            value.acquire.assert_not_called()
            self.assertEqual(set(value.owned), {(PID, START)})

    def test_children_read_exit_is_benign(self):
        for code in EXIT:
            value = runner(); value.acquire = Mock()
            with self.subTest(errno=code), patch.object(common, "identity", return_value=dict(FIRST)), \
                    patch.object(common.Path, "iterdir", return_value=iter([Path("/proc/123/task/123")])), \
                    patch.object(common.Path, "read_text", side_effect=OSError(code, "task exited")):
                value.discover()
            value.acquire.assert_not_called()
            self.assertEqual(set(value.owned), {(PID, START)})

    def test_directory_and_children_nonexit_faults_propagate(self):
        for code in FATAL:
            for site in ("directory", "children"):
                value = runner()
                directory = failed_iteration(code) if site == "directory" else iter([Path("/proc/123/task/123")])
                with self.subTest(errno=code, site=site), patch.object(common, "identity", return_value=dict(FIRST)), \
                        patch.object(common.Path, "iterdir", return_value=directory), \
                        patch.object(common.Path, "read_text", side_effect=OSError(code, "fatal")):
                    with self.assertRaises(OSError) as caught:
                        value.discover()
                self.assertEqual(caught.exception.errno, code)

    def test_acquire_errors_are_outside_children_read_handler(self):
        for code in EXIT + FATAL:
            value = runner(); error = OSError(code, "ownership failure")
            value.acquire = Mock(side_effect=error)
            with self.subTest(errno=code), patch.object(common, "identity", return_value=dict(FIRST)), \
                    patch.object(common.Path, "iterdir", return_value=iter([Path("/proc/123/task/123")])), \
                    patch.object(common.Path, "read_text", return_value="456"):
                with self.assertRaises(OSError) as caught:
                    value.discover()
            self.assertIs(caught.exception, error)

    def test_owned_target_cgroup_exit_keeps_the_registered_pidfd(self):
        for code in EXIT:
            value = runner(False)
            with self.subTest(errno=code), patch.object(common, "identity", side_effect=[dict(FIRST), dict(FIRST)]), \
                    patch.object(common.os, "pidfd_open", return_value=FD, create=True) as opened, \
                    patch.object(common.os, "close") as close, \
                    patch.object(common.Path, "read_text", side_effect=OSError(code, "owned target exited")) as read:
                value.acquire(PID, PARENT)
            opened.assert_called_once_with(PID)
            self.assertEqual(value.owned[(PID, START)]["fd"], FD)
            self.assertEqual(read.call_count, 1)
            close.assert_not_called()

    def test_coordinator_cgroup_faults_never_mean_target_exit(self):
        for code in EXIT + FATAL:
            value = runner(False); error = OSError(code, "coordinator proof unavailable")
            with self.subTest(errno=code), patch.object(common, "identity", side_effect=[dict(FIRST), dict(FIRST)]), \
                    patch.object(common.os, "pidfd_open", return_value=FD, create=True), \
                    patch.object(common.os, "close") as close, \
                    patch.object(common.Path, "read_text", side_effect=["0::/original\n", error]):
                with self.assertRaises(OSError) as caught:
                    value.acquire(PID, PARENT)
            self.assertIs(caught.exception, error)
            self.assertEqual(value.owned[(PID, START)]["fd"], FD)
            close.assert_not_called()

    def test_cgroup_escape_and_nonexit_target_faults_remain_fatal(self):
        for failure in [OSError(code, "fatal target read") for code in FATAL] + [None]:
            value = runner(False)
            reads = [failure] if failure is not None else ["0::/escaped\n", "0::/original\n"]
            with self.subTest(failure=repr(failure)), patch.object(common, "identity", side_effect=[dict(FIRST), dict(FIRST)]), \
                    patch.object(common.os, "pidfd_open", return_value=FD, create=True), \
                    patch.object(common.os, "close") as close, patch.object(common.Path, "read_text", side_effect=reads):
                with self.assertRaises(OSError if failure is not None else ValueError):
                    value.acquire(PID, PARENT)
            self.assertEqual(value.owned[(PID, START)]["fd"], FD)
            close.assert_not_called()

    def test_birth_and_initial_parent_contracts_are_unchanged(self):
        value = runner(False); replacement = dict(FIRST, starttime=START + 1)
        with patch.object(common, "identity", side_effect=[dict(FIRST), replacement]), \
                patch.object(common.os, "pidfd_open", return_value=FD, create=True), patch.object(common.os, "close") as close:
            with self.assertRaisesRegex(ValueError, "birth changed"):
                value.acquire(PID, PARENT)
        self.assertEqual(value.owned, {}); close.assert_called_once_with(FD)
        value = runner(False)
        with patch.object(common, "identity", return_value=dict(FIRST, parent_pid=PARENT + 1)), \
                patch.object(common.os, "pidfd_open", create=True) as opened:
            value.acquire(PID, PARENT)
        self.assertEqual(value.owned, {}); opened.assert_not_called()
        self.assertEqual(value.unproven[(PID, START)]["parent_pid"], PARENT + 1)

    def test_rss_exit_only_and_numeric_value_are_preserved(self):
        for code in EXIT:
            value = runner()
            with self.subTest(errno=code), patch.object(common, "identity", return_value=dict(FIRST)), \
                    patch.object(common.Path, "read_text", side_effect=OSError(code, "exited")):
                self.assertEqual(value.rss(), 0)
            self.assertEqual(set(value.owned), {(PID, START)})
        with patch.object(common, "identity", return_value=dict(FIRST)), \
                patch.object(common.Path, "read_text", return_value="Name:\tcompiler\nVmRSS:\t321 kB\n"):
            self.assertEqual(runner().rss(), 321 * 1024)

    def test_rss_fatal_faults_and_replaced_birth_are_not_hidden(self):
        for code in FATAL:
            with self.subTest(errno=code), patch.object(common, "identity", return_value=dict(FIRST)), \
                    patch.object(common.Path, "read_text", side_effect=OSError(code, "fatal")):
                with self.assertRaises(OSError) as caught:
                    runner().rss()
            self.assertEqual(caught.exception.errno, code)
        with patch.object(common, "identity", return_value=dict(FIRST, starttime=START + 1)), \
                patch.object(common.Path, "read_text") as read:
            self.assertEqual(runner().rss(), 0)
        read.assert_not_called()


class DanglingInventoryContracts(unittest.TestCase):
    def test_dangling_link_is_preserved_without_invented_target_bytes(self):
        with tempfile.TemporaryDirectory() as temporary:
            base = Path(temporary).resolve(); root = base / "sdk"; root.mkdir()
            link = root / "dangling"; os.symlink("missing-target", link)
            first = common.catalog(root, base / "first.jsonl", [root])
            second = common.catalog(root, base / "second.jsonl", [root])
            row = json.loads((base / "first.jsonl").read_text())
            self.assertEqual(row["kind"], "symlink")
            self.assertEqual(row["target"], "missing-target")
            self.assertEqual(row["resolution_state"], "MISSING_TARGET_ENOENT")
            self.assertIsNone(row["resolved"])
            self.assertNotIn("target_file", row)
            self.assertEqual((first["files"], first["bytes"], first["symlinks"]), (0, 0, 1))
            self.assertEqual(first["content_sha256"], second["content_sha256"])
            # Required inputs still use strict resolution/opening. A catalog
            # entry does not turn this missing target into a usable tool.
            with self.assertRaises(FileNotFoundError):
                common.opened(link.resolve(strict=True))
            (root / "missing-target").write_bytes(b"actual later bytes")
            third = common.catalog(root, base / "third.jsonl", [root])
            self.assertNotEqual(first["content_sha256"], third["content_sha256"])

    def test_dangling_inventory_tolerates_enoent_only(self):
        for code in FATAL + (errno.ELOOP, errno.ESRCH):
            with self.subTest(errno=code), tempfile.TemporaryDirectory() as temporary:
                base = Path(temporary).resolve(); root = base / "sdk"; root.mkdir()
                link = root / "dangling"; os.symlink("missing-target", link)
                original = common.Path.resolve; error = OSError(code, "must remain fatal")
                def resolve(value, *args, **kwargs):
                    if value == link:
                        raise error
                    return original(value, *args, **kwargs)
                with patch.object(common.Path, "resolve", new=resolve):
                    with self.assertRaises(OSError) as caught:
                        common.catalog(root, base / "catalog.jsonl", [root])
                self.assertIs(caught.exception, error)

    def test_target_appearance_during_inventory_is_not_a_stable_missing_fact(self):
        with tempfile.TemporaryDirectory() as temporary:
            base = Path(temporary).resolve(); root = base / "sdk"; root.mkdir()
            target = root / "actual-target"; target.write_bytes(b"body")
            link = root / "dangling"; os.symlink("actual-target", link)
            original = common.Path.resolve; calls = []
            def resolve(value, *args, **kwargs):
                if value == link:
                    calls.append(True)
                    if len(calls) == 1:
                        raise FileNotFoundError(errno.ENOENT, "initial missing snapshot")
                return original(value, *args, **kwargs)
            with patch.object(common.Path, "resolve", new=resolve):
                with self.assertRaisesRegex(ValueError, "target appeared"):
                    common.catalog(root, base / "catalog.jsonl", [root])
            self.assertEqual(len(calls), 2)

    def test_link_target_spelling_must_remain_stable(self):
        with tempfile.TemporaryDirectory() as temporary:
            base = Path(temporary).resolve(); root = base / "sdk"; root.mkdir()
            link = root / "dangling"; os.symlink("missing-target", link)
            original = common.Path.resolve
            def resolve(value, *args, **kwargs):
                if value == link:
                    raise FileNotFoundError(errno.ENOENT, "still missing")
                return original(value, *args, **kwargs)
            with patch.object(common.Path, "resolve", new=resolve), \
                    patch.object(common.os, "readlink", side_effect=["missing-target", "changed-target"]):
                with self.assertRaisesRegex(ValueError, "symlink changed"):
                    common.catalog(root, base / "catalog.jsonl", [root])

    def test_existing_directory_symlink_escape_remains_forbidden(self):
        with tempfile.TemporaryDirectory() as temporary:
            base = Path(temporary).resolve(); root = base / "sdk"; root.mkdir()
            outside = base / "outside"; outside.mkdir()
            os.symlink(str(outside), root / "escaped")
            with self.assertRaisesRegex(ValueError, "directory symlink escapes"):
                common.catalog(root, base / "catalog.jsonl", [root])


if __name__ == "__main__":
    unittest.main()
