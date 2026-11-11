#!/usr/bin/env python3
"""Actual VM active-data/f32 module updates and terminal-fault admission.

Uses a disposable Ed25519 fixture key and inherited authenticated socketpairs.
An unsupported target fails its update attempt; it is never counted as a pass.
Standalone target execution and native signed publication are separate evidence.
Fallback fault pause is a whole-process SIGSTOP. The explicit supervisor may
continue terminal handling; this does not resume the rejected Guest instruction.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import shlex
import signal
import socket
import struct
import subprocess
import time

from run_signed_update_boundaries import permit, request, sha
from cryptography.hazmat.primitives.asymmetric.ed25519 import Ed25519PrivateKey
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat


def u(value):
    out = bytearray()
    while True:
        byte = value & 127
        value >>= 7
        out.append(byte | (128 if value else 0))
        if not value:
            return bytes(out)


def s32(value):
    value = value - (1 << 32) if value & (1 << 31) else value
    out = bytearray()
    while True:
        byte = value & 127
        value >>= 7
        more = not ((value == 0 and not byte & 64) or (value == -1 and byte & 64))
        out.append(byte | (128 if more else 0))
        if not more:
            return bytes(out)


def section(number, payload):
    return bytes([number]) + u(len(payload)) + payload


def f32(bits):
    return b'\x43' + struct.pack('<I', bits)


def assert_value(selector, expected):
    # value(f32) -> f32 crosses an actual guest call ABI. Compare raw bits so
    # signed zero/subnormal payloads are not hidden by float equality or text.
    return f32(selector) + b'\x10\x01\xbc\x41' + s32(expected) + b'\x47\x04\x40\x00\x0b'


def state_module(changed=False, *, revision=0, fault=False, uncaught=False, pages=1, invalid_data=False):
    global_bits = 1 if changed else 0x80000000
    first_data = b'EFGH' if changed else b'ABCD'
    memory_bits = int.from_bytes(first_data[:2] + b'xy', 'little')
    start = b'\x00\x10\x02'
    if fault:
        # The real namespaced INSERT opts this memory into WasmMTE; an ordinary
        # high i32 address in a non-MTE module is correctly a Wasm bounds fault.
        start += b'\x41\x00\x41\x01\xff' + u(0x01010022) + u(0) + b'\x28\x02\x00\x1a'
    if uncaught:
        start += b'\x08\x00'  # Throw the actual locally defined () -> () tag.
    start += b'\x0b'
    value = b'\x00' + b'\x01' * revision + b'\x20\x00' + f32(0) + b'\x5b\x04\x7d\x23\x00\x05\x41\x00\x2a\x02\x00\x0b\x0b'
    checked = b'\x00' + assert_value(0, global_bits) + assert_value(0x3f800000, memory_bits) + b'\x0b'
    exports = b'\x02\x06_start\x00\x00\x05value\x00\x01'
    data = b'\x02\x00\x41' + s32(pages * 65536 if invalid_data else 0) + b'\x0b\x04' + first_data
    data += b'\x00\x41\x02\x0b\x02xy'
    return (b'\x00asm\x01\x00\x00\x00'
            + section(1, b'\x02\x60\x00\x00\x60\x01\x7d\x01\x7d')
            + section(3, b'\x03\x00\x01\x00')
            + section(5, b'\x01\x01' + u(pages) + u(pages))
            + (section(13, b'\x01\x00\x00') if uncaught else b'')
            + section(6, b'\x01\x7d\x01' + f32(global_bits) + b'\x0b')
            + section(7, exports)
            + section(10, b'\x03' + b''.join(u(len(body)) + body for body in (start, value, checked)))
            + section(11, data))


def manifest(kind, *, omit_global=False):
    scopes = [0, 1, 2] + ([] if omit_global else [1 << 63])
    if kind == 'migrate':
        migration = b'UWM1' + struct.pack('<III', 1, 1, 0)
        migration += struct.pack('<QQQQQ', 0, 0, 0, 0, 4) + struct.pack('<QQ', 0, 0)
        return (b'UUT3' + struct.pack('<III', 8, len(scopes), 0) + bytes(48)
                + struct.pack('<II', len(migration), 0)
                + b''.join(struct.pack('<Q', index) for index in scopes) + migration)
    flags = {'reset': 2, 'preserve': 4}[kind]
    return b'UUT2' + struct.pack('<III', flags, len(scopes), 0) + bytes(48) + b''.join(struct.pack('<Q', index) for index in scopes)


class Harness:
    def __init__(self, args):
        self.args = args
        self.runner = shlex.split(args.runner)
        self.rows, self.processes, self.completed = [], [], []
        self.key = Ed25519PrivateKey.from_private_bytes(bytes(range(1, 33)))
        self.public = self.key.public_key().public_bytes(Encoding.Raw, PublicFormat.Raw)
        self.current = ''

    def check(self, name, condition, detail):
        self.rows.append(dict(scenario=self.current, name=name, passed=bool(condition), detail=detail))
        print(self.current, name, 'PASS' if condition else 'FAIL', flush=True)
        if not condition:
            raise AssertionError(name)

    def command(self):
        return [*self.runner, str(self.args.vm.resolve()), '--runtime-' + self.args.engine,
                '--runtime-compile-threads', '0', '--log-color', 'disable',
                *(['--runtime-llvm-jit-cache-path', 'disable'] if self.args.engine == 'aot' else [])]

    def write_image(self, name, contents):
        path = self.args.out / (name + '.wasm')
        path.write_bytes(contents)
        return path

    def call(self, vm, selector):
        payload = struct.pack('<III', 1, 4, 4) + struct.pack('<I', selector)
        deadline = time.monotonic() + 5
        while True:
            reply = request(vm.host, 5, payload)
            if reply['status'] != 8 or not reply['active'] or time.monotonic() >= deadline:
                return reply
            time.sleep(.002)

    def values(self, vm, global_bits, memory_bits, label):
        for selector, bits, part in ((0, global_bits, 'global'), (0x3f800000, memory_bits, 'active-data')):
            reply = self.call(vm, selector)
            self.check(label + ' ' + part, reply['status'] == 0 and reply['active']
                       and reply['result'] == struct.pack('<I', bits).hex(), reply)

    def launch(self, vm):
        state = request(vm.host, 0)
        self.check('awaiting explicit signed launch', state['status'] == 0 and state['state'] == 8
                   and not state['active'] and state['epoch'] == 0, state)
        launch = permit(self.key, state, 4, bytes.fromhex(state['installed']), sha(b'state-launch'))
        reply = request(vm.host, 9, launch)
        self.check('signed launch epoch1', reply['status'] == 0 and reply['active'] and reply['epoch'] == 1, reply)
        vm.launch_facts = reply
        return state, launch

    def prepare(self, vm, target, declaration, transaction, *, corrupt_signature=False):
        facts = request(vm.host, 0)
        token = permit(self.key, facts, 1, sha(target), sha(transaction), declaration=declaration)
        if corrupt_signature:
            token = token[:-1] + bytes([token[-1] ^ 1])
        return facts, request(vm.host, 7, token + struct.pack('<I', len(declaration)) + declaration + target)

    def commit(self, vm, before, target, declaration, transaction, *, test_tamper=False):
        ready = request(vm.host, 2)
        self.check('READY seals old epoch', ready['status'] == 0 and ready['state'] == 4 and not ready['active']
                   and ready['epoch'] == before['epoch'] and ready['installed'] == before['installed']
                   and int(ready['prepared'], 16) != 0 and int(ready['ready'], 16) != 0, ready)
        token = permit(self.key, before, 2, sha(target), sha(transaction), declaration=declaration,
                       prepared=bytes.fromhex(ready['prepared']), ready=bytes.fromhex(ready['ready']))
        if test_tamper:
            invalid = token[:-1] + bytes([token[-1] ^ 1])
            reply = request(vm.host, 3, invalid)
            self.check('tampered COMMIT cannot publish', reply['status'] == 4 and reply['state'] == 4
                       and not reply['active'] and reply['epoch'] == before['epoch']
                       and reply['installed'] == before['installed'], reply)
        reply = request(vm.host, 3, token)
        self.check('signed publication advances one epoch', reply['status'] == 0 and reply['active']
                   and reply['epoch'] == before['epoch'] + 1 and reply['installed'] == sha(target).hex(), reply)
        replay = request(vm.host, 3, token)
        self.check('COMMIT replay is receipt only', replay['status'] == 0 and replay['active']
                   and replay['epoch'] == reply['epoch'] and replay['installed'] == reply['installed'], replay)

    def stop(self, vm):
        reply = request(vm.host, 6)
        self.check('authorized healthy stop', reply['status'] == 0, reply)
        vm.child.wait(timeout=5)
        self.check('healthy process exit', vm.child.returncode == 0, dict(exit=vm.child.returncode))


class Instance:
    def __init__(self, harness, name, image, *, pause=False, features=()):
        self.harness, self.name, self.image, self.pause = harness, name, image, pause
        self.features = list(features)

    def __enter__(self):
        h = self.harness
        self.host, guest = socket.socketpair(socket.AF_UNIX, socket.SOCK_STREAM)
        guest.setsockopt(socket.SOL_SOCKET, socket.SO_PASSCRED, 1)
        self.host.settimeout(10)
        self.error_path = h.args.out / (self.name + '.stderr')
        self.error = self.error_path.open('wb')
        self.output = (h.args.out / (self.name + '.stdout')).open('wb')
        path = h.write_image(self.name, self.image)
        self.argv = h.command() + self.features + ['--debug-permissions', 'full', *(['--debug-fault-action', 'pause'] if self.pause else []),
            '--update-control-fd', str(guest.fileno()), '--update-public-key', h.public.hex(),
            '--update-contract', sha(b'test approved contract').hex(), '--update-scope', sha(b'test isolated deployment scope').hex(),
            '--update-call-export', 'value', '--wasip1-single-create', 'main', '--wasip1-single-enable', 'main',
            '--wasm-set-main-module-name', 'main', '-Wstart', '0', '--run', str(path.resolve())]
        self.child = subprocess.Popen(self.argv, pass_fds=(guest.fileno(),), stdout=self.output, stderr=self.error)
        guest.close()
        return self

    def __exit__(self, *_):
        cleanup = None
        if self.child.poll() is None:
            cleanup = 'supervisor terminated owned fixture process'
            self.child.terminate()
            try:
                self.child.wait(timeout=3)
            except subprocess.TimeoutExpired:
                cleanup = 'supervisor killed owned fixture process'
                self.child.kill()
                self.child.wait(timeout=3)
        self.host.close()
        self.error.close()
        self.output.close()
        self.harness.processes.append(dict(name=self.name, command=self.argv, exit=self.child.returncode,
            cleanup=cleanup, image_sha256=sha(self.image).hex(), stderr=self.error_path.read_text(errors='replace'),
            fault_probe=getattr(self, 'fault_probe', None)))


def standalone(h):
    for name, image, expected in (('initial', state_module(), 0), ('target', state_module(True), 0),
                                  ('fault', state_module(fault=True), 122)):
        path = h.write_image('standalone-' + name, image)
        command = h.command() + ['-Wstart', '0', '--run', str(path.resolve())]
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=20)
        (h.args.out / ('standalone-' + name + '.stderr')).write_bytes(result.stderr)
        h.processes.append(dict(name='standalone-' + name, command=command, exit=result.returncode,
            image_sha256=sha(image).hex(), stderr=result.stderr.decode(errors='replace')))
        okay = result.returncode == expected
        if expected == 122:
            okay = (okay and b'[crash] memory-protection' in result.stderr
                    and b'Reason: memory tag mismatch' in result.stderr and b'Exit status: 122' in result.stderr)
        h.check(name + ' actual guest execution', okay, dict(exit=result.returncode, expected=expected,
                global_bits='00000001' if name == 'target' else '80000000', memory_bytes='EFxy' if name == 'target' else 'ABxy'))


def state_update(h):
    initial, target = state_module(), state_module(True)
    h.write_image('reset-target', target)
    with Instance(h, 'module-state', initial) as vm:
        h.launch(vm)
        h.values(vm, 0x80000000, int.from_bytes(b'ABxy', 'little'), 'initial bits')
        negatives = [('signed preserve cannot alter initial state', target, manifest('preserve'), 7, False),
                     ('missing global scope cannot reset f32', target, manifest('reset', omit_global=True), 7, False),
                     ('bad active-data bounds is validation_failed', state_module(True, invalid_data=True), manifest('reset'), 10, False),
                     ('bad PREPARE signature cannot change state', target, manifest('reset'), 4, True)]
        for name, candidate, declaration, status, corrupt in negatives:
            _, reply = h.prepare(vm, candidate, declaration, name.encode(), corrupt_signature=corrupt)
            h.check(name, reply['status'] == status and reply['active'] and reply['epoch'] == 1
                    and reply['installed'] == sha(initial).hex(), reply)
        h.values(vm, 0x80000000, int.from_bytes(b'ABxy', 'little'), 'failed candidates left live bits unchanged')
        declaration = manifest('reset')
        before, reply = h.prepare(vm, target, declaration, b'signed-reset')
        h.check('active-data plus f32 reset prepared', reply['status'] == 0 and reply['state'] == 3
                and reply['epoch'] == 1 and reply['installed'] == sha(initial).hex(), reply)
        h.commit(vm, before, target, declaration, b'signed-reset', test_tamper=True)
        h.values(vm, 1, int.from_bytes(b'EFxy', 'little'), 'reset published exact bits')
        target = state_module(True, revision=1)
        declaration = manifest('preserve')
        before, reply = h.prepare(vm, target, declaration, b'signed-preserve')
        h.check('healthy code-only preserve prepared', reply['status'] == 0 and reply['state'] == 3, reply)
        h.commit(vm, before, target, declaration, b'signed-preserve')
        h.values(vm, 1, int.from_bytes(b'EFxy', 'little'), 'preserve retained exact bits')
        target = state_module(False, revision=2)
        declaration = manifest('migrate')
        before, reply = h.prepare(vm, target, declaration, b'signed-migrate')
        h.check('healthy signed memory and f32 migration prepared', reply['status'] == 0 and reply['state'] == 3, reply)
        h.commit(vm, before, target, declaration, b'signed-migrate')
        h.values(vm, 1, int.from_bytes(b'EFxy', 'little'), 'migrate retained signed old fields over target initializers')
        h.stop(vm)


def budget(h):
    with Instance(h, 'budget-state', state_module(pages=1025)) as vm:
        h.launch(vm)
        before, reply = h.prepare(vm, state_module(True, pages=1025), manifest('reset'), b'reset-memory-budget')
        h.check('actual private reset over64MiB is exhausted', reply['status'] == 9 and reply['active']
                and reply['epoch'] == 1 and reply['installed'] == before['installed'], reply)
        h.values(vm, 0x80000000, int.from_bytes(b'ABxy', 'little'), 'budget refusal left real state unchanged')
        h.stop(vm)


def default_terminal_fault(h, name, image, expected, category, reason, *, features=()):
    with Instance(h, name, image, features=features) as vm:
        h.launch(vm)
        facts = None
        timed_out = False
        try:
            vm.child.wait(timeout=3)
        except subprocess.TimeoutExpired:
            timed_out = True
            facts = request(vm.host, 0)
            # Preserve the actual post-fault state before explicitly retiring
            # this isolated fixture. A containment75 is never a fault-exit pass.
            vm.host.close()
            vm.child.wait(timeout=5)
        error = vm.error_path.read_text(errors='replace')
        vm.fault_probe = dict(no_pause_option=True, alive_after_three_seconds=timed_out,
            facts_before_cleanup=facts, exit_after_explicit_disconnect=vm.child.returncode if timed_out else None,
            exit_without_intervention=vm.child.returncode if not timed_out else None,
            stderr=error)
        h.check(f'default fault terminates{expected} without host intervention', not timed_out and vm.child.returncode == expected
                and '[crash] ' + category in error and f'Exit status: {expected}' in error
                and 'Reason: ' + reason in error
                and 'Fault action: terminate execution' in error, vm.fault_probe)


def fault_default(h):
    default_terminal_fault(h, 'fault-default', state_module(fault=True), 122, 'memory-protection', 'memory tag mismatch')


def fault_uncaught(h):
    default_terminal_fault(h, 'fault-uncaught', state_module(uncaught=True), 121, 'execution', 'tag: uncatched wasm exception',
                           features=('--wasm-feature-enable-exceptions',))


def fault_pause(h):
    initial = state_module(fault=True)
    with Instance(h, 'faulted-epoch', initial, pause=True) as vm:
        _, launch = h.launch(vm)
        # The production update profile owns a private cooperative pause domain,
        # managed entry ledger and unwind boundary, but no interactive observer.
        # Its fallback fault pause SIGSTOPs the whole process. An explicit
        # supervisor SIGCONT then permits terminal unwind, not Guest retry.
        deadline = time.monotonic() + 5
        stopped_pid, wait_status = 0, None
        while time.monotonic() < deadline:
            stopped_pid, wait_status = os.waitpid(vm.child.pid, os.WUNTRACED | os.WNOHANG)
            if stopped_pid:
                break
            time.sleep(.002)
        stopped = bool(stopped_pid and os.WIFSTOPPED(wait_status))
        if stopped_pid and not stopped:
            vm.child.returncode = os.waitstatus_to_exitcode(wait_status)
        h.check('kernel reports whole process SIGSTOP', stopped and os.WSTOPSIG(wait_status) == signal.SIGSTOP,
                dict(pid=stopped_pid, wait_status=wait_status, stopped=stopped))
        error = vm.error_path.read_bytes()
        h.check('pause has actual MTE122 human report', b'[crash] memory-protection' in error
                and b'Reason: memory tag mismatch' in error
                and b'Exit status: 122' in error and b'Fault action: pause for the authorized host' in error, error.decode(errors='replace'))
        os.kill(vm.child.pid, signal.SIGCONT)
        deadline = time.monotonic() + 5
        while True:
            facts = request(vm.host, 0)
            if facts['state'] == 9 or time.monotonic() >= deadline:
                break
            time.sleep(.002)
        h.check('supervisor continuation leaves faulted epoch closed', facts['status'] == 0 and facts['state'] == 9 and not facts['active']
                and facts['epoch'] == 1 and vm.child.poll() is None, facts)
        for kind in ('reset', 'preserve', 'migrate'):
            target = state_module(kind != 'preserve')
            _, reply = h.prepare(vm, target, manifest(kind), ('fault-' + kind).encode())
            h.check('fault denies signed ' + kind, reply['status'] == 8 and reply['state'] == 9
                    and not reply['active'] and reply['epoch'] == 1 and reply['installed'] == sha(initial).hex(), reply)
        reply = request(vm.host, 2)
        h.check('fault cannot reach READY', reply['status'] == 2 and reply['state'] == 9 and not reply['active'], reply)
        reply = request(vm.host, 4)
        h.check('private cancel cannot revive fault', reply['status'] == 2 and reply['state'] == 9 and not reply['active'], reply)
        reply = request(vm.host, 9, launch)
        h.check('old launch receipt cannot revive fault', reply['status'] == 0 and reply['state'] == 9
                and not reply['active'] and reply['epoch'] == 1, reply)
        reply = request(vm.host, 5, struct.pack('<III', 1, 4, 4) + struct.pack('<I', 0))
        h.check('fault cannot invoke business export', reply['status'] == 8 and reply['state'] == 9 and not reply['active'] and not reply['result'], reply)
        reply = request(vm.host, 6)
        h.check('ordinary stop cannot treat fault as healthy', reply['status'] == 8 and reply['state'] == 9 and not reply['active'], reply)
        vm.fault_probe = dict(kernel_stop=wait_status, supervisor_continued_terminal_unwind=True,
            final_facts=reply, expected_disconnect_exit=75,
            meaning='Private update management remains available after terminal Guest unwind; no new epoch or Guest continuation is opened. Containment disconnect75 is separate from the recorded fault category122.')
        vm.host.close()
        vm.child.wait(timeout=5)
        h.check('explicit owner disconnect ends fault containment75', vm.child.returncode == 75, vm.fault_probe)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--vm', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--engine', choices=('aot', 'int'), default='aot')
    parser.add_argument('--runner', default='')
    parser.add_argument('--scenario', choices=('all', 'standalone', 'state-update', 'budget', 'fault-default', 'fault-uncaught', 'fault-pause'), default='all')
    args = parser.parse_args()
    args.out.mkdir(parents=True, exist_ok=False)
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    before = hashlib.sha256(args.vm.read_bytes()).hexdigest()
    sources = [Path(__file__), Path(__file__).with_name('run_signed_update_boundaries.py')]
    source_hashes = {str(path.resolve()): sha(path.read_bytes()).hex() for path in sources}
    h = Harness(args)
    selected = {'standalone': standalone, 'state-update': state_update, 'budget': budget,
                'fault-default': fault_default, 'fault-uncaught': fault_uncaught, 'fault-pause': fault_pause}
    selected = selected if args.scenario == 'all' else {args.scenario: selected[args.scenario]}
    for name, run in selected.items():
        h.current = name
        try:
            run(h)
            h.completed.append(name)
        except (AssertionError, OSError, ValueError, EOFError, subprocess.TimeoutExpired) as error:
            if not isinstance(error, AssertionError):
                h.rows.append(dict(scenario=name, name='transport/operation', passed=False, detail=repr(error)))
                print(name, 'transport/operation FAIL', repr(error), flush=True)
    report = dict(binary=str(args.vm.resolve()), binary_sha256=before, engine=args.engine, runner=h.runner,
        test_source_sha256=source_hashes,
        scenario=args.scenario, cpu_affinity=sorted(os.sched_getaffinity(0)), expected_scenarios=list(selected),
        completed_scenarios=h.completed, passed=sum(row['passed'] for row in h.rows), total=len(h.rows),
        cases=h.rows, processes=h.processes, complete=len(selected) == len(h.completed) and all(row['passed'] for row in h.rows),
        scope='Actual VM input and execution; signed update success requires real authenticated replies, publication and exact f32/data bits. QEMU, unsupported backend and supervisor cleanup are recorded independently.')
    assert hashlib.sha256(args.vm.read_bytes()).hexdigest() == before
    assert all(sha(path.read_bytes()).hex() == source_hashes[str(path.resolve())] for path in sources)
    (args.out / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({key: report[key] for key in ('binary_sha256', 'passed', 'total', 'complete')}), flush=True)
    return 0 if report['complete'] else 1


if __name__ == '__main__':
    raise SystemExit(main())
