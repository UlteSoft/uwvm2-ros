#!/usr/bin/env python3
"""Closed DATA predicates for measured Direct3 profiling; no process or import.

This module never creates a ROS entity, edits a historical report, predicts a
namespace, obtains a hardware privilege or converts profiling into performance
acceptance. The owning entry must bind these bytes to its complete manifest.
"""
import copy
import math
from pathlib import PurePosixPath

LANGUAGES = ('c', 'cpp', 'javascript')
BASELINE_KIND = 'unitree-three-direct-Guest-controlled-physics'
BASELINE_STATUS = 'PASS_THREE_ACTUAL_STANDALONE_GUEST_NATIVE_DDS_MUJOCO_CONTROL_CASES'
PROFILE_KIND = 'unitree-direct3-vtune-hardware-profiles'
PROFILE_STATUS = 'PASS_DIRECT3_SELECTED_WASM_ACTUAL_HARDWARE_PROFILES_NOT_SPEED_QUALIFIED'
INPUT_LABELS = ('direct3-profile-baseline', 'direct3-profile-config',
                'direct3-profile-selection', 'direct3-profile-vtune')
SLOWDOWN_POLICY = 'measured-slowdown'
REQUIRED_HARDWARE_POLICY = 'required-hardware'
COLLECTION_POLICIES = (SLOWDOWN_POLICY, REQUIRED_HARDWARE_POLICY)
MUTABLE_CP_PATHS = ('HOME', 'TMPDIR', 'XDG_CACHE_HOME', 'XDG_CONFIG_HOME')


def need(value, reason):
    if not value:
        raise ValueError(reason)


def clean(value, require_cleanup=True):
    """Success requires original normal cleanup; a partial/failed row stays so."""
    need(not value.get('error') and not value.get('errors') and
         not value.get('finalization_errors'), 'failed or incomplete Direct3 evidence refused')
    rows = value.get('cleanup')
    if rows is None and not require_cleanup:
        # The maintained Direct3 coordinator closes/removes each owner and
        # retains its cleanup in cases[]. It has no remaining top-level actor.
        return
    rows = [rows] if type(rows) is dict else rows
    need(type(rows) is list and rows and all(type(row) is dict and row.get('status') == 'PASS' and
         not row.get('remaining') and not row.get('errors') for row in rows),
         'actual normal owned cleanup is required before profiling')


def statistics(values):
    """Recompute the existing nearest-rank summary from actual retained samples."""
    need(type(values) is list and 30 <= len(values) <= 32 and
         all(type(value) is int and value > 0 for value in values),
         'at least thirty actual positive post-warmup samples required')
    ordered = sorted(values)
    return {'count': len(values), 'p50_ns': ordered[math.ceil(.50 * len(values)) - 1],
            'p95_ns': ordered[math.ceil(.95 * len(values)) - 1],
            'p99_ns': ordered[math.ceil(.99 * len(values)) - 1],
            'min_ns': ordered[0], 'max_ns': ordered[-1], 'mean_ns': sum(values) / len(values)}


def profile_policy(recipe):
    """Return the explicit host recipe policy; omission retains the old gate."""
    need(type(recipe) is dict, 'actual explicit profiling recipe required')
    value = recipe.get('collection_policy', SLOWDOWN_POLICY)
    need(type(value) is str and value in COLLECTION_POLICIES,
         'collection_policy must be measured-slowdown or explicit required-hardware')
    return value


def select_case(report, language, *, collection_policy=SLOWDOWN_POLICY):
    """Admit an unchanged measured baseline under an explicit host policy.

    All original successful cases, samples, physics observations and cleanup
    remain mandatory. The default selects only a genuinely slow language.
    Required hardware profiling is a separate explicit replay request and
    never rewrites the baseline's measured slowdown or performance flags.
    """
    profile_policy({'collection_policy': collection_policy})
    modular=report.get('kind')=='unitree-three-modular-direct-Guest-controlled-physics'
    expected_status=('PASS_THREE_ACTUAL_MODULAR_GUEST_NATIVE_DDS_MUJOCO_CONTROL_CASES' if modular else BASELINE_STATUS)
    need(language in LANGUAGES and report.get('kind') in (BASELINE_KIND,'unitree-three-modular-direct-Guest-controlled-physics') and
         report.get('status') == expected_status and report.get('physical_robot_access') is False and
         report.get('default_ROS_authority') == report.get('optional_WASI_rights') == [] and
         all(report.get(key) is True for key in ('source_preserved', 'input_pins_preserved', 'whole_roots_preserved')),
         'actual complete preserved Direct3 baseline required')
    clean(report, require_cleanup=False)
    cases = report.get('cases')
    need(type(cases) is list and [row.get('language') for row in cases] == list(LANGUAGES) and
         report.get('actual_matched_roles') == 6 and report.get('actual_default_denied_witnesses') == 3,
         'all three genuine Direct3 cases and six actual controller roles required')
    total = 0
    for row in cases:
        need(row.get('status') == 'PASS_ACTUAL_GUEST_NATIVE_DDS_MUJOCO', 'unfinished baseline language refused')
        clean(row)
        selected = row['derived_selection']; actual = row['actual_control']
        steps, warmup = selected['steps'], selected['warmup_steps']
        need(type(steps) is int and 1 <= steps <= 32 and type(warmup) is int and
             0 <= warmup <= 2 and warmup < steps and selected['language'] == row['language'] and
             selected['controller_mode'] == 'go2-stand' and selected['modular'] is modular and
             ((not modular and selected['libraries']==[]) or
              (modular and type(selected['libraries']) is list and
               [item.get('name') for item in selected['libraries']]==['callbacks','motion','perception'] and
               all(type(item) is dict and set(item)=={'name','guest_input','build_input'} for item in selected['libraries']))) and
             type(actual.get('profiling_required')) is bool and
             actual.get('Guest_language') == row['language'] and actual.get('controller_mode') == 'go2-stand' and
             set(actual['actual_matched_controllers']) == {'native', 'wasm'}, 'original actual standalone control selection required')
        quiet = actual.get('default_denial_witness', {})
        need(quiet.get('kind') == 'quiet' and quiet.get('native_dds_commands') == quiet.get('actual_mujoco_steps') == 0 and
             quiet.get('samples', 0) >= 30 and quiet.get('elapsed_ns', 0) >= 200000000,
             'original independently observed default-denied quiet interval required')
        for role, controller in actual['actual_matched_controllers'].items():
            need(controller.get('steps') == steps and len(controller.get('actual_physics_states', [])) == steps and
                 len(controller.get('guest_callbacks', [])) == steps and
                 len(controller.get('raw_control_roundtrip_ns', [])) == steps and
                 type(controller.get('polls')) is int and steps <= controller['polls'] <= 4096,
                 'counted baseline steps need actual state/callback/timing observations')
            total += steps
    if modular:
        need(all(row['derived_selection']['libraries']==cases[0]['derived_selection']['libraries'] for row in cases),
             'all measured languages must use the exact same actual provider graph')
    need(report.get('actual_controlled_physics_steps') == total and
         report.get('profiling_required') is any(row['actual_control']['profiling_required'] for row in cases),
         'original aggregate physics/profiling inventory differs')
    row = next(row for row in cases if row['language'] == language)
    actual, selected = row['actual_control'], row['derived_selection']
    need(actual.get('performance_sample_qualification') == 'AT_LEAST_30_PER_ROLE',
         'insufficient baseline samples cannot select hardware profiling')
    for role, controller in actual['actual_matched_controllers'].items():
        expected = statistics(controller['raw_control_roundtrip_ns'][selected['warmup_steps']:])
        need(actual['control_roundtrip_statistics'][role] == expected, 'baseline timing summary differs from actual samples')
    ratio = actual['control_roundtrip_statistics']['wasm']['p50_ns'] / actual['control_roundtrip_statistics']['native']['p50_ns']
    trigger = selected['slowdown_trigger']
    need(type(trigger) in (int, float) and math.isfinite(trigger) and 1 <= trigger <= 10 and
         actual.get('slowdown_trigger') == trigger and type(actual.get('slowdown_p50')) in (int, float) and
         math.isfinite(actual['slowdown_p50']) and actual['slowdown_p50'] == ratio and
         actual.get('profiling_required') is (ratio > trigger),
         'original selected timing ratio/trigger/profiling flag must match its actual samples')
    need(collection_policy == REQUIRED_HARDWARE_POLICY or ratio > trigger,
         'measured-slowdown policy requires this language to exceed its original trigger')
    return row


def same_physics(original, current):
    """Permit only a fresh actual namespace and the four fresh owned CP paths."""
    old, new = copy.deepcopy(original), copy.deepcopy(current)
    old.pop('expected_network_namespace'); new.pop('expected_network_namespace')
    for value in (old, new):
        for key in MUTABLE_CP_PATHS:
            value['cp310']['environment'].pop(key)
    need(old == new, 'model/Worker/Gateway/Guest/view/measurement selection changed during profiling')


def same_configuration(original, current):
    """Keep immutable workload fields and all original non-selection inputs.

    Extra immutable roots are restricted to the pinned baseline metadata and
    explicitly selected VTune package by the Root stager/owner. The caller also
    reopens both original metadata files and every old/current physical pin.
    """
    need(original.get('benchmark') is original.get('profiler') is None and
         type(current.get('profiler')) is dict and current.get('benchmark') is None,
         'baseline stays unprofiled; only an explicit profiling recipe may opt in')
    for key in ('format_version', 'cgroup', 'environment', 'limits', 'upstreams', 'simulation', 'benchmark'):
        need(original[key] == current[key], 'immutable baseline configuration field changed: ' + key)
    need(current['input_roots'][:len(original['input_roots'])] == original['input_roots'], 'original complete root selections changed')
    need(set(current['inputs']) == set(original['inputs']) | set(INPUT_LABELS) and
         not set(INPUT_LABELS) & set(original['inputs']), 'only four explicit profiling inputs may be added')
    for key, value in original['inputs'].items():
        if key != 'physics_selection':
            need(current['inputs'][key] == value, 'original physical workload input changed: ' + key)


def adapted_roots(original, recipe_roots, actual_pins, runtime_controls=None):
    """Derive the finite complete-root additions without any filesystem lookup.

    Original roots retain their order. Only selected VTune/provider roots,
    parents of three actual retained metadata files, and the fresh owner-created
    runtime-control directory are added. The owner separately proves canonical
    physical directories, source/output separation and the complete catalogs.
    """
    need(type(recipe_roots) is list and 1 <= len(recipe_roots) <= 8 and
         len(set(recipe_roots)) == len(recipe_roots) and set(actual_pins) == set(INPUT_LABELS),
         'closed finite profiler roots/four actual selected inputs required')
    values = [*original['input_roots'], *recipe_roots,
              *[str(PurePosixPath(actual_pins[label]['path']).parent) for label in INPUT_LABELS[:-1]]]
    if runtime_controls is not None:
        values.append(runtime_controls)
    roots = list(dict.fromkeys(values))
    need(len(roots) <= (64 if runtime_controls is not None else 63) and
         all(type(value) is str and value.startswith('/') and value != '/' and '..' not in PurePosixPath(value).parts for value in roots),
         'finite explicit complete profile roots exceed scope')
    return roots


def exact_adaptation(original, current, recipe_roots, actual_pins, runtime_controls):
    """Independently recheck stager output at the owning namespace boundary."""
    same_configuration(original, current)
    need(current['input_roots'] == adapted_roots(original, recipe_roots, actual_pins, runtime_controls),
         'profile root additions differ from the exact selected package/metadata/control scope')
    for label in INPUT_LABELS:
        need(current['inputs'][label] == {key: actual_pins[label][key] for key in ('path', 'bytes', 'sha256')},
             'profile metadata/VTune selection differs from its original actual full7')
