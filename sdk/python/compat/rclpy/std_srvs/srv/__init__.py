"""Exact original Empty/Trigger/SetBool fields, without fake native identities.

Trigger and SetBool use the actual existing Runtime builtin service contracts.
Empty has a complete zero-byte codec but is refused by the migration factory
until a coherent actual Runtime/Gateway/native identity extension is supplied.
No dummy member from native empty structs is serialized as an invented byte.
"""
from roswasm import codecs


class _Empty:
    __slots__ = ()
    _MIN_WIRE_SIZE = _MAX_WIRE_SIZE = 0

    @classmethod
    def get_fields_and_field_types(cls):
        return {}

    @classmethod
    def _encode(cls, value):
        if type(value) is not cls:
            raise TypeError('exact empty request/response required')
        return b''

    @classmethod
    def _decode(cls, data):
        if data != b'':
            raise ValueError('empty record has no wire fields')
        return cls()


class Empty_Request(_Empty):
    __slots__ = ()


class Empty_Response(_Empty):
    __slots__ = ()


class Trigger_Request(_Empty):
    __slots__ = ()


class SetBool_Request:
    __slots__ = ('_data',)
    _MIN_WIRE_SIZE = _MAX_WIRE_SIZE = 1

    def __init__(self, *, data=False):
        self.data = data

    @property
    def data(self):
        return self._data

    @data.setter
    def data(self, value):
        if type(value) is not bool:
            raise TypeError('bool field required')
        self._data = value

    @classmethod
    def get_fields_and_field_types(cls):
        return {'data': 'boolean'}

    @staticmethod
    def _encode(value):
        if type(value) is not SetBool_Request:
            raise TypeError('SetBool.Request required')
        return codecs.Bool.encode(value.data)

    @staticmethod
    def _decode(data):
        return SetBool_Request(data=codecs.Bool.decode(data))


class _Response:
    __slots__ = ('_success', '_message')
    _MIN_WIRE_SIZE = 5

    def __init__(self, *, success=False, message=''):
        self.success, self.message = success, message

    @property
    def success(self):
        return self._success

    @success.setter
    def success(self, value):
        if type(value) is not bool:
            raise TypeError('success must be bool')
        self._success = value

    @property
    def message(self):
        return self._message

    @message.setter
    def message(self, value):
        if not isinstance(value, str):
            raise TypeError('message must be str')
        self._message = value

    @classmethod
    def get_fields_and_field_types(cls):
        return {'success': 'boolean', 'message': 'string'}

    @classmethod
    def _encode(cls, value):
        if type(value) is not cls:
            raise TypeError('exact service Response required')
        return codecs.ServiceResponse.encode((value.success, value.message))

    @classmethod
    def _decode(cls, data):
        success, message = codecs.ServiceResponse.decode(data)
        return cls(success=success, message=message)


class Trigger_Response(_Response):
    __slots__ = ()


class SetBool_Response(_Response):
    __slots__ = ()


class Empty:
    Request, Response = Empty_Request, Empty_Response
    _ROSWASM_TYPE = 'std_srvs/srv/Empty'


class Trigger:
    Request, Response = Trigger_Request, Trigger_Response
    _ROSWASM_TYPE = 'std_srvs/srv/Trigger'


class SetBool:
    Request, Response = SetBool_Request, SetBool_Response
    _ROSWASM_TYPE = 'std_srvs/srv/SetBool'


__all__ = ['Empty', 'Trigger', 'SetBool', 'Empty_Request', 'Empty_Response',
           'Trigger_Request', 'Trigger_Response', 'SetBool_Request', 'SetBool_Response']
