"""Exact nanosecond Time values; no clock is read during construction."""
import math


class Time:
    def __init__(self, *, seconds=0, nanoseconds=0, clock_type=None):
        if type(seconds) not in (int, float) or not math.isfinite(seconds) or type(nanoseconds) is not int:
            raise ValueError('finite seconds and integer nanoseconds required')
        value = int(seconds * 1000000000) + nanoseconds
        if not 0 <= value < 1 << 63:
            raise ValueError('time exceeds nonnegative signed64 nanoseconds')
        if clock_type is None:
            from .clock import ClockType
            clock_type = ClockType.SYSTEM_TIME
        self.nanoseconds, self.clock_type = value, clock_type

    def seconds_nanoseconds(self):
        return divmod(self.nanoseconds, 1000000000)

    def __eq__(self, other):
        return isinstance(other, Time) and self.clock_type == other.clock_type and self.nanoseconds == other.nanoseconds

    def __lt__(self, other):
        if not isinstance(other, Time) or self.clock_type != other.clock_type:
            raise TypeError('cannot compare times from different clocks')
        return self.nanoseconds < other.nanoseconds
