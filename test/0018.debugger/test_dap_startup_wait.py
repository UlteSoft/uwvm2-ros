"""Linux startup transport tests; fake peers qualify protocol behavior, not Wasm."""
import importlib.util
import io
import os
from pathlib import Path
import signal
import socket
import subprocess
import sys
import tempfile
import threading
import time
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result
dap = module("uwvm_startup_dap", ROOT / "tools/debug/dap_adapter.py")
server = module("uwvm_startup_server", ROOT / "tools/debug/secure_server.py")

@unittest.skipUnless(sys.platform == "linux" and hasattr(os, "pidfd_open"), "Linux pidfd transport")
class StartupWait(unittest.TestCase):
    def setUp(self):
        self.guest = subprocess.Popen([sys.executable, "-c", "import signal; signal.pause()"])
        self.client, self.host = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        self.host.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        self.host.settimeout(.2)
        self.vm, self.reply = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        self.vm.settimeout(.2)
        self.pidfd = os.pidfd_open(os.getpid())
        self.identity = (os.getpid(), os.geteuid(), os.getegid())
    def tearDown(self):
        for item in (self.client, self.host, self.vm, self.reply):
            item.close()
        os.close(self.pidfd)
        if self.guest.poll() is None:
            self.guest.terminate()
        self.guest.wait(timeout=5)
    def wait(self, packet=b"status", timeout=None):
        return server.wait_startup_reply(self.host, self.identity, self.pidfd, self.vm,
                                         self.guest, packet, timeout)
    def test_default_wait_uses_ready_event_without_clock_deadline(self):
        self.reply.send(b"prepared\n")
        with mock.patch.object(server.time, "monotonic", side_effect=AssertionError("no default deadline")):
            self.assertEqual(self.wait(), b"prepared\n")
    def test_explicit_deadline(self):
        with self.assertRaisesRegex(server.VMChannelFailure, "explicit host deadline"):
            self.wait(timeout=.01)
    def test_closed_client_cancels_inflight_unsequenced_request(self):
        self.client.close()
        with self.assertRaisesRegex(server.VMChannelFailure, "canceled"):
            self.wait()
    def test_authenticated_disconnect_cancels(self):
        self.client.send(b"disconnect")
        with self.assertRaisesRegex(server.VMChannelFailure, "canceled"):
            self.wait()
    def test_pipelining_cannot_consume_late_reply(self):
        self.client.send(b"status")
        with self.assertRaisesRegex(server.VMChannelFailure, "before startup"):
            self.wait()
    def test_original_guest_exit_is_terminal_status(self):
        self.guest.terminate()
        self.guest.wait(timeout=5)
        self.assertEqual(self.wait(), b"guest exited: -15\n")
    def test_original_guest_exit_never_acknowledges_mutation(self):
        self.guest.terminate()
        self.guest.wait(timeout=5)
        with self.assertRaisesRegex(server.VMChannelFailure, "original VM exited"):
            self.wait(b"continue")

@unittest.skipUnless(sys.platform == "linux", "Linux event-driven transport")
class ActualClientWait(unittest.TestCase):
    def setUp(self):
        self.channel, self.peer = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        self.broker = object.__new__(dap.UnixBroker)
        self.broker.channel = self.channel
        self.broker.first_request = True
        self.broker.startup_timeout = None
    def tearDown(self):
        self.channel.close()
        self.peer.close()
    def test_actual_ready_event_without_default_deadline(self):
        self.peer.send(b"ready-result")
        with mock.patch.object(dap.time, "monotonic", side_effect=AssertionError("no implicit deadline")):
            self.assertEqual(self.broker.request("status"), "ready-result")
        self.assertEqual(self.peer.recv(64), b"status")
        self.assertEqual(self.channel.gettimeout(), 8)
    def test_explicit_client_deadline_retires_connection(self):
        self.broker.startup_timeout = .01
        with self.assertRaisesRegex(TimeoutError, "explicit IDE deadline"):
            self.broker.request("status")
        self.assertEqual(self.channel.fileno(), -1)
    def test_close_from_reader_thread_unblocks_poll(self):
        result = []
        def run():
            try: self.broker.request("status")
            except (OSError, ValueError) as error: result.append(type(error).__name__)
        thread = threading.Thread(target=run)
        thread.start()
        self.peer.settimeout(2)
        self.assertEqual(self.peer.recv(64), b"status")
        self.broker.close()
        thread.join(2)
        self.assertFalse(thread.is_alive())
        self.assertEqual(len(result), 1)
        self.assertEqual(self.channel.fileno(), -1)

class PendingBroker:
    def __init__(self):
        self.entered = threading.Event()
        self.released = threading.Event()
        self.closed = False
    def request(self, command):
        if command == "permissions": return "permissions full\n"
        assert command == "status"
        self.entered.set()
        if not self.released.wait(5):
            raise AssertionError("test failed to release pending broker")
        if self.closed:
            raise ConnectionError("startup connection closed")
        return "stopped: prepared; no Wasm instruction executed\nstop-id 1\n"
    def close(self):
        self.closed = True
        self.released.set()

class AdapterCancellation(unittest.TestCase):
    def setUp(self):
        self.output = io.BytesIO()
        self.adapter = dap.Adapter(self.output)
        self.request = {"seq": 11, "command": "attach", "arguments": {"socketDir": "/owned"}}
    def test_cancel_queued_before_attach_reaches_broker(self):
        self.adapter.note_input(self.request)
        self.adapter.note_input({"seq":12,"command":"cancel","arguments":{"requestId":11}})
        with mock.patch.object(dap, "UnixBroker") as factory:
            self.adapter.handle(self.request)
        factory.assert_not_called()
        self.assertIn(b'"message":"cancelled"', self.output.getvalue())
        self.assertIn(b'"showUser":false', self.output.getvalue())
        self.assertIsNone(self.adapter.startup_request)
    def canceled(self, request):
        broker = PendingBroker()
        self.adapter.note_input(self.request)
        with mock.patch.object(dap, "UnixBroker", return_value=broker):
            thread = threading.Thread(target=self.adapter.handle, args=(self.request,))
            thread.start()
            self.assertTrue(broker.entered.wait(2))
            self.adapter.note_input(request)
            thread.join(2)
            self.assertFalse(thread.is_alive())
        self.assertTrue(broker.closed)
        self.assertIn(b'"message":"cancelled"', self.output.getvalue())
        self.assertIn(b'"showUser":false', self.output.getvalue())
        self.assertIsNone(self.adapter.broker)
        self.assertIsNone(self.adapter.startup_request)
        self.assertNotIn(b'"event":"initialized"', self.output.getvalue())
    def test_matching_cancel_interrupts_startup(self):
        self.canceled({"seq":12,"command":"cancel","arguments":{"requestId":11}})
    def test_disconnect_interrupts_startup(self):
        self.canceled({"seq":12,"command":"disconnect"})
    def test_input_eof_interrupts_startup(self):
        self.canceled(None)
    def test_unrelated_cancel_cannot_interrupt_other_request(self):
        broker = PendingBroker()
        self.adapter.note_input(self.request)
        with mock.patch.object(dap, "UnixBroker", return_value=broker):
            thread = threading.Thread(target=self.adapter.handle, args=(self.request,))
            thread.start()
            self.assertTrue(broker.entered.wait(2))
            self.adapter.note_input({"seq":12,"command":"cancel","arguments":{"requestId":10}})
            self.assertFalse(broker.closed)
            self.adapter.note_input({"seq":13,"command":"cancel","arguments":{"requestId":11,"unknown":True}})
            self.assertFalse(broker.closed)
            broker.released.set()
            thread.join(2)
            self.assertFalse(thread.is_alive())
        self.assertIs(self.adapter.broker, broker)
        self.assertIn(b'"event":"initialized"', self.output.getvalue())
    def test_invalid_deadlines_never_contact_broker(self):
        for value in (True, 0, -1, "10", float("nan"), float("inf"), 10**400):
            with self.subTest(value=repr(value)), mock.patch.object(dap, "UnixBroker") as factory:
                self.request["arguments"]["startupTimeoutSeconds"] = value
                self.adapter.handle(self.request)
                factory.assert_not_called()

@unittest.skipUnless(sys.platform == "linux" and hasattr(os, "pidfd_open"), "Linux pidfd transport")
class RealSupervisorCancellation(unittest.TestCase):
    def test_cancel_retires_original_process_and_removes_reconnect_endpoint(self):
        # The child below intentionally emulates an unready transport only.
        # Full JIT startup is independently qualified with real SDK Wasm in IDE.
        with tempfile.TemporaryDirectory(prefix="startup-", dir=ROOT / ".tmp") as directory:
            root = Path(directory)
            child = root / "peer.py"
            child.write_text("#!/usr/bin/python3\nimport os,signal,socket,sys\nfrom pathlib import Path\np=Path(__file__).parent\np.joinpath('pid').write_text(str(os.getpid()))\nc=socket.socket(fileno=int(sys.argv[sys.argv.index('--debug-jit-control-fd')+1]))\nc.recv(8448)\np.joinpath('queued').write_text('yes')\nsignal.pause()\n")
            child.chmod(0o700)
            with (root / "broker.log").open("w") as log:
                broker = subprocess.Popen([sys.executable, str(ROOT/"tools/debug/secure_server.py"),
                    "serve", "--uwvm", str(child), "--socket-dir", str(root), "--", "--run", "transport-fixture"],
                    stdout=log, stderr=subprocess.STDOUT)
                guest = None
                try:
                    end = time.monotonic()+5
                    while not (root/"control.sock").exists() and time.monotonic()<end:
                        self.assertIsNone(broker.poll())
                        time.sleep(.01)
                    client = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
                    with client:
                        client.settimeout(5)
                        client.connect(str(root/"control.sock"))
                        client.send((root/"capability").read_bytes())
                        self.assertEqual(client.recv(64), b"ready\n")
                        client.send(b"status")
                        while not (root/"queued").exists() and time.monotonic()<end:
                            time.sleep(.01)
                        self.assertTrue((root/"queued").exists())
                        guest = int((root/"pid").read_text())
                    self.assertNotEqual(broker.wait(timeout=5), 0)
                    self.assertFalse((root/"control.sock").exists())
                    self.assertFalse((root/"capability").exists())
                    with self.assertRaises(ProcessLookupError):
                        os.kill(guest, 0)
                finally:
                    if broker.poll() is None:
                        broker.terminate()
                        broker.wait(timeout=5)
                    if guest is not None:
                        try: os.kill(guest, signal.SIGKILL)
                        except ProcessLookupError: pass

if __name__ == "__main__":
    unittest.main()
