#!/usr/bin/env python3
"""Encode a closed, readable parameter policy as an inert canonical definition.

The logical profile SHA256 covers the complete body after magic, parameter
identity and logical digest. The transport SHA256 covers the entire file.
Neither digest selects a path, provider or permission. This tool performs no
ROS/native loading; VM deployment must independently bound and verify its input.
"""
import argparse
import base64
import datetime
import hashlib
import json
import math
from pathlib import Path
import re
import struct

MAXIMUM = 4 * 1024 * 1024
MAGIC = 'uwvm.ros.parameter.definition'
KINDS = dict(zip(('not_set', 'boolean', 'integer', 'floating', 'string', 'bytes', 'booleans', 'integers', 'floats', 'strings',
                  'xmlrpc_array', 'xmlrpc_struct', 'xmlrpc_datetime'), (*range(10), 32, 33, 34)))


def fail(message):
    raise ValueError(message)


def object_pairs(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            fail('Duplicate JSON field: ' + key)
        result[key] = value
    return result


def closed(value, fields):
    if not isinstance(value, dict) or set(value) != set(fields):
        fail('Expected exactly these fields: ' + ', '.join(fields))
    return value


def integer(value, minimum, maximum):
    if type(value) is not int or not minimum <= value <= maximum:
        fail('Integer outside explicit range')
    return value


def text(value, maximum, *, empty=True, nul=False):
    if not isinstance(value, str) or not empty and not value or not nul and '\0' in value:
        fail('Invalid text')
    encoded = value.encode('utf-8', errors='strict')
    if len(encoded) > maximum:
        fail('Text exceeds byte bound')
    return value


def finite(value):
    if type(value) not in (int, float) or not math.isfinite(value):
        fail('Finite number required')
    return float(value)


class Writer:
    def __init__(self):
        self.data = bytearray()

    def raw(self, data):
        if len(data) > MAXIMUM - len(self.data):
            fail('Canonical definition exceeds 4 MiB')
        self.data.extend(data)

    def u64(self, value):
        self.raw(struct.pack('<Q', integer(value, 0, (1 << 64) - 1)))

    def i64(self, value):
        self.raw(struct.pack('<q', integer(value, -(1 << 63), (1 << 63) - 1)))

    def f64(self, value):
        self.raw(struct.pack('<d', value))

    def boolean(self, value):
        if type(value) is not bool:
            fail('Explicit boolean required')
        self.u64(int(value))

    def blob(self, data):
        self.u64(len(data))
        self.raw(data)

    def text(self, value):
        self.blob(value.encode('utf-8', errors='strict'))


def numeric_range(out, value, kind):
    closed(value, ('minimum', 'maximum', 'step'))
    if kind == 'integer':
        lower = integer(value['minimum'], -(1 << 63), (1 << 63) - 1)
        upper = integer(value['maximum'], -(1 << 63), (1 << 63) - 1)
        step = integer(value['step'], 0, (1 << 64) - 1)
        if lower > upper:
            fail('Reversed integer range')
        out.i64(lower); out.i64(upper); out.u64(step)
    else:
        lower, upper, step = (finite(value[name]) for name in ('minimum', 'maximum', 'step'))
        if lower > upper:
            fail('Reversed floating range')
        out.f64(lower); out.f64(upper); out.f64(step)


def descriptor(out, value):
    closed(value, ('name', 'type', 'description', 'additional_constraints', 'read_only', 'dynamic_type', 'range'))
    out.text(text(value['name'], 256, empty=False))
    if value['type'] not in KINDS:
        fail('Unknown declared type')
    out.text(value['type']); out.text(text(value['description'], 512)); out.text(text(value['additional_constraints'], 512))
    out.boolean(value['read_only']); out.boolean(value['dynamic_type']); out.boolean(value['range'] is not None)
    if value['range'] is not None:
        interval = closed(value['range'], ('kind', 'minimum', 'maximum', 'step'))
        kind = interval['kind']
        if kind not in ('integer', 'floating') or kind != value['type']:
            fail('Descriptor range differs from declared numeric type')
        out.u64(1 if kind == 'integer' else 2)
        numeric_range(out, {k: interval[k] for k in ('minimum', 'maximum', 'step')}, kind)


class ValueBudget:
    def __init__(self):
        self.nodes = 65536
        self.elements = 65536

    def consume(self, nodes=0, elements=0):
        if nodes > self.nodes or elements > self.elements:
            fail('Override exceeds hard parser budget')
        self.nodes -= nodes; self.elements -= elements


def encode_value(out, value, algebra, nonfinite, budget, depth=1):
    if depth > 16:
        fail('Override nesting exceeds 16')
    budget.consume(nodes=1)
    closed(value, ('type', 'value'))
    kind = value['type']
    if kind not in KINDS:
        fail('Unknown parameter value type')
    tag = KINDS[kind]
    if algebra == 'ros1' and tag not in (1, 2, 3, 4, 5, 32, 33, 34) or algebra == 'ros2' and tag > 9:
        fail('Value belongs to the other ROS algebra')
    data = value['value']; out.u64(tag)

    def floating(item):
        if isinstance(item, str) and item in ('nan', '+infinity', '-infinity') and nonfinite:
            return {'nan': math.nan, '+infinity': math.inf, '-infinity': -math.inf}[item]
        return finite(item)

    def string(item):
        item = text(item, MAXIMUM, nul=True)
        if algebra == 'ros1' and any(ord(char) < 32 and char not in '\t\n\r' or ord(char) in (0xfffe, 0xffff) for char in item):
            fail('ROS1 value is not XML 1.0 text')
        out.text(item)

    if kind == 'not_set':
        if data is not None:
            fail('not_set requires an explicit null value')
    elif kind == 'boolean':
        out.boolean(data)
    elif kind == 'integer':
        out.i64(integer(data, -(1 << (31 if algebra == 'ros1' else 63)), (1 << (31 if algebra == 'ros1' else 63)) - 1))
    elif kind == 'floating':
        out.f64(floating(data))
    elif kind == 'string':
        string(data)
    elif kind == 'bytes':
        if not isinstance(data, str):
            fail('bytes value must be canonical base64 text')
        decoded = base64.b64decode(data, validate=True)
        if base64.b64encode(decoded).decode('ascii') != data:
            fail('Noncanonical base64')
        budget.consume(elements=len(decoded)); out.blob(decoded)
    elif kind == 'xmlrpc_datetime':
        if not isinstance(data, str) or not re.fullmatch(r'[0-9]{8}T[0-9]{2}:[0-9]{2}:[0-9]{2}', data):
            fail('Invalid XML-RPC datetime')
        datetime.datetime.strptime(data, '%Y%m%dT%H:%M:%S'); string(data)
    else:
        if not isinstance(data, list):
            fail('Array value must be a list')
        budget.consume(elements=len(data)); out.u64(len(data))
        if kind == 'xmlrpc_struct':
            if len(data) > 128:
                fail('XML-RPC struct exceeds 128 members')
            names = set()
            for member in data:
                closed(member, ('name', 'value'))
                name = text(member['name'], MAXIMUM, nul=True)
                if name in names:
                    fail('Duplicate XML-RPC member')
                names.add(name); string(name); encode_value(out, member['value'], algebra, nonfinite, budget, depth + 1)
        elif kind == 'xmlrpc_array':
            for item in data:
                encode_value(out, item, algebra, nonfinite, budget, depth + 1)
        else:
            for item in data:
                budget.consume(nodes=1)
                if kind == 'booleans': out.boolean(item)
                elif kind == 'integers': out.i64(item)
                elif kind == 'floats': out.f64(floating(item))
                elif kind == 'strings': string(item)


def encode(document):
    closed(document, ('profile', 'node_profile', 'algebra', 'nonfinite', 'deferred_validation', 'floating_tolerance', 'search_root', 'search_candidates', 'keys'))
    profile = closed(document['profile'], ('identity',))
    identity = text(profile['identity'], 96, empty=False)
    node = closed(document['node_profile'], ('identity', 'sha256'))
    node_identity = text(node['identity'], 96, empty=False)
    if not isinstance(node['sha256'], str) or not re.fullmatch(r'[0-9a-fA-F]{64}', node['sha256']):
        fail('Node profile SHA256 must contain exactly 64 hexadecimal characters')
    algebra = document['algebra']
    if algebra not in ('ros1', 'ros2'):
        fail('Choose exactly ros1 or ros2')
    out = Writer(); out.text(node_identity); out.blob(bytes.fromhex(node['sha256'])); out.text(algebra)
    out.boolean(document['nonfinite']); out.boolean(document['deferred_validation'])
    tolerance = finite(document['floating_tolerance'])
    if tolerance < 0:
        fail('Negative floating tolerance')
    out.f64(tolerance)
    root = text(document['search_root'], 255)
    candidates = integer(document['search_candidates'], 0, 256)
    if bool(root) != bool(candidates) or candidates and (algebra != 'ros1' or
            root != '/' and not re.fullmatch(r'(?:/[A-Za-z_][A-Za-z_0-9]*)+', root)):
        fail('Search requires an explicit ROS1 absolute root and finite candidate bound; empty/zero disables it')
    out.text(root); out.u64(candidates)
    keys = document['keys']
    if not isinstance(keys, list) or len(keys) > 65536:
        fail('Invalid key list')
    out.u64(len(keys)); previous = None
    for key in keys:
        closed(key, ('descriptor', 'types', 'integer_range', 'floating_range', 'override'))
        descriptor(out, key['descriptor']); name = key['descriptor']['name']
        if previous is not None and name <= previous:
            fail('Keys must be unique and sorted')
        previous = name; types = key['types']
        if not isinstance(types, list) or not types or any(type(item) is not str or item not in KINDS for item in types) or len(set(types)) != len(types):
            fail('Types must be an explicit unique list')
        if key['descriptor']['type'] not in types:
            fail('Declared type is absent from the allowed type list')
        if any((KINDS[item] not in (1, 2, 3, 4, 5, 32, 33, 34)) if algebra == 'ros1' else KINDS[item] > 9 for item in types):
            fail('Allowed type belongs to the other ROS algebra')
        out.u64(len(types))
        for item in sorted(types, key=KINDS.get): out.text(item)
        for field, kind in (('integer_range', 'integer'), ('floating_range', 'floating')):
            out.boolean(key[field] is not None)
            if key[field] is not None:
                if kind not in types: fail('Numeric constraint without its allowed type')
                numeric_range(out, key[field], kind)
        out.boolean(key['override'] is not None)
        if key['override'] is not None:
            override = closed(key['override'], ('type', 'value'))
            if override['type'] not in types:
                fail('Override type is absent from the allowed type list')
            if override['type'] != 'not_set' and not key['descriptor']['dynamic_type'] and override['type'] != key['descriptor']['type']:
                fail('Override differs from the fixed descriptor type')
            value = Writer(); encode_value(value, key['override'], algebra, document['nonfinite'], ValueBudget()); out.blob(value.data)
    logical = hashlib.sha256(out.data).digest()
    file = Writer(); file.text(MAGIC); file.text(identity); file.blob(logical); file.raw(out.data)
    metadata = {'bytes': len(file.data), 'definition_sha256': hashlib.sha256(file.data).hexdigest(),
                'profile': {'identity': identity, 'sha256': logical.hex()},
                'node_profile': {'identity': node_identity, 'sha256': node['sha256'].lower()}, 'algebra': algebra}
    return bytes(file.data), metadata


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--metadata', type=Path)
    args = parser.parse_args()
    try:
        with args.input.open('rb') as source:
            raw = source.read(MAXIMUM + 1)
        if len(raw) > MAXIMUM: fail('JSON source exceeds 4 MiB')
        document = json.loads(raw, object_pairs_hook=object_pairs, parse_constant=lambda value: fail('Nonstandard JSON constant: ' + value))
        encoded, metadata = encode(document)
        if args.output.exists() or args.metadata and args.metadata.exists(): fail('Output already exists')
        with args.output.open('xb') as output: output.write(encoded)
        if args.metadata:
            with args.metadata.open('x', encoding='utf-8') as output: output.write(json.dumps(metadata, indent=2) + '\n')
        print(json.dumps(metadata, indent=2))
    except (OSError, ValueError, TypeError, UnicodeError, OverflowError) as error:
        parser.error(str(error))


if __name__ == '__main__':
    main()
