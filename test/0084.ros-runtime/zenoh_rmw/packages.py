"""Authenticate and stage the two native ROS Zenoh packages from the Noble archive."""
import argparse
from email.parser import BytesHeaderParser
import gzip
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import urllib.request

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from run_archived_dds_peer import SIGNER, require
from run_scene_vm import sha, verify_gate

RELEASES = ('jazzy', 'kilted')
MIRROR = 'https://repo.ros2.org/ubuntu/main/'


def entries(index, keyring, release):
    require(release in RELEASES, 'unsupported Noble release')
    signature = subprocess.run(['/usr/bin/gpgv', '--keyring', keyring, '--status-fd', '1', index / 'InRelease'],
                               capture_output=True, check=True)
    require(('[GNUPG:] VALIDSIG ' + SIGNER + ' ').encode() in signature.stdout, 'ROS archive signer differs')
    metadata = (index / 'InRelease').read_bytes()
    require(b'\nOrigin: ROS\n' in metadata and b'\nCodename: noble\n' in metadata, 'wrong ROS archive')
    table = metadata.split(b'\nSHA256:\n', 1)[1].split(b'\nSHA512:', 1)[0]
    digest, size = re.search(rb'^ ([0-9a-f]{64})\s+(\d+) main/binary-amd64/Packages\.gz$', table, re.M).groups()
    compressed = (index / 'Packages.gz').read_bytes()
    require(len(compressed) == int(size) and hashlib.sha256(compressed).hexdigest() == digest.decode(), 'wrong index')
    names = {f'ros-{release}-{name}' for name in ('rmw-zenoh-cpp', 'zenoh-cpp-vendor')}
    selected = {}
    for block in gzip.decompress(compressed).split(b'\n\n'):
        name = block.partition(b'\n')[0].removeprefix(b'Package: ').decode(errors='replace')
        if name not in names:
            continue
        entry = BytesHeaderParser().parsebytes(block)
        relative = Path(entry['Filename'])
        require(entry['Architecture'] == 'amd64' and relative.parts[:2] == ('pool', 'main')
                and '..' not in relative.parts, 'unexpected archive member')
        selected[name] = {k: entry[k] for k in ('Package', 'Version', 'Architecture', 'Filename', 'SHA256', 'Size')}
    require(set(selected) == names, 'missing ROS Zenoh package')
    return {'release': release, 'suite': 'noble', 'signer': SIGNER, 'indexSha256': digest.decode(),
            'inReleaseSha256': sha(index / 'InRelease'), 'packages': selected}


def runtime_member(path):
    return (re.fullmatch(r'.*\.so(?:\.[0-9]+)*', path.name) or path.name == 'rmw_zenohd'
            or path.suffix == '.json5' or 'ament_index' in path.parts
            or path.name in ('package.xml', 'copyright', 'LICENSE', 'LICENSE.txt', 'NOTICE', 'LICENSE.md'))


def prepare(index, keyring, release, output):
    plan = entries(index, keyring, release)
    output.mkdir(parents=True, exist_ok=True)
    for name, record in plan['packages'].items():
        archive = output / Path(record['Filename']).name
        if not archive.exists():
            with urllib.request.urlopen(MIRROR + record['Filename'], timeout=60) as stream:
                data = stream.read(int(record['Size']) + 1)
            require(len(data) == int(record['Size']) and hashlib.sha256(data).hexdigest() == record['SHA256'],
                    'download disagrees with signed index')
            archive.write_bytes(data)
        require(archive.stat().st_size == int(record['Size']) and sha(archive) == record['SHA256'], 'wrong archive bytes')
        with tempfile.TemporaryDirectory(dir='/dev/shm', prefix='rmw-zenoh-') as directory:
            root = Path(directory)
            subprocess.run(['/usr/bin/dpkg-deb', '-x', archive, root], check=True)
            record['members'] = {}
            for item in sorted(root.rglob('*')):
                relative = item.relative_to(root)
                if not item.is_file() or not runtime_member(relative):
                    continue
                resolved = item.resolve(strict=True)
                require(resolved.is_relative_to(root), 'external archive link')
                target = output / 'fs' / relative
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(resolved, target)
                record['members'][str(relative)] = {'sha256': sha(target), 'sourceMember': str(resolved.relative_to(root))}
    (output / 'inputs.json').write_text(json.dumps(plan, indent=2) + '\n')
    return plan


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('release', choices=RELEASES)
    for name in ('index', 'keyring', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    require(os.sched_getaffinity(0).issubset({4, 5, 6, 7}), 'unexpected CPU affinity')
    prepare(args.index, args.keyring, args.release, args.output)


if __name__ == '__main__':
    main()
