#!/usr/bin/env python3
"""Execute owned ROS1 legacy TCPROS sessions, never a substitute ROS2 client.

The caller admits the complete eleven-message native provider, six same-main
Guest commands, both actual interpreter profiles and all execution catalogs.
This module observes real pipe bytes and opened process descriptors. A build
receipt or a printed PID alone cannot complete any of the twelve cases.
"""
from __future__ import annotations

import http.client
import json
import os
from pathlib import Path
import re
import select
import signal
import socket
import stat
import struct
import time
import xmlrpc.client


def require(value, reason):
    if not value:
        raise ValueError(reason)


class Lines:
    """Consume finite ASCII records from the owner's original binary pipe log.

    Stderr is retained separately: the native witness must leave it empty,
    whereas the Runtime's real audit records and rosmaster diagnostics are
    independently inspected after their actual processes have terminated.
    """
    def __init__(self, owner, child):
        self.owner, self.child = owner, child
        self.row = next(row for actor, row in owner.rows if actor is child)
        require(self.row['stdout_protocol'] == 'binary', 'original binary pipe owner required')
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
                require(newline < 4096, 'legacy protocol line exceeds4096 bytes')
                value = self.owner.receive_bytes(self.child, newline + 1)
                require(value == raw[:newline + 1] and b'\r' not in value and b'\0' not in value,
                        'legacy protocol requires exact ordinary lines')
                self.consumed.extend(value)
                require(len(self.consumed) <= 512 * 1024, 'legacy protocol transcript exceeds finite bound')
                return value[:-1].decode('ascii', 'strict')
            require(len(raw) <= 4096 and self.child.poll() is None,
                    'legacy actor retired or emitted a truncated/oversized record')
            select.select([self.child.stdout], [], [], .01)

    def expect(self, expected):
        actual = self.read()
        require(actual == expected, 'legacy protocol expected %r, observed %r' % (expected, actual))
        return actual

    def finish(self, api, prefix=b'', empty_stderr=False):
        self.owner.check()
        raw = api.bounded(Path(self.row['stdout']), self.owner.frame.limits['log_bytes'])
        require(raw == prefix + bytes(self.consumed), 'unexpected legacy actor trailing output')
        if empty_stderr:
            require(not api.bounded(Path(self.row['stderr']), self.owner.frame.limits['log_bytes']),
                    'native legacy witness emitted stderr')
        return {'stdout': self.owner.frame.pin(self.row['stdout']),
                'stderr': self.owner.frame.pin(self.row['stderr']),
                'consumed_lines': bytes(self.consumed).count(b'\n')}


def unsigned(text, maximum=(1 << 64) - 1):
    require(type(text) is str and re.fullmatch(r'0|[1-9][0-9]{0,19}', text), 'canonical unsigned legacy number required')
    value = int(text)
    require(value <= maximum, 'legacy numeric record exceeds its bound')
    return value


def payload(line, marker, length, crc):
    """Check the complete canonical serialization, including its CRC field.

    FNV is recomputed independently over every byte. The CRC word is a fixture
    payload field, not a claim of authenticated firmware CRC or robot safety.
    Native C++ additionally compares every generated-message field and byte to
    its separately constructed expectation before emitting this record.
    """
    parts = line.split(' ')
    require(len(parts) == 5 and parts[0] == marker and unsigned(parts[1], 1536) == length and
            unsigned(parts[2], 0xffffffff) == crc and re.fullmatch('[0-9a-f]{%d}' % (length * 2), parts[3]),
            'complete native legacy payload shape differs')
    raw = bytes.fromhex(parts[3])
    digest = 14695981039346656037
    for byte in raw:
        digest = ((digest ^ byte) * 1099511628211) & ((1 << 64) - 1)
    require(unsigned(parts[4]) == digest, 'complete native legacy payload FNV differs')
    return {'bytes': length, 'crc_field': crc, 'canonical_hex': parts[3], 'fnv1a64': digest}


def quiet(line, positive):
    parts = line.split(' ')
    require(len(parts) == 6 and parts[0] == 'LEGACY_QUIET', 'complete legacy quiet record required')
    values = [unsigned(item) for item in parts[1:]]
    require(values[0] == int(positive) and 30 <= values[1] <= 4096 and
            2_000_000_000 <= values[2] <= 10_000_000_000 and values[3:] == ([1, 1] if positive else [0, 0]),
            'actual native legacy quiet interval/callback counts differ')
    return dict(mode=values[0], samples=values[1], elapsed_ns=values[2], states=values[3], commands=values[4])


def runtime_argv(api, frame, product, profile, positive, connection, grants, socket_path):
    """Keep every optional WASI right and every ROS2 target disabled.

    The negative case adds only explicit denial-audit *budgets*. They grant no
    ROS authority and do not enable a Gateway connection or an alias. A positive
    case enables exactly main's ROS1 target and its two literal topic aliases.
    """
    argv = api.runtime_argv(frame, profile, Path(product['artifact']['path']))
    require(argv[-2:] == ['--run', product['artifact']['path']], 'one actual main command required')
    extra = ['--wasm-set-start-func', str(product['boundary']['local_entry'])]
    settings = {'audit-deny-max-rate-hz': '100', 'audit-deny-burst': '16',
                'audit-deny-max-records': '10000', 'audit-deny-max-bytes': '1048576'}
    if positive:
        positions = [index for index in range(len(argv) - 1)
                     if argv[index:index + 2] == ['--ros1-single-disable', 'main']]
        require(len(positions) == 1 and len(grants) == 2, 'one explicit main ROS1 target/two aliases required')
        argv[positions[0]] = '--ros1-single-enable'
        settings.update(connection, **{'gateway-socket': str(socket_path), 'gateway-uid': str(os.getuid()),
                                      'connect-timeout-ms': '10000'})
    else:
        require(not grants, 'default-denied instance cannot receive an alias')
    for key, value in settings.items():
        extra += ['--ros1-single-' + key, 'main', value]
    for grant in grants:
        extra += ['--ros1-single-grant', 'main',
                  *[grant[key] for key in ('alias', 'operation', 'resource', 'type', 'identity')]]
        for key, value in grant['limits'].items():
            extra += ['--ros1-single-set', 'main', grant['alias'], key, value]
    return [*argv[:-2], *extra, *argv[-2:]]


def denial_audit(api, frame, row):
    """Observe the disabled target's two real status13 records, without IPC."""
    raw = api.bounded(Path(row['stderr']), frame.limits['log_bytes'])
    records = []
    for line in raw.splitlines():
        def pairs(items):
            result = {}
            for key, value in items:
                require(key not in result, 'duplicate real Worker audit key')
                result[key] = value
            return result
        item = json.loads(line, object_pairs_hook=pairs)
        require(type(item) is dict and set(item) == {'wall_ns', 'ros', 'module', 'runtime_generation',
            'observation_instance', 'event', 'cross_module', 'error'} and item['ros'] == 'ros1' and
            item['module'] == 'main' and item['event'] == 'unresolved-denial' and
            item['cross_module'] is False and type(item['error']) is int and item['error'] == 13 and
            all(type(item[key]) is int and item[key] > 0 for key in
                ('wall_ns', 'runtime_generation', 'observation_instance')), 'actual disabled-target denial audit differs')
        records.append(item)
    require(len(records) == 2 and len({(row['runtime_generation'], row['observation_instance']) for row in records}) == 1,
            'exactly two real main-principal alias-open denials required')
    return {'stderr': frame.pin(row['stderr']), 'records': records,
            'ROS_target_enabled': False, 'Gateway_started': False, 'alias_grants': []}


def positive_audit(api, frame, row):
    """Require real main-principal create/close audit for both positive aliases."""
    records = []
    wanted = {('topic.subscribe', '/uwvm_unitree_legacy/lowstate'),
              ('topic.publish', '/uwvm_unitree_legacy/lowcmd')}
    for line in api.bounded(Path(row['stderr']), frame.limits['log_bytes']).splitlines():
        def pairs(items):
            value = {}
            for key, item in items:
                require(key not in value, 'duplicate real positive audit key')
                value[key] = item
            return value
        item = json.loads(line, object_pairs_hook=pairs)
        require(type(item) is dict and set(item) == {'time_unix_ns', 'ros_version', 'module', 'instance',
            'event', 'operation', 'resource', 'result', 'error'} and
            item['ros_version'] == 'ros1' and item['module'] == 'main' and
            (item['operation'], item['resource']) in wanted and item['event'] in
                ('resource-create', 'resource-close', 'allow', 'gateway-result', 'gateway-error') and
            type(item['error']) is int and item['error'] in (0, 11) and
            item['result'] == ('allow' if item['error'] == 0 else 'deny') and
            all(type(item[key]) is int and item[key] > 0 for key in ('time_unix_ns', 'instance')),
            'actual positive main-principal audit differs')
        records.append(item)
    require(records and len({item['instance'] for item in records}) == 1 and all(
        sum(item['event'] == event and (item['operation'], item['resource']) == role and item['error'] == 0
            for item in records) == 1 for event in ('resource-create', 'resource-close') for role in wanted),
        'exactly one actual create/close for each positive ROS1 alias required')
    return {'stderr': frame.pin(row['stderr']), 'records': records, 'main_principal': 'main',
            'unresolved_or_cross_module_denials': 0}


class LoopbackTransport(xmlrpc.client.Transport):
    """Bound every XMLRPC request and forbid proxy or alternate-host routing."""
    def __init__(self, port):
        super().__init__()
        self.port = port

    def make_connection(self, host):
        require(host == '127.0.0.1:%d' % self.port, 'master XMLRPC must remain literal IPv4 loopback')
        if self._connection[0] != host:
            self.close()
            self._connection = host, http.client.HTTPConnection('127.0.0.1', self.port, timeout=.25)
        return self._connection[1]

    def parse_response(self, response):
        """Refuse compression/chunking and bound the entire XML before parsing."""
        require(response.getheader('Content-Encoding', 'identity') == 'identity' and
                response.getheader('Transfer-Encoding') is None, 'ordinary bounded master XMLRPC response required')
        length = response.getheader('Content-Length')
        require(type(length) is str and re.fullmatch(r'0|[1-9][0-9]{0,4}', length) and
                0 < int(length) <= 65536, 'master XMLRPC response length exceeds64KiB')
        raw = response.read(65537)
        require(len(raw) == int(length), 'master XMLRPC response bytes differ from finite length')
        parser, unmarshaller = self.getparser()
        parser.feed(raw)
        parser.close()
        return unmarshaller.close()


def start_master(api, native, frame, owner, admitted, label):
    """Start the pinned Focal loader/Python/master, not the coordinator Python."""
    port = admitted['port']
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as probe:
        probe.bind(('127.0.0.1', port))
    launch = admitted['launch']['master']
    argv = [launch['executable']['path'], *[item.replace('{port}', str(port)) for item in launch['arguments']]]
    child, row = owner.start(label + '-master', argv, json_output=False, environment=admitted['environment'])
    until, reply = time.monotonic() + 20, None
    transport = LoopbackTransport(port)
    with xmlrpc.client.ServerProxy('http://127.0.0.1:%d/' % port, transport=transport,
                                   allow_none=False, use_builtin_types=True) as master:
        while time.monotonic() < until:
            owner.check()
            require(child.poll() is None, 'owned actual ROS1 master retired before readiness')
            try:
                reply = master.getPid('/uwvm_unitree_legacy/owner')
                require(type(reply) in (list, tuple) and len(reply) == 3 and reply[0] == 1 and
                        type(reply[2]) is int and reply[2] == child.pid, 'actual master XMLRPC PID differs from owned child')
                break
            except (OSError, http.client.HTTPException, xmlrpc.client.Error):
                time.sleep(.01)
        require(reply is not None and reply[2] == child.pid, 'actual loopback ROS1 master readiness exhausted')
        uri = master.getUri('/uwvm_unitree_legacy/owner')
        require(type(uri) in (list, tuple) and len(uri) == 3 and uri[0] == 1 and
                uri[2].rstrip('/') == 'http://127.0.0.1:%d' % port, 'actual master URI escaped isolated loopback')
    proof = native.mappings(api, frame, owner, child.pid, Path(launch['executable']['path']),
        admitted['mappings']['master'], admitted['file_catalog'], admitted['elf_catalog'])
    return child, row, {'argv': argv, 'getPid': reply, 'getUri': uri, 'mapping': proof}


def stop_master(api, frame, owner, child):
    """Signal only the retained same-birth pidfd; normal ROS shutdown is required."""
    owner.check()
    entries = [(key, item) for key, item in owner.owner.owned.items() if key[0] == child.pid]
    require(child.poll() is None and len(entries) == 1, 'live owned master pidfd required for shutdown')
    key, item = entries[0]
    require(frame.common.birth(frame.common.identity(child.pid)) == key, 'owned master birth changed')
    signal.pidfd_send_signal(item['fd'], signal.SIGINT)
    return api.finish_actor(frame, owner, child, serial=False)


def run_case(api, native, frame, admitted, product, positive):
    """Perform one fresh real TCPROS lifecycle and retain its complete evidence."""
    index = len(frame.report['cases'])
    require(0 <= index < 12, 'finite serial legacy case index required')
    # Twelve explicit consecutive loopback ports avoid relying on a ROS
    # distribution's server-reuse/TIME_WAIT policy. This is a fixed mapping,
    # never a port search, retry or host-network fallback.
    port = admitted['port'] + index
    require(1024 <= port <= 65535, 'actual fixed serial master port out of range')
    admitted = {**admitted, 'port': port, 'environment': {**admitted['environment'],
        'ROS_MASTER_URI': 'http://127.0.0.1:%d' % port}}
    frame.unitree.current_loopback(frame.report['isolation']['network_namespace'])
    label = product['recipe']['id'] + ('-positive' if positive else '-denied')
    socket_path = frame.out / ('s%02d' % index)
    require(len(os.fsencode(socket_path)) < 104 and not socket_path.exists(), 'fresh short owned socket required')
    owner = frame.unitree.Actors(frame)
    master, master_row, master_proof = start_master(api, native, frame, owner, admitted, label)
    connection, grants = admitted['policy'](admitted['identities'], admitted['port'], positive, admitted['codecs'])
    profile = product['worker_profile']
    argv = runtime_argv(api, frame, product, profile, positive, connection, grants, socket_path)
    worker, binding, release = native.gated_worker(api, frame, owner, label + '-worker', admitted, argv)
    binding['live_gate_before_execveat'] = native.mappings(api, frame, owner, worker.pid,
        Path(admitted['exec_gate']['path']), admitted['mappings']['gate'],
        admitted['worker_file_catalog'], admitted['worker_elf_catalog'])
    gateway, gateway_row, gateway_proof, config_pin = None, None, None, None
    if positive:
        config = frame.out / (label + '.conf')
        with config.open('x', encoding='utf-8') as stream:
            stream.write(native.configuration(1, worker.pid, connection, grants, socket_path))
        config.chmod(0o600)
        config_pin = frame.pin(config)
        gateway, gateway_row = owner.start(label + '-gateway', [admitted['gateway']['path'], str(config)],
            json_output=False, environment=admitted['environment'])
        until = time.monotonic() + 20
        while not socket_path.exists():
            owner.check()
            require(gateway.poll() is None and time.monotonic() < until, 'actual legacy Gateway readiness exhausted')
            time.sleep(.01)
        facts = socket_path.lstat()
        require(stat.S_ISSOCK(facts.st_mode) and facts.st_uid == os.getuid(), 'actual owned Gateway socket required')
        gateway_proof = native.mappings(api, frame, owner, gateway.pid, Path(admitted['gateway']['path']),
            admitted['mappings']['gateway'], admitted['file_catalog'], admitted['elf_catalog'])
    release()
    frame.report['actual_Runtime_launches_released'] += 1
    frame.save()
    guest_lines = Lines(owner, worker)
    guest_lines.expect('READY_LEGACY_MAIN')
    frame.report['actual_Runtime_cases_executed'] += 1
    frame.save()
    binding['live_actual_Worker_before_ULG1'] = native.mappings(api, frame, owner, worker.pid,
        frame.workers[profile], admitted['mappings']['worker_' + profile],
        admitted['worker_file_catalog'], admitted['worker_elf_catalog'])
    # Language initialization may be much slower than a native ROS context.
    # Start the finite30-second native input witness only after actual Guest
    # readiness; neither module has opened an alias before the following ULG1.
    peer, _ = owner.start(label + '-peer', [admitted['peer']['path']],
                          json_output=False, environment=admitted['environment'])
    peer_lines = Lines(owner, peer)
    peer_lines.expect('LEGACY_NATIVE_GATE %d' % peer.pid)
    peer_proof = native.mappings(api, frame, owner, peer.pid, Path(admitted['peer']['path']),
        admitted['mappings']['peer'], admitted['file_catalog'], admitted['elf_catalog'])
    owner.write(peer, b'ULN1' + struct.pack('<III', int(positive), 42, 0))
    for type_name in admitted['identity_order']:
        peer_lines.expect('LEGACY_NATIVE_TYPE %s %s' % (type_name, admitted['identities'][type_name]))
    peer_lines.expect('READY_NATIVE_LEGACY_V1 %d' % positive)
    peer_proof['after_actual_ROS_context'] = native.mappings(api, frame, owner, peer.pid,
        Path(admitted['peer']['path']), admitted['mappings']['peer_context'], admitted['file_catalog'], admitted['elf_catalog'])
    owner.write(worker, b'ULG1' + struct.pack('<III', 4, int(positive), 0))
    guest_lines.expect('LEGACY_INITIALIZED %d' % positive)
    if positive:
        gateway_proof['after_actual_ROS_context'] = native.mappings(api, frame, owner, gateway.pid,
            Path(admitted['gateway']['path']), admitted['mappings']['gateway_context'],
            admitted['file_catalog'], admitted['elf_catalog'])
    else:
        guest_lines.expect('LEGACY_DENIED 2')
    owner.write(peer, b'M')
    peer_lines.expect('LEGACY_MATCHED %d %d 0' % (positive, 2 if positive else 1))
    observations, retries = None, 0
    if positive:
        owner.write(peer, b'P')
        state = payload(peer_lines.read(), 'LEGACY_STATE', 1069, 0x13572468)
        until = time.monotonic() + 10
        for retries in range(4096):
            require(time.monotonic() < until, 'actual legacy Guest polling deadline exhausted')
            owner.write(worker, b'R')
            line = guest_lines.read()
            if line != 'LEGACY_AGAIN':
                parts = line.split(' ')
                require(parts == ['LEGACY_GUEST_STATE', '1069', state['canonical_hex']],
                        'all1069 actual native/Guest state bytes differ')
                break
            # This is an owned parent scheduling delay, not a Guest clock right.
            # Stay below the explicit100Hz alias budget even during real EAGAIN.
            time.sleep(.011)
        else:
            raise ValueError('actual legacy Guest poll count exhausted')
        owner.write(peer, b'A')
        peer_lines.expect('LEGACY_ARMED_COMMAND')
        owner.write(worker, b'S')
        guest_lines.expect('LEGACY_SUBMITTED')
        command = payload(peer_lines.read(), 'LEGACY_COMMAND', 734, 0x89abcdef)
        observations = {'state': state, 'command': command,
            'native_full_generated_message_and_serialization_equality': True,
            'real_TCPROS_command_callerid': '/uwvm_unitree_legacy/legacy_gateway',
            'firmware_CRC_authentication': False}
        owner.write(worker, b'C')
        guest_lines.expect('LEGACY_CLOSED')
    owner.write(worker, b'F')
    guest_lines.expect('PASS_LEGACY_CASE')
    worker_row = api.finish_actor(frame, owner, worker, serial=False)
    guest_log = guest_lines.finish(api, binding['gate_stdout'].encode('ascii'))
    audit = denial_audit(api, frame, worker_row) if not positive else None
    actual_positive_audit = positive_audit(api, frame, worker_row) if positive else None
    gateway_audit = None
    if gateway is not None:
        gateway_row = api.finish_actor(frame, owner, gateway, serial=False)
        require(not api.bounded(Path(gateway_row['stdout']), frame.limits['log_bytes']), 'unexpected legacy Gateway stdout')
        gateway_audit = native.gateway_audit(api, frame, gateway_row, 1, worker.pid)
    # Endpoint quiet is observed only AFTER the actual Guest and Gateway exit.
    owner.write(peer, b'Z' if positive else b'Q')
    quiet_proof = quiet(peer_lines.read(), positive)
    owner.write(peer, b'F')
    peer_lines.expect('PASS_NATIVE_LEGACY_WITNESS %d' % positive)
    peer_row = api.finish_actor(frame, owner, peer, serial=False)
    peer_log = peer_lines.finish(api, empty_stderr=True)
    master_row = stop_master(api, frame, owner, master)
    api.terminal(frame, owner)
    cleanup = owner.close()
    require(cleanup['status'] == 'PASS', 'legacy case owned cleanup incomplete')
    frame.actors.remove(owner)
    result = {'case': label, 'status': 'PASS_ACTUAL_LEGACY_TCPROS_CASE', 'positive': positive,
        'product': product['recipe']['id'], 'language': product['recipe']['language'], 'backend': 'ros1',
        'same_main': True, 'worker_profile': profile, 'artifact': product['artifact'], 'runtime_argv': argv,
        'worker': binding, 'guest_log': guest_log, 'native_log': peer_log, 'native_mapping': peer_proof,
        'master': master_proof, 'master_port': port, 'gateway_mapping': gateway_proof, 'gateway_config': config_pin,
        'gateway_audit': gateway_audit, 'default_denial_audit': audit, 'positive_Worker_audit': actual_positive_audit,
        'native_effects': observations, 'native_quiet': quiet_proof, 'poll_retries': retries,
        'worker_exit': worker_row['returncode'], 'native_exit': peer_row['returncode'],
        'master_exit': master_row['returncode'], 'cleanup': cleanup,
        'default_authority': [], 'positive_aliases': grants, 'optional_WASI': [],
        'physical_robot_qualification': False}
    frame.report['cases'].append(result)
    frame.save()
    return result
