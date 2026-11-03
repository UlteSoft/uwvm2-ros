"""Bounded Linux execution support for the delegated migration controllers."""
from __future__ import annotations
import fcntl
import hashlib
import json
import os
from pathlib import Path
import signal
import resource
import subprocess
import sys
import time


def sha(path: Path) -> str:
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def save(path: Path, value: object) -> None:
    path.write_text(json.dumps(value, indent=2) + '\n')


def guard(lease: Path) -> dict:
    if sys.platform != 'linux':
        raise RuntimeError('Linux keeper only; never execute this controller on macOS')
    if sys.flags.optimize != 0:
        raise RuntimeError('qualification checks require unoptimized Python startup')
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    membership = Path('/proc/self/cgroup').read_text().strip()
    base = Path('/sys/fs/cgroup')
    if '\n' in membership or not membership.startswith('0::'):
        raise RuntimeError('one actual unified process cgroup is required')
    fields = {name: (base / name).read_text().strip() for name in
              ('memory.max', 'memory.swap.max', 'cpuset.cpus.effective', 'memory.events')}
    if fields['memory.max'] != '68719476736' or fields['memory.swap.max'] != '0':
        raise RuntimeError('actual process must stay in the existing 64 GiB/swap-zero cgroup')
    if fields['cpuset.cpus.effective'] != '0,2,4,6,16-31':
        raise RuntimeError('existing twenty-CPU qualification cpuset is required')
    if str(os.getpid()) not in (base / 'cgroup.procs').read_text().split():
        raise RuntimeError('the controlling process is not in the inspected cgroup')
    # The outer host keeper holds this same mounted inode during docker exec.
    # Acquiring it here would prove the required outer lease is absent.
    with lease.resolve(strict=True).open('rb') as stream:
        identity = os.fstat(stream.fileno())
        try:
            fcntl.flock(stream, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            pass
        else:
            fcntl.flock(stream, fcntl.LOCK_UN)
            raise RuntimeError('outer centralized test lease is not held')
    return {'pid': os.getpid(), 'membership': membership,
            'boot_id': Path('/proc/sys/kernel/random/boot_id').read_text().strip(),
            'lease': str(lease), 'lease_device': identity.st_dev, 'lease_inode': identity.st_ino,
            **fields}


def start_ticks(pid: int) -> str:
    return Path(f'/proc/{pid}/stat').read_text().rsplit(') ', 1)[1].split()[19]


def run(argv: list[str], cwd: Path, log: Path, timeout: int, lease: Path,
        *, limit: int = 16 * 1024 * 1024, expected: int | None = 0) -> dict:
    before = guard(lease)
    argv = list(map(str, argv))
    began = time.monotonic()
    with log.open('wb') as output:
        child = subprocess.Popen(argv, cwd=cwd, stdout=output, stderr=subprocess.STDOUT,
                                 stdin=subprocess.DEVNULL, start_new_session=True,
                                 env=dict(os.environ, PYTHONDONTWRITEBYTECODE='1'))
        ticks = start_ticks(child.pid)
        stop_reason = None
        while child.poll() is None:
            if time.monotonic() - began > timeout:
                stop_reason = 'timeout'
            elif log.stat().st_size > limit:
                stop_reason = 'output limit'
            if stop_reason:
                # Only this just-created session and its verified leader birth.
                if start_ticks(child.pid) == ticks and os.getpgid(child.pid) == child.pid:
                    os.killpg(child.pid, signal.SIGKILL)
                child.wait(timeout=15)
                break
            time.sleep(0.05)
        status = child.wait()
    after = guard(lease)
    for event in ('oom', 'oom_kill'):
        first = dict(line.split() for line in before['memory.events'].splitlines())
        last = dict(line.split() for line in after['memory.events'].splitlines())
        if first.get(event) != last.get(event):
            stop_reason = 'cgroup ' + event + ' increased'
    row = {'argv': argv, 'cwd': str(cwd), 'exit': status,
           'seconds': time.monotonic() - began, 'log': str(log), 'log_sha256': sha(log),
           'cgroup_before': before, 'cgroup_after': after, 'stop_reason': stop_reason}
    save(log.with_suffix(log.suffix + '.command.json'), row)
    if stop_reason or (expected is not None and status != expected):
        raise RuntimeError(f'command failed: {row}')
    return row


def verify_manifest(root: Path, path: Path, expected_sha: str) -> dict:
    if sha(path) != expected_sha:
        raise RuntimeError('frozen migration manifest hash differs')
    manifest = json.loads(path.read_text())
    for row in manifest['files']:
        relative = Path(row['path'])
        if relative.is_absolute() or '..' in relative.parts or sha(root / relative) != row['sha256']:
            raise RuntimeError('frozen migration source changed: ' + str(relative))
    return manifest
