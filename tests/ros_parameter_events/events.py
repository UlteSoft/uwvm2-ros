"""Independent expanded ROS2 ParameterEvent wire fixtures; no old fixture writes."""
import struct

KINDS = ('string', 'byte-array', 'bool-array', 'int64-array', 'float64-array', 'string-array')
NODE = '/typed_events/native'
NAME = 'value'
TAGS = {kind: index + 4 for index, kind in enumerate(KINDS)}


def field(value):
    return struct.pack('<I', len(value)) + value


def payload(kind, phase=1):
    if not phase:
        return struct.pack('<I', 0)
    if kind == 'string':
        return field('中'.encode() + b'value')
    if kind == 'byte-array':
        return field(bytes((0, 127, 128, 255)))
    if kind == 'bool-array':
        return field(bytes((0, 1, 0)))
    if kind == 'int64-array':
        values = [9007199254740993] if phase == 2 else [-(1 << 63), 9007199254740993, (1 << 63) - 1]
        return struct.pack('<I', len(values)) + b''.join(struct.pack('<q', value) for value in values)
    if kind == 'float64-array':
        return struct.pack('<IQQQ', 3, 0x8000000000000000, 0x3ff0000000000001, 0x0010000000000000)
    if kind == 'string-array':
        return struct.pack('<I', 3) + b''.join(field(value) for value in (b'', '中'.encode(), b'tail'))
    raise ValueError(kind)


def envelope(kind, event, stamp, value):
    return struct.pack('<IIBBB', 123, stamp, event, 2, TAGS[kind]) + field(NODE.encode()) + field(NAME.encode()) + field(value)


def expected_events(kind, scenario):
    if scenario != 'roundtrip':
        return [envelope(kind, 2, 9, payload(kind, 2))]
    return [envelope(kind, event, phase * 3 + event, b'' if event == 3 else payload(kind, phase))
            for phase in (0, 1) for event in (1, 2, 3)]


def cases():
    result = []
    for kind in KINDS:
        scenarios = ['roundtrip', 'wrong-kind', 'wrong-node', 'wrong-name', 'not-set-delete', 'stamp', 'fields', 'bytes']
        if kind != 'string':
            scenarios.append('elements')
        if kind in ('string', 'string-array'):
            scenarios.append('utf8')
        if kind in ('int64-array', 'float64-array'):
            scenarios.append('bounds')
        if kind == 'float64-array':
            scenarios.append('nonfinite')
        scenarios += ['permission-denied', 'no-capability']
        result += [{'kind': kind, 'scenario': scenario,
                    'name': 'expanded_parameter_event_' + kind.replace('-', '_') + '_' + scenario.replace('-', '_')}
                   for scenario in scenarios]
    return result


def grant(kind, scenario='roundtrip', alias='cap'):
    name = 'quota_value' if scenario == 'bytes' and alias == 'cap' else NAME
    limits = {'audit': 'allow-and-deny', 'audit-max-rate-hz': '100', 'audit-burst': '20',
              'audit-max-records': '10000', 'audit-max-bytes': '1048576', 'timeout-ms': '2000',
              'max-inflight': '2', 'max-rate-hz': '100', 'burst': '8', 'max-bytes-per-second': '1048576',
              'max-parameter-bytes': '1' if scenario == 'bytes' and alias == 'cap' else '4096',
              'validator': 'finite' if kind == 'float64-array' else 'none', 'parameter-node': NODE,
              'parameter-name': name, 'max-subscribers': '1', 'max-event-fields': '1' if scenario == 'fields' else '8',
              'max-event-queue-depth': '8', 'max-event-queue-bytes': '32768', 'overflow-policy': 'reject',
              'qos.reliability': 'reliable', 'qos.durability': 'volatile', 'qos.history': 'keep-last', 'qos.depth': '32',
              'qos.deadline': 'infinite', 'qos.lifespan': 'infinite', 'qos.liveliness': 'automatic',
              'qos.liveliness-lease-duration': 'infinite'}
    if kind != 'string':
        limits['max-parameter-elements'] = '4'
    if kind == 'int64-array':
        low, high = ('-9007199254740995', '9007199254740995') if scenario == 'bounds' else ('-9223372036854775808', '9223372036854775807')
        limits.update({'parameter-min-value': low, 'parameter-max-value': high})
    if kind == 'float64-array':
        limits.update({'parameter-min-value': '-2', 'parameter-max-value': '2'})
    return {'alias': alias, 'operation': 'param.subscribe-events', 'resource': NODE + '/' + name,
            'type': 'builtin/param-event-' + kind + '-v1', 'identity': 'builtin:param-event-' + kind + '-v1', 'limits': limits}


def grants(description, scalar_get_grant):
    kind, scenario = description['kind'], description['scenario']
    if scenario == 'no-capability':
        return []
    if scenario == 'permission-denied':
        return [scalar_get_grant(2, 'param.get', 'bool')]
    result = [grant(kind, scenario)]
    if scenario == 'bytes':
        result.append(grant(kind, scenario, 'marker'))
    return result
