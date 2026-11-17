"""Adversarial design-ledger checks; none are native-provider qualification."""
import copy
import importlib.util
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('operation_design_check', ROOT/'tools/ros_scene/check_operation_design.py')
check = importlib.util.module_from_spec(spec)
spec.loader.exec_module(check)


class OperationDesign(unittest.TestCase):
    def setUp(self):
        self.inputs = [check.load(ROOT/name) for name in check.INPUTS]

    def reject(self, change):
        values = copy.deepcopy(self.inputs)
        change(*values)
        with self.assertRaises(ValueError):
            check.audit(*values)

    def test_complete_inventory_does_not_claim_execution(self):
        report = check.audit(*self.inputs)
        self.assertEqual((report['operations'], report['domains'], report['typed_profile_fields']), (162, 26, 62))
        self.assertEqual((report['registered'], report['planned']), (162, 0))
        self.assertTrue(report['design_inventory_complete'])
        self.assertTrue(report['registered_interfaces_complete'])
        for field in ['providers_complete', 'native_equivalence_proven', 'package_portability_proven', 'industrial_assurance_proven']:
            self.assertFalse(report[field])

    def test_missing_duplicate_extra_operations(self):
        self.reject(lambda c, d, r: d['operations'].pop())
        self.reject(lambda c, d, r: d['operations'].append(copy.deepcopy(d['operations'][0])))
        self.reject(lambda c, d, r: d['operations'][0].update(operation='made.up'))
        self.reject(lambda c, d, r: c['operations'].append(copy.deepcopy(c['operations'][0])))

    def test_authority_and_domain_drift(self):
        for field, value in [('authority_action', 'control.submit'), ('secondary_actions', ['device.stop']),
            ('primary_domain', 0), ('related_domains', [1, 1]), ('wire_source', 'another.json#abi'),
            ('semantics_source', 'another.json#abi'), ('behavior', 'inferred'), ('provider_contract', 'invented')]:
            with self.subTest(field=field):
                self.reject(lambda c, d, r: d['operations'][0].update({field: value}))
        self.reject(lambda c, d, r: r['domains'].pop())
        self.reject(lambda c, d, r: r['domains'][0]['operations'].append('unknown.operation'))
        self.reject(lambda c, d, r: r['domains'][25]['operations'].append('abi'))

    def test_no_empty_failure_or_safety_contracts(self):
        self.reject(lambda c, d, r: d['behaviors']['robot_provider'].pop('copy_failure'))
        self.reject(lambda c, d, r: d['common'].pop('physical_execution'))
        self.reject(lambda c, d, r: d['common'].update(allocation=''))
        self.reject(lambda c, d, r: r['operations']['device.submit']['inherits'].remove('stop'))
        self.reject(lambda c, d, r: r['operations']['device.submit'].update(effect=''))
        self.reject(lambda c, d, r: r['operations']['device.stop'].update(sequence_domain='command'))
        self.reject(lambda c, d, r: r['operations']['control.stop'].update(sequence_domain='cycle_write'))
        self.reject(lambda c, d, r: r['operations']['media.close'].update(sequence_domain='frame'))

    def test_profile_shapes_are_closed_and_bounded(self):
        self.reject(lambda c, d, r: r['families']['device']['profile_field_types'].pop('receiver_fencing'))
        self.reject(lambda c, d, r: r['families']['device']['profile_field_types'].update(protocol='arbitrary_native_pointer'))
        self.reject(lambda c, d, r: r['families']['component']['capacity_fields'].remove('stop_slots'))
        self.reject(lambda c, d, r: r['families']['component']['capacity_fields'].append('processes'))
        self.reject(lambda c, d, r: r['deployment_types']['u32'].update(rule=''))
        self.reject(lambda c, d, r: r['deployment_types']['profile_ref']['fields'].append(['identity', 'nonempty_utf8']))
        self.reject(lambda c, d, r: r['deployment_types']['profile_ref']['fields'].append(['recursive', 'profile_ref']))
        self.reject(lambda c, d, r: r['deployment_types']['effect_class'].update(enum=[]))
        self.reject(lambda c, d, r: r['deployment_types']['effect_class'].update(native_callback='permitted'))

    def test_unknown_cannot_become_success(self):
        self.reject(lambda c, d, r: r['reply_effect_matrix']['succeeded'].append('unknown'))
        self.reject(lambda c, d, r: r['reply_effect_matrix']['outcome_unknown'].append('none'))
        self.reject(lambda c, d, r: r['reply_effect_matrix'].pop('cancelled'))
        self.reject(lambda c, d, r: r['reply_effect_matrix']['failed'].append('unrecognized'))

    def test_duplicate_json_never_uses_last_value(self):
        for text in ['{"capacity":0,"capacity":999999}', '{"x":{"action":"read","action":"write"}}']:
            with self.assertRaises(ValueError):
                json.loads(text, object_pairs_hook=check.unique_object)


if __name__ == '__main__':
    if Path('/proc/self/cgroup').read_text().strip() != '0::/' + str(check.GROUP.relative_to('/sys/fs/cgroup')):
        raise SystemExit('must use existing 64 GiB cgroup')
    if (check.GROUP/'memory.max').read_text().strip() != '68719476736' or (check.GROUP/'memory.swap.max').read_text().strip() != '0':
        raise SystemExit('test gate changed')
    unittest.main()
