#!/usr/bin/env python3
import argparse
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys
import tempfile

p = argparse.ArgumentParser()
p.add_argument('--suite', choices=['security', 'ipc', 'ipc-handshake', 'ipc-client-deadline', 'sorted-rows', 'sdk', 'unified-events',
    'unified-event-queue', 'unified-event-async', 'unified-event-manager', 'unified-event-server', 'all'], default='all')
p.add_argument('--out', type=Path, required=True)
a = p.parse_args()
root = Path(__file__).resolve().parents[5]
a.out.mkdir(parents=True, exist_ok=True)
rows = []
event_units = [('unified-event-queue', root/'test/0073.ros-tests/support/source/ros_unified_events/queue_contract.cc'),
               ('unified-event-async', root/'test/0073.ros-tests/support/source/ros_unified_events/async_contract.cc'),
               ('unified-event-manager', root/'test/0073.ros-tests/support/source/ros_unified_events/manager_contract.cc'),
               ('unified-event-server', root/'test/0073.ros-tests/support/source/ros_unified_events/server_contract.cc')]
for name, source in [('security', root/'test/0073.ros-tests/support/source/ros_security/capability_policy_test.cc'), ('ipc', root/'test/0073.ros-tests/support/source/ros_ipc/protocol_test.cc'), ('ipc-handshake', root/'test/0073.ros-tests/support/source/ros_ipc/client_handshake_contract.cc'), ('ipc-client-deadline', root/'test/0073.ros-tests/support/source/ros_ipc_client_deadline/unit.cc'), ('sorted-rows', root/'test/0073.ros-tests/support/source/ros_sorted_rows/unit.cc'), ('sdk', root/'test/0073.ros-tests/support/source/ros/sdk_codec.cpp'), *event_units]:
    if a.suite not in ('all', name) and not (a.suite=='ipc' and name in ('ipc-handshake','ipc-client-deadline')) and not (a.suite=='unified-events' and (name,source) in event_units):
        continue
    binary = a.out/name
    flags = shlex.split(os.environ.get('ROS_TEST_CXXFLAGS', '-std=c++26 -O1 -g0'))
    compiler = shlex.split(os.environ.get('CXX', 'clang++'))
    command = [*compiler, *flags, '-UNDEBUG', *(['-I', str(root)] if name=='sorted-rows' else []), '-I', str(root/'src'), '-I', str(root/'third-parties/fast_io/include'), str(source), '-pthread', '-o', str(binary)]
    try:
        with (a.out/(name+'.build.log')).open('wb') as log:
            result = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT, timeout=180)
        if result.returncode:
            rows.append({'suite':name, 'status':'FAIL', 'stage':'build', 'command':command, 'returncode':result.returncode})
            continue
        # Keep the real filesystem socket and its cleanup inside the selected
        # output. Choose a short --out path for Linux's 108-byte sun_path limit.
        with tempfile.TemporaryDirectory(prefix='ipc-',dir=a.out.resolve()) as temporary:
            environment={**os.environ,'TMPDIR':temporary}
            result = subprocess.run([str(binary.resolve())], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=45,env=environment)
        (a.out/(name+'.log')).write_bytes(result.stdout)
        rows.append({'suite':name, 'status':'PASS' if result.returncode==0 else 'BLOCKED' if name=='ipc-handshake' and result.returncode==77 else 'FAIL', 'returncode':result.returncode})
    except FileNotFoundError as error:
        rows.append({'suite':name, 'status':'BLOCKED', 'reason':str(error)})
    except subprocess.TimeoutExpired:
        rows.append({'suite':name, 'status':'FAIL', 'reason':'bounded test timeout'})
status = 'PASS' if rows and all(r['status']=='PASS' for r in rows) else 'FAIL' if any(r['status']=='FAIL' for r in rows) else 'BLOCKED'
(a.out/'results.json').write_text(json.dumps({'status':status, 'suites':rows}, indent=2)+'\n')
for r in rows:
    print(r['status'], r['suite'])
sys.exit(0 if status=='PASS' else 77 if status=='BLOCKED' else 1)
