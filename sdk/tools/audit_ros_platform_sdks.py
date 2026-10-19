#!/usr/bin/env python3
"""Audit shipped vendor SDK Source coverage without claiming execution results.

The historical research inventory is immutable provenance, not a completion
database. This separate report checks generated files against their actual
generation receipts, and distinguishes a vendor kit from a configuration option.
File presence and a matching Source hash do not establish API compatibility,
positive Wasm delivery, official-suite acceptance, simulation or hardware safety.
"""
import argparse
import hashlib
import json
from pathlib import Path


FAMILIES = {
    'unitree': ('unitree-go-complete', 'unitree-hg-complete', 'unitree-api-complete', 'unitree-ros1-complete'),
    'elephant': tuple('elephant-mycobot-'+p+'-complete' for p in ('kinetic','melodic','noetic','foxy','galactic','humble')),
    'booster': ('booster-ros2-complete', 'booster-sdk-complete'),
    'limx': ('limx-bridge-complete',), 'abb': ('abb-ros1-complete', 'abb-ros2-complete'),
    'fanuc': ('fanuc-ros2-complete',), 'kuka': ('kuka-ros2-complete',),
    'yaskawa': ('yaskawa-ros2-complete',),
    'kinova': ('mobile-fjt-humble-complete',), 'hello_robot': ('mobile-fjt-humble-complete',),
    'boston_dynamics': ('spot-bridge-complete',),
    'agilex': ('agilex-scout-ros1-complete', 'agilex-scout-ros2-complete'),
    'universal_robots': ('universal-robots-ros1-complete','universal-robots-ros2-complete','universal-robots-polyscopex-complete'),
    'franka': ('franka-ros1-complete', 'franka-ros2-complete'),
    'clearpath': ('clearpath-noetic-complete', 'clearpath-humble-complete', 'clearpath-jazzy-complete', 'clearpath-main-complete'),
    'robotis': ('robotis-turtlebot3-noetic-complete', 'robotis-turtlebot3-humble-complete',
                'robotis-turtlebot3-jazzy-complete', 'robotis-turtlebot3-main-complete'),
}
LAYERS = {'isaac-ros': ('isaac-ros-complete',), 'isaac-sim': ('isaac-sim-complete',),
          'groot': ('groot-bridge-complete',)}
LANGUAGES = dict(c='h', cpp='hpp', rust='rs', javascript='mjs', assemblyscript='ts', tinygo='go')


def audit(root):
    root = Path(root).resolve(strict=True)
    inventory_path = root / 'sdk/robots/vendors/inventory-v2.json'
    raw = inventory_path.read_bytes()
    inventory = json.loads(raw)
    kits = {}
    for name in sorted({n for group in (*FAMILIES.values(), *LAYERS.values()) for n in group}):
        path = root / 'sdk/custom-types' / name
        receipt = path / 'generation.vendor.json'
        if not receipt.is_file():
            kits[name] = dict(status='MISSING_GENERATED_SOURCE', missing=['generation.vendor.json'])
            continue
        raw_receipt = receipt.read_bytes()
        generation = json.loads(raw_receipt)
        required = ['bounded_records.' + suffix for suffix in LANGUAGES.values()]
        missing = [n for n in required if n not in generation.get('outputs', {})]
        mismatches = []
        if generation.get('default_authority') != [] or generation.get('implicit_permissions') != []:
            mismatches.append('nonempty-or-missing-default-authority/implicit-permissions')
        for relative, pin in generation.get('outputs', {}).items():
            parts = Path(relative)
            if (parts.is_absolute() or '..' in parts.parts or (path / parts).is_symlink() or
                    not (path / parts).resolve().is_relative_to(path.resolve())):
                mismatches.append(relative)
                continue
            target = path / parts
            if not target.is_file():
                missing.append(relative)
                continue
            data = target.read_bytes()
            if len(data) != pin['bytes'] or hashlib.sha256(data).hexdigest() != pin['sha256']:
                mismatches.append(relative)
        kits[name] = dict(status='GENERATED_SOURCE_HASHES_MATCH' if not (missing or mismatches) else 'SOURCE_MISMATCH',
                          receipt_sha256=hashlib.sha256(raw_receipt).hexdigest(),
                          missing=missing, mismatches=mismatches,
                          checked_outputs=len(generation.get('outputs', {})),
                          complete_records=len(generation.get('records', [])),
                          native_registry_emitted=generation.get('native_registry_emitted', False),
                          default_authority=generation.get('default_authority'),
                          languages=list(LANGUAGES),
                          qualification='Generated source only; retain separately scoped execution evidence.')
    rows = []
    for vendor in inventory['vendors']:
        names = FAMILIES.get(vendor['id'], ())
        rows.append(dict(id=vendor['id'], name=vendor['name'],
                         xmake_option='ros-vendor-' + vendor['id'].replace('_', '-'),
                         generated_families=list(names),
                         source_status=('GENERATED_SELECTED_INTERFACE_FAMILIES' if names and
                              all(kits[n]['status'] == 'GENERATED_SOURCE_HASHES_MATCH' for n in names)
                              else 'MISSING_OR_PARTIAL_VENDOR_SDK'),
                         complete_vendor_support_claimed=False))
    return dict(kind='shipped-ros-platform-sdk-source-audit-v1',
                inventory_sha256=hashlib.sha256(raw).hexdigest(), vendors=rows,
                vendor_count=len(rows), data_platforms=LAYERS, generated_kits=kits,
                vendors_with_selected_generated_source=sum(r['source_status'] == 'GENERATED_SELECTED_INTERFACE_FAMILIES' for r in rows),
                all_vendors_have_selected_generated_source=all(r['source_status'] == 'GENERATED_SELECTED_INTERFACE_FAMILIES' for r in rows),
                actual_compiler_or_guest_or_native_runs=0, default_authority=[],
                qualification='Source coverage only. Missing families remain work; existing handwritten/model/protobuf kits retain their own narrower status. No whole-vendor/API/release/official-suite acceptance is inferred.')


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--source', type=Path, default=Path(__file__).resolve().parents[2])
    p.add_argument('--require-all', action='store_true', help='Fail CI while any vendor lacks a selected generated family')
    a = p.parse_args()
    result = audit(a.source)
    print(json.dumps(result, indent=2))
    failed = any(k['status'] != 'GENERATED_SOURCE_HASHES_MATCH' for k in result['generated_kits'].values())
    return 1 if failed or (a.require_all and not result['all_vendors_have_selected_generated_source']) else 0


if __name__ == '__main__':
    raise SystemExit(main())
