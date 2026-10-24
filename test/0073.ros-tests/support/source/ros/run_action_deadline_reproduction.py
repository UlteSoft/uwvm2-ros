#!/usr/bin/env python3
"""Run the same incoming-Action deadline probe against frozen pre-fix headers.

Requires a completed PASS from run_ros2_followup.py. Run as root in the existing
64 GiB cgroup. --workspace must be fresh. Only its variant, scratch, reports and
private recursive rootfs mounts are written. Baseline/followup sources, their
native binaries and original rootfs symlinks are read-only inputs. The baseline
probe's original exit 1 is retained as FAIL(expected), never a normal test PASS.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import posixpath
import shlex
import shutil
import signal
import subprocess
import sys
import time


PROBE = 'test/0073.ros-tests/support/source/ros_action/deadline_native_ros2.cc'
ACTION = 'src/ros/gateway/ros2/action.h'
FAILURE = 'expired incoming respond accepted native success'
SUCCESS = 'PASS actual incoming Action accepted/event ticket before1500ms deadline'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def inventory(root):
    result = {}
    for path in sorted(root.rglob('*')):
        if path.is_symlink():
            raise RuntimeError('unexpected input symlink: ' + str(path))
        if path.is_file():
            result[str(path.relative_to(root))] = {'sha256': sha(path), 'bytes': path.stat().st_size}
    return result


def make_tokens(path):
    text = path.read_text().replace('\\\n', ' ')
    colon = next((i for i, c in enumerate(text) if c == ':' and (i == 0 or text[i - 1] != '\\')), -1)
    if colon < 0:
        raise RuntimeError('invalid compiler dependency file')
    return shlex.split(text[colon + 1:], comments=False, posix=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--baseline', type=Path, required=True)
    parser.add_argument('--followup', type=Path, required=True)
    parser.add_argument('--workspace', type=Path, required=True)
    parser.add_argument('--domain-id', type=int, default=89)
    args = parser.parse_args()
    base, follow = args.baseline.resolve(), args.followup.resolve()
    out = args.workspace.absolute()
    if os.geteuid() != 0 or sys.platform != 'linux' or out.exists() or not 0 <= args.domain_id <= 232:
        parser.error('Linux root, a fresh workspace and a valid isolated ROS domain are required')
    for protected in (base, follow/'source', base/'source', base/'gateway', follow/'gateway'):
        if out == protected or protected in out.parents:
            parser.error('workspace overlaps a protected input tree')
    sys.path.insert(0, str(base/'source/test/0073.ros-tests/support/source/ros_cgroup'))
    from probe import probe
    before = probe()
    if before['status'] != 'PASS':
        print('BLOCKED provided 64 GiB cgroup not proven')
        return 77
    reports, variant, scratch, root = out/'reports', out/'variant', out/'scratch', out/'rootfs'
    reports.mkdir(parents=True)
    children, mounts, rows = [], [], []
    report = {'status': 'FAIL', 'regression_proven': 'FAIL', 'scope': 'Same current probe, frozen pre-fix Action versus already-executed followup; not full acceptance',
              'cgroup_before': before, 'commands': rows, 'sanitizers': 'This native reproduction is not an instrumented ROS middleware run.'}
    report['executed_driver_sha256'] = sha(Path(__file__))
    follow_report = follow/'reports/report.json'
    source_roots = ('src/ros/gateway', 'test/0073.ros-tests/support/source/ros_action', 'test/0073.ros-tests/support/source/ros_ipc', 'src')
    wrapper = reports/'exec_in_cgroup.py'
    wrapper.write_text('''import json,os,sys
from pathlib import Path
proof=Path(sys.argv[1]);expected=json.loads(sys.argv[2]);command=sys.argv[3:]
membership=Path('/proc/self/cgroup').read_text().splitlines()
assert membership==expected
unified=next(line.split(':',2)[2] for line in membership if line.startswith('0::'))
actual=Path('/sys/fs/cgroup')/unified.lstrip('/')
assert str(os.getpid()) in (actual/'cgroup.procs').read_text().split()
proof.write_text(json.dumps({'pid':os.getpid(),'membership':membership,'live_cgroup_procs':True,'exec_command':command,
    'environment':{key:os.environ.get(key) for key in ('ROS_DOMAIN_ID','RMW_IMPLEMENTATION','ROS_SECURITY_ENABLE')}},indent=2)+'\\n')
os.execvpe(command[0],command,os.environ)
''')

    def run(command, name, timeout=30, expected=0, live_artifact=None):
        log_path, proof_path = reports/(name+'.log'), reports/(name+'-exec.json')
        row = {'name': name, 'command': list(map(str, command)), 'status': 'FAIL'}
        rows.append(row)
        env = os.environ.copy()
        env.update(ROS_DOMAIN_ID=str(args.domain_id), RMW_IMPLEMENTATION='rmw_fastrtps_cpp', ROS_SECURITY_ENABLE='false',
                   HOME='/tmp/home', ROS_LOG_DIR='/tmp/ros-logs', XDG_CONFIG_HOME='/tmp/config')
        full = [sys.executable, str(wrapper), str(proof_path), json.dumps(before['membership']), *map(str, command)]
        with log_path.open('wb') as log:
            child = subprocess.Popen(full, stdout=log, stderr=subprocess.STDOUT, start_new_session=True, env=env)
            children.append(child);row['pid'] = child.pid
            until = time.monotonic()+timeout
            try:
                while child.poll() is None:
                    if live_artifact is not None and 'actual_main_live_PID' not in row:
                        # uutils chroot forks the requested program. Keep the
                        # wrapper proof and inspect its real live descendants.
                        pending, seen = [(child.pid, [child.pid])], set()
                        while pending:
                            pid, ancestry = pending.pop()
                            if pid in seen:
                                continue
                            seen.add(pid)
                            try:
                                descendants=Path(f'/proc/{pid}/task/{pid}/children').read_text().split()
                                pending.extend((int(value),ancestry+[int(value)]) for value in descendants)
                                proc_exe=Path(f'/proc/{pid}/exe')
                                actual,wanted=proc_exe.stat(),live_artifact.stat()
                                parent=int(next(line.split()[1] for line in Path(f'/proc/{pid}/status').read_text().splitlines() if line.startswith('PPid:')))
                                observed={'pid':pid,'parent_pid':parent,'ancestry':ancestry,'exe_path':os.readlink(proc_exe),
                                          'exe_device':actual.st_dev,'exe_inode':actual.st_ino}
                                samples=row.setdefault('observed_execs', [])
                                if observed not in samples and len(samples)<32:
                                    samples.append(observed)
                                if (actual.st_dev,actual.st_ino)!=(wanted.st_dev,wanted.st_ino):
                                    continue
                                membership=Path(f'/proc/{pid}/cgroup').read_text().splitlines()
                                cgroup_path=Path(before['cgroup_ancestors'][0]['path'])
                                live=str(pid) in (cgroup_path/'cgroup.procs').read_text().splitlines()
                                if membership!=before['membership'] or not live or sha(proc_exe)!=sha(live_artifact) or (len(ancestry)>1 and parent!=ancestry[-2]):
                                    raise RuntimeError('native main PID/artifact/cgroup/ancestry mismatch')
                                row['actual_main_live_PID']={**observed,'membership':membership,'live_cgroup_procs':live,'exe_sha256':sha(proc_exe)}
                            except (FileNotFoundError,ProcessLookupError):
                                pass
                    if time.monotonic() >= until:
                        raise subprocess.TimeoutExpired(command, timeout)
                    time.sleep(0.01)
                code = child.wait()
            except BaseException:
                if child.poll() is None:
                    os.killpg(child.pid, signal.SIGKILL);child.wait(timeout=5)
                raise
        row.update(returncode=code, log_sha256=sha(log_path), log_path=str(log_path))
        if not proof_path.is_file():
            raise RuntimeError('missing pre-exec cgroup/PID proof for '+name)
        row['pre_exec_live_PID'] = json.loads(proof_path.read_text())
        if row['pre_exec_live_PID']['membership'] != before['membership'] or row['pre_exec_live_PID']['pid'] != child.pid:
            raise RuntimeError('pre-exec proof mismatch for '+name)
        row['status'] = 'PASS' if code == 0 else 'FAIL'
        if code != expected or live_artifact is not None and 'actual_main_live_PID' not in row:
            raise RuntimeError('unexpected exit or missing actual native main proof for '+name)
        return row

    def command(arguments):
        return ['chroot', str(root), '/usr/bin/env', '-u', 'CC', '-u', 'CXX', '-u', 'LD_LIBRARY_PATH',
                '/bin/bash', '-lc', 'source /opt/ros/jazzy/setup.bash && cd /tmp && exec "$@"', 'deadline-reproduction', *arguments]

    def interrupted(signum, frame):
        raise RuntimeError('interrupted; bounded owned process/mount cleanup follows')
    signal.signal(signal.SIGTERM, interrupted);signal.signal(signal.SIGINT, interrupted)
    try:
        completed = json.loads(follow_report.read_text())
        if completed['status'] != 'PASS':
            raise RuntimeError('followup must have completed PASS before reproduction')
        after_run = next(row for row in completed['commands'] if row['name'] == 'action-deadline-run')
        after_log = follow/'reports/action-deadline-run.log'
        if after_run['status'] != 'PASS' or after_run['returncode'] != 0 or sha(after_log) != after_run['log_sha256'] or SUCCESS not in after_log.read_text(errors='replace'):
            raise RuntimeError('missing actual candidate Action deadline PASS')
        candidate_probe_sha, candidate_action_sha = sha(follow/'source'/PROBE), sha(follow/'source'/ACTION)
        if completed['candidate_source_inputs'][PROBE] != candidate_probe_sha or completed['compiled_project_inputs'][ACTION] != candidate_action_sha:
            raise RuntimeError('candidate probe/Action bytes differ from its tested report')
        baseline_action_sha = sha(base/'source'/ACTION)
        if baseline_action_sha == candidate_action_sha:
            raise RuntimeError('baseline does not contain the pre-fix Action')
        report['after'] = {'status': 'PASS', 'returncode': 0, 'source_report': str(follow_report), 'source_report_sha256': sha(follow_report),
                           'probe_sha256': candidate_probe_sha, 'action_sha256': candidate_action_sha,
                           'actual_recorded_run': after_run, 'log_path': str(after_log), 'log_sha256': sha(after_log)}
        baseline_inputs = {part: inventory(base/'source'/part) for part in source_roots}
        candidate_inputs = {part: inventory(follow/'source'/part) for part in source_roots}
        if baseline_inputs['src'] != candidate_inputs['src']:
            raise RuntimeError('Worker src changed; this reproduction requires identical Worker inputs')
        report.update(baseline_source_inputs=baseline_inputs, candidate_source_inputs=candidate_inputs, worker_src_identical=True,
                      baseline_action={'path': str(base/'source'/ACTION), 'sha256': baseline_action_sha})
        previous_binaries = {str(path.relative_to(base)): sha(path) for path in (base/'gateway/jazzy-rootfs/tmp/ros2-build').glob('uwvm-ros2-*') if path.is_file()}
        if len(previous_binaries) != 10:
            raise RuntimeError('expected all 10 frozen Jazzy baseline binaries')
        report['baseline_native_binary_inputs'] = previous_binaries
        for part in source_roots:
            destination = variant/part;destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copytree(follow/'source'/part, destination)
        # Keep every other active compiler input identical to the successful
        # candidate; substitute only its pre-fix Action implementation.
        shutil.copy2(base/'source'/ACTION, variant/ACTION)
        if sha(variant/PROBE) != candidate_probe_sha or sha(variant/ACTION) != baseline_action_sha:
            raise RuntimeError('variant copy binding failed')
        report['variant_source_inputs'] = inventory(variant)
        scratch.mkdir();root.mkdir()
        for directory in ('home', 'ros-logs', 'config'):
            (scratch/directory).mkdir()
        provided = base/'gateway/jazzy-rootfs'
        run(['mount', '--rbind', str(provided), str(root)], 'rootfs-recursive-bind');mounts.append(root)
        run(['mount', '--make-rprivate', str(root)], 'rootfs-private-propagation')
        for name, source in (('work', variant), ('tmp', scratch)):
            target = root/name
            if not target.is_dir() or target.is_symlink():
                raise RuntimeError('provided rootfs lacks regular existing /'+name+' mountpoint')
            run(['mount', '--bind', str(source), str(target)], 'private-'+name+'-bind')
        # The supplied recursive rootfs clone must retain real proc/sys/dev;
        # require those mounts and actual cgroup/character-device visibility.
        lines = Path('/proc/self/mountinfo').read_text().splitlines()
        selected = [line for line in lines if line.split()[4] == str(root) or line.split()[4].startswith(str(root)+'/')]
        for child in ('proc', 'sys', 'dev'):
            if not any(line.split()[4] == str(root/child) for line in selected):
                raise RuntimeError('recursive clone lost /'+child+' mount')
        report['rootfs_mountinfo'] = selected
        flags_path = follow/'reports/build-metadata/ros2/CMakeFiles/uwvm-ros2-gateway.dir/flags.make'
        link_path = flags_path.with_name('link.txt')
        values = {line.split(' = ', 1)[0]: shlex.split(line.split(' = ', 1)[1]) for line in flags_path.read_text().splitlines() if ' = ' in line}
        link = shlex.split(link_path.read_text())
        for name, path in (('flags.make', flags_path), ('link.txt', link_path)):
            frozen = reports/(name+'-source.txt');shutil.copy2(path, frozen)
            report.setdefault('build_metadata', {})[name] = {'source_path': str(path), 'sha256': sha(path), 'frozen_path': str(frozen)}
        run(command(['/usr/bin/g++', '--version']), 'compiler-version')
        inside = '/tmp/action-deadline-before'
        run(command(['/usr/bin/g++', *values['CXX_DEFINES'], *values['CXX_INCLUDES'], '-std=c++17', '-O1', '-g0',
                     '-MMD', '-MF', inside+'.d', '-c', '/work/'+PROBE, '-o', inside+'.o']), 'before-build', 180)
        objects = [i for i, token in enumerate(link) if token.endswith('main.cc.o')]
        if len(objects) != 1 or link.count('-o') != 1:
            raise RuntimeError('unsupported recorded Gateway link layout')
        link[objects[0]] = inside+'.o';link[link.index('-o')+1] = inside
        run(command(link), 'before-link', 90)
        tokens = make_tokens(scratch/'action-deadline-before.d');dependencies = {}
        for token in tokens:
            normalized = posixpath.normpath(token)
            if not normalized.startswith('/work/'):
                raise RuntimeError('unexpected non-project MMD input '+token)
            relative = normalized[6:];path = variant/relative
            frozen = reports/'compiled-inputs'/relative;frozen.parent.mkdir(parents=True, exist_ok=True);shutil.copy2(path, frozen)
            actual_sha = sha(path)
            if relative != ACTION and actual_sha != sha(follow/'source'/relative):
                raise RuntimeError('another active compiler input differs from candidate: '+relative)
            dependencies[relative] = {'sha256': actual_sha, 'bytes': path.stat().st_size, 'frozen_path': str(frozen)}
        if PROBE not in dependencies or ACTION not in dependencies or dependencies[ACTION]['sha256'] != baseline_action_sha:
            raise RuntimeError('actual compiler did not consume intended probe/old Action')
        report['compiler_dependencies'] = {'tokens': tokens, 'token_count': len(tokens), 'unique_project_inputs': dependencies,
                                            'scope': 'Actual -MMD project dependencies; ROS system headers are described by recorded includes/link/package metadata, not this inventory.'}
        binary = scratch/'action-deadline-before'
        before_run = run(command([inside]), 'before-run', 20, expected=1, live_artifact=binary)
        raw_log = (reports/'before-run.log').read_text(errors='replace')
        if FAILURE not in raw_log or SUCCESS in raw_log:
            raise RuntimeError('baseline failed for a different reason or unexpectedly passed')
        report['before'] = {'status': 'FAIL', 'expected_failure': True, 'returncode': 1, 'failure_substring': FAILURE,
                            'action_sha256': baseline_action_sha, 'probe_sha256': candidate_probe_sha,
                            'binary_sha256': sha(binary), 'actual_run': before_run, 'log_path': str(reports/'before-run.log'), 'log_sha256': sha(reports/'before-run.log')}
        # Repeat the already-tested candidate binary with the same live native
        # PID/cgroup sampling as the old implementation, retaining both runs.
        candidate_binary=follow/'native-temp/jazzy/ros2-followup-build/action-deadline'
        candidate_binary_sha=sha(candidate_binary)
        repeated=scratch/'action-deadline-after';shutil.copy2(candidate_binary,repeated)
        if sha(repeated)!=candidate_binary_sha or sha(candidate_binary)!=candidate_binary_sha:
            raise RuntimeError('candidate executable changed during repeat snapshot')
        after_repeat=run(command(['/tmp/action-deadline-after']), 'after-repeat-run', 20, live_artifact=repeated)
        if SUCCESS not in (reports/'after-repeat-run.log').read_text() or sha(candidate_binary)!=candidate_binary_sha:
            raise RuntimeError('candidate repeat missing completion or executable changed')
        report['after'].update(actual_repeat_run=after_repeat,binary_sha256=candidate_binary_sha,
                               repeat_log_sha256=sha(reports/'after-repeat-run.log'))
        if inventory(variant) != report['variant_source_inputs']:
            raise RuntimeError('variant compiler inputs changed')
        if {part: inventory(base/'source'/part) for part in source_roots} != baseline_inputs or {part: inventory(follow/'source'/part) for part in source_roots} != candidate_inputs:
            raise RuntimeError('baseline or followup inputs changed')
        if any(sha(base/path) != value for path, value in previous_binaries.items()):
            raise RuntimeError('baseline native binary was modified')
        if sha(follow_report) != report['after']['source_report_sha256'] or sha(after_log) != report['after']['log_sha256']:
            raise RuntimeError('candidate evidence changed')
        report['status'] = report['regression_proven'] = 'PASS'
    except Exception as error:
        report['error'] = str(error);print('FAIL '+str(error), flush=True)
    finally:
        for child in reversed(children):
            try:
                os.killpg(child.pid, signal.SIGTERM)
            except ProcessLookupError:
                continue
            if child.poll() is None:
                try:
                    child.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    os.killpg(child.pid, signal.SIGKILL);child.wait(timeout=3)
            # Include descendants after their group leader exited.
            try:
                os.killpg(child.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
        cleanup = []
        for mount in reversed(mounts):
            try:
                result = subprocess.run(['umount', '-R', str(mount)], capture_output=True, timeout=15)
                cleanup.append({'mount': str(mount), 'returncode': result.returncode, 'stderr': result.stderr.decode(errors='replace')})
                if result.returncode:
                    report['status'] = report['regression_proven'] = 'FAIL'
            except Exception as error:
                cleanup.append({'mount': str(mount), 'error': str(error)});report['status'] = report['regression_proven'] = 'FAIL'
        report['mount_cleanup'] = cleanup
        after = probe();report['cgroup_after'] = after
        events = lambda value: dict(line.split() for line in value['cgroup_ancestors'][0]['memory.events'].splitlines())
        if after['status'] != 'PASS' or any(events(before).get(key) != events(after).get(key) for key in ('oom', 'oom_kill', 'oom_group_kill')):
            report['status'] = report['regression_proven'] = 'FAIL';report['cgroup_error'] = 'limit mismatch or OOM counter increased'
        (reports/'report.json').write_text(json.dumps(report, indent=2)+'\n')
        print(report['status']+' '+str(reports/'report.json'), flush=True)
    return 0 if report['status'] == 'PASS' else 1


if __name__ == '__main__':
    sys.exit(main())
