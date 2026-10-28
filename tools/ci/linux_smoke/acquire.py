"""Acquire fixed bootstrap and fixture bytes without package installation."""
from pathlib import Path, PurePosixPath
import hashlib
import json
import os
import shutil
import tarfile
import time
import urllib.request


def require(value, message):
    if not value:
        raise RuntimeError(message)


def digest(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def save(path, value):
    Path(path).write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')


def download(spec, destination, receipt):
    destination = Path(destination)
    require(not destination.exists(), 'fresh provider destination required')
    require(spec['url'].startswith('https://'), 'HTTPS provider required')
    maximum = spec.get('bytes', spec.get('maximum_bytes'))
    require(type(maximum) is int and 0 < maximum <= 100 * 1024**2, 'bounded locked provider required')
    started = time.monotonic()
    row = {'url': spec['url'], 'expected_sha256': spec['sha256'], 'bytes': 0,
           'status': 'FAIL', 'executed_downloaded_code': False, 'tls_verification': True}
    try:
        request = urllib.request.Request(spec['url'], headers={'User-Agent': 'uwvm-linux-smoke/1'})
        with urllib.request.urlopen(request, timeout=30) as response, destination.open('xb') as output:
            require(response.status == 200 and response.url.startswith('https://'), 'unexpected provider response')
            row['final_url'] = response.url
            checksum = hashlib.sha256()
            while chunk := response.read(1024**2):
                row['bytes'] += len(chunk)
                require(row['bytes'] <= maximum and time.monotonic() - started <= 300,
                        'provider size or total download deadline exceeded')
                require(shutil.disk_usage(destination.parent).free >= 2 * 1024**3, '2GiB owned work reserve reached')
                checksum.update(chunk)
                output.write(chunk)
            output.flush()
            os.fsync(output.fileno())
        row['sha256'] = checksum.hexdigest()
        require(row['sha256'] == spec['sha256'] and row['bytes'] == spec.get('bytes', row['bytes']),
                'downloaded provider whole-file hash or extent differs')
        row['status'] = 'PASS_BYTES_ONLY'
    except BaseException as error:
        row['error'] = repr(error)
        raise
    finally:
        row['seconds'] = time.monotonic() - started
        save(receipt, row)
    return row


def member_path(root, name):
    relative = PurePosixPath(name)
    require(not relative.is_absolute() and '..' not in relative.parts and '\\' not in name,
            'archive member escapes extraction root')
    path = root.joinpath(*relative.parts)
    require(path.resolve().is_relative_to(root.resolve()), 'archive traverses an outside symlink')
    return path


def unpack(archive_path, root):
    root = Path(root)
    root.mkdir(mode=0o700)
    count, total, links = 0, 0, []
    with tarfile.open(archive_path, 'r:xz') as archive:
        for member in archive:
            count += 1
            total += member.size
            require(count <= 50000 and total <= 2 * 1024**3, 'bootstrap archive expansion bound exceeded')
            path = member_path(root, member.name)
            path.parent.mkdir(parents=True, exist_ok=True)
            if member.isdir():
                path.mkdir(exist_ok=True)
            elif member.isfile():
                require(not path.exists() and not path.is_symlink(), 'archive file collision')
                with archive.extractfile(member) as incoming, path.open('xb') as outgoing:
                    shutil.copyfileobj(incoming, outgoing)
                path.chmod(member.mode & 0o777)
            elif member.issym() or member.islnk():
                links.append((member, path))
            else:
                raise RuntimeError('archive contains a special file')
    for member, path in links:
        require(not path.exists() and not path.is_symlink(), 'archive alias collision')
        if member.islnk():
            target = member_path(root, member.linkname)
            require(target.is_file() and not target.is_symlink(), 'archive hardlink target differs')
            os.link(target, path)
        else:
            require(not PurePosixPath(member.linkname).is_absolute(), 'absolute bootstrap symlink')
            require((path.parent/member.linkname).resolve().is_relative_to(root.resolve()), 'archive symlink escapes root')
            path.symlink_to(member.linkname)
    for member, path in links:
        require(path.resolve(strict=True).is_relative_to(root.resolve()), 'final archive alias chain escapes root')
    return {'members': count, 'regular_bytes': total}
