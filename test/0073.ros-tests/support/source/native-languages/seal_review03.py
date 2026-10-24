#!/usr/bin/env python3
"""@brief Verify retained acceptance archives and a current owned-source capsule.

@details This finite-owner retention step hashes/reads data and checks Python
source syntax. It performs no SDK publication, language compilation or Guest
execution. Both immutable accepted/failed archive copies remain untouched.
@see handoffs/03/review03/README.md
"""
from pathlib import Path
import argparse
import hashlib
import json
import tarfile


def digest(path):
    """@brief Hash an ordinary complete data file without executing its contents.

    @param[in] path Retained archive or source capsule; borrowed during reading.
    @return Complete lowercase SHA-256; I/O errors propagate.
    @see main
    """
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    """@brief Bind current source hashes and Nim inventories to retained evidence.

    @details Fixed own run roots and their exact two archive locations are
    rehashed. Current source is checked against its captured hash manifest;
    four Nim module hashes must match the accepted frontend/semantic receipt.
    @return Zero after retention and source-integrity checks succeed.
    @failure Drift, wrong archive locations or incomplete source membership
    aborts retention. No historical file, permission or archive is modified.
    @see digest; review03_patch_audit.main
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capsule', type=Path, required=True)
    parser.add_argument('--sha256', required=True)
    args = parser.parse_args()
    if digest(args.capsule) != args.sha256:
        raise ValueError('Current owned source capsule changed')
    agents = Path('/home/macromodel/Documents/uwvm-test-261018/agents')
    rows = []
    for name in ('20261021-round03-source-r1', '20261021-round03-installed-nim-r1',
                 '20261021-round03-installed-nim-r2', '20261021-round03-nim-core-docs-r2',
                 '20261021-round03-patch-audit-r1', '20261021-round03-rustfmt-r1'):
        run = agents / '03' / name
        facts = json.loads((run / 'resource-archives/verified-copies.json').read_bytes())
        paths = [run / 'resource-archives/evidence.tar', agents / 'director-20261021/infrastructure/archives' / ('03-' + name + '.tar')]
        if facts['copies'] != 2 or set(facts['verified_paths']) != {str(path) for path in paths}:
            raise ValueError('Two original archive paths required')
        for path in paths:
            if path.is_symlink() or path.stat().st_size != facts['bytes'] or digest(path) != facts['sha256']:
                raise ValueError('Original immutable archive changed')
        rows.append({'run': name, 'both_original_copies_rehashed': True,
                     'bytes': facts['bytes'], 'sha256': facts['sha256']})
    handoff = 'sdk/origin/coordination/20261021-director/handoffs/03/review03/'
    with tarfile.open(args.capsule) as archive:
        entries = {item.name: item for item in archive if item.isfile()}
        def read(name):
            with archive.extractfile(entries[name]) as stream:
                return stream.read()
        source_manifest = read(handoff + 'OWNED-SOURCE-SNAPSHOT.sha256').decode()
        checked = 0
        for line in source_manifest.splitlines():
            expected, name = line.split('  ', 1)
            if hashlib.sha256(read(name)).hexdigest() != expected:
                raise ValueError('Current owned source pin differs: ' + name)
            checked += 1
        nim = json.loads(read(handoff + 'evidence/nim-core-docs-r2.json'))
        mapping = {'owner': 'roswasm.nim', 'wire': 'roswasm/wire.nim',
                   'typed': 'roswasm/typed.nim', 'raw': 'roswasm/raw.nim'}
        for inventory in nim['inventories']:
            name = 'sdk/origin/nim/src/' + mapping[inventory['module']]
            if hashlib.sha256(read(name)).hexdigest() != inventory['source_sha256']:
                raise ValueError('Nim documentation receipt is not for current source')
        syntax = 0
        for name in entries:
            if name.endswith('.py'):
                compile(read(name), name, 'exec', dont_inherit=True)
                syntax += 1
    result = {'kind': 'task03-review03-current-source-retention-v1', 'status': 'PASS_RETENTION_ONLY',
        'source_capsule_sha256': args.sha256, 'source_capsule_bytes': args.capsule.stat().st_size,
        'current_owned_source_files_checked': checked, 'current_Nim_core_receipt_matches': True,
        'Python_source_syntax_files_checked': syntax, 'original_archives': rows,
        'new_language_compilations': 0, 'new_Guest_runs': 0,
        'complete_platform_support': False, 'current_head_full_SDK_acceptance': False,
        'official_vendor_ports_accepted': 0, 'default_authority': []}
    (Path.cwd() / 'final-retention.json').write_text(json.dumps(result, indent=2) + '\n')
    print('PASS_CURRENT_SOURCE_AND_ORIGINAL_ARCHIVE_RETENTION', flush=True)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
