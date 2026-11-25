"""Check real threaded ELF admission boundaries using the host's emitted object."""
import argparse
import re
import subprocess
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", type=Path, required=True)
    parser.add_argument("--wasm-tools", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    work = args.out.resolve()
    work.mkdir(parents=True, exist_ok=True)

    def assemble(name, source):
        wat = work / (name + ".wat")
        wasm = wat.with_suffix(".wasm")
        wat.write_text(source)
        subprocess.run([args.wasm_tools.resolve(), "parse", wat, "-o", wasm], check=True)
        return wasm

    owner = assemble("owner", '(module (memory (export "memory") 1 1 shared))')
    guest = assemble("guest", """(module
      (import "env" "memory" (memory 1 1 shared))
      (import "wasi" "thread-spawn" (func (param i32) (result i32)))
      (func (export "_start") (i32.atomic.store (i32.const 0) (i32.const 1)))
      (func (export "wasi_thread_start") (param i32 i32)))""")

    def run(name, budget, status):
        command = [args.vm.resolve(), "-Raot", "-Rct", "0", "-Rllvm-cache-path", "disable",
                   "--wasm-policy-global", "runtime.llvm.policy", "fast-compile", "--debug-fault-action", "exit",
                   "--wasm-policy-global", "parser.baseline", "wasm3", "--wasm-policy-global", "parser.threads", "on", "--wasm-policy-global", "runtime.memtag-checks", "off",
                   "--wasi-threads", "1", "1", "1048576", "16777216", budget, "65536",
                   "--wasm-set-main-module-name", "main", "--wasip1-single-create", "main",
                   "--wasip1-single-enable", "main", "--wasithread-single-create", "main",
                   "--wasithread-single-enable", "main", "--wasithread-single-interface", "main",
                   "thread-spawn", "allow", "--wasithread-single-limit", "main", "max-extra-threads", "1",
                   "--wasithread-single-limit", "main", "max-spawns", "1",
                   "--wasm-preload-library", owner, "env", "--run", guest]
        result = subprocess.run(command, cwd=work, capture_output=True, timeout=90)
        (work / (name + ".err")).write_bytes(result.stderr)
        if result.returncode != status or result.stdout or (status == 0 and result.stderr):
            raise RuntimeError(f"{name} exited {result.returncode}: {result.stdout!r} {result.stderr!r}")
        return result.stderr.decode()

    budget = "67108864"
    run("default", budget, 0)
    diagnostic = run("measure", budget + ",relocations=1", 126)
    match = re.search(r"\(relocation count exceeds host limit\) totals-complete=1 sections=(\d+) "
                      r"relocations=(\d+) symbols=(\d+) relocation-limit=1 charged-bytes=(\d+) "
                      r"arena-bytes=(\d+) code-limit=67108864", diagnostic)
    if match is None:
        raise RuntimeError(diagnostic)
    sections, relocations, symbols, charge, arena = map(int, match.groups())
    if min(sections, relocations, symbols) < 2 or charge <= relocations * 512 or arena != (charge + 65535) // 65536 * 65536:
        raise RuntimeError(diagnostic)
    count_failure = run("one-relocation-short", f"{arena},relocations={relocations - 1}", 126)
    if f"relocations={relocations} symbols={symbols} relocation-limit={relocations - 1}" not in count_failure:
        raise RuntimeError(count_failure)
    byte_failure = run("one-byte-short", f"{arena - 1},relocations={relocations}", 126)
    if "(native arena byte charge)" not in byte_failure or f"arena-bytes={arena} code-limit={arena - 1}" not in byte_failure:
        raise RuntimeError(byte_failure)
    run("inclusive", f"{arena},relocations={relocations}", 0)


if __name__ == "__main__":
    main()
