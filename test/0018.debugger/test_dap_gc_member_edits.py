"""GC setVariable protocol tests; the fake broker has no native object authority."""
import io
import unittest
import test_dap_wasm_state as state

dap = state.dap

class Broker:
    def __init__(self):
        self.stop, self.kind, self.value, self.mutable = 41, "i32", "17", True
        self.commands, self.handles = [], {}
        self.handle = 0
        self.lost_commit = self.retire_before_commit = False
        self.ack = lambda text: text
        self.path = ()
    def status(self):
        return f"stopped: breakpoint\nstop-id {self.stop}\nthread 1 module=0 function=1 byte-offset=4 generation=7\n"
    def page(self, path, token=None):
        depth = len(path)
        spelling = f"@5:{token} depth={depth}" if token is not None else ",".join(map(str, path)) or "-"
        selected = 2 if depth else 1
        root = ("object #1 struct module=0 type=3 members=1 first=0 next=0 more=yes\n"
                "  metadata only; expand through the original root and member path\n") if depth else ""
        return (f"wasm-stop {self.stop}\nWasm state thread=1 module=0 epoch=7 first=0 total=1\n"
                f"Wasm members object={selected} first=0 count=1 path={spelling}\n"
                "global 0 (ref null type-index=3 module=0) = struct #1\n"
                f"object #{selected} struct module=0 type=3 members=1 first=0 next=1 more=no\n"
                f"  0 {self.kind} = {self.value} {'mutable' if self.mutable else 'immutable'}\n" + root +
                ("Wasm state truncated: rows=1 objects=2\n" if depth else ""))
    def request(self, command):
        self.commands.append(command)
        w = command.split()
        if command == "status":
            return self.status()
        if command.startswith("members globals "):
            return self.page(tuple(map(int, w[9:])))
        if w[:2] in (["path", "create"], ["path", "extend"]):
            path = tuple(map(int, w[8:])) if w[1] == "create" else self.handles.pop(int(w[3]))[1] + tuple(map(int, w[4:]))
            self.handle += 1
            self.handles[self.handle] = (self.stop, path)
            return f"wasm-stop {self.stop}\nWasm path session=5 handle={self.handle} depth={len(path)} view=3 protocol=1\n"
        if w[:2] == ["path", "members"]:
            handle = int(w[3]); path = self.handles[handle][1]
            return (f"wasm-stop {self.stop}\nWasm path session=5 handle={handle} depth={len(path)} view=3 protocol=1\n" +
                    self.page(path, handle).split("\n", 1)[1])
        if w[:3] == ["set", "wasm", "member"]:
            assert w[:6] == ["set", "wasm", "member", "1", "handle", "5"]
            if self.retire_before_commit:
                self.stop += 1
            if self.handles[int(w[6])][0] != self.stop:
                return "error: stale mutation path\n"
            assert w[7:10] == ["at", "0", "bits"]
            self.kind = w[10]
            self.value = str(int(w[11], 16)) if self.kind.startswith("i") else (
                "bytes=" + int(w[11], 16).to_bytes(8, "little").hex() + int(w[12], 16).to_bytes(8, "little").hex()
                if self.kind == "v128" else "bits=0x" + w[11].zfill(8 if self.kind == "f32" else 16))
            if self.kind.startswith("i"):
                width = int(self.kind[1:])
                integer = int(self.value)
                if integer >= 1 << (width - 1):
                    self.value = str(integer - (1 << width))
            self.stop += 1
            if self.lost_commit:
                raise ConnectionError("lost acknowledgement")
            return self.ack(f"wasm-stop {self.stop}\nWasm mutation v=3 status=available applied=1 reason=none runtime=7 module=0 target=member index=3 element=0 bytes=0 address-bytes=0\n")
        raise AssertionError(command)

class GCMemberEdits(unittest.TestCase):
    send = state.StateProtocol.send
    def setUp(self):
        self.output = io.BytesIO()
        self.adapter = dap.Adapter(self.output)
        self.adapter.step_level = "wasm"
        self.adapter.broker = self.broker = Broker()
        self.adapter.observe(self.broker.status(), notify=False)
        self.sequence = 0
    def reference(self, path=()):
        ref = self.adapter.new_reference()
        self.adapter.wasm_object_views[ref] = (self.adapter.stop_key,
            {"kind": "struct", "label": "struct module=0 type=3", "total": 1},
            (1, 0, 0, "globals", 0, 0, path))
        return ref
    def write(self, value, path=()):
        return self.send("setVariable", variablesReference=self.reference(path), name="[0]", value=value)
    def writes(self):
        return [c for c in self.broker.commands if c.startswith("set wasm ")]
    def test_numeric_vector_edits_use_current_path_and_retire_old_refs(self):
        for kind, value, bits in (("i32","-1","ffffffff"), ("i64","bits fedcba9876543210","fedcba9876543210"),
                ("f32","bits 7fc12345","7fc12345"), ("f64","1.5","3ff8000000000000"),
                ("v128","bits 10000000000000002","2 1")):
            with self.subTest(kind=kind):
                self.setUp(); self.broker.kind = kind
                self.broker.value = "bytes=" + "00"*16 if kind == "v128" else "bits=0x" + "0"*(8 if kind == "f32" else 16) if kind.startswith("f") else "17"
                ref = self.reference()
                response = self.send("setVariable", variablesReference=ref, name="[0]", value=value)
                self.assertTrue(response["success"], response)
                self.assertEqual(self.writes(), [f"set wasm member 1 handle 5 1 at 0 bits {kind} {bits}"])
                self.assertNotIn(ref, self.adapter.wasm_object_views)
    def test_deep_cycle_path_uses_stop_bound_handle(self):
        response = self.write("99", (0,)*33)
        self.assertTrue(response["success"], response)
        self.assertEqual(len(self.writes()), 1)
        self.assertTrue(any(c.startswith("path extend ") for c in self.broker.commands))
    def test_immutable_member_rejected_without_commit(self):
        self.broker.mutable = False
        self.assertFalse(self.write("9")["success"])
        self.assertEqual(self.writes(), [])
    def test_retired_stop_and_invalid_width_rejected_without_commit(self):
        for value in ("4294967296", "bits 100000000", "call()", "1\ncontinue"):
            self.setUp()
            self.assertFalse(self.write(value)["success"])
            self.assertEqual(self.writes(), [])
        self.setUp()
        ref = self.reference(); self.broker.stop += 1
        self.assertFalse(self.send("setVariable", variablesReference=ref, name="[0]", value="9")["success"])
        self.assertEqual(self.writes(), [])
    def test_external_resume_before_commit_cannot_retarget_root(self):
        self.broker.retire_before_commit = True
        self.assertFalse(self.write("9")["success"])
        self.assertEqual(self.broker.value, "17")
        self.assertEqual(len(self.writes()), 1)
    def test_lost_or_invalid_commit_ack_never_retries(self):
        for transform in (lambda x: x+"extra\n", lambda x: x.replace("index=3", "index=4"),
                          lambda x: x.replace("module=0", "module=1"), lambda x: x.replace("wasm-stop 42", "wasm-stop 41")):
            self.setUp(); self.broker.ack = transform
            self.assertFalse(self.write("9")["success"])
            self.assertEqual(self.broker.value, "9")
            self.assertEqual(len(self.writes()), 1)
        self.setUp(); self.broker.lost_commit = True
        self.assertFalse(self.write("9")["success"])
        self.assertEqual(len(self.writes()), 1)
    def test_permissions_deny_before_broker_mutation(self):
        for permission in ("read-only", "limited", "none"):
            self.setUp(); self.adapter.debug_permissions = permission
            self.assertFalse(self.write("9")["success"])
            self.assertEqual(self.writes(), [])

if __name__ == "__main__":
    unittest.main()
