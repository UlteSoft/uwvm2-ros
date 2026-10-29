#!/usr/bin/env python3
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import subprocess
import stat
import sys
import tempfile

p = argparse.ArgumentParser()
p.add_argument('--suite', choices=['security', 'ipc', 'ipc-handshake', 'ipc-client-deadline', 'sorted-rows', 'sdk', 'unified-events',
    'unified-event-queue', 'unified-event-async', 'unified-event-manager', 'unified-event-server', 'all'], default='all')
p.add_argument('--out', type=Path, required=True)
p.add_argument('--require-exec-observation', action='store_true',
    help='On Linux, retain exact kernel exec-stop identity and exit evidence for every selected unit')
p.add_argument('--sdk-root', type=Path, default=os.environ.get('UWVM_ROS_SDK_ROOT'),
    help='Installed SDK containing src/gateway/common; required by the security suite')
a = p.parse_args()
root = Path(__file__).resolve().parents[5]
a.out.mkdir(parents=True, exist_ok=True)
rows = []

def dependency_inputs(path, target_name):
    """Retain every real compiler-reported source and system-header input."""
    raw = path.read_bytes()
    if len(raw) > 1048576:
        raise ValueError('compiler dependency file exceeds one MiB')
    target, separator, body = raw.decode('utf-8').replace('\\\n', '').partition(':')
    if separator != ':' or target.strip() != target_name:
        raise ValueError('compiler dependency target differs')
    names = shlex.split(body, comments=False, posix=True)
    if not names or len(names) > 4096 or len(set(names)) != len(names):
        raise ValueError('compiler dependency input census differs')
    rows = []
    for name in names:
        requested = Path(name)
        actual = (requested if requested.is_absolute() else root/requested).resolve(strict=True)
        with actual.open('rb') as stream:
            info = os.fstat(stream.fileno())
            if not stat.S_ISREG(info.st_mode) or info.st_size > 16*1024**2:
                raise ValueError('compiler dependency input is not a bounded regular file')
            checksum = hashlib.file_digest(stream, 'sha256').hexdigest()
        rows.append({'requested_path':name, 'path':str(actual), 'device':info.st_dev,
            'inode':info.st_ino, 'bytes':info.st_size, 'mode':stat.S_IMODE(info.st_mode),
            'sha256':checksum, 'open_mode':'rb'})
    return rows


def dependency_record(path, rows):
    data = (json.dumps({'format':'uwvm-compiler-dependency-inputs-v1', 'files':rows},
                       indent=2, sort_keys=True)+'\n').encode()
    path.write_bytes(data)
    return {'path':str(path.resolve()), 'bytes':len(data), 'sha256':hashlib.sha256(data).hexdigest()}

event_units = [('unified-event-queue', root/'test/0073.ros-tests/support/source/ros_unified_events/queue_contract.cc'),
               ('unified-event-async', root/'test/0073.ros-tests/support/source/ros_unified_events/async_contract.cc'),
               ('unified-event-manager', root/'test/0073.ros-tests/support/source/ros_unified_events/manager_contract.cc'),
               ('unified-event-server', root/'test/0073.ros-tests/support/source/ros_unified_events/server_contract.cc')]
for name, source in [('security', root/'test/0073.ros-tests/support/source/ros_security/capability_policy_test.cc'), ('ipc', root/'test/0073.ros-tests/support/source/ros_ipc/protocol_test.cc'), ('ipc-handshake', root/'test/0073.ros-tests/support/source/ros_ipc/client_handshake_contract.cc'), ('ipc-client-deadline', root/'test/0073.ros-tests/support/source/ros_ipc_client_deadline/unit.cc'), ('sorted-rows', root/'test/0073.ros-tests/support/source/ros_sorted_rows/unit.cc'), ('sdk', root/'test/0073.ros-tests/support/source/ros/sdk_codec.cpp'), *event_units]:
    if a.suite not in ('all', name) and not (a.suite=='ipc' and name in ('ipc-handshake','ipc-client-deadline')) and not (a.suite=='unified-events' and (name,source) in event_units):
        continue
    binary = a.out/name
    dependency_path = a.out/(name+'.d')
    flags = shlex.split(os.environ.get('ROS_TEST_CXXFLAGS', '-std=c++26 -O1 -g0'))
    compiler = shlex.split(os.environ.get('CXX', 'clang++'))
    fixture_inputs = []
    sdk_flags = []
    if name == 'security':
        sdk = a.sdk_root.resolve() if a.sdk_root is not None else None
        names = ('config.h', 'native_io.h')
        if sdk is None or any(not (sdk/'src/gateway/common'/part).is_file() for part in names):
            rows.append({'suite':name, 'status':'BLOCKED', 'stage':'fixture',
                'reason':'BLOCKED_INSTALLED_SDK_GATEWAY_HEADERS: pass --sdk-root or UWVM_ROS_SDK_ROOT',
                'compiled':False, 'executed':False})
            continue
        for part in names:
            path = sdk/'src/gateway/common'/part
            fixture_inputs.append({'path':str(path), 'bytes':path.stat().st_size,
                'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
        sdk_flags = ['-I', str(sdk/'src')]
    command = [*compiler, *flags, '-UNDEBUG', '-MD', '-MF', str(dependency_path), '-MT', name, *sdk_flags, *(['-I', str(root)] if name=='sorted-rows' else []), '-I', str(root/'src'), '-I', str(root/'third-parties/fast_io/include'), str(source), '-pthread', '-o', str(binary)]
    compiled = executed = False
    try:
        with (a.out/(name+'.build.log')).open('wb') as log:
            result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=180)
        if result.returncode:
            rows.append({'suite':name, 'status':'FAIL', 'stage':'build', 'command':command, 'returncode':result.returncode, 'fixture_inputs':fixture_inputs, 'compiled':False, 'executed':False})
            continue
        compiled = True
        dependency_bytes = dependency_path.read_bytes()
        dependency = {'path':str(dependency_path.resolve()), 'bytes':len(dependency_bytes),
            'sha256':hashlib.sha256(dependency_bytes).hexdigest()}
        dependency_before = dependency_inputs(dependency_path, name)
        dependency_before_record = dependency_record(a.out/(name+'.dependencies-before.json'), dependency_before)
        # Keep the real filesystem socket and its cleanup inside the selected
        # output. Choose a short --out path for Linux's 108-byte sun_path limit.
        with tempfile.TemporaryDirectory(prefix='ipc-',dir=a.out.resolve()) as temporary:
            environment={**os.environ,'TMPDIR':temporary}
            executed = True
            actual_exec = None
            if a.require_exec_observation:
                sys.path.insert(0, str(root/'tools/ci/linux_smoke'))
                from process_evidence import run_observed
                result, actual_exec = run_observed([str(binary.resolve())],
                    log=a.out/(name+'.log'), receipt=a.out/(name+'.actual-exec.json'),
                    timeout=45, env=environment)
            else:
                result = subprocess.run([str(binary.resolve())], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=45,env=environment)
        (a.out/(name+'.log')).write_bytes(result.stdout)
        stable = all(Path(row['path']).stat().st_size == row['bytes'] and
            hashlib.sha256(Path(row['path']).read_bytes()).hexdigest() == row['sha256'] for row in fixture_inputs)
        dependency_after = dependency_inputs(dependency_path, name)
        dependency_after_record = dependency_record(a.out/(name+'.dependencies-after.json'), dependency_after)
        dependency_unchanged = dependency_path.read_bytes() == dependency_bytes and dependency_before == dependency_after
        rows.append({'suite':name, 'status':'PASS' if result.returncode==0 and stable and dependency_unchanged else 'BLOCKED' if name=='ipc-handshake' and result.returncode==77 else 'FAIL',
            'returncode':result.returncode, 'compiled':True, 'executed':True, 'command':command,
            'actual_exec':actual_exec,
            'fixture_inputs':fixture_inputs, 'fixture_inputs_unchanged':stable,
            'compiler_dependency_file':dependency, 'compiler_dependency_file_unchanged':dependency_unchanged,
            'compiler_dependency_inputs_before':dependency_before_record,
            'compiler_dependency_inputs_after':dependency_after_record,
            'compiler_dependency_inputs_unchanged':dependency_unchanged})
    except RuntimeError as error:
        rows.append({'suite':name, 'status':'FAIL', 'reason':str(error), 'compiled':compiled, 'executed':executed, 'command':command})
    except FileNotFoundError as error:
        rows.append({'suite':name, 'status':'BLOCKED', 'reason':str(error), 'compiled':compiled, 'executed':executed, 'command':command})
    except OSError as error:
        rows.append({'suite':name, 'status':'FAIL', 'reason':str(error), 'compiled':compiled, 'executed':executed, 'command':command})
    except subprocess.TimeoutExpired:
        rows.append({'suite':name, 'status':'FAIL', 'reason':'bounded test timeout', 'compiled':compiled, 'executed':executed, 'command':command})
status = 'PASS' if rows and all(r['status']=='PASS' for r in rows) else 'FAIL' if any(r['status']=='FAIL' for r in rows) else 'BLOCKED'
(a.out/'results.json').write_text(json.dumps({'status':status, 'suites':rows}, indent=2)+'\n')
for r in rows:
    print(r['status'], r['suite'])
sys.exit(0 if status=='PASS' else 77 if status=='BLOCKED' else 1)
