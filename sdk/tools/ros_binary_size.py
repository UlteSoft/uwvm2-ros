#!/usr/bin/env python3
"""Measure an existing Linux ELF without executing or modifying the executable.

The JSON separates disk extent, PT_LOAD file bytes, PT_LOAD memory bytes and
non-allocated symbols/debug data. PT_LOAD totals are ELF extents, not measured
RSS, page sharing, a throughput result or a dependency deployment size.
"""
import argparse
import hashlib
import json
import mmap
import os
from pathlib import Path
import stat
import struct


def measure(path):
    """Read one bounded ordinary ELF through one descriptor, checking stability."""
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or not 64 <= before.st_size <= 2 * 1024**3:
            raise ValueError('expected an ordinary ELF of 64 bytes..2 GiB')
        with mmap.mmap(fd, 0, access=mmap.ACCESS_READ) as data:
            if data[:4] != b'\x7fELF' or data[4] not in (1, 2) or data[5] not in (1, 2):
                raise ValueError('expected ELF32/ELF64 in a declared byte order')
            bits, order = (32 if data[4] == 1 else 64), ('<' if data[5] == 1 else '>')
            def unpack(fmt, offset):
                size = struct.calcsize(order + fmt)
                if offset < 0 or offset + size > len(data):
                    raise ValueError('ELF table exceeds file extent')
                return struct.unpack_from(order + fmt, data, offset)
            header = unpack('HHIIIIIHHHHHH' if bits == 32 else 'HHIQQQIHHHHHH', 16)
            kind, machine, version, _, phoff, shoff, flags, ehsize, phentsize, phnum, shentsize, shnum, shstr = header
            if version != 1 or ehsize != (52 if bits == 32 else 64):
                raise ValueError('unsupported ELF header')
            if kind not in (2, 3) or not phnum:
                raise ValueError('expected an executable/shared-image ELF with load headers')
            pfmt = 'IIIIIIII' if bits == 32 else 'IIQQQQQQ'
            sfmt = 'IIIIIIIIII' if bits == 32 else 'IIQQQQIIQQ'
            if phnum == 65535 or shnum == 0 or shstr == 65535:
                raise ValueError('extended or absent section tables require a separate reviewed reader')
            if (phnum and phentsize != struct.calcsize(order + pfmt)) or shentsize != struct.calcsize(order + sfmt):
                raise ValueError('unexpected ELF entry size')
            sections = [unpack(sfmt, shoff + i * shentsize) for i in range(shnum)]
            if shstr >= len(sections):
                raise ValueError('invalid section-name table')
            strings = sections[shstr]
            def extent(offset, length):
                if offset > len(data) or length > len(data) - offset:
                    raise ValueError('ELF content exceeds file extent')
            extent(strings[4], strings[5])
            def name_at(index):
                if index >= strings[5]:
                    raise ValueError('invalid section-name offset')
                start = strings[4] + index
                end = data.find(b'\0', start, strings[4] + strings[5])
                if end == -1:
                    raise ValueError('unterminated section name')
                return data[start:end].decode('utf-8', errors='replace')
            rows = []
            for row in sections:
                name, kind, flags, address, offset, size = row[:6]
                if kind != 8:  # SHT_NOBITS has no corresponding file content.
                    extent(offset, size)
                rows.append(dict(name=name_at(name), type=kind, allocated=bool(flags & 2),
                                 address=address, file_offset=offset,
                                 file_bytes=0 if kind == 8 else size, memory_bytes=size))
            loads = []
            for i in range(phnum):
                row = unpack(pfmt, phoff + i * phentsize)
                if bits == 32:
                    kind, offset, address, _, size, memory, flags, align = row
                else:
                    kind, flags, offset, address, _, size, memory, align = row
                extent(offset, size)
                if kind == 1:
                    if memory < size:
                        raise ValueError('ELF load memory is shorter than its file extent')
                    loads.append(dict(file_offset=offset, virtual_address=address,
                                      file_bytes=size, memory_bytes=memory, flags=flags, alignment=align))
            digest = hashlib.sha256(data).hexdigest()
            if not loads:
                raise ValueError('ELF has no load segments')
        after = os.fstat(fd)
        if (before.st_dev, before.st_ino, before.st_size, before.st_mtime_ns, before.st_ctime_ns) != (
                after.st_dev, after.st_ino, after.st_size, after.st_mtime_ns, after.st_ctime_ns):
            raise ValueError('ELF changed during measurement')
        return dict(path=str(Path(path).absolute()), bytes=before.st_size, sha256=digest,
                    device=before.st_dev, inode=before.st_ino, mtime_ns=before.st_mtime_ns,
                    ctime_ns=before.st_ctime_ns, elf_class=bits, machine=machine, elf_flags=flags,
                    byte_order='little' if order == '<' else 'big', load_segments=loads,
                    load_file_bytes=sum(r['file_bytes'] for r in loads),
                    load_memory_bytes=sum(r['memory_bytes'] for r in loads),
                    allocated_section_file_bytes=sum(r['file_bytes'] for r in rows if r['allocated']),
                    debug_section_file_bytes=sum(r['file_bytes'] for r in rows if r['name'].startswith(('.debug', '.zdebug'))),
                    symbol_table_file_bytes=sum(r['file_bytes'] for r in rows if r['name'] in ('.symtab', '.strtab')),
                    sections=sorted(rows, key=lambda r: r['memory_bytes'], reverse=True))
    finally:
        os.close(fd)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('binary', type=Path)
    p.add_argument('--max-file-bytes', type=int)
    p.add_argument('--max-load-bytes', type=int)
    a = p.parse_args()
    if any(x is not None and x < 1 for x in (a.max_file_bytes, a.max_load_bytes)):
        p.error('budgets must be positive byte counts')
    try:
        result = measure(a.binary)
    except (ValueError, OSError, struct.error) as error:
        p.error(str(error))
    limits = {'file_bytes': a.max_file_bytes, 'load_file_bytes': a.max_load_bytes}
    passed = ((a.max_file_bytes is None or result['bytes'] <= a.max_file_bytes) and
              (a.max_load_bytes is None or result['load_file_bytes'] <= a.max_load_bytes))
    print(json.dumps(dict(kind='observed-linux-elf-size-v1', binary=result,
                          budgets=limits, passed=passed, executable_invocations=0), indent=2))
    return 0 if passed else 1


if __name__ == '__main__':
    raise SystemExit(main())
