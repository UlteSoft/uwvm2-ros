#!/usr/bin/env python3
"""Execute only after joining the existing shared 64 GiB, zero-swap test gate.

This launcher never creates a replacement budget or changes resource limits.
A restricted control container enrolls the waiting child in the selected
existing group. The child checks membership, inode and limits before exec;
the requested compiler/test/profiler and its descendants inherit that group.
"""
import json
import os
from pathlib import Path
import subprocess
import sys


CONTAINER = 'uwvm-sdk-resource-gate-20261024'
IDENTITY = 'af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913'


def main():
    if len(sys.argv) < 2:
        raise SystemExit('Usage: in_existing_cgroup.py COMMAND [ARG ...]')
    info = json.loads(subprocess.check_output(['docker', 'inspect', CONTAINER], text=True))[0]
    group = Path('/sys/fs/cgroup/system.slice') / ('docker-' + IDENTITY + '.scope')
    assert info['Id'] == IDENTITY and info['State']['Running'], 'Existing gate unavailable'
    assert (group/'memory.max').read_text().strip() == '68719476736'
    assert (group/'memory.swap.max').read_text().strip() == '0'
    inode = group.stat().st_ino
    guard = '''import os,sys,time
from pathlib import Path
p=Path(sys.argv[1]);inode=int(sys.argv[2]);deadline=time.monotonic()+15
expected='0::/'+str(p.relative_to('/sys/fs/cgroup'))
while Path('/proc/self/cgroup').read_text().strip()!=expected:
 if time.monotonic()>=deadline:raise SystemExit('cgroup enrollment timed out; command was not executed')
 time.sleep(.02)
assert p.stat().st_ino==inode
assert (p/'memory.max').read_text().strip()=='68719476736'
assert (p/'memory.swap.max').read_text().strip()=='0'
assert str(os.getpid()) in (p/'cgroup.procs').read_text().split()
print('EXISTING_CGROUP '+str(p)+' memory.max=68719476736 swap.max=0',flush=True)
os.execvp(sys.argv[3],sys.argv[3:])
'''
    child = subprocess.Popen([sys.executable, '-c', guard, str(group), str(inode), *sys.argv[1:]])
    try:
        mover = subprocess.run([
            'docker', 'run', '--rm', '--network=none', '--read-only', '--pid=host', '--cgroupns=host',
            '--cap-drop=ALL', '--cap-add=DAC_OVERRIDE', '--mount',
            'type=bind,src='+str(group/'cgroup.procs')+',dst=/enroll', info['Config']['Image'],
            '/bin/sh', '-c', 'printf "%s\\n" "$1" > /enroll', 'enroll', str(child.pid)
        ], timeout=15, capture_output=True, text=True)
        if mover.returncode:
            raise RuntimeError(mover.stderr.strip() or 'cgroup enrollment failed')
        raise SystemExit(child.wait())
    finally:
        if child.poll() is None:
            child.terminate()
            child.wait()


if __name__ == '__main__':
    main()
