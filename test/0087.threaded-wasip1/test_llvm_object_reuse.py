"""Check first and subsequent private ELF loads on a native Linux x86-64 VM."""
import argparse
import re
import subprocess
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("vm", "wrc", "wasm-tools", "out"):
        parser.add_argument("--" + name, required=True, type=Path)
    args = parser.parse_args()
    out = args.out.resolve()
    out.mkdir(parents=True, exist_ok=True)
    here = Path(__file__).resolve().parent
    replay = here.parent / "0050.high_level_languages/threaded_profile/execution-replay"

    def assemble(source):
        binary = out / (source.stem + ".wasm")
        subprocess.run([args.wasm_tools.resolve(), "parse", source, "-o", binary], check=True, timeout=60)
        return binary

    def owner(name, minimum, maximum):
        source = out / (name + ".wat")
        source.write_text(f'(module (memory (export "memory") {minimum} {maximum} shared))\n')
        return assemble(source)

    def run(guest, memory, hits, stdout=b"", wasip1=False, spawn_cap=16):
        command = [
            args.vm.resolve(), "--runtime-aot", "--runtime-compile-threads", "0",
            "--runtime-llvm-jit-cache-path", "disable", "--debug-fault-action", "exit", "--log-color", "disable",
            "--wasm-policy-global", "runtime.llvm.policy", "balanced", "--wasm-policy-global", "parser.baseline", "wasm3",
            "--wasm-policy-global", "parser.threads", "on", "--wasm-policy-global", "parser.memtag", "off",
            "--wasm-policy-global", "parser.gc", "on",
            "--wasm-policy-global", "parser.exceptions", "on", "--wasm-policy-global", "parser.table64", "on",
            "--wasi-threads", "4", "16", "8388608", "67108864", "67108864", "8388608",
            "--wasm-set-main-module-name", "main", "--wasithread-single-create", "main", "--wasithread-single-enable", "main",
            "--wasithread-single-interface", "main", "thread-spawn", "allow",
            "--wasithread-single-limit", "main", "max-extra-threads", "4", "--wasithread-single-limit", "main", "max-spawns", str(spawn_cap),
            "--wasm-preload-library", memory, "env",
        ]
        if wasip1:
            command += ["--wasip1-single-create", "main", "--wasip1-single-enable", "main",
                        "--wasip1-single-hide-host-api", "main", "--wasip1-single-noinherit-system-environment", "main"]
            for interface in ("args_get", "args_sizes_get", "clock_time_get", "fd_fdstat_get", "fd_write", "proc_exit", "sched_yield"):
                command += ["--wasip1-single-interface", "main", interface, "allow"]
            for interface in ("fd_close", "fd_seek"):
                command += ["--wasip1-single-interface", "main", interface, "deny"]
            for grant in ("args", "monotonic-clock", "stdio.metadata", "stdout.write", "proc-exit"):
                command += ["--wasip1-single-grant", "main", grant]
        else:
            command += ["--wasm-policy-global", "runtime.memtag-checks", "off"]
        result = subprocess.run(command + ["--log-verbose", "--run", guest], capture_output=True, timeout=120)
        guest.with_suffix(".stderr").write_bytes(result.stderr)
        guest.with_suffix(".stdout").write_bytes(result.stdout)
        outcomes = re.findall(rb"WASI Threads LLVM group object (hit|miss|fallback)\b", result.stderr)
        if hits is None:
            quiet = subprocess.run(command + ["--run", guest], capture_output=True, timeout=120)
            guest.with_suffix(".quiet.stderr").write_bytes(quiet.stderr)
            guest.with_suffix(".quiet.stdout").write_bytes(quiet.stdout)
            if quiet.returncode or quiet.stdout != stdout or quiet.stderr:
                raise RuntimeError(f"{guest.stem}: unexpected default output or status {quiet.returncode}")
            expected = [b"fallback"]
        else:
            expected = [b"miss"] + [b"hit"] * hits
        if result.returncode or result.stdout != stdout or outcomes != expected:
            raise RuntimeError(f"{guest.stem}: exit={result.returncode}, object decisions={outcomes}, see {guest.with_suffix('.stderr')}")
        print(guest.stem, *(decision.decode() for decision in outcomes))

    small = owner("owner", 1, 1)
    for fixture in ("exception-storage", "dynamic-tables"):
        run(assemble(replay / (fixture + ".wat")), small, 2)
    permission = out / "spawn-permission.wat"
    permission.write_text("""(module
      (import "env" "memory" (memory 1 1 shared))
      (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
      (func (export "wasi_thread_start") (param i32 i32)
        (i32.atomic.store (i32.const 0) (i32.const 1))
        (drop (memory.atomic.notify (i32.const 0) (i32.const 1))))
      (func (export "_start")
        (if (i32.le_s (call $spawn (i32.const 0)) (i32.const 0)) (then unreachable))
        (loop $wait (if (i32.eqz (i32.atomic.load (i32.const 0))) (then
          (drop (memory.atomic.wait32 (i32.const 0) (i32.const 0) (i64.const -1))) (br $wait))))
        (if (i32.ne (call $spawn (i32.const 1)) (i32.const -1)) (then unreachable))))""")
    run(assemble(permission), small, 1, spawn_cap=1)
    # More actual, mutable object bindings than the native symbol registry
    # admits. The original layout must still run and keep ordinary stderr clean.
    fallback = out / "binding-cap.wat"
    globals_ = " ".join(f"(global $g{i} (mut i32) (i32.const 0))" for i in range(260))
    writes = " ".join(f"(global.set $g{i} (i32.const {i}))" for i in range(260))
    fallback.write_text('(module (import "env" "memory" (memory 1 1 shared)) '
                        '(import "wasi" "thread-spawn" (func (param i32) (result i32))) ' + globals_ +
                        '(func (export "_start") ' + writes +
                        '(if (i32.ne (global.get $g259) (i32.const 259)) (then unreachable))) '
                        '(func (export "wasi_thread_start") (param i32 i32)))')
    run(assemble(fallback), small, None)
    guest = out / "pthread.wasm"
    subprocess.run([args.wrc.resolve(), "--target", "wasm32", "--core", "3.0", "--std", "c17", "-O2",
                    "--no-mte", "--no-eh", "--threads", "-mno-simd128", "-mno-bulk-memory-opt",
                    "--shared-memory-pages", "128", here / "pthread_object_reuse.c", "-o", guest], check=True, timeout=120)
    run(guest, owner("pthread-owner", 11, 128), 3, b"pthread TLS, stack and joins passed\n", wasip1=True)


if __name__ == "__main__":
    main()
