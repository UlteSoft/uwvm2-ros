#!/usr/bin/env python3
"""Inventory every acquired package and interface before extending ROSv2.

This is exhaustive over the supplied receipts, not a whole-package semantic
review. Read source definitions as data only. Preserve unowned files, unsupported
syntax, missing sources and multiple distribution variants as separate evidence.
No inferred capability is a grant or a claim that a native adapter exists.
"""
from collections import Counter, defaultdict
from pathlib import Path, PurePosixPath
import argparse
import hashlib
import json
import os
import re
import subprocess

from acquire import save

ROOT = Path(__file__).resolve().parents[2]
GATE = Path('/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope')
PRIMITIVES = {'bool', 'byte', 'char', 'int8', 'uint8', 'int16', 'uint16', 'int32',
              'uint32', 'int64', 'uint64', 'float32', 'float64', 'string', 'wstring', 'time', 'duration'}
TYPE = re.compile(r'(?P<base>[A-Za-z][A-Za-z0-9_/]*)(?:<=(?P<string_bound>[0-9]+))?(?:\[(?P<array>(?:<=)?[0-9]*)\])?\Z')
FIELD = re.compile(r'(?P<type>\S+)\s+(?P<name>[A-Za-z][A-Za-z0-9_]*)(?P<tail>.*)\Z')


def sha(path):
    with Path(path).open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def guard():
    assert Path('/proc/self/cgroup').read_text().strip() == '0::/'+str(GATE.relative_to('/sys/fs/cgroup'))
    assert (GATE/'memory.max').read_text().strip() == '68719476736'
    assert (GATE/'memory.swap.max').read_text().strip() == '0'


def shape(text, extension):
    """Extract declaration shapes, never evaluate constants or default values.

    Constants/defaults are retained in the original pinned file. IDL is reported
    separately rather than incorrectly interpreted with the .msg grammar.
    """
    if extension == '.idl':
        return dict(status='IDL_REQUIRES_PARSER', fields=[], issues=[], sections=None)
    sections, fields, issues = 1, [], []
    for line_number, source in enumerate(text.splitlines(), 1):
        line = source.strip()
        if not line or line.startswith('#'):
            continue
        if line.split('#', 1)[0].strip() == '---':
            sections += 1
            continue
        match = FIELD.fullmatch(line)
        if not match:
            issues.append(dict(line=line_number, reason='unrecognized declaration'))
            continue
        tail = match['tail'].lstrip()
        if tail.startswith('='):
            continue
        parsed = TYPE.fullmatch(match['type'])
        if not parsed:
            issues.append(dict(line=line_number, reason='unrecognized field type', spelling=match['type']))
            continue
        base, array, bound = parsed['base'], parsed['array'], parsed['string_bound']
        if bound is not None and base not in ('string', 'wstring'):
            issues.append(dict(line=line_number, reason='non-string bound'))
        fields.append(dict(line=line_number, section=sections, name=match['name'], type=base,
            primitive=base in PRIMITIVES,
            string_bound=int(bound) if bound is not None else None,
            array='scalar' if array is None else 'sequence' if not array else 'bounded_sequence' if array.startswith('<=') else 'fixed_array',
            array_bound=int(array.removeprefix('<=')) if array else None,
            has_default=bool(tail and not tail.startswith('#'))))
    if sections != {'.msg': 1, '.srv': 2, '.action': 3}[extension]:
        issues.append(dict(line=0, reason='wrong section count', actual=sections))
    return dict(status='SHAPES_EXTRACTED' if not issues else 'SYNTAX_REQUIRES_REVIEW', fields=fields, issues=issues, sections=sections)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    guard()
    root, out = args.root.resolve(), args.output.resolve()
    assert out.is_relative_to((ROOT.parent/'uwvm2-ros-sdk/.tmp').resolve())
    out.mkdir(parents=True, exist_ok=False)
    inventory_path = root/'reconciliation/package-effect-inventory.json'
    summary_path = root/'reconciliation/summary.json'
    inventory = json.loads(inventory_path.read_text())
    coverage = json.loads(summary_path.read_text())
    pins = {str(p): sha(p) for p in (inventory_path, summary_path, Path(__file__))}
    packages, owners, selected = {}, defaultdict(dict), {}
    effects, dependencies = defaultdict(set), defaultdict(set)
    for row in inventory:
        name = row['package']
        package = packages.setdefault(name, dict(name=name, instances=[], interfaces=[],
            semantic_review='PENDING', native_portability='NOT_TESTED'))
        package['instances'].append({k: row[k] for k in ('source_id', 'manifest', 'source_url', 'source_ref', 'archive_sha256')})
        owners[row['source_id']].setdefault(str(PurePosixPath(row['manifest']).parent), set()).add(name)
        for effect in row['effects']:
            effects[effect].add(name)
        for dependency in row['dependency_names']:
            dependencies[dependency].add(name)
    for tree in (root, root/'supplemental', root/'recovery'):
        for path in sorted((tree/'receipts').glob('*.json')):
            record = json.loads(path.read_text())
            if record.get('status') == 'ACQUIRED_AND_SCANNED':
                selected[path.stem] = tree
    definitions, unowned, refused = [], [], []
    types, features, statuses = Counter(), Counter(), Counter()
    # rg performs the corpus traversal without executing any acquired source.
    argv = ['rg', '--files', '--hidden', '--no-ignore', '-0',
            '-g', '*.msg', '-g', '*.srv', '-g', '*.action', '-g', '*.idl',
            *(str(tree/'sources') for tree in (root, root/'supplemental', root/'recovery') if (tree/'sources').is_dir())]
    result = subprocess.run(argv, capture_output=True, check=True)
    paths = sorted(Path(os.fsdecode(p)) for p in result.stdout.split(b'\0') if p)
    for index, path in enumerate(paths):
        guard() if index % 10000 == 0 else None
        tree = next((t for t in (root/'recovery', root/'supplemental', root) if path.is_relative_to(t/'sources')), None)
        assert tree is not None
        relative = path.relative_to(tree/'sources')
        source_id, filename = relative.parts[0], PurePosixPath(*relative.parts[1:])
        if selected.get(source_id) != tree:
            continue
        source_root = tree/'sources'/source_id
        resolved = path.resolve()
        if not resolved.is_relative_to(source_root) or path.stat().st_size > 8 << 20:
            refused.append(dict(path=str(path), reason='escaped or oversized definition'))
            continue
        owning = None
        for parent in filename.parents:
            if str(parent) in owners[source_id]:
                owning = owners[source_id][str(parent)]
                break
        if not owning or len(owning) != 1:
            unowned.append(dict(source_id=source_id, file=str(filename), reason='no unambiguous enclosing package'))
            continue
        package = next(iter(owning))
        data = path.read_bytes()
        record = dict(source_id=source_id, package=package, file=str(path), relative_file=str(filename),
            sha256=hashlib.sha256(data).hexdigest(), bytes=len(data), kind=path.suffix[1:])
        try:
            parsed = shape(data.decode('utf-8-sig'), path.suffix)
        except UnicodeError:
            parsed = dict(status='ENCODING_REQUIRES_REVIEW', fields=[], issues=[], sections=None)
        record.update(parsed)
        statuses[parsed['status']] += 1
        for field in parsed['fields']:
            types[field['type']] += 1
            features[field['array']] += 1
            if not field['primitive']:
                features['nested_message'] += 1
            if field['string_bound'] is not None:
                features['bounded_string'] += 1
            if field['has_default']:
                features['default_value'] += 1
        packages[package]['interfaces'].append(len(definitions))
        definitions.append(record)
    save(out/'definitions.json', definitions)
    save(out/'packages.json', list(packages.values()))
    save(out/'unowned-definitions.json', unowned)
    save(out/'refused-definitions.json', refused)
    save(out/'dependency-consumers.json', {key: sorted(value) for key, value in sorted(dependencies.items())})
    save(out/'effect-consumers.json', {key: sorted(value) for key, value in sorted(effects.items())})
    report = dict(complete=True, scope=__doc__, inputs=pins,
        linux_abis=['x86_64', 'i686', 'aarch64', 'arm', 'armhf'],
        source_snapshots=len(selected), package_names=len(packages), package_instances=len(inventory),
        interface_files=len(definitions), packages_with_interfaces=sum(bool(p['interfaces']) for p in packages.values()),
        definition_kinds=dict(Counter(d['kind'] for d in definitions)), shape_statuses=dict(statuses),
        field_shapes=dict(features), field_types=dict(types),
        effect_package_counts={k: len(v) for k, v in effects.items()},
        most_used_dependencies=sorted((dict(name=k, packages=len(v)) for k, v in dependencies.items()), key=lambda r: (-r['packages'], r['name']))[:100],
        unowned_definitions=len(unowned), refused_definitions=len(refused),
        acquisition_coverage=coverage, semantic_review='PENDING', native_portability='NOT_TESTED',
        cgroup=str(GATE), memory_max=68719476736, swap_max=0,
        inputs_unchanged=all(sha(p) == value for p, value in pins.items()),
        artifacts={p.name: sha(p) for p in out.glob('*.json')})
    assert report['inputs_unchanged']
    save(out/'results.json', report)
    print(json.dumps({k: report[k] for k in ('complete', 'package_names', 'package_instances', 'interface_files', 'definition_kinds', 'shape_statuses', 'field_shapes')}))


if __name__ == '__main__':
    main()
