"""Copied-buffer ROS imports for the CPython/WASI built-in extension.

Every operation is invoked once. Output is owned bytes, opaque IDs remain exact
unsigned Python integers, and an unwritten length is represented by None. This
module never substitutes host Python, a socket relay, or a test double when the
built-in extension is absent. Codecs can be imported without this extension.
"""
from dataclasses import dataclass
from ._registry import FUNCTIONS

MAX_INPUT = 4193786
MAX_OUTPUT = 4194304


def unsigned(value):
    """Validate all 64 bits without accepting bool, truncation or negative IDs."""
    if type(value) is not int or not 0 <= value < 1 << 64:
        raise ValueError('an exact unsigned 64-bit integer is required')
    return value


@dataclass(frozen=True)
class Result:
    """One import outcome, preserving errno separately from optional output.

    required is meaningful only when the host actually overwrote the length
    sentinel on error90. A timeout110 can leave external effects admitted.
    Neither required nor timeout authorizes an automatic retry.
    """
    error: int
    data: bytes = b''
    required: int | None = None
    value: int | None = None

    def checked(self):
        """Raise while retaining the entire ABI outcome on the exception."""
        if self.error:
            raise RosError(self)
        return self


class RosError(RuntimeError):
    """An ABI failure with its original error, size and admission ambiguity."""
    def __init__(self, result):
        self.result = result
        self.error = result.error
        super().__init__(f'ROS ABI error {result.error}; required={result.required}')


class _OpenCapture:
    """Explicit optional C evidence, reserved before a real legacy open.

    The private GC-visible witness retains C flags/fullid/cleanup code
    independently of subsequent generic invokes. All strong Python references,
    including an exact primary/traceback, participate in cyclic GC. Reading a
    copied snapshot may allocate; no observation consumes the record. Neither
    local object identity nor this record grants Host permissions. Destructor
    makes no direct ROS calls; releasing Guest roots may run Guest finalizers.
    """
    __slots__ = ('backend', 'alias', 'data', 'token', 'primary', 'cleanup_error',
                 '_close', '_adopt', '_cleaned', '_failed', '_pending', '_snapshot', '_primary')

    def __init__(self, backend, alias, data, retirement):
        from _roswasm_native import (prepare_open_witness, cleanup_open_witness,
            adopt_open_witness, open_witness_cleaned, open_witness_cleanup_failed,
            open_witness_pending, open_witness_snapshot, open_witness_primary)
        self.backend, self.alias, self.data = backend, alias, data
        self.primary = self.cleanup_error = None
        self._close, self._adopt = cleanup_open_witness, adopt_open_witness
        self._cleaned, self._failed = open_witness_cleaned, open_witness_cleanup_failed
        self._pending, self._snapshot, self._primary = open_witness_pending, open_witness_snapshot, open_witness_primary
        self.token = prepare_open_witness(data, retirement)

    def cleanup(self):
        return self._close(self.token)

    def adopt(self):
        return self._adopt(self.token)

    def cleaned(self):
        return self._cleaned(self.token)

    def cleanup_failed(self):
        return self.cleanup_error is not None or self._failed(self.token)

    def pending(self):
        return self._pending(self.token)

    def snapshot(self):
        data = self._snapshot(self.token)
        return {'entered': bool(data[0]), 'returned': bool(data[1]),
                'delivered': bool(data[2]), 'adopted': bool(data[3]),
                'cleanup_attempted': bool(data[4]), 'cleanup_returned': bool(data[5]),
                'code': int.from_bytes(data[8:12], 'little'),
                'cleanup_code': int.from_bytes(data[12:16], 'little'),
                'cleanup_preparation_code': int.from_bytes(data[24:28], 'little'),
                'full_handle': int.from_bytes(data[16:24], 'little'),
                'C_primary': self._primary(self.token), 'primary': self.primary,
                'cleanup_error': self.cleanup_error, 'retained_context': self.token}


class Backend:
    """One real built-in backend. Construction opens no ROS capability.

    Use invoke for every function in FUNCTIONS[version]. Its copied-buffer
    signature is deliberately separate from raw numeric Wasm addresses:
    name, handle, payload, capacity, other_handle, correlation. The registry
    records the exact form. An unsupported ROS1/ROS2 operation is refused
    before invoking another backend. No cross-backend fallback exists.
    """
    def __init__(self, version):
        if type(version) is not int or version not in (1, 2):
            raise ValueError('select ROS1 or ROS2 explicitly')
        from _roswasm_native import invoke, backend
        if backend != version:
            raise ValueError('interpreter was built for a different ROS backend')
        self.version = version
        self.functions = FUNCTIONS[version]
        self._invoke = invoke

    def invoke(self, name, handle=0, payload=b'', capacity=0, other=0, correlation=0):
        """Perform exactly one named import; never retry or grant permission."""
        if name not in self.functions:
            return Result(95)
        unsigned(handle); unsigned(other); unsigned(correlation)
        if not isinstance(payload, bytes):
            raise TypeError('immutable canonical bytes are required')
        if len(payload) > MAX_INPUT:
            raise ValueError('request exceeds the ABI byte bound')
        if type(capacity) is not int or not 0 <= capacity <= MAX_OUTPUT:
            raise ValueError('invalid output capacity')
        raw = self._invoke(name, handle, payload, capacity, other, correlation)
        try:
            return Result(*raw)
        except BaseException as error:
            if name == 'capability_open' and raw[0] == 0 and raw[3]:
                try:
                    cleanup = self._invoke('capability_close', raw[3], b'', 0, 0, 0)
                except BaseException as cleanup:
                    try:
                        error.roswasm_cleanup = cleanup
                    except BaseException:
                        pass
                else:
                    try:
                        error.roswasm_cleanup = cleanup
                    except BaseException:
                        pass
            raise

    def retire(self, name, handle, witness):
        """Close/revoke with an actual-import witness, never an automatic retry.

        False means the C import boundary was not crossed. True also covers
        unknown/post-effect failures and must never be reset or replayed.
        """
        if name not in ('capability_close', 'capability_revoke'):
            raise ValueError('retirement operation required')
        unsigned(handle)
        if not handle:
            raise ValueError('nonzero retirement handle required')
        if type(witness) is not list or len(witness) != 1 or witness[0] is not False:
            raise ValueError('unattempted exact-list witness required')
        raw = self._invoke(name, handle, b'', 0, 0, 0, witness)
        return Result(*raw)

    def abi_version(self):
        """Read the version value without opening a capability."""
        return self.invoke('abi_version').checked().value

    def prepare_owned_open(self, alias, retirement):
        """Reserve optional same-Wasm C witness; does not enter an import."""
        if self.version != 2 or type(alias) is not str:
            raise ValueError('explicit backend2 Unicode alias required')
        data = alias.encode('utf-8', 'strict')
        if not data or len(data) > 255 or b'\0' in data:
            raise ValueError('invalid alias')
        return _OpenCapture(self, alias, data, retirement)

    def open(self, alias, *, capture=None):
        """Open exactly one host-granted alias; UTF-8 length is 1..255 bytes."""
        if capture is not None:
            if type(capture) is not _OpenCapture or capture.backend is not self or capture.alias != alias:
                raise ValueError('actual prepared same-backend alias capture required')
            try:
                raw = self._invoke('capability_open', 0, capture.data, 0, 0, 0, None, capture.token)
                return Result(*raw)
            except BaseException as primary:
                if capture.primary is None:
                    capture.primary = primary
                try:
                    # Actual C numeric close, not another generic allocating
                    # invoke. An already entered C fallback is never replayed.
                    capture.cleanup()
                except BaseException as cleanup:
                    capture.cleanup_error = cleanup
                raise
        if not isinstance(alias, str):
            raise TypeError('alias must be Unicode text')
        data = alias.encode('utf-8', 'strict')
        if not data or len(data) > 255 or b'\0' in data:
            raise ValueError('invalid alias')
        return self.invoke('capability_open', payload=data)
