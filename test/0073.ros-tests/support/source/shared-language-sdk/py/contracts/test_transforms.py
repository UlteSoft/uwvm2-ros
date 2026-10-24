"""Local TF codec/owner contracts; no native middleware or sandbox proof."""
from dataclasses import replace
import math
import struct
import unittest

from roswasm import (Session, Result, RosError, TransformStamped, TransformCodec,
                     TransformBuffer, TransformBroadcaster)
from test_contracts import LocalBackend


def pose():
    return TransformStamped(0, 2, 3, 'map', 'base_link', (1.25, -2.5, 3.0),
                            (0.0, 0.0, 0.0, 1.0))


def native_bytes():
    return (struct.pack('<III', 0, 2, 3) + struct.pack('<I', 3) + b'map' +
            struct.pack('<I', 9) + b'base_link' +
            struct.pack('<7d', 1.25, -2.5, 3.0, 0.0, 0.0, 0.0, 1.0))


class TransformValues(unittest.TestCase):
    def test_complete_independent_golden(self):
        for backend in (1, 2):
            codec = TransformCodec(backend)
            self.assertEqual(codec.encode(pose()), native_bytes())
            self.assertEqual(codec.decode(native_bytes()), pose())

    def test_all_truncated_prefixes_and_trailing(self):
        for backend in (1, 2):
            codec = TransformCodec(backend)
            for count in range(len(native_bytes())):
                with self.subTest(backend=backend, count=count), self.assertRaises(ValueError):
                    codec.decode(native_bytes()[:count])
            with self.assertRaises(ValueError):
                codec.decode(native_bytes() + b'\0')

    def test_forged_lengths_and_byte_inputs(self):
        for offset in (12, 19):
            for count in (0, 1, 255, 256, 0xffffffff):
                wire = bytearray(native_bytes()); wire[offset:offset + 4] = struct.pack('<I', count)
                with self.subTest(offset=offset, count=count), self.assertRaises(ValueError):
                    TransformCodec(1).decode(bytes(wire))
        for value in (b'', bytes(77), bytes(587), bytearray(native_bytes()), None):
            with self.subTest(value=type(value)), self.assertRaises(ValueError):
                TransformCodec(1).decode(value)

    def test_integer_profiles_and_no_bool_narrowing(self):
        codec = TransformCodec(1)
        last = replace(pose(), sequence=0xffffffff, seconds=0x7fffffff, nanoseconds=999999999)
        self.assertEqual(codec.decode(codec.encode(last)), last)
        for field, values in [('sequence', (-1, 1 << 32, True, 0.0)),
                              ('seconds', (-1, 1 << 31, True, 2.0)),
                              ('nanoseconds', (-1, 1000000000, True, 3.0))]:
            for value in values:
                with self.subTest(field=field, value=value), self.assertRaises(ValueError):
                    codec.encode(replace(pose(), **{field: value}))
        with self.assertRaises(ValueError):
            TransformCodec(2).decode(codec.encode(last))
        for backend in (0, 3, True, '1', 1.0):
            with self.assertRaises(ValueError):
                TransformCodec(backend)

    def test_exact_frame_grammar_and_extents(self):
        codec = TransformCodec(2)
        maximum = replace(pose(), parent_frame='a' * 255, child_frame='b' * 255)
        self.assertEqual(len(codec.encode(maximum)), 586)
        self.assertEqual(codec.decode(codec.encode(maximum)), maximum)
        minimum = replace(pose(), parent_frame='a', child_frame='b')
        self.assertEqual(len(codec.encode(minimum)), 78)
        nested = replace(pose(), parent_frame='map2/_camera0', child_frame='_base0')
        self.assertEqual(codec.decode(codec.encode(nested)), nested)
        for field in ('parent_frame', 'child_frame'):
            for value in ('', '/map', 'map/', 'map//a', '1map', 'a/1b', 'a-b',
                          'a\0', '机器人', 'a' * 256, None):
                with self.subTest(field=field, value=value), self.assertRaises(ValueError):
                    codec.encode(replace(pose(), **{field: value}))
        with self.assertRaises(ValueError):
            codec.encode(replace(pose(), child_frame='map'))

    def test_complete_components_finite_and_quaternion_norm(self):
        codec = TransformCodec(1)
        for value in ((1.0, 2.0), (1.0, 2.0, 3.0, 4.0), [1.0, 2.0, 3.0]):
            with self.assertRaises(ValueError):
                codec.encode(replace(pose(), translation=value))
        for field, size in [('translation', 3), ('quaternion', 4)]:
            for index in range(size):
                for number in (float('nan'), float('inf'), -float('inf'), True, 10 ** 500):
                    values = list(getattr(pose(), field)); values[index] = number
                    with self.subTest(field=field, index=index, number=type(number)), self.assertRaises(ValueError):
                        codec.encode(replace(pose(), **{field: tuple(values)}))
        for quaternion in ((0., 0., 0., 0.), (0., 0., 0., 2.), (1e308, 0., 0., 0.),
                           (0., 0., 0., 1.001), (0., 0., 1.), [0., 0., 0., 1.]):
            with self.assertRaises(ValueError):
                codec.encode(replace(pose(), quaternion=quaternion))
        value = replace(pose(), translation=(1e308, -1e308, -0.0), quaternion=(0., 0., 0., 1.00025))
        self.assertEqual(codec.encode(codec.decode(codec.encode(value))), codec.encode(value))

    def test_signed_zero_and_quaternion_sign_bits(self):
        value = replace(pose(), translation=(-0.0, 0.0, -0.0), quaternion=(-0.0, 0.0, -0.0, -1.0))
        codec = TransformCodec(2); wire = codec.encode(value)
        self.assertEqual(wire[-56:], struct.pack('<7d', -0., 0., -0., -0., 0., -0., -1.))
        self.assertEqual(codec.encode(codec.decode(wire)), wire)
        self.assertEqual(math.copysign(1, codec.decode(wire).translation[0]), -1)


class TransformOwners(unittest.TestCase):
    def setUp(self):
        self.backend = LocalBackend(2)
        self.session = Session(self.backend)
        self.owner = self.session.open('selected-transform')
        self.backend.calls.clear()

    def test_lookup_one_exact_u64_request(self):
        self.backend.replies['tf_lookup'] = [Result(0, native_bytes())]
        self.assertEqual(TransformBuffer(self.owner).lookup((1 << 64) - 1, 586), pose())
        self.assertEqual(self.backend.calls, [('tf_lookup', self.owner.handle, b'\xff' * 8, 586, 0, 0)])

    def test_lookup_latest_zero_does_not_supply_identity(self):
        self.backend.replies['tf_lookup'] = [Result(0, native_bytes())]
        self.assertEqual(TransformBuffer(self.owner).lookup(), pose())
        self.assertEqual(self.backend.calls[0][2], b'\0' * 8)

    def test_lookup_preserves_capacity_timeout_and_disconnect_without_retry(self):
        buffer = TransformBuffer(self.owner)
        for result in (Result(90, required=88), Result(110), Result(107), Result(13)):
            self.backend.calls.clear(); self.backend.replies['tf_lookup'] = [result]
            with self.subTest(error=result.error), self.assertRaises(RosError) as error:
                buffer.lookup(2000000003, 0)
            self.assertIs(error.exception.result, result)
            self.assertEqual(len(self.backend.calls), 1)
            self.assertNotEqual(self.owner.handle, 0)

    def test_invalid_lookup_is_preimport(self):
        buffer = TransformBuffer(self.owner)
        for time in (-1, 1 << 64, True, 1.0):
            with self.assertRaises(ValueError): buffer.lookup(time)
        for capacity in (-1, 4194305, True, 1.0):
            with self.assertRaises(ValueError): buffer.lookup(0, capacity)
        self.assertEqual(self.backend.calls, [])

    def test_malformed_successful_reply_stays_one_call(self):
        self.backend.replies['tf_lookup'] = [Result(0, b'\0')]
        with self.assertRaises(ValueError): TransformBuffer(self.owner).lookup()
        self.assertEqual(len(self.backend.calls), 1)

    def test_broadcast_returns_original_status_and_does_not_open_alias(self):
        result = Result(13); self.backend.replies['tf_broadcast'] = [result]
        self.assertIs(TransformBroadcaster(self.owner).broadcast(pose()), result)
        self.assertEqual(self.backend.calls, [('tf_broadcast', self.owner.handle, native_bytes(), 0, 0, 0)])

    def test_invalid_broadcast_before_host_and_no_native_normalization(self):
        broadcaster = TransformBroadcaster(self.owner)
        with self.assertRaises(ValueError): broadcaster.broadcast(replace(pose(), sequence=1))
        with self.assertRaises(ValueError): broadcaster.broadcast(replace(pose(), quaternion=(0., 0., 0., 2.)))
        self.assertEqual(self.backend.calls, [])

    def test_closed_owner_and_session_do_not_enter_host(self):
        buffer = TransformBuffer(self.owner)
        TransformBroadcaster(self.owner).close().checked(); self.backend.calls.clear()
        with self.assertRaises(RosError) as error: buffer.lookup()
        self.assertEqual(error.exception.error, 9); self.assertEqual(self.backend.calls, [])
        other = self.session.open('other'); self.backend.calls.clear(); self.session.active = False
        with self.assertRaises(RosError) as error: TransformBuffer(other).lookup()
        self.assertEqual(error.exception.error, 125); self.assertEqual(self.backend.calls, [])

    def test_wrapper_construction_creates_no_capability(self):
        TransformBuffer(self.owner); TransformBroadcaster(self.owner)
        self.assertEqual(self.backend.calls, [])
        for wrapper in (TransformBuffer, TransformBroadcaster):
            with self.assertRaises(TypeError): wrapper('alias-is-not-authority')


if __name__ == '__main__':
    unittest.main()
