#!/usr/bin/env python3
"""Explicit owned-image CoW and free-block barriers for Noetic r6.

No filesystem, loop device, cgroup or tool is discovered for authority. Root's
SHA-bound current control names one already-owned Btrfs mount, its existing
sparse backing image, UUID and loop device, and one pinned fstrim executable.
Trimming discards unused filesystem blocks only; it removes no file or input.
The caller supplies a safety-only guard for trim/finalization so low disk never
prevents owned pidfd cleanup. Active payload work keeps its separate8GiB guard.
"""
from __future__ import annotations
import fcntl
import hashlib
import os
from pathlib import Path
import re
import stat
import struct
import sys

MIB, GIB = 1024 ** 2, 1024 ** 3
FICLONE_X86_64 = (1 << 30) | (4 << 16) | (0x94 << 8) | 9
# Linux UAPI _IOW(0x94,9,int), x86_64 asm-generic ioctl encoding.
# https://github.com/torvalds/linux/blob/master/include/uapi/linux/fs.h
# https://github.com/torvalds/linux/blob/master/include/uapi/asm-generic/ioctl.h
# https://man7.org/linux/man-pages/man2/FICLONE.2const.html


def need(value, message):
    if not value: raise ValueError(message)


def facts(info):
    return (info.st_dev, info.st_ino, info.st_mode, info.st_size,
            info.st_mtime_ns, info.st_ctime_ns, info.st_nlink)


def clone_descriptors(source, destination, guard):
    """Require a same-FS independent-inode clone; verify complete cloned bytes.

    Both descriptors are owned by the caller. They remain open at their original
    positions. No timestamp is copied. The source is fully hashed before and
    after the ioctl, and the destination is fully read through a separate FD.
    There is no physical ordinary-copy fallback on an unsupported filesystem.
    """
    need(sys.platform == 'linux' and os.uname().machine == 'x86_64' and struct.calcsize('i') == 4 and
         FICLONE_X86_64 == 0x40049409, 'explicit Linux x86_64 FICLONE encoding required')
    initial, fresh = os.fstat(source), os.fstat(destination)
    need(stat.S_ISREG(initial.st_mode) and stat.S_ISREG(fresh.st_mode) and initial.st_nlink == fresh.st_nlink == 1 and
         initial.st_dev == fresh.st_dev and initial.st_ino != fresh.st_ino and fresh.st_size == 0 and
         0 <= initial.st_size <= 2 * GIB, 'fresh empty independent inode on the same filesystem required')
    def digest(descriptor, length):
        value = hashlib.sha256(); offset = 0
        while offset < length:
            guard(); block = os.pread(descriptor, min(MIB, length - offset), offset)
            need(block, 'descriptor bytes truncated during clone validation'); value.update(block); offset += len(block)
        need(not os.pread(descriptor, 1, length), 'descriptor grew during clone validation')
        return value.hexdigest()
    original = digest(source, initial.st_size)
    if initial.st_size:
        try: fcntl.ioctl(destination, FICLONE_X86_64, source)
        except OSError as error:
            raise OSError(error.errno, 'required same-filesystem FICLONE failed; no ordinary-copy fallback') from error
    os.fsync(destination); after = os.fstat(destination)
    need(after.st_size == initial.st_size and after.st_nlink == 1 and after.st_dev == initial.st_dev and after.st_ino != initial.st_ino,
         'new clone identity/size differs')
    cloned = digest(destination, initial.st_size)
    need(cloned == original and facts(initial) == facts(os.fstat(source)) and
         original == digest(source, initial.st_size) and facts(initial) == facts(os.fstat(source)) and
         facts(after) == facts(os.fstat(destination)), 'source or clone bytes/identity changed')
    return {'kind': 'same-filesystem-independent-inode-FICLONE-full-read-validation', 'bytes': initial.st_size,
        'sha256': original, 'source_device': initial.st_dev, 'source_inode': initial.st_ino,
        'clone_device': after.st_dev, 'clone_inode': after.st_ino,
        'ioctl_performed': bool(initial.st_size), 'ordinary_copy_fallback': False}


def unescape(value):
    return re.sub(r'\\([0-7]{3})', lambda match: chr(int(match[1], 8)), value)


class OwnedStorage:
    """Re-admit the exact current owned mount before every trim command."""
    def __init__(self, selection, *, safety_guard, run_trim, common, remember, product, report, out):
        self.selection, self.guard, self.run_trim, self.common = selection, safety_guard, run_trim, common
        self.remember, self.product, self.report, self.out = remember, product, report, Path(out)
        self.sequence = 0
        need(type(selection) is dict and set(selection) == {'mountpoint', 'mount_root', 'filesystem_uuid', 'loop_name',
            'mount_device', 'root_inode', 'image', 'mount', 'uuid_device_link', 'fstrim'}, 'closed owned storage selection required')
        need(selection['mountpoint'] == '/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1' and
             selection['mount_root'] == '/' and selection['filesystem_uuid'] == '3fe7718a-8039-4f1c-b540-7c5cf05b65ca' and
             selection['loop_name'] == 'loop29' and selection['root_inode'] == 256, 'only the original owned artifact image may be trimmed')
        image = selection['image']
        need(set(image) == {'path', 'device', 'inode', 'logical_bytes'} and image['path'] ==
             '/var/tmp/uwvm-ros-complete-20261015-r1/artifact-storage-r1.btrfs' and image['inode'] == 30723291 and
             image['logical_bytes'] == 274877906944, 'exact original owned sparse image identity required')
        tool = selection['fstrim']
        need(tool['path'] == '/usr/sbin/fstrim' and tool['bytes'] == 43544 and tool['sha256'] ==
             '74f99afbf098df6701fdf4bda8d258ca03b1306d131f341f7ec9530681f55b1d', 'exact explicitly selected fstrim bytes required')
        self.remember(tool['path'], tool['sha256'])
        self.report['owned_storage'] = {'selection': selection, 'trim_barriers': [], 'files_deleted': False,
            'root_filesystems_trimmed': False, 'image_mtime_or_allocation_claimed_immutable': False}

    def verify(self):
        self.guard(); selected = self.selection; root = Path(selected['mountpoint'])
        need(root.resolve(strict=True) == root and root.is_dir(), 'canonical owned mountpoint required')
        info = root.stat(); need((info.st_dev, info.st_ino) == (selected['mount_device'], selected['root_inode']), 'actual owned mount root differs')
        image = Path(selected['image']['path']); need(image.resolve(strict=True) == image and not image.is_symlink(), 'physical owned backing image required')
        descriptor = os.open(image, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC | os.O_NONBLOCK)
        try:
            before = os.fstat(descriptor); named = image.stat(follow_symlinks=False)
            need(stat.S_ISREG(before.st_mode) and (before.st_dev, before.st_ino, before.st_size) ==
                 (selected['image']['device'], selected['image']['inode'], selected['image']['logical_bytes']) and
                 (before.st_dev, before.st_ino, before.st_size) == (named.st_dev, named.st_ino, named.st_size), 'backing image identity changed')
        finally: os.close(descriptor)
        backing = Path('/sys/block') / selected['loop_name'] / 'loop/backing_file'
        need(backing.read_text().strip() == str(image), 'selected loop no longer refers to owned backing file')
        devices = Path('/sys/fs/btrfs') / selected['filesystem_uuid'] / 'devices'
        need(sorted(path.name for path in devices.iterdir()) == [selected['loop_name']], 'owned UUID has an unexpected device set')
        link = devices / selected['loop_name']; prior = link.lstat(); target = os.readlink(link); resolved = str(link.resolve(strict=True)); after = link.lstat()
        need(stat.S_ISLNK(prior.st_mode) and facts(prior) == facts(after) and target == selected['uuid_device_link']['target'] and
             resolved == selected['uuid_device_link']['resolved'] and resolved == '/sys/devices/virtual/block/' + selected['loop_name'],
             'actual filesystem UUID/loop sysfs relation differs')
        matches = []
        for raw in Path('/proc/self/mountinfo').read_text().splitlines():
            left, separator, right = raw.partition(' - '); fields, tail = left.split(), right.split()
            if separator and len(fields) >= 6 and len(tail) == 3 and unescape(fields[4]) == str(root):
                matches.append({'major_minor': fields[2], 'root': unescape(fields[3]), 'mountpoint': unescape(fields[4]),
                    'mount_options': fields[5], 'filesystem_type': tail[0], 'source': unescape(tail[1]), 'super_options': tail[2]})
        need(matches == [selected['mount']] and matches[0]['filesystem_type'] == 'btrfs' and
             matches[0]['source'] == '/dev/' + selected['loop_name'] and matches[0]['root'] == '/', 'exact current owned Btrfs mount relation changed')
        tool = self.common.opened(Path(selected['fstrim']['path']))
        need(tool == selected['fstrim'], 'actual selected fstrim full7 changed')
        space = os.statvfs(image.parent)
        return {'mount_root_device': info.st_dev, 'mount_root_inode': info.st_ino,
            'mount_superblock_major_minor': matches[0]['major_minor'], 'image_device': before.st_dev,
            'image_inode': before.st_ino, 'logical_bytes': before.st_size, 'allocated_bytes': before.st_blocks * 512,
            'backing_available': space.f_bavail * space.f_frsize, 'actual_mount': matches[0],
            'loop_backing_file': str(image), 'uuid_device_target': target, 'uuid_device_resolved': resolved}

    def trim(self, reason):
        need(type(reason) is str and re.fullmatch(r'[a-z0-9-]{1,80}', reason), 'finite explicit trim barrier label required')
        before = self.verify(); label = 'owned-free-block-barrier-%05d-%s' % (self.sequence, reason); self.sequence += 1
        row = self.run_trim(label, [self.selection['fstrim']['path'], '--verbose', self.selection['mountpoint']])
        need(row['status'] == 'PASS' and row.get('returncode') == 0, 'owned free-block trim did not complete')
        after = self.verify()
        need(all(before[key] == after[key] for key in ('mount_root_device', 'mount_root_inode', 'mount_superblock_major_minor',
             'image_device', 'image_inode', 'logical_bytes', 'actual_mount', 'loop_backing_file', 'uuid_device_target', 'uuid_device_resolved')),
             'owned storage identity changed across trim')
        receipt = {'kind': 'owned-Btrfs-unused-block-trim-barrier', 'reason': reason, 'before_actual': before,
            'after_actual': after, 'actual_command': row, 'files_deleted': False, 'scope': 'UNUSED_FILESYSTEM_BLOCKS_ONLY_NO_INPUT_OR_REPORT_REMOVAL'}
        path = self.out / (label + '.json'); self.common.atomic_json(path, receipt)
        self.report['owned_storage']['trim_barriers'].append(self.product(path, 16 * MIB))
        return receipt
