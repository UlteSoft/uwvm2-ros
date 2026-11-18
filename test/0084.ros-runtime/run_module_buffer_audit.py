"""Run the actual registered-message VM entry points with private per-stage observation."""
import argparse
import json
import os
from pathlib import Path
import struct
import subprocess

from run_module_exchange_frames import pair
from run_module_security_vm import CALLS, message_pair
from run_scene_vm import sha, verify_gate


def decode(data):
    if data[:8] != b"WROSCAP\x01":
        raise ValueError("invalid journal header")
    offset, records = 8, []
    while offset < len(data):
        size, = struct.unpack_from("<I", data, offset)
        if size < 8 or offset + size > len(data):
            raise ValueError("incomplete frame")
        point = data[offset + 4]
        if not point:
            if size != 56 or offset + size != len(data):
                raise ValueError("invalid footer")
            return records, struct.unpack_from("<6Q", data, offset + 8)
        size, point, ne, nr, reserved, seq, generation, count = struct.unpack_from("<IBBBBQQI", data, offset)
        if reserved or not generation or seq != len(records) + 1 or size != 28 + ne + nr + count:
            raise ValueError("invalid data frame")
        at = offset + 28
        if data[at : at + ne] != b"exchange" or data[at + ne : at + ne + nr] != b"echo":
            raise ValueError("wrong registered route")
        records.append((point, data[at + ne + nr : offset + size]))
        offset += size
    raise ValueError("missing footer")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runtime", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    runtime, out = args.runtime.resolve(), args.output.resolve()
    if ".tmp" not in out.parts:
        parser.error("use a fresh project .tmp directory")
    out.mkdir(parents=True, exist_ok=False)
    report = {"complete": False, "runtime_sha256": sha(runtime), "cases": []}

    def save():
        (out / "results.json").write_text(json.dumps(report, indent=2) + "\n")

    for width, memory in ((32, 0), (64, 1)):
        def compile_pair(label, endpoints):
            compiled = []
            for name, endpoint in zip(("main", "worker"), endpoints):
                wat, module = (out / f"w{width}-{label}-{name}{suffix}" for suffix in (".wat", ".wasm"))
                wat.write_text(endpoint.render("_start" if name == "main" else None))
                subprocess.run(["wat2wasm", "--enable-threads", "--enable-memory64", "--enable-multi-memory",
                                "--debug-names", wat, "-o", module], check=True)
                compiled.append(module)
            return compiled
        clean = compile_pair("ordinary", pair(width, memory, False, 8))
        invalid = {name: compile_pair(name, message_pair(width, memory, shared=shared, exported=exported))
                   for name, shared, exported in (("private-memory", False, False), ("exported-memory", True, True))}
        flags = ["--ros-scene-create", "exchange", "--ros-scene-bind", "exchange", "main",
                 "--ros-scene-bind", "exchange", "worker"]
        for name in ["capability.open", *CALLS]:
            flags.extend(["--ros-scene-interface", "exchange", name, "allow"])
        flags += ["--ros-scene-call-budget", "exchange", "65536", "1", "--ros-scene-call", "exchange", "echo",
                  "main", "worker", "echo", "octets", "octets", "8", "8", "1", "10000000000",
                  "--ros-scene-grant", "exchange", "exchange", "module.call", "echo", "registered-message", ",".join(CALLS)]
        for engine in ("int", "aot"):
            for name, point in (("none", 0), ("submit", 1), ("take", 2), ("respond", 3), ("result", 4),
                                ("oversize", 1), ("bad-route", 0), ("bad-stage", 0),
                                ("private-memory", 1), ("exported-memory", 1)):
                images = invalid.get(name, clean)
                label = f"w{width}-{engine}-{name}"
                path = out / (label + ".bin")
                fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_CLOEXEC, 0o600)
                options = ["--ros-scene-call-buffer-journal", str(fd), "8", "4096", "7" if name == "oversize" else "8"]
                if name != "none":
                    selected = "submit" if name in ("oversize", "bad-route") or name in invalid else name
                    options += ["--ros-scene-call-buffer-capture", "exchange", "wrong" if name == "bad-route" else "echo", selected]
                command = [str(runtime), "--wasm-set-main-module-name", "main", "--runtime-" + engine,
                           "--runtime-compile-threads", "0", "--runtime-llvm-jit-cache-path", "disable",
                           "--runtime-memtag-backend", "software", *flags, *options, "--wasm-preload-library",
                           str(images[1]), "worker", "--run", str(images[0])]
                try:
                    run = subprocess.run(command, pass_fds=(fd,), capture_output=True, timeout=60)
                finally:
                    os.close(fd)
                (out / (label + ".stderr")).write_bytes(run.stderr)
                expected_failure = name.startswith("bad-")
                if expected_failure:
                    if not run.returncode or path.stat().st_size not in (8, 64):
                        raise ValueError("invalid selector reached a guest: " + label)
                else:
                    if run.returncode:
                        raise RuntimeError((label, run.stderr[-2500:]))
                    rows, counters = decode(path.read_bytes())
                    count = 0 if name in ("none", "oversize") or name in invalid else 2
                    pattern = bytes((i * 17 + (3 if point in (1, 2) else 91)) % 256 for i in range(8))
                    if rows != [(point, pattern)] * count or counters != (count, count, 2 if name == "oversize" else 0, 0, 0, 0):
                        raise ValueError((label, rows, counters))
                report["cases"].append({"name": label, "exit": run.returncode, "expected_failure": expected_failure,
                                        "argv": command, "file_sha256": sha(path), "guest_sha256": [sha(f) for f in images]})
                save()
    verify_gate()
    report["complete"] = True
    save()


if __name__ == "__main__":
    main()
