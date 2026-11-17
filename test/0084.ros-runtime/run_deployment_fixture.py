#!/usr/bin/env python3
"""Exercise test-only deployment ownership with real competing processes.

These checks prove cooperative local ownership and durable fail-closed recovery,
not hardware stop, malicious-root exclusion or a production deployer. An optional
predelegated cgroup root tests distinct per-process memory/pids budgets while all
descendants remain under the existing 64 GiB gate. No sudo or fallback is used.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
from deployment_fixture import DeviceLease, OwnershipConflict, ReconciliationRequired, DelegatedCgroups, GATE, check_gate


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--delegated-root', type=Path)
    args = parser.parse_args()
    check_gate()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    registry = out/'registry'
    registry.mkdir(mode=0o700)
    resource = 'ab'*32
    owner = 'cd'*16
    report = dict(complete=False, scope=__doc__, gate=str(GATE), cases=[],
                  independent_cgroups_tested=False, malicious_device_access_tested=False,
                  physical_stop_tested=False, events_before=(GATE/'memory.events').read_text())
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    def passed(name):
        report['cases'].append(dict(name=name, passed=True)); save(); print('PASS '+name, flush=True)
    def refused(kind, callback):
        try:
            callback()
        except kind:
            return
        raise AssertionError('expected '+kind.__name__)
    child = None
    try:
        with DeviceLease(registry, resource) as held:
            assert held.acquire(owner) == 1
            refused(OwnershipConflict, lambda: DeviceLease(registry, resource))
            child_code = '''from deployment_fixture import *
import sys
check_gate()
try: DeviceLease(Path(sys.argv[1]),sys.argv[2])
except OwnershipConflict: print('REFUSED');sys.exit(0)
sys.exit(1)
'''
            result = subprocess.run([sys.executable, '-c', child_code, str(registry), resource],
                                    cwd=Path(__file__).parent, capture_output=True, text=True, timeout=10)
            assert result.returncode == 0 and result.stdout.strip() == 'REFUSED', result.stderr
            passed('same-process-and-separate-deployer-conflict')
            refused(ReconciliationRequired, lambda: held.reconcile_for_test('', 'effect'))
            refused(ReconciliationRequired, lambda: held.reconcile_for_test('stop', ''))
            passed('partial-evidence-never-unlocks')
        with DeviceLease(registry, resource) as next_owner:
            refused(ReconciliationRequired, lambda: next_owner.acquire('12'*16))
            next_owner.reconcile_for_test('simulator-stopped', 'all-simulated-effects-resolved')
            assert next_owner.acquire('12'*16) == 2
            next_owner.reconcile_for_test('simulator-stopped', 'all-simulated-effects-resolved')
        passed('disconnect-retains-uncertainty-and-rearm-advances-epoch')

        crash_code = '''from deployment_fixture import *
import sys
check_gate()
lease=DeviceLease(Path(sys.argv[1]),sys.argv[2]);lease.acquire('34'*16)
print('ACTIVE',flush=True)
os._exit(42)
'''
        child = subprocess.Popen([sys.executable, '-c', crash_code, str(registry), resource],
                                 cwd=Path(__file__).parent, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = child.communicate(timeout=10)
        assert child.returncode == 42 and stdout.strip() == b'ACTIVE', stderr
        with DeviceLease(registry, resource) as after_crash:
            refused(ReconciliationRequired, lambda: after_crash.acquire('56'*16))
        passed('crashed-deployer-does-not-release-device-authority')

        with DeviceLease(registry, 'ef'*32) as independent:
            assert independent.acquire(owner) == 1
            independent.reconcile_for_test('simulator-stopped', 'all-simulated-effects-resolved')
        passed('independent-resource-remains-available')
        symlink = registry/('01'*32)
        symlink.symlink_to(registry/resource)
        refused(OSError, lambda: DeviceLease(registry, '01'*32))
        os.link(registry/resource, registry/('02'*32))
        refused(PermissionError, lambda: DeviceLease(registry, resource))
        (registry/('02'*32)).unlink()
        passed('symlink-and-hardlink-record-aliases-refused')
        corrupt = registry/('03'*32)
        corrupt.write_text('{"partial":')
        corrupt.chmod(0o600)
        refused(ReconciliationRequired, lambda: DeviceLease(registry, '03'*32))
        passed('torn-journal-refuses-recovery')

        if args.delegated_root:
            groups = DelegatedCgroups(args.delegated_root)
            children = []
            try:
                for label, memory in [('vm-first', 268435456), ('vm-second', 268435456), ('driver-shared', 134217728)]:
                    leaf = groups.create(label, memory, 16)
                    child = subprocess.Popen([sys.executable, '-c',
                        'import sys;from pathlib import Path;sys.stdin.buffer.read(1);print(Path("/proc/self/cgroup").read_text().strip())'],
                        stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                    children.append(child)
                    groups.enroll(child, leaf)
                    stdout, stderr = child.communicate(b'R', timeout=10)
                    assert child.returncode == 0 and stdout.decode().strip() == '0::/'+str(leaf.relative_to('/sys/fs/cgroup')), stderr
                    assert (leaf/'memory.max').read_text().strip() == str(memory)
                assert len({p.stat().st_ino for p in groups.leaves}) == 3
                report['independent_cgroups_tested'] = True
                passed('distinct-vm-and-driver-cgroups-with-finite-budgets')
            finally:
                for child in children:
                    if child.poll() is None:
                        child.kill(); child.wait()
                groups.close()
        else:
            report['cgroup_gap'] = 'No delegated subtree supplied. Per-process cgroup budgets NOT TESTED; all cases used the shared 64 GiB gate.'
        report['inputs'] = {str(p): hashlib.sha256(p.read_bytes()).hexdigest()
                            for p in (Path(__file__), Path(__file__).with_name('deployment_fixture.py'))}
        report['complete'] = True
    finally:
        if child is not None and child.poll() is None:
            child.kill(); child.wait()
        report['events_after'] = (GATE/'memory.events').read_text()
        save()


if __name__ == '__main__':
    main()
