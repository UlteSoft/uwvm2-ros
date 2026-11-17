#!/usr/bin/env python3
"""Small deployment fixtures; never import these helpers into the VM product.

DeviceLease models a trusted host-wide device ownership registry. It combines
kernel lock exclusion with a durable uncertainty record. Every deployment must
use the same protected registry and canonical device identity. This cooperative
fixture does not restrict a hostile native process's syscalls: production needs
device/network access enforcement outside the VM and independent stop evidence.

DelegatedCgroups only accepts an existing delegated subtree below the immutable
64 GiB test gate. It never enables controllers, changes an ancestor's limits,
migrates unrelated processes, or uses a privileged fallback.
"""
import fcntl
import hashlib
import json
import os
from pathlib import Path
import re
import stat

GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


class OwnershipConflict(RuntimeError):
    """Refuse a new owner; attach through the original deployment instead."""


class ReconciliationRequired(RuntimeError):
    """Ownership ended without independently established stop and effect state."""


def check_gate():
    current = Path('/sys/fs/cgroup') / Path('/proc/self/cgroup').read_text().strip().removeprefix('0::/').lstrip('/')
    if not current.is_relative_to(GATE):
        raise RuntimeError('test process is outside the existing 64 GiB gate')
    if (GATE/'memory.max').read_text().strip() != '68719476736' or (GATE/'memory.swap.max').read_text().strip() != '0':
        raise RuntimeError('the existing resource gate changed')


class DeviceLease:
    """One owner incarnation, independent of how many VM sessions it accepts.

    The lock inode is never unlinked or replaced. The registry directory and its
    ancestors are trusted deployment inputs, inaccessible to driver/VM writes.
    A crash can release flock but cannot clear the durable active record. Only
    the trusted deployment's test reconciliation method can make it available.
    Names and paths received through DDS never select a record.
    """
    def __init__(self, registry: Path, resource: str):
        if not re.fullmatch(r'[0-9a-f]{64}', resource):
            raise ValueError('expected a canonical 256-bit physical resource identity')
        self.fd = -1
        self.directory = -1
        self.resource = resource
        self.owner = None
        try:
            self.directory = os.open(registry, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW)
            info = os.fstat(self.directory)
            if info.st_uid != os.geteuid() or info.st_mode & 0o022:
                raise PermissionError('registry must be owned by the provisioner and not group/other writable')
            self.fd = os.open(resource, os.O_RDWR | os.O_CREAT | os.O_CLOEXEC | os.O_NOFOLLOW, 0o600, dir_fd=self.directory)
            info = os.fstat(self.fd)
            if not stat.S_ISREG(info.st_mode) or info.st_nlink != 1 or info.st_uid != os.geteuid() or info.st_mode & 0o077:
                raise PermissionError('unsafe ownership record')
            try:
                fcntl.flock(self.fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
            except BlockingIOError as error:
                raise OwnershipConflict('device already owned; extend access through the original deployment') from error
            self.record = self._read()
        except BaseException:
            self.close()
            raise

    def _read(self):
        size = os.fstat(self.fd).st_size
        if not size:
            return dict(resource=self.resource, state='available', epoch=0, owner=None, evidence=None)
        if size > 2048:
            raise ReconciliationRequired('oversized ownership record')
        try:
            packed = json.loads(os.pread(self.fd, size, 0))
            body = packed['body']
            expected = hashlib.sha256(self._canonical(body)).hexdigest()
            if set(packed) != {'body', 'sha256'} or expected != packed['sha256']:
                raise ValueError('invalid record checksum')
            if set(body) != {'resource', 'state', 'epoch', 'owner', 'evidence'} or body['resource'] != self.resource:
                raise ValueError('invalid record identity')
            if body['state'] not in ('available', 'active') or type(body['epoch']) is not int or not 0 <= body['epoch'] < 2**64:
                raise ValueError('invalid record state')
            if body['state'] == 'active' and not re.fullmatch(r'[0-9a-f]{32}', body['owner'] or ''):
                raise ValueError('invalid owner')
            return body
        except (ValueError, TypeError, KeyError) as error:
            raise ReconciliationRequired('corrupt ownership record; explicit external recovery required') from error

    @staticmethod
    def _canonical(value):
        return json.dumps(value, sort_keys=True, separators=(',', ':')).encode('ascii')

    def _write(self, record):
        data = self._canonical(dict(body=record, sha256=hashlib.sha256(self._canonical(record)).hexdigest()))
        if len(data) > 2048 or os.pwrite(self.fd, data, 0) != len(data):
            raise OSError('incomplete ownership record')
        os.ftruncate(self.fd, len(data))
        os.fsync(self.fd)
        os.fsync(self.directory)
        self.record = record

    def acquire(self, owner: str):
        if not re.fullmatch(r'[0-9a-f]{32}', owner):
            raise ValueError('expected an exact deployment incarnation')
        if self.fd < 0 or self.record['state'] != 'available':
            raise ReconciliationRequired('previous device owner has no trusted stop/effect reconciliation')
        epoch = self.record['epoch'] + 1
        if epoch >= 2**64:
            raise ReconciliationRequired('device epoch exhausted')
        self._write(dict(resource=self.resource, state='active', epoch=epoch, owner=owner, evidence=None))
        self.owner = owner
        return epoch

    def reconcile_for_test(self, stop_observed: str, effects_resolved: str):
        """Test stand-in for authenticated independent evidence, never a VM API.

        Nonempty labels are NOT physical evidence validation. Tests supply them
        only after their simulator has stopped and all effects are accounted for.
        Production must authenticate evidence and bind it to resource/epoch.
        """
        if self.fd < 0 or self.record['state'] != 'active':
            raise ReconciliationRequired('no uncertain owner to reconcile')
        if not stop_observed or not effects_resolved:
            raise ReconciliationRequired('both stop observation and effect reconciliation are required')
        self._write(dict(self.record, state='available', owner=None,
                         evidence=[stop_observed, effects_resolved]))
        self.owner = None

    def close(self):
        # Closing never writes "available": a dead deployer/driver may have
        # left physical effects active. Never unlink a shared lock inode.
        if self.fd >= 0:
            os.close(self.fd)
            self.fd = -1
        if self.directory >= 0:
            os.close(self.directory)
            self.directory = -1

    def __enter__(self):
        return self

    def __exit__(self, *_):
        self.close()


class DelegatedCgroups:
    """Test-owned sibling leaves below one predelegated, unpopulated subtree.

    The administration step is external. A group is configured before enrollment;
    a VM receives no writable cgroup descriptors. Removing a leaf requires it to
    be empty. Cleanup never kills processes or changes other deployments' groups.
    """
    def __init__(self, root: Path):
        check_gate()
        self.root = root.resolve(strict=True)
        if self.root == GATE or not self.root.is_relative_to(GATE):
            raise ValueError('delegation must be strictly below the existing 64 GiB gate')
        if not {'memory', 'pids'}.issubset((self.root/'cgroup.subtree_control').read_text().split()):
            raise PermissionError('delegated memory and pids controllers are required; no shared-budget fallback')
        if (self.root/'cgroup.procs').read_text().strip():
            raise PermissionError('delegated parent must contain no processes')
        self.leaves = []

    def create(self, name: str, memory_bytes: int, processes: int):
        check_gate()
        if not re.fullmatch(r'(vm|driver)-[a-z0-9-]{1,48}', name):
            raise ValueError('invalid fixture leaf name')
        if type(memory_bytes) is not int or not 0 < memory_bytes <= 68719476736 or type(processes) is not int or not 0 < processes <= 4096:
            raise ValueError('finite memory/process budgets are required')
        path = self.root/name
        path.mkdir()
        try:
            (path/'memory.max').write_text(str(memory_bytes))
            (path/'memory.swap.max').write_text('0')
            (path/'pids.max').write_text(str(processes))
            (path/'memory.oom.group').write_text('1')
        except BaseException:
            path.rmdir()
            raise
        self.leaves.append(path)
        return path

    def enroll(self, process, leaf: Path):
        # Only a waiting subprocess created and retained by this fixture is
        # accepted. The caller releases its exec gate after this succeeds.
        check_gate()
        if leaf not in self.leaves or process.poll() is not None:
            raise ValueError('unknown leaf or departed owned child')
        pid = process.pid
        status = Path(f'/proc/{pid}/status').read_text().splitlines()
        if f'PPid:\t{os.getpid()}' not in status:
            raise PermissionError('only an owned direct child may be enrolled')
        expected = '0::/' + str(GATE.relative_to('/sys/fs/cgroup'))
        actual = Path(f'/proc/{pid}/cgroup').read_text().strip()
        if actual != expected and not actual.startswith(expected+'/'):
            raise PermissionError('refuse to import a process from outside the resource gate')
        (leaf/'cgroup.procs').write_text(str(pid))
        if Path(f'/proc/{pid}/cgroup').read_text().strip() != '0::/'+str(leaf.relative_to('/sys/fs/cgroup')):
            raise RuntimeError('child enrollment did not take effect')

    def close(self):
        for leaf in reversed(self.leaves):
            if (leaf/'cgroup.procs').read_text().strip():
                raise RuntimeError('owned process must be reaped before removing its cgroup')
            leaf.rmdir()
        self.leaves.clear()
