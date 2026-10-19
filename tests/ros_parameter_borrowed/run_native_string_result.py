#!/usr/bin/env python3
"""Fresh scalar String native borrowed-return measurements; never changes baseline."""
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
doc = {'status': 'FAIL', 'scope': 'Native ParameterValue and generated ParameterValue return helper, GCC13 CPP17/CPP20 normal+SAN; no ROS network or Worker/A15 claim',
       'phase': a.phase, 'argv': sys.argv, 'workspace': str(W), 'executed_driver_sha256': sha(__file__), 'commands': [], 'cgroup_before': before,
       'baseline_source_inputs': source_before, 'baseline_18_native_binaries': binaries_before, 'baseline_report_sha256': report_before}
shutil.copy2(__file__, W/'executed-driver.py'); mounted = False; children = []

def native(argv):
    return ['/usr/bin/chroot', str(R), '/usr/bin/env', '-u', 'CC', '-u', 'CXX', '-u', 'LD_LIBRARY_PATH',
            'PYTHONDONTWRITEBYTECODE=1', 'ROS_DOMAIN_ID=197', 'RMW_IMPLEMENTATION=rmw_fastrtps_cpp', 'ROS_SECURITY_ENABLE=false',
            'ROS_LOG_DIR=/tmp/ros-logs', '/bin/bash', '-lc', 'source /opt/ros/jazzy/setup.bash && exec "$@"', 'borrowed-string-result', *argv]

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
        if artifact and 'actual_main' not in row:
            pending = [(child.pid, [child.pid])]; seen = set()
            while pending:
                pid, ancestry = pending.pop()
                if pid in seen: continue
                seen.add(pid)
                try:
                    pending += [(int(value), ancestry+[int(value)]) for value in Path('/proc', str(pid), 'task', str(pid), 'children').read_text().split()]
                    fd = os.open(Path('/proc', str(pid), 'exe'), os.O_RDONLY)
                    with os.fdopen(fd, 'rb') as file:
                        actual = os.fstat(file.fileno()); wanted = artifact.stat()
                        if (actual.st_dev, actual.st_ino) != (wanted.st_dev, wanted.st_ino): continue
                        live = live_cgroup(pid); parent = int(next(line.split()[1] for line in Path('/proc', str(pid), 'status').read_text().splitlines() if line.startswith('PPid:')))
                        exe = os.readlink(Path('/proc', str(pid), 'exe')); captured = time.monotonic_ns()
                        assert live['status'] == 'PASS' and live['membership'] == before['membership'] and live['live_cgroup_procs']
                        assert parent == (os.getpid() if len(ancestry) == 1 else ancestry[-2])
                        digest = hashlib.sha256()
                        for block in iter(lambda: file.read(1048576), b''): digest.update(block)
                        assert digest.hexdigest() == sha(artifact)
                        row['actual_main'] = {**live, 'ancestry': ancestry, 'parent_pid': parent, 'exe': exe, 'device': actual.st_dev, 'inode': actual.st_ino,
                                              'sha256': digest.hexdigest(), 'live_identity_captured_before_fd_hash': True, 'capture_monotonic_ns': captured}
                except (FileNotFoundError, ProcessLookupError): pass
                except PermissionError as error: row.setdefault('sampling_unavailable', []).append({'pid': pid, 'reason': str(error)})
        if time.monotonic() > until: raise TimeoutError(name)
        time.sleep(.005)
    row.update(returncode=child.returncode, log=str(log), log_sha256=sha(log), preexec_sha256=sha(pre))
    if artifact: assert 'actual_main' in row, 'missing actual main ELF proof '+name
    text = log.read_text()
    if expected:
        assert row['returncode'] == expected and 'expired service respond budget accepted native response' in text and 'OBSERVED service.respond ' in text, (name, text)
        row['semantic_expected_failure'] = True
    else: assert row['returncode'] == 0, (name, text)
    row['status'] = 'EXPECTED_SEMANTIC_FAIL' if expected else 'PASS'
    save(P/'progress.json', doc)

try:
    shutil.copytree(BASE/'source', W/'source', symlinks=True)
    replacements = ['tests/ros_parameter_borrowed/native_string_result_test.cc']
    if a.phase == 'after': replacements += ['src/uwvm2/imported/ros/ipc/parameter.h', 'gateways/ros2/resources.h', 'gateways/common/config.h']
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
    doc['profiles'] = []
    for standard in ('gnu++17','gnu++20'):
        for sanitizer in (False,True):
            name = standard.replace('+','p') + ('-san' if sanitizer else '-normal')
            options = ['-fsanitize=address,undefined','-fno-omit-frame-pointer'] if sanitizer else []
            if a.phase == 'after': options += ['-DUWVM_ROS_BORROWED_STRING_OPTIMIZED']
            compile = ['/usr/bin/c++', *flags['CXX_DEFINES'], *flags['CXX_INCLUDES'], '-I/work/third-parties/fast_io/include', '-std='+standard,
                       '-O1','-g0', *options, '-MMD','-MF','/tmp/'+name+'.d','-c','/work/tests/ros_parameter_borrowed/native_string_result_test.cc','-o','/tmp/'+name+'.o']
            link = shlex.split((meta/'link.txt').read_text())
            link[link.index('CMakeFiles/uwvm-ros2-gateway.dir/main.cc.o')] = '/tmp/'+name+'.o'; link[link.index('-o')+1] = '/tmp/'+name
            link += options
            run(name+'-compile',native(compile),timeout=180);run(name+'-link',native(link),timeout=120)
            dependency=T/(name+'.d');values={}
            for token in re.findall(r'/work/[^\s\\]+',dependency.read_text().replace('\\\n',' ')):
                relative=os.path.normpath(token)[6:];assert relative in frozen
                values[relative]=sha(W/'source'/relative);assert values[relative]==frozen[relative]
            assert values
            run(name+'-native',native(['/tmp/'+name]),artifact=T/name,timeout=15)
            text=(P/(name+'-native.log')).read_text()
            assert 'PASS borrowed native String checks calls=' in text
            observations={}
            for line in text.splitlines():
                match=re.fullmatch(r'([a-zA-Z0-9-]+) calls=(\d+) bytes=(\d+)',line)
                if match:observations[match[1]]={'calls':int(match[2]),'bytes':int(match[3])}
            assert len(observations)==12
            doc['profiles'].append({'standard':standard,'sanitizer':sanitizer,'binary_sha256':sha(T/name),'actual_allocation_observations':observations,
                'raw_MMD':{'path':str(dependency),'bytes':dependency.stat().st_size,'sha256':sha(dependency),'project_inputs':values}})
    doc['status'] = 'PASS'
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
