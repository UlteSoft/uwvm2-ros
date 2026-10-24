#!/usr/bin/env python3
"""Inspect recovered TF witness dependencies as data, without invoking a loader.

This is an explicit-library-path DT_NEEDED inventory, not dynamic-linker
emulation or permission to execute a recovered rootfs. dlopen, symbol versions,
Python imports, plugin discovery and actual mapped-file checks remain separate.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import struct


def elf(path, tick=lambda: None):
    """Bounded ELF64 little-endian x86-64 program-header reader."""
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or not 64 <= before.st_size <= 512 << 20:
            raise ValueError('ordinary ELF input of 64 bytes..512 MiB required')
        def read(offset, count):
            tick()
            if offset < 0 or count < 0 or offset + count > before.st_size:
                raise ValueError('ELF extent outside file')
            data = os.pread(fd, count, offset)
            if len(data) != count:
                raise ValueError('short ELF read')
            return data
        def unpack(fmt, offset):
            return struct.unpack('<' + fmt, read(offset, struct.calcsize('<' + fmt)))
        ident = read(0, 16)
        if ident[:7] != b'\x7fELF\x02\x01\x01':
            raise ValueError('only declared ELF64 little-endian version1 supported')
        kind, machine, version, _, phoff, _, _, ehsize, phsize, phnum, *_ = unpack('HHIQQQIHHHHHH', 16)
        if kind not in (2, 3) or machine != 62 or version != 1 or ehsize != 64:
            raise ValueError('expected x86-64 executable/shared image')
        if phsize != 56 or not 1 <= phnum <= 4096:
            raise ValueError('unsupported program-header table')
        loads, dynamic, interpreter = [], [], []
        for index in range(phnum):
            tag, _, offset, address, _, size, memory, _ = unpack('IIQQQQQQ', phoff + index * phsize)
            if offset + size > before.st_size or size > memory:
                raise ValueError('invalid segment extent')
            if tag == 1:
                loads.append((address, size, offset))
            elif tag == 2:
                if size % 16 or size > 1 << 20:
                    raise ValueError('unsupported dynamic table extent')
                dynamic.append((offset, size))
            elif tag == 3:
                if not 1 <= size <= 4096:
                    raise ValueError('invalid interpreter extent')
                raw = read(offset, size)
                if raw[-1:] != b'\0' or b'\0' in raw[:-1]:
                    raise ValueError('invalid interpreter string')
                interpreter.append(raw[:-1].decode('ascii'))
        if len(dynamic) > 1 or len(interpreter) > 1 or not loads:
            raise ValueError('ambiguous image program headers')
        tags = {}
        for offset, size in dynamic:
            ended = False
            for index in range(size // 16):
                tag, value = unpack('qQ', offset + index * 16)
                if tag == 0:
                    ended = True
                    break
                tags.setdefault(tag, []).append(value)
            if not ended:
                raise ValueError('unterminated dynamic table')
        strings = b''
        if any(tag in tags for tag in (1, 14, 15, 29)):
            if len(tags.get(5, [])) != 1 or len(tags.get(10, [])) != 1:
                raise ValueError('unique bounded dynamic string table required')
            address, size = tags[5][0], tags[10][0]
            matches = [offset + address - start for start, extent, offset in loads
                       if start <= address and address - start <= extent and size <= extent - (address - start)]
            if len(matches) != 1 or not 1 <= size <= 16 << 20:
                raise ValueError('ambiguous or unbounded dynamic strings')
            strings = read(matches[0], size)
        def string(index):
            end = strings.find(b'\0', index)
            if not 0 <= index < len(strings) or end < 0:
                raise ValueError('invalid dynamic string index')
            return strings[index:end].decode('ascii')
        needed = [string(index) for index in tags.get(1, [])]
        if any(not name or '/' in name or name in ('.', '..') for name in needed):
            raise ValueError('DT_NEEDED requires a nonempty library basename')
        h = hashlib.sha256()
        for offset in range(0, before.st_size, 65536):
            h.update(read(offset, min(65536, before.st_size - offset)))
        after = os.fstat(fd)
        identity = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns)
        if identity(before) != identity(after):
            raise ValueError('ELF changed during inspection')
        return dict(path=str(path), bytes=before.st_size, sha256=h.hexdigest(),
                    needed=needed, interpreter=interpreter,
                    rpath=[string(x) for x in tags.get(15, [])],
                    runpath=[string(x) for x in tags.get(29, [])])
    finally:
        os.close(fd)


def contained(root, path):
    """Refuse external, dangling or absolute symlink paths before opening data."""
    path = Path(os.path.abspath(path))
    if not path.is_relative_to(root):
        raise ValueError('path outside selected root')
    # Inspect links before resolve so an absolute link is never read through.
    pending = list(path.relative_to(root).parts)
    current, followed = root, 0
    while pending:
        part = pending.pop(0)
        if part == '..':
            if current == root:
                raise ValueError('symlink escapes selected root')
            current = current.parent
            continue
        current = current / part
        info = current.lstat()
        if stat.S_ISLNK(info.st_mode):
            followed += 1
            target = os.readlink(current)
            if followed > 40 or os.path.isabs(target):
                raise ValueError('absolute or cyclic symlink refused')
            pending = list(Path(target).parts) + pending
            current = current.parent
    return current


def audit(inputs, root, original_prefix, tick=lambda: None):
    root = root.resolve(strict=True)
    def rebase(value):
        return root / Path(value).relative_to(original_prefix)
    report = dict(kind='static-native-tf-needed-inventory-v1', backends={},
                  payload_executions=0, execution_ready=False, native_delivery_credit=False)
    for backend, native in inputs['native_backends'].items():
        dirs = [contained(root, rebase(p)) for p in native['native_library_path'].split(':')]
        seeds = [native[x] for x in ('native_gateway', 'native_loader', 'peer')]
        if 'master' in native:
            seeds += [p for p in native['master'].values() if isinstance(p, dict) and 'path' in p]
        result = dict(principal_pins=[], images=[], missing=[], unhandled_search_paths=[],
                      library_path=list(map(str, dirs)), static_needed_complete=False)
        queue, seen = [], set()
        for pin in seeds:
            try:
                path = contained(root, rebase(pin['path']))
                # The rosmaster script is a pinned input, not an ELF.
                if path.name == 'rosmaster':
                    data = path.read_bytes()
                    observed = dict(bytes=len(data), sha256=hashlib.sha256(data).hexdigest())
                else:
                    observed = elf(path, tick)
                    queue.append(path)
                if (observed['bytes'], observed['sha256']) != (pin['bytes'], pin['sha256']):
                    raise ValueError('principal input hash/size mismatch')
                result['principal_pins'].append(dict(path=str(path), verified=True, **{k: observed[k] for k in ('bytes', 'sha256')}))
            except (OSError, ValueError, struct.error) as error:
                result['principal_pins'].append(dict(path=str(rebase(pin['path'])), verified=False, error=str(error)))
        while queue:
            tick()
            path = queue.pop(0)
            if path in seen:
                continue
            seen.add(path)
            if len(seen) > 4096:
                raise ValueError('image count bound')
            try:
                image = elf(path, tick)
                result['images'].append(image)
                if image['rpath'] or image['runpath']:
                    result['unhandled_search_paths'].append(dict(path=str(path), rpath=image['rpath'], runpath=image['runpath']))
                for name in image['needed']:
                    failures = []
                    for directory in dirs:
                        try:
                            candidate = contained(root, directory / name)
                            if not candidate.is_file():
                                raise ValueError('not an ordinary library')
                            queue.append(candidate)
                            break
                        except (OSError, ValueError) as error:
                            failures.append(str(error))
                    else:
                        result['missing'].append(dict(image=str(path), needed=name, errors=failures))
            except (OSError, ValueError, struct.error) as error:
                result['missing'].append(dict(image=str(path), error=str(error)))
        result['explicit_path_needed_complete'] = (all(x['verified'] for x in result['principal_pins']) and
                                                   not result['missing'])
        result['static_needed_complete'] = (result['explicit_path_needed_complete'] and
                                             not result['unhandled_search_paths'])
        report['backends'][backend] = result
    return report


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inputs', type=Path, required=True)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--original-prefix', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    value = audit(json.loads(args.inputs.read_bytes()), args.root, args.original_prefix)
    with args.out.open('x') as stream:
        json.dump(value, stream, indent=2)
        stream.write('\n')
    return 0 if all(x['static_needed_complete'] for x in value['backends'].values()) else 1


if __name__ == '__main__':
    raise SystemExit(main())
