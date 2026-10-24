"""Explicit typed array fixtures; all limits are grants, never permission defaults."""
import math
import struct

KINDS = {1: ('bool', 'int32', 'float64', 'string'),
         2: ('bool', 'int64', 'float64', 'string', 'byte')}
VALUES = {
    'bool': [False, True],
    'int32': [-(1 << 31), -7, (1 << 31)-1],
    'int64': [-(1 << 63), 9007199254740993, (1 << 63)-1],
    'float64': [-0.0, math.nextafter(1.0, 2.0), 1.25],
    'string': ['', 'hello<&>', '你好\nline'],
    'byte': [0, 255],
}
BOUNDS = {'int32': ('-2147483648', '2147483647'),
          'int64': ('-9223372036854775808', '9223372036854775807'),
          'float64': ('-2', '2')}
NARROW = {'int32': ('-200000000', '200000000'),
          'int64': ('-9007199254740995', '9007199254740995'),
          'float64': ('-2', '2')}
BASE_SCENARIOS = ('roundtrip', 'empty_roundtrip', 'malformed', 'count_quota',
                  'byte_quota', 'wrong_kind', 'get_count_quota', 'get_byte_quota',
                  'operation_denied')


def encode(kind, values):
    output = struct.pack('<I', len(values))
    if kind == 'string':
        for value in values:
            data = value.encode('utf-8')
            output += struct.pack('<I', len(data)) + data
        return output
    return output + b''.join(struct.pack({'bool': '<B', 'byte': '<B', 'int32': '<i',
                                         'int64': '<q', 'float64': '<d'}[kind], value)
                             for value in values)


def four(kind):
    return ['x']*4 if kind == 'string' else [False]*4 if kind == 'bool' else [0]*4


def malformed(kind, version):
    wire = encode(kind, VALUES[kind])
    rows = [(wire[:-1], 22, 'truncated-element'), (wire+b'\0', 22, 'trailing-byte'),
            (b'\xff\xff\xff\xff', 22, 'impossible-count')]
    if kind == 'bool':
        rows.append((struct.pack('<I', 1)+b'\2', 22, 'noncanonical-bool'))
    if kind == 'float64':
        rows += [(struct.pack('<IQ', 1, bits), 22, label) for bits, label in
                 ((0x7ff8000000000001, 'nan'), (0x7ff0000000000000, 'positive-infinity'),
                  (0xfff0000000000000, 'negative-infinity'))]
    if kind == 'string':
        texts = [(b'\0', 'nul'), (b'\xc0\x80', 'overlong-utf8'),
                 (b'\xed\xa0\x80', 'utf8-surrogate'), (b'\xe4\xbd', 'truncated-utf8')]
        if version == 1:
            texts += [(b'\1', 'xml-control'), (b'\xef\xbf\xbe', 'xml-fffe'),
                      (b'\xef\xbf\xbf', 'xml-ffff')]
        rows += [(struct.pack('<II', 1, len(data))+data, 22, label) for data, label in texts]
    return rows


def outside(kind):
    low, high = NARROW[kind]
    convert = float if kind == 'float64' else int
    return [(encode(kind, [convert(low)-1]), 13, 'below-minimum'),
            (encode(kind, [convert(high)+1]), 13, 'above-maximum')]


def cases(version):
    result = []
    for kind in KINDS[version]:
        scenarios = list(BASE_SCENARIOS)
        if version == 1:
            scenarios += ['create', 'get_mixed_native']
        else:
            scenarios += ['dynamic_denied', 'absent_denied', 'local_roundtrip',
                          'undeclare_wrong_kind']
        if kind in BOUNDS:
            scenarios += ['range', 'get_bounds']
            if version == 2:
                scenarios += ['undeclare_bounds']
        if kind == 'float64':
            scenarios += ['get_nonfinite']
        result += [{'kind': kind, 'scenario': scenario,
                    'name': f'ros{version}-parameter_array_{kind}_{scenario}'}
                   for scenario in scenarios]
    return result


def grant(version, operation, kind, alias, base_grant, local=False):
    cap = base_grant(version, operation, alias)
    cap.update(type=f'builtin/{kind}-array-v1', identity=f'builtin:param-{kind}-array-v1')
    cap['limits'].update({'max-parameter-bytes': '4096', 'max-parameter-elements': '8',
                          'validator': 'finite' if kind == 'float64' else 'none'})
    if kind in BOUNDS:
        cap['limits'].update(dict(zip(('parameter-min-value', 'parameter-max-value'), BOUNDS[kind])))
    if local:
        node = '/uwvm_ros_test/uwvm_ros2_wasm_network'
        cap['resource'] = node+'/value'
        cap['limits']['parameter-node'] = node
    return cap


def grants(version, kind, scenario, base_grant):
    def cap(operation, alias, selected=kind, local=False):
        return grant(version, operation, selected, alias, base_grant, local)
    local = scenario.startswith(('local_', 'undeclare_'))
    if local:
        result = [cap('param.declare', 'declare', local=True), cap('param.get', 'get', local=True)]
        if scenario == 'local_roundtrip':
            result += [cap('param.set', 'set', local=True), cap('param.undeclare', 'undeclare', local=True)]
        elif scenario == 'undeclare_wrong_kind':
            selected = 'float64' if kind == 'bool' else 'bool'
            result += [cap('param.undeclare', 'undeclare', selected, True),
                       cap('param.undeclare', 'cleanup', local=True)]
        else:
            narrow = cap('param.undeclare', 'undeclare', local=True)
            narrow['limits'].update({'parameter-min-value': '0', 'parameter-max-value': '1'})
            result += [narrow, cap('param.set', 'set', local=True)]
        return result
    get = cap('param.get', 'get')
    if scenario in ('get_bounds', 'get_count_quota', 'get_byte_quota', 'get_mixed_native', 'get_nonfinite'):
        if scenario == 'get_bounds':
            get['limits'].update(dict(zip(('parameter-min-value', 'parameter-max-value'), NARROW[kind])))
        if scenario == 'get_count_quota':
            get['limits']['max-parameter-elements'] = '2'
        if scenario == 'get_byte_quota':
            get['limits']['max-parameter-bytes'] = '4'
        return [get]
    if scenario == 'operation_denied':
        return [get]
    set_cap = cap('param.set', 'set')
    if scenario == 'count_quota':
        set_cap['limits']['max-parameter-elements'] = '2'
    if scenario == 'byte_quota':
        set_cap['limits']['max-parameter-bytes'] = '4'
    if scenario == 'range':
        set_cap['limits'].update(dict(zip(('parameter-min-value', 'parameter-max-value'), NARROW[kind])))
    if scenario == 'absent_denied':
        for selected in (get, set_cap):
            selected['resource'] = '/uwvm_resources/native/array_absent_value'
            selected['limits']['parameter-name'] = 'array_absent_value'
    return [get, set_cap]


def native(version, kind, scenario):
    if scenario.startswith(('local_', 'undeclare_')):
        return []
    modes = {'roundtrip': '--array-observe-set', 'empty_roundtrip': '--array-observe-empty',
             'create': '--array-observe-create', 'wrong_kind': '--array-wrong-kind',
             'dynamic_denied': '--array-dynamic-denied'}
    bad = {'get_bounds': 'bounds', 'get_count_quota': 'count', 'get_byte_quota': 'bytes',
           'get_mixed_native': 'mixed', 'get_nonfinite': 'nonfinite'}
    return [f'/tmp/ros{version}-build/uwvm-ros{version}-resources-probe',
            '--array-bad-native', kind, bad[scenario]] if scenario in bad else [
            f'/tmp/ros{version}-build/uwvm-ros{version}-resources-probe',
            modes.get(scenario, '--array-observe-denied'), kind]
