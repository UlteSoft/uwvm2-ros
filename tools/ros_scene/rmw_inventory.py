#!/usr/bin/env python3
"""Inventory indexed ROS middleware and explicitly supplied native run receipts."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

import rmw_native_wire


DEFAULT_TARGETS = Path(__file__).resolve().parents[1] / 'ros_distro/distributions.json'
PRERELEASES = [*(f'alpha{i}' for i in range(1, 9)), *(f'beta{i}' for i in range(1, 4))]
IMPLEMENTATIONS = {
    'rmw_cyclonedds_cpp': 'Cyclone DDS', 'rmw_connext_cpp': 'Connext DDS (legacy)',
    'rmw_connext_dynamic_cpp': 'Connext DDS (legacy dynamic)', 'rmw_connextdds': 'Connext DDS',
    'rmw_connextddsmicro': 'Connext Micro', 'rmw_fastrtps_cpp': 'Fast DDS',
    'rmw_fastrtps_dynamic_cpp': 'Fast DDS (dynamic)', 'rmw_gurumdds_cpp': 'GurumDDS',
    'rmw_opensplice_cpp': 'OpenSplice', 'rmw_zenoh_cpp': 'Zenoh',
    'rmw_int2dds_cpp': 'int2DDS', 'rmw_swiftdds_cpp': 'SWIFT DDS',
    'rmw_desert': 'DESERT', 'rmw_email_cpp': 'Email',
}
HASH = re.compile(r'[0-9a-f]{64}\Z')
LABEL = re.compile(r'[a-z][a-z0-9_]*\Z')


def require(condition, message):
    if not condition:
        raise ValueError(message)


def load(path):
    data = path.read_bytes()
    return json.loads(data), hashlib.sha256(data).hexdigest()


def targets(catalog):
    rows = {}
    for target in catalog['distributions']:
        number, name = target['backend'], target['distro']
        require(type(number) is int and number in (1, 2) and LABEL.fullmatch(name), 'invalid target')
        require((number, name) not in rows, f'duplicate target {name}')
        rows[number, name] = None
    # Earlier compatibility catalogs start at Ardent.
    for name in PRERELEASES:
        rows.setdefault((2, name), None)
    return [(n, d) for n, d in rows if n == 1] + [(2, d) for d in PRERELEASES] + [
        (n, d) for n, d in rows if n == 2 and d not in PRERELEASES]


def package_index(directory, number, name):
    if number == 1:
        return {'status': 'not-applicable'}
    path = directory / (name + '-packages.json')
    if not path.exists():
        return {'status': 'not-supplied'}
    packages, digest = load(path)
    require(isinstance(packages, list), f'{name}: expected package list')
    selected = {}
    for row in packages:
        require(row['distro'] == name, f'{name}: inconsistent index release')
        package = row['package']
        if not package.startswith('rmw_'):
            continue
        require(LABEL.fullmatch(package) and package not in selected, f'{name}: invalid or duplicate RMW entry')
        selected[package] = {'package': package, 'version': row['version'], 'releaseMarker': row['released'],
                             'knownImplementation': IMPLEMENTATIONS.get(package)}
    return {'status': 'indexed', 'sha256': digest, 'packages': [selected[p] for p in sorted(selected)]}


def sdk_run(path):
    report, digest = load(path)
    require(report['format'] == 'uwvm-actual-native-ros-v1' and report['passed'] is True and
            report['backend'] in ('ros1', 'ros2') and type(report['physical_robot']) is bool,
            'expected a completed SDK v1 native inspection fixture')
    runtime = report['executable_hashes']['runtime']
    require(HASH.fullmatch(runtime), 'missing SDK v1 runtime identity')
    positive = []
    for case in report['cases']:
        require(case['status'] == 'PASS', 'failed SDK v1 case')
        if case['grant'] is True and case['refused'] is False and case['guest_exit'] == 0 and case['native_events']:
            require(HASH.fullmatch(case['artifact_sha256']) and case['address_bits'] in (32, 64),
                    'invalid successful guest identity')
            kinds = [event['kind'] for event in case['native_events']]
            require(all(isinstance(k, str) and k for k in kinds), 'invalid native event kind')
            positive.append({'example': case['path'], 'bits': case['address_bits'], 'wasm': case['artifact_sha256'],
                             'nativeEvents': len(kinds), 'eventKinds': sorted(set(kinds))})
    require(positive, 'no positive SDK v1 native event')
    return (int(report['backend'][-1]), report['distribution']), {
        'interface': 'sdk-v1', 'kind': 'native-guest-events', 'receiptSha256': digest,
        'rmwReportedAtRuntime': None, 'vm': runtime, 'physicalRobot': report['physical_robot'], 'positiveCases': positive}


def scene_run(path, manifests):
    report, digest = load(path)
    require(report['complete'] is True, 'incomplete scene run')
    packs = [load(p) for p in manifests]
    if report.get('scope', '').startswith('Actual Wasm -> externally provisioned ROS gateways -> DDS String / SetBool qualification.'):
        # Historical fixture records pin each pack by its original path. Resolve
        # only caller-supplied paths so a relocated evidence directory works.
        for i, (_, checksum) in enumerate(packs):
            require([v for k, v in report['inputs'].items() if k.endswith(f'/pack-{i}/manifest.json')] == [checksum],
                    'scene worker manifest disagrees with receipt')
        guests = {g['name']: g for g in report['guests'] if g['scenario'] == 'success'}
        positive = []
        for case in report['cases']:
            require(case['passed'] is True, 'failed historical scene case')
            name, _, engine = case['name'].rpartition('-')
            if name in guests and engine in ('int', 'aot'):
                guest = guests[name]
                require(case['exit'] == 0 and guest['assertions'] > 0, 'failed scene guest')
                positive.append({'engine': engine, 'bits': guest['width'], 'memory': guest['memory'], 'wasm': guest['sha256']})
        runtime = report['runtime_sha256']
    elif report.get('format') == 'uwvm-scene-dds-roundtrip-v1':
        require(report['manifests'] == [checksum for _, checksum in packs], 'scene manifest pins differ')
        positive = []
        for case in report['cases']:
            require(case['exit'] == 0 and case['guestAssertions'] > 0 and case['engine'] in ('int', 'aot') and
                    re.fullmatch(r'(32|64)-[01]', case['layout']), 'invalid scene run case')
            bits, memory = map(int, case['layout'].split('-'))
            positive.append({'engine': case['engine'], 'bits': bits, 'memory': memory, 'wasm': case['guest']})
        runtime = report['vm']
    else:
        raise ValueError('unrecognized scene execution receipt')
    require(positive and HASH.fullmatch(runtime) and all(HASH.fullmatch(c['wasm']) for c in positive),
            'missing or invalid scene VM/guest identity')
    releases = set()
    for manifest, _ in packs:
        prefix = manifest['ros_prefix']
        match = re.fullmatch(r'/opt/ros/([a-z][a-z0-9_]*)', prefix)
        require(match, 'unrecognized scene ROS prefix')
        releases.add(match[1])
        expected = prefix + '/lib/librmw_fastrtps_cpp.so'
        require(any(f['destination'] == expected and HASH.fullmatch(f['sha256']) for f in manifest['files']),
                'missing sealed Fast DDS implementation in the worker pack')
    require(len(releases) == 1, 'mixed scene worker distributions')
    return (2, releases.pop()), {'interface': 'scene-v2', 'kind': 'String/SetBool-loopback',
            'receiptSha256': digest, 'vm': runtime, 'workerManifestSha256': [val for _, val in packs],
            'providerProfile': 'rmw_fastrtps_cpp', 'rmwReportedAtRuntime': None, 'positiveCases': positive}


def inventory(catalog, directory, sdk, scene, native_wire=(), remaps=(), native_service=()):
    definition, checksum = load(catalog)
    rows = {(n, d): {'ros': n, 'release': d, 'stage': 'prerelease' if n == 2 and d in PRERELEASES else 'distribution',
                     'index': package_index(directory, n, d), 'executions': []} for n, d in targets(definition)}
    evidences = [sdk_run(p) for p in sdk] + [scene_run(paths[0], paths[1:]) for paths in scene]
    seen = set()
    for key, entry in sorted(evidences, key=lambda item: item[1]['receiptSha256']):
        require(key in rows, f'receipt names unknown release {key}')
        require(entry['receiptSha256'] not in seen, 'duplicate execution receipt')
        rows[key]['executions'].append(entry)
        seen.add(entry['receiptSha256'])
    wire = [rmw_native_wire.verify(p, remaps) for p in native_wire]
    wire.extend(rmw_native_wire.verify_service(p, remaps) for p in native_service)
    require(len({w['receiptSha256'] for w in wire}) == len(wire), 'duplicate native wire receipt')
    for run in wire:
        if run['status'] == 'passed':
            require(all((2, run[role]['distribution']) in rows for role in ('requestPeer', 'replyPeer')),
                    'native wire receipt names unknown distribution')
    return {'format': 'uwvm-ros-rmw-inventory-v1', 'catalogSha256': checksum, 'targets': list(rows.values()),
            'nativeWireRuns': sorted(wire, key=lambda run: run['receiptSha256'])}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--catalog', type=Path, default=DEFAULT_TARGETS)
    parser.add_argument('--package-index-dir', type=Path, required=True, help='directory with <release>-packages.json files')
    parser.add_argument('--sdk-native', type=Path, action='append', default=[], metavar='RECEIPT')
    parser.add_argument('--scene-dds', type=Path, action='append', nargs=3, default=[], metavar=('RECEIPT','PACK_0','PACK_1'))
    parser.add_argument('--native-wire', action='append', default=[], type=Path, metavar='RESULTS',
                        help='native isolated Int32 fixture directory results.json; repeat for independent attempts')
    parser.add_argument('--native-service', action='append', default=[], type=Path, metavar='RESULTS',
                        help='correlated GetParameters run, including optional same-installation controls')
    parser.add_argument('--wire-remap', action='append', nargs=2, type=Path, default=[], metavar=('OLD', 'NEW'),
                        help='relocated input directory for native wire evidence')
    args = parser.parse_args()
    try:
        require(args.package_index_dir.is_dir(), 'package index directory does not exist')
        data = inventory(args.catalog, args.package_index_dir, args.sdk_native, args.scene_dds, args.native_wire, args.wire_remap, args.native_service)
    except (OSError, ValueError, TypeError, KeyError, IndexError) as exc:
        parser.exit(2, f'invalid inventory input: {exc}\n')
    json.dump(data, sys.stdout, indent=2)
    print()


if __name__ == '__main__':
    main()
