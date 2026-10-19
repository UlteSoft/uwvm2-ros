#!/usr/bin/env python3
"""Prepare fixed Unitree native metadata builds; never execute a build or ROS.

The prepare command consumes an already lock-checked source materialization.
It copies complete definitions and emits literal includes/types/package names,
not Python/CMake expressions from a catalog. The merge command combines actual
bound catalogs only after reopening their original binding receipts and exports.
Neither command computes a native MD5/RIHS, downloads, imports a generator,
invokes a subprocess, grants a capability or labels source metadata compatible.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import stat

FAMILIES = {'unitree_go': (2, 24), 'unitree_hg': (2, 11),
            'unitree_api': (2, 8), 'unitree_legged_msgs': (1, 11)}
PIN_KEYS = {'path', 'device', 'inode', 'bytes', 'mtime_ns', 'ctime_ns', 'sha256'}


def read(path, maximum=4 * 1024 * 1024):
    """Bind an opened non-symlink regular leaf; no FIFO, unbounded read or mmap."""
    path = Path(path).absolute()
    fd = os.open(path, os.O_RDONLY | os.O_NONBLOCK | os.O_NOFOLLOW)
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or not 0 <= before.st_size <= maximum:
            raise ValueError('bounded regular input required: ' + str(path))
        body = bytearray()
        while len(body) <= before.st_size:
            block = os.read(fd, min(65536, before.st_size + 1 - len(body)))
            if not block:
                break
            body.extend(block)
        after = os.fstat(fd)
        facts = lambda s: (s.st_dev, s.st_ino, s.st_size, s.st_mtime_ns, s.st_ctime_ns)
        if facts(before) != facts(after) or len(body) != before.st_size:
            raise ValueError('input changed while opening: ' + str(path))
        raw = bytes(body)
        return raw, {'path': str(path), 'device': before.st_dev, 'inode': before.st_ino,
                     'bytes': len(raw), 'mtime_ns': before.st_mtime_ns,
                     'ctime_ns': before.st_ctime_ns, 'sha256': hashlib.sha256(raw).hexdigest()}
    finally:
        os.close(fd)


def document(raw):
    def pairs(items):
        result = {}
        for key, value in items:
            if key in result:
                raise ValueError('duplicate JSON key: ' + key)
            result[key] = value
        return result
    def reject(value):
        raise ValueError('nonfinite JSON number: ' + value)
    return json.loads(raw, object_pairs_hook=pairs, parse_constant=reject)


def write(path, raw):
    """Exclusive publication, including failure evidence; no existing output is reused."""
    path.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL | os.O_NOFOLLOW, 0o600)
    try:
        view = memoryview(raw)
        while view:
            count = os.write(fd, view)
            if count <= 0:
                raise OSError('short metadata publication')
            view = view[count:]
    finally:
        os.close(fd)


def json_bytes(value):
    return (json.dumps(value, indent=2, allow_nan=False) + '\n').encode()


def relative(value):
    if not isinstance(value, str) or not value or '\\' in value or '\0' in value:
        raise ValueError('safe relative definition path required')
    path = Path(value)
    if path.is_absolute() or any(p in ('.', '..', '') for p in value.split('/')):
        raise ValueError('definition path must not escape or normalize silently')
    return path


def definition(root, value):
    path = root / relative(value)
    if not path.resolve(strict=True).is_relative_to(root.resolve(strict=True)):
        raise ValueError('definition escapes selected preparation root')
    return read(path, 1024 * 1024)


def preservation(pins):
    for pin in pins:
        if set(pin) != PIN_KEYS or read(pin['path'], max(4 * 1024 * 1024, pin['bytes']))[1] != pin:
            raise ValueError('original input changed: ' + str(pin.get('path')))


def safe_type(value, version, family=None):
    grammar = r'([a-z][a-z0-9_]*)/' + (r'(msg|srv|action)/' if version == 2 else '') + r'([A-Z][A-Za-z0-9]*)'
    if not isinstance(value, str) or not re.fullmatch(grammar, value):
        raise ValueError('complete native type required')
    parts = value.split('/')
    if family is not None and (parts[0] != family or (version == 2 and parts[1] != 'msg')):
        raise ValueError('selected Unitree package/message family required')
    return parts


def snake(name):
    """The official rosidl C++ header convention, including IMUState -> imu_state."""
    first = re.sub(r'(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', first).lower()


def prepare(args):
    source = args.source.resolve(strict=True)
    manifest_raw, manifest_pin = read(args.source_manifest, 16 * 1024 * 1024)
    if manifest_pin['sha256'] != args.source_manifest_sha256:
        raise ValueError('explicit frozen source manifest SHA mismatch')
    manifest = document(manifest_raw)
    if not isinstance(manifest, dict):
        raise ValueError('frozen relative-path manifest required')
    pins = [manifest_pin]
    # Admit source bytes before emitting any derived build inputs. No source
    # module is imported/evaluated; the controller later runs the actual build.
    maintained = ['sdk/integrations/unitree/prepare.py', 'sdk/integrations/unitree/native_identity.cpp',
                  'sdk/integrations/unitree/CMakeLists.txt', 'sdk/integrations/unitree/README.md',
                  'gateways/common/native_io.h', 'gateways/ros2/compat/identity.h',
                  'gateways/ros2/compat/rihs.h', 'gateways/ros2/compat/features.h',
                  'src/uwvm2/imported/ros/ipc/custom_type.h', 'src/uwvm2/imported/ros/ipc/custom_wire.h']
    bodies = {}
    for name in maintained:
        raw, pin = read(source / name)
        expected = manifest.get(name)
        if not isinstance(expected, dict) or any(expected.get(k) != pin[k] for k in ('bytes', 'sha256')):
            raise ValueError('frozen maintained source differs: ' + name)
        pins.append(pin); bodies[name] = raw
    if Path(__file__).resolve(strict=True) != (source / 'sdk/integrations/unitree/prepare.py').resolve(strict=True):
        raise ValueError('execute the exact frozen preparation entry')
    profile_path = args.profile.resolve(strict=True)
    profile_name = 'sdk/robots/unitree/profiles/' + profile_path.name
    if profile_path != (source / profile_name).resolve(strict=True):
        raise ValueError('select a profile from the same frozen source')
    profile_raw, profile_pin = read(profile_path)
    expected_profile = manifest.get(profile_name)
    if not isinstance(expected_profile, dict) or any(expected_profile.get(k) != profile_pin[k] for k in ('bytes', 'sha256')):
        raise ValueError('profile differs from source manifest')
    pins.append(profile_pin); profile = document(profile_raw)
    lock_raw, lock_pin = read(source / 'sdk/robots/unitree/upstream-lock.json')
    expected_lock = manifest.get('sdk/robots/unitree/upstream-lock.json')
    if not isinstance(expected_lock, dict) or any(expected_lock.get(k) != lock_pin[k] for k in ('bytes', 'sha256')):
        raise ValueError('official source lock differs from frozen manifest')
    lock = document(lock_raw); pins.append(lock_pin)
    if lock.get('format') != 'uwvm-unitree-upstream-lock-v1':
        raise ValueError('official external source lock required')
    family = profile.get('family')
    if family not in FAMILIES or profile.get('format') != 'uwvm-unitree-profile-v1' or profile.get('default_authority') != []:
        raise ValueError('closed official Unitree source profile required')
    version, count = FAMILIES[family]
    if type(profile.get('ros_version')) is not int or profile['ros_version'] != version or profile.get('native_identity') != 'REQUIRES_OFFICIAL_EXPORT':
        raise ValueError('profile backend/preparation state mismatch')
    catalog_path = args.catalog.resolve(strict=True)
    if args.catalog.is_symlink() or catalog_path.name != 'catalog.preparation.json':
        raise ValueError('original regular materialized preparation catalog required')
    catalog_raw, catalog_pin = read(catalog_path); pins.append(catalog_pin)
    catalog = document(catalog_raw)
    report_raw, report_pin = read(args.materialization_report); pins.append(report_pin)
    report = document(report_raw)
    if report.get('status') != 'PREPARED_NOT_EXECUTED' or report.get('scope') != 'SOURCE_CATALOG_ONLY' or report.get('family') != family or report.get('inputs_preserved') is not True or report.get('runtime_permissions') != [] or report.get('native_identity') != 'NOT_ACQUIRED' or report.get('generators_compilers_tests_executed') is not False:
        raise ValueError('preserved actual source-materialization report required')
    if report.get('repository') != profile.get('repository') or report.get('commit') != profile.get('commit'):
        raise ValueError('materialization/profile upstream identity differs')
    if not isinstance(catalog, dict) or set(catalog) != {'format', 'types'} or catalog['format'] != 'uwvm-ros-custom-bounded-v2' or not isinstance(catalog['types'], list) or len(catalog['types']) != count:
        raise ValueError('exact finite family catalog required')
    declared = {row['type']: row for row in profile['definitions']}
    if len(declared) != count or set(declared) != {row.get('type') for row in catalog['types']}:
        raise ValueError('complete official profile inventory required')
    original_inputs = report.get('inputs')
    if not isinstance(original_inputs, list) or len(original_inputs) != count + 2:
        raise ValueError('complete original profile/lock/definition inputs required')
    for original in original_inputs:
        if not isinstance(original, dict) or set(original) != PIN_KEYS:
            raise ValueError('materialization input must be a full opened pin')
        pins.append(original)
    if any(not any(all(old.get(k) == selected[k] for k in ('bytes', 'sha256')) for old in original_inputs) for selected in (profile_pin, lock_pin)):
        raise ValueError('materialization used different profile/source-lock bytes')
    outputs = {row['path']: row for row in report.get('outputs', []) if isinstance(row, dict)}
    if outputs.get(catalog_pin['path']) != catalog_pin:
        raise ValueError('catalog is not the exact materialized product')
    copies, includes, entries, selected = [], [], [], []
    for row in catalog['types']:
        if not isinstance(row, dict) or set(row) != {'ros_version', 'type', 'file', 'bounds'} or row['ros_version'] != version:
            raise ValueError('unbound complete Message row required')
        parts = safe_type(row['type'], version, family)
        if row['bounds'] != declared[row['type']]['bounds'] or Path(row['file']).suffix != '.msg':
            raise ValueError('canonical bound/definition kind differs from explicit profile')
        raw, pin = definition(catalog_path.parent, row['file'])
        if outputs.get(pin['path']) != pin:
            raise ValueError('definition is not the exact materialized product')
        upstream = [r for r in lock['files'] if r.get('repository') == profile['repository'] and
                    r.get('commit') == profile['commit'] and r.get('path') == declared[row['type']]['path']]
        if len(upstream) != 1 or any(upstream[0].get(k) != pin[k] for k in ('bytes', 'sha256')):
            raise ValueError('materialized definition differs from actual upstream source lock')
        if not any(all(old.get(k) == pin[k] for k in ('bytes', 'sha256')) and
                   str(old['path']).endswith('/' + declared[row['type']]['path']) for old in original_inputs):
            raise ValueError('original lock-checked definition provenance missing')
        pins.append(pin); copies.append(('catalog/' + row['file'], raw))
        name = parts[-1]
        header = family + ('/msg/' + snake(name) + '.hpp' if version == 2 else '/' + name + '.h')
        cpp = family + ('::msg::' if version == 2 else '::') + name
        if header in includes:
            raise ValueError('native header naming collision')
        includes.append(header); entries.append('  M(' + cpp + ', "' + row['type'] + '")')
        selected.append({'type': row['type'], 'header': header, 'cpp_type': cpp})
    # No string below admits parentheses, quotes, separators, arbitrary package
    # expressions or a path from the external input into executable CMake/C++.
    header = '// Literal selection from a preserved source catalog; grants no rights.\n#pragma once\n'
    header += '#define UWVM_UNITREE_ROS_VERSION ' + str(version) + '\n'
    header += ''.join('#include <' + name + '>\n' for name in includes)
    header += '#define UWVM_UNITREE_NATIVE_MESSAGES(M) \\\n' + ' \\\n'.join(entries) + '\n'
    cmake = '# Fixed safe literals only; no source/CMake evaluation from a catalog.\n'
    cmake += 'set(UWVM_UNITREE_FAMILY "' + family + '")\nset(UWVM_UNITREE_ROS_VERSION "' + str(version) + '")\n'
    selection = {'format_version': 1, 'kind': 'unitree-native-export-selection',
                 'status': 'SOURCE_SELECTION_ONLY', 'family': family, 'ros_version': version,
                 'packages': [family], 'types': selected, 'implicit_permissions': []}
    out = args.out.absolute()
    if out.exists() or out.is_symlink() or out.resolve().is_relative_to(source) or out.resolve().is_relative_to(catalog_path.parent):
        raise ValueError('fresh output outside immutable source/materialization required')
    preservation(pins)
    out.mkdir(mode=0o700, parents=False)
    result = {'kind': 'unitree-native-export-preparation', 'status': 'PREPARED_SOURCE_ONLY',
              'source': str(source), 'source_manifest_sha256': manifest_pin['sha256'],
              'family': family, 'ros_version': version, 'input_pins': pins, 'outputs': [],
              'native_identity_export_executed': False, 'compilers_generators_executed': False,
              'runtime_permissions': [], 'inputs_preserved': False}
    try:
        products = [('selection.hpp', header.encode()), ('selection.cmake', cmake.encode()),
                    ('selection.json', json_bytes(selection)), ('catalog/catalog.preparation.json', catalog_raw),
                    ('source-materialization-report.json', report_raw), ('profile.json', profile_raw),
                    ('native_identity.cpp', bodies['sdk/integrations/unitree/native_identity.cpp']),
                    ('CMakeLists.txt', bodies['sdk/integrations/unitree/CMakeLists.txt']), *copies]
        if len({name for name, _ in products}) != len(products):
            raise ValueError('duplicate prepared output')
        for name, raw in products:
            write(out / relative(name), raw); result['outputs'].append(read(out / name)[1])
        preservation(pins); preservation(result['outputs']); result['inputs_preserved'] = True
    except BaseException as error:
        result.update(status='FAIL', error=repr(error)); raise
    finally:
        write(out / 'preparation.json', json_bytes(result))
    return result


def merge(args):
    """Compose selected complete families and independently bound fixture catalogs.
    Extra independently bound bounded-v2 fixture catalogs may be retained too.
    Merely concatenating four generated headers would redefine macro/array names.
    This step copies data and verifies binding receipts; it is not native proof.
    """
    selected_families = set(args.family)
    if len(selected_families) != len(args.family) or not selected_families <= set(FAMILIES):
        raise ValueError('select each finite Unitree family explicitly once')
    if not 1 <= len(args.bound_catalog) <= 8 or len(args.bound_catalog) != len(args.binding_receipt):
        raise ValueError('selected catalogs need ordered matching actual binding receipts')
    source = args.source.resolve(strict=True)
    manifest_raw, manifest_pin = read(args.source_manifest, 16 * 1024 * 1024)
    if manifest_pin['sha256'] != args.source_manifest_sha256:
        raise ValueError('explicit current source manifest SHA mismatch')
    manifest = document(manifest_raw)
    pins, copies, merged, families, names = [manifest_pin], [], [], set(), set()
    expected_types = {}
    for name in ['sdk/integrations/unitree/prepare.py'] + ['sdk/robots/unitree/profiles/' + family + '.json' for family in FAMILIES]:
        body, pin = read(source / name)
        if not isinstance(manifest.get(name), dict) or any(manifest[name].get(k) != pin[k] for k in ('bytes', 'sha256')):
            raise ValueError('source-bound composition/profile differs')
        pins.append(pin)
        if name.endswith('.json'):
            profile = document(body)
            expected_types[profile['family']] = {(profile['ros_version'], r['type']) for r in profile['definitions']}
    if Path(__file__).resolve(strict=True) != (source / 'sdk/integrations/unitree/prepare.py').resolve(strict=True):
        raise ValueError('execute the exact frozen composition entry')
    for index, (catalog_path, receipt_path) in enumerate(zip(args.bound_catalog, args.binding_receipt)):
        raw, pin = read(catalog_path); receipt_raw, receipt_pin = read(receipt_path)
        pins += [pin, receipt_pin]; catalog, receipt = document(raw), document(receipt_raw)
        if receipt.get('status') != 'NATIVE_IDENTITY_BINDING_SOURCE_ONLY' or receipt.get('output') != {'path': pin['path'], 'sha256': pin['sha256'], 'bytes': pin['bytes']}:
            raise ValueError('actual exact bound catalog receipt required')
        exported = receipt.get('official_export', {}); template = receipt.get('template', {})
        for selected in (exported, template):
            if not isinstance(selected, dict) or set(selected) != {'path', 'bytes', 'sha256'}:
                raise ValueError('original template/export provenance required')
            body, opened = read(selected['path'], 16 * 1024 * 1024)
            if any(opened[k] != selected[k] for k in selected):
                raise ValueError('original template/export changed')
            pins.append(opened)
        if not isinstance(catalog, dict) or set(catalog) != {'format', 'types'} or catalog['format'] != 'uwvm-ros-custom-bounded-v2' or not isinstance(catalog['types'], list):
            raise ValueError('closed bound bounded-v2 catalog required')
        for entry in catalog['types']:
            if not isinstance(entry, dict) or set(entry) - {'ros_version', 'type', 'file', 'bounds', 'native_identity', 'component_identities'} or not {'ros_version', 'type', 'file', 'native_identity'} <= set(entry):
                raise ValueError('closed bound interface row required')
            version = entry['ros_version']
            if type(version) is not int or version not in (1, 2):
                raise ValueError('explicit catalog ROS version required')
            parts = safe_type(entry['type'], version)
            if (version, entry['type']) in names:
                raise ValueError('duplicate/conflicting merged native name')
            names.add((version, entry['type']))
            pattern = r'[0-9a-f]{32}' if version == 1 else r'RIHS01_[0-9a-f]{64}'
            if not isinstance(entry['native_identity'], str) or not re.fullmatch(pattern, entry['native_identity']):
                raise ValueError('bound official identity required')
            raw_definition, definition_pin = definition(Path(catalog_path).absolute().parent, entry['file'])
            if receipt.get('definition_copies', {}).get(entry['file']) != {'bytes': definition_pin['bytes'], 'sha256': definition_pin['sha256']}:
                raise ValueError('definition no longer matches actual binding')
            pins.append(definition_pin)
            new = dict(entry); new['file'] = 'catalog_' + str(index) + '/' + str(relative(entry['file']))
            copies.append((new['file'], raw_definition)); merged.append(new)
            if parts[0] in FAMILIES:
                families.add(parts[0])
    if families != selected_families or not 1 <= len(merged) <= 128:
        raise ValueError('exact explicitly selected families and a finite inventory required')
    # Enforce every complete family, not merely one message bearing its name.
    for family in selected_families:
        version, count = FAMILIES[family]
        actual = {(e['ros_version'], e['type']) for e in merged if e['type'].split('/')[0] == family}
        if len(actual) != count or actual != expected_types[family]:
            raise ValueError('missing/extra Unitree family member')
    out = args.out.absolute()
    if out.exists() or out.is_symlink() or out.resolve().is_relative_to(source) or any(out.resolve().is_relative_to(p.absolute().parent.resolve()) for p in args.bound_catalog):
        raise ValueError('fresh merged output required')
    preservation(pins); out.mkdir(mode=0o700, parents=False)
    result = {'kind': 'unitree-native-bound-catalog-composition', 'status': 'COMPOSED_SOURCE_ONLY',
              'source': str(source), 'source_manifest_sha256': manifest_pin['sha256'],
              'input_pins': pins, 'outputs': [], 'inputs_preserved': False,
              'selected_families': sorted(selected_families),
              'expanded_capacity': 'REQUIRES_ACTUAL_GENERATOR_ADMISSION_WITH_UNCHANGED_LIMITS',
              'native_compatibility': 'NOT_ESTABLISHED', 'runtime_permissions': []}
    try:
        for name, body in copies:
            write(out / relative(name), body); result['outputs'].append(read(out / name)[1])
        write(out / 'catalog.json', json_bytes({'format': 'uwvm-ros-custom-bounded-v2', 'types': merged}))
        result['outputs'].append(read(out / 'catalog.json')[1]); preservation(pins); preservation(result['outputs']); result['inputs_preserved'] = True
    except BaseException as error:
        result.update(status='FAIL', error=repr(error)); raise
    finally:
        write(out / 'composition.json', json_bytes(result))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='mode', required=True)
    p = sub.add_parser('prepare', help='literal native selection, no compiler/generator invocation')
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--source-manifest', type=Path, required=True)
    p.add_argument('--source-manifest-sha256', required=True)
    p.add_argument('--profile', type=Path, required=True)
    p.add_argument('--catalog', type=Path, required=True)
    p.add_argument('--materialization-report', type=Path, required=True)
    p.add_argument('--out', type=Path, required=True)
    p = sub.add_parser('merge', help='compose explicit complete families with optional retained fixtures; generation limits remain unchanged')
    p.add_argument('--source', type=Path, required=True)
    p.add_argument('--source-manifest', type=Path, required=True)
    p.add_argument('--source-manifest-sha256', required=True)
    p.add_argument('--family', choices=sorted(FAMILIES), action='append', required=True)
    p.add_argument('--bound-catalog', type=Path, action='append', required=True)
    p.add_argument('--binding-receipt', type=Path, action='append', required=True)
    p.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    result = prepare(args) if args.mode == 'prepare' else merge(args)
    print(json.dumps(result, indent=2, allow_nan=False))


if __name__ == '__main__':
    main()
