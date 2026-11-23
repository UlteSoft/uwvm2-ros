#!/usr/bin/env python3
"""Exercise atomic trap observability and MTE alignment admission through the real CLI.

The caller selects the resource cgroup. All generated modules and logs go under
--out. No compiler, runtime or toolchain is installed or modified by this test.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import importlib.util
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location(
    "atomic_matrix_encoding", ROOT / "test/0086.memtag/vm_matrix.py")
m = importlib.util.module_from_spec(spec)
spec.loader.exec_module(m)


def atomic(op, width, index=0, offset=0):
    return b'\xfe' + m.u(op) + m.u((width.bit_length() - 1) | (64 if index else 0)) + (
        m.u(index) if index else b'') + m.u(offset)


def module(code, wide, shared, indexed):
    address_type = 0x7e if wide else 0x7f
    body = b'\x01\x01' + bytes([address_type]) + code + b'\x0b'
    memories = (b'\x01\x00\x01' if indexed else b'') + m.u(
        1 + (4 if wide else 0) + (2 if shared else 0)) + m.u(1) + m.u(2)
    return (b'\0asm\x01\0\0\0' + m.sec(1, b'\x01\x60\0\0') +
            m.sec(3, b'\x01\0') + m.sec(5, m.u(2 if indexed else 1) + memories) +
            m.sec(7, b'\x01\x06_start\0\0') + m.sec(10, b'\x01' + m.u(len(body)) + body))


def operation(op, address, address_bits, index):
    i = (op - 0x10) % 7
    bits = 64 if i in (1, 4, 5, 6) else 32
    width = (4, 8, 1, 2, 1, 2, 4)[i]
    code = m.c(address, address_bits)
    if op >= 0x48:
        code += m.c(7, bits)
    if op >= 0x17:
        if op < 0x1e:
            value = 7
        else:
            family = (op - 0x1e) // 7
            value = -1 if family == 2 else 7 if family >= 5 else 0
            # Verify narrowing before recognizing a neutral operation.
            if value == 0 and bits > 8 * width:
                value = 1 << (8 * width)
        code += m.c(value, bits)
    code += atomic(op, width, index)
    if not 0x17 <= op < 0x1e:
        code += b'\x1a'
    return code, width


def fixtures(out):
    cases = []
    for wide in (False, True):
        bits = 64 if wide else 32
        for shared in (False, True):
            for op in range(0x10, 0x4f):
                index = (op - 0x10) & 1
                for kind in ("valid", "oob"):
                    init = m.c(0, bits) + m.c(7, 64) + m.mem(0x37, index=index)
                    code, width = operation(op, 0 if kind == "valid" else 65536, bits, index)
                    if kind == "valid":
                        code += m.assertion(m.c(0, bits) + m.mem(0x29, index=index), 7, 64)
                    name = f"atomic-{bits}-{int(shared)}-{op:02x}-{kind}"
                    cases.append((name, module(init + code, wide, shared, index),
                                  0 if kind == "valid" else 121,
                                  "" if kind == "valid" else "memory access out of bounds", []))
            # All families plus narrow and wide values, both alignment and tag
            # faults, and an access that is simultaneously unaligned and OOB.
            for op in (0x10, 0x11, 0x13, 0x17, 0x18, 0x1e, 0x25, 0x2c, 0x33, 0x3a, 0x41, 0x48):
                index = op & 1
                for kind in ("tag", "unaligned-tag", "unaligned-oob"):
                    address = (1 << (56 if wide else 28))
                    if kind == "unaligned-tag":
                        address |= 1
                    if kind == "unaligned-oob":
                        address = 65537
                    code, width = operation(op, address, bits, index)
                    code = m.op(0x20, index) + b'\x1a' + code
                    name = f"mte-{bits}-{int(shared)}-{op:02x}-{kind}"
                    fault = 122 if kind == "tag" else 121
                    reason = "memory tag mismatch" if fault == 122 else "unaligned atomic memory access"
                    cases.append((name, module(code, wide, shared, index), fault, reason, []))
                    if kind != "tag":
                        cases.append((name + "-log", module(code, wide, shared, index), fault,
                                      reason, ["--runtime-memtag-mode", "async",
                                               "--runtime-memtag-action", "silent-log", "err"]))
            for op, width in ((0, 4), (1, 4), (2, 8)):
                for kind in ("unaligned-tag", "unaligned-oob"):
                    pointer = ((1 << (56 if wide else 28)) | 1) if kind.endswith("tag") else 65537
                    code = m.op(0x20) + b'\x1a' + m.c(pointer, bits)
                    code += m.c(0, 64 if op == 2 else 32)
                    if op:
                        code += m.c(0, 64)
                    code += atomic(op, width) + b'\x1a'
                    name = f"mte-wait-{bits}-{int(shared)}-{op}-{kind}"
                    cases.append((name, module(code, wide, shared, False), 121,
                                  "unaligned atomic memory access", []))
    result = []
    for name, raw, status, reason, options in cases:
        file = out / (name + ".wasm")
        file.write_bytes(raw)
        result.append(dict(name=name, file=str(file), expected=status, reason=reason, options=options))
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--vm", type=Path)
    p.add_argument("--out", required=True, type=Path)
    p.add_argument("--jobs", type=int, default=4)
    p.add_argument("--generate-only", action="store_true")
    args = p.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    cases = fixtures(args.out)
    (args.out / "manifest.json").write_text(json.dumps(cases, indent=2) + "\n")
    if args.generate_only:
        return
    if args.vm is None:
        p.error("--vm is required")
    def run(item):
        case, engine = item
        command = [str(args.vm.resolve()), "-R" + engine, "-Rct", "0",
                   "--debug-fault-action", "exit", "-Rllvm-cache-path", "disable",
                   "--wasm-set-start-func", "0"] + case["options"] + ["--run", case["file"]]
        try:
            q = subprocess.run(command, capture_output=True, timeout=45)
            data = (q.stdout + q.stderr).decode("utf8", "replace")
            status = q.returncode
        except subprocess.TimeoutExpired as exc:
            status = "timeout"
            data = ((exc.stdout or b"") + (exc.stderr or b"")).decode("utf8", "replace")
        (args.out / (case["name"] + "-" + engine + ".log")).write_text(data)
        passed = status == case["expected"] and (not case["reason"] or case["reason"] in data)
        # Misalignment precedes tag inspection, including report-only delivery.
        if "unaligned" in case["name"] and "Memory tag mismatch" in data:
            passed = False
        return dict(name=case["name"], engine=engine, exit=status, passed=passed, command=command)
    rows = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        for job in as_completed([pool.submit(run, (c, e)) for c in cases for e in ("int", "aot")]):
            rows.append(job.result())
    rows.sort(key=lambda x: (x["name"], x["engine"]))
    (args.out / "results.json").write_text(json.dumps(rows, indent=2) + "\n")
    print(json.dumps(dict(checks=len(rows), failed=sum(not r["passed"] for r in rows))))
    raise SystemExit(any(not r["passed"] for r in rows))


if __name__ == "__main__":
    main()
