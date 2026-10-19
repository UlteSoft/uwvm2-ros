"""Unified event v1 validation, independent of permission or native I/O."""
from dataclasses import dataclass
from .codecs import Reader


@dataclass(frozen=True)
class Event:
    """One complete copied64-byte-header event and its unchanged wire body."""
    kind: int
    backend: int
    flags: int
    error: int
    source: int
    correlation: int
    sequence: int
    instance: int
    session: int
    body: bytes


def decode(data):
    """Reject unknown shapes, reserved bits, malformed IDs and partial bodies."""
    r = Reader(data)
    if r.integer(4) != 0x45525755 or r.integer(2) != 1:
        raise ValueError('unified event magic/version')
    kind, backend, flags = r.integer(2), r.integer(1), r.integer(1)
    if r.integer(2):
        raise ValueError('event reserved bits')
    error = r.integer(4)
    source, correlation, sequence, instance, session, size = (r.integer(8) for _ in range(6))
    body = r.take(size); r.end()
    if kind not in range(1, 8) or backend not in (1, 2) or not all((source, sequence, instance, session)):
        raise ValueError('invalid event identity')
    if kind >= 6:
        if body or flags & ~3 or (kind == 6 and not error) or (kind == 7 and not flags & 1):
            raise ValueError('invalid resource state event')
    else:
        if error:
            raise ValueError('data event has an error')
        if kind == 1:
            if correlation or flags not in (0, 8):
                raise ValueError('invalid topic/parameter event')
        elif not correlation or flags not in ((0, 4) if kind == 2 else (0,)):
            raise ValueError('invalid correlated event')
        if kind in (2, 4, 5) and (len(body) < 8 or int.from_bytes(body[:8], 'little') != correlation):
            raise ValueError('body/envelope correlation mismatch')
    return Event(kind, backend, flags, error, source, correlation, sequence, instance, session, body)


class Binding:
    """Local exact-source/sequence audit for one already owned observer."""
    def __init__(self, backend, source, correlation=0):
        if backend not in (1, 2) or type(source) is not int or not 0 < source < 1 << 64:
            raise ValueError('invalid source binding')
        if type(correlation) is not int or not 0 <= correlation < 1 << 64:
            raise ValueError('invalid correlation binding')
        self.backend, self.source, self.correlation = backend, source, correlation
        self.instance = self.session = None
        self.sequence = 0

    def decode(self, data):
        """Commit a new sequence only after all validation succeeds."""
        value = decode(data)
        if (value.backend != self.backend or value.source != self.source or
                value.sequence != self.sequence + 1 or
                (self.instance is not None and (value.instance != self.instance or value.session != self.session)) or
                (self.correlation and value.correlation and self.correlation != value.correlation)):
            raise ValueError('source/session/sequence mismatch')
        self.instance, self.session, self.sequence = value.instance, value.session, value.sequence
        return value
