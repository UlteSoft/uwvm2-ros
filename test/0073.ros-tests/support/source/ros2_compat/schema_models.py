#!/usr/bin/env python3
"""Independently compare generated legacy declarations to official native exports.

Run in the unified serial Linux cgroup phase with Python -B. This suite imports
only the trusted source generator and reads immutable, previously exported Jazzy
metadata as golden declaration fixtures. It does not execute a compiler, ROS,
Wasm, QEMU or a native library. It cannot establish current native SDK behavior.
All temporary source-generation output, if added, must remain under /tmp.
"""
from __future__ import annotations
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import sys
import unittest

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[5]
SCALARS = {'bool': 15, 'uint8': 3, 'int8': 2, 'uint16': 5, 'int16': 4,
           'uint32': 7, 'int32': 6, 'uint64': 9, 'int64': 8,
           'float32': 10, 'float64': 11, 'string': 17, 'char': 13, 'byte': 16}


def load(path, name):
    """Load only this checkout's fixed generator entry, never a catalog module."""
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def description(schema, root):
    """Independent REP-2011 JSON oracle; no native identity is issued by it.

    Compare this test value to a real official exporter identity. Member order
    is preserved, references are sorted by full type name, default_value is
    omitted, and all FieldType keys retain the official insertion order.
    """
    def record(name, fields):
        members = []
        for field, type_, length, array, string_bound, sequence, capacity in fields:
            scalar = SCALARS.get(type_, 1)
            if type_ == 'string' and string_bound:
                scalar = 21
            scalar += 48 if array else (96 if capacity else 144) if sequence else 0
            members.append({'name': field, 'type': {'type_id': scalar,
                            'capacity': length if array else capacity if sequence else 0,
                            'string_capacity': string_bound,
                            'nested_type_name': '' if type_ in SCALARS else type_}})
        return {'type_name': name, 'fields': members}
    records = dict(schema)
    return {'type_description': record(root, records.pop(root)),
            'referenced_type_descriptions': [record(name, fields) for name, fields in sorted(records.items())]}


def digest(schema, root):
    text = json.dumps(description(schema, root), ensure_ascii=True,
                      separators=(', ', ': '), sort_keys=False)
    return 'RIHS01_' + hashlib.sha256(text.encode('utf-8')).hexdigest()


def exported_description(value):
    """Normalize the independent native exporter without consulting the generator."""
    def record(row):
        return {'type_name': row['type'], 'fields': [
            {'name': field['name'], 'type': {
                'type_id': field['type_id'], 'capacity': field['capacity'],
                'string_capacity': field['string_capacity'],
                'nested_type_name': field['nested_type_name']}}
            for field in row['fields']]}
    return {'type_description': record(value['root']),
            'referenced_type_descriptions': [record(row) for row in sorted(value['references'], key=lambda row: row['type'])]}


class CompleteSchemaModels(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.parent = load(ROOT / 'sdk/origin/tools/custom_types.py', 'ros2_compat_schema_parent')
        cls.profile = load(ROOT / 'sdk/origin/custom-types/bounded_profile.py', 'ros2_compat_schema_profile')
        derived = ROOT / 'test/0073.ros-tests/support/source/ros_custom_types/deployment-final/derivation'
        cls.records, _, interfaces, _ = cls.profile.load_catalog(cls.parent, derived / 'bound2/catalog.native.json')
        cls.interfaces = {item.kind + ':' + item.name.rsplit('/', 1)[-1]: item for item in interfaces}
        cls.official = json.loads((derived / 'official-export-2.json').read_text())['types']

    def schema(self, key):
        interface = self.interfaces[key]
        return self.profile.ros2_interface_schema(self.parent, self.records, interface), interface

    def test_all_protocol_fields_equal_official_native_export(self):
        for key in ('service:Echo', 'service:Empty', 'action:Digest'):
            with self.subTest(interface=key):
                schema, interface = self.schema(key)
                self.assertEqual(description(schema, interface.name), exported_description(self.official[interface.name]['official']))

    def test_complete_rihs_equals_all_three_official_native_roots(self):
        for key in ('service:Echo', 'service:Empty', 'action:Digest'):
            with self.subTest(interface=key):
                schema, interface = self.schema(key)
                self.assertEqual(digest(schema, interface.name), self.official[interface.name]['official']['native_identity'])

    def test_service_root_contains_event_and_singleton_bounded_envelopes(self):
        schema, interface = self.schema('service:Echo')
        records = dict(schema)
        self.assertEqual([row[0] for row in records[interface.name]], ['request_message', 'response_message', 'event_message'])
        event = records[interface.name + '_Event']
        self.assertEqual([row[0] for row in event], ['info', 'request', 'response'])
        self.assertTrue(all(row[5:] == (True, 1) for row in event[1:]))

    def test_action_root_contains_all_six_official_protocol_references(self):
        schema, interface = self.schema('action:Digest')
        records = dict(schema)
        self.assertEqual([row[0] for row in records[interface.name]],
                         ['goal', 'result', 'feedback', 'send_goal_service', 'get_result_service', 'feedback_message'])
        self.assertEqual(len(schema), 18)
        for suffix in ('_SendGoal', '_GetResult'):
            self.assertEqual(len(records[interface.name + suffix]), 3)
            self.assertEqual(len(records[interface.name + suffix + '_Event']), 3)
        self.assertIn('unique_identifier_msgs/msg/UUID', records)

    def test_char_msg_field_uses_official_uint8_idl_mapping(self):
        schema, _ = self.schema('service:Empty')
        gid = dict(schema)['service_msgs/msg/ServiceEventInfo'][2]
        self.assertEqual(gid, ('client_gid', 'uint8', 16, True, 0, False, 0))

    def test_empty_components_keep_official_representation_member(self):
        schema, interface = self.schema('service:Empty')
        records = dict(schema)
        placeholder = [('structure_needs_at_least_one_member', 'uint8', 0, False, 0, False, 0)]
        self.assertEqual(records[interface.name + '_Request'], placeholder)
        self.assertEqual(records[interface.name + '_Response'], placeholder)

    def test_omitted_event_is_a_different_identity(self):
        schema, interface = self.schema('service:Echo')
        changed = copy.deepcopy(schema)
        changed[0] = (changed[0][0], changed[0][1][:2])
        self.assertNotEqual(digest(changed, interface.name), self.official[interface.name]['official']['native_identity'])

    def test_action_three_payload_projection_is_a_different_identity(self):
        schema, interface = self.schema('action:Digest')
        changed = copy.deepcopy(schema)
        changed[0] = (changed[0][0], changed[0][1][:3])
        self.assertNotEqual(digest(changed, interface.name), self.official[interface.name]['official']['native_identity'])

    def test_member_order_changes_identity(self):
        schema, interface = self.schema('service:Echo')
        changed = copy.deepcopy(schema)
        changed[0] = (changed[0][0], list(reversed(changed[0][1])))
        self.assertNotEqual(digest(changed, interface.name), digest(schema, interface.name))

    def test_reference_order_is_canonical(self):
        schema, interface = self.schema('action:Digest')
        self.assertEqual(digest(list(reversed(schema)), interface.name), digest(schema, interface.name))

    def test_protocol_schema_excludes_unrelated_catalog_interfaces(self):
        schema, _ = self.schema('service:Empty')
        self.assertEqual(len(schema), 6)
        self.assertFalse(any('/Echo' in name or '/Digest' in name or '/Envelope' in name for name, _ in schema))

    def test_message_cannot_be_projected_as_a_service_or_action(self):
        with self.assertRaises(self.parent.GenerationError):
            self.schema('message:Point')


if __name__ == '__main__':
    unittest.main()
