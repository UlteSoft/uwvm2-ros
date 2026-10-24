"""Independent whole original Unitree .msg to Python value/wire oracle.

No production parser/emitter/codec is imported. The closed original grammar
supplies declaration order, scalar widths, nesting and complete dynamic fields.
Synthetic nonzero values establish representation, not physical validity.
"""
import hashlib
import json
from pathlib import Path
import re
import struct


SCALARS = dict(bool='B', int8='b', uint8='B', int16='h', uint16='H',
               int32='i', uint32='I', int64='q', uint64='Q', float32='f', float64='d')


def build(materialized):
    root = Path(materialized)
    catalog = json.loads((root/'catalog.preparation.json').read_bytes())
    assert catalog['format'] == 'uwvm-ros-vendor-complete-bounded-v1'
    records = {}
    for row in catalog['types']:
        fields = []
        for line in (root/row['file']).read_text().splitlines():
            line = line.split('#', 1)[0].strip()
            if not line: continue
            match = re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:\[([0-9]*)\])?\s+([A-Za-z_][A-Za-z0-9_]*)', line)
            assert match, line
            fields.append(match.groups())
        assert row['type'] not in records and row['ros_version'] in (1, 2)
        records[row['type']] = (row, fields)
    def fill(name, path):
        row, fields = records[name]
        wire, members = bytearray(), {}
        for kind, array, member in fields:
            quota = row['bounds'].get(member, {})
            count = 1 if array is None else int(array) if array else min(2, quota['sequence'])
            if array == '': wire.extend(struct.pack('<I', count))
            values = []
            for index in range(count):
                key = path+'.'+member+('['+str(index)+']' if array is not None else '')
                seed = int.from_bytes(hashlib.sha256(key.encode()).digest()[:4], 'little')
                if kind in SCALARS:
                    if kind == 'bool': value = bool(seed & 1)
                    elif kind.startswith('float'): value = (seed % 401 - 200)/8.0
                    else:
                        bits = int(kind[4:] if kind.startswith('uint') else kind[3:])
                        value = (seed ^ 0xa1529bcd) & ((1 << bits)-1)
                        if kind.startswith('int') and value >= 1 << (bits-1): value -= 1 << bits
                    wire.extend(struct.pack('<'+SCALARS[kind], value))
                    value = dict(scalar=kind, value=value)
                elif kind == 'string':
                    body = ('v'+format(seed, '08x')+'β').encode()
                    assert len(body) <= quota['string']
                    wire.extend(struct.pack('<I', len(body))+body)
                    value = dict(scalar='string', value=body.decode())
                else:
                    parts = kind.split('/')
                    if len(parts) == 1:
                        nested = name.split('/')[0]+('/msg/' if row['ros_version'] == 2 else '/')+kind
                    elif len(parts) == 2 and row['ros_version'] == 2:
                        nested = parts[0]+'/msg/'+parts[1]
                    else:
                        nested = kind
                    body, value = fill(nested, key)
                    wire.extend(body)
                values.append(value)
            members[member] = values[0] if array is None else dict(array=values)
        return bytes(wire), dict(record=name, members=members)
    cases = []
    for name in records:
        wire, value = fill(name, name)
        cases.append(dict(type=name, wire=wire.hex(), value=value))
    assert 1 <= len(cases) <= 128
    return dict(cases=cases)
