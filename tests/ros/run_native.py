#!/usr/bin/env python3
"""Actual ROS Gateway, resource, TF, Lifecycle, Action and SROS2 witnesses."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import signal
import socket
import subprocess
import sys
import time


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--workspace', type=Path, required=True)
    p.add_argument('--suite', choices=['all', 'topics', 'resources', 'tf', 'lifecycle', 'secure', 'action', 'performance'], default='all')
    p.add_argument('--out', type=Path, required=True)
    a = p.parse_args()
    a.out.mkdir(parents=True, exist_ok=True)
    roots = {1:a.workspace/'gateway/noetic-rootfs', 2:a.workspace/'gateway/jazzy-rootfs'}
    processes = []
    rows = []
    commands = []
    secret_dir = None
    snapshots = {}
    env = os.environ.copy()
    env.update(ROS_MASTER_URI='http://127.0.0.1:11359', ROS_IP='127.0.0.1', ROS_DOMAIN_ID='75', RMW_IMPLEMENTATION='rmw_fastrtps_cpp', ROS_SECURITY_ENABLE='false')

    def command(version, args):
        distro = 'noetic' if version == 1 else 'jazzy'
        return ['chroot', str(roots[version]), '/bin/bash', '-lc',
                f'source /opt/ros/{distro}/setup.bash && exec "$@"', 'uwvm-native-test', *args]

    def frozen(version, path):
        key=f'ros{version}:'+path
        if key in snapshots:return snapshots[key]['inside']
        source=roots[version]/path.lstrip('/')
        inside=f'/tmp/uwvm-native-snapshot-{os.getpid()}/'+Path(path).name
        destination=roots[version]/inside.lstrip('/')
        destination.parent.mkdir(mode=0o700,exist_ok=True)
        before=source.stat();shutil.copy2(source,destination)
        digest=hashlib.sha256(destination.read_bytes()).hexdigest();after=source.stat()
        if (before.st_size,before.st_mtime_ns)!=(after.st_size,after.st_mtime_ns) or hashlib.sha256(source.read_bytes()).hexdigest()!=digest:
            raise RuntimeError('FAIL native executable changed during immutable snapshot')
        snapshots[key]={'source':str(source),'snapshot':str(destination),'inside':inside,'sha256':digest}
        return inside

    def start(cmd, name):
        commands.append(cmd)
        with (a.out/(name+'.log')).open('wb') as log:
            proc = subprocess.Popen(cmd, stdout=log, stderr=subprocess.STDOUT, env=env, start_new_session=True)
        processes.append(proc)
        return proc

    def stop(proc):
        if proc.poll() is None:
            os.killpg(proc.pid, signal.SIGTERM)
            try:
                proc.wait(timeout=8)
            except subprocess.TimeoutExpired:
                os.killpg(proc.pid, signal.SIGKILL)
                proc.wait(timeout=5)

    def execute(cmd, name, timeout=45):
        proc = start(cmd, name)
        membership = None
        observed = {}
        try:
            membership = Path(f'/proc/{proc.pid}/cgroup').read_text().splitlines()
            deadline = time.monotonic()+timeout
            while proc.poll() is None:
                pending = [proc.pid]
                seen = set()
                while pending:
                    pid = pending.pop()
                    if pid in seen:
                        continue
                    seen.add(pid)
                    try:
                        pending.extend(map(int, Path(f'/proc/{pid}/task/{pid}/children').read_text().split()))
                        observed[pid] = {'pid':pid, 'membership':Path(f'/proc/{pid}/cgroup').read_text().splitlines(),
                                         'command':Path(f'/proc/{pid}/cmdline').read_bytes().replace(b'\0',b' ').decode(errors='replace')}
                    except OSError:
                        pass
                if time.monotonic()>=deadline:
                    raise subprocess.TimeoutExpired(cmd,timeout)
                time.sleep(.01)
            code = proc.returncode
            row = {'test':name, 'status':'PASS' if code == 0 else 'FAIL', 'returncode':code,
                   'pid':proc.pid, 'membership':membership, 'actual_process_tree':list(observed.values())}
        except subprocess.TimeoutExpired:
            stop(proc)
            row = {'test':name, 'status':'FAIL', 'reason':'bounded native witness timeout', 'pid':proc.pid, 'membership':membership}
        rows.append(row)
        print(row['status'], name, flush=True)
        return row['status'] == 'PASS'

    def interrupted(signum, frame):
        raise RuntimeError('FAIL native witness interrupted; cleaning task processes')

    signal.signal(signal.SIGTERM, interrupted)
    signal.signal(signal.SIGINT, interrupted)
    report = {'status':'BLOCKED', 'results':rows, 'binary_snapshots':snapshots}
    try:
        sys.path.insert(0, str(Path(__file__).resolve().parents[1]/'ros_cgroup'))
        from probe import probe
        report['cgroup_before'] = probe()
        if report['cgroup_before']['status'] != 'PASS':
            raise RuntimeError('BLOCKED live membership in provided 64 GiB cgroup required')
        if a.suite in ('all', 'topics', 'resources', 'tf', 'action', 'performance'):
            master = start(command(1, ['roscore', '-p', '11359']), 'roscore')
            for _ in range(200):
                if master.poll() is not None:
                    raise RuntimeError('FAIL ROS1 Master exited')
                try:
                    with socket.create_connection(('127.0.0.1', 11359), .1):
                        break
                except OSError:
                    time.sleep(.025)
            else:
                raise RuntimeError('FAIL ROS1 Master startup timeout')
        tests = [('topics', 1, 'native'), ('topics', 2, 'native'),
                 ('resources', 1, 'resources'), ('resources', 2, 'resources'),
                 ('tf', 1, 'tf'), ('tf', 2, 'tf'),
                 ('lifecycle', 2, 'lifecycle'), ('action', 1, 'action'), ('action', 2, 'action'),
                 ('performance', 1, 'ipc-service'), ('performance', 2, 'ipc-service')]
        for suite, version, kind in tests:
            if a.suite not in ('all', suite):
                continue
            name = f'ros{version}-{kind}'
            binary = f'/tmp/ros{version}-build/uwvm-ros{version}-{kind}-probe'
            if not (roots[version]/binary.lstrip('/')).exists():
                rows.append({'test':name, 'status':'BLOCKED', 'reason':'missing actual native binary: '+binary})
                continue
            execute(command(version, [frozen(version,binary), frozen(version,f'/tmp/ros{version}-build/uwvm-ros{version}-gateway')]), name, timeout=90 if suite=='resources' else 45)
        if a.suite in ('all', 'secure'):
            binary = '/tmp/ros2-build/uwvm-ros2-secure-probe'
            if not (roots[2]/binary.lstrip('/')).exists():
                rows.append({'test':'sros2-enforce', 'status':'BLOCKED', 'reason':'missing secure native witness'})
            else:
                inside = '/tmp/uwvm-sros2-witness-'+str(os.getpid())
                secret_dir = roots[2]/inside.lstrip('/')
                secret_dir.mkdir(mode=0o700)
                keys = inside+'/keys'
                # SROS2 signs permissions for the selected domain and RMW graph
                # discovery topic. Generation must use the witness's exact settings.
                env.update(ROS_DOMAIN_ID='74', RMW_IMPLEMENTATION='rmw_fastrtps_cpp')
                generated = execute(command(2, ['ros2', 'security', 'generate_artifacts', '-k', keys,
                                             '-p', '/work/tests/ros_security/sros2-policy.xml']), 'sros2-generate', 60)
                if generated:
                    # Gateway intentionally accepts owned regular credentials. Materialize SROS2's
                    # public symlinks only within this freshly generated private keystore.
                    for enclave in (secret_dir/'keys/enclaves').rglob('*'):
                        if enclave.is_symlink():
                            target = enclave.resolve()
                            if not target.is_relative_to((secret_dir/'keys').resolve()):
                                raise RuntimeError('FAIL credential link escapes generated keystore')
                            data = target.read_bytes()
                            enclave.unlink()
                            enclave.write_bytes(data)
                    for key in (secret_dir/'keys').rglob('*.pem'):
                        if 'key' in key.name:
                            key.chmod(0o600)
                    execute(command(2, [frozen(2,binary), frozen(2,'/tmp/ros2-build/uwvm-ros2-gateway'), keys]), 'sros2-enforce')
        report['cgroup_after'] = probe()
        if report['cgroup_after']['status'] != 'PASS':
            rows.append({'test':'cgroup-after', 'status':'FAIL', 'reason':'lost required membership'})
        before = report['cgroup_before']['cgroup_ancestors'][0]['memory.events']
        after = report['cgroup_after']['cgroup_ancestors'][0]['memory.events']
        def events(value):
            return dict(line.split() for line in value.splitlines())
        if any(events(before).get(key) != events(after).get(key) for key in ('oom', 'oom_kill')):
            rows.append({'test':'memory-events', 'status':'FAIL', 'reason':'OOM counters increased'})
        report['status'] = 'FAIL' if any(r['status']=='FAIL' for r in rows) else 'BLOCKED' if not rows or any(r['status']=='BLOCKED' for r in rows) else 'PASS'
    except (RuntimeError, OSError, subprocess.SubprocessError) as error:
        report.update(status='BLOCKED' if str(error).startswith('BLOCKED') else 'FAIL', error=str(error))
    finally:
        for proc in reversed(processes):
            stop(proc)
        if secret_dir is not None:
            shutil.rmtree(secret_dir)
        (a.out/'commands.json').write_text(json.dumps(commands, indent=2)+'\n')
        (a.out/'report.json').write_text(json.dumps(report, indent=2)+'\n')
    print(report['status'], a.out/'report.json')
    return 0 if report['status'] == 'PASS' else 77 if report['status'] == 'BLOCKED' else 1


if __name__ == '__main__':
    sys.exit(main())
