#!/usr/bin/env python3
"""Write an additional closed native-event entry manifest without executing JS.

Run as a guarded source-preparation child. The ordinary guest/prepare.py then
consumes this expanded document, and the owner separately executes every real
compile/link/validation job with full dependencies and trace. A manifest is
neither a compiled artifact nor a Runtime/native result. Source34's complete
manifest must include this script and native_event.mjs before compilation.
"""
import argparse
import json
import os
from pathlib import Path
import stat


def selection(document, backend):
    if document.get('schema') != 'uwvm-javascript-guest-recipes-v1' or backend not in (1, 2):
        raise ValueError('exact current recipes and explicit backend required')
    manifest = {**document['common'], **document['variants']['08-unified-events-ros%d' % backend]}
    manifest['modules'] = [*manifest['modules'], {
        'name': 'tests/ros_javascript_guest/native_event.mjs',
        'source': 'tests/ros_javascript_guest/native_event.mjs'}]
    manifest['entry'] = 'tests/ros_javascript_guest/native_event.mjs'
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--recipes', type=Path, required=True)
    parser.add_argument('--backend', type=int, choices=(1, 2), required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if not args.recipes.is_absolute() or args.recipes.resolve(strict=True) != args.recipes:
        raise ValueError('explicit physical recipe source required')
    descriptor = os.open(args.recipes, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode) or not 0 < before.st_size <= 1024**2:
            raise ValueError('bounded ordinary recipe source required')
        raw = bytearray()
        while True:
            block = os.read(descriptor, min(65536, 1024**2-len(raw)+1))
            if not block: break
            raw.extend(block)
            if len(raw) > 1024**2: raise ValueError('recipe grew beyond bound')
        after, named = os.fstat(descriptor), args.recipes.stat(follow_symlinks=False)
        if len(raw) != before.st_size or any(getattr(before, key) != getattr(after, key) or
            getattr(before, key) != getattr(named, key) for key in
            ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')):
            raise ValueError('ordinary recipe identity changed')
    finally:
        os.close(descriptor)
    def pairs(items):
        result = {}
        for key, value in items:
            if key in result: raise ValueError('duplicate recipe key')
            result[key] = value
        return result
    def constant(_): raise ValueError('nonfinite recipe metadata')
    document = json.loads(raw, object_pairs_hook=pairs, parse_constant=constant)
    if not args.out.is_absolute() or args.out.parent.resolve(strict=True) != args.out.parent or args.out == args.recipes:
        raise ValueError('fresh explicit owned output without symlink parents required')
    descriptor = os.open(args.out, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_CLOEXEC | os.O_NOFOLLOW, 0o600)
    with os.fdopen(descriptor, 'w', encoding='utf-8') as stream:
        json.dump(selection(document, args.backend), stream, indent=2, sort_keys=True, allow_nan=False)
        stream.write('\n'); stream.flush(); os.fsync(stream.fileno())


if __name__ == '__main__':
    raise SystemExit(main())
