#!/usr/bin/env python3
"""Prepare the genuine ROS1 Empty closure and exercise RealMan query models.

Only the admitted finite Linux owner may execute this. The already qualified
private parser/emitter pairing is retained with every guard. This emits source
and runs actual Node whole-codec models, never ROS/Guest or native hardware.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import subprocess


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('paired-source', 'source', 'genmsg', 'records', 'model', 'original', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same admitted finite Linux owner required')
    args.out.mkdir()
    pins = {}

    def capture(path):
        raw = path.read_bytes()
        value = dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
        if pins.setdefault(str(path), value) != value:
            raise RuntimeError('source changed during capture')
        return raw

    for path in (Path(__file__), args.model, args.original, Path('/usr/bin/node'), Path('/usr/share/nodejs/acorn/dist/acorn.mjs')):
        capture(path)
    ast.parse(Path(__file__).read_bytes())
    if sha(args.original) != 'ba6953fdd9dc25ca4b66c6668ec53e26baba44684c35e764b6fd31d431c9ea72':
        raise RuntimeError('exact reviewed RealMan original program required')
    commit = json.loads(capture(args.genmsg / 'commit.json'))
    tree = json.loads(capture(args.genmsg / 'tree.json'))
    original_pins = json.loads(capture(args.genmsg / 'source-pins.json'))
    relative = 'test/files/std_msgs/msg/Empty.msg'
    body = capture(args.genmsg / 'files' / relative)
    copied = capture(args.source / 'std_msgs/Empty.msg')
    if (commit['sha'] != 'f64190bcd7f36ef4ee401041714480473f96742b' or
            commit['commit']['tree']['sha'] != 'fed27ff240cb0ff39516a7f2bd631585d3572b1d' or
            tree['sha'] != commit['commit']['tree']['sha'] or tree.get('truncated') is not False or
            body != b'' or copied != body or original_pins[relative]['bytes'] != 0 or
            original_pins[relative]['sha256'] != hashlib.sha256(body).hexdigest() or
            original_pins[relative]['git_blob_sha1'] != hashlib.sha1(b'blob 0\0').hexdigest()):
        raise RuntimeError('exact whole ROS1 original Empty provenance required')
    entries = [row for row in tree['tree'] if row['path'] == relative]
    if len(entries) != 1 or entries[0]['sha'] != original_pins[relative]['git_blob_sha1'] or entries[0]['type'] != 'blob':
        raise RuntimeError('actual original tree/blob association differs')
    source = args.paired_source
    required = ['sdk/origin/robots/industrial/prepare.py', 'sdk/origin/robots/industrial/vendor_profile.py',
                'sdk/origin/tools/custom_types.py', 'sdk/origin/custom-types/bounded_profile.py',
                'sdk/tools/cpp_profiles.py', 'test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_identity.cc']
    required += ['sdk/origin/custom-types/interface_templates/bounded_interfaces.' + ext + '.in' for ext in ('rs', 'mjs', 'go', 'ts')]
    for relative in required:
        capture(source / relative)
    catalog = args.source / 'catalog.json'
    document = json.loads(capture(catalog))
    if document != {'format': 'uwvm-ros-vendor-complete-bounded-v1', 'types': [
            {'ros_version': 1, 'type': 'std_msgs/Empty', 'file': 'std_msgs/Empty.msg', 'registered': False, 'bounds': {}}]}:
        raise RuntimeError('one exact complete original unregistered ROS1 Empty source dependency required')
    commands = []

    def run(name, command):
        folder = args.out / name
        folder.mkdir()
        with (folder / 'stdout').open('xb') as out, (folder / 'stderr').open('xb') as err:
            result = subprocess.run(list(map(str, command)), stdin=subprocess.DEVNULL, stdout=out, stderr=err, timeout=120, check=False)
        commands.append(dict(name=name, argv=list(map(str, command)), exit=result.returncode,
                             stdout_sha256=sha(folder / 'stdout'), stderr_sha256=sha(folder / 'stderr')))
        (args.out / 'commands.json').write_text(json.dumps(commands, indent=2) + '\n')
        if result.returncode:
            raise RuntimeError('actual generation/syntax/whole-model failed: ' + name)

    prepared = args.out / 'prepared-empty'
    command = ['/usr/bin/python3', '-I', '-S', '-B', source / required[0], '--source-root', source,
               '--catalog', catalog, '--out', prepared, '--mode', 'prepare']
    for name, relative in (('parent', 'sdk/origin/tools/custom_types.py'),
                           ('bounded', 'sdk/origin/custom-types/bounded_profile.py'),
                           ('adapter', 'sdk/origin/robots/industrial/vendor_profile.py')):
        command += ['--' + name + '-sha256', sha(source / relative)]
    run('prepare-empty', command)
    receipt = json.loads((prepared / 'preparation.vendor.json').read_bytes())
    generation = receipt['generation']
    if (receipt['compiler_executed'] is not False or receipt['Guest_executed'] is not False or
            generation['native_registry_emitted'] is not False or len(generation['records']) != 1 or
            generation['records'][0]['type'] != 'std_msgs/Empty'):
        raise RuntimeError('complete Empty generation scope differs')
    for name, fact in generation['outputs'].items():
        output = prepared / 'generated' / name
        if output.stat().st_size != fact['bytes'] or sha(output) != fact['sha256']:
            raise RuntimeError('actual generated complete source differs from receipt')
    root = args.out / 'source-model'
    base = root / 'sdk/origin/platform/realman/query-core'
    for name in ('get_arm_state.mjs', 'api_get_arm_state.mjs'):
        target = base / 'prototype' / name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(capture(args.source / 'prototype' / name))
    target = base / 'generated/bounded_records.mjs'
    target.parent.mkdir(parents=True)
    target.write_bytes((prepared / 'generated/bounded_records.mjs').read_bytes())
    records = root / 'sdk/origin/custom-types/realman-ros1-develop-complete/bounded_records.mjs'
    records.parent.mkdir(parents=True)
    records.write_bytes(capture(args.records))
    run('node-version', ['/usr/bin/node', '--version'])
    run('entry-syntax', ['/usr/bin/node', '--check', base / 'prototype/api_get_arm_state.mjs'])
    run('complete-query-model', ['/usr/bin/node', args.model, '--root', root, '--out', args.out / 'model.json'])
    if any(sha(Path(path)) != value['sha256'] for path, value in pins.items()):
        raise RuntimeError('immutable original/source/tool changed')
    model = json.loads((args.out / 'model.json').read_bytes())
    summary = dict(status=model['status'], whole_generated_records=1, source_outputs=generation['implemented_record_outputs'],
                   host_model_cases=len(model['cases']), public_api_items=len(model['public_api_inventory']),
                   sdk_builds=0, wasm_guest=0, native_ros=0, original_processes=0, physical_motion=False,
                   publication_ready=False, native_identity_emitted=False)
    (args.out / 'source-pins.json').write_text(json.dumps(list(pins.values()), indent=2) + '\n')
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
