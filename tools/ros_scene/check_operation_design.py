#!/usr/bin/env python3
"""Validate the complete operation design and generate its English reference.

Design closure is not provider implementation, native equivalence, platform
qualification or physical assurance. Both reference artifacts say so explicitly.
Run validation/generation in the existing 64 GiB zero-swap cgroup.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
INPUTS = ['tools/ros_scene/interface-catalogue.json', 'tools/ros_scene/operation-contracts.json',
          'tools/ros_scene/robot-provider-contracts.json']
DOCUMENT = 'documents/runtime/ros-operation-reference.md'
HEADER = 'src/uwvm2/imported/ros/scene/operation_design.h'


def require(value, reason):
    if not value:
        raise ValueError(reason)


def unique_object(items):
    result = {}
    for name, value in items:
        require(name not in result, 'duplicate JSON field: ' + name)
        result[name] = value
    return result


def load(path):
    return json.loads(Path(path).read_text(), object_pairs_hook=unique_object)


def audit(catalogue, design, robot):
    operations = {row['name']: row for row in catalogue['operations']}
    require(len(operations) == len(catalogue['operations']), 'duplicate catalogue operation')
    records = {row['operation']: row for row in design['operations']}
    require(len(records) == len(design['operations']) and set(records) == set(operations), 'operation design is not exhaustive')
    require(design['abi_module'] == catalogue['abi_module'] == robot['abi_module'] == 'uwvm_ros_v2', 'ABI identity drift')
    require(catalogue['operational_design'] == 'operation-contracts.json', 'missing catalogue design link')
    require(design['catalogue'] == 'interface-catalogue.json' and design['robot_contract'] == 'robot-provider-contracts.json', 'wrong design inputs')
    require(set(design['common']) == {'default_authority', 'rate_progression', 'origin_closure', 'effect_classification', 'transaction',
        'concurrency', 'physical_execution', 'allocation', 'limits', 'lifecycle', 'observability', 'platform', 'evidence'}, 'incomplete common safety contract')
    require(all(isinstance(v, str) and v.strip() for v in design['common'].values()), 'empty common contract')
    behaviors = design['behaviors']
    for name, rule in behaviors.items():
        require(set(rule) == {'commit', 'copy_failure', 'retry', 'retirement'} and all(rule.values()), 'incomplete behavior: ' + name)
    domains = {d['id']: d for d in robot['domains']}
    require(len(domains) == len(robot['domains']) == 26 and set(domains) == set(range(1, 27)), 'incomplete domain boundary map')
    planned = {n for n, o in operations.items() if o['implementation'] == 'planned_provider'}
    robot_operations = {n for n, o in operations.items() if o['family'] in {'device', 'control', 'media', 'bridge', 'component'}}
    require(set(robot['operations']) == robot_operations and planned <= robot_operations, 'robot design and provider family set differ')
    used = set()
    for name, row in records.items():
        op = operations[name]
        require(set(row) == {'operation', 'primary_domain', 'related_domains', 'behavior', 'wire_source', 'authority_action',
            'secondary_actions', 'semantics_source', 'provider_contract'}, 'unknown/missing operation design field: ' + name)
        require(row['behavior'] in behaviors, 'undefined behavior: ' + name)
        used.add(row['behavior'])
        require(row['authority_action'] == op['action'] and row['secondary_actions'] == op['secondary_actions'], 'authority drift: ' + name)
        require(row['wire_source'] == row['semantics_source'] == 'interface-catalogue.json#' + name, 'wire/semantics drift: ' + name)
        require(row['primary_domain'] in domains and row['primary_domain'] in row['related_domains'], 'missing primary domain: ' + name)
        require(row['related_domains'] == sorted(set(row['related_domains'])) and set(row['related_domains']) <= set(domains), 'invalid related domains: ' + name)
        require(not re.search(r'(^|[._])v[0-9]+($|[._])', name), 'ambiguous internal operation version: ' + name)
        require(row['provider_contract'] == ('robot-provider-contracts.json#' + name if name in robot_operations else None), 'incorrect provider design link: ' + name)
        require(bool(op['contract']) and 'request' in op and 'result' in op, 'missing concrete wire/meaning: ' + name)
    require(used == set(behaviors), 'unused behavior contract')
    for row in domains.values():
        require(row['boundary'] and row['mechanism'] and row['operations'] and set(row['operations']) <= set(operations), 'incomplete domain')
        require(all(row['id'] in records[n]['related_domains'] for n in row['operations']), 'domain operation omitted from ledger')
    types = robot['deployment_types']
    edges = {}
    def base(kind):
        if kind.startswith('optional<') and kind.endswith('>'):
            kind = kind[9:-1]
        require(kind in types, 'undefined deployment type: ' + kind)
        return kind
    for name, kind in types.items():
        require(kind and set(kind) <= {'rule', 'fields', 'element', 'enum'}, 'unknown deployment shape: ' + name)
        require(sum(key in kind for key in ['fields', 'element', 'enum']) <= 1, 'ambiguous deployment shape: ' + name)
        require(any(key in kind for key in ['fields', 'element', 'enum']) or bool(kind.get('rule')), 'undefined scalar contract: ' + name)
        dependencies = []
        if 'fields' in kind:
            require(len({n for n, _ in kind['fields']}) == len(kind['fields']), 'duplicate deployment record field')
            dependencies += [base(t) for _, t in kind['fields']]
        if 'element' in kind:
            dependencies.append(base(kind['element']))
        if 'enum' in kind:
            require(kind['enum'] and len(kind['enum']) == len(set(kind['enum'])), 'empty/duplicate closed selector')
        edges[name] = dependencies
    visiting, visited = set(), set()
    def walk(name):
        require(name not in visiting, 'recursive unbounded deployment shape: ' + name)
        if name in visited:
            return
        visiting.add(name)
        for other in edges[name]:
            walk(other)
        visiting.remove(name)
        visited.add(name)
    for name in types:
        walk(name)
    for name, family in robot['families'].items():
        require(set(family['profile_field_types']) == set(family['profile_fields']), 'untyped profile field: ' + name)
        for kind in family['profile_field_types'].values():
            base(kind)
        for field in ['capacity_fields', 'timing_fields', 'profile_fields', 'required_features', 'states']:
            require(family[field] and len(family[field]) == len(set(family[field])), 'duplicate/empty family field: ' + name)
        require({'stop_bytes', 'stop_slots', 'retained_bytes', 'workspace_bytes', 'workspace_calls'} <= set(family['capacity_fields']), 'missing independent reservations')
        require(family['limit_contract'], 'missing finite limit semantics')
    expected_lanes = {'device.submit': 'command', 'device.stop': 'stop', 'control.submit': 'cycle_write', 'control.stop': 'stop',
        'media.submit': 'frame', 'media.close': 'close', 'bridge.start': 'start', 'bridge.stop': 'stop',
        'component.start': 'start', 'component.stop': 'stop'}
    for name, rule in robot['operations'].items():
        require(rule['primary_action'] == operations[name]['action'] and rule['family'] == operations[name]['family'], 'robot operation mismatch')
        require(set(rule['inherits']) == set(robot['common']), 'omitted robot safety requirement')
        require(all(rule[k] for k in ['secondary_admission', 'effect', 'commit_and_recovery']), 'missing robot operation semantics')
        require('sequence_domain' in rule and rule['sequence_domain'] == expected_lanes.get(name), 'stop/normal replay domains drift: ' + name)
    require(set(robot['reply_effect_matrix']) == set(catalogue['schemas']['receipt']['enums']['state']), 'reply state set drift')
    effects = set(catalogue['schemas']['receipt']['enums']['effect'])
    for states in robot['reply_effect_matrix'].values():
        require(states and len(states) == len(set(states)) and set(states) <= effects, 'reply effect drift')
    require('unknown' not in robot['reply_effect_matrix']['succeeded'] and
        set(robot['reply_effect_matrix']['outcome_unknown']) == {'unknown', 'partial'}, 'unknown outcome concealed')
    return dict(operations=len(records), domains=len(domains), behaviors=len(behaviors),
                typed_profile_fields=sum(len(f['profile_fields']) for f in robot['families'].values()),
                registered=sum(o['implementation'] == 'registered_local' for o in operations.values()),
                planned=len(planned), design_inventory_complete=True, registered_interfaces_complete=not planned,
                providers_complete=False,
                native_equivalence_proven=False, package_portability_proven=False, industrial_assurance_proven=False)


def render(catalogue, design, robot):
    records = {row['operation']: row for row in design['operations']}
    domains = {row['id']: row for row in robot['domains']}
    lines = ['# ROSv2 complete operation design reference', '',
        'Generated by `tools/ros_scene/check_operation_design.py --generate` from the',
        'semantic catalogue and operational contracts. Edit those sources, then regenerate.', '',
        f'This reference specifies all **{len(records)} catalogue operations**. A registered',
        'import, an implemented native provider and a qualified robot deployment are separate',
        'claims. Planned operations below are not callable imports or available CLI options.',
        'The named fields are ordered wire fields; the existing special/fixed carriers retain',
        'their exact concrete encoding. No new ABI integer status is introduced here.', '',
        'See [provider procedures](ros-robot-provider-procedures.md),',
        '[robot capability domains](ros-robot-capabilities.md),',
        '[legacy migration obligations](ros-legacy-coverage.md), and',
        '[implemented CLI](../command-line/ros-scene.md).', '', '## Common rules', '']
    for name, value in design['common'].items():
        lines += [f'### {name.replace("_", " ").capitalize()}', '', value, '']
    lines += ['## Commit, delivery and recovery classes', '']
    for name, rule in design['behaviors'].items():
        lines += [f'### {name}', '']
        for field, text in rule.items():
            lines += [f'**{field.replace("_", " ").capitalize()}.** {text}', '']
    lines += ['## Operations', '']
    def fields(values):
        return ', '.join('`' + v['name'] + ': ' + v['type'] + '`' for v in values) or 'No fields.'
    for op in catalogue['operations']:
        row = records[op['name']]
        lines += [f'### {op["name"]}', '',
            f'**Availability:** `{op["implementation"]}`. **Carrier:** `{op["wire"]}`.', '',
            f'**Domain:** {row["primary_domain"]}, `{domains[row["primary_domain"]]["name"]}`. '
            f'**Recovery class:** `{row["behavior"]}`.', '',
            f'**Request:** {fields(op["request"])}', '', f'**Result:** {fields(op["result"])}', '',
            f'**Authority:** exact `{row["authority_action"]}`; additional named actions: '
            + (', '.join('`'+a+'`' for a in row['secondary_actions']) or 'none listed')
            + '. Actual ancestor, source, profile and target checks still apply.', '', op['contract'].rstrip(), '']
        if op['name'] in robot['operations']:
            specific = robot['operations'][op['name']]
            lines += ['**Additional admission.** ' + specific['secondary_admission'], '',
                      '**Effect boundary.** ' + specific['effect'], '',
                      '**Commit and recovery.** ' + specific['commit_and_recovery'], '']
    lines += ['## Canonical named records', '',
        'These definitions apply to canonical/proposed record carriers. Raw fixed-word',
        'artifact/lifecycle/runtime formats are specified by their concrete contract headers.', '']
    for name, schema in catalogue['schemas'].items():
        lines += ['### ' + name, '']
        if 'variant' in schema:
            for variant, values in schema['variant'].items():
                lines += [f'- Tag `{schema["variant_tags"][variant]}` (`{variant}`): '
                          + fields([dict(name=n, type=t) for n, t in values])]
            lines += ['']
        else:
            lines += [fields([dict(name=n, type=t) for n, t in schema['fields']]), '']
        for field, choices in schema.get('enums', {}).items():
            lines += [f'`{field}` is one of: ' + ', '.join('`'+v+'`' for v in choices) + '.', '']
        for text in schema.get('constraints', []):
            lines += [text, '']
    document = '\n'.join(lines)
    header = ['/** @file', ' * @brief Complete design metadata; this table registers no guest imports.',
        ' * @details Generated from the canonical catalogue and operational design.',
        ' * Availability records registration only, not native or safety qualification.',
        ' * No dynamic storage, policy mutation or provider activation occurs here.', ' */',
        '#pragma once', '#include <array>', '#include <string_view>', '',
        'namespace uwvm2::imported::ros::scene::operation_design', '{',
        '    /** @brief Reviewed wire/authority/commit contract for one named operation. */',
        '    struct descriptor',
        '    {std::string_view name,behavior,meaning;unsigned primary_domain;bool registered;};',
        '    inline constexpr std::array operations{']
    for op in catalogue['operations']:
        row = records[op['name']]
        comment = op['contract'].rstrip().replace('*/', '* /')
        header += [f'        /** @brief {op["name"]}: {row["behavior"].replace("_", " ")}.',
                   '         * @details ' + comment,
                   '         * Apply the complete origin, bounded delivery and recovery contract;',
                   '         * this descriptor neither exposes the operation nor grants authority.',
                   '         */',
                   '        descriptor{' + ','.join([json.dumps(op['name']), json.dumps(row['behavior']),
                       json.dumps(op['contract']), str(row['primary_domain']),
                       'true' if op['implementation'] == 'registered_local' else 'false']) + '},']
    header += ['    };', '}', '']
    return document, '\n'.join(header)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--generate', action='store_true')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    require(Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup')), 'existing cgroup required')
    require((GROUP/'memory.max').read_text().strip() == '68719476736' and (GROUP/'memory.swap.max').read_text().strip() == '0', 'gate limits changed')
    inputs = [load(ROOT/name) for name in INPUTS]
    report = audit(*inputs)
    content = render(*inputs)
    for name, text in zip([DOCUMENT, HEADER], content):
        path = ROOT/name
        if args.generate:
            path.write_text(text)
        else:
            require(path.is_file() and path.read_text() == text, 'generated design drift: ' + name)
    if args.output:
        output = args.output.resolve()
        require(output.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve()) and not output.exists(), 'fresh SDK .tmp receipt required')
        report.update(complete=True, cgroup=str(GROUP), inode=GROUP.stat().st_ino, memory_max=68719476736, swap_max=0,
            inputs={name: hashlib.sha256((ROOT/name).read_bytes()).hexdigest() for name in [*INPUTS, DOCUMENT, HEADER, 'tools/ros_scene/check_operation_design.py']})
        output.parent.mkdir(parents=True, exist_ok=True)
        output.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
