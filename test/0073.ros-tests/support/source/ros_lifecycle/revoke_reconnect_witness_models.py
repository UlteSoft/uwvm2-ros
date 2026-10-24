#!/usr/bin/env python3
"""Check prepared native witness contracts using independent synthetic data only.

No controller is imported. No compiler, validator, crypto command, ROS process,
Guest or QEMU executes. Expected identities remain outside candidate evidence;
coherent substitutions cannot manufacture an independent native observation.
The delivery oracle counts mathematical trace selections, rather than calling
or reproducing the C++ callback implementation. Source checks have a separate
scope and cannot turn these synthetic inputs into an actual runtime receipt.
"""
import sys
sys.dont_write_bytecode = True
import argparse
import copy
import hashlib
import itertools
import json
from pathlib import Path
import re

COMMON = ('pid', 'node', 'enclave', 'domain', 'rmw', 'type', 'dead_topic', 'live_topic',
          'dead_sub_gid', 'live_sub_gid')
OLD_GIDS = ('old_dead_pub_gid', 'old_live_pub_gid')
MARKERS = (
    ('READY_REVOKE_WITNESS', ()),
    ('MATCHED_REVOKE_OLD', (*OLD_GIDS, 'dead_publishers', 'live_publishers')),
    ('BASELINE_REVOKE_WITNESS', (*OLD_GIDS, 'dead_before', 'live_before')),
    ('FROZEN_REVOKE_WITNESS', (*OLD_GIDS, 'dead_total', 'live_before', 'unauthorized_dead')),
    ('MATCHED_REVOKE_NEW', (*OLD_GIDS, 'new_live_pub_gid', 'dead_publishers', 'live_publishers',
                           'dead_total', 'live_before', 'live_after')),
    ('DELIVERED_REVOKE_NEW', (*OLD_GIDS, 'new_live_pub_gid', 'dead_publishers', 'live_publishers',
                             'dead_total', 'live_before', 'live_after')),
    ('PASS_REVOKE_RECONNECT_WITNESS', (*OLD_GIDS, 'new_live_pub_gid', 'dead_publishers', 'live_publishers',
        'dead_total', 'live_before', 'live_after', 'unauthorized_dead', 'fault_live', 'unknown',
        'quiet_samples', 'quiet_ns', 'warmup_dead', 'warmup_live', 'warmup_new')),
)
GIDS = ('dead_sub_gid', 'live_sub_gid', *OLD_GIDS, 'new_live_pub_gid')
SOURCE_NAMES = ('test/0073.ros-tests/support/source/ros_lifecycle/ros2_revoke_reconnect_witness.cc',
                'test/0073.ros-tests/support/source/ros_lifecycle/README.revoke-reconnect-witness.md',
                'test/0073.ros-tests/support/source/ros_lifecycle/revoke_reconnect_witness_models.py')
ENDPOINT_DIAGNOSTIC_FIELDS = (
    'name_unknown', 'name_bare_dds', 'name_mismatch', 'namespace_unknown',
    'namespace_bare_dds', 'namespace_mismatch', 'type', 'direction', 'hash',
    'zero_gid', 'reliability', 'durability')
DIAGNOSTIC_FIELDS = {
    'native_exception': ('unspecified',), 'contract': ('unspecified',),
    'callback_origin': ('implementation', 'zero_gid'),
    'callback_source': ('null_or_intraprocess',),
    'dead_delivery': ('warmup_rejected', 'baseline_rejected', 'unexpected_text'),
    'live_delivery': ('fault_text', 'warmup_rejected', 'baseline_rejected',
                      'reconnect_rejected', 'after_rejected', 'unexpected_text'),
    'callback_counters': ('forbidden_effect', 'positive_count', 'warmup_count'),
    'frozen_counters': ('baseline_count', 'warmup_count'),
    'own_subscription_graph': ('shape',), 'old_publisher_graph': ('shape',),
    'fresh_dead_graph': ('shape', 'old_gid'),
    'fresh_own_subscriptions': ('identity',),
    'fresh_live_graph': ('shape', 'duplicate_old', 'duplicate_fresh', 'independent_gid', 'warmup_origin'),
    'fresh_snapshot': ('delivery_count',),
    'exchange_deadline': ('before_spin', 'after_spin', 'after_sleep', 'before_new_marker'),
    **{site: ENDPOINT_DIAGNOSTIC_FIELDS for site in ('own_subscription', 'old_publisher',
       'fresh_old_dead', 'fresh_dead_candidate', 'fresh_old_live', 'fresh_live_candidate')},
}
DIAGNOSTIC_PHASES = ('input', 'initialization', 'own-discovery', 'old-discovery',
    'baseline', 'revoke-gate', 'fresh-discovery', 'survivor-delivery',
    'completion-gate', 'quiet-observation', 'cleanup')
PENDING_ASSOCIATIONS = (('_NODE_NAME_UNKNOWN_', '_NODE_NAMESPACE_UNKNOWN_'),
                        ('_CREATED_BY_BARE_DDS_APP_', '_CREATED_BY_BARE_DDS_APP_'))
GRAPH_METADATA = {'node': 'uwvm_ros2_gateway_probe', 'namespace': '/uwvm_revoke_test',
                  'type': 'std_msgs/msg/String', 'direction': 'publisher',
                  'hash': 'RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18',
                  'reliability': 'reliable', 'durability': 'volatile'}


def require(condition, reason):
    if not condition:
        raise ValueError(reason)


def gid(text):
    """Admit only a complete nonzero sixteen-byte lowercase native identity."""
    return isinstance(text, str) and re.fullmatch('[0-9a-f]{32}', text) is not None and text != '0' * 32


def decimal(text):
    require(isinstance(text, str) and re.fullmatch('0|[1-9][0-9]*', text) is not None,
            'canonical unsigned decimal required')
    result = int(text)
    require(result <= 2**64 - 1, 'unsigned decimal exceeds native representation')
    return result


def oracle_failure(text):
    """Classify a fixed failure record without accepting any lifecycle result.

    No candidate string becomes a discovered name, payload or exception body.
    The returned record is explicitly failure-only; the success-ledger oracle
    must still reject it, even when every diagnostic field is well formed.
    """
    require(isinstance(text, str) and text.endswith('\n') and text.count('\n') == 1
            and '\r' not in text, 'one exact LF failure record required')
    parts = text[:-1].split(' ')
    require(len(parts) == 4 and parts[0] == 'FAIL_REVOKE_RECONNECT_WITNESS', 'closed native failure prefix required')
    values = {}
    for token, key in zip(parts[1:], ('phase', 'site', 'field')):
        require(token.count('=') == 1 and token.startswith(key + '='), 'ordered fixed diagnostic fields required')
        values[key] = token[len(key) + 1:]
    require(values['phase'] in DIAGNOSTIC_PHASES and values['site'] in DIAGNOSTIC_FIELDS
            and values['field'] in DIAGNOSTIC_FIELDS[values['site']], 'source-owned fixed diagnostic code required')
    return {**values, 'lifecycle_accepted': False}


def source_diagnostic_catalog(native):
    """Inspect fixed C++ diagnostic literals without interpreting native code.

    This source-conformance check is separate from the independent failure
    oracle. It catches missing catalog entries, including multiline calls,
    while never calling a C++ predicate or selecting discovered peer values.
    Only fixed lowercase field literals and source-local site labels qualify.
    """
    text = re.sub(r'//[^\n]*|/\*[\s\S]*?\*/', '', native)
    result = {'native_exception': {'unspecified'}}
    endpoint_fields = set()
    for match in re.finditer(r'\brequire_at\s*\(', text):
        start = index = match.end()
        depth = 1
        quote = None
        commas = []
        braces = brackets = 0
        while depth:
            char = text[index]
            if quote:
                if char == '\\':
                    index += 2
                    continue
                if char == quote:
                    quote = None
            elif char in ('"', "'"):
                quote = char
            elif char == '(':
                depth += 1
            elif char == ')':
                depth -= 1
            elif char == '{':
                braces += 1
            elif char == '}':
                braces -= 1
            elif char == '[':
                brackets += 1
            elif char == ']':
                brackets -= 1
            elif char == ',' and depth == 1 and braces == brackets == 0:
                commas.append(index)
            index += 1
        if len(commas) != 2:
            continue
        site = text[commas[0] + 1:commas[1]].strip()
        fields = set(re.findall(r'"([a-z][a-z0-9_]*)"', text[commas[1] + 1:index - 1]))
        if site == 'site':
            endpoint_fields.update(fields)
        elif re.fullmatch(r'"[a-z][a-z0-9_]*"', site):
            result.setdefault(site[1:-1], set()).update(fields)
    endpoint_sites = {'own_subscription', 'old_publisher'}
    for labels in re.findall(r'auto site\{([^;]+)\};', text):
        endpoint_sites.update(re.findall(r'"([a-z][a-z0-9_]*)"', labels))
    for site in endpoint_sites:
        result.setdefault(site, set()).update(endpoint_fields)
    return result


def expected_facts():
    """Return explicitly synthetic trusted facts, never candidate-selected facts."""
    result = {'pid': '71001', 'node': '/uwvm_revoke_test/revoke_witness', 'enclave': '/witness',
              'domain': '75', 'rmw': 'rmw_fastrtps_cpp', 'type': 'std_msgs/msg/String',
              'dead_topic': '/uwvm_revoke_test/dead', 'live_topic': '/uwvm_revoke_test/live'}
    for index, key in enumerate(GIDS):
        result[key] = bytes([0xa1 + index] * 16).hex()
    return result


def oracle_markers(text, expected):
    """Verify a whole native marker ledger against independent expected facts.

    This deliberately accepts no raw prefix success, foreign PID/context/GID,
    another width or old native-only positive fixture. Publisher counts at PASS
    retain the MATCHED_NEW snapshot; they are not post-close graph evidence.
    """
    require(isinstance(text, str) and text.endswith('\n') and '\r' not in text,
            'complete native ledger requires LF with no CR')
    lines = text.splitlines()
    require(len(lines) == len(MARKERS), 'exactly seven native markers required')
    rows = []
    for line, (name, tails) in zip(lines, MARKERS):
        tokens = line.split(' ')
        require(tokens[0] == name and all(tokens), 'ordered exact marker and spacing required')
        fields = {}
        ordered = []
        for token in tokens[1:]:
            require(token.count('=') == 1, 'one field assignment required')
            key, value = token.split('=')
            require(key not in fields and value, 'no duplicate/empty field allowed')
            fields[key] = value
            ordered.append(key)
        require(tuple(ordered) == COMMON + tails, 'closed exact field order required')
        require(all(fields[key] == expected[key] for key in COMMON), 'independent context/subscriber facts differ')
        require(decimal(fields['pid']) > 0 and decimal(fields['domain']) == 75, 'checked process/domain required')
        for key, value in fields.items():
            if key.endswith('_gid'):
                require(gid(value) and value == expected[key], 'complete independent endpoint identity required')
        if name != 'READY_REVOKE_WITNESS':
            require(len({fields[key] for key in ('dead_sub_gid', 'live_sub_gid', *OLD_GIDS)}) == 4,
                    'old native endpoints must be distinct')
        if name in ('MATCHED_REVOKE_NEW', 'DELIVERED_REVOKE_NEW', 'PASS_REVOKE_RECONNECT_WITNESS'):
            require(len({fields[key] for key in GIDS}) == 5, 'new survivor publisher must be independently fresh')
        numeric = {key: decimal(value) for key, value in fields.items()
                   if key not in COMMON and not key.endswith('_gid')}
        exact = {
            'MATCHED_REVOKE_OLD': {'dead_publishers': 1, 'live_publishers': 1},
            'BASELINE_REVOKE_WITNESS': {'dead_before': 1, 'live_before': 1},
            'FROZEN_REVOKE_WITNESS': {'dead_total': 1, 'live_before': 1, 'unauthorized_dead': 0},
            'MATCHED_REVOKE_NEW': {'dead_publishers': 0, 'live_publishers': 1, 'dead_total': 1,
                                   'live_before': 1, 'live_after': 0},
            'DELIVERED_REVOKE_NEW': {'dead_publishers': 0, 'live_publishers': 1, 'dead_total': 1,
                                     'live_before': 1, 'live_after': 1},
            'PASS_REVOKE_RECONNECT_WITNESS': {'dead_publishers': 0, 'live_publishers': 1, 'dead_total': 1,
                'live_before': 1, 'live_after': 1, 'unauthorized_dead': 0, 'fault_live': 0, 'unknown': 0},
        }.get(name, {})
        require(all(numeric[key] == wanted for key, wanted in exact.items()), 'exact native counts differ')
        if name == 'PASS_REVOKE_RECONNECT_WITNESS':
            require(numeric['quiet_samples'] >= 30 and numeric['quiet_ns'] >= 2000000000,
                    'independent quiet observation below accepted floor')
            require(all(0 <= numeric[key] <= 1 for key in ('warmup_dead', 'warmup_live', 'warmup_new')),
                    'warmup can be lost or received exactly once')
        rows.append(fields)
    return rows


def oracle_deliveries(events, expected):
    """Count only permitted trace facts in independently supplied gate epochs.

    The epochs are the controller/native boundaries, not a candidate's claimed
    capability state. Baseline facts must be exactly once before N; a dead
    effect in any later epoch fails even if final counters were relabeled zero.
    A fault sample always fails. Actual MessageInfo/native context provenance
    still requires the complete executed controller, outside these models.
    """
    require(isinstance(events, list) and len(events) <= 6, 'bounded native delivery ledger required')
    allowed = {
        'WARMUP-DEAD': ('dead', expected['old_dead_pub_gid'], {'old', 'baseline'}),
        'WARMUP-LIVE': ('live', expected['old_live_pub_gid'], {'old', 'baseline'}),
        'BEFORE-DEAD': ('dead', expected['old_dead_pub_gid'], {'baseline'}),
        'BEFORE-LIVE': ('live', expected['old_live_pub_gid'], {'baseline'}),
        'RECONNECT-WARMUP': ('live', expected['new_live_pub_gid'], {'fresh', 'final'}),
        'AFTER-LIVE': ('live', expected['new_live_pub_gid'], {'final', 'quiet'}),
    }
    counts = {}
    for event in events:
        require(isinstance(event, dict) and set(event) == {'topic', 'payload', 'source', 'epoch', 'intra_process'},
                'complete closed independent delivery record required')
        require(event['intra_process'] is False, 'a native DDS effect cannot be inferred from intra-process routing')
        require(event['payload'] in allowed, 'unknown or fault payload is forbidden')
        topic, source, epochs = allowed[event['payload']]
        require(event['topic'] == expected[topic + '_topic'] and event['source'] == source and gid(event['source'])
                and event['epoch'] in epochs, 'delivery endpoint/origin/epoch differs')
        counts[event['payload']] = counts.get(event['payload'], 0) + 1
    require(all(counts.get(key, 0) == 1 for key in ('BEFORE-DEAD', 'BEFORE-LIVE', 'AFTER-LIVE')),
            'each actual baseline and survivor delivery must occur exactly once')
    require(all(counts.get(key, 0) <= 1 for key in allowed), 'no duplicate native effect or warmup allowed')
    return counts


def oracle_advertisement(entry):
    """Classify node association separately from immutable endpoint facts.

    This relational oracle specifies a public discovery observation. It does
    not evaluate the C++ helper. Complete official placeholders can only be
    unresolved: a valid type/hash/direction/QoS/GID remains mandatory, while
    another literal name or mixed placeholder pair is immediately refused.
    """
    require(isinstance(entry, dict) and set(entry) == set(GRAPH_METADATA) | {'gid'}
            and gid(entry['gid']), 'complete nonzero native endpoint facts required')
    require(all(entry[key] == value for key, value in GRAPH_METADATA.items()
                if key not in ('node', 'namespace')), 'stable endpoint type/hash/direction/QoS differs')
    association = (entry['node'], entry['namespace'])
    require(association == (GRAPH_METADATA['node'], GRAPH_METADATA['namespace'])
            or association in PENDING_ASSOCIATIONS, 'foreign or incomplete node association')
    return association not in PENDING_ASSOCIATIONS


def oracle_initial_graph(dead, live, matched, expected):
    """Require complete exact old associations before the baseline gate.

    The trusted selected native identities stay outside candidate snapshots.
    Missing or pending association never creates a READY result, even when
    native match counts are already one. No elapsed delay qualifies a node.
    """
    require(isinstance(dead, list) and isinstance(live, list) and len(dead) <= 1 and len(live) <= 1,
            'initial topic graphs contain at most one writer each')
    require(isinstance(matched, tuple) and len(matched) == 2
            and all(type(value) is int and 0 <= value <= 2 for value in matched), 'exact initial matched counts')
    associated = []
    identities = []
    for entries, identity in ((dead, expected['old_dead_pub_gid']), (live, expected['old_live_pub_gid'])):
        for entry in entries:
            resolved = oracle_advertisement(entry)
            require(entry['gid'] not in {expected['dead_sub_gid'], expected['live_sub_gid']},
                    'initial publisher cannot reuse either independent subscriber identity')
            require(not resolved or entry['gid'] == identity, 'qualified initial writer differs from independent selection')
            associated.append(resolved)
            identities.append(entry['gid'])
    require(len(set(identities)) == len(identities), 'initial topics cannot share a publisher identity even while pending')
    return 'READY' if len(associated) == 2 and all(associated) and matched == (1, 1) else 'WAIT'


def oracle_replacement_graph(dead, live, matched, expected):
    """Classify synthetic discovery snapshots using independent endpoint facts.

    Discovery can expose the old and new live advertisements concurrently;
    this is a wait state, never a final match or a timing-based retirement
    proof. A complete snapshot admits only the independently pinned old/new
    identities and their public context/type/QoS fields. The selected native
    callback and authenticated-session proofs remain separate requirements.
    This does not invoke or translate the C++ graph scan.
    """
    require(isinstance(dead, list) and isinstance(live, list) and len(dead) <= 1 and len(live) <= 2,
            'bounded old/new graph required')
    require(isinstance(matched, tuple) and len(matched) == 2
            and all(type(value) is int and 0 <= value <= 2 for value in matched),
            'bounded independent native matched counts required')
    complete = True
    for entries, admitted in ((dead, {expected['old_dead_pub_gid']}),
                              (live, {expected['old_live_pub_gid'], expected['new_live_pub_gid']})):
        identities = []
        for entry in entries:
            resolved = oracle_advertisement(entry)
            complete = complete and resolved
            if entries is dead:
                require(entry['gid'] == expected['old_dead_pub_gid'], 'revoked dead publisher cannot be resurrected')
            else:
                require(entry['gid'] not in {expected[key] for key in ('dead_sub_gid', 'live_sub_gid', 'old_dead_pub_gid')},
                        'replacement publisher cannot reuse a witness or dead identity')
                require(not resolved or entry['gid'] in admitted, 'qualified replacement differs from independent selection')
            identities.append(entry['gid'])
        require(len(identities) == len(set(identities)), 'duplicate native advertisements are not a transition')
        if entries is live:
            require(sum(identity != expected['old_live_pub_gid'] for identity in identities) <= 1,
                    'at most one fresh candidate exists even before node association')
    return ('READY' if complete and not dead and {entry['gid'] for entry in live} == {expected['new_live_pub_gid']}
            and matched == (0, 1) else 'WAIT')


def oracle_discovery_sequence(samples, epoch, expected):
    """Select only a complete snapshot strictly inside one sixty-second bound.

    Every timestamp is a synthetic monotonic offset from the same exchange
    origin. Pending observations cannot reset it. Missing completion or a
    ready snapshot at/after the bound is failure, not a timing-based match.
    """
    require(isinstance(samples, list) and samples and epoch in ('old', 'fresh'), 'closed discovery trace required')
    times = [sample['ns'] for sample in samples]
    require(all(type(value) is int and 0 <= value < 60000000000 for value in times)
            and all(a <= b for a, b in zip(times, times[1:])), 'one bounded monotonic exchange origin required')
    classify = oracle_initial_graph if epoch == 'old' else oracle_replacement_graph
    states = [classify(sample['dead'], sample['live'], sample['matched'], expected) for sample in samples]
    require(states[-1] == 'READY' and all(state == 'WAIT' for state in states[:-1]),
            'pending observations do not substitute for one final qualified snapshot')
    return times[-1]


def fixture(expected, warmups=(1, 1, 1)):
    """Generate canonical synthetic marker text and independent delivery facts."""
    tails = (
        {},
        {'dead_publishers': 1, 'live_publishers': 1},
        {'dead_before': 1, 'live_before': 1},
        {'dead_total': 1, 'live_before': 1, 'unauthorized_dead': 0},
        {'dead_publishers': 0, 'live_publishers': 1, 'dead_total': 1, 'live_before': 1, 'live_after': 0},
        {'dead_publishers': 0, 'live_publishers': 1, 'dead_total': 1, 'live_before': 1, 'live_after': 1},
        {'dead_publishers': 0, 'live_publishers': 1, 'dead_total': 1, 'live_before': 1, 'live_after': 1,
         'unauthorized_dead': 0, 'fault_live': 0, 'unknown': 0, 'quiet_samples': 30, 'quiet_ns': 2000000000,
         'warmup_dead': warmups[0], 'warmup_live': warmups[1], 'warmup_new': warmups[2]},
    )
    lines = []
    for (name, keys), tail in zip(MARKERS, tails):
        values = {**expected, **tail}
        lines.append(name + ' ' + ' '.join(key + '=' + str(values[key]) for key in COMMON + keys))
    events = []
    for topic, payload, origin, epoch, admitted in (
        ('dead', 'WARMUP-DEAD', 'old_dead_pub_gid', 'old', warmups[0]),
        ('live', 'WARMUP-LIVE', 'old_live_pub_gid', 'old', warmups[1]),
        ('dead', 'BEFORE-DEAD', 'old_dead_pub_gid', 'baseline', 1),
        ('live', 'BEFORE-LIVE', 'old_live_pub_gid', 'baseline', 1),
        ('live', 'RECONNECT-WARMUP', 'new_live_pub_gid', 'fresh', warmups[2]),
        ('live', 'AFTER-LIVE', 'new_live_pub_gid', 'final', 1),
    ):
        if admitted:
            events.append({'topic': expected[topic + '_topic'], 'payload': payload, 'source': expected[origin],
                           'epoch': epoch, 'intra_process': False})
    return '\n'.join(lines) + '\n', events


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    source = args.source.resolve(strict=True)
    receipt = []

    def check(name, accepted):
        receipt.append({'name': name, 'status': 'PASS' if accepted else 'FAIL'})
        require(accepted, name)

    def deny(name, action):
        try:
            action()
        except (ValueError, KeyError, TypeError, IndexError):
            check(name, True)
        else:
            check(name, False)

    expected = expected_facts()
    for warmups in itertools.product((0, 1), repeat=3):
        text, events = fixture(expected, warmups)
        check('positive-warmup-loss-' + str(warmups), len(oracle_markers(text, expected)) == 7
              and oracle_deliveries(events, expected)['AFTER-LIVE'] == 1)
    text, events = fixture(expected)
    for index in range(len(events)):
        for field, choices in (
            ('topic', (expected['dead_topic'], expected['live_topic'], '/foreign')),
            ('payload', ('FAULT-LIVE', 'AFTER-DEAD', 'BEFORE-DEAD', 'BEFORE-LIVE', 'AFTER-LIVE', 'foreign')),
            ('source', tuple(expected[key] for key in GIDS) + ('0' * 32, 'a' * 48)),
            ('epoch', ('old', 'baseline', 'frozen', 'fresh', 'final', 'quiet')),
            ('intra_process', (True, 0, 1, None)),
        ):
            for replacement in choices:
                sample = copy.deepcopy(events)
                if sample[index][field] == replacement and type(sample[index][field]) == type(replacement):
                    continue
                sample[index][field] = replacement
                # Another permitted warmup epoch remains a valid independent
                # fact; refuse only values outside this separately defined set.
                if field == 'epoch' and (sample[index]['payload'], replacement) in {
                    ('WARMUP-DEAD', 'old'), ('WARMUP-DEAD', 'baseline'),
                    ('WARMUP-LIVE', 'old'), ('WARMUP-LIVE', 'baseline'),
                    ('RECONNECT-WARMUP', 'fresh'), ('RECONNECT-WARMUP', 'final'),
                    ('AFTER-LIVE', 'final'), ('AFTER-LIVE', 'quiet')}:
                    check('alternative-allowed-epoch-' + str(index) + '/' + replacement,
                          bool(oracle_deliveries(sample, expected)))
                else:
                    deny('delivery-mutation-' + str(index) + '/' + field + '/' + str(replacement),
                         lambda sample=sample: oracle_deliveries(sample, expected))
    for index in (2, 3, 5):
        sample = copy.deepcopy(events)
        del sample[index]
        deny('missing-positive-delivery-' + str(index), lambda sample=sample: oracle_deliveries(sample, expected))
    for event in events:
        sample = events + [copy.deepcopy(event)]
        deny('duplicate-native-effect-' + event['payload'], lambda sample=sample: oracle_deliveries(sample, expected))
    for epoch in ('frozen', 'fresh', 'final', 'quiet'):
        sample = copy.deepcopy(events)
        sample[2]['epoch'] = epoch
        deny('post-revoke-dead-effect-' + epoch, lambda sample=sample: oracle_deliveries(sample, expected))
    for key in COMMON + OLD_GIDS + ('new_live_pub_gid',):
        value = expected[key]
        for replacement in ('foreign', '', value + 'x'):
            mutated = text.replace(key + '=' + value, key + '=' + replacement)
            deny('coherent-foreign-marker-fact-' + key + '/' + replacement,
                 lambda mutated=mutated: oracle_markers(mutated, expected))
    for width in range(33):
        if width == 16:
            continue
        mutated = text
        for index, key in enumerate(GIDS):
            mutated = mutated.replace(expected[key], bytes([0xa1 + index] * width).hex())
        deny('coherent-native-GID-width-' + str(width), lambda mutated=mutated: oracle_markers(mutated, expected))
    for key in GIDS:
        for replacement in ('0' * 32, 'A' * 32, expected['dead_sub_gid']):
            if replacement == expected[key]:
                continue
            mutated = text.replace(key + '=' + expected[key], key + '=' + replacement)
            deny('native-zero-uppercase-or-alias-' + key + '/' + replacement,
                 lambda mutated=mutated: oracle_markers(mutated, expected))
    for old, new in (('quiet_samples=30', 'quiet_samples=29'), ('quiet_ns=2000000000', 'quiet_ns=1999999999'),
                     ('live_after=1', 'live_after=0'), ('dead_total=1', 'dead_total=0'),
                     ('unauthorized_dead=0', 'unauthorized_dead=1'), ('fault_live=0', 'fault_live=1'),
                     ('unknown=0', 'unknown=1'), ('warmup_new=1', 'warmup_new=2'),
                     ('dead_publishers=0', 'dead_publishers=1'), ('quiet_samples=30', 'quiet_samples=030'),
                     ('quiet_ns=2000000000', 'quiet_ns=-1'), ('quiet_samples=30', 'quiet_samples=30.0')):
        mutated = text.replace(old, new)
        deny('native-count-and-canonical-refusal-' + new, lambda mutated=mutated: oracle_markers(mutated, expected))
    for mutated in (text.rstrip('\n'), text.replace('\n', '\r\n'), text + text.splitlines()[0] + '\n',
                    '\n'.join(reversed(text.splitlines())) + '\n', 'PASS_SECURE_WITNESS\n',
                    text.replace(' pid=', ' foreign=x pid=', 1), text.replace(' pid=', '  pid=', 1),
                    text.replace(' domain=75', ' domain=75 domain=75', 1)):
        deny('native-ledger-shape-' + str(len(receipt)), lambda mutated=mutated: oracle_markers(mutated, expected))

    # Hand-authored public endpoint facts distinguish discovery snapshots from
    # native matched counts. No fixed elapsed time manufactures disappearance.
    def advertisement(identity):
        return {'node': 'uwvm_ros2_gateway_probe', 'namespace': '/uwvm_revoke_test',
                'type': 'std_msgs/msg/String', 'direction': 'publisher', 'gid': identity,
                'hash': 'RIHS01_df668c740482bbd48fb39d76a70dfd4bd59db1288021743503259e948f6b1a18',
                'reliability': 'reliable', 'durability': 'volatile'}

    old_dead = advertisement(expected['old_dead_pub_gid'])
    old_live = advertisement(expected['old_live_pub_gid'])
    new_live = advertisement(expected['new_live_pub_gid'])
    for index, (dead, live, matched) in enumerate((
            ([], [], (0, 0)), ([], [old_live], (0, 1)),
            ([], [old_live, new_live], (0, 2)), ([], [new_live, old_live], (0, 1)),
            ([old_dead], [new_live], (1, 1)), ([], [new_live], (0, 0)),
            ([], [new_live], (0, 2)), ([], [new_live], (1, 1)))):
        check('bounded-retirement-wait-' + str(index),
              oracle_replacement_graph(dead, live, matched, expected) == 'WAIT')
    check('only-retired-old-and-exact-one-fresh-match-admitted',
          oracle_replacement_graph([], [new_live], (0, 1), expected) == 'READY')
    for entries in ([old_live, old_live], [new_live, new_live], [old_live, new_live, old_live],
                    [new_live, advertisement('d1' * 16)], [advertisement(expected['live_sub_gid'])]):
        deny('duplicate-third-or-two-fresh-writers-' + str(len(receipt)),
             lambda entries=entries: oracle_replacement_graph([], entries, (0, 1), expected))
    deny('new-dead-publisher-is-resurrected-authority',
         lambda: oracle_replacement_graph([new_live], [new_live], (0, 1), expected))
    for key in ('node', 'namespace', 'type', 'direction', 'hash', 'reliability', 'durability', 'gid'):
        altered = {**new_live, key: 'foreign'}
        deny('replacement-public-endpoint-mutation-' + key,
             lambda altered=altered: oracle_replacement_graph([], [old_live, altered], (0, 2), expected))
    for counts in ((False, 1), (0, True), (0, 3), (0, -1), [0, 1]):
        deny('replacement-native-match-count-refusal-' + repr(counts),
             lambda counts=counts: oracle_replacement_graph([], [new_live], counts, expected))

    # Node-association records are deliberately independent of stable endpoint
    # facts. Neither an actual match count nor a known GID can turn one of the
    # complete official placeholder pairs into an accepted publisher.
    for pair_index, pair in enumerate(PENDING_ASSOCIATIONS):
        pending_dead = {**old_dead, 'node': pair[0], 'namespace': pair[1]}
        pending_old = {**old_live, 'node': pair[0], 'namespace': pair[1]}
        pending_new = {**new_live, 'node': pair[0], 'namespace': pair[1]}
        for dead_rows, live_rows in (([pending_dead], [old_live]), ([old_dead], [pending_old]),
                                     ([pending_dead], [pending_old])):
            check('initial-complete-placeholder-pair-stays-unselected-' + str(len(receipt)),
                  oracle_initial_graph(dead_rows, live_rows, (1, 1), expected) == 'WAIT')
        for dead_rows, live_rows in (([pending_dead], [new_live]), ([], [pending_old, new_live]),
                                     ([], [old_live, pending_new]), ([], [pending_new]),
                                     ([pending_dead], [pending_old, pending_new])):
            check('fresh-complete-placeholder-pair-stays-unselected-' + str(len(receipt)),
                  oracle_replacement_graph(dead_rows, live_rows, (0, 1), expected) == 'WAIT')
        for epoch, initial in (('old', {'dead': [pending_dead], 'live': [pending_old], 'matched': (1, 1)}),
                               ('fresh', {'dead': [], 'live': [pending_new], 'matched': (0, 1)})):
            final = {'dead': [old_dead] if epoch == 'old' else [],
                     'live': [old_live] if epoch == 'old' else [new_live],
                     'matched': (1, 1) if epoch == 'old' else (0, 1)}
            trace = [{'ns': 2000000, **initial}, {'ns': 59000000000, **final}]
            check('official-placeholder-resolves-inside-original-bound-' + epoch + '-' + str(pair_index),
                  oracle_discovery_sequence(trace, epoch, expected) == 59000000000)
            for end_ns in (59999999999, 60000000000, 60000000001):
                stays_pending = [{'ns': 2000000, **initial}, {'ns': end_ns, **initial}]
                deny('persistent-placeholder-cannot-claim-completion-' + epoch + '-' + str(pair_index) + '-' + str(end_ns),
                     lambda trace=stays_pending, epoch=epoch: oracle_discovery_sequence(trace, epoch, expected))
            for bad_time in (-1, False, 60000000000, 60000000001):
                too_late = [{'ns': 2000000, **initial}, {'ns': bad_time, **final}]
                deny('placeholder-does-not-reset-monotonic-deadline-' + epoch + '-' + str(pair_index) + '-' + repr(bad_time),
                     lambda trace=too_late, epoch=epoch: oracle_discovery_sequence(trace, epoch, expected))
        mutations = {'type': ('foreign',), 'direction': ('subscription',), 'hash': ('RIHS01_' + '0' * 64,),
                     'reliability': ('best_effort',), 'durability': ('transient_local',),
                     'gid': ('0' * 32, 'a' * 48, 'A' * 32),
                     'node': ('', 'foreign', '_NODE_NAME_UNKNOWN_' if pair_index else '_CREATED_BY_BARE_DDS_APP_'),
                     'namespace': ('', '/foreign', '_NODE_NAMESPACE_UNKNOWN_' if pair_index else '_CREATED_BY_BARE_DDS_APP_')}
        for key, values in mutations.items():
            for value in values:
                altered = {**pending_new, key: value}
                deny('placeholder-stable-or-paired-field-refusal-' + str(pair_index) + '-' + key + '-' + value,
                     lambda altered=altered: oracle_replacement_graph([], [altered], (0, 1), expected))
                initial_bad = {**pending_dead, key: value}
                deny('initial-placeholder-stable-or-paired-field-refusal-' + str(pair_index) + '-' + key + '-' + value,
                     lambda altered=initial_bad: oracle_initial_graph([altered], [old_live], (1, 1), expected))
        for key in pending_new:
            altered = {k: v for k, v in pending_new.items() if k != key}
            deny('placeholder-missing-independent-field-' + str(pair_index) + '-' + key,
                 lambda altered=altered: oracle_replacement_graph([], [altered], (0, 1), expected))
        foreign = advertisement('d1' * 16)
        foreign_pending = {**foreign, 'node': pair[0], 'namespace': pair[1]}
        for live_rows in ([pending_old, pending_old], [pending_new, pending_new],
                          [pending_old, pending_new, pending_old], [pending_new, foreign_pending],
                          [pending_old, {**new_live, 'node': 'foreign'}],
                          [pending_new, advertisement(expected['live_sub_gid'])]):
            deny('unresolved-row-cannot-hide-duplicate-third-foreign-live-' + str(len(receipt)),
                 lambda rows=live_rows: oracle_replacement_graph([pending_dead], rows, (1, 1), expected))
        deny('unresolved-foreign-dead-GID-never-recreates-revoked-publisher-' + str(pair_index),
             lambda: oracle_replacement_graph([pending_new], [pending_new], (0, 1), expected))
        deny('initial-placeholder-cannot-hide-second-named-writer-' + str(pair_index),
             lambda: oracle_initial_graph([pending_dead], [old_live, foreign], (1, 1), expected))
        deny('initial-placeholder-cannot-hide-cross-topic-duplicate-GID-' + str(pair_index),
             lambda: oracle_initial_graph([pending_dead], [{**pending_old, 'gid': pending_dead['gid']}], (1, 1), expected))
        for subscriber in ('dead_sub_gid', 'live_sub_gid'):
            deny('initial-placeholder-cannot-alias-independent-subscriber-' + str(pair_index) + '-' + subscriber,
                 lambda subscriber=subscriber: oracle_initial_graph([{**pending_dead, 'gid': expected[subscriber]}],
                                                                    [pending_old], (1, 1), expected))
        for early_payload, epoch in (('BEFORE-DEAD', 'old'), ('BEFORE-LIVE', 'old'),
                                     ('AFTER-LIVE', 'fresh'), ('FAULT-LIVE', 'fresh')):
            delivery = {'topic': expected['dead_topic'] if early_payload == 'BEFORE-DEAD' else expected['live_topic'],
                        'payload': early_payload, 'source': expected['old_dead_pub_gid'] if early_payload == 'BEFORE-DEAD'
                        else expected['old_live_pub_gid'] if early_payload == 'BEFORE-LIVE' else expected['new_live_pub_gid'],
                        'epoch': epoch}
            deny('pending-association-never-drops-or-admits-early-application-effect-' + str(pair_index) + '-' + early_payload,
                 lambda delivery=delivery: oracle_deliveries(events + [delivery], expected))
        for payload in ('WARMUP-DEAD', 'WARMUP-LIVE', 'RECONNECT-WARMUP'):
            retained = copy.deepcopy(events)
            found = next(entry for entry in retained if entry['payload'] == payload)
            found['source'] = foreign_pending['gid']
            deny('prematch-warmup-origin-must-reconcile-with-final-qualified-GID-' + str(pair_index) + '-' + payload,
                 lambda retained=retained: oracle_deliveries(retained, expected))

    # The earlier endpoint predicate grouped eight independent Boolean fields
    # into four conjunctions. Splitting those checks adds only refusal labels;
    # all 256 truth assignments must preserve exactly the same acceptance.
    for fields in itertools.product((False, True), repeat=8):
        before = all((all(fields[:3]), fields[3], all(fields[4:6]), all(fields[6:])))
        after = all(fields)
        check('endpoint-Boolean-conjunction-unchanged-' + ''.join('1' if value else '0' for value in fields),
              before == after)
    for site, fields in DIAGNOSTIC_FIELDS.items():
        for field in fields:
            line = 'FAIL_REVOKE_RECONNECT_WITNESS phase=fresh-discovery site=' + site + ' field=' + field + '\n'
            check('fixed-diagnostic-refusal-only-' + site + '/' + field,
                  oracle_failure(line)['lifecycle_accepted'] is False)
            deny('diagnostic-never-substitutes-success-' + site + '/' + field,
                 lambda line=line: oracle_markers(line, expected))
    canonical = 'FAIL_REVOKE_RECONNECT_WITNESS phase=fresh-discovery site=fresh_old_live field=name_unknown\n'
    for mutated in (canonical.rstrip('\n'), canonical.replace('\n', '\r\n'), canonical + canonical,
                    canonical.replace(' site=', ' field=shape site='),
                    canonical.replace('phase=fresh-discovery', 'phase=foreign'),
                    canonical.replace('site=fresh_old_live', 'site=discovered_peer_name'),
                    canonical.replace('field=name_unknown', 'field=/tmp/private/root'),
                    canonical.replace('field=name_unknown', 'field=raw_payload'),
                    canonical.replace('site=fresh_old_live', 'site=callback_origin'),
                    canonical.replace(' site=', '  site=')):
        deny('diagnostic-shape-or-private-body-refusal-' + str(len(receipt)),
             lambda mutated=mutated: oracle_failure(mutated))

    native = (source / SOURCE_NAMES[0]).read_text()
    docs = (source / SOURCE_NAMES[1]).read_text()
    check('all-fixed-native-source-sites-in-independent-diagnostic-catalog',
          source_diagnostic_catalog(native) == {site: set(fields) for site, fields in DIAGNOSTIC_FIELDS.items()})
    check('all-diagnostic-phases-retained-from-native-source',
          set(re.findall(r'failure_phase(?:\{[^\"]*)?\s*(?:=\s*)?"([a-z-]+)"', native)) == set(DIAGNOSTIC_PHASES))
    check('exact-seven-native-marker-literals', all(native.count('marker("' + name + '"') == 1 for name, _ in MARKERS))
    check('strict16-byte-native-storage', 'static_assert(RMW_GID_STORAGE_SIZE == 16,' in native
          and 'using gid = std::array<std::uint8_t, RMW_GID_STORAGE_SIZE>;' in native)
    check('whole-native-GID-copy', 'std::copy_n(native.data, result.size(), result.begin());' in native)
    check('independent32-byte-native-typehash', 'std::array<std::uint8_t, 32> bytes{};' in native
          and 'std::copy_n(hash.value, bytes.size(), bytes.begin());' in native)
    check('public-context-actual-domain-and-Enforce', all(term in native for term in
          ('rcl_context_get_rmw_context', 'native->actual_domain_id == 75', 'RMW_SECURITY_ENFORCEMENT_ENFORCE',
           'native->options.security_options.security_root_path', 'rmw_get_implementation_identifier')))
    check('two-subscriptions-no-native-publisher-or-IPC', native.count('node->create_subscription<message>') == 2
          and 'create_publisher<' not in native and 'ipc::client' not in native and 'fork(' not in native and 'exec(' not in native)
    check('actual-local-full-QoS-and-honest-remote-fields', 'actual_qos(dead->get_actual_qos(), qos)' in native
          and 'profile.depth = 8;' in native and 'a.avoid_ros_namespace_conventions == b.avoid_ros_namespace_conventions' in native
          and 'Remote DDS discovery does not supply reliable history/depth evidence.' in native)
    check('one-native-thread-rejects-intraprocess', '!native.from_intra_process' in native
          and 'SingleThreadedExecutor' in native and 'std::thread{' not in native)
    check('one-byte-exact-private-native-gates', all(native.count("gate('" + gate + "',") == 1 for gate in ('I', 'N', 'F'))
          and 'fast_io::operations::read_some' in native and 'value == expected' in native)
    check('single60s-exchange-and-actual2s-30-sample-quiet', 'std::chrono::seconds{60}' in native
          and 'quiet_begin + std::chrono::seconds{2}' in native and 'quiet_samples >= 30 && quiet_ns >= 2000000000ULL' in native)
    check('native-context-retired-before-final-marker', native.index('context.close();')
          < native.index('marker("PASS_REVOKE_RECONNECT_WITNESS"'))
    check('fastio-hex-print-and-no-payload-logging', 'fast_io::ostring_ref_fast_io' in native
          and 'fast_io::mnp::hex' in native and 'std::cout' not in native and 'printf(' not in native
          and 'fast_io::println(fast_io::err(), "FAIL_REVOKE_RECONNECT_WITNESS phase=", failure_phase,' in native
          and '" site=", refusal_site, " field=", refusal_field);' in native)
    check('delivery-observed-before-Guest-close-permission',
          native.index('while(observed.live_after != 1)') < native.index('marker("DELIVERED_REVOKE_NEW"')
          < native.index("while(!gate('F', 0))") and 'AFTER_MESSAGE_SENT' in docs and '`Q` gate' in docs)
    fresh = native[native.index('failure_phase = "fresh-discovery";'):native.index('observed.new_live = facts.new_live;')]
    check('fresh-only-known-old-plus-one-qualified-candidate-wait',
          'require_at(live_writers.size() <= 2,' in fresh
          and 'endpoint(writer, gateway_name, true, site)' in fresh
          and 'require_at(!old_live_present,' in fresh and 'require_at(!fresh_live_present,' in fresh
          and 'if(old_live_present || !fresh_live_present) { continue; }' in fresh
          and 'live->get_publisher_count() != 1' in fresh
          and native.count('require_at(entries.size() <= 1,') == 2
          and 'Final admission still' in fresh and 'temporarily coexist' in docs)
    check('fixed-public-diagnostic-does-not-admit-unknown-association',
          'require_at(value.node_name() == name, site,' in native
          and 'require_at(value.node_namespace() == node_namespace, site,' in native
          and 'if(!accepted)' in native and 'throw std::runtime_error("revoke/reconnect witness contract failed")' in native
          and 'diagnostic label only' in docs and 'No unknown-node wait' in docs)
    pending = native[native.index('bool pending_publisher('):native.index('bool publisher(')]
    check('pending-only-two-complete-official-association-pairs',
          '(value.node_name() == "_NODE_NAME_UNKNOWN_" &&' in pending
          and 'value.node_namespace() == "_NODE_NAMESPACE_UNKNOWN_")' in pending
          and '(value.node_name() == "_CREATED_BY_BARE_DDS_APP_" &&' in pending
          and 'value.node_namespace() == "_CREATED_BY_BARE_DDS_APP_")' in pending
          and 'if(!pending) { return false; }' in pending)
    check('every-pending-stable-field-remains-validated', all(term in pending for term in
          ('value.topic_type() == type_name', 'value.endpoint_type() == rclcpp::EndpointType::Publisher',
           'exact_hash(value.topic_type_hash())', 'nonzero(value.endpoint_gid())',
           'qos.reliability == RMW_QOS_POLICY_RELIABILITY_RELIABLE', 'qos.durability == RMW_QOS_POLICY_DURABILITY_VOLATILE')))
    check('raw-fresh-shape-and-GID-classification-precedes-association-wait',
          fresh.index('require_at(writer.endpoint_gid() == facts.old_dead,') < fresh.index('if(!pending_publisher(writer, site))')
          and fresh.index('require_at(!old_live_present,') < fresh.index('if(pending_publisher(writer, site))')
          and fresh.index('require_at(!fresh_live_present,') < fresh.index('if(pending_publisher(writer, site))')
          and fresh.index('"independent_gid");') < fresh.index('if(pending_publisher(writer, site))')
          and 'if(dead_writer_present || association_pending) { continue; }' in fresh)
    check('old-pending-not-admitted-and-local-subscriptions-stay-strict',
          'if(pending_publisher(entries.front(), "old_publisher"))' in native
          and 'output = endpoint(entries.front(), gateway_name, true, "old_publisher");' in native
          and 'output = endpoint(entries.front(), node_name, false, "own_subscription");' in native)
    old = native[native.index('failure_phase = "old-discovery";'):native.index('observed.old_dead = facts.old_dead;')]
    check('old-raw-cross-topic-and-subscriber-GID-checks-before-retry',
          'gid raw_dead{}, raw_live{};' in old
          and 'facts.dead_sub, facts.live_sub, raw_dead' in old and 'facts.dead_sub, facts.live_sub, raw_live' in old
          and 'require(raw_dead != raw_live);' in old
          and old.index('require(raw_dead != raw_live);') < old.index('if(dead_found && live_found')
          and old.index('if(dead_found && live_found') < old.index('facts.old_dead = raw_dead;'))
    check('docs-64GiB-pending-real-acceptance-and-new-snapshot-scope', 'memory.max=68719476736' in docs
          and 'does not imply' in docs and 'at final witness completion' in docs)
    pins = {}
    for relative in SOURCE_NAMES:
        path = source / relative
        raw = path.read_bytes()
        pins[relative] = {'path': str(path), 'bytes': len(raw), 'sha256': hashlib.sha256(raw).hexdigest()}
    report = {'format_version': 1, 'status': 'PASS_SOURCE_AND_SYNTHETIC_NATIVE_MODELS_ONLY',
              'scope': 'Independent native delivery/marker oracles and prepared source controls; no actual execution or cryptography.',
              'synthetic_expected_facts': expected, 'source_pins': pins, 'checks': receipt, 'check_count': len(receipt),
              'actual_compilation': 'NOT_EXECUTED', 'actual_native_Guest_behavior': 'NOT_EXECUTED',
              'runtime_tools_executed': [], 'full_G09_complete': False,
              'diagnostic_phase_catalog': list(DIAGNOSTIC_PHASES),
              'diagnostic_site_field_catalog': {key: list(values) for key, values in DIAGNOSTIC_FIELDS.items()},
              'diagnostic_acceptance_changed': False,
              'discovery_wait_revision': {'complete_official_placeholder_pairs': list(PENDING_ASSOCIATIONS),
                  'final_endpoint_acceptance_changed': False, 'pending_association_is_never_success': True,
                  'shared_exchange_deadline_ns': 60000000000}}
    with args.out.open('x') as stream:
        stream.write(json.dumps(report, indent=2) + '\n')
    print(json.dumps({'status': report['status'], 'checks': len(receipt), 'path': str(args.out),
                      'sha256': hashlib.sha256(args.out.read_bytes()).hexdigest()}, indent=2))


if __name__ == '__main__':
    main()
