#!/usr/bin/env python3
"""Validate the offline reader against actual dynamic-thread Runtime archives."""
import argparse
import hashlib
from pathlib import Path
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import snapshot


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--complete", type=Path, required=True)
    parser.add_argument("--pending", type=Path, required=True)
    args = parser.parse_args()
    checks = 0

    def reject(data):
        nonlocal checks
        data[-32:] = hashlib.sha256(data[:-32]).digest()
        try:
            snapshot.decode(data)
        except snapshot.InvalidSnapshot:
            checks += 1
        else:
            raise AssertionError("invalid threaded archive was accepted")

    for path, pending in ((args.complete, False), (args.pending, True)):
        original = path.read_bytes()
        saved = snapshot.decode(original)
        info = saved.describe()["threadHistory"]
        assert saved.disclosure == "full" and info["format"] == "private-thread-journal-v7"
        assert info["terminal"] != pending and info["retainedNativePending"] == pending
        assert not info["restoreAuthority"] and not info["externalEffectsOnReplay"]
        assert not info["crossProcessUnknownResult"] and len(info["workers"]) >= 3
        assert info["eventCounts"]["host-begin"] >= 2
        assert [int(m["module"]) for m in info["sourceModules"]] == list(range(len(info["sourceModules"])))
        checks += 6
        record = saved.records[-1]
        page = snapshot.threaded_history(record.payload, saved.records, (1, 3))["page"]
        assert [row["index"] for row in page] == [1, 2, 3]
        assert snapshot.threaded_history(record.payload, saved.records, (info["events"], 3))["page"] == []
        checks += 2
        # Locate framing from the actual producer's extents, not a synthetic VM.
        payload = 64 + sum(32 + len(r.payload) for r in saved.records[:-1]) + 32
        at = payload + 16
        source_id = at
        for _ in info["sourceModules"]:
            size, = struct.unpack_from("<Q", original, at + 8)
            at += 16 + size
        journal = at + 8
        # Keep the outer integrity checksum valid: these must be typed refusals.
        for offset, width, value in ((40, 4, 0), (40, 4, 1), (40, 4, 2), (40, 4, 3), (40, 4, 4), (40, 4, 5), (40, 4, 6), (40, 4, 8), (44, 4, 1), (22, 1, 1),
                                     (source_id, 8, 99), (journal + 8, 8, 3),
                                     (journal + 16, 8, 2**64 - 1),
                                     (journal + 24, 8, 2**64 - 1)):
            damaged = bytearray(original)
            damaged[offset:offset + width] = value.to_bytes(width, "little")
            reject(damaged)
        for magic in (b"UTHJNL3\0", b"UTHJNL4\0", b"UTHJNL5\0", b"UTHJNL6\0"):
            damaged = bytearray(original)
            damaged[journal:journal + 8] = magic
            reject(damaged)
        # The first event is the producer's origin; the next is a real entry.
        first = journal + 32
        event_at = first
        for _ in range(info["events"]):
            kind, = struct.unpack_from("<Q", original, event_at)
            ins, outs = struct.unpack_from("<QQ", original, event_at + 64)
            if kind == 6:
                damaged = bytearray(original)
                struct.pack_into("<Q", damaged, event_at + 48, len(info["sourceModules"]) << 1)
                reject(damaged)
                break
            event_at += 80 + ins + outs
        else:
            raise AssertionError("missing genuine native-call admission")
        ins, outs = struct.unpack_from("<QQ", original, first + 64)
        second = first + 80 + ins + outs
        for offset in (second, second + 8):
            damaged = bytearray(original)
            struct.pack_into("<Q", damaged, offset, 0)
            reject(damaged)
        damaged = bytearray(original)
        struct.pack_into("<Q", damaged, journal + 8, 1 if pending else 2)
        reject(damaged)
        # Views must own immutable bytes even if the caller supplied a buffer.
        mutable = bytearray(original)
        decoded = snapshot.decode(mutable)
        mutable[:] = bytes(len(mutable))
        assert decoded.data == original
        checks += 1
    print(f"PASS actual dynamic-thread offline reader: {checks} checks")


if __name__ == "__main__":
    main()
