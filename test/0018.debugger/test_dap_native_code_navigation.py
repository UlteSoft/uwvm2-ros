"""Native code projections never authorize cooperative value access."""
import importlib.util
import io
from pathlib import Path
import unittest

spec = importlib.util.spec_from_file_location('native_navigation', Path(__file__).resolve().parents[2] / 'tools/debug/dap_adapter.py')
dap = importlib.util.module_from_spec(spec)
spec.loader.exec_module(dap)
STATUS = ('stopped: native inspection\nstop-id 17\n'
          'thread 3 module=0 function=2 byte-offset=4 generation=8\n'
          '  native-pc=0x1234\n'
          '  native-wasm provenance=exact module=0 function=2 byte-offset=6 function-generation=1 runtime-epoch=8\n'
          '  native-wasm-start=no\n')

class Tests(unittest.TestCase):
    def setUp(self):
        self.adapter = dap.Adapter(io.BytesIO())
        self.adapter.observe(STATUS, notify=False)

    def test_inspection_stop_and_independent_code_offset(self):
        self.assertEqual(self.adapter.state, 'stopped')
        row = self.adapter.threads[0]
        self.assertEqual(row['offset'], 4)
        self.assertEqual(self.adapter.wasm_code_location(row)['offset'], 6)
        self.assertFalse(row['native_wasm_start'])

    def test_unknown_or_foreign_native_mapping_cannot_reuse_anchor(self):
        row = dict(self.adapter.threads[0])
        row.pop('native_wasm')
        with self.assertRaises(dap.LocalRequestRefusal):
            self.adapter.wasm_code_location(row)
        for key in ('module', 'function', 'epoch'):
            row = dict(self.adapter.threads[0])
            row['native_wasm'] = dict(row['native_wasm'])
            row['native_wasm'][key] += 1
            with self.assertRaises(dap.LocalRequestRefusal):
                self.adapter.wasm_code_location(row)

    def test_native_code_query_keeps_memory_refused(self):
        calls = []
        def command(text):
            calls.append(text)
            if text == 'status': return STATUS
            self.assertEqual(text, 'wasm-code 3 17 1 0 0')
            return ('wasm-code thread=3 stop=17 module=0 function=2 epoch=8 generation=1 pc=6 size=9 first=2 count=1\n'
                    'wasm-op offset=6 size=2 safe=1 text=i32.const 0\n')
        self.adapter.command = command
        result = self.adapter.wasm_disassembly('wasm:0:2:6', 1, thread_id=3, code_only=True)
        self.assertEqual(result[0]['address'], '6')
        with self.assertRaises(dap.LocalRequestRefusal):
            self.adapter.cooperative_memory_stop()
        self.assertFalse(any(c.startswith('memory ') for c in calls))
        self.assertIsNotNone(self.adapter.stop_key)

    def test_policy_stop_does_not_round_through_javascript_number(self):
        value = '18446744073709551615'
        self.assertEqual(dap.ros_state_options({'selection': 'policy', 'ifStop': value})['ifStop'], int(value))
        for invalid in ('18446744073709551616', '01', '1.5', True):
            with self.assertRaises(ValueError):
                dap.ros_state_options({'selection': 'policy', 'ifStop': invalid})

if __name__ == '__main__':
    unittest.main()
