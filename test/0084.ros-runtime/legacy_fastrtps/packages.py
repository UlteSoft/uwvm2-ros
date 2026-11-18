"""Fetch the signed ROS runtime closures for archived Fast RTPS String examples."""
import argparse
from concurrent.futures import ThreadPoolExecutor
from email.parser import BytesHeaderParser
import gzip
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
import urllib.request

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from run_archived_dds_peer import SIGNER, require
from run_scene_vm import sha, verify_gate

SUITES = {'r2b2': 'xenial', 'r2b3': 'xenial', 'ardent': 'xenial', 'bouncy': 'bionic', 'crystal': 'bionic'}
DEMOS = ['examples-rclcpp-minimal-publisher', 'examples-rclcpp-minimal-subscriber', 'rmw-fastrtps-cpp']
FIELDS = ('Package', 'Version', 'Architecture', 'Filename', 'Size', 'SHA256')
MIRROR = 'https://repo.ros2.org/ubuntu/main/'


def signed_plan(release, index_dir, keyring):
    suite = SUITES[release]
    metadata = index_dir / 'InRelease'
    signature = subprocess.run(['/usr/bin/gpgv', '--status-fd', '1', '--keyring', keyring, metadata],
                               capture_output=True, check=True)
    require(('[GNUPG:] VALIDSIG ' + SIGNER + ' ').encode() in signature.stdout, 'unexpected ROS archive signer')
    message = metadata.read_bytes()
    require(('\nCodename: ' + suite + '\n').encode() in message and b'\nOrigin: ROS\n' in message, 'wrong ROS suite')
    table = message.split(b'\nSHA256:\n', 1)[1].split(b'\nSHA512:', 1)[0]
    digest, size = re.search(rb'^ ([0-9a-f]{64})\s+(\d+) main/binary-amd64/Packages\.gz$', table, re.M).groups()
    data = (index_dir / 'Packages.gz').read_bytes()
    require(len(data) == int(size) and hashlib.sha256(data).hexdigest() == digest.decode(), 'unrecognized package index')
    prefix = 'ros-' + release + '-'
    packages = {}
    for block in gzip.decompress(data).split(b'\n\n'):
        if block.startswith(b'Package: ' + prefix.encode()):
            fields = BytesHeaderParser().parsebytes(block)
            if fields['Architecture'] in ('amd64', 'all'):
                packages[fields['Package']] = fields
    pending = [prefix + name for name in DEMOS]
    selected = {}
    system = set()
    while pending:
        name = pending.pop()
        if name in selected:
            continue
        entry = packages[name]
        selected[name] = {key: entry[key] for key in FIELDS}
        for group in (entry.get('Pre-Depends', '') + ',' + entry.get('Depends', '')).split(','):
            alternatives = [term.split()[0] for term in group.split('|') if term.strip()]
            if alternatives and alternatives[0].startswith(prefix):
                pending.append(next(term for term in alternatives if term in packages))
            elif alternatives:
                system.add(alternatives[0])
    return {'format': 'signed-legacy-ros-demo-v1', 'release': release, 'suite': suite, 'signer': SIGNER,
            'inReleaseSha256': sha(metadata), 'indexSha256': digest.decode(), 'packages': selected,
            'systemDependencies': sorted(system)}


def archives(plan, directory, *, fetch=False):
    def check(item):
        name, entry = item
        relative = Path(entry['Filename'])
        require(not relative.is_absolute() and relative.parts[0] == 'pool' and '..' not in relative.parts,
                'invalid signed package path')
        archive = directory / relative.name
        expected = int(entry['Size'])
        if fetch and not archive.exists():
            with urllib.request.urlopen(MIRROR + str(relative), timeout=50) as response:
                data = response.read(expected + 1)
            require(len(data) == expected and hashlib.sha256(data).hexdigest() == entry['SHA256'], 'download mismatch')
            archive.write_bytes(data)
        require(archive.stat().st_size == expected and sha(archive) == entry['SHA256'], 'package file mismatch')
        return name, (archive, entry)
    with ThreadPoolExecutor(max_workers=4) as pool:
        return dict(pool.map(check, plan['packages'].items()))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('release', choices=SUITES)
    for name in ('index-dir', 'keyring', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    plan = signed_plan(args.release, args.index_dir, args.keyring)
    debs = args.output / 'debs'
    debs.mkdir(parents=True, exist_ok=True)
    archives(plan, debs, fetch=True)
    (args.output / 'signed-plan.json').write_text(json.dumps(plan, indent=2) + '\n')


if __name__ == '__main__':
    main()
