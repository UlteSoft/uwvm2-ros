#!/usr/bin/env python3
"""Exercise interoperable stopped scene observations and the real guard dump."""
import argparse
from contextlib import ExitStack
import hashlib
import json
from pathlib import Path
import re
import socket
import struct
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
import snapshot
from dap_adapter import parse_scene_state


def archive(header, rows):
    data = bytearray(header)
    struct.pack_into("<Q", data, 56, len(rows))
    for kind, capture, ident, schema, payload in rows:
        data.extend(struct.pack("<HB5xQQQ", kind, capture, ident, schema, len(payload)))
        data.extend(payload)
    data.extend(struct.pack("<8sQ", snapshot.COMMIT, len(data) + 48))
    return bytes(data) + hashlib.sha256(data).digest()


def interoperability(formatter, original, old_reader, directory):
    # Keep the legacy dump bytes intact and append the actual native codec's
    # observation. The live case below separately tests the collector itself.
    original_data = original.read_bytes()
    original_image = snapshot.decode(original_data)
    assert "sceneObservation" not in original_image.describe()
    stop = original_image.describe()["diagnostic"]["stopId"]
    assert stop == "3", "supply the baseline stopped guard dump"
    core, = [r for r in original_image.records if r.kind == 10 and r.identifier == 0]
    rows = [(r.kind, r.capture, r.identifier, r.schema, bytes(r.payload)) for r in original_image.records]
    payload = subprocess.check_output([formatter])
    schema = snapshot.SCENE_OBSERVATION_SCHEMA

    def image(payload, *, records=rows, ident=1, capture=1, version=schema):
        return archive(original_data[:64], [*records, (10, capture, ident, version, payload)])

    extended = image(payload)
    observed = snapshot.decode(extended).describe()
    assert observed["executableRestore"] is False
    observation = observed["sceneObservation"]
    assert observation["restoreAuthority"] is False and observation["providerStateRestorable"] is False
    assert observation["stopId"] == stop
    pages = observation["pages"]
    assert pages["resources"] == {"available": False, "status": "busy"}
    grants = pages["authority"]
    assert grants["total"] == 9 and grants["next"] == 8 and grants["more"] is True
    assert len(grants["rows"]) == 8 and grants["rows"][0]["alias"]["text"] == "signal"
    assert pages["audit"]["rows"][0]["admitted"] == str(2**64 - 1)
    assert "sceneObservation" not in snapshot.decode(image(payload, version=schema + 1)).describe()
    invalid = [image(payload[:-1]), image(payload + b"x"), image(payload[:16] + b"\xff"*4 + payload[20:]),
               image(payload.replace(b"stop=3", b"stop=2", 1)),
               image(payload.replace(b"total=9", b"total=0", 1)),
               image(payload.replace(b"selection=audit", b"selection=other")),
               image(payload.replace(b"status=busy", b"status=oops")),
               image(payload[:8] + struct.pack("<Q", 99) + payload[16:]),
               image(payload, ident=2), image(payload, capture=0),
               image(payload, records=[r for r in rows if r[0] != 10]),
               image(payload, records=[r for r in rows if r[0] != 2])]
    for candidate in invalid:
        try:
            snapshot.decode(candidate)
        except snapshot.InvalidSnapshot:
            continue
        raise AssertionError("malformed or unbound scene observation was accepted")

    path = directory / "compatibility.uwsnap"
    path.write_bytes(extended)
    if old_reader:
        old = json.loads(subprocess.check_output([sys.executable, "-B", old_reader, "info", path]))
        assert old["records"] == observed["records"] and old["executableRestore"] is False
        assert "sceneObservation" not in old
    recovered, = [r for r in snapshot.decode(extended).records if r.kind == 10 and r.identifier == 0]
    assert recovered.payload == core.payload


def live(runtime, directory):
    from run_scene_dap_live import WAT
    from run_late_control_proc_exit import Child

    wasm = directory / "guard.wasm"
    wat = wasm.with_suffix(".wat")
    wat.write_text(WAT)
    subprocess.run(["wat2wasm", "--debug-names", wat, "-o", wasm], check=True)
    flags = ["--runtime-aot", "--runtime-debug", "--runtime-compile-threads", "0",
             "--runtime-llvm-jit-cache-path", "disable", "--wasm-set-main-module-name", "main",
             "--ros-scene-create", "robot", "--ros-scene-bind", "robot", "main"]
    for operation in ("capability.open", "guard.create", "guard.trigger"):
        flags.extend(["--ros-scene-interface", "robot", operation, "allow"])
    flags.extend(["--ros-scene-grant", "robot", "signal", "guard", "signal", "local",
                  "guard.create,guard.trigger,resource.observe", "--run", str(wasm)])
    with ExitStack() as stack:
        parent, endpoint = socket.socketpair(socket.AF_UNIX, socket.SOCK_SEQPACKET)
        stack.enter_context(parent)
        stack.enter_context(endpoint)
        parent.settimeout(60)
        endpoint.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        log = stack.enter_context((directory / "vm.log").open("wb"))
        vm = Child([str(runtime), "--debug-jit-control-fd", str(endpoint.fileno()), *flags],
                   log, pass_fds=(endpoint.fileno(),))
        stack.callback(vm.close)
        endpoint.close()

        def request(command):
            parent.sendall(command.encode())
            reply = parent.recv(65536).decode()
            assert reply, "VM closed its control endpoint"
            return reply

        assert "breakpoint " in request("breakwasm 0 3 0")
        request("continue")
        request("wait")
        stop = re.search(r"stop-id ([1-9][0-9]*)", request("status"))[1]

        def read_pages():
            result = {}
            for choice in ("authority", "resources", "audit"):
                wire = request(f"info scene {choice} first=0 count=8 if-stop={stop}")
                result[choice] = parse_scene_state(wire, {"selection": choice, "start": 0, "count": 8}, int(stop))
            return result

        before = read_pages()
        assert before["resources"]["rows"][0]["sequence"] == "1"
        for level in ("diagnostic", "full"):
            path = directory / (level + ".uwsnap")
            assert "wasm-dump saved" in request(f"dump wasm {level} {stop} {path}")
            report = snapshot.decode(path.read_bytes()).describe()
            assert report["disclosure"] == level and report["executableRestore"] is False
            if level == "full":
                assert report["sceneObservation"]["pages"] == before
            else:
                assert "sceneObservation" not in report
        assert read_pages() == before


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, help="VM for a real cooperative guard stop")
    parser.add_argument("--formatter", type=Path, help="compiled scene_snapshot.cc")
    parser.add_argument("--legacy-guard", type=Path, help="existing guard dump at stop 3")
    parser.add_argument("--legacy-reader", type=Path, help="optional previous snapshot.py")
    parser.add_argument("--output", type=Path, help="new directory to retain generated archives and logs")
    args = parser.parse_args()
    if bool(args.formatter) != bool(args.legacy_guard) or not (args.runtime or args.formatter):
        parser.error("supply --runtime and/or both --formatter and --legacy-guard")
    from run_scene_vm import verify_gate
    verify_gate()
    with tempfile.TemporaryDirectory() as temporary:
        out = Path(temporary)
        if args.output:
            out = args.output.resolve()
            out.mkdir(parents=True, exist_ok=False)
        if args.formatter:
            interoperability(args.formatter.resolve(), args.legacy_guard, args.legacy_reader, out)
        if args.runtime:
            live(args.runtime.resolve(), out)
    print("PASS scene snapshot")


if __name__ == "__main__":
    main()
