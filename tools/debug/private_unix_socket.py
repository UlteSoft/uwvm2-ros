"""Path-length independent filesystem sockets inside a checked private directory.

No abstract socket, shared alias directory, helper process or process-wide chdir
is used. Credentials therefore still identify the actual connecting process.
"""
import os
from pathlib import Path
import stat
import sys


def _darwin_operation(operation, descriptor, name):
    # Darwin has no /proc/self/fd directory traversal. Use its per-thread cwd
    # on a fresh thread, so neither the caller nor another thread changes cwd.
    # Apple libpthread private.h documents pthread_fchdir_np(-1) as the reset.
    import ctypes
    import threading
    change = ctypes.CDLL(None, use_errno=True).pthread_fchdir_np
    change.argtypes = [ctypes.c_int]
    change.restype = ctypes.c_int
    errors = []

    def checked_change(fd):
        if change(fd) != 0:
            error = ctypes.get_errno()
            raise OSError(error, os.strerror(error))

    def perform():
        try:
            checked_change(descriptor)
            try:
                operation(name)
            finally:
                checked_change(-1)
        except BaseException as error:
            errors.append(error)

    worker = threading.Thread(target=perform, name="uwvm-private-socket")
    worker.start()
    worker.join()
    if errors:
        raise errors[0]


def _operate(operation, path):
    path = Path(path).absolute()
    if path.name in ("", ".", ".."):
        raise ValueError("a private socket filename is required")
    flags = os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW | os.O_CLOEXEC
    descriptor = os.open(path.parent, flags)
    try:
        info = os.fstat(descriptor)
        if not stat.S_ISDIR(info.st_mode) or info.st_uid != os.geteuid() or info.st_mode & 0o077:
            raise RuntimeError("socket directory must be real and owner-only")
        if sys.platform == "linux":
            # The kernel resolves the pinned directory, including after rename.
            # sun_path contains only this bounded fd alias and the fixed leaf.
            operation(f"/proc/self/fd/{descriptor}/{path.name}")
        elif sys.platform == "darwin":
            _darwin_operation(operation, descriptor, path.name)
        else:
            raise RuntimeError("private Unix broker requires Linux or macOS")
    finally:
        os.close(descriptor)


def bind_private(channel, path):
    _operate(channel.bind, path)


def connect_private(channel, path):
    _operate(channel.connect, path)
