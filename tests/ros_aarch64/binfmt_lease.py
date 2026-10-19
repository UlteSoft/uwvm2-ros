#!/usr/bin/env python3
"""Lease one temporary ARM64 binfmt entry for a bounded serial test command.

The default remains two hours. A caller may explicitly select up to two days
for the complete original interpreter matrix; individual compilation/actor
deadlines remain independently bounded by their drivers. Checked completion
and handled cancellation attempt exact-owned-entry cleanup and verify registry
restoration. Abrupt process death requires the external Root registry audit.
"""
import argparse
import base64
import fcntl
import ctypes
import errno
import importlib.util
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import resource
import stat
import sys
import subprocess
import time



sys.dont_write_bytecode = True

# This support lives in binfmt_lease.py so both maintained drivers use the
# same implementation without adding an ambient import or a third script.
def _opened_body(path, maximum=16*1024**2):
    """Read bounded source/metadata bytes through one stable ordinary FD."""
    path = Path(path)
    if not path.is_absolute() or path.resolve(strict=True) != path:
        raise RuntimeError('canonical physical source/metadata path required')
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or not 0 <= first.st_size <= maximum:
            raise RuntimeError('bounded ordinary source/metadata required')
        parts, length = [], 0
        while length <= first.st_size:
            part = os.read(fd, min(65536, first.st_size+1-length))
            if not part:
                break
            parts.append(part)
            length += len(part)
        last, named = os.fstat(fd), path.stat(follow_symlinks=False)
        fields = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode', 'st_nlink')
        if length != first.st_size or any(getattr(first, k) != getattr(last, k) or
                getattr(first, k) != getattr(named, k) for k in fields):
            raise RuntimeError('opened source/metadata changed')
        body = b''.join(parts)
        return body, {'path': str(path), 'bytes': length, 'sha256': hashlib.sha256(body).hexdigest(),
            'device': first.st_dev, 'inode': first.st_ino, 'mtime_ns': first.st_mtime_ns,
            'ctime_ns': first.st_ctime_ns}
    finally:
        os.close(fd)


def load_pidfd_support(source, manifest):
    """Admit exact current source roles before importing process primitives.

    Root's outer owner supplies its original 64-GiB incarnation/watchdog. This
    driver independently binds the actual current incarnation before it starts
    any child or writes binfmt. It never upgrades a historical lease/report.
    """
    source, manifest = Path(source), Path(manifest)
    raw, manifest_pin = _opened_body(manifest)
    def pairs(items):
        result = {}
        for key, value in items:
            if key in result:
                raise RuntimeError('duplicate source manifest key')
            result[key] = value
        return result
    frozen = json.loads(raw, object_pairs_hook=pairs)
    required = ('tests/ros_aarch64/binfmt_lease.py', 'tools/ros_distro/common.py',
                'tests/ros_cgroup/probe.py', 'tests/ros_unitree/common.py')
    roles, modules = {}, {}
    for relative in required:
        body, pin = _opened_body(source/relative)
        expected = frozen[relative]
        if (pin['bytes'], pin['sha256']) != (expected['bytes'], expected['sha256']):
            raise RuntimeError('current manifest role differs: '+relative)
        roles[relative] = pin
        if relative.endswith('binfmt_lease.py'):
            continue
        name = {'tools/ros_distro/common.py': '_uwvm_arm_owned_common',
                'tests/ros_cgroup/probe.py': '_uwvm_arm_owned_probe',
                'tests/ros_unitree/common.py': '_uwvm_arm_owned_actor_snapshot'}[relative]
        spec = importlib.util.spec_from_file_location(name, source/relative)
        module = importlib.util.module_from_spec(spec)
        # Execute exactly the admitted body, not a second path open by a loader.
        exec(compile(body, str(source/relative), 'exec'), module.__dict__)
        modules[name] = module
    common = modules['_uwvm_arm_owned_common']
    common._CGROUP_PROBE = modules['_uwvm_arm_owned_probe']
    common._ARM_ACTOR_SNAPSHOT = modules['_uwvm_arm_owned_actor_snapshot']._owned_actor_snapshot
    return common, {'source': str(source), 'manifest': manifest_pin, 'roles': roles}


_OWNED_MASK_BOOTSTRAP = "import base64, fcntl, hashlib, json, os, signal, stat, sys, time\nstarted = time.monotonic()\ndef need(value, why):\n    if not value:\n        raise RuntimeError(why)\ndef check():\n    need(time.monotonic()-started < 10, 'owned same-PID mask bootstrap deadline')\nfd = int(sys.argv[1])\nexpected = sys.argv[2]\nfirst = os.fstat(fd)\nneed(stat.S_ISREG(first.st_mode) and 0 < first.st_size <= 8*1024**2,\n     'bounded ordinary sealed launch descriptor required')\nrequired = fcntl.F_SEAL_WRITE | fcntl.F_SEAL_GROW | fcntl.F_SEAL_SHRINK | fcntl.F_SEAL_SEAL\nneed(fcntl.fcntl(fd, fcntl.F_GET_SEALS) & required == required,\n     'immutable launch descriptor seals required')\nraw = bytearray()\nwhile True:\n    check()\n    part = os.read(fd, min(65536, first.st_size+1-len(raw)))\n    if not part:\n        break\n    raw.extend(part)\n    need(len(raw) <= first.st_size, 'sealed launch descriptor grew')\nlast = os.fstat(fd)\nneed(len(raw) == first.st_size and hashlib.sha256(raw).hexdigest() == expected and\n     all(getattr(first, k) == getattr(last, k) for k in\n         ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode', 'st_nlink')),\n     'same immutable launch descriptor bytes changed')\nos.close(fd)\ndef pairs(rows):\n    result = {}\n    for key, value in rows:\n        need(key not in result, 'duplicate launch key')\n        result[key] = value\n    return result\ncfg = json.loads(raw, object_pairs_hook=pairs)\nneed(type(cfg) is dict and set(cfg) == {'format_version', 'kind', 'parent', 'python', 'mask',\n     'ignored_signals', 'argv', 'executable', 'environment'} and cfg['format_version'] == 1 and\n     cfg['kind'] == 'owned-command-original-mask-exec-v1' and os.getppid() == cfg['parent'],\n     'exact original direct-parent launch control required')\npython = cfg['python']\nneed(os.path.realpath(sys.executable) == python['path'], 'actual selected physical Python required')\nself_fd = os.open('/proc/self/exe', os.O_RDONLY | os.O_CLOEXEC)\ntry:\n    before = os.fstat(self_fd)\n    need(stat.S_ISREG(before.st_mode) and before.st_size == python['bytes'] and\n         tuple(getattr(before, k) for k in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')) ==\n         tuple(python[k] for k in ('device','inode','bytes','mtime_ns','ctime_ns')),\n         'actual bootstrap executable identity differs from selected Python')\n    digest = hashlib.sha256()\n    while True:\n        check()\n        block = os.read(self_fd, 65536)\n        if not block:\n            break\n        digest.update(block)\n    after = os.fstat(self_fd)\n    need(digest.hexdigest() == python['sha256'] and\n         all(getattr(before,k) == getattr(after,k) for k in\n             ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns','st_mode','st_nlink')),\n         'actual opened bootstrap executable bytes changed')\nfinally:\n    os.close(self_fd)\ndef decode(value):\n    need(type(value) is str, 'encoded original byte argument required')\n    return base64.b64decode(value, validate=True)\nargv = [decode(value) for value in cfg['argv']]\nexecutable = decode(cfg['executable'])\nenvironment = {}\nfor key, value in cfg['environment']:\n    key, value = decode(key), decode(value)\n    need(key not in environment and key and b'=' not in key and b'\\0' not in key+value,\n         'unique original byte environment required')\n    environment[key] = value\nneed(argv and executable and all(b'\\0' not in value for value in [executable, *argv]),\n     'original clean executable/argv required')\nvalid = {int(value) for value in signal.valid_signals()} - {int(signal.SIGKILL), int(signal.SIGSTOP)}\nignored = set(cfg['ignored_signals'])\nneed(ignored <= valid and all(type(value) is int for value in cfg['mask']) and\n     set(cfg['mask']) <= {int(value) for value in signal.valid_signals()}, 'actual original signal state required')\n# exec resets caught handlers to default and preserves ignored dispositions.\n# Reproduce the requested Popen restore_signals result after Python startup.\nfor number in sorted(valid):\n    signal.signal(number, signal.SIG_IGN if number in ignored else signal.SIG_DFL)\nsignal.pthread_sigmask(signal.SIG_SETMASK, set(cfg['mask']))\nneed({int(value) for value in signal.pthread_sigmask(signal.SIG_BLOCK, set())} == set(cfg['mask']),\n     'actual child original signal mask was not restored')\ncheck()\nneed(os.getppid() == cfg['parent'], 'actual launch parent changed before target exec')\n# The original byte environment is supplied explicitly: Python locale startup\n# cannot add/remove/normalize a target variable. No subprocess or extra PID.\nos.execvpe(executable, argv, environment)\n"


def _owned_launch_control(common, argv, kwargs, original_mask, guard):
    """Seal exact target bytes for a same-PID native Python mask bootstrap.

    The caller retains its original Popen before unmasking cancellation. The
    target gets the requested cwd/session/stdio/pass_fds unchanged; Python's
    startup environment is never substituted for the original byte mapping.
    This Linux-only protocol does not use preexec_fn, a shell or another child.
    """
    guard()
    if not isinstance(argv, (list, tuple)) or not argv or len(argv) > 4096:
        raise RuntimeError('bounded literal target argument vector required')
    if kwargs.get('shell', False) or kwargs.get('preexec_fn') is not None:
        raise RuntimeError('owned direct exec forbids shell/preexec_fn')
    target = [os.fsencode(os.fspath(value)) for value in argv]
    if any(b'\0' in value for value in target):
        raise RuntimeError('target argument contains NUL')
    options = dict(kwargs)
    override = options.pop('executable', None)
    executable = os.fsencode(os.fspath(target[0] if override is None else override))
    if not executable or b'\0' in executable:
        raise RuntimeError('clean original executable required')
    environment = options.get('env')
    if environment is None:
        environment = dict(os.environb)
    else:
        actual_environment = {}
        for key, value in environment.items():
            encoded = os.fsencode(key)
            if encoded in actual_environment:
                raise RuntimeError('duplicate byte-equivalent target environment name')
            actual_environment[encoded] = os.fsencode(value)
        environment = actual_environment
    if any(not key or b'=' in key or b'\0' in key+value for key, value in environment.items()):
        raise RuntimeError('clean original byte environment required')
    original_pass = tuple(options.get('pass_fds', ()))
    for descriptor in original_pass:
        if type(descriptor) is not int or descriptor < 0:
            raise RuntimeError('actual original passed file descriptor required')
        os.fstat(descriptor)  # Reject an invalid original FD before allocating the control FD.
    python_path = Path(sys.executable).resolve(strict=True)
    python = common.opened(python_path, 512*1024**2)
    current = Path('/proc/self/exe').stat()
    if tuple(getattr(current, name) for name in ('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')) != tuple(
            python[name] for name in ('device','inode','bytes','mtime_ns','ctime_ns')):
        raise RuntimeError('bootstrap Python must be the actual currently executing physical interpreter')
    with python_path.open('rb') as stream:
        header = stream.read(64)
    if header[:5] != b'\x7fELF\x02' or header[5] != (1 if sys.byteorder == 'little' else 2):
        raise RuntimeError('actual native ELF64 Python required')
    valid = {int(value) for value in signal.valid_signals()} - {int(signal.SIGKILL), int(signal.SIGSTOP)}
    ignored = {number for number in valid if signal.getsignal(number) == signal.SIG_IGN}
    if options.get('restore_signals', True):
        ignored.difference_update(int(getattr(signal, name)) for name in ('SIGPIPE','SIGXFZ','SIGXFSZ') if hasattr(signal, name))
    encode = lambda value: base64.b64encode(value).decode('ascii')
    control = {'format_version': 1, 'kind': 'owned-command-original-mask-exec-v1',
        'parent': os.getpid(), 'python': python, 'mask': sorted(map(int, original_mask)),
        'ignored_signals': sorted(ignored), 'argv': [encode(value) for value in target],
        'executable': encode(executable),
        'environment': [[encode(key),encode(value)] for key,value in sorted(environment.items())]}
    raw = (json.dumps(control, sort_keys=True, separators=(',', ':'))+'\n').encode()
    if not 0 < len(raw) <= 8*1024**2:
        raise RuntimeError('bounded original argv/environment launch control required')
    required = fcntl.F_SEAL_WRITE | fcntl.F_SEAL_GROW | fcntl.F_SEAL_SHRINK | fcntl.F_SEAL_SEAL
    fd = os.memfd_create('uwvm-owned-command-mask', os.MFD_CLOEXEC | os.MFD_ALLOW_SEALING)
    if fd < 3:
        try:
            duplicate = fcntl.fcntl(fd, fcntl.F_DUPFD_CLOEXEC, 3)
        finally:
            os.close(fd)
        fd = duplicate
    try:
        offset = 0
        while offset < len(raw):
            guard()
            count = os.write(fd, raw[offset:])
            if count <= 0:
                raise RuntimeError('sealed control write made no progress')
            offset += count
        os.fsync(fd)
        fcntl.fcntl(fd, fcntl.F_ADD_SEALS, required)
        seals = fcntl.fcntl(fd, fcntl.F_GET_SEALS)
        if seals & required != required:
            raise RuntimeError('immutable original launch bytes required')
        os.lseek(fd, 0, os.SEEK_SET)
        facts = os.fstat(fd)
        control_digest = hashlib.sha256(raw).hexdigest()
        # pass_fds already forces close_fds in Popen. Preserve a genuine
        # close_fds=False call by inheriting just this extra FD temporarily;
        # it is closed in the bootstrap before the original target exec.
        if options.get('close_fds', True) or original_pass:
            options['pass_fds'] = (*original_pass, fd)
        else:
            os.set_inheritable(fd, True)
        options['env'] = environment
        launcher = [str(python_path), '-I', '-B', '-S', '-c', _OWNED_MASK_BOOTSTRAP, str(fd), control_digest]
        receipt = {'kind': 'owned-command-same-PID-mask-bootstrap-v1',
            'status': 'PREPARED_NOT_STARTED', 'python': python,
            'target_argv': [os.fsdecode(value) for value in target], 'target_argv_base64': control['argv'],
            'target_executable_base64': control['executable'], 'original_signal_mask': control['mask'],
            'target_ignored_signals': control['ignored_signals'],
            'target_environment_sha256': hashlib.sha256(json.dumps(control['environment'],separators=(',',':')).encode()).hexdigest(),
            'control': {'bytes': len(raw), 'sha256': control_digest, 'device': facts.st_dev,
                'inode': facts.st_ino, 'mtime_ns': facts.st_mtime_ns, 'ctime_ns': facts.st_ctime_ns,
                'mode': facts.st_mode, 'nlink': facts.st_nlink, 'seals': seals,
                'role': 'SEALED_ANONYMOUS_MEMFD_NOT_A_PATHNAME_FULL7'},
            'bootstrap_source': {'bytes': len(_OWNED_MASK_BOOTSTRAP.encode()),
                'sha256': hashlib.sha256(_OWNED_MASK_BOOTSTRAP.encode()).hexdigest()},
            'same_PID_exec': True, 'extra_process_created': False,
            'target_environment_from_original_control': True,
            'initial_process_identity_scope': 'MAY_BE_BOOTSTRAP_PYTHON; TARGET_MAPS_REQUIRE_ACTUAL_LATER_OBSERVATION'}
        guard()
        if common.opened(python_path,512*1024**2) != python:
            raise RuntimeError('actual bootstrap Python changed before launch')
        result = (fd, launcher, options, receipt)
        fd = None
        return result
    finally:
        if fd is not None:
            os.close(fd)


class OwnedCommand:
    """One exclusive command tree, including double-fork adoption.

    SerialRunner supplies stable-birth admission, all-thread discovery, actual
    cgroup membership and pidfd signals. A private Linux subreaper phase also
    adopts a grandchild whose intermediate parent exits between samples. The
    phase starts with NO pre-existing direct children; every newly adopted
    direct child must have been born no earlier than this phase's retained
    Popen. This is an exclusive single-command ancestry claim, never a scan of
    arbitrary numeric PIDs, cgroup members or a process group.

    A normal root exit is not sufficient for PASS: every retained birth must
    be terminal, adopted children reaped with P_PIDFD, and the direct-child set
    empty before another command or unmount/lease restoration can start.
    """
    def __init__(self, common, out, timeout, rss_bytes, guard=lambda: None, cleanup_guard=lambda: None):
        if not 1 <= timeout <= 172800 or not 64*1024**2 <= rss_bytes <= 10*1024**3:
            raise RuntimeError('explicit bounded command time/RSS required')
        self.common, self.out, self.guard = common, Path(out), guard
        self.cleanup_guard = cleanup_guard
        self.timeout, self.rss_limit = timeout, rss_bytes
        self.child, self.root_birth, self.closed = None, None, False
        self.launch = None
        self.adopted, self.reaped, self.signals = {}, [], []
        self.started, self.cleanup_started = time.monotonic(), None
        self.peak_rss, self.errors = 0, []
        self.cleanup = {'status': 'NOT_PERFORMED'}
        if not hasattr(os, 'P_PIDFD') or not hasattr(os, 'waitid'):
            raise RuntimeError('Linux P_PIDFD waitid support required')
        self.runner = common.SerialRunner(self.out,
            {'command_seconds': timeout, 'rss_bytes': rss_bytes, 'log_bytes': 64*1024**2}, guard)
        # The original discovery method remains the authoritative observed
        # parent->child admission. The added method only covers actual direct
        # adoptions by this exact exclusive subreaper process.
        self.base_discover = self.runner.discover
        self.runner.discover = self.discover
        self.libc = ctypes.CDLL(None, use_errno=True)
        self.libc.prctl.argtypes = [ctypes.c_int, ctypes.c_ulong, ctypes.c_ulong, ctypes.c_ulong, ctypes.c_ulong]
        self.libc.prctl.restype = ctypes.c_int
        old = ctypes.c_int()
        if self.libc.prctl(37, ctypes.addressof(old), 0, 0, 0) != 0:
            raise OSError(ctypes.get_errno(), 'PR_GET_CHILD_SUBREAPER')
        self.previous_subreaper = old.value
        if self.direct_children():
            raise RuntimeError('exclusive owned command requires no pre-existing direct children')
        # Construction is read-only. The caller must retain this object before
        # start() records the restoration obligation and changes prctl state.
        self.subreaper_active = False

    def direct_children(self):
        values = set()
        tasks = list(Path('/proc/self/task').iterdir())
        if not 1 <= len(tasks) <= 4096:
            raise RuntimeError('bounded current process task set required')
        for task in tasks:
            try:
                raw = (task/'children').read_text()
            except OSError as error:
                if error.errno not in (errno.ENOENT, errno.ESRCH):
                    raise
                continue
            fields = raw.split()
            if len(fields) > 4096 or any(not value.isdecimal() for value in fields):
                raise RuntimeError('bounded kernel direct-child list required')
            values.update(map(int, fields))
        if len(values) > 4096:
            raise RuntimeError('bounded direct-child cohort required')
        return values

    def start(self, argv, **kwargs):
        if self.child is not None or self.closed:
            raise RuntimeError('one Popen per owned command phase')
        self.guard()
        if self.direct_children():
            raise RuntimeError('exclusive owned command requires no pre-existing direct children')
        # The owner is retained by the caller. Record restoration BEFORE the
        # syscall, so even an exception immediately after successful prctl has
        # a live restoration path. Ordinary cancellation cannot land between
        # returned Popen and its explicit retention. The independent hard Root
        # watchdog is not disabled or extended here.
        control = None
        blocked = signal.pthread_sigmask(signal.SIG_BLOCK, {signal.SIGTERM, signal.SIGINT})
        try:
            # Enter the cleanup obligation before an owned control FD can
            # exist. Ordinary TERM/INT cancellation remains deferred through
            # bounded preparation and Popen retention; the hard ALRM stays on.
            control, launcher, options, self.launch = _owned_launch_control(
                self.common, argv, kwargs, blocked, self.guard)
            self.subreaper_active = True
            if self.libc.prctl(36, 1, 0, 0, 0) != 0:
                raise OSError(ctypes.get_errno(), 'PR_SET_CHILD_SUBREAPER')
            # The bootstrap restores the ORIGINAL mask/dispositions and exact
            # byte environment, then execs argv in this same retained PID.
            # No preexec_fn runs between fork and exec in the parent process.
            self.child = subprocess.Popen(launcher, **options)
            self.runner.children.append(self.child)
            self.launch.update(status='RETAINED_BOOTSTRAP_POPEN_TARGET_EXEC_UNPROVEN', pid=self.child.pid)
        finally:
            try:
                if control is not None:
                    os.close(control)
            finally:
                signal.pthread_sigmask(signal.SIG_SETMASK, blocked)
        # No poll/wait is performed until this original birth is recorded.
        first = self.common.identity(self.child.pid)
        self.runner.roots[self.child.pid] = first
        self.root_birth = self.common.birth(first)
        if first is None or first['parent_pid'] != os.getpid():
            raise RuntimeError('retained direct Popen birth unavailable')
        self.runner.acquire(self.child.pid, os.getpid())
        if self.root_birth not in self.runner.owned:
            raise RuntimeError('direct Popen pidfd admission failed')
        self.discover()
        return self.child

    def discover(self):
        self.base_discover()
        if len(self.runner.owned) > 4096:
            raise RuntimeError('bounded owned process cohort exceeded')
        for pid in self.direct_children():
            first = self.common.identity(pid)
            if first is None:
                continue
            key = self.common.birth(first)
            if key == self.root_birth:
                continue
            if self.root_birth is None or first['parent_pid'] != os.getpid() or first['starttime'] < self.root_birth[1]:
                raise RuntimeError('unexpected direct child outside exclusive command birth cohort')
            self.runner.acquire(pid, os.getpid())
            if key not in self.runner.owned:
                # A disappearance is allowed only if there is no longer this
                # birth. A still-live unproved child is never signalled.
                if self.common.birth(self.common.identity(pid)) == key:
                    raise RuntimeError('adopted direct child pidfd admission failed')
                continue
            self.adopted[key] = first
        self.reap_adopted()

    def reap_adopted(self):
        for key, original in list(self.adopted.items()):
            item = self.runner.owned.get(key)
            if item is None or key in {(r['pid'], r['starttime']) for r in self.reaped}:
                continue
            if not self.common.pidfd_terminal(item['fd']):
                continue
            try:
                result = os.waitid(os.P_PIDFD, item['fd'], os.WEXITED | os.WNOHANG)
            except ChildProcessError:
                # A different proven owned ancestor can have reaped its child;
                # this is not evidence of direct adoption reaping by us.
                if self.common.birth(self.common.identity(original['pid'])) == key:
                    raise RuntimeError('terminal adopted child not reapable by retained pidfd')
                result = None
            if result is not None and result.si_pid != original['pid']:
                raise RuntimeError('P_PIDFD reaped a different birth')
            if result is not None or self.common.birth(self.common.identity(original['pid'])) != key:
                self.reaped.append({**original, 'method': 'P_PIDFD', 'result': None if result is None else
                    {'pid': result.si_pid, 'code': result.si_code, 'status': result.si_status}})

    def wait(self):
        if self.child is None:
            raise RuntimeError('owned Popen required')
        deadline = self.started + self.timeout
        while True:
            self.guard()
            self.discover()
            for key, item in self.runner.owned.items():
                # Reuse the reviewed same-pidfd procfs-exit/birth/cgroup proof,
                # including mismatch-before-retirement ordering. A missing or
                # recycled numeric PID cannot authorize a different process.
                self.common._ARM_ACTOR_SNAPSHOT(self.common, key, item)
            rss = self.runner.rss()
            self.peak_rss = max(self.peak_rss, rss)
            if rss > self.rss_limit or time.monotonic() >= deadline:
                raise RuntimeError('owned command RSS/deadline exceeded')
            code = self.child.poll()
            if code is not None:
                self.discover()
                # Exiting root + lingering daemon is a failed command, not a
                # successful serial phase whose descendants may leak onward.
                if any(not self.common.pidfd_terminal(row['fd']) for row in self.runner.owned.values()):
                    raise RuntimeError('command exited with a live owned descendant')
                self.reap_adopted()
                unproven = [item for key, item in self.runner.unproven.items()
                    if self.common.birth(self.common.identity(item['pid'])) == key]
                if self.direct_children() or unproven:
                    raise RuntimeError('command terminal birth/child barrier incomplete')
                return code
            time.sleep(.01)

    def close(self):
        if self.closed:
            return self.cleanup
        if self.cleanup_started is None:
            self.cleanup_started = time.monotonic()
        # Keep retained original pidfds until terminal; unlike a numeric killpg,
        # no operation here can target an unrelated recycled PID/process group.
        until = self.cleanup_started + 12
        while True:
            try:
                self.cleanup_guard()
            except BaseException as error:
                self.errors.append(repr(error))
            try:
                if self.child is not None and self.root_birth not in self.runner.owned and self.child.returncode is None:
                    # An admission read can fail before the first birth exists.
                    # Do not poll/reap this retained direct Popen first: its
                    # still-unreaped birth is recoverable from the actual kernel.
                    now = self.common.identity(self.child.pid)
                    if now is None or now['parent_pid'] != os.getpid() or (self.root_birth is not None and
                            self.common.birth(now) != self.root_birth):
                        raise RuntimeError('retained Popen recovery cannot establish original direct birth')
                    if self.root_birth is None:
                        self.root_birth = self.common.birth(now)
                        self.runner.roots[self.child.pid] = now
                    self.runner.acquire(self.child.pid, os.getpid())
                self.discover()
            except BaseException as error:
                self.errors.append(repr(error))
            sig = signal.SIGTERM if time.monotonic()-self.cleanup_started < 5 else signal.SIGKILL
            for key, item in reversed(list(self.runner.owned.items())):
                try:
                    if not self.common.pidfd_terminal(item['fd']):
                        # SIGCONT permits graceful exit even if a mapping gate
                        # left this proven original process stopped.
                        signal.pidfd_send_signal(item['fd'], signal.SIGCONT)
                        signal.pidfd_send_signal(item['fd'], sig)
                        if len(self.signals) < 4096:
                            self.signals.append({'pid': key[0], 'starttime': key[1], 'signal': sig})
                except ProcessLookupError:
                    pass
                except BaseException as error:
                    self.errors.append(repr(error))
            if self.child is not None:
                self.child.poll()  # Popen reaps only its own retained child.
            try:
                self.discover()
                terminal = all(self.common.pidfd_terminal(row['fd']) for row in self.runner.owned.values())
                direct = self.direct_children()
            except BaseException as error:
                self.errors.append(repr(error)); terminal, direct = False, {-1}
            if terminal and not direct or time.monotonic() >= until:
                break
            time.sleep(.02)
        remaining = []
        for key, item in list(self.runner.owned.items()):
            try:
                terminal = self.common.pidfd_terminal(item['fd'])
            except BaseException as error:
                self.errors.append(repr(error)); terminal = False
            if terminal:
                os.close(item['fd']); del self.runner.owned[key]
            else:
                remaining.append(item['identity'])
        for key, original in self.runner.unproven.items():
            if self.common.birth(self.common.identity(original['pid'])) == key:
                remaining.append({'unproven': original})
        try:
            direct = self.direct_children()
        except BaseException as error:
            self.errors.append(repr(error)); direct = {-1}
        if direct:
            remaining.append({'unreaped_direct_children': sorted(direct)})
        if not remaining and self.subreaper_active:
            if self.libc.prctl(36, self.previous_subreaper, 0, 0, 0) != 0:
                self.errors.append('PR_SET_CHILD_SUBREAPER restore: '+str(ctypes.get_errno()))
            else:
                self.subreaper_active = False
        self.cleanup = {'status': 'PASS' if not remaining and not self.errors and not self.subreaper_active else 'CLEANUP_INCOMPLETE',
            'root_identity': None if self.child is None else self.runner.roots.get(self.child.pid),
            'root_returncode': None if self.child is None else self.child.returncode,
            'launch': self.launch,
            'remaining': remaining, 'errors': self.errors[:64], 'adopted_births': list(self.adopted.values()),
            'reaped_adopted_births': self.reaped, 'signals': self.signals,
            'peak_owned_rss_bytes': self.peak_rss, 'rss_limit_bytes': self.rss_limit,
            'previous_subreaper': self.previous_subreaper, 'subreaper_restored': not self.subreaper_active}
        self.closed = not remaining and not self.subreaper_active
        return self.cleanup


class CommandBoundary:
    """Retain actual64/swap0/controller/OOM identity and physical8GiB floor.

    This is an inner check, not a substitute for Root's independent original
    incarnation, whole-driver watchdog and commitment accounting. In cleanup,
    hard boundaries continue to be observed but their failure must not prevent
    original-pidfd retirement or the independent mount/lease cleanup attempts.
    """
    def __init__(self, common, out, seconds):
        self.common, self.out = common, Path(out)
        self.started, self.seconds = time.monotonic(), seconds
        self.probe = common._CGROUP_PROBE.probe
        first = self.probe(expected_memory_max=64*1024**3)
        if first.get('status') != 'PASS':
            raise RuntimeError('actual64GiB/swap0 boundary required: '+str(first.get('reason')))
        inc = first['cgroup_incarnation']
        self.arguments = {'expected_memory_max': 64*1024**3, 'expected_leaf': inc['path'],
            'expected_device': inc['device'], 'expected_inode': inc['inode'], 'expected_boot': inc['boot_id']}
        self.before = self.fixed(first)
        if self.before['ancestors'][0]['cpuset.cpus.effective'] != '0-31':
            raise RuntimeError('unchanged original effective CPUs0-31 required')
        self.last = self.before
        self.check()

    def fixed(self, value):
        result = {'incarnation': value['cgroup_incarnation'], 'membership': value['membership'], 'ancestors': []}
        for row in value['cgroup_ancestors']:
            if 'controllerless_hierarchy_root' in row:
                result['ancestors'].append({'path': row['path'], 'controllerless_hierarchy_root': row['controllerless_hierarchy_root']})
                continue
            events = dict((name, int(number)) for name, number in (line.split() for line in row['memory.events'].splitlines()))
            oom = {key: events.get(key, 0) for key in ('oom', 'oom_kill', 'oom_group_kill')}
            if any(oom.values()):
                raise RuntimeError('actual ancestor OOM evidence is not zero')
            result['ancestors'].append({key: row[key] for key in
                ('path', 'memory.max', 'memory.swap.max', 'cpu.max', 'pids.max', 'cpuset.cpus.effective')})
            result['ancestors'][-1]['events'] = oom
        return result

    def check(self, finalizing=False):
        actual = self.probe(**self.arguments)
        if actual.get('status') != 'PASS' or self.fixed(actual) != self.before:
            raise RuntimeError('actual original cgroup identity/controller/ancestor/OOM boundary changed')
        self.last = self.fixed(actual)
        if not finalizing:
            if time.monotonic()-self.started >= self.seconds:
                raise RuntimeError('finite whole active driver deadline')
            if int(actual['cgroup_ancestors'][0]['memory.current']) >= 60*1024**3:
                raise RuntimeError('actual cgroup soft headroom exhausted')
        if resource.getrusage(resource.RUSAGE_SELF).ru_maxrss*1024 > 768*1024**2:
            raise RuntimeError('driver RSS exceeded768MiB')
        for place in (self.out, *self.out.parents):
            fs = os.statvfs(place)
            if fs.f_bavail*fs.f_frsize < 8*1024**3:
                raise RuntimeError('unchanged physical8GiB backing reserve exhausted')
        return self.last


def recheck_pidfd_support(common, admission):
    """Terminal full-seven source-role preservation, without reimporting code."""
    expected = admission['manifest']
    if common.opened(Path(expected['path']), 16*1024**2) != expected:
        raise RuntimeError('admitted complete source manifest changed')
    for pin in admission['roles'].values():
        if common.opened(Path(pin['path']), 16*1024**2) != pin:
            raise RuntimeError('admitted owned-command source role changed')
    return True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--qemu', type=Path, required=True)
    parser.add_argument('--name', required=True)
    parser.add_argument('--report', type=Path, required=True)
    parser.add_argument('--timeout', type=int, default=7200)
    parser.add_argument('--source', type=Path)
    parser.add_argument('--manifest', type=Path)
    parser.add_argument('--rss-limit-bytes', type=int, default=2*1024**3,
                        help='explicit aggregate owned tree cap,64MiB..10GiB; Root separately reserves it')
    parser.add_argument('command', nargs=argparse.REMAINDER)
    args = parser.parse_args()
    if not re.fullmatch(r'uwvm-ros-arm64-[a-z0-9-]{1,40}', args.name):
        parser.error('use a task-specific uwvm-ros-arm64 name')
    command = args.command[1:] if args.command[:1] == ['--'] else args.command
    if not command or not 1 <= args.timeout <= 172800 or not 64*1024**2 <= args.rss_limit_bytes <= 10*1024**3:
        parser.error('bounded command required; timeout1..172800/RSS64MiB..10GiB')
    source = (args.source or Path(__file__).resolve().parents[2]).resolve(strict=True)
    manifest = (args.manifest or source.parent/'source-manifest.json').resolve(strict=True)
    if Path(__file__).resolve(strict=True) != source/'tests/ros_aarch64/binfmt_lease.py':
        raise RuntimeError('execute the exact current frozen binfmt lease source role')
    common, admission = load_pidfd_support(source, manifest)
    registry = Path('/proc/sys/fs/binfmt_misc')
    entry = registry/args.name
    qemu = args.qemu.resolve(strict=True)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    if args.report.exists() or args.report.is_symlink():
        raise RuntimeError('fresh owned lease report path required')
    boundary = CommandBoundary(common, args.report.parent, args.timeout)
    qemu_pin = common.opened(qemu, 512*1024**2)
    original = {p.name: p.read_text() for p in registry.iterdir() if p.name != 'register'}
    report = {'status': 'FAIL', 'pid': os.getpid(), 'membership': Path('/proc/self/cgroup').read_text().splitlines(),
              'qemu': str(qemu), 'qemu_sha256': qemu_pin['sha256'], 'qemu_pin': qemu_pin,
              'registry_before': original, 'name': args.name, 'command': command,
              'owned_command_source': admission, 'owned_tree_RSS_limit_bytes': args.rss_limit_bytes,
              'cgroup_boundary_before': boundary.before,
              'scope': 'Temporary kernel ARM64 execution registration; shared registry, removed after the owned command.'}
    child, owner, registered = None, None, False
    cancellation = {'signals': [], 'finalizing': False, 'committed': False}
    previous_handlers = {sig: signal.getsignal(sig) for sig in (signal.SIGTERM, signal.SIGINT)}

    def stop(signum, frame):
        if cancellation['committed']:
            return  # Actor-free final report publication is the CLI commit boundary.
        cancellation['signals'].append(signum)
        if not cancellation['finalizing']:
            raise InterruptedError('ARM binfmt command cancelled by signal '+str(signum))

    signal.signal(signal.SIGTERM, stop)
    signal.signal(signal.SIGINT, stop)
    try:
        owner = OwnedCommand(common, args.report.parent, args.timeout, args.rss_limit_bytes, boundary.check,
                             cleanup_guard=lambda: boundary.check(finalizing=True))
        boundary.check()
        if entry.exists() or original.get('status', '').strip() != 'enabled':
            raise RuntimeError('fresh entry and already enabled binfmt registry required')
        # ELF64, little endian, EM_AARCH64; ET_EXEC and ET_DYN only.
        magic = b'\x7fELF\x02\x01\x01' + b'\x00'*9 + b'\x02\x00\xb7\x00'
        mask = b'\xff'*7 + b'\x00' + b'\xff'*8 + b'\xfe\xff\xff\xff'
        for name, value in original.items():
            if 'magic '+magic.hex() in value or 'interpreter ' in value and 'aarch64' in value:
                raise RuntimeError('existing ARM64 interpreter must be reviewed before leasing another: '+name)
        escaped = lambda value: ''.join(f'\\x{byte:02x}' for byte in value).encode('ascii')
        registration = b':'+args.name.encode()+b':M::'+escaped(magic)+b':'+escaped(mask)+b':'+os.fsencode(qemu)+b':F\n'
        # Block ordinary cancellation only across the registration write and
        # initial ownership bookkeeping. Leave the hard Root alarm untouched;
        # abrupt termination is never reported as a restored/PASS lease.
        blocked = signal.pthread_sigmask(signal.SIG_BLOCK, {signal.SIGTERM, signal.SIGINT})
        try:
            with (registry/'register').open('wb', buffering=0) as stream:
                stream.write(registration)
            registered = True
            report['entry'] = entry.read_text()
            if 'interpreter '+str(qemu) not in report['entry'] or 'flags: F' not in report['entry']:
                raise RuntimeError('registered interpreter differs from leased static QEMU')
        finally:
            signal.pthread_sigmask(signal.SIG_SETMASK, blocked)
        common.atomic_json(args.report, {**report, 'status': 'RUNNING'})
        environment = dict(os.environ)
        environment['UWVM_ROS_ARM_BINFMT_ENTRY'] = args.name
        start = time.monotonic()
        child = owner.start(command, env=environment, start_new_session=True)
        report['owned_command_launch'] = owner.launch
        report['child_pid'] = child.pid
        report['child_identity'] = owner.runner.roots[child.pid]
        report['child_membership'] = Path(f'/proc/{child.pid}/cgroup').read_text().splitlines()
        if report['child_membership'] != report['membership']:
            raise RuntimeError('leased command cgroup mismatch')
        report['returncode'] = owner.wait()
        report['seconds'] = round(time.monotonic()-start, 3)
        report['status'] = 'PASS' if report['returncode'] == 0 else 'FAIL'
    except BaseException as error:
        report['status'] = 'FAIL'
        report['error'] = repr(error)
    finally:
        # Defer first cancellation during cleanup; neither descendant retirement
        # nor registry restoration may be skipped by a normal TERM/INT.
        cancellation['finalizing'] = True
        cleanup_errors = []
        if owner is not None:
            try:
                report['owned_command_cleanup'] = owner.close()
                if report['owned_command_cleanup']['status'] != 'PASS':
                    report['status'] = 'FAIL'
            except BaseException as error:
                cleanup_errors.append('owned command: '+repr(error)); report['status'] = 'FAIL'
        # Every lease restoration attempt is independent of ownership failure.
        try:
            if registered and entry.exists():
                value = entry.read_text()
                if value == report.get('entry'):
                    entry.write_text('-1\n')
                else:
                    raise RuntimeError('leased entry changed externally; did not remove another registration')
        except BaseException as error:
            cleanup_errors.append('entry restoration: '+repr(error)); report['status'] = 'FAIL'
        try:
            after = {p.name: p.read_text() for p in registry.iterdir() if p.name != 'register'}
            report['registry_after'] = after
            report['registry_restored'] = after == original
            if not report['registry_restored']:
                report['status'] = 'FAIL'
        except BaseException as error:
            cleanup_errors.append('registry audit: '+repr(error)); report['status'] = 'FAIL'
        try:
            report['source_roles_preserved'] = recheck_pidfd_support(common, admission)
            if common.opened(qemu, 512*1024**2) != qemu_pin:
                raise RuntimeError('leased QEMU bytes/identity changed')
            report['cgroup_boundary_after'] = boundary.check(finalizing=True)
            if int((Path(boundary.arguments['expected_leaf'])/'memory.current').read_text()) >= 60*1024**3:
                raise RuntimeError('terminal current64GiB headroom exhausted after cleanup')
        except BaseException as error:
            cleanup_errors.append('terminal preservation: '+repr(error)); report['status'] = 'FAIL'
        report['cleanup_errors'] = cleanup_errors
        blocked = signal.pthread_sigmask(signal.SIG_BLOCK, {signal.SIGTERM, signal.SIGINT})
        try:
            report['cancellation_signals'] = list(cancellation['signals'])
            if cancellation['signals'] or signal.sigpending() & {signal.SIGTERM, signal.SIGINT}:
                report['status'] = 'FAIL'
            common.atomic_json(args.report, report)
            if signal.sigpending() & {signal.SIGTERM, signal.SIGINT}:
                report['status'] = 'FAIL'; report['cancellation_pending_at_publication'] = True
                common.atomic_json(args.report, report)
            cancellation['committed'] = True
        finally:
            signal.pthread_sigmask(signal.SIG_SETMASK, blocked)
            for sig, handler in previous_handlers.items():
                signal.signal(sig, handler)
        print(report['status'], 'ARM64 binfmt lease', args.report, flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    raise SystemExit(main())
