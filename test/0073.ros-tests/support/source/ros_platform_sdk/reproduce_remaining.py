#!/usr/bin/env python3
"""Verify the shared authors preserve the previously delivered default Leju SDK.

Current client file facts and independently transported family bytes supply
the reference. Regeneration runs only on Linux under the bounded owner. No
archived test status or generated source hash is accepted as an execution pass.
This checks source reproduction, not a compiler, Guest or original driver.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


def sha(path):
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, required=True)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    root, out = args.root.resolve(strict=True), args.out
    inputs = json.loads((root / 'inputs.json').read_bytes())
    source = Path(inputs['source_root'])
    mirror = Path(inputs['audit_source_root'])
    facts_path = root / 'control/current-leju-author-output-facts.json'
    facts = json.loads(facts_path.read_bytes())['files']
    expected = {row['path']: row for row in facts}
    assert len(expected) == len(facts) <= 512
    assert out.parent == root / 'out' and not out.exists()
    out.mkdir(mode=0o700)
    stage = out / 'input-source'
    stage.mkdir()
    restored = []

    def link(actual, relative):
        destination = stage / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        assert actual.is_file() and not actual.is_symlink()
        os.link(actual, destination)
        restored.append(dict(path=relative, bytes=actual.stat().st_size, sha256=sha(actual)))

    kit = 'sdk/origin/robots/humanoids/leju'
    link(source / kit / 'profiles.json', kit + '/profiles.json')
    for relative in ['sdk/origin/robots/arms/ufactory/xarm/examples/guest_ros2_humble_xarm6.mjs',
                     'sdk/origin/robots/arms/ufactory/xarm/examples/wasi_alias.ts',
                     'sdk/origin/robots/arms/dobot/examples/javascript-ros2_v4.json']:
        link(source / relative, relative)
    manifest = {row['path']: row for row in json.loads((mirror / 'source-io.json').read_bytes())}
    for profile in json.loads((source / kit / 'profiles.json').read_bytes())['profiles']:
        family = 'sdk/origin/custom-types/leju-' + profile['id'].replace('_', '-') + '-complete'
        generation = json.loads((mirror / family / 'generation.vendor.json').read_bytes())
        for name in ['generation.vendor.json', *generation['outputs']]:
            relative = family + '/' + name
            actual, row = mirror / relative, manifest[relative]
            assert actual.stat().st_size == row['bytes'] and sha(actual) == row['sha256']
            link(actual, relative)
    commands, matched = [], []
    for tool, directory, prefix in [('emit_examples', 'example-sources', ''),
                                    ('emit_contracts', 'contract-sources', 'tests/')]:
        argv = [sys.executable, '-I', '-S', '-B',
                str(source / kit / 'tools' / (tool + '.py')), '--source', str(stage), '--out', str(out / directory)]
        with (out / (tool + '.stdout')).open('xb') as stdout, (out / (tool + '.stderr')).open('xb') as stderr:
            result = subprocess.run(argv, stdout=stdout, stderr=stderr, check=False, timeout=180)
        commands.append(dict(argv=argv, exit=result.returncode))
        assert result.returncode == 0, (out / (tool + '.stderr')).read_text()[-1800:]
        for path in sorted((out / directory).rglob('*')):
            if not path.is_file():
                continue
            relative = prefix + str(path.relative_to(out / directory))
            row = expected[relative]
            assert path.stat().st_size == row['bytes'] and sha(path) == row['sha256'], relative
            matched.append(relative)
    assert len(matched) == len(set(matched))
    report = dict(status='SHARED_AUTHORS_DEFAULT_LEJU_CURRENT_SOURCE_BYTE_REPRODUCTION_PASS',
                  commands=commands, matched_sources=matched, matched_count=len(matched),
                  expected_facts_sha256=sha(facts_path), current_family_inputs=restored,
                  default_authority=[], compiler_guest_native_or_official_cases_run=0)
    (out / 'reproduction.json').write_text(json.dumps(report, indent=2) + '\n')
    print(report['status'], len(matched), flush=True)


if __name__ == '__main__':
    main()
