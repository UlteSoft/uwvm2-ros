#!/usr/bin/env python3
"""Inspect cached managed lifecycle state through the actual VM, broker and DAP.

The guest creates every closed primary/intermediate state, then waits using
clock.steady until one queued callback deadline has passed. It makes no later
lifecycle call on that binding: debugger reads must retain its pending state.
The test covers exact counters, independent debugger permission, pagination,
callback privacy and unchanged authority admissions. A logical lifecycle is
not a native ROS node, callback executor or physical device stop. All processes
must inherit the existing 64 GiB zero-swap cgroup.
"""
import argparse
import json
from pathlib import Path
import re
import resource
import subprocess
import sys
import time

sys.dont_write_bytecode = True
import run_scene_vm as base
ROOT, GROUP = base.ROOT, base.GATE
sys.path.insert(0, str(ROOT/'test/0018.debugger'))
from run_dap_current_broker import BrokerSession

CONTRACT = ROOT/'src/uwvm2/imported/ros/scene/managed_lifecycle_contract.h'
LIFECYCLE = [dict(name=n, words=int(w), bytes=int(b)) for n, w, b in
             re.findall(r' X\(\w+,\w+,"([^"]+)",(\d+),(\d+)\)', CONTRACT.read_text())]
assert len(LIFECYCLE) == 6
OPERATIONS = LIFECYCLE + [dict(name='clock.create', words=1, bytes=8), dict(name='clock.read', words=1, bytes=32)]
STATES = ('unconfigured', 'inactive', 'active', 'finalized', 'configuring', 'cleaning_up',
          'shutting_down', 'activating', 'deactivating', 'error_processing', 'expired')
STATE_NUMBER = dict(unconfigured=1, inactive=2, active=3, finalized=4, configuring=10,
                    cleaning_up=11, shutting_down=12, activating=13, deactivating=14, error_processing=15)


def guest(fault):
    g = base.Guest(OPERATIONS); expected = {}
    g.check('call $abi', 2)
    clock_cap = g.open('steady'); g.call('clock.create', clock_cap, [0]); clock = g.save('clock')

    def deadline(alias, delta):
        g.call('clock.read', clock_cap, [clock], output=768, length=1104)
        name = alias+'-deadline'; g.globals.add(name)
        g.body.append(g.word(768)+f' i64.const {delta} i64.add global.set ${name}')
        return 'global.get $'+name

    expired_deadline = None
    for alias in STATES:
        cap = g.open(alias); count = 0

        def call(name, arguments):
            nonlocal count
            g.call(name, cap, arguments); count += 1

        call('lifecycle.open', [1]); client = g.save(alias+'-client')
        call('lifecycle.open', [2]); server = g.save(alias+'-server')
        state, generation, key, transition, delivered = 'unconfigured', 1, 0, 0, False
        due, request = None, None

        def submit(action, phase, short=False):
            nonlocal state, generation, key, transition, delivered, due, request
            due = deadline(alias, 500_000_000 if short else 60_000_000_000)
            key += 1; transition = action; delivered = False
            call('lifecycle.submit', [client, key, action, generation, due])
            request = g.save(alias+'-request', 520); generation += 1; state = phase
            g.equal(g.word(528), generation); g.equal(g.word(536), STATE_NUMBER[state]); g.equal(g.word(544), 0)

        def take():
            nonlocal delivered
            call('lifecycle.take', [server]); delivered = True
            g.equal(g.word(520), transition); g.equal(g.word(528), generation)
            g.equal(g.word(536), STATE_NUMBER[state])

        def respond(outcome, target):
            nonlocal state, generation, transition, delivered
            call('lifecycle.respond', [server, request, generation, outcome])
            generation += 1; state = target; delivered = False
            if outcome != 99: transition = 0
            g.equal(g.word(528), generation); g.equal(g.word(536), STATE_NUMBER[state]); g.equal(g.word(544), 0)

        def finish(action, phase, target):
            submit(action, phase); take(); respond(97, target)

        if alias in ('inactive', 'active', 'cleaning_up', 'activating', 'deactivating'):
            finish(1, 'configuring', 'inactive')
        if alias in ('active', 'deactivating'):
            finish(3, 'activating', 'active')
        if alias in ('finalized', 'shutting_down'):
            submit(5, 'shutting_down')
            if alias == 'finalized': take(); respond(97, 'finalized')
        elif alias in ('configuring', 'error_processing', 'expired'):
            submit(1, 'configuring', short=alias == 'expired')
            if alias == 'error_processing': take(); respond(99, 'error_processing')
        elif alias == 'cleaning_up': submit(2, 'cleaning_up')
        elif alias == 'activating': submit(3, 'activating'); take()
        elif alias == 'deactivating': submit(4, 'deactivating'); take()
        # This read establishes guest-observed state before expiry. For expired,
        # deliberately avoid it: only later clock operations advance time.
        if alias != 'expired':
            call('lifecycle.state', [client]); g.equal(g.word(512), STATE_NUMBER[state])
            g.equal(g.word(520), generation); g.equal(g.word(552), 0); g.equal(g.word(560), int(delivered))
        else: expired_deadline = due
        expected[alias] = dict(state=state, generation=str(generation), transition=str(transition),
                               callback_pending=bool(transition), callback_delivered=delivered, admissions=str(count))

    assert expired_deadline is not None
    # Exit only after actual steady time reaches the callback deadline. No
    # lifecycle operation occurs after this point, including before the stop.
    g.body.append('block $deadline_done loop $deadline_wait')
    g.call('clock.read', clock_cap, [clock], output=768, length=1104)
    g.body.append(g.word(768)+' '+expired_deadline+' i64.lt_u br_if $deadline_wait end end')
    g.body.append('unreachable' if fault else 'call $spin')
    spin_index = 3+len(OPERATIONS)+len(g.checks)
    wat = g.render().replace('\n(func (export "_start")',
                            '\n(func $spin (export "spin") (loop $again br $again))\n(func (export "_start")', 1)
    return wat, spin_index, expected


def flags():
    result = ['--wasm-set-main-module-name', 'main', *base.policy(OPERATIONS)]
    actions = ','.join(row['name'] for row in LIFECYCLE)
    for alias in STATES:
        result += ['--ros-scene-grant', 'robot', alias, 'lifecycle', 'node-'+alias, 'local-lifecycle', actions,
                   '--ros-scene-lifecycle', 'robot', alias, '2', '120000000000', '254', '3']
    result += ['--ros-scene-grant', 'robot', 'steady', 'clock.steady', 'steady', 'local', 'clock.create,clock.read']
    return result


def private_metadata_absent(value):
    for row in value['rows']:
        assert not set(row) & {'path', 'fd', 'content', 'payload', 'memoryReference', 'native_node',
                               'key', 'request_key', 'last_key', 'request', 'request_id', 'last_operation'}
    assert value['payloadsRead'] is False and value['nativeOperationsIssued'] is False


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--prepare-only', action='store_true')
    parser.add_argument('--source-root', type=Path, default=ROOT, help='Frozen production source tree used to build the qualified executable')
    args = parser.parse_args(); base.verify_gate(); resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    if not args.prepare_only and args.runtime is None: parser.error('--runtime is required')
    out = args.output.resolve(); assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False); files = {}; spin_index = None; expected = None
    for name, fault in (('breakpoint', False), ('fault', True)):
        wat, wasm = out/(name+'.wat'), out/(name+'.wasm')
        source, spin_index, expected = guest(fault); wat.write_text(source)
        subprocess.run(['wat2wasm', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)], check=True)
        files[name] = wasm
    paths = [Path(__file__).resolve(), CONTRACT, ROOT/'tools/debug/dap_adapter.py', ROOT/'tools/debug/secure_server.py',
             ROOT/'test/0018.debugger/run_dap_current_broker.py', ROOT/'test/0084.ros-runtime/run_scene_vm.py', *files.values(),
             ROOT/'src/uwvm2/imported/ros/scene/debug.h', ROOT/'src/uwvm2/imported/ros/scene/deployment.h',
             ROOT/'src/uwvm2/uwvm/debugger/scene_state.h']
    if args.runtime: paths.append(args.runtime.resolve())
    production_root = args.source_root.resolve(); assert (production_root/'src').is_dir()
    paths = [production_root/path.resolve().relative_to(ROOT) if path.resolve().is_relative_to(ROOT/'src') else path.resolve() for path in paths]
    pins = lambda: {str(path): base.sha(path) for path in paths}
    report = dict(scope=__doc__, pins=pins(), cgroup=str(GROUP), inode=GROUP.stat().st_ino,
                  memory_max='68719476736', swap_max='0', events_before=(GROUP/'memory.events').read_text(),
                  cases=[], expected=expected, complete=False, prepare_only=args.prepare_only)

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
                    views = {}
                    for selection in ('resources', 'authority'):
                        first = client.request('uwvm/sceneState', {'selection': selection, 'environment': 'robot'})['body']
                        assert first['total'] == 12 and len(first['rows']) == 8 and first['more']
                        second = client.request('uwvm/sceneState', {'selection': selection, 'environment': 'robot',
                                                'start': 8, 'ifStop': first['stopId']})['body']
                        assert len(second['rows']) == 4 and not second['more']; views[selection] = [first, second]
                    rows = [row for page in views['resources'] for row in page['rows']]
                    lifecycle = [row for row in rows if row['type']['text'] == 'lifecycle']; assert len(lifecycle) == 11
                    for row in lifecycle:
                        alias = row['alias']['text']; wanted = expected[alias]
                        assert row['id'] == '0' and row['binding']['text'] == 'local-lifecycle' and row['resource']['text'] == 'node-'+alias
                        assert row['state']['text'] == wanted['state'] and row['open_handles'] == '2' and row['status_code'] == '0'
                        for field in ('generation', 'transition', 'callback_pending', 'callback_delivered'):
                            assert row[field] == wanted[field], (alias, field, row[field], wanted[field])
                        assert (int(row['deadline_ns']) > 0) == row['callback_pending']
                        assert all(row[key] == '0' for key in ('size_bytes', 'accepted_bytes', 'reserved_bytes', 'retained_receipts'))
                        assert not any(row[key] for key in ('immutable', 'published', 'durable', 'retired'))
                    expired = next(row for row in lifecycle if row['alias']['text'] == 'expired')
                    assert expired['state']['text'] == 'configuring' and not expired['callback_delivered']
                    assert time.monotonic_ns() >= int(expired['deadline_ns']), 'guest did not reach the pending callback deadline'
                    for page in views['authority']:
                        for row in page['rows']:
                            assert row['open_handles'] == '1'
                            if row['alias']['text'] in expected:
                                assert row['admissions'] == expected[row['alias']['text']]['admissions']
                    for selection, pages in views.items():
                        for page in pages:
                            private_metadata_absent(page)
                            for _ in range(3):
                                repeated = client.request('uwvm/sceneState', {'selection': selection, 'environment': 'robot',
                                                          'start': page['start'], 'ifStop': page['stopId']})['body']
                                assert repeated == page, 'inspection advanced lifecycle state, consumed callback or charged budgets'
                    pages = [client.request('uwvm/sceneState', {'selection': 'resources', 'environment': 'robot',
                             'start': index, 'count': 1, 'ifStop': views['resources'][0]['stopId']})['body'] for index in range(12)]
                    assert [page['rows'][0] for page in pages] == rows
                    assert all(page['more'] for page in pages[:-1]) and not pages[-1]['more']
                    record.update(views=views, single_row_pages=pages, expired_pending_verified=True)
                    if permission == 'full':
                        thread = re.search(rb'thread ([0-9]+)', stopped); assert thread
                        client.evaluate('step wasm '+thread[1].decode())
                        stale = client.request('uwvm/sceneState', {'ifStop': views['resources'][0]['stopId']}, success=False)
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
        report['pins_after'] = pins(); assert report['pins'] == report['pins_after'], 'qualified lifecycle inputs changed'
        report['complete'] = True
    finally: save()
    print('PASS lifecycle DAP guest preparation' if args.prepare_only else
          'PASS actual lifecycle DAP closed states, past-deadline nonmutation, permissions, pagination and privacy')


if __name__ == '__main__':
    main()
