#!/usr/bin/env python3
"""Check exhaustive deprecated ROS import mappings without claiming equivalence.

Both the actual C++ import tuples and the independent host-policy catalogue
must match the mapping exactly. New/unmapped imports, drifted C++ targets,
undefined replacement operations and missing safety obligations fail closed.
Registration, design review and native differential qualification are reported
separately. All execution requires the existing 64 GiB zero-swap test cgroup.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
GROUP = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def audit(root, mapping, catalogue_root=None):
    host = root / mapping['legacy_source']
    policy = root / mapping['policy_source']
    source = host.read_text()
    constants = {}
    for name, text in re.findall(r'\b(\w+)\s*\[\]\s*=\s*u8"([^"]+)"', source):
        require(name not in constants or constants[name] == text, 'ambiguous name constant: ' + name)
        constants[name] = text
    actual, modules = {}, set()
    pattern = r'struct (\w+_local_imported_module_t)\s*\{(.*?)\n    \};'
    entry = r'wrapper::native_function<&([^,]+),\s*(?:wrapper|portable_names)::(\w+),\s*module_name>'
    for declaration in re.finditer(pattern, source, re.S):
        body = declaration[2]
        module = re.search(r'module_name\{u8"([^"]+)"\}', body)
        if not module:
            continue
        require(module[1] not in modules, 'duplicate legacy module')
        modules.add(module[1])
        rows = re.findall(entry, body)
        require(rows and len(rows) == body.count('wrapper::native_function<'), 'unparsed concrete tuple: ' + module[1])
        for target, constant in rows:
            require(constant in constants, 'unknown function constant: ' + constant)
            name = module[1] + '.' + constants[constant]
            require(name not in actual, 'duplicate concrete import: ' + name)
            actual[name] = target
    require(actual, 'no concrete legacy imports')
    policy_rows = re.findall(r'entry\{family::(ros[12]),"([^"]+)","([^"]+)","([^"]+)"\}', policy.read_text())
    require(all(full == module + '.' + function for _, module, function, full in policy_rows), 'host-policy spelling drift')
    policy_names = [full for _, _, _, full in policy_rows]
    require(len(policy_names) == len(set(policy_names)) and set(policy_names) == set(actual), 'host policy and actual tuples differ')
    catalogue = json.loads(((catalogue_root or root) / 'tools/ros_scene' / mapping['catalogue']).read_text())
    require(set(catalogue['deprecated_modules']) == modules, 'deprecation scope differs from concrete modules')
    operations = {row['name']: row for row in catalogue['operations']}
    records = mapping['imports']
    require(len({row['legacy'] for row in records}) == len(records), 'duplicate migration mapping')
    require({row['legacy'] for row in records} == set(actual), 'missing or unknown legacy mapping')
    profiles = mapping['profiles']
    used, incomplete, registered = set(), {}, []
    for row in records:
        name = row['legacy']
        require(row['cpp_target'] == actual[name], 'C++ target drift: ' + name)
        key = row['semantic_profile']
        require(key in profiles, 'undefined semantic profile: ' + name)
        used.add(key)
        profile = profiles[key]
        require(profile['meaning'] and profile['authority'] and profile['verification_obligations'], 'missing semantic/authority/verification obligations: ' + key)
        require(all(isinstance(x, str) and x for x in profile['authority'] + profile['verification_obligations']), 'empty safety obligation: ' + key)
        targets = profile['targets']
        require(targets and len(targets) == len(set(targets)), 'empty/duplicate replacement target: ' + key)
        require(set(targets) <= set(operations), 'undefined replacement operation: ' + key)
        # A prose status cannot serve as a native differential receipt.
        require(profile['differential_qualification'] == 'not_qualified', 'unsupported native-equivalence claim: ' + key)
        missing = [target for target in targets if operations[target]['implementation'] != 'registered_local']
        if missing:
            incomplete[name] = missing
        else:
            registered.append(name)
    require(used == set(profiles), 'unused or stale semantic profile')
    return dict(source_inventory_complete=True, mapping_complete=True,
                legacy_modules=len(modules), legacy_imports=len(actual), semantic_profiles=len(profiles),
                all_targets_registered=not incomplete, mappings_with_registered_targets=registered,
                mappings_requiring_unregistered_targets=incomplete,
                native_equivalence_proven=False, package_portability_proven=False,
                qualification='exhaustive source mapping with reviewed obligations; no native differential proof')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--require-implemented', action='store_true')
    parser.add_argument('--source-root', type=Path, default=ROOT,
                        help='Exact legacy host/policy source snapshot; migration catalogue remains in this checkout')
    args = parser.parse_args()
    require(Path('/proc/self/cgroup').read_text().strip() == '0::/' + str(GROUP.relative_to('/sys/fs/cgroup')), 'must run in existing cgroup')
    require((GROUP / 'memory.max').read_text().strip() == '68719476736' and (GROUP / 'memory.swap.max').read_text().strip() == '0', 'gate limits changed')
    output = args.output.resolve()
    require(output.is_relative_to((ROOT.parent / 'uwvm2-ros-sdk/.tmp').resolve()), 'receipt must stay in SDK .tmp')
    require(not output.exists(), 'do not overwrite prior evidence')
    source_root = args.source_root.resolve()
    paths = [Path(__file__).resolve(), ROOT / 'tools/ros_scene/legacy-coverage.json',
             ROOT / 'tools/ros_scene/interface-catalogue.json', source_root / 'src/uwvm2/uwvm/imported/ros/host.h',
             source_root / 'src/uwvm2/utils/control/host_interfaces.h', ROOT / 'documents/runtime/ros-legacy-coverage.md']
    pins = {str(path): sha(path) for path in paths}
    result = audit(source_root, json.loads(paths[1].read_text()), ROOT)
    require(pins == {str(path): sha(path) for path in paths}, 'source changed during audit')
    result.update(inputs=pins, source_root=str(source_root), catalogue_root=str(ROOT),
                  cgroup=str(GROUP), inode=GROUP.stat().st_ino, memory_max=68719476736, swap_max=0)
    result['complete'] = not args.require_implemented or result['all_targets_registered']
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(result, indent=2) + '\n')
    require(result['complete'], 'replacement operations still unimplemented; see receipt')
    print('PASS source mapping:', result['legacy_modules'], 'modules,', result['legacy_imports'],
          'imports;', len(result['mappings_requiring_unregistered_targets']), 'mappings still need registered targets; native equivalence unproven')


if __name__ == '__main__':
    main()
