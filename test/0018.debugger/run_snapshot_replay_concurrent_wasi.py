#!/usr/bin/env python3
"""Real two-worker clock polling; replay must not reenter the native poll provider."""
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
    parser = argparse.ArgumentParser()
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--runner-json", type=Path)
    parser.add_argument("--repository", type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument("--memory64", action="store_true")
    parser.add_argument("--memtag", action="store_true")
    parser.add_argument("--backend", choices=("software", "hardware"), default="software")
    parser.add_argument("--delivery", choices=("sync", "async"), default="sync")
    parser.add_argument("--report", choices=("crash", "log"), default="crash")
    parser.add_argument("--both-poll", action="store_true")
    parser.add_argument("--io", action="store_true", help="Concurrent real pipe reads and stdout writes")
    parser.add_argument("--socket", choices=("fd", "recv"), help="Real inherited socket; test fd_read or sock_recv")
    parser.add_argument("--quota", action="store_true", help="Explicit random window-call limit with synchronous waiting")
    parser.add_argument("--scalar", action="store_true", help="Concurrent randomness, clocks, arguments, environment and yield")
    parser.add_argument("--metadata", action="store_true", help="Contend fdstat_get with a read of the same descriptor")
    parser.add_argument("--empty", action="store_true", help="Zero-vector read/write result publication")
    parser.add_argument("--grow", action="store_true", help="Grow shared memory while the peer performs real I/O")
    parser.add_argument("--host-tag-mismatch", action="store_true",
                        help="Log actual MTE violations inside random_get; replay must retain every diagnostic")
    parser.add_argument("--tamper", choices=("readonly", "failed", "begin"))
    args = parser.parse_args()
    if args.host_tag_mismatch:
        require(args.memtag and args.report == "log", "mismatch fixture requires MTE log mode")
        args.scalar = True
    if args.quota:
        args.scalar = True
    if args.socket or args.metadata or args.empty or args.grow:
        args.io = True
    require("/docker-" in Path("/proc/self/cgroup").read_text(), "resource cgroup required")
    require(args.memtag or (args.backend == "software" and args.delivery == "sync" and args.report == "crash"),
            "MTE policy options require actual MTE instructions")
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=False)
    binary = args.binary.resolve()
    runner = json.loads(args.runner_json.read_text()) if args.runner_json else []
    address = "i64" if args.memory64 else "i32"
    tag = (3 << (56 if args.memory64 else 28)) if args.memtag else 0
    extend = "i64.extend_i32_u" if args.memory64 else ""
    name = "poll_oneoff_wasm64" if args.memory64 else "poll_oneoff"
    selected = "i32.const 1" if args.both_poll else "local.get $id i32.eqz"
    wat = out / "concurrent.wat"
    wasm = wat.with_suffix(".wasm")
    # Each worker owns one 128-byte slot: subscription[48], event[32], count[8].
    # Only the selected poll workers use these slots; the arithmetic worker
    # touches a separate location in the same real shared memory.
    wat.write_text(f'''(module
      (import "wasi_snapshot_preview1" "{name}"
        (func $poll (param {address} {address} {address} {address}) (result i32)))
      (memory {"i64 " if args.memory64 else ""}1 1 shared)
      (func $initialize)
      (func $worker (result i32) (local $id i32) (local $slot {address})
        {address}.const 0 i32.const 1 i32.atomic.rmw.add local.set $id
        local.get $id i32.const 128 i32.mul {extend} {address}.const {tag+128} {address}.add local.set $slot
        loop $work
          {selected} if
            local.get $slot i64.const 123 i64.store
            local.get $slot i32.const 0 i32.store8 offset=8
            local.get $slot i32.const 1 i32.store offset=16
            local.get $slot i64.const 2000000 i64.store offset=24
            local.get $slot i64.const 1 i64.store offset=32
            local.get $slot i32.const 0 i32.store16 offset=40
            local.get $slot {address}.const 0 {address}.store offset=96
            local.get $slot local.get $slot {address}.const 64 {address}.add
            {address}.const 1 local.get $slot {address}.const 96 {address}.add call $poll
            if unreachable end
            local.get $slot {address}.load offset=96 {address}.const 1 {address}.ne if unreachable end
          else
            {address}.const 32 i32.const 1 i32.atomic.rmw.add drop
          end
          br $work
        end i32.const 0))
''')
    if args.io:
        suffix = "_wasm64" if args.memory64 else ""
        read_name = "sock_recv" if args.socket == "recv" else "fd_read"
        read_extra_types = f" i32 {address}" if args.socket == "recv" else ""
        read_call = (f"i32.const 0 {address}.const 96 {address}.const 1 i32.const 0 {address}.const 192 {address}.const 224"
                     if args.socket == "recv" else f"i32.const 0 {address}.const 96 {address}.const 1 {address}.const 192")
        wat.write_text(f'''(module
          (import "wasi_snapshot_preview1" "{read_name}{suffix}" (func $read (param i32 {address} {address}{read_extra_types} {address}) (result i32)))
          (import "wasi_snapshot_preview1" "fd_write{suffix}" (func $write (param i32 {address} {address} {address}) (result i32)))
          {f'(import "wasi_snapshot_preview1" "fd_fdstat_get{suffix}" (func $stat (param i32 {address}) (result i32)))' if args.metadata else ''}
          (memory {"i64 " if args.memory64 else ""}1 {3 if args.grow else 1} shared)
          (data ({address}.const 64) "G")
          (func $initialize
            {address}.const 80 {address}.const {tag+64} {address}.store
            {address}.const {88 if args.memory64 else 84} {address}.const 1 {address}.store
            {address}.const 96 {address}.const {tag+128} {address}.store
            {address}.const {104 if args.memory64 else 100} {address}.const 1 {address}.store)
          (func $worker (result i32) (local $id i32) {'(local $round i32)' if args.grow else ''}
            {address}.const 0 i32.const 1 i32.atomic.rmw.add local.set $id
            loop $work
              local.get $id i32.eqz if
                {address}.const 192 {address}.const 99 {address}.store
                {read_call.replace(f'{address}.const 1',f'{address}.const 0',1) if args.empty else read_call} call $read if unreachable end
                {address}.const 192 {address}.load {address}.const {0 if args.empty else 1} {address}.ne if unreachable end
              else
                {f'i32.const 0 {address}.const 256 call $stat if unreachable end' if args.metadata else ''}
                {address}.const 208 {address}.const 99 {address}.store
                i32.const 1 {address}.const 80 {address}.const {0 if args.empty else 1} {address}.const 208 call $write if unreachable end
                {address}.const 208 {address}.load {address}.const {0 if args.empty else 1} {address}.ne if unreachable end
                {f'local.get $round i32.const 1 i32.add local.tee $round i32.const 128 i32.eq if {address}.const 2 memory.grow {address}.const 1 {address}.ne if unreachable end end' if args.grow else ''}
              end
              br $work
            end i32.const 0))
''')
    if args.scalar:
        require(not args.io, "scalar and IO fixtures are independent")
        suffix = "_wasm64" if args.memory64 else ""
        imports = [f'(import "wasi_snapshot_preview1" "{name}{suffix}" (func ${name} (param {address} {address}) (result i32)))'
                   for name in ("args_get", "args_sizes_get", "environ_get", "environ_sizes_get", "random_get")]
        imports += [f'(import "wasi_snapshot_preview1" "clock_res_get{suffix}" (func $resolution (param i32 {address}) (result i32)))',
                    f'(import "wasi_snapshot_preview1" "clock_time_get{suffix}" (func $time (param i32 i64 {address}) (result i32)))',
                    f'(import "wasi_snapshot_preview1" "sched_yield{suffix}" (func $yield (result i32)))']
        def pointer(offset):
            return f"local.get $slot {address}.const {offset} {address}.add"
        wat.write_text(f'''(module
          {chr(10).join(imports)}
          (memory {"i64 " if args.memory64 else ""}1 1 shared)
          (func $initialize)
          (func $worker (result i32) (local $slot {address})
            {address}.const 0 i32.const 1 i32.atomic.rmw.add
            i32.const 1024 i32.mul {extend} {address}.const {tag+1024} {address}.add local.set $slot
            loop $work
              {pointer(0)} {pointer(8)} call $args_sizes_get if unreachable end
              {pointer(0)} {address}.load {address}.const 1 {address}.ne if unreachable end
              {pointer(32)} {pointer(64)} call $args_get if unreachable end
              {pointer(16)} {pointer(24)} call $environ_sizes_get if unreachable end
              {pointer(16)} {address}.load {address}.const 1 {address}.ne if unreachable end
              {pointer(128)} {pointer(160)} call $environ_get if unreachable end
              {pointer(256)} {address}.const 64 call $random_get if unreachable end
              i32.const 1 {pointer(352)} call $resolution if unreachable end
              i32.const 1 i64.const 0 {pointer(360)} call $time if unreachable end
              call $yield if unreachable end
              br $work
            end i32.const 0))
''')
    if args.host_tag_mismatch:
        text = wat.read_text()
        before = f"{pointer(256)} {address}.const 64 call $random_get"
        after = f"{pointer(256)} {address}.const {(0x10 << 56) if args.memory64 else (1 << 28)} {address}.xor {address}.const 64 call $random_get"
        require(text.count(before) == 1, "exact imported random-buffer marker")
        wat.write_text(text.replace(before, after))
    subprocess.run(["/usr/bin/wat2wasm", "--enable-threads", "--enable-memory64", "--debug-names", str(wat), "-o", str(wasm)], check=True)
    env = {**os.environ, "UWVM_TEST_WASI_IO" if args.io else "UWVM_TEST_WASI_POLL": "1"}
    env.update(UWVM_TEST_MEMTAG_BACKEND=args.backend,
               UWVM_TEST_MEMTAG_DELIVERY=args.delivery, UWVM_TEST_MEMTAG_REPORT=args.report)
    if args.scalar:
        env["UWVM_TEST_WASI_SCALAR"] = "1"
    if args.quota:
        env["UWVM_TEST_WASI_QUOTA"] = "1"
    if args.socket:
        env["UWVM_TEST_WASI_SOCKET"] = "1"
    if args.empty:
        env["UWVM_TEST_WASI_EMPTY"] = "1"
    if args.metadata:
        env["UWVM_TEST_WASI_METADATA"] = "1"
    if args.grow:
        env["UWVM_TEST_WASI_GROW"] = "1"
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
                require(code[0] == 0, "initializer has no local declarations")
                tags = (vm_matrix.Program(args.memory64).tags(64, 16, 3) + vm_matrix.Program(args.memory64).tags(128, 16, 3)
                        if args.io else vm_matrix.Program(args.memory64).tags(1024, 2048, 3) if args.scalar
                        else vm_matrix.Program(args.memory64).tags(128, 256, 3))
                code = code[:1] + tags + code[1:]
                body = vm_matrix.u(count) + vm_matrix.u(len(code)) + code + body[first + size:]
            encoded += bytes([section]) + vm_matrix.u(len(body)) + body
        wasm.write_bytes(encoded)
        env["UWVM_TEST_REQUIRE_MTE"] = "1"
    if args.host_tag_mismatch:
        env["UWVM_TEST_REQUIRE_REPORTS"] = "1"
    if args.tamper:
        env["UWVM_TEST_TAMPER_HOST"] = args.tamper
    argv = [*runner, str(binary), str(wasm), "12000" if args.grow else "3000"]
    (out / "argv.json").write_text(json.dumps(argv, indent=2))
    begin = time.monotonic()
    with (out / "run.log").open("w") as log, (out / "guest.stdout").open("w") as guest:
        # Each replay also rebuilds the actual native world. Cross-architecture
        # emulation must not share the native wall-time ceiling; this changes
        # only the harness watchdog, never a guest/provider timeout or result.
        result = subprocess.run(argv, cwd=out, env=env, stdout=guest if args.io else log,
                                stderr=log, timeout=900 if runner else 300)
    text = (out / "run.log").read_text()
    if args.io:
        text += (out / "guest.stdout").read_text()
    expected = 125 if args.tamper else 0
    summary = dict(code=result.returncode, expected=expected, seconds=time.monotonic()-begin,
                   mte_backend=args.backend, mte_delivery=args.delivery, mte_report=args.report,
                    wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest(),
                   binary_sha256=hashlib.sha256(binary.read_bytes()).hexdigest())
    (out / "results.json").write_text(json.dumps(summary, indent=2))
    require(result.returncode == expected, text)
    if args.tamper:
        require("replay" in text.lower(), text)
    else:
        require(text.count("memory_equal=1") == 2 and "WASI phases began=" in text and "replay PASS" in text, text)
        if args.grow:
            require("GROW recorded shared memory grew from one to three pages" in text, text)
    if args.host_tag_mismatch:
        def diagnostics(segment):
            result = Counter()
            for line in segment.splitlines():
                if line.startswith('{"schema":"uwvm.memtag.violation.v1"'):
                    row = json.loads(line)
                    require(row["pointer_tag"] == (19 if args.memory64 else 2) and row["allocation_tag"] == 3,
                            "complete memory32/memory64 tag evidence must survive replay")
                    row.pop("sequence")
                    result[json.dumps(row, sort_keys=True)] += 1
            return result
        after_start = text.split("RECORD start=", 1)[1]
        original, rest = after_start.split("RECORD end=", 1)
        first, rest = rest.split("MTE replay round=0", 1)
        second = rest.split("MTE replay round=1", 1)[0]
        expected_reports = diagnostics(original)
        require(expected_reports and diagnostics(first) == expected_reports and diagnostics(second) == expected_reports,
                "each replay must preserve every native-import violation and its source/tag metadata")
        require("MTE crash-policy replay refusal preserved original stop and generation" in text,
                "archive must not weaken current crash policy")
        summary["verified_import_reports_per_replay"] = sum(expected_reports.values())
        (out / "results.json").write_text(json.dumps(summary, indent=2))
    print("concurrent WASI replay PASS", summary, flush=True)


if __name__ == "__main__":
    main()
