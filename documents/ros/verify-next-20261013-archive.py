#!/usr/bin/env python3
"""Independently check the downloaded ROS evidence without extracting it."""
import argparse
import codecs
import collections
import gzip
import hashlib
import io
import json
from pathlib import Path, PurePosixPath
import tarfile

ARCHIVE_SHA = '8a1820e3786381cc947596bed82dda257fe43b78085e1f46bbb26515c1629ba3'
INDEX_SHA = 'ac51e8f214ce78f75f073d514c1a1e9b7355fe28246d73180a02f31e1b021302'
NESTED_SHA = 'c815a628a1067d3545443c621a5b022ebbaa2df03591d8563e17a4802e01434d'
MANIFEST_SHA = '8176ee558dd7621b2e519a84552f0900c54baba1f37f8f60d6ec6e40ba55e316'
REQUIRED = {
    'final-acceptance': '4a59f9a6192514d83273f0a466a122d23f5ae9d7a911cdaf4a3550d7f51ea40c',
    'full-remote-verification': '4660ec865de0f8f757f3a0a3dcb93fe40607a79a3bb842c8763bc486146ee901',
    'full-close-verification': '812d3c20d1b2d0f1ec6670ec4f4e5e86aa3b7d94671f5b62e4f36791ffb34cd9',
    'combined-validation': 'a5a12b672010c925fb2379a0f3e2d12b5ade8a0d8bbc56fbb7ebcdccb6905a43',
    'current-file-manifest': MANIFEST_SHA,
    'executed-input-list': '28a535adf5f8a2512a1c0fdf64c233ca719cc0689df485f708d2d4760a65aefc',
    'actual-pack-command': '322492e5e8c0146b5977c174d5eaeb43528503f3ed38a2a15875c281fe7735f5',
    'prepack-validation': '6726df4239520148a785c088335462a59db1357107cfaff775b0023cbdeffca3',
}
CHUNK = 1048576


def digest(path):
    value = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(CHUNK), b''):
            value.update(block)
    return value.hexdigest()


def safe_name(name, directory=False):
    value = name.rstrip('/') if directory else name
    parts = value.split('/')
    return (bool(value) and not value.startswith('/') and '\\' not in value
            and all(part not in ('', '.', '..') for part in parts)
            and str(PurePosixPath(value)) == value)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--protected', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if args.out.exists():
        raise RuntimeError('refusing to overwrite an existing verification result')
    checks = collections.Counter()

    def check(value, section, description):
        checks[section] += 1
        if not value:
            raise RuntimeError(description)

    docs = args.root / 'documents/ros'
    archive_path = docs / 'evidence-20261013-next.tgz'
    index_path = docs / 'next-20261013-evidence-index.json'
    check(digest(archive_path) == ARCHIVE_SHA, 'artifact', 'whole archive SHA differs')
    check(digest(index_path) == INDEX_SHA, 'artifact', 'external index SHA differs')
    check(archive_path.stat().st_size == 25177484, 'artifact', 'archive size differs')
    check(index_path.stat().st_size == 4427778, 'artifact', 'index size differs')
    index = json.loads(index_path.read_bytes())
    check(index['status'] == 'PASS', 'artifact', 'index status is not PASS')
    check(index['archive']['sha256'] == ARCHIVE_SHA, 'artifact', 'index archive binding differs')
    payloads = index['payloads']
    expected = {entry['arcname']: entry for entry in payloads}
    check(len(expected) == len(payloads) == 8304, 'schema', 'duplicate/missing index payload')
    for entry in payloads:
        check(safe_name(entry['arcname']), 'schema', 'unsafe index member')
        check(entry['kind'] in ('utf8-text', 'binary-nested-archive'), 'schema', 'unknown member kind')
        check(entry['isutf8'] == (entry['kind'] == 'utf8-text'), 'schema', 'UTF8 flag differs from kind')
    seen = set()
    counts = collections.Counter()
    byte_count = 0
    nested = None
    with gzip.open(archive_path, 'rb') as zipped:
        with tarfile.open(fileobj=zipped, mode='r|') as archive:
            for member in archive:
                name = member.name
                check(name in expected and name not in seen, 'member', 'unexpected or duplicate member: ' + name)
                check(member.isfile() and safe_name(name), 'member', 'nonregular or unsafe member: ' + name)
                check((member.uid, member.gid, member.mtime, member.uname, member.gname, member.mode)
                      == (0, 0, 0, '', '', 0o644), 'member', 'noncanonical metadata: ' + name)
                seen.add(name)
                entry = expected[name]
                check(member.size == entry['bytes'], 'member', 'member size differs: ' + name)
                decoder = codecs.getincrementaldecoder('utf-8')('strict') if entry['isutf8'] else None
                raw = io.BytesIO() if entry['kind'] == 'binary-nested-archive' else None
                sha = hashlib.sha256()
                size = 0
                with archive.extractfile(member) as stream:
                    first = True
                    for block in iter(lambda: stream.read(CHUNK), b''):
                        if first and decoder is not None:
                            check(not block.startswith(b'\x7fELF'), 'text', 'ELF labeled UTF8: ' + name)
                        first = False
                        sha.update(block)
                        size += len(block)
                        if decoder is not None:
                            decoder.decode(block, final=False)
                        if raw is not None:
                            raw.write(block)
                if decoder is not None:
                    decoder.decode(b'', final=True)
                    checks['text'] += 1
                check(size == entry['bytes'] and sha.hexdigest() == entry['sha256'],
                      'member', 'member bytes/SHA differ: ' + name)
                counts[entry['kind']] += 1
                byte_count += size
                if raw is not None:
                    check(nested is None and entry['sha256'] == NESTED_SHA, 'nested', 'unexpected nested binary archive')
                    nested = (name, raw.getvalue(), entry)
        # tar readers can stop at the end markers before the gzip trailer.
        while zipped.read(CHUNK):
            pass
    check(seen == set(expected), 'schema', 'exact member set differs')
    check(counts == {'utf8-text': 8303, 'binary-nested-archive': 1}, 'schema', 'member kind counts differ')
    check(byte_count == 133406763 == index['summary']['payload_bytes'], 'schema', 'total payload bytes differ')
    nested_names = set()
    nested_counts = collections.Counter()
    name, data, entry = nested
    with gzip.GzipFile(fileobj=io.BytesIO(data), mode='rb') as zipped:
        with tarfile.open(fileobj=zipped, mode='r|') as archive:
            for member in archive:
                check(member.name not in nested_names and safe_name(member.name, member.isdir()),
                      'nested', 'unsafe/duplicate nested member')
                nested_names.add(member.name)
                check(member.isfile() or member.isdir(), 'nested', 'nested link/special member')
                nested_counts['regular' if member.isfile() else 'directory'] += 1
                if member.isfile():
                    with archive.extractfile(member) as stream:
                        for block in iter(lambda: stream.read(CHUNK), b''):
                            pass
        while zipped.read(CHUNK):
            pass
    check(nested_counts['regular'] == entry['nested_regular_members'], 'nested', 'nested regular-member count differs')
    hash_members = collections.defaultdict(list)
    for entry in payloads:
        hash_members[entry['sha256']].append(entry['arcname'])
    required = {}
    for label, sha in REQUIRED.items():
        check(bool(hash_members[sha]), 'coverage', 'mandatory evidence missing: ' + label)
        required[label] = {'sha256': sha, 'members': hash_members[sha]}
    manifest_path = docs / 'task-files-next-20261013.json'
    check(digest(manifest_path) == MANIFEST_SHA, 'coverage', 'current source manifest differs')
    source_records = json.loads(manifest_path.read_bytes())['files']
    check(len(source_records) == 17, 'coverage', 'source manifest count differs')
    source_coverage = []
    for record in source_records:
        path = args.root / record['path']
        check(path.stat().st_size == record['bytes'] and digest(path) == record['sha256'],
              'current-source', 'current source differs: ' + record['path'])
        binary = record['path'].endswith('.wasm')
        if binary:
            check(not hash_members[record['sha256']], 'coverage', 'generated Wasm unexpectedly packaged as text')
        else:
            check(bool(hash_members[record['sha256']]), 'coverage', 'current UTF8 source not packaged: ' + record['path'])
        source_coverage.append({'path': record['path'], 'sha256': record['sha256'],
                                'binary_excluded': binary, 'members': hash_members[record['sha256']]})
    protected = json.loads(args.protected.read_bytes())
    check(len(protected) == 20, 'preservation', 'unexpected prior artifact count')
    for name, sha in protected.items():
        check(digest(args.root / name) == sha, 'preservation', 'prior delivery changed: ' + name)
    check(digest(archive_path) == ARCHIVE_SHA and digest(index_path) == INDEX_SHA,
          'artifact', 'artifact changed during verification')
    result = {
        'status': 'PASS', 'scope': 'Downloaded archive bytes, external index, all members and current/prior local files',
        'executed_verifier_sha256': digest(__file__), 'checks': sum(checks.values()),
        'check_groups': dict(checks),
        'archive': {'path': str(archive_path), 'sha256': ARCHIVE_SHA, 'bytes': 25177484},
        'external_index': {'path': str(index_path), 'sha256': INDEX_SHA, 'bytes': 4427778, 'is_archive_member': False},
        'members': {'total': len(seen), 'utf8_text': counts['utf8-text'], 'binary_nested_archive': counts['binary-nested-archive'],
                    'payload_bytes': byte_count, 'exact_set': True, 'all_sha256_and_sizes_matched': True,
                    'fixed_metadata': True, 'full_gzip_trailer_checked': True},
        'historical_nested_archive': {'member': nested[0], 'sha256': NESTED_SHA,
                                      'members': len(nested_names), **nested_counts, 'full_gzip_trailer_checked': True},
        'mandatory_evidence': required, 'source_manifest': {'sha256': MANIFEST_SHA, 'files': source_coverage},
        'protected_prior_files': {'manifest_sha256': digest(args.protected), 'files': len(protected), 'all_sha256_unchanged': True},
        'claims_about_ros_execution': False,
    }
    args.out.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n')
    print(json.dumps({'status': result['status'], 'checks': result['checks'], 'members': len(seen),
                      'utf8': counts['utf8-text'], 'nested_members': len(nested_names), 'result': str(args.out)}, ensure_ascii=False))


if __name__ == '__main__':
    main()
