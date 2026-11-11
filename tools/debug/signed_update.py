#!/usr/bin/env python3
"""Independent host relay for the existing signed UUP1 update protocol.

No signing key, implicit launch/commit, retry or debugger execution control is
provided. Client disconnect does not cancel a submitted decision: its original
VM reply is drained before another client may use the coordinator. The VM still
verifies every permit, epoch, nonce, scope, contract and real execution drain.
"""
import argparse
import array
import hmac
import json
import os
from pathlib import Path
import secrets
import socket
import stat
import struct
import sys
import threading

# Exact current protocol.h / transition.h framing bounds, not a guest quota.
PERMIT_BYTES = 32 + 8 + 10 * 32 + 6 * 8 + 64
MAX_PAYLOAD = 64 * 1024 * 1024 + 72 + 64 * 8 + 65536 + 3600 + PERMIT_BYTES + 4
MAX_REPLY_BODY = 296 + 32 + 4 * 8 + 4 + 65536
SOCKET_NAME = "update.sock"
CAPABILITY_NAME = "update-capability"
STATUS = ("ok", "unsupported", "invalid_state", "invalid_input", "unauthorized", "expired", "stale_base",
          "scope_mismatch", "busy", "exhausted", "validation_failed", "compile_failed", "committed_but_not_active")
PHASE = ("disabled", "active", "preparing", "prepared", "waiting_for_decision", "committed", "committed_failed",
         "aborted", "awaiting_launch", "guest_fault", "process_exit_pending")


def receive(channel, size, identity=None, pidfd=None):
    from secure_server import peer_alive
    data = bytearray()
    while len(data) < size:
        if pidfd is not None and not peer_alive(pidfd):
            raise RuntimeError("original update peer exited")
        packet, ancillary, flags, _ = channel.recvmsg(min(size-len(data), 65536),
            socket.CMSG_SPACE(12) + socket.CMSG_SPACE(16 * array.array("i").itemsize), socket.MSG_CMSG_CLOEXEC)
        credentials = []; unexpected = False
        for level, kind, value in ancillary:
            if level == socket.SOL_SOCKET and kind == socket.SCM_CREDENTIALS and len(value) == 12:
                credentials.append(struct.unpack("3i", value))
            else:
                unexpected = True
                if level == socket.SOL_SOCKET and kind == socket.SCM_RIGHTS:
                    rights = array.array("i"); rights.frombytes(value[:len(value)-len(value)%rights.itemsize])
                    for descriptor in rights: os.close(descriptor)
        if unexpected or flags & (socket.MSG_TRUNC | socket.MSG_CTRUNC):
            raise RuntimeError("unexpected update ancillary data")
        if not packet:
            raise EOFError("update channel closed; no decision inferred")
        if identity is not None and (credentials != [identity] or not peer_alive(pidfd)):
            raise RuntimeError("update sender identity changed")
        if identity is None and credentials:
            raise RuntimeError("unexpected update response credentials")
        data.extend(packet)
    return bytes(data)


def request_header(header):
    if len(header) != 12 or header[:4] != b"UUP1":
        raise ValueError("complete versioned UUP1 request required")
    operation, size = struct.unpack("<II", header[4:])
    if operation > 9 or size > MAX_PAYLOAD:
        raise ValueError("update request exceeds the native protocol")
    return size


def response(channel, identity=None, pidfd=None):
    header = receive(channel, 8, identity, pidfd)
    if header[:4] != b"UUR1": raise RuntimeError("invalid update reply version")
    size = struct.unpack("<I", header[4:])[0]
    if not 296 <= size <= MAX_REPLY_BODY: raise RuntimeError("invalid bounded update reply")
    return header + receive(channel, size, identity, pidfd)


def describe(packet):
    status, phase, active, connected = struct.unpack_from("<IIII", packet, 8)
    if status >= len(STATUS) or phase >= len(PHASE) or active > 1 or connected > 1:
        raise RuntimeError("invalid update status facts")
    epoch, committed, sequence = struct.unpack_from("<QQQ", packet, 24)
    return {"status": STATUS[status], "statusCode": status, "phase": PHASE[phase], "active": bool(active),
            "coordinatorConnected": bool(connected), "installedEpoch": str(epoch), "committedEpoch": str(committed),
            "decisionSequence": str(sequence), "replyHex": packet.hex(), "authority": "independent signed update coordinator"}


def read_frame(path):
    descriptor = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode) or not 12 <= before.st_size <= 12+MAX_PAYLOAD:
            raise ValueError("update frame must be a bounded regular file")
        with os.fdopen(os.dup(descriptor), "rb") as source: data = source.read(13+MAX_PAYLOAD)
        after = os.fstat(descriptor)
        if ((before.st_size,before.st_mtime_ns,before.st_ctime_ns) != (after.st_size,after.st_mtime_ns,after.st_ctime_ns)
                or len(data) != before.st_size or request_header(data[:12]) != len(data)-12):
            raise ValueError("update frame changed or has trailing/incomplete bytes")
        return data
    finally: os.close(descriptor)


def submit(directory, frame):
    from secure_server import private_directory
    directory, _ = private_directory(directory)
    if request_header(frame[:12]) != len(frame)-12: raise ValueError("incomplete update frame")
    descriptor = os.open(directory/CAPABILITY_NAME, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        info = os.fstat(descriptor)
        if not stat.S_ISREG(info.st_mode) or info.st_uid != os.geteuid() or info.st_mode & 0o077:
            raise RuntimeError("update capability must be owner-only")
        token = os.read(descriptor, 33)
        if len(token) != 32: raise RuntimeError("invalid update capability size")
    finally: os.close(descriptor)
    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as channel:
        channel.settimeout(10); channel.connect(str(directory/SOCKET_NAME))
        channel.sendall(token)
        if receive(channel, 6) != b"ready\n": raise RuntimeError("update authorization refused")
        # Compilation is not bounded by an IDE timeout. There is exactly one
        # submitted frame and one result; a failed connection must not retry it.
        channel.settimeout(None); channel.sendall(frame)
        return describe(response(channel))


class Relay:
    def __init__(self, directory, vm_args):
        from secure_server import private_directory, reject_guest_preopen_overlap
        self.directory, self.remove_directory = private_directory(directory)
        reject_guest_preopen_overlap(self.directory, vm_args)
        self.path = self.directory/SOCKET_NAME; self.cap_path = self.directory/CAPABILITY_NAME
        self.listener = self.vm = self.child_endpoint = self.thread = self.guest = None
        self.bound = self.owned_cap = False; self.stopping = threading.Event(); self.error = None
        if len(os.fsencode(self.path)) >= 108: raise RuntimeError("update socket exceeds sockaddr_un")
        try:
            self.capability = secrets.token_bytes(32)
            fd = os.open(self.cap_path, os.O_WRONLY|os.O_CREAT|os.O_EXCL|os.O_NOFOLLOW|os.O_CLOEXEC, 0o600)
            self.owned_cap = True
            try: os.write(fd, self.capability); os.fsync(fd)
            finally: os.close(fd)
            self.listener = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            self.listener.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
            self.listener.bind(str(self.path)); self.bound = True
            os.chmod(self.path, 0o600); self.listener.listen(2); self.listener.settimeout(.25)
            self.vm, self.child_endpoint = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)
            self.vm.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
            self.child_endpoint.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        except BaseException: self.close(); raise

    def launch_arguments(self):
        return ["--update-control-fd", str(self.child_endpoint.fileno())]

    def start(self, guest):
        self.guest = guest; self.child_endpoint.close(); self.child_endpoint = None
        self.thread = threading.Thread(target=self.run, name="uwvm-signed-update", daemon=True); self.thread.start()
        print(f"update server: {self.path}", flush=True)

    def run(self):
        from secure_server import accept_client
        guest_fd = os.pidfd_open(self.guest.pid, 0)
        vm_identity = (self.guest.pid, os.geteuid(), os.getegid())
        try:
            while not self.stopping.is_set() and self.guest.poll() is None:
                try: accepted = accept_client(self.listener, self.guest.pid)
                except (OSError, RuntimeError):
                    if self.stopping.is_set(): break
                    continue
                if accepted is None: continue
                client, identity, pidfd = accepted
                try:
                    with client:
                        client.settimeout(10)
                        if not hmac.compare_digest(receive(client, 32, identity, pidfd), self.capability):
                            raise RuntimeError("invalid independent update capability")
                        client.sendall(b"ready\n")
                        header = receive(client, 12, identity, pidfd)
                        data = header + receive(client, request_header(header), identity, pidfd)
                        # No client can occupy the VM stream with a partial frame.
                        # Once sent, always drain the actual reply, even after EOF.
                        try:
                            self.vm.sendall(data)
                            packet = response(self.vm, vm_identity, guest_fd)
                            describe(packet)
                        except (OSError, RuntimeError, EOFError, ValueError) as error:
                            self.error = str(error); self.vm.shutdown(socket.SHUT_RDWR); return
                        client.sendall(packet)
                except (OSError, RuntimeError, EOFError, ValueError):
                    pass  # Client failures have no guest execution authority.
                finally: os.close(pidfd)
        finally: os.close(guest_fd)

    def close(self):
        self.stopping.set()
        for value in (self.listener, self.vm, self.child_endpoint):
            if value is not None:
                try: value.shutdown(socket.SHUT_RDWR)
                except OSError: pass
                value.close()
        if self.thread is not None and self.thread is not threading.current_thread(): self.thread.join(timeout=12)
        if self.bound: self.path.unlink(missing_ok=True); self.bound = False
        if self.owned_cap: self.cap_path.unlink(missing_ok=True); self.owned_cap = False
        if self.remove_directory and self.directory.exists(): self.directory.rmdir()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    request = commands.add_parser("request", help="forward one host-authorized UUP1 frame; no retry")
    request.add_argument("--socket-dir", type=Path, required=True)
    request.add_argument("--frame", type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(submit(args.socket_dir, read_frame(args.frame)), indent=2))


if __name__ == "__main__":
    try: main()
    except (OSError, RuntimeError, EOFError, ValueError) as error:
        print("signed update: " + str(error) + "; outcome may be unknown; never retry a decision blindly", file=sys.stderr)
        raise SystemExit(1)
