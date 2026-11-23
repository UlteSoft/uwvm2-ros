#!/usr/bin/env python3
"""Check pending-native metadata against an actual runtime-produced archive."""
from pathlib import Path
import argparse
import hashlib
import struct
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import snapshot


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("archive", type=Path)
    args = parser.parse_args()
    data = args.archive.read_bytes()
    saved = snapshot.decode(data)
    assert saved.disclosure == "full"
    pending = [r for r in saved.records if r.kind == 11]
    assert len(pending) == 1
    state = saved.describe()["continuation"]
    assert state == {"mode": "retained-native", "crossProcess": False,
                     "environmentObservation": "recording-origin", "requiresOriginalLiveStop": True,
                     "nativeResult": "not-serialized", "restoreAuthority": False}
    at = 64
    for row in saved.records:
        if row.kind == 11:
            break
        at += 32 + len(row.payload)
    checks = 1

    def reject(changed):
        nonlocal checks
        changed[-32:] = hashlib.sha256(changed[:-32]).digest()
        try:
            snapshot.decode(changed)
        except snapshot.InvalidSnapshot:
            checks += 1
        else:
            raise AssertionError("malformed pending continuation passed decode")

    for field, width, value in ((at + 2, 1, 0), (at + 2, 1, 2),
                                (at + 8, 8, 1), (at + 16, 8, 2), (22, 1, 1)):
        changed = bytearray(data)
        changed[field:field + width] = value.to_bytes(width, "little")
        reject(changed)
    # Change the actual payload extent and the framing consistently, then
    # recompute its digest. Integrity alone must not bless a malformed label.
    for size in (0, 1, 31, 33, 64):
        changed = bytearray(data[:at + 32] + bytes(size) + data[at + 64:])
        struct.pack_into("<Q", changed, at + 24, size)
        struct.pack_into("<Q", changed, len(changed) - 40, len(changed))
        reject(changed)
    changed = bytearray(data[:at + 32] + data[at + 64:])
    changed[at + 2] = 3
    struct.pack_into("<Q", changed, at + 24, 0)
    struct.pack_into("<Q", changed, len(changed) - 40, len(changed))
    reject(changed)
    print(f"PASS pending-native reader provenance and malformed metadata: {checks} checks")


if __name__ == "__main__":
    main()
