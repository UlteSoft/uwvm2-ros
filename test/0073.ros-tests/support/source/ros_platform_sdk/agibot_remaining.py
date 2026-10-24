#!/usr/bin/env python3
"""Capture selected source from every SDK archive in the official X2 manifest.

Download one finite archive at a time inside the original 64GiB group. Keep
the original manifest, whole downloaded archive SHA, central-directory facts,
and selected unchanged ordinary source bodies. Never extract links, install or
execute a library/wheel/script, materialize robot assets or start a controller.
The temporary ZIP is removed only after every selected source matches its ZIP
CRC/extent and a complete source receipt has been closed. This source
capture is not signature verification, native SDK qualification or a ROS grant.
"""
import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import stat
import urllib.request
import zipfile


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    inputs = json.loads((a.root / 'inputs.json').read_bytes())
    manifest_path = Path(inputs['remaining_extras']) / 'agibot-sdk-downloads.json'
    manifest_bytes = manifest_path.read_bytes()
    manifest = json.loads(manifest_bytes)
    selected = [row for row in manifest if row.get('group') == 'sdk']
    assert len(selected) == 5 and all(type(row.get('size')) is int and 0 < row['size'] <= 384 << 20 for row in selected)
    a.out.mkdir(mode=0o700)
    (a.out / 'original-download-manifest.json').write_bytes(manifest_bytes)
    report = dict(status='RUNNING', source_manifest_sha256=hashlib.sha256(manifest_bytes).hexdigest(),
                  sdk_archives=[], default_authority=[], upstream_code_executed=False)
    try:
        for row in selected:
            key = re.sub('[^a-z0-9]+', '_', row['label'].lower()).strip('_')
            directory = a.out / key
            directory.mkdir()
            assert row['src'].startswith('/downloads/') and row['src'].endswith('.zip')
            url = 'https://x2-aimdk.agibot.com' + row['src']
            downloaded = directory / 'temporary-download.zip'
            digest, size = hashlib.sha256(), 0
            request = urllib.request.Request(url, headers={'User-Agent': 'uwvm-ros-selected-source-review'})
            with urllib.request.urlopen(request, timeout=30) as response, downloaded.open('xb') as stream:
                assert response.status == 200
                headers = dict(response.headers)
                while block := response.read(1 << 20):
                    size += len(block)
                    assert size <= row['size']
                    digest.update(block)
                    stream.write(block)
            assert size == row['size']
            members, files, total, types = [], [], 0, []
            with zipfile.ZipFile(downloaded) as archive:
                entries = archive.infolist()
                assert len(entries) <= 65536 and len({v.filename for v in entries}) == len(entries)
                for entry in entries:
                    path = PurePosixPath(entry.filename)
                    assert not path.is_absolute() and all(v not in ('', '.', '..') for v in path.parts) and '\\' not in entry.filename
                    mode = entry.external_attr >> 16
                    members.append(dict(path=entry.filename, bytes=entry.file_size, compressed_bytes=entry.compress_size,
                                        crc32=entry.CRC, mode=mode, flags=entry.flag_bits))
                    if entry.is_dir():
                        continue
                    declaration = path.suffix in ('.msg', '.srv', '.action', '.idl', '.proto')
                    metadata = path.name in ('package.xml', 'CMakeLists.txt', '.gitmodules', 'VERSION', 'setup.py', 'pyproject.toml')
                    documentation = path.suffix in ('.md', '.rst') or path.name.upper().startswith(('LICENSE', 'COPYING', 'NOTICE', 'README'))
                    if not (declaration or metadata or documentation):
                        continue
                    assert not stat.S_ISLNK(mode) and (not stat.S_IFMT(mode) or stat.S_ISREG(mode))
                    assert not entry.flag_bits & 1 and entry.file_size <= 2 << 20
                    total += entry.file_size
                    assert total <= 32 << 20
                    body = archive.read(entry)
                    assert len(body) == entry.file_size
                    target = directory / 'files' / path
                    target.parent.mkdir(parents=True, exist_ok=True)
                    target.write_bytes(body)
                    files.append(dict(path=entry.filename, bytes=len(body), sha256=hashlib.sha256(body).hexdigest(), crc32=entry.CRC))
                    if declaration:
                        types.append(entry.filename)
            proof = dict(label=row['label'], id=key, original_manifest_entry=row, url=url, headers=headers,
                         archive_bytes=size, archive_sha256=digest.hexdigest(), members=members, files=files,
                         declaration_files=types, upstream_code_executed=False, default_authority=[])
            (directory / 'source-lock.json').write_text(json.dumps(proof, indent=2) + '\n')
            downloaded.unlink()
            report['sdk_archives'].append(dict(id=key, label=row['label'], archive_bytes=size,
                archive_sha256=digest.hexdigest(), files=len(files), source_bytes=total, declarations=len(types),
                temporary_archive_removed=True))
            (a.out / 'capture.json').write_text(json.dumps(report, indent=2) + '\n')
            print(json.dumps(report['sdk_archives'][-1]), flush=True)
        report['status'] = 'ALL_FIVE_OFFICIAL_MANIFEST_SDK_SOURCE_SELECTIONS_CAPTURED_WITHOUT_EXECUTION'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        (a.out / 'capture.json').write_text(json.dumps(report, indent=2) + '\n')


if __name__ == '__main__':
    main()
