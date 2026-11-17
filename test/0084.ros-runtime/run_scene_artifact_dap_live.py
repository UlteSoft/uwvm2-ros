#!/usr/bin/env python3
"""Verify immutable artifact metadata through the real VM, broker and DAP.

Run through in_existing_cgroup.py. Full permission stops at a genuine Wasm
breakpoint after guest open/read/describe. Read-only and limited permissions
execute to the launcher's explicit fault pause; they never gain execution
control through the debugger. Only copied metadata enters the scene inspector.
This Linux acceptance driver does not qualify a graphical IDE session.
"""
import argparse
import base64
import hashlib
import json
from pathlib import Path
import re
import resource
import subprocess
import sys
import time

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT/'test/0018.debugger'))
from run_dap_current_broker import BrokerSession
from run_scene_debug import gate, GROUP

PAYLOAD = b'SCENE_PRIVATE_ARTIFACT_CONTENT_837cd53e' + bytes(range(32))
GENERATION = 57


def wat_bytes(data):
    return ''.join('\\%02x' % byte for byte in data)


def guest(fault):
    digest = hashlib.sha256(PAYLOAD).digest()
    identity = GENERATION.to_bytes(8, 'little') + digest
    checks = '\n'.join('i32.const %d i32.load8_u i32.const %d i32.ne if unreachable end' % (1024+i, byte)
                       for i, byte in enumerate(PAYLOAD))
    digest_checks = '\n'.join('i32.const %d i64.load i32.const %d i64.load i64.ne if unreachable end' % (544+i, 136+i)
                              for i in range(0, 32, 8))
    return f'''(module
 (import "uwvm_ros_v2" "capability.open" (func $cap (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros_v2" "guard.create" (func $create (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (import "uwvm_ros_v2" "guard.trigger" (func $trigger (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (import "uwvm_ros_v2" "artifact.open" (func $open (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (import "uwvm_ros_v2" "artifact.read" (func $read (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (import "uwvm_ros_v2" "artifact.describe" (func $describe (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (memory 1)
 (data (i32.const 0) "weights") (data (i32.const 16) "signal")
 (data (i32.const 128) "{wat_bytes(identity)}")
 (func $spin (export "spin") (loop $again br $again))
 (func (export "_start")
  i32.const 0 i64.const 0 i64.const 7 i64.const 64 call $cap if unreachable end
  i32.const 64 i64.load i32.const 0 i64.const 128 i64.const 40 i64.const 72 i64.const 8 i64.const 80 call $open if unreachable end
  i32.const 256 i32.const 72 i64.load i64.store
  i32.const 64 i64.load i32.const 0 i64.const 256 i64.const 8 i64.const 512 i64.const 64 i64.const 80 call $describe if unreachable end
  i32.const 80 i64.load i64.const 64 i64.ne if unreachable end
  i32.const 512 i64.load i64.const {GENERATION} i64.ne if unreachable end
  i32.const 520 i64.load i64.const {len(PAYLOAD)} i64.ne if unreachable end
  i32.const 528 i64.load i64.const 5 i64.ne if unreachable end
  i32.const 536 i64.load i64.const {len(PAYLOAD)} i64.ne if unreachable end
  {digest_checks}
  i32.const 264 i64.const 0 i64.store
  i32.const 272 i64.const {len(PAYLOAD)} i64.store
  i32.const 64 i64.load i32.const 0 i64.const 256 i64.const 24 i64.const 1024 i64.const {len(PAYLOAD)} i64.const 80 call $read if unreachable end
  i32.const 80 i64.load i64.const {len(PAYLOAD)} i64.ne if unreachable end
  {checks}
  i32.const 0 i64.const 16 i64.const 6 i64.const 88 call $cap if unreachable end
  i32.const 88 i64.load i32.const 0 i64.const 0 i64.const 0 i64.const 96 i64.const 8 i64.const 80 call $create if unreachable end
  i32.const 88 i64.load i32.const 0 i64.const 96 i64.const 8 i64.const 104 i64.const 0 i64.const 80 call $trigger if unreachable end
  {'unreachable' if fault else 'call $spin'}))'''


def private_metadata_absent(value, source):
    encoded = json.dumps(value)
    for secret in (str(source), source.name, hashlib.sha256(PAYLOAD).hexdigest(),
                   base64.b64encode(PAYLOAD).decode(), 'SCENE_PRIVATE_ARTIFACT_CONTENT_837cd53e'):
        assert secret not in encoded, 'scene inspection disclosed private provider data'
    for row in value['rows']:
        assert not set(row) & {'path', 'fd', 'sha256', 'digest', 'content', 'payload', 'memoryReference'}
    assert value['payloadsRead'] is False and value['nativeOperationsIssued'] is False


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Frozen production source tree used to build the qualified executable')
    args = parser.parse_args(); gate()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    source = out/'private-input.bin'; source.write_bytes(PAYLOAD); source.chmod(0o600)
    files = {}
    for name, fault in (('breakpoint', False), ('fault', True)):
        wat = out/(name+'.wat'); wasm = out/(name+'.wasm'); wat.write_text(guest(fault))
        subprocess.run(['wat2wasm', '--debug-names', str(wat), '-o', str(wasm)], check=True)
        files[name] = wasm
    flags = ['--wasm-set-main-module-name', 'main', '--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', 'main']
    for action in ('capability.open', 'guard.create', 'guard.trigger', 'artifact.open', 'artifact.read', 'artifact.describe'):
        flags += ['--ros-scene-interface', 'robot', action, 'allow']
    flags += ['--ros-scene-grant', 'robot', 'signal', 'guard', 'signal', 'local', 'guard.create,guard.trigger,resource.observe',
              '--ros-scene-grant', 'robot', 'weights', 'model-weights', 'vision-weights', 'sealed-input',
              'artifact.open,artifact.read,artifact.describe',
              '--ros-scene-input', 'robot', 'weights', str(source), hashlib.sha256(PAYLOAD).hexdigest(),
              str(GENERATION), str(len(PAYLOAD)), str(len(PAYLOAD)),
              '--ros-scene-input-budget', 'robot', str(len(PAYLOAD)), '1']
    paths = [args.runtime.resolve(), Path(__file__), ROOT/'tools/debug/dap_adapter.py', ROOT/'tools/debug/secure_server.py',
             ROOT/'test/0018.debugger/run_dap_current_broker.py', ROOT/'test/0084.ros-runtime/run_scene_debug.py',
             source, *files.values()]
    paths += [ROOT/'src/uwvm2/imported/ros/scene'/name for name in ('debug.h', 'deployment.h')]
    paths += [ROOT/'src/uwvm2/uwvm/debugger/scene_state.h', ROOT/'src/uwvm2/uwvm/imported/scene/host.h']
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    paths = [production_root/path.resolve().relative_to(ROOT) if path.resolve().is_relative_to(ROOT/'src') else path.resolve() for path in paths]
    pins = lambda: {str(path): hashlib.sha256(path.read_bytes()).hexdigest() for path in paths}
    report = dict(scope='actual immutable artifact VM/broker/DAP; no graphical IDE claim', pins=pins(),
                  cgroup=str(GROUP), inode=GROUP.stat().st_ino, memory_max='68719476736', swap_max='0',
                  events_before=(GROUP/'memory.events').read_text(), cases=[], complete=False)

    report['production_source_root'] = str(production_root)
    report['fixture_source_root'] = str(ROOT)

    def save():
        report['events_after'] = (GROUP/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    try:
        for permission in ('full', 'read-only', 'limited'):
            folder = out/permission; folder.mkdir(); server = None
            record = dict(permission=permission, complete=False); report['cases'].append(record)
            try:
                launch = ['--runtime-debug'] if permission == 'full' else ['--runtime-aot', '--debug-fault-action', 'pause']
                wasm = files['breakpoint' if permission == 'full' else 'fault']
                argv = [*launch, '--debug-permissions', permission, '--runtime-compile-threads', '0',
                        '--runtime-llvm-jit-cache-path', 'disable', *flags, '--run', str(wasm)]
                server = BrokerSession(args.runtime.resolve(), ROOT/'tools/debug/secure_server.py',
                                       ROOT/'tools/debug/dap_adapter.py', argv, folder)
                record['broker_argv'] = server.command
                client = server.connect('wasm')
                record['permissions'] = client.request('uwvm/permissions')['body']
                assert record['permissions']['permissions'] == permission
                if permission == 'full':
                    point = client.evaluate('breakwasm 0 6 0')
                    match = re.search(rb'breakpoint ([0-9]+)', point); assert match, point
                    client.evaluate('continue'); stopped = client.evaluate('wait')
                    assert b'stopped: breakpoint' in stopped, stopped
                    client.evaluate('delete '+match[1].decode())
                else:
                    deadline = time.monotonic()+30
                    while True:
                        stopped = client.evaluate('status')
                        # Limited permission intentionally returns only a compact
                        # status record, with no thread or Wasm location details.
                        ready = (re.fullmatch(rb'limited status=2 stop-id [1-9][0-9]* participants=[0-9]+ reason=9 exit-status=0\n', stopped)
                                 if permission == 'limited' else re.search(rb'(?m)^stop-id [1-9][0-9]*$', stopped))
                        if ready:
                            break
                        assert time.monotonic() < deadline, stopped
                        time.sleep(0.02)
                    record['execution_refusal'] = client.request('continue', {}, success=False)
                record['stop'] = stopped.decode(errors='replace')
                if permission == 'limited':
                    refused = client.request('uwvm/sceneState', {'selection':'resources'}, success=False)
                    assert 'body' not in refused; record['refusal'] = refused
                else:
                    authority = client.request('uwvm/sceneState', {'selection':'authority', 'environment':'robot', 'alias':'weights'})['body']
                    resources = client.request('uwvm/sceneState', {'selection':'resources', 'environment':'robot'})['body']
                    assert len(authority['rows']) == 1 and authority['rows'][0]['admissions'] == '3'
                    assert authority['rows'][0]['open_handles'] == '1'
                    assert len(resources['rows']) == 2
                    artifact = next(row for row in resources['rows'] if row['type']['text'] == 'artifact')
                    guard = next(row for row in resources['rows'] if row['type']['text'] == 'guard')
                    assert artifact['size_bytes'] == str(len(PAYLOAD)) and artifact['immutable']
                    assert artifact['resource']['text'] == 'vision-weights' and artifact['binding']['text'] == 'sealed-input'
                    assert guard['sequence'] == '1' and guard['size_bytes'] == '0' and not guard['immutable']
                    for value in (authority, resources):
                        private_metadata_absent(value, source)
                    again = client.request('uwvm/sceneState', {'selection':'resources', 'environment':'robot', 'ifStop':resources['stopId']})['body']
                    assert again == resources, 'inspection changed resources'
                    repeated = client.request('uwvm/sceneState', {'selection':'authority', 'environment':'robot', 'alias':'weights'})['body']
                    assert repeated == authority, 'inspection changed artifact admission budget'
                    pages = [client.request('uwvm/sceneState', {'selection':'resources', 'environment':'robot', 'start':n,
                             'count':1, 'ifStop':resources['stopId']})['body'] for n in (0, 1)]
                    assert [page['rows'][0] for page in pages] == resources['rows']
                    assert pages[0]['more'] and not pages[1]['more']
                    record.update(authority=authority, resources=resources, pages=pages)
                    if permission == 'full':
                        thread = re.search(rb'thread ([0-9]+)', stopped); assert thread
                        client.evaluate('step wasm '+thread[1].decode())
                        stale = client.request('uwvm/sceneState', {'ifStop':resources['stopId']}, success=False)
                        assert 'body' not in stale; record['stale'] = stale
                record['complete'] = True
            finally:
                failed = sys.exc_info()[0] is not None
                try:
                    if server is not None:
                        record['cleanup'] = server.close()
                except BaseException as error:
                    record['cleanup_error'] = repr(error)
                    if not failed:
                        raise
                finally:
                    save()
        report['pins_after'] = pins()
        assert report['pins'] == report['pins_after'], 'qualified inputs changed during artifact inspection'
        report['complete'] = True
    finally:
        save()
    print('PASS actual artifact DAP, read-only fault-stop inspection, privacy, pagination and unchanged budgets')


if __name__ == '__main__':
    main()
