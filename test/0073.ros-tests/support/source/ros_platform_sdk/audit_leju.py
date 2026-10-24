#!/usr/bin/env python3
"""Audit current shipped six-language families on Linux, with no ROS credit.

The mirror must first match independently captured current client file facts.
This source-coverage check is separate from executable SDK, native metadata,
driver, simulation, architecture-specific Runtime or performance acceptance.
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
    source = Path(json.loads((root / 'inputs.json').read_bytes())['source_root'])
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    facts = json.loads((root / 'control/current-sdk-source-file-facts.json').read_bytes())['files']
    def stable():
        for row in facts:
            path = source / row['path']
            assert path.is_file() and not path.is_symlink() and path.stat().st_size == row['bytes']
            with path.open('rb') as stream:
                assert hashlib.file_digest(stream, 'sha256').hexdigest() == row['sha256']
    stable()
    results = []
    for label, flags, expected in [('source', [], 0), ('require-all', ['--require-all'], 1)]:
        with (out / (label + '.json')).open('xb') as stdout, (out / (label + '.stderr')).open('xb') as stderr:
            run = subprocess.run([sys.executable, '-I', '-S', '-B',
                str(source / 'sdk/origin/tools/audit_ros_platform_sdks.py'), '--source', str(source), *flags],
                cwd=source, stdout=stdout, stderr=stderr, timeout=120, check=False)
        assert run.returncode == expected
        report = json.loads((out / (label + '.json')).read_bytes())
        assert report['vendors_with_selected_generated_source'] == 22
        assert len(report['generated_kits']) == 86
        assert all(r['status'] == 'GENERATED_SOURCE_HASHES_MATCH' for r in report['generated_kits'].values())
        assert report['default_authority'] == [] and not report['all_vendors_have_selected_generated_source']
        results.append(dict(label=label, exit=run.returncode))
    stable()
    (out / 'audit.json').write_text(json.dumps(dict(
        status='CURRENT_86_FAMILY_SOURCE_AUDIT_22_OF_28_VENDORS_NO_NATIVE_OR_EXECUTION_CREDIT',
        commands=results, default_authority=[], original_official_cases_run=0), indent=2) + '\n')


if __name__ == '__main__':
    main()
