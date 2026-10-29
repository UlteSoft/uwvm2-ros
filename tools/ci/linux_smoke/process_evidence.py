"""Linux process evidence, including an exec stop for short unit executables.

No capabilities, LSM policy, seccomp policy or dumpability are changed. A child
asks its own parent to trace its exec; refusal is an evidence failure. The
observer records the actual loaded ELF before allowing its first instruction.
"""
from pathlib import Path
import ctypes
import hashlib
import json
import os
import select
import signal
import stat
import subprocess
import sys
import time


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def read_value(path):
    try:
        return {'status': 'READ', 'value': Path(path).read_text().strip()}
    except OSError as error:
        return {'status': 'UNREADABLE', 'errno': error.errno, 'reason': str(error)}


def process_resources(pid):
    proc = Path('/proc')/str(pid)
    membership = read_value(proc/'cgroup')
    mountinfo = read_value(proc/'mountinfo')
    result = {'pid': pid, 'membership': membership, 'mountinfo': mountinfo,
              'limits': read_value(proc/'limits'), 'status': read_value(proc/'status'),
              'observed_ns': time.monotonic_ns(), 'manual_64GiB_admission': False}
    try:
        result['affinity'] = {'status': 'READ', 'cpus': sorted(os.sched_getaffinity(pid))}
    except OSError as error:
        result['affinity'] = {'status': 'UNREADABLE', 'errno': error.errno, 'reason': str(error)}
    groups = []
    if membership['status'] == mountinfo['status'] == 'READ':
        unified = [line[3:] for line in membership['value'].splitlines() if line.startswith('0::')]
        mounts = []
        for raw in mountinfo['value'].splitlines():
            left, separator, right = raw.partition(' - ')
            fields = left.split()
            if separator and right.split()[0] == 'cgroup2' and len(fields) >= 6:
                mounts.append((raw, fields[3], fields[4]))
        if len(unified) == 1:
            group = Path(unified[0])
            for raw, root, destination in mounts:
                mount_root, mount = Path(root), Path(destination)
                if '..' in group.parts or not group.is_relative_to(mount_root):
                    continue
                leaf = mount/group.relative_to(mount_root)
                chain = []
                current = leaf
                for unused in range(64):
                    entry = {'path': str(current)}
                    try:
                        info = current.stat()
                        entry.update(device=info.st_dev, inode=info.st_ino)
                    except OSError as error:
                        entry.update(errno=error.errno, reason=str(error))
                    entry['files'] = {name: read_value(current/name) for name in
                        ('cgroup.procs', 'cgroup.events', 'memory.max', 'memory.swap.max',
                         'memory.current', 'memory.events', 'cpu.max', 'cpuset.cpus.effective', 'pids.max')}
                    chain.append(entry)
                    if current == mount:
                        break
                    current = current.parent
                groups.append({'mount': raw, 'leaf': str(leaf), 'ancestors': chain})
    result['cgroups'] = groups
    result['cgroup_resolution'] = ('RESOLVED_WITH_PER_FILE_READ_RESULTS' if groups else
        'UNRESOLVED: no accessible unambiguous unified membership and matching cgroup2 mount; raw reads retained')
    return result


def birth(pid):
    return (Path('/proc')/str(pid)/'stat').read_text().rsplit(') ', 1)[1].split()[19]


def ptrace(request, pid, data=0):
    libc = ctypes.CDLL(None, use_errno=True)
    libc.ptrace.restype = ctypes.c_long
    libc.ptrace.argtypes = [ctypes.c_uint, ctypes.c_uint, ctypes.c_void_p, ctypes.c_void_p]
    ctypes.set_errno(0)
    value = libc.ptrace(request, pid, None, ctypes.c_void_p(data))
    if value == -1:
        number = ctypes.get_errno()
        raise OSError(number, os.strerror(number))
    return value


def exec_child(parent, argv):
    require(sys.platform == 'linux' and argv, 'Linux observed exec requires an exact command')
    libc = ctypes.CDLL(None, use_errno=True)
    require(libc.prctl(1, signal.SIGKILL, 0, 0, 0) == 0, 'parent-death signal could not be installed')
    require(os.getppid() == parent, 'observer retired before exec admission')
    ptrace(0, 0)  # PTRACE_TRACEME: only the requesting process's real parent.
    os.execvpe(argv[0], argv, os.environ)


def run_observed(argv, *, log, receipt, timeout, env):
    require(sys.platform == 'linux', 'actual exec observation requires Linux')
    argv = list(map(str, argv))
    target = Path(argv[0]).resolve(strict=True)
    with target.open('rb') as stream:
        info = os.fstat(stream.fileno())
        require(stat.S_ISREG(info.st_mode) and not info.st_mode & 0o6000, 'plain non-setid unit ELF required')
        require(stream.read(4) == b'\x7fELF', 'unit executable is not an ELF')
        stream.seek(0)
        expected = hashlib.file_digest(stream, 'sha256').hexdigest()
    log, receipt = Path(log), Path(receipt)
    require(not receipt.exists() and not log.exists(), 'fresh actual-exec evidence paths required')
    row = {'format': 'uwvm-unit-exec-stop-v1', 'status': 'FAIL', 'argv': argv,
           'expected_executable_sha256': expected, 'observer_pid': os.getpid(),
           'observer_birth': birth(os.getpid()), 'started_ns': time.monotonic_ns()}
    child, descriptor = None, None
    deadline = time.monotonic()+timeout
    try:
        with log.open('xb') as output:
            child = subprocess.Popen([sys.executable, '-B', str(Path(__file__).resolve()),
                '--exec-child', str(os.getpid()), *argv], env=env, stdin=subprocess.DEVNULL,
                stdout=output, stderr=subprocess.STDOUT)
            descriptor = os.pidfd_open(child.pid)
            ticks = birth(child.pid)
            row.update(pid=child.pid, birth=ticks)
            while True:
                require(time.monotonic() < deadline, 'unit exec-stop deadline exceeded')
                observed_pid, status = os.waitpid(child.pid, os.WNOHANG | os.WUNTRACED)
                if observed_pid:
                    row['exec_stop_wait_status'] = status
                    if os.WIFEXITED(status) or os.WIFSIGNALED(status):
                        child.returncode = os.waitstatus_to_exitcode(status)
                        raise RuntimeError('unit exited before its actual ELF could be observed')
                    require(os.WIFSTOPPED(status) and os.WSTOPSIG(status) == signal.SIGTRAP,
                            'expected the kernel exec SIGTRAP before unit instructions')
                    break
                time.sleep(.002)
            ptrace(0x4200, child.pid, 0x00100000)  # PTRACE_SETOPTIONS / EXITKILL.
            proc = Path('/proc')/str(child.pid)
            with (proc/'exe').open('rb') as stream:
                image = os.fstat(stream.fileno())
                checksum = hashlib.file_digest(stream, 'sha256').hexdigest()
            require(checksum == expected and (image.st_dev, image.st_ino) == (info.st_dev, info.st_ino),
                    'loaded unit ELF differs from the compiled original')
            actual_argv = (proc/'cmdline').read_bytes().rstrip(b'\0').split(b'\0')
            require(actual_argv == [os.fsencode(value) for value in argv], 'actual unit argv differs')
            require(birth(child.pid) == ticks, 'unit PID birth changed at exec stop')
            row['actual_exec'] = {'pid': child.pid, 'birth': ticks, 'argv': argv,
                'executable': os.readlink(proc/'exe'), 'executable_sha256': checksum,
                'device': image.st_dev, 'inode': image.st_ino, 'bytes': image.st_size,
                'resources': process_resources(child.pid), 'before_first_user_instruction': True,
                'observed_ns': time.monotonic_ns(), 'mechanism': 'kernel exec stop requested by the same child; parent-only tracing'}
            ptrace(7, child.pid)  # PTRACE_CONT, after identity/resource capture.
            row['released_ns'] = time.monotonic_ns()
            while True:
                require(time.monotonic() < deadline, 'bounded actual unit timeout')
                require(log.stat().st_size < 16*1024**2, 'bounded unit log exceeded')
                observed_pid, status = os.waitpid(child.pid, os.WNOHANG | os.WUNTRACED)
                if observed_pid:
                    if os.WIFEXITED(status) or os.WIFSIGNALED(status):
                        child.returncode = os.waitstatus_to_exitcode(status)
                        row.update(exit=child.returncode, exit_wait_status=status, exit_observed_ns=time.monotonic_ns())
                        break
                    require(os.WIFSTOPPED(status), 'unexpected traced unit wait status')
                    ptrace(7, child.pid, os.WSTOPSIG(status))
                time.sleep(.002)
            require(select.select([descriptor], [], [], 0)[0], 'actual unit pidfd has not retired')
            row.update(status='OBSERVED_AND_RETIRED', retired=True)
    except BaseException as error:
        row['error'] = repr(error)
        raise
    finally:
        if child is not None and child.returncode is None:
            if descriptor is not None:
                try:
                    signal.pidfd_send_signal(descriptor, signal.SIGKILL)
                except ProcessLookupError:
                    pass
            end = time.monotonic()+10
            while time.monotonic() < end:
                observed_pid, status = os.waitpid(child.pid, os.WNOHANG | os.WUNTRACED)
                if observed_pid and (os.WIFEXITED(status) or os.WIFSIGNALED(status)):
                    child.returncode = os.waitstatus_to_exitcode(status)
                    row.update(exit=child.returncode, exit_wait_status=status, exit_observed_ns=time.monotonic_ns())
                    break
                time.sleep(.01)
            row['retired'] = bool(descriptor is not None and select.select([descriptor], [], [], 0)[0])
            if not row['retired']:
                row.update(status='FAIL', retirement_error='same actual PID has not retired')
        if descriptor is not None:
            os.close(descriptor)
        row['finished_ns'] = time.monotonic_ns()
        if log.exists():
            row['log'] = {'bytes': log.stat().st_size, 'sha256': hashlib.sha256(log.read_bytes()).hexdigest()}
        receipt.write_text(json.dumps(row, indent=2, sort_keys=True)+'\n')
    return subprocess.CompletedProcess(argv, child.returncode, log.read_bytes()), row


if __name__ == '__main__':
    require(len(sys.argv) >= 4 and sys.argv[1] == '--exec-child', 'closed observed-exec entry required')
    exec_child(int(sys.argv[2]), sys.argv[3:])
