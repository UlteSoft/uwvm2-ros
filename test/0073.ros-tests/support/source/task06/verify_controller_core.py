#!/usr/bin/env python3
"""Prepare original controller records and exercise real JS data kernels.

This is finite Linux source generation and host model qualification only.
The exact Task01 pin proposal is applied to a private source copy, never to
the live shared tree. No ROS runtime, stub, motion, SDK build or Guest is used.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path, value):
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')


def need(condition, message):
    if not condition:
        raise RuntimeError(message)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('snapshot', 'core', 'upstream', 'vendor-foxy', 'prototype',
                 'pin-proposal', 'pin-patch', 'pin-evidence', 'model', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    need(os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') == '1', 'same common finite Linux owner required')
    args.out.mkdir()
    pins = {}
    cases = []

    def capture(path):
        path = path.resolve(strict=True)
        need(path.is_file() and not path.is_symlink(), 'ordinary explicit source required')
        raw = path.read_bytes()
        value = dict(path=str(path), bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest())
        need(pins.setdefault(str(path), value) == value, 'source changed during capture')
        return raw

    def passed(name, layer, **facts):
        cases.append(dict(name=name, layer=layer, status='PASS', **facts))
        save(args.out / 'cases.json', cases)

    def run(name, command, layer):
        folder = args.out / name
        folder.mkdir()
        with (folder / 'stdout').open('xb') as out, (folder / 'stderr').open('xb') as err:
            result = subprocess.run(list(map(str, command)), stdin=subprocess.DEVNULL,
                                    stdout=out, stderr=err, timeout=120, check=False)
        cases.append(dict(name=name, layer=layer, argv=list(map(str, command)),
                          exit=result.returncode, status='PASS' if result.returncode == 0 else 'FAIL',
                          stdout_sha256=sha(folder / 'stdout'), stderr_sha256=sha(folder / 'stderr')))
        save(args.out / 'cases.json', cases)
        need(result.returncode == 0, 'actual source/model command failed: ' + name)

    capture(Path(__file__))
    capture(args.model)
    capture(Path('/usr/bin/node'))
    capture(Path('/usr/share/nodejs/acorn/dist/acorn.mjs'))
    required = ['sdk/origin/tools/custom_types.py', 'sdk/origin/custom-types/bounded_profile.py',
                'sdk/origin/robots/industrial/vendor_profile.py', 'sdk/origin/robots/industrial/prepare.py',
                'sdk/tools/cpp_profiles.py',
                'test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_identity.cc']
    required += ['sdk/origin/custom-types/interface_templates/bounded_interfaces.' + ext + '.in'
                 for ext in ('rs', 'mjs', 'go', 'ts')]
    paired = args.out / 'paired-source'
    paired.mkdir()
    for relative in required:
        body = capture(args.snapshot / relative)
        target = paired / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(body)

    proposal = json.loads(capture(args.pin_proposal))
    patch = capture(args.pin_patch)
    evidence = json.loads(capture(args.pin_evidence))
    old = 'b0c207057ddfe4790aa3b97e3f89bf905339e895031cb81097904c19690d356e'
    new = 'e3fded09942c32997ceac7a9d2fcdee1911230b22b45d6b52717d3bf4b88ef89'
    need(proposal['ready_for_task10_semantic_pin_update'] is True and
         proposal['emitter_sha256'] == new and proposal['patch_sha256'] == hashlib.sha256(patch).hexdigest(),
         'exact documented Task01 source pairing proposal required')
    need(evidence['status'] == 'PASS' and evidence['emitter_sha256'] == new and
         len(evidence['replays']) == 4 and all(row['cpp_bytes_and_semantic_guards_identical'] for row in evidence['replays']) and
         len(evidence['commands']) == 8 and all(row['exit_code'] == 0 for row in evidence['commands']),
         'Task01 actual documented emitter evidence differs')
    need(sha(paired / 'sdk/tools/cpp_profiles.py') == new, 'actual paired emitter differs from reviewed source')
    bounded = paired / 'sdk/origin/custom-types/bounded_profile.py'
    before = bounded.read_bytes()
    old_line = ('CPP_PROFILE_EMITTER_SHA256 = "' + old + '"').encode()
    new_line = ('CPP_PROFILE_EMITTER_SHA256 = "' + new + '"').encode()
    need(before.count(old_line) == 1 and patch.count(b'-' + old_line) == 1 and patch.count(b'+' + new_line) == 1,
         'exact one reviewed pin assignment required; no broad hash substitution')
    after = before.replace(old_line, new_line, 1)
    before_ast, after_ast = ast.parse(before), ast.parse(after)
    matches = [node for node in before_ast.body if isinstance(node, ast.Assign) and
               any(isinstance(target, ast.Name) and target.id == 'CPP_PROFILE_EMITTER_SHA256' for target in node.targets)]
    need(len(matches) == 1 and isinstance(matches[0].value, ast.Constant) and matches[0].value.value == old,
         'one literal emitter assignment required')
    matches[0].value.value = new
    need(ast.dump(before_ast, include_attributes=False) == ast.dump(after_ast, include_attributes=False),
         'private semantic rebase changed more than the reviewed emitter pin')
    bounded.write_bytes(after)
    save(args.out / 'private-pairing.json', dict(
        live_shared_source_edited=False, source=proposal['path'],
        captured_before_sha256=hashlib.sha256(before).hexdigest(),
        private_after_sha256=hashlib.sha256(after).hexdigest(), emitter_sha256=new,
        task01_proposal_sha256=sha(args.pin_proposal), task01_patch_sha256=sha(args.pin_patch),
        task01_evidence_sha256=sha(args.pin_evidence), scope='Private complete-core preparation only'))
    passed('exact-task01-private-pairing', 'SOURCE_PAIRING_ONLY')

    commits = {
        'humble': ('08434490d3e4abd37ccc5462cdde69b88853b5b8', '82776fc9068d1e0cd4af11c2e2700195994b2eb1'),
        'foxy': ('6356bc82f3a034f5d2c61c6760df0007a6cadfb0', 'ee04e046132be9d9fdcdaeff8dd54a5d0d9aa022'),
    }
    provenance = []
    for distribution, expected_commits in commits.items():
        catalog = args.core / distribution / 'catalog.json'
        document = json.loads(capture(catalog))
        need(document['format'] == 'uwvm-ros-vendor-complete-bounded-v1' and len(document['types']) == 9,
             'exact nine-type complete application closure required')
        domains = {}
        for domain, expected in zip(('common', 'rcl'), expected_commits):
            root = args.upstream / (domain + '_' + distribution)
            receipt = json.loads(capture(root / 'capture.json'))
            need(receipt['commit'] == expected, 'original distribution/commit changed')
            domains[domain] = (root, {row['path']: row for row in receipt['files']})
        for row in document['types']:
            relative = row['file']
            root, inventory = domains['rcl' if relative.startswith('builtin_interfaces/') else 'common']
            original = capture(root / 'files' / relative)
            copied = capture(catalog.parent / relative)
            fact = inventory[relative]
            blob = hashlib.sha1(b'blob ' + str(len(original)).encode() + b'\0' + original).hexdigest()
            need(copied == original and len(original) == fact['bytes'] and
                 hashlib.sha256(original).hexdigest() == fact['sha256'] and blob == fact['git_blob_sha1'],
                 'whole original definition/commit/blob/extent/hash differs: ' + relative)
            provenance.append(dict(distribution=distribution, type=row['type'], definition=relative,
                                   bytes=len(original), sha256=fact['sha256'], git_blob_sha1=blob))
        product = args.out / ('prepared-' + distribution)
        command = ['/usr/bin/python3', '-I', '-S', '-B', paired / 'sdk/origin/robots/industrial/prepare.py',
                   '--source-root', paired, '--catalog', catalog, '--out', product, '--mode', 'prepare']
        for name, relative in (('parent', 'sdk/origin/tools/custom_types.py'),
                               ('bounded', 'sdk/origin/custom-types/bounded_profile.py'),
                               ('adapter', 'sdk/origin/robots/industrial/vendor_profile.py')):
            command += ['--' + name + '-sha256', sha(paired / relative)]
        run('prepare-' + distribution, command, 'ACTUAL_COMPLETE_SOURCE_GENERATION_ONLY')
        report = json.loads((product / 'preparation.vendor.json').read_bytes())
        proof = report['generation']
        need(report['compiler_executed'] is False and report['Guest_executed'] is False and
             proof['native_registry_emitted'] is False and len(proof['records']) == 9 and
             set(proof['implemented_record_outputs']) == {'c', 'cpp', 'rust', 'javascript', 'assemblyscript', 'tinygo'},
             'source preparation scope or complete inventory changed')
        need({item['type'] for item in proof['records']} == {item['type'] for item in document['types']},
             'original full type inventory was projected')
        for name, fact in proof['outputs'].items():
            path = product / 'generated' / name
            need(path.stat().st_size == fact['bytes'] and sha(path) == fact['sha256'], 'generated body differs from receipt')
        passed(distribution + '-whole-original-nine-type-closure', 'SOURCE_AND_PROVENANCE_ONLY',
               preparation_sha256=sha(product / 'preparation.vendor.json'))
    save(args.out / 'original-definition-provenance.json', provenance)

    runtime = args.out / 'host-model-source/sdk/origin'
    prototype = runtime / 'platform/flexiv/controller-core/prototype'
    prototype.mkdir(parents=True)
    for name in ('trajectory.mjs', 'sine.mjs', 'recorded_controllers.mjs'):
        (prototype / name).write_bytes(capture(args.prototype / name))
    for distribution in commits:
        target = prototype.parent / 'generated' / distribution / 'bounded_records.mjs'
        target.parent.mkdir(parents=True)
        shutil.copyfile(args.out / ('prepared-' + distribution) / 'generated/bounded_records.mjs', target)
    vendor = runtime / 'custom-types/flexiv-ros2-foxy-complete/bounded_records.mjs'
    vendor.parent.mkdir(parents=True)
    vendor.write_bytes(capture(args.vendor_foxy))
    run('actual-node-version', ['/usr/bin/node', '--version'], 'ACTUAL_HOST_TOOL_IDENTITY')
    run('host-complete-controller-model', ['/usr/bin/node', args.model, '--root', runtime, '--out', args.out / 'model.json'],
        'HOST_COMPLETE_RECORD_MODEL_ONLY')
    model = json.loads((args.out / 'model.json').read_bytes())
    need(model['status'] == 'PASS_HOST_MODEL_ONLY' and all(row['status'] == 'PASS' for row in model['cases']),
         'actual complete-codec controller model failed')
    for value in pins.values():
        need(sha(Path(value['path'])) == value['sha256'], 'immutable source/tool changed during preparation/model')
    save(args.out / 'source-pins.json', list(pins.values()))
    summary = dict(status='PASS_COMPLETE_SOURCE_AND_HOST_CONTROLLER_MODEL_ONLY',
                   cases=len(cases), host_model_cases=len(model['cases']),
                   api_inventory_items=len(model['public_api_inventory']),
                   original_definition_bodies=len(provenance), generated_source_families=2,
                   source_output_languages=['c', 'cpp', 'rust', 'javascript', 'assemblyscript', 'tinygo'],
                   actual_sdk_builds=0, wasm_validation=0, guest=0, native_ros=0,
                   original_processes=0, publication_ready=False)
    save(args.out / 'summary.json', summary)
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
