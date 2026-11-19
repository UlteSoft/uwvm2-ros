#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0
"""@file split_linux_debug.py
@brief Prepare a pinned Linux runtime and its separate DWARF companion.
"""
import argparse
import hashlib
import mmap
import os
from pathlib import Path
import stat
import struct
import subprocess
import sys
import tempfile
import zlib

from install import MAXIMUM_IMAGE, digest, new_json, regular_input, require, unchanged


CHUNK = 1 << 20
MAX_BUILD_IMAGE = 512 << 20


class Elf:
    """@brief Inspect the bounded layout of a fully linked ELF32 or ELF64 image."""

    def __init__(self, image):
        self.image = image
        ident = image[:16]
        require(len(ident) == 16 and ident[:4] == b"\x7fELF" and ident[6] == 1,
                "Expected a current ELF image")
        require(ident[4] in (1, 2) and ident[5] in (1, 2), "Unsupported ELF encoding")
        self.wide = ident[4] == 2
        self.endian = "<" if ident[5] == 1 else ">"
        header = struct.Struct(self.endian + ("HHIQQQIHHHHHH" if self.wide else "HHIIIIIHHHHHH"))
        self.header_size = 16 + header.size
        fields = list(header.unpack(self.region(16, header.size)))
        kind, _, version, _, phoff, shoff, _, ehsize, phsize, phnum, shsize, shnum, shstr = fields
        require(kind in (2, 3) and version == 1 and ehsize == self.header_size,
                "Expected a fully linked executable or PIE")

        # A separate section table can move without changing the loaded image.
        fields[5] = 0
        fields[10:13] = [0, 0, 0]
        self.load_header = ident + header.pack(*fields)
        self.program = struct.Struct(self.endian + ("IIQQQQQQ" if self.wide else "IIIIIIII"))
        require(0 < phnum <= 256 and phsize == self.program.size and phoff >= ehsize,
                "Invalid ELF program header table")
        self.program_headers = self.region(phoff, phnum * phsize)

        section = struct.Struct(self.endian + ("IIQQQQIIQQ" if self.wide else "IIIIIIIIII"))
        require(0 < shstr < shnum <= 4096 and shsize == section.size and shoff >= ehsize,
                "Invalid ELF section table")
        rows = tuple(section.iter_unpack(self.region(shoff, shnum * shsize)))
        strings = rows[shstr]
        require(strings[1] == 3 and strings[5] <= CHUNK, "Invalid ELF section names")
        names = self.region(strings[4], strings[5])
        self.sections = {}
        for name_offset, section_type, _, _, offset, size, *_ in rows[1:]:
            end = names.find(b"\0", name_offset)
            require(end >= name_offset, "Unterminated ELF section name")
            name = names[name_offset:end]
            if name.startswith((b".debug_", b".zdebug_")) or name == b".gnu_debuglink":
                require(section_type == 1 and size > 0 and offset + size <= len(image)
                        and name not in self.sections, "Invalid ELF debug section")
                self.sections[name] = (offset, size)

    def region(self, offset, size):
        """@brief Read a byte range wholly contained in the pinned image."""
        require(offset + size <= len(self.image), "ELF range outside the file")
        return self.image[offset:offset + size]

    def fingerprint(self):
        """@brief Hash the loader contract and every file-backed segment."""
        result = hashlib.sha256(self.load_header + self.program_headers)
        loadable = False
        for header in self.program.iter_unpack(self.program_headers):
            kind = header[0]
            offset, size = (header[2], header[5]) if self.wide else (header[1], header[4])
            loadable |= kind == 1 and size > 0
            end = offset + size
            require(end <= len(self.image), "ELF segment outside the file")
            # Use the canonical header in any segment that maps it.
            if size and offset < self.header_size:
                last = min(end, self.header_size)
                result.update(self.load_header[offset:last])
                offset = last
            for start in range(offset, end, CHUNK):
                result.update(self.image[start:min(end, start + CHUNK)])
        require(loadable, "Expected at least one file-backed LOAD segment")
        return result.hexdigest()

    def debug_fingerprints(self):
        """@brief Identify the exact debug data independently of section placement."""
        return {name: hashlib.sha256(self.region(*span)).hexdigest()
                for name, span in self.sections.items() if name != b".gnu_debuglink"}


def sha(path):
    """@brief Hash one file without loading the whole binary into Python."""
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def snapshot(path, expected, destination):
    """@brief Copy only the operator-approved input extent into private staging."""
    stream, before = regular_input(path, MAX_BUILD_IMAGE)
    with stream, destination.open("xb") as output:
        require(stat.S_IMODE(before.st_mode) & 0o111, "Source image has no execute bit")
        copied = hashlib.sha256()
        remaining = before.st_size
        while remaining:
            chunk = stream.read(min(remaining, CHUNK))
            require(bool(chunk), "Source image ended during copy")
            output.write(chunk)
            copied.update(chunk)
            remaining -= len(chunk)
        require(unchanged(before, os.fstat(stream.fileno())) and copied.hexdigest() == expected,
                "Source image digest or identity mismatch")
    return before.st_size


def verify_triplet(original, executable, sidecar):
    """@brief Verify loaded bytes, detached debug sections and the debuglink CRC."""
    with original.open("rb") as source, executable.open("rb") as runtime, sidecar.open("rb") as debug:
        with (mmap.mmap(source.fileno(), 0, access=mmap.ACCESS_READ) as a,
              mmap.mmap(runtime.fileno(), 0, access=mmap.ACCESS_READ) as b,
              mmap.mmap(debug.fileno(), 0, access=mmap.ACCESS_READ) as c):
            loaded = Elf(a)
            compact = Elf(b)
            symbols = Elf(c)
            fingerprint = loaded.fingerprint()
            require(compact.fingerprint() == fingerprint, "ELF loader-visible bytes changed")
            require(b".gnu_debuglink" not in loaded.sections, "Input already has a detached debug link")
            debug_data = loaded.debug_fingerprints()
            require({b".debug_info", b".debug_line"} <= debug_data.keys(), "Input needs complete DWARF sections")
            require(symbols.debug_fingerprints() == debug_data, "Detached DWARF bytes changed")
            require(compact.sections.keys() == {b".gnu_debuglink"}, "Expected only a debug link in the runtime")
            crc = 0
            for start in range(0, len(c), CHUNK):
                crc = zlib.crc32(c[start:start + CHUNK], crc)
            name = os.fsencode(sidecar.name) + b"\0"
            expect = name + bytes(-len(name) % 4) + struct.pack(compact.endian + "I", crc)
            require(compact.region(*compact.sections[b".gnu_debuglink"]) == expect, "Invalid GNU debug link")
            return fingerprint


def main():
    """@brief Publish one executable and companion bound to their source identities."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--image", type=Path, required=True)
    parser.add_argument("--sha256", required=True, help="approved input image SHA-256")
    parser.add_argument("--source-identity", required=True, help="approved sha256:SOURCE_DIGEST")
    parser.add_argument("--objcopy", type=Path, required=True, help="trusted objcopy with support for the input ELF target")
    parser.add_argument("--output", type=Path, required=True, help="new directory for the executable, DWARF and receipt")
    args = parser.parse_args()
    require(sys.platform == "linux", "This packaging profile is Linux-only")
    expected = digest(args.sha256)
    require(args.source_identity.startswith("sha256:"), "Expected an explicit sha256 source identity")
    digest(args.source_identity[7:])
    tool = args.objcopy.resolve(strict=True)
    tool_sha = sha(tool)
    args.output.mkdir(mode=0o700)
    with tempfile.TemporaryDirectory(dir=args.output) as scratch:
        stage = Path(scratch).resolve()
        original = stage / "source.elf"
        size = snapshot(args.image, expected, original)
        executable = stage / "uwvm2-ros"
        sidecar = stage / "uwvm2-ros.debug"
        subprocess.run([tool, "--only-keep-debug", original, sidecar], check=True, timeout=120)
        subprocess.run([tool, "--strip-debug", f"--add-gnu-debuglink={sidecar}", original, executable],
                       check=True, timeout=120)
        require(sha(tool) == tool_sha, "objcopy changed during packaging")
        require(sha(original) == expected, "Staged source changed during packaging")
        require(executable.stat().st_size < size and executable.stat().st_size <= MAXIMUM_IMAGE,
                "Compact image must be smaller and fit the Linux runtime installer")
        fingerprint = verify_triplet(original, executable, sidecar)
        artifact = {"sha256": sha(executable), "bytes": executable.stat().st_size}
        debug = {"sha256": sha(sidecar), "bytes": sidecar.stat().st_size, "file": sidecar.name}
        executable.chmod(0o755)
        sidecar.chmod(0o644)
        os.link(executable, args.output / executable.name)
        os.link(sidecar, args.output / sidecar.name)
        new_json(args.output / "producer-receipt.json", {
            "format": "uwvm2-ros-linux-separated-debug", "source_identity": args.source_identity,
            "original": {"sha256": expected, "bytes": size}, "artifact": artifact,
            "debug": debug, "objcopy_sha256": tool_sha, "loaded_elf_sha256": fingerprint,
        })
    print(args.output / "producer-receipt.json")


if __name__ == "__main__":
    try:
        main()
    except (OSError, ValueError, subprocess.SubprocessError, struct.error) as error:
        sys.exit("runtime packaging refused: " + str(error))
