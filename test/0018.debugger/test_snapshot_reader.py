#!/usr/bin/env python3
"""Exercise the offline reader against real runtime output; no VM state changes."""
import argparse
import hashlib
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools/debug"))
import snapshot


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--diagnostic", type=Path, required=True)
    parser.add_argument("--full", type=Path, required=True)
    args = parser.parse_args()
    checks = 0

    def rejected(data):
        nonlocal checks
        try: snapshot.decode(data)
        except snapshot.InvalidSnapshot: checks += 1
        else: raise AssertionError("invalid image was accepted")

    def resign(data):
        data[-32:] = hashlib.sha256(data[:-32]).digest()
        return data

    for path in (args.diagnostic, args.full):
        data = path.read_bytes();image = snapshot.decode(data);checks += 1
        # Every truncation of a small diagnostic, and every framing boundary of
        # a larger full inspection. No unchecked payload allocation is needed.
        cuts = range(len(data)) if image.disclosure == "diagnostic" else (0, 7, 63, 64, 95, 96, len(data) - 49, len(data) - 48, len(data) - 1)
        for cut in cuts: rejected(data[:cut])
        for pos in range(len(data)):
            damaged = bytearray(data);damaged[pos] ^= 1;rejected(damaged)
        for at, width, value in ((8, 4, 2), (12, 4, 63), (16, 2, 65535), (20, 1, 0), (21, 1, 16), (23, 1, 1), (56, 8, (1 << 64) - 1), (64 + 24, 8, (1 << 64) - 1)):
            damaged = bytearray(data);damaged[at:at + width] = value.to_bytes(width, "little");rejected(resign(damaged))
        # A recomputed checksum cannot bless an invalid typed diagnostic record.
        damaged = bytearray(data);struct.pack_into("<Q", damaged, 96 + 16, 0);rejected(resign(damaged))
        damaged = bytearray(data);struct.pack_into("<Q", damaged, 96 + 40, (1 << 64) - 1);rejected(resign(damaged))
        if image.disclosure == "full":
            damaged = bytearray(data);damaged[22] = 1;rejected(resign(damaged))
        # Borrowed views are backed by a private immutable copy, never a mutable
        # caller buffer which can change after a successful checksum.
        source = bytearray(data);owned = snapshot.decode(source);source[:] = b"x" * len(source)
        assert owned.data == data;checks += 1
    print("PASS offline snapshot framing, mutation, disclosure and ownership", checks, "checks")


if __name__ == "__main__": main()
