#!/usr/bin/env python3
"""Actual complete MoonBit source generation and canonical-integrity refusals.

Run only under the shared admitted Linux owner. No compiler, Guest or native
ROS application executes. Negative fixtures are explicitly altered proof data.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import sys
import types


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--canonical', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    if sys.platform != 'linux' or os.environ.get('UWVM_RESOURCE_HEAVY_LOCK_HELD') != '1':
        raise ValueError('shared admitted Linux finite owner required')
    source, canonical = args.source.resolve(strict=True), args.canonical.resolve(strict=True)
    sdk = source / 'sdk/origin'
    operator = sdk / 'moonbit/tools/generate_control_moonbit.py'
    roles = dict(parent=sdk / 'tools/custom_types.py', bounded=sdk / 'custom-types/bounded_profile.py',
        emitter=sdk / 'custom-types/moonbit_profile.py', services=sdk / 'moonbit/tools/control_services_moonbit.py',
        runtime=sdk / 'custom-types/moonbit_runtime.mbt.in', operator=operator,
        catalog=sdk / 'rmw_wasm/default_control/catalog.preparation.json', wire=sdk / 'moonbit/wire/wire.mbt')
    expected = {role: digest(path) for role, path in roles.items()}
    raw = operator.read_bytes()
    module = types.ModuleType('_review02_actual_moonbit_operator')
    module.__file__ = str(operator)
    sys.modules[module.__name__] = module
    exec(compile(raw, str(operator), 'exec', dont_inherit=True), module.__dict__)
    args.out.mkdir(parents=True, exist_ok=False)
    original = json.loads((canonical / 'generation.json').read_bytes())
    report = dict(format='uwvm-moonbit-control-review02-v1', status='STARTED', cases=[],
        expected_roles=expected, source_inputs=[
            dict(path=str(p.relative_to(source)), bytes=p.stat().st_size, sha256=digest(p))
            for p in sorted(source.rglob('*')) if p.is_file()],
        external_canonical=dict(path=str(canonical), manifest_sha256=digest(canonical / 'generation.json'),
            outputs=original['outputs']), compiler_cases=0, guest_cases=0, native_ros_cases=0, default_authority=[])

    def save():
        (args.out / 'results.json').write_text(json.dumps(report, indent=2) + '\n')

    try:
        proof = module.generate(source, roles['catalog'], canonical, args.out / 'complete', expected)
        if (len(proof['complete_records']) != 32 or len(proof['complete_services']) != 7 or
                proof['implicit_permissions'] != [] or proof['native_registry_emitted'] is not False or
                len(proof['canonical_helper_inputs']) != 3):
            raise ValueError('whole32/seven original source and complete current helper closure required')
        report['cases'].append(dict(name='actual-full32-seven-source', status='PASS',
            records=32, service_roots=7, outputs=proof['outputs']))
        variants = {}
        missing_helpers = json.loads(json.dumps(original))
        del missing_helpers['source_inputs']['native_language_helpers']
        variants['missing-current-helper-closure'] = missing_helpers
        changed_helper = json.loads(json.dumps(original))
        changed_helper['source_inputs']['native_language_helpers'][0]['sha256'] = '0' * 64
        variants['changed-actual-helper-pin'] = changed_helper
        missing_core = json.loads(json.dumps(original))
        del missing_core['outputs']['control_wire.hpp']
        variants['missing-complete-core-product'] = missing_core
        escaping = json.loads(json.dumps(original))
        escaping['outputs'] = {'native_languages/../../outside': dict(bytes=0, sha256='0' * 64), **escaping['outputs']}
        variants['escaping-additional-product'] = escaping
        for name, candidate in variants.items():
            folder = args.out / name
            folder.mkdir()
            (folder / 'generation.json').write_text(json.dumps(candidate) + '\n')
            output = args.out / (name + '-output')
            try:
                module.generate(source, roles['catalog'], folder, output, expected)
            except ValueError as error:
                if output.exists():
                    raise ValueError('refusal must precede source output')
                report['cases'].append(dict(name=name, status='PASS_REFUSED_BEFORE_OUTPUT', diagnostic=str(error)))
            else:
                raise ValueError('altered canonical proof unexpectedly accepted: ' + name)
        if operator.read_bytes() != raw or any(digest(path) != expected[role] for role, path in roles.items()):
            raise ValueError('actual selected source changed')
        report['status'] = 'FIVE_SOURCE_CASES_PASS_NO_COMPILER_GUEST_OR_NATIVE_CREDIT'
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
