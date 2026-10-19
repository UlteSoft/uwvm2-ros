#!/usr/bin/env python3
"""Build the exact non-ROS frozen table using admitted native CPython3.14.8.

Compile source into marshal code DATA; never import/run the application.
Run ONLY through the admitted SSH Linux keeper, including interpreter/stdlib
admission before this entry. This tool does not claim process/resource ownership.
"""
import argparse
import hashlib
import json
import marshal
import os
from pathlib import Path
import stat
import sys


def capture(path):
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_NONBLOCK | os.O_CLOEXEC)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or not 0 <= before.st_size <= 1 << 20:
            raise ValueError("finite ordinary source required")
        pieces = []; size = 0
        while size <= before.st_size:
            part = os.read(fd, min(65536, before.st_size + 1 - size))
            if not part:
                break
            pieces.append(part); size += len(part)
        raw = b''.join(pieces)
        def key(row):
            return {name: str(getattr(row, 'st_' + name)) for name in
                    ('dev', 'ino', 'mode', 'nlink', 'size', 'mtime_ns', 'ctime_ns')}
        if len(raw) != before.st_size or key(before) != key(os.fstat(fd)) or key(before) != key(os.lstat(path)):
            raise ValueError("source changed during capture")
        return raw, dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest(), stat=key(before))
    finally:
        os.close(fd)


def main():
    if sys.version_info[:3] != (3, 14, 8):
        raise ValueError("use the actual matching native CPython3.14.8")
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--entry', type=Path, required=True)
    parser.add_argument('--stdlib', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if not all(path.is_absolute() for path in (args.entry, args.stdlib, args.out)):
        raise ValueError("explicit absolute input/output roles required")
    entry = args.entry.resolve(strict=True); stdlib = args.stdlib.resolve(strict=True)
    parent = args.out.parent.resolve(strict=True); output = parent / args.out.name
    if args.out.name in ('', '.', '..') or output.exists() or output.is_symlink() or output.is_relative_to(stdlib):
        raise ValueError("fresh output outside stdlib required")
    roles = [('encodings', stdlib / 'encodings/__init__.py', 1),
             ('encodings.aliases', stdlib / 'encodings/aliases.py', 0),
             ('encodings.utf_8', stdlib / 'encodings/utf_8.py', 0),
             ('debug_app', entry, 0)]
    header = ['/* Native CPython3.14.8 marshal DATA, not executed application. */']
    entries = []; observations = []; total = 0
    for index, (name, path, package) in enumerate(roles):
        raw, pin = capture(path)
        virtual = '/__uwvm_debug__/' + name.replace('.', '/') + ('/__init__.py' if package else '.py')
        code = marshal.dumps(compile(raw, virtual, 'exec', dont_inherit=True, optimize=0))
        total += len(code)
        if total > 4 << 20:
            raise ValueError("bounded total marshal budget exceeded")
        symbol = 'uwvm_debug_code_' + str(index)
        header.append('static const unsigned char ' + symbol + '[] = {')
        for offset in range(0, len(code), 24):
            header.append(','.join(str(value) for value in code[offset:offset + 24]) + ',')
        header.append('};')
        entries.append('{"' + name + '",' + symbol + ',' + str(len(code)) + ',' + str(package) + '},')
        observations.append(dict(source=pin, module=name, package=bool(package),
                                 virtual_filename=virtual, marshal_bytes=len(code),
                                 marshal_sha256=hashlib.sha256(code).hexdigest()))
    header.extend(['static const struct _frozen debug_frozen[] = {', *entries, '{NULL,NULL,0,0}', '};', ''])
    for row in observations:
        if capture(Path(row['source']['path']))[1] != row['source']:
            raise ValueError("original frozen source changed before publication")
    output.mkdir(exist_ok=False)
    data = '\n'.join(header).encode('ascii')
    with (output / 'debug_frozen.h').open('xb') as stream:
        stream.write(data)
    receipt = dict(status='FROZEN_MARSHAL_DATA_CREATED_NOT_GUEST_EXECUTED',
                   interpreter=sys.executable, version=sys.version,
                   sources=observations, header_bytes=len(data),
                   header_sha256=hashlib.sha256(data).hexdigest(),
                   dynamic_assets=[], ROS_imports_added=False,
                   compiler_or_Guest_invocations=0,
                   limits=['Interpreter/stdlib/tools admission and target bytecode/ABI match remain external keeper responsibilities.',
                           'The target must contain the actual static gc/sys/_codecs and frozen import-bootstrap closure.',
                           'This receipt is generation only, no debugger, Native, WASI or Guest acceptance.'])
    with (output / 'receipt.json').open('x') as stream:
        json.dump(receipt, stream, indent=2); stream.write('\n')


if __name__ == '__main__':
    main()
