#!/usr/bin/env python3
"""Compile the real threads fixture and encode its one nonstandard MTE body."""
import argparse
from pathlib import Path
import subprocess


def u(value):
    result = bytearray()
    while True:
        byte = value & 127
        value >>= 7
        result.append(byte | (128 if value else 0))
        if not value:
            return bytes(result)


def read_u(data, position):
    value = 0
    for shift in range(0, 35, 7):
        byte = data[position]
        position += 1
        value |= (byte & 127) << shift
        if byte < 128:
            return value, position
    raise ValueError("invalid unsigned LEB32")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--wat2wasm", default="/usr/bin/wat2wasm")
    parser.add_argument("--source", type=Path, default=Path(__file__).with_name("b1_memtag_stop.wat"))
    parser.add_argument("--output", required=True, type=Path)
    args = parser.parse_args()
    subprocess.run([args.wat2wasm, "--enable-threads", "--debug-names", str(args.source), "-o", str(args.output)], check=True)
    data = args.output.read_bytes()
    assert data[:8] == b"\0asm\x01\0\0\0"
    result, position, replaced = data[:8], 8, 0
    marker = b"\0\x41\x80\x04\x41\x03\x72\x41\x10\x1a\x1a\x0b"
    tagged = b"\0\x41\x80\x04\x41\x03\xff" + u(0x01010022) + b"\0\x41\x10\xff" + u(0x0101002b) + b"\0\x0b"
    while position < len(data):
        kind = data[position]
        length, begin = read_u(data, position + 1)
        payload = data[begin:begin + length]
        assert len(payload) == length
        position = begin + length
        if kind == 10:
            count, cursor = read_u(payload, 0)
            rewritten = u(count)
            for _ in range(count):
                size, start = read_u(payload, cursor)
                body = payload[start:start + size]
                assert len(body) == size
                cursor = start + size
                if body == marker:
                    body = tagged
                    replaced += 1
                rewritten += u(len(body)) + body
            assert cursor == len(payload)
            payload = rewritten
        result += bytes([kind]) + u(len(payload)) + payload
    assert replaced == 1, "tag marker must identify exactly one whole function body"
    args.output.write_bytes(result)


if __name__ == "__main__":
    main()
