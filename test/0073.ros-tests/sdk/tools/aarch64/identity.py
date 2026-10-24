#!/usr/bin/env python3
"""@file identity.py
@brief Inspect immutable executable identities and actual memory limits.
@details This module reads file bytes; it never infers an instruction set from
a filename, a wrapper, a target triple or an emulated uname response.
"""

import hashlib
import json
import os
from pathlib import Path
import struct
import time


def sha256(path):
    """@brief Hash a complete file in bounded chunks.
    @param path Readable regular input file; ownership remains with the caller.
    @return Lowercase hexadecimal SHA-256 digest.
    @exception OSError The file cannot be read completely.
    """
    digest = hashlib.sha256()
    with Path(path).open("rb") as stream:
        for block in iter(lambda: stream.read(1048576), b""):
            digest.update(block)
    return digest.hexdigest()


def elf(path):
    """@brief Read the ELF class, byte order and machine from its actual header.
    @param path Executable or library, including a resolved interpreter ELF.
    @return A metadata dictionary, or None for an ordinary non-ELF input.
    @exception ValueError A purported ELF has a truncated or invalid header.
    """
    with Path(path).open("rb") as stream:
        header = stream.read(64)
    if header[:4] != b"\x7fELF":
        return None
    if len(header) < 52 or header[4] not in (1, 2) or header[5] not in (1, 2):
        raise ValueError("Invalid ELF identification: " + str(path))
    if header[4] == 2 and len(header) < 64:
        raise ValueError("Truncated ELF64 header: " + str(path))
    order = "<" if header[5] == 1 else ">"
    return {
        "bits": 32 if header[4] == 1 else 64,
        "byte_order": "little" if header[5] == 1 else "big",
        "machine": struct.unpack_from(order + "H", header, 18)[0],
    }


def require_native_elf(path, machine):
    """@brief Require real ELF64 bytes for the selected native target machine.
    @param path Complete native runtime, gateway, peer or interpreter file.
    @param machine Expected actual ELF e_machine, independently selected by the cohort.
    @return Actual validated file-header metadata.
    @exception ValueError Non-ELF, ELF32 or another machine cannot qualify this cohort.
    """
    metadata = elf(path)
    if metadata is None or metadata['bits']!=64 or metadata['machine']!=machine:
        raise ValueError('A native target participant must be the selected real ELF64 architecture')
    return metadata


def cgroup():
    """@brief Require the current process's actual cgroup v2 limits.
    @return Membership and measured memory.max/memory.swap.max in bytes.
    @exception RuntimeError Limits differ from 64 GiB RAM and zero swap.
    """
    filename = os.environ.get("UWVM_ACCEPTANCE_ADMISSION")
    nonce = os.environ.get("UWVM_ACCEPTANCE_NONCE")
    if filename or nonce:
        if not filename or not nonce or len(nonce) != 32:
            raise RuntimeError("Incomplete host cgroup admission identity")
        deadline = time.monotonic() + 30
        while not Path(filename).is_file():
            if time.monotonic() >= deadline:
                raise RuntimeError("Host cgroup verification was not published")
            time.sleep(0.05)
        gate = json.loads(Path(filename).read_text())
        if (gate.get("nonce") != nonce or gate.get("status") != "verified"
                or gate.get("memory_max") != "68719476736" or gate.get("memory_swap_max") != "0"
                or not isinstance(gate.get("pid"), int) or gate["pid"] <= 0):
            raise RuntimeError("Invalid host cgroup admission receipt")
    membership = Path("/proc/self/cgroup").read_text()
    base = Path("/sys/fs/cgroup")
    values = {
        name: (base / name).read_text().strip()
        for name in ("memory.max", "memory.swap.max")
    }
    if values != {"memory.max": "68719476736", "memory.swap.max": "0"}:
        raise RuntimeError("The actual execution cgroup has wrong limits: " + str(values))
    return {"membership": membership, **values}


def _uleb(data, offset, limit):
    """@brief Read one bounded unsigned LEB128 word for Wasm metadata inspection."""
    result = 0
    for shift in range(0, 70, 7):
        if offset >= limit:
            raise ValueError("Truncated Wasm LEB128")
        octet = data[offset]
        offset += 1
        result |= (octet & 127) << shift
        if not octet & 128:
            if shift == 63 and octet > 1:
                raise ValueError("Wasm LEB128 exceeds uint64")
            return result, offset
    raise ValueError("Invalid Wasm LEB128")


def wasm_memories(path, *, require_memory=True):
    """@brief Inspect every defined memory's actual memory64 flag.
    @param path One complete Wasm v1 application or preload library file.
    @param require_memory Require a defined memory for an application. A pure
    function library may explicitly select False and return an empty descriptor list.
    @return Defined memory descriptors; no compiler or execution claim follows.
    @exception ValueError Invalid, truncated or unsupported memory metadata.
    @details Imported memories deliberately fail admission. The harness needs a
    self-contained command with a defined memory to prove the requested width.
    A memoryless library does not prove a pointer width or permit shared pointers.
    """
    data = Path(path).read_bytes()
    if data[:8] != b"\0asm\x01\0\0\0":
        raise ValueError("Expected an actual Wasm v1 module")
    offset, memories, memory_section_seen = 8, [], False
    while offset < len(data):
        section = data[offset]
        size, begin = _uleb(data, offset + 1, len(data))
        end = begin + size
        if end > len(data):
            raise ValueError("Wasm section extends beyond the file")
        if section == 2:
            count, cursor = _uleb(data, begin, end)
            for _ in range(count):
                for _ in range(2):
                    length, cursor = _uleb(data, cursor, end)
                    cursor += length
                    if cursor > end:
                        raise ValueError("Truncated Wasm import name")
                if cursor >= end:
                    raise ValueError("Truncated Wasm import descriptor")
                kind = data[cursor]
                cursor += 1
                if kind == 0:
                    _, cursor = _uleb(data, cursor, end)
                elif kind == 1:
                    if cursor >= end or data[cursor] not in (0x70, 0x6F):
                        raise ValueError("Unqualified Wasm imported table type")
                    cursor += 1
                    flags, cursor = _uleb(data, cursor, end)
                    _, cursor = _uleb(data, cursor, end)
                    if flags & 1:
                        _, cursor = _uleb(data, cursor, end)
                elif kind == 2:
                    raise ValueError("Imported memory cannot qualify a sole command memory")
                elif kind == 3:
                    if cursor + 2 > end or data[cursor] not in (0x7F, 0x7E, 0x7D, 0x7C, 0x7B, 0x70, 0x6F):
                        raise ValueError("Unqualified Wasm imported global type")
                    cursor += 2
                elif kind == 4:
                    if cursor >= end or data[cursor] != 0:
                        raise ValueError("Invalid imported exception tag")
                    _, cursor = _uleb(data, cursor + 1, end)
                else:
                    raise ValueError("Unknown Wasm import kind")
            if cursor != end:
                raise ValueError("Trailing import section bytes")
        elif section == 5:
            if memory_section_seen:
                raise ValueError("Duplicate Wasm memory section")
            memory_section_seen = True
            count, cursor = _uleb(data, begin, end)
            for _ in range(count):
                flags, cursor = _uleb(data, cursor, end)
                if flags & ~7:
                    raise ValueError("Unsupported memory flags")
                minimum, cursor = _uleb(data, cursor, end)
                maximum = None
                if flags & 1:
                    maximum, cursor = _uleb(data, cursor, end)
                if (flags & 2 and maximum is None) or (maximum is not None and maximum < minimum):
                    raise ValueError("Invalid Wasm memory limits")
                memories.append({"bits": 64 if flags & 4 else 32,
                                 "shared": bool(flags & 2),
                                 "minimum_pages": minimum,
                                 "maximum_pages": maximum})
            if cursor != end:
                raise ValueError("Trailing memory section bytes")
        offset = end
    if require_memory and not memories:
        raise ValueError("A command must define its selected memory")
    return memories


def mapped_files(pid, destination):
    """@brief Retain a live process's loader mappings and persistent file hashes.
    @param pid Owned child process identifier; never an unrelated participant.
    @param destination New raw maps receipt path.
    @return Persistent and transient mapping observations, including actual ELFs.
    @exception OSError The process exited before its maps could be retained.
    """
    raw = Path("/proc", str(pid), "maps").read_text()
    Path(destination).write_text(raw)
    result = []
    for name in sorted({line.split(maxsplit=5)[5] for line in raw.splitlines()
                        if len(line.split(maxsplit=5)) == 6
                        and line.split(maxsplit=5)[5].startswith("/")}):
        path = Path(name)
        row = {"path": name, "persistent": path.is_file()}
        if row["persistent"]:
            row.update(sha256=sha256(path), elf=elf(path))
        result.append(row)
    return result
