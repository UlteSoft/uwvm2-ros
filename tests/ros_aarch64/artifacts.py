"""Deduplicate immutable, verified ARM executable snapshots on a bounded test filesystem."""
import json
import errno
import os
from pathlib import Path
import shutil

from runtime import digest, require_arm


class Pool:
    def __init__(self, manifest):
        self.path = Path(manifest).resolve(strict=True)
        self.document = json.loads(self.path.read_text())
        self.manifest_sha256 = digest(self.path)
        if self.document.get('status') != 'PASS':
            raise RuntimeError('verified ARM artifact pool required')
        self.original = shutil.copy2
        self.copy_fallbacks = []
        self.records = self.document['artifacts']
        self.product_names = {Path(source).name for source in self.records}
        self.aliases = self.document.get('aliases', {})
        for alias, selected in self.aliases.items():
            if selected not in self.records or str(Path(alias).resolve(strict=True)) != selected:
                raise RuntimeError('qualified ARM product alias changed: '+alias)
        for source, row in self.records.items():
            self.check(source, row)

    def check(self, source, row):
        if digest(self.path) != self.manifest_sha256:
            raise RuntimeError('ARM pool manifest changed')
        original, frozen = require_arm(source), require_arm(row['pool'])
        if original['sha256'] != row['sha256'] or frozen['sha256'] != row['sha256']:
            raise RuntimeError('ARM pool/source SHA mismatch: '+source)
        source_identity = original['device'], original['inode']
        pool_identity = frozen['device'], frozen['inode']
        if source_identity != (row['device'], row['inode']) or pool_identity != (row['pool_device'], row['pool_inode']) or source_identity == pool_identity:
            raise RuntimeError('ARM pool/source immutable file identity mismatch: '+source)

    def copy(self, source, destination, *args, **kwargs):
        selected = str(Path(source).resolve())
        requested = str(Path(source).absolute())
        expected = self.aliases.get(requested, requested if requested in self.records else None)
        if expected is not None and selected != expected:
            raise RuntimeError('ARM product alias selects a different qualified product: '+str(source))
        if Path(source).name in self.product_names and Path(source).name != Path(selected).name:
            raise RuntimeError('ARM product alias changes the logical product name: '+str(source))
        row = self.records.get(selected)
        if row is None:
            if Path(source).name in self.product_names or Path(selected).name in self.product_names:
                raise RuntimeError('ARM product snapshot source is outside the qualified pool: '+str(source))
            return self.original(source, destination, *args, **kwargs)
        self.check(selected, row)
        destination = Path(destination)
        if destination.is_dir():
            destination = destination/Path(source).name
        # Every snapshot references our separate frozen pool copy. Compilation
        # outputs remain separate inodes, so rebuilding cannot mutate a snapshot.
        try:
            os.link(row['pool'], destination)
        except OSError as error:
            if error.errno != errno.EXDEV:
                raise
            # Linux can reject links across two bind mounts of one filesystem,
            # even when st_dev matches. Keep an independent pool-derived copy.
            self.original(row['pool'], destination, *args, **kwargs)
            acquired = require_arm(destination)
            if acquired['sha256'] != row['sha256'] or (acquired['device'], acquired['inode']) in (
                    (row['device'], row['inode']), (row['pool_device'], row['pool_inode'])):
                raise RuntimeError('ARM pool copy fallback integrity mismatch')
            self.copy_fallbacks.append({'source': row['pool'], 'snapshot': str(destination),
                                       'sha256': acquired['sha256'], 'reason': 'EXDEV across mounts'})
        return str(destination)

    def install(self):
        shutil.copy2 = self.copy

    def restore(self):
        shutil.copy2 = self.original

    def evidence(self):
        if digest(self.path) != self.manifest_sha256:
            raise RuntimeError('ARM pool manifest changed during execution')
        return {'manifest': str(self.path), 'sha256': self.manifest_sha256,
                'qualified_aliases': self.aliases,
                'copy_fallbacks': self.copy_fallbacks,
                'scope': 'Hardlinks or verified copies from separate ARM pool copies; original compiler outputs have different inodes.'}


def create(destination, paths):
    destination = Path(destination).resolve()
    destination.mkdir(mode=0o700, parents=True, exist_ok=False)
    rows = {}
    aliases = {}
    for index, original in enumerate(paths):
        source = Path(original).resolve(strict=True)
        aliases[str(Path(original).absolute())] = str(source)
        if str(source) in rows:
            continue
        before = require_arm(source)
        copy = destination/f'{index}-{source.name}'
        shutil.copy2(source, copy)
        copy.chmod(0o555)
        frozen = require_arm(copy)
        after = require_arm(source)
        if before != after or frozen['sha256'] != before['sha256']:
            raise RuntimeError('ARM source changed during pool freeze: '+str(source))
        if (frozen['device'], frozen['inode']) == (before['device'], before['inode']):
            raise RuntimeError('pool copy must have a separate inode from compiler output')
        rows[str(source)] = {**before, 'pool': str(copy), 'pool_device': frozen['device'], 'pool_inode': frozen['inode']}
    manifest = destination/'manifest.json'
    manifest.write_text(json.dumps({'status': 'PASS', 'artifacts': rows, 'aliases': aliases}, indent=2)+'\n')
    return manifest
