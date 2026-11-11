"""Actual Linux packet/pidfd tests with a transport fixture, not a Wasm/GUI claim."""
import array
import importlib.util
import os
from pathlib import Path
import socket
import subprocess
import sys
import threading
import time
from types import SimpleNamespace
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location("secure_observer_server", ROOT / "tools/debug/secure_server.py")
server = importlib.util.module_from_spec(spec)
spec.loader.exec_module(server)
spec_mac = importlib.util.spec_from_file_location("secure_observer_stream", ROOT / "tools/debug/secure_server_macos.py")
mac = importlib.util.module_from_spec(spec_mac)
spec_mac.loader.exec_module(mac)


@unittest.skipUnless(sys.platform == "linux" and hasattr(os, "pidfd_open"), "Linux credentials and pidfd")
class ObserverCancellation(unittest.TestCase):
    def setUp(self):
        self.guest = subprocess.Popen([sys.executable, "-c", "import signal; signal.pause()"])
        self.vm, self.peer = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        self.vm.settimeout(.2)
        self.peer.settimeout(2)
        self.channels = [self.vm, self.peer]
        self.pidfds = []
        self.identity = (os.getpid(), os.geteuid(), os.getegid())
        self.threads = []
        self.client, self.host, self.pidfd = self.client_pair()

    def tearDown(self):
        # Wake a failed test's transport waiter before joining it. No background
        # waiter is allowed to outlive the fixture's owned descriptors/process.
        for channel in self.channels:
            try:
                channel.shutdown(socket.SHUT_RDWR)
            except OSError:
                pass
        for thread in self.threads:
            thread.join(3)
        for channel in self.channels:
            channel.close()
        for fd in self.pidfds:
            os.close(fd)
        if self.guest.poll() is None:
            self.guest.terminate()
        self.guest.wait(timeout=5)

    def client_pair(self):
        client, host = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        host.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        host.settimeout(.2)
        client.settimeout(2)
        pidfd = os.pidfd_open(os.getpid())
        self.channels += [client, host]
        self.pidfds.append(pidfd)
        return client, host, pidfd

    def background(self, call):
        result, errors = [], []
        def invoke():
            try:
                result.append(call())
            except BaseException as error:
                errors.append(error)
        thread = threading.Thread(target=invoke)
        self.threads.append(thread)
        thread.start()
        return thread, result, errors

    def wait(self, *, timeout=None, control_allowed=False, packet=b"status"):
        return server.wait_startup_reply(self.host, self.identity, self.pidfd, self.vm,
                                         self.guest, packet, timeout, control_allowed=control_allowed)

    def canceled_then_drained(self, cancel):
        observed = threading.Event()
        checked = server.checked_packet
        def record(channel, *args):
            try:
                return checked(channel, *args)
            finally:
                if channel is self.host:
                    observed.set()
        with mock.patch.object(server, "checked_packet", side_effect=record):
            thread, result, errors = self.background(self.wait)
            cancel()
            self.assertTrue(observed.wait(2))
            self.assertTrue(thread.is_alive(), "the outstanding VM reply must still be drained")
            self.assertIsNone(self.guest.poll())
            self.peer.send(b"old-observer-reply\n")
            thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(result, [server._OBSERVER_DISCONNECTED])
        self.assertIsNone(self.guest.poll())
        # The original endpoint remains owned. A new authenticated observation
        # receives its own response, never the canceled command's late packet.
        client, host, pidfd = self.client_pair()
        self.peer.send(b"new-observer-reply\n")
        self.assertEqual(server.wait_startup_reply(host, self.identity, pidfd, self.vm,
                         self.guest, b"status", control_allowed=False), b"new-observer-reply\n")

    def test_authenticated_disconnect_drains_without_retiring_guest(self):
        self.canceled_then_drained(lambda: self.client.send(b"disconnect"))

    def test_client_eof_drains_without_retiring_guest(self):
        self.canceled_then_drained(self.client.close)

    def test_pipelining_only_retires_observer_and_drains(self):
        self.canceled_then_drained(lambda: self.client.send(b"continue"))

    def test_bad_observer_ancillary_data_cannot_cancel_guest(self):
        reader, writer = os.pipe()
        try:
            self.canceled_then_drained(lambda: self.client.sendmsg([b"disconnect"],
                [(socket.SOL_SOCKET, socket.SCM_RIGHTS, array.array("i", [writer]))]))
        finally:
            os.close(reader)
            os.close(writer)

    def test_explicit_host_deadline_remains_independent_after_disconnect(self):
        self.client.send(b"disconnect")
        with self.assertRaisesRegex(server.VMChannelFailure, "explicit host deadline"):
            self.wait(timeout=.02)
        self.assertIsNone(self.guest.poll())  # serve() owns the host failure policy.

    def test_actual_vm_channel_failure_remains_failure_after_disconnect(self):
        self.client.send(b"disconnect")
        self.peer.close()
        with self.assertRaisesRegex(server.VMChannelFailure, "control channel closed"):
            self.wait(packet=b"permissions")

    def test_full_control_cancel_retains_launch_retirement_signal(self):
        self.client.send(b"disconnect")
        with self.assertRaisesRegex(server.VMChannelFailure, "client canceled"):
            self.wait(control_allowed=True)

    def test_serve_client_cancel_then_reconnect_has_no_replay_or_reply_shift(self):
        capability = b"c" * server.CAPABILITY_BYTES
        pending = [True]
        def session(client, host, pidfd):
            return self.background(lambda: server.serve_client(host, self.identity, pidfd,
                self.vm, capability, self.guest, pending, control_allowed=False))
        thread, result, errors = session(self.client, self.host, self.pidfd)
        self.client.send(capability)
        self.assertEqual(self.client.recv(64), b"ready\n")
        self.client.send(b"status")
        self.assertEqual(self.peer.recv(64), b"status")
        self.client.close()
        self.peer.send(b"retired-client-status\n")
        thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(result, [False])
        self.assertFalse(pending[0])
        self.assertIsNone(self.guest.poll())
        client, host, pidfd = self.client_pair()
        thread, result, errors = session(client, host, pidfd)
        client.send(capability)
        self.assertEqual(client.recv(64), b"ready\n")
        client.send(b"status")
        self.assertEqual(self.peer.recv(64), b"status")
        self.peer.send(b"fresh-client-status\n")
        self.assertEqual(client.recv(64), b"fresh-client-status\n")
        client.send(b"disconnect")
        thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(result, [False])

    def test_observer_quit_never_reaches_or_closes_owner_endpoint(self):
        capability = b"c" * server.CAPABILITY_BYTES
        pending = [True]
        thread, result, errors = self.background(lambda: server.serve_client(self.host,
            self.identity, self.pidfd, self.vm, capability, self.guest, pending, control_allowed=False))
        self.client.send(capability)
        self.assertEqual(self.client.recv(64), b"ready\n")
        self.client.send(b"quit")
        self.assertEqual(self.client.recv(128), b"observer detached; guest state unchanged\n")
        thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(result, [False])
        self.assertTrue(pending[0])
        self.assertIsNone(self.guest.poll())
        self.peer.settimeout(.02)
        with self.assertRaises(socket.timeout):
            self.peer.recv(64)

    def test_later_observation_has_no_implicit_reply_deadline(self):
        capability = b"c" * server.CAPABILITY_BYTES
        with mock.patch.object(server, "wait_startup_reply", wraps=server.wait_startup_reply) as waited:
            thread, result, errors = self.background(lambda: server.serve_client(self.host,
                self.identity, self.pidfd, self.vm, capability, self.guest, [False], .001, control_allowed=False))
            self.client.send(capability)
            self.assertEqual(self.client.recv(64), b"ready\n")
            self.client.send(b"wait")
            self.assertEqual(self.peer.recv(64), b"wait")
            self.client.close()
            time.sleep(.02)
            self.assertTrue(thread.is_alive())
            self.assertIsNone(self.guest.poll())
            self.peer.send(b"later-wait-reply\n")
            thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(result, [False])
        self.assertIsNone(waited.call_args.args[-1])


@unittest.skipUnless(hasattr(socket.socket, "recvmsg"), "Unix stream sockets")
class StreamObservation(unittest.TestCase):
    """The Darwin framing implementation on real Linux streams; no Darwin credentials claim."""
    def setUp(self):
        self.guest = subprocess.Popen([sys.executable, "-c", "import signal; signal.pause()"])
        self.vm, self.peer = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)
        self.client, self.host = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)
        self.channels = [self.vm, self.peer, self.client, self.host]
        for channel in self.channels:
            channel.settimeout(.2)
        self.threads = []

    def tearDown(self):
        for channel in self.channels:
            try:
                channel.shutdown(socket.SHUT_RDWR)
            except OSError:
                pass
        for thread in self.threads:
            thread.join(3)
        for channel in self.channels:
            channel.close()
        if self.guest.poll() is None:
            self.guest.terminate()
        self.guest.wait(timeout=5)

    def background(self, call):
        result, errors = [], []
        def invoke():
            try:
                result.append(call())
            except BaseException as error:
                errors.append(error)
        thread = threading.Thread(target=invoke)
        self.threads.append(thread)
        thread.start()
        return thread, result, errors

    def test_fragmented_canceled_reply_is_drained_once_before_next_session(self):
        observed = threading.Event()
        read = mac.recv_piece
        def record(channel, maximum):
            try:
                return read(channel, maximum)
            finally:
                if channel is self.host:
                    observed.set()
        with mock.patch.object(mac, "recv_piece", side_effect=record):
            thread, result, errors = self.background(lambda: mac.wait_observer_reply(
                self.host, self.vm, self.guest, b"wait"))
            self.peer.sendall(b"\0\0")
            mac.send_frame(self.client, b"disconnect", mac.MAX_COMMAND)
            self.assertTrue(observed.wait(2))
            self.assertTrue(thread.is_alive())
            self.assertIsNone(self.guest.poll())
            self.peer.sendall(b"\0\x05ol")
            self.assertTrue(thread.is_alive())
            self.peer.sendall(b"d!\n")
            thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(result, [mac._OBSERVER_DISCONNECTED])
        self.assertEqual(self.vm.gettimeout(), .2)
        client, host = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)
        self.channels += [client, host]
        mac.send_frame(self.peer, b"fresh\n", mac.MAX_REPLY)
        self.assertEqual(mac.wait_observer_reply(host, self.vm, self.guest, b"status"), b"fresh\n")

    def test_eof_and_late_reply_preserve_guest_without_default_timeout(self):
        self.vm.settimeout(.001)
        thread, result, errors = self.background(lambda: mac.wait_observer_reply(
            self.host, self.vm, self.guest, b"wait"))
        self.client.close()
        time.sleep(.02)
        self.assertTrue(thread.is_alive())
        self.assertIsNone(self.guest.poll())
        mac.send_frame(self.peer, b"late\n", mac.MAX_REPLY)
        thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(result, [mac._OBSERVER_DISCONNECTED])
        self.assertEqual(self.vm.gettimeout(), .001)

    def test_explicit_host_deadline_remains_after_observer_eof(self):
        self.client.close()
        with self.assertRaisesRegex(mac.VMChannelFailure, "explicit host deadline"):
            mac.wait_observer_reply(self.host, self.vm, self.guest, b"wait", .02)

    def test_partial_vm_frame_eof_is_not_reusable(self):
        self.peer.sendall(b"\0\0\0\x05ab")
        self.peer.close()
        with self.assertRaisesRegex(mac.VMChannelFailure, "outstanding reply"):
            mac.wait_observer_reply(self.host, self.vm, self.guest, b"status")

    def test_oversized_vm_frame_is_terminal_channel_failure(self):
        self.peer.sendall(b"\0\x01\0\x01")
        with self.assertRaisesRegex(mac.VMChannelFailure, "bounded VM reply"):
            mac.wait_observer_reply(self.host, self.vm, self.guest, b"status")

    def test_observer_quit_stays_local_on_stream_broker(self):
        capability = b"c" * mac.CAPABILITY_BYTES
        peer = (os.getpid(), os.geteuid(), os.getegid(), b"test-only-identity")
        pending = [True]
        # Peer-credential API is Darwin-only; only this call is substituted.
        # The framing, authentication token, EOF, ownership and process are real.
        with mock.patch.object(mac, "identity", return_value=peer):
            thread, result, errors = self.background(lambda: mac.serve_client(self.host,
                peer, self.vm, capability, self.guest, pending, control_allowed=False))
            mac.send_frame(self.client, capability, mac.CAPABILITY_BYTES)
            self.assertEqual(mac.read_frame(self.client, mac.MAX_REPLY), b"ready\n")
            mac.send_frame(self.client, b"quit", mac.MAX_COMMAND)
            self.assertEqual(mac.read_frame(self.client, mac.MAX_REPLY), b"observer detached; guest state unchanged\n")
            thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(result, [False])
        self.assertTrue(pending[0])
        self.assertIsNone(self.guest.poll())
        self.peer.settimeout(.01)
        with self.assertRaises(socket.timeout):
            self.peer.recv(64)


class NoDebuggerEndpoint(unittest.TestCase):
    def test_explicit_none_overrides_runtime_debug_without_creating_capabilities(self):
        for implementation in (server, mac):
            with self.subTest(platform=implementation.__name__):
                arguments = SimpleNamespace(uwvm=Path('/fixture-only'), socket_dir=None,
                    vm_args=['--', '-Rdbg', '--debug-permissions', 'none', '--run', 'fixture'])
                with mock.patch.object(sys, 'path', [str(ROOT/'tools/debug'), *sys.path]), \
                     mock.patch.object(implementation, 'private_directory', side_effect=AssertionError('no debug directory')), \
                     mock.patch.object(implementation.subprocess, 'run', return_value=SimpleNamespace(returncode=23)) as launched:
                    self.assertEqual(implementation.serve(arguments), 23)
                    launched.assert_called_once_with(['/fixture-only', '-Rdbg', '--debug-permissions', 'none', '--run', 'fixture'], check=False)


if __name__ == "__main__":
    unittest.main()
