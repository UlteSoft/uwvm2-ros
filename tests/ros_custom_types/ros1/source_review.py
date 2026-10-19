#!/usr/bin/env python3
"""Review ROS1 custom-topic source admission with pure models only.

No compiler, generated native executable, ROS Master, Gateway, QEMU, or Wasm
runtime is invoked. A SOURCE_AND_MODEL_PASS report records preparation evidence
and must never qualify a compiled product or actual interoperability result.
Output reports are created exclusively so previous results remain immutable.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import pathlib
import re
import struct
import unittest
import xml.etree.ElementTree as etree

ROOT = pathlib.Path(__file__).resolve().parents[3]
OWNED = (
    'gateways/ros1/custom_topics.h', 'gateways/ros1/main.cc',
    'gateways/ros1/CMakeLists.txt', 'tests/ros_custom_types/ros1/CMakeLists.txt',
    'tests/ros_custom_types/ros1/native_topics.cc',
    'tests/ros_custom_types/ros1/catalog.schema.json',
    'tests/ros_custom_types/ros1/uwvm_custom_ros1_fixture/msg/Packet.msg',
    'tests/ros_custom_types/ros1/uwvm_custom_ros1_fixture/package.xml',
    'tests/ros_custom_types/ros1/uwvm_custom_ros1_fixture/CMakeLists.txt',
    'tests/ros_custom_types/ros1/README.md',
    'tests/ros_custom_types/ros1/source_review.py',
)


def source(path: str) -> str:
    """Read review source; reading it does not execute its native contents."""
    return (ROOT / path).read_text(encoding='utf-8')


def qualified(cap: dict, rows: list[dict], descriptor: dict | None) -> bool:
    """Model exact-one provider admission, including all independent identities."""
    selected = [row for row in rows if row['type'] == cap['type']]
    if len(selected) != 1 or descriptor is None:
        return False
    row = selected[0]
    return (
        cap['operation'] in ('topic.publish', 'topic.subscribe')
        and descriptor['version'] == 1 and row['version'] == 1
        and descriptor['type'] == row['type'] == cap['type']
        and descriptor['native'] == row['native'] == cap['native'] == row['actual_native']
        and descriptor['codec'] == row['codec'] == cap.get('codec')
        and descriptor['bytes'] == row['bytes']
        and row['type'] == row['actual_type'] and row['fixed_native']
    )


def canonical_packet(bytes_: bytes) -> bool:
    """Model the fixture layout's exact length, bool, and finite-f64 checks."""
    return len(bytes_) == 23 and bytes_[4] <= 1 and math.isfinite(struct.unpack_from('<d', bytes_, 5)[0])


def definition_ast(text: str, root: str) -> dict:
    """Independently parse bounded official full definitions into complete ASTs.

    This Python model uses a strict declaration grammar and a section dictionary;
    it does not execute or translate the C++ helper. ROS primitive aliases and
    package-relative nested names normalize to the same semantic field type.
    """
    if len(text) > 16 * 1024 * 1024:
        raise ValueError('definition size')
    primitives = {'bool', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32',
                  'int64', 'uint64', 'float32', 'float64'}
    records = {root: []}
    current, pending, total = root, False, 0
    for raw in text.splitlines():
        line = raw.split('#', 1)[0].strip()
        if not line:
            continue
        if re.fullmatch(r'={3,}', line):
            if pending:
                raise ValueError('duplicate separator')
            pending = True
            continue
        if line.startswith('MSG:'):
            name = line[4:].strip()
            if not pending or not re.fullmatch(r'[a-z][a-z0-9_]*/[A-Z][A-Za-z0-9_]*', name) or name in records:
                raise ValueError('unknown or duplicate section')
            if len(records) == 128:
                raise ValueError('section count')
            records[name], current, pending = [], name, False
            continue
        if pending:
            raise ValueError('missing MSG section')
        match = re.fullmatch(r'([A-Za-z][A-Za-z0-9_/]*)(?:\[([0-9]{1,10})\])?\s+([A-Za-z_][A-Za-z0-9_]*)', line)
        if not match:
            raise ValueError('unsupported field declaration')
        kind, count_text, name = match.groups()
        if len(kind) > 512 or len(name) > 255:
            raise ValueError('token size')
        count = None if count_text is None else int(count_text)
        if count is not None and not 1 <= count <= 4096:
            raise ValueError('array bound')
        kind = {'byte': 'int8', 'char': 'uint8'}.get(kind, kind)
        if kind not in primitives:
            if '/' not in kind:
                kind = current.split('/', 1)[0] + '/' + kind
            if not re.fullmatch(r'[a-z][a-z0-9_]*/[A-Z][A-Za-z0-9_]*', kind):
                raise ValueError('unknown type')
        records[current].append((kind, name, count))
        total += 1
        if total > 4096:
            raise ValueError('field count')
    if pending:
        raise ValueError('trailing separator')
    return records


class Review(unittest.TestCase):
    def test_complete_official_definition_ast_models(self):
        root = 'custom_fixture/Outer'
        expected = {'custom_fixture/Outer': [('custom_fixture/Inner', 'members', 2), ('float64', 'reading', None)],
                    'custom_fixture/Inner': [('int32', 'sequence', None), ('bool', 'active', None), ('uint8', 'tag', 4)]}
        native = '# root documentation\nInner[2] members\nfloat64 reading # field\n==========\nMSG: custom_fixture/Inner\nint32 sequence\nbool active\nchar[4] tag\n'
        self.assertEqual(definition_ast(native, root), expected)
        normalized = native.replace('Inner[2]', 'custom_fixture/Inner[2]').replace('char[4]', 'uint8[4]')
        self.assertEqual(definition_ast(normalized, root), expected)
        self.assertEqual(definition_ast(native.replace('char[4]', 'char[04]'), root), expected)
        for changed in (
            native.replace('int32 sequence', 'int16 sequence'),
            native.replace('bool active', 'uint8 active'),  # Equal ROS1 storage does not imply equal semantics.
            native.replace('char[4]', 'char[5]'), native.replace('members', 'renamed'),
            native.replace('bool active\n', ''), native.replace('float64 reading # field\n', ''),
            native.replace('bool active', 'bool active\nuint8 unexpected'),
            native.replace('int32 sequence\nbool active', 'bool active\nint32 sequence'),
            native.replace('Inner[2]', 'other_package/Inner[2]'),
            native.split('==========')[0],
            native + '==========\nMSG: custom_fixture/Extra\nint32 unused\n',
        ):
            self.assertNotEqual(definition_ast(changed, root), expected)
        for malformed in (
            native + '==========\nMSG: custom_fixture/Inner\nint32 sequence\n',
            native.replace('char[4]', 'char[]'), native.replace('char[4]', 'char[0]'),
            native.replace('char[4]', 'char[4097]'), native.replace('char[4]', 'string'),
            native.replace('bool active', 'bool active = false'),
            native.replace('bool active', 'uint8 CONSTANT=1'),
            native.replace('==========\n', ''), native + '==========\n',
            native.replace('==========', '==========\n=========='),
        ):
            with self.assertRaises(ValueError):
                definition_ast(malformed, root)

    def test_independent_identity_refusals_and_exact_one_provider(self):
        cap = {'operation': 'topic.publish', 'type': 'uwvm_custom_ros1_fixture/Packet',
               'native': 'a' * 32, 'codec': 'UWVMROS1_' + 'b' * 64}
        descriptor = {'version': 1, 'type': cap['type'], 'native': cap['native'], 'codec': cap['codec'], 'bytes': 23}
        row = dict(descriptor, actual_native=cap['native'], actual_type=cap['type'], fixed_native=True)
        self.assertTrue(qualified(cap, [row], descriptor))
        self.assertTrue(qualified(dict(cap, operation='topic.subscribe'), [row], descriptor))
        for field, changed in [('operation', 'service.call'), ('operation', 'action.send-goal'),
                               ('type', 'unknown/Packet'), ('native', 'c' * 32), ('codec', 'wrong')]:
            self.assertFalse(qualified(dict(cap, **{field: changed}), [row], descriptor), field)
        missing = dict(cap); missing.pop('codec')
        self.assertFalse(qualified(missing, [row], descriptor))
        for field, changed in [('version', 2), ('type', 'other/Packet'), ('native', 'c' * 32),
                               ('codec', 'wrong'), ('bytes', 22)]:
            self.assertFalse(qualified(cap, [row], dict(descriptor, **{field: changed})), field)
        for field, changed in [('actual_native', 'c' * 32), ('actual_type', 'other/Packet'),
                               ('fixed_native', False), ('version', 2), ('bytes', 24)]:
            self.assertFalse(qualified(cap, [dict(row, **{field: changed})], descriptor), field)
        self.assertFalse(qualified(cap, [], descriptor))
        self.assertFalse(qualified(cap, [row], None))
        self.assertFalse(qualified(cap, [row, row], descriptor))
        stale = dict(row, actual_native='c' * 32)
        self.assertFalse(qualified(cap, [row, stale], descriptor))
        self.assertFalse(qualified(cap, [stale, row], descriptor))
        self.assertTrue(qualified(cap, [row, dict(row, type='other/Packet')], descriptor))

    def test_fixed_wire_negative_cases(self):
        packet = struct.pack('<iBd4B3h', -12345, 1, -1.25, 0, 127, 128, 255, -32768, 0, 32767)
        self.assertTrue(canonical_packet(packet))
        for truncated in range(len(packet)):
            self.assertFalse(canonical_packet(packet[:truncated]), truncated)
        self.assertFalse(canonical_packet(packet + b'\0'))
        # A byte quota is checked before structural validation. Keep these two
        # refusal paths distinct in the prepared native fixture's assertions.
        def publish_refusal(bytes_: bytes, maximum: int) -> int:
            return 90 if len(bytes_) > maximum else 0 if canonical_packet(bytes_) else 22
        self.assertEqual(publish_refusal(packet + b'\0', 23), 90)
        self.assertEqual(publish_refusal(packet + b'\0', 24), 22)
        for value in (2, 127, 255):
            malformed = bytearray(packet); malformed[4] = value
            self.assertFalse(canonical_packet(bytes(malformed)))
        for value in (float('nan'), float('inf'), -float('inf')):
            malformed = bytearray(packet); struct.pack_into('<d', malformed, 5, value)
            self.assertFalse(canonical_packet(bytes(malformed)))
        for value in (0.0, -0.0, 1.25, -1.25, float.fromhex('0x0.0000000000001p-1022')):
            valid = bytearray(packet); struct.pack_into('<d', valid, 5, value)
            self.assertTrue(canonical_packet(bytes(valid)))

    def test_native_source_validation_order_and_default(self):
        text = source('gateways/ros1/custom_topics.h')
        callback = text[text.index('[weak, verified]('):text.index('auto native{store.node.subscribe<Native>')]
        self.assertLess(callback.index('if(!queue->active)'), callback.index('Codec::valid(*message)'))
        self.assertLess(callback.index('Codec::valid(*message)'), callback.index('bytes(Codec::wire_bytes)'))
        self.assertLess(callback.index('validate_custom_topic'), callback.index('queue->enqueue'))
        publisher = text[text.index('static ipc::call_result publish('):text.index('static ipc::call_result subscribe(')]
        self.assertLess(publisher.index('validate_custom_topic'), publisher.index('Codec::decode'))
        self.assertLess(publisher.index('Codec::decode'), publisher.index('store.node.advertise'))
        self.assertIn('IsFixedSize<Native>::value', text)
        self.assertIn('IsMessage<Native>::value', text)
        self.assertIn('return matches == 1 && qualified;', text)
        self.assertIn('++matches;', text)
        self.assertIn('# define UWVM_ROS_CUSTOM_ROS1_TYPES(M)\n', text)
        self.assertIn('defined(UWVM_ROS_CUSTOM_ROS1_HEADER) != defined(UWVM_ROS_CUSTOM_TOPIC_MANIFEST)', text)
        self.assertEqual(text.count('!ros1_custom::known(capability)'), 2)
        self.assertIn('!ros1_custom::known(queue->capability)', callback)
        self.assertLess(callback.index('!verified->load'), callback.index('weak.lock()'))
        subscription = text[text.index('static ipc::call_result subscribe('):text.index('// The trusted generated header')]
        self.assertLess(subscription.index('native.getTopic() != capability.resource'), subscription.index('verified->store(true'))
        self.assertLess(subscription.index('store.subscribers.emplace'), subscription.index('verified->store(true'))
        self.assertIn('existing native subscription', source('tests/ros_custom_types/ros1/native_topics.cc'))
        self.assertIn('store.retained(capability)', text)
        self.assertIn('first.limits == second.limits', text)
        self.assertIn('A custom ROS1 native provider macro cannot precede the selected header', text)
        self.assertIn('message_traits::Definition<Native>::value()', text)
        self.assertIn('Codec::definitions, Codec::definition_count', text)
        self.assertIn('native.size() > 16U * 1024U * 1024U', text)
        self.assertIn('++fields > 4096', text)
        self.assertIn('count > 128', text)
        for disallowed in ('std::span', 'std::bit_cast', 'concept ', '#include <fast_io'):
            self.assertNotIn(disallowed, text)

    def test_gateway_startup_dispatch_and_cleanup(self):
        text = source('gateways/ros1/main.cc')
        main = text[text.index('int main('):]
        self.assertLess(main.index('!known_type(p.second)'), main.index('ros::init('))
        self.assertIn('gateway::ros1_custom::known(c)', text)
        self.assertIn('gateway::ros1_custom::publish(custom_topics_,c,d)', text)
        self.assertIn('gateway::ros1_custom::subscribe(custom_topics_,c,std::move(q))', text)
        self.assertIn('custom_topics_.close(alias)', text)
        binding = source('gateways/ros1/custom_topics.h')
        self.assertIn('native.getTopic() != capability.resource', binding)
        close = binding[binding.index('void close('):binding.index('inline bool known(')]
        self.assertIn('publishers.erase(alias)', close)
        self.assertIn('subscribers.erase(alias)', close)
        self.assertLess(close.index('store(false'), close.index('subscribers.erase(alias)'))
        self.assertLess(close.index('queue->deactivate()'), close.index('subscribers.erase(alias)'))

    def test_explicit_paired_cmake_selection(self):
        text = source('gateways/ros1/CMakeLists.txt')
        for name in ('UWVM_ROS_CUSTOM_REGISTRY_HEADER', 'UWVM_ROS_CUSTOM_ROS1_HEADER', 'UWVM_ROS_CUSTOM_ROS1_PACKAGES'):
            self.assertIn(f'set({name} "" CACHE', text)
            self.assertIn(f'NOT {name}', text)
        self.assertIn('Duplicate explicit custom catkin package name', text)
        self.assertIn('${UWVM_ROS_CUSTOM_ROS1_PACKAGES})', text)
        self.assertIn('UWVM_ROS_CUSTOM_TOPIC_MANIFEST="${UWVM_ROS_CUSTOM_REGISTRY_HEADER}"', text)
        self.assertIn('add_dependencies(uwvm-ros1-gateway ${catkin_EXPORTED_TARGETS})', text)

    def test_official_fixture_and_limited_claim(self):
        text = source('tests/ros_custom_types/ros1/uwvm_custom_ros1_fixture/msg/Packet.msg')
        fields = [line for line in text.splitlines() if line and not line.startswith('#')]
        self.assertEqual(fields, ['int32 sequence', 'bool active', 'float64 measurement', 'uint8[4] tag', 'int16[3] samples'])
        package = etree.fromstring(source('tests/ros_custom_types/ros1/uwvm_custom_ros1_fixture/package.xml'))
        self.assertEqual(package.findtext('name'), 'uwvm_custom_ros1_fixture')
        self.assertEqual(package.findtext('build_depend'), 'message_generation')
        self.assertEqual(package.findtext('exec_depend'), 'message_runtime')
        catalog = json.loads(source('tests/ros_custom_types/ros1/catalog.schema.json'))
        self.assertNotIn('native_identity', catalog['types'][0])
        self.assertEqual(catalog['types'][0]['type'], 'uwvm_custom_ros1_fixture/Packet')
        guide = source('tests/ros_custom_types/ros1/README.md')
        self.assertIn('It does not implement custom', guide)
        self.assertIn('Do not compile or execute it locally', guide)
        fixture = source('tests/ros_custom_types/ros1/native_topics.cc')
        self.assertIn('PASS ROS1 fixed custom topic native binding exact_body=23', fixture)
        self.assertIn('does not authenticate an IPC session', guide)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=pathlib.Path, required=True)
    args = parser.parse_args()
    suite = unittest.defaultTestLoader.loadTestsFromTestCase(Review)
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    pins = []
    for relative in OWNED:
        bytes_ = (ROOT / relative).read_bytes()
        pins.append({'path': relative, 'bytes': len(bytes_), 'sha256': hashlib.sha256(bytes_).hexdigest()})
    report = {'status': 'SOURCE_AND_MODEL_PASS' if result.wasSuccessful() else 'FAIL',
              'scope': 'ROS1 fixed custom topic source and pure admission/wire models only',
              'methods_run': result.testsRun, 'source_inputs': pins,
              'native_compilation': 'NOT_RUN', 'native_execution': 'NOT_RUN',
              'gateway_ipc_execution': 'NOT_RUN', 'wasm_execution': 'NOT_RUN',
              'original_G01_complete': False}
    with args.out.open('x', encoding='utf-8') as file:
        json.dump(report, file, indent=2); file.write('\n')
    return 0 if result.wasSuccessful() else 1


if __name__ == '__main__':
    raise SystemExit(main())
