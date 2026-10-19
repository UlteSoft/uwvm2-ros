"""Complete UMRE256/UMRC160/URMI192 DATA validation; no effect or grant."""
from dataclasses import dataclass

U64_MAX = (1 << 64) - 1
CLIENT, SERVICE = 3, 4
MAX_PACKET, PREFIX, MAX_WAIT = 4193786, 352, 60000000000


class WireError(ValueError):
    def __init__(self, message, code=22, borrow=None):
        self.code, self.borrow = code, borrow
        super().__init__(message)


def need(value, message, code=22, borrow=None):
    if not value:
        raise WireError(message, code, borrow)


def uint(value, maximum=U64_MAX):
    need(type(value) is int and 0 <= value <= maximum, 'exact unsigned value required')
    return value


def full_handle(value):
    uint(value)
    return value & 0xffffffff != 0 and value >> 32 != 0


def number(data, at, size=8, signed=False):
    need(type(data) is bytes and 0 <= at <= len(data) - size, 'bounded owned bytes required')
    return int.from_bytes(data[at:at + size], 'little', signed=signed)


def zeros(data, at, count):
    return data[at:at + count] == bytes(count)


def qos(data):
    need(type(data) is bytes and len(data) == 80, 'exact QoS80 required')
    enums = tuple(number(data, at, 4) for at in (0, 4, 8, 12))
    depth = number(data, 16)
    need(all(value in (1, 2) for value in enums) and (enums[0] != 1 or depth != 0)
         and data[72] <= 1 and zeros(data, 73, 7), 'concrete normalized QoS required')
    durations = tuple((number(data, at), number(data, at + 8)) for at in (24, 40, 56))
    need(all(ns < 1000000000 for _, ns in durations), 'normalized duration required')
    return (enums, depth, durations, data[72] == 1)


def expected_qos(value):
    need(type(value) is tuple and len(value) == 2, 'two independently selected QoS rows required')
    for row in value:
        qos(row)
    return value


@dataclass(frozen=True)
class Binding:
    source: int
    instance: int
    generation: int
    session: int


@dataclass(frozen=True)
class Receipt:
    owner: Binding
    incarnation: int
    kind: int
    profiles: tuple
    gid: bytes
    wire: bytes


def same_owner(data, at, owner):
    return (data[at + 7] == 2 and
            tuple(number(data, at + offset) for offset in (16, 24, 32, 40)) ==
            (owner.source, owner.instance, owner.generation, owner.session))


def receipt(data, source, kind, operation, expected):
    need(type(data) is bytes and len(data) == 256 and kind in (CLIENT, SERVICE)
         and operation in (1, 2), 'exact UMRE256 required')
    need(data[:4] == b'UMRE' and number(data, 4, 2) == 1 and data[6] == operation
         and data[7] == 2 and number(data, 8, 4) == 256 and data[12] == kind
         and data[13] == 2 and data[14] <= 32 and data[15] == 0
         and zeros(data, 56, 8), 'malformed UMRE256')
    bound = Binding(*(number(data, at) for at in (16, 24, 32, 40)))
    need(bound.source == source and full_handle(source) and bound.instance != 0
         and bound.generation != 0 and bound.session != 0, 'complete current owner binding required')
    incarnation = number(data, 48)
    need(incarnation != 0 and (kind != SERVICE or data[14] == 0)
         and zeros(data, 224 + data[14], 32 - data[14]), 'incarnation/full GID extent required')
    profiles = (qos(data[64:144]), qos(data[144:224]))
    need(data[64:144] == expected[0] and data[144:224] == expected[1], 'actual QoS differs from explicit expectation', 13)
    return Receipt(bound, incarnation, kind, profiles, data[224:224 + data[14]], data)


def same_receipt(original, inspected):
    return original.wire[:6] == inspected.wire[:6] and original.wire[6] == 1 and inspected.wire[6] == 2 and original.wire[7:] == inspected.wire[7:]


@dataclass(frozen=True)
class Borrow:
    sequence: int
    reservation: int
    ticket: int


@dataclass(frozen=True)
class NativeInformation:
    source_timestamp: int
    received_timestamp: int
    native_sequence: int
    writer: bytes


@dataclass(frozen=True)
class Packet:
    borrow: Borrow
    kind: int
    control: bool
    control_flags: int
    error: int
    information: NativeInformation | None
    body: bytes


def delivery(data, endpoint, peer, maximum_body, last_committed, last_reservation):
    need(type(data) is bytes and PREFIX <= len(data) <= MAX_PACKET
         and 0 <= maximum_body <= MAX_PACKET - PREFIX and full_handle(peer)
         and peer != endpoint.owner.source, 'finite complete packet/peer required')
    need(data[:4] == b'UMRC' and number(data, 4, 2) == 1 and data[6] == 5
         and number(data, 8, 4) == 160 and data[12] == endpoint.kind and data[13] == 16
         and zeros(data, 14, 2) and zeros(data, 128, 32) and same_owner(data, 0, endpoint.owner)
         and number(data, 48) == endpoint.incarnation and number(data, 56) == peer
         and number(data, 88) == 0 and number(data, 96) == len(data)
         and number(data, 112) == 0 and number(data, 120) == 0, 'unassociated UMRC160')
    seq, nonce, ticket, extent = (number(data, at) for at in (72, 80, 64, 104))
    need(192 <= extent <= len(data) - 160 and extent - 192 <= maximum_body, 'complete record extent/quota', 90)
    need(seq != 0 and nonce != 0 and seq > last_committed and nonce > last_reservation
         and (endpoint.kind != CLIENT or (last_committed != U64_MAX and seq == last_committed + 1)), 'replayed/gapped sequence or nonce', 13)
    borrow = Borrow(seq, nonce, ticket)
    m = data[160:352]
    need(m[:4] == b'URMI' and number(m, 4, 2) == 1 and number(m, 8, 4) == 192
         and same_owner(m, 0, endpoint.owner) and number(m, 48) == endpoint.incarnation
         and number(m, 56) == seq and number(m, 64) == ticket and number(m, 112) == extent - 192
         and m[123] == 0 and zeros(m, 176, 16), 'unassociated same-take URMI192', borrow=borrow)
    kind, flags, control_flags, error = m[6], number(m, 12, 4), m[122], number(m, 124, 4)
    src, recv, native = (number(m, at, signed=True) for at in (72, 80, 104))
    pub, rec = number(m, 88), number(m, 96)
    if kind in (6, 7):
        need(flags == 0 and extent == 192 and m[120] == m[121] == 0 and zeros(m, 128, 48)
             and src == recv == native == 0 and pub == rec == U64_MAX and control_flags & ~3 == 0
             and (error != 0 if kind == 6 else control_flags & 1 != 0), 'invalid complete control reason/flags', borrow=borrow)
        return Packet(borrow, kind, True, control_flags, error, None, b'')
    need(kind == (3 if endpoint.kind == CLIENT else 2) and flags == 35 and ticket != 0
         and error == control_flags == m[120] == 0 and m[121] == 16 and zeros(m, 128, 32)
         and pub == rec == U64_MAX, 'complete request-ID/timestamp metadata required', 95 if m[121] != 16 else 22, borrow)
    info = NativeInformation(src, recv, native, m[160:176])
    return Packet(borrow, kind, False, 0, 0, info, data[352:160 + extent])
