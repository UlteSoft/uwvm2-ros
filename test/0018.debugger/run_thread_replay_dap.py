#!/usr/bin/env python3
"""Exercise the normal DAP adapter against real dynamic WASI Threads.

Run inside the resource cgroup. No mocked management replies, private runtime
constructors or live host effects are used to stand in for replay.
"""
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import re
import sys
import time
from run_dump_launch_policy import VM, require

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
import dap_adapter as dap
import snapshot


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--argv-json", type=Path, required=True, help="real embedding fixture argv; options after -- are reused")
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--pending", action="store_true", help="retain and reattach genuine pipe reads before completing the recording")
    parser.add_argument("--replay-history", type=Path,
                        help="start the managed CLI from a completed history, with no live I/O")
    parser.add_argument("--initial-state", choices=("stopped", "joined"), default="stopped")
    parser.add_argument("--expected-writes", type=int, default=2,
                        help="number of real THREAD-ONCE writes made by this fixture")
    args = parser.parse_args()
    if args.pending and args.replay_history:
        parser.error("a replay cannot create a new native pending input operation")
    if args.initial_state == "joined" and not args.replay_history:
        parser.error("joined entry requires an explicit saved terminal replay endpoint")
    args.out = args.out.resolve(); args.out.mkdir(parents=True, exist_ok=False)
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "resource cgroup required")
    original = json.loads(args.argv_json.read_text()); options = original[original.index("--") + 1:]
    if args.replay_history:
        options[options.index("--run"):options.index("--run")] = [
            "--debug-thread-history", "replay", str(args.replay_history.resolve(strict=True)), "4194304"]
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    result = {"passed": False, "dap": [], "binarySHA256": hashlib.sha256(args.binary.read_bytes()).hexdigest()}
    vm = None
    input_pipe = None
    try:
        if args.pending: input_pipe = os.pipe()
        vm = VM([str(args.binary.resolve()), *options], args.out, prefix, stdin=input_pipe[0] if input_pipe else None)
        output = io.BytesIO(); adapter = dap.Adapter(output)
        adapter.broker = vm; adapter.debug_permissions = "full"; adapter.configuration_complete = True
        status = vm.request("status"); adapter.observe(status, notify=False)
        require(adapter.threaded_profile, status)
        require(adapter.state == ("exited" if args.initial_state == "joined" else "stopped"), status)
        require(not adapter.finished, "managed replay entry disconnected the IDE")
        if args.initial_state == "stopped": require(adapter.threads, status)
        sequence = 0

        def request(command, arguments=None, success=True):
            nonlocal sequence
            sequence += 1; output.seek(0); output.truncate()
            adapter.handle({"seq": sequence, "command": command, "arguments": arguments or {}})
            output.seek(0); messages = []
            while line := output.readline():
                length = int(line.split(b":")[1]); require(output.readline() == b"\r\n", "DAP framing")
                messages.append(json.loads(output.read(length)))
            response = next(item for item in messages if item["type"] == "response")
            result["dap"].append({"command": command, "arguments": arguments, "messages": messages})
            require(response["success"] == success, response)
            return response.get("body", {})

        def stop_id():
            adapter.observe(vm.request("status"), notify=False)
            return str(adapter.threads[0]["stop_id"]) if adapter.state == "stopped" else "0"

        def exited():
            deadline = time.monotonic() + 120
            while True:
                adapter.observe(vm.request("status"), notify=False)
                if adapter.state == "exited":
                    require(not adapter.finished, "DAP terminated the replay management session")
                    return
                require(time.monotonic() < deadline, "execution did not physically finish")
                time.sleep(.01)

        stale = None
        if args.initial_state == "stopped":
            initial = request("stackTrace", {"threadId": adapter.threads[0]["id"]})
            require(initial["stackFrames"], "real initial location missing")
            stale = initial["stackFrames"][0]["id"]
        request("uwvm/threadReplay", {"action": "status"})
        origin = args.out / "origin.uwsnap"
        request("uwvm/dump", {"path": str(origin), "stopId": stop_id(), "disclosure": "full"})
        request("uwvm/dump", {"path": str(args.out / "stale.uwsnap"), "stopId": "18446744073709551615"}, False)
        current_stop = stop_id()
        for permission in ("read-only", "limited", "none"):
            adapter.debug_permissions = permission
            request("uwvm/threadReplay", {"action": "replay", "path": str(origin), "stopId": current_stop}, False)
        adapter.debug_permissions = "full"
        if adapter.state == "stopped": request("continue", {"threadId": adapter.threads[0]["id"]})
        if args.pending:
            deadline = time.monotonic() + 120
            while True:
                blocked = 0
                for task in Path(f"/proc/{vm.process.pid}/task").iterdir():
                    try: blocked += "pipe_read" in (task / "wchan").read_text()
                    except OSError: pass
                # Same-policy stdin aliases deliberately share the descriptor
                # lock. One syscall can be active while its peer waits on it.
                if blocked: break
                require(time.monotonic() < deadline, "an actual native pipe read was not reached"); time.sleep(.01)
            request("pause")
            retained = args.out / "retained.uwsnap"; pending_stop = stop_id()
            saved = request("uwvm/dump", {"path": str(retained), "stopId": pending_stop, "disclosure": "full"})
            require(saved["retainedNativePending"], saved)
            request("uwvm/snapshotHistory", {"path": str(retained)})
            restored = request("uwvm/restoreSnapshot", {"path": str(retained), "stopId": pending_stop, "environment": "replay"})
            require(restored["execution"] == "stopped" and stop_id() == pending_stop, restored)
            os.write(input_pipe[1], b"AB")
            require((args.out / "vm.log").read_bytes().count(b"THREAD-ONCE") == 0, "paused native completion advanced the guest")
            request("continue", {"threadId": adapter.threads[0]["id"]})
        exited()
        if stale is not None:
            require(not request("scopes", {"frameId": stale}, False), "stale frame survived")
        completed = args.out / "complete.uwsnap"
        request("uwvm/recordSnapshot", {"action": "save", "path": str(completed), "stopId": "0"})
        decoded = snapshot.decode(completed.read_bytes()).describe()
        history = decoded["threadHistory"]
        result["history"] = history
        require(history["environmentSamples"], "real environment samples missing")
        for sample in range(len(history["environmentSamples"])):
            index = request("uwvm/snapshotHistory", {"path": str(completed), "event": str(sample)})
            require(index["historical"] and not index["restoreAuthority"], index)
            for environment in index["environments"]:
                value = request("uwvm/snapshotHistory", {"kind": environment["kind"], "id": environment["id"]})
                require(value["environment"] and value["historical"], value)
        before = (args.out / "vm.log").read_bytes().count(b"THREAD-ONCE")
        require(before == (0 if args.replay_history else args.expected_writes),
                "unexpected native writes during replay" if args.replay_history else "unexpected number of genuine recorded writes")
        # Reconstruct an actual prefix (first instruction), invalidate old
        # identities and consume the remainder without performing native I/O.
        for iteration in range(2):
            reply = request("uwvm/restoreSnapshot", {"path": str(completed), "stopId": "0", "environment": "replay", "cursor": "2"})
            require(reply["threaded"] and reply["execution"] == "stopped", reply)
            request("stackTrace", {"threadId": adapter.threads[0]["id"]})
            request("continue", {"threadId": adapter.threads[0]["id"]})
            exited()
            require((args.out / "vm.log").read_bytes().count(b"THREAD-ONCE") == before, "replay repeated external output")
        result["passed"] = True
    except Exception as error:
        result["error"] = repr(error)
    finally:
        if vm:
            result["commands"] = vm.commands; vm.close()
        if input_pipe:
            for fd in input_pipe: os.close(fd)
        (args.out / "results.json").write_text(json.dumps(result, indent=2))
    print(json.dumps({k: v for k, v in result.items() if k in ("passed", "error")}), flush=True)
    return int(not result["passed"])


if __name__ == "__main__":
    raise SystemExit(main())
