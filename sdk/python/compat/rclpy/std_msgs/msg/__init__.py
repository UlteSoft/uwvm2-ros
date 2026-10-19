"""Complete String and Bool fields with canonical codecs for migrated rclpy code.

These guest classes intentionally contain no native rosidl shared-library handle
or type-support capsule. Native ROS identity is independently verified by the
Gateway when its explicitly granted alias is used.
"""
from roswasm import codecs


class String:
    __slots__ = ('data',)
    _ROSWASM_TYPE = 'std_msgs/msg/String'

    def __init__(self, *, data=''):
        if not isinstance(data, str):
            raise TypeError('String.data must be str')
        self.data = data

    def __eq__(self, other):
        return isinstance(other, String) and self.data == other.data

    def __repr__(self):
        return f'std_msgs.msg.String(data={self.data!r})'

    @staticmethod
    def _encode(value):
        if not isinstance(value, String):
            raise TypeError('String message required')
        return codecs.String.encode(value.data)

    @staticmethod
    def _decode(data):
        return String(data=codecs.String.decode(data))


class Bool:
    __slots__ = ('data',)
    _ROSWASM_TYPE = 'std_msgs/msg/Bool'

    def __init__(self, *, data=False):
        if type(data) is not bool:
            raise TypeError('Bool.data must be bool')
        self.data = data

    def __eq__(self, other):
        return isinstance(other, Bool) and self.data == other.data

    @staticmethod
    def _encode(value):
        if not isinstance(value, Bool):
            raise TypeError('Bool message required')
        return codecs.Bool.encode(value.data)

    @staticmethod
    def _decode(data):
        return Bool(data=codecs.Bool.decode(data))
