#!/usr/bin/env python3
"""Generate real scalar WasmMTE guests with in-guest oracle assertions.

These modules are explicitly encoded Wasm, not claimed to be the output of the
WasmMTE C compiler. They execute normal scalar load/store loops in the actual
runtime, using its documented extension opcodes only to initialize tags/pointers.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

HERE = Path(__file__).resolve().parent
MASK = (1 << 32) - 1
WORDS = 1024
BASE = 128
CHASE_BASE = 512
TABLE_BASE = 32768
EXPECTED_BASE = 49152


def reference(kind, iterations):
    words = [(i * 17 + 1) & MASK for i in range(WORDS)]
    acc, rng, cursor = 0x13579bdf, 0x12345678, 0
    for i in range(iterations):
        if kind == "linear":
            index = (i * 13) & 1023
        elif kind == "sequential":
            index = i & 1023
        elif kind == "random":
            rng = (rng * 1664525 + 1013904223) & MASK
            index = (rng >> 12) & 1023
        else:
            index = cursor
            cursor = (cursor * 13 + 17) & 1023
        value = words[index]
        acc = (((acc << 5) | (acc >> 27)) & MASK) ^ ((value + i) & MASK)
        words[index] = (value + 3) & MASK
    return dict(checksum=acc, words=words, rng=rng, cursor=cursor)


def generate(vm, bits, kind, iterations, rounds, oracle):
    p = vm.Program(bits == 64)
    c, u = vm.c, vm.u
    get = lambda i: b"\x20" + u(i)
    put = lambda i: b"\x21" + u(i)
    tee = lambda i: b"\x22" + u(i)
    wide = bits == 64
    plus = b"\x7c" if wide else b"\x6a"
    widen = b"\xad" if wide else b""
    ptr_load, ptr_store = (0x29, 0x37) if wide else (0x28, 0x36)
    maximum = 255 if wide else 15
    # i, acc, value, index, rng, round are i32; ptr and auxiliary ptr are addresses.
    locals_ = b"\x01\x08\x7f" if not wide else b"\x02\x06\x7f\x02\x7e"

    def address(index, base=BASE, stride=4):
        return p.n(base) + index + c(stride) + b"\x6c" + widen + plus

    def color(index):
        if kind == "random":
            value = index + c(6) + b"\x76" + c(7) + b"\x6c"
        elif kind == "chase":
            value = index + c(97) + b"\x6c"
        else:
            return p.n(3)
        return value + c(maximum) + b"\x70" + c(1) + b"\x6a" + widen

    def data_pointer(index):
        return address(index, CHASE_BASE, 16) + color(index) + vm.op(0x22) if kind == "chase" else address(index) + color(index) + vm.op(0x22)

    def next_index(index):
        return index + c(13) + b"\x6c" + c(17) + b"\x6a" + c(1023) + b"\x71"

    def equal(lhs, rhs, width=32):
        return lhs + rhs + (b"\x52" if width == 64 else b"\x47") + b"\x04\x40\x00\x0b"

    def counter_loop(body, count=WORDS):
        return c(0) + put(0) + b"\x03\x40" + body + get(0) + c(1) + b"\x6a" + tee(0) + c(count) + b"\x49\x0d\x00\x0b"

    if kind in ("linear", "sequential"):
        p.add(p.tags(BASE, WORDS * 4, 3))
        p.add(p.tagged(BASE, 3) + put(7))
    elif kind == "random":
        for obj in range(16):
            p.add(p.tags(BASE + obj * 256, 256, 1 + (obj * 7) % maximum))
        p.add(p.tags(TABLE_BASE, WORDS * (bits // 8), 7))
        p.add(p.tagged(TABLE_BASE, 7) + put(7))
    else:
        p.add(counter_loop(data_pointer(get(0)) + p.n(16) + vm.op(0x2b)))

    # Round body initializes every value; tag initialization is invariant.
    p.add(c(0) + put(5) + b"\x03\x40")
    init = data_pointer(get(0)) + put(6)
    if kind == "chase":
        init += get(6) + data_pointer(next_index(get(0))) + vm.mem(ptr_store)
    elif kind == "random":
        init += get(7) + get(0) + c(bits // 8) + b"\x6c" + widen + plus + get(6) + vm.mem(ptr_store)
    init += get(6) + get(0) + c(17) + b"\x6c" + c(1) + b"\x6a" + vm.mem(0x36, 8 if kind == "chase" else 0)
    p.add(counter_loop(init))

    p.add(c(0x13579bdf) + put(1) + c(0x12345678) + put(4))
    if kind == "chase":
        p.add(data_pointer(c(0)) + put(6))
    p.add(c(0) + put(0) + b"\x03\x40")
    if kind == "linear":
        p.add(get(0) + c(13) + b"\x6c" + c(1023) + b"\x71" + put(3))
    elif kind == "sequential":
        p.add(get(0) + c(1023) + b"\x71" + put(3))
    elif kind == "random":
        p.add(get(4) + c(1664525) + b"\x6c" + c(1013904223) + b"\x6a" + tee(4) + c(12) + b"\x76" + c(1023) + b"\x71" + put(3))
    if kind in ("linear", "sequential"):
        p.add(get(7) + get(3) + c(2) + b"\x74" + widen + plus + put(6))
    elif kind == "random":
        p.add(get(7) + get(3) + c(bits // 8) + b"\x6c" + widen + plus + vm.mem(ptr_load) + put(6))
    else:
        p.add(get(6) + vm.mem(ptr_load) + put(7))
    p.add(get(6) + vm.mem(0x28, 8 if kind == "chase" else 0) + put(2))
    p.add(get(1) + c(5) + b"\x77" + get(2) + get(0) + b"\x6a\x73" + put(1))
    p.add(get(6) + get(2) + c(3) + b"\x6a" + vm.mem(0x36, 8 if kind == "chase" else 0))
    if kind == "chase":
        p.add(get(7) + put(6))
    p.add(get(0) + c(1) + b"\x6a" + tee(0) + c(iterations) + b"\x49\x0d\x00\x0b")
    p.add(vm.assertion(get(1), oracle["checksum"]))
    if kind == "random":
        p.add(vm.assertion(get(4), oracle["rng"]))
    elif kind == "chase":
        p.add(equal(get(6), data_pointer(c(oracle["cursor"])), bits))

    # Full written footprint is checked against an independent Python oracle.
    # Expected values occupy a separate default-tag-zero active data segment.
    verify = data_pointer(get(0)) + put(6)
    expected = address(get(0), EXPECTED_BASE) + vm.mem(0x28)
    verify += equal(get(6) + vm.mem(0x28, 8 if kind == "chase" else 0), expected)
    if kind == "random":
        actual_ptr = p.tagged(TABLE_BASE, 7) + get(0) + c(bits // 8) + b"\x6c" + widen + plus + vm.mem(ptr_load)
        verify += equal(actual_ptr, get(6), bits)
    elif kind == "chase":
        verify += equal(get(6) + vm.mem(ptr_load), data_pointer(next_index(get(0))), bits)
        if bits == 32:
            verify += vm.assertion(get(6) + vm.mem(0x28, 4), 0)
        verify += vm.assertion(get(6) + vm.mem(0x28, 12), 0)
    p.add(counter_loop(verify))
    start, size = (CHASE_BASE, WORDS * 16) if kind == "chase" else (BASE, WORDS * 4)
    for addr in (start - 4, start + size):
        p.add(vm.assertion(p.n(addr) + vm.mem(0x28), 0))
    if kind == "random":
        for addr in (TABLE_BASE - 4, TABLE_BASE + WORDS * (bits // 8)):
            p.add(vm.assertion(p.n(addr) + vm.mem(0x28), 0))
    p.add(get(5) + c(1) + b"\x6a" + tee(5) + c(rounds) + b"\x49\x0d\x00\x0b")
    body = locals_ + p.code + b"\x0b"
    binary = b"\0asm\x01\0\0\0" + vm.sec(1,b"\x01\x60\0\0") + vm.sec(3,b"\x01\0")
    binary += vm.sec(5,b"\x01" + u(5 if wide else 1) + u(1) + u(2))
    binary += vm.sec(7,b"\x01\x06_start\0\0") + vm.sec(10,b"\x01" + u(len(body)) + body)
    expected_bytes = b"".join(struct.pack("<I", value) for value in oracle["words"])
    binary += vm.sec(11,b"\x01\x00" + p.n(EXPECTED_BASE) + b"\x0b" + u(len(expected_bytes)) + expected_bytes)
    return binary


def load_encoder(path):
    spec = importlib.util.spec_from_file_location("upstream_encoder",path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--encoder",type=Path,default=HERE/"vm_matrix.py")
    parser.add_argument("--out",type=Path,required=True)
    parser.add_argument("--encoding",choices=("packed","packed-padded"),default="packed")
    parser.add_argument("--iterations",type=int,default=1048576)
    parser.add_argument("--rounds",type=int,default=1)
    parser.add_argument("--kinds",default="linear,sequential,random,chase")
    args=parser.parse_args()
    assert 0<args.iterations<2**31 and 0<args.rounds<2**31
    vm=load_encoder(args.encoder)
    if args.encoding=="packed-padded":
        def padded(operation,memory=0):
            word=0x01010000|operation
            return b"\xff"+bytes(((word>>(7*i))&127)|(128 if i!=4 else 0) for i in range(5))+vm.u(memory)
        vm.op=padded
    args.out.mkdir(parents=True,exist_ok=True)
    rows=[]
    for kind in args.kinds.split(","):
        assert kind in ("linear","sequential","random","chase")
        oracle=reference(kind,args.iterations)
        (args.out/(kind+"-oracle.json")).write_text(json.dumps(oracle,indent=2)+"\n")
        for bits in (32,64):
            binary=generate(vm,bits,kind,args.iterations,args.rounds,oracle)
            name=f"{kind}-{bits}-tagged"
            path=args.out/(name+".wasm")
            path.write_bytes(binary)
            rows.append(dict(name=name,file=str(path.resolve()),sha256=hashlib.sha256(binary).hexdigest(),
                bits=bits,kind=kind,encoding=args.encoding,tagged=True,iterations=args.iterations,rounds=args.rounds,
                expected_checksum=oracle["checksum"],verified_values=WORDS,
                verified_pointer_entries=WORDS if kind in ("random","chase") else 0,
                generation="hand-encoded scalar WasmMTE; not C compiler instrumentation",
                encoder_sha256=hashlib.sha256(args.encoder.read_bytes()).hexdigest()))
    (args.out/"manifest.json").write_text(json.dumps(rows,indent=2)+"\n")
    print(json.dumps(dict(fixtures=len(rows),iterations=args.iterations,rounds=args.rounds),indent=2))


if __name__=="__main__":
    main()
