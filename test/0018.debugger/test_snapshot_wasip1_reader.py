#!/usr/bin/env python3
"""Independent canonical WASI archive-reader and hostile extent tests."""
from pathlib import Path
import struct
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools/debug"))
import snapshot


def blob(value):
    return struct.pack("<Q", len(value)) + value


def fixture(kind=4, path=b"", mount=b"", content=b""):
    header = struct.pack("<8sII16s32s32s9Q", b"UWWASI1\0", 1, 0, b"x" * 16, b"a" * 32, b"b" * 32,
                         2, 3, 2, 2, 1, 1, 1, 2, 0)
    resource = struct.pack("<BBBBHHQ", kind, 1 if kind == 4 else 0, 0, 0, 0, 0, 0)
    resource += blob(mount) + blob(path) + blob(content)
    return (header + struct.pack("<2Q", 2, 3) + blob(b"robot") + blob(b"a" * 8193) + blob(b"MODE=restore") + resource +
            struct.pack("<IIQQ", 1, 0, 64, 0) + struct.pack("<IB3xQQ", 0, 2, 0, 0) + struct.pack("<IB3xQQ", 2, 3, 0, 0))


def main():
    data = fixture()
    result = snapshot.wasip1(memoryview(data))
    assert result["members"] == ["2", "3"] and result["protectedSlots"] == 2
    assert result["argumentCount"] == 2 and not result["restoreAuthority"]
    checks = 1

    def reject(value):
        nonlocal checks
        try:
            snapshot.wasip1(memoryview(value))
        except snapshot.InvalidSnapshot:
            checks += 1
        else:
            raise AssertionError("malformed WASI environment accepted")

    for cut in range(0, len(data), 79):
        reject(data[:cut])
    for at in range(112, 168, 8):
        bad = bytearray(data)
        struct.pack_into("<Q", bad, at, (1 << 64) - 1)
        reject(bad)
    for path in (b"..", b"../a", b"a/..", b"a//b", b"/a", b"a/", b"a\\b", b"a:b"):
        reject(fixture(kind=0, mount=b"work", path=path))
    reject(fixture(content=b"external write is not a checkpoint rollback"))
    bad = bytearray(data)
    struct.pack_into("<Q", bad, len(data) - 16, 1)
    reject(bad)  # protected FD 2 may not acquire even one right
    bad = bytearray(data)
    struct.pack_into("<I", bad, len(data) - 24, 0)
    reject(bad)  # duplicate FD 0
    bad = bytearray(data)
    struct.pack_into("<Q", bad, 176, 2)
    reject(bad)  # duplicate policy member
    reject(data + b"\0")
    print(f"PASS independent WASIp1 snapshot reader: {checks} checks")


if __name__ == "__main__":
    main()
