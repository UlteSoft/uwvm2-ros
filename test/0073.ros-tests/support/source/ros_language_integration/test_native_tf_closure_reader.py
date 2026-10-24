#!/usr/bin/env python3
"""Malformed ELF and path-containment controls; never invoke an ELF loader.

Run on SSH Linux in the selected test cgroup. --root names a fresh owned
fixture directory. Native delivery and loader qualification remain separate.
"""
import argparse
import json
from pathlib import Path
import struct
import audit_native_tf_closure as reader


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    root = parser.parse_args().root.absolute()
    root.mkdir(mode=0o700)
    base = bytearray(272)
    base[:16] = b'\x7fELF\x02\x01\x01' + b'\0' * 9
    struct.pack_into('<HHIQQQIHHHHHH', base, 16, 3, 62, 1, 0, 64, 0, 0,
                     64, 56, 2, 0, 0, 0)
    struct.pack_into('<IIQQQQQQ', base, 64, 1, 4, 0, 0x400000, 0,
                     len(base), len(base), 4096)
    struct.pack_into('<IIQQQQQQ', base, 120, 2, 4, 192, 0x4000c0, 0, 64, 64, 8)
    for index, (tag, value) in enumerate(((5, 0x400100), (10, 16), (1, 1), (0, 0))):
        struct.pack_into('<qQ', base, 192 + 16 * index, tag, value)
    base[256:] = b'\0libfixture.so\0\0'
    assert len(base) == 272
    results = []

    def check_image(name, body, accepted=False):
        path = root / (name + '.elf')
        path.write_bytes(body)
        try:
            image = reader.elf(path)
            assert accepted and image['needed'] == ['libfixture.so'], name
            outcome = 'accepted-known-valid'
        except (ValueError, struct.error) as error:
            assert not accepted, (name, str(error))
            outcome = 'refused: ' + str(error)
        results.append(dict(name=name, outcome=outcome, passed=True))

    check_image('valid', base, accepted=True)
    check_image('truncated', base[:63])
    for name, fmt, offset, value in (
        ('elf32-refused', 'B', 4, 1),
        ('wrong-machine', 'H', 18, 183),
        ('bad-ph-size', 'H', 54, 65535),
        ('forged-string-address', 'Q', 200, 0xffffffffffffffff),
        ('bad-needed-index', 'Q', 232, 500),
        ('missing-dynamic-terminator', 'q', 240, 1),
        ('needed-slash', 'B', 257, ord('/')),
    ):
        body = bytearray(base)
        struct.pack_into('<' + fmt, body, offset, value)
        check_image(name, body)
    body = bytearray(base)
    body[269:] = b'XXX'
    check_image('unterminated-needed', body)

    links = root / 'links'
    links.mkdir()
    (links / 'regular').write_bytes(b'input')
    targets = dict(relative='regular', absolute='/etc/passwd',
                   escape='../../outside', cycle='cycle', dangling='absent')
    for name, target in targets.items():
        path = links / name
        path.symlink_to(target)
        try:
            resolved = reader.contained(links, path)
            assert name == 'relative' and resolved == links / 'regular', name
            outcome = 'accepted-contained-relative'
        except (OSError, ValueError) as error:
            assert name != 'relative', str(error)
            outcome = 'refused: ' + str(error)
        results.append(dict(name='symlink-' + name, outcome=outcome, passed=True))
    assert len(results) == 15
    print(json.dumps(dict(passed=len(results), expected=15, tests=results,
                          payload_executions=0), indent=2))


if __name__ == '__main__':
    main()
