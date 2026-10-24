#!/usr/bin/env python3
"""Twenty-four DATA-only regressions for the explicit Direct3 profile admission.

The guarded Linux owner must load both this file and ``direct3_profile.py`` from
its complete frozen manifest, then call ``suite(rules)`` under its normal current
64-GiB/zero-swap source/root guard. This module deliberately does not discover or
import candidate project code. Its synthetic timing/state rows are policy
fixtures; passing this suite cannot qualify ROS, MuJoCo, PMU counts or speed.
"""
import copy
import unittest


def fixture(rules):
    """Model exactly the maintained producer's three-case DATA shape."""
    rows = []
    for language in rules.LANGUAGES:
        controllers = {role: {'steps': 32, 'polls': 32,
            'actual_physics_states': [{'step': step} for step in range(32)],
            'guest_callbacks': [{'step': step} for step in range(32)],
            'raw_control_roundtrip_ns': [value] * 32} for role, value in (('native', 10), ('wasm', 30))}
        selected = {'language': language, 'steps': 32, 'warmup_steps': 2,
                    'slowdown_trigger': 2, 'controller_mode': 'go2-stand', 'modular': False, 'libraries': []}
        actual = {'Guest_language': language, 'controller_mode': 'go2-stand',
            'actual_matched_controllers': controllers,
            'default_denial_witness': {'kind': 'quiet', 'native_dds_commands': 0,
                'actual_mujoco_steps': 0, 'samples': 30, 'elapsed_ns': 200000000},
            'performance_sample_qualification': 'AT_LEAST_30_PER_ROLE',
            'control_roundtrip_statistics': {role: rules.statistics(value['raw_control_roundtrip_ns'][2:])
                for role, value in controllers.items()},
            'slowdown_trigger': 2, 'slowdown_p50': 3, 'profiling_required': True}
        rows.append({'language': language, 'status': 'PASS_ACTUAL_GUEST_NATIVE_DDS_MUJOCO',
            'derived_selection': selected, 'actual_control': actual, 'cleanup': {'status': 'PASS', 'remaining': []}})
    return {'kind': rules.BASELINE_KIND, 'status': rules.BASELINE_STATUS,
        'physical_robot_access': False, 'default_ROS_authority': [], 'optional_WASI_rights': [],
        'source_preserved': True, 'input_pins_preserved': True, 'whole_roots_preserved': True,
        'cases': rows, 'actual_matched_roles': 6, 'actual_default_denied_witnesses': 3,
        'actual_controlled_physics_steps': 192, 'profiling_required': True}


def adaptation(rules):
    """Minimal immutable config plus the explicit four actual profile labels."""
    original = {'format_version': 1, 'cgroup': {'actual': 7}, 'environment': {'LANG': 'C.UTF-8'},
        'limits': {'jobs': 1}, 'upstreams': {}, 'simulation': {'model': 'same'},
        'benchmark': None, 'profiler': None, 'input_roots': ['/immutable/worker'],
        'inputs': {'physics_selection': {'path': '/old/controls/physics.json', 'bytes': 1, 'sha256': 'a' * 64},
                   'worker': {'path': '/immutable/worker/run', 'bytes': 1, 'sha256': 'b' * 64}}}
    pins = {label: {'path': path, 'bytes': 1, 'sha256': str(index) * 64}
            for index, (label, path) in enumerate(zip(rules.INPUT_LABELS,
                ('/old/result/report.json', '/old/controls/config.json', '/old/controls/physics.json', '/vtune/bin64/vtune')), 1)}
    current = copy.deepcopy(original); current['profiler'] = {'explicit': True}
    current['inputs'].update({label: {key: pin[key] for key in ('path', 'bytes', 'sha256')} for label, pin in pins.items()})
    current['inputs']['physics_selection'] = {'path': '/new/controls/physics.json', 'bytes': 2, 'sha256': 'e' * 64}
    current['input_roots'] = rules.adapted_roots(original, ['/vtune'], pins, '/new/controls')
    return original, current, pins


def suite(rules):
    """Return explicit tests against the owner's already admitted pure module."""
    class Policy(unittest.TestCase):
        def test_select_all_three_without_mutation(self):
            value = fixture(rules); before = copy.deepcopy(value)
            for index, language in enumerate(rules.LANGUAGES):
                self.assertIs(rules.select_case(value, language), value['cases'][index])
            self.assertEqual(value, before)

        def test_real_producer_top_cleanup_is_optional_but_cases_are_not(self):
            value = fixture(rules); rules.select_case(value, 'c')
            del value['cases'][0]['cleanup']
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_failed_running_or_old_kind_cannot_promote(self):
            for key, text in (('status', 'FAIL'), ('status', 'RUNNING'), ('kind', 'unitree-actual-mujoco-ros-simulation')):
                value = fixture(rules); value[key] = text
                with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_other_language_slowdown_is_not_authority(self):
            value = fixture(rules); value['cases'][0]['actual_control']['profiling_required'] = False
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_twenty_nine_samples_are_insufficient(self):
            value = fixture(rules); row = value['cases'][0]
            row['derived_selection']['steps'] = 31
            for controller in row['actual_control']['actual_matched_controllers'].values():
                controller['steps'] = 31
                for name in ('actual_physics_states', 'guest_callbacks', 'raw_control_roundtrip_ns'): controller[name].pop()
            value['actual_controlled_physics_steps'] -= 2
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_summary_cannot_replace_actual_raw_samples(self):
            value = fixture(rules); value['cases'][0]['actual_control']['control_roundtrip_statistics']['wasm']['mean_ns'] = 999
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_raw_samples_cannot_change_under_summary(self):
            value = fixture(rules); value['cases'][0]['actual_control']['actual_matched_controllers']['wasm']['raw_control_roundtrip_ns'][2] = 900
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_quiet_denial_must_have_zero_real_effects(self):
            for key in ('native_dds_commands', 'actual_mujoco_steps'):
                value = fixture(rules); value['cases'][0]['actual_control']['default_denial_witness'][key] = 1
                with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_missing_language_or_role_fails_closed(self):
            value = fixture(rules); value['cases'].pop()
            with self.assertRaises(ValueError): rules.select_case(value, 'c')
            value = fixture(rules); del value['cases'][0]['actual_control']['actual_matched_controllers']['native']
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_preservation_is_required_for_every_scope(self):
            for key in ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved'):
                value = fixture(rules); value[key] = False
                with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_cleanup_cannot_retain_a_live_child(self):
            value = fixture(rules); value['cases'][1]['cleanup']['remaining'] = [{'pid': 123}]
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_only_actual_namespace_and_four_cp_paths_may_change(self):
            original = {'expected_network_namespace': 'net:[1]', 'model': 'same',
                'cp310': {'environment': {**{key: '/old/' + key for key in rules.MUTABLE_CP_PATHS}, 'MUJOCO_GL': 'osmesa'}}}
            current = copy.deepcopy(original); current['expected_network_namespace'] = 'net:[2]'
            for key in rules.MUTABLE_CP_PATHS: current['cp310']['environment'][key] = '/new/' + key
            rules.same_physics(original, current)
            current['cp310']['environment']['MUJOCO_GL'] = 'egl'
            with self.assertRaises(ValueError): rules.same_physics(original, current)

        def test_model_and_grants_are_immutable(self):
            original = {'expected_network_namespace': 'net:[1]', 'model': 'same', 'grants': [],
                'cp310': {'environment': {key: '/old/' + key for key in rules.MUTABLE_CP_PATHS}}}
            for key, value in (('model', 'other'), ('grants', ['new'])):
                current = copy.deepcopy(original); current[key] = value
                with self.assertRaises(ValueError): rules.same_physics(original, current)

        def test_exact_profile_adaptation_keeps_original_inputs(self):
            old, new, pins = adaptation(rules); before = copy.deepcopy(old)
            rules.exact_adaptation(old, new, ['/vtune'], pins, '/new/controls')
            self.assertEqual(old, before)

        def test_unselected_roots_or_extra_input_are_refused(self):
            old, new, pins = adaptation(rules); new['input_roots'].append('/unselected')
            with self.assertRaises(ValueError): rules.exact_adaptation(old, new, ['/vtune'], pins, '/new/controls')
            old, new, pins = adaptation(rules); new['inputs']['new-authority'] = pins[rules.INPUT_LABELS[0]]
            with self.assertRaises(ValueError): rules.same_configuration(old, new)

        def test_original_model_input_or_profile_pin_cannot_change(self):
            old, new, pins = adaptation(rules); new['simulation']['model'] = 'other'
            with self.assertRaises(ValueError): rules.exact_adaptation(old, new, ['/vtune'], pins, '/new/controls')
            old, new, pins = adaptation(rules); new['inputs']['direct3-profile-vtune']['sha256'] = 'f' * 64
            with self.assertRaises(ValueError): rules.exact_adaptation(old, new, ['/vtune'], pins, '/new/controls')
        def test_required_hardware_profiles_fast_actual_baseline_without_mutation(self):
            value = fixture(rules)
            for row in value['cases']:
                actual = row['actual_control']
                actual['actual_matched_controllers']['wasm']['raw_control_roundtrip_ns'] = [10] * 32
                actual['control_roundtrip_statistics']['wasm'] = rules.statistics([10] * 30)
                actual['slowdown_p50'] = 1
                actual['profiling_required'] = False
            value['profiling_required'] = False
            before = copy.deepcopy(value)
            for index, language in enumerate(rules.LANGUAGES):
                self.assertIs(rules.select_case(value, language, collection_policy=rules.REQUIRED_HARDWARE_POLICY), value['cases'][index])
            self.assertEqual(value, before)

        def test_default_and_explicit_slowdown_policies_still_reject_valid_fast_case(self):
            value = fixture(rules); actual = value['cases'][0]['actual_control']
            actual['actual_matched_controllers']['wasm']['raw_control_roundtrip_ns'] = [10] * 32
            actual['control_roundtrip_statistics']['wasm'] = rules.statistics([10] * 30)
            actual['slowdown_p50'] = 1; actual['profiling_required'] = False
            for policy in (rules.SLOWDOWN_POLICY,):
                with self.assertRaises(ValueError): rules.select_case(value, 'c', collection_policy=policy)
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_required_hardware_keeps_exact_threshold_and_false_original_flag(self):
            value = fixture(rules); actual = value['cases'][0]['actual_control']
            actual['actual_matched_controllers']['wasm']['raw_control_roundtrip_ns'] = [20] * 32
            actual['control_roundtrip_statistics']['wasm'] = rules.statistics([20] * 30)
            actual['slowdown_p50'] = 2; actual['profiling_required'] = False
            rules.select_case(value, 'c', collection_policy=rules.REQUIRED_HARDWARE_POLICY)
            with self.assertRaises(ValueError): rules.select_case(value, 'c')

        def test_required_hardware_does_not_accept_inconsistent_original_slowdown_flag(self):
            value = fixture(rules); value['cases'][0]['actual_control']['profiling_required'] = False
            with self.assertRaises(ValueError): rules.select_case(value, 'c', collection_policy=rules.REQUIRED_HARDWARE_POLICY)
            value = fixture(rules); actual = value['cases'][0]['actual_control']
            actual['actual_matched_controllers']['wasm']['raw_control_roundtrip_ns'] = [10] * 32
            actual['control_roundtrip_statistics']['wasm'] = rules.statistics([10] * 30)
            actual['slowdown_p50'] = True; actual['profiling_required'] = False
            with self.assertRaises(ValueError): rules.select_case(value, 'c', collection_policy=rules.REQUIRED_HARDWARE_POLICY)

        def test_required_hardware_does_not_bypass_failed_or_unpreserved_baseline(self):
            for key, item in (('status', 'FAIL'), ('source_preserved', False), ('whole_roots_preserved', False)):
                value = fixture(rules); value[key] = item
                with self.assertRaises(ValueError): rules.select_case(value, 'c', collection_policy=rules.REQUIRED_HARDWARE_POLICY)
            value = fixture(rules); value['cases'][1]['cleanup']['remaining'] = [{'pid': 123}]
            with self.assertRaises(ValueError): rules.select_case(value, 'c', collection_policy=rules.REQUIRED_HARDWARE_POLICY)

        def test_required_hardware_still_requires_real_samples_and_original_summaries(self):
            value = fixture(rules); value['cases'][0]['actual_control']['control_roundtrip_statistics']['wasm']['mean_ns'] = 9
            with self.assertRaises(ValueError): rules.select_case(value, 'c', collection_policy=rules.REQUIRED_HARDWARE_POLICY)
            value = fixture(rules); value['cases'][0]['actual_control']['performance_sample_qualification'] = 'INSUFFICIENT'
            with self.assertRaises(ValueError): rules.select_case(value, 'c', collection_policy=rules.REQUIRED_HARDWARE_POLICY)

        def test_unknown_nonstring_or_null_policy_is_not_explicit_authority(self):
            for value in (None, True, 1, [], {}, '', 'required', 'software-fallback'):
                with self.assertRaises(ValueError): rules.profile_policy({'collection_policy': value})
                with self.assertRaises(ValueError): rules.select_case(fixture(rules), 'c', collection_policy=value)

        def test_policy_omission_preserves_original_default_and_recipe(self):
            recipe = {'language': 'cpp', 'roots': ['/vtune'], 'seconds': 30}; before = copy.deepcopy(recipe)
            self.assertEqual(rules.profile_policy(recipe), rules.SLOWDOWN_POLICY)
            self.assertEqual(recipe, before)
            self.assertEqual(rules.profile_policy({'collection_policy': rules.REQUIRED_HARDWARE_POLICY}), rules.REQUIRED_HARDWARE_POLICY)
    return unittest.defaultTestLoader.loadTestsFromTestCase(Policy)
