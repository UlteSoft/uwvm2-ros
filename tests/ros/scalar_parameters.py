"""Explicit scalar fixture values; no runtime defaults or permission presets."""
import struct

KINDS = {1: ('bool', 'int32', 'float64'), 2: ('bool', 'int64', 'float64')}
VALUES = {
    'bool': {'old': False, 'new': True, 'width': 1, 'bounds': None},
    'int32': {'old': -7, 'new': -123456789, 'width': 4,
              'bounds': ('-200000000', '200000000')},
    'int64': {'old': -7, 'new': 9007199254740993, 'width': 8,
              'bounds': ('-9007199254740995', '9007199254740995')},
    'float64': {'old': -0.5, 'new': 1.25, 'width': 8, 'bounds': ('-2', '2')},
}


def encode(kind, value):
    return struct.pack({'bool': '<B', 'int32': '<i', 'int64': '<q', 'float64': '<d'}[kind], value)


def malformed(kind):
    if kind == 'bool':
        return [(b'\x02', 22, 'boolean-2')]
    value = encode(kind, VALUES[kind]['new'])
    rows = [(value[:-1], 22, 'truncated'), (value + b'\x00', 22, 'excess')]
    if kind == 'float64':
        rows += [(struct.pack('<Q', bits), 22, label) for bits, label in
                 ((0x7ff8000000000001, 'nan'), (0x7ff0000000000000, 'positive-infinity'),
                  (0xfff0000000000000, 'negative-infinity'))]
    return rows


def outside(kind):
    bounds = VALUES[kind]['bounds']
    if bounds is None:
        return []
    number = float if kind == 'float64' else int
    return [(encode(kind, number(bounds[0]) - 1), 13, 'below-minimum'),
            (encode(kind, number(bounds[1]) + 1), 13, 'above-maximum')]


def cases(version):
    rows = []
    for kind in KINDS[version]:
        for scenario in ('roundtrip', 'malformed', 'wrong_kind', 'operation_denied'):
            rows.append({'kind': kind, 'scenario': scenario})
        if kind != 'bool':
            rows += [{'kind': kind, 'scenario': scenario} for scenario in ('range', 'get_bounds')]
        if version == 1:
            rows.append({'kind': kind, 'scenario': 'create'})
        if version == 2:
            rows += [{'kind': kind, 'scenario': scenario} for scenario in
                     ('dynamic_denied', 'absent_denied', 'local_roundtrip', 'undeclare_wrong_kind')]
            if kind != 'bool':
                rows.append({'kind': kind, 'scenario': 'undeclare_bounds'})
    for row in rows:
        row['name'] = f"ros{version}-parameter_{row['kind']}_{row['scenario']}"
    return rows
