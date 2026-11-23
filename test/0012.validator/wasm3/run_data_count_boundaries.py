#!/usr/bin/env python3
"""All full translators must require Data Count for data-index instructions.

Fixtures are encoded directly so missing/mismatched sections are not repaired by
an assembler. Use --vm for a native binary or a cross-architecture runner.
"""
import argparse
import json
import subprocess
from pathlib import Path


def uleb(value):
    result = bytearray()
    while value >= 128:
        result.append((value & 127) | 128)
        value >>= 7
    result.append(value)
    return bytes(result)


def section(kind, body):
    return bytes([kind]) + uleb(len(body)) + body


def cases():
    for operation in ("init", "drop"):
        for wide in (False, True):
            for shared in (False, True):
                for dead in (False, True):
                    for memory_index in (0, 1):
                        for name, count, index, valid in (
                            ("present", 1, 0, True),
                            ("missing", None, 0, False),
                            ("zero-mismatch", 0, 0, False),
                            ("excess-mismatch", 2, 0, False),
                            ("out-of-range", 1, 1, False),
                        ):
                            image = b"\0asm\x01\0\0\0"
                            image += section(1, b"\x01\x60\0\0")
                            image += section(3, b"\x01\0")
                            flags = (4 if wide else 0) | (3 if shared else 1)
                            memory = bytes([flags, 0, 0])
                            image += section(5, uleb(memory_index + 1) + memory * (memory_index + 1))
                            image += section(7, b"\x01\x06_start\0\0")
                            if count is not None:
                                image += section(12, uleb(count))
                            expression = b"\x02\x40\x0c\0" if dead else b""
                            if operation == "init":
                                expression += bytes([0x42 if wide else 0x41, 0, 0x41, 0, 0x41, 0, 0xfc, 8])
                                expression += uleb(index) + uleb(memory_index)
                            else:
                                expression += b"\xfc\x09" + uleb(index)
                            if dead:
                                expression += b"\x0b"
                            expression += b"\x0b"
                            body = b"\0" + expression
                            image += section(10, b"\x01" + uleb(len(body)) + body)
                            image += section(11, b"\x01\x01\0")  # One empty passive segment.
                            label = f"{operation}-m{64 if wide else 32}-shared{int(shared)}-dead{int(dead)}-memory{memory_index}-{name}"
                            yield label, image, valid
    # Passive segments by themselves do not require Data Count. Only instructions
    # referring to data indices introduce the requirement.
    for wide in (False, True):
        image = b"\0asm\x01\0\0\0" + section(1, b"\x01\x60\0\0")
        image += section(3, b"\x01\0") + section(5, bytes([1, 5 if wide else 1, 0, 0]))
        image += section(7, b"\x01\x06_start\0\0") + section(10, b"\x01\x02\0\x0b")
        image += section(11, b"\x01\x01\0")
        yield f"unused-passive-m{64 if wide else 32}", image, True


def run(argv, timeout):
    try:
        q = subprocess.run(list(map(str, argv)), capture_output=True, text=True, timeout=timeout)
        return dict(argv=list(map(str, argv)), exit=q.returncode, stdout=q.stdout, stderr=q.stderr)
    except subprocess.TimeoutExpired:
        return dict(argv=list(map(str, argv)), exit="timeout")


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--vm", type=Path, required=True)
    p.add_argument("--output", type=Path, required=True)
    p.add_argument("--wasm-tools", type=Path)
    p.add_argument("--wabt", type=Path)
    p.add_argument("--wasmtime", type=Path)
    p.add_argument("--timeout", type=float, default=60)
    args = p.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    rows = []
    for name, image, valid in cases():
        path = args.output / (name + ".wasm")
        path.write_bytes(image)
        commands = [(engine, [args.vm, "-R" + engine, "-Rct", "0", "-Rllvm-cache-path", "disable",
                               "--debug-fault-action", "exit", "--run", path]) for engine in ("int", "aot")]
        if args.wasm_tools:
            commands.append(("wasm-tools", [args.wasm_tools, "validate", path]))
        if args.wabt:
            commands.append(("wabt", [args.wabt, "--enable-all", path]))
        if args.wasmtime:
            commands.append(("wasmtime", [args.wasmtime, "run", "-C", "cache=n", "-W",
                                          "threads=y,shared-memory=y,multi-memory=y,memory64=y", path]))
        for tool, argv in commands:
            result = run(argv, args.timeout)
            if tool in ("int", "aot"):
                passed = result['exit'] == (0 if valid else 127)
            elif valid:
                passed = result['exit'] == 0
            else:
                passed = isinstance(result['exit'], int) and result['exit'] > 0
                if tool == "wasmtime":
                    passed = passed and any(message in result.get('stderr', '') for message in ("failed to compile", "failed to parse WebAssembly module"))
            rows.append(dict(name=name, tool=tool, valid=valid, passed=passed, **result))
    result = dict(complete=True, fixtures=len(list(cases())), checks=len(rows),
                  failed=sum(not r['passed'] for r in rows), rows=rows)
    (args.output / "results.json").write_text(json.dumps(result, indent=2) + "\n")
    print(f"{result['checks']} checks, {result['failed']} failures")
    return bool(result['failed'])


if __name__ == "__main__":
    raise SystemExit(main())
