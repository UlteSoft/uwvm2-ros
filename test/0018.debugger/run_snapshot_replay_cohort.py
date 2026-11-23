#!/usr/bin/env python3
"""Record a real native cohort and replay its shared atomics/MTE entropy twice."""
import argparse
from collections import Counter
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import time


def require(value, message):
    if not value:
        raise RuntimeError(message)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--repository", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--workers", type=int, choices=range(2, 33), default=8)
    parser.add_argument("--memory64", action="store_true")
    parser.add_argument("--memtag", action="store_true")
    parser.add_argument("--guest-tag-mismatch", action="store_true")
    parser.add_argument("--backend", choices=("software", "hardware"), default="software")
    parser.add_argument("--delivery", choices=("sync", "async"), default="sync")
    parser.add_argument("--report", choices=("crash", "log"), default="crash")
    args = parser.parse_args()
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "resource cgroup required")
    require(args.memtag or args.backend == "software", "hardware requires actual MTE instructions")
    require(not args.guest_tag_mismatch or (args.memtag and args.report == "log"), "guest mismatch requires active MTE log policy")
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    binary = args.binary.resolve()
    address = "i64" if args.memory64 else "i32"
    extend = "i64.extend_i32_u" if args.memory64 else ""
    wat = out / "cohort.wat"
    wasm = wat.with_suffix(".wasm")
    # Each real worker gets a separate allocation granule, while all workers
    # contend on the second atomic counter. The optional tag opcode consumes
    # genuine entropy on each iteration, rather than replaying a constant tag.
    wat.write_text(f'''(module
      (memory {"i64 " if args.memory64 else ""}1 1 shared)
      (func $initialize)
      (func $worker (result i32) (local $slot {address}) (local $pointer {address})
        {address}.const 0 i32.const 1 i32.atomic.rmw.add
        i32.const 16 i32.mul {extend} {address}.const 256 {address}.add local.set $slot
        loop $work
          nop
          local.get $slot local.set $pointer
          local.get $pointer i32.const 1 i32.atomic.rmw.add drop
          {address}.const 32 i32.const 1 i32.atomic.rmw.add drop
          br $work
        end i32.const 0))
''')
    subprocess.run(["/usr/bin/wat2wasm", "--enable-threads", "--enable-memory64",
                    "--debug-names", str(wat), "-o", str(wasm)], check=True)
    env = {**os.environ, "UWVM_TEST_WORKERS": str(args.workers),
           "UWVM_TEST_ALL_WORKERS_PROGRESS": "1",
           "UWVM_TEST_MEMTAG_BACKEND": args.backend, "UWVM_TEST_MEMTAG_DELIVERY": args.delivery,
           "UWVM_TEST_MEMTAG_REPORT": args.report}
    if args.memtag:
        sys.path.insert(0, str(args.repository.resolve() / "test/0086.memtag"))
        import vm_matrix as mte

        def read_u(data, at):
            value = shift = 0
            while True:
                byte = data[at]
                at += 1
                value |= (byte & 127) << shift
                if not byte & 128:
                    return value, at
                shift += 7

        original = wasm.read_bytes()
        encoded = bytearray(original[:8])
        at = 8
        replaced = False
        while at < len(original):
            section = original[at]
            size, first = read_u(original, at + 1)
            body = original[first:first + size]
            at = first + size
            if section == 10:
                count, cursor = read_u(body, 0)
                require(count == 2, "exact setup/worker function bodies required")
                rebuilt = bytearray(mte.u(count))
                for index in range(count):
                    size, first = read_u(body, cursor)
                    code = body[first:first + size]
                    cursor = first + size
                    if index == 1:
                        needle = b"\x03\x40\x01\x20\x00\x21\x01"
                        require(code.count(needle) == 1, "one authenticated loop marker required")
                        tagged = b"\x03\x40\x20\x00" + mte.c(16, 64 if args.memory64 else 32) + mte.op(0x2f)
                        if args.guest_tag_mismatch:
                            # Toggle an upper tag bit; memory64 keeps identical
                            # hardware low-nibble colors while full tags differ.
                            tagged += mte.c(1 << (60 if args.memory64 else 28), 64 if args.memory64 else 32)
                            tagged += b"\x85" if args.memory64 else b"\x73"
                        tagged += b"\x21\x01"
                        code = code.replace(needle, tagged)
                        replaced = True
                    rebuilt += mte.u(len(code)) + code
                require(cursor == len(body), "body extent")
                body = rebuilt
            encoded += bytes([section]) + mte.u(len(body)) + body
        require(replaced, "tagged opcode must actually be present")
        wasm.write_bytes(encoded)
        env["UWVM_TEST_REQUIRE_MTE"] = "1"
        env["UWVM_TEST_REQUIRE_ENTROPY"] = "1"
    runner = json.loads(args.runner_json.read_text()) if args.runner_json else []
    argv = [*runner, str(binary), str(wasm), str(args.workers * 750)]
    (out / "argv.json").write_text(json.dumps(argv, indent=2))
    begin = time.monotonic()
    with (out / "run.log").open("w") as log:
        result = subprocess.run(argv, cwd=out, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=600)
    text = (out / "run.log").read_text()
    evidence = dict(code=result.returncode, workers=args.workers, seconds=time.monotonic() - begin,
                    mte_backend=args.backend, mte_delivery=args.delivery, mte_report=args.report,
                    wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest(),
                    binary_sha256=hashlib.sha256(binary.read_bytes()).hexdigest())
    (out / "results.json").write_text(json.dumps(evidence, indent=2))
    require(result.returncode == 0, text)
    require(text.count("memory_equal=1") == 2 and "shared-memory instruction replay PASS" in text, text)
    if args.memtag:
        require("MTE actual active mappings=" in text and "random-tag entropy recorded for all" in text, text)
    if args.guest_tag_mismatch:
        def reports(begin, end):
            require(text.count(begin) == 1 and text.count(end) == 1, "unambiguous execution boundaries")
            segment = text.split(begin, 1)[1].split(end, 1)[0]
            found = []
            for line in segment.splitlines():
                if not line.startswith('{"schema":"uwvm.memtag.violation.v1"'):
                    continue
                value = json.loads(line)
                require(value["pointer_tag"] != value["allocation_tag"], "real tag mismatch required")
                if args.memory64:
                    require(value["pointer_tag"] & 15 == value["allocation_tag"] & 15, "high-tag mismatch must survive hardware acceleration")
                del value["sequence"]
                found.append(json.dumps(value, sort_keys=True))
            return Counter(found)
        original = reports("RECORD start=", "RECORD end=")
        require(original, "actual guest instructions must report tag violations")
        for round in range(2):
            replayed = reports(f"PREPARE round={round}", f"REPLAY round={round}")
            require(replayed == original, f"guest report mismatch in replay {round}: original={sum(original.values())} replay={sum(replayed.values())}")
        evidence["guest_violation_reports_per_execution"] = sum(original.values())
        (out / "results.json").write_text(json.dumps(evidence, indent=2))
    print("native cohort replay PASS", evidence, flush=True)


if __name__ == "__main__":
    main()
