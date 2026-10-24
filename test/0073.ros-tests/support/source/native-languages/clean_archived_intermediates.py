#!/usr/bin/env python3
"""Release only fixed Task03 intermediates after exact two-archive checks.

Run through the common finite owner. Never remove tools, downloads, original
input archives, Wasm commands, logs, receipts or either immutable evidence copy.
The original source snapshots and compiled rlibs remain in both archives.
"""

from pathlib import Path
import hashlib
import json
import os
import shutil
import tarfile


BASE = Path('/home/macromodel/Documents/uwvm-test-261018/agents')
RUNS = {
    '20261021-go-values-r1': ('source',),
    '20261021-go-values-r4': ('source',),
    '20261021-native-corrected-r1': ('source', 'generated', 'numeric'),
    '20261021-native-corrected-r4': ('source', 'generated', 'numeric'),
    '20261021-native-corrected-r5': ('source', 'generated', 'numeric'),
    '20261021-nim-odin-r1': ('source', 'generated', 'numeric'),
    '20261021-odin-r6': ('source', 'generated', 'numeric'),
    '20261021-odin-r8': ('source', 'generated', 'numeric'),
    '20261021-odin-r9': ('source', 'generated', 'numeric'),
    '20261021-source-audit-r1': ('source',),
    '20261021-source-audit-r3': ('source',),
    '20261021-source-audit-r4': ('source',),
    '20261021-validation-r1': ('source', 'generated', 'numeric', 'rust'),
    '20261021-validation-r3': ('source', 'generated', 'numeric', 'rust'),
    '20261021-validation-r5': ('source', 'generated', 'numeric'),
}


def digest(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    report = {
        'kind': 'task03-archived-intermediate-cleanup-v1',
        'status': 'VERIFYING',
        'runs': [],
        'removed': [],
        'compiler_runs': 0,
        'Guest_runs': 0,
        'default_authority': [],
    }
    output = Path.cwd() / 'cleanup.json'

    def save():
        output.write_text(json.dumps(report, indent=2) + '\n')

    save()
    selected = []
    for name, directories in RUNS.items():
        root = BASE / '03' / name
        if root.resolve(strict=True) != root:
            raise ValueError('Fixed ordinary Task03 run root required')
        receipt = json.loads((root / 'resource-archives/verified-copies.json').read_bytes())
        actual = {
            Path(value[5:] if value.startswith('/host/') else value)
            for value in receipt.get('verified_paths', [])
        }
        expected = {
            root / 'resource-archives/evidence.tar',
            BASE / 'director-20261021/infrastructure/archives' / ('03-' + name + '.tar'),
        }
        if receipt.get('copies') != 2 or actual != expected:
            raise ValueError('Two exact original archive locations required')
        for path in sorted(actual):
            if path.is_symlink() or path.stat().st_size != receipt['bytes']:
                raise ValueError('Original archive extent/kind changed')
            if digest(path) != receipt['sha256']:
                raise ValueError('Original archive read-back SHA-256 differs')
        row = {'run': name, 'both_archives_rehashed': True,
               'archive_sha256': receipt['sha256'], 'directories': []}
        report['runs'].append(row)
        save()
        with tarfile.open(root / 'resource-archives/evidence.tar') as archive:
            members = {item.name.rstrip('/'): item for item in archive.getmembers()}
            for directory in directories:
                target = root / 'work' / directory
                if target.is_symlink() or not target.is_dir():
                    raise ValueError('Fixed ordinary intermediate directory required')
                if target.resolve(strict=True) != target or target.stat().st_uid != os.getuid():
                    raise ValueError('Intermediate ownership or path changed')
                prefix = 'work/' + directory + '/'
                archived = {key[len(prefix):]: item for key, item in members.items()
                            if key.startswith(prefix)}
                current = {path.relative_to(target).as_posix(): path
                           for path in target.rglob('*')}
                if set(archived) != set(current):
                    raise ValueError('Intermediate entries differ from original archive')
                files = total = 0
                for relative, path in current.items():
                    item = archived[relative]
                    if path.is_symlink() or path.stat().st_uid != os.getuid():
                        raise ValueError('Intermediate link/ownership differs')
                    if path.is_dir():
                        if not item.isdir():
                            raise ValueError('Archived directory kind differs')
                        continue
                    if not path.is_file() or not item.isfile() or path.stat().st_size != item.size:
                        raise ValueError('Intermediate file kind/extent differs')
                    with archive.extractfile(item) as stream:
                        original = hashlib.file_digest(stream, 'sha256').hexdigest()
                    if digest(path) != original:
                        raise ValueError('Intermediate bytes differ from original archive')
                    files += 1
                    total += item.size
                row['directories'].append({'path': str(target), 'verified_files': files,
                                           'verified_bytes': total})
                selected.append(target)
                save()

    # Verify every candidate before the first deletion. A mismatch preserves all.
    report['status'] = 'ALL_VERIFIED'
    save()
    for path in selected:
        shutil.rmtree(path)
        report['removed'].append(str(path))
        save()
    report['status'] = 'PASS'
    save()


if __name__ == '__main__':
    main()
