"""Exercise real scene imports and CLI policy in the existing 64 GiB cgroup.

The guests import only uwvm_ros_v2, including memory64 and a nonzero memory
index. They do not inherit WASI or ROS-v1 privileges. Every assertion is a
separate named Wasm function so traps identify the failed contract check.
This qualifies the local scheduling ABI, not native ROS or model providers.
"""
from pathlib import Path
import argparse
import hashlib
import json
import re
import subprocess
import time


ROOT = Path(__file__).resolve().parents[2]
def current_group():
    """Locate this test process in the cgroup v2 unified hierarchy."""
    rows = Path('/proc/self/cgroup').read_text().splitlines()
    member, = (row.removeprefix('0::/') for row in rows if row.startswith('0::/'))
    relative = Path(member)
    if relative.is_absolute() or '..' in relative.parts:
        raise ValueError('invalid cgroup membership')
    return Path('/sys/fs/cgroup') / relative


GATE = current_group()


def sha(path):
    digest = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def verify_gate():
    if (current_group() != GATE or (GATE / 'memory.max').read_text().strip() != '68719476736'
            or (GATE / 'memory.swap.max').read_text().strip() != '0'):
        raise RuntimeError('scene tests require a 64 GiB cgroup with swap disabled')


class Guest:
    def __init__(self, operations, width=32, memory=0):
        self.operations, self.width, self.memory = operations, width, memory
        self.body, self.checks, self.globals, self.data = [], [], set(), []
        self.cursor = 4096

    def check(self, expression, expected=0):
        name = 'assert_' + str(len(self.checks) + 1)
        self.checks.append(f'(func ${name} {expression} i32.const {expected} i32.ne if unreachable end)')
        self.body.append('call $' + name)

    def word(self, address):
        return f'i{self.width}.const {address} i64.load $work'

    def equal(self, expression, value):
        self.check(f'{expression} i64.const {value} i64.ne')

    def write(self, address, value):
        if isinstance(value, int):
            value = 'i64.const ' + str(value)
        self.body.append(f'i{self.width}.const {address} {value} i64.store $work')

    def save(self, name, address=512):
        self.globals.add(name)
        self.body.append(self.word(address) + ' global.set $' + name)
        return 'global.get $' + name

    def open(self, alias, expected=0, output=512):
        address = self.cursor
        self.cursor += len(alias) + 8
        self.data.append(f'(data (memory $work) (i{self.width}.const {address}) "{alias}")')
        self.check(f'i32.const {self.memory} i64.const {address} i64.const {len(alias)} i64.const {output} call $open', expected)
        if expected == 0:
            return self.save(alias, output)

    def call(self, name, cap, arguments=(), expected=0, capacity=None, memory=None, output=512, length=1024, size=None):
        descriptor = next(row for row in self.operations if row['name'] == name)
        for n, value in enumerate(arguments):
            self.write(128 + 8*n, value)
        if capacity is None:
            capacity = max(descriptor['bytes'], 32)
        if size is None:
            size = descriptor['words'] * 8
        if memory is None:
            memory = self.memory
        if isinstance(cap, int):
            cap = 'i64.const ' + str(cap)
        self.check(f'{cap} i32.const {memory} i64.const 128 i64.const {size} i64.const {output} '
                   f'i64.const {capacity} i64.const {length} call ${name}', expected)

    def render(self):
        imports = ['(import "uwvm_ros_v2" "abi" (func $abi (result i32)))',
                   '(import "uwvm_ros_v2" "capability.open" (func $open (param i32 i64 i64 i64) (result i32)))',
                   '(import "uwvm_ros_v2" "capability.close" (func $close (param i64) (result i32)))']
        imports += [f'(import "uwvm_ros_v2" "{row["name"]}" (func ${row["name"]} '
                    '(param i64 i32 i64 i64 i64 i64 i64) (result i32)))' for row in self.operations]
        memories = '(memory 1) ' * self.memory + '(memory $work ' + ('i64 ' if self.width == 64 else '') + '1)'
        return '(module\n' + '\n'.join(imports) + '\n' + memories + '\n' + '\n'.join(self.data) + '\n' + \
            '\n'.join(f'(global ${name} (mut i64) (i64.const 0))' for name in sorted(self.globals)) + '\n' + \
            '\n'.join(self.checks) + '\n(func (export "_start")\n' + '\n'.join(self.body) + '\n))\n'


def policy(operations, state='allow'):
    flags = ['--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', 'main']
    for name in ['abi', 'capability.open', 'capability.close'] + [row['name'] for row in operations]:
        flags += ['--ros-scene-interface', 'robot', name, state]
    return flags


def grant(alias, purpose, actions, resource=None):
    return ['--ros-scene-grant', 'robot', alias, purpose, resource or alias, 'local', ','.join(actions)]


def success_guest(operations, width, memory):
    g = Guest(operations, width, memory)
    g.check('call $abi', 2)
    g.open('absent', 13)
    caps = {name: g.open(name) for name in ['signal', 'other', 'time', 'steady', 'system', 'timer', 'wait', 'zero', 'rate', 'bytes', 'no-observe', 'no-read']}
    g.open('signal', 11)  # Explicit handle capacity, with no implied rate limit.
    g.call('guard.create', caps['signal'], memory=memory+1, expected=22)
    g.call('guard.create', caps['signal'], size=8, expected=22)
    g.call('guard.create', caps['signal'], output=65535, expected=22)
    g.call('guard.create', caps['signal'], output=2**64-1, expected=22)
    g.call('guard.create', caps['signal'], length=512, expected=22)
    g.call('guard.create', caps['signal'], capacity=4, expected=90)
    g.equal(g.word(1024), 8)
    g.call('guard.create', caps['signal'])
    guard = g.save('guard')
    g.call('guard.create', caps['signal'], expected=11)
    g.call('guard.create', caps['zero'], expected=11)
    g.call('guard.create', caps['bytes'], expected=11)
    g.call('guard.create', caps['rate'])
    g.call('guard.trigger', caps['rate'], [g.word(512)], expected=11)
    g.check(caps['rate'] + ' call $close')
    caps['rate'] = g.open('rate')
    g.call('guard.create', caps['rate'], expected=11)  # Opening cannot replenish a window.
    g.call('guard.trigger', caps['other'], [guard], expected=9)
    g.call('clock.read', caps['signal'], [guard], expected=13)
    g.call('guard.trigger', caps['signal'], [guard])
    g.call('guard.trigger', caps['signal'], [guard])
    g.call('guard.take', caps['signal'], [guard], length=128, expected=22)
    g.call('guard.take', caps['signal'], [guard], capacity=7, expected=90)
    g.call('guard.take', caps['signal'], [guard])
    g.equal(g.word(512), 2)
    g.call('guard.take', caps['signal'], [guard], expected=11)
    g.call('clock.create', caps['time'], [0], expected=13)
    g.call('clock.create', caps['time'], [2])
    clock = g.save('clock')
    g.call('clock.read', caps['time'], [clock])
    g.equal(g.word(528), 0)
    g.call('clock.update', caps['time'], [caps['time'], clock, 100, 1])
    g.call('clock.read', caps['time'], [clock])
    g.equal(g.word(512), 100)
    g.equal(g.word(528), 1)
    for name, kind in [('steady', 0), ('system', 1)]:
        g.call('clock.create', caps[name], [kind])
        resource = g.save(name+'-clock')
        g.call('clock.read', caps[name], [resource])
        g.equal(g.word(528), 1)
        g.equal(g.word(536), kind)
    g.call('timer.create', caps['timer'], [caps['no-read'], clock], expected=13)
    g.call('timer.create', caps['timer'], [caps['time'], clock])
    timer = g.save('alarm')
    g.call('timer.reset', caps['timer'], [timer, 105, 10])
    g.call('timer.take', caps['timer'], [timer], expected=11)
    g.call('clock.update', caps['time'], [caps['time'], clock, 125, 1])
    g.call('timer.take', caps['timer'], [timer], capacity=31, expected=90)
    g.call('timer.take', caps['timer'], [timer])
    g.equal(g.word(512), 105)
    g.equal(g.word(520), 125)
    g.equal(g.word(528), 3)
    g.call('timer.cancel', caps['timer'], [timer])
    g.call('wait.create', caps['wait'])
    wait = g.save('waitset')
    g.call('wait.add', caps['wait'], [wait, caps['no-observe'], guard, 1, 77], expected=13)
    g.call('wait.add', caps['wait'], [wait, caps['signal'], guard, 1, 77])
    g.call('wait.poll', caps['wait'], [wait, 0], expected=110)
    g.call('wait.poll', caps['wait'], [wait, 2**64-1], expected=13)
    g.call('guard.trigger', caps['signal'], [guard])
    g.call('wait.poll', caps['wait'], [wait, 0], capacity=31, expected=90)
    g.equal(g.word(1024), 32)
    g.call('wait.poll', caps['wait'], [wait, 0])
    g.check(g.word(512) + ' ' + guard + ' i64.ne')
    g.equal(g.word(520), 77)
    g.equal(g.word(528), 1)
    sequence = g.save('sequence', 536)
    g.call('wait.acknowledge', caps['wait'], [wait, guard, sequence], expected=22)
    g.call('guard.take', caps['signal'], [guard])
    g.equal(g.word(512), 1)
    g.call('wait.remove', caps['wait'], [wait, guard])
    g.call('wait.add', caps['wait'], [wait, caps['time'], clock, 2, 88])
    g.call('clock.update', caps['time'], [caps['time'], clock, 135, 1])
    g.call('wait.poll', caps['wait'], [wait, 0])
    g.equal(g.word(520), 88)
    g.equal(g.word(528), 2)
    sequence = g.save('sequence', 536)
    g.call('wait.acknowledge', caps['wait'], [wait, clock, sequence])
    g.call('wait.poll', caps['wait'], [wait, 0], expected=110)
    g.call('wait.remove', caps['wait'], [wait, clock])
    for owner, resource in [('signal', guard), ('time', clock), ('timer', timer), ('wait', wait)]:
        g.call('resource.close', caps[owner], [resource])
        g.call('resource.close', caps[owner], [resource], expected=9)
    g.check(caps['signal'] + ' call $close')
    g.check(caps['signal'] + ' call $close', 9)
    g.call('guard.create', caps['signal'], expected=9)
    return g


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--runtime', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--engines', nargs='+', default=['int', 'aot'])
    args = parser.parse_args()
    verify_gate()
    args.runtime = args.runtime.resolve()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    contract = ROOT / 'src/uwvm2/imported/ros/runtime/contract.h'
    operations = [dict(name=n, words=int(w), bytes=int(b)) for n, w, b in
                  re.findall(r' X\(\w+,"([^"]+)","[^"]+",(\d+),(-?\d+)\)', contract.read_text())]
    assert len(operations) == 16
    report = dict(scope='real Wasm local scheduling imports and CLI; no native provider parity claim',
                  cgroup=str(GATE), cgroup_inode=GATE.stat().st_ino, memory_max=(GATE/'memory.max').read_text().strip(),
                  memory_swap_max=(GATE/'memory.swap.max').read_text().strip(),
                  memory_events_before=(GATE/'memory.events').read_text(), runtime_sha256=sha(args.runtime),
                  contract_sha256=sha(contract), cases=[], complete=False)
    def save():
        (out/'results.json').write_text(json.dumps(report, indent=2)+'\n')
    def compile_guest(name, guest):
        wat, wasm = out/(name+'.wat'), out/(name+'.wasm')
        wat.write_text(guest.render())
        subprocess.run(['wat2wasm', '--enable-memory64', '--enable-multi-memory', '--debug-names', str(wat), '-o', str(wasm)], check=True)
        return wasm
    def run(name, flags, wasm=None, expected=True, contains=None):
        verify_gate()
        command = [str(args.runtime), '--wasm-set-main-module-name', 'main', *flags]
        if wasm is not None:
            command += ['--run', str(wasm)]
        start = time.monotonic()
        result = subprocess.run(command, capture_output=True, timeout=120)
        (out/(name+'.stdout')).write_bytes(result.stdout)
        (out/(name+'.stderr')).write_bytes(result.stderr)
        passed = (result.returncode == 0) == expected and (contains is None or contains in result.stderr.decode(errors='replace'))
        report['cases'].append(dict(name=name, argv=command, returncode=result.returncode, passed=passed,
                                   seconds=time.monotonic()-start, guest_sha256=sha(wasm) if wasm else None))
        save()
        assert passed, (name, result.returncode, result.stderr.decode(errors='replace')[-3500:])
        print('PASS', name, flush=True)
    all_allow = policy(operations)
    smoke = compile_guest('imports', Guest(operations))
    run('hidden-default', ['--runtime-int'], smoke, False, 'scene interface is hidden')
    run('hidden-unconfigured', ['--runtime-int', '--ros-scene-create', 'robot', '--ros-scene-bind', 'robot', 'main'], smoke, False)
    wrong_module = all_allow.copy()
    wrong_module[wrong_module.index('--ros-scene-bind')+2] = 'other'
    run('wrong-module', wrong_module, smoke, False, 'scene interface is hidden')
    denied = Guest(operations)
    denied.check('call $abi', 13)
    denied.open('signal', 13)
    denied.check('i64.const 1 call $close', 13)
    for op in operations:
        denied.call(op['name'], 1, expected=13)
    denied_wasm = compile_guest('denied', denied)
    no_grants = Guest(operations)
    no_grants.check('call $abi', 2)
    no_grants.open('signal', 13)
    for op in operations:
        no_grants.call(op['name'], 1, expected=9)
    no_grants_wasm = compile_guest('no-grants', no_grants)
    for engine in args.engines:
        run('deny-'+engine, ['--runtime-'+engine, *policy(operations, 'deny')], denied_wasm)
        run('no-grants-'+engine, ['--runtime-'+engine, *all_allow], no_grants_wasm)
    configured = all_allow.copy()
    for alias, purpose in [('signal','guard'),('other','guard'),('zero','guard'),('rate','guard'),('bytes','guard'),
                           ('no-observe','guard'),('no-read','clock.ros'),('time','clock.ros'),('steady','clock.steady'),
                           ('system','clock.system'),('timer','timer'),('wait','wait')]:
        actions = [op['name'] for op in operations if op['name'].startswith(purpose.split('.')[0]+'.')]
        actions += ['resource.close']
        if alias != 'no-observe': actions += ['resource.observe']
        if alias == 'no-read': actions.remove('clock.read')
        if purpose in ('clock.steady','clock.system'): actions.remove('clock.update')
        configured += grant(alias, purpose, actions)
    limits = [('signal','max-handles','1'),('signal','max-resources','1'),('zero','max-resources','0'),
              ('rate','delivery','may-reject'),('rate','calls-per-window','1'),('rate','window-ns','3600000000000'),
              ('bytes','delivery','may-reject'),('bytes','bytes-per-window','0')]
    for alias, key, value in limits:
        configured += ['--ros-scene-limit','robot',alias,key,value]
    run('valid-policy', [*configured, '--ros-scene-check'])
    for width, memory in [(32,0),(64,0),(32,1),(64,1)]:
        name = f'scheduling-{width}-memory{memory}'
        guest = success_guest(operations, width, memory)
        wasm = compile_guest(name, guest)
        for engine in args.engines:
            run(name+'-'+engine, ['--runtime-'+engine, *configured], wasm)
    base = ['--ros-scene-create','robot']
    one = base + grant('signal','guard',['guard.create'])
    bad = [
        ('duplicate-environment', base+base),
        ('unknown-purpose', base+grant('signal','everything',['guard.create'])),
        ('wrong-action', base+grant('signal','guard',['clock.read'])),
        ('unimplemented-provider', base+grant('signal','topic',['topic.publish'])),
        ('unregistered-binding', base+['--ros-scene-grant','robot','signal','guard','signal','remote','guard.create']),
        ('duplicate-action', base+grant('signal','guard',['guard.create','guard.create'])),
        ('finite-stable', one+['--ros-scene-limit','robot','signal','calls-per-window','1']),
        ('negative-capacity', one+['--ros-scene-limit','robot','signal','max-resources','-1']),
        ('duplicate-capacity', one+['--ros-scene-limit','robot','signal','max-resources','1']*2),
        ('zero-window', one+['--ros-scene-limit','robot','signal','window-ns','0']),
        ('unknown-interface', base+['--ros-scene-interface','robot','guard.create_v1','allow']),
        ('conflicting-membership', base+['--ros-scene-bind','robot','main','--ros-scene-create','other','--ros-scene-bind','other','main']),
    ]
    for name, flags in bad:
        run('cli-'+name, [*flags,'--ros-scene-check'], expected=False)
    report['memory_events_after'] = (GATE/'memory.events').read_text()
    report['complete'] = True
    save()


if __name__ == '__main__':
    main()
