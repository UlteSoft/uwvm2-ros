#!/usr/bin/env python3
"""Measure actual selected Wasm applications and retain closed lane summaries.

This Linux metadata lane checks every reported application body against its
own content pin, measures the existing frozen ELF, and reads retained value
reports as data. It invokes no compiler, Guest, middleware or vendor SDK.
Source volume, individual Guest products and Runtime load extents remain
separate; a data-only frozen Runtime does not qualify a native-bound A+B build.
"""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    readback = module('remaining_closed_value_metadata', source / 'test/0073.ros-tests/support/source/ros_platform_sdk/runtimecheck_remaining.py')
    elf = module('remaining_observed_elf_sizes', source / 'sdk/origin/tools/ros_binary_size.py')
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', applications=[], application_ranges=[], managed_values=[],
                  closed_lanes=[], default_authority=[], compiler_guest_native_runs=0)
    for vendor in ['deep_robotics', 'pal', 'trossen', 'agibot', 'ubtech', 'fourier']:
        slug = vendor.replace('_', '-')
        direct_slug = 'deep' if vendor == 'deep_robotics' else slug
        direct_name = 'qualify-' + direct_slug + ('-r2' if vendor == 'agibot' else '-r1')
        groups = [(direct_name, 'qualification.json'), ('managed-' + slug + '-r1', 'managed.json')]
        ranges = {}
        for phase, filename in groups:
            result = json.loads((root / 'out' / phase / filename).read_bytes())
            owner = json.loads((root / 'out' / ('remaining-' + phase + '.json')).read_bytes())
            assert owner['status'] == 'PASS' and all(row['exit'] == 0 for row in owner['commands'])
            report['closed_lanes'].append(dict(phase=phase, status=result['status'],
                actual_commands=len(result['commands']),
                max_sampled_owned_rss=max(row['max_sampled_owned_rss'] for row in owner['commands']),
                events_before=owner['events_before'], events_after=owner['events_after']))
            for row in result['artifacts']:
                label = row['label']
                if filename == 'qualification.json':
                    if not label.endswith(('-c-app', '-cpp-app')):
                        continue
                    language = 'cpp' if label.endswith('-cpp-app') else 'c'
                else:
                    language = label.rsplit('-', 1)[1]
                path = Path(row['path'])
                assert path.is_relative_to(root) and path.is_file() and not path.is_symlink()
                assert path.stat().st_size == row['bytes'] and sha(path) == row['sha256']
                report['applications'].append(dict(vendor=vendor, language=language, **row))
                ranges.setdefault(language, []).append(row['bytes'])
        assert set(ranges) == {'c', 'cpp', 'rust', 'assemblyscript', 'tinygo', 'javascript'}
        for language, extents in sorted(ranges.items()):
            report['application_ranges'].append(dict(vendor=vendor, language=language,
                count=len(extents), minimum_bytes=min(extents), maximum_bytes=max(extents)))
    primary = Path('/home/macromodel/Documents/uwvm-test-261018/recovery/ros-remaining-r60')
    for label, phases in [
        ('managed-values-first-three-r1', ['values-agibot-r1', 'values-pal-r1', 'values-fourier-r6']),
        ('managed-values-other-three-r1', ['values-deep-robotics-r1', 'values-ubtech-r1', 'values-trossen-r1'])]:
        bodies = readback.retained_roles(root, primary / (label + '.json'),
                                        ['out/' + phase + '/values.json' for phase in phases])
        for name, body in bodies.items():
            value = json.loads(body)
            phase = Path(name).parent.name
            owner = json.loads((root / 'out' / ('remaining-' + phase + '.json')).read_bytes())
            assert owner['status'] == 'PASS'
            report['managed_values'].append(dict(phase=phase, checkpoint=label, vendor=value['vendor'],
                status=value['status'], commands=len(value['commands']), products=len(value['artifacts']),
                profiles=len(value['profiles']), records=sum(row['records'] for row in value['profiles']),
                populated_scalar_or_string_instances=sum(row['original_scalar_or_string_instances'] for row in value['profiles']),
                max_sampled_owned_rss=max(row['max_sampled_owned_rss'] for row in owner['commands']),
                report_sha256=hashlib.sha256(body).hexdigest(),
                events_before=owner['events_before'], events_after=owner['events_after']))
    report['frozen_data_test_runtime'] = elf.measure(root / 'tools/uwvm2-ros')
    assert len(report['applications']) == 156
    assert sum(row['products'] for row in report['managed_values']) == 104
    assert sum(row['records'] for row in report['managed_values']) == 1729
    report['status'] = 'ACTUAL_156_APPLICATION_BODY_SIZES_AND_CLOSED_VALUE_REPORTS_VERIFIED'
    (out / 'sizes.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
