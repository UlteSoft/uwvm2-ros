"""One-use real C contexts; no Python tuple/int allocation at the effect boundary.

Only the statically registered optional builtin supplies transport. Importing
this module does not select a Host backend, grant an alias or activate Native.
An attempted context remains consumed even when later Python observation fails.
"""
from _roswasm_owned_service import (prepare, execute, attempted, returned,
                                    success, version_one, version_bytes, output, status_bytes)


class Fact:
    """Bounded latest attempt plus sticky exception/unknown facts.

    Tokens are installed before entry. Reading diagnostics may allocate, but
    never removes the opaque C result. No destructor performs ROS cleanup.
    """
    __slots__ = ('token', 'error', 'unknown', 'index')

    def __init__(self):
        self.token = self.error = None
        self.index = None
        self.unknown = False

    def prepare(self, index, handle=0, incarnation=0, a=0, b=0, data=b'', capacity=0):
        token = prepare(index, handle, incarnation, a, b, data, capacity)
        self.index = index
        self.token = token
        return token

    def enter(self, token):
        if self.token is not token:
            raise ValueError('actual retained context required')
        try:
            execute(token)
        except BaseException as error:
            self.error = error
            self.unknown = attempted(token) and not returned(token)
            raise
        return success(token)

    def code(self):
        return None if self.token is None or not returned(self.token) else int.from_bytes(status_bytes(self.token), 'little')

    def bytes(self):
        return output(self.token)

    def snapshot(self):
        token = self.token
        return {'attempted': token is not None and attempted(token),
                'returned': token is not None and returned(token),
                'code': self.code(), 'unknown': self.unknown, 'error': self.error,
                'retained_context': token,
                'actual_abi_version': int.from_bytes(version_bytes(token), 'little')
                    if self.index == 0 and token is not None and returned(token) else None}
