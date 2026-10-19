"""Strict canonical wire codecs. These are neither ROS CDR nor Python pickle.

Integers are little endian, UTF-8 strings have u32 byte counts, and payloads
are bounded before copying. Decoders reject trailing bytes. Finite-number
requirements match this runtime's current canonical profile, independently
of the broader native ROS type. Encoding grants no operation or resource.
"""
import math
import struct

MAX_WIRE = 4193786


class Writer:
    """Bounded bytearray writer; one final immutable copy owns the result."""
    def __init__(self, maximum=MAX_WIRE):
        if type(maximum) is not int or not 0 <= maximum <= MAX_WIRE:
            raise ValueError('invalid wire allocation bound')
        self.maximum = maximum
        self.data = bytearray()

    def raw(self, value):
        """Append immutable bytes after checking the complete remaining bound."""
        if not isinstance(value, bytes):
            raise TypeError('bytes required')
        if len(value) > self.maximum - len(self.data):
            raise ValueError('wire allocation bound')
        self.data.extend(value)
        return self

    def integer(self, value, size, signed=False):
        """Append an exact integer; Python bool is not an integer wire value."""
        if type(value) is not int or size not in (1, 2, 4, 8):
            raise ValueError('invalid integer')
        return self.raw(value.to_bytes(size, 'little', signed=signed))

    def f64(self, value):
        """Append finite IEEE754 binary64, retaining signed zero."""
        if type(value) not in (float, int) or not math.isfinite(value):
            raise ValueError('finite float64 required')
        return self.raw(struct.pack('<d', value))

    def f32(self, value):
        """Append finite binary32; reject overflow instead of emitting infinity."""
        if type(value) not in (float, int) or not math.isfinite(value):
            raise ValueError('finite float32 required')
        return self.raw(struct.pack('<f', value))

    def text(self, value):
        """Write byte-counted strict UTF-8, retaining embedded NUL and BOM."""
        if not isinstance(value, str):
            raise TypeError('str required')
        if len(value) > self.maximum - len(self.data) - 4:
            raise ValueError('wire text bound')
        data = value.encode('utf-8', 'strict')
        return self.integer(len(data), 4).raw(data)

    def finish(self):
        """Return immutable owned wire bytes; no I/O occurs."""
        return bytes(self.data)


class Reader:
    """Bounded view over one immutable complete wire value."""
    def __init__(self, value):
        if not isinstance(value, bytes) or len(value) > 4194304:
            raise ValueError('bounded bytes required')
        self.data = value
        self.position = 0

    def take(self, size):
        """Read exactly size bytes, checking bounds before advancing."""
        if type(size) is not int or not 0 <= size <= len(self.data) - self.position:
            raise ValueError('truncated canonical value')
        result = self.data[self.position:self.position + size]
        self.position += size
        return result

    def integer(self, size, signed=False):
        """Read one exact LE integer, retaining all 64 bits."""
        if size not in (1, 2, 4, 8):
            raise ValueError('invalid integer width')
        return int.from_bytes(self.take(size), 'little', signed=signed)

    def f64(self):
        """Read a finite double; reject NaN and infinity."""
        value, = struct.unpack('<d', self.take(8))
        if not math.isfinite(value):
            raise ValueError('nonfinite canonical double')
        return value

    def f32(self):
        """Read finite binary32 while preserving its exact decoded numeric value."""
        value, = struct.unpack('<f', self.take(4))
        if not math.isfinite(value):
            raise ValueError('nonfinite canonical float32')
        return value

    def text(self):
        """Read strict UTF-8 without consuming a leading BOM."""
        return self.take(self.integer(4)).decode('utf-8', 'strict')

    def end(self):
        """Require exact consumption before publishing any decoded value."""
        if self.position != len(self.data):
            raise ValueError('trailing canonical bytes')


class String:
    """std_msgs/String scalar codec, including empty and Unicode values."""
    @staticmethod
    def encode(value):
        return Writer().text(value).finish()

    @staticmethod
    def decode(data):
        reader = Reader(data); value = reader.text(); reader.end(); return value


class Bool:
    """std_msgs/Bool and std_srvs/SetBool request codec, exactly 0 or 1."""
    @staticmethod
    def encode(value):
        if type(value) is not bool:
            raise ValueError('bool required')
        return bytes((int(value),))

    @staticmethod
    def decode(data):
        if data not in (b'\0', b'\1'):
            raise ValueError('invalid Bool')
        return data == b'\1'


class Twist:
    """geometry_msgs/Twist, linear XYZ followed by angular XYZ."""
    @staticmethod
    def encode(value):
        if len(value) != 6:
            raise ValueError('six Twist components required')
        writer = Writer(48)
        for item in value:
            writer.f64(item)
        return writer.finish()

    @staticmethod
    def decode(data):
        reader = Reader(data); value = tuple(reader.f64() for _ in range(6)); reader.end(); return value


class Trigger:
    """std_srvs/Trigger empty request; no fabricated request byte is sent."""
    @staticmethod
    def encode(value=None):
        if value is not None:
            raise ValueError('Trigger request is empty')
        return b''

    @staticmethod
    def decode(data):
        if data != b'':
            raise ValueError('Trigger request is empty')
        return None


class ServiceResponse:
    """Trigger/SetBool response: canonical Bool followed by String."""
    @staticmethod
    def encode(value):
        success, message = value
        return Writer().raw(Bool.encode(success)).text(message).finish()

    @staticmethod
    def decode(data):
        reader = Reader(data)
        value = Bool.decode(reader.take(1)), reader.text()
        reader.end(); return value


class Int64:
    """Signed ROS2 parameter integer; no float or 53-bit narrowing."""
    @staticmethod
    def encode(value):
        return Writer(8).integer(value, 8, True).finish()

    @staticmethod
    def decode(data):
        reader = Reader(data); value = reader.integer(8, True); reader.end(); return value


class Float64:
    """Finite scalar double parameter codec."""
    @staticmethod
    def encode(value):
        return Writer(8).f64(value).finish()

    @staticmethod
    def decode(data):
        reader = Reader(data); value = reader.f64(); reader.end(); return value


def array_codec(element, maximum):
    """Create a bounded u32-count parameter array codec over a scalar codec."""
    if type(maximum) is not int or not 0 <= maximum <= MAX_WIRE:
        raise ValueError('invalid element bound')

    class Array:
        @staticmethod
        def encode(values):
            if not isinstance(values, (tuple, list)) or len(values) > maximum:
                raise ValueError('array element bound')
            writer = Writer().integer(len(values), 4)
            for value in values:
                writer.raw(element.encode(value))
            return writer.finish()

        @staticmethod
        def decode(data):
            # Scalar codecs below have explicit framing; arbitrary generated
            # message codecs must provide their own whole-record decoder.
            reader = Reader(data); count = reader.integer(4)
            if count > maximum:
                raise ValueError('array element bound')
            values = []
            for _ in range(count):
                if element is String:
                    values.append(reader.text())
                elif element is Bool:
                    values.append(Bool.decode(reader.take(1)))
                elif element in (Int64, Float64):
                    values.append(element.decode(reader.take(8)))
                else:
                    raise ValueError('unsupported scalar array codec')
            reader.end(); return tuple(values)
    return Array


def goal_id(value):
    """Encode a nonzero opaque owned Action ID without altering its bits."""
    if type(value) is not int or not 0 < value < 1 << 64:
        raise ValueError('nonzero unsigned goal ID required')
    return Writer(8).integer(value, 8).finish()


def fibonacci_goal(order):
    """Encode the builtin Fibonacci goal within the runtime's 0..46 profile."""
    if type(order) is not int or not 0 <= order <= 46:
        raise ValueError('Fibonacci order must be 0..46')
    return Writer(4).integer(order, 4, True).finish()


def action_sequence(data, terminal=True):
    """Decode ID8/[outcome8]/count32/i32 sequence, at most 48 elements."""
    reader = Reader(data); identity = reader.integer(8)
    kind = reader.integer(1) if terminal else 0
    count = reader.integer(4)
    if not identity or count > 48 or (terminal and kind not in (2, 3, 4)):
        raise ValueError('invalid terminal Action sequence')
    sequence = tuple(reader.integer(4, True) for _ in range(count))
    if any(value < 0 for value in sequence):
        raise ValueError('negative Fibonacci value')
    reader.end(); return identity, kind, sequence
