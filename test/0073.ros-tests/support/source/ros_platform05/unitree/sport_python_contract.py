"""Authored complete Sport values and transactional asynchronous contracts.

Freeze only when --unitree-sport-tests is explicitly selected. These checks
execute the real generated classes and authored payload/ledger code. They do
not use a fake ROS backend, publish to a robot, or count as official tests.
The Linux owner separately decodes values() against pinned original C++ code.
"""
import math
import struct

from roswasm.unitree_sport import (
    Request, Response, OPERATIONS, SportRequestBuilder, SportPending,
    CorrelationError, prepare,
)


def request(identity=0x123456789ab, api=2055):
    value = Request()
    value.header.identity.id = identity
    value.header.identity.api_id = api
    value.header.lease.id = 0x23456789abc
    value.header.policy.priority = -13
    value.header.policy.noreply = True
    value.parameter = '{"original":19}'
    value.binary = [17, 128, 255]
    return value


def values(stream):
    """Emit 78 extent-prefixed full records, including float32 boundaries."""
    builder = SportRequestBuilder()
    for variant in range(2):
        for name, (_, kind) in OPERATIONS.items():
            args = []
            if kind == 'xyz':
                args = [struct.unpack('<f', struct.pack('<I', 0x7f7fffff))[0],
                        struct.unpack('<f', struct.pack('<I', 1))[0], -0.0] if variant else [1.25, -2.5, .0625]
            elif kind == 'integer': args = [-(1 << 31) if variant else -7]
            elif kind == 'boolean': args = [not variant]
            initial = request()
            before = initial.encode()
            value = getattr(builder, name)(initial, *args)
            assert initial.encode() == before
            assert value is not initial and value.header is not initial.header
            assert value.binary is not initial.binary
            wire = value.encode()
            body = struct.pack('<I', len(wire)) + wire
            assert stream.write(body) == len(body)


def check():
    """Return the number of completed assertion/refusal cases, without IO."""
    completed = 0
    def test(condition):
        nonlocal completed
        assert condition
        completed += 1
    def refuses(call, status=None, tracker=None):
        nonlocal completed
        before = None if tracker is None else (dict(tracker._pending), tracker._last_id, tracker._closed)
        try: call()
        except (ValueError, TypeError, OverflowError) as error:
            if status is not None: assert isinstance(error, CorrelationError) and error.error == status
            if tracker is not None: assert before == (tracker._pending, tracker._last_id, tracker._closed)
            completed += 1
        else: raise AssertionError('invalid value or correlation was accepted')
    def pending_request(identity, api=2055):
        value = request(identity, api)
        value.header.policy.noreply = False
        return value
    def reply(identity, api=2055, data='{"data":false}', code=0):
        value = Response()
        value.header.identity.id = identity
        value.header.identity.api_id = api
        value.header.status.code = code
        value.data = data
        value.binary = [13, -127, -2]
        return value
    initial = request()
    before = initial.encode()
    invalid = [('Unknown', ()), ('Move', (1, 2)), ('Move', (math.nan, 0, 0)),
               ('Move', (math.inf, 0, 0)), ('Move', (1e300, 0, 0)),
               ('Move', (True, 0, 0)), ('SpeedLevel', (True,)),
               ('SpeedLevel', (1 << 31,)), ('SpeedLevel', (-(1 << 31)-1,)),
               ('Pose', (1,)), ('Pose', ('true',)), ('Damp', (1,))]
    for operation, args in invalid:
        refuses(lambda: prepare(initial, operation, *args))
        test(initial.encode() == before)
    bad = request(); bad.binary = [256]
    refuses(lambda: prepare(bad, 'Damp'))
    bad = request(); bad.parameter = 'a' * 4097
    refuses(lambda: prepare(bad, 'Damp'))
    refuses(lambda: prepare(None, 'Damp'))
    for capacity in (0, 17, True, 1.0): refuses(lambda: SportPending(capacity))
    ledger = SportPending(1)
    for value, now, deadline in [(pending_request(0), 0, 10), (request(1), 0, 10),
            (pending_request(1, 99), 0, 10), (pending_request(1), True, 10),
            (pending_request(1), 0, 0), (pending_request(1), 0, 1 << 64)]:
        refuses(lambda: ledger.register(value, now, deadline), 22, ledger)
    ledger.register(pending_request(1), 10, 20)
    test(ledger.pending_count == 1)
    refuses(lambda: ledger.register(pending_request(1), 10, 20), 17, ledger)
    refuses(lambda: ledger.register(pending_request(2), 10, 20), 11, ledger)
    refuses(lambda: ledger.accept(reply(2), 11), 9, ledger)
    refuses(lambda: ledger.accept(reply(1, 1001), 11), 9, ledger)
    refuses(lambda: ledger.accept(reply(1), 20), 110, ledger)
    for data in ('', '{}', '[]', 'true', '{"data":0}', '{"data":"false"}',
                 '{"data":true,"data":false}', '{"data":false}x',
                 '{"data":false,"other":NaN}', '{"data":false,"other":Infinity}',
                 '{"data":false,"other":{"x":1,"x":2}}',
                 '{"data":false,"other":1e400}',
                 'a' * 4097, '{"data":false}\0'):
        refuses(lambda: ledger.accept(reply(1, data=data), 11), 22, ledger)
    response = reply(1)
    accepted = ledger.accept(response, 11)
    test(accepted.recovery is False and accepted.response.encode() == response.encode())
    test(accepted.response is not response and accepted.response.binary is not response.binary)
    test(ledger.pending_count == 0)
    refuses(lambda: ledger.accept(response, 11), 9, ledger)
    refuses(lambda: ledger.register(pending_request(1), 11, 20), 17, ledger)
    ledger.register(pending_request(2), 11, 20)
    test(ledger.expire(2, 2055, 19) == 11 and ledger.pending_count == 1)
    test(ledger.expire(2, 2055, 20) == 110 and ledger.pending_count == 0)
    ledger.register(pending_request(3), 20, 30)
    ledger.forget(3, 2055)
    refuses(lambda: ledger.register(pending_request(3), 21, 30), 17, ledger)
    ledger.register(pending_request(4), 21, 30)
    accepted = ledger.accept(reply(4, data='{"data":true,"extra":19}'), 22)
    test(accepted.recovery is True)
    ledger.register(pending_request(5), 22, 30)
    accepted = ledger.accept(reply(5, data='not JSON: native rejection', code=-7), 23)
    test(accepted.recovery is None and accepted.response.header.status.code == -7)
    ledger.register(pending_request(6, 1001), 23, 30)
    test(ledger.accept(reply(6, 1001, data='opaque'), 24).recovery is None)
    ledger.close(); ledger.close()
    test(ledger.pending_count == 0)
    refuses(lambda: ledger.register(pending_request(7), 24, 30), 9, ledger)
    refuses(lambda: ledger.accept(reply(6, 1001), 24), 9, ledger)
    ledger = SportPending()
    for identity in range(1, 17): ledger.register(pending_request(identity), 0, 10)
    test(ledger.pending_count == 16)
    refuses(lambda: ledger.register(pending_request(17), 0, 10), 11, ledger)
    ledger.forget(8, 2055)
    ledger.register(pending_request(17), 0, 10)
    test(ledger.pending_count == 16)
    ledger.close()
    last = SportPending()
    last.register(pending_request((1 << 63)-1), 0, 1)
    test(last.expire((1 << 63)-1, 2055, 1) == 110)
    refuses(lambda: last.register(pending_request(1 << 63), 0, 1), 22, last)
    refuses(lambda: last.register(pending_request((1 << 63)-1), 0, 1), 17, last)
    return completed
