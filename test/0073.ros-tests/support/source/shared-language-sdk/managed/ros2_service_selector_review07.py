#!/usr/bin/env python3
"""Exercise the closed service selector and actual shared binary-import gate.

All runs require the original admitted Linux owner. Protocol fixtures and WAT
import fixtures are not language applications, Guests or native ROS evidence.
The optional gate stays explicitly pending until the real shared checker has
its formal interface; no private checker implementation substitutes for it.
"""

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys


def pin(path):
    with path.open('rb') as stream:
        digest = hashlib.file_digest(stream, 'sha256').hexdigest()
    return dict(path=str(path.resolve()), bytes=path.stat().st_size, sha256=digest)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--validator', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    for name in ('node', 'asc', 'tool-inputs'):
        parser.add_argument('--' + name, type=Path)
    parser.add_argument('--tool-inputs-sha256')
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('the original admitted Linux owner is required')
    sdk = args.source.resolve(strict=True) / 'sdk/origin'
    args.out.mkdir(parents=True, exist_ok=False)
    source_paths = [sdk / relative for relative in (
        'tools/shared_managed04/profile.py', 'tools/xmake/examples/managed_backend.py',
        'tools/xmake/managed.lua', 'tools/xmake/examples/managed.lua',
        'tools/check_wasm_imports.py', 'abi/abi-v1.json', 'abi/rmw-service-v1.json',
        'tools/rmw_service_contract.py', 'assemblyscript/ros2/rmw_service/raw.ts', 'assemblyscript/api.ts')]
    sources = [pin(path) for path in source_paths]
    spec = importlib.util.spec_from_file_location('_service_grammar', source_paths[0])
    grammar = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(grammar)
    report = dict(status='STARTED', sources=sources, grammar_checks=[], backend_checks=[],
                  import_checks=[], commands=[], actual_language_products=0,
                  guest_executions=0, native_ros_executions=0, default_authority=[])

    def save():
        (args.out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')

    def run(label, argv, passed=True):
        result = subprocess.run(list(map(str, argv)), cwd=args.out,
                                stdin=subprocess.DEVNULL, capture_output=True, timeout=30, check=False)
        for name in ('stdout', 'stderr'):
            (args.out / (label + '.' + name)).write_bytes(getattr(result, name))
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=result.returncode))
        save()
        if (result.returncode == 0) is not passed:
            raise RuntimeError('actual command outcome differs: ' + label)
        return result

    try:
        fixture = args.out / 'protocol-fixture-SDK'
        driver = fixture / 'tools/xmake/examples/managed_backend.py'
        driver.parent.mkdir(parents=True)
        shutil.copyfile(source_paths[1], driver)
        runtime = dict(source_ready=True, name='AssemblyScript', route='assemblyscript', version='0.28.20',
                       language='as', target='wasm32-wasip1', address_bits=32,
                       startup='command', exceptions=False, mode='linear-memory')
        inputs = args.out / 'protocol-inputs.json'
        inputs.write_text(json.dumps(dict(format='uwvm-managed-example-inputs-v1', language='as',
                                         runtime=runtime, paths={}, directories=[], files=[])) + '\n')
        selection = dict(format='uwvm-managed-example-selection-v1', builder='as', sdk_root=str(fixture),
                         backend=2, entry='protocol-only.ts', pure=False,
                         inputs=str(inputs), inputs_sha256=pin(inputs)['sha256'])
        profile = dict(target='wasm32-wasip1', address_bits=32, wasi='wasip1', core_baseline='3.0',
                       startup='command', exceptions=False, threads=False)
        provider = dict(driver='uwvm-managed-sdk-consumer-v1', compiler_argv=[], linker_argv=[],
                        runtime_identity=grammar.runtime_identity(runtime))
        resolved_path = args.out / 'protocol-resolved.json'
        output = args.out / 'forbidden-protocol-product'

        def words(selected):
            return ['-I', '-S', '-B', str(driver), '--sdk-root', str(fixture),
                    '--resolved', str(resolved_path), '--inputs', str(inputs),
                    '--inputs-sha256', selected['inputs_sha256'], '--builder', selected['builder'],
                    '--language', 'as', '--backend', str(selected['backend']),
                    '--entry', selected['entry'], '--out', str(output)]

        def check_grammar(label, selected, argv, expected):
            accepted = True
            try:
                options = dict(runtime=runtime, consumer=selected)
                grammar.validate_provider('as', profile, options, provider)
                original = list(argv)
                grammar.validate_arguments('as', profile, options, argv, args.out)
                if argv != original:
                    raise RuntimeError('closed selector rewrote caller arguments')
            except ValueError:
                accepted = False
            if accepted is not expected:
                raise RuntimeError('actual grammar outcome differs: ' + label)
            report['grammar_checks'].append(dict(label=label, accepted=accepted, passed=True))

        check_grammar('legacy-default-off', selection, words(selection), True)
        disabled = dict(selection, ros2_rmw_service=False)
        enabled = dict(selection, ros2_rmw_service=True)
        check_grammar('explicit-false', disabled, words(disabled), True)
        check_grammar('explicit-ros2-true', enabled, words(enabled) + ['--ros2-rmw-service'], True)
        check_grammar('unselected-flag', selection, words(selection) + ['--ros2-rmw-service'], False)
        check_grammar('selected-flag-omitted', enabled, words(enabled), False)
        check_grammar('duplicate-flag', enabled, words(enabled) + ['--ros2-rmw-service'] * 2, False)
        for index, value in enumerate(('true', 1, 0, None)):
            selected = dict(selection, ros2_rmw_service=value)
            check_grammar('nonboolean-' + str(index), selected, words(selected), False)
        for label, changed in [('ros1', dict(backend=1)), ('pure', dict(pure=True)),
                               ('other-builder', dict(builder='lua'))]:
            selected = dict(enabled, **changed)
            check_grammar(label, selected, words(selected) + ['--ros2-rmw-service'], False)
        for index, extra in enumerate((['--allow-module', 'uwvm_ros2_rmw_srv_v1'],
                                        ['--catalog', '/tmp/not-selected.json'],
                                        ['--ros2-rmw-service=true'])):
            check_grammar('open-grammar-' + str(index), enabled,
                          words(enabled) + ['--ros2-rmw-service'] + extra, False)

        # Valid protocol selections must reach the next real closed input gate,
        # never a compiler. Invalid selections must fail before that gate.
        cases = [('default-off', selection, [], 'closed real backend input names required'),
                 ('ros2-selected', enabled, ['--ros2-rmw-service'], 'closed real backend input names required'),
                 ('ros1-selected', dict(enabled, backend=1), ['--ros2-rmw-service'],
                  'optional ROS2 service signatures require'),
                 ('cli-mismatch', enabled, [], 'resolved optional ROS2 service boolean differs'),
                 ('duplicate', enabled, ['--ros2-rmw-service'] * 2, 'duplicate optional ROS2 service selector'),
                 ('abbreviation', enabled, ['--ros2-rmw-ser'], 'unrecognized arguments'),
                 ('arbitrary-catalog', enabled, ['--ros2-rmw-service', '--catalog', '/tmp/unselected.json'],
                  'unrecognized arguments')]
        for label, selected, extra, reason in cases:
            resolved_path.write_text(json.dumps(dict(format='uwvm-sdk-resolved-profile-v1',
                driver='uwvm-managed-sdk-consumer-v1', language='as', sdk_root=str(fixture),
                profile=profile, default_authority=[], language_options=dict(runtime=runtime, consumer=selected))) + '\n')
            result = run('backend-' + label, [sys.executable, *words(selected), *extra], False)
            if reason.encode() not in result.stderr or output.exists():
                raise RuntimeError('backend crossed the wrong boundary: ' + label)
            report['backend_checks'].append(dict(label=label, expected_reason=reason,
                                                 producer_output_created=False, passed=True))

        checker_sdk = args.out / 'checker-SDK'
        checker = checker_sdk / 'tools/check_wasm_imports.py'
        checker.parent.mkdir(parents=True)
        abi = checker_sdk / 'share/abi'
        abi.mkdir(parents=True)
        shutil.copyfile(source_paths[4], checker)
        shutil.copyfile(source_paths[7], checker.parent / 'rmw_service_contract.py')
        shutil.copyfile(source_paths[5], abi / 'abi-v1.json')
        catalog = source_paths[6].read_bytes()
        optional = abi / 'rmw-service-v1.json'
        optional.write_bytes(catalog)
        declarations = json.loads(catalog)['modules']['uwvm_ros2_rmw_srv_v1']['functions']
        if len(declarations) != 12:
            raise ValueError('capture the real12 optional service declarations')
        run('wasm-tools-version', [args.validator, '--version'])
        def import_fixture(label, functions, module='uwvm_ros2_rmw_srv_v1'):
            wat, binary = args.out / (label + '.wat'), args.out / (label + '.wasm')
            wat.write_text('(module\n' + ''.join('(import ' + json.dumps(module) + ' ' + json.dumps(row['name']) +
                ' (func (param ' + ' '.join(row['params']) + ') (result ' + ' '.join(row['results']) + ')))\n'
                for row in functions) + ')\n')
            run('compile-' + label, [args.validator, 'parse', wat, '-o', binary])
            run('validate-' + label, [args.validator, 'validate', binary])
            return binary
        wasm = import_fixture('actual-service-imports', declarations)
        report['actual_validated_wat_fixture'] = pin(wasm)
        run('default-refuses-optional-imports', [sys.executable, '-I', '-S', '-B', checker,
                                               wasm, '--backend', '2'], False)
        run('module-allowance-does-not-select-optional', [sys.executable, '-I', '-S', '-B', checker,
            wasm, '--backend', '2', '--allow-module', 'uwvm_ros2_rmw_srv_v1'], False)
        help_result = run('shared-checker-help', [sys.executable, '-I', '-S', '-B', checker, '--help'])
        if b'--ros2-rmw-service' not in help_result.stdout:
            run('selected-unavailable-shared-gate', [sys.executable, '-I', '-S', '-B', checker,
                                                    wasm, '--backend', '2', '--ros2-rmw-service'], False)
            report['optional_signature_gate'] = 'PENDING_REAL_SHARED_CHECKER_INTERFACE'
        else:
            selected_report = args.out / 'selected-imports.json'
            run('selected-optional-imports', [sys.executable, '-I', '-S', '-B', checker,
                wasm, '--backend', '2', '--ros2-rmw-service', '--report', selected_report])
            observed = json.loads(selected_report.read_bytes())
            expected = {row['name']: (row['params'], row['results']) for row in declarations}
            actual = observed['imports']
            if (len(actual) != 12 or any(row['module'] != 'uwvm_ros2_rmw_srv_v1' or row['kind'] != 0 or
                    (row['params'], row['results']) != expected.get(row['name']) for row in actual)):
                raise ValueError('actual shared checker did not retain the exact12 parsed import signatures')
            report['import_checks'].append(dict(label='selected12', actual_report=pin(selected_report), passed=True))
            options = (args.node, args.asc, args.tool_inputs, args.tool_inputs_sha256)
            if any(value is not None for value in options):
                if not all(value is not None for value in options):
                    raise ValueError('complete explicit real AS compiler closure selection required')
                if pin(args.tool_inputs)['sha256'] != args.tool_inputs_sha256:
                    raise ValueError('selected actual AS compiler receipt changed')
                closure = json.loads(args.tool_inputs.read_bytes())
                if closure['language'] != 'as' or closure['runtime']['version'] != '0.28.20':
                    raise ValueError('the actual pinned AS0.28.20 closure is required')
                admitted = {row['path']: row for row in closure['files']}
                for row in admitted.values():
                    if pin(Path(row['path'])) != row:
                        raise ValueError('actual AS compiler/dependency bytes changed')
                for tool in (args.node, args.asc, args.validator):
                    actual_pin = pin(tool)
                    if admitted.get(actual_pin['path']) != actual_pin:
                        raise ValueError('selected actual compiler tool is outside its complete receipt')
                assembly = args.out / 'actual-AS-raw12-fixture'
                assembly.mkdir()
                shutil.copyfile(source_paths[8], assembly / 'raw.ts')
                shutil.copyfile(source_paths[9], assembly / 'api.ts')
                entry = assembly / 'entry.ts'
                entry.write_text('export { abortGuest } from "./api";\nexport { ' + ', '.join(row['name'] for row in declarations) +
                                 ' } from "./raw";\n')
                binary = assembly / 'raw12.wasm'
                run('actual-asc-version', [args.node, '--max-old-space-size=256', args.asc, '--version'])
                run('actual-AS-raw12-compile', [args.node, '--max-old-space-size=256', args.asc, entry,
                    '--outFile', binary, '--runtime', 'incremental', '--optimize', '--debug',
                    '--use', 'abort=actual-AS-raw12-fixture/entry/abortGuest'])
                run('actual-AS-raw12-validate', [args.validator, 'validate', binary])
                raw_report = assembly / 'actual-imports.json'
                run('actual-AS-raw12-selected-checker', [sys.executable, '-I', '-S', '-B', checker,
                    binary, '--backend', '2', '--ros2-rmw-service', '--allow-module', 'wasi_snapshot_preview1',
                    '--report', raw_report])
                run('actual-AS-raw12-default-refusal', [sys.executable, '-I', '-S', '-B', checker,
                    binary, '--backend', '2', '--allow-module', 'wasi_snapshot_preview1'], False)
                result = json.loads(raw_report.read_bytes())
                if (result.get('optional_functions_checked') != 12 or result.get('default_authority') != []):
                    raise ValueError('actual AS product did not retain all12 checked optional imports')
                if any(pin(Path(row['path'])) != row for row in admitted.values()):
                    raise ValueError('actual complete AS compiler closure changed during compilation')
                report['actual_AS_raw12_compiler_fixture'] = dict(artifact=pin(binary),
                    checker_report=pin(raw_report), language_application_credit=0, guest_credit=0)
            run('ros1-refuses-optional-imports', [sys.executable, '-I', '-S', '-B', checker,
                                                wasm, '--backend', '1', '--ros2-rmw-service'], False)
            run('dual-refuses-optional-selection', [sys.executable, '-I', '-S', '-B', checker,
                                                  wasm, '--backend', 'dual', '--ros2-rmw-service'], False)
            for label, functions, module in (
                    ('unknown-name', [dict(declarations[0], name='not_a_service_function'), *declarations[1:]],
                     'uwvm_ros2_rmw_srv_v1'),
                    ('wrong-signature', [dict(declarations[0], results=['i64']), *declarations[1:]],
                     'uwvm_ros2_rmw_srv_v1'),
                    ('unselected-namespace', declarations, 'uwvm_ros2_rmw_srv_v2')):
                binary = import_fixture(label, functions, module)
                run('refuse-' + label, [sys.executable, '-I', '-S', '-B', checker,
                    binary, '--backend', '2', '--ros2-rmw-service'], False)
            optional.unlink()
            run('missing-catalog-refuses', [sys.executable, '-I', '-S', '-B', checker,
                                           wasm, '--backend', '2', '--ros2-rmw-service'], False)
            optional.write_bytes(catalog)
            changed = json.loads(catalog)
            changed['kind'] = 'not-the-selected-catalog'
            optional.write_text(json.dumps(changed))
            run('changed-catalog-refuses', [sys.executable, '-I', '-S', '-B', checker,
                                           wasm, '--backend', '2', '--ros2-rmw-service'], False)
            optional.write_bytes(catalog)
            report['optional_signature_gate'] = 'ACTUAL_SELECTED12_IMPORT_FIXTURE_AND_CATALOG_REFUSALS_PASS'
        if any(pin(Path(row['path'])) != row for row in sources):
            raise ValueError('captured production source changed')
        report['status'] = 'CLOSED_SELECTOR_CHECKS_PASS_SEE_SEPARATE_OPTIONAL_GATE_STATUS'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
