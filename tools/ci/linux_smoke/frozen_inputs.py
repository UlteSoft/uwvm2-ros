"""Exact existing LLVM inputs, read through actual read-only mounted FDs.

This guarded manual lane does not download, extract, or trust prior runs.
The caller keeps the verified compiler FD open until every actor has retired.
"""
from pathlib import Path
import hashlib
import json
import os
import stat

from acquire import digest, require


def identity(info):
    return {'device': info.st_dev, 'inode': info.st_ino, 'bytes': info.st_size,
            'mode': stat.S_IMODE(info.st_mode), 'mtime_ns': info.st_mtime_ns,
            'ctime_ns': info.st_ctime_ns}


def readonly(path):
    fd = os.open(path, os.O_RDONLY | os.O_NOFOLLOW | os.O_CLOEXEC)
    try:
        info = Path('/proc/self/fdinfo', str(fd)).read_text()
        mount_id = next(row.split()[1] for row in info.splitlines() if row.startswith('mnt_id:'))
        mounts = [row for row in Path('/proc/self/mountinfo').read_text().splitlines()
                  if row.split()[0] == mount_id]
        require(len(mounts) == 1 and 'ro' in mounts[0].split()[5].split(','),
                'frozen original FD is not mounted read-only: ' + str(path))
        require(stat.S_ISREG(os.fstat(fd).st_mode), 'regular frozen input required')
        return fd, {'fd': fd, 'mount_id': mount_id, 'mountinfo': mounts[0],
                    'access_mode': 'O_RDONLY', 'nofollow': True}
    except BaseException:
        os.close(fd)
        raise


def read_file(path, pin, retain=False):
    fd, evidence = readonly(path)
    keep = False
    try:
        before = identity(os.fstat(fd))
        with os.fdopen(os.dup(fd), 'rb') as stream:
            value = hashlib.file_digest(stream, 'sha256').hexdigest()
        require(before == identity(os.fstat(fd)) == identity(Path(path).stat()),
                'frozen file changed while hashing: ' + str(path))
        require(value == pin['sha256'] and before['bytes'] == pin['bytes'],
                'frozen file bytes differ: ' + str(path))
        if 'mode' in pin:
            require(before['mode'] == pin['mode'], 'frozen mode differs: ' + str(path))
        evidence.update(path=str(path), identity=before, sha256=value)
        keep = retain
        return evidence, fd if retain else None
    finally:
        if not keep:
            os.close(fd)


class FrozenInputs:
    def __init__(self, root, archive, fixtures):
        self.root, self.archive, self.fixtures = map(Path, (root, archive, fixtures))
        require(self.root == Path('/compiler') and self.archive == Path('/compiler-archive.xmz') and
                self.fixtures == Path('/packet/subject/tools/ci/linux_smoke/frozen-sdk'),
                'closed guarded manual input mount paths required')
        self.lock_path = Path(__file__).with_name('frozen-inputs.lock.json')
        self.lock = json.loads(self.lock_path.read_text())
        require(self.lock['format'] == 'uwvm-linux-frozen-compiler-v1' and self.lock['version'] == '23.1.4',
                'unknown frozen compiler input contract')
        self.compiler_fd = None
        self.compiler_observation = None

    def verify(self):
        lock = self.lock
        expected = {row['path']: row for row in lock['files']}
        require(len(expected) == len(lock['files']) == 363, 'frozen input census differs')
        for name in expected:
            path = Path(name)
            require(not path.is_absolute() and '..' not in path.parts, 'unsafe frozen input path')
        actual = {str(path.relative_to(self.root)) for path in self.root.rglob('*')
                  if path.is_file() or path.is_symlink()}
        require(actual == set(expected), 'frozen compiler tree has extra or missing entries')
        archive, unused = read_file(self.archive, lock['archive'])
        files = []
        for name, pin in sorted(expected.items()):
            path = self.root/name
            if pin['type'] == 'symlink':
                require(path.is_symlink() and os.readlink(path) == pin['target'] and
                        path.resolve(strict=True) == self.root/pin['resolved_relative'] and
                        path.resolve(strict=True).is_relative_to(self.root), 'frozen alias changed: ' + name)
                files.append(dict(pin))
            else:
                require(not path.is_symlink() and path.resolve(strict=True) == path,
                        'frozen regular input path traverses an alias: ' + name)
                retained = name == lock['compiler_image'] and self.compiler_fd is None
                observed, fd = read_file(path, pin, retain=retained)
                if retained:
                    self.compiler_fd, self.compiler_observation = fd, observed
                files.append(dict(pin, actual_readonly_FD=observed))
        require(digest(self.root/lock['provenance_path']) == lock['provenance_sha256'],
                'frozen provenance bytes differ')
        sdk_lock = json.loads(Path(__file__).with_name('inputs.lock.json').read_text())
        sdk = []
        for pin in sdk_lock['sdk_fixtures']:
            path = self.fixtures/pin['path']
            require(path.resolve(strict=True) == path and path.is_relative_to(self.fixtures),
                    'frozen SDK fixture path differs')
            observed, unused = read_file(path, pin)
            sdk.append(observed)
        require(self.compiler_fd is not None and identity(os.fstat(self.compiler_fd)) ==
                self.compiler_observation['identity'], 'retained compiler FD changed')
        return {'format': 'uwvm-actual-frozen-compiler-inputs-v1', 'version': lock['version'],
                'lock_sha256': digest(self.lock_path), 'archive': archive, 'files': files,
                'sdk_fixtures': sdk, 'compiler_image': self.compiler_observation,
                'network_used': False, 'prior_qualification_borrowed': False}

    def mapped_compiler(self, maps_text):
        require(self.compiler_fd is not None and identity(os.fstat(self.compiler_fd)) ==
                self.compiler_observation['identity'], 'retained compiler FD changed during command')
        info = self.compiler_observation['identity']
        rows = []
        for line in maps_text.splitlines():
            fields = line.split(maxsplit=5)
            if len(fields) == 6 and fields[5] == str(self.root/self.lock['compiler_image']):
                major, minor = (int(value, 16) for value in fields[3].split(':'))
                require(os.makedev(major, minor) == info['device'] and int(fields[4]) == info['inode'],
                        'actual mapped compiler differs from the retained original FD')
                rows.append(line)
        if not any('x' in row.split()[1] for row in rows):
            return None
        return {'path': str(self.root/self.lock['compiler_image']), 'sha256': self.compiler_observation['sha256'],
                'retained_readonly_FD': self.compiler_observation, 'actual_maps_rows': rows,
                'actual_mapped_image_verified': True}

    def close(self):
        if self.compiler_fd is not None:
            os.close(self.compiler_fd)
            self.compiler_fd = None
