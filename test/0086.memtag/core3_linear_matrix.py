#!/usr/bin/env python3
"""Real VM coverage for Core 3 linear-memory domains and globals metadata v2.

Generate immutable fixtures with --generate-only, or execute both production
backends with --vm. No assembler is allowed to erase experimental instructions.
Run the VM through the task's configured resource-control launcher. These tests
do not claim that GC storage or the separate threads proposal is linear memory.
"""
import argparse
from dataclasses import dataclass
import hashlib
import json
from pathlib import Path
import re
import shlex
import subprocess

from vm_matrix import SIMD, assertion, c, sec, u
from vm_diagnostics import fault_matches, human_fault

HEADER = b"\0asm\x01\0\0\0"
LINEAR_GLOBALS = "uwvm.memtag.linear.globals"
PADDED_MTE_WORD = False


def text(value):
    data = value.encode()
    return u(len(data)) + data


def mte(opcode, index=0):
    word = 0x01010000 | opcode
    packed = bytes(((word >> (7*i)) & 127) | (128 if i != 4 else 0) for i in range(5)) if PADDED_MTE_WORD else u(word)
    return b"\xff" + packed + u(index)


def memory(opcode, index=0, offset=0):
    return bytes([opcode]) + u(64 if index else 0) + (u(index) if index else b"") + u(offset)


def simd(opcode, index=0, offset=0, lane=None):
    return b"\xfd" + u(opcode) + u(64 if index else 0) + (u(index) if index else b"") + u(offset) + (bytes([lane]) if lane is not None else b"")


def bulk(opcode, *indices):
    return b"\xfc" + u(opcode) + b"".join(u(index) for index in indices)


def pointer(address, bits, index, tag):
    return c(address, bits) + c(tag, bits) + mte(0x22, index) if tag is not None else c(address, bits)


def mark(index, bits, address=0, size=512, tag=3):
    return pointer(address, bits, index, tag) + c(size, bits) + mte(0x2b, index)


def globals_v2(entries, version=2):
    return u(version) + u(len(entries)) + b"".join(u(index) + u(address) + u(length) for index, address, length in entries)


def module(code=b"", widths=(32,), *, imports=(), sections=(), data=(), start=False, export_memory=False):
    """imports contains (module, field, address_bits); data uses None for passive."""
    result = HEADER + sec(1, b"\x01\x60\0\0")
    limits = lambda bits: u(5 if bits == 64 else 1) + u(1) + u(2)
    if imports:
        result += sec(2, u(len(imports)) + b"".join(text(owner) + text(field) + b"\x02" + limits(bits) for owner, field, bits in imports))
    result += sec(3, b"\x01\0")
    if widths:
        result += sec(5, u(len(widths)) + b"".join(limits(bits) for bits in widths))
    exports = text("_start") + b"\0\0"
    if export_memory:
        exports += text("memory") + b"\x02\0"
    result += sec(7, u(2 if export_memory else 1) + exports)
    if start:
        result += sec(8, b"\0")
    if any(index is None for index, _, _ in data):
        result += sec(12, u(len(data)))
    body = b"\0" + code + b"\x0b"
    result += sec(10, b"\x01" + u(len(body)) + body)
    if data:
        all_widths = tuple(entry[2] for entry in imports) + tuple(widths)
        segments = []
        for index, offset, payload in data:
            mode = b"\x01" if index is None else ((b"\0" if index == 0 else b"\x02" + u(index)) + c(offset, all_widths[index]) + b"\x0b")
            segments.append(mode + u(len(payload)) + payload)
        result += sec(11, u(len(segments)) + b"".join(segments))
    for name, payload in sections:
        result += sec(0, text(name) + payload)
    return result


@dataclass
class Case:
    name: str
    wasm: bytes
    expected: int = 0
    diagnostic: str = ""
    preloads: tuple = ()
    disabled: bool = False
    extension_disabled: bool = False


SCALARS = [
    (0x28, 32, 4, False, False), (0x29, 64, 8, False, False),
    (0x2a, 32, 4, True, False), (0x2b, 64, 8, True, False),
    (0x2c, 32, 1, False, False), (0x2d, 32, 1, False, False),
    (0x2e, 32, 2, False, False), (0x2f, 32, 2, False, False),
    (0x30, 64, 1, False, False), (0x31, 64, 1, False, False),
    (0x32, 64, 2, False, False), (0x33, 64, 2, False, False),
    (0x34, 64, 4, False, False), (0x35, 64, 4, False, False),
    (0x36, 32, 4, False, True), (0x37, 64, 8, False, True),
    (0x38, 32, 4, True, True), (0x39, 64, 8, True, True),
    (0x3a, 32, 1, False, True), (0x3b, 32, 2, False, True),
    (0x3c, 64, 1, False, True), (0x3d, 64, 2, False, True),
    (0x3e, 64, 4, False, True),
]


def access_cases():
    for bits in (32, 64):
        for index in (0, 1):
            setup = mark(index, bits) + mark(1-index, bits, tag=7)
            for family, descriptors in (("scalar", SCALARS), ("simd", SIMD)):
                for descriptor in descriptors:
                    opcode = descriptor[0]
                    width = descriptor[2] if family == "scalar" else descriptor[1]
                    # Cross-granule first/last failures catch short-width proofs;
                    # an adjacent bad granule must not overcheck narrow loads.
                    modes = ("good", "bad-first", "outside") + (("bad-last",) if width > 1 else ())
                    for mode in modes:
                        offset = 16-width if mode == "outside" else 15
                        code = setup
                        if mode != "good":
                            code += mark(index, bits, 0 if mode == "bad-first" else 16, 16, 4)
                        code += pointer(0, bits, index, 3)
                        if family == "scalar":
                            _, value_bits, _, floating, store = descriptor
                            if store:
                                code += c(0, value_bits)
                                if floating:
                                    code += bytes([0xbe if value_bits == 32 else 0xbf])
                            code += memory(opcode, index, offset)
                            if not store:
                                if floating:
                                    code += bytes([0xbc if value_bits == 32 else 0xbd])
                                code = code + c(0, value_bits) + bytes([0x47 if value_bits == 32 else 0x52, 0x04, 0x40, 0x00, 0x0b])
                        else:
                            _, _, lane, consumes, store = descriptor
                            if consumes:
                                code += b"\xfd\x0c" + bytes(16)
                            code += simd(opcode, index, offset, lane if 84 <= opcode <= 91 else None)
                            if not store:
                                code += b"\x1a"
                        rejected = mode.startswith("bad")
                        yield Case(f"{family}-{opcode:02x}-{bits}-mem{index}-{mode}", module(code, (bits, bits)),
                                   122 if rejected else 0, "memory tag mismatch" if rejected else "")


def bulk_cases():
    for destination_bits in (32, 64):
        for source_bits in (32, 64):
            length_bits = 64 if destination_bits == source_bits == 64 else 32
            for dst_tag, src_tag in ((None, None), (3, None), (None, 7), (3, 7)):
                setup = (mark(0, destination_bits, tag=dst_tag) if dst_tag else b"") + (mark(1, source_bits, tag=src_tag) if src_tag else b"")
                modes = ["good", "empty", "empty-oob"]
                if dst_tag:
                    modes.append("bad-destination")
                if src_tag:
                    modes.append("bad-source")
                for mode in modes:
                    code = setup
                    if mode == "bad-destination":
                        code += mark(0, destination_bits, 320, 16, 4)
                    if mode == "bad-source":
                        code += mark(1, source_bits, 96, 16, 4)
                    empty = mode.startswith("empty")
                    code += pointer((65537 if mode == "empty-oob" else 65536) if empty else 256, destination_bits, 0, dst_tag)
                    code += pointer(65536 if empty else 32, source_bits, 1, src_tag)
                    code += c(0 if empty else 128, length_bits) + bulk(10, 0, 1)
                    if not empty:
                        code += assertion(pointer(256, destination_bits, 0, dst_tag) + memory(0x2d, 0), 81)
                        code += assertion(pointer(383, destination_bits, 0, dst_tag) + memory(0x2d, 0), 81)
                    expected = 121 if mode == "empty-oob" else 122 if mode.startswith("bad") else 0
                    diagnostic = "out of bounds" if expected == 121 else "memory tag mismatch" if expected == 122 else ""
                    yield Case(f"copy-{destination_bits}-{source_bits}-tags{dst_tag or 0}{src_tag or 0}-{mode}",
                               module(code, (destination_bits, source_bits), data=((1, 32, b"Q"*128),)), expected, diagnostic)
    for bits in (32, 64):
        for index in (0, 1):
            for operation in ("fill", "init"):
                for mode in ("good", "bad-middle", "empty", "empty-oob", "dropped-empty", "dropped-nonempty"):
                    if operation == "fill" and mode.startswith("dropped"):
                        continue
                    code = mark(index, bits)
                    if mode == "bad-middle":
                        code += mark(index, bits, 32, 16, 4)
                    if mode.startswith("dropped"):
                        code += bulk(9, 0) + bulk(9, 0)
                    empty = mode in ("empty", "empty-oob", "dropped-empty")
                    address = (65537 if mode == "empty-oob" else 65536) if empty else 15
                    code += pointer(address, bits, index, 3)
                    if operation == "fill":
                        code += c(81) + c(0 if empty else 64, bits) + bulk(11, index)
                        data = ()
                    else:
                        code += c(0) + c(0 if empty else 64) + bulk(8, 0, index)
                        data = ((None, 0, b"Q"*64),)
                    if mode == "good":
                        code += assertion(pointer(78, bits, index, 3) + memory(0x2d, index), 81)
                    expected = 122 if mode == "bad-middle" else 121 if mode in ("empty-oob", "dropped-nonempty") else 0
                    diagnostic = "memory tag mismatch" if expected == 122 else "out of bounds" if expected == 121 else ""
                    yield Case(f"{operation}-{bits}-mem{index}-{mode}", module(code, (bits, bits), data=data), expected, diagnostic)
            code = mark(index, bits, 0, 16, 3)
            code += assertion(c(1, bits) + bytes([0x40]) + u(index), 1, bits)
            code += assertion(bytes([0x3f]) + u(index), 2, bits)
            code += assertion(bytes([0x3f]) + u(1-index), 1, bits)
            code += assertion(pointer(0, bits, index, 3) + memory(0x28, index), 0)
            code += assertion(c(65536, bits) + memory(0x28, index), 0)
            code += mark(index, bits, 65536, 16, 7)
            code += assertion(pointer(65536, bits, index, 7) + memory(0x28, index), 0)
            yield Case(f"grow-owner-{bits}-mem{index}", module(code, (bits, bits)))


def metadata_cases():
    admission = "wasmmte admission failed"
    for bits in (32, 64):
        section = (LINEAR_GLOBALS, globals_v2(((1, 32, 32),)))
        # No MTE instruction occurs in this body: the metadata alone must mark
        # memory 1, making the raw zero-tag load fail after active-data copy.
        code = c(32, bits) + memory(0x28, 1) + b"\x1a"
        raw = module(code, (bits, bits), sections=(section,), data=((1, 32, b"Q"*32),))
        yield Case(f"globals-only-activates-{bits}", raw, 122, "memory tag mismatch")
        yield Case(f"globals-only-disabled-{bits}", raw, disabled=True)
        yield Case(f"globals-feature-disabled-{bits}", raw, 127, admission, extension_disabled=True)
        # A raw address with high bits on the neighbouring ordinary owner must
        # still be a bounds failure; silently activating it would yield MTE.
        shift = 28 if bits == 32 else 56
        neighbour = c((3 << shift), bits) + memory(0x28, 0) + b"\x1a"
        yield Case(f"globals-neighbour-ordinary-{bits}", module(neighbour, (bits, bits), sections=(section,)), 121, "out of bounds")
        code = assertion(c(32, bits) + mte(0x27, 1) + memory(0x2d, 1), 81)
        code += assertion(c(32, bits) + memory(0x2d, 0), 0)
        yield Case(f"globals-selected-owner-{bits}", module(code, (bits, bits), sections=(section,), data=((1, 32, b"Q"*32),)))
        imports = (("owner", "memory", bits), ("owner", "memory", bits))
        owner = module(widths=(bits,), export_memory=True)
        for overlap in (False, True):
            entries = ((0, 32, 8), (1, 40 if overlap else 48, 8))
            body = assertion(c(32, bits) + mte(0x27, 0) + memory(0x28, 1), 0)
            yield Case(f"globals-imported-alias-{bits}-overlap{int(overlap)}",
                       module(body, (), imports=imports, sections=((LINEAR_GLOBALS, globals_v2(entries)),)),
                       127 if overlap else 0, admission if overlap else "", (("owner", owner),))
        # The preload start changes the imported owner's tags; the main module
        # then copies deferred active bytes into both the alias and memory 1.
        owner = module(mark(0, bits, 80, 32, 7), (bits,), start=True, export_memory=True)
        code = assertion(c(80, bits) + mte(0x27, 0) + mte(0x21, 0), 7, bits)
        for index in (0, 1):
            code += assertion(c(80, bits) + mte(0x27, index) + memory(0x2d, index), 81)
            code += assertion(c(111, bits) + mte(0x27, index) + memory(0x2d, index), 81)
        raw = module(code, (bits,), imports=(("owner", "memory", bits),),
                     sections=((LINEAR_GLOBALS, globals_v2(((1, 80, 32),))),), data=((0, 80, b"Q"*32), (1, 80, b"Q"*32)))
        yield Case(f"globals-deferred-active-after-start-{bits}", raw, preloads=(("owner", owner),))
        for owner_format in ("memtag.globals", LINEAR_GLOBALS):
            payload = u(1)+u(1)+u(32)+u(16) if owner_format == "memtag.globals" else globals_v2(((0, 32, 16),))
            owner = module(widths=(bits,), export_memory=True, sections=((owner_format, payload),))
            raw = module(widths=(), imports=(("owner", "memory", bits),), sections=((LINEAR_GLOBALS, globals_v2(((0, 32, 16),))),))
            yield Case(f"globals-cross-module-{bits}-{owner_format}", raw, 127, admission, (("owner", owner),))
    code = assertion(c(32) + mte(0x27, 0) + memory(0x28, 0), 0)
    code += assertion(c(32, 64) + mte(0x27, 1) + memory(0x28, 1), 0)
    yield Case("globals-mixed-address-widths", module(code, (32, 64), sections=((LINEAR_GLOBALS, globals_v2(((0, 32, 16), (1, 32, 16)))),)))
    yield Case("globals-empty-no-memory", module(widths=(), sections=((LINEAR_GLOBALS, globals_v2(())),)))
    malformed = [
        ("version", globals_v2((), 1)), ("index", globals_v2(((2, 0, 0),))),
        ("range", globals_v2(((1, 65536, 1),))), ("overflow", globals_v2(((1, (1 << 64)-1, 1),))),
        ("trailing", globals_v2(())+b"\0"), ("count", u(2)+u(100)+b"\0\0\0"),
        ("truncated", u(2)+u(1)+u(1)+b"\x80\x80"),
        ("index-overlong", u(2)+u(1)+b"\x80\x80\x80\x80\x80\0\0\0"),
        ("last-invalid", globals_v2(((0, 16, 16), (1, 65536, 1)))),
        ("byte-overlap", globals_v2(((1, 32, 16), (1, 40, 16)))),
        ("granule-overlap", globals_v2(((1, 32, 8), (1, 40, 8)))),
    ]
    for name, payload in malformed:
        raw = module(widths=(32, 64), sections=((LINEAR_GLOBALS, payload),))
        yield Case("globals-invalid-"+name, raw, 127, admission)
        yield Case("globals-invalid-disabled-"+name, raw, 127, admission, disabled=True)
    linear = (LINEAR_GLOBALS, globals_v2(((1, 32, 16),)))
    legacy = ("memtag.globals", u(1)+u(1)+u(32)+u(16))
    for name, sections in (("duplicate", (linear, linear)), ("legacy-first", (legacy, linear)), ("linear-first", (linear, legacy))):
        yield Case("globals-"+name, module(widths=(32, 32), sections=sections), 127, admission)
    for disabled in (False,True):
        yield Case("globals-obsolete-v1-disabled"+str(int(disabled)),
                   module(sections=(legacy,)),127,"obsolete memtag.globals",disabled=disabled)
        yield Case("globals-obsolete-empty-no-memory-disabled"+str(int(disabled)),
                   module(widths=(),sections=(("memtag.globals",u(1)+u(0)),)),
                   127,"obsolete memtag.globals",disabled=disabled)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--vm", type=Path)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--engines", default="int,aot")
    parser.add_argument("--runner", default="", help="Optional QEMU or resource-control command prefix")
    parser.add_argument("--encoding", choices=("packed", "packed-padded", "both"), default="packed")
    parser.add_argument("--case-filter", default="")
    parser.add_argument("--generate-only", action="store_true")
    args = parser.parse_args()
    if not args.generate_only and args.vm is None:
        parser.error("--vm is required without --generate-only")
    args.out.mkdir(parents=True, exist_ok=True)
    encodings = ("packed", "packed-padded") if args.encoding == "both" else (args.encoding,)
    manifest = []
    cases = []
    global PADDED_MTE_WORD
    for encoding in encodings:
        PADDED_MTE_WORD = encoding == "packed-padded"
        for case in (*access_cases(), *bulk_cases(), *metadata_cases()):
            case.name = encoding + "-" + case.name
            if args.case_filter and re.search(args.case_filter, case.name) is None:
                continue
            path = args.out / (case.name + ".wasm")
            path.write_bytes(case.wasm)
            preloads = []
            for owner, data in case.preloads:
                preload = args.out / (case.name + "-" + owner + ".wasm")
                preload.write_bytes(data)
                preloads.append(dict(name=owner, file=str(preload), sha256=hashlib.sha256(data).hexdigest()))
            entry = dict(name=case.name, file=str(path), sha256=hashlib.sha256(case.wasm).hexdigest(),
                         expected=case.expected, diagnostic=case.diagnostic, preloads=preloads,
                         disabled=case.disabled, extension_disabled=case.extension_disabled)
            manifest.append(entry)
            cases.append((case, entry))
    (args.out / "manifest.json").write_text(json.dumps(manifest, indent=2))
    print(f"generated {len(cases)} Core 3 linear-memory cases", flush=True)
    if args.generate_only:
        return
    rows = []
    for case, entry in cases:
        for engine in args.engines.split(","):
            command = shlex.split(args.runner) + [str(args.vm), "-R"+engine, "--runtime-compile-threads", "0",
                       "--wasm-feature-enable-memory64",
                       "--wasm-feature-enable-multi-memory"]
            if engine == "aot":
                command += ["--runtime-llvm-jit-cache-path", "disable"]
            for preload in entry["preloads"]:
                command += ["--wasm-preload-library", preload["file"], preload["name"]]
            if case.disabled:
                command += ["--runtime-disable-memtag"]
            if case.extension_disabled:
                command += ["--wasm-feature-disable-memtag-extension"]
            command += ["--run", entry["file"]]
            done = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=120)
            output = done.stdout.decode("utf8", "replace")
            log = args.out / (case.name + "-" + engine + ".log")
            log.write_text(output)
            crash = human_fault(output)
            passed = (fault_matches(done.returncode,output,case.expected,case.diagnostic)
                      if case.expected in (121,122) else done.returncode == case.expected and
                      (not case.diagnostic or case.diagnostic in output.lower()))
            rows.append(dict(name=case.name, engine=engine, exit=done.returncode, expected=case.expected,
                             passed=passed, command=command, log=str(log), sha256=entry["sha256"], crash=crash,
                             trap_observation="human-fault-report" if crash else "exit-status"))
            (args.out / "report.json").write_text(json.dumps(rows, indent=2))
            print(case.name, engine, done.returncode, "PASS" if passed else "FAIL", flush=True)
    if not all(row["passed"] for row in rows):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
