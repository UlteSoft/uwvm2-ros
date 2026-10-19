"""Full official AddTwoInts.srv fields, not a native generated type-support object.

The exact original .srv, license and commit/blob are retained in upstream/.
Signed64 values keep all bits, including extrema; bool is not a canonical int.
The existing custom-bounded-v2 native exporter/binder must independently bind
this exact Service and its complete installed request/response/event graph.
"""
from roswasm.codecs import Reader, Writer


def _i64(value):
    if type(value) is not int or not -(1 << 63) <= value < 1 << 63:
        raise ValueError('signed int64 field required')
    return value


class AddTwoInts_Request:
    __slots__ = ('_a', '_b')
    _MIN_WIRE_SIZE = _MAX_WIRE_SIZE = 16

    def __init__(self, *, a=0, b=0):
        self.a, self.b = a, b

    @property
    def a(self):
        return self._a

    @a.setter
    def a(self, value):
        self._a = _i64(value)

    @property
    def b(self):
        return self._b

    @b.setter
    def b(self, value):
        self._b = _i64(value)

    @classmethod
    def get_fields_and_field_types(cls):
        return {'a': 'int64', 'b': 'int64'}

    @staticmethod
    def _encode(value):
        if type(value) is not AddTwoInts_Request:
            raise TypeError('AddTwoInts.Request required')
        return Writer(16).integer(value.a, 8, True).integer(value.b, 8, True).finish()

    @staticmethod
    def _decode(data):
        r = Reader(data)
        value = AddTwoInts_Request(a=r.integer(8, True), b=r.integer(8, True))
        r.end()
        return value


class AddTwoInts_Response:
    __slots__ = ('_sum',)
    _MIN_WIRE_SIZE = _MAX_WIRE_SIZE = 8

    def __init__(self, *, sum=0):
        self.sum = sum

    @property
    def sum(self):
        return self._sum

    @sum.setter
    def sum(self, value):
        self._sum = _i64(value)

    @classmethod
    def get_fields_and_field_types(cls):
        return {'sum': 'int64'}

    @staticmethod
    def _encode(value):
        if type(value) is not AddTwoInts_Response:
            raise TypeError('AddTwoInts.Response required')
        return Writer(8).integer(value.sum, 8, True).finish()

    @staticmethod
    def _decode(data):
        r = Reader(data)
        value = AddTwoInts_Response(sum=r.integer(8, True)); r.end()
        return value


class AddTwoInts:
    Request, Response = AddTwoInts_Request, AddTwoInts_Response
    _ROSWASM_TYPE = 'example_interfaces/srv/AddTwoInts'
