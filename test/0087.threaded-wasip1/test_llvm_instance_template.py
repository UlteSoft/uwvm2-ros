"""Exercise private worker rebinding and the exact-IR fallback on Linux x86-64."""
import argparse
from pathlib import Path
import subprocess


GUEST = """(module
  (import "env" "memory" (memory 1 2 shared))
  (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
  (global $tls (mut i32) (i32.const 7))
  (type $signature (func (result i32)))
  (table 1 funcref)
  (func $original (result i32) i32.const 9)
  (func $changed (result i32) i32.const 23)
  (elem (i32.const 0) func $original)
  (elem declare func $changed)
  (func $assert (param i32) (if (i32.eqz (local.get 0)) (then unreachable)))
  (func $read (result i32) i32.const 0 call_indirect (type $signature))
  (func (export "wasi_thread_start") (param i32 i32)
    (call $assert (i32.eq (global.get $tls) (i32.const 7)))
    (call $assert (i32.eq (call $read) (i32.const 9)))
    (global.set $tls (i32.const 31))
    (table.set (i32.const 0) (ref.func $changed))
    (call $assert (i32.eq (call $read) (i32.const 23)))
    (i32.atomic.store (i32.const 65536) (global.get $tls))
    (drop (memory.atomic.notify (i32.const 65536) (i32.const 1))))
  (func (export "_start")
    (call $assert (i32.eq (memory.grow (i32.const 1)) (i32.const 1)))
    (global.set $tls (i32.const 17))
    (call $assert (i32.gt_s (call $spawn (i32.const 0)) (i32.const 0)))
    (loop $wait (if (i32.eqz (i32.atomic.load (i32.const 65536))) (then
      (drop (memory.atomic.wait32 (i32.const 65536) (i32.const 0) (i64.const -1))) (br $wait))))
    (call $assert (i32.eq (i32.atomic.load (i32.const 65536)) (i32.const 31)))
    (call $assert (i32.eq (global.get $tls) (i32.const 17)))
    (call $assert (i32.eq (call $read) (i32.const 9)))
    OPTIONAL))"""


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("vm", "wasm-tools", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    args = parser.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)

    def assemble(name, wat):
        source = out / (name + ".wat")
        source.write_text(wat)
        binary = source.with_suffix(".wasm")
        subprocess.run([args.wasm_tools.resolve(), "parse", source, "-o", binary], check=True, timeout=60)
        return binary

    owner = assemble("owner", '(module (memory (export "memory") 1 2 shared))')
    for name, extra in (("ordinary", ""), ("reference-op", "(drop (ref.is_null (ref.null func)))")):
        guest = assemble(name, GUEST.replace("OPTIONAL", extra))
        command = [args.vm.resolve(), "-Raot", "-Rct", "0", "-Rllvm-cache-path", "disable",
                   "--debug-fault-action", "exit", "--log-color", "disable", "--log-verbose",
                   "--wasm-policy-global", "runtime.llvm.policy", "balanced",
                   "--wasm-policy-global", "parser.baseline", "wasm3",
                   "--wasm-policy-global", "parser.threads", "on",
                   "--wasi-threads", "2", "4", "8388608", "67108864", "67108864", "8388608",
                   "--wasm-set-main-module-name", "main", "--wasithread-single-create", "main",
                   "--wasithread-single-enable", "main", "--wasithread-single-interface", "main", "thread-spawn", "allow",
                   "--wasithread-single-limit", "main", "max-extra-threads", "2",
                   "--wasithread-single-limit", "main", "max-spawns", "4", "--wasm-preload-library", owner, "env", "--run", guest]
        result = subprocess.run(command, capture_output=True, timeout=90)
        log = out / (name + ".stderr")
        log.write_bytes(result.stderr)
        hits = [line for line in result.stderr.splitlines() if b"LLVM group object hit" in line]
        if (result.returncode or result.stdout or len(hits) != 1 or
                ((b"instance-template" in hits[0]) != (name == "ordinary"))):
            raise RuntimeError(f"{name} returned {result.returncode}, decisions {hits!r}, see {log}")
        print(name, hits[0].decode())


if __name__ == "__main__":
    main()
