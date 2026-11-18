#!/usr/bin/env python3
"""Run two archived String/SetBool guests against a freshly identified scene VM."""
import argparse
import fcntl
import json
import os
from pathlib import Path
import resource
import time

from run_scene_communication import configured, deployment_fixture, verify_gate, worker_definition
from run_scene_vm import sha


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('runtime', 'build-receipt', 'baseline-run', 'pair-receipt', 'gateway', 'output'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    verify_gate()
    assert os.sched_getaffinity(0).issubset({4, 5, 6, 7})
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    runtime = args.runtime.resolve()
    build = json.loads(args.build_receipt.read_bytes())
    old = json.loads(args.baseline_run.read_bytes())
    pair = json.loads(args.pair_receipt.read_bytes())
    assert build['complete'] is True and pair['complete'] is True and old['complete'] is True
    assert old['scope'].startswith('Actual Wasm -> externally provisioned ROS gateways -> DDS String / SetBool qualification.')
    vm_hash = sha(runtime)
    assert vm_hash == build['vm_sha256'] and build['source_commit']
    assert sha(args.pair_receipt) in old['inputs'].values() and sha(args.gateway) in old['inputs'].values()
    packs, profiles, manifest_pins, input_pins = [], [], [], {}
    for i in range(2):
        folder = args.pair_receipt.parent / f'pack-{i}'
        manifest = folder / 'manifest.json'
        expected = [v for k, v in pair['pack_sha256'].items() if k.endswith(f'/pack-{i}/manifest.json')]
        assert expected == [sha(manifest)] and expected[0] in old['inputs'].values()
        manifest_pins += expected
        pack = json.loads(manifest.read_bytes())
        for image in [pack['launcher'], *pack['files']]:
            original = Path(image['path'])
            assert image['sha256'] == old['inputs'][str(original)]
            # Packed images are named by content and live next to the manifest.
            path = (folder / 'images' / original.name).resolve()
            assert path.stat().st_size == image['bytes'] and sha(path) == image['sha256']
            image['path'] = str(path)
            input_pins[path] = image['sha256']
        profile = args.pair_receipt.parent / f'peer-{i}.json'
        assert sha(profile) in old['inputs'].values()
        definition = json.loads(profile.read_bytes())
        encoded, _ = worker_definition.compile_definition(definition)
        startup, = [r for r in pack['files'] if r['destination'] == '/worker/profile']
        assert Path(startup['path']).read_bytes() == encoded
        packs.append(pack)
        profiles.append(definition)
    assert any('--ros-scene-communication-channel' in c['argv'] for c in old['cases'] if c['passed'])
    flags = configured(packs, profiles)
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    report = {'format': 'uwvm-scene-dds-roundtrip-v1', 'complete': False, 'source': build['source_commit'],
              'vm': vm_hash, 'gateway': sha(args.gateway), 'baselineReceipt': sha(args.baseline_run),
              'pairReceipt': sha(args.pair_receipt), 'buildReceipt': sha(args.build_receipt),
              'manifests': manifest_pins, 'provisioner': sha(deployment_fixture.__file__),
              'cgroup': Path('/proc/self/cgroup').read_text().strip(), 'cases': []}

    def save():
        (out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')

    save()
    # Match the transport-wide lock used by the original verify runner.
    with (args.baseline_run.parent.parent / '.communication-transport-ports.lock').open('a') as reservation:
        fcntl.flock(reservation, fcntl.LOCK_EX)
        for engine, layout in (('int', '32-0'), ('aot', '64-1')):
            name = 'success-' + layout
            guest, = [row for row in old['guests'] if row['name'] == name and row['scenario'] == 'success']
            assert any(c['name'] == name + '-' + engine and c['passed'] is True and c['exit'] == 0 for c in old['cases'])
            wasm = args.baseline_run.parent / (name + '.wasm')
            assert sha(wasm) == guest['sha256'] and sha(wasm.with_suffix('.wat')) == guest['wat_sha256']
            command = [str(runtime), '--wasm-set-main-module-name', 'main', '--runtime-compile-threads', '0',
                       '--runtime-llvm-jit-cache-path', 'disable', '--log-color', 'disable', '--runtime-' + engine,
                       *flags, '--run', str(wasm)]
            start = time.monotonic()
            done = deployment_fixture.execute(command, packs, args.gateway.resolve())
            (out / (engine + '.stdout')).write_text(done.stdout)
            (out / (engine + '.stderr')).write_text(done.stderr)
            report['cases'].append({'engine': engine, 'layout': layout, 'guest': guest['sha256'],
                                    'guestAssertions': guest['assertions'], 'exit': done.returncode,
                                    'seconds': time.monotonic() - start})
            save()
            assert done.returncode == 0, (engine, done.stderr[-1000:])
    assert sha(runtime) == vm_hash and all(sha(p) == digest for p, digest in input_pins.items())
    report['complete'] = True
    save()
    print('PASS paired Jazzy String/SetBool scene guests')


if __name__ == '__main__':
    main()
