"""Public execution boundary. Importing a receipt is not an admission grant.

A host-selected, hash-pinned adapter supplies live admission and the guarded
command wrapper. Its whole dependency/input closure must have been qualified by
the independent executor before this process starts. The runner neither creates
that authority nor acquires its lease. Every real product/tool child inherits
that already admitted leaf and is checked immediately before exec by the gate.
"""
from pathlib import Path
import hashlib
import math
import os
import select
import signal
import subprocess
import time
from typing import Protocol


def need(value, message):
    if not value:
        raise RuntimeError(message)


def sha(path):
    with Path(path).open('rb') as source:
        return hashlib.file_digest(source, 'sha256').hexdigest()


class Executor(Protocol):
    def admit(self, stage: str) -> dict: ...
    def verify_product(self, specification: dict) -> dict: ...
    def command(self, argv: list[str]) -> list[str]: ...
    def finish(self) -> dict: ...


class Child:
    def __init__(self, argv, stdout, stderr, pass_fds=()):
        self.process = subprocess.Popen(argv, stdin=subprocess.DEVNULL, stdout=stdout,
                                        stderr=stderr, pass_fds=pass_fds, start_new_session=True)
        try:
            self.pidfd = os.pidfd_open(self.process.pid, 0)
        except BaseException:
            # The still-unreaped direct child identity cannot be reused.
            self.process.kill(); self.process.wait()
            raise

    def wait(self, seconds):
        deadline = time.monotonic() + seconds
        poller = select.poll(); poller.register(self.pidfd, select.POLLIN)
        while True:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                raise subprocess.TimeoutExpired(self.process.args, seconds)
            events = poller.poll(max(1, math.ceil(remaining * 1000)))
            if any(fd == self.pidfd and flags & select.POLLIN for fd, flags in events):
                return self.process.wait()
            need(not any(flags & (select.POLLERR | select.POLLNVAL) for _, flags in events),
                 'owned pidfd failed')

    def close(self):
        try:
            if self.process.poll() is None:
                signal.pidfd_send_signal(self.pidfd, signal.SIGTERM)
                try:
                    self.wait(5)
                except subprocess.TimeoutExpired:
                    signal.pidfd_send_signal(self.pidfd, signal.SIGKILL)
                    self.wait(5)
        finally:
            os.close(self.pidfd)
