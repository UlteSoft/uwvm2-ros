#!/usr/bin/env python3
"""Check exact source/profile selections and executable CMake configuration.

Run only under the independent hard64GiB Linux owner. Matching source is not
native metadata. Expected wrong-domain refusals never count as robot delivery.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    kit = source / 'sdk/origin/robots/humanoids/leju'
    reused = Path(inputs['finish_reuse_builds']).resolve(strict=True) if inputs.get('finish_reuse_builds') else None
    if reused:
        assert reused.parent == root/'out' and reused.name.startswith('finish-')
        attempt=reused.name.removeprefix('finish-')
        prior=json.loads((root/'out'/('leju-finish-'+attempt+'.json')).read_bytes())
        assert prior['status'] in ('PASS','FAILED_RETAINED')
        assert all(c['exit'] is not None for c in prior['commands'])
        # Reuse only closed build-cache DATA with unchanged SDK/compiler
        # inputs. Reconfigure/build and execute every contract/refusal again;
        # never reuse the earlier attempt's test status.
        for pin in prior['inputs']['pins']:
            path=Path(pin['path'])
            if path.is_relative_to(kit/'include') or path.is_relative_to(kit/'examples') or path.is_relative_to(kit/'tests') or path.is_relative_to(source/'sdk/origin/include') or path.is_relative_to(source/'sdk/origin/custom-types') or path.is_relative_to(Path(inputs['toolchains'])):
                if path.suffix=='.md':continue
                assert path.stat().st_size==pin['bytes']
                with path.open('rb')as stream:assert hashlib.file_digest(stream,'sha256').hexdigest()==pin['sha256']
    assert out.is_absolute() and out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    report = dict(status='RUNNING', commands=[], default_authority=[],
                  closed_build_cache_reused=str(reused) if reused else None,
                  earlier_test_status_reused=False)
    def save():
        (out / 'finish.json').write_text(json.dumps(report, indent=2) + '\n')
    def run(label, argv, expected=0):
        directory = out / label
        directory.mkdir()
        with (directory / 'stdout').open('xb') as stdout, (directory / 'stderr').open('xb') as stderr:
            p = subprocess.run(list(map(str, argv)), cwd=source, stdout=stdout, stderr=stderr, timeout=180, check=False)
        report['commands'].append(dict(label=label, argv=list(map(str, argv)), exit=p.returncode,
            expected_exit=expected, stdout_sha256=hashlib.sha256((directory / 'stdout').read_bytes()).hexdigest(),
            stderr_sha256=hashlib.sha256((directory / 'stderr').read_bytes()).hexdigest()))
        save()
        assert p.returncode == expected, (label, p.returncode, (directory / 'stderr').read_text()[-2000:])
        return (directory / 'stdout').read_bytes() + (directory / 'stderr').read_bytes()
    try:
        run('complete-source-check', [sys.executable, '-I', '-S', '-B', kit / 'tests/validate_sources.py', '--source', source])
        common = ['cmake', '-S', kit, '-DCMAKE_BUILD_TYPE=MinSizeRel', '-DCMAKE_C_COMPILER=/usr/bin/clang-21', '-DCMAKE_CXX_COMPILER=/usr/bin/clang++-21']
        profiles = tuple(r['id'] for r in json.loads((kit/'profiles.json').read_bytes())['profiles'])
        for profile in profiles:
            family = source / 'sdk/origin/custom-types' / ('leju-' + profile.replace('_', '-') + '-complete')
            # Assembly already associates fresh Leju families. Checking that
            # exact immutable receipt is independent from build-cache reuse.
            selection_flags=['--check-existing'] if (family/'profile-selection.json').is_file() else []
            run(profile + '-select', [sys.executable, '-I', '-S', '-B', kit / 'tools/select_profile.py',
                '--kit', kit, '--bundle', family, '--profile', profile,*selection_flags])
        for profile in profiles:
            family = source / 'sdk/origin/custom-types' / ('leju-' + profile.replace('_', '-') + '-complete')
            build = (reused if reused else out) / ('build-' + profile)
            run(profile + '-cmake', [*common, '-B', build, '-DUWVM_LEJU_BUILD_CONTRACTS=ON',
                '-DUWVM_LEJU_PROFILE=' + profile, '-DUWVM_LEJU_SOURCE_ROOT=' + str(source),
                '-DUWVM_LEJU_GENERATED=' + str(family)])
            run(profile + '-build', ['cmake', '--build', build, '--parallel', '1'])
            for name in ('leju_contract_c', 'leju_contract_cpp'):
                run(profile + '-' + name, [build / name])
            wasi = Path(inputs['toolchains']) / 'wasi-sdk-33.0-x86_64-linux'
            build_wasi = (reused if reused else out) / ('wasi-' + profile)
            run(profile + '-wasi-cmake', ['cmake', '-S', kit, '-B', build_wasi,
                '-DCMAKE_BUILD_TYPE=MinSizeRel',
                '-DCMAKE_TOOLCHAIN_FILE=' + str(wasi / 'share/cmake/wasi-sdk.cmake'),
                '-DCMAKE_EXE_LINKER_FLAGS=-Wl,--threads=1',
                '-DUWVM_LEJU_BUILD_CONTRACTS=ON', '-DUWVM_LEJU_BUILD_WASI_EXAMPLES=ON',
                '-DUWVM_LEJU_PROFILE=' + profile, '-DUWVM_LEJU_SOURCE_ROOT=' + str(source),
                '-DUWVM_LEJU_GENERATED=' + str(family)])
            run(profile + '-wasi-build', ['cmake', '--build', build_wasi, '--parallel', '1'])
            for name in ('leju_contract_c', 'leju_contract_cpp', 'leju_guest_c', 'leju_guest_cpp'):
                artifact = build_wasi / (name + '.wasm')
                run(profile + '-' + name + '-validate', [inputs['validator'], 'validate', artifact])
                if name.startswith('leju_contract_'):
                    run(profile + '-' + name + '-wasm', [inputs['runtime'], '-Rint',
                        '--wasip1-global-noinherit-system-environment', '--run', artifact])
                else:
                    run(profile + '-' + name + '-imports', [sys.executable, '-I', '-S', '-B',
                        source / 'sdk/origin/tools/check_wasm_imports.py', artifact,
                        '--backend', '1' if profile.startswith('ros1_') else '2',
                        '--allow-module', 'wasi_snapshot_preview1', '--report', out / (profile + '-' + name + '-imports.json')])
                    for mode in ('raw', 'state', 'command'):
                        text = run(profile + '-' + name + '-denied-' + mode,
                            [inputs['runtime'], '-Rint', '--wasip1-global-noinherit-system-environment',
                             '--run', artifact, mode, 'leju.observation'], expected=13)
                        assert text == b''
            for foreign in (profiles[(profiles.index(profile)+1)%len(profiles)],):
                if foreign == profile:
                    continue
                actual = source / 'sdk/origin/custom-types' / ('leju-' + foreign.replace('_', '-') + '-complete')
                text = run(profile + '-refuse-' + foreign, [*common, '-B', out / ('wrong-' + profile + '-' + foreign),
                    '-DUWVM_LEJU_BUILD_CONTRACTS=ON', '-DUWVM_LEJU_PROFILE=' + profile,
                    '-DUWVM_LEJU_SOURCE_ROOT=' + str(source), '-DUWVM_LEJU_GENERATED=' + str(actual)], expected=1)
                assert b'Exact original LEJU source profile differs from bundle' in text
        run('default-off-configure', [*common, '-B', out / 'default-off'])
        text = run('default-off-targets', ['cmake', '--build', out / 'default-off', '--target', 'help'])
        assert b'leju_contract_' not in text and b'leju_guest_' not in text
        text = run('missing-profile-refusal', [*common, '-B', out / 'missing-profile', '-DUWVM_LEJU_BUILD_CONTRACTS=ON'], expected=1)
        assert b'One exact original LEJU ROS/protocol source domain required' in text
        run('reproduce-examples', [sys.executable, '-I', '-S', '-B', kit / 'tools/emit_examples.py', '--source', source, '--out', out / 'example-sources'])
        count = 0
        for p in (out / 'example-sources').rglob('*'):
            if p.is_file():
                assert p.read_bytes() == (kit / p.relative_to(out / 'example-sources')).read_bytes(), p
                count += 1
        report.update(status='LEJU_EXACT_SOURCE_PROFILE_CMAKE_AND_REPRODUCTION_PASS',
                      matched_example_sources=count, accepted_profiles=len(profiles), wrong_domain_refusals=len(profiles),
                      default_off_confirmed=True)
        print(report['status'], flush=True)
    except BaseException as error:
        report.update(status='FAILED_RETAINED', error=repr(error))
        raise
    finally:
        save()


if __name__ == '__main__':
    main()
