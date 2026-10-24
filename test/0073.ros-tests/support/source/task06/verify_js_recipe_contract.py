#!/usr/bin/env python3
"""Check the narrow JS-recipe source/argument proposal without a fake runtime.

The tiny SDK-shaped directory is an argument-path fixture only: its driver is
never executed and no profile/build/Guest/native qualification is inferred.
Actual candidate argparse, Python AST and prototype JS syntax are exercised.
"""
import argparse
import ast
import difflib
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def module(path, name):
    spec = importlib.util.spec_from_file_location(name, path)
    value = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(value)
    return value


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ('proposal', 'prototype', 'out'):
        parser.add_argument('--' + name, type=Path, required=True)
    args = parser.parse_args()
    if os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise RuntimeError('same common finite Linux owner required')
    args.out.mkdir()
    source = args.proposal / 'candidate/sdk/origin'
    before = args.proposal / 'before/sdk/origin'
    files = [source / 'examples/javascript_telemetry/build.py',
             source / 'tools/xmake/examples/managed_backend.py',
             source / 'tools/shared_managed04/profile.py',
             before / 'tools/shared_managed04/profile.py']
    prototypes = sorted(args.prototype.glob('*.mjs'))
    if len(prototypes) != 3:
        raise RuntimeError('exact three owned controller prototypes required')
    relatives = ('examples/javascript_telemetry/build.py',
                 'tools/xmake/examples/managed_backend.py',
                 'tools/shared_managed04/profile.py')
    before_files = [before / relative for relative in relatives]
    pins = [dict(path=str(path), sha256=sha(path), bytes=path.stat().st_size)
            for path in dict.fromkeys([Path(__file__), Path('/usr/bin/node'),
                                      *files, *before_files, *prototypes])]
    cases = []
    for index, path in enumerate(files):
        ast.parse(path.read_bytes(), filename=str(path))
        cases.append(dict(name='python-ast-' + str(index), path=str(path),
                          status='PASS', layer='SOURCE_SYNTAX_ONLY'))

    def run(name, command):
        result = subprocess.run(list(map(str, command)), stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, timeout=30, check=False)
        (args.out / (name + '.stdout')).write_bytes(result.stdout)
        (args.out / (name + '.stderr')).write_bytes(result.stderr)
        cases.append(dict(name=name, argv=list(map(str, command)), exit=result.returncode,
                          status='PASS' if result.returncode == 0 else 'FAIL', layer='ACTUAL_SOURCE_OR_CLI_ONLY'))
        if result.returncode:
            raise RuntimeError('actual source/CLI failure: ' + name)
        return result.stdout

    for index, path in enumerate(files[:2]):
        help_text = run('actual-cli-' + str(index), ['/usr/bin/python3', '-I', '-S', '-B', path, '--help'])
        wanted = b'--recipe' if index == 0 else b'--js-recipe'
        if wanted not in help_text:
            raise RuntimeError('actual candidate did not register its explicit selector')
        if index == 1 and b'--ros2-rmw-service' not in help_text:
            raise RuntimeError('current unrelated ROS2 service selector was lost')
    command = ['/usr/bin/python3', '-I', '-S', '-B', str(files[1]), '--hel']
    refused = subprocess.run(command, capture_output=True, timeout=30, check=False)
    (args.out / 'closed-cli-abbreviation.stdout').write_bytes(refused.stdout)
    (args.out / 'closed-cli-abbreviation.stderr').write_bytes(refused.stderr)
    if refused.returncode != 2:
        raise RuntimeError('current exact managed CLI must reject abbreviated --hel')
    cases.append(dict(name='closed-cli-abbreviation', argv=command, exit=refused.returncode,
                      status='PASS', layer='ACTUAL_SOURCE_OR_CLI_ONLY'))
    run('node-version', ['/usr/bin/node', '--version'])
    for index, path in enumerate(prototypes):
        run('prototype-syntax-' + str(index), ['/usr/bin/node', '--check', path])

    fixture = args.out / 'argument-path-fixture'
    (fixture / 'tools/xmake/examples').mkdir(parents=True)
    driver = fixture / 'tools/xmake/examples/managed_backend.py'
    driver.write_text('# Path fixture only; never executed.\n')
    inputs = args.out / 'input-path-fixture.json'
    inputs.write_text('{"fixture_only":true}\n')
    selected = dict(format='uwvm-managed-example-selection-v1', sdk_root=str(fixture),
                    platform='flexiv', builder='js', backend=2, pure=False,
                    entry='official/robot_states_monitor.mjs', inputs=str(inputs),
                    inputs_sha256=sha(inputs), js_recipe='official/monitor.recipe.json')
    candidate = module(files[2], 'candidate_managed_grammar')
    original = module(files[3], 'original_managed_grammar')

    def check(name, selection, accepted, implementation=candidate, drift=False):
        values = dict(selection)
        arguments = ['-I', '-S', '-B', str(driver), '--sdk-root', str(fixture),
                     '--resolved', str(args.out / 'unused-resolved.json'),
                     '--inputs', str(inputs), '--inputs-sha256', values['inputs_sha256'],
                     '--builder', values['builder'], '--language', 'js', '--backend', '2',
                     '--entry', values['entry'], '--platform', 'flexiv', '--out', str(args.out / 'unused-product')]
        if values.get('js_recipe') is not None:
            arguments += ['--js-recipe', values['js_recipe']]
        if drift:
            arguments[-1] = 'official/changed.recipe.json'
        try:
            implementation.validate_arguments('js', {}, {'consumer': values}, arguments, str(fixture))
            observed = True
        except (ValueError, RuntimeError):
            observed = False
        if observed is not accepted:
            raise RuntimeError('argument fixture outcome differs: ' + name)
        cases.append(dict(name=name, status='PASS', layer='ARGUMENT_GRAMMAR_FIXTURE_ONLY',
                          accepted=observed, no_fixture_driver_executed=True))

    check('new-exact-recipe', selected, True)
    omitted = dict(selected)
    omitted.pop('js_recipe')
    check('unchanged-omitted-recipe', omitted, True)
    check('original-refuses-new-selector', selected, False, original)
    check('recipe-drift-refused', selected, False, drift=True)
    for name, value in (('traversal-refused', '../outside.json'),
                        ('absolute-refused', '/outside.json'),
                        ('script-recipe-refused', 'official/monitor.py')):
        changed = dict(selected, js_recipe=value)
        check(name, changed, False)
    changed = dict(selected, builder='as')
    check('non-js-recipe-refused', changed, False)
    inputs.write_text('{"fixture_only":false}\n')
    check('input-pin-drift-refused', selected, False)
    patch = []
    changes = []
    for relative in relatives:
        previous, changed = before / relative, source / relative
        old, new = previous.read_text(), changed.read_text()
        ast.parse(old, filename=str(previous))
        if old == new:
            raise RuntimeError('review proposal contains an unchanged helper: ' + relative)
        patch.extend(difflib.unified_diff(old.splitlines(keepends=True),
                                         new.splitlines(keepends=True),
                                         fromfile='a/sdk/origin/' + relative,
                                         tofile='b/sdk/origin/' + relative))
        changes.append(dict(path='sdk/origin/' + relative,
                            before=dict(bytes=previous.stat().st_size, sha256=sha(previous)),
                            candidate=dict(bytes=changed.stat().st_size, sha256=sha(changed))))
    review_patch = args.out / 'js-recipe-current.patch'
    review_patch.write_text(''.join(patch))
    manifest = dict(schema='task06-owned-shared-proposal-v1',
                    status='REVIEW_CANDIDATE_NOT_APPLIED_TO_SHARED_PRODUCER',
                    changes=changes,
                    patch=dict(bytes=review_patch.stat().st_size, sha256=sha(review_patch)),
                    test=dict(path=str(Path(__file__)), sha256=sha(Path(__file__))),
                    tests='Actual source/CLI/argument fixtures only; see cases.json',
                    live_shared_edited=False, installed_sdk_builds=0, guest=0, native_ros=0)
    (args.out / 'proposal-manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    for pin in pins:
        if sha(Path(pin['path'])) != pin['sha256']:
            raise RuntimeError('immutable candidate/prototype/tool input changed')
    (args.out / 'source-pins.json').write_text(json.dumps(pins, indent=2) + '\n')
    (args.out / 'cases.json').write_text(json.dumps(cases, indent=2) + '\n')
    summary = dict(status='PASS_SOURCE_CLI_AND_ARGUMENT_FIXTURES_ONLY', cases=len(cases),
                   actual_sdk_builds=0, complete_module_graphs=0, wasm_validation=0,
                   guest=0, native_ros=0, publication_ready=False)
    (args.out / 'summary.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary), flush=True)


if __name__ == '__main__':
    main()
