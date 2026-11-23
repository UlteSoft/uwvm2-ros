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
        if command == "info breakpoints":
            self.commands.append(command)
            return "no breakpoints\n"
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
        self.assertTrue(uri.startswith("uwvm-wasm:/module-0/function-1/code-7-1.uwvm-wat?session="+SESSION))
        return self.adapter.wasm_document_reference(uri), uri

    def test_source_uses_real_stop_checked_disassembly(self):
        ref, uri = self.document()
        reply = self.send("source", sourceReference=ref, source={"path": uri})
        self.assertTrue(reply["success"], reply)
        self.assertIn(";; 0x00000004  end", reply["body"]["content"])
        self.assertTrue(any(c.startswith("wasm-code ") for c in self.broker.commands))
        self.assertEqual(self.adapter.wasm_document_reference(quote(uri, safe=":/?")), ref)

    def test_foreign_session_function_stop_and_reference_cannot_copy(self):
        ref, uri = self.document()
        for bad in [uri.replace(SESSION, "ffffffff-ffff-ffff-ffff-ffffffffffff"),
                    uri.replace("function-1", "function-2"), uri.replace("code-7-1", "code-7-2"),
                    uri+"&ref=2", uri.replace("/module-0/", "/module-0/../")]:
            begin = len(self.broker.commands)
            reply = self.send("source", sourceReference=ref, source={"path": bad})
            self.assertFalse(reply["success"], reply)
            self.assertFalse(any(c.startswith("wasm-code ") for c in self.broker.commands[begin:]))
        reply = self.send("source", sourceReference=ref+1, source={"path": uri})
        self.assertFalse(reply["success"], reply)

    def test_immutable_source_survives_stop_but_gutter_authenticates_current_code(self):
        ref, uri = self.document()
        original = self.send("source", sourceReference=ref, source={"path": uri})["body"]["content"]
        self.broker.stop += 1
        begin = len(self.broker.commands)
        reply = self.send("source", sourceReference=ref, source={"path": uri})
        self.assertTrue(reply["success"], reply)
        self.assertEqual(reply["body"]["content"], original)
        self.assertEqual(len(self.broker.commands), begin)
        reply = self.send("setBreakpoints", source={"path": uri}, breakpoints=[{"line": 6}])
        self.assertTrue(reply["success"], reply)
        self.assertFalse(reply["body"]["breakpoints"][0]["verified"], reply)
        self.assertIn("retired", reply["body"]["breakpoints"][0]["message"])
        self.assertFalse(any(c.startswith("breakwasm ") for c in self.broker.commands[begin:]))
        # Re-observe the new stop before authorizing changes to the same code.
        self.document()
        reply = self.send("setBreakpoints", source={"path": uri}, breakpoints=[{"line": 6}])
        self.assertTrue(reply["body"]["breakpoints"][0]["verified"], reply)

    def test_gutter_uses_only_loaded_current_wasm_document(self):
        ref, uri = self.document()
        # stackTrace eagerly loads the immutable function document.
        self.assertTrue(self.send("source", sourceReference=ref, source={"path": uri})["success"])
        ref, uri = self.document()
        self.assertTrue(self.send("source", sourceReference=ref, source={"path": uri})["success"])
        reply = self.send("setBreakpoints", source={"path": quote(uri, safe=":/?")}, breakpoints=[{"line": 6}])
        self.assertTrue(reply["success"], reply)
        self.assertTrue(reply["body"]["breakpoints"][0]["verified"], reply)
        self.assertIn("breakwasm 0 1 4", self.broker.commands)

    def test_standard_dap_clients_keep_standard_source_references(self):
        self.adapter.wasm_source_session = None
        frame = self.send("stackTrace", threadId=1)["body"]["stackFrames"][0]
        self.assertGreater(frame["source"]["sourceReference"], 0)
        self.assertIn(frame["source"]["sourceReference"], self.adapter.wasm_documents)
        self.assertTrue(frame["source"]["path"].startswith("/uwvm-wasm/"))
        self.assertFalse(self.adapter.wasm_source_uris)

    def test_navigation_layout_and_cached_call_do_not_change_the_stop(self):
        ref, uri = self.document()
        before = self.adapter.stop_key
        self.adapter.wasm_metadata = lambda *args: {'rows': [{'imported': False}]}
        for command, arguments in [('uwvm/wasmDocumentLayout', {'reference': ref}),
                                   ('uwvm/wasmFunctionDocument', {'reference': ref, 'index': '1'})]:
            self.assertTrue(self.send(command, **arguments)['success'])
            self.assertEqual(self.adapter.stop_key, before)
        unknown = self.send('uwvm/wasmFunctionDocument', reference=ref, index='2')
        self.assertFalse(unknown['success'])
        self.assertIn('not been captured', unknown['message'])
        self.assertEqual(self.adapter.stop_key, before)
        self.adapter.wasm_metadata = lambda *args: {'rows': [{'imported': True}]}
        self.assertIn('host import', self.send('uwvm/wasmFunctionDocument', reference=ref, index='0')['message'])
        self.assertEqual(self.adapter.stop_key, before)

    def test_cached_navigation_preserves_permissions_and_generation_boundaries(self):
        ref, uri = self.document()
        self.adapter.wasm_metadata = lambda *args: {'rows': [{'imported': False}]}
        for permission, allowed in [('full', True), ('read-only', True), ('limited', False), ('none', False)]:
            self.adapter.debug_permissions = permission
            for command, extra in [('uwvm/wasmDocumentLayout', {}), ('uwvm/wasmFunctionDocument', {'index': '1'})]:
                self.assertEqual(self.send(command, reference=ref, **extra)['success'], allowed)
        self.adapter.debug_permissions = 'full'
        self.adapter.wasm_documents[ref]['epoch'] = 99
        self.assertIn('retired', self.send('uwvm/wasmFunctionDocument', reference=ref, index='1')['message'])

    def test_bad_host_session_identity(self):
        for value in ["", True, "file:///host", SESSION+"\n"]:
            with self.assertRaises(ValueError):
                dap.Adapter(io.BytesIO(), value)

if __name__ == "__main__":
    unittest.main()
