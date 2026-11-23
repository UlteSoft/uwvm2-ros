#!/usr/bin/env python3
"""Exercise the bounded IDE protocol against the debugger's actual text grammar."""

import array
import base64
import errno
import importlib.util
import io
import os
from pathlib import Path
import socket
import sys
import unittest
from fixture_wasm_metadata import metadata, code
import re
from unittest import mock


ADAPTER_PATH = Path(__file__).resolve().parents[2] / "tools/debug/dap_adapter.py"
SPEC = importlib.util.spec_from_file_location("uwvm_dap_adapter", ADAPTER_PATH)
dap = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(dap)


def messages(data):
    result = []
    stream = io.BytesIO(data)
    while prefix := stream.readline():
        assert prefix.startswith(b"Content-Length: ") and prefix.endswith(b"\r\n")
        length = int(prefix[16:-2])
        assert stream.readline() == b"\r\n"
        raw = stream.read(length)
        assert len(raw) == length
        result.append(__import__("json").loads(raw))
    return result


class FakeBroker:
    def __init__(self, *_):
        self.commands = []
        self.closed = False
        self.status = "stopped: pause\nstop-id 41\nthread 1 module=0 function=1 byte-offset=4 generation=1\n"
        self.points = {}

    def request(self, command):
        if command == "permissions": return "permissions full\n"
        self.commands.append(command)
        stop = re.search(r"stop-id (\d+)", self.status)
        owner = re.search(r"thread (\d+) module=(\d+) function=(\d+) byte-offset=(\d+) generation=(\d+)", self.status)
        if command.startswith(("wasm-meta ", "wasm-code ")):
            assert stop is not None and owner is not None, command
            thread, module, function, pc, epoch = map(int, owner.groups())
            inventory = metadata(command, stop=int(stop[1]), epoch=epoch, functions=2, memories=1, thread=thread)
            if inventory is not None: return inventory
            return code(command, stop=int(stop[1]), epoch=epoch, function=function, module=module, thread=thread, pc=pc)
        if command.startswith("frames "):
            return "error: source debug info unavailable\n"
        if command.startswith(("locals wasm ", "operands ", "controls ", "handlers ", "saved ")):
            assert stop is not None and owner is not None
            first = int(command.split()[-2]); locals_page = command.startswith("locals wasm ")
            rows = "local 0 i32 = 42\n" if locals_page and first == 0 else ""
            return f"wasm-stop {stop[1]}\nWasm state thread=1 module=0 epoch={owner[5]} first={first} total={int(locals_page)}\n" + rows
        if command == "info breakpoints":
            return ''.join(f"breakpoint {i} module=0 function=1 kind={kind} byte-offset=4 temporary=no enabled=yes hits=0 ignore=0\n" for i,kind in self.points.items()) or "no breakpoints\n"
        if command.startswith("disassemble-range "):
            return "error: native instructions unavailable in this protocol fixture\n"
        if command == "status":
            return self.status
        if command == "pause":
            self.status = "stopped: pause\nstop-id 41\nthread 1 module=0 function=1 byte-offset=4 generation=1\n"
            return self.status
        if command == "ni 1":
            return "error: native next requires a current authenticated native trap\n"
        if command.startswith("step "):
            self.status = ("stopped: selected participant step\nstop-id 41\n"
                           "thread 1 module=0 function=1 byte-offset=4 generation=1\n"
                           "  source src/main.c:4:2\n")
            return self.status
        if command.startswith("breaksource "):
            self.points[7] = "source"
            return "breakpoint 7 source=src/main.c:4 function=1 byte-offset=4\n"
        if command == "breakwasm 0 1 4":
            self.points[8] = "wasm"
            return "breakpoint 8 registered at executable Wasm expression byte offset\n"
        if command == "delete 7":
            self.points.pop(7, None)
            return "breakpoint deleted\n"
        if command == "delete 8":
            self.points.pop(8, None)
            return "breakpoint deleted\n"
        if command == "bt 1":
            return self.status + "  #0 module=0 function=1 [demo] main\n"
        if command == "locals 1":
            return "local 0 i32=42\n"
        if command == "memory 0 0 16 4":
            return "memory: 00 01 7f ff\n"
        if command == "continue":
            self.status = "running\n"
            return self.status
        raise AssertionError(command)

    def close(self):
        self.closed = True


# This is the exact formatter grammar from controller::snapshot() and
# console::format_reply(), also present in the archived native CLI raw logs.
# It intentionally has no #caller frame, DWARF source or current Wasm locals.
NATIVE_STATUS = ("stopped: native instruction step\nstop-id 41\n"
                 "thread 1 module=1 function=1 byte-offset=20 generation=1\n"
                 "  native-pc=0x000000010c75842c\n")
NATIVE_BACKTRACE = NATIVE_STATUS + "  backtrace unavailable for this stop\n"


class NativeBroker(FakeBroker):
    def __init__(self):
        super().__init__()
        self.status = NATIVE_STATUS
        self.backtrace = NATIVE_BACKTRACE

    def request(self, command):
        if command == "permissions": return "permissions full\n"
        if command == "bt 1":
            self.commands.append(command)
            return self.backtrace
        return super().request(command)


class DapAdapterTests(unittest.TestCase):
    def setUp(self):
        self.output = io.BytesIO()
        self.adapter = dap.Adapter(self.output)
        self.adapter.configuration_complete = True  # Unit cases model configured sessions.
        self.seq = 0

    def send(self, command, **arguments):
        self.seq += 1
        self.adapter.handle({"seq": self.seq, "type": "request", "command": command,
                             "arguments": arguments})
        result = messages(self.output.getvalue())
        self.output.seek(0)
        self.output.truncate()
        return result

    def test_entry_presentation_has_no_guest_instruction_or_value_authority(self):
        entry_id = (1 << 31) - 1
        for status in ("prepared; no Wasm instruction executed\nstop-id 17\n",
                       "stopped: pause\nstop-id 18\n"):
            with self.subTest(status=status):
                self.setUp()
                fake = FakeBroker(); fake.status = status
                self.adapter.broker = fake
                self.adapter.observe(status)
                stopped = next(row for row in messages(self.output.getvalue()) if row.get("event") == "stopped")
                prepared = status.startswith("prepared")
                if prepared: self.assertEqual(stopped["body"]["threadId"], entry_id)
                else: self.assertNotIn("threadId", stopped["body"])
                self.assertEqual(self.adapter.threads, [])  # UI entry is not a native participant.
                self.output.seek(0); self.output.truncate()
                threads = self.send("threads")[0]["body"]["threads"]
                self.assertEqual(threads, [{"id":entry_id, "name":"Initial Wasm entry — Continue to start"}] if prepared else [])
                trace = self.send("stackTrace", threadId=entry_id)[0]
                if prepared:
                    frame = trace["body"]["stackFrames"][0]
                    self.assertEqual(frame["presentationHint"], "label")
                    self.assertNotIn("instructionPointerReference", frame)
                    self.assertFalse(frame["canRestart"])
                else: self.assertFalse(trace["success"])
                for operation in ("stepIn", "next", "stepOut"):
                    begin = len(fake.commands)
                    reply = self.send(operation, threadId=entry_id)[0]
                    self.assertFalse(reply["success"], reply)
                    self.assertTrue(all(c == "status" for c in fake.commands[begin:]))
                for arguments in ({}, {"threadId":False}, {"threadId":1}, {"threadId":0,"singleThread":True}):
                    begin = len(fake.commands)
                    self.assertFalse(self.send("continue", **arguments)[0]["success"])
                    self.assertTrue(all(c == "status" for c in fake.commands[begin:]))
                resumed = self.send("continue", threadId=entry_id if prepared else 0)
                self.assertTrue(resumed[0]["success"], resumed)
                self.assertEqual(fake.commands[-1], "continue")
                self.assertIsNone(self.adapter.stop_key)
                self.assertFalse(self.adapter.frames)

    def test_steps_reject_missing_or_stale_participants_before_dispatch(self):
        for level in ("source", "wasm", "native"):
            for operation in ("stepIn", "next", "stepOut"):
                for status, thread in (("running\n", 1),
                        ("stopped: pause\nstop-id 51\n", 1),
                        ("stopped: pause\nstop-id 52\nthread 1 module=0 function=1 byte-offset=4 generation=1\n", 2)):
                    with self.subTest(level=level, operation=operation, status=status):
                        self.setUp()
                        fake = FakeBroker(); fake.status = status
                        self.adapter.broker = fake
                        self.adapter.step_level = level
                        self.assertFalse(self.send(operation, threadId=thread)[0]["success"])
                        self.assertTrue(all(c == "status" for c in fake.commands))

    def test_explicit_resume_uses_an_actual_participant(self):
        fake = FakeBroker()
        fake.status = "stopped: pause\nstop-id 19\nthread 7 module=0 function=1 byte-offset=4 generation=1\n"
        self.adapter.broker = fake
        self.adapter.observe(fake.status)
        self.output.seek(0); self.output.truncate()
        self.assertEqual(self.send("threads")[0]["body"]["threads"],
                         [{"id": 7, "name": "Wasm thread 7"}])
        before = list(fake.commands)
        self.assertFalse(self.send("continue", threadId=99)[0]["success"])
        self.assertTrue(all(c == "status" for c in fake.commands[len(before):]))
        resumed = self.send("continue", threadId=7)
        self.assertTrue(resumed[0]["success"], resumed)
        continued = next(row for row in resumed if row.get("event") == "continued")
        self.assertEqual(continued["body"]["threadId"], 7)

    def test_bounded_dap_framing(self):
        body = b'{"seq":1,"type":"request","command":"initialize"}'
        raw = (b"Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n"
               + f"Content-Length: {len(body)}\r\n\r\n".encode() + body)
        self.assertEqual(dap.read_dap(io.BytesIO(raw))["command"], "initialize")
        self.assertIsNone(dap.read_dap(io.BytesIO()))
        for bad in (b"Content-Length: 1048577\r\n\r\n{}",
                    b"Content-Length: 2\r\nContent-Length: 2\r\n\r\n{}",
                    b"Content-Length: 2\r\n\r\n{"):
            with self.subTest(bad=bad), self.assertRaises((ValueError, EOFError)):
                dap.read_dap(io.BytesIO(bad))
        invalid = b'{"seq":1,"type":"request","command":"pause","arguments":[]}'
        with self.assertRaises(ValueError):
            dap.read_dap(io.BytesIO(f"Content-Length: {len(invalid)}\r\n\r\n".encode() + invalid))

    @unittest.skipIf(sys.platform == "win32", "SCM_RIGHTS is a Unix socket feature")
    def test_broker_rejects_and_closes_received_descriptor(self):
        sender, receiver = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)
        try:
            read_fd, write_fd = os.pipe()
            try:
                # Observe the actual kernel-issued receiving descriptor.  A
                # /dev/fd or procfs mount is not required to check its lifetime.
                received = []

                class ReceivingSocket:
                    def recvmsg(self, *args):
                        result = receiver.recvmsg(*args)
                        for level, kind, payload in result[1]:
                            if level == socket.SOL_SOCKET and kind == socket.SCM_RIGHTS:
                                descriptors = array.array("i")
                                descriptors.frombytes(payload[:len(payload) - len(payload) % descriptors.itemsize])
                                received.extend(descriptors)
                        return result

                broker = object.__new__(dap.UnixBroker)
                broker.channel = ReceivingSocket()
                rights = array.array("i", [read_fd])
                sender.sendmsg([b"ready\n"], [(socket.SOL_SOCKET, socket.SCM_RIGHTS, rights)])
                with self.assertRaises(ConnectionError):
                    broker._recv_checked(64)
                self.assertEqual(len(received), 1)
                self.assertNotEqual(received[0], read_fd)
                with self.assertRaises(OSError) as closed:
                    os.fstat(received[0])
                self.assertEqual(closed.exception.errno, errno.EBADF)
                os.fstat(read_fd)
            finally:
                os.close(read_fd)
                os.close(write_fd)
        finally:
            sender.close()
            receiver.close()

    def test_debug_authority_cannot_rebuild_modules(self):
        fake = FakeBroker()
        self.adapter.broker = fake
        for level in ("full", "read-only", "limited"):
            self.adapter.debug_permissions = level
            for expression in ("rebuild /private/image.wasm", "rebuild", "rebuild-module 0 /private/image.wasm"):
                with self.subTest(level=level, expression=expression):
                    before = list(fake.commands)
                    reply = self.send("evaluate", expression=expression, context="repl")[0]
                    self.assertFalse(reply["success"])
                    self.assertIn("independent signed update coordinator", reply["message"])
                    self.assertTrue(all(c == "status" for c in fake.commands[len(before):]))

    def test_four_permission_capabilities_and_mutations(self):
        mutation = ("pause", "continue", "next", "stepIn", "stepOut", "setVariable", "writeMemory", "setBreakpoints", "setInstructionBreakpoints", "uwvm/wasip1Edit")
        for level in dap.DEBUG_PERMISSIONS:
            caps = dap.debug_capabilities(level)
            self.assertEqual(caps["supportsWriteMemoryRequest"], level == "full")
            self.assertEqual(caps["supportsReadMemoryRequest"], level in ("full", "read-only"))
            self.assertEqual(caps["supportsSteppingGranularity"], level == "full")
            for command in mutation:
                self.assertEqual(dap.dap_permission_allowed(level, command), level == "full")
            self.assertEqual(dap.dap_permission_allowed(level, "uwvm/dump"), level != "none")
        for level in ("read-only", "limited"):
            self.adapter.debug_permissions = level
            for command in mutation:
                response = self.send(command)[0]
                self.assertFalse(response["success"])
                self.assertIn("permission", response["message"])
        self.assertFalse(self.send("attach", debugPermissions="none")[0]["success"])

    def test_observers_cannot_control_execution_through_repl_or_disconnect(self):
        fake = FakeBroker()
        self.adapter.broker = fake
        for level in ("read-only", "limited"):
            self.adapter.debug_permissions = level
            for expression in ("pause", "continue", "c", "step 1", "run", "startw", "kill",
                               "wasm-script status", "trace host on", "trace host off", "trace host clear"):
                with self.subTest(level=level, expression=expression):
                    before = list(fake.commands)
                    reply = self.send("evaluate", expression=expression, context="repl")[0]
                    self.assertFalse(reply["success"])
                    self.assertIn("permission", reply["message"])
                    self.assertTrue(all(c == "status" for c in fake.commands[len(before):]))
            for key in ("terminateDebuggee", "suspendDebuggee", "restart"):
                before = list(fake.commands)
                reply = self.send("disconnect", **{key: True})[0]
                self.assertFalse(reply["success"])
                self.assertTrue(all(c == "status" for c in fake.commands[len(before):]))
                self.assertFalse(fake.closed)

    def test_attach_and_three_step_levels(self):
        fake = FakeBroker()
        self.adapter.configuration_complete = False
        self.assertFalse(self.send("initialize")[0]["body"]["supportsSteppingGranularity"])
        with mock.patch.object(dap, "UnixBroker", return_value=fake):
            attached = self.send("attach", socketDir="/private/host-owned", moduleId=0)
        self.assertTrue(attached[0]["success"], attached)
        self.assertFalse(any(item.get("event") == "stopped" for item in attached))
        configured = self.send("configurationDone")
        self.assertEqual(next(x for x in configured if x.get("event")=="stopped")["body"]["reason"], "pause")
        self.assertNotIn("capability", self.output.getvalue().decode())
        for granularity, request, expected in (
            ("statement", "stepIn", "step source 1 into"),
            ("line", "stepIn", "step source 1 into"),
            ("line", "next", "step source 1 over"),
            ("line", "stepOut", "step source 1 out"),
            ("instruction", "stepIn", "step asm 1"),
        ):
            self.assertTrue(self.send(request, threadId=1, granularity=granularity)[0]["success"])
            self.assertEqual(fake.commands[-1], expected)
        unsupported = self.send("next", threadId=1, granularity="instruction")
        self.assertFalse(unsupported[0]["success"])
        self.assertEqual(fake.commands[-1], "ni 1")
        self.assertIn("authenticated native trap", unsupported[0]["message"])
        trace = self.send("stackTrace", threadId=1)[0]["body"]["stackFrames"]
        self.assertEqual((trace[0]["source"]["path"], trace[0]["line"]), ("src/main.c", 4))
        self.assertNotIn("instructionPointerReference", trace[0])
        scopes = self.send("scopes", frameId=trace[0]["id"])[0]["body"]["scopes"]
        self.assertEqual(self.send("variables", variablesReference=scopes[0]["variablesReference"])[0]
                         ["body"]["variables"][0]["value"], "42")
        self.assertTrue(self.send("disconnect")[0]["success"])
        self.assertTrue(fake.closed)

    def test_breakpoint_replacement_and_host_grammar(self):
        fake = FakeBroker()
        self.adapter.broker = fake
        self.adapter.observe(fake.status, notify=False)
        result = self.send("setBreakpoints", source={"path": "src/main.c"},
                           breakpoints=[{"line": 4}])[0]["body"]["breakpoints"]
        self.assertEqual(result, [{"id": 7, "verified": True, "line": 4}])
        self.assertEqual([c for c in fake.commands if c != "info breakpoints"], ["breaksource 0 src/main.c:4"])
        self.send("setBreakpoints", source={"path": "src/main.c"}, breakpoints=[])
        self.assertIn("delete 7", fake.commands)
        rejected = self.send("setBreakpoints", source={"path": "src/evil\nstatus"},
                             breakpoints=[{"line": 4}])
        self.assertFalse(rejected[0]["success"])
        self.assertIn("delete 7", fake.commands)
        points = self.send("setInstructionBreakpoints", breakpoints=[
            {"instructionReference": "wasm:0:1:3", "offset": 1},
            {"instructionReference": "native:0xdeadbeef"},
            {"instructionReference": "wasm:0:1:4", "offset": -(1 << 80)},
        ])[0]["body"]["breakpoints"]
        self.assertEqual([item["verified"] for item in points], [True, False, False])
        self.assertIn("breakwasm 0 1 4", fake.commands)
        self.send("setInstructionBreakpoints", breakpoints=[])
        self.assertIn("delete 8", fake.commands)

    def test_bounded_wasm_memory_read(self):
        fake = FakeBroker()
        self.adapter.broker = fake
        fake.status = "stopped: pause\nstop-id 41\nthread 1 module=0 function=1 byte-offset=4 generation=1\n"
        reply = self.send("readMemory", memoryReference="wasm-memory:0:0:15",
                          offset=1, count=4)[0]
        self.assertTrue(reply["success"])
        self.assertEqual(base64.b64decode(reply["body"]["data"]), b"\0\1\x7f\xff")
        self.assertEqual(fake.commands[-3:], ["status", "memory 0 0 16 4", "status"])
        before = list(fake.commands)
        for reference, offset, count in (
            ("wasm-memory:0:0:15", 0, 65537),
            ("wasm-memory:0:0:18446744073709551615", 0, 4),
            ("wasm-memory:0:0:0", -1, 4),
            ("native:0x1234", 0, 4),
        ):
            self.assertFalse(self.send("readMemory", memoryReference=reference,
                                       offset=offset, count=count)[0]["success"])
            self.assertTrue(all(c == "status" for c in fake.commands[len(before):]))

    def test_malformed_ids_do_not_change_debugger_state(self):
        fake = FakeBroker()
        self.adapter.broker = fake
        for invalid in (None, True, 1.5, "1", [], {}, 0, -1, 1 << 64):
            for request, arguments in (
                ("continue", {"threadId": invalid}),
                ("stepIn", {"threadId": invalid}),
                ("stackTrace", {"threadId": invalid}),
                ("scopes", {"frameId": invalid}),
                ("variables", {"variablesReference": invalid}),
                ("source", {"sourceReference": invalid}),
            ):
                with self.subTest(request=request, invalid=invalid):
                    self.assertFalse(self.send(request, **arguments)[0]["success"])
                    self.assertTrue(all(c == "status" for c in fake.commands))
        for line in (True, 0, -1, 1 << 64):
            with self.subTest(source_line=line):
                reply = self.send("setBreakpoints", source={"path": "main.c"}, breakpoints=[{"line": line}])[0]
                self.assertFalse(reply["success"])
                self.assertTrue(all(c == "status" for c in fake.commands))

    def test_frame_ids_do_not_alias_after_a_new_stop(self):
        self.adapter.broker = FakeBroker()
        self.adapter.observe(self.adapter.broker.status)
        self.output.seek(0)
        self.output.truncate()
        old_frame = self.send("stackTrace", threadId=1)[0]["body"]["stackFrames"][0]["id"]
        old_scope = self.send("scopes", frameId=old_frame)[0]["body"]["scopes"][0]["variablesReference"]
        self.adapter.broker.status = "stopped: selected participant step\nstop-id 42\nthread 1 module=0 function=1 byte-offset=4 generation=1\n"
        self.adapter.observe(self.adapter.broker.status)
        self.output.seek(0)
        self.output.truncate()
        new_frame = self.send("stackTrace", threadId=1)[0]["body"]["stackFrames"][0]["id"]
        self.assertGreater(new_frame, old_scope)
        self.assertFalse(self.send("scopes", frameId=old_frame)[0]["success"])
        self.assertFalse(self.send("variables", variablesReference=old_scope)[0]["success"])

    def test_attach_validation_and_failed_status_close_channel(self):
        fake = FakeBroker()
        with mock.patch.object(dap, "UnixBroker", return_value=fake) as factory:
            rejected = self.send("attach", socketDir="/private/broker", moduleId=-1)
            self.assertFalse(rejected[0]["success"])
            factory.assert_not_called()
        fake.request = lambda command: "error: guest debug domain closed\n"
        with mock.patch.object(dap, "UnixBroker", return_value=fake):
            rejected = self.send("attach", socketDir="/private/broker")
        self.assertFalse(rejected[0]["success"])
        self.assertTrue(fake.closed)
        self.assertIsNone(self.adapter.broker)

    def native_frame(self, broker=None):
        self.adapter.broker = broker or NativeBroker()
        response = self.send("stackTrace", threadId=1)[0]
        self.assertTrue(response["success"], response)
        self.assertEqual(response["body"]["totalFrames"], 1)
        return response["body"]["stackFrames"][0]

    def test_native_stop_is_selectable_without_fabricating_source(self):
        frame = self.native_frame()
        self.assertEqual(self.adapter.broker.commands[:2], ["status", "bt 1"])
        self.assertTrue(all(c == "status" or c.startswith("wasm-meta ") for c in self.adapter.broker.commands[2:]))
        self.assertEqual((frame["line"], frame["column"], frame["presentationHint"]), (1, 1, "normal"))
        self.assertIn("asm", frame["name"])
        self.assertEqual(self.adapter.frames[frame["id"]][2:], (1, 1))
        self.assertNotIn("byte-offset", frame["name"])
        self.assertEqual(frame["source"]["origin"], "Current owned LLVM JIT instruction")
        self.assertNotIn("path", frame["source"])
        self.assertFalse(frame["canRestart"])
        self.assertEqual(frame["instructionPointerReference"], f"uwvm-native-stop:{frame['id']}")
        self.assertEqual([s["name"] for s in self.send("scopes", frameId=frame["id"])[0]["body"]["scopes"]], ["Registers", "Frame details"])
        self.assertFalse(self.send("source", sourceReference=frame["id"])[0]["success"])

    def test_native_display_reference_grants_no_address_authority(self):
        frame = self.native_frame()
        reference = frame["instructionPointerReference"]
        commands = list(self.adapter.broker.commands)
        for command, arguments in (
            ("disassemble", {"memoryReference": reference, "instructionCount": 1}),
            ("readMemory", {"memoryReference": reference, "count": 1}),
            ("writeMemory", {"memoryReference": reference, "data": "AA=="}),
            ("restartFrame", {"frameId": frame["id"]}),
        ):
            with self.subTest(command=command):
                self.assertFalse(self.send(command, **arguments)[0]["success"])
                self.assertTrue(all(c == "status" or c == "info breakpoints" or c.startswith("disassemble-range ") for c in self.adapter.broker.commands[len(commands):]))
        points = self.send("setInstructionBreakpoints", breakpoints=[
            {"instructionReference": reference},
            {"instructionReference": "0x000000010c75842c"},
        ])[0]["body"]["breakpoints"]
        self.assertEqual([point["verified"] for point in points], [False, False])
        self.assertTrue(all(c == "status" or c == "info breakpoints" or c.startswith("disassemble-range ") for c in self.adapter.broker.commands[len(commands):]))
        self.assertFalse(self.send("initialize")[0]["body"].get("supportsDisassembleRequest", False))

    def test_native_status_rejects_malformed_or_unbound_pc(self):
        malformed = [
            NATIVE_STATUS.replace("0x000000010c75842c", value)
            for value in ("0x0", "0x", "0xxyz", "-1", "0x10000000000000000", "0x1234 extra")
        ]
        malformed += [
            "  native-pc=0x1234\n" + NATIVE_STATUS,
            NATIVE_STATUS + "  native-pc=0x1234\n",
            NATIVE_STATUS.replace("stopped: native instruction step", "stopped: pause"),
            NATIVE_STATUS.replace("generation=1", "generation=0"),
            NATIVE_STATUS.replace("thread 1 ", "thread 0 "),
            NATIVE_STATUS + "  source old.c:12:3\n",
            NATIVE_STATUS.replace("  native-pc=", "  source old.c:12:3\n  native-pc="),
            NATIVE_STATUS + "running\n",
            NATIVE_STATUS + "thread 1 module=1 function=1 byte-offset=20 generation=1\n",
        ]
        for status in malformed:
            with self.subTest(status=status):
                with self.assertRaises(ValueError):
                    dap.parse_status(status)

    def test_native_backtrace_must_match_complete_selected_thread(self):
        malformed = [
            NATIVE_BACKTRACE.replace("thread 1 ", "thread 2 "),
            NATIVE_BACKTRACE.replace("module=1", "module=2"),
            NATIVE_BACKTRACE.replace("generation=1", "generation=2"),
            NATIVE_BACKTRACE.replace("0x000000010c75842c", "0x000000010c758430"),
            NATIVE_BACKTRACE + "  #0 module=1 function=1 [old] old\n",
        ]
        for trace in malformed:
            with self.subTest(trace=trace):
                broker = NativeBroker()
                broker.backtrace = trace
                self.adapter.broker = broker
                response = self.send("stackTrace", threadId=1)[0]
                self.assertFalse(response["success"])
                self.assertEqual((self.adapter.frames, self.adapter.scopes, self.adapter.native_frames), ({}, {}, set()))

    def test_filtered_native_backtrace_does_not_replace_other_stopped_threads(self):
        broker = NativeBroker()
        broker.status += "thread 2 module=2 function=3 byte-offset=7 generation=4\n"
        frame = self.native_frame(broker)
        self.assertEqual([thread["id"] for thread in self.adapter.threads], [1, 2])
        self.assertEqual([s["name"] for s in self.send("scopes", frameId=frame["id"])[0]["body"]["scopes"]], ["Registers", "Frame details"])
        response = self.send("stackTrace", threadId=3)[0]
        self.assertFalse(response["success"])
        self.assertFalse(any(c.startswith(("step ", "continue", "frames ")) for c in broker.commands))

    def test_native_frame_is_retired_on_the_next_pc_or_generation(self):
        for old, new in (("0x000000010c75842c", "0x000000010c758430"), ("generation=1", "generation=2")):
            with self.subTest(new=new):
                broker = NativeBroker()
                first = self.native_frame(broker)
                broker.status = broker.status.replace(old, new)
                broker.backtrace = broker.status + "  backtrace unavailable for this stop\n"
                second = self.send("stackTrace", threadId=1)[0]["body"]["stackFrames"][0]
                self.assertGreater(second["id"], first["id"])
                self.assertNotEqual(second["instructionPointerReference"], first["instructionPointerReference"])
                self.assertFalse(self.send("scopes", frameId=first["id"])[0]["success"])

    def test_running_closed_exited_and_disconnect_retire_native_references(self):
        for status in ("running\n", "debug domain closed\n", "guest exited: 0\n"):
            with self.subTest(status=status):
                frame = self.native_frame()
                self.adapter.observe(status)
                self.output.seek(0)
                self.output.truncate()
                self.assertFalse(self.send("scopes", frameId=frame["id"])[0]["success"])
        frame = self.native_frame()
        self.assertTrue(self.send("disconnect")[0]["success"])
        self.assertFalse(self.send("scopes", frameId=frame["id"])[0]["success"])

    def test_resume_aliases_and_failed_steps_retire_references_before_io(self):
        commands = ("continue", " c ", "step wasm 1", "step asm 1", "step source 1 into", "s 1", "step 1", "quit", "q")
        for command in commands:
            with self.subTest(command=command):
                broker = NativeBroker()
                frame = self.native_frame(broker)
                def failure(text):
                    self.assertEqual(text, command)
                    self.assertNotIn(frame["id"], self.adapter.frames)
                    self.assertEqual(self.adapter.native_frames, set())
                    raise ConnectionError("failed after execution may have resumed")
                broker.request = failure
                self.assertFalse(self.send("evaluate", expression=command)[0]["success"])
                self.assertEqual(self.adapter.frames, {})
        for request, arguments in (("continue", {"threadId": 1}),
                                   ("stepIn", {"threadId": 1, "granularity": "instruction"}),
                                   ("next", {"threadId": 1, "granularity": "line"}),
                                   ("stepOut", {"threadId": 1, "granularity": "line"})):
            with self.subTest(request=request):
                broker = NativeBroker()
                frame = self.native_frame(broker)
                def error(text):
                    if text == "status": return broker.status
                    self.assertNotIn(frame["id"], self.adapter.frames)
                    return "error: selected step failed after its pending pause was cancelled\n"
                broker.request = error
                self.assertFalse(self.send(request, **arguments)[0]["success"])

    def test_successful_resume_retires_wasm_and_native_references(self):
        for native in (False, True):
            with self.subTest(native=native):
                broker = NativeBroker() if native else FakeBroker()
                self.adapter.broker = broker
                frame = self.send("stackTrace", threadId=1)[0]["body"]["stackFrames"][0]
                scope = self.send("scopes", frameId=frame["id"])[0]["body"]["scopes"]
                response = self.send("continue", threadId=1)[0]
                self.assertTrue(response["success"])
                self.assertNotIn(frame["id"], self.adapter.frames)
                self.assertEqual(self.adapter.native_frames, set())
                if scope:
                    self.assertNotIn(scope[0]["variablesReference"], self.adapter.scopes)

    def test_failed_read_only_command_or_invalid_pc_retires_native_references(self):
        broker = NativeBroker()
        frame = self.native_frame(broker)
        broker.request = lambda text: "error: debug domain closed\n"
        self.assertFalse(self.send("evaluate", expression="status")[0]["success"])
        self.assertFalse(self.send("scopes", frameId=frame["id"])[0]["success"])
        frame = self.native_frame()
        with self.assertRaises(ValueError):
            self.adapter.observe(NATIVE_STATUS.replace("0x000000010c75842c", "0x0"))
        self.assertNotIn(frame["id"], self.adapter.frames)

    def test_native_reference_exhaustion_never_wraps(self):
        self.adapter.next_reference = (1 << 31) - 1
        frame = self.native_frame()
        self.assertEqual(frame["id"], (1 << 31) - 1)
        self.assertTrue(self.send("stackTrace", threadId=1)[0]["success"])  # Same-stop display reuses its ID.
        self.adapter.broker.status = self.adapter.broker.status.replace("stop-id 41", "stop-id 42")
        self.adapter.broker.backtrace = self.adapter.broker.status + "  backtrace unavailable for this stop\n"
        self.assertFalse(self.send("stackTrace", threadId=1)[0]["success"])
        self.assertEqual(self.adapter.next_reference, 1 << 31)


if __name__ == "__main__":
    unittest.main()
