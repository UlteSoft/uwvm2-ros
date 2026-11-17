#!/usr/bin/env python3
"""Encode an explicit transform topology as a bounded canonical definition.

This host configuration tool starts no ROS provider and grants no capability.
Its SHA256 covers the entire canonical definition; there is no embedded digest.
All thirteen capacity/deadline limits remain separate explicit deployment input.
Only --input, --output and --metadata select paths. JSON fields never do so.
"""
import argparse
import hashlib
import json
import math
from pathlib import Path
import struct

MAXIMUM_INPUT = 1024 * 1024
MAXIMUM_DEFINITION = 65536
MAGIC = 'uwvm.ros.transform.definition'
LIMIT_NAMES = ('cache.samples', 'cache.samples-per-child', 'publish.batch', 'cache.history-ns', 'publish.future-ns',
               'velocity.window-ns', 'session.requests', 'query.deadline-ns', 'sessions', 'budget.data-bytes',
               'budget.workspace-bytes', 'budget.request-bytes', 'budget.carrier-bytes')
PHYSICAL_FIELDS = ('translation_metres', 'quaternion_tolerance', 'linear_metres_per_second', 'angular_radians_per_second')


def reject(message):
    raise ValueError(message)


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            reject('Duplicate JSON field: ' + key)
        result[key] = value
    return result


def closed(value, fields):
    if not isinstance(value, dict) or set(value) != set(fields):
        reject('Expected exactly these fields: ' + ', '.join(fields))
    return value


def text(value, maximum):
    if type(value) is not str or not value or '\0' in value:
        reject('Nonempty UTF-8 text without NUL is required')
    encoded = value.encode('utf-8', errors='strict')
    if len(encoded) > maximum:
        reject('UTF-8 text exceeds its byte bound')
    return encoded


def number(value):
    if type(value) not in (int, float):
        reject('Physical bounds must be explicit finite numbers')
    value = float(value)
    if not math.isfinite(value) or value < 0:
        reject('Physical bounds must be finite and nonnegative')
    return value


class Writer:
    def __init__(self):
        self.data = bytearray()

    def raw(self, value):
        if len(value) > MAXIMUM_DEFINITION - len(self.data):
            reject('Definition exceeds 65536 bytes')
        self.data.extend(value)

    def u64(self, value):
        self.raw(struct.pack('<Q', value))

    def blob(self, value):
        self.u64(len(value)); self.raw(value)

    def f64(self, value):
        self.raw(struct.pack('<d', value))


def encode(document):
    closed(document, ('identity', 'frames', 'edges', 'physical_limits'))
    identity = text(document['identity'], 96)
    frames = document['frames']
    if type(frames) is not list or not 1 <= len(frames) <= 128:
        reject('Frame list must contain between 1 and 128 exact names')
    names = [text(name, 255) for name in frames]
    if any(left >= right for left, right in zip(names, names[1:])):
        reject('Frames must be unique and sorted by UTF-8 bytes')
    supplied = document['edges']
    if type(supplied) is not list or len(supplied) > 512:
        reject('At most 512 explicit edges are permitted')
    edges = []; seen = set(); adjacency = [[] for _ in frames]; indegree = [0] * len(frames)
    for edge in supplied:
        closed(edge, ('parent', 'child', 'dynamic', 'static'))
        parent, child = edge['parent'], edge['child']
        if type(parent) is not int or type(child) is not int or not 0 <= parent < len(frames) or not 0 <= child < len(frames):
            reject('Edge indices must address the explicit frame list')
        if parent == child or (parent, child) in seen:
            reject('Self edges and duplicate directed edges are forbidden')
        if type(edge['dynamic']) is not bool or type(edge['static']) is not bool or not (edge['dynamic'] or edge['static']):
            reject('Each edge requires explicit booleans and at least one permitted mode')
        seen.add((parent, child)); adjacency[parent].append(child); indegree[child] += 1
        edges.append((parent, child, edge['dynamic'], edge['static']))
    queue = [index for index, degree in enumerate(indegree) if degree == 0]
    for parent in queue:
        for child in adjacency[parent]:
            indegree[child] -= 1
            if indegree[child] == 0:
                queue.append(child)
    if len(queue) != len(frames):
        reject('The union of dynamic and static permitted edges must be acyclic')
    physical = closed(document['physical_limits'], PHYSICAL_FIELDS)
    bounds = [number(physical[name]) for name in PHYSICAL_FIELDS]
    if bounds[1] >= 1:
        reject('Quaternion tolerance must be below one')
    writer = Writer(); writer.blob(MAGIC.encode('ascii')); writer.blob(identity); writer.u64(len(names))
    for name in names:
        writer.blob(name)
    writer.u64(len(edges))
    # Stable canonical output independent of JSON edge order; frame indices never change.
    for parent, child, dynamic, fixed in sorted(edges):
        writer.u64(parent); writer.u64(child); writer.u64(int(dynamic)); writer.u64(int(fixed))
    for bound in bounds:
        writer.f64(bound)
    data = bytes(writer.data); digest = hashlib.sha256(data).hexdigest()
    metadata = dict(identity=document['identity'], sha256=digest, bytes=len(data), frames=len(names), edges=len(edges),
                    required_separate_limits=list(LIMIT_NAMES), authority_granted=False)
    return data, metadata


def cli_arguments(metadata, output, environment, alias):
    """Return argument arrays for review; never execute them or invent grants."""
    return [
        ['--ros-scene-transform-profile', environment, alias, metadata['identity'], metadata['sha256']],
        ['--ros-scene-transform-definition', environment, alias, str(output), str(metadata['bytes']), metadata['sha256']],
    ]


def parse(raw):
    if len(raw) > MAXIMUM_INPUT:
        reject('JSON source exceeds 1 MiB')
    return json.loads(raw, object_pairs_hook=unique_object, parse_constant=lambda value: reject('Nonstandard JSON constant: ' + value))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--metadata', type=Path)
    parser.add_argument('--environment', default='<environment>')
    parser.add_argument('--alias', default='<alias>')
    args = parser.parse_args()
    try:
        with args.input.open('rb') as source:
            document = parse(source.read(MAXIMUM_INPUT + 1))
        data, metadata = encode(document)
        metadata['suggested_cli_arguments'] = cli_arguments(metadata, args.output, args.environment, args.alias)
        if args.output.exists() or args.metadata and args.metadata.exists():
            reject('Output already exists')
        with args.output.open('xb') as destination:
            destination.write(data)
        if args.metadata:
            with args.metadata.open('x', encoding='utf-8') as destination:
                destination.write(json.dumps(metadata, ensure_ascii=False, indent=2) + '\n')
        print(json.dumps(metadata, ensure_ascii=False, indent=2))
    except (OSError, ValueError, TypeError, UnicodeError, OverflowError, RecursionError) as error:
        parser.error(str(error))


if __name__ == '__main__':
    main()
