#!/usr/bin/env python3
"""Build actual replay guests using an existing Core 3 assembler; never install tools."""
from pathlib import Path
import argparse, subprocess

def unsigned(data, at):
    value = shift = 0
    while True:
        if at >= len(data) or shift >= 35:
            raise ValueError("malformed input u32")
        byte = data[at]; at += 1; value |= (byte & 127) << shift
        if byte < 128:
            return value, at
        shift += 7

def encode(value):
    out = bytearray()
    while value >= 128:
        out.append((value & 127) | 128); value >>= 7
    out.append(value)
    return bytes(out)

def tag_instruction(module, replacements=None, target_body=None):
    # Each supplied MTE fixture has exactly one call to the deliberately inert
    # $tag function at index 1. Replace that expression, rebuilding body/section
    # lengths. Never pass the unpatched placeholder to an MTE acceptance test.
    replacements = replacements or {bytes.fromhex("10 01"): bytes.fromhex("ff af 80 84 08 00")}
    counts = {key: 0 for key in replacements}
    at = 8; result = bytearray(module[:8]); replaced = 0
    while at < len(module):
        kind = module[at]; size, at = unsigned(module, at + 1)
        section = module[at:at + size]; at += size
        if len(section) != size:
            raise ValueError("truncated input section")
        if kind == 10:
            count, pos = unsigned(section, 0); bodies = bytearray(encode(count))
            for function_index in range(count):
                size, pos = unsigned(section, pos)
                body = section[pos:pos + size]; pos += size
                for old, new in (replacements.items() if target_body is None or function_index == target_body else []):
                    count_here = body.count(old); counts[old] += count_here; replaced += count_here
                    body = body.replace(old, new)
                bodies += encode(len(body)) + body
            if pos != len(section):
                raise ValueError("invalid code section")
            section = bodies
        result.append(kind); result += encode(len(section)) + section
    if replaced != len(replacements) or any(count != 1 for count in counts.values()):
        raise ValueError("fixture no longer has one unambiguous $tag placeholder")
    return result

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--wasm-tools", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args(); args.out.mkdir(parents=True, exist_ok=True)
    for source in sorted(Path(__file__).resolve().parent.glob("*.wat")):
        destination = args.out / source.with_suffix(".wasm").name
        subprocess.run([str(args.wasm_tools), "parse", str(source), "-o", str(destination)], check=True)
        subprocess.run([str(args.wasm_tools), "validate", str(destination)], check=True)
        if source.stem.startswith("mte"):
            destination.write_bytes(tag_instruction(destination.read_bytes()))
        if source.stem == "private-mte":
            replacements = {bytes([0x10, function]): bytes([0xff]) + encode(0x01010000 | op) + encode(memory)
                            for function, op, memory in [(1, 0x2f, 1), (2, 0x2f, 2), (3, 0x27, 1), (4, 0x27, 2)]}
            code = tag_instruction(destination.read_bytes(), replacements, target_body=4)
            name = b"uwvm.memtag.linear.globals"
            globals_section = encode(len(name)) + name + bytes([2, 2, 1, 16, 16, 2, 16, 16])
            destination.write_bytes(code + bytes([0]) + encode(len(globals_section)) + globals_section)
        print(destination)

if __name__ == "__main__":
    main()
