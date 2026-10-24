#!/usr/bin/env python3
"""Create an independently owned CP310 file tree without executing that tree.

The caller must qualify the entire source tree against its explicitly selected
historical catalog first. Nonempty files use same-filesystem FICLONE to obtain
independent inodes sharing copy-on-write extents; zero-length files are freshly
created. There is no ordinary-data-copy fallback. Every file is then fully
hashed from BOTH source and clone. No hardlink, package installation, SDK merge
or target execution is performed.
Every source entry remains opened or directory-descriptor-relative while it is
copied. Relative symlinks are preserved only when their lexical destination is
confined to the tree. No source timestamp or historical physical pin is returned
as an observation of the newly created copy.
"""
from __future__ import annotations
import hashlib
import fcntl
import json
import os
from pathlib import Path
import posixpath
import stat
import struct
import sys

MIB, GIB = 1024 ** 2, 1024 ** 3
# Linux UAPI fs.h: FICLONE = _IOW(0x94, 9, int). The explicitly admitted x86_64
# host uses asm-generic ioctl encoding: WRITE=1, DIRSHIFT=30, SIZESHIFT=16,
# TYPESHIFT=8, native int size4. This is deliberately not claimed portable to
# architectures overriding those fields.
# https://github.com/torvalds/linux/blob/master/include/uapi/linux/fs.h
# https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/ioctl.h
# https://man7.org/linux/man-pages/man2/FICLONE.2const.html
FICLONE_X86_64 = (1 << 30) | (4 << 16) | (0x94 << 8) | 9


def need(value, message):
    if not value: raise ValueError(message)


def facts(info):
    return (info.st_dev, info.st_ino, info.st_mode, info.st_size,
            info.st_mtime_ns, info.st_ctime_ns, info.st_nlink)


def directory(path):
    path = Path(path)
    need(path.is_absolute() and '..' not in path.parts, 'explicit absolute physical directory required')
    descriptor = os.open(path.anchor, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC)
    try:
        for name in path.parts[1:]:
            other = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW, dir_fd=descriptor)
            os.close(descriptor); descriptor = other
        return descriptor
    except BaseException:
        os.close(descriptor); raise


def stream_copy(source, destination, ledger, guard, *, max_entries=131072, max_bytes=3 * GIB):
    """Clone one admitted cached view to a fresh destination, using1MiB reads.

    The ledger records actual current source/copy identities and full SHA256
    for each file. It is flushed/fsynced before return. Complete source and
    destination inventories remain the caller's responsibility; the caller
    compares those catalogs before publishing a view-preparation receipt.
    """
    need(sys.platform == 'linux' and os.uname().machine == 'x86_64' and struct.calcsize('i') == 4 and
         FICLONE_X86_64 == 0x40049409, 'explicit Linux x86_64 FICLONE ioctl encoding required')
    source, destination, ledger = Path(source), Path(destination), Path(ledger)
    need(source.is_absolute() and destination.is_absolute() and ledger.is_absolute() and
         destination.parent.resolve(strict=True) == destination.parent and
         not destination.exists() and not destination.is_symlink() and
         not ledger.exists() and not ledger.is_symlink() and source != destination and
         source not in destination.parents and destination not in source.parents,
         'fresh disjoint copy and ledger paths required')
    sourcefd = directory(source); first_root = os.fstat(sourcefd)
    destination.mkdir(mode=0o700); targetfd = directory(destination)
    seen = files = links = directories = total = clones = 0
    try:
        need(os.fstat(targetfd).st_dev == first_root.st_dev, 'FICLONE requires source and fresh view on the same filesystem')
        with ledger.open('xb') as output:
            def record(value):
                output.write((json.dumps(value, sort_keys=True, separators=(',', ':')) + '\n').encode())
            def walk(original, copied, relative, depth):
                nonlocal seen, files, links, directories, total, clones
                guard(); need(depth <= 128, 'CP310 copy depth bound exceeded')
                before = os.fstat(original); names = sorted(os.listdir(original))
                need(before.st_dev == first_root.st_dev and stat.S_ISDIR(before.st_mode) and
                     not before.st_mode & 0o7022, 'unsafe/cross-filesystem source directory refused')
                for name in names:
                    guard(); seen += 1
                    need(seen <= max_entries and name not in ('.', '..') and len(name.encode()) <= 255,
                         'CP310 copy entry/name bound exceeded')
                    path = relative + '/' + name if relative else name
                    need(len(path.encode()) <= 4096, 'CP310 copy path bound exceeded')
                    first = os.stat(name, dir_fd=original, follow_symlinks=False)
                    need(first.st_dev == first_root.st_dev, 'source view crossed a mounted filesystem')
                    if stat.S_ISDIR(first.st_mode):
                        need(not first.st_mode & 0o7022, 'unsafe source directory permissions refused')
                        os.mkdir(name, first.st_mode & 0o777, dir_fd=copied)
                        old = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW, dir_fd=original)
                        new = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_CLOEXEC | os.O_NOFOLLOW, dir_fd=copied)
                        try:
                            need(facts(first) == facts(os.fstat(old)), 'source directory changed while opening')
                            walk(old, new, path, depth + 1); os.fchmod(new, first.st_mode & 0o777); os.fsync(new)
                        finally:
                            os.close(old); os.close(new)
                        directories += 1
                        record({'relative': path, 'kind': 'directory', 'mode': first.st_mode & 0o777})
                    elif stat.S_ISLNK(first.st_mode):
                        target = os.readlink(name, dir_fd=original)
                        need(target and len(target.encode()) <= 4096 and not target.startswith('/') and '\0' not in target,
                             'only bounded relative source aliases may be copied')
                        resolved = posixpath.normpath(posixpath.join(relative or '.', target))
                        need(resolved != '..' and not resolved.startswith('../'), 'copied alias escapes SDK root')
                        os.symlink(target, name, dir_fd=copied)
                        need(os.readlink(name, dir_fd=original) == target, 'source alias changed while copying')
                        links += 1; record({'relative': path, 'kind': 'symlink', 'target': target,
                            'source_device': first.st_dev, 'source_inode': first.st_ino})
                    else:
                        need(stat.S_ISREG(first.st_mode) and first.st_nlink == 1 and
                             not first.st_mode & 0o7000 and 0 <= first.st_size <= 2 * GIB,
                             'bounded non-set-id ordinary source file with one name required')
                        opened = os.open(name, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK, dir_fd=original)
                        writer = None
                        try:
                            writer = os.open(name, os.O_RDWR | os.O_CREAT | os.O_EXCL | os.O_CLOEXEC | os.O_NOFOLLOW, 0o600, dir_fd=copied)
                            need(facts(first) == facts(os.fstat(opened)), 'source file changed while opening')
                            if first.st_size:
                                try:
                                    fcntl.ioctl(writer, FICLONE_X86_64, opened)
                                except OSError as error:
                                    raise OSError(error.errno, 'required FICLONE failed; ordinary COPY fallback is disabled', str(destination / path)) from error
                                clones += 1
                            digest = hashlib.sha256(); count = 0
                            while True:
                                guard(); block = os.read(opened, MIB)
                                if not block: break
                                count += len(block); total += len(block)
                                need(count <= first.st_size and total <= max_bytes, 'CP310 copy byte bound exceeded')
                                digest.update(block)
                            need(count == first.st_size and facts(first) == facts(os.fstat(opened)), 'source bytes changed while copying')
                            os.fchmod(writer, first.st_mode & 0o777); os.fsync(writer)
                            actual = os.fstat(writer)
                            need(actual.st_nlink == 1 and actual.st_size == count and (actual.st_dev, actual.st_ino) != (first.st_dev, first.st_ino),
                                 'copy must have its own ordinary inode')
                            os.lseek(writer, 0, os.SEEK_SET); cloned_digest = hashlib.sha256(); cloned_bytes = 0
                            while True:
                                guard(); block = os.read(writer, MIB)
                                if not block: break
                                cloned_bytes += len(block); need(cloned_bytes <= count, 'new clone exceeded source length')
                                cloned_digest.update(block)
                            need(cloned_bytes == count and cloned_digest.digest() == digest.digest() and
                                 facts(actual) == facts(os.fstat(writer)) and facts(first) == facts(os.fstat(opened)),
                                 'full cloned bytes/source identity changed or differ')
                        finally:
                            os.close(opened)
                            if writer is not None: os.close(writer)
                        pin = lambda root, info: {'path': str(root / path), 'bytes': count, 'sha256': digest.hexdigest(),
                            'device': info.st_dev, 'inode': info.st_ino, 'mtime_ns': info.st_mtime_ns, 'ctime_ns': info.st_ctime_ns}
                        files += 1; record({'relative': path, 'kind': 'file', 'mode': first.st_mode & 0o777,
                            'source_current': pin(source, first), 'copy_actual': pin(destination, actual),
                            'copy_method': 'FICLONE_REQUIRED_INDEPENDENT_INODE_FULL_REHASH' if first.st_size else 'EMPTY_FRESH_ORDINARY_FILE'})
                    need(facts(first) == facts(os.stat(name, dir_fd=original, follow_symlinks=False)),
                         'source named entry changed while copying')
                need(names == sorted(os.listdir(original)) and facts(before) == facts(os.fstat(original)),
                     'source directory changed while copying')
            walk(sourcefd, targetfd, '', 0)
            output.flush(); os.fsync(output.fileno())
        need(facts(first_root) == facts(os.fstat(sourcefd)) == facts(source.stat(follow_symlinks=False)),
             'source view root changed during independent copy')
        os.fsync(targetfd)
        parent = directory(destination.parent)
        try: os.fsync(parent)
        finally: os.close(parent)
        return {'kind': 'independent-current-CP310-verified-CoW-file-copy', 'source_root': str(source),
            'destination_root': str(destination), 'entries': seen, 'files': files, 'directories': directories,
            'symlinks': links, 'file_bytes': total, 'ledger': str(ledger), 'reflinks_used': clones > 0, 'FICLONE_successful_files': clones,
            'ordinary_copy_fallback_allowed': False, 'source_and_clone_fully_rehashed': True,
            'hardlinks_used': False, 'source_timestamps_reused_as_current': False, 'target_code_executed': False,
            'scope': 'FRESH_FILE_BYTES_ONLY; INDEPENDENT_FULL_CATALOG_COMPARISON_REQUIRED'}
    finally:
        os.close(sourcefd); os.close(targetfd)
