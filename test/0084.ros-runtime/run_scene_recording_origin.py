"""Compare recording in a ROSv2-enabled VM with and without a selected scene provider."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import socket
import subprocess
import sys

from run_scene_vm import verify_gate

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
from snapshot import decode


def digest(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def fixture(kind):
    imports = {
        "wasm": "",
        "wasip1": '(import "wasi_snapshot_preview1" "args_sizes_get" (func $args (param i32 i32) (result i32)))',
        "scene": '(import "uwvm_ros_v2" "abi" (func $abi (result i32)))',
    }
    check = {
        "wasm": "",
        "wasip1": """(if (call $args (i32.const 0) (i32.const 4)) (then unreachable))
    (if (i32.ne (i32.load (i32.const 0)) (i32.const 1)) (then unreachable))
    (if (i32.ne (i32.load (i32.const 4)) (i32.const 7)) (then unreachable))""",
        "scene": "(if (i32.ne (call $abi) (i32.const 2)) (then unreachable))",
    }
    return f'''(module
  {imports[kind]}
  (memory (export "memory") 1)
  (global $value (mut i32) (i32.const 3))
  (func $marker nop)
  (func (export "_start")
    {check[kind]}
    (global.set $value (i32.add (global.get $value) (i32.const 39)))
    (call $marker)
    (if (i32.ne (global.get $value) (i32.const 42)) (then unreachable))))
'''


def exercise(kind, runtime, wat2wasm, directory):
    directory.mkdir()
    wat = directory / "program.wat"
    wasm = wat.with_suffix(".wasm")
    wat.write_text(fixture(kind))
    subprocess.run([wat2wasm, "--debug-names", wat, "-o", wasm], check=True)
    flags = ["--ros-scene-create", "robot", "--ros-scene-bind", "robot", "main",
             "--ros-scene-interface", "robot", "abi", "allow"]
    if kind == "wasip1":
        flags += ["--wasip1-global-enable", "--wasip1-global-interface", "args_sizes_get", "allow",
                  "--wasip1-global-grant", "args", "--wasip1-global-force-args", "1", "sample"]
    channel, inherited = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
    channel.settimeout(30)
    with inherited, (directory / "vm.log").open("wb") as log:
        inherited.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        vm = subprocess.Popen(
            [runtime, "--debug-jit-control-fd", str(inherited.fileno()),
             "--runtime-aot", "--runtime-debug", "--debug-permissions", "full",
             "--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable",
             "--wasm-set-main-module-name", "main", *flags, "--run", str(wasm)],
            pass_fds=(inherited.fileno(),), stdout=log, stderr=subprocess.STDOUT)
    records = []

    def request(command):
        channel.sendall(command.encode())
        reply = channel.recv(65536).decode().strip()
        assert reply, command
        records.append({"command": command, "reply": reply})
        (directory / "transcript.json").write_text(json.dumps(records, indent=2) + "\n")
        return reply

    def reach(function):
        registered = request(f"breakwasm 0 {function} 0")
        breakpoint = re.search(r"breakpoint ([0-9]+) registered", registered)
        assert breakpoint, registered
        assert "running" in request("continue")
        stopped = request("wait")
        assert "stopped: breakpoint" in stopped and f"function={function} byte-offset=0" in stopped, stopped
        match = re.search(r"stop-id ([1-9][0-9]*)", stopped)
        assert match, stopped
        assert "breakpoint deleted" in request("delete " + breakpoint[1])
        return match[1], int(re.search(r"generation=([1-9][0-9]*)", stopped)[1])

    try:
        marker = int(kind != "wasm")
        origin, generation = reach(marker + 1)
        stale = request(f"dump record start {int(origin) + 1}")
        assert "snapshot-record rejected" in stale and "origin-wasm-state=" not in stale, stale
        started = request("dump record start " + origin)
        if kind == "scene":
            assert "snapshot-record rejected" in started and "origin-wasm-state=unavailable" in started, started
        else:
            assert "snapshot-record mode=1" in started, started
        end, end_generation = reach(marker)
        assert generation == end_generation
        archive = directory / "interval.uwsnap"
        reply = request(f"dump record save {end} {archive}")
        if kind == "scene":
            assert "snapshot-record rejected" in reply and not archive.exists(), reply
            output = directory / "observed.uwsnap"
            assert "wasm-dump saved" in request(f"dump wasm full {end} {output}")
            components = decode(output.read_bytes()).describe()["records"]
            assert any(row["kind"] == "wasm-state" and row["capture"] == "unavailable" for row in components)
            assert any(row["kind"] == "wasm-module" and row["capture"] == "complete" for row in components)
            return {"module_sha256": digest(wasm), "full_dump_sha256": digest(output)}
        assert "saved=1" in reply and archive.is_file(), reply
        restored = request(f"dump replay {end} {archive}")
        assert "snapshot-restored stop=" in restored and "source=ready" in restored, restored
        restored_position = request("status")
        replay_generation = int(re.search(r"generation=([1-9][0-9]*)", restored_position)[1])
        assert replay_generation != generation and f"function={marker + 1} byte-offset=0" in restored_position
        assert "running" in request("continue")
        endpoint = request("wait")
        assert f"function={marker} byte-offset=0 generation={replay_generation}" in endpoint, endpoint
        final = request("dump record status")
        assert "mode=4" in final and "journal-error=0" in final, final
        return {"module_sha256": digest(wasm), "interval_sha256": digest(archive), "endpoint": final}
    finally:
        # This test owns a disposable VM paused at the endpoint. Sending a
        # continue here would run the guest beyond the interval being checked.
        channel.close()
        if vm.poll() is None:
            vm.terminate()
        try:
            vm.wait(timeout=5)
        except subprocess.TimeoutExpired:
            vm.kill()
            vm.wait(timeout=5)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--wat2wasm", default="wat2wasm")
    args = parser.parse_args()
    verify_gate()
    output = args.out.resolve()
    output.mkdir(parents=True, exist_ok=False)
    runtime = args.runtime.resolve(strict=True)
    receipt = {"complete": False, "runtime_sha256": digest(runtime), "cases": {}}
    for kind in ("wasm", "wasip1", "scene"):
        receipt["cases"][kind] = exercise(kind, str(runtime), args.wat2wasm, output / kind)
        (output / "results.json").write_text(json.dumps(receipt, indent=2) + "\n")
    receipt["complete"] = True
    (output / "results.json").write_text(json.dumps(receipt, indent=2) + "\n")


if __name__ == "__main__":
    main()
