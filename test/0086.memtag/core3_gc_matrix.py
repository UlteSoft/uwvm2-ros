#!/usr/bin/env python3
"""Actual-VM checks of the boundary between Core 3 managed objects and linear MTE.

Emit exact binary instructions: ordinary WAT tools do not understand WasmMTE.
No guest imports, network calls, or driver access are required. Generated modules
and logs belong in --out, outside the source tree. Collection witnesses are
separate from instruction-semantic checks and require the full LLVM backend.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import resource
import shlex
import subprocess

from vm_matrix import assertion, c, mem, op as packed_op, s, sec, u
from vm_diagnostics import fault_matches, human_fault


I32, I64, I8, I16, EQREF = b"\x7f", b"\x7e", b"\x78", b"\x77", b"\x6d"
DROP, END = b"\x1a", b"\x0b"
ENCODING = "packed"


def op(operation, memory=0):
    if ENCODING == "packed":
        return packed_op(operation, memory)
    word = 0x01010000 | operation
    padded = bytes(((word >> (7*i)) & 127) | (128 if i != 4 else 0) for i in range(5))
    return b"\xff" + padded + u(memory)


def gc(opcode, *immediates):
    return b"\xfb" + u(opcode) + b"".join(u(i) for i in immediates)


def ref_type(index):
    return b"\x63" + s(index, 33)


def ref_null(index):
    return b"\xd0" + s(index, 33)


def local(index, action=0x20):
    return bytes([action]) + u(index)


def struct_type(*fields):
    return b"\x5f" + u(len(fields)) + b"".join(t + b"\x01" for t in fields)


def array_type(storage):
    return b"\x5e" + storage + b"\x01"


def make_module(code, types=(), locals_=(), memory=None, table=None,
                data=None, elements=None, unused_tag=False):
    """Type zero and function zero are always the () -> () entry point."""
    result = b"\0asm\x01\0\0\0"
    result += sec(1, u(1 + len(types)) + b"\x60\0\0" + b"".join(types))
    result += sec(3, b"\x01\0")
    if table is not None:
        wide, count, element_type = table
        result += sec(4, b"\x01" + element_type + u(4 if wide else 0) + u(count))
    if memory is not None:
        result += sec(5, b"\x01" + u(5 if memory == 64 else 1) + u(1) + u(1))
    if unused_tag:
        result += sec(13, b"\x01\0\0")
    exports = [b"\x06_start\0\0"]
    if table is not None:
        exports.append(b"\x05roots\x01\0")
    result += sec(7, u(len(exports)) + b"".join(exports))
    if elements is not None:
        result += sec(9, b"\x01\x05" + EQREF + u(len(elements)) + b"".join(e + END for e in elements))
    if data is not None:
        result += sec(12, b"\x01")
    body = u(len(locals_)) + b"".join(b"\x01" + t for t in locals_) + code + END
    result += sec(10, b"\x01" + u(len(body)) + body)
    if data is not None:
        result += sec(11, b"\x01\x01" + u(len(data)) + data)
    return result


def pure_data_arrays():
    # These offsets select a DATA SEGMENT; this module has no linear memory.
    array = local(0)
    code = c(0) + c(4) + gc(9, 1, 0) + local(0, 0x21)
    code += assertion(array + gc(15), 4)
    code += assertion(array + c(1) + gc(13, 1), 0x22)
    code += array + c(1) + c(0) + c(2) + gc(18, 1, 0)
    code += array + c(0) + c(255) + c(2) + gc(16, 1)
    code += array + c(1) + array + c(0) + c(3) + gc(17, 1, 1)
    code += assertion(array + c(0) + gc(12, 1), -1)
    code += assertion(array + c(3) + gc(13, 1), 0x22)
    code += b"\xfc\x09\0"  # data.drop
    code += array + c(4) + c(0) + c(0) + gc(18, 1, 0)
    return make_module(code, [array_type(I8)], [ref_type(1)], data=b"\x11\x22\x33\x44")


def pure_element_arrays():
    array = local(0)
    code = c(0) + c(3) + gc(10, 2, 0) + local(0, 0x21)
    code += assertion(array + c(0) + gc(11, 2) + b"\xfb\x16\x6c" + gc(30), 7)
    code += assertion(array + c(1) + gc(11, 2) + gc(22, 1) + gc(2, 1, 0), 9)
    code += array + c(0) + c(2) + c(1) + gc(19, 2, 0)
    code += assertion(array + c(0) + gc(11, 2) + b"\xd1", 1)
    # any/extern conversion preserves identity and does not expose address bits.
    code += assertion(array + b"\xfb\x1b\xfb\x1a\xfb\x16\x6d" + array + b"\xd3", 1)
    code += b"\xfc\x0d\0"  # elem.drop
    code += array + c(3) + c(0) + c(0) + gc(19, 2, 0)
    return make_module(code, [struct_type(I32), array_type(EQREF)], [ref_type(2)],
                       elements=[c(7) + gc(28), c(9) + gc(0, 1), ref_null(-19)])



def pure_aggregate_markers():
    """Cover the remaining Core 3 0xfb operations without inventing a memory."""
    structure, array = local(0), local(1)
    code = c(-1) + gc(0, 1) + local(0, 0x21)
    code += assertion(structure + gc(3, 1, 0), -1)
    code += assertion(structure + gc(4, 1, 0), 255)
    code += structure + c(-128) + gc(5, 1, 0)
    code += assertion(structure + gc(3, 1, 0), -128)
    code += assertion(gc(1, 1) + gc(4, 1, 0), 0)
    code += c(3) + gc(7, 2) + local(1, 0x21)
    code += assertion(array + c(1) + gc(11, 2), 0)
    code += array + c(1) + c(99) + gc(14, 2)
    code += assertion(array + c(1) + gc(11, 2), 99)
    code += c(7) + c(8) + c(9) + gc(8, 2, 3) + local(1, 0x21)
    code += assertion(array + gc(15), 3)
    code += assertion(array + c(2) + gc(11, 2), 9)
    code += assertion(array + gc(20, 2), 1)
    code += assertion(array + gc(20, 1), 0)
    code += assertion(ref_null(2) + gc(20, 2), 0)
    code += assertion(ref_null(2) + gc(21, 2), 1)
    code += assertion(ref_null(2) + gc(23, 2) + b"\xd1", 1)
    code += assertion(c(-1) + gc(28) + gc(29), -1)
    code += assertion(c(-1) + gc(28) + gc(30), 0x7fffffff)
    # Branch-cast immediates are flags:u8, label:u32, source/dest heap:s33.
    # Taken and fallthrough paths both retain an actual managed reference.
    cast = lambda operation: gc(operation) + b"\x01\0\x6d\x01"
    code += assertion(b"\x02\x6d" + structure + cast(24) + DROP + b"\x00\x0b" + gc(22, 1) + gc(4, 1, 0), 128)
    code += assertion(b"\x02\x6d" + array + cast(25) + DROP + b"\x00\x0b" + gc(22, 2) + gc(15), 3)
    code += c(0) + local(2, 0x21)
    code += b"\x02\x6d" + array + cast(24) + c(1) + local(2, 0x21) + b"\x0b" + DROP
    code += assertion(local(2), 1)
    code += c(0) + local(2, 0x21)
    code += b"\x02\x6d" + structure + cast(25) + c(1) + local(2, 0x21) + b"\x0b" + DROP
    code += assertion(local(2), 1)
    return make_module(code, [struct_type(I8), array_type(I32)], [ref_type(1), ref_type(2), I32])


def table64_array(index64=False):
    code = c(0, 64) + c(77) + c(2) + gc(6, 1) + b"\x26\0"
    read = c(0, 64) + b"\x25\0" + gc(22, 1)
    code += assertion(read + c(1, 64 if index64 else 32) + gc(11, 1), 77)
    return make_module(code, [array_type(I32)], table=(True, 2, EQREF), memory=64)


def tagged_through_gc(bits, mismatch=False):
    value_type = I64 if bits == 64 else I32
    tag = 0xA3 if bits == 64 else 3
    tagged = lambda address, color=tag: c(address, bits) + c(color, bits) + op(0x22)
    code = tagged(0) + c(64, bits) + op(0x2B)
    code += tagged(16) + c(0x12345678) + mem(0x36)
    code += tagged(16) + gc(0, 1) + local(0, 0x21)
    # Exercise numeric fields/arrays and a table root, preserving all tag bits.
    code += c(0) + local(0) + b"\x26\0"
    pointer = c(0) + b"\x25\0" + gc(22, 1) + gc(2, 1, 0)
    code += assertion(pointer + op(0x21), tag, bits)
    code += pointer + c(1) + gc(6, 2) + local(1, 0x21)
    pointer = local(1) + c(0) + gc(11, 2)
    code += assertion(pointer + op(0x21), tag, bits)
    if mismatch:
        code += tagged(16, tag ^ 1) + c(16, bits) + op(0x2B)
    code += assertion(pointer + mem(0x28), 0x12345678)
    return make_module(code, [struct_type(value_type), array_type(value_type)],
                       [ref_type(1), ref_type(2)], memory=bits, table=(False, 1, EQREF))


def copy_failure(kind):
    types = [array_type(I8), array_type(I16)]
    if kind == "type":
        code = ref_null(1) + c(0) + ref_null(2) + c(0) + c(0) + gc(17, 1, 2)
    else:
        code = c(0) + c(4) + gc(6, 1) + local(0, 0x21)
        dst = ref_null(1) if kind == "null-dst" else local(0)
        src = ref_null(1) if kind == "null-src" else local(0)
        code += dst + c(4 if kind == "dst" else 0) + src + c(4 if kind == "src" else 0)
        code += c(0 if kind.startswith("null") else 1) + gc(17, 1, 1)
    return make_module(code, types, [ref_type(1)])


def data_after_drop():
    code = b"\xfc\x09\0" + c(0) + c(1) + gc(9, 1, 0) + DROP
    return make_module(code, [array_type(I8)], data=b"x")


def cast_failure():
    code = gc(1, 1) + gc(22, 2) + DROP
    return make_module(code, [struct_type(I32), array_type(I32)])


def collection_ring(tagged=False, ineligible=False):
    count, state = local(0), local(1)
    code = c(8192) + local(0, 0x21) + c(17) + local(1, 0x21)
    if tagged:
        code += c(0) + c(3) + op(0x22) + c(16) + op(0x2B)
    code += b"\x03\x40"
    code += state + c(3) + b"\x6c" + c(1) + b"\x6a" + local(1, 0x21)
    code += count + c(127) + b"\x71" + state + gc(0, 1) + b"\x26\0"
    code += count + c(127) + b"\x71\x25\0" + gc(22, 1) + gc(2, 1, 0)
    code += state + b"\x47\x04\x40\x00\x0b"
    code += count + c(1) + b"\x6b" + local(0, 0x22) + b"\x0d\0\x0b"
    if tagged:
        code += c(0) + c(3) + op(0x22) + state + mem(0x36)
        code += c(0) + c(3) + op(0x22) + mem(0x28) + state + b"\x47\x04\x40\x00\x0b"
    return make_module(code, [struct_type(I32)], [I32, I32],
                       memory=32 if tagged else None, table=(False, 128, EQREF), unused_tag=ineligible)


def cases():
    yield "pure-data-arrays", pure_data_arrays(), "ok", "", False
    yield "pure-element-arrays", pure_element_arrays(), "ok", "", False
    yield "pure-aggregate-markers", pure_aggregate_markers(), "ok", "", False
    yield "table64-gc-index32", table64_array(), "ok", "", False
    yield "table64-gc-index64-invalid", table64_array(True), "invalid", "", False
    for bits in (32, 64):
        yield f"tagged-through-gc-{bits}", tagged_through_gc(bits), "ok", "", False
        yield f"tagged-through-gc-{bits}-mismatch", tagged_through_gc(bits, True), "memtag", "memory tag mismatch", False
    for kind in ("dst", "src", "null-dst", "null-src", "type"):
        expected = "invalid" if kind == "type" else "trap"
        diagnostic = "null reference" if kind.startswith("null") else "out of bounds"
        yield "array-copy-" + kind, copy_failure(kind), expected, diagnostic, False
    yield "data-after-drop", data_after_drop(), "trap", "out of bounds", False
    yield "cast-failure", cast_failure(), "trap", "cast", False
    yield "collect-pure-gc", collection_ring(), "ok", "", True
    yield "collect-with-linear-mte", collection_ring(True), "ok", "", True
    yield "collect-ineligible-tag", collection_ring(ineligible=True), "ok", "", True


def main():
    global ENCODING
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--engines", default="int,aot")
    parser.add_argument("--runner", default="", help="Optional shlex-parsed emulator command prefix.")
    parser.add_argument("--generate-only", action="store_true")
    parser.add_argument("--skip-collection", action="store_true")
    parser.add_argument("--encoding", choices=("packed", "packed-padded", "both"), default="both")
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=True)
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    manifest = []
    for encoding in ("packed", "packed-padded") if args.encoding == "both" else (args.encoding,):
        ENCODING = encoding
        for name, wasm, expected, diagnostic, collect in cases():
            path = (args.out / (encoding + "-" + name + ".wasm")).resolve()
            path.write_bytes(wasm)
            manifest.append(dict(name=name, encoding=encoding, file=str(path), sha256=hashlib.sha256(wasm).hexdigest(),
                                 expected=expected, diagnostic=diagnostic, collection=collect))
    (args.out / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    if args.generate_only:
        return
    if args.vm is None:
        parser.error("--vm is required without --generate-only")
    rows = []
    for case in manifest:
        for engine in args.engines.split(","):
            if case["collection"] and (args.skip_collection or engine != "aot"):
                continue
            command = shlex.split(args.runner) + [str(args.vm.resolve()), f"--runtime-{engine}"]
            if engine == "aot":
                command += ["--runtime-llvm-jit-cache-path", "disable", "--runtime-compile-threads", "0"]
            command += ["-WFE-gc", "-WFE-function-references", "-WFE-reference-types", "-WFE-memory64", "-WFE-table64",
                       "-WFE-exceptions", "-Rclog", "err", "--wasm-set-start-func", "0", "--", case["file"]]
            result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=90)
            text = result.stdout.decode("utf-8", "replace")
            (args.out / (case["encoding"] + "-" + case["name"] + "-" + engine + ".log")).write_text(text)
            fault = human_fault(text)
            classified_status = fault["exit_status"] if fault else None
            if case["expected"] == "ok":
                passed = result.returncode == 0
            elif case["expected"] == "memtag":
                passed = fault_matches(result.returncode,text,122,case["diagnostic"])
            elif case["expected"] == "invalid":
                passed = result.returncode == 127 and "validation error" in text.lower()
            else:
                passed = fault_matches(result.returncode,text,121,case["diagnostic"])
            metrics = None
            if case["collection"]:
                reports = re.findall(r"\[gc-managed\] ([^\n]+)", text)
                if len(reports) == 1:
                    metrics = {key: int(value) for key, value in (part.split("=", 1) for part in reports[0].split())}
                    if case["name"] == "collect-ineligible-tag":
                        passed &= metrics.get("disabled") == 1 and metrics.get("collections") == 0 and metrics.get("reclaimed") == 0
                    else:
                        passed &= metrics.get("disabled") == 0 and metrics.get("collections", 0) > 0 and metrics.get("reclaimed", 0) > 0
                else:
                    passed = False
            rows.append(dict(name=case["name"], encoding=case["encoding"], engine=engine, exit=result.returncode, classified_status=classified_status,
                             expected=case["expected"], passed=bool(passed), metrics=metrics, command=command))
            (args.out / "report.json").write_text(json.dumps(rows, indent=2) + "\n")
            print(case["encoding"], case["name"], engine, result.returncode, "PASS" if passed else "FAIL", flush=True)
    raise SystemExit(any(not row["passed"] for row in rows))


if __name__ == "__main__":
    main()
