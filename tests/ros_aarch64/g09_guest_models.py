#!/usr/bin/env python3
"""Independent synthetic evidence models for the focused G09 signed exchange.

Every process, mapping, signature, credential, endpoint and cgroup observation
below is synthetic. The expected facts are deliberately held outside the
candidate receipt: consistently replacing its self-reported facts must still
fail. These models do not perform cryptography, inspect the running machine,
import a launch controller, compile/validate Wasm or execute ROS/QEMU/Guests.

The oracle describes evidence association, not a replacement runtime verifier.
An actual controller must collect and bind its own measurements and checked
signature commands. Passing these models establishes only their finite pure
metadata cases and the reviewed source contracts.
"""
import sys
sys.dont_write_bytecode = True
import argparse
import ast
import copy
import hashlib
import json
from pathlib import Path
import re
import xml.etree.ElementTree as ET

MEMORY_MAX = 68719476736
TYPE = 'std_msgs/msg/String'
IDENTITY = 'RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18'
NODE = '/uwvm_test/secure_witness'
GUEST_MARKERS = ('SECURE_ENDPOINTS_CREATED', 'PASS_SECURE_GUEST')
WITNESS_MARKERS = ('READY_SECURE_WITNESS', 'READY_SECURE_MATCHED', 'PASS_SECURE_WITNESS')
ROLES = ('worker', 'gateway', 'witness')
GID_FIELDS = ('witness_sub_gid', 'witness_pub_gid', 'gateway_pub_gid', 'gateway_sub_gid')
STAGES = ('gateway_map', 'witness_map', 'I', 'READY_SECURE_WITNESS',
          'worker_map', 'G', 'SECURE_ENDPOINTS_CREATED', 'READY_SECURE_MATCHED',
          'M', 'PASS_SECURE_GUEST', 'guest_exit_zero', 'F',
          'PASS_SECURE_WITNESS', 'witness_exit_zero', 'gateway_shutdown')
SOURCES = ('tests/ros_aarch64/run_g09_guest_enforce.py',
           'tests/ros_aarch64/g09_guest_models.py',
           'tests/ros_aarch64/README.g09-guest-enforce.md',
           'tests/ros_security/guest_enforce_ros2.wat',
           'tests/ros_security/guest_enforce_witness_ros2.cc',
           'tests/ros_security/sros2-policy.xml',
           'sdk/abi/abi-v1.json',
           'tests/ros_aarch64/runtime.py',
           'tests/ros_aarch64/gateway_qualification.py')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def integer(value, minimum, maximum):
    return type(value) is int and minimum <= value <= maximum


def sha(value):
    return isinstance(value, str) and re.fullmatch(r'[0-9a-f]{64}', value) is not None


def gid(value):
    """Require the pinned Jazzy 16-byte ABI, with every byte represented once.

    The native witness asserts RMW_GID_STORAGE_SIZE==16. This independent
    oracle deliberately refuses old 24-byte values, padding, truncation and
    variable-width encodings rather than negotiating a weaker marker format.
    """
    return isinstance(value, str) and re.fullmatch(r'[0-9a-f]{32}', value) is not None and value != '0' * 32


def decimal(value, minimum, maximum):
    require(isinstance(value, str) and re.fullmatch(r'0|[1-9][0-9]*', value) is not None,
            'marker count must be canonical unsigned decimal')
    number = int(value)
    require(minimum <= number <= maximum, 'marker count out of bounds')
    return number


def marker(line, name, fields):
    """Decode a closed public marker schema, refusing duplicates and extensions."""
    require(isinstance(line, str) and line.isascii() and '\r' not in line and '\n' not in line,
            'marker must be one ASCII line')
    tokens = line.split(' ')
    require(tokens[0] == name and all(tokens), 'marker name/spacing differs')
    result = {}
    for token in tokens[1:]:
        key, separator, value = token.partition('=')
        require(separator == '=' and key in fields and key not in result and value != '',
                'missing, duplicate or unknown marker field')
        result[key] = value
    require(set(result) == set(fields), 'incomplete marker metadata')
    return result


def witness_contract(text, expected_pid):
    """Independent exact three-line protocol oracle derived from public output."""
    require(isinstance(text, str) and text.endswith('\n') and '\r' not in text,
            'complete LF-terminated witness log required')
    lines = text.splitlines()
    require(len(lines) == 3, 'exactly three witness marker lines required')
    common = ('pid', 'node', 'type', 'sub_topic', 'pub_topic')
    ready = marker(lines[0], WITNESS_MARKERS[0], (*common, 'enclave', 'domain', 'rmw', 'pub_gid', 'out_bytes', 'in_bytes'))
    matched = marker(lines[1], WITNESS_MARKERS[1], (*common, *GID_FIELDS, 'matched_publishers', 'matched_subscribers', 'received', 'sent'))
    passed = marker(lines[2], WITNESS_MARKERS[2], (*common, *GID_FIELDS, 'matched_publishers', 'matched_subscribers',
                    'received', 'sent', 'mismatched', 'denied_native', 'permission_events', 'out_bytes', 'in_bytes'))
    for values in (ready, matched, passed):
        require(decimal(values['pid'], 1, 2**31 - 1) == expected_pid, 'witness marker belongs to another PID')
        require(values['node'] == NODE and values['type'] == TYPE and
                values['sub_topic'] == '/uwvm_test/pubString' and values['pub_topic'] == '/uwvm_test/subString',
                'witness direction/node/type differs')
    require(ready['enclave'] == '/witness' and ready['domain'] == '74' and ready['rmw'] == 'rmw_fastrtps_cpp',
            'witness context marker differs')
    require(ready['out_bytes'] == passed['out_bytes'] == '31' and ready['in_bytes'] == passed['in_bytes'] == '23',
            'canonical direction sizes differ')
    require(gid(ready['pub_gid']), 'nonzero 16-byte publisher GID required')
    for values in (matched, passed):
        require(all(gid(values[key]) for key in GID_FIELDS) and len({values[key] for key in GID_FIELDS}) == 4,
                'four distinct nonzero 16-byte GIDs required')
        require(values['matched_publishers'] == values['matched_subscribers'] == '1', 'exact native matches required')
    require(all(matched[key] == passed[key] for key in GID_FIELDS) and ready['pub_gid'] == matched['witness_pub_gid'],
            'endpoint GIDs changed across witness phases')
    before = decimal(matched['received'], 0, 2)
    received = decimal(passed['received'], 1, 2)
    sent = decimal(passed['sent'], 1, 100)
    require(matched['sent'] == '0' and before <= received, 'witness counters reversed or premature reply')
    require(passed['mismatched'] == '0' and passed['denied_native'] == passed['permission_events'] == '1',
            'positive exchange and exact native permission refusal required')
    return {'gids': {key: passed[key] for key in GID_FIELDS}, 'received': received, 'sent': sent,
            'pid': expected_pid, 'markers': [ready, matched, passed]}


def guest_contract(text):
    """Full source-bound completion log, including both exact newline markers."""
    require(text == 'SECURE_ENDPOINTS_CREATED\nPASS_SECURE_GUEST\n', 'exact Guest gate/completion markers required')


def completion_oracle(candidate, expected):
    """Associate separate trusted synthetic facts before admitting a PASS.

    This intentionally has no path-opening or crypto primitive. ``expected``
    represents independently measured inputs/processes and verified signed
    plaintext. A candidate's nested status flags cannot replace those facts.
    """
    require(candidate.get('kind') == 'g09-guest-enforce-arm' and candidate.get('status') == 'PASS' and
            candidate.get('actual_guest_behavior') == 'PASS' and candidate.get('scope') == 'COMBINED_GUEST_NATIVE',
            'focused combined Guest/native behavior required')
    require(candidate.get('actual_guest_cases') == 1 and type(candidate['actual_guest_cases']) is int,
            'one real Guest case required')
    require(candidate.get('input_pins') == expected['input_pins'] and candidate.get('inputs_preserved') is True,
            'source/product/policy inputs substituted')
    group = candidate['architecture']
    require(group.get('status') == 'PASS' and group.get('fatal_errors') == [] and
            group.get('remaining_owned_processes') == [], 'architecture/cleanup receipt incomplete')
    require(group.get('incarnation_before') == expected['incarnation'] == group.get('incarnation_after_before_probe') == group.get('incarnation_after'),
            'boot/leaf identity changed or belongs to another run')
    require(group.get('container_started_at') == expected['container_started_at'], 'container start identity differs')
    for phase in ('before', 'after'):
        fact = group[phase]
        require(type(fact.get('memory_max')) is int and fact['memory_max'] == MEMORY_MAX and fact.get('swap_max') == '0',
                'current64GiB zero-swap group required')
        require(fact.get('membership') == expected['membership'], 'cgroup membership differs')
        require(fact.get('ancestor_events') == expected['ancestor_events'], 'missing/changed/nonzero OOM counters')
        require(all(set(event) == {'path', 'oom', 'oom_kill', 'oom_group_kill'} and
                    all(integer(event[key], 0, 2**64 - 1) for key in ('oom', 'oom_kill', 'oom_group_kill'))
                    for event in fact['ancestor_events']), 'raw OOM counters must be explicit integers')
    require(candidate.get('remaining_owned_mounts') == [] and candidate.get('remaining_owned_processes') == [] and
            candidate.get('private_credentials_removed') is True and candidate.get('domain_lease_released') is True,
            'owned process/mount/credential/domain cleanup incomplete')
    processes = candidate['processes']
    require(set(processes) == set(ROLES), 'exact independently owned roles required')
    require(len({processes[role]['pid'] for role in ROLES}) == 3, 'Witness and Gateway must be independent processes')
    for role in ROLES:
        fact, trusted = processes[role], expected['processes'][role]
        require(fact == trusted, 'role PID/birth/map/snapshot/emulator observation differs: ' + role)
        require(integer(fact['pid'], 1, 2**31 - 1) and integer(fact['birth'], 1, 2**64 - 1) and
                fact['live_membership'] is True and fact['owned_pidfd'] is True and
                fact['incarnation'] == expected['incarnation'] and fact['membership'] == expected['membership'],
                'role ownership/current cgroup proof incomplete: ' + role)
        require(fact['mapped_arm']['e_machine'] == 183 and fact['host_emulator']['e_machine'] == 62 and
                fact['mapped_arm'] == expected['input_pins'][role] and fact['host_emulator'] == expected['input_pins']['qemu'],
                'actual role map and QEMU must match pinned products: ' + role)
        require(fact['launcher_birth'] <= fact['birth'] and fact['ancestry'][0] == fact['launcher_pid'] and
                fact['ancestry'][-1] == fact['pid'] and fact['current_map_before_gate'] is True,
                'role map/ancestry does not precede its gate: ' + role)
    require(candidate['contexts'] == expected['contexts'], 'native context belongs to another role/settings/source')
    for role in ('gateway', 'witness'):
        context = candidate['contexts'][role]
        require(context['pid'] == processes[role]['pid'] and context['birth'] == processes[role]['birth'] and
                context['domain'] == 74 and context['rmw'] == 'rmw_fastrtps_cpp' and context['strategy'] == 'Enforce' and
                context['enclave'] == '/' + role and context['identity'] == IDENTITY,
                'source-bound public native context/type attestation differs')
        if role == 'witness':
            require(context['attestation_kind'] == 'public-rmw-context' and context['attestation_pass'] is True,
                    'Witness public native context checks required')
        else:
            require(context['attestation_kind'] == 'checked-source-selection' and context['source_selected'] is True and
                    context['inherited_override_cleared'] is True,
                    'Gateway checked source selection must not imply public-context attestation')
    require(candidate['signatures'] == expected['signatures'], 'verified signed credentials or policy association substituted')
    for role in ('gateway', 'witness'):
        proof = candidate['signatures'][role]
        require(proof['enclave'] == '/' + role and proof['policy_sha256'] == expected['input_pins']['policy']['sha256'] and
                proof['source_manifest_sha256'] == expected['input_pins']['source_manifest']['sha256'] and
                proof['certificate_key_match'] is True and proof['permissions_subject_matches_certificate'] is True and
                proof['signed_governance_verified'] is True and proof['signed_permissions_verified'] is True and
                proof['signed_input_sha256'] == expected['signatures'][role]['signed_input_sha256'] and
                sha(proof['signed_input_sha256']) and sha(proof['verified_plaintext_sha256']),
                'signature status must bind exact role, signed input and verified plaintext')
    require(candidate['policy'] == expected['policy'], 'explicit mirrored capability/WASI policy changed')
    require(candidate['guest_source_assertions'] == {'ungranted_errno': 13, 'stale_errno': 9,
            'protected_outputs_unchanged': True, 'outbound_bytes': 31, 'inbound_bytes': 23},
            'source-bound denied13/stale9/byte/sentinel contract differs')
    require(candidate['guest_source_assertions']['protected_outputs_unchanged'] is True,
            'Guest protected-output fact must be boolean true')
    require(candidate['guest_source_sha256'] == expected['input_pins']['wat']['sha256'], 'Guest assertions belong to another WAT')
    require(candidate['events'] == list(STAGES), 'map/initialization/Guest/match/completion gates reordered or missing')
    guest_contract(candidate['guest_log'])
    result = witness_contract(candidate['witness_log'], processes['witness']['pid'])
    require(result['gids'] == expected['endpoint_gids'], 'marker GIDs not associated with independent current endpoint receipt')
    require(candidate['exit_codes'] == {'worker': 0, 'witness': 0, 'gateway': 0} and
            all(type(value) is int for value in candidate['exit_codes'].values()), 'successful owned exits required')
    return result


def fixture():
    """Construct independent fake facts and marker text; never execute images."""
    identity = {'path': '/synthetic/current-leaf', 'device': 29, 'inode': 9001, 'boot_id': 'synthetic-current-boot'}
    membership = ['0::/synthetic/current-leaf']
    pins = {}
    for index, name in enumerate((*ROLES, 'qemu', 'policy', 'wat', 'wasm', 'source_manifest')):
        pins[name] = {'path': '/synthetic/' + name, 'device': 29, 'inode': 100 + index,
                      'bytes': 100 + index, 'sha256': hashlib.sha256(('synthetic-' + name).encode()).hexdigest()}
        if name in ROLES or name == 'qemu':
            pins[name].update(e_machine=62 if name == 'qemu' else 183, elf_class=2, elf_data=1)
    processes = {}
    contexts = {}
    signatures = {}
    for index, role in enumerate(ROLES):
        pid, birth = 1000 + index * 10, 2000 + index * 10
        processes[role] = {'pid': pid, 'birth': birth, 'launcher_pid': pid - 1, 'launcher_birth': birth - 1,
            'ancestry': [pid - 1, pid], 'membership': membership, 'incarnation': identity,
            'owned_pidfd': True, 'live_membership': True, 'current_map_before_gate': True,
            'mapped_arm': copy.deepcopy(pins[role]), 'host_emulator': copy.deepcopy(pins['qemu'])}
        if role != 'worker':
            contexts[role] = {'pid': pid, 'birth': birth, 'domain': 74, 'rmw': 'rmw_fastrtps_cpp',
                'enclave': '/' + role, 'strategy': 'Enforce', 'identity': IDENTITY}
            if role == 'witness':
                contexts[role].update(attestation_kind='public-rmw-context', attestation_pass=True)
            else:
                contexts[role].update(attestation_kind='checked-source-selection', source_selected=True,
                                     inherited_override_cleared=True)
            signatures[role] = {'enclave': '/' + role, 'policy_sha256': pins['policy']['sha256'],
                'source_manifest_sha256': pins['source_manifest']['sha256'],
                'signed_input_sha256': hashlib.sha256(('signed-' + role).encode()).hexdigest(),
                'verified_plaintext_sha256': hashlib.sha256(('plaintext-' + role).encode()).hexdigest(),
                'certificate_key_match': True, 'permissions_subject_matches_certificate': True,
                'signed_governance_verified': True, 'signed_permissions_verified': True}
    gids = {name: bytes([0xa1 + index] * 16).hex() for index, name in enumerate(GID_FIELDS)}
    policy = {'connection': {'domain-id': '74', 'security-mode': 'enforce', 'rmw-implementation': 'rmw_fastrtps_cpp',
                            'enclave': '/gateway', 'keystore': '/tmp/g09-keys'},
        'grants': [('pubString', 'topic.publish', '/uwvm_test/pubString', TYPE, IDENTITY),
                   ('subString', 'topic.subscribe', '/uwvm_test/subString', TYPE, IDENTITY)],
        'wasi': {'target': 'main', 'enabled': True, 'inherit_system_environment': False, 'grants': ['monotonic-clock']}}
    events = [{'path': '/synthetic/current-leaf', 'oom': 0, 'oom_kill': 0, 'oom_group_kill': 0},
              {'path': '/synthetic/ancestor', 'oom': 0, 'oom_kill': 0, 'oom_group_kill': 0}]
    expected = {'incarnation': identity, 'membership': membership, 'container_started_at': 'synthetic-current-start',
                'ancestor_events': events, 'input_pins': pins, 'processes': processes,
                'contexts': contexts, 'signatures': signatures, 'endpoint_gids': gids, 'policy': policy}
    pid = processes['witness']['pid']
    common = f'pid={pid} node={NODE} type={TYPE} sub_topic=/uwvm_test/pubString pub_topic=/uwvm_test/subString'
    gid_text = ' '.join(key + '=' + gids[key] for key in GID_FIELDS)
    witness_log = (f'READY_SECURE_WITNESS {common} enclave=/witness domain=74 rmw=rmw_fastrtps_cpp pub_gid={gids["witness_pub_gid"]} out_bytes=31 in_bytes=23\n'
        f'READY_SECURE_MATCHED {common} {gid_text} matched_publishers=1 matched_subscribers=1 received=0 sent=0\n'
        f'PASS_SECURE_WITNESS {common} {gid_text} matched_publishers=1 matched_subscribers=1 received=1 sent=1 mismatched=0 denied_native=1 permission_events=1 out_bytes=31 in_bytes=23\n')
    group = {'status': 'PASS', 'fatal_errors': [], 'remaining_owned_processes': [],
        'incarnation_before': copy.deepcopy(identity), 'incarnation_after_before_probe': copy.deepcopy(identity),
        'incarnation_after': copy.deepcopy(identity), 'container_started_at': expected['container_started_at'],
        'before': {'memory_max': MEMORY_MAX, 'swap_max': '0', 'membership': membership, 'ancestor_events': copy.deepcopy(events)},
        'after': {'memory_max': MEMORY_MAX, 'swap_max': '0', 'membership': membership, 'ancestor_events': copy.deepcopy(events)}}
    candidate = {'kind': 'g09-guest-enforce-arm', 'status': 'PASS', 'actual_guest_behavior': 'PASS',
        'scope': 'COMBINED_GUEST_NATIVE', 'actual_guest_cases': 1, 'input_pins': copy.deepcopy(pins),
        'inputs_preserved': True, 'architecture': group, 'remaining_owned_mounts': [], 'remaining_owned_processes': [],
        'private_credentials_removed': True, 'domain_lease_released': True,
        'processes': copy.deepcopy(processes), 'contexts': copy.deepcopy(contexts), 'signatures': copy.deepcopy(signatures),
        'policy': copy.deepcopy(policy), 'guest_source_assertions': {'ungranted_errno': 13, 'stale_errno': 9,
            'protected_outputs_unchanged': True, 'outbound_bytes': 31, 'inbound_bytes': 23},
        'guest_source_sha256': pins['wat']['sha256'], 'events': list(STAGES),
        'guest_log': 'SECURE_ENDPOINTS_CREATED\nPASS_SECURE_GUEST\n', 'witness_log': witness_log,
        'exit_codes': {'worker': 0, 'witness': 0, 'gateway': 0}}
    return candidate, expected


def run_models():
    """Enumerate meaningful corruption/substitution cases with no live inputs."""
    passed = []
    refused = []

    def allow(name, action):
        action()
        passed.append(name)

    def deny(name, action):
        try:
            action()
        except (ValueError, KeyError, TypeError):
            refused.append(name)
        else:
            raise AssertionError('corrupted evidence admitted: ' + name)

    original, expected = fixture()
    allow('complete independently associated synthetic combined receipt', lambda: completion_oracle(original, expected))
    for received in (1, 2):
        sample = copy.deepcopy(original)
        sample['witness_log'] = sample['witness_log'].replace('received=1 sent=1', f'received={received} sent=100')
        allow('one or two native receives; bounded replies100: ' + str(received), lambda sample=sample: completion_oracle(sample, expected))
    prior, prior_expected = fixture()
    prior_expected['ancestor_events'][0]['oom'] = 7
    for phase in ('before', 'after'):
        prior['architecture'][phase]['ancestor_events'][0]['oom'] = 7
    allow('nonzero historical baseline without new same-incarnation OOM delta', lambda: completion_oracle(prior, prior_expected))
    direct = [('status', 'FAIL'), ('kind', 'native-secure-arm'), ('scope', 'NATIVE_ONLY'),
        ('actual_guest_behavior', 'NOT_PERFORMED'), ('actual_guest_cases', 0), ('actual_guest_cases', True),
        ('inputs_preserved', False), ('remaining_owned_mounts', ['/synthetic/mount']),
        ('remaining_owned_processes', [1000]), ('private_credentials_removed', False), ('domain_lease_released', False),
        ('guest_source_sha256', '0' * 64), ('guest_log', 'PASS_SECURE_GUEST\n'),
        ('guest_log', 'SECURE_ENDPOINTS_CREATED\nPASS_SECURE_GUEST\r\n'),
        ('guest_log', original['guest_log'] * 2)]
    for name, value in direct:
        sample = copy.deepcopy(original)
        sample[name] = value
        deny('receipt field corruption: ' + name + '/' + str(value), lambda sample=sample: completion_oracle(sample, expected))
    for key in original:
        sample = copy.deepcopy(original)
        sample.pop(key)
        deny('required receipt field missing: ' + key, lambda sample=sample: completion_oracle(sample, expected))
    for phase in ('before', 'after'):
        for key, value in [('memory_max', 51539607552), ('memory_max', True), ('swap_max', 'max'),
                           ('swap_max', '1'), ('membership', ['0::/other']), ('ancestor_events', [])]:
            sample = copy.deepcopy(original)
            sample['architecture'][phase][key] = value
            deny('architecture field corruption: ' + phase + '/' + key, lambda sample=sample: completion_oracle(sample, expected))
        for event in ('oom', 'oom_kill', 'oom_group_kill'):
            sample = copy.deepcopy(original)
            sample['architecture'][phase]['ancestor_events'][0][event] = 1
            deny('raw ancestor OOM counter differs: ' + phase + '/' + event, lambda sample=sample: completion_oracle(sample, expected))
            sample = copy.deepcopy(original)
            sample['architecture'][phase]['ancestor_events'][0][event] = False
            deny('boolean masquerading as zero OOM counter: ' + phase + '/' + event, lambda sample=sample: completion_oracle(sample, expected))
    for phase in ('incarnation_before', 'incarnation_after_before_probe', 'incarnation_after'):
        for key, value in [('path', '/other/leaf'), ('device', 30), ('inode', 9002), ('boot_id', 'other-boot')]:
            sample = copy.deepcopy(original)
            sample['architecture'][phase][key] = value
            deny('cgroup incarnation substitution: ' + phase + '/' + key, lambda sample=sample: completion_oracle(sample, expected))
    for key, value in [('container_started_at', 'another-start'), ('fatal_errors', ['sampler failure']),
                        ('status', 'FAIL'), ('remaining_owned_processes', [999])]:
        sample = copy.deepcopy(original)
        sample['architecture'][key] = value
        deny('architecture top-level corruption: ' + key, lambda sample=sample: completion_oracle(sample, expected))
    for role in ROLES:
        for key, value in [('pid', 999), ('birth', 1), ('launcher_birth', 99999), ('launcher_pid', 9999),
            ('ancestry', [999]), ('live_membership', False), ('owned_pidfd', False), ('current_map_before_gate', False),
            ('membership', ['0::/elsewhere']), ('incarnation', {'boot_id': 'old'}),
            ('mapped_arm', copy.deepcopy(expected['input_pins']['qemu'])),
            ('host_emulator', copy.deepcopy(expected['input_pins'][role]))]:
            sample = copy.deepcopy(original)
            sample['processes'][role][key] = value
            deny('role identity/origin corruption: ' + role + '/' + key, lambda sample=sample: completion_oracle(sample, expected))
        for key in ('device', 'inode', 'sha256'):
            sample = copy.deepcopy(original)
            sample['processes'][role]['mapped_arm'][key] = 888 if key != 'sha256' else '0' * 64
            deny('role mapped product substitution: ' + role + '/' + key, lambda sample=sample: completion_oracle(sample, expected))
    for role in ('gateway', 'witness'):
        for key, value in [('pid', 1000), ('birth', 1), ('domain', 75), ('rmw', 'rmw_cyclonedds_cpp'),
                           ('enclave', '/other'), ('strategy', 'Permissive'), ('identity', '0' * 64),
                           ('attestation_kind', 'self-declared')]:
            sample = copy.deepcopy(original)
            sample['contexts'][role][key] = value
            deny('native context association corruption: ' + role + '/' + key, lambda sample=sample: completion_oracle(sample, expected))
        for key in (('attestation_pass',) if role == 'witness' else ('source_selected', 'inherited_override_cleared')):
            sample = copy.deepcopy(original)
            sample['contexts'][role][key] = False
            deny('native context/source check absent: ' + role + '/' + key, lambda sample=sample: completion_oracle(sample, expected))
        for key in expected['signatures'][role]:
            sample = copy.deepcopy(original)
            sample['signatures'][role][key] = False if isinstance(expected['signatures'][role][key], bool) else 'substituted'
            deny('signature association corruption: ' + role + '/' + key, lambda sample=sample: completion_oracle(sample, expected))
    for name in expected['input_pins']:
        for key in ('path', 'inode', 'sha256'):
            sample = copy.deepcopy(original)
            sample['input_pins'][name][key] = 999 if key == 'inode' else 'substituted'
            deny('pinned input substitution: ' + name + '/' + key, lambda sample=sample: completion_oracle(sample, expected))
    for key, value in [('ungranted_errno', 0), ('stale_errno', 0), ('protected_outputs_unchanged', False), ('protected_outputs_unchanged', 1),
                       ('outbound_bytes', 23), ('inbound_bytes', 31)]:
        sample = copy.deepcopy(original)
        sample['guest_source_assertions'][key] = value
        deny('source-bound Guest contract weakened: ' + key, lambda sample=sample: completion_oracle(sample, expected))
    for stage in STAGES:
        sample = copy.deepcopy(original)
        sample['events'].remove(stage)
        deny('required gate/stage missing: ' + stage, lambda sample=sample: completion_oracle(sample, expected))
    for i in range(len(STAGES) - 1):
        sample = copy.deepcopy(original)
        sample['events'][i], sample['events'][i + 1] = sample['events'][i + 1], sample['events'][i]
        deny('adjacent gate/stage inversion: ' + str(i), lambda sample=sample: completion_oracle(sample, expected))
    for role in ROLES:
        for value in (1, -9, True):
            sample = copy.deepcopy(original)
            sample['exit_codes'][role] = value
            deny('nonzero/non-integer owned exit: ' + role + '/' + str(value), lambda sample=sample: completion_oracle(sample, expected))
    for mutate in ('same-process', 'coherent-old-boot', 'coherent-old-PID', 'coherent-old-signed-policy', 'coherent-old-GIDs', 'coherent48GiB'):
        sample = copy.deepcopy(original)
        if mutate == 'same-process':
            sample['processes']['witness'] = copy.deepcopy(sample['processes']['gateway'])
        elif mutate == 'coherent-old-boot':
            for key in ('incarnation_before', 'incarnation_after_before_probe', 'incarnation_after'):
                sample['architecture'][key]['boot_id'] = 'historical-boot'
            for role in ROLES:
                sample['processes'][role]['incarnation']['boot_id'] = 'historical-boot'
        elif mutate == 'coherent-old-PID':
            old = sample['processes']['witness']['pid']
            sample['processes']['witness']['pid'] = 3333
            sample['processes']['witness']['ancestry'][-1] = 3333
            sample['contexts']['witness']['pid'] = 3333
            sample['witness_log'] = sample['witness_log'].replace(f'pid={old}', 'pid=3333')
        elif mutate == 'coherent-old-signed-policy':
            sample['input_pins']['policy']['sha256'] = '0' * 64
            for role in ('gateway', 'witness'):
                sample['signatures'][role]['policy_sha256'] = '0' * 64
        elif mutate == 'coherent-old-GIDs':
            for key in GID_FIELDS:
                sample['witness_log'] = sample['witness_log'].replace(expected['endpoint_gids'][key], bytes([25] * 16).hex())
        else:
            for phase in ('before', 'after'):
                sample['architecture'][phase]['memory_max'] = 51539607552
        deny('consistent candidate self-substitution: ' + mutate, lambda sample=sample: completion_oracle(sample, expected))
    sample = copy.deepcopy(original)
    sample.update(status='PASS', scope='NATIVE_ONLY', actual_guest_behavior='NOT_PERFORMED', actual_guest_cases=0)
    sample['historical_native_subrow'] = {'status': 'PASS', 'native_secure_ros2': True}
    deny('historical native-only subrow cannot promote outer combined PASS', lambda: completion_oracle(sample, expected))
    for mutate in ('missing-G', 'missing-M', 'implicit-clock', 'extra-ROS-grant', 'ambient-environment'):
        sample = copy.deepcopy(original)
        if mutate.startswith('missing-'):
            sample['events'].remove(mutate[-1])
        elif mutate == 'implicit-clock':
            sample['policy']['wasi']['grants'] = []
        elif mutate == 'extra-ROS-grant':
            sample['policy']['grants'].append(('notGranted', 'topic.publish', '/uwvm_test/notGranted', TYPE, IDENTITY))
        else:
            sample['policy']['wasi']['inherit_system_environment'] = True
        deny('explicit authority/gate requirement: ' + mutate, lambda sample=sample: completion_oracle(sample, expected))

    # Marker mutations operate on original independent text, not output from
    # the parser under test. Duplicate/unknown/missing fields never disappear
    # through dict construction or successful-prefix substring matching.
    lines = original['witness_log'].splitlines()
    for index, line in enumerate(lines):
        for bad_line in (line + ' pid=1020', line + ' private_key=secret', line.replace(' pid=1020', ''),
                         line.replace('pid=1020', 'pid=01020'), line.replace('pid=1020', 'pid=-1'),
                         line.replace(' type=' + TYPE, ' type=wrong/Type'),
                         line.replace(' pub_topic=/uwvm_test/subString', ' pub_topic=/uwvm_test/pubString'),
                         line.replace(' node=' + NODE, ' node=/uwvm_test/uwvm_ros2_gateway_probe'),
                         line.replace(' ', '  ', 1), line + '\r'):
            altered = lines[:]
            altered[index] = bad_line
            text = '\n'.join(altered) + '\n'
            deny('closed witness marker metadata mutation: ' + str(index) + '/' + str(len(refused)),
                 lambda text=text: witness_contract(text, expected['processes']['witness']['pid']))
    for key in GID_FIELDS:
        value = expected['endpoint_gids'][key]
        for replaced in ('0' * 32, value[:-2], 'GG' * 16, value.upper(), bytes([42] * 16).hex()):
            altered = lines[:]
            # Alter only the final row so inconsistent yet plausible GIDs fail.
            altered[2] = altered[2].replace(key + '=' + value, key + '=' + replaced)
            text = '\n'.join(altered) + '\n'
            if replaced == value:
                continue
            deny('GID width/nonzero/origin mutation: ' + key + '/' + replaced,
                 lambda text=text: witness_contract(text, expected['processes']['witness']['pid']))
    for byte_count in range(33):
        if byte_count == 16:
            continue
        text = original['witness_log']
        for index, key in enumerate(GID_FIELDS):
            text = text.replace(expected['endpoint_gids'][key], bytes([0xa1 + index] * byte_count).hex())
        # Replace every phase coherently: endpoint continuity alone cannot
        # reject an obsolete 24-byte or another variable-width representation.
        deny('exact Jazzy GID width oracle refusal bytes=' + str(byte_count),
             lambda text=text: witness_contract(text, expected['processes']['witness']['pid']))
    for fragment, substitute in [('domain=74', 'domain=75'), ('enclave=/witness', 'enclave=/gateway'),
        ('rmw=rmw_fastrtps_cpp', 'rmw=rmw_cyclonedds_cpp'), ('out_bytes=31', 'out_bytes=23'),
        ('in_bytes=23', 'in_bytes=31'), ('received=1 sent=1', 'received=0 sent=1'),
        ('received=1 sent=1', 'received=3 sent=1'), ('received=1 sent=1', 'received=1 sent=0'),
        ('received=1 sent=1', 'received=1 sent=101'), ('permission_events=1', 'permission_events=0'),
        ('permission_events=1', 'permission_events=2'), ('denied_native=1', 'denied_native=0'),
        ('mismatched=0', 'mismatched=1'), ('matched_publishers=1', 'matched_publishers=0'),
        ('matched_subscribers=1', 'matched_subscribers=2')]:
        text = original['witness_log'].replace(fragment, substitute)
        deny('witness context/counter/refusal metadata: ' + substitute,
             lambda text=text: witness_contract(text, expected['processes']['witness']['pid']))
    for text in ('\n'.join(lines[:2]) + '\n', original['witness_log'] + lines[2] + '\n',
                 original['witness_log'][:-1], original['witness_log'] + 'native secret\n',
                 '\n'.join(reversed(lines)) + '\n'):
        deny('missing/repeated/reordered/extraneous witness log line: ' + str(len(refused)),
             lambda text=text: witness_contract(text, expected['processes']['witness']['pid']))
    return {'positive_models': passed, 'refusal_models': refused}


def source_pin(path):
    data = path.read_bytes()
    return {'path': str(path), 'bytes': len(data), 'sha256': hashlib.sha256(data).hexdigest()}


def pure_controller(source):
    """Load only five reviewed deterministic functions and literal constants.

    The controller module is never imported: its launch helpers, imports and
    main function cannot run. Compilation here is Python AST evaluation for
    pure metadata models, not a native or WebAssembly compiler invocation.
    """
    tree = ast.parse(source.read_text(), filename=str(source))
    names = {'protocol_require', 'parse_witness_markers', 'parse_guest_markers',
             'valid_identity', 'validate_combined_completion'}
    scope = {'re': re}

    class BoundLiterals(ast.NodeTransformer):
        def visit_Name(self, node):
            value = scope.get(node.id)
            if node.id in scope and (value is None or type(value) in (str, int, bool)):
                return ast.copy_location(ast.Constant(value=value), node)
            return node

    for node in tree.body:
        if isinstance(node, ast.Assign) and len(node.targets) == 1 and isinstance(node.targets[0], ast.Name):
            try:
                scope[node.targets[0].id] = ast.literal_eval(BoundLiterals().visit(copy.deepcopy(node.value)))
            except (ValueError, TypeError):
                pass
    definitions = [node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name in names]
    require({node.name for node in definitions} == names, 'five deterministic controller functions required')
    allowed_calls = names | {'isinstance', 'type', 'len', 'int', 'set', 'tuple', 'list', 'bool', 'all', 'any', 'RuntimeError'}
    for definition in definitions:
        require(not definition.decorator_list, 'pure controller function must have no decorator side effect')
        for node in ast.walk(definition):
            require(not isinstance(node, (ast.Import, ast.ImportFrom, ast.Global, ast.Nonlocal, ast.With, ast.AsyncWith)),
                    'pure function cannot import, mutate module state or open a context')
            if isinstance(node, ast.Call) and isinstance(node.func, ast.Name):
                require(node.func.id in allowed_calls, 'unsupported pure controller call: ' + node.func.id)
            if isinstance(node, ast.Call) and isinstance(node.func, ast.Attribute):
                require(node.func.attr in {'get', 'startswith', 'endswith', 'splitlines', 'split', 'items',
                    'isascii', 'count', 'fullmatch', 'add', 'append'}, 'unsupported pure attribute call: ' + node.func.attr)
    selected = ast.Module(body=definitions, type_ignores=[])
    exec(compile(ast.fix_missing_locations(selected), str(source) + ':pure_metadata', 'exec'), scope)
    return scope


def controller_fixture(scope):
    """Shape an independent synthetic row for the real completion predicate."""
    candidate, expected = fixture()
    roles, products = {}, {}
    membership = expected['membership'][:]
    for role in ROLES:
        origin = copy.deepcopy(expected['input_pins'][role])
        snapshot = copy.deepcopy(origin)
        snapshot.update(path='/synthetic/snapshot/' + role, inode=origin['inode'] + 1000)
        products[role] = {'qualified': True, 'origin': origin, 'snapshot': snapshot}
        process = expected['processes'][role]
        actual = {'pid': process['pid'], 'starttime_ticks': process['birth'], 'state': 'T'}
        sample = {'pid': process['pid'], 'starttime_ticks': process['birth'], 'membership': membership,
            'live_cgroup_procs': True, 'mapped_arm_targets': [scope['ROLE_LABELS'][role]],
            'kernel_exe_elf': {'e_machine': 62, 'sha256': expected['input_pins']['qemu']['sha256']},
            'ancestry': process['ancestry'][:],
            'target_mappings': ['1000-2000 r-xp 00000000 00:1d ' + str(snapshot['inode']) + ' ' + snapshot['path']]}
        roles[role] = {'pid': process['pid'], 'starttime_ticks': process['birth'], 'membership': membership,
            'live_before_G': True, 'pre_init_mapping': role == 'witness', 'mapped_product': copy.deepcopy(snapshot),
            'proof': {'actual_stopped_ARM': actual, 'target_proof': sample,
                      'current_mapping_check': True, 'ancestry': process['ancestry'][:]}}
    identity = {'path': '/synthetic/current-leaf', 'device': scope['EXPECTED_LEAF_DEVICE'],
                'inode': scope['EXPECTED_LEAF_INODE'], 'boot_id': scope['EXPECTED_BOOT']}
    gates = []
    for role, byte, prior in [('witness', 'I', 'PRE_INIT_MAP'), ('worker', 'G', 'READY_SECURE_WITNESS'),
        ('worker', 'M', 'READY_SECURE_MATCHED+SECURE_ENDPOINTS_CREATED'), ('witness', 'F', 'PASS_SECURE_GUEST')]:
        gate = {'role': role, 'byte': byte, 'pid': roles[role]['pid'], 'starttime_ticks': roles[role]['starttime_ticks'],
                'current_birth_mapping_verified': True, 'sent': True, 'prior_marker': prior}
        if byte == 'F':
            gate['guest_exit'] = 0
        gates.append(gate)
    return {'kind': scope['KIND'], 'format_version': 1, 'mode': 'execute',
        'source_manifest_sha256': expected['input_pins']['source_manifest']['sha256'],
        **{name: True for name in ('source_preserved', 'sdk_preserved', 'driver_inputs_preserved',
            'immutable_inputs_preserved', 'qualified_inputs_preserved', 'products_preserved')},
        'cgroup': {'before': copy.deepcopy(identity), 'after': copy.deepcopy(identity), 'same_incarnation': True,
            'memory_max': MEMORY_MAX, 'swap_max': '0', 'readable_ancestors': True, 'zero_oom': True, 'membership': membership},
        'signed': {'status': 'PASS', 'keystore': '/tmp/g09-keys',
            'subjects': {'gateway': 'CN=/gateway', 'witness': 'CN=/witness'},
            'policy_sha256': expected['input_pins']['policy']['sha256'], 'closure_sha256': 'a' * 64,
            **{name: True for name in ('certificate_CA_verified', 'permissions_signatures_verified', 'governance_signatures_verified',
                'key_cert_public_match', 'default_deny', 'exact_topic_permissions', 'keystore_preserved_before_removal')}},
        'native_context': {
            'witness': {'attested': True, 'enclave': '/witness', 'domain': 74, 'rmw': 'rmw_fastrtps_cpp',
                'security': 'Enforce', 'root': '/tmp/g09-keys/enclaves/witness'},
            'gateway': {'source_selected': True, 'enclave': '/gateway', 'domain': 74, 'rmw': 'rmw_fastrtps_cpp',
                'security': 'Enforce', 'root': '/tmp/g09-keys/enclaves/gateway',
                'inherited_override_cleared': True, 'direct_used_root_observed': False}},
        'products': products, 'roles': roles, 'qemu_sha256': expected['input_pins']['qemu']['sha256'],
        'markers': {'witness_text': candidate['witness_log'], 'guest_text': candidate['guest_log'],
                    'witness': scope['parse_witness_markers'](candidate['witness_log'], roles['witness']['pid'])},
        'gates': gates, 'commands': {'worker_exit': 0, 'witness_exit': 0, 'generation_exit': 0,
            'signed_verification_exits': [0, 0, 0], 'gateway_exit_after_worker': 'OWNED_CLEANUP'},
        'native_denial': {'attested': True, 'denied_native': 1, 'permission_events': 1,
            'raw_error_body_captured': False, 'predicate_bound_to_qualified_witness': True},
        'cleanup': {'owned_processes': [], 'mounts': [], 'socket_absent': True, 'secrets_removed': True, 'domain_lock_removed': True},
        'G09_complete': False}


def controller_models(scope):
    """Feed independently generated malicious inputs to the real pure helpers."""
    positives, refusals = [], []

    def allow(name, action):
        action()
        positives.append(name)

    def deny(name, action):
        try:
            action()
        except (RuntimeError, ValueError, KeyError, TypeError):
            refusals.append(name)
        else:
            raise AssertionError('controller admitted corrupted evidence: ' + name)

    candidate, expected = fixture()
    witness, guest = scope['parse_witness_markers'], scope['parse_guest_markers']
    pid = expected['processes']['witness']['pid']
    allow('real parser accepts independent closed witness text', lambda: witness(candidate['witness_log'], pid))
    allow('real parser accepts independent Guest text', lambda: guest(candidate['guest_log']))
    allow('real Guest parser permits ordinary Worker diagnostics', lambda: guest('Worker diagnostic\n' + candidate['guest_log']))
    allow('real incomplete parser admits READY prefix before match', lambda: witness(candidate['witness_log'].splitlines()[0] + '\n', pid, complete=False))
    lines = candidate['witness_log'].splitlines()
    for index in range(3):
        for mutate in ('missing', 'repeat', 'pid', 'unknown-field', 'wrong-node', 'CRLF', 'noncanonical-count'):
            altered = lines[:]
            if mutate == 'missing':
                altered.pop(index)
            elif mutate == 'repeat':
                altered.insert(index, altered[index])
            elif mutate == 'pid':
                altered[index] = altered[index].replace(f'pid={pid}', 'pid=999')
            elif mutate == 'unknown-field':
                altered[index] += ' unexpected=1'
            elif mutate == 'wrong-node':
                altered[index] = altered[index].replace('node=' + NODE, 'node=/other')
            elif mutate == 'CRLF':
                altered[index] += '\r'
            else:
                altered[index] = altered[index].replace(f'pid={pid}', f'pid=0{pid}')
            text = '\n'.join(altered) + '\n'
            deny('real witness parser mutation: ' + str(index) + '/' + mutate, lambda text=text: witness(text, pid))
    for text in (candidate['witness_log'] + 'unfinished', candidate['witness_log'][:-1]):
        deny('real complete native log refuses unfinished tail: ' + str(len(refusals)), lambda text=text: witness(text, pid))
    for text in ('PASS_SECURE_GUEST\n', candidate['guest_log'] * 2, candidate['guest_log'].replace('\n', '\r\n'),
                 candidate['guest_log'] + 'PASS_SECURE_GUESTjunk', candidate['guest_log'] + 'SECURE_ENDPOINTS_CREATEDjunk\n'):
        deny('real Guest parser mutation: ' + str(len(refusals)), lambda text=text: guest(text))
    for value in (0, -1, True, str(pid)):
        deny('real witness expected PID type/value: ' + repr(value), lambda value=value: witness(candidate['witness_log'], value))
    for key in GID_FIELDS:
        for value in ('0' * 32, 'A' * 32, 'a' * 30, expected['endpoint_gids']['witness_pub_gid']):
            if value == expected['endpoint_gids'][key]:
                continue
            text = candidate['witness_log'].replace(key + '=' + expected['endpoint_gids'][key], key + '=' + value)
            deny('real native GID parser mutation: ' + key + '/' + value, lambda text=text: witness(text, pid))
    for byte_count in range(33):
        if byte_count == 16:
            continue
        text = candidate['witness_log']
        for index, key in enumerate(GID_FIELDS):
            text = text.replace(expected['endpoint_gids'][key], bytes([0xa1 + index] * byte_count).hex())
        deny('real parser refuses coherent non16-byte GIDs bytes=' + str(byte_count),
             lambda text=text: witness(text, pid))
    check = scope['validate_combined_completion']
    row = controller_fixture(scope)
    allow('real completion predicate accepts independent synthetic shape', lambda: check(row))
    # New live controllers select the independently opened current incarnation;
    # it is deliberately distinct from retained historical model constants.
    selected={'path':'/synthetic/new-current-leaf','device':53,'inode':99123,
              'boot_id':'cccccccc-cccc-cccc-cccc-cccccccccccc'}
    fresh=copy.deepcopy(row);fresh['cgroup']['before']=copy.deepcopy(selected);fresh['cgroup']['after']=copy.deepcopy(selected)
    allow('explicit new incarnation is admitted independently of historical constants',lambda:check(fresh,selected))
    deny('historical row cannot satisfy explicit new incarnation',lambda:check(row,selected))
    for name in selected:
        wrong=copy.deepcopy(selected);wrong[name]='old' if name=='boot_id' else '/synthetic/replaced' if name=='path' else 1
        deny('explicit cgroup external pin differs '+name,lambda wrong=wrong:check(fresh,wrong))
    partial=copy.deepcopy(selected);partial.pop('inode')
    deny('partial explicit incarnation is refused',lambda:check(fresh,partial))
    boolean=copy.deepcopy(selected);boolean['device']=True
    deny('boolean is not an explicit cgroup device identity',lambda:check(fresh,boolean))
    for key in row:
        altered = copy.deepcopy(row)
        altered.pop(key)
        deny('real completion required field missing: ' + key, lambda altered=altered: check(altered))
    for key, value in [('format_version', True), ('mode', 'prepare'), ('kind', 'native-secure-arm'), ('G09_complete', True)]:
        altered = copy.deepcopy(row)
        altered[key] = value
        deny('real completion mode/type/scope: ' + key, lambda altered=altered: check(altered))
    for name in ('worker_exit', 'witness_exit', 'generation_exit'):
        for value in (1, False):
            altered = copy.deepcopy(row)
            altered['commands'][name] = value
            deny('real completion exact command exit: ' + name + '/' + repr(value), lambda altered=altered: check(altered))
    for key, value in [('memory_max', 51539607552), ('swap_max', 'max'), ('zero_oom', False), ('readable_ancestors', False)]:
        altered = copy.deepcopy(row)
        altered['cgroup'][key] = value
        deny('real completion current cgroup budget: ' + key, lambda altered=altered: check(altered))
    for key in ('device', 'inode', 'boot_id'):
        altered = copy.deepcopy(row)
        value = 'old-boot' if key == 'boot_id' else 999
        altered['cgroup']['before'][key] = altered['cgroup']['after'][key] = value
        deny('real completion coherent old incarnation: ' + key, lambda altered=altered: check(altered))
    for role in ROLES:
        for key, value in [('qualified', False), ('snapshot', copy.deepcopy(row['products'][role]['origin']))]:
            altered = copy.deepcopy(row)
            altered['products'][role][key] = value
            deny('real completion immutable product: ' + role + '/' + key, lambda altered=altered: check(altered))
        for key, value in [('live_before_G', False), ('membership', ['0::/other'])]:
            altered = copy.deepcopy(row)
            altered['roles'][role][key] = value
            deny('real completion actual role origin: ' + role + '/' + key, lambda altered=altered: check(altered))
        altered = copy.deepcopy(row)
        altered['roles'][role]['mapped_product']['inode'] += 1
        deny('real completion live mapping must bind exact snapshot: ' + role, lambda altered=altered: check(altered))
    for index in range(4):
        for key, value in [('pid', 999), ('starttime_ticks', 1), ('sent', False), ('current_birth_mapping_verified', False)]:
            altered = copy.deepcopy(row)
            altered['gates'][index][key] = value
            deny('real completion gate association: ' + str(index) + '/' + key, lambda altered=altered: check(altered))
        altered = copy.deepcopy(row)
        altered['gates'].pop(index)
        deny('real completion gate missing: ' + str(index), lambda altered=altered: check(altered))
    altered = copy.deepcopy(row)
    altered['gates'][3]['guest_exit'] = False
    deny('real completion F needs exact integer zero Guest exit', lambda: check(altered))
    for role in ('gateway', 'witness'):
        for key in row['native_context'][role]:
            altered = copy.deepcopy(row)
            altered['native_context'][role][key] = 'substituted'
            deny('real completion native context/source-selection: ' + role + '/' + key, lambda altered=altered: check(altered))
        for key, value in row['native_context'][role].items():
            if type(value) is bool:
                altered = copy.deepcopy(row)
                altered['native_context'][role][key] = int(value)
                deny('real completion exact boolean context fact: ' + role + '/' + key, lambda altered=altered: check(altered))
    for key, value in row['native_denial'].items():
        altered = copy.deepcopy(row)
        altered['native_denial'][key] = int(value) if type(value) is bool else True
        deny('real completion exact denial predicate leaf type: ' + key, lambda altered=altered: check(altered))
    altered = copy.deepcopy(row)
    old_pid = altered['roles']['witness']['pid']
    pid = altered['roles']['gateway']['pid']
    process = altered['roles']['witness']
    process['pid'] = process['proof']['actual_stopped_ARM']['pid'] = process['proof']['target_proof']['pid'] = pid
    process['proof']['ancestry'][-1] = process['proof']['target_proof']['ancestry'][-1] = pid
    for gate in altered['gates']:
        if gate['role'] == 'witness':
            gate['pid'] = pid
    altered['markers']['witness_text'] = altered['markers']['witness_text'].replace(f'pid={old_pid}', f'pid={pid}')
    altered['markers']['witness'] = witness(altered['markers']['witness_text'], pid)
    deny('real completion same PID with different births cannot be independent participants', lambda: check(altered))
    for key in row['signed']:
        altered = copy.deepcopy(row)
        altered['signed'][key] = None
        deny('real completion signed policy proof missing: ' + key, lambda altered=altered: check(altered))
    for key in row['cleanup']:
        altered = copy.deepcopy(row)
        altered['cleanup'][key] = None
        deny('real completion cleanup proof missing: ' + key, lambda altered=altered: check(altered))
    return {'positive_models': positives, 'refusal_models': refusals}


def source_models(root, scope):
    """Verify the current source contract without importing any live helper."""
    names = []

    def check(name, condition):
        require(condition, 'source contract: ' + name)
        names.append(name)

    runtime = ast.parse((root / 'tests/ros_aarch64/runtime.py').read_text())
    limits = [ast.literal_eval(node.value) for node in runtime.body if isinstance(node, ast.Assign) and
              len(node.targets) == 1 and isinstance(node.targets[0], ast.Name) and node.targets[0].id == 'EXPECTED_MEMORY_MAX']
    check('current runtime guard requires64GiB integer', limits == [MEMORY_MAX] and type(limits[0]) is int)
    check('combined controller uses current64GiB and domain74', scope['MEMORY_MAX'] == MEMORY_MAX and scope['DOMAIN'] == 74)
    qualifier = ast.parse((root / 'tests/ros_aarch64/gateway_qualification.py').read_text())
    architecture = next(node for node in qualifier.body if isinstance(node, ast.FunctionDef) and node.name == 'architecture_binding')
    constants = {node.value for node in ast.walk(architecture) if isinstance(node, ast.Constant) and type(node.value) is int}
    check('current architecture qualifier includes64GiB and no48GiB acceptance constant', MEMORY_MAX in constants and 51539607552 not in constants)
    wat = (root / 'tests/ros_security/guest_enforce_ros2.wat').read_text()
    executable = '\n'.join(line.split(';;', 1)[0] for line in wat.splitlines())
    imported = re.findall(r'\(import "([^"]+)" "([^"]+)"', executable)
    expected = [('uwvm_ros2_v1', name) for name in ('capability_open', 'capability_close', 'topic_publish', 'topic_subscribe', 'event_poll')]
    expected += [('wasi_snapshot_preview1', name) for name in ('fd_read', 'fd_write', 'proc_exit', 'clock_time_get')]
    check('Guest imports closed to five ROS2 and four explicit WASI functions', imported == expected)
    check('Guest has sole fixed Wasm32 exported memory', executable.count('(memory (export "memory") 1 1)') == 1 and executable.count('(memory ') == 1)
    check('Guest two gate bytes are exactlyG thenM', re.findall(r'\(call \$gate \(i32.const (0x[0-9a-f]+)\)\)', executable) == ['0x47', '0x4d'])
    check('Guest has two admitted publishes and one stale attempt', executable.count('(call $publish ') == 3 and executable.count('(call $check (call $publish ') == 2)
    check('Guest ungranted alias open requires13 at protected handle96', re.search(
        r'\(i32.eq\s+\(call \$open \(i32.const 0\) \(i64.const 32\) \(i64.const 10\) \(i64.const 96\)\) \(i32.const 13\)\)', executable) is not None)
    check('Guest stale publication requires9 using original local publisher', re.search(
        r'\(i32.eq\s+\(call \$publish \(local.get \$pub\) \(i32.const 0\) \(i64.const 4096\) \(i64.const 31\)\) \(i32.const 9\)\)', executable) is not None)
    check('Guest empty poll permits11 with untouched length sentinel',
          '(i32.eq (local.get $error) (i32.const 11))' in executable and
          '(i64.eq (i64.load (i32.const 128)) (i64.const -1))' in executable and
          '(call $unchanged_tail (i32.const 0))' in executable)
    check('Guest success checks exact23 length/all-byte loop/protected41B tail',
          '(i64.eq (i64.load (i32.const 128)) (i64.const 23))' in executable and
          '(br_if $equal (i32.ge_u (local.get $i) (i32.const 23)))' in executable and
          '(i32.load8_u (i32.add (i32.const 256) (local.get $i)))' in executable and
          '(i32.load8_u (i32.add (i32.const 4160) (local.get $i)))' in executable and
          '(call $unchanged_tail (i32.const 23))' in executable)
    check('Guest closes each original local opaque handle', '(call $close (local.get $pub))' in executable and '(call $close (local.get $sub))' in executable)
    check('Guest explicit monotonic deadline and output capacity', '(call $clock (i32.const 1) (i64.const 1) (i32.const 192))' in executable and
          '(i64.const 10000000000)' in executable and '(i64.const 256) (i64.const 64) (i64.const 128)' in executable)
    literals = {}
    for offset, encoded in re.findall(r'\(data \(i32.const ([0-9]+)\) "([^"]*)"\)', executable):
        data = bytearray()
        i = 0
        while i < len(encoded):
            if encoded[i] == '\\':
                require(i + 2 < len(encoded) and re.fullmatch(r'[0-9a-f]{2}', encoded[i + 1:i + 3]) is not None,
                        'simple source decoder accepts explicit byte escapes only')
                data.append(int(encoded[i + 1:i + 3], 16))
                i += 3
            else:
                data.extend(encoded[i].encode())
                i += 1
        literals[int(offset)] = bytes(data)
    out = b'uwvm real ROS exact payload'
    reply = b'native signed reply'
    check('Guest canonical31/23 literal records equal independent UTF8 construction',
          literals.get(4096) == len(out).to_bytes(4, 'little') + out and literals.get(4160) == len(reply).to_bytes(4, 'little') + reply)
    check('Guest exact endpoint25/PASS18 marker byte literals', literals.get(4352) == b'SECURE_ENDPOINTS_CREATED\n' and
          literals.get(4416) == b'PASS_SECURE_GUEST\n' and len(literals[4352]) == 25 and len(literals[4416]) == 18)
    abi = json.loads((root / 'sdk/abi/abi-v1.json').read_text())
    declared = {item['name']: item for item in abi['modules']['uwvm_ros2_v1']['functions']}
    for name in ('capability_open', 'capability_close', 'topic_publish', 'topic_subscribe', 'event_poll'):
        match = re.search(r'\(import "uwvm_ros2_v1" "' + name + r'" \(func \$\w+ \(param ([^)]*)\) \(result ([^)]*)\)\)\)', executable)
        check('raw ROS2 imported ABI agrees with current catalog: ' + name, match is not None and
              match.group(1).split() == [p['type'] for p in declared[name]['params']] and match.group(2).split() == declared[name]['results'])
    policy = ET.fromstring((root / 'tests/ros_security/sros2-policy.xml').read_bytes())
    profiles = {}
    for enclave in policy.findall('./enclaves/enclave'):
        for profile in enclave.findall('./profiles/profile'):
            identifier = (enclave.attrib['path'], profile.attrib['ns'], profile.attrib['node'])
            require(identifier not in profiles, 'duplicate signed profile')
            groups = profile.findall('topics')
            profiles[identifier] = {tuple(topic.text for topic in group.findall('topic')):
                                    dict(group.attrib) for group in groups}
            require(len(profiles[identifier]) == len(groups), 'duplicate signed direction group')
    gateway = profiles.get(('/gateway', '/uwvm_test', 'uwvm_ros2_gateway_probe'), {})
    witness = profiles.get(('/witness', '/uwvm_test', 'secure_witness'), {})
    check('two exact signed enclave/node profiles', len(profiles) == 2 and bool(gateway) and bool(witness))
    check('Gateway signed direction is explicit pubString/subString', gateway == {
        ('/uwvm_test/pubString',): {'publish': 'ALLOW'}, ('/uwvm_test/subString', '/parameter_events'): {'subscribe': 'ALLOW'}})
    check('independent witness signed directions are complementary', witness == {
        ('/uwvm_test/pubString', '/parameter_events'): {'subscribe': 'ALLOW'}, ('/uwvm_test/subString',): {'publish': 'ALLOW'}})
    check('valid native denied topic absent from every signed allow rule', all('/uwvm_test/not_permitted' not in topics for group in profiles.values() for topics in group))
    native = (root / 'tests/ros_security/guest_enforce_witness_ros2.cc').read_text()
    check('native public context/RMW/GID source contracts present', all(value in native for value in
        ('rcl_context_get_rmw_context', 'RMW_SECURITY_ENFORCEMENT_ENFORCE', 'security_root_path', 'publisher_gid', 'from_intra_process')))
    check('native witness enforces actual Jazzy16-byte RMW ABI',
          re.search(r'static_assert\s*\(\s*RMW_GID_STORAGE_SIZE\s*==\s*16\s*,', native) is not None and
          re.search(r'static_assert\s*\(\s*RMW_GID_STORAGE_SIZE\s*==\s*24\s*,', native) is None)
    check('native exact permission scanner remains source-bound', all(value in native for value in
        ('get_datawriter_sec_attributes', 'rt/uwvm_test/not_permitted topic not found in allow rule.', 'permission_events', 'denied_native=1')))
    check('no post-context native ClearConsumers call', native.count('dds::Log::ClearConsumers();') == 1)
    doc = (root / 'tests/ros_aarch64/README.g09-guest-enforce.md').read_text()
    check('English documentation states prepared scope/current64GiB', 'this document does not report a compilation' in doc and '**64 GiB**' in doc and
          'memory.max=68719476736' in doc and 'memory.swap.max=0' in doc)
    check('documentation preserves raw-denial and Gateway context boundary', 'raw `Log::Entry` body is not captured' in doc and
          'does\nnot invent a separate public Gateway context measurement' in doc)
    return names


def native_wrapper_models(path):
    """Model the checked witness-only setup chain without launching a shell.

    Only the nested argv-construction function is AST-extracted. The status
    table below models Bash's documented && short-circuit behavior with fake
    source/unset/absence/exec results; it is not actual ARM Bash execution.
    """
    raw = Path(path).read_bytes().decode('utf-8', 'strict')
    tree = ast.parse(raw)
    main = next(node for node in tree.body if isinstance(node, ast.FunctionDef) and node.name == 'main')
    native = next(node for node in main.body if isinstance(node, ast.FunctionDef) and node.name == 'native')
    selected = {'root': Path('/synthetic/owned-sdk-mount')}
    exec(compile(ast.Module(body=[native], type_ignores=[]), '<pure-native-argv-only>', 'exec'), selected)
    checks = []

    def check(name, condition):
        require(condition, name)
        checks.append(name)

    arguments = ['/tmp/g09-secure-witness', '/tmp/g09-keys']
    ordinary = selected['native'](arguments)
    explicit_ordinary = selected['native'](arguments, witness=False)
    witness = selected['native'](arguments, witness=True)
    legacy = 'source /opt/ros/jazzy/setup.bash && exec "$@"'
    checked = ('source /opt/ros/jazzy/setup.bash && unset ROS_AUTOMATIC_DISCOVERY_RANGE && '
               '[[ ! -v ROS_AUTOMATIC_DISCOVERY_RANGE ]] && exec "$@"')
    check('ordinary native tool/Gateway argv is exactly preserved', ordinary == explicit_ordinary and ordinary[-4:] == [legacy, 'g09-signed-native', *arguments])
    check('witness argv uses exact checked post-setup cleanup', witness[-4:] == [checked, 'g09-signed-native', *arguments])
    check('witness cleanup changes only the SDK launch-script slot', witness[:-4] == ordinary[:-4] and witness[-3:] == ordinary[-3:])
    check('checked cleanup follows source and precedes unchanged exec', checked.split(' && ') == ['source /opt/ros/jazzy/setup.bash',
          'unset ROS_AUTOMATIC_DISCOVERY_RANGE', '[[ ! -v ROS_AUTOMATIC_DISCOVERY_RANGE ]]', 'exec "$@"'])
    check('only the witness launch opts into post-setup cleanup', raw.count('witness=True') == 1 and
          "native(['/tmp/'+WITNESS,KEYSTORE],witness=True)" in raw)
    check('Gateway test override remains explicit and unfiltered', "gateway_env['ROS_SECURITY_ENCLAVE_OVERRIDE'] = '/witness'" in raw and
          "native(['/tmp/gateway','/tmp/control/gateway.conf'])" in raw)
    check('no broad environment erase or additional native helper used', 'env -i' not in checked and '/usr/bin/env' not in checked and
          checked.count('unset ') == 1 and 'ROS_SECURITY_ENCLAVE_OVERRIDE' not in checked)
    for source_status in (0, 1, 126):
        for unset_status in (0, 1, 126):
            for absence_status in (0, 1):
                for exec_status in (0, 1, 126):
                    statuses = (source_status, unset_status, absence_status, exec_status)
                    called = []
                    result = 0
                    for stage, status in zip(('source', 'unset', 'absence', 'exec'), statuses):
                        called.append(stage)
                        result = status
                        if status != 0:
                            break
                    expected_end = next((index + 1 for index, status in enumerate(statuses) if status != 0), 4)
                    check('fake checked && chain refuses nonzero and stops before later stages ' + repr(statuses),
                          called == list(('source', 'unset', 'absence', 'exec')[:expected_end]) and
                          (result == 0) == all(status == 0 for status in statuses) and
                          ('exec' in called) == all(status == 0 for status in statuses[:3]))
    return checks


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[2])
    parser.add_argument('--out', type=Path, required=True, help='Fresh source/model JSON receipt; existing files are refused.')
    args = parser.parse_args()
    root = args.source.resolve(strict=True)
    args.out.parent.mkdir(parents=True, exist_ok=True)
    require(not args.out.exists(), 'use a fresh model output path')
    pins = {name: source_pin(root / name) for name in SOURCES}
    result = run_models()
    selected = pure_controller(root / 'tests/ros_aarch64/run_g09_guest_enforce.py')
    controller = controller_models(selected)
    source_checks = source_models(root, selected)
    native_wrappers = native_wrapper_models(root / 'tests/ros_aarch64/run_g09_guest_enforce.py')
    require(pins == {name: source_pin(root / name) for name in SOURCES}, 'source inputs changed during pure models')
    receipt = {'format': 'uwvm-g09-guest-pure-models-v1', 'status': 'PASS_SOURCE_MODELS_ONLY',
        'actual_execution': 'NOT_EXECUTED', 'actual_signature_verification': 'NOT_EXECUTED',
        'actual_compilation': 'NOT_EXECUTED', 'actual_wasm_validation': 'NOT_EXECUTED',
        'model_scope': 'Independent synthetic evidence association/marker oracle. Candidate receipt schema is an abstract model, not the controller report schema.',
        'future_memory_max_bytes': MEMORY_MAX, 'future_swap_max_bytes': 0, 'source_pins': pins,
        'positive_models': result['positive_models'], 'refusal_models': result['refusal_models'],
        'real_controller_positive_models': controller['positive_models'],
        'real_controller_refusal_models': controller['refusal_models'],
        'source_checks': source_checks,
        'native_wrapper_source_and_fake_chain_checks': native_wrappers,
        'checks': sum(len(values) for values in (result['positive_models'], result['refusal_models'],
                                               controller['positive_models'], controller['refusal_models'], source_checks, native_wrappers))}
    with args.out.open('x') as stream:
        stream.write(json.dumps(receipt, indent=2) + '\n')
    print(json.dumps({'status': receipt['status'], 'checks': receipt['checks'], 'report': source_pin(args.out)}, indent=2))


if __name__ == '__main__':
    main()
