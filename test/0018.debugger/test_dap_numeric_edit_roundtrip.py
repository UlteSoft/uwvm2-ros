"""Displayed bit patterns round-trip without float conversion or byte reversal."""
import importlib.util
from pathlib import Path
import unittest

spec = importlib.util.spec_from_file_location(
    'numeric_edit', Path(__file__).resolve().parents[2] / 'tools/debug/dap_adapter.py')
dap = importlib.util.module_from_spec(spec)
spec.loader.exec_module(dap)


class NumericEditRoundtrip(unittest.TestCase):
    def test_float_bits_preserve_negative_zero_and_nan_payloads(self):
        for kind, bits in [('f32', '80000000'), ('f32', '7fc00123'),
                           ('f64', '8000000000000000'), ('f64', '7ff8000000000123')]:
            self.assertEqual(dap.Adapter.encode_numeric_value(kind, 'bits=0x' + bits), bits)

    def test_vector_bytes_are_memory_order(self):
        self.assertEqual(dap.Adapter.encode_numeric_value(
            'v128', 'bytes=01000000020000000300000004000000'), '200000001 400000003')
        self.assertEqual(dap.Adapter.encode_numeric_value(
            'v128', 'bits 0x4000000030000000200000001'), '200000001 400000003')

    def test_malformed_or_oversized_patterns_are_refused(self):
        for kind, value in [('v128', 'bytes=01'), ('v128', 'bytes=' + '00' * 17),
                            ('v128', 'bytes=' + 'gg' * 16), ('f32', 'bits=0x100000000'),
                            ('f64', 'bits=0x-1'), ('i32', 'bytes=' + '00' * 16)]:
            with self.subTest(kind=kind, value=value), self.assertRaises(ValueError):
                dap.Adapter.encode_numeric_value(kind, value)


if __name__ == '__main__':
    unittest.main()
