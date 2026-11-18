"""Execute the authenticated cases inside the fixture's disposable Linux guest."""
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
from types import SimpleNamespace

from packages import SUITES
from run import worker, write_json


def observe(release):
    output = Path('/results') / release
    status = Path('/proc/self/status').read_text().splitlines()
    capabilities = {line.split(':', 1)[0]: line.split(':', 1)[1].strip() for line in status if line.startswith('Cap')}
    if not all(int(value, 16) == 0 for value in capabilities.values()) or 'NoNewPrivs:\t1' not in status:
        raise RuntimeError('native observation process retained privileges')
    write_json(output / 'environment.json', {'capabilities': capabilities, 'noNewPrivileges': True,
                                             'kernel': os.uname().release})
    suite = SUITES[release]
    ethernet = Path('/devices/e1000.ko').is_file()
    args = SimpleNamespace(plan=Path('/input') / release / 'signed-plan.json', index_dir=Path('/input') / suite,
                           keyring=Path('/input/keyring.gpg'), legacy_system=Path('/syslib') / suite,
                           jazzy_prefix=Path('/opt/ros/jazzy'), jazzy_system=Path('/syslib/jazzy'),
                           profile=Path('/input/loopback.xml'), witness=Path('/fixture/jazzy-string'),
                           output=output, legacy_defaults=ethernet, guest_ethernet=ethernet)
    return worker(args)


def main():
    if len(sys.argv) == 2:
        return observe(sys.argv[1])
    output = Path('/results')
    output.mkdir()
    for release in json.loads(Path('/input/releases.json').read_bytes()):
        print('Starting', release, flush=True)
        folder = output / release
        folder.mkdir()
        command = ['/usr/bin/setpriv', '--bounding-set=-all', '--inh-caps=-all', '--ambient-caps=-all',
                   '--no-new-privs', sys.executable, '-B', __file__, release]
        with (folder / 'worker.log').open('wb') as log:
            result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT)
        print('Finished', release, result.returncode, flush=True)
        for name in ('legacy', 'ros-home'):
            temporary = Path('/tmp') / name
            if temporary.exists():
                shutil.rmtree(temporary)
    return 0


if __name__ == '__main__':
    sys.exit(main())
