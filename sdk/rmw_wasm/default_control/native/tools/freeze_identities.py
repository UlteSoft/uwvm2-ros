#!/usr/bin/env python3
"""Freeze separately acquired actual native ID DATA into a fresh input header.

No SDK module, native accessor, compiler or subprocess is loaded. The exact
actual export packet SHA must be explicitly supplied by the external owner.
This data association does not qualify its producer or native library closure.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import stat

def read(path):
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        first = os.fstat(fd)
        if not stat.S_ISREG(first.st_mode) or first.st_size > 1024 * 1024:
            raise ValueError('bounded ordinary DATA input required')
        parts, count = [], 0
        while True:
            part = os.read(fd, min(65536, 1024 * 1024 - count + 1))
            if not part:
                break
            count += len(part)
            if count > 1024 * 1024:
                raise ValueError('DATA grew beyond cap')
            parts.append(part)
        after, named = os.fstat(fd), os.lstat(path)
        def key(s):
            return s.st_dev, s.st_ino, s.st_size, s.st_mode, s.st_mtime_ns, s.st_ctime_ns, s.st_nlink
        if key(first) != key(after) or key(first) != key(named) or count != first.st_size:
            raise ValueError('DATA identity or extent drift')
        return b''.join(parts)
    finally:
        os.close(fd)

def strict(raw):
    def pairs(items):
        result = {}
        for key, value in items:
            if key in result:
                raise ValueError('duplicate DATA key')
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=lambda _: (_ for _ in ()).throw(ValueError('nonfinite DATA')))

def run(required, actual, actual_sha, output):
    if not all(p.is_absolute() for p in (required, actual, output)):
        raise ValueError('explicit absolute DATA paths required')
    required_raw, actual_raw = read(required), read(actual)
    if hashlib.sha256(actual_raw).hexdigest() != actual_sha:
        raise ValueError('actual external export packet SHA differs')
    names = strict(required_raw)['native_types']
    data = strict(actual_raw)
    if data.get('format_version') != 1 or data.get('status') != 'ACTUAL_NATIVE_EXPORT_REQUIRES_EXTERNAL_QUALIFICATION' or set(data) != {'format_version', 'status', 'native_types'}:
        raise ValueError('exact actual export schema required')
    if not isinstance(names, list) or not 1 <= len(names) <= 128 or names != sorted(set(names)):
        raise ValueError('exact complete selected sorted name list required')
    rows = data['native_types']
    if not isinstance(rows, list) or len(rows) != len(names):
        raise ValueError('actual export count differs')
    values = []
    for name, row in zip(names, rows):
        if not isinstance(name, str) or not re.fullmatch(r'[a-z][a-z0-9_]*/(?:msg|srv)/[A-Za-z][A-Za-z0-9_]*', name):
            raise ValueError('invalid selected native spelling')
        if not isinstance(row, dict) or set(row) != {'native_type', 'rihs01'} or row['native_type'] != name or not isinstance(row['rihs01'], str) or not re.fullmatch('[0-9a-f]{64}', row['rihs01']) or not int(row['rihs01'], 16):
            raise ValueError('actual name/hash association differs')
        values.append(bytes.fromhex(row['rihs01']))
    body = '// Actual independently acquired ID DATA; producer qualification remains separate.\n#pragma once\n#include <uwvm_rmw/default_control.h>\n#include <array>\nnamespace uwvm_default_control_input {\n'
    body += 'inline constexpr char export_packet_sha256[]="' + actual_sha + '";\n'
    body += 'inline constexpr std::array<uwvm_default_control_identity,' + str(len(names)) + '> identities{{\n'
    body += ''.join(' {' + json.dumps(name) + ',{' + ','.join(str(b) for b in value) + '}},\n' for name, value in zip(names, values))
    body += '}};\n}\n'
    if read(required) != required_raw or read(actual) != actual_raw:
        raise ValueError('terminal DATA drift')
    if output.exists():
        raise ValueError('fresh identity input directory required')
    output.mkdir(parents=True)
    path = output / 'uwvm_default_control_identity_input.hpp'
    path.write_text(body, encoding='utf-8')
    (output / 'identity-input.json').write_text(json.dumps(dict(status='FROZEN_ACTUAL_EXPORT_DATA_NOT_QUALIFICATION',
        required=dict(path=str(required), bytes=len(required_raw), sha256=hashlib.sha256(required_raw).hexdigest()),
        actual=dict(path=str(actual), bytes=len(actual_raw), sha256=actual_sha),
        output=dict(bytes=len(body.encode()), sha256=hashlib.sha256(body.encode()).hexdigest())), indent=2) + '\n')

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('required', 'actual', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    parser.add_argument('--actual-sha256', required=True)
    args = parser.parse_args()
    try:
        run(args.required, args.actual, args.actual_sha256, args.out)
    except (OSError, ValueError, KeyError, TypeError) as error:
        parser.error(str(error))

if __name__ == '__main__':
    main()
