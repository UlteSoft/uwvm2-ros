#!/usr/bin/env python3
"""Fresh ROS2 Action advertiser/responder byte quota probes; never changes baseline evidence."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import signal
import subprocess
import sys
import time
import traceback

sys.dont_write_bytecode = True
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--workspace', type=Path, required=True)
p.add_argument('--baseline', type=Path, required=True)
p.add_argument('--sdk', type=Path, required=True)
p.add_argument('--inputs', type=Path, required=True)
p.add_argument('--phase', choices=('before', 'after'), required=True)
a = p.parse_args()
W, BASE, SDK, INPUTS = [value.resolve() for value in (a.workspace, a.baseline, a.sdk, a.inputs)]
assert not W.exists() and not W.is_relative_to(BASE) and not W.is_relative_to(SDK)
W.mkdir(); P = W/'reports'; P.mkdir(); T = W/'native-temp'; T.mkdir(); R = W/'rootfs'; R.mkdir()

def sha(path):
    h = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(1048576), b''): h.update(block)
    return h.hexdigest()

def inventory(root):
    return {str(path.relative_to(root)): sha(path) for path in root.rglob('*') if path.is_file()}

def save(path, value): path.write_text(json.dumps(value, indent=2)+'\n')

sys.path.insert(0, str(BASE/'source/tests/ros_cgroup'))
from probe import probe
before = probe(); assert before['status'] == 'PASS' and before['effective_memory_max'] == 68719476736
source_before, binaries_before, report_before = inventory(BASE/'source'), inventory(BASE/'native-binaries'), sha(BASE/'reports/report.json')
doc = {'status': 'FAIL', 'scope': 'Real DDS Action advertiser/responder canonical byte quotas, fresh GCC13 CPP20 fast_io probe and CPP17 Gateway, domain198; no Worker/A15 rerun',
       'phase': a.phase, 'argv': sys.argv, 'workspace': str(W), 'executed_driver_sha256': sha(__file__), 'commands': [], 'cgroup_before': before,
       'baseline_source_inputs': source_before, 'baseline_18_native_binaries': binaries_before, 'baseline_report_sha256': report_before}
shutil.copy2(__file__, W/'executed-driver.py'); mounted = False; children = []

def native(argv):
    return ['/usr/bin/chroot', str(R), '/usr/bin/env', '-u', 'CC', '-u', 'CXX', '-u', 'LD_LIBRARY_PATH',
            'PYTHONDONTWRITEBYTECODE=1', 'ROS_DOMAIN_ID=198', 'RMW_IMPLEMENTATION=rmw_fastrtps_cpp', 'ROS_SECURITY_ENABLE=false',
            'ROS_LOG_DIR=/tmp/ros-logs', '/bin/bash', '-lc', 'source /opt/ros/jazzy/setup.bash && exec "$@"', 'action-response-limits', *argv]

def live_cgroup(pid):
    facts = probe(); membership = Path('/proc', str(pid), 'cgroup').read_text().splitlines()
    assert facts['status'] == 'PASS' and membership == facts['membership'] == before['membership']
    live = str(pid) in (Path(facts['cgroup_ancestors'][0]['path'])/'cgroup.procs').read_text().split()
    assert live
    return {**facts, 'pid': pid, 'membership': membership, 'live_cgroup_procs': live,
            'ancestor_sampling': 'Driver and actual ELF PID proved live in exactly the same cgroup'}

def run(name, argv, expected=0, artifact=None, timeout=120):
    log = P/(name+'.log'); pre = P/(name+'-preexec.json'); row = {'name': name, 'argv': argv, 'expected_returncode': expected, 'status': 'FAIL'}; doc['commands'].append(row)
    def preexec(): save(pre, probe())
    with log.open('xb') as stream: child = subprocess.Popen(argv, stdout=stream, stderr=subprocess.STDOUT, preexec_fn=preexec, start_new_session=True)
    children.append(child); row['wrapper_pid'] = child.pid; until = time.monotonic()+timeout
    while child.poll() is None:
        if artifact and len(row.get('actual_mains', {})) < len(artifact):
            pending = [(child.pid, [child.pid])]; seen = set()
            while pending:
                pid, ancestry = pending.pop()
                if pid in seen: continue
                seen.add(pid)
                try:
                    pending += [(int(value), ancestry+[int(value)]) for value in Path('/proc', str(pid), 'task', str(pid), 'children').read_text().split()]
                    fd = os.open(Path('/proc', str(pid), 'exe'), os.O_RDONLY)
                    with os.fdopen(fd, 'rb') as file:
                        actual = os.fstat(file.fileno())
                        found = next((x for x in artifact if (x.stat().st_dev, x.stat().st_ino) == (actual.st_dev, actual.st_ino)), None)
                        if found is None or str(found) in row.get('actual_mains', {}): continue
                        live = live_cgroup(pid); parent = int(next(line.split()[1] for line in Path('/proc', str(pid), 'status').read_text().splitlines() if line.startswith('PPid:')))
                        exe = os.readlink(Path('/proc', str(pid), 'exe')); captured = time.monotonic_ns()
                        assert live['status'] == 'PASS' and live['membership'] == before['membership'] and live['live_cgroup_procs']
                        assert parent == (os.getpid() if len(ancestry) == 1 else ancestry[-2])
                        digest = hashlib.sha256()
                        for block in iter(lambda: file.read(1048576), b''): digest.update(block)
                        assert digest.hexdigest() == sha(found)
                        row.setdefault('actual_mains', {})[str(found)] = {**live, 'ancestry': ancestry, 'parent_pid': parent, 'exe': exe, 'device': actual.st_dev, 'inode': actual.st_ino,
                                              'sha256': digest.hexdigest(), 'live_identity_captured_before_fd_hash': True, 'capture_monotonic_ns': captured}
                except (FileNotFoundError, ProcessLookupError): pass
                except PermissionError as error: row.setdefault('sampling_unavailable', []).append({'pid': pid, 'reason': str(error)})
        if time.monotonic() > until: raise TimeoutError(name)
        time.sleep(.005)
    row.update(returncode=child.returncode, log=str(log), log_sha256=sha(log), preexec_sha256=sha(pre))
    if artifact: assert set(row.get('actual_mains', {})) == {str(x) for x in artifact}, 'missing probe/Gateway actual ELF proof '+name
    text = log.read_text()
    if expected:
        assert row['returncode'] == expected and 'advertiser byte quota accepted oversized native Action response' in text and 'OBSERVED action-limits ' in text, (name, text)
        row['semantic_expected_failure'] = True
    else: assert row['returncode'] == 0, (name, text)
    row['status'] = 'EXPECTED_SEMANTIC_FAIL' if expected else 'PASS'
    save(P/'progress.json', doc)

try:
    shutil.copytree(BASE/'source', W/'source', symlinks=True)
    replacements = ['tests/ros_action_limits/native_ros2.cc', 'gateways/ros2/action.h', 'src/uwvm2/imported/ros/ipc/unix.h', 'gateways/common/server.h', 'gateways/common/native_io.h', 'gateways/ros2/resources.h']
    doc['overlay_inputs'] = {}
    for name in replacements:
        target = W/'source'/name; target.parent.mkdir(parents=True, exist_ok=True); shutil.copy2(INPUTS/name, target); doc['overlay_inputs'][name] = sha(target)
    frozen = inventory(W/'source'); save(P/'source-before.json', frozen)
    subprocess.run(['mount', '--rbind', str(SDK/'gateway/jazzy-rootfs'), str(R)], check=True); mounted = True
    subprocess.run(['mount', '--make-rprivate', str(R)], check=True)
    subprocess.run(['mount', '--bind', str(W/'source'), str(R/'work')], check=True)
    subprocess.run(['mount', '-o', 'remount,bind,ro', str(R/'work')], check=True)
    subprocess.run(['mount', '--bind', str(T), str(R/'tmp')], check=True)
    meta = BASE/'native-temp/jazzy/ros2-next-r1-build/CMakeFiles/uwvm-ros2-gateway.dir'
    flags = {}
    for line in (meta/'flags.make').read_text().splitlines():
        if ' = ' in line:
            key, value = line.split(' = ', 1); flags[key] = shlex.split(value)
    for name in ('flags.make', 'link.txt'): shutil.copy2(meta/name, P/name)
    run('compiler-version', native(['/usr/bin/c++', '--version']))
    doc['raw_MMD'] = {}
    for label, source in (('gateway', 'gateways/ros2/main.cc'), ('native-action-limits', 'tests/ros_action_limits/native_ros2.cc')):
        compile = ['/usr/bin/c++', *flags['CXX_DEFINES'], *flags['CXX_INCLUDES'], *(['-I','/work/third-parties/fast_io/include'] if label!='gateway' else []), '-std=gnu++17' if label=='gateway' else '-std=gnu++20', '-O1', '-g0', '-MMD', '-MF', '/tmp/'+label+'.d', '-c', '/work/'+source, '-o', '/tmp/'+label+'.o']
        link = shlex.split((meta/'link.txt').read_text()); link[link.index('CMakeFiles/uwvm-ros2-gateway.dir/main.cc.o')] = '/tmp/'+label+'.o'; link[link.index('-o')+1] = '/tmp/'+label
        run(label+'-compile', native(compile), timeout=180); run(label+'-link', native(link))
        doc.setdefault('binary_sha256', {})[label] = sha(T/label)
        dependency = T/(label+'.d'); values = {}
        for token in re.findall(r'/work/[^\s\\]+', dependency.read_text().replace('\\\n', ' ')):
            name = os.path.normpath(token)[6:]; assert name in frozen; values[name] = sha(W/'source'/name); assert values[name] == frozen[name]
        doc['raw_MMD'][label] = {'path': str(dependency), 'bytes': dependency.stat().st_size, 'sha256': sha(dependency), 'project_inputs': values}; assert values
    (T/'ros-logs').mkdir()
    for mode in ('feedback','success','abort','canceled','feedback-edge','result-edge','responder-feedback','responder-result'):
        run('native-'+mode, native(['/tmp/native-action-limits', '/tmp/gateway', mode]), expected=1 if a.phase == 'before' and mode in ('feedback','success','abort','canceled') else 0, artifact=[T/'native-action-limits', T/'gateway'], timeout=30)
    doc['status'] = 'CONFIRMED_BEFORE_SEMANTIC_FAILURES' if a.phase == 'before' else 'PASS'
except BaseException:
    doc['error'] = traceback.format_exc()
finally:
    for child in children:
        if child.poll() is None:
            os.killpg(child.pid, signal.SIGKILL); child.wait(timeout=5)
    if mounted:
        attempts = []
        for _ in range(20):
            result = subprocess.run(['umount', '-R', str(R)], capture_output=True, text=True); attempts.append({'returncode': result.returncode, 'output': result.stdout+result.stderr})
            if result.returncode == 0 or 'target is busy' not in result.stderr: break
            time.sleep(.05)
        doc['mount_cleanup'] = attempts
    doc['remaining_owned_mounts'] = [line for line in Path('/proc/self/mountinfo').read_text().splitlines() if line.split()[4] == str(W) or line.split()[4].startswith(str(W)+'/')]
    owned, unavailable = [], []
    for path in Path('/proc').iterdir():
        if not path.name.isdigit(): continue
        try:
            exe = os.readlink(path/'exe')
            if exe.startswith(str(W)+'/'): owned.append({'pid': int(path.name), 'exe': exe})
        except OSError as error:
            if error.errno not in (2, 3): unavailable.append({'pid': int(path.name), 'errno': error.errno})
    doc.update(remaining_owned_processes=owned, unavailable_process_samples=unavailable, cgroup_after=probe())
    after = doc['cgroup_after']; assert after['status'] == 'PASS' and [row['path'] for row in before['cgroup_ancestors']] == [row['path'] for row in after['cgroup_ancestors']]
    deltas = []
    for old, new in zip(before['cgroup_ancestors'], after['cgroup_ancestors']):
        assert old['memory.max'] == new['memory.max'] and (old['memory.events'] is None) == (new['memory.events'] is None)
        if old['memory.events'] is None: continue
        events = lambda text: {key: int(value) for key, value in (line.split() for line in text.splitlines())}
        first, last = events(old['memory.events']), events(new['memory.events']); delta = {'path': old['path'], **{key: last[key]-first[key] for key in ('oom', 'oom_kill', 'oom_group_kill')}}; deltas.append(delta)
    doc['oom_deltas'] = deltas
    doc['baseline_preserved'] = {'source': inventory(BASE/'source') == source_before, '18_native_binaries': inventory(BASE/'native-binaries') == binaries_before, 'report': sha(BASE/'reports/report.json') == report_before}
    doc['source_preserved'] = inventory(W/'source') == frozen if 'frozen' in globals() else False
    if owned or doc['remaining_owned_mounts'] or not doc['source_preserved'] or not all(doc['baseline_preserved'].values()) or any(any(row[key] for key in ('oom', 'oom_kill', 'oom_group_kill')) for row in deltas) or mounted and doc['mount_cleanup'][-1]['returncode']:
        doc['status'] = 'FAIL'
    save(P/'report.json', doc)
    print(json.dumps({'status': doc['status'], 'report': str(P/'report.json'), 'sha256': sha(P/'report.json'), 'error': doc.get('error'),
                      'commands': [(row['name'], row['status'], row.get('returncode')) for row in doc['commands']], 'cleanup': doc.get('mount_cleanup'), 'owned': owned,
                      'mounts': doc['remaining_owned_mounts'], 'oom': deltas}, indent=2))
    raise SystemExit(0 if doc['status'] in ('PASS', 'CONFIRMED_BEFORE_SEMANTIC_FAILURES') else 1)
