"""IDE pages use the actual recorder parser, including stale/binary/large data."""
import io
import sys
import unittest
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import host_trace_view as view
import dap_adapter as dap
from test_dap_host_trace_export import Journal
from test_dap_wasm_state import Broker


class Views(unittest.TestCase):
    def test_calls_all_families_and_lossless_argument_bits(self):
        journal = Journal()
        rows, after = [], "0"
        while True:
            page = view.query(journal.request, journal.checkpoint, {"after": after})
            rows += page["calls"];after = page["next"]
            if not page["more"]:break
        self.assertEqual(len(rows), 19)
        self.assertEqual({r["family"] for r in rows}, {"wasip1","ros1","ros2"})
        self.assertEqual(rows[0]["args"][1]["unsigned"], "18446744073709551615")
        self.assertEqual(rows[0]["duration-ns"], "9007199254740993")
        self.assertTrue(all("trace host read" in c for c in journal.commands))
        for family in ("wasip1","ros1","ros2"):
            page=view.query(journal.request,journal.checkpoint,{"family":family})
            self.assertTrue(all(r["family"] == family for r in page["calls"]))

    def test_large_buffers_have_no_total_capacity_limit(self):
        data=bytes(range(256))*300
        journal=Journal(data=data)
        result=bytearray();offset="0"
        while True:
            page=view.query(journal.request,journal.checkpoint,
                {"call":"1","segment":"0","offset":offset},byte_page=True)
            result+=bytes.fromhex(page["hex"]);offset=page["next"]
            if not page["more"]:break
        self.assertEqual(result,data)
        self.assertEqual(offset,"76800")
        self.assertIsNone(page["utf8IfValid"])

    def test_zero_length_and_full_module_name(self):
        j=Journal(label=b'A"*'*30000,data=b"")
        self.assertEqual(view.query(j.request,j.checkpoint,{"call":1},byte_page=True)["hex"],"")
        p=view.query(j.request,j.checkpoint,{"call":"1","segment":str(view.U64),"offset":"65536"},byte_page=True)
        self.assertEqual(bytes.fromhex(p["hex"]),j.label[65536:69632])

    def test_changed_stop_and_malformed_provider_are_rejected(self):
        for mutate in (lambda s:s.replace("host-calls next=8","host-calls next=9"),
                       lambda s:s.replace("family=1","family=3"),
                       lambda s:s.replace("result=18446744073709551615","result=18446744073709551616")):
            j=Journal();j.mutate=lambda c,r:mutate(r)
            with self.assertRaises(ValueError):view.query(j.request,j.checkpoint,{})
        j=Journal();calls=iter([(1,7),(1,8)])
        with self.assertRaises(ValueError):view.query(j.request,lambda:next(calls),{})

    def test_invalid_arguments_never_query_the_broker(self):
        for args in ({"count":0},{"count":9},{"after":-1},{"after":"01"},{"after":True},
                     {"after":str(1<<64)},{"family":"ros"},{"hostAddress":"0x1"}):
            j=Journal()
            with self.assertRaises(ValueError):view.query(j.request,j.checkpoint,args)
            self.assertEqual(j.commands,[])
        for args in ({"call":0},{"call":1,"count":4097},{"call":1,"offset":-1},{"call":1,"segment":False}):
            j=Journal()
            with self.assertRaises(ValueError):view.query(j.request,j.checkpoint,args,byte_page=True)
            self.assertEqual(j.commands,[])

    def test_wasm_listing_fresh_pages_and_refusal_of_host_or_stale_reference(self):
        a=dap.Adapter(io.BytesIO());a.broker=Broker();a.observe(a.broker.status(),notify=False)
        a.frames[1]=(1,0,0,1)
        rows=a.wasm_disassembly("wasm:0:1:4",64)
        self.assertEqual(rows[0],{"address":"4","instruction":"end","symbol":"wasm:0:1:4"})
        self.assertEqual(len(rows),64)
        self.assertTrue(all(r["presentationHint"]=="invalid" for r in rows[1:]))
        for ref in ("0x1000","uwvm-native-code:1","wasm:0:2:4","wasm:0:1:0"):
            with self.assertRaises(ValueError):a.wasm_disassembly(ref,1)
        a.invalidate_stop()
        with self.assertRaises(ValueError):a.wasm_disassembly("wasm:0:1:4",1)


if __name__ == "__main__":unittest.main()
