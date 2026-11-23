#!/usr/bin/env python3
"""Focused filesystem-socket regression; no VM permissions are weakened."""
from concurrent.futures import ThreadPoolExecutor
import os
from pathlib import Path
import socket
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
from private_unix_socket import bind_private, connect_private


@unittest.skipUnless(sys.platform in ("linux", "darwin"), "Unix broker")
class PrivateSocketTests(unittest.TestCase):
    def setUp(self):
        (ROOT / ".tmp").mkdir(exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=ROOT / ".tmp", prefix="socket-path-")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def exchange(self, directory, kind):
        directory.mkdir(mode=0o700, parents=True)
        name = directory / "control.sock"
        before = os.stat(".")
        with socket.socket(socket.AF_UNIX, kind) as server, socket.socket(socket.AF_UNIX, kind) as client:
            server.settimeout(3); client.settimeout(3)
            bind_private(server, name); os.chmod(name, 0o600); server.listen(1)
            connect_private(client, name)
            with server.accept()[0] as accepted:
                client.sendall(b"checked directory")
                self.assertEqual(accepted.recv(64), b"checked directory")
                accepted.sendall(b"reply")
                self.assertEqual(client.recv(64), b"reply")
        self.assertEqual((before.st_dev, before.st_ino), (os.stat('.').st_dev, os.stat('.').st_ino))
        name.unlink()

    def test_long_multibyte_and_short_directories(self):
        for index, suffix in enumerate(("short", "工业部署" * 20 + "/" + "nested" * 25)):
            for kind in (socket.SOCK_STREAM, socket.SOCK_SEQPACKET) if sys.platform == 'linux' else (socket.SOCK_STREAM,):
                with self.subTest(suffix=suffix, kind=kind):
                    self.exchange(self.root / str(index) / str(kind) / suffix, kind)

    def test_concurrent_sessions_keep_process_cwd(self):
        with ThreadPoolExecutor(max_workers=4) as pool:
            jobs=[pool.submit(self.exchange, self.root / str(i) / ("long" * 40), socket.SOCK_STREAM) for i in range(8)]
            for job in jobs: job.result()

    def test_nonprivate_or_symlink_directory_refused(self):
        private = self.root / 'private'; private.mkdir(mode=0o700)
        alias = self.root / 'alias'; alias.symlink_to(private, target_is_directory=True)
        with socket.socket(socket.AF_UNIX) as channel:
            with self.assertRaises(OSError): bind_private(channel, alias / 'control.sock')
            private.chmod(0o755)
            with self.assertRaises(RuntimeError): bind_private(channel, private / 'control.sock')
        self.assertFalse((private / 'control.sock').exists())

    def test_darwin_thread_local_directory_restored_on_error(self):
        # Exercises the worker/error contract; Darwin's real syscall still
        # requires a macOS run and is not emulated by this Linux-hosted check.
        import ctypes
        import threading
        from types import SimpleNamespace
        from unittest.mock import Mock, patch
        from private_unix_socket import _darwin_operation
        caller = threading.get_ident()
        change = Mock(return_value=0)
        original = RuntimeError("connect failed")
        def operation(name):
            self.assertEqual(name, "control.sock")
            self.assertNotEqual(threading.get_ident(), caller)
            raise original
        with patch.object(ctypes, 'CDLL', return_value=SimpleNamespace(pthread_fchdir_np=change)):
            with self.assertRaises(RuntimeError) as result:
                _darwin_operation(operation, 123, "control.sock")
        self.assertIs(result.exception, original)
        self.assertEqual([args.args[0] for args in change.call_args_list], [123, -1])

    def test_update_relay_long_path_and_cleanup(self):
        if sys.platform != 'linux': self.skipTest('Linux signed update relay')
        from signed_update import Relay, SOCKET_NAME, CAPABILITY_NAME
        directory = self.root / ('update-' * 24); directory.mkdir(mode=0o700)
        relay = Relay(directory, [])
        try:
            self.assertTrue((directory / SOCKET_NAME).is_socket())
            self.assertEqual(len((directory / CAPABILITY_NAME).read_bytes()), 32)
        finally: relay.close()
        self.assertFalse((directory / SOCKET_NAME).exists())
        self.assertFalse((directory / CAPABILITY_NAME).exists())

if __name__ == '__main__': unittest.main()
