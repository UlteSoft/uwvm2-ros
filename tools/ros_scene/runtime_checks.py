#!/usr/bin/env python3
"""Run scene authority/resource safety tests in the existing 64 GiB cgroup.

Sanitizer tests use the host Clang; portability tests use GCC in the same
existing Docker resource gate. Only the new scene core is built. The tool
refuses execution outside the exact gate, records source hashes, and neither
changes cgroup limits nor configures native robot providers.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import time


ROOT = Path(__file__).resolve().parents[2]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--mode', choices=['sanitizer', 'gcc', 'thread', 'all'], default='all')
parser.add_argument('--container', default='uwvm-sdk-resource-gate-20261024')
parser.add_argument('--output', type=Path, default=ROOT.parent/'uwvm2-ros-sdk/.tmp/ros-scene-security/runtime-current')
args = parser.parse_args()
info = json.loads(subprocess.check_output(['docker', 'inspect', args.container]))[0]
group = Path('/sys/fs/cgroup/system.slice')/('docker-'+info['Id']+'.scope')
membership = '0::/'+str(group.relative_to('/sys/fs/cgroup'))


def guard():
    assert info['State']['Running'] and Path('/proc/self/cgroup').read_text().strip() == membership
    assert (group/'memory.max').read_text().strip() == '68719476736'
    assert (group/'memory.swap.max').read_text().strip() == '0'


guard()
out = args.output.resolve()
out.mkdir(parents=True, exist_ok=True)
names = ['resources', 'authority', 'broker_security', 'retirement_allocation', 'deployment']
inputs = ['src/uwvm2/imported/ros/runtime/resources.h', 'src/uwvm2/imported/ros/runtime/contract.h',
          'src/uwvm2/imported/ros/scene/authority.h', 'src/uwvm2/imported/ros/scene/broker.h',
          'src/uwvm2/imported/ros/scene/artifact_contract.h', 'src/uwvm2/imported/ros/scene/artifacts.h',
          'src/uwvm2/imported/ros/scene/storage.h',
          'src/uwvm2/imported/ros/scene/output_contract.h', 'src/uwvm2/imported/ros/scene/outputs.h', 'src/uwvm2/imported/ros/scene/byte_buffer.h',
          'src/uwvm2/imported/ros/scene/managed_lifecycle.h', 'src/uwvm2/imported/ros/scene/managed_lifecycle_contract.h',
          'src/uwvm2/imported/ros/scene/deployment.h', 'src/uwvm2/imported/ros/scene/debug.h']
inputs += ['test/0084.ros-runtime/'+name+'.cc' for name in names]


def hashes():
    return {name: hashlib.sha256((ROOT/name).read_bytes()).hexdigest() for name in inputs}


record = {'mode': args.mode, 'cgroup': membership, 'inode': group.stat().st_ino, 'memory_max': 68719476736,
          'swap_max': 0, 'events_before': (group/'memory.events').read_text(), 'sources': hashes(), 'runs': [], 'complete': False}


def save():
    (out/(args.mode+'.json')).write_text(json.dumps(record, indent=2)+'\n')


def run(label, command, data=None):
    guard()
    start = time.monotonic()
    result = subprocess.run(command, cwd=ROOT, input=data, capture_output=True, timeout=300)
    (out/(label+'.stdout')).write_bytes(result.stdout)
    (out/(label+'.stderr')).write_bytes(result.stderr)
    record['runs'].append({'label': label, 'argv': command, 'exit': result.returncode, 'seconds': time.monotonic()-start})
    save()
    print(label, result.returncode, flush=True)
    if result.returncode:
        print(result.stderr.decode(errors='replace')[-6000:])
        raise SystemExit(result.returncode)


if args.mode in ('sanitizer', 'all', 'thread'):
    compiler = shutil.which('clang++')
    if not compiler:
        raise SystemExit('Clang is required for the selected sanitizer checks')
    selected = ['resources', 'broker_security', 'deployment'] if args.mode == 'thread' else names
    sanitizer = 'thread' if args.mode == 'thread' else 'address,undefined'
    for name in selected:
        binary = out/(name+'-'+args.mode)
        run(name+'-compile-'+args.mode, [compiler, '-std=c++23', '-O1', '-g', '-Wall', '-Wextra', '-Werror',
            '-fsanitize='+sanitizer, '-fno-omit-frame-pointer', '-pthread', '-Isrc',
            str(ROOT/('test/0084.ros-runtime/'+name+'.cc')), '-o', str(binary)])
        run(name+'-'+args.mode, [str(binary)])

if args.mode in ('gcc', 'all'):
    script = '''import pathlib,json,sys,subprocess,os
assert pathlib.Path('/sys/fs/cgroup/memory.max').read_text().strip()=='68719476736'
assert pathlib.Path('/sys/fs/cgroup/memory.swap.max').read_text().strip()=='0'
root=pathlib.Path('/dev/uwvm-scene-core');root.mkdir(exist_ok=True)
for name,text in json.load(sys.stdin).items():
 p=root/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(text)
env=dict(os.environ,TMPDIR='/dev/shm')
print(subprocess.check_output(['g++','--version'],text=True),flush=True)
for name in ['resources','authority','broker_security','retirement_allocation','deployment']:
 command=['g++','-std=c++23','-O2','-g','-Wall','-Wextra','-Werror','-pthread','-I'+str(root/'src'),str(root/('test/0084.ros-runtime/'+name+'.cc')),'-o',str(root/name)]
 print(json.dumps(command),flush=True);subprocess.run(command,env=env,check=True,timeout=90)
 subprocess.run([str(root/name)],check=True,timeout=30);print('PASS '+name,flush=True)
'''
    # Docker exec joins the same memory gate but does not inherit CPU affinity.
    # Preserve the launcher's CPU selection for compiler/profiler coordination.
    script = 'import os;os.sched_setaffinity(0,'+repr(sorted(os.sched_getaffinity(0)))+')\n'+script
    run('gcc-portability', ['docker', 'exec', '-i', args.container, 'python3', '-c', script],
        json.dumps({name: (ROOT/name).read_text() for name in inputs}).encode())

guard()
record['sources_after'] = hashes()
record['events_after'] = (group/'memory.events').read_text()
record['inputs_unchanged'] = record['sources'] == record['sources_after']
assert record['inputs_unchanged'], 'Source changed during qualification'
record['complete'] = True
save()
