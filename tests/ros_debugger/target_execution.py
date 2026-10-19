"""Explicit cross-built LLVM-full debugger launch selection, never an emulator verdict.

The native v1 route does not call this adapter. A v2 receipt binds host QEMU,
target loader, independent target ELF and whole inventoried sysroot separately.
No executable or subprocess is started here. Actual stopped map_files, console
credentials, LLVM-full admission, Guest effects and cleanup remain mandatory.
"""
from pathlib import Path
import os
import platform
import re
import stat
import struct

ARCHES = {'x86_64': (62, 64, 'little'), 'aarch64': (183, 64, 'little'),
          'arm': (40, 32, 'little'), 'i386': (3, 32, 'little'),
          'riscv64': (243, 64, 'little'), 'loongarch64': (258, 64, 'little'),
          'ppc64le': (21, 64, 'little'), 's390x': (22, 64, 'big')}
LLVM_ARCHES = {'x86_64': 'X86', 'i386': 'X86', 'aarch64': 'AArch64', 'arm': 'ARM',
               'riscv64': 'RISCV', 'loongarch64': 'LoongArch', 'ppc64le': 'PowerPC', 's390x': 'SystemZ'}
HOST_ALIASES = {'amd64': 'x86_64', 'arm64': 'aarch64', 'ppc64el': 'ppc64le', 'i686': 'i386'}
PIN = {'path', 'bytes', 'sha256', 'device', 'inode', 'mtime_ns', 'ctime_ns'}
CONFIG_KEYS = {'architecture', 'mode', 'qemu', 'sysroot', 'loader', 'environment'}
RECEIPT_KEYS = CONFIG_KEYS | {'host_elf', 'sysroot_identity', 'argument_prefix'}


def need(value, reason):
    if not value:
        raise ValueError(reason)


def full_pin(value):
    need(type(value) is dict and set(value) == PIN and type(value['path']) is str and
         Path(value['path']).is_absolute() and type(value['sha256']) is str and
         re.fullmatch('[0-9a-f]{64}', value['sha256']) and
         all(type(value[k]) is int and value[k] >= 0 for k in PIN - {'path', 'sha256'}) and
         value['inode'] > 0 and value['bytes'] > 0, 'closed actual physical full7 required')
    return value


def host_target(machine=None):
    machine = platform.machine().lower() if machine is None else machine
    canonical = HOST_ALIASES.get(machine, machine)
    need(canonical in ARCHES, 'unsupported actual compiler/QEMU host ELF')
    return dict(zip(('machine', 'bits', 'endian'), ARCHES[canonical]))


def validate_selection(target, value, actual_host=None):
    """Pure schema/architecture policy; never turns a declaration into evidence."""
    need(type(value) is dict and set(value) == CONFIG_KEYS and value['mode'] == 'qemu',
         'v2 requires an explicit user-QEMU selection')
    architecture = value['architecture']
    need(architecture in ARCHES and tuple(target[k] for k in ('machine', 'bits', 'endian')) == ARCHES[architecture],
         'selected target ELF/architecture differs')
    actual_host = platform.machine().lower() if actual_host is None else actual_host
    host = host_target(actual_host)
    need(target['host_machine'] == actual_host and any(target[k] != host[k] for k in host),
         'v2 is a foreign-target route; unchanged native route is v1')
    prefix = target['triple'].split('-', 1)[0]
    matches = {'x86_64': prefix == 'x86_64', 'aarch64': prefix == 'aarch64',
        'arm': prefix.startswith('arm') and not prefix.startswith('armeb'),
        'i386': re.fullmatch('i[3-6]86', prefix) is not None, 'riscv64': prefix == 'riscv64',
        'loongarch64': prefix == 'loongarch64', 'ppc64le': prefix in ('powerpc64le', 'ppc64le'),
        's390x': prefix == 's390x'}
    need(matches[architecture], 'compiler triple and selected architecture differ')
    full_pin(value['qemu']); full_pin(value['loader'])
    need(type(value['sysroot']) is str and Path(value['sysroot']).is_absolute() and
         not any(c in value['sysroot'] for c in ('\0', '\n', '\r')), 'explicit target sysroot required')
    environment = value['environment']
    need(type(environment) is dict and set(environment) <= {'LD_LIBRARY_PATH'},
         'only explicit guest library search paths are admitted')
    for item in environment.values():
        need(type(item) is str and 0 < len(item) <= 16384 and not any(c in item for c in ('\0', '\n', '\r', ',')) and
             len(item.split(':')) <= 64 and all(component and Path(component).is_absolute() for component in item.split(':')),
             'guest loader search paths must be nonempty absolute directories')
    return host


def elf_image(filename, opened):
    """Inspect stable complete ELF identity and bounded program-header PT_INTERP."""
    filename = Path(filename)
    need(filename.resolve(strict=True) == filename, 'canonical physical ELF spelling required')
    pin = opened(filename); full_pin(pin)
    fd = os.open(filename, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        first = os.fstat(fd); header = os.read(fd, 64)
        need(stat.S_ISREG(first.st_mode) and len(header) >= 52 and header[:4] == b'\x7fELF' and
             header[4] in (1, 2) and header[5] in (1, 2), 'ordinary complete ELF header required')
        # Decode the very inode whose complete bytes were admitted. A stable
        # second descriptor alone is insufficient if the named entry changed
        # between opened() and open(); a final pathname recheck cannot make
        # that different descriptor authoritative retroactively.
        need({'device': first.st_dev, 'inode': first.st_ino, 'bytes': first.st_size,
              'mtime_ns': first.st_mtime_ns, 'ctime_ns': first.st_ctime_ns} ==
             {key: pin[key] for key in ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns')},
             'decoded ELF descriptor differs from admitted complete input')
        bits, endian = (32 if header[4] == 1 else 64), ('little' if header[5] == 1 else 'big')
        order = '<' if endian == 'little' else '>'
        need(len(header) >= (52 if bits == 32 else 64), 'complete selected ELF header required')
        offset = struct.unpack_from(order + ('I' if bits == 32 else 'Q'), header, 28 if bits == 32 else 32)[0]
        size, count = struct.unpack_from(order + 'HH', header, 42 if bits == 32 else 54)
        need(count <= 512 and (count == 0 or size >= (32 if bits == 32 else 56)) and
             offset + size * count <= pin['bytes'], 'bounded complete program-header table required')
        interpreter = None
        for index in range(count):
            os.lseek(fd, offset + index * size, os.SEEK_SET); row = os.read(fd, size)
            need(len(row) == size, 'complete program header required')
            if struct.unpack_from(order + 'I', row)[0] != 3:
                continue
            need(interpreter is None, 'one target ELF interpreter required')
            start = struct.unpack_from(order + ('I' if bits == 32 else 'Q'), row, 4 if bits == 32 else 8)[0]
            length = struct.unpack_from(order + ('I' if bits == 32 else 'Q'), row, 16 if bits == 32 else 32)[0]
            need(1 < length <= 4096 and start + length <= pin['bytes'], 'bounded PT_INTERP required')
            os.lseek(fd, start, os.SEEK_SET); text = os.read(fd, length)
            need(len(text) == length and text.endswith(b'\0') and b'\0' not in text[:-1], 'complete PT_INTERP string required')
            interpreter = text[:-1].decode('ascii', 'strict')
            need(interpreter.startswith('/'), 'absolute target interpreter required')
        last = os.fstat(fd)
        named = filename.lstat()
        need(all(getattr(first, k) == getattr(last, k) == getattr(named, k) for k in
             ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')), 'ELF descriptor changed')
    finally:
        os.close(fd)
    need(opened(filename) == pin, 'complete ELF bytes changed while decoding')
    return {'input': pin, 'machine': int.from_bytes(header[18:20], endian), 'bits': bits,
        'endian': endian, 'type': int.from_bytes(header[16:18], endian), 'interpreter': interpreter}


def rooted(root, logical):
    """Resolve guest absolute symlinks inside its sysroot, never host /lib."""
    need(type(logical) is str and logical.startswith('/'), 'absolute guest path required')
    pending, parts, links = logical.split('/'), [], 0
    while pending:
        item = pending.pop(0)
        if item in ('', '.'):
            continue
        if item == '..':
            need(parts, 'guest alias escapes selected sysroot'); parts.pop(); continue
        path = root.joinpath(*parts, item); info = path.lstat()
        if stat.S_ISLNK(info.st_mode):
            links += 1; need(links <= 40, 'guest alias depth exhausted')
            target = os.readlink(path)
            need(target and len(target.encode()) <= 4096 and '\0' not in target, 'bounded guest alias required')
            last = path.lstat()
            need(all(getattr(info, k) == getattr(last, k) for k in
                 ('st_dev', 'st_ino', 'st_mode', 'st_size', 'st_mtime_ns', 'st_ctime_ns')), 'guest alias changed')
            if target.startswith('/'):
                parts = []
            pending = target.split('/') + pending
        else:
            need(stat.S_ISREG(info.st_mode) or stat.S_ISDIR(info.st_mode), 'guest alias crosses special node')
            parts.append(item)
    result = root.joinpath(*parts)
    need(result.resolve(strict=True) == result, 'guest physical spelling changed')
    return result


def directory_identity(root):
    info = root.stat(follow_symlinks=False)
    need(root.resolve(strict=True) == root and stat.S_ISDIR(info.st_mode), 'physical sysroot directory required')
    return {'path': str(root), 'device': info.st_dev, 'inode': info.st_ino, 'mode': info.st_mode}


def launch_prefix(selection):
    """Guest env travels through QEMU -E, never its host dynamic loader."""
    result = [selection['qemu']['path'], '-L', selection['sysroot'], '-U', 'LD_PRELOAD', '-U', 'LD_LIBRARY_PATH']
    for key, value in sorted(selection['environment'].items()):
        result += ['-E', key + '=' + value]
    return result


def qualify(config, roots, opened):
    value = config['execution']; host = validate_selection(config['target'], value)
    root = Path(value['sysroot'])
    need(config['sysroot'] == value['sysroot'] and root.resolve(strict=True) == root and root.is_dir() and
         any(root == base or root.is_relative_to(base) for base in roots), 'same fully inventoried compile/runtime sysroot required')
    for role in ('qemu', 'loader'):
        pin = value[role]; filename = Path(pin['path'])
        need(filename.resolve(strict=True) == filename and opened(filename) == pin and os.access(filename, os.X_OK) and
             any(filename.is_relative_to(base) for base in roots), 'actual selected executable must belong to complete roots')
    qemu = elf_image(value['qemu']['path'], opened); loader = elf_image(value['loader']['path'], opened)
    need(all(qemu[k] == host[k] for k in host) and qemu['type'] in (2, 3), 'QEMU must be an actual host executable')
    need(tuple(loader[k] for k in ('machine', 'bits', 'endian')) == ARCHES[value['architecture']] and
         loader['type'] in (2, 3) and Path(value['loader']['path']).is_relative_to(root), 'target loader architecture/root differs')
    for item in value['environment'].values():
        for component in item.split(':'):
            directory = Path(component)
            need(directory.resolve(strict=True) == directory and directory.is_dir() and
                 any(directory == base or directory.is_relative_to(base) for base in roots),
                 'guest loader directories require complete physical inventories')
    return {**value, 'host_elf': host, 'sysroot_identity': directory_identity(root), 'argument_prefix': launch_prefix(value)}


def check_image(filename, target, execution, opened):
    image = elf_image(filename, opened)
    need(all(image[k] == target[k] for k in ('machine', 'bits', 'endian')) and image['type'] in (2, 3),
         'foreign Runtime image architecture/type differs')
    need(directory_identity(Path(execution['sysroot'])) == execution['sysroot_identity'], 'selected sysroot identity changed')
    if image['interpreter'] is not None:
        need(str(rooted(Path(execution['sysroot']), image['interpreter'])) == execution['loader']['path'] and
             opened(Path(execution['loader']['path'])) == execution['loader'], 'foreign PT_INTERP differs from selected physical loader')
    return image
