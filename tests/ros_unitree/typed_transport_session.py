#!/usr/bin/env python3
"""Actual UTM1/UTN1 sessions for the fourteen same-main typed commands.

This module never invents a native observation or launches a replacement DDS
client. Its only inputs are pipe bytes from the real retained Runtime and the
independently compiled native generated-message witness. The caller admits
all executables, source receipts, complete file catalogs and loopback policy
before calling :func:`run_case`. Merely importing this module executes nothing.
"""
from __future__ import annotations

import os
from pathlib import Path
import re
import select
import stat
import struct
import time


def require(value, reason):
    if not value:
        raise ValueError(reason)


class Lines:
    """Consume bounded logged binary lines without a procfs walk per byte.

    Actors logs the actual nonblocking pipe before we inspect it. We find the
    next newline in that log, then advance the owner's binary read offset once.
    Unexpected trailing bytes are rejected by ``finish`` after actual exit.
    """
    def __init__(self, owner, child):
        self.owner, self.child = owner, child
        self.row = next(row for actor, row in owner.rows if actor is child)
        require(self.row['stdout_protocol'] == 'binary', 'binary protocol actor required')
        self.start = owner.read_offsets[child.pid]
        self.consumed = bytearray()

    def read(self):
        while True:
            self.owner.check()
            offset = self.owner.read_offsets[self.child.pid]
            with Path(self.row['stdout']).open('rb') as stream:
                stream.seek(offset)
                raw = stream.read(4097)
            newline = raw.find(b'\n')
            if newline >= 0:
                require(newline < 4096, 'typed protocol line exceeds native bound')
                value = self.owner.receive_bytes(self.child, newline + 1)
                require(value == raw[:newline + 1] and b'\r' not in value and b'\0' not in value,
                        'typed protocol must contain exact ordinary lines')
                self.consumed.extend(value)
                require(len(self.consumed) <= 512 * 1024, 'finite typed protocol transcript required')
                return value.decode('ascii', 'strict').removesuffix('\n')
            require(len(raw) <= 4096 and self.child.poll() is None,
                    'typed actor exited or left an oversized/truncated record')
            select.select([self.child.stdout], [], [], .01)

    def expect(self, expected):
        value = self.read()
        require(value == expected, 'actual typed record differs: expected %r, observed %r' % (expected, value))
        return value

    def finish(self, prefix=b''):
        """Require the complete actual transcript, not just a matching prefix."""
        self.owner.check()
        raw = Path(self.row['stdout']).read_bytes()
        require(raw == prefix + bytes(self.consumed), 'unexpected typed actor output after protocol completion')
        require(not Path(self.row['stderr']).read_bytes(), 'typed actor emitted stderr')
        return {'stdout': self.owner.frame.pin(self.row['stdout']),
                'stderr': self.owner.frame.pin(self.row['stderr']),
                'consumed_lines': bytes(self.consumed).count(b'\n')}


def words(line, marker, count):
    """Parse only canonical unsigned decimal32 words from a closed record."""
    parts = line.split(' ')
    require(len(parts) == count + 1 and parts[0] == marker and
            all(re.fullmatch(r'0|[1-9][0-9]{0,9}', part) for part in parts[1:]),
            'typed native/Guest numeric record shape differs')
    result = [int(part) for part in parts[1:]]
    require(all(value <= 0xffffffff for value in result), 'typed word exceeds32 bits')
    return result


def carrier(slots):
    """Independent binary32 oracle for every carrier; no wire codec is used.

    These fractions are exactly representable, so every one of60/105 words
    must agree without a tolerance. The native witness additionally compares
    the *complete generated message*, including all default nested fields.
    """
    require(slots in (20, 35), 'closed actual GO/HG motor count required')
    return [struct.unpack('<I', struct.pack('<f', value))[0]
            for index in range(1, slots + 1)
            for value in (index / 32, -index / 64, index / 128)]


def quiet_record(value, family, positive):
    parts = value.split(' ')
    require(len(parts) == 7 and parts[0] == 'NATIVE_TYPED_QUIET' and
            all(re.fullmatch(r'0|[1-9][0-9]{0,19}', part) for part in parts[1:]),
            'closed actual quiet record required')
    actual = list(map(int, parts[1:]))
    expected = [0, 0, 0] if not positive else ([0, 1, 1] if family == 3 else [1, 0, 0])
    require(actual[0] == family and 30 <= actual[1] <= 16384 and
            2_000_000_000 <= actual[2] <= 10_000_000_000 and actual[3:] == expected,
            'independent native graph/payload quiet witness differs')
    return {'family': family, 'samples': actual[1], 'elapsed_ns': actual[2],
            'states': actual[3], 'requests': actual[4], 'responses': actual[5]}


def poll(owner, worker, lines, marker):
    """Request at most4096 *actual* SDK polls with a finite10-second deadline."""
    until = time.monotonic() + 10
    for attempt in range(4096):
        require(time.monotonic() < until, 'actual typed Guest polling deadline exhausted')
        owner.write(worker, b'R')
        value = lines.read()
        if value != 'TYPED_AGAIN':
            require(value.startswith(marker + ' '), 'actual typed successful poll record differs')
            return value, attempt
        time.sleep(.001)
    raise ValueError('actual typed Guest poll count exhausted')


def runtime_argv(api, frame, product, worker_profile, positive, connection, grants, socket_path, *, libraries=()):
    """Grant basic stdio and, only for positive cases, exact ROS2 aliases.

    The dual HG Worker keeps ROS1 globally and individually disabled. There
    are no preloads, mounts, clocks, randomness, system environment, raw network
    rights, service operations, LowCmd aliases or generic guest argv suffixes.
    The entry is the observed local void export of the *same* actual module.
    """
    require(not libraries or (positive and worker_profile=='ros2' and
            tuple(name for name,_ in libraries)==('callbacks','motion','perception')),
            'only the explicit positive Direct pure graph may add providers')
    argv = api.runtime_argv(frame, worker_profile, Path(product['artifact']['path']), libraries=libraries)
    require(argv[-2:] == ['--run', product['artifact']['path']], 'one actual main artifact required')
    extra = ['--wasm-set-start-func', str(product['boundary']['local_entry'])]
    if positive:
        disabled = [index for index in range(len(argv) - 1)
                    if argv[index:index + 2] == ['--ros2-single-disable', 'main']]
        require(len(disabled) == 1 and grants, 'one explicit main ROS2 policy required')
        argv[disabled[0]] = '--ros2-single-enable'
        settings = {**connection, 'gateway-socket': str(socket_path), 'gateway-uid': str(os.getuid()),
                    'connect-timeout-ms': '10000', 'audit-deny-max-rate-hz': '100',
                    'audit-deny-burst': '16', 'audit-deny-max-records': '10000',
                    'audit-deny-max-bytes': '1048576'}
        for key, value in settings.items():
            extra += ['--ros2-single-' + key, 'main', value]
        for grant in grants:
            extra += ['--ros2-single-grant', 'main',
                      *[grant[key] for key in ('alias', 'operation', 'resource', 'type', 'identity')]]
            for key, value in grant['limits'].items():
                extra += ['--ros2-single-set', 'main', grant['alias'], key, value]
    else:
        require(not grants, 'default-denied case cannot acquire any alias')
    return [*argv[:-2], *extra, *argv[-2:]]


def run_case(api, native, frame, admitted, product, positive):
    """Run one fresh owned DDS lifecycle; report PASS only after actual exits.

    A live held Runtime is mapped after READY_TYPED_MAIN and before UTM1, so
    every language initializer, SDK call and callback remains in that exact
    main module. Gate proof establishes the actual exec target and PID used
    for Gateway SO_PEERCRED, rather than a guessed future child PID.
    """
    family = product['recipe']['family_code']
    chosen = admitted['families'][product['recipe']['family']]
    label = product['recipe']['id'] + ('-positive' if positive else '-denied')
    # UNIX sun_path is bounded independently of ordinary log filenames. All
    # cases are serial and each owns a fresh numeric name inside Frame.out;
    # never create an unowned /tmp alias or silently truncate a product name.
    case_index = len(frame.report['cases'])
    require(0 <= case_index < 28, 'closed actual serial case index required')
    socket_path = frame.out / ('s%02d' % case_index)
    require(len(os.fsencode(socket_path)) < 104 and not socket_path.exists(), 'fresh short owned socket required')
    owner = frame.unitree.Actors(frame)
    peer, _ = owner.start(label + '-peer', [admitted['peer']['path']],
                          json_output=False, environment=admitted['environment'])
    native_lines = Lines(owner, peer)
    owner.write(peer, b'UTN1' + struct.pack('<III', family, int(positive), 0))
    native_lines.expect('READY_NATIVE_TYPED_V1 %d %d' % (family, positive))
    peer_mapping = native.mappings(api, frame, owner, peer.pid, Path(admitted['peer']['path']),
        chosen['peer_mappings'], admitted['file_catalog'], admitted['elf_catalog'])
    connection, grants = admitted['policy'](chosen, positive)
    argv = runtime_argv(api, frame, product, chosen['worker_profile'], positive, connection, grants, socket_path)
    worker, binding, release = native.gated_worker(api, frame, owner, label + '-worker', admitted, argv)
    gateway, gateway_row, gateway_proof, config_pin = None, None, None, None
    if positive:
        config = frame.out / (label + '.conf')
        with config.open('x', encoding='utf-8') as stream:
            stream.write(native.configuration(2, worker.pid, connection, grants, socket_path))
        config.chmod(0o600)
        config_pin = frame.pin(config)
        gateway, gateway_row = owner.start(label + '-gateway', [chosen['gateway']['path'], str(config)],
            json_output=False, environment=admitted['environment'])
        until = time.monotonic() + 20
        while not socket_path.exists():
            owner.check()
            require(gateway.poll() is None and time.monotonic() < until, 'actual typed Gateway readiness failed')
            time.sleep(.01)
        facts = socket_path.lstat()
        require(stat.S_ISSOCK(facts.st_mode) and facts.st_uid == os.getuid(), 'actual owned Gateway socket required')
        gateway_proof = native.mappings(api, frame, owner, gateway.pid, Path(chosen['gateway']['path']),
            chosen['gateway_mappings'], admitted['file_catalog'], admitted['elf_catalog'])
    release()
    guest_lines = Lines(owner, worker)
    guest_lines.expect('READY_TYPED_MAIN')
    # Count a real observed entry even if a later protocol/mapping check fails.
    # Completed successful cases remain a separate inventory, so a failed run
    # cannot hide an executed initializer by leaving its completed count zero.
    frame.report['actual_Runtime_cases_executed'] += 1
    frame.save()
    binding['live_actual_Worker_before_UTM1'] = native.mappings(api, frame, owner, worker.pid,
        frame.workers[chosen['worker_profile']], chosen['worker_mappings'],
        admitted['worker_file_catalog'], admitted['worker_elf_catalog'])
    owner.write(worker, b'UTM1' + struct.pack('<III', family, int(positive), 0))
    guest_lines.expect('TYPED_INITIALIZED %d' % positive)
    if positive:
        gateway_proof['after_actual_ROS_context'] = native.mappings(api, frame, owner, gateway.pid,
            Path(chosen['gateway']['path']), chosen['gateway_context_mappings'],
            admitted['file_catalog'], admitted['elf_catalog'])
    else:
        guest_lines.expect('TYPED_DENIED_VERIFIED')
    owner.write(peer, b'M')
    native_lines.expect('NATIVE_TYPED_MATCHED %d %d' % (family, positive))
    observations, retries = None, 0
    if not positive:
        owner.write(peer, b'Q')
        quiet_proof = quiet_record(native_lines.read(), family, False)
    elif family in (1, 2):
        slots = 20 if family == 1 else 35
        owner.write(peer, b'P')
        native_value = words(native_lines.read(), 'NATIVE_TYPED_STATE', 4 + 3 * slots)
        actual, retries = poll(owner, worker, guest_lines, 'TYPED_STATE')
        guest_value = words(actual, 'TYPED_STATE', 3 + 3 * slots)
        expected = [42, 0x13572468, slots, *carrier(slots)]
        require(native_value == [family, *expected] and guest_value == expected,
                'complete actual native/Guest GO/HG carrier words differ from independent fixture')
        observations = {'native': native_value, 'guest': guest_value, 'all_carrier_words': 3 * slots,
                        'native_full_generated_message_equality': True}
    else:
        owner.write(worker, b'S')
        guest_lines.expect('TYPED_SUBMITTED')
        owner.write(peer, b'A')
        native_lines.expect('NATIVE_TYPED_RPC 1 1 0 typed-example-ack')
        actual, retries = poll(owner, worker, guest_lines, 'TYPED_RPC')
        require(words(actual, 'TYPED_RPC', 3) == [0, 1, 0], 'real generated Response/callback result differs')
        observations = {'native_request_id': 1, 'native_api_id': 1, 'parameter': '{}', 'binary': [],
                        'native_response_status': 0, 'native_response_data': 'typed-example-ack',
                        'guest_response_status': 0, 'guest_count': 1, 'guest_denied': 0,
                        'native_full_generated_Request_and_Response_equality': True}
    if positive:
        owner.write(worker, b'C')
        guest_lines.expect('TYPED_CLOSED')
        owner.write(peer, b'Z')
        quiet_proof = quiet_record(native_lines.read(), family, True)
    owner.write(worker, b'F')
    guest_lines.expect('PASS_TYPED_CASE')
    worker_row = api.finish_actor(frame, owner, worker, serial=False)
    guest_log = guest_lines.finish(binding['gate_stdout'].encode('ascii'))
    owner.write(peer, b'F')
    native_lines.expect('PASS_NATIVE_TYPED_WITNESS %d %d' % (family, positive))
    peer_row = api.finish_actor(frame, owner, peer, serial=False)
    peer_log = native_lines.finish()
    audit = None
    if gateway is not None:
        until = time.monotonic() + 10
        while gateway.poll() is None and time.monotonic() < until:
            owner.check()
            time.sleep(.01)
        require(gateway.poll() == 0, 'actual Gateway must retire normally after its Worker exits')
        gateway_row = api.finish_actor(frame, owner, gateway, serial=False)
        require(not Path(gateway_row['stdout']).read_bytes(), 'unexpected typed Gateway stdout')
        audit = native.gateway_audit(api, frame, gateway_row, 2, worker.pid)
    api.terminal(frame, owner)
    cleanup = owner.close()
    require(cleanup['status'] == 'PASS', 'typed case owned cleanup incomplete')
    # Frame.finish must not close already-closed pipe/log handles a second time.
    frame.actors.remove(owner)
    result = {'case': label, 'status': 'PASS_ACTUAL_TYPED_DDS_CASE', 'product': product['recipe']['id'],
        'positive': positive, 'family': product['recipe']['family'], 'same_main': True,
        'artifact': product['artifact'], 'runtime_argv': argv, 'worker': binding,
        'guest_log': guest_log, 'native_log': peer_log, 'native_mapping': peer_mapping,
        'gateway_mapping': gateway_proof, 'gateway_config': config_pin, 'gateway_audit': audit,
        'native_effects': observations, 'native_quiet': quiet_proof, 'poll_retries': retries,
        'worker_exit': worker_row['returncode'], 'native_exit': peer_row['returncode'], 'cleanup': cleanup,
        'default_authority': [], 'positive_aliases': grants, 'physical_robot_qualification': False}
    frame.report['cases'].append(result)
    frame.save()
    return result
