#!/usr/bin/env python3
"""Merge explicit compatible complete source catalogs, never generated headers.

Shared types must have identical original bytes and identical metadata (including
bounds, constants/default interpretation and registration choice). A conflict
requires a new reviewed input catalog; this tool never widens a bound or endpoint.
The result is source preparation only: independently export/bind Native metadata
and regenerate one coherent SDK/Runtime/Gateway domain before deployment.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat


PROFILE = 'uwvm-ros-vendor-complete-bounded-v1'
# Match the complete-source preparer. The legacy bounded-v2 pipeline retains
# its independent ceiling; source size and downstream node/wire limits remain.
MAX_TYPES = 512


def strict_json(raw):
    def pairs(items):
        result = {}
        for key, value in items:
            if key in result:
                raise ValueError('duplicate JSON key: ' + key)
            result[key] = value
        return result
    return json.loads(raw, object_pairs_hook=pairs,
                      parse_constant=lambda _: (_ for _ in ()).throw(ValueError('nonfinite JSON')))


def read(path, limit):
    fd = os.open(path, os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW | os.O_NONBLOCK)
    try:
        a = os.fstat(fd)
        if not stat.S_ISREG(a.st_mode) or not 0 <= a.st_size <= limit:
            raise ValueError('bounded ordinary source required')
        chunks, total = [], 0
        while True:
            block = os.read(fd, min(65536, limit + 1 - total))
            if not block:
                break
            total += len(block)
            if total > a.st_size or total > limit:
                raise ValueError('source grew beyond admission')
            chunks.append(block)
        b = os.fstat(fd)
        c = os.stat(path, follow_symlinks=False)
        key = lambda s: (s.st_dev, s.st_ino, s.st_mode, s.st_size, s.st_mtime_ns, s.st_ctime_ns)
        if total != a.st_size or key(a) != key(b) or key(b) != key(c):
            raise ValueError('source changed during descriptor read')
        raw = b''.join(chunks)
        return raw, dict(path=str(path), bytes=total, sha256=hashlib.sha256(raw).hexdigest(),
                         device=a.st_dev, inode=a.st_ino, mtime_ns=a.st_mtime_ns, ctime_ns=a.st_ctime_ns)
    finally:
        os.close(fd)


def merge(catalogs, out):
    """Validate the whole source union before creating a fresh output directory."""
    if not 2 <= len(catalogs) <= 8:
        raise ValueError('select two to eight explicit catalogs')
    paths = [Path(p).absolute() for p in catalogs]
    out = Path(out).absolute()
    if out.exists() or out.is_symlink() or out.parent.resolve(strict=True) != out.parent:
        raise ValueError('fresh physical output directory required')
    if len(set(paths)) != len(paths):
        raise ValueError('repeated input catalog')
    records, captured, sources, backend, total = {}, {}, [], None, 0
    native_kinds = {}
    for path in paths:
        if path.resolve(strict=True) != path or out == path.parent or path.parent in out.parents:
            raise ValueError('physical catalogs and output outside every input tree required')
        raw, pin = read(path, 2 * 1024**2)
        captured[path] = raw
        total += len(raw)
        document = strict_json(raw)
        if (type(document) is not dict or set(document) != {'format', 'types'} or
                document['format'] != PROFILE or type(document['types']) is not list or
                not 1 <= len(document['types']) <= MAX_TYPES):
            raise ValueError('complete source catalog required')
        definitions, seen = [], set()
        for row in document['types']:
            if type(row) is not dict or type(row.get('file')) is not str or type(row.get('type')) is not str:
                raise ValueError('explicit definition and type required')
            relative = row['file']
            if ('\\' in relative or '\0' in relative or Path(relative).is_absolute() or
                    any(part in ('', '.', '..') for part in relative.split('/'))):
                raise ValueError('closed relative definition required')
            major = row.get('ros_version')
            if type(major) is not int or major not in (1, 2) or (backend is not None and backend != major):
                raise ValueError('merge exactly one ROS backend per domain')
            backend = major
            suffix = Path(relative).suffix
            if suffix not in ('.msg', '.srv', '.action'):
                raise ValueError('original Message/Service/Action definition required')
            native = (major, row['type'])
            kinds = native_kinds.setdefault(native, set())
            combined = kinds | {suffix}
            # ROS1 permits an original Message and Action to share a basename.
            # Preserve both definitions without changing either native name.
            # Every other cross-category collision remains an explicit refusal.
            if len(combined) > 1 and (major != 1 or combined != {'.msg', '.action'}):
                raise ValueError('native interface category collision: ' + row['type'])
            kinds.add(suffix)
            key = (major, row['type'], suffix)
            if key in seen:
                raise ValueError('duplicate type inside input catalog')
            seen.add(key)
            definition = path.parent / relative
            if definition.resolve(strict=True) != definition or path.parent not in definition.parents:
                raise ValueError('definition escaped physical catalog root')
            body, definition_pin = read(definition, 1024**2)
            captured[definition] = body
            definitions.append(definition_pin)
            total += len(body)
            if total > 64 * 1024**2:
                raise ValueError('whole source union exceeds 64 MiB')
            metadata = {k: v for k, v in row.items() if k != 'file'}
            if 'registered' in metadata and type(metadata['registered']) is not bool:
                raise ValueError('registration must be an explicit Boolean')
            if key in records:
                old, original, _ = records[key]
                if (json.dumps(old, sort_keys=True) != json.dumps(metadata, sort_keys=True)
                        or original != body):
                    raise ValueError('conflicting complete type/bounds/registration: ' + row['type'])
            else:
                target = 'definitions/type-%03d%s' % (len(records), suffix)
                records[key] = (metadata, body, target)
                if len(records) > MAX_TYPES:
                    raise ValueError('merged domain exceeds 512 complete roots')
        sources.append(dict(catalog=pin, definitions=definitions))
    for path, raw in captured.items():
        if read(path, max(len(raw), 1))[0] != raw:
            raise ValueError('source changed before output')
    out.mkdir(mode=0o700)
    (out / 'definitions').mkdir(mode=0o700)
    rows = []
    for key in sorted(records):
        metadata, body, target = records[key]
        (out / target).write_bytes(body)
        rows.append(dict(metadata, file=target))
    product = dict(format=PROFILE, types=rows)
    encoded = (json.dumps(product, sort_keys=True, indent=2) + '\n').encode()
    (out / 'catalog.preparation.json').write_bytes(encoded)
    receipt = dict(kind='explicit-compatible-complete-source-union-not-native-binding',
                   source_catalogs=sources, input_definitions=sum(len(s['definitions']) for s in sources),
                   unique_definitions=len(rows), ros_version=backend,
                   catalog_sha256=hashlib.sha256(encoded).hexdigest(), default_authority=[],
                   generated_headers=0, native_identities_generated=0, executions=0)
    for path, raw in captured.items():
        if read(path, max(len(raw), 1))[0] != raw:
            raise ValueError('source changed; incomplete output must not be adopted')
    (out / 'merge-source-receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    return receipt


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--catalog', type=Path, action='append', required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    try:
        receipt = merge(a.catalog, a.out)
    except (ValueError, OSError, TypeError) as error:
        p.error(str(error))
    print(json.dumps({k: receipt[k] for k in ('unique_definitions', 'input_definitions', 'ros_version')}))


if __name__ == '__main__':
    main()
