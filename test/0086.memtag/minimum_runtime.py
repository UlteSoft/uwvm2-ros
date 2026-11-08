#!/usr/bin/env python3
"""Real WAT/VM checks for minimum-bound optimization and observable growth.

The standard fixtures also run in Wasmtime. No host imports or permissions are
needed; the extension-specific regressions remain in vm_matrix.py.
"""
import argparse
import hashlib
import json
import subprocess
from pathlib import Path


def grow_case(bits, shared, initial):
    kind = f"i{bits}"
    def constant(value): return f"({kind}.const {value})"
    def check(value, expected):
        return f"(if ({kind}.ne {value} {constant(expected)}) (then unreachable))"
    def check32(value, expected):
        return f"(if (i32.ne {value} (i32.const {expected})) (then unreachable))"
    old = initial * 65536
    body = [check("(memory.size)", initial)]
    if initial:
        body += [f"(i32.store {constant(0)} (i32.const 12345))"]
    body += [check(f"(memory.grow {constant(1)})", initial),
             check("(memory.size)", initial + 1),
             check32(f"(i32.load {constant(old)})", 0),
             f"(i32.store {constant(old)} (i32.const 98765))",
             check32(f"(i32.load {constant(old)})", 98765)]
    if initial:
        body += [check32(f"(i32.load {constant(0)})", 12345)]
    body += [check(f"(memory.grow {constant(0)})", initial + 1),
             check("(memory.size)", initial + 1),
             check(f"(memory.grow {constant(3)})", -1),
             check("(memory.size)", initial + 1),
             check32(f"(i32.load {constant(old)})", 98765)]
    declaration = f"(memory {'i64 ' if bits == 64 else ''}{initial} 3{' shared' if shared else ''})"
    return f'(module {declaration} (func (export "_start") {" ".join(body)}))'


def owner_case(bits, left, right, selected, address):
    kind = f"i{bits}"
    memories = " ".join(f"(memory $m{i} {'i64 ' if bits == 64 else ''}{n} 3)"
                        for i, n in enumerate([left, right]))
    return f'(module {memories} (func (export "_start") (drop (i32.load $m{selected} ({kind}.const {address})))))'


def growth_oob(bits):
    kind = f"i{bits}"
    return f"""(module (memory {'i64 ' if bits == 64 else ''}1 2)
    (func (export "_start")
      (if ({kind}.ne (memory.grow ({kind}.const 1)) ({kind}.const 1)) (then unreachable))
      (drop (i32.load ({kind}.const 131070)))))"""


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--vm", type=Path, required=True)
    parser.add_argument("--wat2wasm", default="wat2wasm")
    parser.add_argument("--wasmtime", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    cases = []
    for bits in (32, 64):
        for shared in (False, True):
            for initial in (0, 1):
                cases.append((f"grow-{bits}-{int(shared)}-{initial}", grow_case(bits, shared, initial), False))
        for left, right, selected, address, denied in [
                (2, 0, 1, 0, True), (2, 1, 1, 65536, True),
                (1, 2, 1, 65536, False), (0, 2, 0, 0, True)]:
            cases.append((f"owner-{bits}-{left}-{right}", owner_case(bits, left, right, selected, address), denied))
        cases.append((f"grown-oob-{bits}", growth_oob(bits), True))
    rows = []
    for name, text, denied in cases:
        source = args.out / (name + ".wat")
        wasm = args.out / (name + ".wasm")
        source.write_text(text + "\n")
        subprocess.run([args.wat2wasm, "--enable-memory64", "--enable-multi-memory",
                        "--enable-threads", str(source), "-o", str(wasm)], check=True)
        commands = [(engine, [str(args.vm), "--runtime-" + engine,
                    "--runtime-llvm-jit-cache-path", "disable", "--wasm-feature-enable-memory64",
                    "--wasm-feature-enable-multi-memory", "--wasm-feature-enable-threads",
                    "--run", str(wasm)]) for engine in ("int", "aot")]
        if args.wasmtime:
            commands.append(("wasmtime", [str(args.wasmtime), "run", "-C", "cache=n",
                             "-W", "memory64=y", "-W", "multi-memory=y", "-W", "threads=y",
                             "-W", "shared-memory=y", str(wasm)]))
        for engine, command in commands:
            result = subprocess.run(command, capture_output=True, timeout=90)
            output = (result.stdout + result.stderr).decode(errors="replace")
            (args.out / (name + "-" + engine + ".log")).write_text(output)
            if denied:
                correct_exit = result.returncode != 0 if engine == "wasmtime" else result.returncode == 121
                passed = correct_exit and "out of bounds" in output.lower()
            else:
                passed = result.returncode == 0
            rows.append(dict(name=name, engine=engine, command=command, exit=result.returncode,
                             expected="bounds" if denied else "success", passed=passed,
                             wasm_sha256=hashlib.sha256(wasm.read_bytes()).hexdigest()))
            (args.out / "report.json").write_text(json.dumps(rows, indent=2))
            print(name, engine, result.returncode, "PASS" if passed else "FAIL", flush=True)
    raise SystemExit(any(not row["passed"] for row in rows))


if __name__ == "__main__":
    main()
