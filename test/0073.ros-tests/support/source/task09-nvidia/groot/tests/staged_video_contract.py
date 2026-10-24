"""Authored host transport contracts; zero model or ROS exchange credit.

These exercise the real staging implementation with real NumPy ownership.
The small descriptor objects are typed protocol inputs, not a fake GR00T model,
ROS type support, simulator, Gateway or Wasm runtime.
"""

import unittest
from types import SimpleNamespace

import numpy as np

from native_conversion import Refused
from staged_video import StagedVideo


class Contracts(unittest.TestCase):
    def setUp(self):
        self.now = 1
        self.config = {"video": (("camera",), 1)}
        self.store = StagedVideo(
            self.config, 10**9, 10**6, 1024**2, 4, 4096, clock=lambda: self.now
        )

    def descriptor(self, shape=(1, 1, 224, 224, 3), key="camera"):
        return SimpleNamespace(key=key, shape=shape)

    def refused(self, code, operation, *args):
        with self.assertRaises(Refused) as error:
            operation(*args)
        self.assertEqual(error.exception.code, code)

    def fill(self, nonce, count):
        for offset in range(0, count, self.store.CHUNK_BYTES):
            chunk = bytes(
                (
                    index % 251
                    for index in range(
                        offset, min(offset + self.store.CHUNK_BYTES, count)
                    )
                )
            )
            self.assertEqual(
                self.store.write(nonce, "camera", offset, chunk), offset + len(chunk)
            )

    def test_complete_224_square_observation_exceeds_old_inline_limit(self):
        count = 224 * 224 * 3
        self.store.begin(7, [self.descriptor()])
        self.fill(7, count)
        result = self.store.take(7, np)["camera"]
        self.assertEqual(result.shape, (1, 1, 224, 224, 3))
        self.assertEqual(result.dtype, np.uint8)
        np.testing.assert_array_equal(
            result.reshape(-1), np.arange(count, dtype=np.uint32) % 251
        )
        self.assertTrue(result.flags.owndata)
        self.assertIsNone(self.store.active)
        self.assertEqual(self.store.accepted_bytes, count)
        self.refused(125, self.store.begin, 7, [self.descriptor()])

    def test_incomplete_frame_cannot_reach_inference(self):
        self.store.begin(7, [self.descriptor()])
        self.store.write(7, "camera", 0, b"abc")
        self.refused(11, self.store.take, 7, np)
        self.assertEqual(self.store.active["frames"]["camera"]["filled"], 3)

    def test_unknown_owner_or_nonce_cannot_access_frame(self):
        self.store.begin(7, [self.descriptor()])
        self.refused(125, self.store.write, 8, "camera", 0, b"abc")
        other = StagedVideo(
            self.config, 10**9, 10**6, 1024, 1, 2, clock=lambda: self.now
        )
        self.refused(125, other.write, 7, "camera", 0, b"abc")

    def test_contiguous_offsets_refuse_replay_and_gaps(self):
        self.store.begin(7, [self.descriptor()])
        self.store.write(7, "camera", 0, b"abc")
        self.refused(125, self.store.write, 7, "camera", 0, b"abc")
        self.refused(125, self.store.write, 7, "camera", 4, b"def")

    def test_full_chunk_validation_precedes_commit(self):
        self.store.begin(7, [self.descriptor()])
        self.refused(22, self.store.write, 7, "camera", 0, [1, 2, 256])
        self.refused(22, self.store.write, 7, "camera", 0, [True])
        self.refused(90, self.store.write, 7, "camera", 0, b"x" * 769)
        self.assertEqual(self.store.active["frames"]["camera"]["filled"], 0)

    def test_frame_overrun_is_whole_failure(self):
        self.store.begin(7, [self.descriptor((1, 1, 1, 1, 3))])
        self.refused(90, self.store.write, 7, "camera", 0, b"abcd")
        self.assertEqual(
            bytes(self.store.active["frames"]["camera"]["bytes"]), b"\0\0\0"
        )

    def test_lease_expires_without_extension_on_upload(self):
        self.store.begin(7, [self.descriptor()])
        self.now = 999999
        self.store.write(7, "camera", 0, b"abc")
        self.now = 1000001
        self.refused(110, self.store.write, 7, "camera", 3, b"def")
        self.assertIsNone(self.store.active)
        self.refused(125, self.store.begin, 7, [self.descriptor()])

    def test_one_active_observation_and_explicit_release(self):
        self.store.begin(7, [self.descriptor()])
        self.refused(17, self.store.begin, 8, [self.descriptor()])
        self.store.release(7)
        self.refused(125, self.store.begin, 7, [self.descriptor()])
        self.store.begin(8, [self.descriptor()])

    def test_dimensions_keys_and_total_budget_are_exact(self):
        self.refused(22, self.store.begin, 1, [self.descriptor(key="another")])
        self.refused(22, self.store.begin, 1, [self.descriptor((2, 1, 2, 2, 3))])
        self.refused(22, self.store.begin, 1, [self.descriptor((1, 1, 2, 2, 4))])
        self.refused(90, self.store.begin, 1, [self.descriptor((1, 1, 1024, 1024, 3))])
        self.assertIsNone(self.store.active)

    def test_multicamera_complete_budget_is_not_per_camera(self):
        config = {"video": (("a", "b"), 1)}
        store = StagedVideo(config, 10**9, 10**6, 16, 4, 64, clock=lambda: self.now)
        self.refused(
            90,
            store.begin,
            1,
            [self.descriptor((1, 1, 2, 2, 3), key) for key in ("a", "b")],
        )

    def test_finite_operations_and_history_never_evict(self):
        store = StagedVideo(self.config, 10**9, 10**6, 16, 1, 2, clock=lambda: self.now)
        row = self.descriptor((1, 1, 1, 1, 3))
        store.begin(1, [row])
        store.release(1)
        self.refused(90, store.begin, 2, [row])
        store.maximum_operations = 4
        self.refused(90, store.begin, 2, [row])

    def test_owner_deadline_and_retirement(self):
        self.now = 10**9
        self.refused(110, self.store.begin, 1, [self.descriptor()])
        self.now = 1
        self.store.close()
        self.refused(125, self.store.begin, 1, [self.descriptor()])


if __name__ == "__main__":
    unittest.main()
