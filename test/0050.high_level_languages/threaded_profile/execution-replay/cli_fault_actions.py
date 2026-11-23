#!/usr/bin/env python3
"""Check the real CLI fault policy and sealed histories, using explicit launch options.

Run inside the required cgroup. The options JSON is an authorized CLI argument
array, excluding the executable/runner and any thread-history selection. No
compiler, runtime, native gateway or environment is installed by this test.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import signal
import subprocess
import time


def arguments(path):
    value = json.loads(path.read_text())
    if not isinstance(value, list) or any(not isinstance(x, str) or not x for x in value):
        raise ValueError("arguments must be a nonempty-string argv array")
    return value


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--options-json", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--expected-status", type=int, choices=(120, 121, 122), required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--action", choices=("default", "crash", "exit", "pause"), action="append")
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    binary = args.binary.resolve(strict=True)
    options = arguments(args.options_json)
    if "--debug-thread-history" in options:
        raise ValueError("test owns the recording/replay selection")
    if "--debug-fault-action" in options:
        at = options.index("--debug-fault-action")
        del options[at:at + 2]
    runner = arguments(args.runner_json) if args.runner_json else []
    output = args.output.resolve(); output.mkdir(parents=True, exist_ok=False)
    (output / "provenance.json").write_text(json.dumps({
        "binary": str(binary), "sha256": hashlib.sha256(binary.read_bytes()).hexdigest(),
        "options": options, "runner": runner, "cgroup": Path("/proc/self/cgroup").read_text(),
    }, indent=2))
    rows = []
    for action in args.action or ("default", "crash", "exit", "pause"):
        archive = output / (action + ".uwsnap")
        for mode in ("record", "replay"):
            selected = list(options)
            at = selected.index("--run")
            selected[at:at] = ["--debug-thread-history", mode, str(archive), "4194304"] + (
                [] if action == "default" else ["--debug-fault-action", action])
            command = runner + [str(binary)] + selected
            label = action + "-" + mode
            (output / (label + ".argv.json")).write_text(json.dumps(command, indent=2))
            start = time.monotonic(); stopped = False; timeout = False
            with (output / (label + ".log")).open("wb") as log:
                child = subprocess.Popen(command, stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT)
                try:
                    while child.poll() is None:
                        if time.monotonic() - start > 45:
                            timeout = True; child.kill(); break
                        if not stopped:
                            try:
                                state = next(line for line in Path(f"/proc/{child.pid}/status").read_text().splitlines()
                                             if line.startswith("State:"))
                                if state.split()[1] in ("T", "t"):
                                    stopped = True
                                    # This is the trusted parent's explicit terminal continuation.
                                    os.kill(child.pid, signal.SIGCONT)
                            except (FileNotFoundError, ProcessLookupError):
                                pass
                        time.sleep(0.01)
                    code = child.wait(timeout=5)
                finally:
                    if child.poll() is None:
                        child.kill(); child.wait()
            text = (output / (label + ".log")).read_bytes()
            crashed = code in (-signal.SIGILL, -signal.SIGABRT, -signal.SIGTRAP,
                               128 + signal.SIGILL, 128 + signal.SIGABRT, 128 + signal.SIGTRAP)
            diagnostic = b"Exit status: " + str(args.expected_status).encode()
            passed = (not timeout and diagnostic in text and archive.is_file()
                      and archive.stat().st_size > 64 and stopped == (action == "pause")
                      and (code == args.expected_status if action == "exit" else crashed))
            row = dict(action=action, mode=mode, code=code, stopped=stopped,
                       archive=archive.name, passed=passed, seconds=time.monotonic() - start)
            rows.append(row); print(row, flush=True)
            (output / "results.json").write_text(json.dumps(rows, indent=2))
            if not passed:
                print(text[-4000:].decode(errors="replace"), flush=True)
                break
    return int(any(not row["passed"] for row in rows))


if __name__ == "__main__":
    raise SystemExit(main())
