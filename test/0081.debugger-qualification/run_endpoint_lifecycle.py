#!/usr/bin/env python3
"""Actual late management and detach, including a live native trap and peer refusal.

No VM/compiler runs are permitted on macOS. The parent keeper owns the outer
Linux lock/cgroup/wall deadline. Launch authorization is inherited at exec;
this is not arbitrary PID attachment or endpoint reattachment after retirement.
"""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import re
import resource
import socket
import subprocess
import sys

sys.dont_write_bytecode = True
from qualification_support import admission, build_proof, packet_pins, require, sha


def position(reply):
    stop = re.findall(rb"(?m)^stop-id ([0-9]+)\r?$", reply)
    members = re.findall(rb"(?m)^thread ([0-9]+) module=([0-9]+) function=([0-9]+) byte-offset=([0-9]+) generation=([0-9]+)\r?$", reply)
    require(b"stopped:" in reply and len(stop) == 1 and len(members) == 1,
            "one genuine stopped participant required", reply)
    thread, module, function, offset, epoch = map(int, members[0])
    require(thread > 0 and module == 0 and epoch > 0 and int(stop[0]) > 0,
            "invalid actual stop identity", reply)
    native = re.findall(rb"native-pc=0x([0-9a-fA-F]+)", reply)
    require(len(native) <= 1, "ambiguous native trap", reply)
    return dict(thread=thread, module=module, function=function, offset=offset,
                epoch=epoch, stop=int(stop[0]), native_pc=int(native[0], 16) if native else None)


def memory_words(reply, adapter):
    data = adapter.parse_wasm_memory_page(reply.decode("ascii"), 0, 0, 0, 12)
    require(len(data) == 12, "fixed result slot extent", reply)
    return [int.from_bytes(data[index:index + 4], "little") for index in (0, 4, 8)]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("source-root", "uwvm", "build-receipt", "wasm-tools", "out"):
        parser.add_argument("--" + name, required=True, type=Path)
    parser.add_argument("--ros", action="store_true")
    parser.add_argument("--policy", choices=("instruction", "unwind", "both"), default="both",
                        help="upstream only; ROS has mandatory checked native unwind and no public stack option")
    parser.add_argument("--case", action="append", choices=("quit-wasm", "eof-wasm", "quit-native", "eof-native", "foreign-peer"))
    args = parser.parse_args()
    require(not args.ros or args.policy == "both", "ROS exposes no selectable unwind policy")
    require(sys.platform == "linux" and os.uname().machine == "x86_64",
            "this native-trap qualification requires Linux x86-64")
    root = args.source_root.resolve(strict=True)
    packet = Path(__file__).resolve().parent
    require(packet == root / "test/0081.debugger-qualification", "runner must reside in reviewed source tree")
    before_admission = admission(root)
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    binary = args.uwvm.resolve(strict=True)
    proof = build_proof(root, binary, args.build_receipt)
    # Reuse the maintained real original-Popen/pidfd owner and Wasm parser.
    sys.path[:0] = [str(root / "test/0018.debugger"), str(root / "test/0017.runtime")]
    from run_late_control_proc_exit import Child
    import run_debug_source_inline_metadata_cli as metadata
    import importlib.util
    spec = importlib.util.spec_from_file_location("qualified_endpoint_dap", root / "tools/debug/dap_adapter.py")
    adapter = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(adapter)
    tools = args.wasm_tools.absolute()
    tools.resolve(strict=True)
    pins = packet_pins(packet)
    for path in (binary, args.build_receipt.resolve(strict=True), tools,
                 root / "test/0018.debugger/run_late_control_proc_exit.py", Path(metadata.__file__),
                 root / "tools/debug/dap_adapter.py",
                 *(Path(value) for value in proof["fingerprint_paths"])):
        pins[str(path)] = sha(path)
    out = args.out.absolute()
    out.mkdir(parents=True, exist_ok=False)
    result = dict(passed=False, scope=__doc__, proof={key: value for key, value in proof.items() if key != "production"}, inputs_before=pins,
                  admission_before=before_admission, rows=[], producers=[],
                  concurrent_guest_workers_qualified=False, arbitrary_pid_attach_qualified=False)

    def save():
        (out / "summary.json").write_text(json.dumps(result, indent=2) + "\n")

    try:
        wat, wasm = packet / "fixtures/endpoint_lifecycle.wat", out / "endpoint_lifecycle.wasm"
        for name, command in (("parse", [str(tools), "parse", str(wat), "-o", str(wasm)]),
                              ("validate", [str(tools), "validate", str(wasm)])):
            with (out / (name + ".log")).open("wb") as log:
                child = Child(command, log, cwd=root)
                try:
                    code = child.wait(30)
                    result["producers"].append(dict(argv=command, returncode=code))
                    require(code == 0, "official fixture tool failed", command)
                finally:
                    child.close()
        pins[str(wasm)] = sha(wasm)
        start, body = metadata.function(wasm, "_start")
        groups, expression = metadata.u32(body, 0, len(body))
        require(groups == 0 and body[expression:expression + 4] == b"\x03\x40\x41\x00",
                "official fixture recurring loop point differs", body)
        cases = args.case or ("quit-wasm", "eof-wasm", "quit-native", "eof-native", "foreign-peer")
        require(len(cases) == len(set(cases)), "duplicate cases would overwrite evidence")
        policies = (("mandatory-native",) if args.ros else
                    ("instruction", "unwind") if args.policy == "both" else (args.policy,))
        for policy in policies:
            for kind in cases:
                row = dict(case=kind, policy=policy, passed=False, actions=[])
                result["rows"].append(row)
                parent, inherited = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
                inherited.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
                parent.settimeout(120)
                child = None
                try:
                    mode = ["-Raot"] if args.ros else ["-Rcc", "jit", "-Rcm", "full"]
                    stack = [] if args.ros else ["-Rllvm-call-stack", policy]
                    command = [str(binary), "--debug-jit-control-fd", str(inherited.fileno()), "-m", "run",
                               *mode, "-Rct", "0", *stack,
                               "-Rllvm-exception-dispatch", "native-unwind", "-Rllvm-cache-path", "disable", "--run", str(wasm)]
                    row["actual_argv"] = command
                    with (out / (policy + "-" + kind + ".guest.log")).open("wb") as log:
                        child = Child(command, log, cwd=root, stdin=subprocess.DEVNULL,
                                      pass_fds=(inherited.fileno(),))
                        inherited.close()

                        def ask(text, error=False):
                            wire = text.encode("ascii")
                            require(0 < len(wire) <= 8448, "bounded real control packet")
                            require(parent.send(wire) == len(wire), "whole seqpacket write")
                            reply, _, flags, _ = parent.recvmsg(65536)
                            require(reply and not flags & (socket.MSG_TRUNC | socket.MSG_CTRUNC),
                                    "bounded live endpoint reply", reply)
                            row["actions"].append(dict(command=text, reply=reply.decode(errors="replace")))
                            require(error or b"error:" not in reply, "actual endpoint command rejected", (text, reply))
                            save()
                            return reply

                        row["running_before_attach"] = ask("status").decode(errors="replace")
                        ask("pause")
                        point = ask(f"breakwasm 0 {start} 2")
                        match = re.search(rb"breakpoint ([0-9]+)", point)
                        require(match is not None, "actual recurring breakpoint missing", point)
                        point_id = int(match[1])
                        # Two genuine visits allow the first loop to store its
                        # checked result even if management arrived at startup.
                        for _ in range(3):
                            ask("continue")
                            stopped = ask("wait")
                            require(b"stopped: breakpoint" in stopped, "real recurring breakpoint was not reached", stopped)
                            at = position(ask("status"))
                            require((at["function"], at["offset"]) == (start, 2), "breakpoint owner differs", at)
                            words = memory_words(ask("memory 0 0 0 12"), adapter)
                            if words[0] > 0 and words[1:] == [1, 12]:
                                break
                        else:
                            raise RuntimeError(("guest did not execute before late management", words))
                        row["actual_mid_execution_stop"] = at
                        row["guest_counter_gate_result"] = words
                        ask(f"delete {point_id}")
                        ask(f"set wasm memory 0 0 {at['thread']} 4 bytes 00")
                        armed = position(ask("status"))
                        require(armed["thread"] == at["thread"] and armed["stop"] > at["stop"] and
                                memory_words(ask("memory 0 0 0 12"), adapter)[1:] == [0, 12],
                                "authenticated finite exit gate did not arm at a fresh actual stop")
                        if kind.endswith("native"):
                            stepped = ask(f"step asm {at['thread']}")
                            native = position(ask("status"))
                            require(native["native_pc"] is not None and native["stop"] > armed["stop"],
                                    "detach requires a real later native trap", (stepped, native))
                            row["actual_native_trap"] = native
                        if kind == "foreign-peer":
                            # The same UID and same inherited FD do not mint the
                            # original launcher's SCM_CREDENTIALS identity.
                            sender = os.fork()
                            if sender == 0:
                                try:
                                    parent.send(b"pause")
                                    os._exit(0)
                                except BaseException:
                                    os._exit(1)
                            _, sender_status = os.waitpid(sender, 0)
                            require(os.waitstatus_to_exitcode(sender_status) == 0, "owned foreign sender failed")
                            try:
                                refused = parent.recv(65536)
                            except ConnectionResetError:
                                refused = b""
                            require(refused == b"", "foreign sender obtained a control reply", refused)
                            row["same_uid_foreign_pid_rejected"] = True
                        elif kind.startswith("quit"):
                            detached = ask("quit")
                            require(detached == b"debug control detached; guest continues\n",
                                    "explicit detach acknowledgement", detached)
                        else:
                            parent.close()
                        # Success comes only from waiting the original native
                        # child. Closing the endpoint releases a still-paused
                        # guest; the guest validates its result before return.
                        row["process_exit"] = child.wait(30)
                        require(row["process_exit"] == 0, "original guest failed to continue and exit after detach", row)
                        row["kernel_process_retirement_observed"] = True
                        row["passed"] = True
                except BaseException as error:
                    row["error"] = repr(error)
                    raise
                finally:
                    inherited.close()
                    parent.close()
                    if child is not None:
                        child.close()
                    save()
                require(sha(wasm) == pins[str(wasm)], "fixture changed during actual VM execution")
        result["admission_after"] = admission(root)
        require(result["admission_after"] == before_admission, "boot or cgroup changed")
        require(build_proof(root, binary, args.build_receipt) == proof, "original production closure changed")
        result["passed"] = bool(result["rows"]) and all(row["passed"] for row in result["rows"])
    except BaseException as error:
        result["error"] = repr(error)
        raise
    finally:
        result["inputs_after"] = {path: sha(path) for path in pins}
        if result["inputs_after"] != pins:
            result["passed"] = False
            result["inputs_changed"] = True
        save()
    require(result["passed"], "endpoint lifecycle qualification failed")
    print("PASS actual late management/detach:", len(result["rows"]), "owned native guest sessions")


if __name__ == "__main__":
    main()
