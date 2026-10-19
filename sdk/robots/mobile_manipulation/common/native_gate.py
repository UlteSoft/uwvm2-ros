"""Concrete default-denied native Spot operation allowlist and once ledger.

This is NOT the Runtime capability authority. A Guest reaches these services
only through its original ROS ABI and genuine per-Instance service grants.
The operator must independently isolate the native ROS network. No Guest
management endpoint exposes this gate, audit ledger, credentials or config.
"""
from dataclasses import dataclass
import json
import os
import stat
import threading
import time


SERVICES = {
    'state': '/uwvm/spot/v5/state',
    'lease.acquire': '/uwvm/spot/v5/lease/acquire',
    'lease.retain': '/uwvm/spot/v5/lease/retain',
    'lease.return': '/uwvm/spot/v5/lease/return',
    'command.stand': '/uwvm/spot/v5/command/stand',
    'command.sit': '/uwvm/spot/v5/command/sit',
    'command.velocity': '/uwvm/spot/v5/command/velocity',
    'command.stop': '/uwvm/spot/v5/command/stop',
    'command.feedback': '/uwvm/spot/v5/command/feedback',
}


def read_native_json(path, maximum_bytes=16384):
    """Trusted-native regular owner-only file, never a Guest-provided path."""
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(fd)
        if (not stat.S_ISREG(before.st_mode) or before.st_uid not in (0, os.geteuid()) or
                before.st_mode & 0o077 or not 0 < before.st_size <= maximum_bytes):
            raise PermissionError(13, 'native configuration file ownership/mode/extent')
        data = os.read(fd, maximum_bytes + 1)
        after = os.fstat(fd)
        if (len(data) != before.st_size or len(data) > maximum_bytes or
                (before.st_dev, before.st_ino, before.st_size, before.st_mtime_ns, before.st_ctime_ns) !=
                (after.st_dev, after.st_ino, after.st_size, after.st_mtime_ns, after.st_ctime_ns)):
            raise ValueError('native configuration changed or exceeded bound')
        def unique(pairs):
            result = {}
            for key, value in pairs:
                if key in result:
                    raise ValueError('duplicate native configuration key')
                result[key] = value
            return result
        def invalid_constant(value):
            raise ValueError('nonfinite native JSON constant')
        return json.loads(data.decode('utf-8'), object_pairs_hook=unique, parse_constant=invalid_constant)
    finally:
        os.close(fd)


@dataclass
class Audit:
    token: int = 0
    nonce: int = 0
    operation: str = ''
    arguments: tuple = ()
    deadline_ns: int = 0
    state: str = 'unused'
    error: object = None


class _Permit:
    def __init__(self, gate, audit):
        self.gate, self.audit = gate, audit

    def __enter__(self):
        if self.audit.state != 'reserved':
            raise OSError(125, 'once permit cannot be entered again')
        self.audit.state = 'unknown'
        return self.audit

    def __exit__(self, kind, value, traceback):
        self.audit.error = value
        self.audit.state = 'returned' if kind is None else 'unknown'
        # No callable audit sink or additional native call after the SDK RPC.
        # Rows are retained for explicit native-only observation.


class NativeGate:
    def __init__(self, configuration=None):
        configuration = {} if configuration is None else configuration
        if type(configuration) is not dict or set(configuration) - {'format', 'maximum_history', 'operations'}:
            raise ValueError('closed operator allowlist schema')
        if configuration.get('format', 'uwvm-spot-native-allowlist-v1') != 'uwvm-spot-native-allowlist-v1':
            raise ValueError('native allowlist version')
        maximum = configuration.get('maximum_history', 64)
        operations = configuration.get('operations', {})
        if type(maximum) is not int or not 1 <= maximum <= 256 or type(operations) is not dict:
            raise ValueError('bounded native lifetime history required')
        checked = {}
        for operation, value in operations.items():
            if operation not in SERVICES or type(value) is not dict or set(value) != {'maximum_rpc_ns'}:
                raise ValueError('unknown operation or nonnumeric native dispatch configuration')
            limit = value['maximum_rpc_ns']
            if type(limit) is not int or not 0 < limit <= 5_000_000_000:
                raise ValueError('finite native RPC maximum required')
            checked[operation] = limit
        self._operations = checked
        self._rows = tuple(Audit() for _ in range(maximum))
        self._permits = tuple(_Permit(self, row) for row in self._rows)
        self._cleanup_rows = (Audit(), Audit())
        self._cleanup_permits = tuple(_Permit(self, row) for row in self._cleanup_rows)
        self._cleanup_used = [False, False]
        self._used = 0
        self._lock = threading.Lock()

    @property
    def enabled(self):
        return bool(self._operations)

    def permits_operation(self, operation):
        return operation in self._operations

    def enter(self, alias, operation, arguments, deadline_ns, token, nonce):
        """Reserve once before RPC; token and nonce are separate nonzero u64s.

        This anti-replay key is not caller identity, a Runtime owner, native
        lease or ROS request ID. A native ROS operator outside Runtime could
        still reach an enabled service unless the ROS network is isolated.
        """
        if operation not in self._operations or SERVICES.get(operation) != alias:
            raise PermissionError(13, 'native operation is not explicitly enabled')
        if (type(token) is not int or type(nonce) is not int or
                not 0 < token < 1 << 64 or not 0 < nonce < 1 << 64):
            raise ValueError('full nonzero token and independent nonce required')
        if type(arguments) is not tuple or type(deadline_ns) is not int:
            raise ValueError('fixed native argument/deadline domain')
        now = time.monotonic_ns()
        if not 0 < deadline_ns - now <= self._operations[operation]:
            raise TimeoutError('native operation deadline is expired or exceeds allowlist')
        if not self._lock.acquire(blocking=False):
            raise BlockingIOError(17, 'native allowlist ledger busy')
        try:
            if self._used == len(self._rows):
                raise OSError(90, 'non-reusing native operation history exhausted')
            for index in range(self._used):
                old = self._rows[index]
                if old.token == token or old.nonce == nonce:
                    raise ValueError('token/nonce was already used; uncertain operations are not replayed')
            row, permit = self._rows[self._used], self._permits[self._used]
            row.token, row.nonce, row.operation = token, nonce, operation
            row.arguments, row.deadline_ns, row.state = arguments, deadline_ns, 'reserved'
            self._used += 1
            return permit
        finally:
            self._lock.release()

    def audit_snapshot_native_only(self):
        """No ROS service/Guest import exposes this potentially sensitive data."""
        if not self._lock.acquire(blocking=False):
            raise BlockingIOError(17, 'native allowlist ledger busy')
        try:
            return tuple(self._rows[index] for index in range(self._used))
        finally:
            self._lock.release()

    def cleanup_enter_native_only(self, operation, deadline_ns):
        """Two reserved once slots: ordinary history exhaustion cannot block
        actual Native operator Stop/ReturnLease. Neither is silently granted.
        No ROS service exposes this method. Positive u64 keys are chosen from
        genuinely unused values; they remain anti-replay data, not authority.
        """
        if operation not in ('command.stop', 'lease.return') or operation not in self._operations:
            raise PermissionError(13, 'independent native cleanup operation not enabled')
        index = 0 if operation == 'command.stop' else 1
        if not self._lock.acquire(blocking=False):
            raise BlockingIOError(17, 'native cleanup ledger busy')
        try:
            if self._cleanup_used[index]:
                raise OSError(125, 'native cleanup already attempted; never replay')
            now = time.monotonic_ns()
            if not 0 < deadline_ns - now <= self._operations[operation]:
                raise TimeoutError('actual native cleanup deadline')
            key = (1 << 64) - 1
            # Finite <=258 retained rows; there is always an unused positive
            # u64. Never reset or evict ordinary histories to obtain one.
            while any(row.token == key or row.nonce == key for row in (*self._rows, *self._cleanup_rows)):
                key -= 1
            row = self._cleanup_rows[index]
            row.token, row.nonce = key, key
            row.operation, row.deadline_ns, row.state = operation, deadline_ns, 'reserved'
            self._cleanup_used[index] = True
            return self._cleanup_permits[index]
        finally:
            self._lock.release()

    def maximum_rpc_ns(self, operation):
        """Native immutable policy inspection; missing operations stay denied."""
        return self._operations.get(operation, 0)
