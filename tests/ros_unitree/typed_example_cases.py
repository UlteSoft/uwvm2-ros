#!/usr/bin/env python3
"""Closed acceptance descriptions for the real typed Unitree Guest examples.

This module is data and validation for a future independently owned Runtime
driver.  It does not compile, instantiate, publish, simulate, or report PASS.
In particular, a codec/compiler receipt cannot satisfy any transport witness
below.  A caller must supply actual current producer pins and actual native
peer observations; no executable, native identity, or message hash is invented
here.  The two policies always run in different fresh Runtime instances.
"""

from __future__ import annotations

import re


FORMAT = 'uwvm-unitree-typed-native-cases-v1'
STATUS = 'SOURCE_ACCEPTANCE_DESCRIPTIONS_NOT_EXECUTED'
RECIPE = 'sdk/robots/unitree/recipes/closedtyped-example-recipes.json'
PRODUCTS = (
    ('rust-lowstate-go', 'lowstate', 'unitree_go', 20),
    ('rust-lowstate-hg', 'lowstate', 'unitree_hg', 35),
    ('rust-topic-rpc-api', 'topic-rpc', 'unitree_api', 0),
    ('javascript-lowstate-go', 'lowstate', 'unitree_go', 20),
    ('javascript-lowstate-hg', 'lowstate', 'unitree_hg', 35),
    ('javascript-topic-rpc-api', 'topic-rpc', 'unitree_api', 0),
    ('assemblyscript-lowstate-go', 'lowstate', 'unitree_go', 20),
    ('assemblyscript-lowstate-hg', 'lowstate', 'unitree_hg', 35),
    ('assemblyscript-topic-rpc-api', 'topic-rpc', 'unitree_api', 0),
    ('tinygo-lowstate-go', 'lowstate', 'unitree_go', 20),
    ('tinygo-lowstate-hg', 'lowstate', 'unitree_hg', 35),
    ('tinygo-topic-rpc-api', 'topic-rpc', 'unitree_api', 0),
    ('c-topic-rpc-api', 'topic-rpc', 'unitree_api', 0),
    ('cpp-topic-rpc-api', 'topic-rpc', 'unitree_api', 0),
)
PIN_FIELDS = frozenset(('path', 'device', 'inode', 'bytes', 'mtime_ns',
                        'ctime_ns', 'sha256'))


def require_pin(value: object) -> None:
    """Require a physical opened-file pin, not a self-declared producer label.

    The actual driver must reopen it and compare all seven facts before and
    after execution.  Shape validation here alone never proves those facts.
    """
    if not isinstance(value, dict) or set(value) != PIN_FIELDS:
        raise ValueError('actual full seven-field pin required')
    if not isinstance(value['path'], str) or not value['path'].startswith('/'):
        raise ValueError('absolute physical pin path required')
    for name in ('device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns'):
        if type(value[name]) is not int or value[name] < 0:
            raise ValueError('actual integer physical facts required')
    if value['inode'] == 0 or value['bytes'] == 0 or not isinstance(value['sha256'], str) \
            or re.fullmatch(r'[0-9a-f]{64}', value['sha256']) is None:
        raise ValueError('nonempty opened product and complete SHA256 required')


def describe_cases() -> tuple[dict, ...]:
    """Describe all 28 finite cases without creating a Runtime or native actor.

    This deliberately has no fallback native fixture.  The future caller must
    bind a separately compiled real ROS2 peer that uses the same actual native
    generated GO/HG/API definitions and exports their true native identities.
    A String relay, fabricated host callback, JS Node mock, or manually encoded
    replacement message cannot satisfy this description.
    """
    result = []
    for product, role, family, slots in PRODUCTS:
        family_arg = 1 if family == 'unitree_go' else 2
        initialize = (family_arg, 0) if role == 'lowstate' else (1, 1)
        for positive in (False, True):
            row = {
                'id': product + ('-positive' if positive else '-default-denied'),
                'product': product, 'recipe': RECIPE, 'backend': 2,
                'role': role, 'family': family, 'fresh_instance': True,
                'initialize_case': (int(positive), *initialize),
                'poll_limit': 4096, 'finite_process_deadline_required': True,
                'real_runtime_required': True, 'actual_native_peer_required': True,
                'actual_source_and_whole_input_preservation_required': True,
                'actual_owned_actor_cleanup_required': True,
                'local_only_operator_domain': 1,
                'aliases': (), 'expected_initial_status': 0,
                'expected_denied_verified': int(not positive),
                'expected_received_count': 1 if positive else 0,
            }
            if not positive:
                row.update({
                    'policy': 'actual-default-empty',
                    'native_witness': (
                        'no newly created Guest subscription or publisher',
                        'zero Guest-origin payload before and after denial',
                        'actual alias-open result 13; no implicit grant',
                    ),
                    'after_denial': {
                        'poll_once': 9, 'close_case': 9,
                        'submit_once': 9 if role == 'topic-rpc' else 'not-exported',
                    },
                })
            elif role == 'lowstate':
                row.update({
                    'policy': 'explicit-one-topic-subscription',
                    'aliases': (('unitree-lowstate', 'topic.subscribe',
                                 family + '/msg/LowState'),),
                    'native_witness': (
                        'real native matched subscription before publication',
                        'one independently witnessed complete LowState',
                        'native tick and crc equal Guest scalar observations',
                        'all carrier q/dq/tau_est binary32 words equal',
                        'zero LowCmd publications and zero service calls',
                    ),
                    'carrier_slots': slots, 'motor_word_fields': ('q', 'dq', 'tau_est'),
                    'all_observed_floats_finite': True,
                    'getter_word_count': slots * 3,
                    'after_success': {'case_status': 0, 'close_case': 0,
                                      'second_close_case': 9},
                })
            else:
                row.update({
                    'policy': 'explicit-independent-request-response-topics',
                    'aliases': (
                        ('unitree-api-request', 'topic.publish', 'unitree_api/msg/Request'),
                        ('unitree-api-response', 'topic.subscribe', 'unitree_api/msg/Response'),
                    ),
                    'request': {'id': 1, 'api_id': 1, 'parameter': '{}', 'binary': ()},
                    'response': {'id': 1, 'api_id': 1, 'status_code': 0,
                                 'data': 'typed-example-ack', 'binary': ()},
                    'native_witness': (
                        'native Request subscription and Guest Response subscription matched before submission',
                        'actual lazily created Request publisher matched and full Request received after submission',
                        'exactly one independently decoded full Request',
                        'matching native fixture Response only after Request',
                        'one Guest full Response observation; response_code 0',
                        'zero ROS services/actions or firmware API endpoints',
                    ),
                    'submission': {'first_submit_once': 0, 'second_submit_once': 9},
                    'after_success': {'case_status': 0, 'close_case': 0,
                                      'second_close_case': 9},
                })
            result.append(row)
    return tuple(result)


def require_actual_selection(value: object) -> None:
    """Validate only the closed future-driver input envelope, never its truth.

    Every receipt remains separately hash-bound.  The actual driver must verify
    actual MD/source provenance, native identity export, matching registry and
    peer mapped files; copying a string identifier into this mapping is not an
    identity export.  Legacy ROS1 is intentionally absent until an actual
    native Noetic binding has been acquired and built.
    """
    keys = {'format', 'status', 'source', 'source_manifest', 'recipe',
            'runtime', 'gateway', 'native_peer', 'native_peer_receipt',
            'native_identity_export', 'registry', 'products'}
    if not isinstance(value, dict) or set(value) != keys \
            or value['format'] != FORMAT or value['status'] != 'ACTUAL_INPUTS_REQUIRED':
        raise ValueError('closed actual typed-native selection required')
    if not isinstance(value['source'], str) or not value['source'].startswith('/'):
        raise ValueError('absolute frozen source root required')
    for name in keys - {'format', 'status', 'source', 'products'}:
        require_pin(value[name])
    rows = value['products']
    expected = {row[0] for row in PRODUCTS}
    if not isinstance(rows, list) or len(rows) != len(expected):
        raise ValueError('all fourteen actual product bindings required')
    seen = set()
    for row in rows:
        if not isinstance(row, dict) or set(row) != {'id', 'binary', 'receipt', 'family'} \
                or row['id'] not in expected or row['id'] in seen:
            raise ValueError('unique exact typed product binding required')
        seen.add(row['id'])
        expected_family = next(item[2] for item in PRODUCTS if item[0] == row['id'])
        if row['family'] != expected_family:
            raise ValueError('selected complete native family mismatch')
        require_pin(row['binary'])
        require_pin(row['receipt'])
