#!/usr/bin/env python3
"""Inspect actual volatile transactions through the real VM, broker and DAP.

Guest assertions verify published bytes, all five output states and retained
receipt recovery before a genuine breakpoint or explicit launcher fault pause.
Debugger snapshots must not execute output operations, acknowledge receipts or
change admission counts. Full, read-only and limited inspection permissions,
mixed-resource pagination, and stale-stop refusal are qualified separately.
This Linux driver requires the existing 64 GiB, zero-swap cgroup and does not
claim a graphical IDE session or durable output storage.
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
import run_scene_vm as base
from run_scene_output_boundaries import OPERATIONS as OUTPUT_OPERATIONS, OutputCalls
ROOT, GROUP = base.ROOT, base.GATE
sys.path.insert(0, str(ROOT/'test/0018.debugger'))
from run_dap_current_broker import BrokerSession

PAYLOAD = b'PRIVATE_OUTPUT_BYTES_46be1e98c701'.ljust(40, b'!')
OPERATIONS = [*OUTPUT_OPERATIONS, dict(name='guard.create', words=0, bytes=8),
              dict(name='guard.trigger', words=1, bytes=0)]
STATES = ('staged', 'published', 'removed', 'aborted', 'none')
ADMISSIONS = {'staged': 4, 'published': 6, 'removed': 6, 'aborted': 4, 'none': 2, 'signal': 8}


class Guest(OutputCalls, base.Guest):
    pass


def guest(fault):
    g = Guest(OPERATIONS)
    payload_words = [int.from_bytes(PAYLOAD[index:index+8], 'little') for index in range(0, len(PAYLOAD), 8)]
    for state in STATES:
        cap = g.open(state)
        g.call('artifact.begin', cap, [1, 65 if state == 'none' else 64])
        g.equal(g.word(536), 11 if state == 'none' else 0)
        if state == 'none':
            g.equal(g.word(528), 0); g.equal(g.word(568), 0)
            g.call('artifact.receipt', cap, [1]); g.equal(g.word(536), 11)
            continue
        obj = g.save(state+'-object', 528)
        g.call('artifact.append', cap, [2, obj, 0, *payload_words])
        g.equal(g.word(536), 0); g.equal(g.word(544), len(PAYLOAD)); g.equal(g.word(552), len(PAYLOAD))
        key, code = 2, 1
        if state in ('published', 'removed'):
            g.call('artifact.commit', cap, [3, obj, len(PAYLOAD)])
            g.equal(g.word(536), 0); g.equal(g.word(568), 2); g.equal(g.word(576), 1)
            g.call('artifact.read', cap, [obj, 0, len(PAYLOAD)], capacity=len(PAYLOAD))
            g.equal(g.word(1024), len(PAYLOAD))
            for index, word in enumerate(payload_words): g.equal(g.word(512+8*index), word)
            key, code = 3, 2
        if state in ('removed', 'aborted'):
            key += 1; code = 4 if state == 'removed' else 3
            g.call('artifact.remove' if state == 'removed' else 'artifact.abort', cap, [key, obj])
            g.equal(g.word(536), 0); g.equal(g.word(560), 0); g.equal(g.word(568), code)
        else:
            g.call('artifact.status', cap, [obj]); g.equal(g.word(568), code)
        g.call('artifact.receipt', cap, [key]); g.equal(g.word(512), key)
        g.equal(g.word(568), code); g.equal(g.word(552), len(PAYLOAD))
    cap = g.open('signal')
    for index in range(4):
        g.call('guard.create', cap); obj = g.save('signal'+str(index))
        g.call('guard.trigger', cap, [obj])
    g.body.append('unreachable' if fault else 'call $spin')
    spin_index = 3+len(OPERATIONS)+len(g.checks)
    wat = g.render().replace('\n(func (export "_start")',
                            '\n(func $spin (export "spin") (loop $again br $again))\n(func (export "_start")', 1)
    return wat, spin_index


def flags():
    result = ['--wasm-set-main-module-name', 'main', *base.policy(OPERATIONS)]
    actions = ','.join(row['name'] for row in OUTPUT_OPERATIONS)
    for state in STATES:
        result += ['--ros-scene-grant', 'robot', state, 'cache', 'output-'+state, 'volatile-output', actions,
                   '--ros-scene-output', 'robot', state, '64', '1', '64', '64']
    result += ['--ros-scene-output-budget', 'robot', '320', '5', '800',
               '--ros-scene-grant', 'robot', 'signal', 'guard', 'signal', 'local', 'guard.create,guard.trigger',
               '--ros-scene-limit', 'robot', 'signal', 'max-resources', '4']
    return result


def private_metadata_absent(value):
    encoded = json.dumps(value)
    for secret in (PAYLOAD.decode(), base64.b64encode(PAYLOAD).decode(), hashlib.sha256(PAYLOAD).hexdigest()):
        assert secret not in encoded, 'scene inspection disclosed private output data'
    for row in value['rows']:
        assert not set(row) & {'path', 'fd', 'sha256', 'digest', 'content', 'payload', 'memoryReference',
                               'key', 'request_key', 'last_key', 'request', 'last_operation'}
    assert value['payloadsRead'] is False and value['nativeOperationsIssued'] is False


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--prepare-only', action='store_true')
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Frozen production source tree used to build the qualified executable')
    args = parser.parse_args(); base.verify_gate()
    if not args.prepare_only and args.runtime is None: parser.error('--runtime is required')
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=False)
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    files = {}; spin_index = None
    for name, fault in (('breakpoint', False), ('fault', True)):
        wat = out/(name+'.wat'); wasm = out/(name+'.wasm'); source, spin_index = guest(fault); wat.write_text(source)
        subprocess.run(['wat2wasm', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)], check=True)
        files[name] = wasm
    paths = [Path(__file__), ROOT/'tools/debug/dap_adapter.py', ROOT/'tools/debug/secure_server.py',
             ROOT/'test/0018.debugger/run_dap_current_broker.py', ROOT/'test/0084.ros-runtime/run_scene_vm.py',
             ROOT/'test/0084.ros-runtime/run_scene_output_boundaries.py', *files.values()]
    if args.runtime: paths.append(args.runtime.resolve())
    paths += [ROOT/'src/uwvm2/imported/ros/scene'/name for name in ('debug.h', 'deployment.h')]
    paths += [ROOT/'src/uwvm2/uwvm/debugger/scene_state.h', ROOT/'src/uwvm2/uwvm/imported/scene/host.h']
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    paths = [production_root/path.resolve().relative_to(ROOT) if path.resolve().is_relative_to(ROOT/'src') else path.resolve() for path in paths]
    pins = lambda: {str(path): base.sha(path) for path in paths}
    report = dict(scope='actual volatile output VM/broker/DAP; no graphical IDE or durability claim', pins=pins(),
                  cgroup=str(GROUP), inode=GROUP.stat().st_ino, memory_max='68719476736', swap_max='0',
                  events_before=(GROUP/'memory.events').read_text(), cases=[], complete=False, prepare_only=args.prepare_only)

    report['production_source_root'] = str(production_root)
    report['fixture_source_root'] = str(ROOT)

    def save():
        report['events_after'] = (GROUP/'memory.events').read_text()
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')

    try:
        for permission in (() if args.prepare_only else ('full', 'read-only', 'limited')):
            folder = out/permission; folder.mkdir(); server = None
            record = dict(permission=permission, complete=False); report['cases'].append(record)
            try:
                launch = ['--runtime-debug'] if permission == 'full' else ['--runtime-aot', '--debug-fault-action', 'pause']
                wasm = files['breakpoint' if permission == 'full' else 'fault']
                argv = [*launch, '--debug-permissions', permission, '--runtime-compile-threads', '0',
                        '--runtime-llvm-jit-cache-path', 'disable', *flags(), '--run', str(wasm)]
                server = BrokerSession(args.runtime.resolve(), ROOT/'tools/debug/secure_server.py',
                                       ROOT/'tools/debug/dap_adapter.py', argv, folder)
                record['broker_argv'] = server.command; client = server.connect('wasm')
                record['permissions'] = client.request('uwvm/permissions')['body']
                assert record['permissions']['permissions'] == permission
                if permission == 'full':
                    point = client.evaluate(f'breakwasm 0 {spin_index} 0')
                    match = re.search(rb'breakpoint ([0-9]+)', point); assert match, point
                    client.evaluate('continue'); stopped = client.evaluate('wait')
                    assert b'stopped: breakpoint' in stopped, stopped
                    client.evaluate('delete '+match[1].decode())
                else:
                    deadline = time.monotonic()+30
                    while True:
                        stopped = client.evaluate('status')
                        ready = (re.fullmatch(rb'limited status=2 stop-id [1-9][0-9]* participants=[0-9]+ reason=9 exit-status=0\n', stopped)
                                 if permission == 'limited' else re.search(rb'(?m)^stop-id [1-9][0-9]*$', stopped))
                        if ready: break
                        assert time.monotonic() < deadline, stopped
                        time.sleep(0.02)
                    record['execution_refusal'] = client.request('continue', {}, success=False)
                record['stop'] = stopped.decode(errors='replace')
                if permission == 'limited':
                    refused = client.request('uwvm/sceneState', {'selection': 'resources'}, success=False)
                    assert 'body' not in refused; record['refusal'] = refused
                else:
                    resources = client.request('uwvm/sceneState', {'selection': 'resources', 'environment': 'robot'})['body']
                    assert len(resources['rows']) == 8 and resources['more']
                    second = client.request('uwvm/sceneState', {'selection': 'resources', 'environment': 'robot',
                                            'start': 8, 'ifStop': resources['stopId']})['body']
                    assert len(second['rows']) == 1 and not second['more']
                    rows = resources['rows']+second['rows']; assert len(rows) == 9
                    guards = [row for row in rows if row['type']['text'] == 'guard']; assert len(guards) == 4
                    for guard in guards:
                        assert guard['sequence'] == '1' and guard['state']['text'] == ''
                        assert guard['accepted_bytes'] == guard['reserved_bytes'] == guard['retained_receipts'] == '0'
                        assert not guard['published'] and not guard['durable']
                    for state in STATES:
                        row = next(row for row in rows if row['alias']['text'] == state)
                        assert row['state']['text'] == state and row['binding']['text'] == 'volatile-output'
                        assert row['resource']['text'] == 'output-'+state and not row['immutable'] and row['size_bytes'] == '0'
                        assert row['accepted_bytes'] == ('0' if state == 'none' else str(len(PAYLOAD)))
                        assert row['reserved_bytes'] == ('64' if state in ('staged', 'published') else '0')
                        assert row['retained_receipts'] == '1' and row['published'] == (state == 'published') and not row['durable']
                        assert row['type']['text'] == ('artifact.output' if state in ('staged', 'published') else 'artifact.receipt')
                        assert (row['id'] == '0') == (state == 'none')
                    authority = client.request('uwvm/sceneState', {'selection': 'authority', 'environment': 'robot'})['body']
                    assert len(authority['rows']) == 6
                    for row in authority['rows']:
                        assert row['admissions'] == str(ADMISSIONS[row['alias']['text']]) and row['open_handles'] == '1'
                    for value in (resources, second, authority): private_metadata_absent(value)
                    for _ in range(3):
                        for selection, start, previous in (('resources', 0, resources), ('resources', 8, second), ('authority', 0, authority)):
                            repeated = client.request('uwvm/sceneState', {'selection': selection, 'environment': 'robot',
                                                      'start': start, 'ifStop': resources['stopId']})['body']
                            assert repeated == previous, 'inspection changed output/receipt state or budgets'
                    pages = [client.request('uwvm/sceneState', {'selection': 'resources', 'environment': 'robot',
                             'start': index, 'count': 1, 'ifStop': resources['stopId']})['body'] for index in range(9)]
                    assert [page['rows'][0] for page in pages] == rows
                    assert all(page['more'] for page in pages[:-1]) and not pages[-1]['more']
                    record.update(authority=authority, resources=[resources, second], single_row_pages=pages)
                    if permission == 'full':
                        thread = re.search(rb'thread ([0-9]+)', stopped); assert thread
                        client.evaluate('step wasm '+thread[1].decode())
                        stale = client.request('uwvm/sceneState', {'ifStop': resources['stopId']}, success=False)
                        assert 'body' not in stale; record['stale'] = stale
                record['complete'] = True
            finally:
                failed = sys.exc_info()[0] is not None
                try:
                    if server is not None: record['cleanup'] = server.close()
                except BaseException as error:
                    record['cleanup_error'] = repr(error)
                    if not failed: raise
                finally: save()
        report['pins_after'] = pins()
        assert report['pins'] == report['pins_after'], 'qualified inputs changed during output inspection'
        report['complete'] = True
    finally:
        save()
    print('PASS output DAP guest preparation' if args.prepare_only else
          'PASS actual output DAP states, permissions, pagination, privacy and unchanged receipts/budgets')


if __name__ == '__main__':
    main()
