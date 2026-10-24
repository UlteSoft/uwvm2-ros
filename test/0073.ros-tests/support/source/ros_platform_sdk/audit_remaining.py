#!/usr/bin/env python3
"""Audit transported current SDK source under the original bounded Linux owner.

Both ordinary source coverage and --require-all must pass once all inventoried
vendors have a selected six-language family. This is a file/hash property only:
it grants no native driver, official-suite, release or controller acceptance.
The six newly executed SDK families must also match the actual execution root.
"""
import argparse
import hashlib
import json
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
    source = Path(inputs['audit_source_root'])
    executed = Path(inputs['source_root'])
    manifest = json.loads((source / 'source-io.json').read_bytes())
    prefixes = tuple('sdk/origin/custom-types/' + vendor + '-' for vendor in
                     ('deep-robotics', 'pal', 'trossen', 'agibot', 'ubtech', 'fourier'))

    def stable():
        for row in manifest:
            relative = Path(row['path'])
            assert not relative.is_absolute() and '..' not in relative.parts
            paths = [source / relative]
            if row['path'].startswith(prefixes):
                paths.append(executed / relative)
            for path in paths:
                assert path.is_file() and not path.is_symlink()
                assert path.stat().st_size == row['bytes']
                with path.open('rb') as stream:
                    assert hashlib.file_digest(stream, 'sha256').hexdigest() == row['sha256']

    assert out.is_absolute() and out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    stable()
    result, commands = None, []
    for label, flags in [('source-coverage', []), ('selected-source-ci', ['--require-all'])]:
        argv = [sys.executable, '-I', '-S', '-B',
                str(source / 'sdk/origin/tools/audit_ros_platform_sdks.py'), '--source', str(source), *flags]
        with (out / (label + '.json')).open('xb') as stdout, (out / (label + '.stderr')).open('xb') as stderr:
            process = subprocess.run(argv, stdout=stdout, stderr=stderr, check=False, timeout=120)
        assert process.returncode == 0
        actual = json.loads((out / (label + '.json')).read_bytes())
        if result is not None:
            assert actual == result
        result = actual
        commands.append(dict(argv=argv, exit=process.returncode))
    stable()
    assert result['vendor_count'] == result['vendors_with_selected_generated_source'] == 28
    assert len(result['generated_kits']) == 112
    assert result['all_vendors_have_selected_generated_source']
    assert all(row['status'] == 'GENERATED_SOURCE_HASHES_MATCH'
               for row in result['generated_kits'].values())
    report = dict(status='CURRENT_SELECTED_SOURCE_COVERAGE_AND_REQUIRE_ALL_PASS',
                  commands=commands, checked_files=len(manifest), generated_families=112,
                  vendors_with_selected_source=28, total_vendors=28,
                  actual_compiler_guest_native_runs=0, original_official_cases_run=0,
                  complete_vendor_support_claimed=False, default_authority=[])
    (out / 'audit.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], flush=True)


if __name__ == '__main__':
    main()
