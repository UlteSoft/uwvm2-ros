"""Explicit v6 typed ParameterEvent profiles and exact expected canonical bytes."""
import struct

KINDS = ('bool', 'int64', 'float64')
NODE = '/uwvm_resources/native'
NAME = 'value'
TAGS = {'bool': 1, 'int64': 2, 'float64': 3}
BOUNDS = {'int64': ('-9223372036854775808', '9223372036854775807'),
          'float64': ('-2', '2')}
MARKERS = {'bool': b'\x01', 'int64': struct.pack('<q', 9007199254740993),
           'float64': struct.pack('<Q', 0x3ff0000000000001)}


def envelope(kind, event, stamp, payload, node=NODE, name=NAME):
    def field(data):
        return struct.pack('<I', len(data)) + data
    return (struct.pack('<IIBBB', 123, stamp, event, 2, TAGS[kind]) +
            field(node.encode()) + field(name.encode()) + field(payload))


def expected_events(kind, scenario):
    if scenario != 'roundtrip':
        return [envelope(kind, 2, 9, MARKERS[kind])]
    if kind == 'bool':
        values = [(1, b'\x00'), (2, b'\x01'), (3, b'')]
    elif kind == 'int64':
        values = [(1, struct.pack('<q', -(1 << 63))), (2, MARKERS[kind]),
                  (2, struct.pack('<q', (1 << 63)-1)), (3, b'')]
    else:
        values = [(1, struct.pack('<Q', 0x8000000000000000)),
                  (2, MARKERS[kind]), (3, b'')]
    return [envelope(kind, event, index+1, value)
            for index, (event, value) in enumerate(values)]


def cases():
    rows = []
    for kind in KINDS:
        scenarios = ['roundtrip', 'wrong-kind', 'wrong-node', 'wrong-name',
                     'not-set-delete', 'malformed-stamp', 'fields', 'bytes']
        if kind != 'bool':
            scenarios.append('bounds')
        if kind == 'float64':
            scenarios.append('nonfinite')
        for scenario in scenarios:
            rows.append({'kind': kind, 'scenario': scenario,
                         'name': f'ros2-typed_parameter_event_{kind}_{scenario.replace("-", "_")}'})
    for scenario in ('permission-denied', 'no-capability'):
        rows.append({'kind': 'bool', 'scenario': scenario,
                     'name': 'ros2-typed_parameter_event_'+scenario.replace('-', '_')})
    return rows


def grant(kind, scenario='roundtrip', alias='cap'):
    name = 'quota_value' if scenario == 'bytes' and alias == 'cap' else NAME
    limits = {'audit': 'allow-and-deny', 'audit-max-rate-hz': '100', 'audit-burst': '20',
              'audit-max-records': '10000', 'audit-max-bytes': '1048576',
              'timeout-ms': '2000', 'max-inflight': '2', 'max-rate-hz': '100',
              'burst': '8', 'max-bytes-per-second': '1048576',
              'max-parameter-bytes': '1' if scenario == 'bytes' and alias == 'cap' else '4096',
              'validator': 'finite' if kind == 'float64' else 'none',
              'parameter-node': NODE, 'parameter-name': name, 'max-subscribers': '1',
              'max-event-fields': '1' if scenario == 'fields' else '8',
              'max-event-queue-depth': '8', 'max-event-queue-bytes': '32768',
              'overflow-policy': 'reject', 'qos.reliability': 'reliable',
              'qos.durability': 'volatile', 'qos.history': 'keep-last', 'qos.depth': '32',
              'qos.deadline': 'infinite', 'qos.lifespan': 'infinite',
              'qos.liveliness': 'automatic', 'qos.liveliness-lease-duration': 'infinite'}
    if kind in BOUNDS:
        low, high = BOUNDS[kind]
        if kind == 'int64' and scenario == 'bounds':
            low, high = '-9007199254740995', '9007199254740995'
        limits.update({'parameter-min-value': low, 'parameter-max-value': high})
    return {'alias': alias, 'operation': 'param.subscribe-events',
            'resource': NODE+'/'+name, 'type': 'builtin/param-event-'+kind+'-v1',
            'identity': 'builtin:param-event-'+kind+'-v1', 'limits': limits}


def grants(description, scalar_get_grant):
    kind, scenario = description['kind'], description['scenario']
    if scenario == 'no-capability':
        return []
    if scenario == 'permission-denied':
        return [scalar_get_grant(2, 'param.get', 'bool')]
    result = [grant(kind, scenario)]
    if scenario == 'bytes':
        # Different exact resources avoid duplicate alias grants for one contract.
        result.append(grant(kind, scenario, 'marker'))
    return result
