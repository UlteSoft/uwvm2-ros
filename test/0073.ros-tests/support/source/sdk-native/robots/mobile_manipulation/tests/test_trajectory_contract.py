"""Unrun local wire/ownership models; no ROS/native/canonical-codec credit."""
import unittest
from types import SimpleNamespace
from roswasm.raw import Result, RosError
from ..trajectory_client import TrajectoryClient, close_owned_reverse


class CompleteCodec:
    MAX_WIRE_BYTES = 32

    @staticmethod
    def encode(value):
        if value != ('whole-goal-with-multidof-and-tolerances',):
            raise ValueError('model encode refusal')
        return b'complete-model-wire'

    @staticmethod
    def decode(value):
        if value != b'complete-model-wire':
            raise ValueError('model decode refusal')
        return ('whole-goal-with-multidof-and-tolerances',)


class Owner:
    def __init__(self, session, handle):
        self.session, self.handle = session, handle
        self.busy = False
        self.calls, self.responses, self.closed = [], [], 0

    def invoke(self, name, data, capacity):
        self.calls.append((name, data, capacity))
        answer = self.responses.pop(0)
        if isinstance(answer, BaseException):
            raise answer
        return answer

    def close(self):
        if self.busy:
            return Result(17)
        self.closed += 1
        raise RuntimeError('independent close failure ' + str(self.handle))


class Contracts(unittest.TestCase):
    def make(self, maximum=2):
        session = SimpleNamespace(active=True, retiring=False, backend=SimpleNamespace(version=2))
        owners = tuple(Owner(session, index + 1) for index in range(4))
        return TrajectoryClient(*owners, CompleteCodec, CompleteCodec, CompleteCodec, maximum), owners

    def test_full_u64_id_and_complete_result(self):
        client, owners = self.make()
        identity = (1 << 64) - 1
        owners[0].responses.append(Result(0, identity.to_bytes(8, 'little')))
        row = client.submit(('whole-goal-with-multidof-and-tolerances',))
        owners[2].responses.append(Result(0, identity.to_bytes(8, 'little') + b'\x04complete-model-wire'))
        reply, value = client.receive(row, terminal=True)
        self.assertEqual(value, ('whole-goal-with-multidof-and-tolerances',))
        self.assertEqual(row.identity, identity)
        self.assertEqual(row.state, 'terminal')

    def test_encode_refusal_before_effect(self):
        client, owners = self.make()
        with self.assertRaises(ValueError):
            client.submit(('erased-tolerances',))
        self.assertEqual(client.used, 0)
        self.assertFalse(owners[0].calls)

    def test_timeout_never_replayed_and_history_not_reused(self):
        client, owners = self.make(1)
        owners[0].responses.append(Result(110))
        with self.assertRaises(RosError):
            client.submit(('whole-goal-with-multidof-and-tolerances',))
        self.assertEqual(client.rows[0].state, 'unknown')
        with self.assertRaises(RosError) as failure:
            client.submit(('whole-goal-with-multidof-and-tolerances',))
        self.assertEqual(failure.exception.error, 90)
        self.assertEqual(len(owners[0].calls), 1)

    def test_cancel_acceptance_is_not_terminal(self):
        client, owners = self.make()
        owners[0].responses.append(Result(0, (3).to_bytes(8, 'little')))
        row = client.submit(('whole-goal-with-multidof-and-tolerances',))
        owners[3].responses.append(Result(0, b'\x01'))
        self.assertTrue(client.cancel(row))
        self.assertEqual(row.state, 'cancel-requested')
        self.assertIsNone(row.value)

    def test_malformed_feedback_poisoned_not_delivered(self):
        client, owners = self.make()
        owners[0].responses.append(Result(0, (3).to_bytes(8, 'little')))
        row = client.submit(('whole-goal-with-multidof-and-tolerances',))
        owners[1].responses.append(Result(0, (4).to_bytes(8, 'little') + b'complete-model-wire'))
        with self.assertRaises(ValueError):
            client.receive(row)
        self.assertEqual(row.state, 'unknown')
        self.assertIsNone(row.value)

    def test_duplicate_native_id_is_not_a_new_owned_goal(self):
        client, owners = self.make()
        owners[0].responses.extend([Result(0, (3).to_bytes(8, 'little'))] * 2)
        client.submit(('whole-goal-with-multidof-and-tolerances',))
        with self.assertRaises(ValueError):
            client.submit(('whole-goal-with-multidof-and-tolerances',))
        self.assertEqual(client.rows[1].state, 'unknown')

    def test_cleanup_attempts_every_owned_role_even_on_errors(self):
        client, owners = self.make()
        slots = [None] * 4
        client.retire_local()
        close_owned_reverse(owners, slots)
        self.assertEqual([owner.closed for owner in owners], [1] * 4)
        self.assertTrue(all(isinstance(item, RuntimeError) for item in slots))

    def test_decode_holds_all_role_leases(self):
        client, owners = self.make()
        owners[0].responses.append(Result(0, (3).to_bytes(8, 'little')))
        row = client.submit(('whole-goal-with-multidof-and-tolerances',))
        class ReentrantCodec(CompleteCodec):
            @staticmethod
            def decode(data):
                self.assertEqual([owner.close().error for owner in owners], [17] * 4)
                return CompleteCodec.decode(data)
        client.codecs = (CompleteCodec, ReentrantCodec, CompleteCodec)
        owners[1].responses.append(Result(0, (3).to_bytes(8, 'little') + b'complete-model-wire'))
        response, value = client.receive(row)
        self.assertIsNotNone(value)
        self.assertEqual([owner.closed for owner in owners], [0] * 4)
        self.assertFalse(any(owner.busy for owner in owners))

    def test_idle_keeps_owner_but_lost_receive_poisoned(self):
        client, owners = self.make()
        owners[0].responses.append(Result(0, (7).to_bytes(8, 'little')))
        row = client.submit(('whole-goal-with-multidof-and-tolerances',))
        owners[1].responses.extend((Result(11), Result(110)))
        self.assertEqual(client.receive(row)[0].error, 11)
        self.assertEqual(row.state, 'accepted')
        self.assertEqual(client.receive(row)[0].error, 110)
        self.assertEqual(row.state, 'unknown')
        with self.assertRaises(RosError):client.receive(row)
        self.assertEqual(len(owners[1].calls), 2)


    def test_receive_exception_after_effect_poisoned_without_replay(self):
        for terminal, initial in ((False, 'accepted'), (True, 'cancel-requested')):
            with self.subTest(terminal=terminal, state=initial):
                client, owners = self.make()
                owners[0].responses.append(Result(0, (9).to_bytes(8, 'little')))
                row = client.submit(('whole-goal-with-multidof-and-tolerances',))
                row.state = initial
                target = owners[2 if terminal else 1]
                lost = BaseException('Native receive consumed; Result allocation failed')
                target.responses.append(lost)
                with self.assertRaises(BaseException) as failure:
                    client.receive(row, terminal=terminal)
                self.assertIs(failure.exception, lost)
                self.assertIs(row.primary, lost)
                self.assertEqual(row.state, 'unknown')
                self.assertIsNone(row.value)
                with self.assertRaises(RosError) as refused:
                    client.receive(row, terminal=terminal)
                self.assertEqual(refused.exception.error, 125)
                self.assertEqual(len(target.calls), 1)
                self.assertFalse(owners[3].calls)
                self.assertFalse(any(owner.busy for owner in owners))


if __name__ == '__main__':
    unittest.main()
