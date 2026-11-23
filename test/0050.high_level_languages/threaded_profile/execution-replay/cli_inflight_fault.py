#!/usr/bin/env python3
"""A real blocked pipe import must not prevent trusted terminal fault handling."""
import argparse
import json
import os
from pathlib import Path
import resource
import signal
import socket
import subprocess
import threading
import time
from cli_fault_actions import arguments


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--options-json", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--proc-exit", action="store_true", help="expect authorized proc_exit(73), not a Wasm trap")
    parser.add_argument("--managed", action="store_true", help="use the genuine authenticated debugger control channel")
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    options = arguments(args.options_json)
    if "--debug-fault-action" in options:
        at = options.index("--debug-fault-action"); del options[at:at + 2]
    runner = arguments(args.runner_json) if args.runner_json else []
    output = args.output.resolve(); output.mkdir(parents=True, exist_ok=False)
    rows = []
    for recording in (True, False):
        for action in (("default",) if args.proc_exit else ("default", "crash", "exit", "pause")):
            label = ("record-" if recording else "ordinary-") + action
            archive = output / (label + ".uwsnap")
            selected = list(options); at = selected.index("--run")
            selected[at:at] = (["--debug-thread-history", "record", str(archive), "4194304"] if recording else []) + (
                [] if action == "default" else ["--debug-fault-action", action])
            command = runner + [str(args.binary.resolve(strict=True))] + selected
            channel = peer = None; messages = []; control = None
            if args.managed:
                channel, peer = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
                peer.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
                channel.settimeout(30)
                command = runner + [str(args.binary.resolve(strict=True)), "--debug-jit-control-fd", str(peer.fileno())] + selected
            (output / (label + ".argv.json")).write_text(json.dumps(command, indent=2))
            read_fd, write_fd = os.pipe()
            start = time.monotonic(); stopped = False; blocked = False; timeout = False
            with (output / (label + ".log")).open("wb") as log:
                child = subprocess.Popen(command, stdin=read_fd, stdout=log, stderr=subprocess.STDOUT,
                                         pass_fds=(peer.fileno(),) if peer else ())
                if peer: peer.close()
                os.close(read_fd)
                if channel:
                    def drive():
                        try:
                            for request in ("status", "continue"):
                                channel.sendall(request.encode()); reply = channel.recv(65536).decode()
                                messages.append({"request": request, "reply": reply})
                                if request == "status":
                                    if "stop-id " not in reply or "threaded-profile" not in reply:
                                        raise AssertionError("missing genuine private entry stop")
                            while child.poll() is None:
                                channel.sendall(b"status"); reply = channel.recv(65536).decode()
                                if not reply: break
                                messages.append({"request": "status", "reply": reply})
                                time.sleep(.002)
                        except (OSError, AssertionError) as error:
                            messages.append({"controlEnd": repr(error)})
                    control = threading.Thread(target=drive, daemon=True); control.start()
                try:
                    while child.poll() is None:
                        if time.monotonic() - start > 25:
                            timeout = True; child.kill(); break
                        try:
                            if not blocked:
                                for task in Path(f"/proc/{child.pid}/task").iterdir():
                                    try:
                                        if "pipe_read" in (task / "wchan").read_text():
                                            blocked = True; break
                                    except OSError:
                                        pass
                            if not stopped:
                                state = next(line for line in Path(f"/proc/{child.pid}/status").read_text().splitlines()
                                             if line.startswith("State:"))
                                if state.split()[1] in ("T", "t"):
                                    stopped = True; os.kill(child.pid, signal.SIGCONT)
                        except (FileNotFoundError, ProcessLookupError):
                            pass
                        time.sleep(0.002)
                    code = child.wait(timeout=5)
                finally:
                    if child.poll() is None:
                        child.kill(); child.wait()
                    os.close(write_fd)
                    if channel:
                        channel.close(); control.join(timeout=2)
                        (output / (label + ".control.json")).write_text(json.dumps(messages, indent=2))
            text = (output / (label + ".log")).read_bytes()
            crashed = code in (-signal.SIGILL, -signal.SIGABRT, -signal.SIGTRAP,
                               128 + signal.SIGILL, 128 + signal.SIGABRT, 128 + signal.SIGTRAP)
            diagnostic = b"[crash]" not in text if args.proc_exit else b"Exit status: 120" in text
            outcome = code == 73 if args.proc_exit else (code == 120 if action == "exit" else crashed)
            passed = (not timeout and diagnostic and not archive.exists()
                      and stopped == (action == "pause") and (blocked or not recording)
                      and outcome and (not args.managed or (messages and "stop-id " in messages[0].get("reply", ""))))
            row = dict(recording=recording, action=action, code=code, blocked_pipe_observed=blocked,
                       stopped=stopped, timeout=timeout, published=archive.exists(), passed=passed,
                       managed=args.managed,
                       seconds=time.monotonic() - start)
            rows.append(row); print(row, flush=True)
            (output / "results.json").write_text(json.dumps(rows, indent=2))
            if not passed:
                print(text[-4000:].decode(errors="replace"), flush=True)
                break
    return int(any(not row["passed"] for row in rows))


if __name__ == "__main__":
    raise SystemExit(main())
