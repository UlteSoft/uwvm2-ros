"""Real LLVM-AOT VM/guest regression with an explicitly synthetic protocol peer.

This verifies the actual permission/import/memory-copy path. The peer is NOT an
official simulator, ROS driver, or robot, and this test grants no such acceptance.
Run only inside an admitted shared 64 GiB, zero-swap job. Do not ship as an SDK
application example or use its fixture descriptors as a production deployment.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import socket
import struct
import subprocess
import sys
import tempfile
import threading
import time
import uuid


def digest(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def write(path, value):
    path.write_text(json.dumps(value, indent=2)+'\n')


class ProtocolFixture:
    def __init__(self, path, mode):
        self.path, self.mode = path, mode
        self.requests, self.responses, self.errors = [], [], []
        self.goal = uuid.uuid4().bytes
        self.command = None
        self.targets = [100, -100]
        self.binding = None
        self.sequence = 0
        self.stop = threading.Event()
        self.listener = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.listener.bind(str(path))
        path.chmod(0o600)
        self.listener.listen(1)
        self.listener.settimeout(0.1)
        self.thread = threading.Thread(target=self.serve, daemon=True)

    def state(self, stale=False):
        self.sequence += 1
        observed = time.monotonic_ns() - (1_000_000_000 if stale else 0)
        data = bytearray(96)
        struct.pack_into('<4sHHIIQQQQ', data, 0, b'URP1', 1, 2, 96, 2,
                         1, self.sequence, observed, observed+250_000_000)
        for i, value in enumerate(self.targets):
            struct.pack_into('<IIq', data, 64+16*i, i, 1, value)
        return bytes(data)

    @staticmethod
    def receive(connection, count):
        data = b''
        while len(data) < count:
            chunk = connection.recv(count-len(data))
            if not chunk:
                if not data:
                    return None
                raise RuntimeError('Truncated actual VM protocol request')
            data += chunk
        return data

    def serve(self):
        try:
            connection = None
            while not self.stop.is_set():
                try:
                    connection = self.listener.accept()[0]
                    break
                except TimeoutError:
                    continue
            if connection is None:
                return
            with connection:
                connection.settimeout(10)
                while True:
                    header = self.receive(connection, 40)
                    if header is None:
                        return
                    magic, version, operation, size, status, number, session, deadline = struct.unpack('<4sHHIIQQQ', header)
                    assert magic == b'UPN1' and version == 1 and status == 0 and 168 <= size <= 4264
                    packet = self.receive(connection, size)
                    assert packet is not None
                    binding, body = packet[:168], packet[168:]
                    if self.binding is None:
                        assert operation == 1
                        self.binding = binding
                    assert binding == self.binding and number == len(self.requests)+1 and session
                    assert time.monotonic_ns() < deadline
                    self.requests.append({'operation': operation, 'number': number, 'session': session,
                                          'deadline': deadline, 'binding_hex': binding.hex(), 'body_hex': body.hex()})
                    if operation == 1:
                        assert not body
                        response = b''
                    elif operation == 2:
                        assert not body
                        response = self.state(self.mode == 'stale')
                    elif operation == 3:
                        assert self.command is None and len(body) == 112
                        magic, version, kind, total, count, epoch, command, baseline = struct.unpack_from('<4sHHIIQQQ', body)
                        assert (magic, version, kind, total, count, epoch, command, baseline) == (b'URP1', 1, 3, 112, 2, 1, 1, 1)
                        assert body[40:72] == binding[32:64] and struct.unpack_from('<Q', body, 72)[0] == 1_000_000_000
                        target = []
                        for i in range(2):
                            slot, flags, value = struct.unpack_from('<IIq', body, 80+16*i)
                            assert slot == i and flags == 0 and value == self.targets[i]+1000
                            target.append(value)
                        self.command, self.targets = body, target
                        response = self.goal + struct.pack('<QIi', time.monotonic_ns(), 1, 0)
                    elif operation == 4:
                        assert self.command is not None and body == self.goal+struct.pack('<Q', 1)
                        # The native terminal confirmation is distinct from the
                        # subsequent read_state response written to guest memory.
                        terminal_observed = time.monotonic_ns()
                        confirmation = self.state()
                        response = self.goal+struct.pack('<QIi', terminal_observed, 2, 0)+confirmation
                    else:
                        raise AssertionError(('Unexpected native operation', operation))
                    self.responses.append({'operation': operation, 'body_hex': response.hex()})
                    reply = bytearray(header)
                    struct.pack_into('<I', reply, 8, 168+len(response))
                    connection.sendall(reply+binding+response)
        except BaseException as error:
            self.errors.append(repr(error))
        finally:
            self.listener.close()


def case(binary, sdk, directory, mode):
    directory.mkdir()
    wasm = sdk / 'lib/platform/ros/portable/v0.1.0/joint-step.wasm'
    from guest_feedback import WASM_SHA256, JointFeedback, snapshot
    JointFeedback(sdk, wasm)  # Require the actual existing frozen source and bytes.
    assert digest(wasm) == WASM_SHA256
    run_id = uuid.uuid4().hex
    with tempfile.TemporaryDirectory(prefix='uwvm-state-', dir='/tmp') as scratch:
        sock = Path(scratch) / 'native.sock'
        fixture = None if mode in ('denied', 'wrong-module') else ProtocolFixture(sock, mode)
        resources = {'joint_state_topic': '/fixture/joint_states', 'position_action': '/fixture/arm/follow_joint_trajectory',
                     'controller_status_service': '/fixture/controller_manager/list_controllers'}
        grant = {'format': 'uwvm-portable-robot-grant-v1', 'alias': 'robot', 'module': 'main',
                 'capability': 'joint.position.bounded.v1', 'wasm_sha256': WASM_SHA256,
                 'policy_epoch': 2, 'rights': 15, 'resources': resources,
                 'native': {'socket': str(sock), 'uid': os.getuid(), 'executable_sha256': digest(sys.executable),
                            'provenance_sha256': digest(__file__), 'generation': 3}}
        policy = directory / 'fixture-grant.json'
        write(policy, grant)
        adapter = {'format': 'uwvm-portable-robot-adapter-v1', 'contract_version': '0.1.0',
                   'platform_id': 'runtime_protocol_fixture', 'execution_class': 'software_simulation',
                   'capability': 'joint.position.bounded.v1', 'frame': 'joint_space', 'mapping_epoch': 1,
                   'policy_sha256': digest(policy), 'resources': resources,
                   'official_source': {'repository': 'https://example.invalid/runtime-regression-only',
                       'commit': '0'*40, 'license': 'fixture', 'mode': 'synthetic-protocol-fixture',
                       'scope': 'No official simulator, native ROS, or robot acceptance'},
                   'ros': {'release': 'fixture', 'os_abi': 'linux-x86_64', 'rmw': 'fixture',
                           'transport': 'local-regression-fixture', 'security': 'fixture'},
                   'joints': [{'slot': i, 'native_joint': 'fixture_joint_'+str(i), 'kind': 'bounded_revolute',
                       'source_unit': 'rad', 'sign': 1, 'zero_offset_microrad': 0,
                       'lower_microrad': -1_000_000, 'upper_microrad': 1_000_000,
                       'max_step_microrad': 1000, 'tolerance_microrad': 0} for i in range(2)]}
        descriptor = directory / 'fixture-adapter.json'
        write(descriptor, adapter)
        argv = [str(binary), '--runtime-aot', '--wasm-set-main-module-name',
                'ungranted-module' if mode == 'wrong-module' else 'main']
        if mode != 'disabled':
            argv += ['--portable-robot-audit-run', run_id]
        if mode == 'denied':
            argv += ['--portable-robot-audit-inputs', str(descriptor), str(policy)]
        else:
            argv += ['--portable-robot', str(descriptor), digest(descriptor), str(policy), digest(policy)]
        argv += ['--run', str(wasm)]
        if fixture:
            fixture.thread.start()
        started = time.monotonic_ns()
        try:
            with (directory / 'vm.stdout').open('xb') as stdout, (directory / 'vm.stderr').open('xb') as stderr:
                child = subprocess.Popen(argv, stdout=stdout, stderr=stderr)
                try:
                    status = child.wait(timeout=180)
                finally:
                    if child.poll() is None:
                        child.kill()
                        child.wait()
            finished = time.monotonic_ns()
            write(directory / 'execution.json', {'argv': argv, 'pid': child.pid, 'exit_code': status,
                  'runtime_sha256': digest(binary), 'wasm_sha256': digest(wasm),
                  'cgroup': Path('/proc/self/cgroup').read_text(), 'started_ns': started, 'finished_ns': finished})
        finally:
            if fixture:
                fixture.stop.set()
                fixture.thread.join(timeout=12)
                write(directory / 'actual-native-protocol.json', {'requests': fixture.requests,
                      'responses': fixture.responses, 'errors': fixture.errors,
                      'scope': 'Synthetic protocol fixture; real VM and Wasm path only'})
        assert fixture is None or not fixture.thread.is_alive(), 'Native fixture did not retire'
        assert fixture is None or not fixture.errors, fixture.errors if fixture else None
        expected = 13 if mode in ('denied', 'wrong-module') else 11 if mode == 'stale' else 0
        assert status == expected, (mode, status, expected)
        rows = [json.loads(line) for line in (directory / 'vm.stdout').read_text().splitlines() if line.startswith('{')]
        for row in rows:
            assert row['run_id'] == run_id and row['pid'] == child.pid
            assert started <= row['monotonic_ns'] <= finished
        states = [row for row in rows if row.get('format') == 'uwvm-sdk-simulation-state-delivery-v1']
        if mode in ('disabled', 'stale', 'denied', 'wrong-module'):
            assert not states
            if mode == 'disabled':
                assert not rows
            if mode in ('denied', 'wrong-module'):
                assert len(rows) == 1 and rows[0]['decision'] == 'DENY'
        else:
            assert len(states) == 2 and len(rows) == 5
            before, after = (snapshot(row) for row in states)
            assert before['positions'] == [100, -100] and after['positions'] == [1100, 900]
            delivered = [bytes.fromhex(row['body_hex']) for row in fixture.responses if row['operation'] == 2]
            assert [row['payload_hex'] for row in states] == [data.hex() for data in delivered]
            assert states[0]['native_operation_id'] is None and 'guest_command_sha256' not in states[0]
            assert states[1]['native_operation_id'] == fixture.goal.hex()
            assert states[1]['guest_command_sha256'] == hashlib.sha256(fixture.command).hexdigest()
            assert states[1]['command_id'] == 1 and states[1]['command_outcome'] == 2
            assert states[0]['permission_session'] == states[1]['permission_session'] > 0
            assert states[0]['call_sequence'] < states[1]['call_sequence']
        write(directory / 'result.json', {'status': 'PASS', 'mode': mode, 'state_copies': len(states),
            'scope': 'Actual LLVM-AOT VM import/guest-memory regression with synthetic native protocol; not official simulation'})
        print('PASS', mode, flush=True)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--vm', type=Path, required=True)
    parser.add_argument('--sdk', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    assert '/uwvmros.slice/' in Path('/proc/self/cgroup').read_text()
    ancestor = Path('/sys/fs/cgroup/uwvmros.slice')
    assert (ancestor/'memory.max').read_text().strip() == '68719476736'
    assert (ancestor/'memory.swap.max').read_text().strip() == '0'
    args.output.mkdir()
    sys.path.insert(0, str(args.sdk/'tools/shared/python'))
    for mode in ('success', 'disabled', 'denied', 'wrong-module', 'stale'):
        case(args.vm, args.sdk, args.output/mode, mode)


if __name__ == '__main__':
    main()
