"""Read-only admission proof for maintenance tools running below a live owner.

Environment variables locate the evidence, but do not establish admission.
Validation also requires the actual child cgroup, pinned original parent,
root-owned running receipt, live root ancestor, and the kernel's held flock.
This contract never grants Guest permissions or changes resource limits.
"""
import fcntl
import hashlib
import json
import os
from pathlib import Path
import re
import sys


def require(condition, message):
    if not condition:
        raise ValueError(message)


def text(path):
    return path.read_text().strip()


def root_file(path):
    require(not path.is_symlink() and path.is_file(), 'ordinary resource-owner evidence required')
    info = path.stat()
    require(info.st_uid == 0 and not info.st_mode & 0o022, 'root-owned non-writable resource evidence required')


def validate_resource_owner():
    require(sys.platform == 'linux', 'maintenance generation belongs to the admitted Linux owner')
    spelling = os.environ.get('UWVM_RESOURCE_RUN', '')
    native = spelling.startswith('/home/macromodel/Documents/uwvm-test-261018/agents/')
    require(native or spelling.startswith('/host/home/macromodel/Documents/uwvm-test-261018/agents/'),
        'the installed shared resource owner is required')
    root = Path(spelling)
    require(root.resolve(strict=True) == root and re.fullmatch(r'0[1-9]|10', root.parent.name), 'confined task run required')
    evidence = root / 'resource-evidence/result.json'
    root_file(evidence)
    report = json.loads(evidence.read_bytes())
    require(report['schema'] == 'uwvm-finite-resource-result-v1' and report['status'] == 'RUNNING',
        'live resource receipt required')
    require(report['root'] == spelling and report['task'] == root.parent.name and report['run_id'] == root.name,
        'resource receipt is for another run')
    infrastructure = root.parent.parent / 'director-20261021/infrastructure'
    policies = [Path(row['path']) for row in report['source_pins'] if Path(row['path']).name == 'resource-policy.json']
    require(len(policies) == 1 and policies[0].is_relative_to(infrastructure), 'one installed root-owned resource policy pin required')
    policy_path = policies[0]
    root_file(policy_path)
    policy = json.loads(policy_path.read_bytes())
    membership = text(Path('/proc/self/cgroup'))
    expected_scope = policy['parent_membership'] + '/uwvm-director-' + report['task'] + '-' + report['run_id']
    require(membership == '0::' + expected_scope and report['scope']['membership'] == expected_scope,
        'generator is outside its actual resource child')
    parent = Path(policy['host_root'] + '/sys/fs/cgroup' + policy['parent_membership'])
    scope = parent / expected_scope.rsplit('/', 1)[1]
    require([parent.stat().st_dev, parent.stat().st_ino] == policy['parent_identity'], 'original parent identity changed')
    require([scope.stat().st_dev, scope.stat().st_ino] == report['scope']['identity'], 'owned child identity changed')
    require(text(parent / 'memory.max') == str(64 << 30) and text(parent / 'memory.swap.max') == '0',
        'original shared hard64GiB/swap0 parent required')
    require(text(Path('/proc/sys/kernel/random/boot_id')) == policy['boot_id'], 'resource boot changed')
    reference = Path('/proc') / str(policy['reference_pid'])
    require(text(reference / 'cgroup') == '0::' + policy['parent_membership']
        and int(text(reference / 'stat').rsplit(')', 1)[1].split()[19]) == policy['reference_start_ticks'],
        'original reference process changed')
    for row in report['source_pins']:
        path = Path(row['path'])
        root_file(path)
        with path.open('rb') as stream:
            require(hashlib.file_digest(stream, 'sha256').hexdigest() == row['sha256'], 'resource owner source changed')
    owner_sources = {str(policy_path.with_name(name)).encode()
        for name in ('resource_owner.py', 'resource_staging_owner.py')}
    require(any(row['path'].encode() in owner_sources for row in report['source_pins']), 'installed resource supervisor source pin required')
    ancestor = os.getppid()
    supervisor = None
    for _ in range(128):
        if ancestor <= 1:
            break
        proc = Path('/proc') / str(ancestor)
        status = dict(line.split(':', 1) for line in text(proc / 'status').splitlines() if ':' in line)
        command = (proc / 'cmdline').read_bytes().split(b'\0')
        if (text(proc / 'cgroup') == '0::' + policy['parent_membership']
                and status['Uid'].split() == ['0'] * 4 and owner_sources.intersection(command)):
            supervisor = ancestor
            break
        ancestor = int(status['PPid'])
    require(supervisor is not None, 'the live root resource supervisor is not an ancestor')
    lock_path = infrastructure / 'locks/heavy.lock'
    root_file(lock_path)
    lock_info = lock_path.stat()
    held_by_supervisor = False
    for line in text(Path('/proc/locks')).splitlines():
        fields = line.split()
        if len(fields) < 8 or fields[1:4] != ['FLOCK', 'ADVISORY', 'WRITE']:
            continue
        device = fields[5].split(':')
        if (int(fields[4]) == supervisor and len(device) == 3 and int(device[2]) == lock_info.st_ino
                and int(device[0], 16) == os.major(lock_info.st_dev)
                and int(device[1], 16) == os.minor(lock_info.st_dev)):
            held_by_supervisor = True
    require(held_by_supervisor, 'the resource supervisor does not own the common heavy lock')
    with lock_path.open('rb') as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            pass
        else:
            fcntl.flock(lock, fcntl.LOCK_UN)
            raise ValueError('the common heavy lock is not held')
    return dict(run=str(root), evidence=str(evidence), supervisor_pid=supervisor,
        parent=str(parent), child=str(scope), default_authority=[])
