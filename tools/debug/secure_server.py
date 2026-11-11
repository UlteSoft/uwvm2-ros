#!/usr/bin/env python3
"""Host-owned Unix debugger server for an opt-in LLVM-full UWVM guest.

The VM receives only a preauthorized SOCK_SEQPACKET endpoint. This supervisor
owns the other endpoint throughout the run and never passes it to a client.
The socket and capability file live in an owner-only directory outside guest
preopens. A connecting client must present both matching Linux credentials and
the 256-bit launch capability. Packets from the VM's own PID are rejected.
"""

import argparse
import array
import hmac
import math
import os
from pathlib import Path
import secrets
import select
import signal
import socket
import stat
import struct
import subprocess
import sys
import tempfile
import time


MAX_COMMAND = 8448  # VM restricts long input to bounded WASIp1 text edits
MAX_REPLY = 65536
CAPABILITY_BYTES = 32
SOCKET_NAME = "control.sock"
CAPABILITY_NAME = "capability"
_TIMEOUT = object()
_OBSERVER_DISCONNECTED = object()


def is_quit_command(packet):
    """Match the VM's single-token quit grammar, including ASCII whitespace."""
    return packet.strip(b" \t\n\r\v\f") in (b"q", b"quit")


class VMChannelFailure(RuntimeError):
    """The unsequenced VM transport cannot safely serve another client."""


def private_directory(raw):
    if raw is None:
        return Path(tempfile.mkdtemp(prefix="uwvm-debug-")), True
    path = Path(raw).absolute()
    info = path.lstat()
    if not stat.S_ISDIR(info.st_mode) or info.st_uid != os.geteuid() or info.st_mode & 0o077:
        raise RuntimeError("server directory must be a real, owner-only directory")
    return path, False


def reject_guest_preopen_overlap(directory, vm_args):
    """Keep the capability outside every explicit WASI Preview 1 host mount."""
    mount_flags = {
        "--wasip1-mount-dir": 2, "--wasip1-global-mount-dir": 2, "-I1dir": 2,
        "--wasip1-single-mount-dir": 3, "-I1Sdir": 3,
        "--wasip1-group-mount-dir": 3, "-I1Gdir": 3,
    }
    secret_directory = directory.resolve(strict=True)
    for index, arg in enumerate(vm_args):
        offset = mount_flags.get(arg)
        if offset is None:
            continue
        if index + offset >= len(vm_args):
            raise RuntimeError("incomplete WASI mount option")
        host_mount = Path(vm_args[index + offset]).resolve(strict=True)
        if secret_directory == host_mount or host_mount in secret_directory.parents:
            raise RuntimeError("debugger capability directory overlaps a guest WASI preopen")


def send_packet(channel, data):
    if channel.send(data, socket.MSG_NOSIGNAL) != len(data):
        raise RuntimeError("short debugger packet")


def peer_alive(pidfd):
    watched = __import__("select").poll()
    watched.register(pidfd, __import__("select").POLLIN)
    return not watched.poll(0)


def checked_packet(channel, capacity, identity=None, pidfd=None):
    """Receive one bounded packet, rejecting rights and credential changes."""
    ancillary_capacity = socket.CMSG_SPACE(struct.calcsize("3i")) + socket.CMSG_SPACE(16 * struct.calcsize("i"))
    try:
        data, ancillary, flags, _ = channel.recvmsg(capacity, ancillary_capacity, socket.MSG_CMSG_CLOEXEC)
    except socket.timeout:
        return _TIMEOUT
    credentials = []
    unexpected = False
    for level, kind, value in ancillary:
        if level == socket.SOL_SOCKET and kind == socket.SCM_CREDENTIALS and len(value) == struct.calcsize("3i"):
            credentials.append(struct.unpack("3i", value))
        else:
            unexpected = True
            if level == socket.SOL_SOCKET and kind == socket.SCM_RIGHTS:
                rights = array.array("i")
                rights.frombytes(value[:len(value) - len(value) % rights.itemsize])
                for descriptor in rights:
                    os.close(descriptor)
    if flags & (socket.MSG_TRUNC | socket.MSG_CTRUNC) or unexpected:
        raise RuntimeError("truncated packet or unexpected ancillary data")
    # SOCK_SEQPACKET can deliver SCM_RIGHTS with a zero-byte payload. Drain
    # and close ancillary rights above before interpreting that payload as EOF;
    # returning early here leaked one broker FD per hostile empty packet.
    if not data:
        return None
    if identity is not None:
        if len(credentials) != 1 or credentials[0] != identity or not peer_alive(pidfd):
            raise RuntimeError("debugger client identity changed")
    elif credentials:
        raise RuntimeError("unexpected credentials on VM response")
    return data


def accept_client(listener, guest_pid):
    try:
        client, _ = listener.accept()
    except socket.timeout:
        return None
    try:
        raw = client.getsockopt(socket.SOL_SOCKET, socket.SO_PEERCRED, struct.calcsize("3i"))
        identity = struct.unpack("3i", raw)
        if identity[0] <= 0 or identity[0] == guest_pid or identity[1] != os.geteuid() or identity[2] != os.getegid():
            raise RuntimeError("unauthorized debugger peer")
        pidfd = os.pidfd_open(identity[0], 0)
        if not peer_alive(pidfd):
            os.close(pidfd)
            raise RuntimeError("debugger peer exited")
        client.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        client.settimeout(0.5)
        return client, identity, pidfd
    except BaseException:
        client.close()
        raise


def finished_guest_reply(guest, packet, *, wait=False):
    # A transport EOF is not an exit result. Only the original Popen OS wait
    # can answer an authenticated terminal status/wait; never acknowledge an
    # uncertain mutation, replay a packet, or reuse the unsequenced endpoint.
    if packet not in (b"status", b"wait"):
        return None
    code = guest.poll()
    if code is None and wait:
        try:
            code = guest.wait(timeout=2)
        except subprocess.TimeoutExpired:
            return None
    return None if code is None else f"guest exited: {code}\n".encode("ascii")



def startup_timeout_seconds(value):
    value = float(value)
    if not math.isfinite(value) or value <= 0:
        raise ValueError("startup timeout must be a positive finite number of seconds")
    return value


def wait_startup_reply(client, identity, pidfd, vm_channel, guest, packet, timeout=None, *, control_allowed=True):
    """One authenticated command: actual VM reply, exit, or client cancellation.

    Compilation has no default wall-clock deadline. A full-control client may
    cancel the launch. An observer only cancels its own connection: retain the
    original VM owner and drain this single outstanding reply before another
    client can use the unsequenced endpoint. An explicit host deadline and an
    actual VM/transport failure retain their independent launcher semantics.
    """
    deadline = None if timeout is None else time.monotonic() + timeout
    if guest.poll() is not None:
        reply = finished_guest_reply(guest, packet)
        if reply is not None:
            return reply
        raise VMChannelFailure("original VM exited before startup command completed")
    guest_fd = os.pidfd_open(guest.pid, 0)
    observing = True
    try:
        watched = select.poll()
        events = select.POLLIN | select.POLLERR | select.POLLHUP | select.POLLNVAL
        for fd in (client.fileno(), pidfd, vm_channel.fileno(), guest_fd):
            watched.register(fd, events)
        while True:
            remaining = None if deadline is None else max(0.0, deadline - time.monotonic())
            # poll(2)'s signed millisecond argument bounds one wait, not the
            # user's deadline or total compilation duration.
            milliseconds = None if remaining is None else math.ceil(min(2147483.647, remaining) * 1000)
            ready = dict(watched.poll(milliseconds))
            if not ready:
                if deadline is not None and time.monotonic() >= deadline:
                    raise VMChannelFailure("VM debugger startup exceeded the explicit host deadline")
                continue
            if observing and (pidfd in ready or client.fileno() in ready):
                if control_allowed:
                    if client.fileno() in ready:
                        pending = checked_packet(client, MAX_COMMAND, identity, pidfd)
                        if pending not in (None, b"disconnect"):
                            raise VMChannelFailure("another debugger packet arrived before startup completed")
                    raise VMChannelFailure("debugger client canceled VM startup")
                # A bad/disconnected observer has no authority to terminate
                # the Guest. Consume bounded client ancillary data if present,
                # then stop watching it while the already-sent VM reply drains.
                if client.fileno() in ready:
                    try:
                        checked_packet(client, MAX_COMMAND, identity, pidfd)
                    except (OSError, RuntimeError):
                        pass
                watched.unregister(client.fileno())
                watched.unregister(pidfd)
                observing = False
                client.close()
            if vm_channel.fileno() in ready:
                reply = checked_packet(vm_channel, MAX_REPLY)
                if reply is not None and reply is not _TIMEOUT:
                    return reply if observing else _OBSERVER_DISCONNECTED
                reply = finished_guest_reply(guest, packet, wait=True)
                if reply is not None:
                    return reply if observing else _OBSERVER_DISCONNECTED
                raise VMChannelFailure("VM debugger control channel closed before original guest exit")
            if guest_fd in ready:
                reply = finished_guest_reply(guest, packet)
                if reply is not None:
                    return reply if observing else _OBSERVER_DISCONNECTED
                raise VMChannelFailure("original VM exited before startup command completed")
    finally:
        os.close(guest_fd)


def serve_client(client, identity, pidfd, vm_channel, capability, guest, startup_pending=None, startup_timeout=None, *, control_allowed=True):
    deadline = time.monotonic() + 10
    while guest.poll() is None and time.monotonic() < deadline:
        packet = checked_packet(client, CAPABILITY_BYTES, identity, pidfd)
        if packet is _TIMEOUT:
            continue
        if packet is None or not hmac.compare_digest(packet, capability):
            raise RuntimeError("invalid debugger capability")
        send_packet(client, b"ready\n")
        break
    else:
        raise RuntimeError("debugger authentication timed out")

    idle = time.monotonic() + 30
    terminal_deadline = None
    while True:
        packet = checked_packet(client, MAX_COMMAND, identity, pidfd)
        if packet is _TIMEOUT:
            if guest.poll() is not None or time.monotonic() >= idle:
                return False
            continue
        if packet is None or packet == b"disconnect":
            return False
        if is_quit_command(packet) and not control_allowed:
            # This endpoint belongs to the launcher, not to an observation
            # client. In the VM grammar quit closes that endpoint; forwarding
            # it would let an observer disable later authenticated sessions.
            send_packet(client, b"observer detached; guest state unchanged\n")
            return False
        if guest.poll() is not None:
            if terminal_deadline is None:
                terminal_deadline = time.monotonic() + 2
            if time.monotonic() >= terminal_deadline:
                return False
            reply = finished_guest_reply(guest, packet)
            if reply is not None:
                send_packet(client, reply)
                continue  # DAP may query status before its explicit terminal wait.
            return False
        idle = time.monotonic() + 30
        # The VM independently parses the bounded grammar and checks state.
        # Full JIT compilation precedes control-channel adoption. Only the
        # first authenticated command may wait for actual startup. Its request
        # is never renewed/replayed; a canceled observer's reply is drained.
        startup = startup_pending is not None and startup_pending[0]
        if startup:
            startup_pending[0] = False
        vm_channel.settimeout(6)
        try:
            send_packet(vm_channel, packet)
            reply = (wait_startup_reply(client, identity, pidfd, vm_channel, guest, packet,
                                        startup_timeout if startup else None,
                                        control_allowed=control_allowed)
                     if startup or not control_allowed else checked_packet(vm_channel, MAX_REPLY))
            if reply is _OBSERVER_DISCONNECTED:
                return False
            if reply is None:
                reply = finished_guest_reply(guest, packet, wait=True)
                if reply is None:
                    raise VMChannelFailure("VM debugger control channel closed before original guest exit")
            elif reply is _TIMEOUT:
                raise VMChannelFailure("VM debugger control channel timed out")
        except (BrokenPipeError, ConnectionResetError) as error:
            reply = finished_guest_reply(guest, packet, wait=True)
            if reply is None:
                raise VMChannelFailure(str(error)) from error
        except (OSError, RuntimeError) as error:
            # The packet may already have executed. A late reply must never
            # answer the next client's command on this unsequenced endpoint.
            raise VMChannelFailure(str(error)) from error
        finally:
            vm_channel.settimeout(6)
        send_packet(client, reply)
        if is_quit_command(packet):
            return True  # The authenticated host intentionally detached.
    return False


def serve(arguments):
    vm_args = list(arguments.vm_args)
    if vm_args and vm_args[0] == "--":
        vm_args.pop(0)
    from fault_action import validate_fault_action, host_options, selected_debug_permissions
    validate_fault_action(vm_args, RuntimeError)
    options = host_options(vm_args, RuntimeError)
    selected = selected_debug_permissions(vm_args, RuntimeError)
    update_directory = getattr(arguments, "update_socket_dir", None)
    if update_directory is not None and "--update-control-fd" in options:
        raise RuntimeError("the host update relay owns its private control descriptor")
    if selected == "none":
        # No debugger socket, capability file, adapter or control descriptor.
        # Independent deployment authority may still be explicitly provisioned.
        if update_directory is None:
            return subprocess.run([str(arguments.uwvm), *vm_args], check=False).returncode
        from signed_update import Relay
        relay = Relay(update_directory, vm_args)
        guest = None
        try:
            guest = subprocess.Popen([str(arguments.uwvm), *relay.launch_arguments(), *vm_args],
                                     pass_fds=(relay.child_endpoint.fileno(),))
            relay.start(guest)
            return guest.wait()
        finally:
            relay.close()
            if guest is not None and guest.poll() is None:
                guest.terminate()
                try: guest.wait(timeout=5)
                except subprocess.TimeoutExpired: guest.kill(); guest.wait()
    directory, remove_directory = private_directory(arguments.socket_dir)
    try:
        if not vm_args:
            raise RuntimeError("guest command after -- is required")
        reject_guest_preopen_overlap(directory, vm_args)
    except BaseException:
        if remove_directory:
            directory.rmdir()
        raise
    socket_path = directory / SOCKET_NAME
    capability_path = directory / CAPABILITY_NAME
    if len(os.fsencode(socket_path)) >= 108:
        raise RuntimeError("Unix socket path exceeds the Linux sockaddr_un limit")
    capability = secrets.token_bytes(CAPABILITY_BYTES)
    flags = os.O_WRONLY | os.O_CREAT | os.O_EXCL | getattr(os, "O_NOFOLLOW", 0)
    descriptor = os.open(capability_path, flags, 0o600)
    try:
        os.write(descriptor, capability)
        os.fsync(descriptor)
    finally:
        os.close(descriptor)
    listener = socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET)
    guest = None
    supervisor = None
    detached = False
    socket_bound = False
    relay = None
    try:
        if update_directory is not None:
            from signed_update import Relay
            relay = Relay(update_directory, vm_args)
        # Accepted sockets inherit this before a fast peer can queue its token.
        # Enabling it after accept loses credentials on already queued packets.
        listener.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        listener.bind(str(socket_path))
        socket_bound = True
        os.chmod(socket_path, 0o600)
        listener.listen(2)
        listener.settimeout(0.25)
        supervisor, vm_endpoint = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        try:
            # Authenticate even the first command queued before the VM adopts
            # its inherited endpoint. Linux attaches sender credentials when
            # the packet is sent; enabling PASSCRED later loses that identity.
            vm_endpoint.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
            update_args = [] if relay is None else relay.launch_arguments()
            command = [str(arguments.uwvm), "--debug-jit-control-fd", str(vm_endpoint.fileno()), *update_args, *vm_args]
            inherited = (vm_endpoint.fileno(),) if relay is None else (vm_endpoint.fileno(), relay.child_endpoint.fileno())
            guest = subprocess.Popen(command, pass_fds=inherited)
            if relay is not None: relay.start(guest)
        finally:
            vm_endpoint.close()
        supervisor.settimeout(6)
        startup_pending = [True]
        print(f"debug server: {socket_path}", flush=True)
        print(f"client: {Path(__file__).name} connect --socket-dir {directory}", flush=True)
        while guest.poll() is None:
            try:
                accepted = accept_client(listener, guest.pid)
            except (OSError, RuntimeError) as error:
                print(f"debug client rejected: {error}", file=sys.stderr, flush=True)
                continue
            if accepted is None:
                continue
            client, identity, pidfd = accepted
            try:
                with client:
                    detached = serve_client(client, identity, pidfd, supervisor, capability, guest, startup_pending,
                                            getattr(arguments, "startup_timeout", None), control_allowed=selected == "full")
            except VMChannelFailure:
                raise  # retire this original launch through serve()'s finally
            except (OSError, RuntimeError) as error:
                print(f"debug client disconnected: {error}", file=sys.stderr, flush=True)
            finally:
                os.close(pidfd)
            if detached:
                break
        supervisor.close()
        if detached:
            print("debugger detached; guest continues without a control endpoint", flush=True)
            return 0 if relay is None else guest.wait()
        return guest.wait()
    finally:
        if relay is not None: relay.close()
        if supervisor is not None:
            supervisor.close()
        listener.close()
        try:
            if socket_bound:
                socket_path.unlink(missing_ok=True)
        finally:
            capability_path.unlink(missing_ok=True)
        if remove_directory:
            directory.rmdir()
        if guest is not None and not detached and guest.poll() is None:
            guest.terminate()
            try:
                guest.wait(timeout=5)
            except subprocess.TimeoutExpired:
                guest.kill()
                guest.wait()


def connect(arguments):
    directory, _ = private_directory(arguments.socket_dir)
    path = directory / CAPABILITY_NAME
    flags = os.O_RDONLY | getattr(os, "O_NOFOLLOW", 0)
    descriptor = os.open(path, flags)
    try:
        info = os.fstat(descriptor)
        if not stat.S_ISREG(info.st_mode) or info.st_uid != os.geteuid() or info.st_mode & 0o077:
            raise RuntimeError("debugger capability file is not private")
        capability = os.read(descriptor, CAPABILITY_BYTES + 1)
        if len(capability) != CAPABILITY_BYTES:
            raise RuntimeError("invalid debugger capability size")
    finally:
        os.close(descriptor)
    with socket.socket(socket.AF_UNIX, socket.SOCK_SEQPACKET) as channel:
        channel.settimeout(8)
        channel.connect(str(directory / SOCKET_NAME))
        send_packet(channel, capability)
        if checked_packet(channel, MAX_REPLY) != b"ready\n":
            raise RuntimeError("debugger authorization rejected")
        first_request = True
        for command in arguments.commands:
            encoded = command.encode("ascii")
            if not 0 < len(encoded) <= MAX_COMMAND:
                raise RuntimeError("command exceeds 8448 bytes")
            if encoded == b"disconnect":
                return 0
            channel.settimeout(getattr(arguments, "startup_timeout", None) if first_request else 8)
            first_request = False
            send_packet(channel, encoded)
            reply = checked_packet(channel, MAX_REPLY)
            if reply is None or reply is _TIMEOUT:
                raise RuntimeError("debugger server closed")
            sys.stdout.buffer.write(reply)
            sys.stdout.buffer.flush()
        if arguments.commands:
            return 0
        while True:
            try:
                line = input("(uwvm-debug-server) ")
            except EOFError:
                return 0
            encoded = line.encode("ascii")
            if not 0 < len(encoded) <= MAX_COMMAND:
                print("error: command must contain 1..8448 ASCII bytes")
                continue
            if encoded == b"disconnect":
                return 0
            channel.settimeout(getattr(arguments, "startup_timeout", None) if first_request else 8)
            first_request = False
            send_packet(channel, encoded)
            reply = checked_packet(channel, MAX_REPLY)
            if reply is None or reply is _TIMEOUT:
                raise RuntimeError("debugger server closed")
            sys.stdout.buffer.write(reply)
            sys.stdout.buffer.flush()
            if encoded == b"quit":
                return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    subcommands = parser.add_subparsers(dest="mode", required=True)
    host = subcommands.add_parser("serve", help="launch a VM with an authenticated control server")
    host.add_argument("--uwvm", required=True, type=Path)
    host.add_argument("--socket-dir", type=Path, help="existing owner-only directory; default is a fresh private directory")
    host.add_argument("--update-socket-dir", type=Path, help="independent owner-only signed-update relay; requires VM key/contract/scope; available with every debugger permission, including none")
    host.add_argument("--startup-timeout", type=startup_timeout_seconds, help="optional host deadline for the first VM reply; by default an observer cancellation drains that reply and preserves the guest")
    host.add_argument("vm_args", nargs=argparse.REMAINDER)
    client = subcommands.add_parser("connect", help="send commands through a running host server")
    client.add_argument("--socket-dir", required=True, type=Path)
    client.add_argument("--command", dest="commands", action="append", default=[])
    client.add_argument("--startup-timeout", type=startup_timeout_seconds, help="optional first-reply deadline in seconds")
    arguments = parser.parse_args()
    if sys.platform != "linux" or not hasattr(os, "pidfd_open"):
        parser.error("secure debugger server requires Linux pidfd support")
    if arguments.mode == "serve":
        # A requested shutdown must run the socket/capability and child cleanup
        # in serve()'s finally block, including when a terminal sends SIGTERM.
        def stop_on_signal(_number, _frame):
            raise KeyboardInterrupt
        signal.signal(signal.SIGTERM, stop_on_signal)
    try:
        return serve(arguments) if arguments.mode == "serve" else connect(arguments)
    except KeyboardInterrupt:
        return 130
    except (OSError, RuntimeError, UnicodeEncodeError) as error:
        print(f"debug server: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
