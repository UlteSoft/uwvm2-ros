#!/usr/bin/env python3
"""Real DAP reattachment/replay with existing, authenticated stable-v1 gateways.

Reuse the authorized options and guest from a completed native ROS matrix.
Gateways are genuine native ROS, running in the caller's isolated namespace.
"""
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import re
import signal
import socket
import subprocess
import sys
import tempfile
import time
from run_dump_launch_policy import VM, require

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
import dap_adapter as dap


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--cases-json", type=Path, required=True)
    parser.add_argument("--native-root", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    args = parser.parse_args(); args.out = args.out.resolve(); args.out.mkdir(parents=True, exist_ok=False)
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "resource cgroup required")
    require([name for _, name in socket.if_nameindex()] == ["lo"], "isolated network namespace required")
    prefix = json.loads(args.runner_json.read_text()) if args.runner_json else []
    results = []
    for case in json.loads(args.cases_json.read_text()):
        version = case["backend"]; require(case["passed"] and case["abi"] == "v1", "qualified stable-v1 input required")
        old = Path(case["path"]); folder = args.out / f"ros{version}-{case['engine']}"; folder.mkdir()
        receipt = json.loads((args.native_root / f"native-ros{version}/receipt.json").read_text())
        gateway = (args.native_root / f"native-ros{version}/build/gateway/uwvm-ros{version}-gateway").resolve()
        require(hashlib.sha256(gateway.read_bytes()).hexdigest() == receipt["binaries"][str(gateway)], "gateway digest differs")
        env = {**os.environ, **receipt["env"], "PATH": str(args.native_root / "tools") + ":" + os.environ["PATH"],
               "ROS_DOMAIN_ID": "193", "ROS_LOCALHOST_ONLY": "1", "RMW_IMPLEMENTATION": "rmw_fastrtps_cpp",
               "ROS_MASTER_URI": "http://127.0.0.1:11393", "ROS_IP": "127.0.0.1",
               "ROS_HOME": str(folder / "ros-home"), "ROS_LOG_DIR": str(folder / "ros-log")}
        row = {"backend": version, "abi": "v1", "engine": case["engine"], "passed": False}
        actors = []; logs = []; vm = None
        with tempfile.TemporaryDirectory(prefix="trd-", dir=ROOT / ".tmp") as short:
            address = Path(short) / "g.sock"
            def launch(name, command):
                log = (folder / (name + ".log")).open("wb"); logs.append(log)
                actor = subprocess.Popen(command, env=env, stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
                actors.append(actor); return actor
            try:
                if version == 1:
                    master = launch("master", [str(args.native_root / "tools/roscore"), "-p", "11393"])
                    deadline = time.monotonic() + 30
                    while True:
                        with socket.socket() as check:
                            if check.connect_ex(("127.0.0.1", 11393)) == 0: break
                        require(master.poll() is None and time.monotonic() < deadline, "native master startup")
                        time.sleep(.01)
                original = json.loads((old / "argv.json").read_text()); options = original[original.index("--") + 1:]
                at = options.index(f"--ros{version}-single-gateway-socket")
                options[at + 2] = str(address)
                vm = VM([str(args.binary.resolve()), *options], folder, prefix, start_paused=True)
                deadline = time.monotonic() + 20
                while "\nState:\tT" not in Path(f"/proc/{vm.process.pid}/status").read_text():
                    require(vm.process.poll() is None and time.monotonic() < deadline, "VM launch gate"); time.sleep(.005)
                output = io.BytesIO(); adapter = dap.Adapter(output)
                adapter.broker = vm; adapter.debug_permissions = "full"; adapter.configuration_complete = True
                contract = (old / "gateway.conf").read_text()
                contract = re.sub(r"(?m)^worker-pid .*", "worker-pid " + str(vm.process.pid), contract)
                contract = re.sub(r"(?m)^socket .*", "socket " + str(address), contract)
                config = folder / "gateway.conf"; config.write_text(contract); config.chmod(0o600)
                peer = launch("gateway", [str(gateway), str(config)])
                deadline = time.monotonic() + 30
                while not address.is_socket():
                    require(peer.poll() is None and time.monotonic() < deadline, "native gateway startup"); time.sleep(.01)
                os.kill(vm.process.pid, signal.SIGCONT)
                adapter.observe(vm.request("status"), notify=False)
                require(adapter.threaded_profile and adapter.state == "stopped", "genuine entry stop required")
                os.kill(peer.pid, signal.SIGSTOP)
                while "\nState:\tT" not in Path(f"/proc/{peer.pid}/status").read_text():
                    require(time.monotonic() < deadline, "native gateway stop"); time.sleep(.005)
                sequence = 0
                def request(command, arguments=None):
                    nonlocal sequence
                    sequence += 1; output.seek(0); output.truncate()
                    adapter.handle({"seq": sequence, "command": command, "arguments": arguments or {}})
                    output.seek(0); messages = []
                    while line := output.readline():
                        length = int(line.split(b":")[1]); require(output.readline() == b"\r\n", "DAP framing")
                        messages.append(json.loads(output.read(length)))
                    reply = next(m for m in messages if m["type"] == "response")
                    require(reply["success"], reply); return reply.get("body", {})
                def completed():
                    deadline = time.monotonic() + 120
                    while True:
                        adapter.observe(vm.request("status"), notify=False)
                        if adapter.state == "exited":
                            require(not adapter.finished, "joined guest closed replay management"); return
                        require(time.monotonic() < deadline, "cohort completion"); time.sleep(.01)
                def counts():
                    text = (folder / "gateway.log").read_text()
                    return [text.count("operation=topic.publish "), text.count("operation=session.bind ")]
                request("continue", {"threadId": adapter.threads[0]["id"]})
                deadline = time.monotonic() + 30
                while True:
                    time.sleep(.01); request("pause")
                    status = vm.request("status"); adapter.observe(status, notify=False)
                    if "native-operation=pending" in status:
                        stop = str(adapter.threads[0]["stop_id"])
                        retained = folder / ("retained-" + stop + ".uwsnap")
                        saved = request("uwvm/dump", {"path": str(retained), "stopId": stop, "disclosure": "full"})
                        # A native participant can be parked before admission,
                        # when no effect has been sent and its result is known.
                        # Require the producer's genuine unknown-result seal.
                        if saved["retainedNativePending"]: break
                    require(time.monotonic() < deadline, "no actual unresolved ROS import")
                    request("continue", {"threadId": adapter.threads[0]["id"]})
                require(saved["retainedNativePending"], saved)
                request("uwvm/snapshotHistory", {"path": str(retained)})
                restored = request("uwvm/restoreSnapshot", {"path": str(retained), "stopId": stop, "environment": "replay"})
                require(restored["execution"] == "stopped", restored)
                os.kill(peer.pid, signal.SIGCONT)
                # An already-dispatched publish may finish externally while
                # paused. Its result must not advance the Wasm continuation.
                locations = re.findall(r"thread \d+ module=\d+ function=\d+ byte-offset=\d+ generation=\d+", status)
                time.sleep(.05)
                after = vm.request("status")
                require(re.search(r"stop-id (\d+)", after)[1] == stop and
                        re.findall(r"thread \d+ module=\d+ function=\d+ byte-offset=\d+ generation=\d+", after) == locations,
                        "paused native completion advanced Wasm execution")
                row["pausedNativeCompletions"] = counts()
                request("continue", {"threadId": adapter.threads[0]["id"]}); completed()
                row["recordCounts"] = counts(); require(row["recordCounts"] == [2, 2], row)
                archive = folder / "complete.uwsnap"
                request("uwvm/dump", {"path": str(archive), "stopId": "0", "disclosure": "full"})
                row["replayCounts"] = []
                for _ in range(2):
                    reply = request("uwvm/restoreSnapshot", {"path": str(archive), "stopId": "0", "environment": "replay", "cursor": "2"})
                    require(reply["execution"] == "stopped", reply)
                    request("continue", {"threadId": adapter.threads[0]["id"]}); completed()
                    row["replayCounts"].append(counts()); require(counts() == row["recordCounts"], "repeated native ROS effect")
                row["passed"] = True
            except Exception as error:
                row["error"] = repr(error)
            finally:
                if vm: row["commands"] = vm.commands; vm.close()
                for actor in reversed(actors):
                    if actor.poll() is None: os.killpg(actor.pid, signal.SIGKILL); actor.wait(timeout=10)
                for log in logs: log.close()
                results.append(row); (args.out / "results.json").write_text(json.dumps(results, indent=2))
                print(json.dumps({k: v for k, v in row.items() if k != "commands"}), flush=True)
    return int(any(not row["passed"] for row in results))


if __name__ == "__main__":
    raise SystemExit(main())
