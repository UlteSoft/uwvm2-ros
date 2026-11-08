#!/usr/bin/env python3
"""IDE-specific document identities reuse genuine stop-checked DAP operations."""
import io
import unittest
from urllib.parse import quote
import test_dap_wasm_state as typed
import test_dap_frame_scope_lifetime as lifetime

dap = typed.dap
SESSION = "11223344-5566-7788-99aa-bbccddeeff00"

class Broker(typed.Broker):
    def request(self, command):
        if command == "breakwasm 0 1 4":
            self.commands.append(command)
            return "breakpoint 1 registered at executable Wasm expression byte offset\n"
        return super().request(command)

class DocumentURI(unittest.TestCase):
    send = lifetime.Lifetime.send

    def setUp(self):
        self.output = io.BytesIO()
        self.adapter = dap.Adapter(self.output, SESSION)
        self.adapter.step_level = "wasm"
        self.broker = Broker()
        self.adapter.broker = self.broker
        self.sequence = 0

    def document(self):
        reply = self.send("stackTrace", threadId=1)
        self.assertTrue(reply["success"], reply)
        frame = reply["body"]["stackFrames"][0]
        uri = frame["source"]["path"]
        self.assertEqual(frame["source"]["sourceReference"], 0)
        self.assertTrue(uri.startswith("uwvm-wasm:/module-0/function-1/stop-41.wat?session="+SESSION))
        return frame["id"], uri

    def test_source_uses_real_stop_checked_disassembly(self):
        ref, uri = self.document()
        reply = self.send("source", sourceReference=ref, source={"path": uri})
        self.assertTrue(reply["success"], reply)
        self.assertIn(";; 4  end", reply["body"]["content"])
        self.assertTrue(any(c.startswith("wasm-code ") for c in self.broker.commands))
        self.assertEqual(self.adapter.wasm_document_reference(quote(uri, safe=":/?")), ref)

    def test_foreign_session_function_stop_and_reference_cannot_copy(self):
        ref, uri = self.document()
        for bad in [uri.replace(SESSION, "ffffffff-ffff-ffff-ffff-ffffffffffff"),
                    uri.replace("function-1", "function-2"), uri.replace("stop-41", "stop-42"),
                    uri+"&ref=2", uri.replace("/module-0/", "/module-0/../")]:
            begin = len(self.broker.commands)
            reply = self.send("source", sourceReference=ref, source={"path": bad})
            self.assertFalse(reply["success"], reply)
            self.assertFalse(any(c.startswith("wasm-code ") for c in self.broker.commands[begin:]))
        reply = self.send("source", sourceReference=ref+1, source={"path": uri})
        self.assertFalse(reply["success"], reply)

    def test_source_and_gutter_retire_on_external_stop_change(self):
        ref, uri = self.document()
        self.broker.stop += 1
        self.assertFalse(self.send("source", sourceReference=ref, source={"path": uri})["success"])
        self.assertFalse(self.adapter.wasm_source_uris)
        begin = len(self.broker.commands)
        self.assertFalse(self.send("setBreakpoints", source={"path": uri}, breakpoints=[{"line": 2}])["success"])
        self.assertFalse(any(c.startswith("breakwasm ") for c in self.broker.commands[begin:]))

    def test_gutter_uses_only_loaded_current_wasm_document(self):
        ref, uri = self.document()
        self.assertFalse(self.send("setBreakpoints", source={"path": uri}, breakpoints=[{"line": 2}])["success"])
        ref, uri = self.document()
        self.assertTrue(self.send("source", sourceReference=ref, source={"path": uri})["success"])
        reply = self.send("setBreakpoints", source={"path": quote(uri, safe=":/?")}, breakpoints=[{"line": 2}])
        self.assertTrue(reply["success"], reply)
        self.assertTrue(reply["body"]["breakpoints"][0]["verified"], reply)
        self.assertIn("breakwasm 0 1 4", self.broker.commands)

    def test_standard_dap_clients_keep_standard_source_references(self):
        self.adapter.wasm_source_session = None
        frame = self.send("stackTrace", threadId=1)["body"]["stackFrames"][0]
        self.assertEqual(frame["source"]["sourceReference"], frame["id"])
        self.assertTrue(frame["source"]["path"].startswith("/uwvm-wasm/"))
        self.assertFalse(self.adapter.wasm_source_uris)

    def test_bad_host_session_identity(self):
        for value in ["", True, "file:///host", SESSION+"\n"]:
            with self.assertRaises(ValueError):
                dap.Adapter(io.BytesIO(), value)

if __name__ == "__main__":
    unittest.main()
