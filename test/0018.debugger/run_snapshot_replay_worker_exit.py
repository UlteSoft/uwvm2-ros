#!/usr/bin/env python3
"""A genuine native entry returns; replay reconstructs the two-worker origin."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import time


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--repository", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--memory64", action="store_true")
    parser.add_argument("--memtag", action="store_true")
    parser.add_argument("--between-entries", action="store_true")
    parser.add_argument("--tls", action="store_true", help="Hold a real native TLS destructor across retirement")
    args = parser.parse_args()
    if "/docker-" not in Path("/proc/self/cgroup").read_text():
        raise RuntimeError("resource cgroup required")
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    address = "i64" if args.memory64 else "i32"
    tag = (3 << (56 if args.memory64 else 28)) if args.memtag else 0
    extend = "i64.extend_i32_u" if args.memory64 else ""
    wat = out / "worker-exit.wat"
    wasm = wat.with_suffix(".wasm")
    wat.write_text(f'''(module
      (memory {"i64 " if args.memory64 else ""}1 1 shared)
      (func $initialize)
      (func $worker (result i32) (local $id i32) (local $n i32) (local $slot {address})
        {address}.const 0 i32.const 1 i32.atomic.rmw.add local.set $id
        local.get $id i32.const 4 i32.mul {extend} {address}.const {tag+16} {address}.add local.set $slot
        loop $barrier
          {address}.const 0 i32.atomic.load i32.const 2 i32.ne br_if $barrier
        end
        loop $work
          local.get $slot i32.const 1 i32.atomic.rmw.add drop
          local.get $id i32.eqz if
            local.get $n i32.const 1 i32.add local.tee $n i32.const 256 i32.eq
            if i32.const 7 return end
          end
          br $work
        end i32.const 0))
''')
    subprocess.run(["/usr/bin/wat2wasm", "--enable-threads", "--enable-memory64", "--debug-names",
                    str(wat), "-o", str(wasm)], check=True)
    env = {**os.environ, "UWVM_TEST_WORKER_EXIT": "1"}
    if args.between_entries:
        env["UWVM_TEST_BETWEEN_ENTRIES"] = "1"
    if args.tls:
        if args.between_entries:
            raise RuntimeError("callback and TLS cases are separate")
        env["UWVM_TEST_WORKER_TLS"] = "1"
    if args.memtag:
        sys.path.insert(0, str(args.repository.resolve() / "test/0086.memtag"))
        import vm_matrix

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
        while at < len(original):
            section = original[at]
            length, start = read_u(original, at + 1)
            body = original[start:start + length]
            at = start + length
            if section == 10:
                count, cursor = read_u(body, 0)
                size, first = read_u(body, cursor)
                code = body[first:first + size]
                if code[0] != 0:
                    raise RuntimeError("initializer must have no local declarations")
                code = code[:1] + vm_matrix.Program(args.memory64).tags(16, 16, 3) + code[1:]
                body = vm_matrix.u(count) + vm_matrix.u(len(code)) + code + body[first + size:]
            encoded += bytes([section]) + vm_matrix.u(len(body)) + body
        wasm.write_bytes(encoded)
        env["UWVM_TEST_REQUIRE_MTE"] = "1"
    runner = json.loads(args.runner_json.read_text()) if args.runner_json else []
    argv = [*runner, str(args.binary.resolve()), str(wasm), "4000"]
    (out / "argv.json").write_text(json.dumps(argv, indent=2))
    begin = time.monotonic()
    with (out / "run.log").open("w") as log:
        result = subprocess.run(argv, cwd=out, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=300)
    summary = dict(code=result.returncode, seconds=time.monotonic()-begin,
                   binary_sha256=hashlib.sha256(args.binary.read_bytes()).hexdigest(),
                   wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest())
    (out / "results.json").write_text(json.dumps(summary, indent=2))
    text = (out / "run.log").read_text()
    if result.returncode or text.count("memory_equal=1") != 2 or "EXIT genuine native entry returned" not in text:
        raise RuntimeError(text)
    if args.between_entries and "EXIT between-entry callback refused without retiring" not in text:
        raise RuntimeError(text)
    if args.tls and "EXIT pending native TLS join prevented publication" not in text:
        raise RuntimeError(text)
    print("native worker exit and repeated replay PASS", summary, flush=True)


if __name__ == "__main__":
    main()
